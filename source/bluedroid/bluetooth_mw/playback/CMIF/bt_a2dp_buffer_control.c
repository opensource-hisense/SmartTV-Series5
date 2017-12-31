#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <assert.h>
#include <fcntl.h>
#include <sys/prctl.h>
#include <sys/time.h>
#include <dlfcn.h>
#include <math.h>


#include "bt_mw_common.h"   // BT_DEBUG_ function
#include "bt_mw_a2dp_common.h"   // BT_DEBUG_ function
#include "bluetooth_sync.h"
#include "bt_a2dp_buffer_control.h"

#if ENABLE_BUFFER_CONTROL

#define LDAC_PACKET_LENGTH_AFTER_COMPENSATED (660*2+1)
#define LDAC_ORIGINAL_PACKET_LENGTH (1024)
#define SBC_AAC_TEMP_DATA_LENGTH (4096+10)

bt_frame_sample_info_t bt_frame_info;
bt_detection_info_t bt_detection_info;
bt_compensation_info_t bt_compensation_info;
INT16 *SBC_AAC_data_comp = NULL;
BOOL buffer_control_flag = FALSE;
BOOL bt_detect_init = FALSE;
BOOL bt_compentate_init = FALSE;
BOOL buffer_control_reset_flag = FALSE;
UINT32 buf_count = 0;
UINT32 bt_buf_ctl_time = 0;
BOOL bt_get_start_time = FALSE;
DOUBLE bt_buf_receive_sample = 0;
INT32 minute_index=0;
DOUBLE sumTxA = 0;
DOUBLE sumNxA = 0;

#if ENABLE_LDAC_CODEC
UINT8 *LDAC_packet_buffer = NULL;
UINT8 *LDAC_data_temp = NULL;
UINT16 PCMnumber_per_frame = 0;
INT32 number_of_frames = 0;
UINT32 Fcomp = 0;//  act as a counter,  the number of repeated/removed frames in one minute

UINT8 frame_status[6] = {0};
UINT8 bitmap[6] = {0};
UINT8 search_result[6] = {0};
UINT32 search_word = 0;
UINT8 last_frame_select = 0;
static UINT32 j = 0;

extern INT32 ldac_switch;
extern INT32 repeat_LDAC_frame_number;
extern BOOL repeat_LDAC_frame_flag;
extern DOUBLE last_drift;
#endif	/* ENABLE_LDAC_CODEC */

extern DOUBLE i4_Ncomp;
extern BOOL Ncomp_flag;
extern DOUBLE Drift;
extern BOOL Cli_Drift;
extern UINT8 bt_buf_ctl_G1_1;
extern UINT8 bt_buf_ctl_G1_2;
extern UINT8 bt_buf_ctl_G2_1;
extern UINT8 bt_buf_ctl_G2_2;
extern BOOL bt_buf_enable_detection;
extern BOOL SBC_AAC_enable_compensation;


/* FUNCTION NAME: i4TimerHandler
 * PURPOSE:
 *      { the routine of timer thread }
 * INPUT:
 *      parameter-1  -- no used
 * OUTPUT:
 *      parameter-n  -- no used
 * RETURN:
 *      return-value-1 --
 * NOTES:
 *
 */
VOID *i4TimerHandler(VOID *args)
{
    FUNC_ENTRY;
    buf_count = 0;
    while (buffer_control_flag)
    {
        /*x_thread_delay(10);*/
        usleep(10*1000);
        buf_count++;
        /*1 minter is over*/
        if (0 == buf_count%1000)
        {
            BT_DBG_NORMAL(BT_DEBUG_A2DP,"<buffer control>timer,buf_count:%ld",buf_count);
        }

        if (buf_count == TIME_INTERVAL/10)
        {
            buf_count = 0;
            BT_DBG_NORMAL(BT_DEBUG_A2DP,"One minute is over");
            bt_buf_ctl_time++;
            if (MAX_TIME_COUNT == bt_buf_ctl_time)
            {
                BT_DBG_NORMAL(BT_DEBUG_A2DP,"<buffer control> bt_buf_ctl_time:%ld minutes (%f hours).buffer control will be reset.",bt_buf_ctl_time,bt_buf_ctl_time/(float)60);
                buffer_control_reset_flag = TRUE;
            }
        }
    }
    /*exit buffer control*/
    buffer_control_flag = FALSE;
    BT_DBG_NORMAL(BT_DEBUG_A2DP,"Buffer control timer pthread_exit!");
    pthread_exit(NULL);
    FUNC_EXIT;
    return NULL;
}

/* FUNCTION NAME: Buffer_control_process
 * PURPOSE:
 *      { 1 . start buffer control
	   2 . when the condition is fulfilled ,reset buffer control
 }
 * INPUT:
 *      parameter-1  --
 * OUTPUT:
 *      parameter-n  --
 * RETURN:
 *      return-value-1 --
 * NOTES:
 *
 */
VOID Buffer_control_process(VOID)
{
    if (buffer_control_reset_flag)
    {
        buf_count = 0;
        bt_buf_ctl_time = 0;
        btaudio_buffer_control_reset();
        buffer_control_reset_flag = FALSE;
    }

    if (bt_detect_init) // detection has init!
    {
        if (!bt_get_start_time)
        {
            gettimeofday(&bt_frame_info.start_time,NULL);
            bt_get_start_time = TRUE;
            BT_DBG_NORMAL(BT_DEBUG_A2DP,"<buffer control> start! ");
        }
    }
    else // detection has not init!
    {
        //if (u4count % 50 == 0)
        //    BT_DBG_NORMAL(BT_DEBUG_A2DP,"<buffer control>clock drift detection is not init!");
        //return;
    }
}

/* FUNCTION NAME: Buffer_control_timer_pthread
 * PURPOSE:
 *      { create a timer thread for clock drift detection }
 * INPUT:
 *      parameter-1  --
 * OUTPUT:
 *      parameter-n  --
 * RETURN:
 *      return-value-1 --  not used
 * NOTES:
 *
 */
INT32 Buffer_control_timer_pthread(VOID)
{
    FUNC_ENTRY;
    static pthread_t  h_ps_thread;
    pthread_attr_t attr;
    INT32 i4_ret = 0;

    if (get_bt_is_call_power_off())
    {
        BT_DBG_NORMAL(BT_DEBUG_A2DP,"have call power off");
        return BT_ERR_STATUS_NOT_READY;
    }
    i4_ret = pthread_attr_init(&attr);
    if (0 != i4_ret)
    {
        BT_DBG_ERROR(BT_DEBUG_A2DP,"pthread_attr_init i4_ret:%ld", i4_ret);
        return BT_ERR_STATUS_FAIL;
    }
    i4_ret = pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
    if (0 == i4_ret)
    {
        if (0 != (i4_ret = pthread_create(&h_ps_thread,
                                          &attr,
                                          i4TimerHandler,
                                          NULL)))
        {
            BT_DBG_ERROR(BT_DEBUG_A2DP,"pthread_create i4_ret:%ld", i4_ret);
            assert(0);
        }
    }
    else
    {
        BT_DBG_ERROR(BT_DEBUG_A2DP,"pthread_attr_setdetachstate i4_ret:%ld", i4_ret);
    }
    pthread_attr_destroy(&attr);
    BT_DBG_NORMAL(BT_DEBUG_A2DP,"create timer ok!");
    return BT_SUCCESS;
}

/* FUNCTION NAME: btaudio_detect_init
 * PURPOSE:
 *      {  initialize the clock drift detection
 	 }
 * INPUT:
 *      parameter-1  --
 * OUTPUT:
 *      parameter-n  --
 * RETURN:
 *      return-value-1 -- not used
 * NOTES:
 *
 */
INT32 btaudio_detect_init(VOID)
{
    FUNC_ENTRY;
    /*Initializing Clock drift detection*/
    memset(&bt_detection_info,0,sizeof(bt_detection_info_t));
    memset(&bt_frame_info,0,sizeof(bt_frame_sample_info_t));
    bt_detect_init = TRUE;
    minute_index=0;
    sumTxA = 0;
    sumNxA = 0;
    return BT_SUCCESS;
}

/* FUNCTION NAME: btaudio_detect_uninit
 * PURPOSE:
 *      { uninitialize the clock drift detection }
 * INPUT:
 *      parameter-1  --
 * OUTPUT:
 *      parameter-n  --
 * RETURN:
 *      return-value-1 -- not used
 * NOTES:
 *
 */
INT32 btaudio_detect_uninit(VOID)
{
    FUNC_ENTRY;
    /*Initializing Clock drift detection*/
    memset(&bt_detection_info,0,sizeof(bt_detection_info_t));
    memset(&bt_frame_info,0,sizeof(bt_frame_sample_info_t));
    bt_detect_init = FALSE;
    bt_buf_receive_sample = 0;
    bt_buf_ctl_time = 0;
    return BT_SUCCESS;
}

/* FUNCTION NAME: btaudio_compentate_init
 * PURPOSE:
 *      { initialize clock drift compensation }
 * INPUT:
 *      parameter-1  --
 * OUTPUT:
 *      parameter-n  --
 * RETURN:
 *      return-value-1 -- not used
 * NOTES:
 *
 */
INT32 btaudio_compentate_init(VOID)
{
    FUNC_ENTRY;

    //total_repeated_frames_number = 0;
    /*Initializing Clock drift compensation*/
    memset(&bt_compensation_info,0,sizeof(bt_compensation_info_t));

    bt_compentate_init = TRUE;
    return BT_SUCCESS;
}

/* FUNCTION NAME: btaudio_compentate_uninit
 * PURPOSE:
 *      { uninitialize clock drift compensation }
 * INPUT:
 *      parameter-1  --
 * OUTPUT:
 *      parameter-n  --
 * RETURN:
 *      return-value-1 -- not used
 * NOTES:
 *
 */
INT32 btaudio_compentate_uninit(VOID)
{
    FUNC_ENTRY;

    //total_repeated_frames_number = 0;
    /*Initializing Clock drift compensation*/
    memset(&bt_compensation_info,0,sizeof(bt_compensation_info_t));

    bt_compentate_init = FALSE;
    return BT_SUCCESS;
}

/* FUNCTION NAME: initMemoryForBufferControl
 * PURPOSE:
 *      { allocate memory for buffer control}
 * INPUT:
 *      parameter-1  --
 * OUTPUT:
 *      parameter-n  --
 * RETURN:
 *      return-value-1 --
 * NOTES:	when A2DP connected, this function will be called
 *
 */
VOID initMemoryForBufferControl(VOID)
{
    FUNC_ENTRY;

    SBC_AAC_data_comp = (INT16 *)malloc(SBC_AAC_TEMP_DATA_LENGTH);	// for SBC/AAC  temporary store the received audio data
    if (NULL == SBC_AAC_data_comp)
    {
        BT_DBG_ERROR(BT_DEBUG_A2DP,"malloc memory fail");
        return;
    }
    memset(SBC_AAC_data_comp,0,SBC_AAC_TEMP_DATA_LENGTH);
#if ENABLE_LDAC_CODEC
    LDAC_packet_buffer = (UINT8 *)malloc(LDAC_PACKET_LENGTH_AFTER_COMPENSATED); // for LDAC , stroe a packet, even if all the frame of a packet is repeated
    if (NULL == LDAC_packet_buffer)
    {
        BT_DBG_ERROR(BT_DEBUG_A2DP,"LDAC_packet_buffer:malloc memory fail.");
        return;
    }
    memset(LDAC_packet_buffer,0,LDAC_PACKET_LENGTH_AFTER_COMPENSATED);

    LDAC_data_temp = (UINT8 *)malloc(LDAC_ORIGINAL_PACKET_LENGTH); // for LDAC,  temporary store a LDAC packet
    if (NULL == LDAC_data_temp)
    {
        BT_DBG_ERROR(BT_DEBUG_A2DP,"data_temp:malloc memory fail.");
        return;
    }
    memset(LDAC_data_temp,0,LDAC_ORIGINAL_PACKET_LENGTH);
#endif	/* ENABLE_LDAC_CODEC */
    BT_DBG_NORMAL(BT_DEBUG_A2DP,"<buffer control>get memory successfully!");
}

/* FUNCTION NAME: uninitMemoryForBufferControl
 * PURPOSE:
 *      {free the memory that is allocated}
 * INPUT:
 *      parameter-1  --
 * OUTPUT:
 *      parameter-n  --
 * RETURN:
 *      return-value-1 --
 * NOTES:	when A2DP disconnected, this function will be called
 *
 */
VOID uninitMemoryForBufferControl(VOID)
{
    FUNC_ENTRY;
    free(SBC_AAC_data_comp);
    SBC_AAC_data_comp = NULL;
#if ENABLE_LDAC_CODEC
    free(LDAC_packet_buffer);
    LDAC_packet_buffer = NULL;
    free(LDAC_data_temp);
    LDAC_data_temp = NULL;
#endif	/* ENABLE_LDAC_CODEC */
    BT_DBG_NORMAL(BT_DEBUG_A2DP,"<buffer control>free memory successfully!");
}

/* FUNCTION NAME: btaudio_buffer_control_init
 * PURPOSE:
 *      { initialize buffer control }
 * INPUT:
 *      parameter-1  --
 * OUTPUT:
 *      parameter-n  --
 * RETURN:
 *      return-value-1 -- not used
 * NOTES:
 *
 */
INT32 btaudio_buffer_control_init(VOID)
{
    FUNC_ENTRY;

    /*Creating timer pthread*/
    if (FALSE == buffer_control_flag)
    {
        buffer_control_flag = TRUE;
        bt_buf_ctl_time = 0;
        Buffer_control_timer_pthread();
    }

    /*Initializing Clock drift detection*/
    if (FALSE == bt_detect_init)
    {
        btaudio_detect_init();
        bt_get_start_time = FALSE;
    }

    /*Initializing Clock drift compensation*/
    if (FALSE == bt_compentate_init)
    {
        btaudio_compentate_init();
    }
    BT_DBG_NORMAL(BT_DEBUG_A2DP,"Currently,bt_buf_enable_detection is %s",bt_buf_enable_detection?"TRUE":"FALSE");
#if ENABLE_LDAC_CODEC
    number_of_frames = 0;
	Fcomp = 0;
#endif	/* ENABLE_LDAC_CODEC */
    minute_index = 0;

    return BT_SUCCESS;
}

/* FUNCTION NAME: btaudio_buffer_control_uninit
 * PURPOSE:
 *      { uninitialize buffer control }
 * INPUT:
 *      parameter-1  --
 * OUTPUT:
 *      parameter-n  --
 * RETURN:
 *      return-value-1 -- not used
 * NOTES:
 *
 */
INT32 btaudio_buffer_control_uninit(VOID)
{
    FUNC_ENTRY;

    /*Uninitializing Clock drift detection*/
    if (TRUE == bt_detect_init)
    {
        btaudio_detect_uninit();
    }

    /*Uninitializing Clock drift compensation*/
    if (TRUE == bt_compentate_init)
    {
        btaudio_compentate_uninit();
    }

    /*Exit timer pthread*/
    buffer_control_flag = FALSE;
    BT_DBG_NORMAL(BT_DEBUG_A2DP,"Currently,bt_buf_enable_detection is %s",bt_buf_enable_detection?"TRUE":"FALSE");
    //memset(&current_packet_tv, 0, sizeof(struct timeval));
    //current_packet_tv = last_packet_tv; // it seems that the 2 global variables is useless

    return BT_SUCCESS;
}

/* FUNCTION NAME: btaudio_buffer_control_reset
 * PURPOSE:
 *      {reset buffer control}
 * INPUT:
 *      parameter-1  --
 * OUTPUT:
 *      parameter-n  --
 * RETURN:
 *      return-value-1 --
 * NOTES:
 *
 */
VOID btaudio_buffer_control_reset(VOID)
{
    FUNC_ENTRY;

#if ENABLE_LDAC_CODEC
    PCMnumber_per_frame = 0;
    number_of_frames = 0;
    Fcomp = 0;
    memset(LDAC_packet_buffer,0,LDAC_PACKET_LENGTH_AFTER_COMPENSATED);
    memset(LDAC_data_temp,0,LDAC_ORIGINAL_PACKET_LENGTH);
#endif	/* ENABLE_LDAC_CODEC */
    bt_get_start_time = FALSE;
    bt_buf_receive_sample = 0;
    minute_index=0;
    sumTxA = 0;
    sumNxA = 0;

    memset(&bt_detection_info,0,sizeof(bt_detection_info_t));
    memset(&bt_frame_info,0,sizeof(bt_frame_sample_info_t));
    memset(&bt_compensation_info,0,sizeof(bt_compensation_info_t));
    memset(SBC_AAC_data_comp,0,SBC_AAC_TEMP_DATA_LENGTH);

    FUNC_EXIT;
}

/* FUNCTION NAME: received_accumulated_data_length
 * PURPOSE:
 *      {to calculate received accumulated audio data,used for clock drift detection}
 * INPUT:
 *      parameter-1  --  codec type, currently SBC,AAC and LDAC is supported
 *      parameter-2  --  the length of the data after compensation
 * OUTPUT:
 *      parameter-n  --
 * RETURN:
 *      return-value-1 --
 * NOTES:
 *
 */
VOID received_accumulated_data_length(BT_AUDIO_CODEC_TYPE codec_type,UINT16 data_len)
{
    if ((BT_AUDIO_SBC == codec_type) ||(BT_AUDIO_AAC == codec_type))
    {
        bt_buf_receive_sample += data_len/(DOUBLE)6;/*Number of data to audio input*/
    }
#if ENABLE_LDAC_CODEC
    else if (BT_AUDIO_LDAC == codec_type)
    {
        /*Different bitrate, the received samples is different*/
        /*
        if (g_i4ldac_bitrate > 900)
        {
            BT_DBG_MINOR(BT_DEBUG_A2DP,"990Kbps");
            bt_buf_receive_sample += data_len/(DOUBLE)3;// Number of data to audio input
        }
        else if (g_i4ldac_bitrate > 600)
        {
            BT_DBG_MINOR(BT_DEBUG_A2DP,"660Kbps");
            bt_buf_receive_sample += data_len/(DOUBLE)2;// Number of data to audio input
        }
        else if (g_i4ldac_bitrate > 300)
        {
            BT_DBG_MINOR(BT_DEBUG_A2DP,"330Kbps");
            bt_buf_receive_sample += data_len;// Number of data to audio input
        }
        else
        {
            BT_DBG_ERROR(BT_DEBUG_A2DP,"Invalid bitrate");
            return;
        }
        */
        bt_buf_receive_sample += data_len;// Number of data to audio input
    }
#endif	/* ENABLE_LDAC_CODEC */
    else
    {
        BT_DBG_ERROR(BT_DEBUG_A2DP,"Invalid codec");
    }
}


/* FUNCTION NAME: btaudio_detection_process
 * PURPOSE:
 *      {}
 * INPUT:
 *      parameter-1  --  BT audio codec type, currently SBC,AAC and LDAC is supported
 * OUTPUT:
 *      parameter-n  --  not used
 * RETURN:
 *      return-value-1 --
 * NOTES:
 *
 */
INT32 btaudio_detection_process(BT_AUDIO_CODEC_TYPE codec_type)
{
    BT_DBG_MINOR(BT_DEBUG_A2DP,"Enter into--->%s", __FUNCTION__);
    UINT32 i4_samplerate = 0;
    UINT32 i4_channel = 0;
    //DOUBLE frame_interval = 0;
    //UINT32 frame_count = 0;
    struct timeval last_time = {0};
    struct timeval current_time = {0};
    struct timeval temp_tv = {0};

    gettimeofday(&bt_frame_info.current_time, NULL);

    bluetooth_a2dp_get_cur_audio_info(&i4_samplerate,&i4_channel);
    i4_samplerate /= 100;/*in order to calculate*/
    if ((BT_AUDIO_SBC == codec_type) ||(BT_AUDIO_AAC == codec_type))
    {
        i4_samplerate = (i4_samplerate *(DOUBLE)2)/(DOUBLE)3;
    }

    /*Start ------------Calculate the average sample of continuous 15 frame*/
    last_time = bt_frame_info.last_time;
    current_time = bt_frame_info.current_time;

    UINT8 fg_bt_move_flag = 0;
    for (fg_bt_move_flag = MAX_FRAME_COUNT-1; fg_bt_move_flag > 0; fg_bt_move_flag--)
    {
        bt_frame_info.Number[fg_bt_move_flag%MAX_FRAME_COUNT] = bt_frame_info.Number[(fg_bt_move_flag-1)%MAX_FRAME_COUNT];
        bt_frame_info.Time[fg_bt_move_flag%MAX_FRAME_COUNT] = bt_frame_info.Time[(fg_bt_move_flag-1)%MAX_FRAME_COUNT];
    }

    if ((0 != current_time.tv_sec) && (0 == last_time.tv_sec))//the first packet
    {
        bt_frame_info.Number[0] = bt_buf_receive_sample;
        bt_frame_info.Time[0] = current_time;
        bt_frame_info.last_time = bt_frame_info.current_time;
        bt_frame_info.frame_count = (bt_frame_info.frame_count + 1)%MAX_FRAME_COUNT;
    }
    else if ((0 != current_time.tv_sec) && (0 != last_time.tv_sec))
    {
        bt_frame_info.Time[0] = current_time;
        bt_frame_info.Number[0] = bt_buf_receive_sample;
        bt_frame_info.last_time = bt_frame_info.current_time;
        bt_frame_info.frame_count = (bt_frame_info.frame_count + 1)%MAX_FRAME_COUNT;
        //BT_DBG_MINOR(BT_DEBUG_A2DP,"bt_frame_info.Number[%ld]:%6f receive_sample:%6f",frame_count,bt_frame_info.Number[frame_count],bt_buf_receive_sample);

        /*Save the latest 16 packets*/
        if (bt_frame_info.Number[0] &&
                bt_frame_info.Number[MAX_FRAME_COUNT-1])
        {
            struct timeval T0 = {0};
            struct timeval T15 = {0};
            DOUBLE Number_0 = 0;
            DOUBLE Number_15 = 0;
            T15.tv_sec = bt_frame_info.Time[MAX_FRAME_COUNT-1].tv_sec - bt_frame_info.start_time.tv_sec;
            T15.tv_usec = bt_frame_info.Time[MAX_FRAME_COUNT-1].tv_usec - bt_frame_info.start_time.tv_usec;
            T0.tv_sec = bt_frame_info.Time[0].tv_sec - bt_frame_info.start_time.tv_sec;
            T0.tv_usec = bt_frame_info.Time[0].tv_usec - bt_frame_info.start_time.tv_usec;
            bt_detection_info.AveT = (((T0.tv_sec + T15.tv_sec) * 1000) + (T0.tv_usec + T15.tv_usec)/(DOUBLE)1000)/(DOUBLE)2;
            BT_DBG_MINOR(BT_DEBUG_A2DP,"(%ld:%ld + %ld:%ld)/2 = %6f",T0.tv_sec ,T0.tv_usec ,T15.tv_sec,T15.tv_usec, bt_detection_info.AveT);

            Number_0 = bt_frame_info.Number[0];
            Number_15 = bt_frame_info.Number[MAX_FRAME_COUNT-1];
            bt_detection_info.AveN = (Number_0 + Number_15)/(DOUBLE)2;
            BT_DBG_MINOR(BT_DEBUG_A2DP,"Number_0:%6f Number_15:%6f receive_sample:%6f",Number_0,Number_15,bt_buf_receive_sample);
            BT_DBG_MINOR(BT_DEBUG_A2DP,"AveN formula:(%6f + %6f)/2 = %6f",Number_0,Number_15,bt_detection_info.AveN);
        }
        else
        {
            BT_DBG_MINOR(BT_DEBUG_A2DP,"One frame N is null at least");
        }
        /*
        UINT8 j = 0;
        if(g_bt_debug_level >= BT_DEBUG_LEVEL_MINOR)
        {
        	BT_DBG_NORMAL(BT_DEBUG_A2DP,"Number[j](j = 0:15)of bt_frame_info is :");
        for (j = 0;j < MAX_FRAME_COUNT; j++)
        {
        		if(j%5 == 0)
        			BT_DBG_NORMAL(BT_DEBUG_A2DP,"");
        		BT_DBG_NORMAL(BT_DEBUG_A2DP,"Number[%d]:%4f ",j,bt_frame_info.Number[j]);
            }
            BT_DBG_NORMAL(BT_DEBUG_A2DP,"");
        }
        */
    }
    /*Finish ------------Calculate the average sample of continuous 16 frame*/

    /*Monitor the detection info*/
    if ((BT_AUDIO_SBC == codec_type) ||(BT_AUDIO_AAC == codec_type) || (BT_AUDIO_LDAC== codec_type))
    {
        /*the first minue, it does not need to monitor*/
        if (0 == bt_buf_ctl_time)
        {
            //BT_DBG_NORMAL(BT_DEBUG_A2DP,"The first minute, it does not need to monitor");
            return BT_ERR_STATUS_NOT_READY;
        }
        /*one minute timer*/
        BT_DBG_MINOR(BT_DEBUG_A2DP,"bt_buf_ctl_time:%ld Minute_count:%ld",(unsigned long)bt_buf_ctl_time, (unsigned long)bt_detection_info.Minute_count);
        if (bt_buf_ctl_time != bt_detection_info.Minute_count)
        {
            DOUBLE FS_T = 0;
            gettimeofday(&temp_tv, NULL);
            //BT_DBG_NORMAL(BT_DEBUG_A2DP,"AveN is %6f",bt_detection_info.AveN);
            //BT_DBG_NORMAL(BT_DEBUG_A2DP,"AveT is %6f",bt_detection_info.AveT/(DOUBLE)1000000);
            BT_DBG_NORMAL(BT_DEBUG_A2DP,"bt_buf_ctl_time:%ld Minute_count:%ld",(unsigned long)bt_buf_ctl_time, (unsigned long)bt_detection_info.Minute_count);
            if (1 == bt_buf_ctl_time)/*The first minute, the result will be Drift base information*/
            {
                bt_detection_info.AveT_base = bt_detection_info.AveT;
                bt_detection_info.AveN_base = bt_detection_info.AveN;

                bt_detection_info.Time[bt_buf_ctl_time] = (temp_tv.tv_sec - bt_frame_info.start_time.tv_sec)*1000 +
                        (temp_tv.tv_usec - bt_frame_info.start_time.tv_usec)/(DOUBLE)1000;
                /*
                bt_detection_info.Nminute[bt_buf_ctl_time] = (bt_detection_info.AveN_base +
                (i4_samplerate * (bt_detection_info.Time[bt_buf_ctl_time] - bt_detection_info.AveT_base))/(DOUBLE)10000);
                */
                bt_detection_info.AveT_current = bt_detection_info.AveT_base;
                bt_detection_info.AveN_current = bt_detection_info.AveN_base;

                if ((BT_AUDIO_SBC == codec_type) || (BT_AUDIO_AAC == codec_type))/*For 48/44.1KHz, reverse the algorithm*/
                {
                    // for SBC/AAC, at the beginning, bt_buf_receive_sample += data_len/(DOUBLE)6
                    bt_detection_info.AveN_current = (bt_detection_info.AveN_current *(DOUBLE)3)/(DOUBLE)2;
                    bt_detection_info.AveN_base = (bt_detection_info.AveN_base *(DOUBLE)3)/(DOUBLE)2;
                    // now , bt_detection_info.AveN_base is (accumulated average length)/4, the unit is number of samples a sample has 4 bytes.(L R  sounl channels)
                }
                bt_detection_info.Nminute[bt_buf_ctl_time] = bt_detection_info.AveN_current;

                BT_DBG_NORMAL(BT_DEBUG_A2DP,"AveN[%ld] is %6f",(unsigned long)bt_buf_ctl_time,bt_detection_info.AveN_current);
                BT_DBG_NORMAL(BT_DEBUG_A2DP,"AveT[%ld] is %6f",(unsigned long)bt_buf_ctl_time,bt_detection_info.AveT_current);

                BT_DBG_NORMAL(BT_DEBUG_A2DP,"Nminute is %6f",bt_detection_info.Nminute[bt_buf_ctl_time]);
                BT_DBG_NORMAL(BT_DEBUG_A2DP,"Time is %6f",bt_detection_info.Time[bt_buf_ctl_time]/(DOUBLE)1000);

                if ((BT_AUDIO_SBC == codec_type) || (BT_AUDIO_AAC == codec_type))/*For 48/44.1KHz, reverse the algorithm*/
                {
                    // for SBC/AAC, at the beginning, "i4_samplerate /=100", "i4_samplerate = (i4_samplerate *2)/3"
                    // in the following 2 instruments,  the unit of bt_detection_info.Time[bt_buf_ctl_time] is ms
                    // FS_T = FS * T,  the unit of FS_T will be number of sample.
                    FS_T = i4_samplerate * bt_detection_info.Time[bt_buf_ctl_time] /(DOUBLE)10;
                    FS_T = FS_T * (DOUBLE)3 / (DOUBLE)2;
                }
#if ENABLE_LDAC_CODEC
                else if (BT_AUDIO_LDAC == codec_type)/*For LDAC*/
                {
                    // for LDAC, at the beginning, "i4_samplerate /=100"
                    // the unit of bt_detection_info.Time[bt_buf_ctl_time] is ms
                    // FS_T = FS * T
                    FS_T = i4_samplerate * bt_detection_info.Time[bt_buf_ctl_time] /(DOUBLE)10;
                }
#endif	/* ENABLE_LDAC_CODEC */
                BT_DBG_NORMAL(BT_DEBUG_A2DP,"Fs:%ld,T:%6f",(unsigned long)(i4_samplerate*100),bt_detection_info.Time[bt_buf_ctl_time]/(DOUBLE)1000);
                BT_DBG_NORMAL(BT_DEBUG_A2DP,"FS*T is %6f", FS_T);
                bt_detection_info.DriftN[bt_buf_ctl_time] = FS_T - bt_detection_info.Nminute[bt_buf_ctl_time];
                bt_detection_info.Minute_count = bt_buf_ctl_time;
                BT_DBG_NORMAL(BT_DEBUG_A2DP,"The DriftN[%ld] number of this minute is %6f",(unsigned long)bt_buf_ctl_time,bt_detection_info.DriftN[bt_buf_ctl_time]);
            }
            else if (1 < bt_buf_ctl_time && bt_buf_ctl_time <= 64)
            {
                bt_detection_info.AveT_current = bt_detection_info.AveT;
                bt_detection_info.AveN_current = bt_detection_info.AveN;
                /*
                bt_detection_info.Time[bt_buf_ctl_time] = (temp_tv.tv_sec - bt_frame_info.start_time.tv_sec)*1000 + (temp_tv.tv_usec -
                        bt_frame_info.start_time.tv_usec)/(DOUBLE)1000;
                */
                if ((BT_AUDIO_SBC == codec_type) || (BT_AUDIO_AAC == codec_type))/*For 48/44.1KHz, reverse the algorithm*/
                    bt_detection_info.AveN_current = (bt_detection_info.AveN_current *(DOUBLE)3)/(DOUBLE)2;

                /*update AveT/AveN*/
                bt_detection_info.AveT_current -= bt_detection_info.AveT_base;
                bt_detection_info.AveN_current -= bt_detection_info.AveN_base;

                bt_detection_info.Time[bt_buf_ctl_time]=bt_detection_info.AveT_current;
                bt_detection_info.Nminute[bt_buf_ctl_time] = bt_detection_info.AveN_current;

                BT_DBG_NORMAL(BT_DEBUG_A2DP,"AveN[1] is %6f", bt_detection_info.AveN_base);
                BT_DBG_NORMAL(BT_DEBUG_A2DP,"AveT[1] is %6f", bt_detection_info.AveT_base);
                BT_DBG_NORMAL(BT_DEBUG_A2DP,"AveN[%ld] is %6f",(unsigned long)bt_buf_ctl_time,bt_detection_info.AveN_current);
                BT_DBG_NORMAL(BT_DEBUG_A2DP,"AveT[%ld] is %6f",(unsigned long)bt_buf_ctl_time,bt_detection_info.AveT_current);

                BT_DBG_NORMAL(BT_DEBUG_A2DP,"Nminute is %6f",bt_detection_info.Nminute[bt_buf_ctl_time]);
                BT_DBG_NORMAL(BT_DEBUG_A2DP,"Time is %6f",bt_detection_info.Time[bt_buf_ctl_time]/(DOUBLE)1000);


                if ((BT_AUDIO_SBC == codec_type) || (BT_AUDIO_AAC == codec_type))/*For SBC/AAC 48/44.1KHz, reverse the algorithm*/
                {
                    FS_T = i4_samplerate * bt_detection_info.AveT_current /(DOUBLE)10;
                    FS_T = FS_T * (DOUBLE)3 /(DOUBLE)2;

                }
#if ENABLE_LDAC_CODEC
                else if (BT_AUDIO_LDAC == codec_type)/*For LDAC*/
                {
                    FS_T = i4_samplerate * bt_detection_info.AveT_current /(DOUBLE)10;
                }
#endif	/* ENABLE_LDAC_CODEC */
                bt_detection_info.DriftN[bt_buf_ctl_time] = FS_T - bt_detection_info.Nminute[bt_buf_ctl_time];
                BT_DBG_NORMAL(BT_DEBUG_A2DP,"Fs:%ldHz,T:%6fs",i4_samplerate*100,bt_detection_info.AveT_current/(DOUBLE)1000);
                BT_DBG_NORMAL(BT_DEBUG_A2DP,"FS*T is %6f", FS_T);
                bt_detection_info.Minute_count = bt_buf_ctl_time;
                BT_DBG_NORMAL(BT_DEBUG_A2DP,"The DriftN[%ld] number of this minute is %6f",(unsigned long)bt_buf_ctl_time,bt_detection_info.DriftN[bt_buf_ctl_time]);
            }
            else if (bt_buf_ctl_time >= 65) // for long time play
            {
                if (bt_buf_ctl_time%60 == 5)
                {
                    minute_index = minute_index+1;

                    DOUBLE Tx1A = 0;
                    DOUBLE Nx1A = 0;
                    DOUBLE tempT = 0;
                    DOUBLE tempN = 0;

                    // at the beginning, "i4_samplerate /=100"
                    i4_samplerate = i4_samplerate * (DOUBLE)100;
                    if ((BT_AUDIO_SBC == codec_type) || (BT_AUDIO_AAC == codec_type))/*For 48/44.1KHz, reverse the algorithm*/
                    {
                        // for SBC/AAC, at the beginning, "i4_samplerate /=100", "i4_samplerate = (i4_samplerate *2)/3"
                        i4_samplerate = i4_samplerate * (DOUBLE)3 / (DOUBLE)2;
                    }

                    if ((BT_AUDIO_SBC == codec_type) || (BT_AUDIO_AAC == codec_type))/*For 48/44.1KHz, reverse the algorithm*/
                    {
                        bt_detection_info.AveN = (bt_detection_info.AveN *(DOUBLE)3)/(DOUBLE)2;
                    }

                    Tx1A = bt_detection_info.AveT - bt_detection_info.Time[bt_buf_ctl_time-1]; // the unit is ms
                    Nx1A = bt_detection_info.AveN - i4_samplerate*bt_detection_info.Time[bt_buf_ctl_time-1]/(DOUBLE)1000;
                    tempT = (Tx1A - bt_detection_info.Time[bt_buf_ctl_time - 60])/(DOUBLE)1000;
                    tempN =  Nx1A - bt_detection_info.Nminute[bt_buf_ctl_time - 60];

                    sumTxA += Tx1A/(DOUBLE)1000;
                    sumNxA += Nx1A;

                    //bt_detection_info.DriftN[bt_buf_ctl_time] = i4_samplerate*tempT - tempN;
                    bt_detection_info.DriftN[bt_buf_ctl_time] = i4_samplerate*sumTxA - sumNxA;

                    BT_DBG_NORMAL(BT_DEBUG_A2DP,"bt_detection_info.AveT:%6f, bt_detection_info.AveN:%6f",bt_detection_info.AveT,bt_detection_info.AveN);

                    BT_DBG_NORMAL(BT_DEBUG_A2DP,"T%ldA:%6f,    N%ldA:%6f",bt_buf_ctl_time,Tx1A/(DOUBLE)1000,bt_buf_ctl_time,Nx1A);
                    BT_DBG_NORMAL(BT_DEBUG_A2DP,"tempT:%6f,    tempN:%6f",tempT,tempN);
                    BT_DBG_NORMAL(BT_DEBUG_A2DP,"sumTxA:%6f,sumNxA:%6f",sumTxA,sumNxA);
                    //BT_DBG_NORMAL(BT_DEBUG_A2DP,"DriftN[%ld] =Fs * tempT-tempN,Fs is %ld",bt_buf_ctl_time,i4_samplerate);
                    BT_DBG_NORMAL(BT_DEBUG_A2DP,"DriftN[%ld] =Fs * sumTxA-sumNxA,Fs is %ld",(unsigned long)bt_buf_ctl_time,i4_samplerate);
                    BT_DBG_NORMAL(BT_DEBUG_A2DP,"The DriftN[%ld] number of this minute is %6f",(unsigned long)bt_buf_ctl_time,bt_detection_info.DriftN[bt_buf_ctl_time]);

                    bt_detection_info.Time[bt_buf_ctl_time] = 0;
                    bt_detection_info.Nminute[bt_buf_ctl_time] = 0;

                    BT_DBG_NORMAL(BT_DEBUG_A2DP,"minute:%ld,for long time play,reset AveN_base and AveN_base to 0.",(unsigned long)bt_buf_ctl_time);
                    bt_detection_info.AveN_base = 0;
                    bt_detection_info.AveT_base = 0;

                    BT_DBG_NORMAL(BT_DEBUG_A2DP,"minute:%ld,for long time play,set current time to start_time.",(unsigned long)bt_buf_ctl_time);
                    gettimeofday(&bt_frame_info.start_time,NULL); // update start time

                    BT_DBG_NORMAL(BT_DEBUG_A2DP,"reset the length of accumulated_received_data to 0!");
                    bt_buf_receive_sample = 0; //  set accumulated_receive_data length to 0

                    bt_detection_info.Minute_count = bt_buf_ctl_time;

                }
                else
                {
                    bt_detection_info.AveT_current = bt_detection_info.AveT;
                    bt_detection_info.AveN_current = bt_detection_info.AveN;

                    bt_detection_info.Time[bt_buf_ctl_time] = (temp_tv.tv_sec - bt_frame_info.start_time.tv_sec)*1000 + (temp_tv.tv_usec -
                            bt_frame_info.start_time.tv_usec)/(DOUBLE)1000;

                    // at the beginning, "i4_samplerate /=100",
                    i4_samplerate = i4_samplerate * (DOUBLE)100;
                    if ((BT_AUDIO_SBC == codec_type) || (BT_AUDIO_AAC == codec_type))/*For 48/44.1KHz, reverse the algorithm*/
                    {
                        // for SBC/AAC, at the beginning, "i4_samplerate /=100", "i4_samplerate = (i4_samplerate *2)/3"
                        i4_samplerate = i4_samplerate * (DOUBLE)3 / (DOUBLE)2;
                    }

                    if ((BT_AUDIO_SBC == codec_type) || (BT_AUDIO_AAC == codec_type))/*For 48/44.1KHz, reverse the algorithm*/
                    {
                        bt_detection_info.AveN_current = (bt_detection_info.AveN_current *(DOUBLE)3)/(DOUBLE)2;
                    }

                    /*update AveT/AveN*/
                    bt_detection_info.AveT_current -= bt_detection_info.AveT_base;
                    bt_detection_info.AveN_current -= bt_detection_info.AveN_base;

                    bt_detection_info.Time[bt_buf_ctl_time]=bt_detection_info.AveT_current;
                    bt_detection_info.Nminute[bt_buf_ctl_time] = bt_detection_info.AveN_current;

                    BT_DBG_NORMAL(BT_DEBUG_A2DP,"AveN[%ld] is %6f", (long)(minute_index*60+5),bt_detection_info.AveN_base);
                    BT_DBG_NORMAL(BT_DEBUG_A2DP,"AveT[%ld] is %6f", (long)(minute_index*60+5),bt_detection_info.AveT_base);
                    BT_DBG_NORMAL(BT_DEBUG_A2DP,"AveN[%ld] is %6f",(unsigned long)bt_buf_ctl_time,bt_detection_info.AveN_current);
                    BT_DBG_NORMAL(BT_DEBUG_A2DP,"AveT[%ld] is %6f",(unsigned long)bt_buf_ctl_time,bt_detection_info.AveT_current);

                    BT_DBG_NORMAL(BT_DEBUG_A2DP,"Nminute is %6f",bt_detection_info.Nminute[bt_buf_ctl_time]);
                    BT_DBG_NORMAL(BT_DEBUG_A2DP,"Time is %6f",bt_detection_info.Time[bt_buf_ctl_time]/(DOUBLE)1000);

                    bt_detection_info.AveT_current = bt_detection_info.AveT_current/(DOUBLE)1000;
                    FS_T = i4_samplerate * (bt_detection_info.AveT_current + sumTxA);
                    bt_detection_info.DriftN[bt_buf_ctl_time] = FS_T - (bt_detection_info.Nminute[bt_buf_ctl_time] + sumNxA);

                    BT_DBG_NORMAL(BT_DEBUG_A2DP,"Fs:%ldHz,T:%6fs",i4_samplerate,bt_detection_info.AveT_current);
                    BT_DBG_NORMAL(BT_DEBUG_A2DP,"FS*T is %6f", FS_T);
                    bt_detection_info.Minute_count = bt_buf_ctl_time;
                    BT_DBG_NORMAL(BT_DEBUG_A2DP,"The DriftN[%ld] number of this minute is %6f",(unsigned long)bt_buf_ctl_time,bt_detection_info.DriftN[bt_buf_ctl_time]);
                }
            }

        }
    }
    else
    {
        BT_DBG_MINOR(BT_DEBUG_A2DP,"Invalid codec");
    }
    return BT_SUCCESS;
}

/* FUNCTION NAME: btaudio_compensation_process
 * PURPOSE:
 *      {to execute the clock drift compensation of buffer control}
 * INPUT:
 *      parameter-1  -- codec_type: codec type
 *      parameter-2  -- minute: the minute elapsed from buffer control start
 *      parameter-3  -- data_temp: audio data
 *      parameter-4  -- datalen_temp: length of audio data
 * OUTPUT:
 *      parameter-n  --
 * RETURN:
 *      return-value-1 --
 * NOTES:
 		for SBC/AAC , the return value is no use
 		for LDAC, the return value indicate frame number after compensated
 *
 */
UINT32 btaudio_compensation_process(BT_AUDIO_CODEC_TYPE codec_type, UINT32 minute, VOID *data_temp, VOID* datalen_temp)
{
    BT_DBG_MINOR(BT_DEBUG_A2DP,"Enter into--->%s", __FUNCTION__);
    //UINT32 i4_samplerate = 0;
    //UINT32 i4_channel = 0;
    UINT32 i4_max =0 ;
    static UINT32 packet_count = 0;
#if ENABLE_LDAC_CODEC
    static UINT32 packet_count_temp =0;
#endif
    const INT32 LOG_COUNT = 200;

    static struct timeval ten_second_tv = {0};
    static struct timeval temp_tv={0};

    //bluetooth_a2dp_get_cur_audio_info(&i4_samplerate,&i4_channel);i4_samplerate /= 100;

    packet_count ++;
    if (packet_count == 100000000)
    {
        packet_count = 0;
    }
    if (packet_count%LOG_COUNT == 0 )
    {
        BT_DBG_NORMAL(BT_DEBUG_A2DP,"packet_count:%ld",(unsigned long)packet_count);
    }
    if ((BT_AUDIO_SBC == codec_type) ||(BT_AUDIO_AAC == codec_type))
    {
        INT16 * data = (INT16 *)data_temp;
        INT32 *datalen = (INT32 *)datalen_temp;
        if ((0 == minute) || (1 == minute))
        {
            if (packet_count%LOG_COUNT == 0)
            {
                BT_DBG_NORMAL(BT_DEBUG_A2DP,"codec:%s, minute:%ld, need enable clock drift compensation from 2nd minute.",(BT_AUDIO_SBC == codec_type)?"SBC":((BT_AUDIO_AAC == codec_type)?"AAC":"unknow codec"), (unsigned long)minute);
            }
            bt_compensation_info.Cvalue[minute] = 0;
        }
        else
        {
            if (Cli_Drift == TRUE) /* set_drift (non 0) is executed  */
            {
                bt_detection_info.DriftN[minute] = Drift;
            }
            if (packet_count%LOG_COUNT == 0 )
            {
                BT_DBG_NORMAL(BT_DEBUG_A2DP,"DriftN[%ld]:%6f,DriftN[%ld]:%6f",(unsigned long)minute,bt_detection_info.DriftN[minute],(unsigned long)(minute-1),bt_detection_info.DriftN[minute-1]);
            }
            if (FALSE == Ncomp_flag) /* ncomp (non 0) is not executed, so we get i4_Ncomp by calculating */
            {
                bt_compensation_info.Cvalue[minute] = bt_detection_info.DriftN[minute] * (DOUBLE)bt_buf_ctl_G1_1/(DOUBLE)bt_buf_ctl_G1_2 +
                                                      bt_compensation_info.Cvalue[minute-1] * (DOUBLE)bt_buf_ctl_G2_1/(DOUBLE)bt_buf_ctl_G2_2;
                if (bt_compensation_info.Cvalue[minute] - (-576.0) < 0.000001)
                {
                    bt_compensation_info.Cvalue[minute] = -576.0;
                }
                else if (bt_compensation_info.Cvalue[minute] - 576.0 > 0.000001)
                {
                    bt_compensation_info.Cvalue[minute] = 576.0;
                }
                i4_Ncomp = bt_compensation_info.Cvalue[minute]/(DOUBLE)6;
                if (packet_count%LOG_COUNT == 0)
                {
                    BT_DBG_NOTICE(BT_DEBUG_A2DP,"DriftN:%6f Cvalue:%6f i4_Ncomp:%6f",bt_detection_info.DriftN[minute], bt_compensation_info.Cvalue[minute-1], i4_Ncomp);
                }
                if (((INT32)i4_Ncomp)!= 0)
                {
                    i4_max = floor(399/((INT32)i4_Ncomp))* (INT32)i4_Ncomp -1;
                    if (packet_count%LOG_COUNT == 0)
                    {
                        BT_DBG_NORMAL(BT_DEBUG_A2DP,"i4_Ncomp:%6f,i4_max:%ld",i4_Ncomp,(unsigned long)i4_max);
                    }
                }
                else if ((labs(i4_Ncomp) < 0.000001) || ((INT32)i4_Ncomp == 0))
                {
                    BT_DBG_NORMAL(BT_DEBUG_A2DP,"(warning)i4_Ncomp(%6f) should not be 0,or less than 0!",i4_Ncomp);
                    return BT_ERR_STATUS_FAIL;
                }

                /*10 second is arrived or not*/
                gettimeofday(&ten_second_tv, NULL);
                if ((0 != temp_tv.tv_sec) && (0 != temp_tv.tv_usec))
                {
                    BT_DBG_MINOR(BT_DEBUG_A2DP,"ten_second_tv.tv_sec:%ld,temp_tv.tv_sec:%ld,timeval:%ld",ten_second_tv.tv_sec,temp_tv.tv_sec,((ten_second_tv.tv_sec* 1000 + ten_second_tv.tv_usec) - (temp_tv.tv_sec* 1000 + temp_tv.tv_usec)));
                    /*10 second timer is over*/
                    if ( (ten_second_tv.tv_sec - temp_tv.tv_sec >= 9) &&
                            (((ten_second_tv.tv_sec*1000 + ten_second_tv.tv_usec/1000) - (temp_tv.tv_sec*1000 + temp_tv.tv_usec/1000)) >= 10*1000))
                    {
                        BT_DBG_MINOR(BT_DEBUG_A2DP,"packet_count:%ld, to reset 10_minute_timer to 0 because 10 second timer is over.",packet_count);
                        packet_count = 0;
                        temp_tv = ten_second_tv;
                    }
                }
                else
                {
                    BT_DBG_ERROR(BT_DEBUG_A2DP,"Temp_TV is Zero.");
                    temp_tv = ten_second_tv;
                }

                if (packet_count%LOG_COUNT == 0)
                {
                    BT_DBG_NORMAL(BT_DEBUG_A2DP,"minute:%ld, DriftN[%ld]:%6f, Cvalue:%6f, i4_Ncomp:%6f, packet_count:%ld, max_n:%ld",(unsigned long)minute,(unsigned long)minute,bt_detection_info.DriftN[minute],bt_compensation_info.Cvalue[minute-1], i4_Ncomp,(unsigned long)packet_count, (unsigned long)i4_max);
                }
                if (packet_count < i4_max)
                {
                    UINT32 X;
                    if (labs((INT32)i4_Ncomp)!= 0)
                    {
                        X = floor(399/labs((INT32)i4_Ncomp));
                    }
                    else if (labs(i4_Ncomp) < 0.000001) //   i4_Ncomp is 0
                    {
                        BT_DBG_ERROR(BT_DEBUG_A2DP,"(warning)i4_Ncomp is 0");
                        return BT_ERR_STATUS_FAIL;
                    }
                    else if ((INT32)i4_Ncomp == 0)
                    {
                        BT_DBG_ERROR(BT_DEBUG_A2DP,"(warning)(INT32)i4_Ncomp is 0,exit compensation process!");
                        return BT_ERR_STATUS_FAIL;
                    }

                    if (0 == packet_count % X)
                    {
                        //if (bt_compensation_info.Cvalue[minute] > 0)
                        if (i4_Ncomp > 0)
                        {
                            BT_DBG_NORMAL(BT_DEBUG_A2DP,"Repeat %ldth packet.", packet_count);

                            /*Repeat the first sample*/
                            memcpy(&data[2],SBC_AAC_data_comp,*datalen);
                            *datalen = *datalen + 4;
                            BT_DBG_NORMAL(BT_DEBUG_A2DP,"Final length:%ld", (long)(*datalen));
                        }
                        /*remove PCM data to be copied*/
                        else if (i4_Ncomp < 0)
                        {
                            BT_DBG_NORMAL(BT_DEBUG_A2DP,"Remove %ldth packet.", packet_count);

                            /*Remove the first sample*/
                            memcpy(data,&SBC_AAC_data_comp[2],*datalen);
                            *datalen = *datalen - 4;
                            BT_DBG_NORMAL(BT_DEBUG_A2DP,"Final length:%ld.", (long)(*datalen));
                        }
                    }
                }
                else
                {   /*Copy PCM data to audio buffer*/
                    BT_DBG_MINOR(BT_DEBUG_A2DP,"Packet count >= max_n, no need to do compensation");
                }
            }
        }
    }
#if ENABLE_LDAC_CODEC
    else if (BT_AUDIO_LDAC== codec_type)
    {
        static UINT32 last_minute = 1;
        UINT32 bitrate = 0;
        static UINT32 Add_frame = 0;
        static UINT32 Remove_frame = 0;
        UINT8 * data = (UINT8 *)data_temp;
        UINT16 *datalen = (UINT16 *)datalen_temp;
        UINT16 frames_length[15] = {0}; // the maximum number of frames in a LDAC packet is 15
        UINT8 frames_number = 0;

        if (packet_count%LOG_COUNT == 0 )
        {
            BT_DBG_MINOR(BT_DEBUG_A2DP,"bt_detection_info.DriftN[%ld]:%6f",(unsigned long)minute,bt_detection_info.DriftN[minute]);
        }

        get_LADC_packet_information(data, datalen, frames_length, 15, &frames_number);

        // bitrate = number_of_frames;
        number_of_frames = frames_number;
        bitrate = frames_number;

        if (packet_count%LOG_COUNT == 0 )
        {
            BT_DBG_NORMAL(BT_DEBUG_A2DP,"before repeat/remove,this packet has %ld frames.",(unsigned long)bitrate);
        }

        if ((0 == minute) || (1 == minute))
        {
            if (packet_count%LOG_COUNT == 0)
            {
                BT_DBG_NORMAL(BT_DEBUG_A2DP,"codec:LDAC, minute:%ld, need enable clock drift compensation from DriftN[2].",(unsigned long)minute);
            }
            return bitrate;
        }

        if (should_packet_be_repeated_removed(frames_length, bitrate) != BT_SUCCESS)
        {
            if (packet_count%LOG_COUNT == 0)
            {
                BT_DBG_NORMAL(BT_DEBUG_A2DP,"cannot repeat or remove this packet, because condition is not satisfied.");
            }
            last_frame_select = 0x0;
            return bitrate;
        }

        if (minute != last_minute)
        {
            BT_DBG_NORMAL(BT_DEBUG_A2DP,"the %ldth minute come!",minute);
            last_minute = minute;
            if (minute > 2)
            {
                BT_DBG_NORMAL(BT_DEBUG_A2DP,"in %ldth minute, receive %ld LDAC packets.",(unsigned long)(minute-1),(unsigned long)(packet_count - packet_count_temp));
            }
            packet_count_temp = packet_count;

            /*Drift update*/
            if (Cli_Drift == TRUE)/*cli command*/
            {	//  this judgement used only CLI command set_drift is executed within the  first two minutes
                last_drift = Drift;
                bt_detection_info.DriftN[minute] = Drift;
                BT_DBG_NORMAL(BT_DEBUG_A2DP,"DriftN[%ld] use the latest CLI command value %6f!",(unsigned long)minute,Drift);
            }

            BT_DBG_NORMAL(BT_DEBUG_A2DP,"in %ldth minute,total %ld frames are repeated/removed!",(unsigned long)(minute-1),(unsigned long)Fcomp);
            Fcomp = 0;  // Only for one minute is arrived, Fcomp should be accumulated, so reset this flag

            if ((INT32)bt_detection_info.DriftN[minute] > 0)
            {
                BT_DBG_MINOR(BT_DEBUG_A2DP,"Sink clock is faster");
                Add_frame = floor(bt_detection_info.DriftN[minute]/PCMnumber_per_frame);
                Remove_frame = 0;
                BT_DBG_NORMAL(BT_DEBUG_A2DP,"[Repeat][in %ldth minute] up to %ld frames can be Repeated.",(unsigned long)minute,(unsigned long)Add_frame);
            }
            else
            {
                BT_DBG_MINOR(BT_DEBUG_A2DP,"SRC clock is faster");
                Add_frame = 0;
                Remove_frame = floor(labs(bt_detection_info.DriftN[minute])/PCMnumber_per_frame);
                BT_DBG_NORMAL(BT_DEBUG_A2DP,"[Remove][in %ldth minute] up to %ld frames can be removed.",(unsigned long)minute,(unsigned long)Remove_frame);
            }
        }

        /*Drift update ,use CLI command*/
        if ((Cli_Drift !=FALSE) && (Drift != last_drift)) // within this one minute, use ""set_drift" to set DriftN[minute] as new value
        {
            last_drift = Drift;
            BT_DBG_NORMAL(BT_DEBUG_A2DP,"Within the same minute,previous compensation operation is [%s],and now [%s].",bt_detection_info.DriftN[minute]>0?"repeat":"remove",Drift>0?"repeat":"remove");
            bt_detection_info.DriftN[minute] = Drift;
            if ((INT32)bt_detection_info.DriftN[minute] > 0)
            {
                BT_DBG_MINOR(BT_DEBUG_A2DP,"Sink clock is faster");
                Add_frame = floor(bt_detection_info.DriftN[minute]/PCMnumber_per_frame);
                Remove_frame = 0;
                BT_DBG_NORMAL(BT_DEBUG_A2DP,"[Repeat][in %ldth minute] up to %ld frames can be Repeated.",(unsigned long)minute,(unsigned long)Add_frame);
            }
            else
            {
                BT_DBG_MINOR(BT_DEBUG_A2DP,"SRC clock is faster");
                Add_frame = 0;
                Remove_frame = floor(labs(bt_detection_info.DriftN[minute])/PCMnumber_per_frame);
                BT_DBG_NORMAL(BT_DEBUG_A2DP,"[Remove][in %ldth minute] up to %ld frames can be removed.",(unsigned long)minute,(unsigned long)Remove_frame);
            }
            BT_DBG_NORMAL(BT_DEBUG_A2DP,"Within the same minute,change Fcomp(%ld) to 0.",(unsigned long)Fcomp);
            Fcomp = 0;
        }

        if (Add_frame>0)
        {
            if (Fcomp < Add_frame)
            {
                btaudio_analyze_frame_status(data, *datalen);
                if (0 != search_word)
                {
                    /*repeat frame*/
                    UINT32 repeat_number = 0;
                    repeat_number = btaudio_repeat_remove(data, datalen, 1, Add_frame);
                    bitrate += repeat_number;
                    BT_DBG_NORMAL(BT_DEBUG_A2DP,"[Repeat][in %ldth minute]Repeat %ld frames of the current packet,so the current packet has %ld frames.",(unsigned long)minute,(unsigned long)repeat_number,(unsigned long)bitrate);
                }
            }
        }
        else if (Remove_frame>0)
        {
            if (Fcomp < Remove_frame)
            {
                btaudio_analyze_frame_status(data, *datalen);
                if (0 != search_word)
                {
                    /*remove frame*/
                    UINT32 remove_number = 0;
                    remove_number = btaudio_repeat_remove(data, datalen, 2, Remove_frame);
                    bitrate -= remove_number;
                    BT_DBG_NORMAL(BT_DEBUG_A2DP,"[Remove][in %ldth minute]Remove %ld frames of the current packet,so the current packet has %ld frames.",(unsigned long)minute,(unsigned long)remove_number,(unsigned long)bitrate);
                }
            }
        }
        //return number_of_frames;
        return bitrate; // return the number of frames in this packet after  compensated
    }
#endif	/* ENABLE_LDAC_CODEC */
    return 0;
}



/* FUNCTION NAME: SBCAAC_compensate_detect
 * PURPOSE:
 *      {}
 * INPUT:
 *      parameter-1  --  codec_type: codec type, currently only SBC/AAC will call this function
 *      parameter-2  --  data: audio data
 *      parameter-3  --  length: length of audio length
 *      parameter-4  --  ldac_procedure:
 * OUTPUT:
 *      parameter-n  --
 * RETURN:
 *      return-value-1 --
 * NOTES:			currently only SBC/AAC will call this function
 *
 */
INT32 SBCAAC_compensate_detect(BT_AUDIO_CODEC_TYPE codec_type,INT16 *data,UINT32 *length,LDAC_PROCEDURE ldac_procedure)
{
    static UINT32 u4count = 0;
    u4count++;
    if (u4count >= 10000000)
    {
        u4count = 0;
    }
    if ((BT_AUDIO_SBC == codec_type) ||(BT_AUDIO_AAC == codec_type))
    {
        if (SBC_AAC_data_comp)
            if ((*length) <= SBC_AAC_TEMP_DATA_LENGTH)
            {
                memcpy(SBC_AAC_data_comp,data,*length);
            }
            else
            {
                BT_DBG_NORMAL(BT_DEBUG_A2DP,"data length exceed the maximum!");
            }
        else
        {
            BT_DBG_ERROR(BT_DEBUG_A2DP,"SBC_AAC_data_comp is NULL!");
            return BT_ERR_STATUS_PARM_INVALID;
        }
        if (!((0 == bt_buf_ctl_G1_1) && (1 == bt_buf_ctl_G1_1)))
        {
            if (bt_detect_init && bt_buf_enable_detection)
            {
                if (SBC_AAC_enable_compensation)
                {
                    btaudio_compensation_process(codec_type,bt_detection_info.Minute_count, data, length);
                }
                else
                {
                    if (u4count % 200 == 0)
                    {
                        BT_DBG_NORMAL(BT_DEBUG_A2DP,"SBC/AAC Compensation process is disabled");
                    }
                }
            }
            //else
            //if (u4count % 200 == 0)
            //BT_DBG_NORMAL(BT_DEBUG_A2DP,"drift detection process is disabled");
        }
        else
        {
            if (u4count % 100 == 0)
            {
                //BT_DBG_INFO(BT_DEBUG_A2DP,"Compensation process is disabled");
            }
        }
        BT_DBG_MINOR(BT_DEBUG_A2DP,"%ld",(unsigned long)(*length));
        if (bt_detect_init)
        {
            received_accumulated_data_length(codec_type,*length);
        }
        if (bt_detect_init && bt_buf_enable_detection)
        {
            btaudio_detection_process(codec_type);
        }
        else
        {
            if (u4count % 100 == 0)
            {
                BT_DBG_INFO(BT_DEBUG_A2DP,"drift detection process is disabled");
            }
        }

    }
	/*
    else if (BT_AUDIO_LDAC == codec_type)	// currently only SBC/AAC will call this function , so this condition will not be fillfuled
    {
        static INT32 frames_number_after_compensated = 0;
        if (ldac_procedure == LDAC_COMPENSATION) // LDAC compensation
        { //SBCAAC_compensate_detect(BT_AUDIO_LDAC,(INT16 *)data,(INT32 *)&datalen,u4count,LDAC_COMPENSATION,&frames_number_after_compensated)
            UINT32 i4_samplerate = 0;
            UINT32 i4_channel = 0;
            bluetooth_a2dp_get_cur_audio_info(&i4_samplerate,&i4_channel);
            if ((i4_samplerate == 44100) || (i4_samplerate == 48000))
            {
                PCMnumber_per_frame = 128;
            }
            else if ((i4_samplerate == 88200) || (i4_samplerate == 96000))
            {
                PCMnumber_per_frame = 256;
            }
            else
            {
                BT_DBG_NORMAL(BT_DEBUG_A2DP,"warning:i4_samplerate is not 44100,48000,88200 or 96000!");
            }

            if (LDAC_packet_buffer && LDAC_data_temp)
            {
                memcpy(LDAC_packet_buffer,(UINT8 *)data,*length);
                memcpy(LDAC_data_temp,(UINT8 *)data,*length);
            }
            else
            {
                BT_DBG_ERROR(BT_DEBUG_A2DP,"LDAC_packet_buffer or LDAC_data_temp is NULL!");
                return BT_ERR_STATUS_PARM_INVALID;
            }

            if (u4count % 100 == 0)
                BT_DBG_NORMAL(BT_DEBUG_A2DP,"<LDAC,before compensated> datalength is:%ld",(unsigned long)(*length));

            if (!((0 == bt_buf_ctl_G1_1) && (1 == bt_buf_ctl_G1_1)))
            {
                if (bt_buf_enable_detection)
                {
                    frames_number_after_compensated = btaudio_compensation_process(codec_type,bt_detection_info.Minute_count, LDAC_packet_buffer, length);
                }
                else
                {
                    if (u4count % 100 == 0)
                    {
                        BT_DBG_MINOR(BT_DEBUG_A2DP,"compensation process is disable");
                    }
                }
            }
            else
            {
                if (u4count % 100 == 0)
                {
                    BT_DBG_INFO(BT_DEBUG_A2DP,"Compensation process is disable");
                }
            }
            // ldac_compensated_length = datalen;
            if (u4count % 100 == 0)
            {
                BT_DBG_NORMAL(BT_DEBUG_A2DP,"<LDAC,after compensated> before decode,datalength is:%ld",(unsigned long)(*length));
            }
        }
        else if (ldac_procedure == LDAC_DETECTION)// LDAC detection
        {
            if (u4count % 100 == 0)
            {
                //BT_DBG_NORMAL(BT_DEBUG_A2DP,"<LDAC,after compensated> after decode,datalength is:%d",total_decoded_data_length);
                BT_DBG_NORMAL(BT_DEBUG_A2DP,"<LDAC>frames_number_after_compensated(%ld) * PCMnumber_per_frame(%ld) is:%ld",(long)frames_number_after_compensated,(unsigned long)PCMnumber_per_frame,(unsigned long)(frames_number_after_compensated * PCMnumber_per_frame));
            }
            received_accumulated_data_length(codec_type,frames_number_after_compensated * PCMnumber_per_frame);
            if (bt_buf_enable_detection)
            {
                btaudio_detection_process(codec_type);
            }
            else
            {
                if (u4count % 100 == 0)
                {
                    BT_DBG_INFO(BT_DEBUG_A2DP,"Detection process is disable");
                }
            }
        }
    }
	*/
    return BT_SUCCESS;
}

#if ENABLE_LDAC_CODEC


/* FUNCTION NAME: btaudio_analyze_frame_status
 * PURPOSE:
 *      { to analyze the ststus of the LDAC frame , used for clock drift compensation}
 * INPUT:
 *      parameter-1  -- data: the original LDAC audio data
 *      parameter-2  -- datalen: the length of the original LDAC audio data
 		besides, the global variable last_frame_select is used as input

 * OUTPUT:
 *      parameter-1  --
			 search_word:	 btaudio_repeat_remove may be executed if it is not 0,
					 btaudio_repeat_remove will never be executed if it is 0.
			 search_result: will be used in function btaudio_repeat_remove.
			 last_frame_select: global variable ,update it
 * RETURN:
 *      return-value-1 -- the number of the LDAC frame
 * NOTES:
 *
 */
INT32 btaudio_analyze_frame_status(UINT8 * data, UINT16 datalen)
{
    UINT32 frame_length = 0;
    UINT32 i = 0;
    //UINT8 number_of_frames = 0;

    j++;
    if (j == 65535)
    {
        j=0;
    }
    search_word = 0;
    memset(search_result,0,sizeof(search_result));

    UINT8 temp_Byte = 0;
    UINT8 free_position[6] = {0};
    BOOL result_flag[6] = {0};
    BOOL temp[6] = {0};

    BT_DBG_MINOR(BT_DEBUG_A2DP,"The bit 22~23 of every frame is Frame status region");
    frame_length = datalen/number_of_frames;
    BT_DBG_MINOR(BT_DEBUG_A2DP,"datalen[%ld] = number_of_frames[%ld] * frame_length[%ld]",(unsigned long)datalen, (long)number_of_frames, (unsigned long)frame_length);
    BT_DBG_MINOR(BT_DEBUG_A2DP,"%ld = %ld * %ld",(unsigned long)datalen, (long)number_of_frames, (unsigned long)frame_length);
    UINT32 LDAC_CODEC_POS = 0;
    for (i=0;i < number_of_frames; i++)
    {
        /*the bit 22~23 of every frame is Frame status region
        For LDAC, the codec data format is following:
        330Kbs:
        03 AA 73 60 ....... AA 73 60 ........ AA 73 60 ......AA 73 60 ........ AA 73 60 .......AA 73 60 ....... AA 73 60 ......
           <----110Byte----><----110Byte----><----110Byte---><----110Byte----><----110Byte---><----110Byte----><----110Byte--->
        660Kbs:
        03 AA 73 60 ....... AA 73 60 ....... AA 73 60 ......
           <----220Byte----><----220Byte----><----220Byte--->
        990Kbs:
        03 AA 73 60 ....... AA 73 60 .......
           <----330Byte----><----330Byte---->
        The total length is always 661Bytes.
        */
        if (i == 0)
        {
            LDAC_CODEC_POS++;
            LDAC_CODEC_POS += 1;
            //BT_DBG_MINOR(BT_DEBUG_A2DP,"%ldth %ld frame: 0x%02X %02X %02X %02X",i+1 ,LDAC_CODEC_POS, data[LDAC_CODEC_POS - 3],data[LDAC_CODEC_POS - 2],data[LDAC_CODEC_POS - 1],data[LDAC_CODEC_POS]);
        }
        temp_Byte = data[LDAC_CODEC_POS];
        frame_status[i] = temp_Byte & 0x03;
        if (i)
            BT_DBG_MINOR(BT_DEBUG_A2DP,"%ldth %ld frame: 0x%02X %02X %02X %02X",(unsigned long)(i+1) ,LDAC_CODEC_POS, data[LDAC_CODEC_POS - 3],data[LDAC_CODEC_POS - 2],data[LDAC_CODEC_POS - 1],data[LDAC_CODEC_POS]);
        BT_DBG_MINOR(BT_DEBUG_A2DP,"temp_Byte:%x frame_status:%X",temp_Byte, frame_status[i]);
        LDAC_CODEC_POS += frame_length;

        /*bitmap*/
        if (0x1 == frame_status[i])
        {
            bitmap[i] = 0x1;
        }
        else if (0x2 == frame_status[i])
        {
            bitmap[i] = 0x2;
        }
        else if (0x3 == frame_status[i])
        {
            bitmap[i] = 0x4;
        }
        else if (0x0 == frame_status[i])
        {
            bitmap[i] = 0x0;
        }
        else
        {
            /*invalid status*/
        }
        search_result[i] = bitmap[i];
    }

    if (ldac_switch && 1)
    {
        BT_DBG_NORMAL(BT_DEBUG_A2DP,"<swl>(1) total %ld frames,each status is:",(long)number_of_frames);
        for (i=0;i < number_of_frames; i++)
        {
            BT_DBG_NORMAL(BT_DEBUG_A2DP,"%X  ",search_result[i]);
        }
        BT_DBG_NORMAL(BT_DEBUG_A2DP,"");
    }

    BT_DBG_MINOR(BT_DEBUG_A2DP,"select status = 3");
    /*select status = 3*/
    for (i=0;i < number_of_frames; i++)
    {
        if (0x4 == search_result[i])//status = 3
        {
            result_flag[i] = TRUE;
        }
    }

    if (ldac_switch && (1))
    {
        BT_DBG_NORMAL(BT_DEBUG_A2DP,"<swl>(2) after select status = 3,result_flag is:");
        for (i=0;i < number_of_frames; i++)
        {
            BT_DBG_NORMAL(BT_DEBUG_A2DP,"%s  ",result_flag[i]==TRUE?"TRUE":"FALSE");
        }
        BT_DBG_NORMAL(BT_DEBUG_A2DP,"TRUE means status is 3.");
    }

    BT_DBG_MINOR(BT_DEBUG_A2DP,"Mark data for before and after status = 3");
    /*Mark data for before and after status = 3*/
    for (i=0;i < number_of_frames; i++)
    {
        temp[i] = result_flag[i];
    }
    for (i=0;i < number_of_frames; i++)
    {
        if (result_flag[i])
        {
            if (i == 0)
            {
                temp[i + 1] = TRUE;
            }
            else if (i == number_of_frames-1)
            {
                temp[i - 1] = TRUE;
            }
            else
            {
                if (i >0)
                {
                    temp[i - 1] = TRUE;
                }
                if (i < number_of_frames - 1)
                {
                    temp[i + 1] = TRUE;
                }
            }
        }
    }
    for (i=0;i < number_of_frames; i++)
    {
        result_flag[i] = temp[i];
    }

    if (ldac_switch && (1))
    {
        BT_DBG_NORMAL(BT_DEBUG_A2DP,"<swl>(3) after Mark data for before and after status = 3,result_flag is:");
        for (i=0;i < number_of_frames; i++)
        {
            BT_DBG_NORMAL(BT_DEBUG_A2DP,"%s  ",result_flag[i]==TRUE?"TRUE":"FALSE");
        }
        BT_DBG_NORMAL(BT_DEBUG_A2DP,"");
    }

    BT_DBG_MINOR(BT_DEBUG_A2DP,"get free position");
    /*get free position*/
    for (i=0;i < number_of_frames; i++)
    {
        if (FALSE == result_flag[i])
        {
            free_position[i] = 0xF;
        }
        else
        {
            free_position[i] = 0x4;
        }
    }

    if (ldac_switch && (1))
    {
        BT_DBG_NORMAL(BT_DEBUG_A2DP,"<swl>(4) after get free position,free_position is:");
        for (i=0;i < number_of_frames; i++)
        {
            BT_DBG_NORMAL(BT_DEBUG_A2DP,"%X  ",free_position[i]);
        }
        BT_DBG_NORMAL(BT_DEBUG_A2DP,"");
    }


    BT_DBG_MINOR(BT_DEBUG_A2DP,"update free position");
    /*update free position*/
    if ((0x1 == last_frame_select) || (0x2 == last_frame_select))
    {
        if (0xF == free_position[0])
        {
            free_position[0] = 0x0;
        }
    }
    else if (0x4 == last_frame_select)
    {
        if (0xF == free_position[0])
        {
            free_position[0] = 0x4;
        }
    }

    if (ldac_switch && (1))
    {
        BT_DBG_NORMAL(BT_DEBUG_A2DP,"<swl>(5) last_frame_select is 0x%X, after update free position,free_position is:",last_frame_select);
        for (i=0;i < number_of_frames; i++)
        {
            BT_DBG_NORMAL(BT_DEBUG_A2DP,"%X  ",free_position[i]);
        }
        BT_DBG_NORMAL(BT_DEBUG_A2DP,"");
    }

    BT_DBG_MINOR(BT_DEBUG_A2DP,"select status = 1 or 2");
    /*select status = 1 or 2*/
    for (i=0;i < number_of_frames; i++)
    {
        search_result[i] &= free_position[i];
    }

    if (ldac_switch && (1))
    {
        BT_DBG_NORMAL(BT_DEBUG_A2DP,"<swl>(6) after select status = 1 or 2,search_result is:");
        for (i=0;i < number_of_frames; i++)
        {
            BT_DBG_NORMAL(BT_DEBUG_A2DP,"%X  ",search_result[i]);
        }
        BT_DBG_NORMAL(BT_DEBUG_A2DP,"");
    }

    BT_DBG_MINOR(BT_DEBUG_A2DP,"Search result:");
    for (i=0;i < number_of_frames; i++)
    {
        if (search_result[i] != 0)
        {
            search_word = 1; //  work as a flag . 1:may be repeat or remove operation, 0: never to repeat or remove operation
            break;
        }
    }

    /*set last_frame_selected*/
    last_frame_select = bitmap[number_of_frames-1];

    if (ldac_switch && (1))
    {
        BT_DBG_NORMAL(BT_DEBUG_A2DP,"<swl>(7) at last,search_word is 0x%lX, and update \"last_frame_select\" as 0x%X",search_word,last_frame_select);
    }

    return (INT32)number_of_frames;
}

INT32 get_LADC_packet_information(UINT8 * data, UINT16 *datalen, UINT16 *frames_length ,UINT8 maximum, UINT8 *frames_number)
{
    UINT8 * pdata =  data;
    static UINT32 counter = 0;

    UINT8 i = 0; // to store the catual number of frames in the current paclet
    UINT8 j = 0;
    UINT16 *p_frame_length = frames_length;
    UINT8 tmp1 = 0,tmp2 = 0;
    UINT16 length_of_frame = 0;

    counter ++;
    if (counter == 10000000)
    {
        counter = 0;
    }
    while (pdata < data +(*datalen))
    {
        tmp1 = pdata[1]&0x07;
        tmp2 = (pdata[2]>>2)&0x3F;
        length_of_frame = (tmp1 * 64 + tmp2)+1+3;
        p_frame_length[i] = length_of_frame;
        i++;
        if (i > maximum)
        {
            BT_DBG_ERROR(BT_DEBUG_A2DP,"frame index:%ld,the maximum number of frames in a LDAC packet cannot exceed 15.",(unsigned long)i);
        }
        pdata = pdata + length_of_frame;
    }
    if (0)//bt_buf_ctl_time >= 2)
    {
        BT_DBG_NORMAL(BT_DEBUG_A2DP,"there are %ld frames in the current packet,frame length is:",(unsigned long)i);
        for (j=0;j<i;j++)
        {
            BT_DBG_NORMAL(BT_DEBUG_A2DP,"%ldth frame:%ld bytes",(unsigned long)j,(unsigned long)frames_length[j]);
        }
        BT_DBG_NORMAL(BT_DEBUG_A2DP,"");
    }

    *frames_number = i;

    return BT_SUCCESS;
}


/* FUNCTION NAME: should_packet_be_repeated_removed
 * PURPOSE:
 *      { to judge whether frames in the LDAC packet should be repeated/removed or not   }
 * INPUT:
 *      parameter-1  -- frames_length: a array pointer, the array store the length of each frame in the LDAC packet
 *      parameter-2  -- frame_number : the number of frames in the LDAC packet
 * OUTPUT:
 *      parameter-n  --
 * RETURN:
 *      return-value-1 -- if return BT_SUCCESS, then this packet may be repeated/removed
 			      else ,mustn't be repeated/removed

 * NOTES:
 *
 */
INT32 should_packet_be_repeated_removed(UINT16 *frames_length, UINT32 frame_number)
{
    UINT8 i=0;
    INT32 ret = BT_SUCCESS;
    static UINT32 counter = 0;

    counter ++;
    if (counter == 10000000)
    {
        counter = 0;
    }
    if (!((frame_number == 2) || (frame_number == 3) || (frame_number == 4) || (frame_number == 5) || (frame_number == 6))) // when the number of the packet is not 2,3,4,5 or 6
    {
        if (counter%100 == 0)
        {
            BT_DBG_NORMAL(BT_DEBUG_A2DP,"This packet has %ld frames,so don't repeat/remove frames in the packet.",(unsigned long)frame_number);
        }
        ret = BT_ERR_STATUS_UNSUPPORTED;
    }
    else if ((frame_number == 2) || (frame_number == 3) || (frame_number == 4) ||(frame_number == 5) || (frame_number == 6))// when the number of the packet is 2,3,4,5 or 6
    {
        UINT16 temp=0;
        if (counter%100 == 0)
        {
            BT_DBG_NORMAL(BT_DEBUG_A2DP,"This packet has %ld frames, length of each frame is:",(unsigned long)frame_number);
            for (i=0;i<frame_number;i++)
            {
                BT_DBG_NORMAL(BT_DEBUG_A2DP,"%ld  ",(unsigned long)frames_length[i]);
            }
            BT_DBG_NORMAL(BT_DEBUG_A2DP,"");
        }
        for (i=0;i<frame_number;i++)
        {
            if (i == 0)
            {
                temp = frames_length[i];
            }
            else
            {
                if (frames_length[i] != temp)
                {
                    ret = BT_ERR_STATUS_UNSUPPORTED;
                    if (counter%100 == 0)
                    {
                        BT_DBG_NORMAL(BT_DEBUG_A2DP,"this packet has %ld frames,but the length of frames is not exactly the same.",(unsigned long)frame_number);
                    }
                    break;
                }
            }
        }
    }

    return ret;
}

/* FUNCTION NAME: btaudio_repeat_remove
 * PURPOSE:
 *      { the condition is fulfilled ,so execute the operation of repeat or remove}
 * INPUT:
 *      parameter-1  -- data: the LDAC audio data
 *      parameter-2  -- datalen: pointer of the data length
 *      parameter-3  -- Type: indicate the operation type,repeat or remove
 *      parameter-4  -- the maximum number of repeated/removed frames in one minute
 * OUTPUT:
 *      parameter-n  --
 * RETURN:
 *      return-value-1 --  the number of frames in the LDAC packet after repeat/remove
 * NOTES:
 *
 */
UINT32 btaudio_repeat_remove(UINT8 * data, UINT16 *datalen, UINT8 Type, UINT32 Comp_limit_number)
{
    BT_DBG_MINOR(BT_DEBUG_A2DP,"Enter into--->%s", __FUNCTION__);
    UINT32 i = 0;
    //UINT32 k = 0;

    UINT32 comp_number = 0;
    UINT32 frame_length = 0;
#if LDAC_PACKET_LIMIT
    UINT16 pre_datalen = *datalen;
#endif
    bt_repeat_remove_status_t Exchange_flag = BT_REPEAT_REMOVE_IDEL;

    UINT8 pos[6] = {0};
    BT_DBG_NOTICE(BT_DEBUG_A2DP,"Calculate the remove/repeat position.");
    /*Calculate the remove/repeat position*/
    for (i = 0;i < number_of_frames;i++)
    {
        BT_DBG_MINOR(BT_DEBUG_A2DP,"search_result[%ld]:%X",(unsigned long)i, search_result[i]);
        if ((0x1 == search_result[i]) || (0x2 == search_result[i]))
        {
            /*For 0x1 or 0x2, if the previous one is not repeated or removed,it should be repeated or removed*/
            if ((BT_REPEAT_REMOVE_IDEL == Exchange_flag) || (BT_REPEAT_REMOVE_SINGLE_STOP == Exchange_flag))
            {
                Exchange_flag = BT_REPEAT_REMOVE_SINGLE_START;
                pos[i] = search_result[i];
            }
            else/*For 0x1 or 0x2, SUCCESSIVE Ox1 or 0x2 can't be repeated or removed*/
            {
                Exchange_flag = BT_REPEAT_REMOVE_SINGLE_STOP;
            }
        }
        else if (0x4 == search_result[i])
        {
            /*For 0x4, it should be repeated or removed*/
            pos[i] = search_result[i];
            Exchange_flag = BT_REPEAT_REMOVE_MULTI;
        }
        else
        {
            Exchange_flag = BT_REPEAT_REMOVE_IDEL;
        }
    }

    if (ldac_switch && 1)
    {
        BT_DBG_NORMAL(BT_DEBUG_A2DP,"<swl>(8) after Calculate the remove/repeat position:");
        for (i=0;i < number_of_frames; i++)
        {
            BT_DBG_NORMAL(BT_DEBUG_A2DP,"%X  ",pos[i]);
        }
        BT_DBG_NORMAL(BT_DEBUG_A2DP,"if not 0, it will be remove or repeat.");
    }

    BT_DBG_MINOR(BT_DEBUG_A2DP,"Remove/Repeat");

    frame_length = (*datalen)/number_of_frames;
    UINT32 current_pos = 0;

    memset(data,0,*datalen);
    *datalen = 0;

    if (1 == Type)//repeat
    {
        current_pos = 0;
        for (i = 0;i < number_of_frames;i++)
        {
            // if repeated frame number is Comp_limit_number, the afterward frames should not be repeated even if their flag is not 0
            if (Fcomp >= Comp_limit_number)
            {
                if (current_pos + frame_length <= LDAC_PACKET_LENGTH_AFTER_COMPENSATED)
                {
                    if ((i * frame_length < LDAC_ORIGINAL_PACKET_LENGTH) && (i * frame_length + frame_length < LDAC_ORIGINAL_PACKET_LENGTH))
                    {
                        memcpy(&data[current_pos], &LDAC_data_temp[i * frame_length], frame_length);
                        current_pos += frame_length;
                        *datalen += frame_length;
                    }
                    else
                    {
                        BT_DBG_ERROR(BT_DEBUG_A2DP,"LDAC frame repeat error!(i * frame_length:%ld,frame_length:%ld)",(unsigned long)(i * frame_length),(unsigned long)frame_length);
                    }
                }
                else
                {
                    BT_DBG_ERROR(BT_DEBUG_A2DP,"LDAC frame repeat error!(current_pos:%ld,frame_length:%ld)",(unsigned long)current_pos,(unsigned long)frame_length);
                }

                continue;
            }
            if (pos[i] > 0)
            {
                Fcomp ++;
                comp_number++;
                BT_DBG_NOTICE(BT_DEBUG_A2DP,"<swl>(9) pos[%ld]:%ld repeated",(unsigned long)i,(unsigned long)pos[i]);
                /*The repeating number of Current packet is over compensation number*/
                if (Fcomp >= Comp_limit_number)
                {
                    BT_DBG_NORMAL(BT_DEBUG_A2DP,"Fcomp(%ld)increase to the limit(Add_frame:%ld),so won't repeat frame within the minute.", (unsigned long)Fcomp, (unsigned long)Comp_limit_number);
                }

                /**************Original part**************/

                //BT_DBG_NOTICE(BT_DEBUG_A2DP,"copy from %ld to %ld",i * frame_length,current_pos);
                if (current_pos + frame_length <= LDAC_PACKET_LENGTH_AFTER_COMPENSATED)
                {
                    if ((i * frame_length < LDAC_ORIGINAL_PACKET_LENGTH) && (i * frame_length + frame_length < LDAC_ORIGINAL_PACKET_LENGTH))
                    {
                        memcpy(&data[current_pos], &LDAC_data_temp[i * frame_length], frame_length);
                        current_pos += frame_length;
                        *datalen += frame_length;
                    }
                    else
                    {
                        BT_DBG_ERROR(BT_DEBUG_A2DP,"LDAC frame repeat error!(i * frame_length:%ld,frame_length:%ld)",(unsigned long)(i * frame_length),(unsigned long)frame_length);
                    }
                }
                else
                {
                    BT_DBG_ERROR(BT_DEBUG_A2DP,"LDAC frame repeat error!(current_pos:%ld,frame_length:%ld)",(unsigned long)current_pos,(unsigned long)frame_length);
                }

                /**************repeat part**************/

                //BT_DBG_NOTICE(BT_DEBUG_A2DP,"copy from %ld to %ld",i * frame_length,current_pos);
                if (current_pos + frame_length <= LDAC_PACKET_LENGTH_AFTER_COMPENSATED)
                {
                    if ((i * frame_length < LDAC_ORIGINAL_PACKET_LENGTH) && (i * frame_length + frame_length < LDAC_ORIGINAL_PACKET_LENGTH))
                    {
                        memcpy(&data[current_pos], &LDAC_data_temp[i * frame_length], frame_length);
                        current_pos += frame_length;
                        *datalen += frame_length;
                    }
                    else
                    {
                        BT_DBG_ERROR(BT_DEBUG_A2DP,"LDAC frame repeat error!(i * frame_length:%ld,frame_length:%ld)",(unsigned long)(i * frame_length),(unsigned long)frame_length);
                    }
                }
                else
                {
                    BT_DBG_ERROR(BT_DEBUG_A2DP,"LDAC frame repeat error!(current_pos:%ld,frame_length:%ld)",(unsigned long)current_pos,(unsigned long)frame_length);
                }

            }
            else
            {
                //BT_DBG_NORMAL(BT_DEBUG_A2DP,"copy from %ld to %ld",i * frame_length,current_pos);
                memcpy(&data[current_pos], &LDAC_data_temp[i * frame_length], frame_length);
                current_pos += frame_length;
                *datalen += frame_length;
            }
        }
        if ((*datalen) != (number_of_frames + comp_number)*(frame_length))
        {
            BT_DBG_ERROR(BT_DEBUG_A2DP,"repeat operation!");
        }
        BT_DBG_NOTICE(BT_DEBUG_A2DP,"in this packet total repeat %ld frames.",(unsigned long)comp_number);
        BT_DBG_NOTICE(BT_DEBUG_A2DP,"after compensated this packet data has %ld frames.", (unsigned long)(number_of_frames + comp_number));

    }
    else if (2 == Type)//remove
    {
        current_pos = 0;
        for (i = 0;i < number_of_frames;i++)
        {
            // if removed frame number is Comp_limit_number, the afterward frames should not be removed even if their flag is not 0
            if (Fcomp>= Comp_limit_number)
            {
                memcpy(&data[current_pos], &LDAC_data_temp[i*frame_length], frame_length);
                *datalen += frame_length;
                current_pos += frame_length;
                continue;
            }
            if (pos[i] > 0)
            {
                Fcomp ++;
                comp_number ++;
                BT_DBG_NOTICE(BT_DEBUG_A2DP,"<swl>(9) pos[%ld]:%ld removed",(unsigned long)i,(unsigned long)pos[i]);
                if (Fcomp >= Comp_limit_number)
                {
                    BT_DBG_NORMAL(BT_DEBUG_A2DP,"Fcomp(%ld)increase to the limit(Remove_frame:%ld),so won't remove frame within the minute.", (unsigned long)Fcomp, (unsigned long)Comp_limit_number);
                }
            }
            else
            {
                memcpy(&data[current_pos], &LDAC_data_temp[i*frame_length], frame_length);
                *datalen += frame_length;
                current_pos += frame_length;
            }
        }
        BT_DBG_NOTICE(BT_DEBUG_A2DP,"in this packet total remove %ld frames.",(unsigned long)comp_number);
        BT_DBG_NOTICE(BT_DEBUG_A2DP,"after compensated this packet data has %ld frames.", (unsigned long)(number_of_frames - comp_number));
    }

    BT_DBG_NOTICE(BT_DEBUG_A2DP,"<swl>(10)in this packet, total %ld frames are %s,%s is %ld.",(unsigned long)comp_number,Type==1?"repeated":"removed",Type==1?"Add_frame":"Remove_frame",(unsigned long)Comp_limit_number);

    FUNC_EXIT;
    return comp_number;
}

/* FUNCTION NAME: repeat_LDAC_frame
 * PURPOSE:
 *      { }
 * INPUT:
 *      parameter-1  -- data_temp :  audio data
 *      parameter-2  -- datalen_temp :  the length of audio data
 *      parameter-3  -- repeat_number :  the number of frame that you want to repeat
 * OUTPUT:
 *      parameter-n  --
 * RETURN:
 *      return-value-1 --
 * NOTES:	this function won't be call in normal buffer control flow, just to test LDAC decode flow and LDAC buffer control repeat operation
 		when the specific CLI is use , this function will be called.
 *
 */
UINT32 repeat_LDAC_frame(VOID *data_temp, VOID* datalen_temp,INT32 repeat_number)
{
    UINT8 * data = (UINT8 *)data_temp;
    UINT16 * datalen = (UINT16 *)datalen_temp;
    UINT16 local_length = 0;
    UINT16 frames_length[15] = {0}; // the maximum number of frames in a LDAC packet is 15
    UINT8 frames_number = 0;
    UINT32 frame_length = 0;
    UINT8 i = 0;
    UINT32 current_pos = 0;

    static UINT32 counter = 0;

    counter ++;
    if (counter == 10000000)
    {
        counter = 0;
    }
    /* get_LADC_packet_information */
    get_LADC_packet_information(data, datalen, frames_length, 15, &frames_number);
    if (counter % 500 ==0)
    {
        BT_DBG_NORMAL(BT_DEBUG_A2DP,"This packet has %ld frames, length of each frame is:",(unsigned long)frames_number);
        for (i=0;i<frames_number;i++)
        {
            BT_DBG_NORMAL(BT_DEBUG_A2DP,"%ld  ",(unsigned long)frames_length[i]);
        }
        BT_DBG_NORMAL(BT_DEBUG_A2DP,"");
    }
    /* to judge whether this LDAC packet should be repeated or not */
    if (should_packet_be_repeated_removed(frames_length,frames_number) != BT_SUCCESS)
    {
        BT_DBG_NORMAL(BT_DEBUG_A2DP,"Cannot repeat, because condition is not satisfied.");
        return frames_number;
    }
    if ((counter != 0) && (counter % 600 !=0))
    {
        repeat_LDAC_frame_flag = FALSE;
        return frames_number; //  every 600 packet packet , repeat
    }
    else
    {
        repeat_LDAC_frame_flag = TRUE;
    }
    frame_length = frames_length[0];
    // BT_DBG_NORMAL(BT_DEBUG_A2DP,"begin to repeat,each frame has the length:%ld",frame_length);
    if (repeat_LDAC_frame_flag)
    {
        BT_DBG_NORMAL(BT_DEBUG_A2DP,"before repeat,datalen is :%ld",(unsigned long)(*datalen));
    }
    memset(data,0,*datalen);
    //*datalen = 0;

    if ((data == NULL) || (LDAC_data_temp ==NULL))
    {
        BT_DBG_ERROR(BT_DEBUG_A2DP,"data or LDAC_data_temp is NULL!");
    }
    if (repeat_number <= frames_number)
    {
        current_pos = 0;
        for (i = 0;i < frames_number;i++)
        {
            /**************Original part**************/
            if (current_pos + frame_length <= LDAC_PACKET_LENGTH_AFTER_COMPENSATED)
            {
                if ((i * frame_length < LDAC_ORIGINAL_PACKET_LENGTH) && (i * frame_length + frame_length < LDAC_ORIGINAL_PACKET_LENGTH))
                {
                    memcpy(&data[current_pos], &LDAC_data_temp[i * frame_length], frame_length);
                    current_pos += frame_length;
                    //*datalen += frame_length;
                    local_length += frame_length;
                }
                else
                {
                    BT_DBG_ERROR(BT_DEBUG_A2DP,"LDAC frame repeat error!(i * frame_length:%ld,frame_length:%ld)",(unsigned long)(i * frame_length),(unsigned long)frame_length);
                }
            }
            else
            {
                BT_DBG_ERROR(BT_DEBUG_A2DP,"LDAC frame repeat error!(current_pos:%ld,frame_length:%ld)",(unsigned long)current_pos,(unsigned long)frame_length);
            }
            /**************repeat part**************/
            if (repeat_number > 0)
            {
                if (current_pos + frame_length <= LDAC_PACKET_LENGTH_AFTER_COMPENSATED)
                {
                    if ((i * frame_length < LDAC_ORIGINAL_PACKET_LENGTH) && (i * frame_length + frame_length < LDAC_ORIGINAL_PACKET_LENGTH))
                    {
                        memcpy(&data[current_pos], &LDAC_data_temp[i * frame_length], frame_length);
                        current_pos += frame_length;
                        //*datalen += frame_length;
                        local_length += frame_length;
                        repeat_number--;
                    }
                    else
                    {
                        BT_DBG_ERROR(BT_DEBUG_A2DP,"LDAC frame repeat error!(i * frame_length:%ld,frame_length:%ld)",(unsigned long)(i * frame_length),(unsigned long)frame_length);
                    }
                }
                else
                {
                    BT_DBG_ERROR(BT_DEBUG_A2DP,"LDAC frame repeat error!(current_pos:%ld,frame_length:%ld)",(unsigned long)current_pos,(unsigned long)frame_length);
                }
            }
        }
    }
    else
    {
        BT_DBG_NORMAL(BT_DEBUG_A2DP,"repeat number error!");
        return frames_number;
    }
    *datalen = local_length;
    if (repeat_LDAC_frame_flag)
    {
        BT_DBG_NORMAL(BT_DEBUG_A2DP,"*datalen:%ld,local_length:%ld",(unsigned long)(*datalen),(unsigned long)local_length);
    }
    BT_DBG_NORMAL(BT_DEBUG_A2DP,"(for debug)repeat %ld frames, total size:%ld (%ld).",(unsigned long)repeat_LDAC_frame_number, (unsigned long)((frames_number + repeat_LDAC_frame_number)*frame_length),(unsigned long)(*(UINT16 *)datalen_temp));
    return frames_number + repeat_LDAC_frame_number;

}

/* FUNCTION NAME: LDAC_compensate
 * PURPOSE:
 *      {for LDAC compensation}
 * INPUT:
 *      parameter-1  -- pu1InBuf	: input data
 *      parameter-2  -- u4InLen	: input data length
 *      parameter-3  -- pu1OutBuf	: output data
 *      parameter-4  -- pu4OutLen	: output data length
 * OUTPUT:
 *      parameter-n  --
 * RETURN:
 *      return-value-1 -- frames number after compensated
 * NOTES:
 *
 */
UINT32 LDAC_compensate(UINT8 *pu1InBuf,UINT32 u4InLen, UINT8 *pu1OutBuf, UINT32 *pu4OutLen)
{
    UINT32 i4_samplerate = 0;
    UINT32 i4_channel = 0;
    //INT32 total_decoded_data_length = 0;
    UINT32 frames_number_after_compensated = 0;
    UINT32 frames_number_after_repeated = 0;

    static UINT32 u4count = 0;
    u4count++;
    if (u4count >= 10000000)
    {
        u4count = 0;
    }

    bluetooth_a2dp_get_cur_audio_info(&i4_samplerate,&i4_channel);
    if ((i4_samplerate == 44100) || (i4_samplerate == 48000))
    {
        PCMnumber_per_frame = 128;
    }
    else if ((i4_samplerate == 88200) || (i4_samplerate == 96000))
    {
        PCMnumber_per_frame = 256;
    }
    else
    {
        if (u4count % 100 == 0)
        {
            BT_DBG_NORMAL(BT_DEBUG_A2DP,"warning:i4_samplerate is not 44100,48000,88200 or 96000!");
        }
    }
    if (LDAC_packet_buffer && LDAC_data_temp)
    {
        if (u4InLen < LDAC_PACKET_LENGTH_AFTER_COMPENSATED)
        {
            memcpy(LDAC_packet_buffer,pu1InBuf,u4InLen);
        }
        else
        {
            BT_DBG_ERROR(BT_DEBUG_A2DP,"LDAC pu1InBuf pu4InLen too great,memcpy error.");
        }
        if (u4InLen < LDAC_ORIGINAL_PACKET_LENGTH)
        {
            memcpy(LDAC_data_temp,pu1InBuf,u4InLen);
        }
        else
        {
            BT_DBG_ERROR(BT_DEBUG_A2DP,"LDAC pu1InBuf pu4InLen too great,memcpy error.");
        }
    }
    else
    {
        BT_DBG_ERROR(BT_DEBUG_A2DP,"LDAC_packet_buffer or LDAC_data_temp is NULL!");
        if (pu1OutBuf != pu1InBuf)
        {
            memcpy(pu1OutBuf, pu1InBuf, u4InLen);
        }
        *pu4OutLen = u4InLen;

        return BT_ERR_STATUS_PARM_INVALID; //  when run to this place , unexpected result may happen.
    }
    if ((repeat_LDAC_frame_number >0) && (!bt_buf_enable_detection)) /* when CLI "detect 0" and "ldac_repeat [integer]" is executed */
    {
        frames_number_after_repeated = repeat_LDAC_frame(LDAC_packet_buffer, &u4InLen, repeat_LDAC_frame_number);
        memcpy(pu1OutBuf, LDAC_packet_buffer, u4InLen);
        *pu4OutLen = u4InLen;
        return frames_number_after_repeated;
    }

    if (u4count % 200 == 0)
    {
        BT_DBG_NORMAL(BT_DEBUG_A2DP,"<LDAC,before compensated> datalength is:%ld",(unsigned long)u4InLen);
    }

    if (!((0 == bt_buf_ctl_G1_1) && (1 == bt_buf_ctl_G1_1)))
    {
        if (bt_detect_init && bt_buf_enable_detection)
        {
            frames_number_after_compensated =
                btaudio_compensation_process(BT_AUDIO_LDAC,bt_detection_info.Minute_count, LDAC_packet_buffer, &u4InLen);
        }
        else
        {
            if (u4count % 100 == 0)
            {
                BT_DBG_MINOR(BT_DEBUG_A2DP,"compensation process is disable");
            }
        }
    }
    else
    {
        if (u4count % 100 == 0)
        {
            BT_DBG_INFO(BT_DEBUG_A2DP,"Compensation process is disable");
        }
    }
    // ldac_compensated_length = datalen;
    if (u4count % 200 == 0)
    {
        BT_DBG_NORMAL(BT_DEBUG_A2DP,"<LDAC,after compensated> before decode,datalength is:%ld",(unsigned long)u4InLen);
    }

    memcpy(pu1OutBuf, LDAC_packet_buffer, u4InLen);
    *pu4OutLen = u4InLen;
    return frames_number_after_compensated;
}

/* FUNCTION NAME: LDAC_detect
 * PURPOSE:
 *      {for LDAC detection}
 * INPUT:
 *      parameter-1  -- frames_number_after_compensated :
 * OUTPUT:
 *      parameter-n  --
 * RETURN:
 *      return-value-1 --  not used
 * NOTES:
 *
 */
INT32 LDAC_detect(INT32 frames_number_after_compensated)
{
    static UINT32 u4count = 0;
    u4count++;
    if (u4count >= 10000000)
    {
        u4count = 0;
    }

    if (u4count % 100 == 0)
    {
        if (frames_number_after_compensated <= 0)
        {
            BT_DBG_WARNING(BT_DEBUG_A2DP,"frames_number_after_compensated:%ld !!! number_of_frames:%ld ",frames_number_after_compensated,(long)number_of_frames);
            frames_number_after_compensated = number_of_frames;
        }
    }
    if (bt_detect_init)
    {
        received_accumulated_data_length(BT_AUDIO_LDAC,frames_number_after_compensated * PCMnumber_per_frame);
        if (u4count % 100 == 0)
        {
            if (frames_number_after_compensated != 0)
            {
                BT_DBG_NORMAL(BT_DEBUG_A2DP,"<LDAC>frames_number_after_compensated(%ld),PCMnumber_per_frame(%ld)",(long)frames_number_after_compensated,(unsigned long)PCMnumber_per_frame);
            }
        }
    }
    if (bt_detect_init && bt_buf_enable_detection)
    {
        btaudio_detection_process(BT_AUDIO_LDAC);
    }
    else
    {
        if (u4count % 100 == 0)
        {
            BT_DBG_INFO(BT_DEBUG_A2DP,"Detection process is disable");
        }
    }

    return BT_SUCCESS;
}

#endif// #if ENABLE_LDAC_CODEC

#endif // #if ENABLE_BUFFER_CONTROL

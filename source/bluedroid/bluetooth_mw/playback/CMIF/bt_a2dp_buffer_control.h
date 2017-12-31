/********************************************************************************************
 *     LEGAL DISCLAIMER
 *
 *     (Header of MediaTek Software/Firmware Release or Documentation)
 *
 *     BY OPENING OR USING THIS FILE, BUYER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND AGREES
 *     THAT THE SOFTWARE/FIRMWARE AND ITS DOCUMENTATIONS ("MEDIATEK SOFTWARE") RECEIVED
 *     FROM MEDIATEK AND/OR ITS REPRESENTATIVES ARE PROVIDED TO BUYER ON AN "AS-IS" BASIS
 *     ONLY. MEDIATEK EXPRESSLY DISCLAIMS ANY AND ALL WARRANTIES, EXPRESS OR IMPLIED,
 *     INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR
 *     A PARTICULAR PURPOSE OR NONINFRINGEMENT. NEITHER DOES MEDIATEK PROVIDE ANY WARRANTY
 *     WHATSOEVER WITH RESPECT TO THE SOFTWARE OF ANY THIRD PARTY WHICH MAY BE USED BY,
 *     INCORPORATED IN, OR SUPPLIED WITH THE MEDIATEK SOFTWARE, AND BUYER AGREES TO LOOK
 *     ONLY TO SUCH THIRD PARTY FOR ANY WARRANTY CLAIM RELATING THERETO. MEDIATEK SHALL ALSO
 *     NOT BE RESPONSIBLE FOR ANY MEDIATEK SOFTWARE RELEASES MADE TO BUYER'S SPECIFICATION
 *     OR TO CONFORM TO A PARTICULAR STANDARD OR OPEN FORUM.
 *
 *     BUYER'S SOLE AND EXCLUSIVE REMEDY AND MEDIATEK'S ENTIRE AND CUMULATIVE LIABILITY WITH
 *     RESPECT TO THE MEDIATEK SOFTWARE RELEASED HEREUNDER WILL BE, AT MEDIATEK'S OPTION,
 *     TO REVISE OR REPLACE THE MEDIATEK SOFTWARE AT ISSUE, OR REFUND ANY SOFTWARE LICENSE
 *     FEES OR SERVICE CHARGE PAID BY BUYER TO MEDIATEK FOR SUCH MEDIATEK SOFTWARE AT ISSUE.
 *
 *     THE TRANSACTION CONTEMPLATED HEREUNDER SHALL BE CONSTRUED IN ACCORDANCE WITH THE LAWS
 *     OF THE STATE OF CALIFORNIA, USA, EXCLUDING ITS CONFLICT OF LAWS PRINCIPLES.
 ************************************************************************************************/
/* FILE NAME:  bt_a2dp_buffer_control.h
 * PURPOSE:
 *  {1. What is covered in this file - function and scope.}
 *  {2. Related documents or hardware information}
 * NOTES:
 *  {Something must be known or noticed}
 *  {1. How to use these functions - Give an example.}
 *  {2. Sequence of messages if applicable.}
 *  {3. Any design limitation}
 *  {4. Any performance limitation}
 *  {5. Is it a reusable component}
 *
 *
 *
 */

#ifndef _BT_A2DP_BUFFER_CONTROL_H
#define _BT_A2DP_BUFFER_CONTROL_H

#include <stdio.h>
#include <sys/time.h>
#include "u_bluetooth.h"

#if ENABLE_BUFFER_CONTROL
#define NORMAL_LDAC_PACKET_LENGTH 661
#define MAX_TIME_COUNT 480 // 480 minute, 8 hours
#define TIME_INTERVAL 60000/*one minute*/
#define MAX_FRAME_COUNT 16
/*
#define myPrintf(...) \
                do{ \
                    printf(__VA_ARGS__);    \
                }   \
                while(0)
*/
/*sample info for every frame*/
typedef struct frame_sample_info_s
{
    UINT32 frame_count;
    struct timeval last_time;
    struct timeval current_time;
    struct timeval start_time;
    struct timeval Time[MAX_FRAME_COUNT];/*Actually Time*/
    double Number[MAX_FRAME_COUNT];// sample number of this frame
    double DriftN[MAX_FRAME_COUNT];// drift sample number during these frame , this domain seems not used
}bt_frame_sample_info_t;

/*monitor the sample every duration time*/
typedef struct detection_info_s
{
    double Time[MAX_TIME_COUNT];/*Actually Time*/
    double AveT;
    double AveN;
    double Nminute[MAX_TIME_COUNT];/*The sample number at current minute*/
    double DriftN[MAX_TIME_COUNT];/*Actually receive smaple number*/
    double AveT_base;/*The AveT in the first minute*/
    double AveN_base;/*The AveN in the first minute*/
    double AveT_current;/*The AveT in the first minute*/
    double AveN_current;/*The AveN in the first minute*/
    UINT32 Minute_count;
}bt_detection_info_t;

/*the info for compentation*/
typedef struct compentation_info_s
{
    double Cvalue[MAX_TIME_COUNT];/*Floor(seletor,SIGN(seletor))*/
}bt_compensation_info_t;

typedef enum
{
    LDAC_COMPENSATION,
    LDAC_DETECTION,
    NONE
} LDAC_PROCEDURE;

extern bt_frame_sample_info_t bt_frame_info;
extern bt_detection_info_t bt_detection_info;
extern bt_compensation_info_t bt_compensation_info;
extern INT16 *SBC_AAC_data_comp;

extern BOOL buffer_control_flag;
extern BOOL bt_detect_init;
extern BOOL bt_compentate_init;
extern BOOL buffer_control_reset_flag;
extern UINT32 buf_count;
extern UINT32 bt_buf_ctl_time;
extern BOOL bt_get_start_time;
extern double bt_buf_receive_sample;

extern BOOL bt_buf_enable_detection;
extern BOOL SBC_AAC_enable_compensation;

#if ENABLE_LDAC_CODEC
typedef enum
{
    BT_REPEAT_REMOVE_IDEL = 0,
    BT_REPEAT_REMOVE_SINGLE_START,
    BT_REPEAT_REMOVE_SINGLE_STOP,
    BT_REPEAT_REMOVE_MULTI
}bt_repeat_remove_status_t;

extern UINT8 *LDAC_packet_buffer;
extern UINT8 *LDAC_data_temp;
extern UINT16 PCMnumber_per_frame;
extern INT32 number_of_frames;
extern UINT32 Fcomp; //  act as a counter,  the number of repeated/removed frames in one minute
#endif

VOID Buffer_control_process(VOID);
VOID *i4TimerHandler(VOID *args);
INT32 Buffer_control_timer_pthread(VOID);
INT32 btaudio_detect_init(VOID);
INT32 btaudio_detect_uninit(VOID);
INT32 btaudio_compentate_init(VOID);
INT32 btaudio_compentate_uninit(VOID);
VOID initMemoryForBufferControl(VOID);
VOID uninitMemoryForBufferControl(VOID);
INT32 btaudio_buffer_control_init(VOID);
INT32 btaudio_buffer_control_uninit(VOID);
VOID btaudio_buffer_control_reset(VOID);
VOID received_accumulated_data_length(BT_AUDIO_CODEC_TYPE codec_type,UINT16 data_len);
INT32 btaudio_detection_process(BT_AUDIO_CODEC_TYPE codec_type);
UINT32 btaudio_compensation_process(BT_AUDIO_CODEC_TYPE codec_type, UINT32 minute, VOID *data_temp, VOID* datalen_temp);
INT32 SBCAAC_compensate_detect(BT_AUDIO_CODEC_TYPE codec_type,INT16 *data,UINT32 *length,LDAC_PROCEDURE ldac_procedure);
#if ENABLE_LDAC_CODEC
INT32 btaudio_analyze_frame_status(UINT8 * data, UINT16 datalen);
INT32 get_LADC_packet_information(UINT8 * data, UINT16 *datalen, UINT16 *frames_length ,UINT8 maximum, UINT8 *frames_number);
INT32 should_packet_be_repeated_removed(UINT16 *frames_length, UINT32 frame_number);
UINT32 btaudio_repeat_remove(UINT8 * data, UINT16 *datalen, UINT8 Type, UINT32 Comp_limit_number);
UINT32 repeat_LDAC_frame(VOID *data_temp, VOID* datalen_temp,INT32 repeat_number);
UINT32 LDAC_compensate(UINT8 *pu1InBuf,UINT32 u4InLen, UINT8 *pu1OutBuf, UINT32 *pu4OutLen);
INT32 LDAC_detect(INT32 frames_number_after_compensated);
#endif/* ENABLE_LDAC_CODEC  */
#endif/* ENABLE_BUFFER_CONTROL */
#endif/* _BT_A2DP_BUFFER_CONTROL_H */


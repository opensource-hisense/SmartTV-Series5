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
/* FILE NAME:  bt_a2dp_alsa_playback.c
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
/* INCLUDE FILE DECLARATIONS
 */
/* NAMING CONSTANT DECLARATIONS
 */
/* MACRO FUNCTION DECLARATIONS
 */
/* DATA TYPE DECLARATIONS
 */
/* GLOBAL VARIABLE DECLARATIONS
 */
/* LOCAL SUBPROGRAM DECLARATIONS
 */
/* STATIC VARIABLE DECLARATIONS
 */
/* EXPORTED SUBPROGRAM BODIES
 */
/* LOCAL SUBPROGRAM BODIES
 */

#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <assert.h>
#include <fcntl.h>
#include <sys/prctl.h>
#include <sys/select.h>
#include <sys/time.h>
#include <dlfcn.h>

#include "u_bt_mw_common.h"
#include "u_bluetooth.h"
#include "bt_a2dp_alsa_playback.h"
#include "c_bt_mw_a2dp_snk.h"
//#include "osi/include/log.h"
#include "bt_mw_log.h"
#include "bt_mw_common.h"
#include <asoundlib.h>

typedef enum bt_audio_status_e
{
    BT_AUDIO_UNINIT = 0,
    BT_AUDIO_OPENED,
    BT_AUDIO_PLAYED,
    BT_AUDIO_PAUSED,
    BT_AUDIO_STOPED,
}bt_audio_status_t;


#define ALSA_DBG_MINOR(s, ...) BT_DBG_MINOR(BT_DEBUG_PB, s, ## __VA_ARGS__)

#define ALSA_DBG_INFO(s, ...) BT_DBG_INFO(BT_DEBUG_PB, s, ## __VA_ARGS__)

#define ALSA_DBG_NOTICE(s, ...) BT_DBG_NOTICE(BT_DEBUG_PB, s, ## __VA_ARGS__)

#define ALSA_DBG_NORMAL(s, ...) BT_DBG_NORMAL(BT_DEBUG_PB, s, ## __VA_ARGS__)

#define ALSA_DBG_WARNING(s, ...) BT_DBG_WARNING(BT_DEBUG_PB, s, ## __VA_ARGS__)

#define ALSA_DBG_ERROR(s, ...) BT_DBG_ERROR(BT_DEBUG_PB, s, ## __VA_ARGS__)

#define BT_ALSA_PLAYBACK_DETACH_IN_THREAD /* do dsp in one thread */

#ifdef BT_ALSA_PLAYBACK_DETACH_IN_THREAD
#include <sys/time.h>
#include <sys/resource.h>
#include <sys/types.h>

//#define BT_ALSA_PLAYBACK_TEST

//#define BT_ALSA_PLAYBACK_DEBUG

#define BT_ALSA_PLAYBACK_BUF_SIZE (4096)
#define BT_ALSA_PLAYBACK_BUF_CNT  (56)  /* it same as btif_media queue size */

#define BT_ALSA_PLAYBACK_CMD_Q_SIZE (10)
#define BT_ALSA_PLAYBACK_DATA_Q_SIZE BT_ALSA_PLAYBACK_BUF_CNT

#define BT_ALSA_PLAYBACK_CMD_OPEN       (0)
#define BT_ALSA_PLAYBACK_CMD_CLOSE      (1)
#define BT_ALSA_PLAYBACK_CMD_PLAY       (2)
#define BT_ALSA_PLAYBACK_CMD_PAUSE      (3)
#define BT_ALSA_PLAYBACK_CMD_DATA       (4)
#define BT_ALSA_PLAYBACK_CMD_QUIT       (5)

typedef struct BT_ALSA_PLAYBACK_MEM_NODE_S
{
    struct BT_ALSA_PLAYBACK_MEM_NODE_S *next;
}BT_ALSA_PLAYBACK_MEM_NODE;

typedef struct
{
    int type;       /* item type */
    union
    {
        struct
        {
            int fs;
            int channels;
        }param;
        struct
        {
#ifdef BT_ALSA_PLAYBACK_DEBUG
            int id;
#endif
            int data_len;   /* data length in p_data */
            unsigned char *data;   /* data buffer */
        }data;
    }u;
}BT_ALSA_PLAYBACK_QUEUE_ITEM;

typedef struct
{
    unsigned int w_pos; /* write position */
    unsigned int r_pos; /* read position */
    unsigned int w_cnt; /* write counter */
    unsigned int r_cnt; /* read counter */
    unsigned int capacity; /* queue size */
    BT_ALSA_PLAYBACK_QUEUE_ITEM *items; /* queue items */
}BT_ALSA_PLAYBACK_QUEUE;

typedef struct
{
    int inited;
    int has_q_data;  /* 0-no data in cmd_q/data_q, 1-has data in cmd_q/data_q */
    pthread_t work_thread; /* work thread, process dsp_open/close/write */
    pthread_mutex_t lock;
    pthread_mutex_t mem_lock; /* memory pool lock */
    pthread_cond_t signal;
    BT_ALSA_PLAYBACK_QUEUE *cmd_q;
    BT_ALSA_PLAYBACK_QUEUE *data_q;
    BT_ALSA_PLAYBACK_MEM_NODE *mem_pool;
    unsigned char *data_mem;
}BT_ALSA_PLAYBACK_CB;

static BT_ALSA_PLAYBACK_CB s_bt_alsa_pb_cb;
static void bt_a2dp_alsa_pb_init(void);
static void bt_a2dp_alsa_pb_deinit(void);
static void bt_a2dp_alsa_pb_enq_cmd(BT_ALSA_PLAYBACK_QUEUE_ITEM *cmd);
static int bt_a2dp_alsa_pb_enq_data(unsigned char *data, int len);

#ifdef BT_ALSA_PLAYBACK_DEBUG
typedef struct
{
    int rx_started;
    int rx_bytes;
    struct timeval rx_start_tm;

    int push_started;
    int push_bytes;
    struct timeval push_start_tm;
}BT_ALSA_PLAYBACK_DBG_CB;

static BT_ALSA_PLAYBACK_DBG_CB s_bt_alsa_pb_dbg_cb;

#endif


#ifdef BT_ALSA_PLAYBACK_TEST
static void bt_a2dp_alsa_pb_test(void);
#endif
#endif


#if defined(MTK_BT_PLAYBACK_DEFAULT_ALSA)
#define ALSA_DEVICE_PLAYER "default" /* for MTK branch tree */
#else
#define ALSA_DEVICE_PLAYER "main"
#endif
#define FRAGMENT_SAMPLES    (4096*4)

static snd_pcm_t *s_alsa_handle = NULL;

static snd_pcm_uframes_t chunk_size = 0;

static UINT32 u4buffer_time = 500000; /* ring buffer length in us */
static UINT32 u4period_time = 1000; /* period time in us */

static snd_pcm_uframes_t period_frames = 0;
static snd_pcm_uframes_t buffer_frames = 0;
static INT32 i4avail_min = -1;
static INT32 i4start_delay = 200000;
static INT32 i4stop_delay = 0;

BT_PLAYBACK_CB_T g_bt_playback_cb = {0};
bt_audio_status_t audio_status = BT_AUDIO_UNINIT;
BOOL fg_audio_reset = FALSE;
BT_PLAYER_SNK_CB_FUNC snk_func = {0};

INT32 bt_a2dp_playback_init(INT32 trackFreq, INT32 channelType);
INT32 bt_a2dp_playback_play(VOID);
INT32 bt_a2dp_playback_suspend(VOID);
INT32 bt_a2dp_playback_uninit(VOID);
INT32 bt_a2dp_playback_push_data(VOID *data, INT32 datalen);

EXPORT_SYMBOL BT_PLAYER_SNK_CB_FUNC* bt_a2dp_playback_getPb(VOID)
{
    snk_func.player_init_cb = bt_a2dp_playback_init;
    snk_func.player_start_cb = bt_a2dp_playback_play;
    snk_func.player_pause_cb = bt_a2dp_playback_suspend;
    snk_func.player_stop_cb = bt_a2dp_playback_suspend;
    snk_func.player_deinit_cb = bt_a2dp_playback_uninit;
    snk_func.player_push_data_cb = bt_a2dp_playback_push_data;
    bt_a2dp_alsa_pb_init();

    return &snk_func;
}


static INT32 set_params(INT32 fs, INT32 channel_num)
{
    ALSA_DBG_NORMAL("+++into");
    INT32 i4ret;
    size_t n;
    UINT32 u4rate;
    snd_pcm_uframes_t start_threshold;
    snd_pcm_uframes_t stop_threshold;
    snd_pcm_uframes_t buffer_size;
    snd_pcm_hw_params_t *hwparams;
    snd_pcm_sw_params_t *swparams;

    snd_pcm_hw_params_alloca(&hwparams);
    snd_pcm_sw_params_alloca(&swparams);

    /* choose all parameters */
    i4ret = snd_pcm_hw_params_any(s_alsa_handle, hwparams);
    if (i4ret < 0)
    {
        ALSA_DBG_ERROR("Broken configuration for playback: no configurations available: %s", snd_strerror(i4ret));
        return i4ret;
    }
    /* set the sample format */
    i4ret = snd_pcm_hw_params_set_format(s_alsa_handle, hwparams, SND_PCM_FORMAT_S16_LE);
    if (i4ret < 0)
    {
        ALSA_DBG_ERROR("Sample format not available for playback: %s", snd_strerror(i4ret));
        return i4ret;
    }
    /* set the interleaved read/write format */
    i4ret = snd_pcm_hw_params_set_access(s_alsa_handle, hwparams, SND_PCM_ACCESS_RW_INTERLEAVED);
    if (i4ret < 0)
    {
        ALSA_DBG_ERROR("Access type not available for playback: %s", snd_strerror(i4ret));
        return i4ret;
    }
    /* set the count of channels */
    i4ret = snd_pcm_hw_params_set_channels(s_alsa_handle, hwparams, channel_num);
    if (i4ret < 0)
    {
        ALSA_DBG_ERROR("Channels count (%i) not available for playbacks: %s", channel_num, snd_strerror(i4ret));
        return i4ret;
    }
    /* set the stream sampling rate */
    i4ret = snd_pcm_hw_params_set_rate(s_alsa_handle, hwparams, fs, 0);
    if (i4ret < 0)
    {
        ALSA_DBG_ERROR("Rate %iHz not available for playback: %s", fs, snd_strerror(i4ret));
        return i4ret;
    }

    u4rate = fs;
    if ((u4buffer_time == 0) && (buffer_frames == 0))
    {
        i4ret = snd_pcm_hw_params_get_buffer_time_max(hwparams, &u4buffer_time, 0);
        if (i4ret < 0)
        {
            ALSA_DBG_ERROR("fail to get max buffer time:%d, %s", i4ret, snd_strerror(i4ret));
            return i4ret;
        }
        ALSA_DBG_NORMAL("u4buffer_time:%d", u4buffer_time);
        if (u4buffer_time > 500000)
        {
            u4buffer_time = 500000;
        }
    }
    if ((u4period_time == 0) && (period_frames == 0))
    {
        if (u4buffer_time > 0)
        {
            u4period_time = u4buffer_time / 4;
        }
        else
        {
            period_frames = buffer_frames / 4;
        }
    }
    if (u4period_time > 0)
    {
        i4ret = snd_pcm_hw_params_set_period_time_near(s_alsa_handle, hwparams,
                &u4period_time, 0);
    }
    else
    {
        i4ret = snd_pcm_hw_params_set_period_size_near(s_alsa_handle, hwparams,
                &period_frames, 0);
    }
    if (i4ret < 0)
    {
        ALSA_DBG_ERROR("fail to get period size:%d, %s", i4ret, snd_strerror(i4ret));
        return i4ret;
    }
    if (u4buffer_time > 0)
    {
        i4ret = snd_pcm_hw_params_set_buffer_time_near(s_alsa_handle, hwparams,
                &u4buffer_time, 0);
    }
    else
    {
        i4ret = snd_pcm_hw_params_set_buffer_size_near(s_alsa_handle, hwparams,
                &buffer_frames);
    }
    if (i4ret < 0)
    {
        ALSA_DBG_ERROR("fail to get buffer size:%d, %s", i4ret, snd_strerror(i4ret));
        return i4ret;
    }

    i4ret = snd_pcm_hw_params(s_alsa_handle, hwparams);
    if (i4ret < 0)
    {
        ALSA_DBG_ERROR("Unable to install hw params");
        return i4ret;
    }

    snd_pcm_hw_params_get_period_size(hwparams, &chunk_size, 0);
    snd_pcm_hw_params_get_buffer_size(hwparams, &buffer_size);
    ALSA_DBG_NORMAL("chunk_size:%lu, buffer_size:%lu", chunk_size, buffer_size);
    if (chunk_size == buffer_size)
    {
        ALSA_DBG_ERROR("Can't use period equal to buffer size (%lu == %lu)",
                       chunk_size, buffer_size);
        return i4ret;
    }

    /* get the current swparams */
    snd_pcm_sw_params_current(s_alsa_handle, swparams);
    if (i4avail_min < 0)
    {
        n = chunk_size;
    }
    else
    {
        n = (double) u4rate * i4avail_min / 1000000;
    }
    i4ret = snd_pcm_sw_params_set_avail_min(s_alsa_handle, swparams, n);

    /* round up to closest transfer boundary */
    n = buffer_size;
    if (i4start_delay <= 0)
    {
        start_threshold = n + (double) u4rate * i4start_delay / 1000000;
    }
    else
    {
        start_threshold = (double) u4rate * i4start_delay / 1000000;
    }
    if (start_threshold < 1)
    {
        start_threshold = 1;
    }
    if (start_threshold > n)
    {
        start_threshold = n;
    }
    /* start the transfer when the buffer is almost full: */
    /* (buffer_size / avail_min) * avail_min */
    i4ret = snd_pcm_sw_params_set_start_threshold(s_alsa_handle, swparams, start_threshold);
    if (i4ret < 0)
    {
        ALSA_DBG_ERROR("fail to set start threshold:%d, %s", i4ret, snd_strerror(i4ret));
        return i4ret;
    }
    if (i4stop_delay <= 0)
    {
        stop_threshold = buffer_size + (double) u4rate * i4stop_delay / 1000000;
    }
    else
    {
        stop_threshold = (double) u4rate * i4stop_delay / 1000000;
    }
    i4ret = snd_pcm_sw_params_set_stop_threshold(s_alsa_handle, swparams, stop_threshold);
    if (i4ret < 0)
    {
        ALSA_DBG_ERROR("fail to set stop threshold:%d, %s", i4ret, snd_strerror(i4ret));
        return i4ret;
    }

    /* write the parameters to the playback device */
    if ((i4ret = snd_pcm_sw_params(s_alsa_handle, swparams)) < 0)
    {
        ALSA_DBG_ERROR("unable to install sw params");
        return i4ret;
    }

    snd_pcm_sw_params_get_start_threshold(swparams, &start_threshold);
    snd_pcm_sw_params_get_stop_threshold(swparams, &stop_threshold);
    ALSA_DBG_NORMAL("start_threshold:%lu, stop_threshold:%lu", start_threshold, stop_threshold);
    //snd_pcm_hw_params_free(hwparams);
    //snd_pcm_sw_params_free(swparams);
    ALSA_DBG_NORMAL("---exit");
    return 0;
}

static INT32 dsp_open(INT32 fs, INT32 channel_num)
{
    ALSA_DBG_NORMAL("+++into");
    INT32 i4ret;

    //snd_pcm_hw_params_t *hwparams;

    if (s_alsa_handle != NULL)
    {
        ALSA_DBG_WARNING("---exit already opened s_alsa_handle");
        return 0;
    }

    i4ret = snd_pcm_open(&s_alsa_handle, ALSA_DEVICE_PLAYER, SND_PCM_STREAM_PLAYBACK, 0 );
    ALSA_DBG_NORMAL("fs %d, channel num %d ", fs, channel_num);
    ALSA_DBG_NORMAL("dsp_open %s i4ret=%d[%s]", ALSA_DEVICE_PLAYER, i4ret, snd_strerror(i4ret));
    if (i4ret < 0)
    {
        ALSA_DBG_ERROR("Cannot open %s ERROR %d[%s]", ALSA_DEVICE_PLAYER, i4ret, snd_strerror(i4ret));
        s_alsa_handle = NULL;
        return  -1;
    }

    set_params(fs, channel_num);

#if 0

    snd_pcm_hw_params_malloc(&hwparams);
    /* choose all parameters */
    i4ret = snd_pcm_hw_params_any(s_alsa_handle, hwparams);
    if (i4ret < 0)
    {
        ALSA_DBG_ERROR("Broken configuration for playback: no configurations available: %s", snd_strerror(i4ret));
        return i4ret;
    }
    /* set the sample format */
    i4ret = snd_pcm_hw_params_set_format(s_alsa_handle, hwparams, SND_PCM_FORMAT_S16_LE);
    if (i4ret < 0)
    {
        ALSA_DBG_ERROR("Sample format not available for playback: %s", snd_strerror(i4ret));
        return i4ret;
    }
    /* set the interleaved read/write format */
    i4ret = snd_pcm_hw_params_set_access(s_alsa_handle, hwparams, SND_PCM_ACCESS_RW_INTERLEAVED);
    if (i4ret < 0)
    {
        ALSA_DBG_ERROR("Access type not available for playback: %s", snd_strerror(i4ret));
        return i4ret;
    }
    /* set the count of channels */
    i4ret = snd_pcm_hw_params_set_channels(s_alsa_handle, hwparams, 2);
    if (i4ret < 0)
    {
        ALSA_DBG_ERROR("Channels count (%i) not available for playbacks: %s", channel_num, snd_strerror(i4ret));
        return i4ret;
    }
    /* set the stream sampling u4rate */
    i4ret = snd_pcm_hw_params_set_rate(s_alsa_handle, hwparams, fs, 0);
    if (i4ret < 0)
    {
        ALSA_DBG_ERROR("Rate %iHz not available for playback: %s", fs, snd_strerror(i4ret));
        return i4ret;
    }
    /* set the buffer time */
    snd_pcm_hw_params_set_buffer_time_near(s_alsa_handle, hwparams, &u4buffer_time, 0);
    if (i4ret < 0)
    {
        ALSA_DBG_ERROR("Rate %iHz not available for playback: %s", fs, snd_strerror(i4ret));
        return i4ret;
    }
    /* set the period time */
    snd_pcm_hw_params_set_period_time_near(s_alsa_handle, hwparams, &u4period_time, 0);
    if (i4ret < 0)
    {
        ALSA_DBG_ERROR("Rate %iHz not available for playback: %s", fs, snd_strerror(i4ret));
        return i4ret;
    }

    //snd_pcm_hw_params_set_buffer_size(s_alsa_handle, hwparams, FRAGMENT_SAMPLES * 4);
    //snd_pcm_hw_params_set_period_size(s_alsa_handle, hwparams, 10, 0);
    snd_pcm_hw_params(s_alsa_handle, hwparams);
    snd_pcm_hw_params_free(hwparams);
#endif

    snd_pcm_prepare(s_alsa_handle);
    audio_status = BT_AUDIO_OPENED;

    ALSA_DBG_NORMAL("---exit");
    return 0;
}

static INT32 dsp_write(UINT8 *buf, UINT32 size)
{
    INT32 i4ret;

    if (s_alsa_handle == NULL)
    {
        ALSA_DBG_ERROR("s_alsa_handle == NULL");
        return -1;
    }

    i4ret = snd_pcm_writei(s_alsa_handle, buf, size/4);
    if (i4ret < 0)
    {
        ALSA_DBG_ERROR("ALSA ERROR %d[%s]", i4ret, snd_strerror(i4ret));
        snd_pcm_prepare(s_alsa_handle);
        if ((i4ret = snd_pcm_prepare(s_alsa_handle))<0)
        {
            ALSA_DBG_ERROR("ALSA snd_pcm_prepare ERROR %d[%s]", i4ret, snd_strerror(i4ret));
        }
    }
    ALSA_DBG_MINOR("alsa write i4ret = %d", i4ret);
    return i4ret;
}

static INT32 dsp_close(VOID)
{
    INT32 i4ret;
    ALSA_DBG_NORMAL("+++into");
    if (s_alsa_handle == NULL)
    {
        ALSA_DBG_ERROR("---exit s_alsa_handle == NULL");
        return -1;
    }
    if (s_alsa_handle != NULL)
    {
        if (0 <= (i4ret = snd_pcm_close(s_alsa_handle)))
        {
            ALSA_DBG_NORMAL("dsp_close success");
        }
        else
        {
            ALSA_DBG_NORMAL("dsp_close fail i4ret=%d[%s]", i4ret, snd_strerror(i4ret));
        }
        s_alsa_handle = NULL;
    }
    audio_status = BT_AUDIO_UNINIT;

    ALSA_DBG_NORMAL("---exit");
    return 0;
}

INT32 bt_a2dp_playback_init(INT32 trackFreq, INT32 channelNum)
{
#ifdef BT_ALSA_PLAYBACK_DETACH_IN_THREAD
    BT_ALSA_PLAYBACK_QUEUE_ITEM cmd = {0};

#ifdef BT_ALSA_PLAYBACK_DEBUG
    struct timeval tv;
    gettimeofday(&tv,NULL);
    ALSA_DBG_NORMAL(" at %lu.%06lu", tv.tv_sec, tv.tv_usec);
#endif

    memset((void*)&cmd, 0, sizeof(cmd));
    cmd.type = BT_ALSA_PLAYBACK_CMD_OPEN;
    cmd.u.param.fs = trackFreq;
    cmd.u.param.channels = channelNum;
    bt_a2dp_alsa_pb_enq_cmd(&cmd);

#ifdef BT_ALSA_PLAYBACK_DEBUG
    memset((void*)&s_bt_alsa_pb_dbg_cb, 0, sizeof(s_bt_alsa_pb_dbg_cb));
#endif

    return BT_SUCCESS;
#else
    INT32 i4_ret = BT_SUCCESS;

    if (FALSE == g_bt_playback_cb.fgPlayBackInit)
    {
        i4_ret = dsp_open(trackFreq, channelNum);
        g_bt_playback_cb.fgPlayBackInit = TRUE;
    }
    else
    {
        ALSA_DBG_WARNING("BT playback have init,no need init again");
    }
    return i4_ret;
#endif
}

INT32 bt_a2dp_playback_uninit(VOID)
{
#ifdef BT_ALSA_PLAYBACK_DETACH_IN_THREAD
    BT_ALSA_PLAYBACK_QUEUE_ITEM cmd = {0};

#ifdef BT_ALSA_PLAYBACK_DEBUG
    struct timeval tv;
    gettimeofday(&tv,NULL);
    ALSA_DBG_NORMAL(" at %lu.%06lu", tv.tv_sec, tv.tv_usec);
#endif

    memset((void*)&cmd, 0, sizeof(cmd));
    cmd.type = BT_ALSA_PLAYBACK_CMD_CLOSE;
    bt_a2dp_alsa_pb_enq_cmd(&cmd);
    return BT_SUCCESS;
#else
    ALSA_DBG_NORMAL("+++into");
    INT32 i4_ret = BT_SUCCESS;

    if (TRUE == g_bt_playback_cb.fgPlayBackInit)
    {
        g_bt_playback_cb.fgPlayBackInit = FALSE;
        i4_ret = dsp_close();
    }
    else
    {
        ALSA_DBG_WARNING("BT playback have not init,no need deinit");
    }

    ALSA_DBG_NORMAL("---exit");
    return i4_ret;
#endif
}

INT32 bluetooth_a2dp_pb_push_data(UINT8 *pPcmBuf, UINT32 u4PcmLen)
{
    if ((NULL == pPcmBuf) || (0 >= u4PcmLen))
    {
        ALSA_DBG_ERROR("invalid data");
        return BT_ERR_STATUS_PARM_INVALID;
    }

    ALSA_DBG_MINOR("send %ld data", (long)u4PcmLen);
    dsp_write(pPcmBuf, u4PcmLen);

    return BT_SUCCESS;
}

INT32 bt_a2dp_playback_play(VOID)
{
#ifdef BT_ALSA_PLAYBACK_DETACH_IN_THREAD
    BT_ALSA_PLAYBACK_QUEUE_ITEM cmd = {0};

#ifdef BT_ALSA_PLAYBACK_DEBUG
    struct timeval tv;
    gettimeofday(&tv,NULL);
    ALSA_DBG_NORMAL(" at %lu.%06lu", tv.tv_sec, tv.tv_usec);
#endif

    memset((void*)&cmd, 0, sizeof(cmd));
    cmd.type = BT_ALSA_PLAYBACK_CMD_PLAY;
    bt_a2dp_alsa_pb_enq_cmd(&cmd);
    return BT_SUCCESS;
#else
    if (FALSE == g_bt_playback_cb.fgPlayBackInit)
    {
        ALSA_DBG_NOTICE("BT playback have not init");
        return BT_SUCCESS;
    }
    if ((audio_status != BT_AUDIO_OPENED)
            && (audio_status != BT_AUDIO_PAUSED)
            && (audio_status != BT_AUDIO_STOPED))
    {
        ALSA_DBG_NORMAL("BT AUDIO wrong status, current status = %d", audio_status);
    }
    else
    {
        audio_status = BT_AUDIO_PLAYED;
        ALSA_DBG_NORMAL("BT AUDIO current status = %d", audio_status);
    }

    return BT_SUCCESS;
#endif
}

INT32 bt_a2dp_playback_suspend(VOID)
{
#ifdef BT_ALSA_PLAYBACK_DETACH_IN_THREAD
    BT_ALSA_PLAYBACK_QUEUE_ITEM cmd = {0};

#ifdef BT_ALSA_PLAYBACK_DEBUG
    struct timeval tv;
    gettimeofday(&tv,NULL);
    ALSA_DBG_NORMAL(" at %lu.%06lu", tv.tv_sec, tv.tv_usec);
#endif

    memset((void*)&cmd, 0, sizeof(cmd));
    cmd.type = BT_ALSA_PLAYBACK_CMD_PAUSE;
    bt_a2dp_alsa_pb_enq_cmd(&cmd);
    return BT_SUCCESS;
#else
    ALSA_DBG_NORMAL("+++into");

    if (FALSE == g_bt_playback_cb.fgPlayBackInit)
    {
        ALSA_DBG_NOTICE("BT playback have not init");
        return BT_SUCCESS;
    }
    if (audio_status != BT_AUDIO_PLAYED)
    {
        ALSA_DBG_NOTICE("BT AUDIO wrong status, current status = %d", audio_status);
    }
    else
    {
        audio_status = BT_AUDIO_PAUSED;
        ALSA_DBG_NORMAL("BT AUDIO current status = %d", audio_status);
    }

    return BT_SUCCESS;
#endif
}

INT32 bt_a2dp_playback_push_data(VOID *data, INT32 datalen)
{
#ifdef BT_ALSA_PLAYBACK_DETACH_IN_THREAD
    return bt_a2dp_alsa_pb_enq_data(data, datalen);
#else
    UINT32 u4PcmLen = datalen;
    UINT8 *pu1PcmBuf = (UINT8 *)data;
    INT32 i4_ret = BT_SUCCESS;

    if (NULL == data || 0 == datalen)
    {
        ALSA_DBG_ERROR("data is null(%p) or data len=%d", data, datalen);
        return BT_ERR_STATUS_FAIL;
    }

    ALSA_DBG_INFO("data=%p, datalen=%u", data, datalen);

    if (FALSE == g_bt_playback_cb.fgPlayBackInit)
    {
        ALSA_DBG_NOTICE("bt playback does not init");
        return i4_ret;
    }

    if (TRUE == g_bt_playback_cb.fgAudioReset)
    {
        ALSA_DBG_NOTICE("audio_reset");
        return i4_ret;
    }

    if (audio_status != BT_AUDIO_PLAYED)
    {
        ALSA_DBG_NOTICE("audio_status is not played %d", audio_status);
        return i4_ret;
    }

    if (BT_SUCCESS != bluetooth_a2dp_pb_push_data(pu1PcmBuf, u4PcmLen))
    {
        ALSA_DBG_ERROR("play pcm failed, pcmbuf:%p, len:%lu", pu1PcmBuf, (long)u4PcmLen);
        return i4_ret;
    }

    return i4_ret;
#endif
}

/******************************************************************************
 */



#ifdef BT_ALSA_PLAYBACK_DETACH_IN_THREAD

static BT_ALSA_PLAYBACK_QUEUE* bt_a2dp_alsa_pb_alloc_q(int capacity)
{
    BT_ALSA_PLAYBACK_QUEUE* q = NULL;
    q = (BT_ALSA_PLAYBACK_QUEUE *)malloc(sizeof(BT_ALSA_PLAYBACK_QUEUE));
    if (NULL == q)
    {
        ALSA_DBG_ERROR("allocate queue fail");
        return NULL;
    }

    q->w_pos = 0;
    q->r_pos = 0;
    q->w_cnt = 0;
    q->r_cnt = 0;
    q->capacity = capacity;
    q->items = (BT_ALSA_PLAYBACK_QUEUE_ITEM *)malloc(sizeof(BT_ALSA_PLAYBACK_QUEUE_ITEM)*capacity);
    if (NULL == q->items)
    {
        free(q);
        ALSA_DBG_ERROR("allocate queue items fail");
        return NULL;
    }

    return q;
}

static void bt_a2dp_alsa_pb_free_q(BT_ALSA_PLAYBACK_QUEUE *q)
{
    if (NULL == q)
    {
        return;
    }

    if (NULL != q->items)
    {
        free(q->items);
        q->items = NULL;
    }

    free(q);

    return;
}

static int bt_a2dp_alsa_pb_enq(BT_ALSA_PLAYBACK_QUEUE *q, BT_ALSA_PLAYBACK_QUEUE_ITEM *items)
{
    if (NULL == q)
    {
        return -1;
    }

    if ((q->w_cnt - q->r_cnt) >= q->capacity)
    {
        ALSA_DBG_ERROR("queue is full, w_cnt=%u, r_cnt=%u, capcity=%u",
            q->w_cnt, q->r_cnt, q->capacity);
        return -1;
    }

    q->items[q->w_pos] = *items;
    q->w_pos++;
    q->w_cnt++;
    if (q->w_pos >= q->capacity)
    {
        q->w_pos = 0;
    }
    return 0;
}

static int bt_a2dp_alsa_pb_deq(BT_ALSA_PLAYBACK_QUEUE *q, BT_ALSA_PLAYBACK_QUEUE_ITEM *items)
{
    if (NULL == q)
    {
        return -1;
    }

    if (q->w_cnt == q->r_cnt)
    {
        //ALSA_DBG_ERROR("queue is empty, w_cnt=%u, r_cnt=%u, capcity=%u",
        //    q->w_cnt, q->r_cnt, q->capacity);
        return -1;
    }

    *items = q->items[q->r_pos];
    q->r_pos++;
    q->r_cnt++;
    if (q->r_pos >= q->capacity)
    {
        q->r_pos = 0;
    }

    return 0;
}

static void* bt_a2dp_alsa_pb_alloc_buf(BT_ALSA_PLAYBACK_MEM_NODE **mem_pool)
{
    BT_ALSA_PLAYBACK_MEM_NODE *node = NULL;
    if ((NULL == mem_pool) || (NULL == *mem_pool))
    {
        return NULL;
    }

    node = *mem_pool;
    *mem_pool = node->next;

    return (void*)node;
}

static void bt_a2dp_alsa_pb_free_buf(BT_ALSA_PLAYBACK_MEM_NODE **mem_pool, void *buf)
{
    BT_ALSA_PLAYBACK_MEM_NODE *node = NULL;
    if ((NULL == mem_pool) || (NULL == buf))
    {
        ALSA_DBG_NORMAL("alloc buf bad parameter: %p, %p", mem_pool, buf);
        return;
    }

    node = (BT_ALSA_PLAYBACK_MEM_NODE *)buf;
    node->next = *mem_pool;
    *mem_pool = node;

    return;
}

static void bt_a2dp_alsa_pb_open(INT32 fs, INT32 channel_num)
{
#ifdef BT_ALSA_PLAYBACK_DEBUG
    struct timeval tv;
    gettimeofday(&tv,NULL);
    ALSA_DBG_NORMAL(" at %lu.%06lu", tv.tv_sec, tv.tv_usec);
#endif
    if (FALSE == g_bt_playback_cb.fgPlayBackInit)
    {
        dsp_open(fs, channel_num);
        g_bt_playback_cb.fgPlayBackInit = TRUE;
    }
    else
    {
        ALSA_DBG_WARNING("BT playback have init,no need init again");
    }
}

static void bt_a2dp_alsa_pb_close(void)
{
#ifdef BT_ALSA_PLAYBACK_DEBUG
    struct timeval tv;
    gettimeofday(&tv,NULL);
    ALSA_DBG_NORMAL(" at %lu.%06lu", tv.tv_sec, tv.tv_usec);
#endif
    if (TRUE == g_bt_playback_cb.fgPlayBackInit)
    {
        dsp_close();
        g_bt_playback_cb.fgPlayBackInit = FALSE;
    }
    else
    {
        ALSA_DBG_WARNING("BT playback have not init,no need deinit");
    }
}

static void bt_a2dp_alsa_pb_play(void)
{
#ifdef BT_ALSA_PLAYBACK_DEBUG
    struct timeval tv;
    gettimeofday(&tv,NULL);
    ALSA_DBG_NORMAL(" at %lu.%06lu", tv.tv_sec, tv.tv_usec);
#endif
    if (FALSE == g_bt_playback_cb.fgPlayBackInit)
    {
        ALSA_DBG_NOTICE("BT playback have not init");
        return;
    }
    if ((audio_status != BT_AUDIO_OPENED)
        && (audio_status != BT_AUDIO_PAUSED)
        && (audio_status != BT_AUDIO_STOPED))
    {
        ALSA_DBG_NORMAL("BT AUDIO wrong status, current status = %d", audio_status);
    }
    else
    {
        if (BT_AUDIO_PLAYED != audio_status)
        {
            ALSA_DBG_NORMAL("BT AUDIO current status = %d", audio_status);
        }
        audio_status = BT_AUDIO_PLAYED;
    }

    return;
}

static void bt_a2dp_alsa_pb_pause(void)
{
#ifdef BT_ALSA_PLAYBACK_DEBUG
    struct timeval tv;
    gettimeofday(&tv,NULL);
    ALSA_DBG_NORMAL(" at %lu.%06lu", tv.tv_sec, tv.tv_usec);
#endif
    if (FALSE == g_bt_playback_cb.fgPlayBackInit)
    {
        ALSA_DBG_NOTICE("BT playback have not init");
        return;
    }

    if (audio_status != BT_AUDIO_PLAYED)
    {
        ALSA_DBG_NOTICE("BT AUDIO wrong status, current status = %d", audio_status);
    }
    else
    {
        audio_status = BT_AUDIO_PAUSED;
        ALSA_DBG_NORMAL("BT AUDIO current status = %d", audio_status);
    }

    return;
}

static void bt_a2dp_alsa_pb_push(void *data, int datalen)
{
    UINT32 u4PcmLen = datalen;
    UINT8 *pu1PcmBuf = (UINT8 *)data;

    if ((NULL == data) || (0 == datalen))
    {
        ALSA_DBG_ERROR("data is null(%p) or data len=%d", data, datalen);
        return;
    }

    ALSA_DBG_INFO("data=%p, datalen=%u", data, datalen);

    if (FALSE == g_bt_playback_cb.fgPlayBackInit)
    {
        ALSA_DBG_NOTICE("bt playback does not init");
        return;
    }

    if (TRUE == g_bt_playback_cb.fgAudioReset)
    {
        ALSA_DBG_NOTICE("audio_reset");
        return;
    }

    if (audio_status != BT_AUDIO_PLAYED)
    {
        ALSA_DBG_NOTICE("audio_status is not played %d", audio_status);
        return;
    }

    if (BT_SUCCESS != bluetooth_a2dp_pb_push_data(pu1PcmBuf, u4PcmLen))
    {
        ALSA_DBG_ERROR("play pcm failed, pcmbuf:%p, len:%lu", pu1PcmBuf, (long)u4PcmLen);
        return;
    }

    return;
}


static void bt_a2dp_alsa_pb_adjust_priority(int start)
{
    int priority = start ? -19 : -16;
    if (setpriority(PRIO_PROCESS, 0, priority) < 0)
    {
        ALSA_DBG_WARNING("failed to change priority to %d", priority);
    }

    return;
}


static void bt_a2dp_alsa_pb_flush_data_q(BT_ALSA_PLAYBACK_MEM_NODE **mem_pool, BT_ALSA_PLAYBACK_QUEUE *q)
{
    BT_ALSA_PLAYBACK_QUEUE_ITEM data_item;
    memset((void*)&data_item, 0, sizeof(data_item));
    while (0 == bt_a2dp_alsa_pb_deq(q, &data_item))
    {
        if (NULL != data_item.u.data.data)
        {
            bt_a2dp_alsa_pb_free_buf(mem_pool, data_item.u.data.data);
        }
    }

    return;
}

static void* bt_a2dp_alsa_pb_handler(void *data)
{
    BT_ALSA_PLAYBACK_CB *pb_cb = (BT_ALSA_PLAYBACK_CB *)data;
    BT_ALSA_PLAYBACK_QUEUE_ITEM cmd_item;
    BT_ALSA_PLAYBACK_QUEUE_ITEM data_item;
#ifdef BT_ALSA_PLAYBACK_DEBUG
    static struct timeval last;
    struct timeval tv, tv1;
#endif
    memset((void*)&cmd_item, 0, sizeof(cmd_item));
    memset((void*)&data_item, 0, sizeof(data_item));

    while (1)
    {
        pthread_mutex_lock(&pb_cb->lock);
        if(!pb_cb->has_q_data)
        {
            pthread_cond_wait(&pb_cb->signal, &pb_cb->lock);
        }
        pb_cb->has_q_data = 0;
        pthread_mutex_unlock(&pb_cb->lock);

        while(1)
        {
            if (0 == bt_a2dp_alsa_pb_deq(pb_cb->cmd_q, &cmd_item))
            {
                if (BT_ALSA_PLAYBACK_CMD_OPEN == cmd_item.type)
                {
                    bt_a2dp_alsa_pb_open(cmd_item.u.param.fs, cmd_item.u.param.channels);
                    bt_a2dp_alsa_pb_adjust_priority(1);
#ifdef BT_ALSA_PLAYBACK_DEBUG
                    gettimeofday(&last, NULL);
#endif
                }
                else if(BT_ALSA_PLAYBACK_CMD_CLOSE == cmd_item.type)
                {
                    bt_a2dp_alsa_pb_close();
                    bt_a2dp_alsa_pb_flush_data_q(&pb_cb->mem_pool, pb_cb->data_q);
                    bt_a2dp_alsa_pb_adjust_priority(0);
                }
                else if(BT_ALSA_PLAYBACK_CMD_PLAY == cmd_item.type)
                {
                    bt_a2dp_alsa_pb_play();
                }
                else if(BT_ALSA_PLAYBACK_CMD_PAUSE == cmd_item.type)
                {
                    bt_a2dp_alsa_pb_pause();
                }
                else if(BT_ALSA_PLAYBACK_CMD_QUIT == cmd_item.type)
                {
                    pthread_exit(NULL);
                }
                else
                {
                    ALSA_DBG_ERROR("invalid cmd: %u", cmd_item.type);
                }
            }

            if (0 == bt_a2dp_alsa_pb_deq(pb_cb->data_q, &data_item))
            {
#ifdef BT_ALSA_PLAYBACK_DEBUG
                unsigned int usec = 0;
                gettimeofday(&tv, NULL);
                if (s_bt_alsa_pb_dbg_cb.push_started == 0)
                {
                    s_bt_alsa_pb_dbg_cb.push_started = 1;
                    s_bt_alsa_pb_dbg_cb.push_start_tm = tv;
                }
                s_bt_alsa_pb_dbg_cb.push_bytes += data_item.u.data.data_len;
                usec = (tv.tv_sec - last.tv_sec) * 1000000 + (long)tv.tv_usec - (long)last.tv_usec;
                if (usec > 100000)
                {
                    ALSA_DBG_ERROR("long time to play: %u us, q_size=%d",
                        usec, pb_cb->data_q->w_cnt - pb_cb->data_q->r_cnt);
                    ALSA_DBG_ERROR("push bytes: %d, time=%d ms",
                        s_bt_alsa_pb_dbg_cb.push_bytes,
                        ((tv.tv_sec - s_bt_alsa_pb_dbg_cb.push_start_tm.tv_sec) * 1000000
                        + (long)tv.tv_usec - (long)s_bt_alsa_pb_dbg_cb.push_start_tm.tv_usec)/1000);
                }
                last = tv;
#endif
                bt_a2dp_alsa_pb_push(data_item.u.data.data, data_item.u.data.data_len);

#ifdef BT_ALSA_PLAYBACK_DEBUG
                gettimeofday(&tv1, NULL);
                usec = (tv1.tv_sec - tv.tv_sec) * 1000000 + (long)tv1.tv_usec - (long)tv.tv_usec;

                if (usec /1000 > 50)
                {
                    ALSA_DBG_ERROR("bt_a2dp_alsa_pb_push[%d] time: %d ms @%lds.%06ldus",
                        data_item.u.data.id, usec/1000, tv1.tv_sec, tv1.tv_usec);
                    ALSA_DBG_ERROR("push bytes: %d, time=%d ms",
                        s_bt_alsa_pb_dbg_cb.push_bytes,
                        ((tv1.tv_sec - s_bt_alsa_pb_dbg_cb.push_start_tm.tv_sec) * 1000000
                        + (long)tv1.tv_usec - (long)s_bt_alsa_pb_dbg_cb.push_start_tm.tv_usec)/1000);

                }
#endif
                pthread_mutex_lock(&pb_cb->mem_lock);
                bt_a2dp_alsa_pb_free_buf(&pb_cb->mem_pool, data_item.u.data.data);
                pthread_mutex_unlock(&pb_cb->mem_lock);
            }
            else
            {
                break;
            }
        }
    }

    return NULL;
}

static void bt_a2dp_alsa_pb_init(void)
{
    int i = 0;
    BT_ALSA_PLAYBACK_MEM_NODE *node = NULL;
    if (1 == s_bt_alsa_pb_cb.inited)
    {
        return;
    }

    pthread_mutex_init(&s_bt_alsa_pb_cb.lock, NULL);
    pthread_mutex_init(&s_bt_alsa_pb_cb.mem_lock, NULL);

    pthread_cond_init(&s_bt_alsa_pb_cb.signal, NULL);

    s_bt_alsa_pb_cb.cmd_q = bt_a2dp_alsa_pb_alloc_q(BT_ALSA_PLAYBACK_CMD_Q_SIZE);
    if (NULL == s_bt_alsa_pb_cb.cmd_q)
    {
        ALSA_DBG_ERROR("alloc cmd q fail");
        goto __ERROR;
    }

    s_bt_alsa_pb_cb.data_q = bt_a2dp_alsa_pb_alloc_q(BT_ALSA_PLAYBACK_DATA_Q_SIZE);
    if (NULL == s_bt_alsa_pb_cb.data_q)
    {
        ALSA_DBG_ERROR("alloc data q fail");
        goto __ERROR;
    }

    s_bt_alsa_pb_cb.data_mem = malloc(BT_ALSA_PLAYBACK_BUF_SIZE*BT_ALSA_PLAYBACK_BUF_CNT);
    if (NULL == s_bt_alsa_pb_cb.data_mem)
    {
        ALSA_DBG_ERROR("alloc data mem pool fail");
        goto __ERROR;
    }

    s_bt_alsa_pb_cb.mem_pool = NULL;
    for (i=0;i<BT_ALSA_PLAYBACK_BUF_CNT;i++)
    {
        node = (BT_ALSA_PLAYBACK_MEM_NODE *)(s_bt_alsa_pb_cb.data_mem + i * BT_ALSA_PLAYBACK_BUF_SIZE);
        node->next = s_bt_alsa_pb_cb.mem_pool;
        s_bt_alsa_pb_cb.mem_pool = node;
    }

    if(0 < pthread_create(&s_bt_alsa_pb_cb.work_thread, NULL,
        bt_a2dp_alsa_pb_handler, (void*)&s_bt_alsa_pb_cb))
    {
        ALSA_DBG_ERROR("create bt alsa thread fail");
        goto __ERROR;
    }
    s_bt_alsa_pb_cb.inited = 1;

    return;
__ERROR:
    if (NULL != s_bt_alsa_pb_cb.data_mem)
    {
        free(s_bt_alsa_pb_cb.data_mem);
        s_bt_alsa_pb_cb.data_mem = NULL;
    }

    if (NULL != s_bt_alsa_pb_cb.data_q)
    {
        bt_a2dp_alsa_pb_free_q(s_bt_alsa_pb_cb.data_q);
        s_bt_alsa_pb_cb.data_q = NULL;
    }

    if (NULL != s_bt_alsa_pb_cb.cmd_q)
    {
        bt_a2dp_alsa_pb_free_q(s_bt_alsa_pb_cb.cmd_q);
        s_bt_alsa_pb_cb.cmd_q = NULL;
    }

    pthread_mutex_destroy(&s_bt_alsa_pb_cb.lock);
    pthread_mutex_destroy(&s_bt_alsa_pb_cb.mem_lock);
    pthread_cond_destroy(&s_bt_alsa_pb_cb.signal);
    return;
}

static void bt_a2dp_alsa_pb_deinit(void)
{
    BT_ALSA_PLAYBACK_QUEUE_ITEM cmd_item = {0};
    if (0 == s_bt_alsa_pb_cb.inited)
    {
        return;
    }
    memset((void*)&cmd_item, 0, sizeof(cmd_item));
    cmd_item.type = BT_ALSA_PLAYBACK_CMD_QUIT;
    bt_a2dp_alsa_pb_enq(s_bt_alsa_pb_cb.cmd_q, &cmd_item);
    pthread_mutex_lock(&s_bt_alsa_pb_cb.lock);
    s_bt_alsa_pb_cb.has_q_data = 1;
    pthread_cond_signal(&s_bt_alsa_pb_cb.signal);
    pthread_mutex_unlock(&s_bt_alsa_pb_cb.lock);

    pthread_join(s_bt_alsa_pb_cb.work_thread, NULL);

    if (NULL != s_bt_alsa_pb_cb.data_mem)
    {
        free(s_bt_alsa_pb_cb.data_mem);
        s_bt_alsa_pb_cb.data_mem = NULL;
    }

    if (NULL != s_bt_alsa_pb_cb.data_q)
    {
        bt_a2dp_alsa_pb_free_q(s_bt_alsa_pb_cb.data_q);
        s_bt_alsa_pb_cb.data_q = NULL;
    }

    if (NULL != s_bt_alsa_pb_cb.cmd_q)
    {
        bt_a2dp_alsa_pb_free_q(s_bt_alsa_pb_cb.cmd_q);
        s_bt_alsa_pb_cb.cmd_q = NULL;
    }

    pthread_mutex_destroy(&s_bt_alsa_pb_cb.lock);
    pthread_mutex_destroy(&s_bt_alsa_pb_cb.mem_lock);
    pthread_cond_destroy(&s_bt_alsa_pb_cb.signal);

    s_bt_alsa_pb_cb.inited = 0;
    return;
}

static void bt_a2dp_alsa_pb_enq_cmd(BT_ALSA_PLAYBACK_QUEUE_ITEM *cmd)
{
    if (NULL == cmd)
    {
        return;
    }

    bt_a2dp_alsa_pb_enq(s_bt_alsa_pb_cb.cmd_q, cmd);

    pthread_mutex_lock(&s_bt_alsa_pb_cb.lock);
    s_bt_alsa_pb_cb.has_q_data = 1;
    pthread_cond_signal(&s_bt_alsa_pb_cb.signal);
    pthread_mutex_unlock(&s_bt_alsa_pb_cb.lock);

    return;
}

static int bt_a2dp_alsa_pb_enq_data(unsigned char *data, int len)
{
    BT_ALSA_PLAYBACK_QUEUE_ITEM data_item = {0};
#ifdef BT_ALSA_PLAYBACK_DEBUG
    struct timeval tv;
    static int seq_id = 0;
#endif
    if ((NULL == data) || (0 == len))
    {
        return BT_ERR_STATUS_PARM_INVALID;
    }
    memset((void*)&data_item, 0, sizeof(data_item));

#ifdef BT_ALSA_PLAYBACK_DEBUG
    gettimeofday(&tv,NULL);
    if (s_bt_alsa_pb_dbg_cb.rx_started == 0)
    {
        s_bt_alsa_pb_dbg_cb.rx_started = 1;
        s_bt_alsa_pb_dbg_cb.rx_start_tm = tv;
    }
    s_bt_alsa_pb_dbg_cb.rx_bytes += len;
#endif

    data_item.type = BT_ALSA_PLAYBACK_CMD_DATA;
    if (BT_ALSA_PLAYBACK_BUF_SIZE < len)
    {
        data_item.u.data.data_len = BT_ALSA_PLAYBACK_BUF_SIZE;
        ALSA_DBG_ERROR("data len is larger than %d", len);
    }
    else
    {
        data_item.u.data.data_len = len;
    }

    pthread_mutex_lock(&s_bt_alsa_pb_cb.mem_lock);
    data_item.u.data.data = bt_a2dp_alsa_pb_alloc_buf(&s_bt_alsa_pb_cb.mem_pool);
    pthread_mutex_unlock(&s_bt_alsa_pb_cb.mem_lock);
    if (NULL == data_item.u.data.data)
    {
        ALSA_DBG_ERROR("no data buffer, data q size: %d",
            s_bt_alsa_pb_cb.data_q->w_cnt-s_bt_alsa_pb_cb.data_q->r_cnt);
#ifdef BT_ALSA_PLAYBACK_DEBUG
        ALSA_DBG_ERROR("rx bytes: %d, rx time: %u ms\npush bytes: %d, push time: %u ms",
            s_bt_alsa_pb_dbg_cb.rx_bytes,
            ((tv.tv_sec - s_bt_alsa_pb_dbg_cb.rx_start_tm.tv_sec) * 1000000
            + (long)tv.tv_usec - (long)s_bt_alsa_pb_dbg_cb.rx_start_tm.tv_usec)/1000,
            s_bt_alsa_pb_dbg_cb.push_bytes,
            ((tv.tv_sec - s_bt_alsa_pb_dbg_cb.push_start_tm.tv_sec) * 1000000
            + (long)tv.tv_usec - (long)s_bt_alsa_pb_dbg_cb.push_start_tm.tv_usec)/1000);
#endif
        return BT_ERR_STATUS_NOMEM;
    }

    memcpy(data_item.u.data.data, data, data_item.u.data.data_len);

#ifdef BT_ALSA_PLAYBACK_DEBUG
    data_item.u.data.id = seq_id++;
    gettimeofday(&tv,NULL);
    ALSA_DBG_INFO("bt_a2dp_alsa_pb_enq_data[%d]  @%lds.%06ldus",
        data_item.u.data.id, tv.tv_sec, tv.tv_usec);
#endif

    if (0 > bt_a2dp_alsa_pb_enq(s_bt_alsa_pb_cb.data_q, &data_item))
    {
        ALSA_DBG_ERROR("enq fail, data q size: %d",
            s_bt_alsa_pb_cb.data_q->w_cnt-s_bt_alsa_pb_cb.data_q->r_cnt);
        pthread_mutex_lock(&s_bt_alsa_pb_cb.mem_lock);
        bt_a2dp_alsa_pb_free_buf(&s_bt_alsa_pb_cb.mem_pool, data_item.u.data.data);
        pthread_mutex_unlock(&s_bt_alsa_pb_cb.mem_lock);
        return BT_ERR_STATUS_BUSY;
    }

#ifdef BT_ALSA_PLAYBACK_DEBUG
    if ((s_bt_alsa_pb_cb.data_q->capacity * 7 / 8)
        < (s_bt_alsa_pb_cb.data_q->w_cnt - s_bt_alsa_pb_cb.data_q->r_cnt))
    {
        ALSA_DBG_ERROR("almost full, data q size: %d",
            s_bt_alsa_pb_cb.data_q->w_cnt-s_bt_alsa_pb_cb.data_q->r_cnt);
    }
#endif

    pthread_mutex_lock(&s_bt_alsa_pb_cb.lock);
    s_bt_alsa_pb_cb.has_q_data = 1;
    pthread_cond_signal(&s_bt_alsa_pb_cb.signal);
    pthread_mutex_unlock(&s_bt_alsa_pb_cb.lock);

    return BT_SUCCESS;
}

#ifdef BT_ALSA_PLAYBACK_TEST
static void* bt_a2dp_alsa_pb_test_push_handler(void *data)
{
    unsigned char *buf = NULL;
    ALSA_DBG_NORMAL("start");
    sleep(10);
    buf = (unsigned char*)malloc(4800);
    if (NULL == buf)
    {
        ALSA_DBG_ERROR("alloc test buffer fail");
        return NULL;
    }
    bt_a2dp_alsa_pb_adjust_priority(1);

    while (1)
    {
        bt_a2dp_playback_push_data(buf, 4800);
        usleep(random()%5000+20000);
    }

    return NULL;
}

static void* bt_a2dp_alsa_pb_test_open_close_handler(void *data)
{
    ALSA_DBG_NORMAL("start");
    sleep(10);
    while (1)
    {
        bt_a2dp_playback_init(48000, 2);
        usleep(random() % 1000000 + 100000);
        bt_a2dp_playback_uninit();
    }
    return NULL;
}

static void bt_a2dp_alsa_pb_test(void)
{
    pthread_t t1, t2, t3;
    ALSA_DBG_NORMAL("start");

    pthread_create(&t1, NULL, bt_a2dp_alsa_pb_test_open_close_handler, NULL);
    pthread_create(&t2, NULL, bt_a2dp_alsa_pb_test_push_handler, NULL);
    pthread_create(&t3, NULL, bt_a2dp_alsa_pb_test_open_close_handler, NULL);

    return;
}
#endif

#endif
/******************************************************************************
 */


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
/* FILE NAME:  bt_a2dp_playback.c
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

#include "bluetooth.h"
#include "bt_av.h"

#include "bt_mw_common.h"

#include "bt_a2dp_alsa_playback.h"
#include "bt_mw_a2dp_snk.h"
//#include "sbc_dec_exp.h"
//#include "bt_a2dp_sbc_dec.h"
#if ENABLE_AAC_CODEC
//#include "bt_a2dp_aac_dec.h"
#endif
#if ENABLE_LDAC_CODEC
//#include "bt_a2dp_ldac.h"
#endif

#include "IMtkPb_Ctrl.h"
#if ENABLE_MTK_TV_PLATFORM
#include "x_os.h"
#include "dbg/x_dbg.h"
#include "inc/x_common.h"
#else
#include "IMtkPb_Sys.h"
#endif
#if ENABLE_BUFFER_CONTROL
#include "bt_a2dp_buffer_control.h"
#endif
#if ENABLE_INTERNAL_SUSPEND
#include "bt_a2dp_internal_suspend.h"


extern BOOL fg_is_inter_suspend;
extern BOOL fg_bt_is_pb_pause;
extern VOID bt_in_internal_timer_cbk(HANDLE_T pt_handle, VOID* pv_tag);
#endif

BT_PLAYBACK_CB_T g_bt_playback_cb = {0};

BOOL fg_playback_init = FALSE;
HANDLE_T  h_bt_playback_mutex = NULL_HANDLE;

static IMTK_PB_HANDLE_T  h_handle = IMTK_NULL_HANDLE;
bt_dec_audio_dump_t* dec_audio_dump_conf = NULL;

BT_A2DP_SNK_CB_T g_a2dp_snk_cb;
IMTK_PB_CTRL_SET_MEDIA_INFO_T g_t_media_info = {0};
bt_audio_status_t audio_status = BT_AUDIO_UNINIT;

BOOL fg_audio_reset = FALSE;
static UINT32 g_u4_BufferSize;
static BOOL g_fgBufferReady = TRUE;
#if DISCARD_DATA_BEFORE_PLAY
//BOOL g_fg_enable_discard_data = FALSE;
BOOL g_fgdiscard_data = TRUE;
#endif
INT32 g_remained_buf_size = 0;
extern INT32 optimise_latency;//for use optimise latency CLI
VOID bluetooth_a2dp_sink_prepareDump(const BT_AUDIO_CODEC_TYPE audio_codec_type);
VOID bluetooth_a2dp_sink_finishDump(VOID);

void linuxbt_playback_init(int trackFreq, int channelType);
void linuxbt_playback_deinit(void);
void linuxbt_playback_play(void);
void linuxbt_playback_pause(void);
int linuxbt_playback_send_data(void *audioBuffer, int bufferlen);


BtifAvrcpAudioTrack g_t_track_cb =
{
    sizeof(BtifAvrcpAudioTrack),
    linuxbt_playback_init,
    linuxbt_playback_deinit,
    linuxbt_playback_play,
    linuxbt_playback_pause,
    linuxbt_playback_send_data,
};

INT32 bluetooth_a2dp_sink_player_init(VOID)
{
    FUNC_ENTRY;
    return BtifAvrcpAudioTrackInit(&g_t_track_cb);
}

INT32 bluetooth_a2dp_sink_player_deinit(VOID)
{
    FUNC_ENTRY;
    return BtifAvrcpAudioTrackDeinit();
}

static INT32 set_media_info(INT32 trackFreq, INT32 channelType)
{
    FUNC_ENTRY;

    //Defalut value
    g_t_media_info.eMediaType = IMTK_PB_CTRL_MEDIA_TYPE_AUDIO_ES;
    g_t_media_info.uFormatInfo.tAudioEsInfo.tAudInfo.eAudEnc = IMTK_PB_CTRL_AUD_ENC_PCM;
    g_t_media_info.uFormatInfo.tAudioEsInfo.tAudInfo.uAudCodecInfo.t_pcm_info.e_pcm_type = IMTK_PB_CTRL_AUD_PCM_TYPE_NORMAL;
    g_t_media_info.uFormatInfo.tAudioEsInfo.tAudInfo.uAudCodecInfo.t_pcm_info.eNumCh = IMTK_PB_CTRL_AUD_CH_STEREO;
    g_t_media_info.uFormatInfo.tAudioEsInfo.tAudInfo.uAudCodecInfo.t_pcm_info.eSampleRate = IMTK_PB_CTRL_AUD_SAMPLE_RATE_44K;
    g_t_media_info.uFormatInfo.tAudioEsInfo.tAudInfo.uAudCodecInfo.t_pcm_info.u2BlockAlign = 4;
    g_t_media_info.uFormatInfo.tAudioEsInfo.tAudInfo.uAudCodecInfo.t_pcm_info.eBitsPerSample = IMTK_PB_CTRL_AUD_PCM_BIT_DEPTH_16;
    g_t_media_info.uFormatInfo.tAudioEsInfo.tAudInfo.uAudCodecInfo.t_pcm_info.fgBigEndian = 0;


    switch (trackFreq)
    {
    case 48000:
    {
        g_t_media_info.uFormatInfo.tAudioEsInfo.tAudInfo.uAudCodecInfo.t_pcm_info.eSampleRate = IMTK_PB_CTRL_AUD_SAMPLE_RATE_48K;
        break;
    }
    case 32000:
    {
        g_t_media_info.uFormatInfo.tAudioEsInfo.tAudInfo.uAudCodecInfo.t_pcm_info.eSampleRate = IMTK_PB_CTRL_AUD_SAMPLE_RATE_32K;
        break;
    }
    case 16000:
    {
        g_t_media_info.uFormatInfo.tAudioEsInfo.tAudInfo.uAudCodecInfo.t_pcm_info.eSampleRate = IMTK_PB_CTRL_AUD_SAMPLE_RATE_16K;
        break;
    }
    case 44100:
    {
        g_t_media_info.uFormatInfo.tAudioEsInfo.tAudInfo.uAudCodecInfo.t_pcm_info.eSampleRate = IMTK_PB_CTRL_AUD_SAMPLE_RATE_44K;
        break;
    }
    default:
    {
        BT_DBG_NORMAL(BT_DEBUG_A2DP,"sampling rate not support --> %d  ", trackFreq);
        break;
    }
    }

    if (channelType == 1)
    {
        g_t_media_info.uFormatInfo.tAudioEsInfo.tAudInfo.uAudCodecInfo.t_pcm_info.eNumCh = IMTK_PB_CTRL_AUD_CH_MONO;
    }
    else
    {
        g_t_media_info.uFormatInfo.tAudioEsInfo.tAudInfo.uAudCodecInfo.t_pcm_info.eNumCh = IMTK_PB_CTRL_AUD_CH_STEREO;
    }

    return BT_SUCCESS;
}

static IMTK_PB_CB_ERROR_CODE_T   pb_cbk (
    IMTK_PB_CTRL_EVENT_T eEventType,
    VOID* pvAppCbTag,
    uint32_t ui4Data)
{
    BT_DBG_MINOR(BT_DEBUG_A2DP,"Enter into--->%s", __FUNCTION__);

    switch (eEventType)
    {
    case IMTK_PB_CTRL_EVENT_GET_BUF_READY:
    {
        BT_DBG_MINOR(BT_DEBUG_A2DP,"received get buffer ready callback!");
        g_fgBufferReady = TRUE;
        break;
    }

    case IMTK_PB_CTRL_EVENT_EOS:
    {
        BT_DBG_MINOR(BT_DEBUG_A2DP,"received EOS callback!");
        g_fgBufferReady = TRUE;
        break;
    }

    case IMTK_PB_CTRL_EVENT_BUFFER_UNDERFLOW:
    {
        BT_DBG_MINOR(BT_DEBUG_A2DP,"IMTK_PB_CTRL_EVENT_BUFFER_UNDERFLOW!");
        break;
    }

    case IMTK_PB_CTRL_EVENT_BUFFER_LOW:
    {
        BT_DBG_MINOR(BT_DEBUG_A2DP,"IMTK_PB_CTRL_EVENT_BUFFER_LOW!");
        break;
    }

    case IMTK_PB_CTRL_EVENT_BUFFER_HIGH:
    {
        BT_DBG_MINOR(BT_DEBUG_A2DP,"IMTK_PB_CTRL_EVENT_BUFFER_HIGH!");
        break;
    }
    default:
    {
        break;
    }
    }

    return IMTK_PB_ERROR_CODE_OK;
}

static INT32 bluetooth_a2dp_pb_conf(INT32 trackFreq, INT32 channelType)
{
    BT_DBG_INFO(BT_DEBUG_A2DP,"Enter into--->%s", __FUNCTION__);

    IMTK_PB_ERROR_CODE_T i4_ret = 0;
    IMTK_PB_CTRL_ENGINE_PARAM_T t_engine_param = {0};
    //int alloc_method;
    //int subband_num = 8;
    //int block_len = 16;
    //current_config->codec_type = BT_A2DP_SBC;
    set_media_info(trackFreq, channelType);


    i4_ret = IMtkPb_Ctrl_Open(&h_handle,IMTK_PB_CTRL_BUFFERING_MODEL_PUSH,IMTK_PB_CTRL_APP_MASTER, NULL);
    if (i4_ret != IMTK_PB_ERROR_CODE_OK)
    {
        BT_DBG_NORMAL(BT_DEBUG_A2DP,"open engine fail %d", i4_ret);
        if (NULL_HANDLE != h_bt_playback_mutex)
        {
            x_sema_unlock(h_bt_playback_mutex);
        }
        return BT_ERR_STATUS_FAIL;
    }
    audio_status = BT_AUDIO_OPENED;
    BT_DBG_NORMAL(BT_DEBUG_A2DP,"BT AUDIO change status = %d", audio_status);


    t_engine_param.u4PlayFlag = IMTK_PB_CTRL_PLAY_FLAG_AUDIO;

    i4_ret = IMtkPb_Ctrl_SetEngineParam(h_handle, &t_engine_param);
    if (i4_ret != IMTK_PB_ERROR_CODE_OK)
    {
        BT_DBG_NORMAL(BT_DEBUG_A2DP,"failed to set engine parameter i4_ret %d ", i4_ret);
        if (NULL_HANDLE != h_bt_playback_mutex)
        {
            x_sema_unlock(h_bt_playback_mutex);
        }
        return BT_ERR_STATUS_FAIL;
    }
    else
    {
        BT_DBG_NORMAL(BT_DEBUG_A2DP,"set engine parameter test successful! ");
    }
#if !ENABLE_MTK_TV_PLATFORM
    if (1 == optimise_latency)
    {
        BT_DBG_NORMAL(BT_DEBUG_A2DP,"Optimise latency");
        IMTK_PB_CTRL_SET_EXTRA_INFO_T tExtraInfo;
        tExtraInfo.eExtraInfoType = IMTK_PB_CTRL_EXTRA_INFO_TYPE_AUDIO_ONLY_FAST_OUTPUT;
        i4_ret = IMtkPb_Ctrl_SetExtraInfo(h_handle, &tExtraInfo);
        if (i4_ret != IMTK_PB_ERROR_CODE_OK)
        {
            BT_DBG_NORMAL(BT_DEBUG_A2DP,"failed to set SetExtraInfo i4_ret %d",i4_ret);
            if (NULL_HANDLE != h_bt_playback_mutex)
            {
                x_sema_unlock(h_bt_playback_mutex);
            }
            return BT_ERR_STATUS_FAIL;
        }
        else
        {
            BT_DBG_NORMAL(BT_DEBUG_A2DP,"set SetExtraInfo successful! ");
        }
    }
#endif
    i4_ret = IMtkPb_Ctrl_SetMediaInfo(h_handle, &g_t_media_info);
    if (i4_ret != IMTK_PB_ERROR_CODE_OK)
    {
        BT_DBG_NORMAL(BT_DEBUG_A2DP,"failed to set media info i4_ret %d ", i4_ret);
        if (NULL_HANDLE != h_bt_playback_mutex)
        {
            x_sema_unlock(h_bt_playback_mutex);
        }
        return BT_ERR_STATUS_FAIL;
    }
    else
    {
        BT_DBG_NORMAL(BT_DEBUG_A2DP,"set meida info test successful! ");
    }

    i4_ret = IMtkPb_Ctrl_RegCallback(h_handle, NULL, pb_cbk);
    if (i4_ret != IMTK_PB_ERROR_CODE_OK)
    {
        BT_DBG_NORMAL(BT_DEBUG_A2DP,"auto test failed to register callback return i4_ret %d", i4_ret);
        if (NULL_HANDLE != h_bt_playback_mutex)
        {
            x_sema_unlock(h_bt_playback_mutex);
        }
        return BT_ERR_STATUS_FAIL;
    }
    else
    {
        BT_DBG_NORMAL(BT_DEBUG_A2DP,"register callback test successful! ");
    }
    if (dec_audio_dump_conf && dec_audio_dump_conf->dec_dump_enable)
    {
        bluetooth_a2dp_sink_prepareDump(bluetooth_a2dp_getCurrentCodecType());
    }
    return BT_SUCCESS;
}

VOID bluetooth_a2dp_reconfig_pb_conf(void)
{
    FUNC_ENTRY;
    IMTK_PB_ERROR_CODE_T i4_ret = 0;
    IMTK_PB_CTRL_ENGINE_PARAM_T t_engine_param = {0};
    /*
    memcpy(&bt_current_config,
            current_config,
            sizeof(bt_current_config)
            );
    */


    if (NULL_HANDLE != h_bt_playback_mutex)
    {
        x_sema_lock(h_bt_playback_mutex, X_SEMA_OPTION_WAIT);
    }
    if (TRUE == g_bt_playback_cb.fgPlayBackInit)
    {
        if (audio_status >= BT_AUDIO_PLAYED
                && audio_status != BT_AUDIO_STOPED)
        {
            /*avoid double stop*/
            if (audio_status == BT_AUDIO_STOPED)
            {
                BT_DBG_NORMAL(BT_DEBUG_A2DP,"BT AUDIO change status = %d", audio_status);
            }
            else
            {
                i4_ret = IMtkPb_Ctrl_Stop(h_handle);
                if (i4_ret != IMTK_PB_ERROR_CODE_OK)
                {
                    BT_DBG_NORMAL(BT_DEBUG_A2DP,"stop playback fail %d", i4_ret);
                    goto PB_RECONFIG_END;
                }
            }
        }
        i4_ret = IMtkPb_Ctrl_SetMediaInfo(h_handle, NULL);
        if (i4_ret != IMTK_PB_ERROR_CODE_OK)
        {
            BT_DBG_NORMAL(BT_DEBUG_A2DP,"unset mediainfo fail %d", i4_ret);
            goto PB_RECONFIG_END;
        }

        i4_ret = IMtkPb_Ctrl_Close(h_handle);
        if (i4_ret != IMTK_PB_ERROR_CODE_OK)
        {
            BT_DBG_NORMAL(BT_DEBUG_A2DP,"close playback fail %d", i4_ret);
            goto PB_RECONFIG_END;
        }

        BT_DBG_ERROR(BT_DEBUG_A2DP,"CMIF close playback success %d", i4_ret);

        audio_status = BT_AUDIO_UNINIT;
        BT_DBG_NORMAL(BT_DEBUG_A2DP,"BT AUDIO change status = %d", audio_status);
        h_handle = IMTK_NULL_HANDLE;
    }
    else
    {
        BT_DBG_NORMAL(BT_DEBUG_A2DP,"playback not init, no need do pb_close");
    }

    if (TRUE == g_bt_playback_cb.fgPlayBackInit)
    {
        i4_ret = IMtkPb_Ctrl_Open(&h_handle,IMTK_PB_CTRL_BUFFERING_MODEL_PUSH,IMTK_PB_CTRL_APP_MASTER, NULL);
        if (i4_ret != IMTK_PB_ERROR_CODE_OK)
        {
            BT_DBG_NORMAL(BT_DEBUG_A2DP,"open engine fail %d", i4_ret);
            goto PB_RECONFIG_END;
        }
        audio_status = BT_AUDIO_OPENED;
        BT_DBG_NORMAL(BT_DEBUG_A2DP,"BT AUDIO change status = %d", audio_status);

        //bluetooth_a2dp_get_media_info(&g_t_media_info/*, pt_codec_info*/);

        t_engine_param.u4PlayFlag = IMTK_PB_CTRL_PLAY_FLAG_AUDIO;

        i4_ret = IMtkPb_Ctrl_SetEngineParam(h_handle, &t_engine_param);
        if (i4_ret != IMTK_PB_ERROR_CODE_OK)
        {
            BT_DBG_NORMAL(BT_DEBUG_A2DP,"failed to set engine parameter i4_ret %d ", i4_ret);
            goto PB_RECONFIG_END;
        }
        else
        {
            BT_DBG_NORMAL(BT_DEBUG_A2DP,"set engine parameter test successful! ");
        }

        if (1 == optimise_latency)
        {
            BT_DBG_NORMAL(BT_DEBUG_A2DP,"Optimise latency");
            IMTK_PB_CTRL_SET_EXTRA_INFO_T tExtraInfo;
            tExtraInfo.eExtraInfoType = IMTK_PB_CTRL_EXTRA_INFO_TYPE_AUDIO_ONLY_FAST_OUTPUT;
            i4_ret = IMtkPb_Ctrl_SetExtraInfo(h_handle, &tExtraInfo);
            if (i4_ret != IMTK_PB_ERROR_CODE_OK)
            {
                BT_DBG_NORMAL(BT_DEBUG_A2DP,"failed to set SetExtraInfo i4_ret %d",i4_ret);
                goto PB_RECONFIG_END;
            }
            else
            {
                BT_DBG_NORMAL(BT_DEBUG_A2DP,"set SetExtraInfo successful! ");
            }
        }

        i4_ret = IMtkPb_Ctrl_SetMediaInfo(h_handle, &g_t_media_info);
        if (i4_ret != IMTK_PB_ERROR_CODE_OK)
        {
            BT_DBG_NORMAL(BT_DEBUG_A2DP,"failed to set media info i4_ret %d ", i4_ret);
            goto PB_RECONFIG_END;
        }
        else
        {
            BT_DBG_NORMAL(BT_DEBUG_A2DP,"set meida info test successful! ");
        }

        i4_ret = IMtkPb_Ctrl_RegCallback(h_handle, NULL, pb_cbk);
        if (i4_ret != IMTK_PB_ERROR_CODE_OK)
        {
            BT_DBG_NORMAL(BT_DEBUG_A2DP,"auto test failed to register callback return i4_ret %d", i4_ret);
            goto PB_RECONFIG_END;
        }
        else
        {
            BT_DBG_NORMAL(BT_DEBUG_A2DP,"register callback test successful! ");
        }
    }
    else
    {
        BT_DBG_NORMAL(BT_DEBUG_A2DP,"playback not init, no need restart playback");
    }

PB_RECONFIG_END:
    if (NULL_HANDLE != h_bt_playback_mutex)
    {
        x_sema_unlock(h_bt_playback_mutex);
    }
}


VOID bluetooth_a2dp_snk_init(VOID)
{
    memset(&g_a2dp_snk_cb, 0x0, sizeof(g_a2dp_snk_cb));
}

INT32 bluetooth_playback_init(INT32 trackFreq, INT32 channelType)
{
    FUNC_ENTRY;

    INT32 i4_ret = BT_SUCCESS;

    if (FALSE == g_bt_playback_cb.fgPlayBackInit)
    {

#if ENABLE_INTERNAL_SUSPEND
        bt_internal_suspend_init();
#endif
#if DISCARD_DATA_BEFORE_PLAY
        //g_fgdiscard_data = TRUE;
        g_bt_playback_cb.fgInterDiscard = TRUE;
#endif
        if (NULL_HANDLE == h_bt_playback_mutex)
        {
            x_sema_create(&h_bt_playback_mutex,
                          X_SEMA_TYPE_MUTEX,
                          X_SEMA_STATE_UNLOCK);
        }
        if (NULL_HANDLE != h_bt_playback_mutex)
        {
            x_sema_lock(h_bt_playback_mutex, X_SEMA_OPTION_WAIT);
        }
        i4_ret = bluetooth_a2dp_pb_conf(trackFreq, channelType);
        if (0 != i4_ret)
        {
            if (NULL_HANDLE != h_bt_playback_mutex)
            {
                x_sema_unlock(h_bt_playback_mutex);
            }
            g_bt_playback_cb.fgPlayBackInit = FALSE;
            return BT_ERR_STATUS_FAIL;
        }
        g_bt_playback_cb.fgPlayBackInit = TRUE;
        //g_remained_buf_size = 0;
        bluetooth_a2dp_snk_init();

        if (TRUE == g_bt_playback_cb.fgPlayBackInit)
        {

#if DISCARD_DATA_BEFORE_PLAY
            if (0)
            {
                g_bt_playback_cb.fgInterDiscard = TRUE;
            }
            else
            {
                g_bt_playback_cb.fgInterDiscard = FALSE;
            }
#endif/*DISCARD_DATA_BEFORE_PLAY*/
        }
        else
        {
            BT_DBG_NORMAL(BT_DEBUG_A2DP,"BT playback have not init,no need start internal suspend timer");
        }
        if (NULL_HANDLE != h_bt_playback_mutex)
        {
            x_sema_unlock(h_bt_playback_mutex);
        }
        if (get_bt_stream_suspend())
        {
            //bluetooth_a2dp_pb_suspend();

        }
        else
        {

#if ENABLE_INTERNAL_SUSPEND
            /* into internal suspend mode */
            fg_is_inter_suspend = TRUE;
#else
            bluetooth_a2dp_pb_restart();
#endif/* ENABLE_INTERNAL_SUSPEND */
        }
    }
    else
    {
        BT_DBG_NORMAL(BT_DEBUG_A2DP,"BT playback have init,no need init again");
    }
    FUNC_EXIT;
    return 0;
}

INT32 bluetooth_playback_uninit(VOID)
{
    FUNC_ENTRY;

    IMTK_PB_ERROR_CODE_T i4_ret = 0;
    /*check playback init or not*/
    if (TRUE == g_bt_playback_cb.fgPlayBackInit)
    {
        if (NULL_HANDLE != h_bt_playback_mutex)
        {
            x_sema_lock(h_bt_playback_mutex, X_SEMA_OPTION_WAIT);
        }
        /*check playback status is play or pause*/
        if (audio_status >= BT_AUDIO_PLAYED
                && audio_status != BT_AUDIO_STOPED)
        {
            /*avoid double stop*/
            if (audio_status == BT_AUDIO_STOPED)
            {
                BT_DBG_NORMAL(BT_DEBUG_A2DP,"BT AUDIO change status = %d", audio_status);
            }
            else
            {
                /*avoid quick init playback and uninit playback*/
                if (FALSE == g_bt_playback_cb.fgPlayBackInit)
                {
                    BT_DBG_ERROR(BT_DEBUG_A2DP,"BT playback have uninit");
                    if (NULL_HANDLE != h_bt_playback_mutex)
                    {
                        x_sema_unlock(h_bt_playback_mutex);
                    }
                    return 0;
                }
                i4_ret = IMtkPb_Ctrl_Stop(h_handle);
                if (i4_ret != IMTK_PB_ERROR_CODE_OK)
                {
                    BT_DBG_NORMAL(BT_DEBUG_A2DP,"stop playback fail %d", i4_ret);
                    if (NULL_HANDLE != h_bt_playback_mutex)
                    {
                        x_sema_unlock(h_bt_playback_mutex);
                    }
                    goto FINISH;
                }
                audio_status = BT_AUDIO_STOPED;
            }
        }
        /*avoid quick init playback and uninit playback*/
        if (FALSE == g_bt_playback_cb.fgPlayBackInit)
        {
            BT_DBG_ERROR(BT_DEBUG_A2DP,"BT playback have uninit");
            if (NULL_HANDLE != h_bt_playback_mutex)
            {
                x_sema_unlock(h_bt_playback_mutex);
            }
            return 0;
        }
#if !ENABLE_MTK_TV_PLATFORM
        i4_ret = IMtkPb_Ctrl_SetMediaInfo(h_handle, NULL);
        if (i4_ret != IMTK_PB_ERROR_CODE_OK)
        {
            BT_DBG_NORMAL(BT_DEBUG_A2DP,"unset mediainfo fail %d", i4_ret);
            goto FINISH;
        }
#endif
        i4_ret = IMtkPb_Ctrl_Close(h_handle);
        if (i4_ret != IMTK_PB_ERROR_CODE_OK)
        {
            BT_DBG_NORMAL(BT_DEBUG_A2DP,"close playback fail %d", i4_ret);
            goto FINISH;
        }

        BT_DBG_ERROR(BT_DEBUG_A2DP,"CMIF close playback success %d", i4_ret);
        audio_status = BT_AUDIO_UNINIT;
        BT_DBG_NORMAL(BT_DEBUG_A2DP,"BT AUDIO change status = %d", audio_status);
        h_handle = IMTK_NULL_HANDLE;
        if (NULL_HANDLE != h_bt_playback_mutex)
        {
            x_sema_unlock(h_bt_playback_mutex);
        }
FINISH:

        g_bt_playback_cb.fgPlayBackInit = FALSE;
    }
    else
    {
        BT_DBG_NORMAL(BT_DEBUG_A2DP,"playback not init, no need do pb_close");
    }
    return 0;
}

VOID bluetooth_a2dp_pb_stop (VOID)
{
    FUNC_ENTRY;
    INT32 i4_ret;

    if (FALSE == g_bt_playback_cb.fgPlayBackInit)
    {
        BT_DBG_NORMAL(BT_DEBUG_A2DP,"BT playback have not init");
        return;
    }
    if (audio_status >= BT_AUDIO_PLAYED)
    {
        if (NULL_HANDLE != h_bt_playback_mutex)
        {
            x_sema_lock(h_bt_playback_mutex, X_SEMA_OPTION_WAIT);
        }
        /*avoid double stop or invalid stop*/
        if (audio_status == BT_AUDIO_STOPED || audio_status == BT_AUDIO_UNINIT)
        {
            BT_DBG_NORMAL(BT_DEBUG_A2DP,"BT AUDIO change status = %d", audio_status);
        }
        else
        {
            audio_status = BT_AUDIO_STOPED;
            BT_DBG_NORMAL(BT_DEBUG_A2DP,"BT AUDIO change status = %d", audio_status);

#if ENABLE_INTERNAL_SUSPEND
            fg_bt_is_pb_pause = TRUE;
#endif
            /*avoid quick init playback and uninit playback*/
            if (FALSE == g_bt_playback_cb.fgPlayBackInit)
            {
                BT_DBG_ERROR(BT_DEBUG_A2DP,"BT playback have uninit");
                if (NULL_HANDLE != h_bt_playback_mutex)
                {
                    x_sema_unlock(h_bt_playback_mutex);
                }
                return;
            }

            i4_ret = IMtkPb_Ctrl_Stop(h_handle);
            if (i4_ret != IMTK_PB_ERROR_CODE_OK)
            {
                BT_DBG_NORMAL(BT_DEBUG_A2DP,"stop playback fail %ld", (long)i4_ret);
                if (NULL_HANDLE != h_bt_playback_mutex)
                {
                    x_sema_unlock(h_bt_playback_mutex);
                }
                return;
            }
        }
        if (NULL_HANDLE != h_bt_playback_mutex)
        {
            x_sema_unlock(h_bt_playback_mutex);
        }

#if ENABLE_BUFFER_CONTROL
        btaudio_buffer_control_uninit();
#endif

    }
}
/*-----------------------------------------------------------------------------
 * Name: bluetooth_a2dp_pb_suspend
 *
 * Description: -
 *
 * Inputs:  -
 *
 * Outputs: -
 *
 * Returns: -
 *
 ----------------------------------------------------------------------------*/
VOID bluetooth_a2dp_pb_suspend(VOID)
{
    FUNC_ENTRY;
    IMTK_PB_ERROR_CODE_T i4_ret = 0;

    g_fgBufferReady = FALSE;
    if (audio_status != BT_AUDIO_PLAYED
            && audio_status != BT_AUDIO_STOPED)
    {
        BT_DBG_NORMAL(BT_DEBUG_A2DP,"BT AUDIO wrong status, current status = %d", audio_status);
    }
    else
    {
        if (NULL_HANDLE != h_bt_playback_mutex)
        {
            x_sema_lock(h_bt_playback_mutex, X_SEMA_OPTION_WAIT);
        }
        audio_status = BT_AUDIO_PAUSED;
        BT_DBG_NORMAL(BT_DEBUG_A2DP,"BT AUDIO change status = %d", audio_status);

#if ENABLE_INTERNAL_SUSPEND
        fg_bt_is_pb_pause = TRUE;
#endif
#if DISCARD_DATA_BEFORE_PLAY
        if (0)
        {
            //g_fgdiscard_data = TRUE;
            g_bt_playback_cb.fgInterDiscard = TRUE;
        }
        else
        {
            //g_fgdiscard_data = FALSE;
            g_bt_playback_cb.fgInterDiscard = FALSE;
        }
#endif/*DISCARD_DATA_BEFORE_PLAY*/

        /*avoid quick init playback and uninit playback*/
        if (FALSE == g_bt_playback_cb.fgPlayBackInit)
        {
            BT_DBG_ERROR(BT_DEBUG_A2DP,"BT playback have uninit");
            if (NULL_HANDLE != h_bt_playback_mutex)
            {
                x_sema_unlock(h_bt_playback_mutex);
            }
            return;
        }
        i4_ret = IMtkPb_Ctrl_Pause(h_handle);
        if (IMTK_PB_ERROR_CODE_OK != i4_ret)
        {
            BT_DBG_ERROR(BT_DEBUG_A2DP,"bluetooth_a2dp_pb_suspend failed i4_ret %d ! ", i4_ret);
            if (NULL_HANDLE != h_bt_playback_mutex)
            {
                x_sema_unlock(h_bt_playback_mutex);
            }
            return;
        }
        else
        {
            //g_fgIsPaused = TRUE;
            //g_fgIsPlayed = FALSE;
            BT_DBG_ERROR(BT_DEBUG_A2DP,"CMIF bluetooth_a2dp_pb_suspend successful ");
        }
        if (NULL_HANDLE != h_bt_playback_mutex)
        {
            x_sema_unlock(h_bt_playback_mutex);
        }
    }

    return;
}

VOID bluetooth_a2dp_pb_restart (VOID)
{
    FUNC_ENTRY;

    IMTK_PB_ERROR_CODE_T i4_ret = 0;
    struct timeval before_tv;
    struct timeval after_tv;

    g_fgBufferReady = FALSE;
    if (audio_status != BT_AUDIO_OPENED
            && audio_status != BT_AUDIO_PAUSED
            && audio_status != BT_AUDIO_STOPED)
    {
        BT_DBG_NORMAL(BT_DEBUG_A2DP,"BT AUDIO wrong status, current status = %d", audio_status);
    }
    else
    {
        if (NULL_HANDLE != h_bt_playback_mutex)
        {
            x_sema_lock(h_bt_playback_mutex, X_SEMA_OPTION_WAIT);
        }
#if ENABLE_INTERNAL_SUSPEND
        fg_bt_is_pb_pause = FALSE;
#endif
        audio_status = BT_AUDIO_PLAYED;
#if DISCARD_DATA_BEFORE_PLAY
        if (0)
        {
            g_bt_playback_cb.fgInterDiscard = TRUE;
        }
        else
        {
            g_bt_playback_cb.fgInterDiscard = FALSE;
        }
#endif/*DISCARD_DATA_BEFORE_PLAY*/

        BT_DBG_NORMAL(BT_DEBUG_A2DP,"BT AUDIO change status = %d", audio_status);
        gettimeofday(&before_tv, NULL);
        /*avoid quick init playback and uninit playback*/
        if (FALSE == g_bt_playback_cb.fgPlayBackInit)
        {
            BT_DBG_ERROR(BT_DEBUG_A2DP,"BT playback have uninit");
            if (NULL_HANDLE != h_bt_playback_mutex)
            {
                x_sema_unlock(h_bt_playback_mutex);
            }
            return;
        }
        i4_ret = IMtkPb_Ctrl_Play(h_handle, 0);
        if (IMTK_PB_ERROR_CODE_OK != i4_ret)
        {
            BT_DBG_ERROR(BT_DEBUG_A2DP,"CMIF bluetooth_a2dp_pb_restart failed i4_ret %d ! ", i4_ret);
            if (NULL_HANDLE != h_bt_playback_mutex)
            {
                x_sema_unlock(h_bt_playback_mutex);
            }
            return;
        }
        else
        {
            //g_fgIsPaused = FALSE;
            //g_fgIsPlayed = TRUE;

            BT_DBG_NOTICE(BT_DEBUG_A2DP, "CMIF bluetooth_a2dp_pb_restart successful! ");
        }
        gettimeofday(&after_tv, NULL);
        BT_DBG_INFO(BT_DEBUG_A2DP,"play execute time:%lds:%ldms",after_tv.tv_sec-before_tv.tv_sec,(after_tv.tv_usec-before_tv.tv_usec)/1000);


#if DISCARD_DATA_BEFORE_PLAY
        g_bt_playback_cb.fgInterDiscard = FALSE;
#endif/*DISCARD_DATA_BEFORE_PLAY*/

        if (NULL_HANDLE != h_bt_playback_mutex)
        {
            x_sema_unlock(h_bt_playback_mutex);
        }

    }
    //g_remained_buf_size = 0;
    bluetooth_a2dp_snk_init();
#if ENABLE_BUFFER_CONTROL
    btaudio_buffer_control_init();
#endif

    return;
}



/*-----------------------------------------------------------------------------
 * Name: bluetooth_a2dp_pb_close
 *
 * Description: -
 *
 * Inputs:  -
 *
 * Outputs: -
 *
 * Returns: -
 *
 ----------------------------------------------------------------------------*/
VOID bluetooth_a2dp_pb_close(VOID)
{
    FUNC_ENTRY;
    /*Avoid playback initation is not completed, A2DP disconnection is triggered.It will result other audio feature can't open playback*/
    if (NULL_HANDLE != h_bt_playback_mutex)
    {
        x_sema_lock(h_bt_playback_mutex, X_SEMA_OPTION_WAIT);
    }

#if ENABLE_INTERNAL_SUSPEND
    bt_internal_suspend_uninit();
#endif/* ENABLE_INTERNAL_SUSPEND */

    if (audio_status == BT_AUDIO_UNINIT)
    {
        BT_DBG_NOTICE(BT_DEBUG_A2DP, "BT AUDIO wrong status, current status = %d", audio_status);
    }
    else
    {
        bluetooth_playback_uninit();

#if ENABLE_BUFFER_CONTROL
        btaudio_buffer_control_uninit();
#endif
        bluetooth_a2dp_sink_finishDump();
    }

    if (NULL_HANDLE != h_bt_playback_mutex)
    {
        x_sema_unlock(h_bt_playback_mutex);
    }
}


INT32 bt_reset_audio(VOID)
{
    FUNC_ENTRY;
    INT32 i4_ret;
    uint8_t *pu1PushBuf = NULL;
    uint32_t u4BufferSize = g_u4_BufferSize;

    if (FALSE == g_bt_playback_cb.fgPlayBackInit)
    {
        BT_DBG_NORMAL(BT_DEBUG_A2DP,"BT playback have not init");
        return 0;
    }

#if ENABLE_INTERNAL_SUSPEND
    /* into internal suspend mode */
    fg_bt_is_pb_pause = TRUE;
#endif

    if (audio_status >= BT_AUDIO_PLAYED
            && audio_status != BT_AUDIO_STOPED)
    {
        //fg_audio_reset = TRUE;
        bluetooth_a2dp_pb_setAudioResetFg(TRUE);
        if (NULL_HANDLE != h_bt_playback_mutex)
        {
            x_sema_lock(h_bt_playback_mutex, X_SEMA_OPTION_WAIT);
        }
        i4_ret = IMtkPb_Ctrl_Stop(h_handle);
        if (i4_ret != IMTK_PB_ERROR_CODE_OK)
        {
            BT_DBG_NORMAL(BT_DEBUG_A2DP,"stop playback fail %ld", (long)i4_ret);
            if (NULL_HANDLE != h_bt_playback_mutex)
            {
                x_sema_unlock(h_bt_playback_mutex);
            }
            return BT_ERR_STATUS_FAIL;
        }
        if (NULL_HANDLE != h_bt_playback_mutex)
        {
            x_sema_unlock(h_bt_playback_mutex);
        }

        //fg_audio_reset = FALSE;
        bluetooth_a2dp_pb_setAudioResetFg(FALSE);
        audio_status = BT_AUDIO_STOPED;
        BT_DBG_NORMAL(BT_DEBUG_A2DP,"BT AUDIO change status = %d", audio_status);
    }

    i4_ret = IMtkPb_Ctrl_GetBuffer(h_handle, u4BufferSize*10, &pu1PushBuf);
    if (IMTK_PB_ERROR_CODE_GET_BUF_PENDING == i4_ret)
    {
        BT_DBG_INFO(BT_DEBUG_A2DP,"get push buffer pending !");
        return -1;
    }
    else if (IMTK_PB_ERROR_CODE_OK != i4_ret)
    {
        BT_DBG_INFO(BT_DEBUG_A2DP,"get push buffer failed !");
        return BT_ERR_STATUS_FAIL;
    }
    memset(pu1PushBuf, 0x0, u4BufferSize*10);
    i4_ret = IMtkPb_Ctrl_SendData(h_handle, u4BufferSize*10, pu1PushBuf);
    if (IMTK_PB_ERROR_CODE_OK != i4_ret)
    {
        BT_DBG_INFO(BT_DEBUG_A2DP,"send push data failed ! ");
        return BT_ERR_STATUS_FAIL;
    }

    /*For fix BDP00427455:When c_btm_audio_reset is called, g_fgdiscard_data should be changed into FALSE after out of internal suspend;
      but if bluetooth_a2dp_pb_restart is not called, g_fgdiscard_data is not changed into FALSE and data is always discarded*/
    //audio_status = BT_AUDIO_PLAYED;
    //i4_ret = IMtkPb_Ctrl_Play(h_handle, 0);

#if ENABLE_INTERNAL_SUSPEND
    /* into internal suspend mode */
    fg_is_inter_suspend = TRUE;
#else
    bluetooth_a2dp_pb_restart();
#endif/* ENABLE_INTERNAL_SUSPEND */
    return BT_SUCCESS;
}



INT32 bt_track_change_reset_audio(VOID)
{
    FUNC_ENTRY;
    INT32 i4_ret;

    if (FALSE == g_bt_playback_cb.fgPlayBackInit)
    {
        BT_DBG_NORMAL(BT_DEBUG_A2DP,"BT playback have not init");
        return BT_ERR_STATUS_NOT_READY;
    }
    if (audio_status >= BT_AUDIO_PLAYED
            && audio_status != BT_AUDIO_STOPED)
    {
        //fg_audio_reset = TRUE;
        bluetooth_a2dp_pb_setAudioResetFg(TRUE);
        if (NULL_HANDLE != h_bt_playback_mutex)
        {
            x_sema_lock(h_bt_playback_mutex, X_SEMA_OPTION_WAIT);
        }
        i4_ret = IMtkPb_Ctrl_Stop(h_handle);
        if (i4_ret != IMTK_PB_ERROR_CODE_OK)
        {
            BT_DBG_NORMAL(BT_DEBUG_A2DP,"stop playback fail %ld", (long)i4_ret);
            if (NULL_HANDLE != h_bt_playback_mutex)
            {
                x_sema_unlock(h_bt_playback_mutex);
            }
            return BT_ERR_STATUS_FAIL;
        }
        if (NULL_HANDLE != h_bt_playback_mutex)
        {
            x_sema_unlock(h_bt_playback_mutex);
        }

        //fg_audio_reset = FALSE;
        bluetooth_a2dp_pb_setAudioResetFg(FALSE);
        audio_status = BT_AUDIO_STOPED;
        BT_DBG_NORMAL(BT_DEBUG_A2DP,"BT AUDIO change status = %d", audio_status);
    }


    if (NULL_HANDLE != h_bt_playback_mutex)
    {
        x_sema_lock(h_bt_playback_mutex, X_SEMA_OPTION_WAIT);
    }
    i4_ret = IMtkPb_Ctrl_Play(h_handle, 0);
    if (IMTK_PB_ERROR_CODE_OK != i4_ret)
    {
        BT_DBG_NORMAL(BT_DEBUG_A2DP,"bluetooth_a2dp_pb_restart failed i4_ret %ld ! ", (long)i4_ret);
        if (NULL_HANDLE != h_bt_playback_mutex)
        {
            x_sema_unlock(h_bt_playback_mutex);
        }
        return BT_ERR_STATUS_FAIL;
    }
    else
    {
        //g_fgIsPaused = FALSE;
        audio_status = BT_AUDIO_PLAYED;
        BT_DBG_NORMAL(BT_DEBUG_A2DP,"BT AUDIO change status = %d", audio_status);
        BT_DBG_NORMAL(BT_DEBUG_A2DP,"bluetooth_a2dp_pb_restart successful! ");
    }
    if (NULL_HANDLE != h_bt_playback_mutex)
    {
        x_sema_unlock(h_bt_playback_mutex);
    }

    return BT_SUCCESS;
}


INT32 bluetooth_a2dp_pb_push_data(UINT8 *pPcmBuf, INT32 i4PcmLen)
{
    IMTK_PB_ERROR_CODE_T i4_ret = 0;
    UINT8 *pu1PushBuf = NULL;
    UINT32 u4BufferSize = i4PcmLen;
    g_u4_BufferSize = i4PcmLen;

    if (NULL == pPcmBuf || 0 == i4PcmLen)
    {
        return BT_ERR_STATUS_PARM_INVALID;
    }

    g_fgBufferReady = TRUE;
    i4_ret = IMtkPb_Ctrl_GetBuffer(h_handle, u4BufferSize, &pu1PushBuf);
    if (IMTK_PB_ERROR_CODE_GET_BUF_PENDING == i4_ret)
    {
        BT_DBG_INFO(BT_DEBUG_A2DP,"get push buffer pending !");
        g_fgBufferReady = FALSE;
        return BT_ERR_STATUS_FAIL;
    }
    else if (IMTK_PB_ERROR_CODE_OK != i4_ret)
    {
        BT_DBG_INFO(BT_DEBUG_A2DP,"get push buffer failed !");
        return BT_ERR_STATUS_FAIL;
    }

    if (0 != i4PcmLen)
    {
        memcpy(pu1PushBuf, pPcmBuf, i4PcmLen);
        BT_DBG_MINOR(BT_DEBUG_A2DP,"send %ld data", (long)i4PcmLen);
        if (dec_audio_dump_conf && dec_audio_dump_conf->dec_dump_enable)
        {
            bluetooth_a2dp_dump_data((CHAR *)pu1PushBuf, u4BufferSize, dec_audio_dump_conf->pcm_dec_fp);
        }

        i4_ret = IMtkPb_Ctrl_SendData(h_handle, u4BufferSize, pu1PushBuf);
        if (IMTK_PB_ERROR_CODE_OK != i4_ret)
        {
            g_fgBufferReady = FALSE;
            BT_DBG_INFO(BT_DEBUG_A2DP,"send push data failed ! ");
            return BT_ERR_STATUS_FAIL;
        }
    }
    return BT_SUCCESS;
}

BOOL bluetooth_a2dp_pb_getInit(VOID)
{
    return g_bt_playback_cb.fgPlayBackInit;
}



VOID bluetooth_a2dp_pb_setAudioResetFg(BOOL fgAudioReset)
{
    g_bt_playback_cb.fgAudioReset = fgAudioReset;
    return;
}

BOOL bluetooth_a2dp_pb_getAudioResetFg(VOID)
{
    return g_bt_playback_cb.fgAudioReset;
}


INT32 bt_a2dp_playback_play(VOID)
{
    FUNC_ENTRY;

    set_bt_stream_suspend(FALSE);
    if (FALSE == bluetooth_a2dp_pb_getInit())
    {
        BT_DBG_NOTICE(BT_DEBUG_A2DP, "BT playback have not init");
        return 0;
    }
    if (audio_status != BT_AUDIO_OPENED
            && audio_status != BT_AUDIO_PAUSED
            && audio_status != BT_AUDIO_STOPED)
    {
        BT_DBG_NORMAL(BT_DEBUG_A2DP, "BT AUDIO wrong status, current status = %d", audio_status);
    }
    else
    {
        BT_DBG_NORMAL(BT_DEBUG_A2DP, "BT AUDIO current status = %d", audio_status);
#if ENABLE_INTERNAL_SUSPEND
        /* into internal suspend mode */
        BT_DBG_NORMAL(BT_DEBUG_A2DP, "into internal suspend mode");
        fg_is_inter_suspend = TRUE;
#else
        bluetooth_a2dp_pb_restart();
#endif/* ENABLE_INTERNAL_SUSPEND */
    }

    return 0;
}

INT32 bt_a2dp_playback_suspend(VOID)
{
    FUNC_ENTRY;
    set_bt_stream_suspend(TRUE);
    if (FALSE == bluetooth_a2dp_pb_getInit())
    {
        BT_DBG_NOTICE(BT_DEBUG_A2DP, "BT playback have not init");
        return 0;
    }
    if (audio_status != BT_AUDIO_PLAYED)
    {
        BT_DBG_NOTICE(BT_DEBUG_A2DP, "BT AUDIO wrong status, current status = %d", audio_status);
    }
    else
    {
#if ENABLE_INTERNAL_SUSPEND
        if (!fg_bt_is_pb_pause)
        {
            fg_bt_is_pb_pause = TRUE;
            //bluetooth_a2dp_pb_suspend();
            bluetooth_a2dp_pb_stop();
        }
        else
        {
            BT_DBG_NOTICE(BT_DEBUG_A2DP, "pb is pause no need suspend");
        }
#else
        //bluetooth_a2dp_pb_suspend();
        bluetooth_a2dp_pb_stop();
#endif/* ENABLE_INTERNAL_SUSPEND */
    }

    return 0;
}


VOID bluetooth_a2dp_sink_setBuffer(UINT8 *pu1Buffer, const UINT32 u4BufLen)
{
    if (u4BufLen + g_a2dp_snk_cb.i4LeftLen <= IN_BUF_SIZE)
    {
        memcpy(&g_a2dp_snk_cb.u1InBuf[g_a2dp_snk_cb.i4LeftLen], pu1Buffer, u4BufLen);
        g_a2dp_snk_cb.u4InLen = u4BufLen + g_a2dp_snk_cb.i4LeftLen;
    }
    else
    {
        memcpy(&g_a2dp_snk_cb.u1InBuf[g_a2dp_snk_cb.i4LeftLen],
               pu1Buffer, IN_BUF_SIZE-g_a2dp_snk_cb.i4LeftLen);
        g_a2dp_snk_cb.u4InLen = IN_BUF_SIZE;
    }
}


void btaudio_write_stream_data(UINT8 *data, UINT16 datalen)
{
    BT_AUDIO_CODEC_TYPE codec_type;
    UINT32 u4PcmLen = datalen;
    UINT8 *pu1PcmBuf = data;

    if (NULL == data || 0 == datalen)
    {
        BT_DBG_ERROR(BT_DEBUG_A2DP, "data is null(%p) or data len=%d", data, datalen);
        return;
    }

    BT_DBG_INFO(BT_DEBUG_A2DP, "data=%p, datalen=%u", data, datalen);

    if (dec_audio_dump_conf && dec_audio_dump_conf->dec_dump_enable)
    {
        bluetooth_a2dp_dump_data((CHAR *)data, datalen, dec_audio_dump_conf->dec_dump_fp);
    }

    if (FALSE == bluetooth_a2dp_pb_getInit())
    {
        BT_DBG_NOTICE(BT_DEBUG_A2DP, "bt playback does not init");
        return;
    }

#if ENABLE_INTERNAL_SUSPEND
    bt_internal_suspend_start();
#endif/* ENABLE_INTERNAL_SUSPEND */

    if (TRUE == bluetooth_a2dp_pb_getAudioResetFg())
    {
        BT_DBG_NOTICE(BT_DEBUG_A2DP, "audio_reset");
        return;
    }

    codec_type = bluetooth_a2dp_getCurrentCodecType();



    bluetooth_a2dp_sink_setBuffer(data, datalen);

    if (BT_SUCCESS != bluetooth_a2dp_pb_push_data(pu1PcmBuf, u4PcmLen))
    {
        BT_DBG_ERROR(BT_DEBUG_A2DP, "play pcm failed, pcmbuf:%p, len:%lu", pu1PcmBuf, u4PcmLen);
        return;
    }

    return;
}




VOID bluetooth_a2dp_sink_prepareDump(const BT_AUDIO_CODEC_TYPE audio_codec_type)
{
    CHAR *codec_type_name =NULL;
    CHAR udisk_path[MAX_NAME_STR_LEN] = BT_TMP_PATH;
    codec_type_name =    audio_codec_type == BT_AUDIO_SBC?"SBC":\
                         audio_codec_type == BT_AUDIO_AAC?"AAC":\
                         audio_codec_type == BT_AUDIO_LDAC?"LDAC":"UNKNOW";

    if (BT_SUCCESS != bluetooth_a2dp_getUdiskPath(udisk_path))
    {
        BT_DBG_WARNING(BT_DEBUG_A2DP, "get udisk path failed, use default: %s", udisk_path);
    }
    if (dec_audio_dump_conf
            && dec_audio_dump_conf->dec_dump_enable
            && !dec_audio_dump_conf->dec_dump_fp)
    {
        snprintf(dec_audio_dump_conf->dec_dump_path, sizeof(dec_audio_dump_conf->dec_dump_path),
                 "%s/before_decode_%s.data", udisk_path, codec_type_name);
        remove(dec_audio_dump_conf->dec_dump_path);
        dec_audio_dump_conf->dec_dump_fp = fopen(dec_audio_dump_conf->dec_dump_path, "w+");
        if (!dec_audio_dump_conf->dec_dump_fp)
        {
            BT_DBG_ERROR(BT_DEBUG_A2DP, "open decoder dump file failed, path=[%s]!", dec_audio_dump_conf->dec_dump_path);
            return;
        }
        BT_DBG_NORMAL(BT_DEBUG_A2DP, "open decoder dump file successful, path=[%s]!", dec_audio_dump_conf->dec_dump_path);
        snprintf(dec_audio_dump_conf->pcm_dec_path, sizeof(dec_audio_dump_conf->pcm_dec_path),
                 "%s/after_decode_%s.pcm", udisk_path, codec_type_name);
        remove(dec_audio_dump_conf->pcm_dec_path);
        dec_audio_dump_conf->pcm_dec_fp = fopen(dec_audio_dump_conf->pcm_dec_path, "w+");
        if (!dec_audio_dump_conf->pcm_dec_fp)
        {
            BT_DBG_ERROR(BT_DEBUG_A2DP, "open decoded pcm dump file failed, path=[%s]!", dec_audio_dump_conf->pcm_dec_path);
            return;
        }
        BT_DBG_NORMAL(BT_DEBUG_A2DP, "open decoded pcm dump file successful, path=[%s]!", dec_audio_dump_conf->pcm_dec_path);
    }
}

VOID bluetooth_a2dp_sink_finishDump(VOID)
{
    if (dec_audio_dump_conf && dec_audio_dump_conf->dec_dump_enable)
    {
        if (dec_audio_dump_conf->dec_dump_fp)
        {
            BT_DBG_NORMAL(BT_DEBUG_A2DP, "close dump decoded file:[%s]!", dec_audio_dump_conf->dec_dump_path);
            fclose(dec_audio_dump_conf->dec_dump_fp);
            dec_audio_dump_conf->dec_dump_fp = NULL;
        }
        if (dec_audio_dump_conf->pcm_dec_fp)
        {
            BT_DBG_NORMAL(BT_DEBUG_A2DP, "close dump decoder pcm file:[%s]!", dec_audio_dump_conf->pcm_dec_path);
            fclose(dec_audio_dump_conf->pcm_dec_fp);
            dec_audio_dump_conf->pcm_dec_fp = NULL;
        }
        free(dec_audio_dump_conf);
        dec_audio_dump_conf = NULL;
    }
}

void linuxbt_playback_init(int trackFreq, int channelType)
{
    BT_DBG_NORMAL(BT_DEBUG_A2DP, "[A2DP] %s()", __FUNCTION__);
    bluetooth_playback_init(trackFreq, channelType);
}

void linuxbt_playback_deinit(void)
{
    BT_DBG_NORMAL(BT_DEBUG_A2DP, "[A2DP] %s()", __FUNCTION__);
    bluetooth_a2dp_pb_close();
}

void linuxbt_playback_play(void)
{
    BT_DBG_NORMAL(BT_DEBUG_A2DP, "[A2DP] %s()", __FUNCTION__);
    bt_a2dp_playback_play();
}

void linuxbt_playback_pause(void)
{
    BT_DBG_NORMAL(BT_DEBUG_A2DP, "[A2DP] %s()", __FUNCTION__);
    bt_a2dp_playback_suspend();
}

int linuxbt_playback_send_data(void *audioBuffer, int bufferlen)
{
    BT_DBG_NORMAL(BT_DEBUG_A2DP, "[A2DP] %s()", __FUNCTION__);
    btaudio_write_stream_data(audioBuffer, bufferlen);
    return 0;
}

BT_AUDIO_CODEC_TYPE bluetooth_a2dp_getCurrentCodecType(VOID)
{
    bt_print_codec_info(BT_AUDIO_SBC);
    return BT_AUDIO_SBC;
}



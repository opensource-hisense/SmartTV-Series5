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
/* FILE NAME:  bt_mw_a2dp_src.c
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

#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <assert.h>
#include <fcntl.h>
#include <sys/prctl.h>
#include <sys/select.h>
#include <sys/time.h>

#include "bt_mw_common.h"
#include "bt_mw_a2dp_common.h"
#include "c_bt_mw_a2dp_common.h"
#include "bt_mw_a2dp_src.h"
#include "linuxbt_a2dp_if.h"
#include "bt_mw_msg.h"
#include "bt_mw_avrcp.h"
#include "bt_mw_manager.h"
#include "bt_mw_devices_info.h"

bt_enc_audio_dump_t* enc_audio_dump_conf = NULL;
extern BT_TARGET_DEV_LIST *g_pt_paired_sink_dev_history;
BtAppA2dpPlaybackCtrlCbk AppPlaybackCtrlCbk;
extern BtAppEventCbk      BtAppCbk;
extern BtAppA2dpResetAudioCbk AppResetAudioCbk;

extern UINT8 fg_bt_avdtp_start_ind_is_triggered;

BT_AUDIO_SRC_CB_FUNC upload_func = {0};

static UINT32 s_bt_a2dp_src_freq = 48000;
static UINT8 s_bt_a2dp_src_channel = 2;

INT32 bluetooth_a2dp_register_upload_cb(BT_AUDIO_SRC_CB_FUNC *func)
{
    FUNC_ENTRY;
    INT32 i4_ret = BT_SUCCESS;

    memset(&upload_func, 0x0, sizeof(BT_AUDIO_SRC_CB_FUNC));

    if (NULL == func)
    {
        BT_DBG_ERROR(BT_DEBUG_COMM, "callback func is null!");
        return BT_ERR_STATUS_NULL_POINTER;
    }
    if (func->bt_upd_init_cb)
    {
        upload_func.bt_upd_init_cb = func->bt_upd_init_cb;
    }
    else
    {
        BT_DBG_ERROR(BT_DEBUG_COMM, "bt_upd_init_cb func is null!");
        i4_ret = BT_ERR_STATUS_NULL_POINTER;
    }

    if (func->bt_upd_start_cb)
    {
        upload_func.bt_upd_start_cb = func->bt_upd_start_cb;
    }
    else
    {
        BT_DBG_WARNING(BT_DEBUG_COMM, "bt_upd_start_cb func is null!");
        i4_ret = BT_ERR_STATUS_NULL_POINTER;
    }

    if (func->bt_upd_stop_cb)
    {
        upload_func.bt_upd_stop_cb = func->bt_upd_stop_cb;
    }
    else
    {
        BT_DBG_WARNING(BT_DEBUG_COMM, "bt_upd_stop_cb func is null!");
        i4_ret = BT_ERR_STATUS_NULL_POINTER;
    }

    if (func->bt_upd_deinit_cb)
    {
        upload_func.bt_upd_deinit_cb = func->bt_upd_deinit_cb;
    }
    else
    {
        BT_DBG_WARNING(BT_DEBUG_COMM, "bt_upd_deinit_cb func is null!");
        i4_ret = BT_ERR_STATUS_NULL_POINTER;
    }
    return i4_ret;
}

INT32 bluetooth_a2dp_set_audio_log_lvl(UINT8 log_level)
{
    return linuxbt_a2dp_set_audio_hw_log_lvl(log_level);
}

INT32 bluetooth_a2dp_src_enable(UINT8 src_enable)
{
    FUNC_ENTRY;
    set_bt_enabled_a2dp_role(TRUE, src_enable);
    return linuxbt_a2dp_src_enable_handler(src_enable);
}

INT32 bluetooth_a2dp_src_send_stream_data(CHAR *data, INT32 len)
{
    if (TRUE == get_bt_is_mute())
    {
        memset(data, 0, len);
    }

    return linuxbt_send_stream_data(data, len);
}

VOID bluetooth_a2dp_src_prepareDump(const BT_AUDIO_CODEC_TYPE audio_codec_type)
{
    BT_DBG_NOTICE(BT_DEBUG_A2DP, "");

    CHAR *codec_type_name = NULL;
    CHAR udisk_path[MAX_NAME_STR_LEN] = BT_TMP_PATH;

    codec_type_name =    audio_codec_type == BT_AUDIO_SBC?"SBC":\
                         audio_codec_type == BT_AUDIO_AAC?"AAC":\
                         audio_codec_type == BT_AUDIO_LDAC?"LDAC":"UNKNOW";

    if (BT_SUCCESS != bluetooth_a2dp_getUdiskPath(udisk_path))
    {
        BT_DBG_WARNING(BT_DEBUG_A2DP, "get udisk path failed, use default: %s", udisk_path);
    }

    if (enc_audio_dump_conf
        && enc_audio_dump_conf->enc_dump_enable
        && !enc_audio_dump_conf->enc_dump_fp)
    {
        snprintf(enc_audio_dump_conf->enc_dump_path, sizeof(enc_audio_dump_conf->enc_dump_path),
             "%s/after_encode_%s.data", udisk_path, codec_type_name);
        remove(enc_audio_dump_conf->enc_dump_path);
        enc_audio_dump_conf->enc_dump_fp = fopen(enc_audio_dump_conf->enc_dump_path, "w+");
        if (!enc_audio_dump_conf->enc_dump_fp)
        {
            BT_DBG_ERROR(BT_DEBUG_A2DP, "open encoded dump file failed, path=[%s]!", enc_audio_dump_conf->enc_dump_path);
            return;
        }
        BT_DBG_NORMAL(BT_DEBUG_A2DP, "open encoded dump file successful, path=[%s]!", enc_audio_dump_conf->enc_dump_path);

        snprintf(enc_audio_dump_conf->pcm_enc_path, sizeof(enc_audio_dump_conf->pcm_enc_path),
             "%s/before_encode_%s.pcm", udisk_path, codec_type_name);
        remove(enc_audio_dump_conf->pcm_enc_path);
        enc_audio_dump_conf->pcm_enc_fp = fopen(enc_audio_dump_conf->pcm_enc_path, "w+");
        if (!enc_audio_dump_conf->pcm_enc_fp)
        {
            BT_DBG_ERROR(BT_DEBUG_A2DP, "open encoder pcm dump file failed, path=[%s]!", enc_audio_dump_conf->pcm_enc_path);
            return;
        }
        BT_DBG_NORMAL(BT_DEBUG_A2DP, "open encoder pcm dump file successful, path=[%s]!", enc_audio_dump_conf->pcm_enc_path);

#ifdef DUMP_UPLOADER_DATA
        snprintf(enc_audio_dump_conf->pcm_upload_path, sizeof(enc_audio_dump_conf->pcm_upload_path),
             "%s/before_encode_%s.pcm", udisk_path, codec_type_name);
        remove(enc_audio_dump_conf->pcm_upload_path);
        enc_audio_dump_conf->pcm_upload_fp = fopen(enc_audio_dump_conf->pcm_upload_path, "w+");
        if (!enc_audio_dump_conf->pcm_upload_fp)
        {
            BT_DBG_ERROR(BT_DEBUG_A2DP, "open uploaded dump file failed, path=[%s]!", enc_audio_dump_conf->pcm_upload_path);
            return;
        }
        BT_DBG_NORMAL(BT_DEBUG_A2DP, "open uploaded dump file successful, path=[%s]!", enc_audio_dump_conf->pcm_upload_path);
#endif
    }
}

VOID bluetooth_a2dp_src_finishDump(VOID)
{
    BT_DBG_NOTICE(BT_DEBUG_A2DP, "");

    if (enc_audio_dump_conf && enc_audio_dump_conf->enc_dump_enable)
    {
        if (enc_audio_dump_conf->enc_dump_fp)
        {
            BT_DBG_NORMAL(BT_DEBUG_A2DP, "close dump encoded file:[%s]!", enc_audio_dump_conf->enc_dump_path);
            fclose(enc_audio_dump_conf->enc_dump_fp);
            enc_audio_dump_conf->enc_dump_fp = NULL;
        }

        if (enc_audio_dump_conf->pcm_enc_fp)
        {
            BT_DBG_NORMAL(BT_DEBUG_A2DP, "close dump encoder pcm file:[%s]!", enc_audio_dump_conf->pcm_enc_path);
            fclose(enc_audio_dump_conf->pcm_enc_fp);
            enc_audio_dump_conf->pcm_enc_fp = NULL;
        }

#ifdef DUMP_UPLOADER_DATA
        if (enc_audio_dump_conf->pcm_upload_fp)
        {
            BT_DBG_NORMAL(BT_DEBUG_A2DP, "close dump uploader pcm file:[%s]!", enc_audio_dump_conf->pcm_upload_path);
            fclose(enc_audio_dump_conf->pcm_upload_fp);
            enc_audio_dump_conf->pcm_upload_fp = NULL;
        }
#endif
        free(enc_audio_dump_conf);
        enc_audio_dump_conf = NULL;
    }
}

INT32 bluetooth_a2dp_src_get_start_delay(VOID)
{
    /* SBH50 is no sound when 1st connection and need wait 3s to send AVDTP_START */
    /* In case
     * 1. 1st connection
     * 2. send data after connected(on phone, play music before connecting.
     */
    if (0 == strncmp(g_bt_target_dev_info.name, "SBH50", 6))
    {
        return 3000;
    }

    return 0;
}

INT32 bluetooth_a2dp_src_open_cb(VOID)
{
    FUNC_ENTRY;
    INT32 delay_ms = 0;

    /*reset the last time of send streaming data*/
    set_bt_a2dp_be_sink(FALSE);
    bluetooth_a2dp_2nd_con_enable(0);
    if (get_bt_a2dp_connect())
    {
        /*Reset flag*/

        set_bt_mode(BT_MODE_PLAYING);
        set_bt_connect(TRUE);
        BtAppCbk(BT_PLAYING_MODE, bt_last_device_addr);
        BtAppCbk(BT_A2DP_CONNECTION_SUCCESS, bt_last_device_addr);
        BtAppCbk(BT_CONNECT_SUCCESS, bt_last_device_addr);
#if ENABLE_A2DP_SRC // cc_temp
        _bt_update_dev_history(&g_bt_target_dev_info, g_pt_paired_sink_dev_history);
#endif

        if (NULL != upload_func.bt_upd_init_cb)
        {
            upload_func.bt_upd_init_cb(s_bt_a2dp_src_freq, s_bt_a2dp_src_channel);
        }
        else
        {
            BT_DBG_WARNING(BT_DEBUG_A2DP, "bt_upd_init_cb func is null!");
        }
        delay_ms = bluetooth_a2dp_src_get_start_delay();
        if (NULL != upload_func.bt_upd_start_cb)
        {
            upload_func.bt_upd_start_cb(delay_ms);
        }
        else
        {
            BT_DBG_WARNING(BT_DEBUG_A2DP, "bt_upd_start_cb func is null!");
        }
        return BT_SUCCESS;
    }
    return BT_SUCCESS;
}

INT32 bluetooth_a2dp_src_start_cb(VOID)
{
    INT32 i4_ret = BT_ERR_STATUS_FAIL;
    //btav_audio_cap_struct tcurrent_config;

    /*it means that AVDTP START has been triggered*/
    fg_bt_avdtp_start_ind_is_triggered = 1;
    set_bt_a2dp_pause(FALSE);
    set_bt_a2dp_pause_done(FALSE);

    /*avoid using not-init codec of encode_Init, wait for the realtime codec of encode_Init has not been gotten*/
    i4_ret = bluetooth_a2dp_get_audio_cap_info();
    BT_CHECK_RESULT(BT_DEBUG_A2DP, i4_ret);

    return BT_SUCCESS;
}

INT32 bluetooth_a2dp_src_suspend_cb(VOID)
{
    /*add A2DP SRC handle*/
    set_bt_a2dp_pause(TRUE);
    set_bt_a2dp_pause_done(TRUE);

    return BT_SUCCESS;
}

INT32 bluetooth_a2dp_src_reconfig_cb(UINT32 freq, UINT8 channel)
{
    BT_DBG_NORMAL(BT_DEBUG_A2DP, "freq=%u, channel=%u", freq, channel);
    s_bt_a2dp_src_freq = freq;
    s_bt_a2dp_src_channel = channel;
    return BT_SUCCESS;
}

INT32 bluetooth_a2dp_src_close_cb(CHAR *device_addr)
{
    BT_DBG_NORMAL(BT_DEBUG_A2DP, "");
    /*add A2DP SRC handle*/
    if (NULL != upload_func.bt_upd_stop_cb)
    {
        upload_func.bt_upd_stop_cb();
    }
    else
    {
        BT_DBG_WARNING(BT_DEBUG_A2DP, "bt_upd_stop_cb func is null!");
    }
    if (NULL != upload_func.bt_upd_deinit_cb)
    {
        upload_func.bt_upd_deinit_cb();
    }
    else
    {
        BT_DBG_WARNING(BT_DEBUG_A2DP, "bt_upd_deinit_cb func is null!");
    }

    //bluetooth_encoder_uninit();
    bluetooth_a2dp_src_finishDump();

    /*notify current A2DP status to APP*/
    if (get_bt_avrcp_support())
    {
        BT_DBG_NOTICE(BT_DEBUG_A2DP, "support avrcp");
        /* avrcp connection have been disconnected */
        if (!(get_bt_a2dp_connect() || get_bt_avrcp_connect()))
        {
            BT_DBG_NOTICE(BT_DEBUG_A2DP, "a2dp and avrcp down");
            set_bt_connect(FALSE);
            /*avoid to result bt_mode execption while simultaneously pressing HOME and disconnecting from SRC device*/
            if (BT_MODE_PLAYING == get_bt_mode())
            {
                BT_DBG_NORMAL(BT_DEBUG_A2DP, "from playing mode");
                set_bt_mode(BT_MODE_PAIRING);
                BtAppCbk(BT_DISCONNECTED, device_addr);
            }
            else if (BT_MODE_PAIRING== get_bt_mode())
            {
                BT_DBG_NORMAL(BT_DEBUG_A2DP, "from pairing mode");
                BtAppCbk(BT_DISCONNECTED, device_addr);
            }
            else if (BT_MODE_CLOSE == get_bt_mode())
            {
                BT_DBG_NORMAL(BT_DEBUG_A2DP, "from close mode");
                BtAppCbk(BT_DISCONNECTED, device_addr);
            }
            else
            {
                BT_DBG_NORMAL(BT_DEBUG_A2DP, "not at CLOSE mode, no need send disconnection notify to APP");
            }
        }
        else
        {
            BT_DBG_NORMAL(BT_DEBUG_A2DP, "avrcp disconnection does not down");
        }
    }
    else
    {
        if (!get_bt_a2dp_connect())
        {
            BT_DBG_NORMAL(BT_DEBUG_A2DP, "not support avrcp, and a2dp down");
            set_bt_connect(FALSE);
            if (BT_MODE_CLOSE == get_bt_mode())
            {
                BT_DBG_NORMAL(BT_DEBUG_A2DP, "From close mode,it should not be change to pairing mode");
            }
            else
            {
                set_bt_mode(BT_MODE_PAIRING);
            }
            BtAppCbk(BT_DISCONNECTED, device_addr);

        }
    }
    return BT_SUCCESS;
}

VOID bluetooth_a2dp_src_avdtp_disconnected(VOID)
{
    if (get_bt_avrcp_support())
    {
        BT_DBG_NOTICE(BT_DEBUG_A2DP, "support avrcp");
        /* avrcp connection have been disconnected */
        if (!(get_bt_a2dp_connect() || get_bt_avrcp_connect()))
        {
            BT_DBG_NOTICE(BT_DEBUG_A2DP, "a2dp and avrcp down");
            set_bt_connect(FALSE);
            if (BT_MODE_CLOSE == get_bt_mode())
            {
                BT_DBG_NOTICE(BT_DEBUG_A2DP, "From close mode,it should not be change to pairing mode");
            }
            else
            {
                set_bt_mode(BT_MODE_PAIRING);
            }
            set_bt_avrcp_support(FALSE);
            BtAppCbk(BT_DISCONNECTED, NULL);
        }
        else
        {
            BT_DBG_NOTICE(BT_DEBUG_A2DP, "avrcp disconnection does not down");
        }
    }
    else
    {
        //if (!get_bt_a2dp_connect())
        {
            BT_DBG_NOTICE(BT_DEBUG_A2DP, "not support avrcp, and a2dp down");
            set_bt_connect(FALSE);
            if (BT_MODE_CLOSE == get_bt_mode())
            {
                BT_DBG_NOTICE(BT_DEBUG_A2DP, "From close mode,it should not be change to pairing mode");
            }
            else
            {
                set_bt_mode(BT_MODE_PAIRING);
            }

            set_bt_avrcp_support(FALSE);
            BtAppCbk(BT_DISCONNECTED, NULL);
        }
    }
    //bluetooth_set_connectable(TRUE);
}

VOID bluetooth_a2dp_mute_speaker(BOOL fg_mute)
{
#if ENABLE_MTK_TV_PLATFORM
    bt_upload_mute_speaker(fg_mute);
#endif
}


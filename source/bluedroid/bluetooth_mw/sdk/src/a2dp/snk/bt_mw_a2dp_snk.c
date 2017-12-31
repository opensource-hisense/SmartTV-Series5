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
/* FILE NAME:  bt_mw_a2dp_snk.c
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
#include <dlfcn.h>
//#include "u_handle.h"

#include <bt_audio_track.h>
#include "bt_mw_a2dp_common.h"
#include "bt_mw_msg.h"

#include "linuxbt_a2dp_if.h"
#include "bt_mw_a2dp_snk.h"
#include "bt_mw_devices_info.h"
#include "bt_mw_avrcp.h"
#include "bt_mw_manager.h"

extern BT_TARGET_DEV_LIST *g_pt_paired_src_dev_history;
extern BtAppEventCbk      BtAppCbk;
extern BtAppA2dpResetAudioCbk AppResetAudioCbk;
bt_dec_audio_dump_t* dec_audio_dump_conf = NULL;

EXPORT_SYMBOL BT_PLAYER_SNK_CB_FUNC player_func = {0};

void bluetooth_a2dp_playback_init(int trackFreq, int channelType);
void bluetooth_a2dp_playback_deinit(void);
void bluetooth_a2dp_playback_play(void);
void bluetooth_a2dp_playback_pause(void);
int bluetooth_a2dp_playback_send_data(void *audioBuffer, int bufferLen);

BtifAvrcpAudioTrack g_t_track_cb =
{
    sizeof(BtifAvrcpAudioTrack),
    bluetooth_a2dp_playback_init,
    bluetooth_a2dp_playback_deinit,
    bluetooth_a2dp_playback_play,
    bluetooth_a2dp_playback_pause,
    bluetooth_a2dp_playback_send_data,
};

INT32 bluetooth_a2dp_register_player_cb(BT_PLAYER_SNK_CB_FUNC *func)
{
    FUNC_ENTRY;
    INT32 i4_ret = BT_SUCCESS;

    if (NULL == func)
    {
        BT_DBG_ERROR(BT_DEBUG_A2DP, "callback func is null!");
        return BT_ERR_STATUS_NULL_POINTER;
    }
    if (func->player_init_cb)
    {
        player_func.player_init_cb = func->player_init_cb;
    }
    else
    {
        BT_DBG_ERROR(BT_DEBUG_A2DP, "player_init_cb func is null!");
        i4_ret = BT_ERR_STATUS_NULL_POINTER;
    }

    if (func->player_start_cb)
    {
        player_func.player_start_cb = func->player_start_cb;
    }
    else
    {
        BT_DBG_WARNING(BT_DEBUG_A2DP, "player_start_cb func is null!");
        i4_ret = BT_ERR_STATUS_NULL_POINTER;
    }

    if (func->player_stop_cb)
    {
        player_func.player_stop_cb = func->player_stop_cb;
    }
    else
    {
        BT_DBG_WARNING(BT_DEBUG_A2DP, "player_stop_cb func is null!");
        i4_ret = BT_ERR_STATUS_NULL_POINTER;
    }

    if (func->player_pause_cb)
    {
        player_func.player_pause_cb = func->player_pause_cb;
    }
    else
    {
        BT_DBG_WARNING(BT_DEBUG_A2DP, "player_pause_cb func is null!");
        i4_ret = BT_ERR_STATUS_NULL_POINTER;
    }

    if (func->player_deinit_cb)
    {
        player_func.player_deinit_cb = func->player_deinit_cb;
    }
    else
    {
        BT_DBG_WARNING(BT_DEBUG_A2DP, "player_deinit_cb func is null!");
        i4_ret = BT_ERR_STATUS_NULL_POINTER;
    }

    if (func->player_push_data_cb)
    {
        player_func.player_push_data_cb = func->player_push_data_cb;
    }
    else
    {
        BT_DBG_WARNING(BT_DEBUG_A2DP, "player_push_data_cb func is null!");
        i4_ret = BT_ERR_STATUS_NULL_POINTER;
    }
    return i4_ret;
}

void bluetooth_a2dp_playback_init(int trackFreq, int channelType)
{
    BT_DBG_NORMAL(BT_DEBUG_A2DP, "fs:%d channel:%d", trackFreq, channelType);
    if (player_func.player_init_cb)
    {
        player_func.player_init_cb(trackFreq, channelType);
    }
    else
    {
        BT_DBG_WARNING(BT_DEBUG_COMM, "player_start_cb func is null!");
    }
}

void bluetooth_a2dp_playback_deinit(void)
{
    BT_DBG_NORMAL(BT_DEBUG_A2DP, "");
    if (player_func.player_deinit_cb)
    {
        player_func.player_deinit_cb();
    }
    else
    {
        BT_DBG_WARNING(BT_DEBUG_A2DP, "player_deinit_cb func is null!");
    }
}

void bluetooth_a2dp_playback_play(void)
{
    BT_DBG_NORMAL(BT_DEBUG_A2DP, "");
    if (player_func.player_start_cb)
    {
        player_func.player_start_cb();
    }
    else
    {
        BT_DBG_WARNING(BT_DEBUG_A2DP, "player_start_cb func is null!");
    }
}

void bluetooth_a2dp_playback_pause(void)
{
    BT_DBG_NORMAL(BT_DEBUG_A2DP, "");
    if (player_func.player_pause_cb)
    {
        player_func.player_pause_cb();
    }
    else
    {
        BT_DBG_WARNING(BT_DEBUG_A2DP, "player_pause_cb func is null!");
    }
}

int bluetooth_a2dp_playback_send_data(void *audioBuffer, int bufferLen)
{
    if (player_func.player_push_data_cb)
    {
        if (dec_audio_dump_conf && dec_audio_dump_conf->dec_dump_enable)
        {
            bluetooth_a2dp_dump_data((CHAR *)audioBuffer, bufferLen, dec_audio_dump_conf->dec_dump_fp);
        }
        player_func.player_push_data_cb(audioBuffer, bufferLen);
    }
    else
    {
        BT_DBG_WARNING(BT_DEBUG_A2DP, "player_push_data_cb func is null!");
        return BT_ERR_STATUS_FAIL;
    }
    return BT_SUCCESS;
}

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

INT32 bluetooth_a2dp_sink_enable(UINT8 sink_enable)
{
    FUNC_ENTRY;
    set_bt_enabled_a2dp_role(FALSE, sink_enable);
    return linuxbt_a2dp_sink_enable_handler(sink_enable);
}

INT32 bluetooth_a2dp_sink_open_cb(VOID)
{
    FUNC_ENTRY;

#if ENABLE_SYMPHONY
    bluetooth_a2dp_2nd_con_enable(1);
#endif

    if (get_bt_a2dp_connect())
    {

        /*Reset flag*/

        //set_bt_mode(BT_MODE_PLAYING);
        set_bt_connect(TRUE);
        //BtAppCbk(BT_PLAYING_MODE);
        //bluetooth_sdp_request(bt_last_device_addr);
        BtAppCbk(BT_A2DP_CONNECTION_SUCCESS, bt_last_device_addr);
        BtAppCbk(BT_CONNECT_SUCCESS, bt_last_device_addr);
        _bt_update_dev_history(&g_bt_target_dev_info, g_pt_paired_src_dev_history);
    }
    /* for send a2dp connection successful event */
    BT_DBG_INFO(BT_DEBUG_A2DP, "send BT_A2DP_CONNECTION_SUCCESS event");

#if DISPATCH_A2DP_WITH_PLAYBACK
    if (get_bt_pts_enable())
    {
        BT_DBG_NOTICE(BT_DEBUG_A2DP, "PTS test,need playback init here");
        if(NULL != player_func.player_init_cb)
        {
            player_func.player_init_cb(44100, 2);
        }
        else
        {
            BT_DBG_WARNING(BT_DEBUG_COMM, "player_init_cb func is null!");
        }
    }

#else
    if (NULL != player_func.player_init_cb)
    {
        player_func.player_init_cb(44100, 2);
    }
    else
    {
        BT_DBG_WARNING(BT_DEBUG_COMM, "player_init_cb func is null!");
    }
#endif

#if ENABLE_BUFFER_CONTROL
    initMemoryForBufferControl();
#endif

    return BT_SUCCESS;
}

INT32 bluetooth_a2dp_sink_start_cb(VOID)
{
    FUNC_ENTRY;

    set_bt_stream_suspend(FALSE);
    if (NULL != player_func.player_start_cb)
    {
        player_func.player_start_cb();
    }
    else
    {
        BT_DBG_WARNING(BT_DEBUG_COMM, "player_start_cb func is null!");
    }

    return 0;
}

INT32 bluetooth_a2dp_sink_suspend_cb(VOID)
{
    FUNC_ENTRY;
    set_bt_stream_suspend(TRUE);
    if (NULL != player_func.player_pause_cb)
    {
        player_func.player_pause_cb();
    }
    else
    {
        BT_DBG_WARNING(BT_DEBUG_COMM, "player_pause_cb func is null!");
    }

    return 0;
}


VOID bluetooth_a2dp_sink_reconfig_cb(UINT32 freq, UINT8 channel)
{
    //bluetooth_a2dp_reconfig_pb_conf(&bt_current_config);
}

INT32 bluetooth_a2dp_sink_close_cb(CHAR *device_addr)
{

    if (NULL != player_func.player_deinit_cb)
    {
        player_func.player_deinit_cb();
    }
    else
    {
        BT_DBG_WARNING(BT_DEBUG_COMM, "player_deinit_cb func is null!");
    }

    if (!get_bt_2nd_connect_comming())
    {
        if (BT_MODE_PLAYING == get_bt_mode())
        {
            BT_DBG_NOTICE(BT_DEBUG_A2DP, "from PLAYING mode");
            if (get_bt_avrcp_support())
            {
                BT_DBG_NOTICE(BT_DEBUG_A2DP, "support avrcp");
                /* avrcp connection have been disconnected */
                if (!(get_bt_a2dp_connect() || get_bt_avrcp_connect()))
                {
                    BT_DBG_NOTICE(BT_DEBUG_A2DP, "a2dp and avrcp down");
                    set_bt_connect(FALSE);
                    /*avoid to result bt_mode execption while simultaneously pressing HOME and disconnecting from SRC device*/
                    if (BT_MODE_PLAYING != get_bt_mode())
                    {
                        BT_DBG_NOTICE(BT_DEBUG_A2DP, "Bt_mode has changed into non-playing mode, it can not be set pairing mode");
                    }
                    else
                    {
                        set_bt_mode(BT_MODE_PAIRING);
                    }
                    BtAppCbk(BT_DISCONNECTED, device_addr);
                }
                else
                {
                    BT_DBG_INFO(BT_DEBUG_A2DP, "avrcp disconnection does not down");
                }
            }
            else
            {
                if (!get_bt_a2dp_connect())
                {
                    BT_DBG_NOTICE(BT_DEBUG_A2DP, "not support avrcp, and a2dp down");
                    set_bt_connect(FALSE);
                    /*avoid to result bt_mode execption while simultaneously pressing HOME and disconnecting from SRC device*/
                    if (BT_MODE_PLAYING != get_bt_mode())
                    {
                        BT_DBG_NOTICE(BT_DEBUG_A2DP, "Bt_mode has changed into non-playing mode, it can not be set pairing mode");
                    }
                    else
                    {
                        set_bt_mode(BT_MODE_PAIRING);
                    }
                    BtAppCbk(BT_DISCONNECTED, device_addr);
                }
            }
        }
        else if (BT_MODE_CLOSE == get_bt_mode())
        {
            BT_DBG_NOTICE(BT_DEBUG_A2DP, "from CLOSE mode");
            if (get_bt_avrcp_support())
            {
                BT_DBG_NOTICE(BT_DEBUG_A2DP, "support avrcp");
                /* avrcp connection have been disconnected */
                if (!(get_bt_a2dp_connect() || get_bt_avrcp_connect()))
                {
                    BT_DBG_NOTICE(BT_DEBUG_A2DP, "a2dp and avrcp down");
                    set_bt_connect(FALSE);
                    /*avoid to result bt_mode execption while simultaneously disconnect from phone and press HOME key*/
                    BT_DBG_NOTICE(BT_DEBUG_A2DP, "From close mode,it should not be change to pairing mode");
                    //set_bt_mode(BT_MODE_PAIRING);
                    BtAppCbk(BT_DISCONNECTED, device_addr);
                }
                else
                {
                    BT_DBG_NOTICE(BT_DEBUG_A2DP, "avrcp disconnection does not down");
                }
            }
            else
            {
                if (!get_bt_a2dp_connect())
                {
                    BT_DBG_NOTICE(BT_DEBUG_A2DP, "not support avrcp, and a2dp down");
                    set_bt_connect(FALSE);
                    /*avoid to result bt_mode execption while simultaneously disconnect from phone and press HOME key*/
                    BT_DBG_NOTICE(BT_DEBUG_A2DP, "From close mode,it should not be change to pairing mode");
                    //set_bt_mode(BT_MODE_PAIRING);
                    BtAppCbk(BT_DISCONNECTED, device_addr);
                }
            }
        }
        else
        {
            BT_DBG_NOTICE(BT_DEBUG_A2DP, "not at PLAYING or CLOSE mode, no need send disconnection notify to APP");
        }
    }
    else
    {
        BT_DBG_NOTICE(BT_DEBUG_A2DP, "bt_2nd_device_addr = %s", bt_2nd_device_addr);
        if ((NULL != device_addr)
                && (0 == strncmp(bt_2nd_device_addr, device_addr, strlen(device_addr))))
        {
            BT_DBG_NOTICE(BT_DEBUG_A2DP, "2nd connection disconnect, send disconnect msg to app");
            /*avoid to result bt_mode execption while simultaneously pressing HOME and disconnecting from SRC device*/
            if (BT_MODE_PLAYING != get_bt_mode())
            {
                BT_DBG_NOTICE(BT_DEBUG_A2DP, "Bt_mode has changed into non-playing mode, it can not be set pairing mode");
            }
            else
            {
                set_bt_mode(BT_MODE_PAIRING);
            }
            BtAppCbk(BT_2ND_CONNECT_FAIL, device_addr);
        }
        else
        {
            BT_DBG_NOTICE(BT_DEBUG_A2DP, "for 2nd connection, no need send disconnet msg to app");
            //BtAppCbk(BT_2ND_CONNECT_COMMING);
        }
    }
#if ENABLE_BUFFER_CONTROL
    uninitMemoryForBufferControl();
#endif
    return BT_SUCCESS;
}

VOID bluetooth_a2dp_sink_prepareDump(const BT_AUDIO_CODEC_TYPE audio_codec_type)
{
    CHAR *codec_type_name =NULL;
    CHAR udisk_path[MAX_NAME_STR_LEN] = "/tmp";
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




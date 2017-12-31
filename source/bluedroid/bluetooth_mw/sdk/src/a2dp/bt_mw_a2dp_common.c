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
/* FILE NAME:  bt_mw_a2dp_common.c
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

#include "bluetooth.h"
#include "bt_mw_common.h"
#include "bluetooth_sync.h"
#include "bt_mw_msg.h"
#include "linuxbt_a2dp_if.h"
#include "linuxbt_avrcp_if.h"

#include "linuxbt_common.h"

#include "bt_mw_a2dp_common.h"
#include "bt_mw_devices_info.h"
#include "bt_mw_avrcp.h"

#if ENABLE_A2DP_SINK
#include "bt_mw_a2dp_snk.h"
#endif
#if ENABLE_A2DP_SRC
#include "bt_mw_a2dp_src.h"
//#include "bt_a2dp_uploader.h"
#endif

#if ENABLE_BUFFER_CONTROL
#include "bt_a2dp_buffer_control.h"

#endif



#if ENABLE_LDAC_CODEC
#include "bt_a2dp_ldac.h"
/*In A2DP stream open/start, the sample rate is save into this various*/
UINT8 fg_bt_ldac_sample_rate;
/*the sample rate which ldac decode configuration has configurated*/
UINT8 fg_bt_ldac_config_sample_rate = 0;
#endif


#if ENABLE_A2DP_SRC
extern BT_TARGET_DEV_LIST *g_pt_paired_sink_dev_history;
#endif

BT_AUDIO_CODEC_TYPE g_current_codec_type = BT_AUDIO_SBC;

UINT8 g_scms_t_value = 0;

/*BT connection is triggered by SNK device(opposite device), but it only trigger AVDTP OPEN, not AVDTP START, platform should trigger AVDTP START*/
extern UINT8 fg_bt_avdtp_start_ind_is_triggered;


extern BtAppEventCbk      BtAppCbk;
extern BtAppA2dpResetAudioCbk AppResetAudioCbk;

/*a2dp disconnect 0x1, avrcp disconnect 0x2, acl disconnect 0x4*/
//UINT8 u1_need_notify_app = 0;
CHAR bt_last_device_addr[18];
CHAR bt_2nd_device_addr[18];
//static sys_enable_t log_enable = DISABLED;
//btav_audio_cap_struct bt_current_config;
BT_TARGET_DEV_INFO g_bt_target_dev_info;




INT32 bluetooth_a2dp_disconnect(CHAR *addr, UINT8 local_role)
{
    FUNC_ENTRY;
    BT_CHECK_POINTER(BT_DEBUG_A2DP, addr);
    if ((strlen(addr) > BT_ADDR_STR_MAX_LEN))
    {
        BT_DBG_ERROR(BT_DEBUG_A2DP, "invalid parameter");
        return BT_ERR_STATUS_PARM_INVALID;
    }

    return linuxbt_a2dp_disconnect_handler(addr, local_role);
} /*bluetooth_a2dp_disconnect*/


INT32 bluetooth_a2dp_disconnect_sync(VOID)
{
    FUNC_ENTRY;
    UINT32 ui4_loop = 10;

    if (!get_bt_a2dp_be_sink())
    {
        bluetooth_a2dp_disconnect(bt_last_device_addr, A2DP_LOCAL_ROLE_SRC);
    }
    else
    {
        bluetooth_a2dp_disconnect(bt_last_device_addr, A2DP_LOCAL_ROLE_SNK);
    }
    BOOL temp_role;
    /*wait for a2dp disconnect successful event*/
    ui4_loop = 40;//15;
    temp_role = get_bt_a2dp_be_sink();
    while (!get_bt_stream_closed() && (0 < ui4_loop))
    {
        /*A2DP role change, in open_cb, it will results to reset fg_bt_stream_closed*/
        if ((!get_bt_stream_closed()) && (temp_role != get_bt_a2dp_be_sink()))
        {
            BT_DBG_NOTICE(BT_DEBUG_A2DP, "A2DP Role changed, no need wait");
            return BT_SUCCESS;//means disconnected successful
        }
        BT_DBG_NOTICE(BT_DEBUG_A2DP, "ui4_loop:%ld,fg_bt_stream_closed:%d!",
                      (long)ui4_loop,
                      get_bt_stream_closed()
                     );
        /*x_thread_delay(500);*/
        usleep(500*1000);
        ui4_loop --;
    }
    if (!get_bt_stream_closed())
    {
        BT_DBG_ERROR(BT_DEBUG_A2DP, "can't disconnect a2dp");
        return BT_ERR_STATUS_PARM_INVALID;
    }
    return BT_SUCCESS;
} /*bluetooth_a2dp_disconnect_sync*/


INT32 bluetooth_a2dp_connect(CHAR *addr, UINT8 local_role)
{
    FUNC_ENTRY;
    INT32 i4_ret = -1;

    BT_CHECK_POINTER(BT_DEBUG_A2DP, addr);
    if ((strlen(addr) > BT_ADDR_STR_MAX_LEN))
    {
        BT_DBG_ERROR(BT_DEBUG_A2DP, "invalid parameter");
        return BT_ERR_STATUS_PARM_INVALID;
    }
    BT_DBG_NOTICE(BT_DEBUG_A2DP, "A2DP %s", !get_bt_a2dp_be_sink()?"SOURCE":"SINK");
    i4_ret = linuxbt_a2dp_connect_int_handler(addr, local_role);

    return i4_ret;
}


INT32 bluetooth_a2dp_start(CHAR *addr)
{
    FUNC_ENTRY;

    BT_CHECK_POINTER(BT_DEBUG_A2DP, addr);
    if ((strlen(addr) > BT_ADDR_STR_MAX_LEN))
    {
        BT_DBG_ERROR(BT_DEBUG_A2DP, "invalid parameter");
        return BT_ERR_STATUS_PARM_INVALID;
    }

    if (get_bt_a2dp_pause())
    {
        set_bt_a2dp_pause(FALSE);
        return linuxbt_a2dp_start_handler(addr);
    }
    else
    {
        BT_DBG_NOTICE(BT_DEBUG_A2DP, "a2dp have started, no need start again");
    }

    return BT_SUCCESS;
} /*bluetooth_a2dp_start*/


INT32 bluetooth_a2dp_suspend(CHAR *addr)
{
    FUNC_ENTRY;

    BT_CHECK_POINTER(BT_DEBUG_A2DP, addr);
    if ((strlen(addr) > BT_ADDR_STR_MAX_LEN))
    {
        BT_DBG_ERROR(BT_DEBUG_A2DP, "invalid parameter");
        return BT_ERR_STATUS_PARM_INVALID;
    }

    if (get_bt_a2dp_pause())
    {
        BT_DBG_NOTICE(BT_DEBUG_A2DP, "a2dp have suspended, no need suspend again");
    }
    else
    {
        set_bt_a2dp_pause(TRUE);
        return linuxbt_a2dp_suspend_handler(addr);
    }


    return BT_SUCCESS;
} /*bluetooth_a2dp_suspend*/


INT32 bluetooth_a2dp_abort(CHAR *addr)
{
    FUNC_ENTRY;

    BT_CHECK_POINTER(BT_DEBUG_A2DP, addr);
    if ((strlen(addr) > BT_ADDR_STR_MAX_LEN))
    {
        BT_DBG_ERROR(BT_DEBUG_A2DP, "invalid parameter");
        return BT_ERR_STATUS_PARM_INVALID;
    }

    return linuxbt_a2dp_abort_handler(addr);
} /*bluetooth_a2dp_abort*/

INT32 bluetooth_a2dp_reconfig(CHAR *addr,
                              INT32 sample_rate,
                              INT32 channel_num)
{
    FUNC_ENTRY;

    BT_CHECK_POINTER(BT_DEBUG_A2DP, addr);
    if ((strlen(addr) > BT_ADDR_STR_MAX_LEN))
    {
        BT_DBG_ERROR(BT_DEBUG_A2DP, "invalid parameter");
        return BT_ERR_STATUS_PARM_INVALID;
    }

    return linuxbt_a2dp_reconfig_handler(addr, sample_rate, channel_num);
} /*bluetooth_a2dp_reconfig*/


INT32 bluetooth_a2dp_aac_enable(UINT8 fg_enable)
{
    FUNC_ENTRY;
    set_bt_enabled_codec_type(BT_AUDIO_AAC, fg_enable);
    BT_DBG_NORMAL(BT_DEBUG_A2DP, "AAC codec :%s", fg_enable?"enable":"disable");

    return linuxbt_a2dp_aac_enable_handler(fg_enable);
}

INT32 bluetooth_a2dp_ldac_enable(UINT8 fg_enable)
{
#if ENABLE_LDAC_CODEC
    FUNC_ENTRY;
    set_bt_enabled_codec_type(BT_AUDIO_LDAC, fg_enable);
    BT_DBG_NORMAL(BT_DEBUG_A2DP, "LDAC codec :%s", fg_enable?"enable":"disable");

    return linuxbt_a2dp_ldac_enable_handler(fg_enable);
#else
    BT_DBG_ERROR(BT_DEBUG_A2DP, "not support ldac");
    return BT_ERR_STATUS_UNSUPPORTED;
#endif
}


INT32 bluetooth_a2dp_aptx_enable(UINT8 fg_enable)
{
#if ENABLE_APTX_CODEC
    FUNC_ENTRY;

    return linuxbt_a2dp_aptx_enable_handler(fg_enable);
#else
    BT_DBG_ERROR(BT_DEBUG_A2DP, "not support aptx");
    return BT_ERR_STATUS_UNSUPPORTED;
#endif
}

INT32 bluetooth_a2dp_2nd_con_enable(UINT8 fg_enable)
{
    FUNC_ENTRY;

    return linuxbt_a2dp_2nd_con_enable_handler(fg_enable);
}

INT32 bluetooth_a2dp_get_audio_cap_info(VOID)
{
    //BT_CHECK_POINTER(BT_DEBUG_A2DP, ptcurrent_config);
    //memcpy(ptcurrent_config, &bt_current_config, sizeof(bt_current_config));
    return BT_SUCCESS;
}
VOID bluetooth_a2dp_save_audio_cap_info(UINT32 sample_rate, UINT8 channel_count)
{
    FUNC_ENTRY;

    if (!get_bt_a2dp_be_sink())
    {
#if ENABLE_A2DP_SRC
        //bluetooth_a2dp_src_prepareDump(bluetooth_a2dp_getCurrentCodecType());
#endif
    }
    else
    {
#if ENABLE_A2DP_SINK
        //bluetooth_a2dp_sink_prepareDump(bluetooth_a2dp_getCurrentCodecType());
#endif
    }
}

VOID bluetooth_a2dp_get_cur_audio_info(UINT32 *u4SampleRate, UINT32 *u4ChannelCnt)
{
    BT_DBG_MINOR(BT_DEBUG_A2DP, "into+++ %s", __FUNCTION__);

    *u4SampleRate = linuxbt_a2dp_get_sample_rate();
    *u4ChannelCnt = linuxbt_a2dp_get_channel_num();
}


INT32 bluetooth_a2dp_get_sbc_info(UINT32 *u4SampleRate, UINT32 *u4ChannelCnt, UINT32 *u4Bitdepth)
{
    BT_DBG_MINOR(BT_DEBUG_A2DP, "into+++ %s", __FUNCTION__);
#if 0
    btav_audio_cap_struct *pcurrent_config = &bt_current_config;
    btav_audio_sbc_codec_cap_struct *pSbc_conf = NULL;

    if (BT_AUDIO_SBC == bluetooth_a2dp_getCurrentCodecType())
    {
        BT_DBG_NOTICE(BT_DEBUG_A2DP, "BT_A2DP_SBC");
        pSbc_conf = &pcurrent_config->codec_cap.sbc;

        switch (pSbc_conf->sample_rate)
        {
        case SBC_48000:
        {
            *u4SampleRate = 48000;
            break;
        }
        case SBC_32000:
        {
            *u4SampleRate = 32000;
            break;
        }
        case SBC_16000:
        {
            *u4SampleRate = 16000;
            break;
        }
        case SBC_44100:
        {
            *u4SampleRate = 44100;
            break;
        }
        default:
        {
            BT_DBG_ERROR(BT_DEBUG_A2DP, "sampling rate not support --> %d  ", pSbc_conf->sample_rate);
            break;
        }
        }

        switch (pSbc_conf->block_len)
        {
        case BLOCK_LENGTH_16:
        {
            *u4Bitdepth = 16;
            break;
        }
        case BLOCK_LENGTH_8:
        {
            *u4Bitdepth = 8;
            break;
        }
        default:
        {
            BT_DBG_ERROR(BT_DEBUG_A2DP, "bitPerSample not support --> %d  ", pSbc_conf->block_len);
            break;
        }
        }

        if (pSbc_conf->channel_mode == SBC_CHANNEL_MODE_MONO)
        {
            *u4ChannelCnt = 1;
        }
        else
        {
            *u4ChannelCnt = 2;
        }
        BT_DBG_NOTICE(BT_DEBUG_A2DP, "fs:%ld channel num:%ld bitdepth:%ld", (long)*u4SampleRate, (long)*u4ChannelCnt, (long)*u4Bitdepth);

    }
    else
    {
        BT_DBG_ERROR(BT_DEBUG_A2DP, "Not SBC codec");
        return BT_ERR_STATUS_FAIL;
    }
#endif
    return BT_SUCCESS;
}



INT32 bluetooth_connect_audio_sources(CHAR *addr, audio_conn_type_t type)
{
    FUNC_ENTRY;

    BT_A2DP_ROLE a2dp_role;

    BT_CHECK_POINTER(BT_DEBUG_A2DP, addr);
    if ((strlen(addr) > BT_ADDR_STR_MAX_LEN))
    {
        BT_DBG_ERROR(BT_DEBUG_A2DP, "invalid parameter");
        return BT_ERR_STATUS_PARM_INVALID;
    }
    if (!get_bt_a2dp_be_sink())
    {
        a2dp_role = A2DP_LOCAL_ROLE_SRC;
    }
    else
    {
        a2dp_role = A2DP_LOCAL_ROLE_SNK;
    }

    if (type == CONN_TYPE_A2DP)
    {
        BT_DBG_NOTICE(BT_DEBUG_A2DP, "<<< call BtA2dpOpenStream(%s,%d) >>>", addr, a2dp_role);
        return linuxbt_a2dp_connect_int_handler(addr, a2dp_role);
    }
    else
    {
        BT_DBG_NOTICE(BT_DEBUG_A2DP, "<<< call BtAvrcpConnectReq(%s) >>>", addr);
        return linuxbt_rc_connect_handler(addr);
    }
} /*bluetooth_connect_audio_sources*/



INT32 bt_launch_connection_sync(CHAR *addr, audio_conn_type_t type)
{
    FUNC_ENTRY;
    UINT32 ui4_loop = 10;
    /*bluedroid cannat not establish AVRCP connection upper MW*/
    if (CONN_TYPE_AVCTP == type)
    {
        bluetooth_connect_audio_sources(addr, CONN_TYPE_AVCTP);
        do
        {
            BT_DBG_NOTICE(BT_DEBUG_A2DP, "ui4_loop:%ld,avrcp_connect:%d!",
                          (long)ui4_loop,
                          get_bt_avrcp_connect()
                         );
            /*x_thread_delay(500);*/
            usleep(500*1000);
            ui4_loop --;
        }
        while (!get_bt_avrcp_connect() && (0 < ui4_loop));
        if (!get_bt_avrcp_connect())
        {
            BT_DBG_ERROR(BT_DEBUG_A2DP, "can't establish AVRCP connection");
            return BT_ERR_STATUS_FAIL;
        }
    }
    /* A2DP connection*/
    else
    {
        bluetooth_connect_audio_sources(addr, CONN_TYPE_A2DP);
        do
        {
            BT_DBG_NOTICE(BT_DEBUG_A2DP, "ui4_loop:%ld,a2dp_connect:%d!",
                          (long)ui4_loop,
                          get_bt_a2dp_connect()
                         );
            /*x_thread_delay(500);*/
            usleep(500*1000);
            ui4_loop --;
        }
        while (!get_bt_a2dp_connect() && (0 < ui4_loop));
        if (!get_bt_a2dp_connect())
        {
            BT_DBG_ERROR(BT_DEBUG_A2DP, "can't establish A2DP connection");
            return BT_ERR_STATUS_FAIL;
        }
    }

    FUNC_EXIT;
    return BT_SUCCESS;
}

BOOL bt_audio_is_stream_suspend(VOID)
{
    BOOL fg_stream_suspend;
    fg_stream_suspend = get_bt_stream_suspend();

    BT_DBG_MINOR(BT_DEBUG_A2DP, "stream %s", fg_stream_suspend?"suspend":"start");
    return fg_stream_suspend;
} /*bt_audio_get_abortrsp_mode*/

INT32 bt_audio_set_stream_suspend(BOOL fg_suspend)
{
    set_bt_stream_suspend(fg_suspend);
    BT_DBG_INFO(BT_DEBUG_A2DP, "have set stream %s", fg_suspend?"suspend":"start");
    return BT_SUCCESS;
} /*bt_audio_get_abortrsp_mode*/



VOID btaudio_handle_a2dp_stream_qos_ind(UINT8 qos)
{
#if ENABLE_LDAC_CODEC
    INT32 ldac_quality_counter = 0;

    ldac_quality_counter += qos;

    if (BT_QUALITY_AUTO == g_e_audio_quality)
    {
        if ( ldac_quality_counter == 0 )
        {
            BT_DBG_MINOR(BT_DEBUG_A2DP, "Stream QoS, Audio Quality : Up");
            g_ldac_quality_auto_mode.i2audio_data_stream_quality_change = 1;
        }
        else if ( ldac_quality_counter <= 6 )
        {
            BT_DBG_MINOR(BT_DEBUG_A2DP, "Stream QoS, Audio Quality : Same");
            g_ldac_quality_auto_mode.i2audio_data_stream_quality_change = 0;
        }
        else
        {
            BT_DBG_MINOR(BT_DEBUG_A2DP, "Stream QoS, Audio Quality : Down");
            g_ldac_quality_auto_mode.i2audio_data_stream_quality_change = -1;
        }
    }
#endif
    return;
}


/*call by MW&wrap*/
EXPORT_SYMBOL VOID bt_print_codec_info(BT_AUDIO_CODEC_TYPE i4current_codec_type)
{
    switch (i4current_codec_type)
    {
    case BT_AUDIO_SBC:
    {
        BT_DBG_MINOR(BT_DEBUG_A2DP, "BT_AUDIO_SBC");
        break;
    }
    case BT_AUDIO_AAC:
    {
        BT_DBG_MINOR(BT_DEBUG_A2DP, "BT_AUDIO_AAC");
        break;
    }
    case BT_AUDIO_LDAC:
    {
        BT_DBG_MINOR(BT_DEBUG_A2DP, "BT_AUDIO_LDAC");
        break;
    }
    default:
    {
        BT_DBG_NOTICE(BT_DEBUG_A2DP, "unknow codec");
        break;
    }
    }
}



BT_AUDIO_CODEC_TYPE btaudio_get_codec_type(VOID)
{
    bt_print_codec_info(BT_AUDIO_SBC);
    return BT_AUDIO_SBC;
}

UINT8 btaudio_get_enabled_codec(VOID)
{
    return get_bt_enabled_codec_type();
}

INT32 btaudio_get_current_target_dev_info(BT_TARGET_DEV_INFO *pt_target_dev_info)
{
    BT_CHECK_POINTER(BT_DEBUG_A2DP, pt_target_dev_info);

    /*fix BDP00443445(X2 show UI without no name):1.power on 2.conneting 3.then power off before connection successful 4. stack notify MW connection successful
    5.APP layer receive it and then get target info, notify to X2*/
    if (1)//get_bt_a2dp_connect()
    {
        memcpy(pt_target_dev_info, &g_bt_target_dev_info, sizeof(BT_TARGET_DEV_INFO));
        BT_DBG_NOTICE(BT_DEBUG_A2DP, "current device address:%s", pt_target_dev_info->bdAddr);
        BT_DBG_NOTICE(BT_DEBUG_A2DP, "current device name:%s", pt_target_dev_info->name);
    }
    else
    {
        BT_DBG_NOTICE(BT_DEBUG_A2DP, "does not create connection");
        return BT_ERR_STATUS_NOT_READY;
    }
    return BT_SUCCESS;
}

INT32 btaudio_handle_a2dp_stream_open_cb(CHAR *device_addr)
{
    FUNC_ENTRY;

    BT_DEV_MAPPING_INFO target_mapping_dev;
    BT_MSG_T bt_a2dp_msg;

    if (NULL == device_addr)
    {
        BT_DBG_ERROR(BT_DEBUG_A2DP, "null pointer");
        return BT_ERR_STATUS_NULL_POINTER;
    }
    /***reset the global flags***/
    set_bt_a2dp_support(TRUE);
    set_bt_stream_suspend(TRUE);
    set_bt_2nd_connect_comming(FALSE);
    //memset(&bt_current_config, 0, sizeof(bt_current_config));
    //memset(&g_bt_target_dev_info, 0, sizeof(BT_TARGET_DEV_INFO));
    memset(&bt_last_device_addr, 0, sizeof(bt_last_device_addr));
    strncpy(bt_last_device_addr,
            device_addr,
            sizeof(bt_last_device_addr)
           );
    strncpy(g_bt_target_dev_info.bdAddr,
            device_addr,
            sizeof(g_bt_target_dev_info.bdAddr)
           );
    strncpy(target_mapping_dev.bdAddr,
            device_addr,
            sizeof(target_mapping_dev.bdAddr)
           );
    if (0 == bluetooth_get_dev_mapping_name(&target_mapping_dev))
    {
        memcpy(g_bt_target_dev_info.name, target_mapping_dev.name, sizeof(g_bt_target_dev_info.name));
        g_bt_target_dev_info.cod = target_mapping_dev.cod;
    }
    else
    {
        BT_DBG_ERROR(BT_DEBUG_A2DP, "addr:%s, Not Found Name", bt_last_device_addr);
        //memcpy(g_bt_target_dev_info.name, "no name", sizeof(g_bt_target_dev_info.name));
    }


    BT_DBG_NORMAL(BT_DEBUG_A2DP, "a2dp stream open addr:%s", bt_last_device_addr);


    memset(bt_2nd_device_addr, 0, sizeof(bt_2nd_device_addr));
    set_bt_stream_closed(FALSE);
    set_bt_a2dp_connect(TRUE);

    bt_a2dp_msg.tMsgType = BT_MSG_FROM_MW;
    bt_a2dp_msg.body.evtType = BLUETOOTH_A2DP_EVENT_STREAM_OPEN_CNF;
    i4SendMsg(&bt_a2dp_msg);

    return BT_SUCCESS;
}

INT32 btaudio_handle_a2dp_stream_start_cb(VOID)
{
    FUNC_ENTRY;

    if (get_bt_a2dp_be_sink())
    {
#if ENABLE_A2DP_SINK
        /*act as A2DP SINK*/
        if (get_bt_stream_suspend())
        {
            return bluetooth_a2dp_sink_start_cb();
        }
        else
        {
            /*a2dp stream started before this point*/
            BT_DBG_NOTICE(BT_DEBUG_A2DP, "a2dp stream started before this point");
        }
#endif
    }
    else
    {
#if ENABLE_A2DP_SRC
        /*act as A2DP SRC*/
        bluetooth_a2dp_src_start_cb();
#endif
    }

    return 0;
}


INT32 btaudio_handle_a2dp_stream_suspend_cb(VOID)
{
    FUNC_ENTRY;

    if (get_bt_a2dp_be_sink())
    {
#if ENABLE_A2DP_SINK
        /*act as A2DP SINK*/
        if (get_bt_a2dp_connect())
        {
            return bluetooth_a2dp_sink_suspend_cb();
        }
        else
        {
            /*a2dp stream suspened before this point*/
            BT_DBG_ERROR(BT_DEBUG_A2DP, "a2dp stream suspened before this point");
        }
#endif
    }
    else
    {
#if ENABLE_A2DP_SRC
        /*act as A2DP SRC*/
        bluetooth_a2dp_src_suspend_cb();
#endif
    }

    return BT_SUCCESS;
}


INT32 btaudio_handle_a2dp_stream_reconfig_cb(UINT32 freq, UINT8 channel)
{
    FUNC_ENTRY;

    BT_DBG_INFO(BT_DEBUG_A2DP, "I'm %s", !get_bt_a2dp_be_sink()?"SRC":"SINK");
    if (!get_bt_a2dp_be_sink())//msg->result == BT_A2DP_RESULT_OK
    {
#if ENABLE_A2DP_SRC
        /*act as A2DP SRC*/
        bluetooth_a2dp_src_reconfig_cb(freq, channel);
#endif
    }
    else
    {
#if ENABLE_A2DP_SINK
        /*act as A2DP SINK*/
        bluetooth_a2dp_sink_reconfig_cb(freq, channel);
#endif
    }
    return BT_SUCCESS;
}

INT32 btaudio_handle_a2dp_scmst_support_cb(BOOL bt_scmst_support)
{
    BT_DBG_NOTICE(BT_DEBUG_A2DP, "SCMS-T Used : %s", bt_scmst_support?"TRUE":"FALSE");

    if (bt_scmst_support)
    {
        set_bt_support_scms_t(TRUE);
        g_scms_t_value = TRUE;
    }
    else
    {
        set_bt_support_scms_t(FALSE);
        g_scms_t_value = FALSE;
    }
    /*Target device don't support SCMS-T & local device is Japan mode should send zero data to SINK*/
    if (0)
    {
        set_bt_send_zero_data(TRUE);
    }
    else
    {
        set_bt_send_zero_data(FALSE);
    }
    BtAppCbk(BT_SUPPORT_SCMS_T, NULL);
    return BT_SUCCESS;
}

INT32 btaudio_handle_a2dp_second_connection_cb(CHAR *device_addr)
{
    FUNC_ENTRY;

    if (strlen(device_addr) < sizeof(bt_2nd_device_addr))
    {
        memcpy(bt_2nd_device_addr, device_addr, strlen(device_addr));
        bt_2nd_device_addr[strlen(device_addr)] = '\0';
    }
    else
    {
        memcpy(bt_2nd_device_addr, device_addr, sizeof(bt_2nd_device_addr));
        bt_2nd_device_addr[sizeof(bt_2nd_device_addr) - 1] = '\0';
    }
    BT_DBG_NORMAL(BT_DEBUG_A2DP, "bt_2nd_device_addr = %s", bt_2nd_device_addr);
    set_bt_2nd_connect_comming(TRUE);
    BtAppCbk(BT_2ND_CONNECT_COMMING, device_addr);
    return BT_SUCCESS;
}

INT32 btaudio_handle_a2dp_stream_close_cb(CHAR *device_addr)
{
    FUNC_ENTRY;
    BT_MSG_T bt_a2dp_msg;

    set_bt_a2dp_connect(FALSE);
    set_bt_stream_closed(TRUE);
    set_bt_stream_suspend(TRUE);
    /*Reset flag*/
    fg_bt_avdtp_start_ind_is_triggered = 0;

    bt_a2dp_msg.tMsgType = BT_MSG_FROM_MW;
    bt_a2dp_msg.body.evtType = BLUETOOTH_A2DP_EVENT_STREAM_CLOSED_CNF;
    i4SendMsg(&bt_a2dp_msg);

    return BT_SUCCESS;
}

INT32 bluetooth_a2dp_init(VOID )
{
    FUNC_ENTRY;

    if (!get_bt_a2dp_be_sink())
    {
        linuxbt_a2dp_src_init();
    }
    else
    {
        linuxbt_a2dp_sink_init();
    }
    return 0;
}

INT32 bluetooth_a2dp_deinit(VOID )
{
    FUNC_ENTRY;

    if (!get_bt_a2dp_be_sink())
    {
        linuxbt_a2dp_src_deinit();
    }
    else
    {
        linuxbt_a2dp_sink_deinit();
    }
    return BT_SUCCESS;
}


INT32 bt_get_last_device_addr(CHAR *addr)
{
    BT_CHECK_POINTER(BT_DEBUG_A2DP, addr);

    if (0 == strlen(bt_last_device_addr))
    {
        BT_DBG_ERROR(BT_DEBUG_A2DP, "can't get last device mac");
        return BT_ERR_STATUS_FAIL;
    }

    strncpy(addr,
            bt_last_device_addr,
            sizeof(bt_last_device_addr)
           );
    BT_DBG_NORMAL(BT_DEBUG_A2DP, "addr:%s", bt_last_device_addr);
    return BT_SUCCESS;
}

EXPORT_SYMBOL INT32 bluetooth_a2dp_getUdiskPath(CHAR *szUPath)
{
    if (0 == access("/mnt/sda1", F_OK))
    {
        strncpy(szUPath, "/mnt/sda1", MAX_NAME_STR_LEN-1);
    }
    else if (0 == access("/mnt/sdb1", F_OK))
    {
        strncpy(szUPath, "/mnt/sdb1", MAX_NAME_STR_LEN-1);
    }
    else if (0 == access("/mnt/sda", F_OK))
    {
        strncpy(szUPath, "/mnt/sda", MAX_NAME_STR_LEN-1);
    }
    else if (0 == access("/mnt/sdb", F_OK))
    {
        strncpy(szUPath, "/mnt/sdb", MAX_NAME_STR_LEN-1);
    }
    else if (0 == access(BT_TMP_PATH, F_OK))
    {
        strncpy(szUPath, BT_TMP_PATH, MAX_NAME_STR_LEN-1);
    }
    else
    {
        BT_DBG_ERROR(BT_DEBUG_A2DP, "path error");
        return BT_ERR_STATUS_FAIL;
    }

    return BT_SUCCESS;
}

EXPORT_SYMBOL INT32 bluetooth_a2dp_dump_data(CHAR *pbuf, INT32 i4length, FILE *pfp)
{
    INT32 i4_count = 0;

    BT_CHECK_POINTER(BT_DEBUG_A2DP, pbuf);
    BT_CHECK_POINTER(BT_DEBUG_A2DP, pfp);
    if (0 >= i4length)
    {
        BT_DBG_ERROR(BT_DEBUG_A2DP, "invalid length parameter");
        return BT_ERR_STATUS_PARM_INVALID;
    }
    /*dump a2dp data to file*/
    BT_DBG_MINOR(BT_DEBUG_A2DP, "try to dump %ld data to file", (long)i4length);
    i4_count = fwrite(pbuf, 1, i4length, pfp);
    if (i4_count != i4length)
    {
        BT_DBG_ERROR(BT_DEBUG_A2DP, "fwrite error:%ld", (long)i4_count);
        return BT_ERR_STATUS_FAIL;
    }
    BT_DBG_MINOR(BT_DEBUG_A2DP, "success to dump %ld data to file", (long)i4length);

    return BT_SUCCESS;
}


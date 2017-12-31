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

/*-----------------------------------------------------------------------------
                            include files
-----------------------------------------------------------------------------*/
#include <stdio.h>

#include "mtk_bt_service_a2dp.h"
#include "c_bt_mw_a2dp_src.h"
#include "c_bt_mw_a2dp_snk.h"
#include "c_bt_mw_a2dp_common.h"

#define BT_RC_LOG(_stmt...) \
        do{ \
            if(0){    \
                printf("[A2DP IPC Server]Func:%s Line:%d--->: ", __FUNCTION__, __LINE__);   \
                printf(_stmt); \
                printf("\n"); \
            }        \
        }   \
        while(0)



INT32 x_mtkapi_a2dp_source_sink_on_off(BOOL fg_src_on, BOOL fg_sink_on)
{
    BT_RC_LOG("[.c][%s]", __FUNCTION__);
    return c_btm_source_sink_on_off(fg_src_on, fg_sink_on);
}

INT32 x_mtkapi_a2dp_src_set_audio_hw_dbg_lvl(UINT8 log_level)
{
    BT_RC_LOG("[.c][%s]", __FUNCTION__);
    return c_btm_a2dp_set_audio_hw_log_lvl(log_level);
}

INT32 x_mtkapi_a2dp_connect_audio_sources(char *addr, audio_conn_type_t type)
{
    BT_RC_LOG("[.c][%s]", __FUNCTION__);
    return c_btm_connect_audio_sources(addr, type);
}

INT32 x_mtkapi_a2dp_disconnect_audio_sources(char *addr, audio_conn_type_t type)
{
    BT_RC_LOG("[.c][%s]", __FUNCTION__);
    return c_btm_bt_disconnect_audio_sources(addr, type);
}

INT32 x_mtkapi_a2dp_send_stream_data(const CHAR *data, INT32 len)
{
    BT_RC_LOG("[.c][%s]", __FUNCTION__);
    return c_btm_a2dp_send_stream_data(data, len);
}

INT32 x_mtkapi_a2dp_get_role(VOID)
{
    BT_RC_LOG("[.c][%s]", __FUNCTION__);
    return c_btm_get_a2dp_role();
}

INT32 x_mtkapi_a2dp_stream_is_suspend(VOID)
{
    BT_RC_LOG("[.c][%s]", __FUNCTION__);
    return c_btm_stream_is_suspend();
}

INT32 x_mtkapi_a2dp_get_connect_status(VOID)
{
    BT_RC_LOG("[.c][%s]", __FUNCTION__);
    return c_btm_get_connect_status();
}

VOID x_mtkapi_a2dp_cmd(bt_a2dp_status_t status)
{
    BT_RC_LOG("[.c][%s]", __FUNCTION__);
    return c_btm_a2dp_cmd(status);
}

INT32 x_mtkapi_a2dp_get_paired_sink_dev_list(BT_TARGET_DEV_LIST *pt_device_list)
{
    BT_RC_LOG("[.c][%s]", __FUNCTION__);
    return c_btm_get_paired_sink_dev_list(pt_device_list);
}

INT32 x_mtkapi_a2dp_get_paired_src_dev_list(BT_TARGET_DEV_LIST *pt_device_list)
{
    BT_RC_LOG("[.c][%s]", __FUNCTION__);
    return c_btm_get_paired_src_dev_list(pt_device_list);
}

VOID x_mtkapi_a2dp_mute_device(UINT8 fg_mute)
{
    BT_RC_LOG("[.c][%s]", __FUNCTION__);
    return c_btm_mute_device(fg_mute);
}

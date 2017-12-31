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

#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include "c_mw_config.h"

#include "bt_mw_common.h"

#include "bluetooth_sync.h"
#include "bt_mw_a2dp_common.h"
#include "bluetooth_sync.h"
#include "linuxbt_avrcp_tg_if.h"
#include "c_bt_mw_a2dp_common.h"
#include "bt_mw_avrcp.h"
#include "bt_mw_devices_info.h"
#include "bt_mw_a2dp_src.h"

extern BT_TARGET_DEV_LIST *g_pt_paired_sink_dev_history;

/*SRC mode get sink device list*/
EXPORT_SYMBOL INT32 c_btm_get_paired_sink_dev_list(BT_TARGET_DEV_LIST *pt_device_list)
{
    FUNC_ENTRY;
    INT32 i4_ret;
    INT32 i;
    INT32 j;
    INT32 temp;
    BT_TARGET_DEV_LIST reversed_pt_device_list;

    i4_ret = bluetooth_get_device_history(&reversed_pt_device_list, g_pt_paired_sink_dev_history);
    if (BT_SUCCESS != i4_ret)
    {
        BT_DBG_ERROR(BT_DEBUG_A2DP, "NO paired sink device!");
        return BT_SUCCESS;
    }
    if (MAX_DEV_NUM < reversed_pt_device_list.dev_num)
    {
        BT_DBG_ERROR(BT_DEBUG_A2DP, "the device number is wrong:%ld!", (long)reversed_pt_device_list.dev_num);
        return BT_ERR_STATUS_FAIL;
    }
    memset(pt_device_list, 0, sizeof(BT_TARGET_DEV_LIST));
    pt_device_list->dev_num = reversed_pt_device_list.dev_num;
    /* i : the first node in device list*/
    i = reversed_pt_device_list.head_idx;
    /* temp : the lase node in device list*/
    temp = (i + reversed_pt_device_list.dev_num - 1) % MAX_DEV_NUM;
    /* j : the first node in app device list always is 0*/
    j = 0;
    BT_DBG_NORMAL(BT_DEBUG_A2DP, "num:%ld,idx:%ld", (long)pt_device_list->dev_num, (long)i);
    while (j < reversed_pt_device_list.dev_num)
    {

        memcpy(&pt_device_list->device_list[j],
               &reversed_pt_device_list.device_list[temp],
               sizeof(BT_TARGET_DEV_INFO)
              );
        BT_DBG_INFO(BT_DEBUG_A2DP, "device_list[%ld].name: %s, device_list[%ld].bdAddr: %s",
                      (long)j,
                      pt_device_list->device_list[j].name,
                      (long)j,
                      pt_device_list->device_list[j].bdAddr
                     );
        BT_DBG_INFO(BT_DEBUG_A2DP,"reversed_pt_device_list[%ld].name: %s, reversed_pt_device_list[%ld].bdAddr: %s",
                      (long)temp,
                      reversed_pt_device_list.device_list[temp].name,
                      (long)temp,
                      reversed_pt_device_list.device_list[temp].bdAddr
                     );
        i = (i + 1) % MAX_DEV_NUM;
        temp = (temp - 1 + MAX_DEV_NUM) % MAX_DEV_NUM;
        j++;
    }
    return BT_SUCCESS;
}

EXPORT_SYMBOL VOID c_btm_a2dp_cmd(bt_a2dp_status_t status)
{
    FUNC_ENTRY;
    CHAR bt_addr[18];
    if (BT_A2DP_START == status)
    {
        BT_DBG_NORMAL(BT_DEBUG_A2DP, "status:%s", "start");
    }
    else if (BT_A2DP_SUSPEND == status)
    {
        BT_DBG_NORMAL(BT_DEBUG_A2DP, "status:%s", "suspend");
    }
    /*for fix BDP00426890:Change from TX to RX, after power off, but APP also call c_btm_a2dp_cmd;if not return, it will result stack to hang up*/
    if (get_bt_is_call_power_off())
    {
        BT_DBG_NORMAL(BT_DEBUG_A2DP, "have call power off");
        return;
    }

    bt_get_last_device_addr(bt_addr);
    if (BT_A2DP_START == status)
    {
        if (get_bt_a2dp_pause())
        {
            UINT8 loop = 10;
            BT_DBG_NORMAL(BT_DEBUG_A2DP, "fg_bt_a2dp_pause_cnf:%d", get_bt_a2dp_pause_done());
            while (!get_bt_a2dp_pause_done() && loop > 0 && get_bt_a2dp_connect())
            {
                /*x_thread_delay(50);*/
                usleep(50*1000);
                BT_DBG_NORMAL(BT_DEBUG_A2DP, "Remained timer:%d", loop);
                loop --;
            }
            bluetooth_a2dp_start(bt_addr);
        }
        else
        {
            BT_DBG_INFO(BT_DEBUG_A2DP, "a2dp have started, no need start again");
        }
    }
    else if (BT_A2DP_SUSPEND == status)
    {
        if (get_bt_a2dp_pause())
        {
            BT_DBG_INFO(BT_DEBUG_A2DP, "a2dp have suspended, no need suspend again");
        }
        else
        {
            bluetooth_a2dp_suspend(bt_addr);
        }
    }
    else
    {
        BT_DBG_ERROR(BT_DEBUG_A2DP, "invalid a2dp status");
    }

}

EXPORT_SYMBOL INT32 c_btm_a2dp_send_stream_data(const CHAR *data, INT32 len)
{
    return bluetooth_a2dp_src_send_stream_data(data, len);
}

EXPORT_SYMBOL INT32 c_btm_a2dp_set_audio_hw_log_lvl(UINT8 log_level)
{
    return bluetooth_a2dp_set_audio_log_lvl(log_level);
}

EXPORT_SYMBOL INT32 c_btm_a2dp_register_uploader_cb(BT_AUDIO_SRC_CB_FUNC *func)
{
    return bluetooth_a2dp_register_upload_cb(func);
}

EXPORT_SYMBOL VOID c_btm_mute_device(UINT8 fg_mute)
{
    FUNC_ENTRY;

    if (0 == fg_mute)
    {
        set_bt_is_mute(FALSE);
    }
    else
    {
        set_bt_is_mute(TRUE);
    }
    BT_DBG_NORMAL(BT_DEBUG_A2DP, "the device %s mute\n", fg_mute?"is":"is not");
}



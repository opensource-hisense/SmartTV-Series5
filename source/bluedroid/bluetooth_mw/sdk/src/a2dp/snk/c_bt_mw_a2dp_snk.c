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
#include "bt_mw_a2dp_snk.h"
#include "bt_mw_devices_info.h"
#if ENABLE_LDAC_CODEC
#include "bt_a2dp_ldac.h"
#endif

extern BT_TARGET_DEV_LIST *g_pt_paired_src_dev_history;

EXPORT_SYMBOL INT32 c_btm_a2dp_register_player_cb(BT_PLAYER_SNK_CB_FUNC *func)
{
    return bluetooth_a2dp_register_player_cb(func);
}

/*SINK mode get src device list*/
EXPORT_SYMBOL INT32 c_btm_get_paired_src_dev_list(BT_TARGET_DEV_LIST *pt_device_list)
{
    FUNC_ENTRY;
    INT32 i4_ret;
    INT32 i;
    INT32 j;
    INT32 temp;
    BT_TARGET_DEV_LIST reversed_pt_device_list;
    i4_ret = bluetooth_get_device_history(&reversed_pt_device_list, g_pt_paired_src_dev_history);
    if (BT_SUCCESS != i4_ret)
    {
        BT_DBG_ERROR(BT_DEBUG_A2DP, "NO paired src device!");
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
        BT_DBG_INFO(BT_DEBUG_A2DP, "device_list[%ld].name: %s,bdAddr: %s, cod:%lx",
                      (long)j,
                      pt_device_list->device_list[j].name,
                      pt_device_list->device_list[j].bdAddr,
                      (long)pt_device_list->device_list[j].cod
                     );
        BT_DBG_INFO(BT_DEBUG_A2DP, "reversed_pt_device_list[%ld].name: %s,bdAddr: %s, cod:%lx",
                      (long)temp,
                      reversed_pt_device_list.device_list[temp].name,
                      reversed_pt_device_list.device_list[temp].bdAddr,
                      (long)reversed_pt_device_list.device_list[temp].cod
                     );
        i = (i + 1) % MAX_DEV_NUM;
        temp = (temp - 1 + MAX_DEV_NUM) % MAX_DEV_NUM;
        j++;
    }
    return BT_SUCCESS;
}



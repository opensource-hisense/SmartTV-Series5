/*******************************************************************************
*  Copyright Statement:
*  --------------------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of MediaTek Inc. (C) 2013
*
*  BY OPENING THIS FILE, BUYER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND AGREES
*  THAT THE SOFTWARE/FIRMWARE AND ITS DOCUMENTATIONS ("MEDIATEK SOFTWARE")
*  RECEIVED FROM MEDIATEK AND/OR ITS REPRESENTATIVES ARE PROVIDED TO BUYER ON
*  AN "AS-IS" BASIS ONLY. MEDIATEK EXPRESSLY DISCLAIMS ANY AND ALL WARRANTIES,
*  EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF
*  MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE OR NONINFRINGEMENT.
*  NEITHER DOES MEDIATEK PROVIDE ANY WARRANTY WHATSOEVER WITH RESPECT TO THE
*  SOFTWARE OF ANY THIRD PARTY WHICH MAY BE USED BY, INCORPORATED IN, OR
*  SUPPLIED WITH THE MEDIATEK SOFTWARE, AND BUYER AGREES TO LOOK ONLY TO SUCH
*  THIRD PARTY FOR ANY WARRANTY CLAIM RELATING THERETO. MEDIATEK SHALL ALSO
*  NOT BE RESPONSIBLE FOR ANY MEDIATEK SOFTWARE RELEASES MADE TO BUYER'S
*  SPECIFICATION OR TO CONFORM TO A PARTICULAR STANDARD OR OPEN FORUM.
*
*  BUYER'S SOLE AND EXCLUSIVE REMEDY AND MEDIATEK'S ENTIRE AND CUMULATIVE
*  LIABILITY WITH RESPECT TO THE MEDIATEK SOFTWARE RELEASED HEREUNDER WILL BE,
*  AT MEDIATEK'S OPTION, TO REVISE OR REPLACE THE MEDIATEK SOFTWARE AT ISSUE,
*  OR REFUND ANY SOFTWARE LICENSE FEES OR SERVICE CHARGE PAID BY BUYER TO
*  MEDIATEK FOR SUCH MEDIATEK SOFTWARE AT ISSUE.
*
*  THE TRANSACTION CONTEMPLATED HEREUNDER SHALL BE CONSTRUED IN ACCORDANCE
*  WITH THE LAWS OF THE STATE OF CALIFORNIA, USA, EXCLUDING ITS CONFLICT OF
*  LAWS PRINCIPLES.  ANY DISPUTES, CONTROVERSIES OR CLAIMS ARISING THEREOF AND
*  RELATED THERETO SHALL BE SETTLED BY ARBITRATION IN SAN FRANCISCO, CA, UNDER
*  THE RULES OF THE INTERNATIONAL CHAMBER OF COMMERCE (ICC).
*
*******************************************************************************/

/* FILE NAME:  c_bt_mw_hid.c
 * AUTHOR: zwei chen
 * PURPOSE:
 *      It provides HID HOST  API to APP.
 * NOTES:
 */


//#include "bt_mw_hidh.h"
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include "bt_mw_common.h"
#include "bluetooth.h"
#include "c_bt_mw_hidh.h"
#include "bluetooth_sync.h"
#include "bt_mw_hidh.h"
#include "bt_mw_devices_info.h"
#include "bt_mw_gap.h"

BtAppHidCbk      HidCbk = NULL;
/*HANDLE_T bt_hid_connect_timer = NULL_HANDLE;*/

extern BT_HID_TARGET_DEV_LIST g_paired_hid_dev_history;
extern BT_HID_TARGET_DEV_LIST *g_pt_paired_hid_dev_history;

extern BT_HID_CBK_STRUCT fg_bt_hid_struct_data;
extern BT_HID_STATUS_STRUCT_LIST *g_pt_hid_status_list;


/****************************************************
 * FUNCTION NAME: hid_connect_timer_cbk
 * PURPOSE:
 *      The function is used to note APP connetion is timeout.
 * INPUT:
 *      pt_handle               -- handle
 *      pv_tag
 * OUTPUT:
 *      None
 * RETURN:
 *      VOID
 * NOTES:
 *      None
 */
VOID hid_connect_timer_cbk(timer_t pt_handle, VOID* pv_tag)
{
    FUNC_ENTRY;

    BT_DBG_ERROR(BT_DEBUG_HID, "HID CONNECTING timeout!");
    fg_bt_hid_struct_data.bt_hid_event_type = BT_HID_CONNECT_FAIL;
    if(NULL != HidCbk)
    {
        HidCbk(&fg_bt_hid_struct_data);
    }
    else
    {
        BT_DBG_ERROR(BT_DEBUG_HID, "null pointer of hidcbk");
    }
}

/****************************************************
 * FUNCTION NAME: c_btm_connect_hid_sources
 * PURPOSE:
 *      The function is used to call HID connection
 * INPUT:
 *      *addr               -- device address
 *
 * OUTPUT:
 *      None
 * RETURN:
 *      INT32:error code
 * NOTES:
 *      None
 */
EXPORT_SYMBOL INT32 c_btm_connect_hid_sources(char *addr)
{
    FUNC_ENTRY;
    INT32 i4_ret = BT_SUCCESS;
    if (NULL == addr)
    {
        BT_DBG_ERROR(BT_DEBUG_HID, "null pointer!");
        return BT_ERR_STATUS_PARM_INVALID;
    }
    BT_DBG_NORMAL(BT_DEBUG_HID, "the MAC is: %s", addr);

    i4_ret = bluetooth_hid_connect(addr);
    if(i4_ret != BT_SUCCESS)
    {
        return i4_ret;
    }

    return BT_SUCCESS;
}

/****************************************************
 * FUNCTION NAME: c_btm_disconnect_hid_sources
 * PURPOSE:
 *      The function is used to call HID disconnect
 * INPUT:
 *      *addr               -- device address
 *
 * OUTPUT:
 *      None
 * RETURN:
 *      INT32:error code
 * NOTES:
 *      None
 */
EXPORT_SYMBOL INT32 c_btm_disconnect_hid_sources(char *addr)
{
    INT32 i4_ret = BT_SUCCESS;

    if (NULL == addr)
    {
        BT_DBG_ERROR(BT_DEBUG_HID, "null pointer!");
        return BT_ERR_STATUS_PARM_INVALID;
    }
    BT_DBG_NORMAL(BT_DEBUG_HID, "the MAC is: %s", addr);

    i4_ret = bluetooth_hid_disconnect(addr);

    if (BT_SUCCESS != i4_ret)
    {
        BT_DBG_ERROR(BT_DEBUG_HID, "can't disconnect HID");
        return BT_ERR_STATUS_FAIL;
    }

    return i4_ret;
}


/****************************************************
 * FUNCTION NAME: c_btm_get_hid_target_info
 * PURPOSE:
 *      The function is used to get devide info:address and name
 * INPUT:
 *      None
 *
 * OUTPUT:
 *      pt_target_dev_info: devide info:address and name
 * RETURN:
 *      INT32:error code
 * NOTES:
 *      None
 */

/*get current connected device(eg. cell phone) MAC address and name*/
INT32 c_btm_get_hid_target_info(BT_TARGET_DEV_INFO *pt_target_dev_info)
{
    FUNC_ENTRY;
    return btaudio_get_current_hid_target_dev_info(pt_target_dev_info);
}

/****************************************************
 * FUNCTION NAME: c_btm_bluetooth_hid_set_output_report
 * PURPOSE:
 *      The function is used to set output report
 * INPUT:
 *      pbt_addr:device address
 *      preport_data: output report data
 * OUTPUT:
 *      None
 * RETURN:
 *      INT32:error code
 * NOTES:
 *      None
 */
EXPORT_SYMBOL INT32 c_btm_bluetooth_hid_set_output_report(char *pbt_addr, char *preport_data)
{
    return bluetooth_hid_set_output_report(pbt_addr, preport_data);
}

/****************************************************
 * FUNCTION NAME: c_btm_get_paired_hid_dev_list
 * PURPOSE:
 *      The function is used to get device history list and its paired or connected status.
 * INPUT:
 *      None
 *
 * OUTPUT:
 *      pt_device_list: hid device history list
 *      pt_hid_status_list: hid device paired or connected status.
 * RETURN:
 *      INT32:error code
 * NOTES:
 *      None
 */
EXPORT_SYMBOL INT32 c_btm_get_paired_hid_dev_list(BT_HID_TARGET_DEV_LIST *pt_device_list, BT_HID_STATUS_STRUCT_LIST *pt_hid_status_list)
{
    FUNC_ENTRY;
    INT32 i4_ret = BT_SUCCESS;
    INT32 i;
    INT32 j;
    INT32 temp;
    /**/
    BT_HID_TARGET_DEV_LIST reversed_pt_device_list;
    memset(&reversed_pt_device_list,0,sizeof(BT_HID_TARGET_DEV_LIST));
    print_hid_history_info(g_pt_paired_hid_dev_history);
    print_hid_status_info(g_pt_hid_status_list);
    i4_ret = bluetooth_get_hid_device_history(&reversed_pt_device_list, g_pt_paired_hid_dev_history);
    if (BT_SUCCESS != i4_ret)
    {
        BT_DBG_ERROR(BT_DEBUG_HID, "NO paired HID device!");
        memset(g_pt_paired_hid_dev_history, 0x0, sizeof(BT_HID_TARGET_DEV_LIST));
        memset(g_pt_hid_status_list, 0x0, sizeof(BT_HID_STATUS_STRUCT_LIST));
        return BT_SUCCESS;
    }
    if (MAX_HID_DEV_NUM < reversed_pt_device_list.dev_num)
    {
        BT_DBG_ERROR(BT_DEBUG_HID, "the device number is wrong:%ld!", (long)reversed_pt_device_list.dev_num);
        memset(g_pt_paired_hid_dev_history, 0x0, sizeof(BT_HID_TARGET_DEV_LIST));
        memset(g_pt_hid_status_list, 0x0, sizeof(BT_HID_STATUS_STRUCT_LIST));
        return BT_ERR_STATUS_FAIL;
    }

    memset(pt_device_list, 0, sizeof(BT_HID_TARGET_DEV_LIST));
    memset(pt_hid_status_list, 0, sizeof(BT_HID_STATUS_STRUCT_LIST));
    pt_device_list->dev_num = reversed_pt_device_list.dev_num;
    pt_hid_status_list->dev_num = g_pt_hid_status_list->dev_num;
    /* i : the first node in device list*/
    i = reversed_pt_device_list.head_idx;
    /* temp : the last node in device list*/
    temp = (i + reversed_pt_device_list.dev_num - 1) % MAX_HID_DEV_NUM;
    /* j : the first node in app device list always is 0*/
    j = 0;
    BT_DBG_NORMAL(BT_DEBUG_HID, "num:%ld,idx:%ld", (long)pt_device_list->dev_num,
    (long)i);
    while (j < reversed_pt_device_list.dev_num)
    {
        /*error handling for invalid MAC Address*/
        if (MAX_BDADDR_LEN-1 != strlen(reversed_pt_device_list.device_list[j].bdAddr))
        {
            BT_DBG_ERROR(BT_DEBUG_HID, "error happend in device list");
            memset(g_pt_paired_hid_dev_history, 0x0, sizeof(BT_HID_TARGET_DEV_LIST));
            memset(g_pt_hid_status_list, 0x0, sizeof(BT_HID_TARGET_DEV_LIST));
            return BT_ERR_STATUS_FAIL;
        }
        memcpy(&pt_device_list->device_list[j],
               &reversed_pt_device_list.device_list[temp],
               sizeof(BT_TARGET_DEV_INFO)
              );

        memcpy(&pt_hid_status_list->device_list[j],
               &g_pt_hid_status_list->device_list[temp],
               sizeof(BT_HID_STATUS_STRUCT)
              );
        BT_DBG_INFO(BT_DEBUG_HID, "device_list[%ld].name: %s,bdAddr: %s, cod:%ld",
                      (long)j,
                      pt_device_list->device_list[j].name,
                      pt_device_list->device_list[j].bdAddr,
                      (long)pt_device_list->device_list[j].cod
                     );
        BT_DBG_INFO(BT_DEBUG_HID, "reversed_pt_device_list[%ld].name: %s,bdAddr: %s, cod:%ld",
                      (long)temp,
                      reversed_pt_device_list.device_list[temp].name,
                      reversed_pt_device_list.device_list[temp].bdAddr,
                      (long)reversed_pt_device_list.device_list[temp].cod
                     );

        BT_DBG_INFO(BT_DEBUG_HID, "device_list[%ld].bdAddr: %s, status:%d",
                      (long)j,
                      pt_hid_status_list->device_list[j].bdAddr,
                      pt_hid_status_list->device_list[j].status
                     );
        BT_DBG_INFO(BT_DEBUG_HID, "reversed_pt_device_list[%ld].bdAddr: %s, status:%d",
                      (long)temp,
                      g_pt_hid_status_list->device_list[temp].bdAddr,
                      g_pt_hid_status_list->device_list[temp].status
                     );

        i = (i + 1) % MAX_HID_DEV_NUM;
        temp = (temp - 1 + MAX_HID_DEV_NUM) % MAX_HID_DEV_NUM;
        j++;
    }

    BT_DBG_NORMAL(BT_DEBUG_HID, "num:%ld,idx:%ld", (long)pt_device_list->dev_num,
        (long)i);

    for (i=0;i<pt_device_list->dev_num;i++)
    {
        BT_DBG_INFO(BT_DEBUG_HID, "device_list[%ld].name: %s,bdAddr: %s, cod:%ld",
                      (long)i,
                      pt_device_list->device_list[i].name,
                      pt_device_list->device_list[i].bdAddr,
                      (long)pt_device_list->device_list[i].cod
                     );
    }

    for (i=0;i<pt_hid_status_list->dev_num;i++)
    {
        BT_DBG_INFO(BT_DEBUG_HID, "device_list[%ld].bdAddr: %s, status:%d",
                      (long)i,
                      pt_hid_status_list->device_list[i].bdAddr,
                      pt_hid_status_list->device_list[i].status
                     );
    }

    return BT_SUCCESS;
}

/****************************************************
 * FUNCTION NAME: c_btm_del_paired_hid_dev_one
 * PURPOSE:
 *      The function is used to delete one paired hid device
 * INPUT:
 *      pt_device_info: device info: address and name
 *
 * OUTPUT:
 *      None
 * RETURN:
 *      INT32:error code
 * NOTES:
 *      remove one item from paired device histroy, not do unpair operation
 */
EXPORT_SYMBOL INT32 c_btm_del_paired_hid_dev_one(BT_TARGET_DEV_INFO *pt_device_info)
{
    FUNC_ENTRY;
    INT32 i4_ret = BT_SUCCESS;
    INT32 i4_temp = BT_SUCCESS;

    if (NULL == pt_device_info)
    {
        BT_DBG_ERROR(BT_DEBUG_HID, "null pointer of pt_device_info!");
        return BT_ERR_STATUS_PARM_INVALID;
    }

    BT_DBG_NORMAL(BT_DEBUG_HID, "delete device:%s from hid device list", pt_device_info->bdAddr);
#if ENABLE_HID_PROFILE_H
    bluetooth_remove_paired_dev(pt_device_info->bdAddr);
    i4_temp = bluetooth_hid_removeHistory_one(pt_device_info->bdAddr, g_pt_paired_hid_dev_history);
    if (BT_SUCCESS != i4_temp)
    {
        i4_ret = BT_ERR_STATUS_FAIL;
        BT_DBG_ERROR(BT_DEBUG_HID, "cannot delete the device:%s", pt_device_info->bdAddr);
    }

    i4_temp = bluetooth_hid_removeStatus_one(pt_device_info->bdAddr, g_pt_hid_status_list);
    if (BT_SUCCESS != i4_temp)
    {
        i4_ret = BT_ERR_STATUS_FAIL;
        BT_DBG_ERROR(BT_DEBUG_HID, "cannot delete the device:%s", pt_device_info->bdAddr);
    }
#endif

    return i4_ret;
}

/****************************************************
 * FUNCTION NAME: c_btm_del_paired_hid_dev_all
 * PURPOSE:
 *      The function is used to delete all paired device
 * INPUT:
 *      None
 *
 * OUTPUT:
 *      None
 * RETURN:
 *      INT32:error code
 * NOTES:
 *      None
 */
EXPORT_SYMBOL INT32 c_btm_del_paired_hid_dev_all(VOID)
{
    FUNC_ENTRY;
    INT32 i4_ret = BT_SUCCESS;

    i4_ret = bluetooth_hid_removeHistory_all();
    if (BT_SUCCESS != i4_ret)
    {
        BT_DBG_ERROR(BT_DEBUG_HID, "cannot delete the all device");
        return BT_ERR_STATUS_FAIL;
    }

    i4_ret = bluetooth_hid_removeStatus_all();
    if (BT_SUCCESS != i4_ret)
    {
        BT_DBG_ERROR(BT_DEBUG_HID, "cannot delete the all device");
        return BT_ERR_STATUS_FAIL;
    }

    return i4_ret;
}


#if 0
/****************************************************
 * FUNCTION NAME: c_bt_hid_activate
 * PURPOSE:
 *      The function is used to activate hid
 * INPUT:
 *      None
 *
 * OUTPUT:
 *      None
 * RETURN:
 *      INT32:error code
 * NOTES:
 *      None
 */
INT32 c_bt_hid_activate(VOID)
{
    FUNC_ENTRY;
    return bluetooth_hid_activate();
}

/****************************************************
 * FUNCTION NAME: c_bt_hid_deactivate
 * PURPOSE:
 *      The function is used to deactivate hid
 * INPUT:
 *      None
 *
 * OUTPUT:
 *      None
 * RETURN:
 *      INT32:error code
 * NOTES:
 *      None
 */
INT32 c_bt_hid_deactivate(VOID)
{
    FUNC_ENTRY;
    return bluetooth_hid_deactivate();
}
#endif
/****************************************************
 * FUNCTION NAME: c_bt_hid_init
 * PURPOSE:
 *      The function is used to init hid profile
 * INPUT:
 *      None
 *
 * OUTPUT:
 *      None
 * RETURN:
 *      INT32:error code
 * NOTES:
 *      None
 */
EXPORT_SYMBOL INT32 c_bt_hid_init(VOID)
{
    FUNC_ENTRY;
    return bluetooth_hid_init();
}

/****************************************************
 * FUNCTION NAME: c_bt_hid_deinit
 * PURPOSE:
 *      The function is used to deinit hid profile
 * INPUT:
 *      None
 *
 * OUTPUT:
 *      None
 * RETURN:
 *      INT32:error code
 * NOTES:
 *      None
 */
EXPORT_SYMBOL INT32 c_bt_hid_deinit(VOID)
{
    FUNC_ENTRY;
    return bluetooth_hid_deinit();
}

/****************************************************
 * FUNCTION NAME: c_bt_hid_connect
 * PURPOSE:
 *      The function is used to connect  hid device
 * INPUT:
 *      pbt_addr:device address
 *
 * OUTPUT:
 *      None
 * RETURN:
 *      INT32:error code
 * NOTES:
 *      None
 */
EXPORT_SYMBOL INT32 c_bt_hid_connect(char *pbt_addr)
{
    FUNC_ENTRY;
    return bluetooth_hid_connect(pbt_addr);
}

/****************************************************
 * FUNCTION NAME: c_bt_hid_disconnect
 * PURPOSE:
 *      The function is used to disconnect  hid device
 * INPUT:
 *      pbt_addr:device address
 *
 * OUTPUT:
 *      None
 * RETURN:
 *      INT32:error code
 * NOTES:
 *      None
 */
EXPORT_SYMBOL INT32 c_bt_hid_disconnect(char *pbt_addr)
{
    FUNC_ENTRY;
    return bluetooth_hid_disconnect(pbt_addr);
}

/****************************************************
 * FUNCTION NAME: c_btm_hid_auto_disconnection
 * PURPOSE:
 *      The function is used to disconnect all hid connected device.
 * INPUT:
 *      None
 * OUTPUT:
 *      None
 * RETURN:
 *      INT32:error code
 * NOTES:
 *      None
 */
EXPORT_SYMBOL INT32 c_btm_hid_auto_disconnection(VOID)
{
    FUNC_ENTRY;
    INT32 u_i4 = BT_SUCCESS;
    INT32 i_ret = BT_SUCCESS;
    UINT32 i = 0;
    UINT32 j = 0;/* j is for count device num */
    BT_CHECK_POINTER(BT_DEBUG_HID, g_pt_hid_status_list);
    i = g_pt_hid_status_list->head_idx;
    BT_DBG_INFO(BT_DEBUG_HID, "head_idx:%ld", (long)i);
    BT_DBG_INFO(BT_DEBUG_HID, "dev_num:%ld", (long)g_pt_hid_status_list->dev_num);
    while (j < g_pt_hid_status_list->dev_num)
    {
        if(g_pt_hid_status_list->device_list[i].status == BT_CONNECTED_STATUS)
        {
            BT_DBG_INFO(BT_DEBUG_HID, "index:%ld, address:%s.", (long)i, g_pt_hid_status_list->device_list[i].bdAddr);
            u_i4 = c_bt_hid_disconnect(g_pt_hid_status_list->device_list[i].bdAddr);
            if(BT_SUCCESS != u_i4)
            {
                i_ret = BT_ERR_STATUS_FAIL;
                BT_DBG_ERROR(BT_DEBUG_HID, "disconnect status:%d,address Error:%s",
                    u_i4,g_pt_hid_status_list->device_list[i].bdAddr);

            }
        }
        i = (i + 1) % MAX_HID_DEV_NUM;
        j++;
    }

    return i_ret;
}


/****************************************************
 * FUNCTION NAME: c_bt_hid_set_output_report
 * PURPOSE:
 *      The function is used to set output report
 * INPUT:
 *      pbt_addr:device address
 *      preport_data: output report data
 * OUTPUT:
 *      None
 * RETURN:
 *      INT32:error code
 * NOTES:
 *      None
 */
EXPORT_SYMBOL INT32 c_bt_hid_set_output_report(char *pbt_addr, char *preport_data)
{
    FUNC_ENTRY;
    return bluetooth_hid_set_output_report(pbt_addr, preport_data);
}

/****************************************************
 * FUNCTION NAME: c_bt_hid_get_input_report
 * PURPOSE:
 *      The function is used to get input report
 * INPUT:
 *      pbt_addr:device address
 *      reportId: report id
 *     bufferSize : size of the buffer for input data
 * OUTPUT:
 *      None
 * RETURN:
 *      INT32:error code
 * NOTES:
 *      None
 */
EXPORT_SYMBOL INT32 c_bt_hid_get_input_report(char *pbt_addr, int reportId, int bufferSize)
{
    FUNC_ENTRY;
    return bluetooth_hid_get_input_report(pbt_addr, reportId, bufferSize);
}

/****************************************************
 * FUNCTION NAME: c_bt_hid_get_output_report
 * PURPOSE:
 *      The function is used to get output report
 * INPUT:
 *      pbt_addr:device address
 *      reportId: report id
 *     bufferSize : size of the buffer for output data
 * OUTPUT:
 *      None
 * RETURN:
 *      INT32:error code
 * NOTES:
 *      None
 */
EXPORT_SYMBOL INT32 c_bt_hid_get_output_report(char *pbt_addr, int reportId, int bufferSize)
{
    FUNC_ENTRY;
    return bluetooth_hid_get_output_report(pbt_addr, reportId, bufferSize);
}

/****************************************************
 * FUNCTION NAME: c_bt_hid_set_input_report
 * PURPOSE:
 *      The function is used to set input report
 * INPUT:
 *      pbt_addr:device address
 *      preport_data: input report data
 *
 * OUTPUT:
 *      None
 * RETURN:
 *      INT32:error code
 * NOTES:
 *      None
 */
EXPORT_SYMBOL INT32 c_bt_hid_set_input_report(char *pbt_addr, char *preport_data)
{
    FUNC_ENTRY;
    return bluetooth_hid_set_input_report(pbt_addr, preport_data);
}

/****************************************************
 * FUNCTION NAME: c_bt_hid_get_feature_report
 * PURPOSE:
 *      The function is used to get feature report
 * INPUT:
 *      pbt_addr:device address
 *      reportId: report id
 *     bufferSize : size of the buffer for feature data
 * OUTPUT:
 *      None
 * RETURN:
 *      INT32:error code
 * NOTES:
 *      None
 */
EXPORT_SYMBOL INT32 c_bt_hid_get_feature_report(char *pbt_addr, int reportId, int bufferSize)
{
    FUNC_ENTRY;
    return bluetooth_hid_get_feature_report(pbt_addr, reportId, bufferSize);
}

/****************************************************
 * FUNCTION NAME: c_bt_hid_set_feature_report
 * PURPOSE:
 *      The function is used to set feature report
 * INPUT:
 *      pbt_addr:device address
 *      preport_data: input report data
 *
 * OUTPUT:
 *      None
 * RETURN:
 *      INT32:error code
 * NOTES:
 *      None
 */
EXPORT_SYMBOL INT32 c_bt_hid_set_feature_report(char *pbt_addr, char *preport_data)
{
    FUNC_ENTRY;
    return bluetooth_hid_set_feature_report(pbt_addr, preport_data);
}

#if defined(MTK_LINUX_HIDH_PTS_TEST) && (MTK_LINUX_HIDH_PTS_TEST == TRUE)
/****************************************************
 * FUNCTION NAME: c_bt_hid_virtual_unplug
 * PURPOSE:
 *      The function is used to send virtual unplug request for test
 * INPUT:
 *      pbt_addr:device address
 *
 * OUTPUT:
 *      None
 * RETURN:
 *      INT32:error code
 * NOTES:
 *      None
 */
EXPORT_SYMBOL INT32 c_bt_hid_virtual_unplug(char *pbt_addr)
{
    FUNC_ENTRY;
    return bluetooth_hid_virtual_unplug(pbt_addr);
}

/****************************************************
 * FUNCTION NAME: c_bt_hid_send_data
 * PURPOSE:
 *      The function is used to send data for test
 * INPUT:
 *      pbt_addr:device address
 *      psend_data: input send data
 *
 * OUTPUT:
 *      None
 * RETURN:
 *      INT32:error code
 * NOTES:
 *      None
 */
EXPORT_SYMBOL INT32 c_bt_hid_send_data(char *pbt_addr, char *psend_data)
{
    FUNC_ENTRY;
    return bluetooth_hid_send_data(pbt_addr, psend_data);
}
#endif

/****************************************************
 * FUNCTION NAME: c_bt_hid_get_protocol
 * PURPOSE:
 *      The function is used to get  protocol
 * INPUT:
 *      pbt_addr:device address
 *
 *
 * OUTPUT:
 *      None
 * RETURN:
 *      INT32:error code
 * NOTES:
 *      None
 */
EXPORT_SYMBOL INT32 c_bt_hid_get_protocol(char *pbt_addr)
{
    FUNC_ENTRY;
    return bluetooth_hid_get_protocol(pbt_addr);
}

/****************************************************
 * FUNCTION NAME: c_bt_hid_set_protocol
 * PURPOSE:
 *      The function is used to set  protocol
 * INPUT:
 *      pbt_addr:device address
 *      protocol_mode: protocol mode:boot or report mode
 *
 * OUTPUT:
 *      None
 * RETURN:
 *      INT32:error code
 * NOTES:
 *      None
 */
EXPORT_SYMBOL INT32 c_bt_hid_set_protocol(char *pbt_addr, int protocol_mode)
{
    FUNC_ENTRY;
    return bluetooth_hid_set_protocol(pbt_addr, protocol_mode);
}
#if 0
/****************************************************
 * FUNCTION NAME: c_bt_hid_send_control
 * PURPOSE:
 *      The function is used to suspend or exit suspend mode
 * INPUT:
 *      pbt_addr:device address
 *      pcontrol_mode: suspend or exit suspend mode
 *
 * OUTPUT:
 *      None
 * RETURN:
 *      INT32:error code
 * NOTES:
 *      None
 */
INT32 c_bt_hid_send_control(char *pbt_addr, int pcontrol_mode)
{
    FUNC_ENTRY;
    return bluetooth_hid_send_control(pbt_addr, pcontrol_mode);
}
#endif


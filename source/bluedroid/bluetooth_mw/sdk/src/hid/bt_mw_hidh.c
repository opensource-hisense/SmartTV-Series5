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

/* FILE NAME:  bt_mw_hid.c
 * AUTHOR: zwei chen
 * PURPOSE:
 *      It provides hid  interface to c_bt_mw_hid.c.
 * NOTES:
 */


#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>

#include "bluetooth.h"
#include "bt_mw_common.h"
#include "bt_mw_hidh.h"
#include "bluetooth_sync.h"
#include "linuxbt_hid_if.h"
#include "linuxbt_gap_if.h"
#include "bt_mw_devices_info.h"
#include "linuxbt_common.h"
#include "bt_mw_gap.h"

extern BtAppHidCbk      HidCbk;
BT_TARGET_DEV_INFO g_bt_hid_target_dev_info;
extern BT_HID_TARGET_DEV_LIST g_paired_hid_dev_history;
extern BT_HID_TARGET_DEV_LIST *g_pt_paired_hid_dev_history;
BT_HID_CBK_STRUCT fg_bt_hid_struct_data;
BT_HID_STATUS_STRUCT g_bt_hid_status;
extern BT_HID_STATUS_STRUCT_LIST *g_pt_hid_status_list;
//BT_HOGP_STATUS hogp_status;

#if 0
/****************************************************
 * FUNCTION NAME: bluetooth_hid_activate
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

INT32 bluetooth_hid_activate(VOID)
{
    FUNC_ENTRY;
    return linuxbt_hid_activate_handler();
}

/****************************************************
 * FUNCTION NAME: bluetooth_hid_deactivate
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
INT32 bluetooth_hid_deactivate(VOID)
{
    FUNC_ENTRY;
    return linuxbt_hid_deactivate_handler();
}
#endif
/****************************************************
 * FUNCTION NAME: bluetooth_hid_init
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
INT32 bluetooth_hid_init(VOID)
{
    FUNC_ENTRY;
    memset(&fg_bt_hid_struct_data,0,sizeof(BT_HID_CBK_STRUCT));
    //memset(&hogp_status,0,sizeof(BT_HOGP_STATUS));
    return linuxbt_hid_init();
}


/****************************************************
 * FUNCTION NAME: bluetooth_hid_deinit
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
INT32 bluetooth_hid_deinit(VOID)
{
    FUNC_ENTRY;
    return linuxbt_hid_deinit();
}

/****************************************************
 * FUNCTION NAME: bluetooth_btaddr_htos
 * PURPOSE:
 *      The function is used to convert hex address format to string
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
INT32 bluetooth_btaddr_htos(bt_bdaddr_t *bdaddr_h, char *btaddr_s)
{
    if(NULL == btaddr_s || NULL == bdaddr_h)
    {
        return BT_ERR_STATUS_FAIL;
    }
    snprintf(btaddr_s, 18, "%02X:%02X:%02X:%02X:%02X:%02X",
               bdaddr_h->address[0],
               bdaddr_h->address[1],
               bdaddr_h->address[2],
               bdaddr_h->address[3],
               bdaddr_h->address[4],
               bdaddr_h->address[5]);
    btaddr_s[17] = '\0';
    return BT_SUCCESS;
}

/****************************************************
 * FUNCTION NAME: btaudio_handle_hid_disconnect_status_check
 * PURPOSE:
 *      The function is used to check disconnet status
 * INPUT:
 *      i4device_index: first or second
 *
 * OUTPUT:
 *      None
 * RETURN:
 *      INT32:error code
 * NOTES:
 *      None
 */
INT32 btaudio_handle_hid_disconnect_status_check(INT32 i4device_index)
{
    FUNC_ENTRY;
    UINT32 ui4_loop = 10;

    if (i4device_index >= MAX_HID_DEV_NUM)
    {
        BT_DBG_ERROR(BT_DEBUG_HID, "input index Error:%d.", i4device_index);
        return BT_ERR_STATUS_FAIL;
    }

    BT_CHECK_POINTER(BT_DEBUG_HID, g_pt_hid_status_list);

    do
    {
        BT_DBG_NORMAL(BT_DEBUG_HID, "ui4_loop:%ld, hid_connect_status:%d!",
            (long)ui4_loop, g_pt_hid_status_list->device_list[i4device_index].status);
        usleep(500*1000);
        ui4_loop --;
    }
    while (g_pt_hid_status_list->device_list[i4device_index].status && (0 < ui4_loop));

    if (g_pt_hid_status_list->device_list[i4device_index].status)
    {
        BT_DBG_ERROR(BT_DEBUG_HID, "can't disconnect hid device:%s, status:%d.",
            g_pt_hid_status_list->device_list[i4device_index].bdAddr,
            g_pt_hid_status_list->device_list[i4device_index].status);
        return BT_ERR_STATUS_FAIL;
    }

    return BT_SUCCESS;
}

/****************************************************
 * FUNCTION NAME: btaudio_handle_hid_address_match
 * PURPOSE:
 *      The function is used to find device index by device address
 * INPUT:
 *      i4device_index: first or second
 *
 * OUTPUT:
 *      None
 * RETURN:
 *      INT32:error code
 * NOTES:
 *      None
 */
INT32 btaudio_handle_hid_address_match(CHAR *pbt_addr)
{
    FUNC_ENTRY;
    UINT32 i = 0;
    UINT32 j = 0;/* j is for count device num */
    INT32 iret4 = MAX_HID_DEV_NUM+1;
    BT_CHECK_POINTER(BT_DEBUG_HID, g_pt_paired_hid_dev_history);
    i = g_pt_paired_hid_dev_history->head_idx;
    BT_DBG_INFO(BT_DEBUG_HID, "head_idx:%ld", (long)i);
    while (j < g_pt_paired_hid_dev_history->dev_num)
    {
        BT_DBG_NORMAL(BT_DEBUG_HID, "device_list[%ld].bdAddr:%s device_list[%ld].name:%s device_list[%ld].cod:0x%02lX",
                        (long)i,
                        g_pt_paired_hid_dev_history->device_list[i].bdAddr,
                        (long)i,
                        g_pt_paired_hid_dev_history->device_list[i].name,
                        (long)i,
                        (long)g_pt_paired_hid_dev_history->device_list[i].cod
                       );
        if(0 == strncmp(pbt_addr, g_pt_paired_hid_dev_history->device_list[i].bdAddr, MAX_BDADDR_LEN))
        {
            BT_DBG_NORMAL(BT_DEBUG_HID, "Find address index:%d.", i);
            iret4 = i;
            break;
        }
        i = (i + 1) % MAX_HID_DEV_NUM;
        j++;
    }

    if (iret4 == (MAX_HID_DEV_NUM+1))
    {
        BT_DBG_ERROR(BT_DEBUG_HID, "Error HID address.");
        iret4 = -1;
    }

    return iret4;
}

#if 0
INT32 get_hogp_connect_state(CHAR *pbt_add)
{
    FUNC_ENTRY;
    INT32 ret = 0;
    if(strncmp(pbt_add,hogp_status.bdAddr,sizeof(hogp_status.bdAddr)) == 0){
       return hogp_status.status;
    }

    return ret;
}

void set_hogp_connect_state(UINT8 status,CHAR *pbt_add)
{
    FUNC_ENTRY;
    hogp_status.status = status;
    strncpy(hogp_status.bdAddr, pbt_add, sizeof(hogp_status.bdAddr));
}
#endif
/****************************************************
 * FUNCTION NAME: bluetooth_hid_connect
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
INT32 bluetooth_hid_connect(CHAR *pbt_addr)
{
    FUNC_ENTRY;
#if 0
    INT32 devicetype = bluetooth_get_device_type(pbt_addr);

    /*HID case*/
    if (devicetype == 1){
       return linuxbt_hid_connect_int_handler(pbt_addr);
       //return bluetooth_pair(pbt_addr);
    }
    /*HOGP case :should pair before connecting*/
    else if ((devicetype == 0) || (devicetype == 2)){
        bluetooth_ble_pair(pbt_addr);
        set_hogp_connect_state(1,pbt_addr);
    }
    else{
        BT_DBG_ERROR(BT_DEBUG_HID, "unkown device type:%d ",devicetype);
        /*default hid case for PTS test*/
        return linuxbt_hid_connect_int_handler(pbt_addr);
    }
    return BT_SUCCESS;
#endif
    return linuxbt_hid_connect_int_handler(pbt_addr);
}



/****************************************************
 * FUNCTION NAME: bluetooth_hid_disconnect
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
INT32 bluetooth_hid_disconnect(CHAR *pbt_addr)
{
    FUNC_ENTRY;
    INT32 ret = BT_SUCCESS;
    INT32 i4device_index = 0;
    BT_DBG_INFO(BT_DEBUG_HID, "pbt_addr:%s", pbt_addr);
    ret = linuxbt_hid_disconnect_handler(pbt_addr);
    if(ret != BT_SUCCESS)
    {
        return ret;
    }
    i4device_index = btaudio_handle_hid_address_match(pbt_addr);
    if(-1 == i4device_index)
    {
        return BT_ERR_STATUS_FAIL;
    }
    ret = btaudio_handle_hid_disconnect_status_check(i4device_index);
    return ret;
}


/****************************************************
 * FUNCTION NAME: bluetooth_hid_disconnect_sync
 * PURPOSE:
 *      The function is used to disconnect hid device
 * INPUT:
 *      no input
 *
 * OUTPUT:
 *      None
 * RETURN:
 *      INT32:error code
 * NOTES:
 *      None
 */
INT32 bluetooth_hid_disconnect_sync()
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
            BT_DBG_INFO(BT_DEBUG_HID, "index:%ld,address:%s.", (long)i, g_pt_hid_status_list->device_list[i].bdAddr);
            u_i4 = bluetooth_hid_disconnect(g_pt_hid_status_list->device_list[i].bdAddr);
            if(BT_SUCCESS != u_i4)
            {
                i_ret = BT_ERR_STATUS_FAIL;
                BT_DBG_ERROR(BT_DEBUG_HID, "disconnect status:%d,address Error:%s",
                    u_i4, g_pt_hid_status_list->device_list[i].bdAddr);

            }
        }
        i = (i + 1) % MAX_HID_DEV_NUM;
        j++;
    }

    return i_ret;
}

/****************************************************
 * FUNCTION NAME: bluetooth_hid_set_output_report
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
INT32 bluetooth_hid_set_output_report(char *pbt_addr, char *preport_data)
{
    FUNC_ENTRY;
    return linuxbt_hid_set_output_report_handler(pbt_addr, preport_data);
}


/****************************************************
 * FUNCTION NAME: bluetooth_hid_get_input_report
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
INT32 bluetooth_hid_get_input_report(char *pbt_addr, int reportId, int bufferSize)
{
    FUNC_ENTRY;
    return linuxbt_hid_get_input_report_handler(pbt_addr, reportId, bufferSize);
}

/****************************************************
 * FUNCTION NAME: bluetooth_hid_get_output_report
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
INT32 bluetooth_hid_get_output_report(char *pbt_addr, int reportId, int bufferSize)
{
    FUNC_ENTRY;
    return linuxbt_hid_get_output_report_handler(pbt_addr, reportId, bufferSize);
}

/****************************************************
 * FUNCTION NAME: bluetooth_hid_set_input_report
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
INT32 bluetooth_hid_set_input_report(char *pbt_addr, char *preport_data)
{
    FUNC_ENTRY;
    return linuxbt_hid_set_input_report_handler(pbt_addr, preport_data);
}

/****************************************************
 * FUNCTION NAME: linuxbt_hid_get_feature_report_handler
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
INT32 bluetooth_hid_get_feature_report(char *pbt_addr, int reportId, int bufferSize)
{
    FUNC_ENTRY;
    return linuxbt_hid_get_feature_report_handler(pbt_addr, reportId, bufferSize);
}

/****************************************************
 * FUNCTION NAME: bluetooth_hid_set_feature_report
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
INT32 bluetooth_hid_set_feature_report(char *pbt_addr, char *preport_data)
{
    FUNC_ENTRY;
    return linuxbt_hid_set_feature_report_handler(pbt_addr, preport_data);
}

/****************************************************
 * FUNCTION NAME: bluetooth_hid_get_protocol
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
INT32 bluetooth_hid_get_protocol(char *pbt_addr)
{
    FUNC_ENTRY;
    return linuxbt_hid_get_protocol_handler(pbt_addr);
}

/****************************************************
 * FUNCTION NAME: bluetooth_hid_set_protocol
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
INT32 bluetooth_hid_set_protocol(char *pbt_addr, int protocol_mode)
{
    FUNC_ENTRY;
    return linuxbt_hid_set_protocol_handler(pbt_addr, protocol_mode);
}

INT32 bluetooth_hid_virtual_unplug(char *pbt_addr)
{
    FUNC_ENTRY;
    return linuxbt_hid_virtual_unplug_handler(pbt_addr);
}

INT32 bluetooth_hid_send_data(char *pbt_addr, char *data)
{
    FUNC_ENTRY;
    return linuxbt_hid_send_data_handler(pbt_addr, data);
}
/****************************************************
 * FUNCTION NAME: bluetooth_hid_send_control
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

#if 0
INT32 bluetooth_hid_send_control(char *pbt_addr, int pcontrol_mode)
{
    FUNC_ENTRY;
    return linuxbt_hid_send_control_handler(pbt_addr, pcontrol_mode);
}
#endif
/****************************************************
 * FUNCTION NAME: btaudio_get_current_hid_target_dev_info
 * PURPOSE:
 *      The function is used to get curent hid device info
 * INPUT:
 *      None
 *
 *
 * OUTPUT:
 *      pt_target_dev_info: device info (address and name)
 * RETURN:
 *      INT32:error code
 * NOTES:
 *      None
 */
INT32 btaudio_get_current_hid_target_dev_info(BT_TARGET_DEV_INFO *pt_target_dev_info)
{
    if (NULL == pt_target_dev_info)
    {
        BT_DBG_ERROR(BT_DEBUG_HID, "null pointer of pt_target_dev_info");
        return BT_ERR_STATUS_PARM_INVALID;
    }
    if (1)
    {
        memcpy(pt_target_dev_info, &g_bt_hid_target_dev_info, sizeof(BT_TARGET_DEV_INFO));
        BT_DBG_NORMAL(BT_DEBUG_HID, "current device address:%s", pt_target_dev_info->bdAddr);
        BT_DBG_NORMAL(BT_DEBUG_HID, "current device name:%s", pt_target_dev_info->name);
    }
    else
    {
        BT_DBG_NORMAL(BT_DEBUG_HID, "does not create connection");
        return BT_ERR_STATUS_FAIL;
    }
    return BT_SUCCESS;
}

/**/
INT32 bluetooth_hid_save_dev_info(bt_bdaddr_t *device_addr);

/****************************************************
 * FUNCTION NAME: bluetooth_handle_hid_connect_cb
 * PURPOSE:
 *      The function is used for callback for connected
 * INPUT:
 *      bd_addr:device address
 *
 *
 * OUTPUT:
 *      None
 * RETURN:
 *      INT32:error code
 * NOTES:
 *      None
 */
INT32 bluetooth_handle_hid_connect_cb(bt_bdaddr_t *bd_addr)
{
    FUNC_ENTRY;
    char local_address[MAX_BDADDR_LEN] = {0};
    int conn_dev_num = 0;
    UINT32 i = 0;
    UINT32 j = 0;/* j is for count device num */

    set_bt_hid_support(TRUE);
    bluetooth_btaddr_htos(bd_addr,local_address);

    BT_DBG_NORMAL(BT_DEBUG_HID, "[HID]Connect address:%s", local_address);
    memset(&fg_bt_hid_struct_data, 0, sizeof(BT_HID_CBK_STRUCT));
    fg_bt_hid_struct_data.bt_hid_event_type = BT_HID_CONNECTED;
    memcpy(fg_bt_hid_struct_data.current_addr, local_address, sizeof(fg_bt_hid_struct_data.current_addr));

    BT_DBG_INFO(BT_DEBUG_HID, "[HID]fg_bt_hid_struct_data.bt_hid_event_type=%d",
        fg_bt_hid_struct_data.bt_hid_event_type);
    BT_DBG_INFO(BT_DEBUG_HID, "[HID]fg_bt_hid_struct_data.current_addr=%s",
        fg_bt_hid_struct_data.current_addr);

    BT_CHECK_POINTER(BT_DEBUG_HID, g_pt_hid_status_list);
    i = g_pt_hid_status_list->head_idx;
    BT_DBG_INFO(BT_DEBUG_HID, "head_idx:%ld", (long)i);
    while (j < g_pt_hid_status_list->dev_num)
    {
        BT_DBG_INFO(BT_DEBUG_HID, "device_list[%ld].bdaddr:%s device_list[%ld].status:%d.",
                        (long)i,
                        g_pt_hid_status_list->device_list[i].bdAddr,
                        (long)i,
                        g_pt_hid_status_list->device_list[i].status
                       );

        if(BT_CONNECTED_STATUS == g_pt_hid_status_list->device_list[i].status)
        {
            conn_dev_num++;
        }

        i = (i + 1) % MAX_HID_DEV_NUM;
        j++;
    }

    if (conn_dev_num > MAX_HID_CONN_NUM)
    {
        BT_DBG_NORMAL(BT_DEBUG_HID, "Connection HID dev number larger than MAX_HID_CONN_NUM.");
        BT_DBG_NORMAL(BT_DEBUG_HID, "conn_dev_num=%d, MAX_HID_CONN_NUM=%d.", conn_dev_num, MAX_HID_CONN_NUM);
        fg_bt_hid_struct_data.bt_hid_event_type = BT_HID_CONNECTION_REJECT;
        fg_bt_hid_struct_data.bt_get_dev_hid_history_done = FALSE;

        linuxbt_gap_detrust_one_handler(local_address);
        if(HidCbk != NULL)
        {
            HidCbk(&fg_bt_hid_struct_data);
        }
        else
        {
            BT_DBG_ERROR(BT_DEBUG_HID, "null pointer of hidcbk");
        }
    }
    else
    {
        bluetooth_hid_save_dev_info(bd_addr);
    }

    return BT_SUCCESS;
}

/****************************************************
 * FUNCTION NAME: bluetooth_handle_hid_connect_fail_cb
 * PURPOSE:
 *      The function is used for callback for connect failed
 * INPUT:
 *      bd_addr:device address
 *
 *
 * OUTPUT:
 *      None
 * RETURN:
 *      INT32:error code
 * NOTES:
 *      None
 */
INT32 bluetooth_handle_hid_connect_fail_cb(bt_bdaddr_t *bd_addr, BT_HID_EVENT event)
{
    FUNC_ENTRY;
    char local_address[MAX_BDADDR_LEN] = {0};

    set_bt_hid_support(TRUE);
    bluetooth_btaddr_htos(bd_addr, local_address);

    BT_DBG_NORMAL(BT_DEBUG_HID, "[HID]Connect address:%s", local_address);
    memset(&fg_bt_hid_struct_data, 0, sizeof(BT_HID_CBK_STRUCT));
    fg_bt_hid_struct_data.bt_hid_event_type = event;
    memcpy(fg_bt_hid_struct_data.current_addr, local_address, sizeof(fg_bt_hid_struct_data.current_addr));

    BT_DBG_INFO(BT_DEBUG_HID, "[HID]fg_bt_hid_struct_data.bt_hid_event_type=%d",
        fg_bt_hid_struct_data.bt_hid_event_type);
    BT_DBG_INFO(BT_DEBUG_HID, "[HID]fg_bt_hid_struct_data.current_addr=%s",
        fg_bt_hid_struct_data.current_addr);


    if(HidCbk != NULL)
    {
        HidCbk(&fg_bt_hid_struct_data);
    }
    else
    {
        BT_DBG_ERROR(BT_DEBUG_HID, "null pointer of hidcbk");
    }
    return BT_SUCCESS;
}

/****************************************************
 * FUNCTION NAME: bluetooth_handle_hid_disconnect_cb
 * PURPOSE:
 *      The function is used for callback for disconnected
 * INPUT:
 *      bd_addr:device address
 *
 *
 * OUTPUT:
 *      None
 * RETURN:
 *      INT32:error code
 * NOTES:
 *      None
 */
INT32 bluetooth_handle_hid_disconnect_cb(bt_bdaddr_t *bd_addr)
{
    FUNC_ENTRY;
    INT32 ret = BT_SUCCESS;
    char local_address[MAX_BDADDR_LEN] = {0};

    ret = bluetooth_btaddr_htos(bd_addr, local_address);
    if(ret != BT_SUCCESS)
    {
        return ret;
    }

    set_bt_hid_support(TRUE);
    memset(&fg_bt_hid_struct_data, 0, sizeof(BT_HID_CBK_STRUCT));
    fg_bt_hid_struct_data.bt_hid_event_type = BT_HID_DISCONNECTED;
    memcpy(fg_bt_hid_struct_data.current_addr, local_address, sizeof(fg_bt_hid_struct_data.current_addr));
    BT_DBG_ERROR(BT_DEBUG_HID, "[HID]Connect address:%s", local_address);

    BT_DBG_ERROR(BT_DEBUG_HID, "[HID]fg_bt_hid_struct_data.bt_hid_event_type=%d",
        fg_bt_hid_struct_data.bt_hid_event_type);
    BT_DBG_ERROR(BT_DEBUG_HID, "[HID]fg_bt_hid_struct_data.current_addr=%s",
        fg_bt_hid_struct_data.current_addr);

    //modify the hid status from connected to paired. can not use API bluetooth_hid_setStatus.
    bluetooth_set_device_status(local_address, BT_PAIRED_STATUS);

    if(HidCbk != NULL)
    {
        HidCbk(&fg_bt_hid_struct_data);
    }
    else
    {
        BT_DBG_ERROR(BT_DEBUG_HID, "null pointer of hidcbk");
    }
    return BT_SUCCESS;
}

/****************************************************
 * FUNCTION NAME: _bt_update_hid_dev_history
 * PURPOSE:
 *      The function is used to update paired device list
 * INPUT:
 *      bd_addr:device address
 *
 *
 * OUTPUT:
 *      None
 * RETURN:
 *      INT32:error code
 * NOTES:
 *      None
 */
INT32 _bt_update_hid_dev_history(BT_TARGET_DEV_INFO *pt_dev_info, BT_HID_TARGET_DEV_LIST *pt_paired_dev_history)
{
    FUNC_ENTRY;
    if (NULL == pt_dev_info)
    {
        BT_DBG_ERROR(BT_DEBUG_HID, "null pointer of pt_dev_info!");
        return BT_ERR_STATUS_PARM_INVALID;
    }
    bluetooth_hid_setHistory(pt_dev_info, pt_paired_dev_history);
    fg_bt_hid_struct_data.bt_get_dev_hid_history_done = TRUE;

    if(HidCbk != NULL)
    {
        HidCbk(&fg_bt_hid_struct_data);
    }
    else
    {
        BT_DBG_ERROR(BT_DEBUG_HID, "null pointer of hidcbk");
    }

    BT_DBG_NORMAL(BT_DEBUG_HID, "Exit %s", __FUNCTION__);
    return BT_SUCCESS;
}

/****************************************************
 * FUNCTION NAME: bluetooth_hid_save_dev_info
 * PURPOSE:
 *      The function is used to save device to paired device list
 * INPUT:
 *      bd_addr:device address
 *
 *
 * OUTPUT:
 *      None
 * RETURN:
 *      INT32:error code
 * NOTES:
 *      None
 */
INT32 bluetooth_hid_save_dev_info(bt_bdaddr_t *device_addr)
{
    FUNC_ENTRY;
    BT_DEV_MAPPING_INFO target_mapping_dev;
    char bt_address[18];

    if(NULL == device_addr)
    {
        BT_DBG_ERROR(BT_DEBUG_HID, "null pointer of device_addr");
        return BT_ERR_STATUS_PARM_INVALID;
    }
    linuxbt_btaddr_htos(device_addr, bt_address);
    memset(&g_bt_hid_target_dev_info, 0, sizeof(BT_TARGET_DEV_INFO));
    strncpy(g_bt_hid_target_dev_info.bdAddr,
           bt_address,
           sizeof(g_bt_hid_target_dev_info.bdAddr)
          );

    strncpy(g_bt_hid_status.bdAddr,
           bt_address,
           sizeof(g_bt_hid_status.bdAddr)
          );
    g_bt_hid_status.status = BT_CONNECTED_STATUS;
    strncpy(target_mapping_dev.bdAddr,
       bt_address,
       sizeof(target_mapping_dev.bdAddr)
      );
    if(0 == bluetooth_get_dev_mapping_name(&target_mapping_dev))
    {
        memcpy(g_bt_hid_target_dev_info.name, target_mapping_dev.name, sizeof(g_bt_hid_target_dev_info.name));
        memcpy(fg_bt_hid_struct_data.name, target_mapping_dev.name, sizeof(fg_bt_hid_struct_data.name));

        g_bt_hid_target_dev_info.cod = target_mapping_dev.cod;
        fg_bt_hid_struct_data.cod = target_mapping_dev.cod;
    }
    else
    {
        BT_DBG_ERROR(BT_DEBUG_HID, "addr:%s, Not Found Name", bt_address);
        #if ENABLE_PHANTOM
        if(strlen("Wireless controller") < sizeof(g_bt_hid_target_dev_info.name))
        {
           memcpy(g_bt_hid_target_dev_info.name, "Wireless controller", sizeof(g_bt_hid_target_dev_info.name));
        }
        #endif
    }

    bluetooth_hid_setStatus(&g_bt_hid_status, g_pt_hid_status_list);
    _bt_update_hid_dev_history(&g_bt_hid_target_dev_info, g_pt_paired_hid_dev_history);
    FUNC_EXIT;
    return BT_SUCCESS;
}




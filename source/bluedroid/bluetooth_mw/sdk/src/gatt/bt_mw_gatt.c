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

/* FILE NAME:  bt_mw_gatt.c
 * AUTHOR: Xuemei Yang
 * PURPOSE:
 *      It provides GATTC and GATTS API to c_bt_mw_gatt and other mw layer modules.
 * NOTES:
 */

/*-----------------------------------------------------------------------------
                    include files
 ----------------------------------------------------------------------------*/
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include "bt_mw_common.h"
#include "bluetooth.h"
#include "linuxbt_gattc_if.h"
#include "linuxbt_gatts_if.h"
#include "bt_mw_gatt.h"
#include "c_bt_mw_gatt.h"


BtAppGATTCEventCbk      BtGattAppCbk = NULL;
BtAppGATTCRegClientCbk      BtGattRegClientCbk = NULL;
BtAppGATTCScanCbk BtGattScanCbk = NULL;
BtAppGATTCSearchCbk BtGattSearchCbk = NULL;
BtAppGATTCGetGattDbCbk BtGattGetGattDbCbk = NULL;
BtAppGATTCGetCharCbk BtGattGetCharCbk = NULL;
BtAppGATTCGetDescCbk BtGattGetDescCbk = NULL;
BtAppGATTCGetIncludedCbk BtGattGetIncludedCbk = NULL;
BtAppGATTCGetRegNotiCbk BtGattGetRegNotiCbk = NULL;
BtAppGATTCNotifyCbk BtGattNotifyCbk = NULL;
BtAppGATTCReadCharCbk BtGattReadCharCbk = NULL;
BtAppGATTCWriteCharCbk BtGattWriteCharCbk = NULL;
BtAppGATTCReadDescCbk BtGattReadDescCbk = NULL;
BtAppGATTCWriteDescCbk BtGattWriteDescCbk = NULL;
BtAppGATTScanFilterParamCbk BtGattScanFilterParamCbk = NULL;
BtAppGATTScanFilterStatusCbk BtGattScanFilterStatusCbk = NULL;
BtAppGATTScanFilterCfgCbk BtGattScanFilterCfgCbk = NULL;
BtAppGATTGetDeviceNameCbk BtGattGetDeviceNameCbk = NULL;

/*GATT server*/
BtAppGATTSEventCbk BtGattsAppCbk = NULL;
BtAppGATTSRegServerCbk BtGattsRegSeverCbk = NULL;
BtAppGATTSAddSrvcCbk BtGattsAddSrvcCbk = NULL;
BtAppGATTSAddInclCbk BtGattsAddInclCbk = NULL;
BtAppGATTSAddCharCbk BtGattsAddCharCbk = NULL;
BtAppGATTSAddDescCbk BtGattsAddDescCbk = NULL;
BtAppGATTSOpSrvcCbk BtGattsOpSrvcCbk = NULL;
BtAppGATTSReqReadCbk BtGattsReqReadCbk = NULL;
BtAppGATTSReqWriteCbk BtGattsReqWriteCbk = NULL;
BtAppGATTSIndSentCbk BtGattsIndSentCbk = NULL;


static BT_GATTS_REG_SERVER_RST_T gatts_reg_server_info = {0};
static BT_GATTS_ADD_SRVC_RST_T gatts_add_srvc_info = {0};
static BT_GATTS_ADD_CHAR_RST_T gatts_add_char_info = {0};
static BT_GATTS_SRVC_RST_T gatts_srvc_info = {0};
static BT_GATTS_CONNECT_RST_T gatts_connect_rst_info = {0};
static BT_GATTS_CONNECT_RST_T gatts_disconnect_rst_info = {0};
static BT_GATTS_ADD_INCL_RST_T gatts_add_incl_info = {0};
static BT_GATTS_ADD_DESCR_RST_T gatts_add_desc_info = {0};
static BT_GATTS_REQ_READ_RST_T gatts_req_read_info = {0};
static BT_GATTS_REQ_WRITE_RST_T gatts_req_write_info = {0};

/*GATT client*/
static BT_GATTC_REG_CLIENT_T g_reg_client_info = {0};
static BT_GATTC_SCAN_RST_T g_scan_rst_info = {0};
static BT_GATTC_CONNECT_RST_T g_connect_rst_info = {0};
static BT_GATTC_CONNECT_RST_T g_disconnect_rst_info = {0};
static BT_GATTC_SEARCH_RST_T g_search_rst_info = {0};
static BT_GATTC_GET_GATT_DB_T g_get_gatt_db_info = {0};
static BT_GATTC_GET_CHAR_RST_T g_get_char_rst_info = {0};
static BT_GATTC_GET_DESCR_RST_T g_get_desc_rst_info = {0};
static BT_GATTC_GET_INCL_RST_T g_get_included_rst_info = {0};
static BT_GATTC_GET_REG_NOTI_RST_T g_get_reg_noti_rst_info = {0};
static BT_GATTC_GET_NOTIFY_T g_get_notify_info = {0};
static BT_GATTC_READ_CHAR_RST_T g_get_read_char_rst_info = {0};
static BT_GATTC_WRITE_CHAR_RST_T g_get_write_char_rst_info = {0};
static BT_GATTC_READ_DESCR_RST_T g_get_read_desc_rst_info = {0};
static BT_GATTC_WRITE_DESCR_RST_T g_get_write_desc_rst_info = {0};
static BT_GATTC_GET_REMOTE_RSSI_T g_get_remote_rssi_info = {0};
static bluetooth_gatt_scan_filter_param_t g_scan_filter_param_info = {0};
static bluetooth_gatt_scan_filter_status_t g_scan_filter_status_info = {0};
static bluetooth_gatt_scan_filter_cfg_t g_scan_filter_cfg_info = {0};

static BOOL fg_gattc_register_done = FALSE;
static BOOL fg_gatts_register_done = FALSE;
static BOOL fg_gatts_add_service = FALSE;
static BOOL fg_gatts_add_char = FALSE;
static BOOL fg_gatts_start_service = FALSE;
static BOOL fg_gattc_adv_enable = FALSE;
static BOOL fg_gattc_adv_data = FALSE;

#if MTK_LINUX_GATTC_PTS_TEST
static INT32 PTS_TEST_MTU = 23;
static CHAR *pts_test_value = NULL;
static INT32 pts_write_offset = 0;
static INT32 pts_exe_write = 1;
static CHAR pts_test_read_value[BT_GATT_MAX_ATTR_LEN] = {0};
static INT32 PTS_MAX_LENGTH = 6;
static CHAR *pts_test_value_invalid = NULL;
static CHAR pts_test_read_value_invalid[BT_GATT_MAX_ATTR_LEN] = {0};

static INT32 pts_invalid_length = 0;
#endif
/**
 * FUNCTION NAME: bluetooth_gatt_register_cbk_fct
 * PURPOSE:
 *      The function is used to register APP callback function.
 * INPUT:
 *      func               -- gatt client app callback function structure
 * OUTPUT:
 *      None
 * RETURN:
 *      BT_SUCCESS                    -- Operate success.
 *      BT_ERR_STATUS_PARM_INVALID    -- paramter invalid.
 * NOTES:
 *      None
 */
INT32 bluetooth_gatt_register_cbk_fct(BT_APP_GATTC_CB_FUNC_T *func)
{
    BT_ERR_STATUS_T ret = BT_SUCCESS;

    BT_DBG_NORMAL(BT_DEBUG_GATT, "start bluetooth_gatt_register_cbk_fct");

    if (NULL == func)
    {
        BT_DBG_ERROR(BT_DEBUG_GATT, "callback func is null!");
        return BT_ERR_STATUS_PARM_INVALID;
    }
    /*1*/
    if (func->bt_gatt_event_cb)
    {
        BtGattAppCbk = func->bt_gatt_event_cb;
    }
    else
    {
        BT_DBG_NOTICE(BT_DEBUG_GATT,"gatt event callback func is null!");
        //BtGattAppCbk = NULL;
        //ret = BT_ERR_STATUS_PARM_INVALID;
    }
    /*2*/
    if (func->bt_gatt_reg_client_cb)
    {
        BtGattRegClientCbk = func->bt_gatt_reg_client_cb;
    }
    else
    {
        BT_DBG_NOTICE(BT_DEBUG_GATT,"gatt register client callback func is null!");
        //BtGattRegClientCbk = NULL;
        //ret = BT_ERR_STATUS_PARM_INVALID;
    }

    /*3*/
    if (func->bt_gatt_scan_cb)
    {
        BtGattScanCbk = func->bt_gatt_scan_cb;
    }
    else
    {
        BT_DBG_NOTICE(BT_DEBUG_GATT,"gatt scan callback func is null!");
        //BtGattScanCbk = NULL;
        //ret = BT_ERR_STATUS_PARM_INVALID;
    }
    /*4*/
    if (func->bt_gatt_search_cb)
    {
        BtGattSearchCbk = func->bt_gatt_search_cb;
    }
    else
    {
        BT_DBG_NOTICE(BT_DEBUG_GATT,"gatt search callback func is null!");
        //BtGattSearchCbk = NULL;
        //ret = BT_ERR_STATUS_PARM_INVALID;
    }
    /*5*/
    if (func->bt_gatt_get_char_cb)
    {
        BtGattGetCharCbk = func->bt_gatt_get_char_cb;
    }
    else
    {
        BT_DBG_NOTICE(BT_DEBUG_GATT,"gatt get characteristic callback func is null!");
        //BtGattGetCharCbk = NULL;
        //ret = BT_ERR_STATUS_PARM_INVALID;
    }
    /*6*/
    if (func->bt_gatt_get_desc_cb)
    {
        BtGattGetDescCbk = func->bt_gatt_get_desc_cb;
    }
    else
    {
        BT_DBG_NOTICE(BT_DEBUG_GATT,"gatt get descripor callback func is null!");
        //BtGattGetDescCbk = NULL;
        //ret = BT_ERR_STATUS_PARM_INVALID;
    }
    /*7*/
    if (func->bt_gatt_get_included_cb)
    {
        BtGattGetIncludedCbk = func->bt_gatt_get_included_cb;
    }
    else
    {
        BT_DBG_NOTICE(BT_DEBUG_GATT,"gatt get included callback func is null!");
        //BtGattGetIncludedCbk = NULL;
        //ret = BT_ERR_STATUS_PARM_INVALID;
    }
    /*8*/
    if (func->bt_gatt_get_reg_noti_cb)
    {
        BtGattGetRegNotiCbk = func->bt_gatt_get_reg_noti_cb;
    }
    else
    {
        BT_DBG_NOTICE(BT_DEBUG_GATT,"gatt register notify callback func is null!");
        //BtGattGetRegNotiCbk = NULL;
        //ret = BT_ERR_STATUS_PARM_INVALID;
    }
    /*9*/
    if (func->bt_gatt_notify_cb)
    {
        BtGattNotifyCbk = func->bt_gatt_notify_cb;
    }
    else
    {
        BT_DBG_NOTICE(BT_DEBUG_GATT,"gatt notify callback func is null!");
        //BtGattNotifyCbk = NULL;
        //ret = BT_ERR_STATUS_PARM_INVALID;
    }
    /*10*/
    if (func->bt_gatt_get_char_cb)
    {
        BtGattGetCharCbk = func->bt_gatt_get_char_cb;
    }
    else
    {
        BT_DBG_NOTICE(BT_DEBUG_GATT,"gatt get characteristic callback func is null!");
        //BtGattGetCharCbk = NULL;
        //ret = BT_ERR_STATUS_PARM_INVALID;
    }
    /*11*/
    if (func->bt_gatt_read_char_cb)
    {
        BtGattReadCharCbk = func->bt_gatt_read_char_cb;
    }
    else
    {
        BT_DBG_NOTICE(BT_DEBUG_GATT,"gatt read characteristic callback func is null!");
        //BtGattReadCharCbk = NULL;
        //ret = BT_ERR_STATUS_PARM_INVALID;
    }
    /*12*/
    if (func->bt_gatt_read_desc_cb)
    {
        BtGattReadDescCbk = func->bt_gatt_read_desc_cb;
    }
    else
    {
        BT_DBG_NOTICE(BT_DEBUG_GATT,"gatt read descripor callback func is null!");
        //BtGattReadDescCbk = NULL;
        //ret = BT_ERR_STATUS_PARM_INVALID;
    }
    /*13*/
    if (func->bt_gatt_write_char_cb)
    {
        BtGattWriteCharCbk = func->bt_gatt_write_char_cb;
    }
    else
    {
        BT_DBG_NOTICE(BT_DEBUG_GATT,"gatt write characteristic callback func is null!");
        //BtGattWriteCharCbk = NULL;
        //ret = BT_ERR_STATUS_PARM_INVALID;
    }
    /*14*/
    if (func->bt_gatt_write_desc_cb)
    {
        BtGattWriteDescCbk = func->bt_gatt_write_desc_cb;
    }
    else
    {
        BT_DBG_NOTICE(BT_DEBUG_GATT,"gatt write descripor callback func is null!");
        //BtGattWriteDescCbk = NULL;
        //ret = BT_ERR_STATUS_PARM_INVALID;
    }
    /*15*/
    if (func->bt_gatt_scan_filter_param_cb)
    {
        BtGattScanFilterParamCbk = func->bt_gatt_scan_filter_param_cb;
    }
    else
    {
        BT_DBG_NOTICE(BT_DEBUG_GATT,"gatt scan filter param callback func is null!");
        //BtGattScanFilterParamCbk = NULL;
        //ret = BT_ERR_NULL_POINTER;
    }

    /*16*/
    if (func->bt_gatt_scan_filter_status_cb)
    {
        BtGattScanFilterStatusCbk = func->bt_gatt_scan_filter_status_cb;
    }
    else
    {
        BT_DBG_NOTICE(BT_DEBUG_GATT,"gatt scan filter status callback func is null!");
        //BtGattScanFilterStatusCbk = NULL;
        //ret = BT_ERR_NULL_POINTER;
    }
    /*17*/
    if (func->bt_gatt_scan_filter_cfg_cb)
    {
        BtGattScanFilterCfgCbk = func->bt_gatt_scan_filter_cfg_cb;
    }
    else
    {
        BT_DBG_NOTICE(BT_DEBUG_GATT,"gatt scan filter cfg callback func is null!");
        //BtGattScanFilterCfgCbk = NULL;
        //ret = BT_ERR_NULL_POINTER;
    }
    /*18*/
    if (func->bt_gatt_get_device_name_cb)
    {
        BtGattGetDeviceNameCbk = func->bt_gatt_get_device_name_cb;
    }
    else
    {
        BT_DBG_NOTICE(BT_DEBUG_GATT,"gatt get device name callback func is null!");
        //BtGattGetDeviceNameCbk = NULL;
        //ret = BT_ERR_NULL_POINTER;
    }
    if (func->bt_gatt_get_gatt_db_cb)
    {
        BtGattGetGattDbCbk = func->bt_gatt_get_gatt_db_cb;
    }
    else
    {
        BT_DBG_NOTICE(BT_DEBUG_GATT,"get gatt db callback func is null!");
        //BtGattSearchCbk = NULL;
        //ret = BT_ERR_STATUS_PARM_INVALID;
    }
    return ret;
}


/**
 * FUNCTION NAME: bluetooth_gatt_base_init
 * PURPOSE:
 *      The function is used to register APP callback function and initialize static global varibles.
 * INPUT:
 *      func               -- gatt client app callback function structure
 * OUTPUT:
 *      None
 * RETURN:
 *      BT_SUCCESS                    -- Operate success.
 *      BT_ERR_STATUS_PARM_INVALID    -- paramter invalid.
 * NOTES:
 *      None
 */
INT32 bluetooth_gatt_base_init(BT_APP_GATTC_CB_FUNC_T *func)
{
    //FUNC_ENTRY;
    fg_gattc_register_done = FALSE;
    fg_gatts_register_done = FALSE;
    fg_gatts_add_service = FALSE;
    fg_gatts_add_char = FALSE;
    fg_gatts_start_service = FALSE;
    fg_gattc_adv_enable = FALSE;
    fg_gattc_adv_data = FALSE;
    return bluetooth_gatt_register_cbk_fct(func);
} /*bluetooth_gatt_init*/

//Basic Gatt Client function

/**
 * FUNCTION NAME: bluetooth_gattc_register_app_sync
 * PURPOSE:
 *      The function is used to synchronous register gatt client APP.
 * INPUT:
 *      pt_uuid            -- app uuid
 * OUTPUT:
 *      client_if          -- registered app identifer
 * RETURN:
 *	 BT_SUCCESS			             -- Operate success.
 * 	 BT_ERR_STATUS_FAIL 			   -- Operate fail.
 * 	 BT_ERR_STATUS_PARM_INVALID 	   -- paramter is invalid.
 * NOTES:
 *      None
 */
INT32 bluetooth_gattc_register_app_sync(CHAR *pt_uuid, INT32 *client_if)
{
    //FUNC_ENTRY;
    INT32 i4_loop = 300;
    BT_ERR_STATUS_T ret = BT_SUCCESS;
    fg_gattc_register_done = FALSE;
    ret = linuxbt_gattc_register_app(pt_uuid);
    while (0<i4_loop && !fg_gattc_register_done)
    {
        BT_DBG_INFO(BT_DEBUG_GATT, "Wait gattc register app finish:%ld", (long)i4_loop);
        /*x_thread_delay(10);*/
        usleep(10*1000);
        i4_loop--;
    }
    if (fg_gattc_register_done)
    {
        *client_if = g_reg_client_info.client_if;
    }
    else
    {
        BT_DBG_ERROR(BT_DEBUG_GATT,"gattc register app failed");
        *client_if = -1;
        ret = BT_ERR_STATUS_FAIL;
    }
    return ret;
}

/**
 * FUNCTION NAME: bluetooth_gattc_register_app
 * PURPOSE:
 *      The function is used to register gatt client APP.
 * INPUT:
 *      pt_uuid               -- app uuid
 * OUTPUT:
 *      None
 * RETURN:
 *      BT_SUCCESS                        -- Operate success.
 *      BT_ERR_STATUS_FAIL                -- Operate fail.
 *      BT_ERR_STATUS_PARM_INVALID        -- paramter is invalid.
 * NOTES:
 *      None
 */
INT32 bluetooth_gattc_register_app(CHAR *pt_uuid)
{
    return linuxbt_gattc_register_app(pt_uuid);
}

/**
 * FUNCTION NAME: bluetooth_gattc_unregister_app
 * PURPOSE:
 *      The function is used to deregister gatt client APP.
 * INPUT:
 *      client_if              -- registered app identifer
 * OUTPUT:
 *      None
 * RETURN:
 *      BT_SUCCESS                        -- Operate success.
 *      BT_ERR_STATUS_FAIL                -- Operate fail.
 *      BT_ERR_STATUS_PARM_INVALID        -- paramter is invalid.
 * NOTES:
 *      None
 */
INT32 bluetooth_gattc_unregister_app(INT32 client_if)
{
    return linuxbt_gattc_unregister_app(client_if);
}

/**
 * FUNCTION NAME: bluetooth_gattc_scan
 * PURPOSE:
 *      The function is used for gatt client scan remote device.
 * INPUT:
 *      None
 * OUTPUT:
 *      None
 * RETURN:
 *      BT_SUCCESS                        -- Operate success.
 *      BT_ERR_STATUS_FAIL                -- Operate fail.
 *      BT_ERR_STATUS_PARM_INVALID        -- paramter is invalid.
 * NOTES:
 *      None
 */
INT32 bluetooth_gattc_scan()
{
    return linuxbt_gattc_scan();
}

/**
 * FUNCTION NAME: bluetooth_gattc_stop_scan
 * PURPOSE:
 *      The function is used for gatt client stop scan remote device.
 * INPUT:
 *      client_if              -- registered app identifer
 * OUTPUT:
 *      None
 * RETURN:
 *      BT_SUCCESS                        -- Operate success.
 *      BT_ERR_STATUS_FAIL                -- Operate fail.
 *      BT_ERR_STATUS_PARM_INVALID        -- paramter is invalid.
 * NOTES:
 *      None
 */
INT32 bluetooth_gattc_stop_scan()
{
    return linuxbt_gattc_stop_scan();
}

/**
 * FUNCTION NAME: bluetooth_gattc_open
 * PURPOSE:
 *      The function is used for gatt client connect with remote device.
 * INPUT:
 *      client_if              -- registered app identifer
 *      bt_addr                -- remote device bt address
 *      is_direct              -- is direct connection or background connection
 *      transport              -- transport type:(0 : auto, 1 : BREDR, 2 : LE)
 * OUTPUT:
 *      None
 * RETURN:
 *      BT_SUCCESS                        -- Operate success.
 *      BT_ERR_STATUS_FAIL                -- Operate fail.
 *      BT_ERR_STATUS_PARM_INVALID        -- paramter is invalid.
 * NOTES:
 *      None
 */
INT32 bluetooth_gattc_open(INT32 client_if, CHAR *bt_addr, UINT8 is_direct, INT32 transport)
{
    return linuxbt_gattc_open(client_if, bt_addr, is_direct, transport);
}

/**
 * FUNCTION NAME: bluetooth_gattc_close
 * PURPOSE:
 *      The function is used for gatt client disconnect with remote device.
 * INPUT:
 *      client_if              -- registered app identifer
 *      bt_addr                -- remote device bt address
 *      conn_id                -- connection id
 * OUTPUT:
 *      None
 * RETURN:
 *      BT_SUCCESS                        -- Operate success.
 *      BT_ERR_STATUS_FAIL                -- Operate fail.
 *      BT_ERR_STATUS_PARM_INVALID        -- paramter is invalid.
 * NOTES:
 *      None
 */
INT32 bluetooth_gattc_close(INT32 client_if, CHAR *bt_addr, INT32 conn_id)
{
    return linuxbt_gattc_close(client_if, bt_addr, conn_id);
}

/**
 * FUNCTION NAME: bluetooth_gattc_listen
 * PURPOSE:
 *      The function is used for gatt client listening operation.
 * INPUT:
 *      client_if              -- registered app identifer
 * OUTPUT:
 *      None
 * RETURN:
 *      BT_SUCCESS                        -- Operate success.
 *      BT_ERR_STATUS_FAIL                -- Operate fail.
 *      BT_ERR_STATUS_PARM_INVALID        -- paramter is invalid.
 * NOTES:
 *      None
 */
INT32 bluetooth_gattc_listen(INT32 client_if)
{
    return linuxbt_gattc_listen(client_if);
}

/**
 * FUNCTION NAME: bluetooth_gattc_refresh
 * PURPOSE:
 *      The function is used for gatt client refresh operation.
 * INPUT:
 *      client_if              -- registered app identifer
 *      bt_addr                -- remote device bt address
 * OUTPUT:
 *      None
 * RETURN:
 *      BT_SUCCESS                        -- Operate success.
 *      BT_ERR_STATUS_FAIL                -- Operate fail.
 *      BT_ERR_STATUS_PARM_INVALID        -- paramter is invalid.
 * NOTES:
 *      None
 */
INT32 bluetooth_gattc_refresh(INT32 client_if, CHAR *bt_addr)
{
    return linuxbt_gattc_refresh(client_if, bt_addr);
}

/**
 * FUNCTION NAME: bluetooth_gattc_search_service
 * PURPOSE:
 *      The function is used for gatt client search service on the remote device.
 * INPUT:
 *      conn_id                -- connection id
 *      pt_uuid                -- service uuid
 * OUTPUT:
 *      None
 * RETURN:
 *      BT_SUCCESS                        -- Operate success.
 *      BT_ERR_STATUS_FAIL                -- Operate fail.
 *      BT_ERR_STATUS_PARM_INVALID        -- paramter is invalid.
 * NOTES:
 *      None
 */
INT32 bluetooth_gattc_search_service(INT32 conn_id, CHAR *pt_uuid)
{
    return linuxbt_gattc_search_service(conn_id, pt_uuid);
}

INT32 bluetooth_gattc_get_gatt_db(INT32 conn_id)
{
    return linuxbt_gattc_get_gatt_db(conn_id);
}

/**
 * FUNCTION NAME: bluetooth_gattc_get_char
 * PURPOSE:
 *      The function is used for gatt client get characiteristic on loacal cache database.
 * INPUT:
 *      conn_id                -- connection id
 *      pt_service_uuid        -- service uuid
 *      service_inst_id        -- service instance id
 *      pt_start_uuid          -- first charateristic uuid
 *      start_inst_id          -- first charateristic instance id
 * OUTPUT:
 *      None
 * RETURN:
 *      BT_SUCCESS                        -- Operate success.
 *      BT_ERR_STATUS_FAIL                -- Operate fail.
 *      BT_ERR_STATUS_PARM_INVALID        -- paramter is invalid.
 * NOTES:
 *      None
 */
INT32 bluetooth_gattc_get_char(INT32 conn_id,
                               CHAR *pt_service_uuid,
                               INT32 service_inst_id,
                               CHAR *pt_start_uuid,
                               INT32 start_inst_id)
{
    return linuxbt_gattc_get_char(conn_id,
                                  pt_service_uuid,
                                  service_inst_id,
                                  pt_start_uuid,
                                  start_inst_id);
}

/**
 * FUNCTION NAME: bluetooth_gattc_get_descr
 * PURPOSE:
 *      The function is used for gatt client get char description on loacal cache database.
 * INPUT:
 *      conn_id                -- connection id
 *      pt_service_uuid        -- service uuid
 *      service_inst_id        -- service instance id
 *      pt_char_uuid           -- first charateristic uuid
 *      char_inst_id           -- first charateristic instance id
 *      pt_start_uuid          -- first char description uuid
 *      start_inst_id          -- first char description instance id
 * OUTPUT:
 *      None
 * RETURN:
 *      BT_SUCCESS                        -- Operate success.
 *      BT_ERR_STATUS_FAIL                -- Operate fail.
 *      BT_ERR_STATUS_PARM_INVALID        -- paramter is invalid.
 * NOTES:
 *      None
 */
INT32 bluetooth_gattc_get_descr(INT32 conn_id,
                                CHAR *pt_service_uuid,
                                INT32 service_inst_id,
                                CHAR *pt_char_uuid,
                                INT32 char_inst_id,
                                CHAR *pt_start_uuid,
                                INT32 start_inst_id)
{
    return linuxbt_gattc_get_descr(conn_id,
                                   pt_service_uuid,
                                   service_inst_id,
                                   pt_char_uuid,
                                   char_inst_id,
                                   pt_start_uuid,
                                   start_inst_id);
}

/**
 * FUNCTION NAME: bluetooth_gattc_get_incl_service
 * PURPOSE:
 *      The function is used for gatt client get included service on loacal cache database.
 * INPUT:
 *      conn_id                -- connection id
 *      pt_service_uuid        -- service uuid
 *      service_inst_id        -- service instance id
 *      pt_incl_uuid           -- included service uuid
 *      incl_inst_id           -- included service instance id
 * OUTPUT:
 *      None
 * RETURN:
 *      BT_SUCCESS                        -- Operate success.
 *      BT_ERR_STATUS_FAIL                -- Operate fail.
 *      BT_ERR_STATUS_PARM_INVALID        -- paramter is invalid.
 * NOTES:
 *      None
 */
INT32 bluetooth_gattc_get_incl_service(INT32 conn_id,
                                       CHAR *pt_service_uuid,
                                       INT32 service_inst_id,
                                       CHAR *pt_incl_uuid,
                                       INT32 incl_inst_id)
{
    return linuxbt_gattc_get_incl_service(conn_id,
                                          pt_service_uuid,
                                          service_inst_id,
                                          pt_incl_uuid,
                                          incl_inst_id);
}

/**
 * FUNCTION NAME: bluetooth_gattc_read_char
 * PURPOSE:
 *      The function is used for gatt client read characteristic from remote device
 * INPUT:
 *      conn_id                -- connection id
 *      pt_service_uuid        -- service uuid
 *      service_inst_id        -- service instance id
 *      pt_char_uuid           -- characteristic uuid
 *      char_inst_id           -- characteristic instance id
 *      auth_req               -- authentication request flag
 * OUTPUT:
 *      None
 * RETURN:
 *      BT_SUCCESS                        -- Operate success.
 *      BT_ERR_STATUS_FAIL                -- Operate fail.
 *      BT_ERR_STATUS_PARM_INVALID        -- paramter is invalid.
 * NOTES:
 *      None
 */
INT32 bluetooth_gattc_read_char(INT32 conn_id,
                                INT32 char_handle,
                                INT32 auth_req)
{
    return linuxbt_gattc_read_char(conn_id,
                                   char_handle,
                                   auth_req);
}

/**
 * FUNCTION NAME: bluetooth_gattc_read_descr
 * PURPOSE:
 *      The function is used for gatt client read char description from remote device
 * INPUT:
 *      conn_id                -- connection id
 *      pt_service_uuid        -- service uuid
 *      service_inst_id        -- service instance id
 *      pt_char_uuid           -- characteristic uuid
 *      char_inst_id           -- characteristic instance id
 *      pt_descr_uuid          -- char descriptor uuid
 *      descr_inst_id          -- char descriptor instance id
 *      auth_req               -- authentication request flag
 * OUTPUT:
 *      None
 * RETURN:
 *      BT_SUCCESS                        -- Operate success.
 *      BT_ERR_STATUS_FAIL                -- Operate fail.
 *      BT_ERR_STATUS_PARM_INVALID        -- paramter is invalid.
 * NOTES:
 *      None
 */
INT32 bluetooth_gattc_read_descr(INT32 conn_id,
                                 INT32 descr_handle,
                                 INT32 auth_req)
{
    return linuxbt_gattc_read_descr(conn_id,
                                    descr_handle,
                                    auth_req);
}

/**
 * FUNCTION NAME: bluetooth_gattc_write_char
 * PURPOSE:
 *      The function is used for gatt client write characteristic to remote device
 * INPUT:
 *      conn_id                -- connection id
 *      pt_service_uuid        -- service uuid
 *      service_inst_id        -- service instance id
 *      pt_char_uuid           -- characteristic uuid
 *      char_inst_id           -- characteristic instance id
 *      write_type             -- write type(1:WRITE_TYPE_NO_RSP  2:WRITE_TYPE_REQUEST  3:WRITE_TYPE_PREPARE)
 *      len                    -- write characteristic value length
 *      auth_req               -- authentication request flag
 *      value                  -- write characteristic value
 * OUTPUT:
 *      None
 * RETURN:
 *      BT_SUCCESS                        -- Operate success.
 *      BT_ERR_STATUS_FAIL                -- Operate fail.
 *      BT_ERR_STATUS_PARM_INVALID        -- paramter is invalid.
 * NOTES:
 *      None
 */
INT32 bluetooth_gattc_write_char(INT32 conn_id,
                                 INT32 char_handle,
                                 INT32 write_type,
                                 INT32 len,
                                 INT32 auth_req,
                                 CHAR *value)
{
    return linuxbt_gattc_write_char(conn_id,
                                    char_handle,
                                    write_type,
                                    len,
                                    auth_req,
                                    value);
}

INT32 bluetooth_gattc_execute_write(INT32 conn_id, INT32 execute)
{
    return linuxbt_gattc_execute_write(conn_id, execute);
}


/**
 * FUNCTION NAME: bluetooth_gattc_write_descr
 * PURPOSE:
 *      The function is used for gatt client write char description to remote device
 * INPUT:
 *      conn_id                -- connection id
 *      pt_service_uuid        -- service uuid
 *      service_inst_id        -- service instance id
 *      pt_char_uuid           -- characteristic uuid
 *      char_inst_id           -- characteristic instance id
 *      pt_descr_uuid          -- char description uuid
 *      descr_inst_id          -- char description instance id
 *      write_type             -- write type(1:WRITE_TYPE_NO_RSP  2:WRITE_TYPE_REQUEST  3:WRITE_TYPE_PREPARE)
 *      len                    -- write characteristic value length
 *      auth_req               -- authentication request flag
 *      value                  -- write characteristic value
 * OUTPUT:
 *      None
 * RETURN:
 *      BT_SUCCESS                        -- Operate success.
 *      BT_ERR_STATUS_FAIL                -- Operate fail.
 *      BT_ERR_STATUS_PARM_INVALID        -- paramter is invalid.
 * NOTES:
 *      None
 */
INT32 bluetooth_gattc_write_descr(INT32 conn_id,
                                  INT32 descr_handle,
                                  INT32 write_type,
                                  INT32 len,
                                  INT32 auth_req,
                                  CHAR *value)
{
    return linuxbt_gattc_write_descr(conn_id,
                                     descr_handle,
                                     write_type,
                                     len,
                                     auth_req,
                                     value);
}

/**
 * FUNCTION NAME: bluetooth_gattc_reg_noti
 * PURPOSE:
 *      The function is used for gatt client register notification
 * INPUT:
 *      client_if              -- registered client app identifier
 *      bt_addr                -- remote device bt address
 *      pt_service_uuid        -- service uuid
 *      service_inst_id        -- service instance id
 *      pt_char_uuid           -- characteristic uuid
 *      char_inst_id           -- characteristic instance id
 * OUTPUT:
 *      None
 * RETURN:
 *      BT_SUCCESS                        -- Operate success.
 *      BT_ERR_STATUS_FAIL                -- Operate fail.
 *      BT_ERR_STATUS_PARM_INVALID        -- paramter is invalid.
 * NOTES:
 *      None
 */
INT32 bluetooth_gattc_reg_noti(INT32 client_if,
                               CHAR *bt_addr,
                               INT32 char_handle)
{
    return linuxbt_gattc_reg_noti(client_if,
                                  bt_addr,
                                  char_handle);
}

/**
 * FUNCTION NAME: bluetooth_gattc_dereg_noti
 * PURPOSE:
 *      The function is used for gatt client deregister notification
 * INPUT:
 *      client_if              -- registered client app identifier
 *      bt_addr                -- remote device bt address
 *      pt_service_uuid        -- service uuid
 *      service_inst_id        -- service instance id
 *      pt_char_uuid           -- characteristic uuid
 *      char_inst_id           -- characteristic instance id
 * OUTPUT:
 *      None
 * RETURN:
 *      BT_SUCCESS                        -- Operate success.
 *      BT_ERR_STATUS_FAIL                -- Operate fail.
 *      BT_ERR_STATUS_PARM_INVALID        -- paramter is invalid.
 * NOTES:
 *      None
 */
INT32 bluetooth_gattc_dereg_noti(INT32 client_if,
                                 CHAR *bt_addr,
                                 INT32 char_handle)
{
    return linuxbt_gattc_dereg_noti(client_if,
                                    bt_addr,
                                    char_handle);
}

/**
 * FUNCTION NAME: bluetooth_gattc_read_rssi
 * PURPOSE:
 *      The function is used for gatt client read the received signal strength indicator
 * INPUT:
 *      client_if              -- registered client app identifier
 *      bt_addr                -- remote device bt address
 * OUTPUT:
 *      None
 * RETURN:
 *      BT_SUCCESS                        -- Operate success.
 *      BT_ERR_STATUS_FAIL                -- Operate fail.
 *      BT_ERR_STATUS_PARM_INVALID        -- paramter is invalid.
 * NOTES:
 *      None
 */
INT32 bluetooth_gattc_read_rssi(INT32 client_if, CHAR *bt_addr)
{
    return linuxbt_gattc_read_rssi(client_if, bt_addr);
}

/*after receive cbk notify, APP can call following API to get information*/
/**
 * FUNCTION NAME: bluetooth_gattc_get_connect_result_info
 * PURPOSE:
 *      The function is used for gatt client to get connection result information
 * INPUT:
 *      None
 * OUTPUT:
 *      connect_rst_info       -- connection result information
 * RETURN:
 *      None
 * NOTES:
 *      None
 */
VOID bluetooth_gattc_get_connect_result_info(BT_GATTC_CONNECT_RST_T *connect_rst_info)
{
    //FUNC_ENTRY;
    if (NULL == connect_rst_info)
    {
        BT_DBG_ERROR(BT_DEBUG_GATT,"[GATT] null pointer");
        return;
    }
    memcpy(connect_rst_info, &g_connect_rst_info, sizeof(BT_GATTC_CONNECT_RST_T));
}

/**
 * FUNCTION NAME: bluetooth_gattc_get_disconnect_result_info
 * PURPOSE:
 *      The function is used for gatt client to get disconnection result information
 * INPUT:
 *      None
 * OUTPUT:
 *      disconnect_rst_info       -- disconnection result information
 * RETURN:
 *      None
 * NOTES:
 *      None
 */
VOID bluetooth_gattc_get_disconnect_result_info(BT_GATTC_CONNECT_RST_T *disconnect_rst_info)
{
    //FUNC_ENTRY;
    if (NULL == disconnect_rst_info)
    {
        BT_DBG_ERROR(BT_DEBUG_GATT,"[GATT] null pointer");
        return;
    }
    memcpy(disconnect_rst_info, &g_disconnect_rst_info, sizeof(BT_GATTC_CONNECT_RST_T));
}

/**
 * FUNCTION NAME: bluetooth_gattc_read_rssi_result_info
 * PURPOSE:
 *      The function is used for gatt client to read received signal strength indicator result inforamtion
 * INPUT:
 *      None
 * OUTPUT:
 *      get_remote_rssi_info     -- received signal strength indicator result inforamtion
 * RETURN:
 *      None
 * NOTES:
 *      None
 */
VOID bluetooth_gattc_read_rssi_result_info(BT_GATTC_GET_REMOTE_RSSI_T *get_remote_rssi_info)
{
    //FUNC_ENTRY;
    if (NULL == get_remote_rssi_info)
    {
        BT_DBG_ERROR(BT_DEBUG_GATT,"[GATT] null pointer");
        return;
    }
    memcpy(get_remote_rssi_info, &g_get_remote_rssi_info, sizeof(BT_GATTC_GET_REMOTE_RSSI_T));
}

/**
 * FUNCTION NAME: bluetooth_gattc_conn_parameter_update
 * PURPOSE:
 *      The function is used for gatt client to update connection paramters
 * INPUT:
 *      pbt_addr               -- remote device bt address
 *      min_interval           -- the minimum allowed connection interval
 *      max_interval           -- the maximum allowed connection interval
 *      latency                -- the maximum allowed connection latency
 *      timeout                -- define the link supervision timeout
 * OUTPUT:
 *      None
 * RETURN:
 *      BT_SUCCESS                        -- Operate success.
 *      BT_ERR_STATUS_FAIL                -- Operate fail.
 *      BT_ERR_STATUS_PARM_INVALID        -- paramter is invalid.
 * NOTES:
 *      None
 */
INT32 bluetooth_gattc_conn_parameter_update(CHAR *pbt_addr,
        INT32 min_interval,
        INT32 max_interval,
        INT32 latency,
        INT32 timeout)
{
    return linuxbt_gattc_conn_parameter_update(pbt_addr,
            min_interval,
            max_interval,
            latency,
            timeout);
}

/**
 * FUNCTION NAME: bluetooth_gattc_set_scan_parameters
 * PURPOSE:
 *      The function is used for gatt client to set scan paramters
 * INPUT:
 *      scan_interval          -- how frequently the controller should scan
 *      scan_window            -- how long the controller should scan
 * OUTPUT:
 *      None
 * RETURN:
 *      BT_SUCCESS                        -- Operate success.
 *      BT_ERR_STATUS_FAIL                -- Operate fail.
 *      BT_ERR_STATUS_PARM_INVALID        -- paramter is invalid.
 * NOTES:
 *      None
 */
INT32 bluetooth_gattc_set_scan_parameters(INT32 client_if, INT32 scan_interval, INT32 scan_window)
{
    return linuxbt_gattc_set_scan_parameters(client_if, scan_interval, scan_window);
}

/**
 * FUNCTION NAME: bluetooth_gattc_multi_adv_enable
 * PURPOSE:
 *      The function is used for gatt to enable multiple advertising
 * INPUT:
 *      client_if              -- registered client app identifier
 *      min_interval           -- minimum advertising interval
 *      max_interval           -- maximum advertising interval
 *      adv_type               -- advertising type(0:ADV_IND  1:ADV_DIRECT_IND   2:ADV_SCAN_IND
 *                                                 3:ADV_NONCONN_IND  4:SCAN_RSP)
 *      chnl_map               -- advertising channel map
 *      tx_power               -- transmit power(unit in dBm)
 *      timeout_s              -- not used.
 * OUTPUT:
 *      None
 * RETURN:
 *      BT_SUCCESS                        -- Operate success.
 *      BT_ERR_STATUS_FAIL                -- Operate fail.
 *      BT_ERR_STATUS_PARM_INVALID        -- paramter is invalid.
 * NOTES:
 *      None
 */
INT32 bluetooth_gattc_multi_adv_enable(INT32 client_if,
                                       INT32 min_interval,
                                       INT32 max_interval,
                                       INT32 adv_type,
                                       INT32 chnl_map,
                                       INT32 tx_power,
                                       INT32 timeout_s)
{
    return linuxbt_gattc_multi_adv_enable(client_if,
                                          min_interval,
                                          max_interval,
                                          adv_type,
                                          chnl_map,
                                          tx_power,
                                          timeout_s);
}

/**
 * FUNCTION NAME: bluetooth_gattc_multi_adv_enable_sync
 * PURPOSE:
 *      The function is used for gatt to enable multiple advertising
 * INPUT:
 *      client_if              -- registered client app identifier
 *      min_interval           -- minimum advertising interval
 *      max_interval           -- maximum advertising interval
 *      adv_type               -- advertising type(0:ADV_IND  1:ADV_DIRECT_IND   2:ADV_SCAN_IND
 *                                                 3:ADV_NONCONN_IND  4:SCAN_RSP)
 *      chnl_map               -- advertising channel map
 *      tx_power               -- transmit power(unit in dBm)
 *      timeout_s              -- not used.
 * OUTPUT:
 *      None
 * RETURN:
 *      BT_SUCCESS                        -- Operate success.
 *      BT_ERR_STATUS_FAIL                -- Operate fail.
 *      BT_ERR_STATUS_PARM_INVALID        -- paramter is invalid.
 * NOTES:
 *      None
 */
INT32 bluetooth_gattc_multi_adv_enable_sync(INT32 client_if,
        INT32 min_interval,
        INT32 max_interval,
        INT32 adv_type,
        INT32 chnl_map,
        INT32 tx_power,
        INT32 timeout_s)
{
    INT32 i4_loop = 300;
    BT_ERR_STATUS_T ret = BT_SUCCESS;
    fg_gattc_adv_enable = FALSE;
    ret = linuxbt_gattc_multi_adv_enable(client_if,
                                         min_interval,
                                         max_interval,
                                         adv_type,
                                         chnl_map,
                                         tx_power,
                                         timeout_s);
    while (0<i4_loop && !fg_gattc_adv_enable)
    {
        BT_DBG_INFO(BT_DEBUG_GATT, "Wait gattc adv enable finish:%ld", (long)i4_loop);
        /*x_thread_delay(10);*/
        usleep(10*1000);
        i4_loop--;
    }
    if (fg_gattc_adv_enable)
    {
        return ret;
    }
    else
    {
        BT_DBG_ERROR(BT_DEBUG_GATT, "gattc adb enable failed");
        return BT_ERR_STATUS_FAIL;
    }
}

/**
 * FUNCTION NAME: bluetooth_gattc_multi_adv_update
 * PURPOSE:
 *      The function is used for gatt to update multiple advertising parameters
 * INPUT:
 *      client_if              -- registered client app identifier
 *      min_interval           -- minimum advertising interval
 *      max_interval           -- maximum advertising interval
 *      adv_type               -- advertising type(0:ADV_IND  1:ADV_DIRECT_IND   2:ADV_SCAN_IND
 *                                                 3:ADV_NONCONN_IND  4:SCAN_RSP)
 *      chnl_map               -- advertising channel map
 *      tx_power               -- transmit power(unit in dBm)
 *      timeout_s              -- not used.
 * OUTPUT:
 *      None
 * RETURN:
 *      BT_SUCCESS                        -- Operate success.
 *      BT_ERR_STATUS_FAIL                -- Operate fail.
 *      BT_ERR_STATUS_PARM_INVALID        -- paramter is invalid.
 * NOTES:
 *      None
 */
INT32 bluetooth_gattc_multi_adv_update(INT32 client_if,
                                       INT32 min_interval,
                                       INT32 max_interval,
                                       INT32 adv_type,
                                       INT32 chnl_map,
                                       INT32 tx_power,
                                       INT32 timeout_s)
{
    return linuxbt_gattc_multi_adv_update(client_if,
                                          min_interval,
                                          max_interval,
                                          adv_type,
                                          chnl_map,
                                          tx_power,
                                          timeout_s);
}

/**
 * FUNCTION NAME: bluetooth_gattc_multi_adv_setdata
 * PURPOSE:
 *      The function is used for gatt to set multiple advertising data
 * INPUT:
 *      client_if              -- registered client app identifier
 *      set_scan_rsp           -- set scan response flag
 *      include_name           -- include device name flag
 *      include_txpower        -- include transmit power flag
 *      appearance             -- appearance data
 *      manufacturer_len       -- manufacturer data length
 *      manufacturer_data      -- manufacturer data
 *      service_data_len       -- service data length
 *      service_data           -- service data
 *      service_uuid_len       -- service uuid length
 *      service_uuid           -- service uuid
 *      le_name_len            -- device name length
 *      le_name                -- device name
 * OUTPUT:
 *      None
 * RETURN:
 *      BT_SUCCESS                        -- Operate success.
 *      BT_ERR_STATUS_FAIL                -- Operate fail.
 *      BT_ERR_STATUS_PARM_INVALID        -- paramter is invalid.
 * NOTES:
 *      None
 */
INT32 bluetooth_gattc_multi_adv_setdata(INT32 client_if,
                                        UINT8 set_scan_rsp,
                                        UINT8 include_name,
                                        UINT8 include_txpower,
                                        INT32 appearance,
                                        INT32 manufacturer_len,
                                        CHAR* manufacturer_data,
                                        INT32 service_data_len,
                                        CHAR* service_data,
                                        INT32 service_uuid_len,
                                        CHAR* service_uuid)
{
    return linuxbt_gattc_multi_adv_setdata(client_if,
                                           set_scan_rsp,
                                           include_name,
                                           include_txpower,
                                           appearance,
                                           manufacturer_len,
                                           manufacturer_data,
                                           service_data_len,
                                           service_data,
                                           service_uuid_len,
                                           service_uuid);

}

/**
 * FUNCTION NAME: bluetooth_gattc_multi_adv_setdata_sync
 * PURPOSE:
 *      The function is used for gatt to set multiple advertising data
 * INPUT:
 *      client_if              -- registered client app identifier
 *      set_scan_rsp           -- set scan response flag
 *      include_name           -- include device name flag
 *      include_txpower        -- include transmit power flag
 *      appearance             -- appearance data
 *      manufacturer_len       -- manufacturer data length
 *      manufacturer_data      -- manufacturer data
 *      service_data_len       -- service data length
 *      service_data           -- service data
 *      service_uuid_len       -- service uuid length
 *      service_uuid           -- service uuid
 *      le_name_len            -- device name length
 *      le_name                -- device name
 * OUTPUT:
 *      None
 * RETURN:
 *      BT_SUCCESS                        -- Operate success.
 *      BT_ERR_STATUS_FAIL                -- Operate fail.
 *      BT_ERR_STATUS_PARM_INVALID        -- paramter is invalid.
 * NOTES:
 *      None
 */
INT32 bluetooth_gattc_multi_adv_setdata_sync(INT32 client_if,
        UINT8 set_scan_rsp,
        UINT8 include_name,
        UINT8 include_txpower,
        INT32 appearance,
        INT32 manufacturer_len,
        CHAR* manufacturer_data,
        INT32 service_data_len,
        CHAR* service_data,
        INT32 service_uuid_len,
        CHAR* service_uuid)
{
    INT32 i4_loop = 300;
    BT_ERR_STATUS_T ret = BT_SUCCESS;
    fg_gattc_adv_data = FALSE;
    ret = linuxbt_gattc_multi_adv_setdata(client_if,
                                          set_scan_rsp,
                                          include_name,
                                          include_txpower,
                                          appearance,
                                          manufacturer_len,
                                          manufacturer_data,
                                          service_data_len,
                                          service_data,
                                          service_uuid_len,
                                          service_uuid);
    while (0<i4_loop && !fg_gattc_adv_data)
    {
        BT_DBG_INFO(BT_DEBUG_GATT, "Wait gattc adv data finish:%ld", (long)i4_loop);
        /*x_thread_delay(10);*/
        usleep(10*1000);
        i4_loop--;
    }
    if (fg_gattc_adv_data)
    {
        return ret;
    }
    else
    {
        BT_DBG_ERROR(BT_DEBUG_GATT, "gattc adb data failed");
        return BT_ERR_STATUS_FAIL;
    }
}

/**
 * FUNCTION NAME: bluetooth_gattc_multi_adv_disable
 * PURPOSE:
 *      The function is used for gatt to disable multiple advertising
 * INPUT:
 *      client_if              -- registered client app identifier
 * OUTPUT:
 *      None
 * RETURN:
 *      BT_SUCCESS                        -- Operate success.
 *      BT_ERR_STATUS_FAIL                -- Operate fail.
 *      BT_ERR_STATUS_PARM_INVALID        -- paramter is invalid.
 * NOTES:
 *      None
 */
INT32 bluetooth_gattc_multi_adv_disable(INT32 client_if)
{
    return linuxbt_gattc_multi_adv_disable(client_if);
}

INT32 bluetooth_gattc_batchscan_cfg_storage(INT32 client_if, INT32 batch_scan_full_max,
                                                INT32 batch_scan_trunc_max, INT32 batch_scan_notify_threshold)
{
    return linuxbt_gattc_batchscan_cfg_storage(client_if, batch_scan_full_max,
                                               batch_scan_trunc_max, batch_scan_notify_threshold);
}

INT32 bluetooth_gattc_batchscan_enb_batch_scan(INT32 client_if, INT32 scan_mode,
                        INT32 scan_interval, INT32 scan_window, INT32 addr_type, INT32 discard_rule)
{
    return linuxbt_gattc_batchscan_enb_batch_scan(client_if, scan_mode, scan_interval, scan_window, addr_type, discard_rule);
}

INT32 bluetooth_gattc_batchscan_dis_batch_scan(INT32 client_if)
{
    return linuxbt_gattc_batchscan_dis_batch_scan(client_if);
}


INT32 bluetooth_gattc_batchscan_read_reports(INT32 client_if, INT32 scan_mode)
{
    return linuxbt_gattc_batchscan_read_reports(client_if, scan_mode);
}

INT32 bluetooth_gattc_configure_mtu(INT32 conn_id, INT32 mtu)
{
    return linuxbt_gattc_configure_mtu(conn_id, mtu);
}

/**
 * FUNCTION NAME: bluetooth_gatt_parse_adv_data
 * PURPOSE:
 *      The function is used for gatt to parse advertising data.
 * INPUT:
 *      adv_data               -- advertising data
 * OUTPUT:
 *      None
 * RETURN:
 *      None
 * NOTES:
 *      None
 */
VOID bluetooth_gatt_parse_adv_data (UINT8* adv_data)
{
    UINT8* ptr = adv_data;
    CHAR count = 0;

    if (NULL == adv_data)
    {
        BT_DBG_ERROR(BT_DEBUG_GATT,"[GATT] null pointer\n");
        return;
    }
    while (1)
    {
        CHAR length = *ptr;
        if (length == 0)
        {
            break;
        }
        UINT8 type = *(ptr+1);
        UINT8 value_len = length-1;
        UINT8* value = (UINT8*)malloc(value_len+1);
        if (NULL == value)
        {
            return;
        }
        memset(value,0,value_len);
        memcpy(value, ptr+2, value_len);
        value[value_len] = '\0';

        ptr = ptr+length+1;
        switch (type)
        {
            case 0x01: //Flags
                BT_DBG_INFO(BT_DEBUG_GATT, "[GATT] Flags : %02X\n",value[0]);
                break;
            case 0x02: //16-bit uuid
            case 0x03:  //16-bit uuid
            {
                CHAR temp[value_len*2+1];
                INT32 i = 0;
                INT32 j = 0;
                for (i = value_len-1 ; i >= 0 ; i--)
                {
                    snprintf(&temp[j*2],sizeof(temp)-2*j,"%02X",value[i]);
                    j++;
                }
                BT_DBG_INFO(BT_DEBUG_GATT, "[GATT] 16-bit Service Class length: %d UUIDs : %s\n",value_len,temp);
                break;
            }
            case 0x04: //32-bit uuid
            case 0x05:  //32-bit uuid
            {
                CHAR temp[value_len*2+1];
                INT32 i = 0;
                INT32 j = 0;
                for (i = value_len-1 ; i >= 0 ; i--)
                {
                    snprintf(&temp[j*2],sizeof(temp)-2*j,"%02X",value[i]);
                    j++;
                }
                BT_DBG_INFO(BT_DEBUG_GATT, "[GATT] 32-bit Service Class length: %d UUIDs : %s\n",value_len,temp);
                break;
            }
            case 0x06: //128-bit uuid
            case 0x07:  //128-bit uuid
            {
                CHAR temp[value_len*2+1];
                INT32 i = 0;
                INT32 j = 0;
                memset(temp,0,sizeof(temp));
                for (i = value_len-1 ; i >= 0 ; i--)
                {
                    snprintf(&temp[j*2],sizeof(temp)-2*j,"%02X",value[i]);
                    j++;
                }
                BT_DBG_INFO(BT_DEBUG_GATT, "[GATT] 128-bit Service Class length: %d UUIDs : %s\n",value_len,temp);
                if (value_len > sizeof(g_scan_rst_info.uuid))
                {
                    BT_DBG_INFO(BT_DEBUG_GATT, "[GATT] value_len:%d invalid\n",value_len);
                    //memcpy(g_scan_rst_info.uuid, temp, sizeof(g_scan_rst_info.uuid));
                    j = 0;
                    for (i = sizeof(g_scan_rst_info.uuid)-1 ; i >= 0 ; i--)
                    {
                        g_scan_rst_info.uuid[j] = value[i];
                        j++;
                    }
                }
                else
                {
                    //memcpy(g_scan_rst_info.uuid, temp, value_len);
                    j = 0;
                    for (i = value_len-1 ; i >= 0 ; i--)
                    {
                        g_scan_rst_info.uuid[j] = value[i];
                        j++;
                    }
                }
                break;
            }
            case 0x08: //Shortened Local Name
                BT_DBG_INFO(BT_DEBUG_GATT,"[GATT] Shortened Local length: %d Name : %s\n",value_len,value);
                break;
            case 0x09: //Complete Local Name
                BT_DBG_INFO(BT_DEBUG_GATT,"[GATT] Complete Local length: %d Name : %s\n",value_len,value);
                memcpy(g_scan_rst_info.device_name, value, value_len);
                break;
            case 0x0A: //Tx Power Level
                BT_DBG_INFO(BT_DEBUG_GATT,"[GATT] Tx Power Level : %d\n",value[0]);
                break;
            case 0x1B:  //LE Bluetooth Device Address
            {
                BT_DBG_INFO(BT_DEBUG_GATT,"[GATT] LE Bluetooth Device Address : %02X:%02X:%02X:%02X:%02X:%02X\n",
                            value[5], value[4], value[3],
                            value[2], value[1], value[0]);
                break;
            }
            case 0xFF:  //Manufacturer Specific Data
            {
                CHAR temp[value_len*2+1];
                INT32 i = 0;
                INT32 j = 0;
#if ENABLE_HERCULES
                for (i = 0 ; i <= value_len-1 ; i++)
                {
                    snprintf(&temp[j*2],sizeof(temp)-2*j,"%02X",value[i]);
                    j++;
                }
#else
                for (i = value_len-1 ; i >= 0 ; i--)
                {
                    snprintf(&temp[j*2],sizeof(temp)-2*j,"%02X",value[i]);
                    j++;
                }
#endif
                BT_DBG_INFO(BT_DEBUG_GATT,"[GATT] Manufacturer Specific Data : %s\n",temp);
                strncpy(g_scan_rst_info.manu_spec_data, temp, sizeof(g_scan_rst_info.manu_spec_data));
                break;
            }
            default:
            {
                CHAR temp[length*2];
                INT32 i = 0;
                for (i = 0 ; i < length ; i++)
                {
                    snprintf(&temp[i*2],sizeof(temp)-2*i,"%02X",value[i]);
                }
                BT_DBG_INFO(BT_DEBUG_GATT,"[GATT] Type:%02X Length:%d Data:%s\n",type,length,temp);
                break;
            }
        }
        free(value);
        count++;
    }
}


/**
 * FUNCTION NAME: bluetooth_gattc_register_client_cbk
 * PURPOSE:
 *      The function is callback function of gatt client register app
 * INPUT:
 *      status              -- gatt error code status
 *      client_if           -- registered client app identifier
 *      app_uuid            -- registered client app uuid
 * OUTPUT:
 *      None
 * RETURN:
 *      None
 * NOTES:
 *      None
 */
VOID bluetooth_gattc_register_client_cbk(INT32 status, INT32 client_if, CHAR *app_uuid)
{
    //FUNC_ENTRY;
    fg_gattc_register_done = TRUE;
    memset(&g_reg_client_info, 0x0, sizeof(BT_GATTC_REG_CLIENT_T));
    g_reg_client_info.client_if = client_if;
    memcpy(g_reg_client_info.uuid, app_uuid, strlen(app_uuid)+1);
    if (NULL != BtGattRegClientCbk)
    {
        BT_DBG_INFO(BT_DEBUG_GATT,"[GATT] client_if = %d", g_reg_client_info.client_if);
        BtGattRegClientCbk(&g_reg_client_info);
    }
    else
    {
        BT_DBG_ERROR(BT_DEBUG_GATT, "[GATT] null pointer of BtGattRegClientCbk");
    }

#if MTK_BLE_GGL_SETUP_SUPPORT
    extern INT32 reg_flag;
    reg_flag |= 1 << 0;
    BT_DBG_INFO(BT_DEBUG_GATT,"[GATT] reg_flag = %d", reg_flag);
#endif
}

/**
 * FUNCTION NAME: bluetooth_gattc_scan_result_cbk
 * PURPOSE:
 *      The function is callback function of gatt client scan result
 * INPUT:
 *      bda                 -- remote device bt address
 *      rssi                -- received signal strength indicator
 *      adv_data            -- scanned advetising data
 * OUTPUT:
 *      None
 * RETURN:
 *      None
 * NOTES:
 *      None
 */
VOID bluetooth_gattc_scan_result_cbk(CHAR* bda, INT32 rssi, UINT8* adv_data)
{
    memset(&g_scan_rst_info, 0x0, sizeof(BT_GATTC_SCAN_RST_T));
    //linuxbt_btaddr_htos(bda, g_scan_rst_info.btaddr);
    memcpy(g_scan_rst_info.btaddr, bda, strlen(bda)+ 1);
    g_scan_rst_info.rssi = rssi;
    bluetooth_gatt_parse_adv_data(adv_data);
    if (NULL != BtGattScanCbk)
    {
        BtGattScanCbk(&g_scan_rst_info);
    }
    else
    {
        BT_DBG_ERROR(BT_DEBUG_GATT, "[GATT] null pointer of BtGattScanCbk");
    }
}

/**
 * FUNCTION NAME: bluetooth_gattc_connect_cbk
 * PURPOSE:
 *      The function is callback function of gatt client connection with remote device
 * INPUT:
 *      conn_id              -- connection id
 *      status               -- gatt error code status
 *      client_if            -- registered client app identifier
 *      bda                  -- remote device bt address
 * OUTPUT:
 *      None
 * RETURN:
 *      None
 * NOTES:
 *      None
 */
VOID bluetooth_gattc_connect_cbk(INT32 conn_id, INT32 status, INT32 client_if, CHAR* bda)
{
    //FUNC_ENTRY;
    memset(&g_connect_rst_info, 0x0, sizeof(BT_GATTC_CONNECT_RST_T));
    g_connect_rst_info.conn_id = conn_id;
    g_connect_rst_info.client_if = client_if;
    memcpy(g_connect_rst_info.btaddr, bda, strlen(bda) + 1);
    if (NULL != BtGattAppCbk)
    {
        BtGattAppCbk(BT_GATT_CONNECT);
    }
    else
    {
        BT_DBG_ERROR(BT_DEBUG_GATT, "[GATT] null pointer of BtGattAppCbk");
    }
}

/**
 * FUNCTION NAME: bluetooth_gattc_disconnect_cbk
 * PURPOSE:
 *      The function is callback function of gatt client disconnection with remote device
 * INPUT:
 *      conn_id              -- connection id
 *      status               -- gatt error code status
 *      client_if            -- registered client app identifier
 *      bda                  -- remote device bt address
 * OUTPUT:
 *      None
 * RETURN:
 *      None
 * NOTES:
 *      None
 */
VOID bluetooth_gattc_disconnect_cbk(INT32 conn_id, INT32 status, INT32 client_if, CHAR* bda)
{
    //FUNC_ENTRY;
    memset(&g_disconnect_rst_info, 0x0, sizeof(BT_GATTC_CONNECT_RST_T));
    g_disconnect_rst_info.conn_id = conn_id;
    g_disconnect_rst_info.client_if = client_if;
    //linuxbt_btaddr_htos(bda, g_disconnect_rst_info.btaddr);
    memcpy(g_disconnect_rst_info.btaddr, bda, strlen(bda) + 1);
    if (NULL != BtGattAppCbk)
    {
        BtGattAppCbk(BT_GATT_DISCONNECT);
    }
    else
    {
        BT_DBG_ERROR(BT_DEBUG_GATT, "[GATT] null pointer of BtGattAppCbk");
    }
}

/**
 * FUNCTION NAME: bluetooth_gattc_search_complete_cbk
 * PURPOSE:
 *      The function is callback function of gatt client search service complete
 * INPUT:
 *      conn_id              -- connection id
 *      status               -- gatt error code status
 * OUTPUT:
 *      None
 * RETURN:
 *      None
 * NOTES:
 *      None
 */
VOID bluetooth_gattc_search_complete_cbk(INT32 conn_id, INT32 status)
{
}

/**
 * FUNCTION NAME: bluetooth_gattc_search_result_cbk
 * PURPOSE:
 *      The function is callback function of gatt client search service result
 * INPUT:
 *      conn_id              -- connection id
 *      srvc_id              -- gatt client search service information
 * OUTPUT:
 *      None
 * RETURN:
 *      None
 * NOTES:
 *      None
 */
VOID bluetooth_gattc_search_result_cbk(INT32 conn_id, BT_GATTS_SRVC_ID_T *srvc_id)
{
    memset(&g_search_rst_info, 0x0, sizeof(BT_GATTC_SEARCH_RST_T));
    g_search_rst_info.conn_id = conn_id;
    memcpy(&(g_search_rst_info.srvc_id), srvc_id, sizeof(BT_GATTS_SRVC_ID_T));
    if (NULL != BtGattSearchCbk)
    {
        BtGattSearchCbk(&g_search_rst_info);
    }
    else
    {
        BT_DBG_ERROR(BT_DEBUG_GATT, "[GATT] null pointer of BtGattSearchCbk");
    }
}


VOID bluetooth_gattc_get_gatt_db_cbk(INT32 conn_id, bluetooth_gatt_db_element_t *gatt_db, INT32 count)
{
    memset(&g_get_gatt_db_info, 0x0, sizeof(BT_GATTC_GET_GATT_DB_T));
    g_get_gatt_db_info.conn_id = conn_id;
    g_get_gatt_db_info.count = count;
    g_get_gatt_db_info.gatt_db_element = gatt_db; //xuemei:need to verify
    //memcpy(g_get_gatt_db_info.gatt_db_element, gatt_db, sizeof(bluetooth_gatt_db_element_t));
    if (NULL != BtGattGetGattDbCbk)
    {
        BtGattGetGattDbCbk(&g_get_gatt_db_info);
    }
    else
    {
        BT_DBG_ERROR(BT_DEBUG_GATT, "[GATT] null pointer of BtGattSearchCbk");
    }
}


/**
 * FUNCTION NAME: bluetooth_gattc_get_characteristic_cbk
 * PURPOSE:
 *      The function is callback function of gatt client get characteristic
 * INPUT:
 *      conn_id              -- connection id
 *      status               -- gatt error code status
 *      srvc_id              -- service information
 *      char_id              -- characteristic information
 *      char_prop            -- characteristic properties
 * OUTPUT:
 *      None
 * RETURN:
 *      None
 * NOTES:
 *      None
 */
VOID bluetooth_gattc_get_characteristic_cbk(INT32 conn_id, INT32 status, BT_GATTS_SRVC_ID_T *srvc_id,
        BT_GATT_ID_T *char_id, INT32 char_prop)
{
    memset(&g_get_char_rst_info, 0x0, sizeof(BT_GATTC_GET_CHAR_RST_T));
    g_get_char_rst_info.conn_id = conn_id;
    g_get_char_rst_info.char_prop = char_prop;
    memcpy(&g_get_char_rst_info.srvc_id, srvc_id, sizeof(BT_GATTS_SRVC_ID_T));
    memcpy(&g_get_char_rst_info.char_id, char_id, sizeof(BT_GATT_ID_T));
    if (NULL != BtGattGetCharCbk)
    {
        BtGattGetCharCbk(&g_get_char_rst_info);
    }
    else
    {
        BT_DBG_ERROR(BT_DEBUG_GATT, "[GATT] null pointer of BtGattGetCharCbk");
    }
}

/**
 * FUNCTION NAME: bluetooth_gattc_get_descriptor_cbk
 * PURPOSE:
 *      The function is callback function of gatt client get characteristic description
 * INPUT:
 *      conn_id              -- connection id
 *      status               -- gatt error code status
 *      srvc_id              -- service information
 *      char_id              -- characteristic information
 *      descr_id             -- description information
 * OUTPUT:
 *      None
 * RETURN:
 *      None
 * NOTES:
 *      None
 */
VOID bluetooth_gattc_get_descriptor_cbk(INT32 conn_id, INT32 status, BT_GATTS_SRVC_ID_T *srvc_id,
                                        BT_GATT_ID_T *char_id, BT_GATT_ID_T *descr_id)
{
    memset(&g_get_desc_rst_info, 0x0, sizeof(BT_GATTC_GET_DESCR_RST_T));
    g_get_desc_rst_info.conn_id = conn_id;
    memcpy(&g_get_desc_rst_info.srvc_id, srvc_id, sizeof(BT_GATTS_SRVC_ID_T));
    memcpy(&g_get_desc_rst_info.char_id, char_id, sizeof(BT_GATT_ID_T));
    memcpy(&g_get_desc_rst_info.descr_id, descr_id, sizeof(BT_GATT_ID_T));
    if (NULL != BtGattGetDescCbk)
    {
        BtGattGetDescCbk(&g_get_desc_rst_info);
    }
    else
    {
        BT_DBG_ERROR(BT_DEBUG_GATT, "[GATT] null pointer of BtGattGetDescCbk");
    }
}

/**
 * FUNCTION NAME: bluetooth_gattc_get_included_service_cbk
 * PURPOSE:
 *      The function is callback function of gatt client get included service
 * INPUT:
 *      conn_id              -- connection id
 *      status               -- gatt error code status
 *      srvc_id              -- service information
 *      incl_srvc_id         -- included service information
 * OUTPUT:
 *      None
 * RETURN:
 *      None
 * NOTES:
 *      None
 */
VOID bluetooth_gattc_get_included_service_cbk(INT32 conn_id, INT32 status, BT_GATTS_SRVC_ID_T *srvc_id,
        BT_GATTS_SRVC_ID_T *incl_srvc_id)
{
    memset(&g_get_included_rst_info, 0x0, sizeof(BT_GATTC_GET_INCL_RST_T));
    g_get_included_rst_info.conn_id = conn_id;
    memcpy(&g_get_included_rst_info.srvc_id, srvc_id, sizeof(BT_GATTS_SRVC_ID_T));
    memcpy(&g_get_included_rst_info.incl_srvc_id, incl_srvc_id, sizeof(BT_GATTS_SRVC_ID_T));
    if (NULL != BtGattGetIncludedCbk)
    {
        BtGattGetIncludedCbk(&g_get_included_rst_info);
    }
    else
    {
        BT_DBG_ERROR(BT_DEBUG_GATT, "[GATT] null pointer of BtGattGetIncludedCbk");
    }
}

/**
 * FUNCTION NAME: bluetooth_gattc_register_for_notification_cbk
 * PURPOSE:
 *      The function is callback function of gatt client register for notification
 * INPUT:
 *      conn_id              -- connection id
 *      registered           -- registered or not
 *      status               -- gatt error code status
 *      srvc_id              -- service information
 *      char_id              -- characteristic information
 * OUTPUT:
 *      None
 * RETURN:
 *      None
 * NOTES:
 *      None
 */
VOID bluetooth_gattc_register_for_notification_cbk(INT32 conn_id, INT32 registered, INT32 status, UINT16 handle)
{
    FUNC_ENTRY;
    memset(&g_get_reg_noti_rst_info, 0x0, sizeof(BT_GATTC_GET_REG_NOTI_RST_T));
    g_get_reg_noti_rst_info.conn_id = conn_id;
    g_get_reg_noti_rst_info.registered = registered;
    g_get_reg_noti_rst_info.handle = handle;
    if (NULL != BtGattGetRegNotiCbk)
    {
        BtGattGetRegNotiCbk(&g_get_reg_noti_rst_info);
    }
    else
    {
        BT_DBG_ERROR(BT_DEBUG_GATT, "[GATT] null pointer of BtGattGetRegNotiCbk");
    }

}

/**
 * FUNCTION NAME: bluetooth_gattc_notify_cbk
 * PURPOSE:
 *      The function is callback function of gatt client notification
 * INPUT:
 *      conn_id              -- connection id
 *      p_data               -- notification params
 * OUTPUT:
 *      None
 * RETURN:
 *      None
 * NOTES:
 *      None
 */
VOID bluetooth_gattc_notify_cbk(INT32 conn_id, BT_GATT_NOTI_PARAMS_T *p_data)
{
    FUNC_ENTRY;
    memset(&g_get_notify_info, 0x0, sizeof(BT_GATTC_GET_NOTIFY_T));
    g_get_notify_info.conn_id = conn_id;
    memcpy(&g_get_notify_info.notify_data, p_data, sizeof(BT_GATT_NOTI_PARAMS_T));
    if (NULL != BtGattNotifyCbk)
    {
        BtGattNotifyCbk(&g_get_notify_info);
    }
    else
    {
        BT_DBG_ERROR(BT_DEBUG_GATT, "[GATT] null pointer of BtGattNotifyCbk");
    }
}

/**
 * FUNCTION NAME: bluetooth_gattc_read_characteristic_cbk
 * PURPOSE:
 *      The function is callback function of gatt client read char
 * INPUT:
 *      conn_id              -- connection id
 *      status               -- gatt error code status
 *      p_data               -- read params
 * OUTPUT:
 *      None
 * RETURN:
 *      None
 * NOTES:
 *      None
 */
VOID bluetooth_gattc_read_characteristic_cbk(INT32 conn_id, INT32 status, BT_GATT_READ_PARAMS_T *p_data)
{
    memset(&g_get_read_char_rst_info, 0x0, sizeof(BT_GATTC_READ_CHAR_RST_T));
    g_get_read_char_rst_info.conn_id = conn_id;
    memcpy(&g_get_read_char_rst_info.read_data, p_data, sizeof(BT_GATT_READ_PARAMS_T));
    if (NULL != BtGattReadCharCbk)
    {
        BtGattReadCharCbk(&g_get_read_char_rst_info);
    }
    else
    {
        BT_DBG_ERROR(BT_DEBUG_GATT, "[GATT] null pointer of BtGattReadCharCbk");
    }
}

/**
 * FUNCTION NAME: bluetooth_gattc_write_characteristic_cbk
 * PURPOSE:
 *      The function is callback function of gatt client write char
 * INPUT:
 *      conn_id              -- connection id
 *      status               -- gatt error code status
 *      p_data               -- write params
 * OUTPUT:
 *      None
 * RETURN:
 *      None
 * NOTES:
 *      None
 */
VOID bluetooth_gattc_write_characteristic_cbk(INT32 conn_id, INT32 status, UINT16 handle)
{
    memset(&g_get_write_char_rst_info, 0x0, sizeof(BT_GATTC_WRITE_CHAR_RST_T));
    g_get_write_char_rst_info.conn_id = conn_id;
    g_get_write_char_rst_info.status = status;
    g_get_write_char_rst_info.handle = handle;
    if (NULL != BtGattWriteCharCbk)
    {
        BtGattWriteCharCbk(&g_get_write_char_rst_info);
    }
    else
    {
        BT_DBG_ERROR(BT_DEBUG_GATT, "[GATT] null pointer of BtGattWriteCharCbk");
    }
}

/**
 * FUNCTION NAME: bluetooth_gattc_read_descriptor_cbk
 * PURPOSE:
 *      The function is callback function of gatt client read descriptor
 * INPUT:
 *      conn_id              -- connection id
 *      status               -- gatt error code status
 *      p_data               -- read params
 * OUTPUT:
 *      None
 * RETURN:
 *      None
 * NOTES:
 *      None
 */
VOID bluetooth_gattc_read_descriptor_cbk(INT32 conn_id, INT32 status, BT_GATT_READ_PARAMS_T *p_data)
{

    memset(&g_get_read_desc_rst_info, 0x0, sizeof(BT_GATTC_READ_DESCR_RST_T));
    g_get_read_desc_rst_info.conn_id = conn_id;
    memcpy(&g_get_read_desc_rst_info.read_data, p_data, sizeof(BT_GATT_READ_PARAMS_T));
    if (NULL != BtGattReadDescCbk)
    {
        BtGattReadDescCbk(&g_get_read_desc_rst_info);
    }
    else
    {
        BT_DBG_ERROR(BT_DEBUG_GATT, "[GATT] null pointer of BtGattReadDescCbk");
    }
}

/**
 * FUNCTION NAME: bluetooth_gattc_write_descriptor_cbk
 * PURPOSE:
 *      The function is callback function of gatt client write descriptor
 * INPUT:
 *      conn_id              -- connection id
 *      status               -- gatt error code status
 *      p_data               -- write params
 * OUTPUT:
 *      None
 * RETURN:
 *      None
 * NOTES:
 *      None
 */
VOID bluetooth_gattc_write_descriptor_cbk(INT32 conn_id, INT32 status, UINT16 handle)
{
    memset(&g_get_write_desc_rst_info, 0x0, sizeof(BT_GATTC_WRITE_DESCR_RST_T));
    g_get_write_desc_rst_info.conn_id = conn_id;
    g_get_write_desc_rst_info.conn_id = status;
    g_get_write_desc_rst_info.handle = handle;
    if (NULL != BtGattWriteDescCbk)
    {
        BtGattWriteDescCbk(&g_get_write_desc_rst_info);
    }
    else
    {
        BT_DBG_ERROR(BT_DEBUG_GATT, "[GATT] null pointer of BtGattWriteDescCbk");
    }

}

/**
 * FUNCTION NAME: bluetooth_gattc_execute_write_cbk
 * PURPOSE:
 *      The function is callback function of gatt client execute write
 * INPUT:
 *      conn_id              -- connection id
 *      status               -- gatt error code status
 * OUTPUT:
 *      None
 * RETURN:
 *      None
 * NOTES:
 *      None
 */
VOID bluetooth_gattc_execute_write_cbk(INT32 conn_id, INT32 status)
{
}

/**
 * FUNCTION NAME: bluetooth_gattc_read_remote_rssi_cbk
 * PURPOSE:
 *      The function is callback function of gatt client read remote rssi
 * INPUT:
 *      client_if            -- registered client app identifier
 *      bda                  -- remote device bt address
 *      rssi                 -- received signal strength indicator
 *      status               -- gatt error code status
 * OUTPUT:
 *      None
 * RETURN:
 *      None
 * NOTES:
 *      None
 */
VOID bluetooth_gattc_read_remote_rssi_cbk(INT32 client_if, CHAR* bda, INT32 rssi, INT32 status)
{
    //FUNC_ENTRY;
    memset(&g_get_remote_rssi_info, 0x0, sizeof(BT_GATTC_GET_REMOTE_RSSI_T));
    g_get_remote_rssi_info.client_if = client_if;
    g_get_remote_rssi_info.rssi= rssi;
    memcpy(g_get_remote_rssi_info.btaddr, bda, strlen(bda) + 1);
    //linuxbt_btaddr_htos(bda, g_get_remote_rssi_info.btaddr);
    if (NULL != BtGattAppCbk)
    {
        BtGattAppCbk(BT_GATT_GET_RSSI_DONE);
    }
    else
    {
        BT_DBG_ERROR(BT_DEBUG_GATT, "[GATT] null pointer of BtGattAppCbk");
    }
}

void bluetooth_gattc_scan_filter_cfg_cbk(INT32 action, INT32 client_if, INT32 status, INT32 filt_type, INT32 avbl_space)
{
    //FUNC_ENTRY;
    memset(&g_scan_filter_cfg_info, 0x0, sizeof(bluetooth_gatt_scan_filter_cfg_t));
    g_scan_filter_cfg_info.client_if = client_if;
    g_scan_filter_cfg_info.action = action;
    g_scan_filter_cfg_info.filt_type = filt_type;
    g_scan_filter_cfg_info.avbl_space = avbl_space;
    if (NULL != BtGattScanFilterCfgCbk) //need app to implement
    {
        BtGattScanFilterCfgCbk(&g_scan_filter_cfg_info);
    }
    else
    {
        BT_DBG_ERROR(BT_DEBUG_GATT,"[GATT] null pointer of BtGattScanFilterCfgCbk");
    }

}
void bluetooth_gattc_scan_filter_param_cbk(INT32 action, INT32 client_if, INT32 status, INT32 avbl_space)
{
    //FUNC_ENTRY;
    memset(&g_scan_filter_param_info, 0x0, sizeof(bluetooth_gatt_scan_filter_param_t));
    g_scan_filter_param_info.client_if = client_if;
    g_scan_filter_param_info.action = action;
    g_scan_filter_param_info.avbl_space = avbl_space;
    if (NULL != BtGattScanFilterParamCbk) //need app to implement
    {
        BtGattScanFilterParamCbk(&g_scan_filter_param_info);
    }
    else
    {
        BT_DBG_ERROR(BT_DEBUG_GATT,"[GATT] null pointer of BtGattScanFilterCfgCbk");
    }
}
VOID bluetooth_gattc_scan_filter_status_cbk(INT32 enable, INT32 client_if, INT32 status)
{
    //FUNC_ENTRY;
    memset(&g_scan_filter_status_info, 0x0, sizeof(bluetooth_gatt_scan_filter_status_t));
    g_scan_filter_status_info.client_if = client_if;
    g_scan_filter_status_info.enable= enable;
    if (NULL != BtGattScanFilterStatusCbk) //need app to implement
    {
        BtGattScanFilterStatusCbk(&g_scan_filter_status_info);
    }
    else
    {
        BT_DBG_ERROR(BT_DEBUG_GATT,"[GATT] null pointer of BtGattScanFilterCfgCbk");
    }
}

VOID bluetooth_gattc_multi_adv_enable_cbk(INT32 client_if, INT32 status)
{
    //FUNC_ENTRY;
    fg_gattc_adv_enable = TRUE;
}

VOID bluetooth_gattc_multi_adv_data_cbk(INT32 client_if, INT32 status)
{
    fg_gattc_adv_data = TRUE;
}


/*GATT server*/
/**
 * FUNCTION NAME: bluetooth_gatts_register_cbk_fct
 * PURPOSE:
 *      The function is used to gatt server register APP callback function.
 * INPUT:
 *      func               -- gatt server app callback function structure
 * OUTPUT:
 *      None
 * RETURN:
 *      BT_SUCCESS                        -- Operate success.
 *      BT_ERR_STATUS_PARM_INVALID        -- paramter is invalid.
 * NOTES:
 *      None
 */

INT32 bluetooth_gatts_register_cbk_fct(BT_APP_GATTS_CB_FUNC_T *func)
{
    BT_ERR_STATUS_T ret = BT_SUCCESS;

    BT_DBG_NORMAL(BT_DEBUG_GATT, "start bluetooth_gatts_register_cbk_fct");

    if (NULL == func)
    {
        BT_DBG_ERROR(BT_DEBUG_GATT, "callback func is null!");
        return BT_ERR_STATUS_PARM_INVALID;
    }
    /*1*/
    if (func->bt_gatts_event_cb)
    {
        BtGattsAppCbk = func->bt_gatts_event_cb;
    }
    else
    {
        BT_DBG_NOTICE(BT_DEBUG_GATT, "gatts event callback func is null!");
        BtGattsAppCbk = NULL;
        //ret = BT_ERR_STATUS_PARM_INVALID;
    }
    /*2*/
    if (func->bt_gatts_reg_server_cb)
    {
        BtGattsRegSeverCbk = func->bt_gatts_reg_server_cb;
    }
    else
    {
        BT_DBG_NOTICE(BT_DEBUG_GATT, "gatts register server callback func is null!");
        BtGattsRegSeverCbk = NULL;
        //ret = BT_ERR_STATUS_PARM_INVALID;
    }
    /*3*/
    if (func->bt_gatts_add_srvc_cb)
    {
        BtGattsAddSrvcCbk = func->bt_gatts_add_srvc_cb;
    }
    else
    {
        BT_DBG_NOTICE(BT_DEBUG_GATT, "gatts add service callback func is null!");
        BtGattsAddSrvcCbk = NULL;
        //ret = BT_ERR_STATUS_PARM_INVALID;
    }
    /*4*/
    if (func->bt_gatts_add_incl_cb)
    {
        BtGattsAddInclCbk = func->bt_gatts_add_incl_cb;
    }
    else
    {
        BT_DBG_NOTICE(BT_DEBUG_GATT, "gatts add include callback func is null!");
        BtGattsAddInclCbk = NULL;
        //ret = BT_ERR_STATUS_PARM_INVALID;
    }
    /*5*/
    if (func->bt_gatts_add_char_cb)
    {
        BtGattsAddCharCbk = func->bt_gatts_add_char_cb;
    }
    else
    {
        BT_DBG_NOTICE(BT_DEBUG_GATT, "gatts add characteristic callback func is null!");
        BtGattsAddCharCbk = NULL;
        //ret = BT_ERR_STATUS_PARM_INVALID;
    }
    /*6*/
    if (func->bt_gatts_add_desc_cb)
    {
        BtGattsAddDescCbk = func->bt_gatts_add_desc_cb;
    }
    else
    {
        BT_DBG_NOTICE(BT_DEBUG_GATT, "gatts add descripor callback func is null!");
        BtGattsAddDescCbk = NULL;
        //ret = BT_ERR_STATUS_PARM_INVALID;
    }
    /*7*/
    if (func->bt_gatts_op_srvc_cb)
    {
        BtGattsOpSrvcCbk = func->bt_gatts_op_srvc_cb;
    }
    else
    {
        BT_DBG_NOTICE(BT_DEBUG_GATT, "gatts operate service callback func is null!");
        BtGattsOpSrvcCbk = NULL;
        //ret = BT_ERR_STATUS_PARM_INVALID;
    }
    /*8*/
    if (func->bt_gatts_req_read_cb)
    {
        BtGattsReqReadCbk = func->bt_gatts_req_read_cb;
    }
    else
    {
        BT_DBG_NOTICE(BT_DEBUG_GATT, "gatts request read callback func is null!");
        BtGattsReqReadCbk = NULL;
        //ret = BT_ERR_STATUS_PARM_INVALID;
    }
    /*9*/
    if (func->bt_gatts_req_write_cb)
    {
        BtGattsReqWriteCbk = func->bt_gatts_req_write_cb;
    }
    else
    {
        BT_DBG_NOTICE(BT_DEBUG_GATT, "gatts request read callback func is null!");
        BtGattsReqWriteCbk = NULL;
        //ret = BT_ERR_STATUS_PARM_INVALID;
    }
    /*10*/
    if (func->bt_gatts_ind_sent_cb)
    {
        BtGattsIndSentCbk = func->bt_gatts_ind_sent_cb;
    }
    else
    {
        BT_DBG_NOTICE(BT_DEBUG_GATT, "gatts indication sent callback func is null!");
        BtGattsIndSentCbk = NULL;
        //ret = BT_ERR_STATUS_PARM_INVALID;
    }

    return ret;
}

/**
 * FUNCTION NAME: bluetooth_gatts_base_init
 * PURPOSE:
 *      The function is used to gatt server register APP callback function.
 * INPUT:
 *      func               -- gatt server app callback function structure
 * OUTPUT:
 *      None
 * RETURN:
 *      BT_SUCCESS                    -- Operate success.
 *      BT_ERR_STATUS_PARM_INVALID    -- paramter invalid.
 * NOTES:
 *      None
 */
INT32 bluetooth_gatts_base_init(BT_APP_GATTS_CB_FUNC_T *func)
{
    //FUNC_ENTRY;
    return  bluetooth_gatts_register_cbk_fct(func);
} /*bluetooth_gatts_init*/

/**
 * FUNCTION NAME: bluetooth_gatts_register_app_sync
 * PURPOSE:
 *      The function is used to synchronous register gatt server APP.
 * INPUT:
 *      pt_uuid            -- app uuid
 * OUTPUT:
 *      server_if          -- registered app identifer
 * RETURN:
 *      BT_SUCCESS                        -- Operate success.
 *      BT_ERR_STATUS_FAIL                -- Operate fail.
 *      BT_ERR_STATUS_PARM_INVALID        -- paramter is invalid.
 * NOTES:
 *      None
 */
INT32 bluetooth_gatts_register_app_sync(CHAR *pt_uuid, INT32 *server_if)
{
    //FUNC_ENTRY;
    INT32 i4_loop = 300;
    BT_ERR_STATUS_T ret = BT_SUCCESS;
    fg_gatts_register_done = FALSE;
    ret = linuxbt_gatts_register_server(pt_uuid);
    while (0<i4_loop && !fg_gatts_register_done)
    {
        BT_DBG_INFO(BT_DEBUG_GATT, "Wait gatts register server finish:%ld", (long)i4_loop);
        /*x_thread_delay(10);*/
        usleep(10*1000);
        i4_loop--;
    }
    if (fg_gatts_register_done)
    {
        *server_if = gatts_reg_server_info.server_if;
    }
    else
    {
        BT_DBG_ERROR(BT_DEBUG_GATT, "gatts register server failed");
        *server_if = -1;
        return BT_ERR_STATUS_FAIL;
    }
    return ret;
}

/**
 * FUNCTION NAME: bluetooth_gatts_register_server
 * PURPOSE:
 *      The function is used to register gatt server APP.
 * INPUT:
 *      pt_uuid            -- app uuid
 * OUTPUT:
 *      None
 * RETURN:
 *      BT_SUCCESS                        -- Operate success.
 *      BT_ERR_STATUS_FAIL                -- Operate fail.
 *      BT_ERR_STATUS_PARM_INVALID        -- paramter is invalid.
 * NOTES:
 *      None
 */
INT32 bluetooth_gatts_register_server(CHAR *pt_uuid)
{
    return linuxbt_gatts_register_server(pt_uuid);
}

/**
 * FUNCTION NAME: bluetooth_gatts_unregister_server
 * PURPOSE:
 *      The function is used to deregister gatt server APP.
 * INPUT:
 *      server_if              -- registered app identifer
 * OUTPUT:
 *      None
 * RETURN:
 *      BT_SUCCESS                        -- Operate success.
 *      BT_ERR_STATUS_FAIL                -- Operate fail.
 *      BT_ERR_STATUS_PARM_INVALID        -- paramter is invalid.
 * NOTES:
 *      None
 */
INT32 bluetooth_gatts_unregister_server(INT32 server_if)
{
    return linuxbt_gatts_unregister_server(server_if);
}

/**
 * FUNCTION NAME: bluetooth_gatts_open
 * PURPOSE:
 *      The function is used for gatt server connect with remote device.
 * INPUT:
 *      server_if              -- registered app identifer
 *      bt_addr                -- remote device bt address
 *      is_direct              -- is direct connection or background connection
 *      transport              -- transport type:(0 : auto, 1 : BREDR, 2 : LE)
 * OUTPUT:
 *      None
 * RETURN:
 *      BT_SUCCESS                        -- Operate success.
 *      BT_ERR_STATUS_FAIL                -- Operate fail.
 *      BT_ERR_STATUS_PARM_INVALID        -- paramter is invalid.
 * NOTES:
 *      None
 */
INT32 bluetooth_gatts_open(INT32 server_if, CHAR *bt_addr, UINT8 is_direct, INT32 transport)
{
    return linuxbt_gatts_open(server_if, bt_addr, is_direct, transport);
}

/**
 * FUNCTION NAME: bluetooth_gatts_close
 * PURPOSE:
 *      The function is used for gatt server disconnect with remote device.
 * INPUT:
 *      server_if              -- registered app identifer
 *      bt_addr                -- remote device bt address
 *      conn_id                -- connection id
 * OUTPUT:
 *      None
 * RETURN:
 *      BT_SUCCESS                        -- Operate success.
 *      BT_ERR_STATUS_FAIL                -- Operate fail.
 *      BT_ERR_STATUS_PARM_INVALID        -- paramter is invalid.
 * NOTES:
 *      None
 */
INT32 bluetooth_gatts_close(INT32 server_if, CHAR *bt_addr, INT32 conn_id)
{
    return linuxbt_gatts_close(server_if, bt_addr, conn_id);
}

/**
 * FUNCTION NAME: bluetooth_gatts_add_service_sync
 * PURPOSE:
 *      The function is used for gatt server synchronous add service to database.
 * INPUT:
 *      server_if              -- registered app identifer
 *      pt_service_uuid        -- service uuid
 *      is_primary             -- is primary service or not
 *      number                 -- handle number
 * OUTPUT:
 *      srvc_handle            -- added service start handle
 * RETURN:
 *      BT_SUCCESS                        -- Operate success.
 *      BT_ERR_STATUS_FAIL                -- Operate fail.
 *      BT_ERR_STATUS_PARM_INVALID        -- paramter is invalid.
 * NOTES:
 *      None
 */
INT32 bluetooth_gatts_add_service_sync(INT32 server_if, CHAR *pt_service_uuid,
                                       UINT8 is_primary,
                                       INT32 number, INT32 *srvc_handle)
{
    //FUNC_ENTRY;
    INT32 i4_loop = 300;
    BT_ERR_STATUS_T ret = BT_SUCCESS;
    fg_gatts_add_service= FALSE;
    ret = linuxbt_gatts_add_service(server_if,pt_service_uuid,is_primary,number);
    while (0<i4_loop && !fg_gatts_add_service)
    {
        BT_DBG_INFO(BT_DEBUG_GATT, "Wait gatts add service finish:%ld", (long)i4_loop);
        /*x_thread_delay(10);*/
        usleep(10*1000);
        i4_loop--;
    }
    if (fg_gatts_add_service)
    {
        *srvc_handle = gatts_add_srvc_info.srvc_handle;
    }
    else
    {
        BT_DBG_ERROR(BT_DEBUG_GATT, "gatts register server failed");
        *srvc_handle = -1;
        return BT_ERR_STATUS_FAIL;
    }
    return ret;
}

/**
 * FUNCTION NAME: bluetooth_gatts_add_service
 * PURPOSE:
 *      The function is used for gatt server add service to database.
 * INPUT:
 *      server_if              -- registered app identifer
 *      pt_service_uuid        -- service uuid
 *      is_primary             -- is primary service or not
 *      number                 -- handle number
 * OUTPUT:
 *      None
 * RETURN:
 *      BT_SUCCESS                        -- Operate success.
 *      BT_ERR_STATUS_FAIL                -- Operate fail.
 *      BT_ERR_STATUS_PARM_INVALID        -- paramter is invalid.
 * NOTES:
 *      None
 */
INT32 bluetooth_gatts_add_service(INT32 server_if, CHAR *pt_service_uuid,
                                  UINT8 is_primary,
                                  INT32 number)
{
    return linuxbt_gatts_add_service(server_if,
                                     pt_service_uuid,
                                     is_primary,
                                     number);
}

/**
 * FUNCTION NAME: bluetooth_gatts_add_included_service
 * PURPOSE:
 *      The function is used for gatt server add included service to database.
 * INPUT:
 *      server_if              -- registered app identifer
 *      service_handle         -- added service start handle
 *      included_handle        -- include service handle
 * OUTPUT:
 *      None
 * RETURN:
 *      BT_SUCCESS                        -- Operate success.
 *      BT_ERR_STATUS_FAIL                -- Operate fail.
 *      BT_ERR_STATUS_PARM_INVALID        -- paramter is invalid.
 * NOTES:
 *      None
 */
INT32 bluetooth_gatts_add_included_service(INT32 server_if, INT32 service_handle, INT32 included_handle)
{
    return linuxbt_gatts_add_included_service(server_if, service_handle, included_handle);
}

/**
 * FUNCTION NAME: bluetooth_gatts_add_char_sync
 * PURPOSE:
 *      The function is used for gatt server synchronous add characteristic to database.
 * INPUT:
 *      server_if              -- registered app identifer
 *      service_handle         -- added service start handle
 *      uuid                   -- characteristic uuid
 *      properties             -- characteristic properties
 *      permissions            -- access characteristic permissions
 * OUTPUT:
 *      char_handle            -- added characteristic handle
 * RETURN:
 *      BT_SUCCESS                        -- Operate success.
 *      BT_ERR_STATUS_FAIL                -- Operate fail.
 *      BT_ERR_STATUS_PARM_INVALID        -- paramter is invalid.
 * NOTES:
 *      None
 */
INT32 bluetooth_gatts_add_char_sync(INT32 server_if,
                                    INT32 service_handle,
                                    CHAR *uuid,
                                    INT32 properties,
                                    INT32 permissions, INT32 *char_handle)
{
    //FUNC_ENTRY;
    INT32 i4_loop = 300;
    BT_ERR_STATUS_T ret = BT_SUCCESS;
    fg_gatts_add_char = FALSE;
    ret = linuxbt_gatts_add_char(server_if, service_handle, uuid, properties, permissions);
    while (0<i4_loop && !fg_gatts_add_char)
    {
        BT_DBG_INFO(BT_DEBUG_GATT, "Wait gatts add char finish:%ld", (long)i4_loop);
        /*x_thread_delay(10);*/
        usleep(10*1000);
        i4_loop--;
    }
    if (fg_gatts_add_char)
    {
        *char_handle = gatts_add_char_info.char_handle;
    }
    else
    {
        BT_DBG_ERROR(BT_DEBUG_GATT, "gatts register server failed");
        *char_handle = -1;
        return BT_ERR_STATUS_FAIL;
    }
    return ret;
}

/**
 * FUNCTION NAME: bluetooth_gatts_add_char
 * PURPOSE:
 *      The function is used for gatt server add characteristic to database.
 * INPUT:
 *      server_if              -- registered app identifer
 *      service_handle         -- added service start handle
 *      uuid                   -- characteristic uuid
 *      properties             -- characteristic properties
 *      permissions            -- access characteristic permissions
 * OUTPUT:
 *      None
 * RETURN:
 *      BT_SUCCESS                        -- Operate success.
 *      BT_ERR_STATUS_FAIL                -- Operate fail.
 *      BT_ERR_STATUS_PARM_INVALID        -- paramter is invalid.
 * NOTES:
 *      None
 */
INT32 bluetooth_gatts_add_char(INT32 server_if,
                               INT32 service_handle,
                               CHAR *uuid,
                               INT32 properties,
                               INT32 permissions)
{
    return linuxbt_gatts_add_char(server_if, service_handle,
                                  uuid,
                                  properties,
                                  permissions);
}

/**
 * FUNCTION NAME: bluetooth_gatts_add_desc
 * PURPOSE:
 *      The function is used for gatt server add char description to database.
 * INPUT:
 *      server_if              -- registered app identifer
 *      service_handle         -- added service start handle
 *      uuid                   -- char description uuid
 *      permissions            -- access char description permissions
 * OUTPUT:
 *      None
 * RETURN:
 *      BT_SUCCESS                        -- Operate success.
 *      BT_ERR_STATUS_FAIL                -- Operate fail.
 *      BT_ERR_STATUS_PARM_INVALID        -- paramter is invalid.
 * NOTES:
 *      None
 */
INT32 bluetooth_gatts_add_desc(INT32 server_if, INT32 service_handle,
                               CHAR *uuid,
                               INT32 permissions)
{
    return linuxbt_gatts_add_desc(server_if,
                                  service_handle,
                                  uuid,
                                  permissions);
}

/**
 * FUNCTION NAME: bluetooth_gatts_start_service
 * PURPOSE:
 *      The function is used for gatt server start service.
 * INPUT:
 *      server_if              -- registered app identifer
 *      service_handle         -- added service start handle
 *      transport              -- transport type:(0 : auto, 1 : BREDR, 2 : LE)
 * OUTPUT:
 *      None
 * RETURN:
 *      BT_SUCCESS                        -- Operate success.
 *      BT_ERR_STATUS_FAIL                -- Operate fail.
 *      BT_ERR_STATUS_PARM_INVALID        -- paramter is invalid.
 * NOTES:
 *      None
 */
INT32 bluetooth_gatts_start_service(INT32 server_if, INT32 service_handle, INT32 transport)
{
    return linuxbt_gatts_start_service(server_if, service_handle, transport);
}

/**
 * FUNCTION NAME: bluetooth_gatts_start_service_sync
 * PURPOSE:
 *      The function is used for gatt server start service.
 * INPUT:
 *      server_if              -- registered app identifer
 *      service_handle         -- added service start handle
 *      transport              -- transport type:(0 : auto, 1 : BREDR, 2 : LE)
 * OUTPUT:
 *      None
 * RETURN:
 *      BT_SUCCESS                        -- Operate success.
 *      BT_ERR_STATUS_FAIL                -- Operate fail.
 *      BT_ERR_STATUS_PARM_INVALID        -- paramter is invalid.
 * NOTES:
 *      None
 */
INT32 bluetooth_gatts_start_service_sync(INT32 server_if, INT32 service_handle, INT32 transport)
{
    INT32 i4_loop = 300;
    BT_ERR_STATUS_T ret = BT_SUCCESS;
    fg_gatts_start_service = FALSE;
    ret = linuxbt_gatts_start_service(server_if, service_handle, transport);
    while (0<i4_loop && !fg_gatts_start_service)
    {
        BT_DBG_INFO(BT_DEBUG_GATT, "Wait gatts start Service finish:%ld", (long)i4_loop);
        /*x_thread_delay(10);*/
        usleep(10*1000);
        i4_loop--;
    }
    if (fg_gatts_start_service)
    {
        return ret;
    }
    else
    {
        BT_DBG_ERROR(BT_DEBUG_GATT, "gatts start service failed");
        return BT_ERR_STATUS_FAIL;
    }
}

/**
 * FUNCTION NAME: bluetooth_gatts_delete_service
 * PURPOSE:
 *      The function is used for gatt server delete service.
 * INPUT:
 *      server_if              -- registered app identifer
 *      service_handle         -- added service start handle
 * OUTPUT:
 *      None
 * RETURN:
 *      BT_SUCCESS                        -- Operate success.
 *      BT_ERR_STATUS_FAIL                -- Operate fail.
 *      BT_ERR_STATUS_PARM_INVALID        -- paramter is invalid.
 * NOTES:
 *      None
 */

INT32 bluetooth_gatts_delete_service(INT32 server_if, INT32 service_handle)
{
    return linuxbt_gatts_delete_service(server_if, service_handle);
}

/**
 * FUNCTION NAME: bluetooth_gatts_stop_service
 * PURPOSE:
 *      The function is used for gatt server stop service.
 * INPUT:
 *      server_if              -- registered app identifer
 *      service_handle         -- added service start handle
 * OUTPUT:
 *      None
 * RETURN:
 *      BT_SUCCESS                        -- Operate success.
 *      BT_ERR_STATUS_FAIL                -- Operate fail.
 *      BT_ERR_STATUS_PARM_INVALID        -- paramter is invalid.
 * NOTES:
 *      None
 */
INT32 bluetooth_gatts_stop_service(INT32 server_if, INT32 service_handle)
{
    return linuxbt_gatts_stop_service(server_if, service_handle);
}

/**
 * FUNCTION NAME: bluetooth_gatts_send_indication
 * PURPOSE:
 *      The function is used for gatt server send indication to remote device.
 * INPUT:
 *      server_if              -- registered app identifer
 *      attribute_handle       -- send indication attribute handle
 *      conn_id                -- connection id
 *      fg_confirm             -- is need confirmation or not
 *      p_value                -- send indication value
 *      value_len              -- send indication value length
 * OUTPUT:
 *      None
 * RETURN:
 *      BT_SUCCESS                        -- Operate success.
 *      BT_ERR_STATUS_FAIL                -- Operate fail.
 *      BT_ERR_STATUS_PARM_INVALID        -- paramter is invalid.
 * NOTES:
 *      None
 */
INT32 bluetooth_gatts_send_indication(INT32 server_if,
                                      INT32 attribute_handle,
                                      INT32 conn_id,
                                      INT32 fg_confirm,
                                      CHAR* p_value,
                                      INT32 value_len)
{
    return linuxbt_gatts_send_indication(server_if,
                                         attribute_handle,
                                         conn_id,
                                         fg_confirm,
                                         p_value,
                                         value_len);
}

/**
 * FUNCTION NAME: bluetooth_gatts_send_response
 * PURPOSE:
 *      The function is used for gatt server send response to remote device.
 * INPUT:
 *      conn_id                -- connection id
 *      trans_id               -- transaction id
 *      status                 -- send response status
 *      handle                 -- send response handle
 *      p_value                -- send response value
 *      value_len              -- send response value length
 *      auth_req               -- authentication request
 * OUTPUT:
 *      None
 * RETURN:
 *      BT_SUCCESS                        -- Operate success.
 *      BT_ERR_STATUS_FAIL                -- Operate fail.
 *      BT_ERR_STATUS_PARM_INVALID        -- paramter is invalid.
 * NOTES:
 *      None
 */
INT32 bluetooth_gatts_send_response(INT32 conn_id,
                                    INT32 trans_id,
                                    INT32 status,
                                    INT32 handle,
                                    CHAR *p_value,
                                    INT32 value_len,
                                    INT32 auth_req)
{
    return linuxbt_gatts_send_response(conn_id,
                                       trans_id,
                                       status,
                                       handle,
                                       p_value,
                                       value_len,
                                       auth_req);
}

/**
 * FUNCTION NAME: bluetooth_gatts_get_connect_result_info
 * PURPOSE:
 *      The function is used for gatt server to get connection result information.
 * INPUT:
 *      None
 * OUTPUT:
 *      connect_rst_info       -- connection result information
 * RETURN:
 *      None
 * NOTES:
 *      None
 */
VOID bluetooth_gatts_get_connect_result_info(BT_GATTS_CONNECT_RST_T *connect_rst_info)
{
    //FUNC_ENTRY;
    if (NULL == connect_rst_info)
    {
        BT_DBG_ERROR(BT_DEBUG_GATT, "[GATT] null pointer");
        return;
    }
    memcpy(connect_rst_info, &gatts_connect_rst_info, sizeof(BT_GATTS_CONNECT_RST_T));
}

/**
 * FUNCTION NAME: bluetooth_gatts_get_disconnect_result_info
 * PURPOSE:
 *      The function is used for gatt server to get disconnection result information.
 * INPUT:
 *      None
 * OUTPUT:
 *      disconnect_rst_info     -- disconnection result information
 * RETURN:
 *      None
 * NOTES:
 *      None
 */
VOID bluetooth_gatts_get_disconnect_result_info(BT_GATTS_CONNECT_RST_T *disconnect_rst_info)
{
    //FUNC_ENTRY;
    if (NULL == disconnect_rst_info)
    {
        BT_DBG_ERROR(BT_DEBUG_GATT, "[GATT] null pointer");
        return;
    }
    memcpy(disconnect_rst_info, &gatts_disconnect_rst_info, sizeof(BT_GATTS_CONNECT_RST_T));
}

INT32 bluetooth_gattc_scan_filter_param_setup(bluetooth_gatt_filt_param_setup_t scan_filt_param)
{
    //FUNC_ENTRY;
    return linuxbt_gattc_scan_filter_param_setup(scan_filt_param);
}
INT32 bluetooth_gattc_scan_filter_enable(INT32 client_if)
{
    //FUNC_ENTRY;
    return linuxbt_gattc_scan_filter_enable(client_if);
}
INT32 bluetooth_gattc_scan_filter_disable(INT32 client_if)
{
    //FUNC_ENTRY;
    return linuxbt_gattc_scan_filter_disable(client_if);
}
INT32 bluetooth_gattc_scan_filter_add_remove(INT32 client_if, INT32 action,
        INT32 filt_type, INT32 filt_index, INT32 company_id,
        INT32 company_id_mask, const CHAR *p_uuid,
        const CHAR *p_uuid_mask, const CHAR *bd_addr,
        CHAR addr_type, INT32 data_len, CHAR* p_data, INT32 mask_len,
        CHAR* p_mask)
{
    //FUNC_ENTRY;
    return linuxbt_gattc_scan_filter_add_remove(client_if,action,filt_type,
            filt_index,company_id,company_id_mask,p_uuid,
            p_uuid_mask,bd_addr,addr_type,data_len,p_data,mask_len,p_mask);
}
INT32 bluetooth_gattc_scan_filter_clear(INT32 client_if, INT32 filt_index)
{
    //FUNC_ENTRY;
    return linuxbt_gattc_scan_filter_clear(client_if,filt_index);
}

INT32 bluetooth_gattc_get_device_type(CHAR *bt_addr)
{
    return linuxbt_gattc_get_device_type(bt_addr);
}

#if MTK_LINUX_GATTC_LE_NAME //== TRUE
INT32 bluetooth_gattc_set_local_le_name(INT32 client_if, CHAR *name)
{
    //BT_DBG_NORMAL(BT_DEBUG_GATT, "<<< call bt_setlocal_lename >>>");
    return linuxbt_gattc_set_local_le_name(client_if, name);
}
#endif

#if MTK_LINUX_GATTC_RPA
INT32 bluetooth_gattc_get_local_adv_rpa(INT32 client_if)
{
    BT_DBG_NORMAL(BT_DEBUG_GATT, "<<< call bt_getlocal_adv_rpa >>>");
    return linuxbt_gattc_get_local_adv_rpa(client_if);
}

#endif

#if MTK_LINUX_GATTC_DISC_MODE
INT32 bluetooth_gattc_set_local_disc_mode(INT32 client_if, INT32 disc_mode)
{
    //BT_DBG_NORMAL(BT_DEBUG_GATT, "<<< call bluetooth_gattc_set_local_disc_mode >>>");
    return linuxbt_gattc_set_local_disc_mode(client_if, disc_mode);
}
#endif

#if MTK_LINUX_GATTC_PTS_TEST
INT32 bluetooth_gattc_read_using_char_uuid(INT32 conn_id,
                                                 INT32 start_handle,
                                                 INT32 end_handle,
                                                 CHAR *uuid,
                                                 INT32 auth_req)
{
    BT_DBG_NORMAL(BT_DEBUG_GATT, "<<< call bt_read_using_char_uuid >>>");
    return linuxbt_gattc_read_using_char_uuid(conn_id, start_handle, end_handle, uuid, auth_req);
}

INT32 bluetooth_gattc_read_long_characteristic(INT32 conn_id,
                                                 INT32 handle,
                                                 INT32 offset,
                                                 INT32 auth_req)
{
    BT_DBG_NORMAL(BT_DEBUG_GATT, "<<< call bt_read_long_characteristic >>>");
    return linuxbt_gattc_read_long_characteristic(conn_id, handle, offset, auth_req);
}

INT32 bluetooth_gattc_read_multi_characteristic(INT32 conn_id,
                                                 INT32 num_attr,
                                                 UINT16 *handles,
                                                 INT32 auth_req)
{
    BT_DBG_NORMAL(BT_DEBUG_GATT, "<<< call bt_read_multi_characteristic >>>");
    return linuxbt_gattc_read_multi_characteristic(conn_id, num_attr, handles, auth_req);
}

INT32 bluetooth_gattc_read_long_descr(INT32 conn_id,
                                                 INT32 handle,
                                                 INT32 offset,
                                                 INT32 auth_req)
{
    BT_DBG_NORMAL(BT_DEBUG_GATT, "<<< call bt_read_long_characteristic >>>");
    return linuxbt_gattc_read_long_descr(conn_id, handle, offset, auth_req);
}

INT32 bluetooth_gattc_write_long_characteristic(INT32 conn_id,
                                                         INT32 char_handle,
                                                         INT32 write_type,
                                                         INT32 len,
                                                         INT32 offset,
                                                         INT32 auth_req,
                                                         CHAR *value)
{
    BT_DBG_NORMAL(BT_DEBUG_GATT, "<<< call bt_write_long_characteristic >>>");
    return linuxbt_gattc_write_long_characteristic(conn_id, char_handle, write_type,
        len, offset, auth_req, value);
}

INT32 bluetooth_gattc_write_long_descr(INT32 conn_id,
                                                     INT32 descr_handle,
                                                     INT32 write_type,
                                                     INT32 len,
                                                     INT32 offset,
                                                     INT32 auth_req,
                                                     CHAR *value)
{
    BT_DBG_NORMAL(BT_DEBUG_GATT, "<<< call bt_write_long_char_descr >>>");
    return linuxbt_gattc_write_long_descr(conn_id, descr_handle, write_type,
        len, offset, auth_req, value);
}

INT32 bluetooth_gattc_set_pts_flag(UINT8 pts_flag)
{
    BT_DBG_NORMAL(BT_DEBUG_GATT, "<<< call bt_set_pts_flag >>>");
    return linuxbt_gattc_set_pts_flag(pts_flag);
}


#endif

INT32 bluetooth_gattc_test_command(INT32 command, BT_GATTC_TEST_PARAMS_T test_params)
{
    BT_DBG_NORMAL(BT_DEBUG_GATT, "<<< call gattc test command >>>");
    return linuxbt_gattc_test_command(command, test_params);
}

/**
 * FUNCTION NAME: bluetooth_gatts_register_server_callback
 * PURPOSE:
 *      The function is callback function of gatt server register app
 * INPUT:
 *      status              -- gatt error code status
 *      server_if           -- registered server app identifier
 *      app_uuid            -- registered server app uuid
 * OUTPUT:
 *      None
 * RETURN:
 *      None
 * NOTES:
 *      None
 */
VOID bluetooth_gatts_register_server_callback(INT32 status, INT32 server_if, CHAR *app_uuid)
{
    //FUNC_ENTRY;
    fg_gatts_register_done = TRUE;
    gatts_reg_server_info.server_if = server_if;
    memcpy(gatts_reg_server_info.app_uuid, app_uuid, strlen(app_uuid) + 1);
    if (NULL != BtGattsRegSeverCbk)
    {
        BtGattsRegSeverCbk(&gatts_reg_server_info);
    }
    else
    {
        BT_DBG_ERROR(BT_DEBUG_GATT, "[GATT] null pointer of BtGattGetCharCbk");
    }
#if MTK_BLE_GGL_SETUP_SUPPORT
    extern INT32 reg_flag;
    reg_flag |= 1 << 1;
    BT_DBG_INFO(BT_DEBUG_GATT,"[GATT] reg_flag = %d", reg_flag);
#endif

}

/**
 * FUNCTION NAME: bluetooth_gatts_service_added_callback
 * PURPOSE:
 *      The function is callback function of gatt server add service
 * INPUT:
 *      status              -- gatt error code status
 *      server_if           -- registered server app identifier
 *      srvc_id              -- service information
 *      srvc_handle          -- added service handle
 * OUTPUT:
 *      None
 * RETURN:
 *      None
 * NOTES:
 *      None
 */
VOID bluetooth_gatts_service_added_callback(INT32 status, INT32 server_if,
        BT_GATTS_SRVC_ID_T *srvc_id, INT32 srvc_handle)
{
    fg_gatts_add_service = TRUE;
    gatts_add_srvc_info.server_if = server_if;
    gatts_add_srvc_info.srvc_handle = srvc_handle;
    memcpy(&gatts_add_srvc_info.srvc_id, srvc_id, sizeof(BT_GATTS_SRVC_ID_T));
    if (NULL != BtGattsAddSrvcCbk)
    {
        BtGattsAddSrvcCbk(&gatts_add_srvc_info);
    }
    else
    {
        BT_DBG_ERROR(BT_DEBUG_GATT, "[GATT] null pointer of BtGattsAddSrvcCbk");
    }
}

/**
 * FUNCTION NAME: bluetooth_gatts_connection_callback
 * PURPOSE:
 *      The function is callback function of gatt server connection with remote device
 * INPUT:
 *      conn_id              -- connection id
 *      server_if            -- registered server app identifier
 *      connected            -- connected or not
 *      bda                  -- remote device bt address
 * OUTPUT:
 *      None
 * RETURN:
 *      None
 * NOTES:
 *      None
 */
VOID bluetooth_gatts_connection_callback(INT32 conn_id, INT32 server_if,
        INT32 connected, CHAR *bda)
{
    //FUNC_ENTRY;
    if (0 == connected)
    {
        gatts_disconnect_rst_info.conn_id = conn_id;
        gatts_disconnect_rst_info.server_if = server_if;
        memcpy(g_disconnect_rst_info.btaddr, bda, strlen(bda) + 1);
        //linuxbt_btaddr_htos(bda, g_disconnect_rst_info.btaddr);
        if (NULL != BtGattsAppCbk)
        {
            BtGattsAppCbk(BT_GATTS_DISCONNECT);
        }
        else
        {
            BT_DBG_ERROR(BT_DEBUG_GATT, "[GATT] null pointer of BtGattGetCharCbk");
        }
    }
    else
    {
        gatts_connect_rst_info.conn_id = conn_id;
        gatts_connect_rst_info.server_if = server_if;
        memcpy(g_disconnect_rst_info.btaddr, bda, strlen(bda) + 1);
        //linuxbt_btaddr_htos(bda, gatts_connect_rst_info.btaddr);
        if (NULL != BtGattsAppCbk)
        {
            BtGattsAppCbk(BT_GATTS_CONNECT);
        }
        else
        {
            BT_DBG_ERROR(BT_DEBUG_GATT, "[GATT] null pointer of BtGattGetCharCbk");
        }
    }
}

/**
 * FUNCTION NAME: bluetooth_gatts_included_service_added_callback
 * PURPOSE:
 *      The function is callback function of gatt server add included service
 * INPUT:
 *      status              -- gatt error code status
 *      server_if           -- registered server app identifier
 *      srvc_handle         -- added service handle
 *      incl_srvc_handle    -- added included service handle
 * OUTPUT:
 *      None
 * RETURN:
 *      None
 * NOTES:
 *      None
 */
VOID bluetooth_gatts_included_service_added_callback(INT32 status,
        INT32 server_if,
        INT32 srvc_handle,
        INT32 incl_srvc_handle)
{
    gatts_add_incl_info.server_if = server_if;
    gatts_add_incl_info.srvc_handle = srvc_handle;
    gatts_add_incl_info.incl_srvc_handle = incl_srvc_handle;
    if (NULL != BtGattsAddInclCbk)
    {
        BtGattsAddInclCbk(&gatts_add_incl_info);
    }
    else
    {
        BT_DBG_ERROR(BT_DEBUG_GATT, "[GATT] null pointer of BtGattsAddInclCbk");
    }
}

/**
 * FUNCTION NAME: bluetooth_gatts_characteristic_added_callback
 * PURPOSE:
 *      The function is callback function of gatt server add characteristic
 * INPUT:
 *      status              -- gatt error code status
 *      server_if           -- registered server app identifier
 *      uuid                -- added characteristic uuis
 *      srvc_handle         -- added service handle
 *      char_handle         -- added characteristic handle
 * OUTPUT:
 *      None
 * RETURN:
 *      None
 * NOTES:
 *      None
 */
VOID bluetooth_gatts_characteristic_added_callback(INT32 status,
        INT32 server_if,
        CHAR *uuid,
        INT32 srvc_handle,
        INT32 char_handle)
{
    fg_gatts_add_char = TRUE;
    gatts_add_char_info.server_if = server_if;
    gatts_add_char_info.srvc_handle = srvc_handle;
    gatts_add_char_info.char_handle = char_handle;
    memcpy(gatts_add_char_info.uuid, uuid, strlen(uuid) + 1);
    if (NULL != BtGattsAddCharCbk)
    {
        BtGattsAddCharCbk(&gatts_add_char_info);
    }
    else
    {
        BT_DBG_ERROR(BT_DEBUG_GATT, "[GATT] null pointer of BtGattsAddCharCbk");
    }
}

/**
 * FUNCTION NAME: bluetooth_gatts_descriptor_added_callback
 * PURPOSE:
 *      The function is callback function of gatt server add characteristic descriptor
 * INPUT:
 *      status              -- gatt error code status
 *      server_if           -- registered server app identifier
 *      uuid                -- added characteristic descriptor uuid
 *      srvc_handle         -- added service handle
 *      descr_handle        -- added characteristic descriptor handle
 * OUTPUT:
 *      None
 * RETURN:
 *      None
 * NOTES:
 *      None
 */
VOID bluetooth_gatts_descriptor_added_callback(INT32 status,
        INT32 server_if,
        CHAR *uuid,
        INT32 srvc_handle,
        INT32 descr_handle)
{
    gatts_add_desc_info.server_if = server_if;
    gatts_add_desc_info.srvc_handle = srvc_handle;
    gatts_add_desc_info.descr_handle = descr_handle;
    memcpy(gatts_add_desc_info.uuid, uuid, strlen(uuid) + 1);
    if (NULL != BtGattsAddDescCbk)
    {
        BtGattsAddDescCbk(&gatts_add_desc_info);
    }
    else
    {
        BT_DBG_ERROR(BT_DEBUG_GATT, "[GATT] null pointer of BtGattsAddDescCbk");
    }
}

/**
 * FUNCTION NAME: bluetooth_gatts_request_read_callback
 * PURPOSE:
 *      The function is callback function of gatt server request read
 * INPUT:
 *      conn_id             -- connection id
 *      trans_id            -- transaction id
 *      bda                 -- remote device address
 *      attr_handle         -- request read attribute handle
 *      offset              -- read offset
 *      is_long             -- is long characteristic/descriptor or not
 * OUTPUT:
 *      None
 * RETURN:
 *      None
 * NOTES:
 *      None
 */
VOID bluetooth_gatts_request_read_callback(INT32 conn_id,
        INT32 trans_id,
        CHAR *bda,
        INT32 attr_handle,
        INT32 offset,
        BOOL is_long)
{
    gatts_req_read_info.conn_id = conn_id;
    gatts_req_read_info.trans_id = trans_id;
    gatts_req_read_info.attr_handle = attr_handle;
    gatts_req_read_info.offset = offset;
    gatts_req_read_info.is_long = is_long;
    memcpy(gatts_req_read_info.btaddr, bda, strlen(bda) + 1);
    //linuxbt_btaddr_htos(bda, gatts_req_read_info.btaddr);

#if MTK_LINUX_GATTC_PTS_TEST
    if (offset == 0)
    {
        if (attr_handle == 48 || attr_handle == 49) //for invalid attribute  length
        {
            if (NULL != pts_test_value_invalid)
            {
                free(pts_test_value_invalid);
                pts_test_value_invalid = NULL;
            }
            pts_test_value_invalid = malloc(PTS_MAX_LENGTH * sizeof(CHAR));

            if (!pts_test_value_invalid)
            {
                BT_DBG_ERROR(BT_DEBUG_GATT, "[GATT] malloc pts_test_value fail!");
                return;
            }
            else
            {
                memset(pts_test_value_invalid, 0xaa, PTS_MAX_LENGTH * sizeof(CHAR));
                pts_test_value_invalid[PTS_MAX_LENGTH - 1] = '\0';
            }
            if (pts_exe_write == 1)
            {
                memcpy(pts_test_read_value_invalid, pts_test_value_invalid, PTS_MAX_LENGTH * sizeof(CHAR));
                bluetooth_gatts_send_response(conn_id, trans_id, 0, attr_handle, pts_test_value_invalid, strlen(pts_test_value_invalid), 0);
            }
            else
            {
                bluetooth_gatts_send_response(conn_id, trans_id, 0, attr_handle, pts_test_read_value_invalid, strlen(pts_test_read_value_invalid), 0);
            }
        }
        else //for other case
        {
            if (pts_test_value == NULL)
            {
                pts_test_value = malloc((PTS_TEST_MTU + 10) * sizeof(CHAR));

                if (!pts_test_value)
                {
                    BT_DBG_ERROR(BT_DEBUG_GATT, "[GATT] malloc pts_test_value fail!");
                    return;
                }
                else
                {
                    memset(pts_test_value, 0x12, (PTS_TEST_MTU + 10) * sizeof(CHAR));
                    pts_test_value[(PTS_TEST_MTU + 9)] = '\0';
                }
            }

            if (pts_exe_write == 1)
            {
                memcpy(pts_test_read_value, pts_test_value, (PTS_TEST_MTU + 10) * sizeof(CHAR));
                bluetooth_gatts_send_response(conn_id, trans_id, 0, attr_handle, pts_test_value, strlen(pts_test_value), 0);
            }
            else
            {
                bluetooth_gatts_send_response(conn_id, trans_id, 0, attr_handle, pts_test_read_value, strlen(pts_test_read_value), 0);
            }
        }
    }
    else if(offset < (PTS_TEST_MTU + 10))
    {

        CHAR *p_value = NULL;
        INT32 len = 0;
        if (pts_exe_write == 1)
        {
            p_value = pts_test_value + offset;
            len = strlen(pts_test_value) - offset + 1;
        }
        else
        {
            p_value = pts_test_read_value + offset;
            len = strlen(pts_test_read_value) - offset + 1;
        }
        bluetooth_gatts_send_response(conn_id, trans_id, 0, attr_handle, p_value, len, 0);
    }
    else if (offset == (PTS_TEST_MTU + 10))
    {
        bluetooth_gatts_send_response(conn_id, trans_id, 0, attr_handle, NULL, 0, 0);
    }
    else
    {
        //invalid offset
        bluetooth_gatts_send_response(conn_id, trans_id, 0x07, attr_handle, NULL, 0, 0);
    }
#endif
    if (NULL != BtGattsReqReadCbk)
    {
        BtGattsReqReadCbk(&gatts_req_read_info);
    }
    else
    {
        BT_DBG_ERROR(BT_DEBUG_GATT, "[GATT] null pointer of BtGattsReqReadCbk");
    }
}

/**
 * FUNCTION NAME: bluetooth_gatts_request_write_callback
 * PURPOSE:
 *      The function is callback function of gatt server request write operation
 * INPUT:
 *      conn_id             -- connection id
 *      trans_id            -- transaction id
 *      bda                 -- remote device address
 *      attr_handle         -- request read attribute handle
 *      offset              -- read offset
 *      length              -- the value length need to write
 *      need_rsp            -- gatt client need response or not
 *      is_prep             -- is prepare write or not
 *      value               -- the value need to write
 * OUTPUT:
 *      None
 * RETURN:
 *      None
 * NOTES:
 *      None
 */
VOID bluetooth_gatts_request_write_callback(INT32 conn_id,
        INT32 trans_id,
        CHAR *bda,
        INT32 attr_handle,
        INT32 offset,
        INT32 length,
        BOOL need_rsp,
        BOOL is_prep,
        UINT8* value)
{
    gatts_req_write_info.conn_id = conn_id;
    gatts_req_write_info.trans_id = trans_id;
    gatts_req_write_info.attr_handle = attr_handle;
    gatts_req_write_info.offset = offset;
    gatts_req_write_info.length = length;
    gatts_req_write_info.need_rsp = need_rsp;
    gatts_req_write_info.is_prep = is_prep;
    memcpy(gatts_req_write_info.btaddr, bda, strlen(bda) + 1);
    //linuxbt_btaddr_htos(bda, gatts_req_write_info.btaddr);
    memset(gatts_req_write_info.value, 0, BT_GATT_MAX_ATTR_LEN);
    BT_DBG_INFO(BT_DEBUG_GATT,"[GATT] length = %ld", (long)length);
    if ((0 != length) && (length <= BT_GATT_MAX_ATTR_LEN))
    {
        memcpy(gatts_req_write_info.value, value, length);
#if MTK_LINUX_GATTC_PTS_TEST
        pts_write_offset = offset;
        if (pts_test_value == NULL)
        {
            pts_test_value = malloc((PTS_TEST_MTU + 10) * sizeof(CHAR));

            if (!pts_test_value)
            {
                BT_DBG_ERROR(BT_DEBUG_GATT, "[GATT] malloc pts_test_value fail!");
                return;
            }
        }
        if (offset == 0)
        {
            if (attr_handle == 48 || attr_handle == 49)
            {
                memcpy(pts_test_value_invalid, value, length);
                pts_test_value_invalid[length] = '\0';
                if (length >= PTS_MAX_LENGTH)//for invalid attribute  length
                {
                    if (need_rsp)
                    {
                        if (is_prep)
                        {
                            bluetooth_gatts_send_response(conn_id, trans_id, 0x0d, attr_handle, NULL, 0, 0);
                        }
                        else
                        {
                            pts_exe_write = 0;
                            bluetooth_gatts_send_response(conn_id, trans_id, 0x0d, attr_handle, NULL, 0, 0);
                        }
                    }
                }
                else
                {
                    if (need_rsp)
                    {
                        if (is_prep)
                        {
                            bluetooth_gatts_send_response(conn_id, trans_id, BT_STATUS_SUCCESS, attr_handle, pts_test_value_invalid, length, 0);
                        }
                        else
                        {
                            pts_exe_write = 1;
                            bluetooth_gatts_send_response(conn_id, trans_id, BT_STATUS_SUCCESS, attr_handle, NULL, 0, 0);
                        }
                    }
                }
             }
            else  //for other case
            {
                memcpy(pts_test_value, value, length);
                pts_test_value[length] = '\0';
                if (need_rsp)
                {
                    if (is_prep)
                    {
                        bluetooth_gatts_send_response(conn_id, trans_id, BT_STATUS_SUCCESS, attr_handle, pts_test_value, length, 0);
                    }
                    else
                    {
                        pts_exe_write = 1;
                        bluetooth_gatts_send_response(conn_id, trans_id, BT_STATUS_SUCCESS, attr_handle, NULL, 0, 0);
                    }
                }
            }
        }
        else if (offset < (PTS_TEST_MTU + 10))
        {
            if(offset + length > (PTS_TEST_MTU + 10))
            {
                pts_invalid_length = offset + length;
                memcpy(pts_test_value + offset, value, PTS_TEST_MTU + 10 - offset);
                pts_test_value[PTS_TEST_MTU + 9] = '\0';
            }
            else
            {
                memcpy(pts_test_value + offset, value, length);
                pts_test_value[offset + length] = '\0';
            }
            if (need_rsp && is_prep)
            {
                bluetooth_gatts_send_response(conn_id, trans_id, BT_STATUS_SUCCESS, attr_handle, pts_test_value + offset, length, 0);
            }
        }
        else
        {
            if (need_rsp && is_prep)
            {
                bluetooth_gatts_send_response(conn_id, trans_id, BT_STATUS_SUCCESS, attr_handle, (CHAR *)value, length, 0);
            }
            //invalid offset
            //bluetooth_gatts_send_response(conn_id, trans_id, 0x07, attr_handle, NULL, 0, 0);
        }
#endif
    }
    if (NULL != BtGattsReqWriteCbk)
    {
        BtGattsReqWriteCbk(&gatts_req_write_info);
    }
    else
    {
        BT_DBG_ERROR(BT_DEBUG_GATT, "[GATT] null pointer of BtGattsReqWriteCbk");
    }
}

/**
 * FUNCTION NAME: bluetooth_gatts_request_exec_write_callback
 * PURPOSE:
 *      The function is callback function of gatt server execute write operation
 * INPUT:
 *      conn_id             -- connection id
 *      trans_id            -- transaction id
 *      bda                 -- remote device address
 *      exec_write          -- execute write or not
 * OUTPUT:
 *      None
 * RETURN:
 *      None
 * NOTES:
 *      None
 */
VOID bluetooth_gatts_request_exec_write_callback(INT32 conn_id,
        INT32 trans_id,
        CHAR *bda,
        INT32 exec_write)
{
#if MTK_LINUX_GATTC_PTS_TEST
    pts_exe_write = exec_write;
    if ((pts_write_offset < (PTS_TEST_MTU + 10)) && (pts_invalid_length <= (PTS_TEST_MTU + 10)))
    {
         bluetooth_gatts_send_response(conn_id, trans_id, BT_STATUS_SUCCESS, conn_id, NULL, 0, 0);
    }
    else if (pts_invalid_length > (PTS_TEST_MTU + 10))
    {
        bluetooth_gatts_send_response(conn_id, trans_id, 0x0d, conn_id, NULL, 0, 0);
    }
    else
    {
        //invalid offset
        bluetooth_gatts_send_response(conn_id, trans_id, 0x07, conn_id, NULL, 0, 0);
    }
#endif
#if MTK_BLE_GGL_SETUP_SUPPORT
    c_btm_bt_gatts_request_exec_write_cbk(conn_id, trans_id, bda, exec_write);
#endif

}

/**
 * FUNCTION NAME: bluetooth_gatts_response_confirmation_callback
 * PURPOSE:
 *      The function is callback function of gatt server response confirmation operation
 * INPUT:
 *      status              -- gatt error code status
 *      handle              -- response confirmation attribute handle
 * OUTPUT:
 *      None
 * RETURN:
 *      None
 * NOTES:
 *      None
 */
VOID bluetooth_gatts_response_confirmation_callback(INT32 status, INT32 handle)
{
}

/**
 * FUNCTION NAME: bluetooth_gatts_indication_sent_callback
 * PURPOSE:
 *      The function is callback function of gatt server send indication operation
 * INPUT:
 *      conn_id             -- connection id
 *      status              -- gatt error code status
 * OUTPUT:
 *      None
 * RETURN:
 *      None
 * NOTES:
 *      None
 */
VOID bluetooth_gatts_indication_sent_callback(INT32 conn_id, INT32 status)
{
    if (NULL != BtGattsIndSentCbk)
    {
        BtGattsIndSentCbk(conn_id, status);
    }
    else
    {
        BT_DBG_ERROR(BT_DEBUG_GATT, "[GATT] null pointer of BtGattsIndSentCbk");
    }
}

/**
 * FUNCTION NAME: bluetooth_gatts_congestion_callback
 * PURPOSE:
 *      The function is callback function of gatt server congestion operation
 * INPUT:
 *      conn_id             -- connection id
 *      congested           -- congested or not
 * OUTPUT:
 *      None
 * RETURN:
 *      None
 * NOTES:
 *      None
 */
VOID bluetooth_gatts_congestion_callback(INT32 conn_id, BOOL congested)
{
}

/**
 * FUNCTION NAME: bluetooth_gatts_mtu_changed_callback
 * PURPOSE:
 *      The function is callback function of gatt server mtu changed operation
 * INPUT:
 *      conn_id             -- connection id
 *      mtu                 -- mtu
 * OUTPUT:
 *      None
 * RETURN:
 *      None
 * NOTES:
 *      None
 */
VOID bluetooth_gatts_mtu_changed_callback(INT32 conn_id, INT32 mtu)
{
#if MTK_LINUX_GATTC_PTS_TEST
    PTS_TEST_MTU = mtu;
    pts_test_value = malloc((PTS_TEST_MTU + 10) * sizeof(CHAR));

    if (!pts_test_value)
    {
        BT_DBG_ERROR(BT_DEBUG_GATT, "[GATT] malloc pts_test_value fail!");
        return;
    }
    else
    {
        memset(pts_test_value, 0x12, (PTS_TEST_MTU + 10) * sizeof(CHAR));
        pts_test_value[(PTS_TEST_MTU + 9)] = '\0';
    }
#endif

#if MTK_BLE_GGL_SETUP_SUPPORT
    c_btm_bt_gatts_mtu_changed_cbk(conn_id, mtu);
#endif
}

/**
 * FUNCTION NAME: bluetooth_gatts_service_started_callback
 * PURPOSE:
 *      The function is callback function of gatt server start service operation
 * INPUT:
 *      status              -- gatt error code status
 *      server_if           -- registered server app identifier
 *      srvc_handle         -- start service handle
 * OUTPUT:
 *      None
 * RETURN:
 *      None
 * NOTES:
 *      None
 */
VOID bluetooth_gatts_service_started_callback(INT32 status, INT32 server_if, INT32 srvc_handle)
{
    //FUNC_ENTRY;
    fg_gatts_start_service = TRUE;
    gatts_srvc_info.server_if = server_if;
    gatts_srvc_info.srvc_handle = srvc_handle;
    if (NULL != BtGattsOpSrvcCbk)
    {
        BtGattsOpSrvcCbk(BT_GATTS_START_SRVC, &gatts_srvc_info);
    }
    else
    {
        BT_DBG_ERROR(BT_DEBUG_GATT, "[GATT] null pointer of BtGattGetCharCbk");
    }
}

/**
 * FUNCTION NAME: bluetooth_gatts_service_stopped_callback
 * PURPOSE:
 *      The function is callback function of gatt server stop service operation
 * INPUT:
 *      status              -- gatt error code status
 *      server_if           -- registered server app identifier
 *      srvc_handle         -- start service handle
 * OUTPUT:
 *      None
 * RETURN:
 *      None
 * NOTES:
 *      None
 */
VOID bluetooth_gatts_service_stopped_callback(INT32 status, INT32 server_if, INT32 srvc_handle)
{
    //FUNC_ENTRY;
    gatts_srvc_info.server_if = server_if;
    gatts_srvc_info.srvc_handle = srvc_handle;
    if (NULL != BtGattsOpSrvcCbk)
    {
        BtGattsOpSrvcCbk(BT_GATTS_STOP_SRVC, &gatts_srvc_info);
    }
    else
    {
        BT_DBG_ERROR(BT_DEBUG_GATT, "[GATT] null pointer of BtGattGetCharCbk");
    }
}

/**
 * FUNCTION NAME: bluetooth_gatts_service_deleted_callback
 * PURPOSE:
 *      The function is callback function of gatt server delete service operation
 * INPUT:
 *      status              -- gatt error code status
 *      server_if           -- registered server app identifier
 *      srvc_handle         -- start service handle
 * OUTPUT:
 *      None
 * RETURN:
 *      None
 * NOTES:
 *      None
 */
VOID bluetooth_gatts_service_deleted_callback(INT32 status, INT32 server_if, INT32 srvc_handle)
{
    //FUNC_ENTRY;
    gatts_srvc_info.server_if = server_if;
    gatts_srvc_info.srvc_handle = srvc_handle;
    if (NULL != BtGattsOpSrvcCbk)
    {
        BtGattsOpSrvcCbk(BT_GATTS_DEL_SRVC, &gatts_srvc_info);
    }
    else
    {
        BT_DBG_ERROR(BT_DEBUG_GATT, "[GATT] null pointer of BtGattsOpSrvcCbk");
    }
}




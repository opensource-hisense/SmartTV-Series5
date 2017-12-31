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

/* FILE NAME:  c_bt_mw_gatt.c
 * AUTHOR: Xuemei Yang
 * PURPOSE:
 *      It provides GATTC and GATTS API to APP.
 * NOTES:
 */


/*-----------------------------------------------------------------------------
                    include files
 ----------------------------------------------------------------------------*/
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <linux/input.h>
#include <linux/uinput.h>
#include <pthread.h>
#include "bt_mw_common.h"
#include "c_bt_mw_gatt.h"
#include "bt_mw_gatt.h"
#include "bluetooth_sync.h"
#include "c_bt_mw_manager.h"


#if MTK_BLE_GGL_SETUP_SUPPORT
BOOL g_fg_createdService = FALSE;
BOOL g_fg_startedService = FALSE;
char started_service_uuid[8][64];
char created_service_uuid[8][64];
BOOL g_fg_dcoff = FALSE;
BOOL g_fg_charIsExist = FALSE;
BOOL g_fg_serviceIsExist = FALSE;


#endif



/******************** gattc ********************/
/**
 * FUNCTION NAME: c_btm_bt_gattc_base_init
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
EXPORT_SYMBOL INT32 c_btm_bt_gattc_base_init(BT_APP_GATTC_CB_FUNC_T *func)
{
    //FUNC_ENTRY;
    return bluetooth_gatt_base_init(func);
}

/**
 * FUNCTION NAME: c_btm_bt_gattc_register_app_sync
 * PURPOSE:
 *      The function is used to synchronous register gatt client APP.
 * INPUT:
 *      uuid               -- app uuid
 * OUTPUT:
 *      client_if          -- registered app identifer
 * RETURN:
 *      BT_SUCCESS                        -- Operate success.
 *      BT_ERR_STATUS_FAIL                -- Operate fail.
 *      BT_ERR_STATUS_PARM_INVALID        -- paramter is invalid.
 * NOTES:
 *      None
 */
EXPORT_SYMBOL INT32 c_btm_bt_gattc_register_app_sync(CHAR *uuid, INT32 *client_if)
{
    //FUNC_ENTRY;
    return bluetooth_gattc_register_app_sync(uuid, client_if);
}

/**
 * FUNCTION NAME: c_btm_bt_gattc_register_app
 * PURPOSE:
 *      The function is used to register gatt client APP.
 * INPUT:
 *      uuid               -- app uuid
 * OUTPUT:
 *      None
 * RETURN:
 *      BT_SUCCESS                        -- Operate success.
 *      BT_ERR_STATUS_FAIL                -- Operate fail.
 *      BT_ERR_STATUS_PARM_INVALID        -- paramter is invalid.
 * NOTES:
 *      None
 */
EXPORT_SYMBOL INT32 c_btm_bt_gattc_register_app(CHAR *uuid)
{
    //FUNC_ENTRY;
    return bluetooth_gattc_register_app(uuid);
}

/**
 * FUNCTION NAME: c_btm_bt_gattc_unregister_app
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
EXPORT_SYMBOL INT32 c_btm_bt_gattc_unregister_app(INT32 client_if)
{
    //FUNC_ENTRY;
    return bluetooth_gattc_unregister_app(client_if);

}

/**
 * FUNCTION NAME: c_btm_bt_gattc_scan
 * PURPOSE:
 *      The function is used for gatt client scan remote device.
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
EXPORT_SYMBOL INT32 c_btm_bt_gattc_scan()
{
    //FUNC_ENTRY;
    return bluetooth_gattc_scan();

}

/**
 * FUNCTION NAME: c_btm_bt_gattc_stop_scan
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
EXPORT_SYMBOL INT32 c_btm_bt_gattc_stop_scan()
{
    //FUNC_ENTRY;
    return bluetooth_gattc_stop_scan();
}

/**
 * FUNCTION NAME: c_btm_bt_gattc_open
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
EXPORT_SYMBOL INT32 c_btm_bt_gattc_open(INT32 client_if, CHAR *bt_addr, UINT8 is_direct, INT32 transport)
{
    //FUNC_ENTRY;
    return bluetooth_gattc_open(client_if, bt_addr, is_direct, transport);

}

/**
 * FUNCTION NAME: c_btm_bt_gattc_close
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
EXPORT_SYMBOL INT32 c_btm_bt_gattc_close(INT32 client_if, CHAR *bt_addr, INT32 conn_id)
{
    //FUNC_ENTRY;
    return bluetooth_gattc_close(client_if, bt_addr, conn_id);

}

/**
 * FUNCTION NAME: c_btm_bt_gattc_listen
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
EXPORT_SYMBOL INT32 c_btm_bt_gattc_listen(INT32 client_if)
{
    //FUNC_ENTRY;
    return bluetooth_gattc_listen(client_if);

}

/**
 * FUNCTION NAME: c_btm_bt_gattc_refresh
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
EXPORT_SYMBOL INT32 c_btm_bt_gattc_refresh(INT32 client_if, CHAR *bt_addr)
{
    //FUNC_ENTRY;
    return bluetooth_gattc_refresh(client_if, bt_addr);

}

/**
 * FUNCTION NAME: c_btm_bt_gattc_search_service
 * PURPOSE:
 *      The function is used for gatt client search service on the remote device.
 * INPUT:
 *      conn_id                -- connection id
 *      uuid                   -- service uuid
 * OUTPUT:
 *      None
 * RETURN:
 *      BT_SUCCESS                        -- Operate success.
 *      BT_ERR_STATUS_FAIL                -- Operate fail.
 *      BT_ERR_STATUS_PARM_INVALID        -- paramter is invalid.
 * NOTES:
 *      None
 */
EXPORT_SYMBOL INT32 c_btm_bt_gattc_search_service(INT32 conn_id, CHAR *uuid)
{
    //FUNC_ENTRY;
    return bluetooth_gattc_search_service(conn_id, uuid);
}

EXPORT_SYMBOL INT32 c_btm_bt_gattc_get_gatt_db(INT32 conn_id)
{
    return bluetooth_gattc_get_gatt_db(conn_id);
}

/**
 * FUNCTION NAME: c_btm_bt_gattc_get_char
 * PURPOSE:
 *      The function is used for gatt client get characiteristic on loacal cache database.
 * INPUT:
 *      conn_id                -- connection id
 *      service_uuid           -- service uuid
 *      service_inst_id        -- service instance id
 *      start_uuid             -- first charateristic uuid
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
EXPORT_SYMBOL INT32 c_btm_bt_gattc_get_char(INT32 conn_id,
                              CHAR *service_uuid,
                              INT32 service_inst_id,
                              CHAR *start_uuid,
                              INT32 start_inst_id)
{
    //FUNC_ENTRY;
    return bluetooth_gattc_get_char(conn_id, service_uuid, service_inst_id,start_uuid, start_inst_id);
}


/**
 * FUNCTION NAME: c_btm_bt_gattc_get_descr
 * PURPOSE:
 *      The function is used for gatt client get char description on loacal cache database.
 * INPUT:
 *      conn_id                -- connection id
 *      service_uuid           -- service uuid
 *      service_inst_id        -- service instance id
 *      char_uuid             -- first charateristic uuid
 *      char_inst_id          -- first charateristic instance id
 *      start_uuid             -- first char description uuid
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
EXPORT_SYMBOL INT32 c_btm_bt_gattc_get_descr(INT32 conn_id,
                               CHAR *service_uuid,
                               INT32 service_inst_id,
                               CHAR *char_uuid,
                               INT32 char_inst_id,
                               CHAR *start_uuid,
                               INT32 start_inst_id)
{
    //FUNC_ENTRY;
    return bluetooth_gattc_get_descr(conn_id, service_uuid, service_inst_id, char_uuid, char_inst_id,
                                     start_uuid, start_inst_id);
}

/**
 * FUNCTION NAME: c_btm_bt_gattc_get_incl_service
 * PURPOSE:
 *      The function is used for gatt client get included service on loacal cache database.
 * INPUT:
 *      conn_id                -- connection id
 *      service_uuid           -- service uuid
 *      service_inst_id        -- service instance id
 *      incl_uuid              -- included service uuid
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
EXPORT_SYMBOL INT32 c_btm_bt_gattc_get_incl_service(INT32 conn_id,
                                      CHAR *service_uuid,
                                      INT32 service_inst_id,
                                      CHAR *incl_uuid,
                                      INT32 incl_inst_id)
{
    //FUNC_ENTRY;
    return bluetooth_gattc_get_incl_service(conn_id, service_uuid, service_inst_id, incl_uuid, incl_inst_id);
}

/**
 * FUNCTION NAME: c_btm_bt_gattc_read_char
 * PURPOSE:
 *      The function is used for gatt client read characteristic from remote device
 * INPUT:
 *      conn_id                -- connection id
 *      service_uuid           -- service uuid
 *      service_inst_id        -- service instance id
 *      char_uuid              -- characteristic uuid
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
EXPORT_SYMBOL INT32 c_btm_bt_gattc_read_char(INT32 conn_id,
                               INT32 char_handle,
                               INT32 auth_req)
{
    //FUNC_ENTRY;
    return bluetooth_gattc_read_char(conn_id, char_handle, auth_req);
}

/**
 * FUNCTION NAME: c_btm_bt_gattc_read_descr
 * PURPOSE:
 *      The function is used for gatt client read char description from remote device
 * INPUT:
 *      conn_id                -- connection id
 *      service_uuid           -- service uuid
 *      service_inst_id        -- service instance id
 *      char_uuid              -- characteristic uuid
 *      char_inst_id           -- characteristic instance id
 *      descr_uuid              -- char descriptor uuid
 *      descr_inst_id           -- char descriptor instance id
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
EXPORT_SYMBOL INT32 c_btm_bt_gattc_read_descr(INT32 conn_id,
                                INT32 descr_handle,
                                INT32 auth_req)
{
    //FUNC_ENTRY;
    return bluetooth_gattc_read_descr(conn_id, descr_handle, auth_req);
}

/**
 * FUNCTION NAME: c_btm_bt_gattc_write_char
 * PURPOSE:
 *      The function is used for gatt client write characteristic to remote device
 * INPUT:
 *      conn_id                -- connection id
 *      service_uuid           -- service uuid
 *      service_inst_id        -- service instance id
 *      char_uuid              -- characteristic uuid
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
EXPORT_SYMBOL INT32 c_btm_bt_gattc_write_char(INT32 conn_id,
                                INT32 char_handle,
                                INT32 write_type,
                                INT32 len,
                                INT32 auth_req,
                                CHAR *value)
{
    //FUNC_ENTRY;
    return bluetooth_gattc_write_char(conn_id, char_handle,
                                      write_type, len, auth_req, value);
}

EXPORT_SYMBOL INT32 c_btm_bt_gattc_execute_write(INT32 conn_id, INT32 execute)
{
    return bluetooth_gattc_execute_write(conn_id, execute);
}

/**
 * FUNCTION NAME: c_btm_bt_gattc_write_descr
 * PURPOSE:
 *      The function is used for gatt client write char description to remote device
 * INPUT:
 *      conn_id                -- connection id
 *      service_uuid           -- service uuid
 *      service_inst_id        -- service instance id
 *      char_uuid              -- characteristic uuid
 *      char_inst_id           -- characteristic instance id
 *      descr_uuid              -- char description uuid
 *      descr_inst_id           -- char description instance id
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
EXPORT_SYMBOL INT32 c_btm_bt_gattc_write_descr(INT32 conn_id,
                                 INT32 descr_handle,
                                 INT32 write_type,
                                 INT32 len,
                                 INT32 auth_req,
                                 CHAR *value)
{
    //FUNC_ENTRY;
    return bluetooth_gattc_write_descr(conn_id, descr_handle, write_type, len, auth_req, value);
}

/**
 * FUNCTION NAME: c_btm_bt_gattc_reg_noti
 * PURPOSE:
 *      The function is used for gatt client register notification
 * INPUT:
 *      client_if              -- registered client app identifier
 *      bt_addr                -- remote device bt address
 *      service_uuid           -- service uuid
 *      service_inst_id        -- service instance id
 *      char_uuid              -- characteristic uuid
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
EXPORT_SYMBOL INT32 c_btm_bt_gattc_reg_noti(INT32 client_if,
                              CHAR *bt_addr,
                              INT32 char_handle)
{
    //FUNC_ENTRY;
    return bluetooth_gattc_reg_noti(client_if, bt_addr, char_handle);
}

/**
 * FUNCTION NAME: c_btm_bt_gattc_dereg_noti
 * PURPOSE:
 *      The function is used for gatt client deregister notification
 * INPUT:
 *      client_if              -- registered client app identifier
 *      bt_addr                -- remote device bt address
 *      service_uuid           -- service uuid
 *      service_inst_id        -- service instance id
 *      char_uuid              -- characteristic uuid
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
EXPORT_SYMBOL INT32 c_btm_bt_gattc_dereg_noti(INT32 client_if,
                                CHAR *bt_addr,
                                INT32 char_handle)
{
    //FUNC_ENTRY;
    return bluetooth_gattc_dereg_noti(client_if, bt_addr, char_handle);
}

/**
 * FUNCTION NAME: c_btm_bt_gattc_read_rssi
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
EXPORT_SYMBOL INT32 c_btm_bt_gattc_read_rssi(INT32 client_if, CHAR *bt_addr)
{
    //FUNC_ENTRY;
    return bluetooth_gattc_read_rssi(client_if, bt_addr);
}

/**
 * FUNCTION NAME: c_btm_bt_gattc_get_connect_result_info
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
EXPORT_SYMBOL VOID c_btm_bt_gattc_get_connect_result_info(BT_GATTC_CONNECT_RST_T *connect_rst_info)
{
    //FUNC_ENTRY;
    bluetooth_gattc_get_connect_result_info(connect_rst_info);
}

/**
 * FUNCTION NAME: c_btm_bt_gattc_get_disconnect_result_info
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
EXPORT_SYMBOL VOID c_btm_bt_gattc_get_disconnect_result_info(BT_GATTC_CONNECT_RST_T *disconnect_rst_info)
{
    //FUNC_ENTRY;
    bluetooth_gattc_get_disconnect_result_info(disconnect_rst_info);
}

/**
 * FUNCTION NAME: c_btm_bt_gattc_read_rssi_result_info
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
EXPORT_SYMBOL VOID c_btm_bt_gattc_read_rssi_result_info(BT_GATTC_GET_REMOTE_RSSI_T *get_remote_rssi_info)
{
    //FUNC_ENTRY;
    bluetooth_gattc_read_rssi_result_info(get_remote_rssi_info);
}

/**
 * FUNCTION NAME: c_btm_bt_gattc_conn_parameter_update
 * PURPOSE:
 *      The function is used for gatt client to update connection paramters
 * INPUT:
 *      bt_addr                -- remote device bt address
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
EXPORT_SYMBOL INT32 c_btm_bt_gattc_conn_parameter_update(CHAR *bt_addr,INT32 min_interval,INT32 max_interval,
        INT32 latency,INT32 timeout)
{
    //FUNC_ENTRY;
    return bluetooth_gattc_conn_parameter_update(bt_addr, min_interval, max_interval, latency, timeout);
}

/**
 * FUNCTION NAME: c_btm_bt_gattc_set_scan_parameters
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
EXPORT_SYMBOL INT32 c_btm_bt_gattc_set_scan_parameters(INT32 client_if, INT32 scan_interval, INT32 scan_window)
{
    //FUNC_ENTRY;
    return bluetooth_gattc_set_scan_parameters(client_if, scan_interval, scan_window);
}

/**
 * FUNCTION NAME: c_btm_bt_gattc_multi_adv_enable
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
EXPORT_SYMBOL INT32 c_btm_bt_gattc_multi_adv_enable(INT32 client_if,
                                      INT32 min_interval,
                                      INT32 max_interval,
                                      INT32 adv_type,
                                      INT32 chnl_map,
                                      INT32 tx_power,
                                      INT32 timeout_s)
{
    //FUNC_ENTRY;
    return bluetooth_gattc_multi_adv_enable(client_if, min_interval, max_interval,
                                            adv_type, chnl_map, tx_power, timeout_s);
}

/**
 * FUNCTION NAME: c_btm_bt_gattc_multi_adv_enable_sync
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
EXPORT_SYMBOL INT32 c_btm_bt_gattc_multi_adv_enable_sync(INT32 client_if,
        INT32 min_interval,
        INT32 max_interval,
        INT32 adv_type,
        INT32 chnl_map,
        INT32 tx_power,
        INT32 timeout_s)
{
    //FUNC_ENTRY;
    return bluetooth_gattc_multi_adv_enable_sync(client_if, min_interval, max_interval,
            adv_type, chnl_map, tx_power, timeout_s);
}

/**
 * FUNCTION NAME: c_btm_bt_gattc_multi_adv_update
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
EXPORT_SYMBOL INT32 c_btm_bt_gattc_multi_adv_update(INT32 client_if,
                                      INT32 min_interval,
                                      INT32 max_interval,
                                      INT32 adv_type,
                                      INT32 chnl_map,
                                      INT32 tx_power,
                                      INT32 timeout_s)
{
    //FUNC_ENTRY;
    return bluetooth_gattc_multi_adv_update(client_if, min_interval, max_interval,
                                            adv_type, chnl_map, tx_power, timeout_s);
}

/**
 * FUNCTION NAME: c_btm_bt_gattc_multi_adv_setdata
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
EXPORT_SYMBOL INT32 c_btm_bt_gattc_multi_adv_setdata(INT32 client_if,
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
    //FUNC_ENTRY;
    return bluetooth_gattc_multi_adv_setdata(client_if, set_scan_rsp, include_name,
            include_txpower, appearance, manufacturer_len, manufacturer_data,
            service_data_len, service_data, service_uuid_len, service_uuid);
}

/**
 * FUNCTION NAME: c_btm_bt_gattc_multi_adv_setdata_sync
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
EXPORT_SYMBOL INT32 c_btm_bt_gattc_multi_adv_setdata_sync(INT32 client_if,
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
    //FUNC_ENTRY;
    return bluetooth_gattc_multi_adv_setdata_sync(client_if, set_scan_rsp, include_name,
            include_txpower, appearance, manufacturer_len, manufacturer_data,
            service_data_len, service_data, service_uuid_len, service_uuid);
}


/**
 * FUNCTION NAME: c_btm_bt_gattc_multi_adv_setdata
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
EXPORT_SYMBOL INT32 c_btm_bt_gattc_multi_adv_disable(INT32 client_if)
{
    //FUNC_ENTRY;
    return bluetooth_gattc_multi_adv_disable(client_if);
}

EXPORT_SYMBOL INT32 c_btm_bt_gattc_batchscan_cfg_storage(INT32 client_if, INT32 batch_scan_full_max,
                                                INT32 batch_scan_trunc_max, INT32 batch_scan_notify_threshold)
{
    return bluetooth_gattc_batchscan_cfg_storage(client_if, batch_scan_full_max,
                                                 batch_scan_trunc_max, batch_scan_notify_threshold);
}

EXPORT_SYMBOL INT32 c_btm_bt_gattc_batchscan_enb_batch_scan(INT32 client_if, INT32 scan_mode,
                        INT32 scan_interval, INT32 scan_window, INT32 addr_type, INT32 discard_rule)
{
    return bluetooth_gattc_batchscan_enb_batch_scan(client_if, scan_mode,
                        scan_interval, scan_window, addr_type, discard_rule);
}

EXPORT_SYMBOL INT32 c_btm_bt_gattc_batchscan_dis_batch_scan(INT32 client_if)
{
    return bluetooth_gattc_batchscan_dis_batch_scan(client_if);
}


EXPORT_SYMBOL INT32 c_btm_bt_gattc_batchscan_read_reports(INT32 client_if, INT32 scan_mode)
{
    return bluetooth_gattc_batchscan_read_reports(client_if, scan_mode);
}


EXPORT_SYMBOL INT32 c_btm_gattc_scan_filter_param_setup(bluetooth_gatt_filt_param_setup_t scan_filt_param)
{
    //FUNC_ENTRY;
    return bluetooth_gattc_scan_filter_param_setup(scan_filt_param);
}

EXPORT_SYMBOL INT32 c_btm_gattc_scan_filter_enable(INT32 client_if)
{
    //FUNC_ENTRY;
    return bluetooth_gattc_scan_filter_enable(client_if);
}

EXPORT_SYMBOL INT32 c_btm_gattc_scan_filter_disable(INT32 client_if)
{
    //FUNC_ENTRY;
    return bluetooth_gattc_scan_filter_disable(client_if);
}

EXPORT_SYMBOL INT32 c_btm_gattc_scan_filter_add_remove(INT32 client_if, INT32 action,
        INT32 filt_type, INT32 filt_index, INT32 company_id,
        INT32 company_id_mask, const CHAR *p_uuid,
        const CHAR *p_uuid_mask, const CHAR *bd_addr,
        CHAR addr_type, INT32 data_len, CHAR *p_data, INT32 mask_len,
        CHAR *p_mask)
{
    //FUNC_ENTRY;
    return bluetooth_gattc_scan_filter_add_remove(client_if, action,
            filt_type, filt_index, company_id,
            company_id_mask, p_uuid,
            p_uuid_mask, bd_addr,
            addr_type, data_len, p_data, mask_len,
            p_mask);
}


EXPORT_SYMBOL INT32 c_btm_gattc_set_local_le_name(INT32 client_if, CHAR *le_name)
{
#if MTK_LINUX_GATTC_LE_NAME //== TRUE
    return bluetooth_gattc_set_local_le_name(client_if, le_name);
#else
    return BT_SUCCESS;
#endif
}

EXPORT_SYMBOL INT32 c_btm_gattc_set_local_disc_mode(INT32 client_if, INT32 disc_mode)
{
#if MTK_LINUX_GATTC_DISC_MODE
    return bluetooth_gattc_set_local_disc_mode(client_if, disc_mode);
#else
    return BT_SUCCESS;
#endif
}

EXPORT_SYMBOL INT32 c_btm_gattc_get_local_adv_rpa(INT32 client_if)
{
#if MTK_LINUX_GATTC_RPA
    return bluetooth_gattc_get_local_adv_rpa(client_if);
#else
    return BT_SUCCESS;
#endif
}

EXPORT_SYMBOL INT32 c_btm_gattc_read_using_char_uuid(INT32 conn_id,
                                                 INT32 start_handle,
                                                 INT32 end_handle,
                                                 CHAR *uuid,
                                                 INT32 auth_req)
{
#if MTK_LINUX_GATTC_PTS_TEST
    return bluetooth_gattc_read_using_char_uuid(conn_id, start_handle, end_handle, uuid, auth_req);
#else
    return BT_SUCCESS;
#endif
}

EXPORT_SYMBOL INT32 c_btm_gattc_read_long_characteristic(INT32 conn_id,
                                                    INT32 handle,
                                                    INT32 offset,
                                                    INT32 auth_req)
{
#if MTK_LINUX_GATTC_PTS_TEST
    return bluetooth_gattc_read_long_characteristic(conn_id, handle, offset, auth_req);
#else
    return 0;
#endif
}

EXPORT_SYMBOL INT32 c_btm_gattc_read_long_descr(INT32 conn_id,
                                                    INT32 handle,
                                                    INT32 offset,
                                                    INT32 auth_req)
{
#if MTK_LINUX_GATTC_PTS_TEST
    return bluetooth_gattc_read_long_descr(conn_id, handle, offset, auth_req);
#else
    return BT_SUCCESS;
#endif
}

EXPORT_SYMBOL INT32 c_btm_gattc_write_long_characteristic(INT32 conn_id,
                                                     INT32 char_handle,
                                                     INT32 write_type,
                                                     INT32 len,
                                                     INT32 offset,
                                                     INT32 auth_req,
                                                     CHAR *value)
{
#if MTK_LINUX_GATTC_PTS_TEST
    return bluetooth_gattc_write_long_characteristic(conn_id, char_handle, write_type,
        len, offset, auth_req, value);
#else
    return BT_SUCCESS;
#endif
}

EXPORT_SYMBOL INT32 c_btm_gattc_write_long_descr(INT32 conn_id,
                                                 INT32 descr_handle,
                                                 INT32 write_type,
                                                 INT32 len,
                                                 INT32 offset,
                                                 INT32 auth_req,
                                                 CHAR *value)
{
#if MTK_LINUX_GATTC_PTS_TEST
    return bluetooth_gattc_write_long_descr(conn_id, descr_handle, write_type,
        len, offset, auth_req, value);
#else
    return BT_SUCCESS;
#endif
}

EXPORT_SYMBOL INT32 c_btm_gattc_set_pts_flag(UINT8 pts_flag)
{
#if MTK_LINUX_GATTC_PTS_TEST
    return bluetooth_gattc_set_pts_flag(pts_flag);
#else
    return BT_SUCCESS;
#endif
}
EXPORT_SYMBOL INT32 c_btm_gattc_read_multi_characteristic(INT32 conn_id,
                                                 INT32 num_attr,
                                                 UINT16 *handles,
                                                 INT32 auth_req)
{
#if MTK_LINUX_GATTC_PTS_TEST
    return bluetooth_gattc_read_multi_characteristic(conn_id, num_attr, handles, auth_req);
#else
    return BT_SUCCESS;
#endif
}


EXPORT_SYMBOL INT32 c_btm_gattc_scan_filter_clear(INT32 client_if, INT32 filt_index)
{
    //FUNC_ENTRY;
    return bluetooth_gattc_scan_filter_clear(client_if, filt_index);
}

EXPORT_SYMBOL INT32 c_btm_gattc_get_device_type(CHAR *bt_addr)
{
    return bluetooth_gattc_get_device_type(bt_addr);
}

EXPORT_SYMBOL INT32 c_btm_gattc_configure_mtu(INT32 conn_id, INT32 mtu)
{
    return bluetooth_gattc_configure_mtu(conn_id, mtu);
}

EXPORT_SYMBOL INT32 c_btm_gattc_test_command(INT32 command, BT_GATTC_TEST_PARAMS_T test_params)
{
    return bluetooth_gattc_test_command(command, test_params);
}

/******************** gatts ********************/
/**
 * FUNCTION NAME: c_btm_bt_gatts_base_init
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
EXPORT_SYMBOL INT32 c_btm_bt_gatts_base_init(BT_APP_GATTS_CB_FUNC_T *func)
{
    //FUNC_ENTRY;
    return bluetooth_gatts_base_init(func);
}

/**
 * FUNCTION NAME: c_btm_bt_gatts_register_server_sync
 * PURPOSE:
 *      The function is used to synchronous register gatt server APP.
 * INPUT:
 *      uuid               -- app uuid
 * OUTPUT:
 *      server_if          -- registered app identifer
 * RETURN:
 *      BT_SUCCESS                        -- Operate success.
 *      BT_ERR_STATUS_FAIL                -- Operate fail.
 *      BT_ERR_STATUS_PARM_INVALID        -- paramter is invalid.
 * NOTES:
 *      None
 */
EXPORT_SYMBOL INT32 c_btm_bt_gatts_register_server_sync(CHAR *uuid, INT32 *server_if)
{
    //FUNC_ENTRY;
    return bluetooth_gatts_register_app_sync(uuid, server_if);

}

/**
 * FUNCTION NAME: c_btm_bt_gatts_register_server
 * PURPOSE:
 *      The function is used to register gatt server APP.
 * INPUT:
 *      uuid               -- app uuid
 * OUTPUT:
 *      None
 * RETURN:
 *      BT_SUCCESS                        -- Operate success.
 *      BT_ERR_STATUS_FAIL                -- Operate fail.
 *      BT_ERR_STATUS_PARM_INVALID        -- paramter is invalid.
 * NOTES:
 *      None
 */
EXPORT_SYMBOL INT32 c_btm_bt_gatts_register_server(CHAR *uuid)
{
    //FUNC_ENTRY;
    return bluetooth_gatts_register_server(uuid);
}

/**
 * FUNCTION NAME: c_btm_bt_gatts_unregister_server
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
EXPORT_SYMBOL INT32 c_btm_bt_gatts_unregister_server(INT32 server_if)
{
    //FUNC_ENTRY;
    return bluetooth_gatts_unregister_server(server_if);
}

/**
 * FUNCTION NAME: c_btm_bt_gattc_open
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
EXPORT_SYMBOL INT32 c_btm_bt_gatts_open(INT32 server_if, CHAR *bt_addr, UINT8 is_direct, INT32 transport)
{
    //FUNC_ENTRY;
    return bluetooth_gatts_open(server_if, bt_addr, is_direct, transport);
}

/**
 * FUNCTION NAME: c_btm_bt_gatts_close
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
EXPORT_SYMBOL INT32 c_btm_bt_gatts_close(INT32 server_if, CHAR *bt_addr, INT32 conn_id)
{
    //FUNC_ENTRY;
    return bluetooth_gatts_close(server_if, bt_addr, conn_id);
}

/**
 * FUNCTION NAME: c_btm_bt_gatts_gatts_add_service_sync
 * PURPOSE:
 *      The function is used for gatt server synchronous add service to database.
 * INPUT:
 *      server_if              -- registered app identifer
 *      service_uuid           -- service uuid
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
EXPORT_SYMBOL INT32 c_btm_bt_gatts_gatts_add_service_sync(INT32 server_if, CHAR *service_uuid,
        UINT8 is_primary,
        INT32 number, INT32 *srvc_handle)
{
    //FUNC_ENTRY;
    return bluetooth_gatts_add_service_sync(server_if, service_uuid, is_primary, number, srvc_handle);

}

/**
 * FUNCTION NAME: c_btm_bt_gatts_add_service
 * PURPOSE:
 *      The function is used for gatt server add service to database.
 * INPUT:
 *      server_if              -- registered app identifer
 *      service_uuid           -- service uuid
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
EXPORT_SYMBOL INT32 c_btm_bt_gatts_add_service(INT32 server_if,
                                 CHAR *service_uuid,
                                 UINT8 is_primary,
                                 INT32 number)
{
    //FUNC_ENTRY;
    return bluetooth_gatts_add_service(server_if, service_uuid, is_primary, number);
}

/**
 * FUNCTION NAME: c_btm_bt_gatts_add_included_service
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
EXPORT_SYMBOL INT32 c_btm_bt_gatts_add_included_service(INT32 server_if,
        INT32 service_handle,
        INT32 included_handle)
{
    //FUNC_ENTRY;
    return bluetooth_gatts_add_included_service(server_if, service_handle, included_handle);

}

/**
 * FUNCTION NAME: c_btm_bt_gatts_add_char_sync
 * PURPOSE:
 *      The function is used for gatt server synchronous add characteristic to database.
 * INPUT:
 *      server_if              -- registered app identifer
 *      service_handle         -- added service start handle
 *      char_uuid              -- characteristic uuid
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
EXPORT_SYMBOL INT32 c_btm_bt_gatts_add_char_sync(INT32 server_if, INT32 service_handle, CHAR *char_uuid,
                                   INT32 properties, INT32 permissions, INT32 *char_handle)
{
    //FUNC_ENTRY;
    return bluetooth_gatts_add_char_sync(server_if, service_handle, char_uuid, properties, permissions, char_handle);

}

/**
 * FUNCTION NAME: c_btm_bt_gatts_add_char
 * PURPOSE:
 *      The function is used for gatt server add characteristic to database.
 * INPUT:
 *      server_if              -- registered app identifer
 *      service_handle         -- added service start handle
 *      char_uuid              -- characteristic uuid
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
EXPORT_SYMBOL INT32 c_btm_bt_gatts_add_char(INT32 server_if,
                              INT32 service_handle,
                              CHAR *uuid,
                              INT32 properties,
                              INT32 permissions)
{
    //FUNC_ENTRY;
    return bluetooth_gatts_add_char(server_if, service_handle, uuid, properties, permissions);
}

/**
 * FUNCTION NAME: c_btm_bt_gatts_add_desc
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
EXPORT_SYMBOL INT32 c_btm_bt_gatts_add_desc(INT32 server_if,
                              INT32 service_handle,
                              CHAR *uuid,
                              INT32 permissions)
{
    //FUNC_ENTRY;
    return bluetooth_gatts_add_desc(server_if, service_handle, uuid, permissions);
}

/**
 * FUNCTION NAME: c_btm_bt_gatts_start_service
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
EXPORT_SYMBOL INT32 c_btm_bt_gatts_start_service(INT32 server_if, INT32 service_handle, INT32 transport)
{
    //FUNC_ENTRY;
    return bluetooth_gatts_start_service(server_if, service_handle, transport);
}

/**
 * FUNCTION NAME: c_btm_bt_gatts_start_service_sync
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
EXPORT_SYMBOL INT32 c_btm_bt_gatts_start_service_sync(INT32 server_if, INT32 service_handle, INT32 transport)
{
    //FUNC_ENTRY;
    return bluetooth_gatts_start_service_sync(server_if, service_handle, transport);
}


/**
 * FUNCTION NAME: c_btm_bt_gatts_stop_service
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
EXPORT_SYMBOL INT32 c_btm_bt_gatts_stop_service(INT32 server_if, INT32 service_handle)
{
    //FUNC_ENTRY;
    return bluetooth_gatts_stop_service(server_if, service_handle);
}

/**
 * FUNCTION NAME: c_btm_bt_gatts_delete_service
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
EXPORT_SYMBOL INT32 c_btm_bt_gatts_delete_service(INT32 server_if, INT32 service_handle)
{
    //FUNC_ENTRY;
    return bluetooth_gatts_delete_service(server_if, service_handle);
}

/**
 * FUNCTION NAME: c_btm_bt_gatts_send_indication
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
EXPORT_SYMBOL INT32 c_btm_bt_gatts_send_indication(INT32 server_if,
                                     INT32 attribute_handle,
                                     INT32 conn_id,
                                     INT32 fg_confirm,
                                     CHAR *p_value,
                                     INT32 value_len)
{
    //FUNC_ENTRY;
    return bluetooth_gatts_send_indication(server_if, attribute_handle, conn_id, fg_confirm, p_value, value_len);
}

/**
 * FUNCTION NAME: c_btm_bt_gatts_send_response
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
EXPORT_SYMBOL INT32 c_btm_bt_gatts_send_response(INT32 conn_id,
                                   INT32 trans_id,
                                   INT32 status,
                                   INT32 handle,
                                   CHAR *p_value,
                                   INT32 value_len,
                                   INT32 auth_req)
{
    //FUNC_ENTRY;
    return bluetooth_gatts_send_response(conn_id, trans_id, status, handle, p_value, value_len,auth_req);
}

/**
 * FUNCTION NAME: c_btm_bt_gatt_parse_adv_data
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
EXPORT_SYMBOL VOID c_btm_bt_gatt_parse_adv_data (UINT8 *adv_data)
{
    //FUNC_ENTRY;
    bluetooth_gatt_parse_adv_data(adv_data);
}

/*GATTS after receive cbk notify, APP can call following API to get information*/

/**
 * FUNCTION NAME: c_btm_bt_gatts_get_connect_result_info
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
EXPORT_SYMBOL VOID c_btm_bt_gatts_get_connect_result_info(BT_GATTS_CONNECT_RST_T *connect_rst_info)
{
    //FUNC_ENTRY;
    bluetooth_gatts_get_connect_result_info(connect_rst_info);
}

/**
 * FUNCTION NAME: c_btm_bt_gatts_get_disconnect_result_info
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
EXPORT_SYMBOL VOID c_btm_bt_gatts_get_disconnect_result_info(BT_GATTS_CONNECT_RST_T *disconnect_rst_info)
{
    //FUNC_ENTRY;
    bluetooth_gatts_get_disconnect_result_info(disconnect_rst_info);
}

#if MTK_BLE_GGL_SETUP_SUPPORT
extern INT32 ascii_2_hex(CHAR *p_ascii, INT32 len, UINT8 *p_hex);

static c4tv_setup_info_t *c4tv_setup_info = NULL;
static c4tv_uuid_handle_t *c4tv_uuid_handle = NULL;

static CHAR * p_deviceName = NULL;
static BOOL adv_transmit_name = 0;
static BOOL set_adv_flag = 0;
static BOOL scan_transmit_name = 0;
static BOOL set_scan_flag = 0;
static a_mtkapi_bt_gatt_nfy_fct_cbk bt_gatt_nfy_fct_cbk = NULL;
static VOID * bt_gatt_nfy_pv_tag = NULL;
static a_mtkapi_bt_gatt_evt_fct_cbk bt_gatt_evt_fct_cbk = NULL;
static VOID * bt_gatt_evt_pv_tag = NULL;
INT32 reg_flag = 0;
static INT32 setup_flag = 0;
static INT32 btm_client_if = -1;
static INT32 btm_server_if = -1;
static INT32 err_log_flag = 0;
//static CHAR *read_attr_value[MAX_C4TV_SETUP_INFO];
/*static HANDLE_T gatt_sema_data_buf = NULL_HANDLE;*/
pthread_mutex_t gatt_sema_data_buf;
static INT32 EXCHANGE_MTU = 0;
static INT32 prepare_setup_index = 0;
extern BtAppGATTGetDeviceNameCbk BtGattGetDeviceNameCbk;

#include <time.h>
#include <sys/time.h>
#define CC_TEMP \
do \
{   \
    struct timeval now; \
    gettimeofday(&now, NULL);  \
    BT_DBG_ERROR(BT_DEBUG_GATT, "cccccccc: %s: %d: tv_sec = %ld, tv_usec = %ld ", __func__, __LINE__, (long)now.tv_sec, (long)now.tv_usec);    \
}while(0)

INT32 c_btm_gatt_sema_create(VOID)
{
    INT32 i4_ret = 0;

    /*if (NULL_HANDLE == gatt_sema_data_buf)
    {
        i4_ret = c_sema_create(&gatt_sema_data_buf,
                               X_SEMA_TYPE_BINARY,
                               X_SEMA_STATE_UNLOCK);
        if (i4_ret != OSR_OK)
        {
            if (err_log_flag < 30)
            {
                BT_DBG_ERROR(BT_DEBUG_GATT, "ERR: create gatt buf sema fail, %d", i4_ret);
            }
        }
    }*/
    if (0 != pthread_mutex_init(&gatt_sema_data_buf, NULL))
    {
        BT_DBG_ERROR(BT_DEBUG_GATT, "ERR: create gatt buf sema fail, %d", i4_ret);
    }

    return i4_ret;
}

VOID c_btm_gatt_data_buf_lock(VOID)
{
    INT32 i4_ret = 0;

    /*if (NULL_HANDLE == gatt_sema_data_buf)
    {
        if (err_log_flag < 30)
        {
            BT_DBG_ERROR(BT_DEBUG_GATT, "ERR: has not create gatt buf sema");
        }
        return;
    }

    i4_ret = c_sema_lock(gatt_sema_data_buf, X_SEMA_OPTION_WAIT);*/
    i4_ret = pthread_mutex_lock(&gatt_sema_data_buf);
    if (i4_ret != 0)
    {
        if (err_log_flag < 30)
        {
            BT_DBG_ERROR(BT_DEBUG_GATT, "ERR: lock gatt buf sema fail, %d", i4_ret);
        }
    }
}

VOID c_btm_gatt_data_buf_unlock(VOID)
{
    INT32 i4_ret = 0;

    /*if (NULL_HANDLE == gatt_sema_data_buf)
    {
        if (err_log_flag < 30)
        {
            BT_DBG_ERROR(BT_DEBUG_GATT, "ERR: has not create gatt buf sema");
        }
        return;
    }

    i4_ret = c_sema_unlock(gatt_sema_data_buf);*/
    i4_ret = pthread_mutex_unlock(&gatt_sema_data_buf);
    if (i4_ret != 0)
    {
        if (err_log_flag < 30)
        {
            BT_DBG_ERROR(BT_DEBUG_GATT, "ERR: unlock gatt buf sema fail, %d", i4_ret);
        }
    }
}

INT32 c_btm_gatt_sema_delete(VOID)
{
    INT32 i4_ret = 0;

    /*if (NULL_HANDLE != gatt_sema_data_buf)
    {
        i4_ret = c_sema_delete(gatt_sema_data_buf);
        if (i4_ret != OSR_OK)
        {
            if (err_log_flag < 30)
            {
                BT_DBG_ERROR(BT_DEBUG_GATT, "ERR: delete gatt buf sema fail, %d", i4_ret);
            }
        }
    }*/
    pthread_mutex_destroy(&gatt_sema_data_buf);
    if (i4_ret != 0)
    {
        if (err_log_flag < 30)
        {
            BT_DBG_ERROR(BT_DEBUG_GATT, "ERR: delete gatt buf sema fail, %d", i4_ret);
        }
    }

    return i4_ret;
}


/**
 * FUNCTION NAME: c_btm_gatt_findSetupInfo
 * PURPOSE:
 *      The function is used to find setup information index according to setup info.
 * INPUT:
 *      info                 -- setup information
 * OUTPUT:
 *      None
 * RETURN:
 *      MAX_C4TV_SETUP_INFO  -- not found.
 *      index                -- setup information index.
 * NOTES:
 *      None
 */
static UINT32 c_btm_gatt_findSetupInfo(c4tv_setup_info_t * info)
{
    //FUNC_ENTRY;
    UINT32 index = 0;

    if (NULL == info)
    {
        BT_DBG_ERROR(BT_DEBUG_GATT, "[GATTS] info is NULL");
        return MAX_C4TV_SETUP_INFO;
    }

    if (NULL == c4tv_setup_info)
    {
        if (err_log_flag < 30)
        {
            BT_DBG_ERROR(BT_DEBUG_GATT, "[GATTS] c4tv_setup_info is NULL");
        }
        return MAX_C4TV_SETUP_INFO;
    }

    switch (info->type)
    {
    case C4TV_SETUP_INFO_TYPE_E_ADV:
        for (index = 0; index < MAX_C4TV_SETUP_INFO; index++)
        {
            if ((C4TV_SETUP_INFO_TYPE_E_ADV == c4tv_setup_info[index].type)
                    && (0 == memcmp(c4tv_setup_info[index].u.adv.bt_service_uuid, info->u.adv.bt_service_uuid, BT_GATT_MAX_UUID_LEN))
                    && (0 == memcmp(c4tv_setup_info[index].u.adv.bt_advertised_uuid, info->u.adv.bt_advertised_uuid, BT_GATT_MAX_UUID_LEN)))
            {
                break;
            }
        }
        break;
    case C4TV_SETUP_INFO_TYPE_E_SCAN:
        for (index = 0; index < MAX_C4TV_SETUP_INFO; index++)
        {
            if ((C4TV_SETUP_INFO_TYPE_E_SCAN == c4tv_setup_info[index].type)
                    && (0 == memcmp(c4tv_setup_info[index].u.scan.bt_service_uuid, info->u.scan.bt_service_uuid, BT_GATT_MAX_UUID_LEN))
                    && (0 == memcmp(c4tv_setup_info[index].u.scan.bt_advertised_uuid, info->u.scan.bt_advertised_uuid, BT_GATT_MAX_UUID_LEN)))
            {
                break;
            }
        }
        break;
    case C4TV_SETUP_INFO_TYPE_E_CHAR:
        for (index = 0; index < MAX_C4TV_SETUP_INFO; index++)
        {
            if ((C4TV_SETUP_INFO_TYPE_E_CHAR == c4tv_setup_info[index].type)
//                    && (0 == memcmp(c4tv_setup_info[index].u.attr.bt_service_uuid, info->u.attr.bt_service_uuid, BT_GATT_MAX_UUID_LEN))
                    && (0 == memcmp(c4tv_setup_info[index].u.attr.bt_attribute_uuid, info->u.attr.bt_attribute_uuid, BT_GATT_MAX_UUID_LEN)))
            {
                break;
            }
        }
        break;
    default:
        BT_DBG_ERROR(BT_DEBUG_GATT, "[GATTS] type error:%d", info->type);
        return MAX_C4TV_SETUP_INFO;
    }
    return index;
}

/**
 * FUNCTION NAME: c_btm_gatt_findSetupInfoByCtrlUuid
 * PURPOSE:
 *      The function is used to find setup information index according to control uuid.
 * INPUT:
 *      info                 -- setup information with control uuid
 * OUTPUT:
 *      None
 * RETURN:
 *      MAX_C4TV_SETUP_INFO  -- not found.
 *      index                -- setup information index.
 * NOTES:
 *      None
 */
static UINT32 c_btm_gatt_findSetupInfoByCtrlUuid(c4tv_setup_info_t * info)
{
    //FUNC_ENTRY;
    UINT32 index = 0;

    if (NULL == info)
    {
        BT_DBG_ERROR(BT_DEBUG_GATT, "[GATTS] info is NULL");
        return MAX_C4TV_SETUP_INFO;
    }

    if (NULL == c4tv_setup_info)
    {
        BT_DBG_ERROR(BT_DEBUG_GATT, "[GATTS] c4tv_setup_info is NULL");
        return MAX_C4TV_SETUP_INFO;
    }

    switch (info->type)
    {
    case C4TV_SETUP_INFO_TYPE_E_CHAR:
        for (index = 0; index < MAX_C4TV_SETUP_INFO; index++)
        {
            if ((C4TV_SETUP_INFO_TYPE_E_CHAR == c4tv_setup_info[index].type)
                    && (0 == memcmp(c4tv_setup_info[index].u.attr.bt_control_uuid, info->u.attr.bt_control_uuid, BT_GATT_MAX_UUID_LEN)))
            {
                break;
            }
        }
        break;
    default:
        BT_DBG_ERROR(BT_DEBUG_GATT, "[GATTS] type error:%d", info->type);
        return MAX_C4TV_SETUP_INFO;
    }
    return index;
}

/**
 * FUNCTION NAME: c_btm_gatt_allocNewSetupInfo
 * PURPOSE:
 *      The function is used to allocate a new index for setup information.
 * INPUT:
 *      None
 * OUTPUT:
 *      None
 * RETURN:
 *      MAX_C4TV_SETUP_INFO  -- not found.
 *      index                -- setup information index.
 * NOTES:
 *      None
 */
static UINT32 c_btm_gatt_allocNewSetupInfo()
{
    //FUNC_ENTRY;
    UINT32 index = 0;

    if (NULL == c4tv_setup_info)
    {
        BT_DBG_ERROR(BT_DEBUG_GATT, "[GATTS] c4tv_setup_info is NULL");
        return MAX_C4TV_SETUP_INFO;
    }

    for (index = 0; index < MAX_C4TV_SETUP_INFO; index++)
    {
        if (C4TV_SETUP_INFO_TYPE_E_NULL == c4tv_setup_info[index].type)
        {
            break;
        }
    }
    return index;
}

/**
 * FUNCTION NAME: c_btm_gatt_findUuidByHandle
 * PURPOSE:
 *      The function is used to find uuid index according to uuid handle.
 * INPUT:
 *      info                 -- uuid and uuid handle mapping information
 * OUTPUT:
 *      None
 * RETURN:
 *      MAX_C4TV_UUID_HANDLE  -- not found.
 *      index                 -- uuid and uuid handle mapping information index.
 * NOTES:
 *      None
 */
static UINT32 c_btm_gatt_findUuidByHandle(c4tv_uuid_handle_t * info)
{
    //FUNC_ENTRY;
    UINT32 index = 0;

    if (NULL == info)
    {
        BT_DBG_ERROR(BT_DEBUG_GATT, "[GATTS] info is NULL");
        return MAX_C4TV_UUID_HANDLE;
    }

    if (NULL == c4tv_uuid_handle)
    {
        BT_DBG_ERROR(BT_DEBUG_GATT, "[GATTS] c4tv_uuid_handle is NULL");
        return MAX_C4TV_SETUP_INFO;
    }

    for (index = 0; index < MAX_C4TV_UUID_HANDLE; index++)
    {
        if ((info->type == c4tv_uuid_handle[index].type)
                && (info->handle == c4tv_uuid_handle[index].handle))
        {
            break;
        }
    }

    return index;
}
/**
 * FUNCTION NAME: c_btm_gatt_findHandleByUuid
 * PURPOSE:
 *      The function is used to find uuid index according to uuid.
 * INPUT:
 *      info                 -- uuid and uuid handle mapping information
 * OUTPUT:
 *      None
 * RETURN:
 *      MAX_C4TV_UUID_HANDLE  -- not found.
 *      index                 -- uuid and uuid handle mapping information index.
 * NOTES:
 *      None
 */
static UINT32 c_btm_gatt_findHandleByUuid(c4tv_uuid_handle_t * info)
{
    //FUNC_ENTRY;
    UINT32 index = 0;

    if (NULL == info)
    {
        BT_DBG_ERROR(BT_DEBUG_GATT, "[GATTS] info is NULL");
        return MAX_C4TV_UUID_HANDLE;
    }

    if (NULL == c4tv_uuid_handle)
    {
        BT_DBG_ERROR(BT_DEBUG_GATT, "[GATTS] c4tv_uuid_handle is NULL");
        return MAX_C4TV_SETUP_INFO;
    }

    for (index = 0; index < MAX_C4TV_UUID_HANDLE; index++)
    {
        if ((info->type == c4tv_uuid_handle[index].type)
                && (0 == memcmp(info->uuid, c4tv_uuid_handle[index].uuid, BT_GATT_MAX_UUID_LEN)))
        {
            break;
        }
    }

    return index;
}

/**
 * FUNCTION NAME: c_btm_gatt_allocNewUuidHandle
 * PURPOSE:
 *      The function is used to allocate a new index for uuid and handle mapping info.
 * INPUT:
 *      None
 * OUTPUT:
 *      None
 * RETURN:
 *      MAX_C4TV_UUID_HANDLE  -- not found.
 *      index                 -- uuid and uuid handle mapping information index.
 * NOTES:
 *      None
 */
static UINT32 c_btm_gatt_allocNewUuidHandle()
{
    //FUNC_ENTRY;
    UINT32 index = 0;

    if (NULL == c4tv_uuid_handle)
    {
        BT_DBG_ERROR(BT_DEBUG_GATT, "[GATTS] c4tv_uuid_handle is NULL");
        return MAX_C4TV_UUID_HANDLE;
    }

    for (index = 0; index < MAX_C4TV_UUID_HANDLE; index++)
    {
        if (C4TV_UUID_HANDLE_TYPE_E_NULL == c4tv_uuid_handle[index].type)
        {
            break;
        }
    }
    return index;
}

/**
 * FUNCTION NAME: c_btm_gatt_reg_client_cbk
 * PURPOSE:
 *      The function is callback function of gatt client register app
 * INPUT:
 *      bt_gatt_event       -- registered clent app information
 * OUTPUT:
 *      None
 * RETURN:
 *      None
 * NOTES:
 *      None
 */
static VOID c_btm_gatt_reg_client_cbk(BT_GATTC_REG_CLIENT_T *bt_gatt_event)
{
    //FUNC_ENTRY;
    if (NULL == bt_gatt_event)
    {
        BT_DBG_ERROR(BT_DEBUG_GATT, "bt_gatt_event is NULL");
        return;
    }
    btm_client_if = bt_gatt_event->client_if;
    reg_flag |= 1 << 0;
    if (0x3 == (reg_flag & 0x3))
    {
        if (NULL != bt_gatt_evt_fct_cbk)
        {
            bt_gatt_evt_fct_cbk(1, 0, bt_gatt_evt_pv_tag);
        }
        else
        {
            BT_DBG_ERROR(BT_DEBUG_GATT, "[GATT] null pointer of bt_gatt_evt_fct_cbk");
        }
    }
    else
    {
        BT_DBG_INFO(BT_DEBUG_GATT, "[GATT] reg_flag = %ld", (long)reg_flag);
    }

    return;
}

/**
 * FUNCTION NAME: c_btm_gatt_reg_server_cbk
 * PURPOSE:
 *      The function is callback function of gatt server register app
 * INPUT:
 *      bt_gatts_reg_server        -- registered server app information
 * OUTPUT:
 *      None
 * RETURN:
 *      None
 * NOTES:
 *      None
 */
static VOID c_btm_gatt_reg_server_cbk(BT_GATTS_REG_SERVER_RST_T *bt_gatts_reg_server)
{
    //FUNC_ENTRY;
    if (NULL == bt_gatts_reg_server)
    {
        BT_DBG_ERROR(BT_DEBUG_GATT, "bt_gatt_event is NULL");
        return;
    }
    btm_server_if = bt_gatts_reg_server->server_if;
    reg_flag |= 1 << 1;
    if (0x3 == (reg_flag & 0x3))
    {
        if (NULL != bt_gatt_evt_fct_cbk)
        {
            bt_gatt_evt_fct_cbk(1, 0, bt_gatt_evt_pv_tag);
        }
        else
        {
            BT_DBG_ERROR(BT_DEBUG_GATT, "[GATT] null pointer of bt_gatt_evt_fct_cbk");
        }
    }
    else
    {
        BT_DBG_INFO(BT_DEBUG_GATT, "[GATT] reg_flag = %ld", (long)reg_flag);
    }

    return;
}

/**
 * FUNCTION NAME: c_btm_gatt_add_srvc_cbk
 * PURPOSE:
 *      The function is callback function of gatt server add service
 * INPUT:
 *      bt_gatts_add_srvc   -- added service information
 * OUTPUT:
 *      None
 * RETURN:
 *      None
 * NOTES:
 *      None
 */
static VOID c_btm_gatt_add_srvc_cbk(BT_GATTS_ADD_SRVC_RST_T *bt_gatts_add_srvc)
{
    //FUNC_ENTRY;
    UINT32 index = 0;
    c4tv_uuid_handle_t info;
    if (NULL == bt_gatts_add_srvc)
    {
        BT_DBG_ERROR(BT_DEBUG_GATT, "bt_gatt_event is NULL");
        return;
    }

    info.type = C4TV_UUID_HANDLE_TYPE_E_SRVC;
    info.handle = bt_gatts_add_srvc->srvc_handle;

    index = c_btm_gatt_findUuidByHandle(&info);
    if (MAX_C4TV_UUID_HANDLE == index)
    {
        index = c_btm_gatt_allocNewUuidHandle();
        if (MAX_C4TV_UUID_HANDLE == index)
        {
            BT_DBG_ERROR(BT_DEBUG_GATT, "[GATTS] add service failed");
            return;
        }
    }

    c4tv_uuid_handle[index].type = C4TV_UUID_HANDLE_TYPE_E_SRVC;
    c4tv_uuid_handle[index].handle = bt_gatts_add_srvc->srvc_handle;
    memcpy(c4tv_uuid_handle[index].uuid, bt_gatts_add_srvc->srvc_id.id.uuid, BT_GATT_MAX_UUID_LEN);

    return;
}

/**
 * FUNCTION NAME: c_btm_gatt_add_char_cbk
 * PURPOSE:
 *      The function is callback function of gatt server add characteristic
 * INPUT:
 *      bt_gatts_add_char    -- added characteristic information
 * OUTPUT:
 *      None
 * RETURN:
 *      None
 * NOTES:
 *      None
 */
static VOID c_btm_gatt_add_char_cbk(BT_GATTS_ADD_CHAR_RST_T *bt_gatts_add_char)
{
    //FUNC_ENTRY;
    UINT32 index = 0;
    c4tv_uuid_handle_t info;

    if (NULL == bt_gatts_add_char)
    {
        BT_DBG_ERROR(BT_DEBUG_GATT, "bt_gatt_event is NULL");
        return;
    }

    info.type = C4TV_UUID_HANDLE_TYPE_E_CHAR;
    memcpy(info.uuid, bt_gatts_add_char->uuid, BT_GATT_MAX_UUID_LEN);

    index = c_btm_gatt_findHandleByUuid(&info);
    if (MAX_C4TV_UUID_HANDLE == index)
    {
        BT_DBG_ERROR(BT_DEBUG_GATT, "[GATTS] find char failed");
        return;
    }

    c4tv_uuid_handle[index].type = C4TV_UUID_HANDLE_TYPE_E_CHAR;
    c4tv_uuid_handle[index].handle = bt_gatts_add_char->char_handle;
    memcpy(c4tv_uuid_handle[index].uuid, bt_gatts_add_char->uuid, BT_GATT_MAX_UUID_LEN);

    return;
}

/**
 * FUNCTION NAME: c_btm_gatt_req_read_cbk
 * PURPOSE:
 *      The function is callback function of gatt server request read
 * INPUT:
 *      bt_gatts_read      -- request read information
 * OUTPUT:
 *      None
 * RETURN:
 *      None
 * NOTES:
 *      None
 */
static VOID c_btm_gatt_req_read_cbk(BT_GATTS_REQ_READ_RST_T *bt_gatts_read)
{
    //FUNC_ENTRY;
    UINT32 uuid_handle_index = 0;
    UINT32 setup_index = 0;
    c4tv_uuid_handle_t uuid_handle;
    c4tv_setup_info_t setup;
    memset(&setup, 0, sizeof(c4tv_setup_info_t));
    INT32 length = 0;
    CHAR * p_value = NULL;
    INT32 i4_loop = 300;
    INT32 i4_loop_handle = 300;

    if (NULL == bt_gatts_read)
    {
        BT_DBG_ERROR(BT_DEBUG_GATT, "bt_gatts_read is NULL");
        return;
    }

    uuid_handle.type = C4TV_UUID_HANDLE_TYPE_E_CHAR;
    uuid_handle.handle = bt_gatts_read->attr_handle;

    uuid_handle_index = c_btm_gatt_findUuidByHandle(&uuid_handle);
    if (MAX_C4TV_UUID_HANDLE == uuid_handle_index)
    {
        BT_DBG_ERROR(BT_DEBUG_GATT, "[GATTS] find char failed, handle %ld", (long)bt_gatts_read->attr_handle);
        return;
    }
    setup.type = C4TV_SETUP_INFO_TYPE_E_CHAR;
    memcpy(setup.u.attr.bt_attribute_uuid, c4tv_uuid_handle[uuid_handle_index].uuid, BT_GATT_MAX_UUID_LEN);
    setup_index = c_btm_gatt_findSetupInfo(&setup);
    if (MAX_C4TV_SETUP_INFO == setup_index)
    {
        while (0 < i4_loop_handle)
        {
            BT_DBG_ERROR(BT_DEBUG_GATT, "[GATTS] hanlde is invaild, need wait!");
            usleep(10*1000);
            i4_loop_handle--;
            memcpy(setup.u.attr.bt_attribute_uuid, c4tv_uuid_handle[uuid_handle_index].uuid, BT_GATT_MAX_UUID_LEN);
            setup_index = c_btm_gatt_findSetupInfo(&setup);
            if (MAX_C4TV_SETUP_INFO != setup_index)
            {
                break;
            }
            BT_DBG_INFO(BT_DEBUG_GATT, "c_btm_gatt_req_read_cbk setup_index =%d, i4_loop = %d", setup_index, i4_loop_handle);
        }
    }
    if (MAX_C4TV_SETUP_INFO == setup_index)
    {
        BT_DBG_ERROR(BT_DEBUG_GATT, "[GATTS] find value failed");
        return;
    }

    if (c4tv_setup_info[setup_index].u.attr.value_len == 0)
    {
        while (0 < i4_loop)
        {
            BT_DBG_ERROR(BT_DEBUG_GATT, "[GATTS] value is null, need wait!");
            usleep(10*1000);
            i4_loop--;
            if (c4tv_setup_info[setup_index].u.attr.value_len != 0)
            {
                break;
            }
            BT_DBG_INFO(BT_DEBUG_GATT, "c_btm_gatt_req_read_cbk setup_index =%d, data_len = %d, i4_loop = %d",
            setup_index, c4tv_setup_info[setup_index].u.attr.value_len, i4_loop);
        }
    }
    if (c4tv_setup_info[setup_index].u.attr.value_len == 0)
    {
        length = 0;
        p_value = NULL;
        BT_DBG_ERROR(BT_DEBUG_GATT, "[GATTS] value is NULL, c4tv_setup_info[%d].u.attr.value_len = %ld",
                     setup_index, (long)c4tv_setup_info[setup_index].u.attr.value_len);
        BT_DBG_ERROR(BT_DEBUG_GATT, "c4tv_setup_info[%d].u.attr.value = %p",
                     setup_index, c4tv_setup_info[setup_index].u.attr.value);
        return;
    }
    else
    {
        if (c4tv_setup_info[setup_index].u.attr.value_len < (c4tv_setup_info[setup_index].u.attr.control_index * CONTROL_UUID_BLOCK_SIZE))
        {
            BT_DBG_INFO(BT_DEBUG_GATT, "[GATTS] control_index is invalid, index = %ld. length = %ld",
                        (long)c4tv_setup_info[setup_index].u.attr.control_index,
                        (long)c4tv_setup_info[setup_index].u.attr.value_len);
            //c4tv_setup_info[setup_index].u.attr.control_index = 0;
        }
        if ((0 == c4tv_setup_info[setup_index].u.attr.control_index) && (0 == bt_gatts_read->offset))
        {
            //CC_TEMP;
            c_btm_gatt_data_buf_lock();
            memcpy(c4tv_setup_info[setup_index].u.attr.read_value, c4tv_setup_info[setup_index].u.attr.value, c4tv_setup_info[setup_index].u.attr.value_len);
            c4tv_setup_info[setup_index].u.attr.read_value_len = c4tv_setup_info[setup_index].u.attr.value_len;
            c_btm_gatt_data_buf_unlock();
        }
#if 0
        if ( c4tv_uuid_handle[uuid_handle_index].handle == 288)
        {
            BT_DBG_ERROR(BT_DEBUG_GATT, "c4tv_uuid_handle[%d].uuid = %s, read_value_len = %d",
                         uuid_handle_index, c4tv_uuid_handle[uuid_handle_index].uuid, c4tv_setup_info[setup_index].u.attr.read_value_len);
        }
#endif
        //BT_DBG_ERROR(BT_DEBUG_GATT, "[GATT] attribute_uuid = %s, handle = %d",
        //            c4tv_setup_info[setup_index].u.attr.attribute_uuid, uuid_handle.handle);

        //value = c4tv_setup_info[setup_index].u.attr.value + c4tv_setup_info[setup_index].u.attr.control_index * CONTROL_UUID_BLOCK_SIZE;
        if ((c4tv_setup_info[setup_index].u.attr.control_index) * CONTROL_UUID_BLOCK_SIZE > c4tv_setup_info[setup_index].u.attr.read_value_len)
        {
            length = 0;
            BT_DBG_ERROR(BT_DEBUG_GATT, "for IOS!");
        }
        else
        {
            p_value = c4tv_setup_info[setup_index].u.attr.read_value + c4tv_setup_info[setup_index].u.attr.control_index * CONTROL_UUID_BLOCK_SIZE;
            if (0 == bt_gatts_read->offset)
            {
                if (((c4tv_setup_info[setup_index].u.attr.read_value_len - 1) / CONTROL_UUID_BLOCK_SIZE) != c4tv_setup_info[setup_index].u.attr.control_index)
                {
                    length = CONTROL_UUID_BLOCK_SIZE;
                    //CC_TEMP;
                }
                else
                {
                    //CC_TEMP;
                    length = c4tv_setup_info[setup_index].u.attr.read_value_len - c4tv_setup_info[setup_index].u.attr.control_index * CONTROL_UUID_BLOCK_SIZE;
                    //c_btm_gatt_data_buf_unlock();

                }
#if 0
                if (CONTROL_UUID_BLOCK_SIZE == length)
                {
                    length = CONTROL_UUID_BLOCK_SIZE - 1;
                }
#endif
                if (length >= EXCHANGE_MTU)
                {
                    length = EXCHANGE_MTU - 1;
                }
            }
            else
            {
                if (bt_gatts_read->offset >= CONTROL_UUID_BLOCK_SIZE)
                {
                    BT_DBG_ERROR(BT_DEBUG_GATT, "[GATTS] offset is invalid, offset = %ld", (long)bt_gatts_read->offset);
                    return;
                    //                bt_gatts_read->offset = 511;
                }
                p_value += bt_gatts_read->offset;
                //length = CONTROL_UUID_BLOCK_SIZE - bt_gatts_read->offset;
                if (((c4tv_setup_info[setup_index].u.attr.read_value_len - 1) / CONTROL_UUID_BLOCK_SIZE) != c4tv_setup_info[setup_index].u.attr.control_index)
                {
                    length = CONTROL_UUID_BLOCK_SIZE - bt_gatts_read->offset;
                }
                else
                {
                    length = (c4tv_setup_info[setup_index].u.attr.read_value_len -
                              c4tv_setup_info[setup_index].u.attr.control_index * CONTROL_UUID_BLOCK_SIZE) -
                             bt_gatts_read->offset;

                }

                if (length >= EXCHANGE_MTU)
                {
                    length = EXCHANGE_MTU - 1;
                }
            }
        }
    }
    if (NULL == p_value)
    {
        BT_DBG_INFO(BT_DEBUG_GATT, "[GATTS] value is NULL, c4tv_setup_info[%d].u.attr.read_value_len = %ld",
                    setup_index, (long)c4tv_setup_info[setup_index].u.attr.read_value_len);
        BT_DBG_INFO(BT_DEBUG_GATT, "c4tv_setup_info[%d].u.attr.value = %p",
                    setup_index, c4tv_setup_info[setup_index].u.attr.value);
        BT_DBG_INFO(BT_DEBUG_GATT, "c4tv_setup_info[%d].u.attr.control_index = %ld, length = %ld",
                    setup_index, (long)c4tv_setup_info[setup_index].u.attr.control_index, (long)length);
        //return;
    }
#if 0
    if ( c4tv_uuid_handle[uuid_handle_index].handle == 288)
    {
        BT_DBG_ERROR(BT_DEBUG_GATT, "c4tv_uuid_handle[%d].uuid = %s, length = %d",
                     uuid_handle_index, c4tv_uuid_handle[uuid_handle_index].uuid, length);
    }
#endif
    c_btm_bt_gatts_send_response(bt_gatts_read->conn_id,
                                 bt_gatts_read->trans_id,
                                 0,
                                 bt_gatts_read->attr_handle,
                                 p_value,
                                 length,
                                 0);
    //if ( c4tv_uuid_handle[uuid_handle_index].handle == 288)
    //{
    //	CC_TEMP;
    //}
    return;
}

/**
 * FUNCTION NAME: c_btm_gatt_req_write_cbk
 * PURPOSE:
 *      The function is callback function of gatt server request write operation
 * INPUT:
 *      bt_gatts_write       -- request write information
 * OUTPUT:
 *      None
 * RETURN:
 *      None
 * NOTES:
 *      None
 */
static VOID c_btm_gatt_req_write_cbk(BT_GATTS_REQ_WRITE_RST_T *bt_gatts_write)
{
    //FUNC_ENTRY;

    UINT32 uuid_handle_index = 0;
    UINT32 setup_index = 0;
    c4tv_uuid_handle_t uuid_handle;
    c4tv_setup_info_t setup;

    if (NULL == bt_gatts_write)
    {
        BT_DBG_ERROR(BT_DEBUG_GATT, "bt_gatts_write is NULL");
        return;
    }

    uuid_handle.type = C4TV_UUID_HANDLE_TYPE_E_CHAR;
    uuid_handle.handle = bt_gatts_write->attr_handle;

    uuid_handle_index = c_btm_gatt_findUuidByHandle(&uuid_handle);
    if (MAX_C4TV_UUID_HANDLE == uuid_handle_index)
    {
        BT_DBG_ERROR(BT_DEBUG_GATT, "[GATTS] find char failed, handle %ld", (long)bt_gatts_write->attr_handle);
        return;
    }

    setup.type = C4TV_SETUP_INFO_TYPE_E_CHAR;
    memcpy(setup.u.attr.bt_attribute_uuid, c4tv_uuid_handle[uuid_handle_index].uuid, BT_GATT_MAX_UUID_LEN);
    setup_index = c_btm_gatt_findSetupInfo(&setup);
    if (MAX_C4TV_SETUP_INFO != setup_index)
    {
        if ((0 != bt_gatts_write->length) && (NULL != bt_gatts_write->value))
        {
            if (0 == (c4tv_setup_info[setup_index].u.attr.option & 0x2))    //  write
            {
                BT_DBG_ERROR(BT_DEBUG_GATT, "[GATTS] option is not write");
                return;
            }

            if (NULL != c4tv_setup_info[setup_index].u.attr.value)
            {
                if (bt_gatts_write->length >= ATTRIBUTE_VALUE_MEM_SIZE)
                {
                    BT_DBG_ERROR(BT_DEBUG_GATT, "[GATTS] bt_gatts_write->length = %ld", (long)bt_gatts_write->length);
                    free(c4tv_setup_info[setup_index].u.attr.value);
                    c4tv_setup_info[setup_index].u.attr.value = NULL;
                    c4tv_setup_info[setup_index].u.attr.value = malloc(bt_gatts_write->length + 1);
                    memset(c4tv_setup_info[setup_index].u.attr.value, 0, bt_gatts_write->length + 1);
                    c4tv_setup_info[setup_index].u.attr.value_len = 0;
                    if (NULL == c4tv_setup_info[setup_index].u.attr.value)
                    {
                        BT_DBG_ERROR(BT_DEBUG_GATT, "[GATTS] set Characteristic Value failed");
                        return;
                    }
                }
                else if ((0 == bt_gatts_write->offset) && bt_gatts_write->is_prep)
                {
                    memset(c4tv_setup_info[setup_index].u.attr.value, 0, ATTRIBUTE_VALUE_MEM_SIZE);
                    c4tv_setup_info[setup_index].u.attr.value_len = 0;
                }
            }
            else
            {
                c4tv_setup_info[setup_index].u.attr.value = malloc(ATTRIBUTE_VALUE_MEM_SIZE);
                memset(c4tv_setup_info[setup_index].u.attr.value, 0, ATTRIBUTE_VALUE_MEM_SIZE);
                c4tv_setup_info[setup_index].u.attr.value_len = 0;
                if (NULL == c4tv_setup_info[setup_index].u.attr.value)
                {
                    BT_DBG_ERROR(BT_DEBUG_GATT, "[GATTS] set Characteristic Value failed");
                    return;
                }
            }
            if (bt_gatts_write->is_prep)
            {
                BT_DBG_INFO(BT_DEBUG_GATT, "offset = %d, value_len = %d, value = %s",bt_gatts_write->offset,
                    c4tv_setup_info[setup_index].u.attr.value_len, bt_gatts_write->value);
                memcpy(c4tv_setup_info[setup_index].u.attr.value + bt_gatts_write->offset, bt_gatts_write->value, bt_gatts_write->length);
                //c4tv_setup_info[setup_index].u.attr.value[bt_gatts_write->length] = '\0';
                c4tv_setup_info[setup_index].u.attr.value_len += bt_gatts_write->length;
                c4tv_setup_info[setup_index].u.attr.control_index = 0;
                prepare_setup_index = setup_index;
                c_btm_bt_gatts_send_response(bt_gatts_write->conn_id,  bt_gatts_write->trans_id, 0, bt_gatts_write->attr_handle,
                                             (CHAR *)bt_gatts_write->value, bt_gatts_write->length, 0);
            }
            else
            {
                memcpy(c4tv_setup_info[setup_index].u.attr.value, bt_gatts_write->value, bt_gatts_write->length);
                c4tv_setup_info[setup_index].u.attr.value[bt_gatts_write->length] = '\0';
                c4tv_setup_info[setup_index].u.attr.value_len = bt_gatts_write->length;
                c4tv_setup_info[setup_index].u.attr.control_index = 0;
                c_btm_bt_gatts_send_response(bt_gatts_write->conn_id,  bt_gatts_write->trans_id, 0, bt_gatts_write->attr_handle,
                                             NULL, 0, 0);
            }

        }
        else if (0 == bt_gatts_write->length)
        {
            BT_DBG_INFO(BT_DEBUG_GATT, "c4tv_setup_info[setup_index].u.attr.value = %p",
                        c4tv_setup_info[setup_index].u.attr.value);
            if (0 == (c4tv_setup_info[setup_index].u.attr.option & 0x2))    //  write
            {
                BT_DBG_ERROR(BT_DEBUG_GATT, "[GATTS] option is not write");
                return;
            }

            c4tv_setup_info[setup_index].u.attr.value = malloc(bt_gatts_write->length + 2);
            if (NULL == c4tv_setup_info[setup_index].u.attr.value)
            {
                BT_DBG_ERROR(BT_DEBUG_GATT, "[GATTS] set Characteristic Value failed");
                return;
            }

            //memcpy(c4tv_setup_info[setup_index].u.attr.value, bt_gatts_write->value, bt_gatts_write->length);
            c4tv_setup_info[setup_index].u.attr.value[0] = 0x01;
            c4tv_setup_info[setup_index].u.attr.value[1] = '\0';
            c4tv_setup_info[setup_index].u.attr.value_len = 1;//bt_gatts_write->length;
            c4tv_setup_info[setup_index].u.attr.control_index = 0;
            c_btm_bt_gatts_send_response(bt_gatts_write->conn_id,  bt_gatts_write->trans_id, 0, bt_gatts_write->attr_handle,
                                             NULL, 0, 0);
        }

        if (NULL != bt_gatt_nfy_fct_cbk && 0 == bt_gatts_write->is_prep)
        {
            BT_DBG_INFO(BT_DEBUG_GATT, "[GATTS] bt_gatt_nfy_pv_tag = %p,c4tv_setup_info[setup_index].u.attr.value = %p",
                        bt_gatt_nfy_pv_tag,c4tv_setup_info[setup_index].u.attr.value);
            bt_gatt_nfy_fct_cbk(c4tv_setup_info[setup_index].u.attr.service_uuid,
                                c4tv_setup_info[setup_index].u.attr.attribute_uuid,
                                c4tv_setup_info[setup_index].u.attr.value,
                                bt_gatt_nfy_pv_tag);
        }
        else
        {
            BT_DBG_ERROR(BT_DEBUG_GATT, "[GATTS] No bt_gatt_nfy_fct_cbk callback!");
        }
    }
    else
    {
        setup.type = C4TV_SETUP_INFO_TYPE_E_CHAR;
        memcpy(setup.u.attr.bt_control_uuid, c4tv_uuid_handle[uuid_handle_index].uuid, BT_GATT_MAX_UUID_LEN);
        setup_index = c_btm_gatt_findSetupInfoByCtrlUuid(&setup);
        if (MAX_C4TV_SETUP_INFO == setup_index)
        {
            BT_DBG_ERROR(BT_DEBUG_GATT, "[GATTS] find characteristic setup info, index:%d", uuid_handle_index);
            return;
        }

        if ((0 != bt_gatts_write->length) && (NULL != bt_gatts_write->value))
        {
            c4tv_setup_info[setup_index].u.attr.control_index = 0;
            if (1 == bt_gatts_write->length)
            {
                if ((bt_gatts_write->value[0]) * CONTROL_UUID_BLOCK_SIZE > c4tv_setup_info[setup_index].u.attr.read_value_len)
                {
                    BT_DBG_INFO(BT_DEBUG_GATT, "[GATTS] set control_uuid value failed, value = %d. length = %ld, handle = %d",
                                bt_gatts_write->value[0], (long)c4tv_setup_info[setup_index].u.attr.read_value_len,
                                uuid_handle.handle);
                }
                else
                {
                    BT_DBG_INFO(BT_DEBUG_GATT, "[GATTS] value = %d", bt_gatts_write->value[0]);
                }
                c4tv_setup_info[setup_index].u.attr.control_index = bt_gatts_write->value[0];
                c_btm_bt_gatts_send_response(bt_gatts_write->conn_id,  bt_gatts_write->trans_id, 0, bt_gatts_write->attr_handle,
                                             NULL, 0, 0);
            }
            else
            {
                BT_DBG_ERROR(BT_DEBUG_GATT, "[GATTS] length = %ld", (long)bt_gatts_write->length);
            }
        }
    }

    return;
}

static VOID c_btm_gatts_connect_cbk(BT_GATTS_EVENT_T bt_gatts_event)
{
    INT32 ret = 0;

    BT_DBG_INFO(BT_DEBUG_GATT, "[GATTS] status = %d", bt_gatts_event);
    if (BT_GATTS_CONNECT == bt_gatts_event)
    {
        ret = c_btm_bt_gattc_multi_adv_disable(btm_client_if);
        if (ret != BT_SUCCESS)
        {
            BT_DBG_ERROR(BT_DEBUG_GATT, "[GATTC] disable advertisement failed %d", ret);
        }
    }
    else if (BT_GATTS_DISCONNECT == bt_gatts_event)
    {
        ret = c_btm_bt_gattc_multi_adv_enable_sync(btm_client_if, 2048, 2048, 0, 0, 1, 0);
        if (ret != BT_SUCCESS)
        {
            BT_DBG_ERROR(BT_DEBUG_GATT, "[GATTC] enable advertisement failed %d", ret);
        }
    }
}

EXPORT_SYMBOL VOID c_btm_bt_gatts_mtu_changed_cbk(INT32 conn_id, INT32 mtu)
{
    EXCHANGE_MTU = mtu;
    BT_DBG_ERROR(BT_DEBUG_GATT, "mtu = %d", EXCHANGE_MTU);
}

EXPORT_SYMBOL VOID c_btm_bt_gatts_request_exec_write_cbk(INT32 conn_id, INT32 trans_id, CHAR *bda, INT32 exec_write)
{
    if (exec_write == 1 && !MTK_LINUX_GATTC_PTS_TEST)
    {
        c4tv_setup_info[prepare_setup_index].u.attr.value[c4tv_setup_info[prepare_setup_index].u.attr.value_len] = '\0';
        BT_DBG_INFO(BT_DEBUG_GATT, "value_len = %d, value = %s",
                    c4tv_setup_info[prepare_setup_index].u.attr.value_len, c4tv_setup_info[prepare_setup_index].u.attr.value);

        if (NULL != bt_gatt_nfy_fct_cbk)
        {
            BT_DBG_INFO(BT_DEBUG_GATT, "[GATTS] bt_gatt_nfy_pv_tag = %p,c4tv_setup_info[prepare_setup_index].u.attr.value = %p",
                        bt_gatt_nfy_pv_tag,c4tv_setup_info[prepare_setup_index].u.attr.value);
            bt_gatt_nfy_fct_cbk(c4tv_setup_info[prepare_setup_index].u.attr.service_uuid,
                                c4tv_setup_info[prepare_setup_index].u.attr.attribute_uuid,
                                c4tv_setup_info[prepare_setup_index].u.attr.value,
                                bt_gatt_nfy_pv_tag);
        }
        else
        {
            BT_DBG_ERROR(BT_DEBUG_GATT, "[GATTS] No bt_gatt_nfy_fct_cbk callback!");
        }

    }
}

/**
 * FUNCTION NAME: c_btm_gatt_reg_nfy
 * PURPOSE:
 *      The function is used for gatt server to register callback for app.
 * INPUT:
 *      pf_nfy                 -- gatt write attribute value callback for app
 *      pv_tag                 -- app delegate data
 * OUTPUT:
 *      None
 * RETURN:
 *      BT_SUCCESS                        -- Operate success.
 * NOTES:
 *      None
 */
EXPORT_SYMBOL INT32 c_btm_gatt_reg_nfy(a_mtkapi_bt_gatt_nfy_fct_cbk pf_nfy, VOID * pv_tag)
{
    //FUNC_ENTRY;
    INT32 ret = BT_SUCCESS;
    BT_DBG_NOTICE(BT_DEBUG_GATT, "[GATT] pf_nfy = %p, pv_tag = %p", pf_nfy, pv_tag);
    bt_gatt_nfy_fct_cbk = pf_nfy;
    bt_gatt_nfy_pv_tag = pv_tag;
    return ret;
}

/**
 * FUNCTION NAME: c_btm_gatt_reg_evt
 * PURPOSE:
 *      Not used.
 * INPUT:
 *      pf_evt                 -- not used
 *      pv_tag                 -- app delegate data
 * OUTPUT:
 *      None
 * RETURN:
 *      BT_SUCCESS                        -- Operate success.
 * NOTES:
 *      None
 */
EXPORT_SYMBOL INT32 c_btm_gatt_reg_evt(a_mtkapi_bt_gatt_evt_fct_cbk pf_evt, VOID * pv_tag)
{
    //FUNC_ENTRY;
    INT32 ret = BT_SUCCESS;
    BT_DBG_NOTICE(BT_DEBUG_GATT, "[GATT] pf_nfy = %p, pv_tag = %p", pf_evt, pv_tag);
    bt_gatt_evt_fct_cbk = pf_evt;
    bt_gatt_evt_pv_tag = pv_tag;
    return ret;
}
#define ONLY_FOR_BLE_SETUP_TEST 0
#if ONLY_FOR_BLE_SETUP_TEST
static VOID BtAppEventCallback(BTEvent bt_event)
{
    FUNC_ENTRY;
    BT_DBG_NORMAL(BT_DEBUG_COMM, "bt_event is: %d");

    BT_TARGET_DEV_LIST pt_device_list;
    memset(&pt_device_list,0,sizeof(BT_TARGET_DEV_LIST));

    // need a sample state machine here
    switch (bt_event)
    {
    case BT_GET_CODEC_INFORMATION_SUCCESS:
        if (get_bt_a2dp_connect())
        {
            //c_btm_btstart("PLAYING",300);
            set_bt_mode(BT_MODE_PLAYING);
        }
        break;
    case BT_CONNECT_SUCCESS:
        break;
    case BT_DISCONNECTED:
        //c_btm_btstart("PAIRING",300);
        //set_bt_mode(BT_MODE_PAIRING);
        //c_btm_get_paired_dev_list(&pt_device_list);
        c_btm_set_connectable_and_discoverable(TRUE,TRUE);
        break;
    case BT_EXIT:
        break;
    default:
        break;
    }
}

#if ENABLE_AVRCP_PROFILE
VOID BtAppPlayStateCallback(BTPlayState bt_play_state)
{
    FUNC_ENTRY;
    BT_DBG_NORMAL(BT_DEBUG_COMM, "bt_play_state is: %d", bt_play_state);
}

VOID BtAppVolumeCallback(BTVolType bt_volume)
{
    FUNC_ENTRY;
    BT_DBG_NORMAL(BT_DEBUG_COMM, "bt_volume is: %s", bt_volume?"DOWN":"UP");
}

VOID BtAppTrackChangeCallback(UINT32 i4_track_index)
{
    FUNC_ENTRY;
    BT_DBG_NORMAL(BT_DEBUG_COMM, "i4_track_index is: %ld", (long)i4_track_index);
}

VOID BtAppMediaStatusCallback(BTMediaPlayState bt_status)
{
    FUNC_ENTRY;
    BT_DBG_NORMAL(BT_DEBUG_COMM, "media play status is: %d", bt_status);
}

VOID BtAppAbsoluteVolumeCallback(BTAbsoluteVolumeType flag, UINT8 *bt_absolute_volume)
{
    FUNC_ENTRY;
}

VOID BtAppAVRCPCMDCallback(BTCmdType bt_cmd_type, UINT8 press)
{
    FUNC_ENTRY;
}

VOID BtAppGetPlayStatusCallback(BTPlayState *bt_playstatus)
{
    FUNC_ENTRY;
}

#endif

#if ENABLE_SPP_PROFILE
VOID BtSppCallback(BT_SPP_CBK_STRUCT *pt_spp_struct)
{
    FUNC_ENTRY;
}

#endif
#if ENABLE_HID_PROFILE_H
VOID BtHidCallback(BT_HID_CBK_STRUCT *pt_hid_struct)
{
    FUNC_ENTRY;
}

#endif


#if (ENABLE_A2DP_SRC || ENABLE_A2DP_SINK)
VOID BtAppUploadInfoCallback(BT_A2DP_UPLOAD_INFO* pt_upload_info, BOOL fg_only_check_mute)
{
    FUNC_ENTRY;
}

VOID BtAppPlaybackCtrlCallback(BTPlayState bt_playback_cmd)
{
    FUNC_ENTRY;
}
#endif


EXPORT_SYMBOL VOID c_btm_gatt_power_on()
{
    BT_APP_CB_FUNC app_func;
    memset(&app_func, 0x0, sizeof(BT_APP_CB_FUNC));
    app_func.bt_event_cb = BtAppEventCallback;
#if ENABLE_AVRCP_PROFILE
    app_func.bt_play_change_cb = BtAppPlayStateCallback;
    app_func.bt_volume_change_cb = BtAppVolumeCallback;
    app_func.bt_track_change_cb = BtAppTrackChangeCallback;
    app_func.bt_media_playstatus_cb = BtAppMediaStatusCallback;
    app_func.bt_absolute_volume_cb = BtAppAbsoluteVolumeCallback;
    app_func.bt_avrcp_cmd_cb = BtAppAVRCPCMDCallback;
    app_func.bt_get_play_status_cb = BtAppGetPlayStatusCallback;
#endif
#if ENABLE_SPP_PROFILE
    app_func.bt_spp_cb = BtSppCallback;
#endif
#if (ENABLE_A2DP_SRC || ENABLE_A2DP_SINK)
    app_func.bt_upload_info_cb = BtAppUploadInfoCallback;
    app_func.bt_playback_ctrl_cb = BtAppPlaybackCtrlCallback;
#endif
#if ENABLE_HID_PROFILE_H
    app_func.bt_hid_cb = BtHidCallback;
#endif
    c_btm_bt_base_init(&app_func);
}

EXPORT_SYMBOL VOID otherInitAfterBtInit(VOID)
{
#if ENABLE_AAC_CODEC
    c_btm_set_aac_enable(TRUE);
#endif
#if ENABLE_LDAC_CODEC
    c_btm_set_ldac_enable(TRUE);
#endif

    c_btm_bluetooth_on_off(TRUE);
    usleep(100*1000);
    c_btm_source_sink_on_off(TRUE,TRUE);
    usleep(100*1000);
    c_btm_set_local_name("TV_TURNKEY_1501");
    usleep(100*1000);
    c_btm_set_connectable_and_discoverable(TRUE,TRUE);
    usleep(100*1000);
}

#endif
/**
 * FUNCTION NAME: c_btm_gatt_initialize
 * PURPOSE:
 *      The function is used for gatt server to do initilization.
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
EXPORT_SYMBOL INT32 c_btm_gatt_initialize()
{
    //FUNC_ENTRY;
    //CC_TEMP;
    INT32 sec = 0;
    INT32 i4_loop = 100;

    while (0 < i4_loop && !get_bt_power_on())
    {
        /*x_thread_delay(100);*/
        usleep(100*1000);
        sec++;
        i4_loop--;
    }
    BT_DBG_INFO(BT_DEBUG_GATT, "[GATT] sleep %ld s, bt_power_on = %d", (long)sec, get_bt_power_on());
    INT32 ret = BT_SUCCESS;
    BT_APP_GATTS_CB_FUNC_T gatts_func;
    memset(&gatts_func, 0, sizeof(BT_APP_GATTS_CB_FUNC_T));
    gatts_func.bt_gatts_reg_server_cb = c_btm_gatt_reg_server_cbk;
    gatts_func.bt_gatts_add_srvc_cb = c_btm_gatt_add_srvc_cbk;
    gatts_func.bt_gatts_add_char_cb = c_btm_gatt_add_char_cbk;
    gatts_func.bt_gatts_req_read_cb = c_btm_gatt_req_read_cbk;
    gatts_func.bt_gatts_req_write_cb = c_btm_gatt_req_write_cbk;
    gatts_func.bt_gatts_event_cb = c_btm_gatts_connect_cbk;
    ret = c_btm_bt_gatts_base_init(&gatts_func);
    if (BT_SUCCESS != ret)
    {
        BT_DBG_ERROR(BT_DEBUG_GATT, "[GATT] Register BT_APP_GATTS_CB_FUNC failed.");
        return ret;

    }
    BT_APP_GATTC_CB_FUNC_T gatt_func;
    memset(&gatt_func, 0, sizeof(BT_APP_GATTC_CB_FUNC_T));
    gatt_func.bt_gatt_reg_client_cb = c_btm_gatt_reg_client_cbk;
    ret = c_btm_bt_gattc_base_init(&gatt_func);
    if (BT_SUCCESS != ret)
    {
        BT_DBG_ERROR(BT_DEBUG_GATT, "[GATT] Register BT_APP_GATT_CB_FUNC failed.");
        return ret;
    }

    ret = c_btm_bt_gatts_register_server_sync(BTM_GATTS_SERVER_UUID, &btm_server_if);
    if (BT_SUCCESS == ret)
    {
        BT_DBG_INFO(BT_DEBUG_GATT, "[GATTS] register server success!");
    }
    else
    {
        BT_DBG_ERROR(BT_DEBUG_GATT, "[GATTS] register server failed, ret = %ld", (long)ret);
        return ret;
    }
    ret = c_btm_bt_gattc_register_app_sync(BTM_GATTC_APP_UUID, &btm_client_if);
    if (BT_SUCCESS == ret)
    {
        BT_DBG_INFO(BT_DEBUG_GATT, "[GATTS] register client success!");
    }
    else
    {
        BT_DBG_ERROR(BT_DEBUG_GATT, "[GATTS] register client failed, ret = %ld", (long)ret);
        return ret;
    }
    //while(0x3 != (reg_flag & 0x3))
    //{
    //    sleep(1);
    //    sec++;
    //}
    //INT32 i = 0;
    //read_attr_value[0] = (CHAR *)malloc(sizeof(CHAR) * ATTRIBUTE_VALUE_MEM_SIZE * MAX_C4TV_SETUP_INFO);
    //for (i = 1; i < MAX_C4TV_SETUP_INFO; i++)
    //{
    //	read_attr_value[i] = read_attr_value[i-1] + ATTRIBUTE_VALUE_MEM_SIZE;
    //}
    //read_attr_value = malloc(ATTRIBUTE_VALUE_MEM_SIZE * sizeof(CHAR));
    //if (NULL == read_attr_value[0])
    //{
    //    BT_DBG_ERROR(BT_DEBUG_GATT, "[GATT] alloc memory failed");
    //    return BT_ERR_STATUS_NULL_POINTER;
    //}
    //memset(read_attr_value[0], 0, ATTRIBUTE_VALUE_MEM_SIZE * sizeof(CHAR));

    g_fg_charIsExist = FALSE;
    g_fg_serviceIsExist = FALSE;
    memset(started_service_uuid, 0, 8*64);
    memset(created_service_uuid, 0, 8*64);

    c4tv_setup_info = malloc(MAX_C4TV_SETUP_INFO * sizeof(c4tv_setup_info_t));
    if (NULL == c4tv_setup_info)
    {
        BT_DBG_ERROR(BT_DEBUG_GATT, "[GATT] alloc memory failed");
        return BT_ERR_STATUS_NULL_POINTER;
    }
    memset(c4tv_setup_info, 0, MAX_C4TV_SETUP_INFO * sizeof(c4tv_setup_info_t));

    c4tv_uuid_handle = malloc(MAX_C4TV_UUID_HANDLE * sizeof(c4tv_uuid_handle_t));
    if (NULL == c4tv_uuid_handle)
    {
        BT_DBG_ERROR(BT_DEBUG_GATT, "[GATT] alloc memory failed");
        return BT_ERR_STATUS_NULL_POINTER;
    }
    memset(c4tv_uuid_handle, 0, MAX_C4TV_UUID_HANDLE * sizeof(c4tv_uuid_handle_t));

    if (NULL != bt_gatt_evt_fct_cbk)
    {
        bt_gatt_evt_fct_cbk(1, 0, bt_gatt_evt_pv_tag);
    }
    else
    {
        BT_DBG_ERROR(BT_DEBUG_GATT, "[GATT] null pointer of bt_gatt_evt_fct_cbk");
    }
    c_btm_gatt_sema_create();
//extern INT32 bluetooth_set_virtual_sniffer(INT32 enable);
//bluetooth_set_virtual_sniffer(1);
    //CC_TEMP;
    return ret;
}

//extern INT32 c_btm_gatts_unregister_server(void);

/**
 * FUNCTION NAME: c_btm_gatt_finalize
 * PURPOSE:
 *      The function is used for gatt server to do uninitilization.
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
EXPORT_SYMBOL INT32 c_btm_gatt_finalize()
{
    //FUNC_ENTRY;
    INT32 ret = BT_SUCCESS;
    INT32 index = 0;
    ret = c_btm_bt_gatts_unregister_server(btm_server_if);
    if (BT_SUCCESS == ret)
    {
        BT_DBG_INFO(BT_DEBUG_GATT, "[GATTS] Unregister server success");
    }
    else
    {
        BT_DBG_ERROR(BT_DEBUG_GATT, "[GATTS] Unregister server failed");
        return ret;
    }
    ret = c_btm_bt_gattc_unregister_app(btm_client_if);
    if (BT_SUCCESS == ret)
    {
        BT_DBG_INFO(BT_DEBUG_GATT, "[GATTS] Unregister client success");
    }
    else
    {
        BT_DBG_ERROR(BT_DEBUG_GATT, "[GATTS] Unregister client failed");
        return ret;
    }
    g_fg_createdService = FALSE;
    g_fg_startedService = FALSE;
    g_fg_charIsExist = FALSE;
    g_fg_serviceIsExist = FALSE;

    for (index = 0; index < MAX_C4TV_SETUP_INFO; index++)
    {
        if (C4TV_SETUP_INFO_TYPE_E_NULL == c4tv_setup_info[index].type)
        {
            break;
        }
        switch (c4tv_setup_info[index].type)
        {
            case C4TV_SETUP_INFO_TYPE_E_ADV:
                if (NULL != c4tv_setup_info[index].u.adv.service_uuid)
                {
                    free(c4tv_setup_info[index].u.adv.service_uuid);
                    c4tv_setup_info[index].u.adv.service_uuid = NULL;
                }
                if (NULL != c4tv_setup_info[index].u.adv.advertised_uuid)
                {
                    free(c4tv_setup_info[index].u.adv.advertised_uuid);
                    c4tv_setup_info[index].u.adv.advertised_uuid = NULL;
                }
                if (NULL != c4tv_setup_info[index].u.adv.advertise_data)
                {
                    free(c4tv_setup_info[index].u.adv.advertise_data);
                    c4tv_setup_info[index].u.adv.advertise_data = NULL;
                }
                if (NULL != c4tv_setup_info[index].u.adv.manufacturer_data)
                {
                    free(c4tv_setup_info[index].u.adv.manufacturer_data);
                    c4tv_setup_info[index].u.adv.manufacturer_data = NULL;
                }
                break;
            case C4TV_SETUP_INFO_TYPE_E_SCAN:
                if (NULL != c4tv_setup_info[index].u.scan.service_uuid)
                {
                    free(c4tv_setup_info[index].u.scan.service_uuid);
                    c4tv_setup_info[index].u.scan.service_uuid = NULL;
                }
                if (NULL != c4tv_setup_info[index].u.scan.advertised_uuid)
                {
                    free(c4tv_setup_info[index].u.scan.advertised_uuid);
                    c4tv_setup_info[index].u.scan.advertised_uuid = NULL;
                }
                if (NULL != c4tv_setup_info[index].u.scan.advertise_data)
                {
                    free(c4tv_setup_info[index].u.scan.advertise_data);
                    c4tv_setup_info[index].u.scan.advertise_data = NULL;
                }
                if (NULL != c4tv_setup_info[index].u.scan.manufacturer_data)
                {
                    free(c4tv_setup_info[index].u.scan.manufacturer_data);
                    c4tv_setup_info[index].u.scan.manufacturer_data = NULL;
                }
                break;
            case C4TV_SETUP_INFO_TYPE_E_CHAR:
                if (NULL != c4tv_setup_info[index].u.attr.service_uuid)
                {
                    free(c4tv_setup_info[index].u.attr.service_uuid);
                    c4tv_setup_info[index].u.attr.service_uuid = NULL;
                }
                if (NULL != c4tv_setup_info[index].u.attr.attribute_uuid)
                {
                    free(c4tv_setup_info[index].u.attr.attribute_uuid);
                    c4tv_setup_info[index].u.attr.attribute_uuid = NULL;
                }
                if (NULL != c4tv_setup_info[index].u.attr.control_uuid)
                {
                    free(c4tv_setup_info[index].u.attr.control_uuid);
                    c4tv_setup_info[index].u.attr.control_uuid = NULL;
                }
                if (NULL != c4tv_setup_info[index].u.attr.value)
                {
                    free(c4tv_setup_info[index].u.attr.value);
                    c4tv_setup_info[index].u.attr.value = NULL;
                }
                if (NULL != c4tv_setup_info[index].u.attr.read_value)
                {
                    free(c4tv_setup_info[index].u.attr.read_value);
                    c4tv_setup_info[index].u.attr.read_value = NULL;
                }
                break;
            default:
                break;
        }

    }
    if (NULL != c4tv_setup_info)
    {
        free(c4tv_setup_info);
        c4tv_setup_info = NULL;
    }

    if (NULL != c4tv_uuid_handle)
    {
        free(c4tv_uuid_handle);
        c4tv_uuid_handle = NULL;
    }
    c_btm_gatt_sema_delete();
    return ret;
}

/**
 * FUNCTION NAME: c_btm_gatt_setDelegate
 * PURPOSE:
 *      The function is used for setting up an observer which should notified whenever a characteristic is written to,
 *      but not used in MW.
 * INPUT:
 *      delegate       -- observer
 * OUTPUT:
 *      None
 * RETURN:
 *      BT_SUCCESS                        -- Operate success.
 * NOTES:
 *      None
 */
EXPORT_SYMBOL INT32 c_btm_gatt_setDelegate(INT32 delegate)
{
    //FUNC_ENTRY;
    return BT_SUCCESS;
}

/**
 * FUNCTION NAME: c_btm_gatt_onCharacteristicWrite
 * PURPOSE:
 *      The function is used for whenever a characteristic is written to,
 *      but not used in MW.
 * INPUT:
 *      service_uuid         -- service uuid which include the characteristic
 *      char_uuid            -- characteristic uuid
 *      value                -- characteristic value
 * OUTPUT:
 *      None
 * RETURN:
 *      BT_SUCCESS                        -- Operate success.
 * NOTES:
 *      None
 */
EXPORT_SYMBOL INT32 c_btm_gatt_onCharacteristicWrite(CHAR * service_uuid, CHAR * char_uuid, CHAR * value)
{
    //FUNC_ENTRY;
    return BT_SUCCESS;
}

/**
 * FUNCTION NAME: c_btm_gatt_isSupported
 * PURPOSE:
 *      The function is used to judge the device supports bluetooth operations or not.
 * INPUT:
 *      None
 * OUTPUT:
 *      None
 * RETURN:
 *      TRUE      -- support bluetooth operations.
 * NOTES:
 *      None
 */
EXPORT_SYMBOL BOOL c_btm_gatt_isSupported()
{
    //FUNC_ENTRY;
    return TRUE;
}

/**
 * FUNCTION NAME: c_btm_gatt_createService
 * PURPOSE:
 *      The function is used to create a GATT service with the specified service uuid.
 * INPUT:
 *      service_uuid         -- service uuid which need to be created
 * OUTPUT:
 *      None
 * RETURN:
 *      BT_SUCCESS                        -- Operate success.
 *      BT_ERR_STATUS_FAIL                -- Operate fail.
 *      BT_ERR_STATUS_PARM_INVALID        -- paramter is invalid.
 * NOTES:
 *      None
 */
EXPORT_SYMBOL INT32 c_btm_gatt_createService(CHAR * service_uuid)
{
    //FUNC_ENTRY;
    //bt_uuid_t uuid;
    CHAR uuid[BT_GATT_MAX_UUID_LEN] = {0};
    INT32 ret = BT_SUCCESS;
    INT32 i= 0;
    if ((NULL == service_uuid) || (0 == strncmp(service_uuid, "", strlen(service_uuid))))
    {
        BT_DBG_ERROR(BT_DEBUG_GATT, "service_uuid is NULL");
        return BT_ERR_STATUS_PARM_INVALID;
    }
    g_fg_createdService= TRUE;
    for (i = 0; i < 8; i++)
    {
        if (created_service_uuid[i][0] != '\0')
        {
            if (memcmp(created_service_uuid[i], service_uuid, 64) == 0)
            {
                 break;
            }
        }
        else
        {
            memcpy(created_service_uuid[i], service_uuid, 32);
            break;
        }
    }

    //linuxbt_uuid_stoh(service_uuid, &uuid);
    bluetooth_uuid_stos(service_uuid, uuid);

    if (g_fg_serviceIsExist)
    {
        BT_DBG_INFO(BT_DEBUG_GATT, "[GATTS] already Create service uuid:'%s'", service_uuid);
        return 0;
    }

    BT_DBG_NOTICE(BT_DEBUG_GATT, "[GATT] service_uuid = %s, uuid = %s", service_uuid, uuid);
    INT32 srvc_handle = 0;
    ret = c_btm_bt_gatts_gatts_add_service_sync(btm_server_if, uuid, 1, 255, &srvc_handle);
    if (BT_SUCCESS == ret)
    {
        g_fg_serviceIsExist = TRUE;
        BT_DBG_INFO(BT_DEBUG_GATT, "[GATTS] Create service uuid:'%s'", service_uuid);
    }
    else
    {
        BT_DBG_ERROR(BT_DEBUG_GATT, "[GATTS] Create service failed, uuid:'%s'", service_uuid);
    }
    return ret;
}

/**
 * FUNCTION NAME: c_btm_gatt_destroyService
 * PURPOSE:
 *      The function is used to destroy a GATT service with the specified service uuid.
 * INPUT:
 *      service_uuid         -- service uuid which need to be desctoryed
 * OUTPUT:
 *      None
 * RETURN:
 *      BT_SUCCESS                        -- Operate success.
 *      BT_ERR_STATUS_FAIL                -- Operate fail.
 *      BT_ERR_STATUS_PARM_INVALID        -- paramter is invalid.
 * NOTES:
 *      None
 */
EXPORT_SYMBOL INT32 c_btm_gatt_destroyService(CHAR * service_uuid)
{
    //FUNC_ENTRY;
    UINT32 index = 0;
    UINT32 sec = 0;
    INT32 ret = BT_SUCCESS;
    c4tv_uuid_handle_t uuid_handle;
    //bt_uuid_t srvc_uuid;
    CHAR srvc_uuid[BT_GATT_MAX_UUID_LEN] = {0};

    if ((NULL == service_uuid) || (0 == strncmp(service_uuid, "", strlen(service_uuid))))
    {
        BT_DBG_ERROR(BT_DEBUG_GATT, "service_uuid is NULL");
        return BT_ERR_STATUS_PARM_INVALID;
    }

    //linuxbt_uuid_stoh(service_uuid, &srvc_uuid);
    bluetooth_uuid_stos(service_uuid, srvc_uuid);

    uuid_handle.type = C4TV_UUID_HANDLE_TYPE_E_SRVC;
    memcpy(uuid_handle.uuid, srvc_uuid, BT_GATT_MAX_UUID_LEN);
    index = c_btm_gatt_findHandleByUuid(&uuid_handle);
    while (MAX_C4TV_UUID_HANDLE == index)
    {
        /*x_thread_delay(10);*/
        usleep(10*1000);
        sec++;
        if (10 == sec)
        {
            BT_DBG_ERROR(BT_DEBUG_GATT, "[GATTS] find service handle failed, uuid:'%s'", service_uuid);
            return BT_ERR_STATUS_FAIL;
        }
        index = c_btm_gatt_findHandleByUuid(&uuid_handle);
    }
    ret = c_btm_bt_gatts_delete_service(btm_server_if, c4tv_uuid_handle[index].handle);
    if (BT_SUCCESS == ret)
    {
        g_fg_charIsExist = FALSE;
        BT_DBG_INFO(BT_DEBUG_GATT, "[GATTS] Destroy server uuid:'%s'", service_uuid);
    }
    else
    {
        BT_DBG_ERROR(BT_DEBUG_GATT, "[GATTS] Destroy server failed, uuid:'%s'", service_uuid);
    }
    return ret;
}

/**
 * FUNCTION NAME: c_btm_gatt_startService
 * PURPOSE:
 *      The function is used to start a created GATT service, this triggers advertising, and enables
 *      connections to the service, enabling attribute I/O.
 * INPUT:
 *      service_uuid         -- service uuid which need to be started
 * OUTPUT:
 *      None
 * RETURN:
 *      BT_SUCCESS                        -- Operate success.
 *      BT_ERR_STATUS_FAIL                -- Operate fail.
 *      BT_ERR_STATUS_PARM_INVALID        -- paramter is invalid.
 * NOTES:
 *      None
 */
EXPORT_SYMBOL INT32 c_btm_gatt_startService(CHAR * service_uuid)
{
    //FUNC_ENTRY;
    CC_TEMP;
    UINT32 uuid_handle_index = 0;
    UINT32 setup_index = 0;
    UINT32 sec = 0;
    INT32 service_handle = 0;
    INT32 properties = 0;
    INT32 permissions = 0;
    c4tv_uuid_handle_t uuid_handle;
    c4tv_setup_info_t setup_info;
    //bt_uuid_t srvc_uuid;
    INT32 ret = BT_SUCCESS;
    CHAR srvc_uuid[BT_GATT_MAX_UUID_LEN] = {0};
    INT32 char_handle = -1;
    INT32 i = 0;
    if ((NULL == service_uuid) || (0 == strncmp(service_uuid, "", strlen(service_uuid))))
    {
        BT_DBG_ERROR(BT_DEBUG_GATT, "service_uuid is NULL");
        return BT_ERR_STATUS_PARM_INVALID;
    }
    g_fg_startedService= TRUE;
    for (i = 0; i < 8; i++)
    {
        if (started_service_uuid[i][0] != '\0')
        {
            if (memcmp(started_service_uuid[i], service_uuid, 64) == 0 )
            {
                 break;
            }
        }
        else
        {
            memcpy(started_service_uuid[i], service_uuid, 32);
            break;
        }
    }

    //linuxbt_uuid_stoh(service_uuid, &srvc_uuid);
    bluetooth_uuid_stos(service_uuid, srvc_uuid);

    uuid_handle.type = C4TV_UUID_HANDLE_TYPE_E_SRVC;
    memcpy(uuid_handle.uuid, srvc_uuid, BT_GATT_MAX_UUID_LEN);
    uuid_handle_index = c_btm_gatt_findHandleByUuid(&uuid_handle);
    while (MAX_C4TV_UUID_HANDLE == uuid_handle_index)
    {
        /*x_thread_delay(10);*/
        usleep(10*1000);
        sec++;
        if (10 == sec)
        {
            BT_DBG_ERROR(BT_DEBUG_GATT, "[GATTS] find service handle failed, uuid:'%s'", service_uuid);
            return BT_ERR_STATUS_FAIL;
        }
        uuid_handle_index = c_btm_gatt_findHandleByUuid(&uuid_handle);
    }

    service_handle = c4tv_uuid_handle[uuid_handle_index].handle;
    if (!g_fg_charIsExist)
    {
        for (uuid_handle_index = 0; uuid_handle_index < MAX_C4TV_UUID_HANDLE; uuid_handle_index++)
        {
            if (C4TV_UUID_HANDLE_TYPE_E_CHAR == c4tv_uuid_handle[uuid_handle_index].type)
            {
                setup_info.type = C4TV_SETUP_INFO_TYPE_E_CHAR;
                memcpy(setup_info.u.attr.bt_attribute_uuid, c4tv_uuid_handle[uuid_handle_index].uuid, BT_GATT_MAX_UUID_LEN);

                setup_index = c_btm_gatt_findSetupInfo(&setup_info);
                if (MAX_C4TV_SETUP_INFO == setup_index)
                {
                    BT_DBG_ERROR(BT_DEBUG_GATT, "[GATTS] find characteristic setup info, index:%d", uuid_handle_index);
                    continue;
                }

                properties = 0;
                permissions = 0;

                if (c4tv_setup_info[setup_index].u.attr.option & 0x1)   //  read
                {
                    properties |= 0x2;
                    permissions |= 0x1;
                }

                if (c4tv_setup_info[setup_index].u.attr.option & 0x2)   //  write
                {
                    properties |= 0x8;
                    permissions |= 0x10;
                }

                if (c4tv_setup_info[setup_index].u.attr.option & 0x4)   //  notify
                {
                    properties |= 0x10;
                }
                char_handle = -1;
                ret = c_btm_bt_gatts_add_char_sync(btm_server_if, service_handle, c4tv_uuid_handle[uuid_handle_index].uuid,
                                                   properties, permissions, &char_handle);
                if (BT_SUCCESS == ret)
                {
                    BT_DBG_INFO(BT_DEBUG_GATT, "[GATTS] Add characteristic, index:%d, properties:0x%x, permissions:0x%x", uuid_handle_index, properties, permissions);
                }
                else
                {
                    BT_DBG_ERROR(BT_DEBUG_GATT, "[GATTS] Add characteristic failed, index:%d, properties:0x%x, permissions:0x%x", uuid_handle_index, properties, permissions);
                    return ret;
                }

                if (NULL != c4tv_setup_info[setup_index].u.attr.control_uuid)
                {
                    char_handle = -1;
                    ret = c_btm_bt_gatts_add_char_sync(btm_server_if, service_handle, c4tv_setup_info[setup_index].u.attr.bt_control_uuid,
                                                       0xa, 0x11, &char_handle);
                    //BT_DBG_ERROR(BT_DEBUG_GATT, "[GATTS] Add characteristic , c4tv_setup_info[%d].u.attr.control_uuid = %s, c4tv_setup_info[%d].u.attr.bt_control_uuid = %s", setup_index,
                    //			c4tv_setup_info[setup_index].u.attr.control_uuid, setup_index, c4tv_setup_info[setup_index].u.attr.bt_control_uuid);
                    if (BT_SUCCESS == ret)
                    {
                        BT_DBG_INFO(BT_DEBUG_GATT, "[GATTS] Add characteristic, index:%d\n, properties:0x%x, permissions:0x%x", setup_index, properties, permissions);
                    }
                    else
                    {
                        BT_DBG_ERROR(BT_DEBUG_GATT, "[GATTS] Add characteristic failed, index:%d\n, ret = %dn", setup_index, ret);
                        return ret;
                    }
                }
                else
                {
                    BT_DBG_INFO(BT_DEBUG_GATT, "control_uuid is NULL");
                }
            }
        }
        g_fg_charIsExist = TRUE;
    }


    //sleep(3);
    ret = c_btm_bt_gatts_start_service_sync(btm_server_if, service_handle, 0);
    if (BT_SUCCESS == ret)
    {
        BT_DBG_INFO(BT_DEBUG_GATT, "[GATTS] Start server uuid:'%s'", service_uuid);
    }
    else
    {
        BT_DBG_ERROR(BT_DEBUG_GATT, "[GATTS] Start server failed, uuid:'%s'", service_uuid);
        return ret;
    }

    if (1 == set_adv_flag)
    {
        CHAR adv_advertise_data[32] = {0};
        INT32 adv_advertise_data_len = 0;
        c_btm_gattc_set_local_le_name(btm_client_if, p_deviceName);
        c_btm_gattc_set_local_disc_mode(btm_client_if, 6);
        usleep(100*1000);
        //sleep(1);
        ret = c_btm_bt_gattc_multi_adv_enable_sync(btm_client_if, 2048, 2048, 0, 0, 1, 0);
        if (BT_SUCCESS == ret)
        {
            BT_DBG_INFO(BT_DEBUG_GATT, "[GATTS] enable advertisement for start server uuid:'%s'", service_uuid);
        }
        else
        {
            BT_DBG_ERROR(BT_DEBUG_GATT, "[GATTS] enable advertisement for start server failed, uuid:'%s'", service_uuid);
            return ret;
        }

        //sleep(1);
        for (setup_index = 0; setup_index < MAX_C4TV_SETUP_INFO; setup_index++)
        {
            if (C4TV_SETUP_INFO_TYPE_E_ADV != c4tv_setup_info[setup_index].type)
            {
                continue;
            }

            adv_advertise_data_len = ascii_2_hex(c4tv_setup_info[setup_index].u.adv.advertise_data, strlen(c4tv_setup_info[setup_index].u.adv.advertise_data), (UINT8 *)adv_advertise_data);

            //bluetooth_uuid_stoh(c4tv_setup_info[setup_index].u.adv.bt_advertised_uuid, uuid);

            ret = c_btm_bt_gattc_multi_adv_setdata_sync(btm_client_if,
                    FALSE,
                    adv_transmit_name,
                    FALSE,
                    0,
                    0,       //  strlen(p_adv_manufacturer_data),
                    NULL,    //  p_adv_manufacturer_data,
                    adv_advertise_data_len,
                    adv_advertise_data,
                    BT_GATT_MAX_UUID_LEN,
                    c4tv_setup_info[setup_index].u.adv.bt_advertised_uuid);

            if (BT_SUCCESS == ret)
            {
                BT_DBG_INFO(BT_DEBUG_GATT, "[GATTS] Set advertisement for start server uuid:'%s'", service_uuid);
            }
            else
            {
                BT_DBG_ERROR(BT_DEBUG_GATT, "[GATTS] Set advertisement for start server failed, uuid:'%s'", service_uuid);
                return ret;
            }
        }
        set_adv_flag = 0;
    }
    else
    {
        BT_DBG_ERROR(BT_DEBUG_GATT, "[GATTS] No set Advertisement");
    }

    if (1 == set_scan_flag)
    {
        c_btm_gattc_set_local_le_name(btm_client_if, p_deviceName);
        c_btm_gattc_set_local_disc_mode(btm_client_if, 6);
        usleep(100*1000);

//        char scan_advertise_data[32] = {0};
//        int scan_advertise_data_len = 0;
        //sleep(1);

//        scan_advertise_data_len = ascii_2_hex(p_adv_advertise_data, strlen(p_adv_advertise_data), (unsigned char *)scan_advertise_data);
        ret = c_btm_bt_gattc_multi_adv_setdata_sync(btm_client_if,
                TRUE,
                scan_transmit_name,
                FALSE,
                0,
                0,       //  strlen(p_scan_manufacturer_data),
                NULL,    //  p_scan_manufacturer_data,
                0,       //  scan_advertise_data_len,
                NULL,    //  scan_advertise_data,
                0,       //  strlen(p_scan_advertised_uuid) * 8,
                NULL);

        if (BT_SUCCESS == ret)
        {
            BT_DBG_INFO(BT_DEBUG_GATT, "[GATTS] Set scan response for start server uuid:'%s'", service_uuid);
        }
        else
        {
            BT_DBG_ERROR(BT_DEBUG_GATT, "[GATTS] Set scan response for start server failed, uuid:'%s'", service_uuid);
            return ret;
        }

        set_scan_flag = 0;
    }
    else
    {
        BT_DBG_ERROR(BT_DEBUG_GATT, "[GATTS] No set Scan Response");
    }

    setup_flag = 1;
    CC_TEMP;
    return ret;
}

/**
 * FUNCTION NAME: c_btm_gatt_stopService
 * PURPOSE:
 *      The function is used to stop a started GATT service, disable advertisenment, and disable
 *      connections.
 * INPUT:
 *      service_uuid         -- service uuid which need to be stopped.
 * OUTPUT:
 *      None
 * RETURN:
 *      BT_SUCCESS                        -- Operate success.
 *      BT_ERR_STATUS_FAIL                -- Operate fail.
 *      BT_ERR_STATUS_PARM_INVALID        -- paramter is invalid.
 * NOTES:
 *      None
 */
EXPORT_SYMBOL INT32 c_btm_gatt_stopService(CHAR * service_uuid)
{
    //FUNC_ENTRY;
    UINT32 index = 0;
    UINT32 sec = 0;
    c4tv_uuid_handle_t uuid_handle;
    //bt_uuid_t srvc_uuid;
    INT32 ret = BT_SUCCESS;
    CHAR srvc_uuid[BT_GATT_MAX_UUID_LEN] = {0};

    if ((NULL == service_uuid) || (0 == strncmp(service_uuid, "", strlen(service_uuid))))
    {
        BT_DBG_ERROR(BT_DEBUG_GATT, "service_uuid is NULL");
        return BT_ERR_STATUS_PARM_INVALID;
    }

    //linuxbt_uuid_stoh(service_uuid, &srvc_uuid);
    bluetooth_uuid_stos(service_uuid, srvc_uuid);

    uuid_handle.type = C4TV_UUID_HANDLE_TYPE_E_SRVC;
    memcpy(uuid_handle.uuid, srvc_uuid, BT_GATT_MAX_UUID_LEN);
    index = c_btm_gatt_findHandleByUuid(&uuid_handle);
    while (MAX_C4TV_UUID_HANDLE == index)
    {
        /*x_thread_delay(10);*/
        usleep(10*1000);
        sec++;
        if (10 == sec)
        {
            BT_DBG_ERROR(BT_DEBUG_GATT, "[GATTS] find service handle failed, uuid:'%s'", service_uuid);
            return BT_ERR_STATUS_FAIL;
        }
        index = c_btm_gatt_findHandleByUuid(&uuid_handle);
    }
    ret = c_btm_bt_gatts_stop_service(btm_server_if, c4tv_uuid_handle[index].handle);
    if (BT_SUCCESS == ret)
    {
        BT_DBG_INFO(BT_DEBUG_GATT, "[GATTS] Stop server uuid:'%s'", service_uuid);
    }
    else
    {
        BT_DBG_ERROR(BT_DEBUG_GATT, "[GATTS] Stop server failed, uuid:'%s'", service_uuid);
    }
    return ret;
}

/**
 * FUNCTION NAME: c_btm_gatt_setDeviceName
 * PURPOSE:
 *      The function is used to set local device name, this can be included in advertisments or scan responses.
 * INPUT:
 *      name         -- local device name.
 * OUTPUT:
 *      None
 * RETURN:
 *      BT_SUCCESS                    -- Operate success.
 *      BT_ERR_STATUS_NOMEM           -- no memory.
 *      BT_ERR_STATUS_PARM_INVALID    -- paramter is invalid.
 * NOTES:
 *      None
 */
EXPORT_SYMBOL INT32 c_btm_gatt_setDeviceName(CHAR * name)
{
    //FUNC_ENTRY;

    if ((NULL == name) || (0 == strncmp(name, "", strlen(name))))
    {
        BT_DBG_ERROR(BT_DEBUG_GATT, "name is NULL");
        return BT_ERR_STATUS_PARM_INVALID;
    }

    if (NULL != p_deviceName)
    {
        if (strlen(name) > strlen(p_deviceName))
        {
            free(p_deviceName);
            p_deviceName = NULL;
            p_deviceName = malloc(strlen(name) + 1);
            if (NULL == p_deviceName)
            {
                BT_DBG_ERROR(BT_DEBUG_GATT, "[GATTS] set device name, failed: %s", name);
                return BT_ERR_STATUS_NOMEM;
            }
        }
    }
    else
    {
        p_deviceName = malloc(strlen(name) + 1);
        if (NULL == p_deviceName)
        {
            BT_DBG_ERROR(BT_DEBUG_GATT, "[GATTS] set device name, failed: %s", name);
            return BT_ERR_STATUS_NOMEM;
        }
    }

    strncpy(p_deviceName, name, strlen(name));
    p_deviceName[strlen(name)] = '\0';
//  adv_transmit_name = TRUE;
    set_scan_flag = 1;
    scan_transmit_name = TRUE;

    if (NULL != BtGattGetDeviceNameCbk)
    {
        BtGattGetDeviceNameCbk(p_deviceName);
    }
    else
    {
        // BT_DBG_ERROR(BT_DEBUG_GATT, "[GATT] get device name callback is null.");
    }
    BT_DBG_INFO(BT_DEBUG_GATT, "LE name is %s", p_deviceName);
    return BT_SUCCESS;
}

/**
 * FUNCTION NAME: c_btm_gatt_getDeviceName
 * PURPOSE:
 *      The function is used to get local device name.
 * INPUT:
 *      name         -- local device name.
 * OUTPUT:
 *      None
 * RETURN:
 *      BT_SUCCESS                    -- Operate success.
 *      BT_ERR_STATUS_NOMEM           -- no memory.
 *      BT_ERR_STATUS_PARM_INVALID    -- paramter is invalid.
 * NOTES:
 *      None
 */
INT32 c_btm_gatt_getDeviceName(CHAR * name)
{
    if (name == NULL)
    {
        BT_DBG_ERROR(BT_DEBUG_GATT, "input null pointer.");
        return BT_ERR_NULL_POINTER;
    }

    if (p_deviceName == NULL || 0 == strlen(p_deviceName))
    {
        BT_DBG_ERROR(BT_DEBUG_GATT, "can't get device name.");
        return BT_ERR_FAILED;
    }

    strncpy(name,
            p_deviceName,
            strlen(p_deviceName)
           );
    name[strlen(p_deviceName)] = '\0';
    BT_DBG_INFO(BT_DEBUG_GATT, "get device name=%s.", name);

    return BT_SUCCESS;
}

/**
 * FUNCTION NAME: c_btm_gatt_setAdvertisement
 * PURPOSE:
 *      The function is used to set advertisment, customizes the payload data of the GAP service advertisment.
 * INPUT:
 *      service_uuid         -- the created service uuid
 *      advertised_uuid      -- uuid to put in the complete list of 16bit service class uuid field of the payload.
 *      advertise_data       -- data to put in the service data field of the payload.
 *      manufacturer_data    -- the value to put in the manufacturer specific data field of the payload.
 *      transmit_name        -- if true, the name set by SetDeviceName is sent in Local Name field.
 * OUTPUT:
 *      None
 * RETURN:
 *      BT_SUCCESS                    -- Operate success.
 *      BT_ERR_STATUS_NOMEM           -- no memory.
 *      BT_ERR_STATUS_PARM_INVALID    -- paramter is invalid.
 *      BT_ERR_STATUS_FAIL            -- Operate fail.
 * NOTES:
 *      None
 */
EXPORT_SYMBOL INT32 c_btm_gatt_setAdvertisement(CHAR * service_uuid, CHAR * advertised_uuid, CHAR * advertise_data, CHAR * manufacturer_data, BOOL transmit_name)
{
    //FUNC_ENTRY;
    BT_DBG_NOTICE(BT_DEBUG_GATT, "[GATT] service_uuid = %s, advertised_uuid = %s, advertise_data = %s, manufacturer_data = %s, transmit_name = %d",
                  service_uuid, advertised_uuid, advertise_data, manufacturer_data, transmit_name);

    UINT32 index = 0;
    c4tv_setup_info_t info;
    //bt_uuid_t srvc_uuid;
    //bt_uuid_t adv_uuid;
    INT32 ret = BT_SUCCESS;
    CHAR srvc_uuid[BT_GATT_MAX_UUID_LEN] = {0};
    CHAR adv_uuid[BT_GATT_MAX_UUID_LEN] = {0};

    if (0 == setup_flag)
    {
        set_adv_flag = 1;

        //linuxbt_uuid_stoh(service_uuid, &srvc_uuid);
        //linuxbt_uuid_stoh(advertised_uuid, &adv_uuid);
        bluetooth_uuid_stos(service_uuid, srvc_uuid);
        bluetooth_uuid_stos(advertised_uuid, adv_uuid);

        info.type = C4TV_SETUP_INFO_TYPE_E_ADV;
        memcpy(info.u.adv.bt_service_uuid, srvc_uuid, BT_GATT_MAX_UUID_LEN);
        memcpy(info.u.adv.bt_advertised_uuid, adv_uuid, BT_GATT_MAX_UUID_LEN);

        index = c_btm_gatt_findSetupInfo(&info);
        if (MAX_C4TV_SETUP_INFO == index)
        {
            index = c_btm_gatt_allocNewSetupInfo();
            if (MAX_C4TV_SETUP_INFO == index)
            {
                BT_DBG_ERROR(BT_DEBUG_GATT, "[GATTS] Set advertisement before start service failed, uuid:'%s'", service_uuid);
                return BT_ERR_STATUS_FAIL;
            }
        }

        c4tv_setup_info[index].type = C4TV_SETUP_INFO_TYPE_E_ADV;

        if ((NULL == service_uuid) || (0 == strncmp(service_uuid, "", strlen(service_uuid))))
        {
            BT_DBG_ERROR(BT_DEBUG_GATT, "[GATTS] service_uuid is NULL");
        }
        else
        {
            if (NULL != c4tv_setup_info[index].u.adv.service_uuid)
            {
                if (strlen(service_uuid) > strlen(c4tv_setup_info[index].u.adv.service_uuid))
                {
                    free(c4tv_setup_info[index].u.adv.service_uuid);
                    c4tv_setup_info[index].u.adv.service_uuid = NULL;
                    c4tv_setup_info[index].u.adv.service_uuid = malloc(strlen(service_uuid) + 1);
                    if (NULL == c4tv_setup_info[index].u.adv.service_uuid)
                    {
                        BT_DBG_ERROR(BT_DEBUG_GATT, "[GATTS] set device name, failed: %s", service_uuid);
                        return BT_ERR_STATUS_NOMEM;
                    }
                }
            }
            else
            {
                c4tv_setup_info[index].u.adv.service_uuid = malloc(strlen(service_uuid) + 1);
                if (NULL == c4tv_setup_info[index].u.adv.service_uuid)
                {
                    BT_DBG_ERROR(BT_DEBUG_GATT, "[GATTS] set device name, failed: %s", service_uuid);
                    return BT_ERR_STATUS_NOMEM;
                }
            }

            memcpy(c4tv_setup_info[index].u.adv.bt_service_uuid, srvc_uuid, BT_GATT_MAX_UUID_LEN);
            strncpy(c4tv_setup_info[index].u.adv.service_uuid, service_uuid, strlen(service_uuid));
            c4tv_setup_info[index].u.adv.service_uuid[strlen(service_uuid)] = '\0';
        }

        if ((NULL == advertised_uuid) || (0 == strncmp(advertised_uuid, "", strlen(advertised_uuid))))
        {
            BT_DBG_ERROR(BT_DEBUG_GATT, "[GATTS] advertised_uuid is NULL");
        }
        else
        {
            if (NULL != c4tv_setup_info[index].u.adv.advertised_uuid)
            {
                if (strlen(advertised_uuid) > strlen(c4tv_setup_info[index].u.adv.advertised_uuid))
                {
                    free(c4tv_setup_info[index].u.adv.advertised_uuid);
                    c4tv_setup_info[index].u.adv.advertised_uuid = NULL;
                    c4tv_setup_info[index].u.adv.advertised_uuid = malloc(strlen(advertised_uuid) + 1);
                    if (NULL == c4tv_setup_info[index].u.adv.advertised_uuid)
                    {
                        BT_DBG_ERROR(BT_DEBUG_GATT, "[GATTS] set device name, failed: %s", advertised_uuid);
                        return BT_ERR_STATUS_NOMEM;
                    }
                }
            }
            else
            {
                c4tv_setup_info[index].u.adv.advertised_uuid = malloc(strlen(advertised_uuid) + 1);
                if (NULL == c4tv_setup_info[index].u.adv.advertised_uuid)
                {
                    BT_DBG_ERROR(BT_DEBUG_GATT, "[GATTS] set device name, failed: %s", advertised_uuid);
                    return BT_ERR_STATUS_NOMEM;
                }
            }

            memcpy(c4tv_setup_info[index].u.adv.bt_advertised_uuid, adv_uuid, BT_GATT_MAX_UUID_LEN);
            strncpy(c4tv_setup_info[index].u.adv.advertised_uuid, advertised_uuid, strlen(advertised_uuid));
            c4tv_setup_info[index].u.adv.advertised_uuid[strlen(advertised_uuid)] = '\0';
        }

        if ((NULL == advertise_data) || (0 == strncmp(advertise_data, "", strlen(advertise_data))))
        {
            BT_DBG_ERROR(BT_DEBUG_GATT, "[GATTS] advertise_data is NULL");
        }
        else
        {
            if (NULL != c4tv_setup_info[index].u.adv.advertise_data)
            {
                if (strlen(advertise_data) > strlen(c4tv_setup_info[index].u.adv.advertise_data))
                {
                    free(c4tv_setup_info[index].u.adv.advertise_data);
                    c4tv_setup_info[index].u.adv.advertise_data = NULL;
                    c4tv_setup_info[index].u.adv.advertise_data = malloc(strlen(advertise_data) + 1);
                    if (NULL == c4tv_setup_info[index].u.adv.advertise_data)
                    {
                        BT_DBG_ERROR(BT_DEBUG_GATT, "[GATTS] set device name, failed: %s", advertise_data);
                        return BT_ERR_STATUS_NOMEM;
                    }
                }
            }
            else
            {
                c4tv_setup_info[index].u.adv.advertise_data = malloc(strlen(advertise_data) + 1);
                if (NULL == c4tv_setup_info[index].u.adv.advertise_data)
                {
                    BT_DBG_ERROR(BT_DEBUG_GATT, "[GATTS] set device name, failed: %s", advertise_data);
                    return BT_ERR_STATUS_NOMEM;
                }
            }

            strncpy(c4tv_setup_info[index].u.adv.advertise_data, advertise_data, strlen(advertise_data));
            c4tv_setup_info[index].u.adv.advertise_data[strlen(advertise_data)] = '\0';
        }

        if ((NULL == manufacturer_data) || (0 == strncmp(manufacturer_data, "", strlen(manufacturer_data))))
        {
            BT_DBG_ERROR(BT_DEBUG_GATT, "[GATTS] manufacturer_data is NULL");
        }
        else
        {
            if (NULL != c4tv_setup_info[index].u.adv.manufacturer_data)
            {
                if (strlen(manufacturer_data) > strlen(c4tv_setup_info[index].u.adv.manufacturer_data))
                {
                    free(c4tv_setup_info[index].u.adv.manufacturer_data);
                    c4tv_setup_info[index].u.adv.manufacturer_data = NULL;
                    c4tv_setup_info[index].u.adv.manufacturer_data = malloc(strlen(manufacturer_data) + 1);
                    if (NULL == c4tv_setup_info[index].u.adv.manufacturer_data)
                    {
                        BT_DBG_ERROR(BT_DEBUG_GATT, "[GATTS] set device name, failed: %s", manufacturer_data);
                        return BT_ERR_STATUS_NOMEM;
                    }
                }
            }
            else
            {
                c4tv_setup_info[index].u.adv.manufacturer_data = malloc(strlen(manufacturer_data) + 1);
                if (NULL == c4tv_setup_info[index].u.adv.manufacturer_data)
                {
                    BT_DBG_ERROR(BT_DEBUG_GATT, "[GATTS] set device name, failed: %s", manufacturer_data);
                    return BT_ERR_STATUS_NOMEM;
                }
            }

            strncpy(c4tv_setup_info[index].u.adv.manufacturer_data, manufacturer_data, strlen(manufacturer_data));
            c4tv_setup_info[index].u.adv.manufacturer_data[strlen(manufacturer_data)] = '\0';
        }
    }
    else
    {
        CHAR adv_advertise_data[32] = {0};
        INT32 adv_advertise_data_len = 0;
        //CHAR t_uuid[16] = {0};
        //CHAR uuid[16] = {0};

        adv_advertise_data_len = ascii_2_hex(advertise_data, strlen(advertise_data), (UINT8 *)adv_advertise_data);

        //bluetooth_uuid_stoh(advertised_uuid, uuid);
#if 0
        ascii_2_hex(advertised_uuid, sizeof(advertised_uuid), (UINT8 *)t_uuid);

        uuid[15] = 0x00;
        uuid[14] = 0x00;
        uuid[13] = t_uuid[0];
        uuid[12] = t_uuid[1];

        uuid[11] = 0x00;
        uuid[10] = 0x00;
        uuid[9] = 0x10;
        uuid[8] = 0x00;

        uuid[7] = 0x80;
        uuid[6] = 0x00;
        uuid[5] = 0x00;
        uuid[4] = 0x80;

        uuid[3] = 0x5F;
        uuid[2] = 0x9B;
        uuid[1] = 0x34;
        uuid[0] = 0xFB;
#endif
        c_btm_gattc_set_local_le_name(btm_client_if, p_deviceName);
        c_btm_gattc_set_local_disc_mode(btm_client_if, 6);

        usleep(100*1000);
        ret= c_btm_bt_gattc_multi_adv_setdata(btm_client_if,
                                              FALSE,
                                              transmit_name,
                                              FALSE,
                                              0,
                                              0,       //  strlen(manufacturer_data),
                                              NULL,    //  manufacturer_data,
                                              adv_advertise_data_len,
                                              adv_advertise_data,
                                              strlen(advertised_uuid),
                                              advertised_uuid);
        if (BT_SUCCESS == ret)

        {
            BT_DBG_INFO(BT_DEBUG_GATT, "[GATTS] Set advertisement for start server uuid:'%s'", service_uuid);
        }
        else
        {
            BT_DBG_ERROR(BT_DEBUG_GATT, "[GATTS] Set advertisement for start server failed, uuid:'%s'", service_uuid);
        }
    }

    return ret;
}

/**
 * FUNCTION NAME: c_btm_gatt_setScanResponse
 * PURPOSE:
 *      The function is used to set scan response data, customizes the payload data of the GAP scan response.
 * INPUT:
 *      service_uuid         -- the created service uuid
 *      advertised_uuid      -- uuid to put in the complete list of 16bit service class uuid field of the payload.
 *      advertise_data       -- data to put in the service data field of the payload.
 *      manufacturer_data    -- the value to put in the manufacturer specific data field of the payload.
 *      transmit_name        -- if true, the name set by SetDeviceName is sent in Local Name field.
 * OUTPUT:
 *      None
 * RETURN:
 *      BT_SUCCESS                    -- Operate success.
 *      BT_ERR_STATUS_NOMEM           -- no memory.
 *      BT_ERR_STATUS_PARM_INVALID    -- paramter is invalid.
 *      BT_ERR_STATUS_FAIL            -- Operate fail.
 * NOTES:
 *      None
 */
EXPORT_SYMBOL INT32 c_btm_gatt_setScanResponse(CHAR * service_uuid, CHAR * advertised_uuid, CHAR * advertise_data, CHAR * manufacturer_data, BOOL transmit_name)
{
    //FUNC_ENTRY;
    BT_DBG_NOTICE(BT_DEBUG_GATT, "[GATT] service_uuid = %s, advertised_uuid = %s, advertise_data = %s, manufacturer_data = %s, transmit_name = %d",
                  service_uuid, advertised_uuid, advertise_data, manufacturer_data, transmit_name);

    UINT32 index = 0;
    c4tv_setup_info_t info;
    //bt_uuid_t srvc_uuid;
    //bt_uuid_t adv_uuid;
    INT32 ret = BT_SUCCESS;
    CHAR srvc_uuid[BT_GATT_MAX_UUID_LEN] = {0};
    CHAR adv_uuid[BT_GATT_MAX_UUID_LEN] = {0};

    if (0 == setup_flag)
    {
        set_scan_flag = 1;

        //linuxbt_uuid_stoh(service_uuid, &srvc_uuid);
        //linuxbt_uuid_stoh(advertised_uuid, &adv_uuid);
        bluetooth_uuid_stos(service_uuid, srvc_uuid);
        bluetooth_uuid_stos(advertised_uuid, adv_uuid);

        info.type = C4TV_SETUP_INFO_TYPE_E_SCAN;
        memcpy(info.u.scan.bt_service_uuid, srvc_uuid, BT_GATT_MAX_UUID_LEN);
        memcpy(info.u.scan.bt_advertised_uuid, adv_uuid, BT_GATT_MAX_UUID_LEN);

        index = c_btm_gatt_findSetupInfo(&info);
        if (MAX_C4TV_SETUP_INFO == index)
        {
            index = c_btm_gatt_allocNewSetupInfo();
            if (MAX_C4TV_SETUP_INFO == index)
            {
                BT_DBG_ERROR(BT_DEBUG_GATT, "[GATTS] Set advertisement before start service failed, uuid:'%s'", service_uuid);
                return BT_ERR_STATUS_FAIL;
            }
        }

        c4tv_setup_info[index].type = C4TV_SETUP_INFO_TYPE_E_SCAN;

        if ((NULL == service_uuid) || (0 == strncmp(service_uuid, "", strlen(service_uuid))))
        {
            BT_DBG_ERROR(BT_DEBUG_GATT, "[GATTS] service_uuid is NULL");
        }
        else
        {
            if (NULL != c4tv_setup_info[index].u.scan.service_uuid)
            {
                if (strlen(service_uuid) > strlen(c4tv_setup_info[index].u.scan.service_uuid))
                {
                    free(c4tv_setup_info[index].u.scan.service_uuid);
                    c4tv_setup_info[index].u.scan.service_uuid = NULL;
                    c4tv_setup_info[index].u.scan.service_uuid = malloc(strlen(service_uuid) + 1);
                    if (NULL == c4tv_setup_info[index].u.scan.service_uuid)
                    {
                        BT_DBG_ERROR(BT_DEBUG_GATT, "[GATTS] set device name, failed: %s", service_uuid);
                        return BT_ERR_STATUS_NOMEM;
                    }
                }
            }
            else
            {
                c4tv_setup_info[index].u.scan.service_uuid = malloc(strlen(service_uuid) + 1);
                if (NULL == c4tv_setup_info[index].u.scan.service_uuid)
                {
                    BT_DBG_ERROR(BT_DEBUG_GATT, "[GATTS] set device name, failed: %s", service_uuid);
                    return BT_ERR_STATUS_NOMEM;
                }
            }

            memcpy(c4tv_setup_info[index].u.scan.bt_service_uuid, srvc_uuid, BT_GATT_MAX_UUID_LEN);
            strncpy(c4tv_setup_info[index].u.scan.service_uuid, service_uuid, strlen(service_uuid));
            c4tv_setup_info[index].u.scan.service_uuid[strlen(service_uuid)] = '\0';
        }

        if ((NULL == advertised_uuid) || (0 == strncmp(advertised_uuid, "", strlen(advertised_uuid))))
        {
            BT_DBG_ERROR(BT_DEBUG_GATT, "[GATTS] advertised_uuid is NULL");
        }
        else
        {
            if (NULL != c4tv_setup_info[index].u.scan.advertised_uuid)
            {
                if (strlen(advertised_uuid) > strlen(c4tv_setup_info[index].u.scan.advertised_uuid))
                {
                    free(c4tv_setup_info[index].u.scan.advertised_uuid);
                    c4tv_setup_info[index].u.scan.advertised_uuid = NULL;
                    c4tv_setup_info[index].u.scan.advertised_uuid = malloc(strlen(advertised_uuid) + 1);
                    if (NULL == c4tv_setup_info[index].u.scan.advertised_uuid)
                    {
                        BT_DBG_ERROR(BT_DEBUG_GATT, "[GATTS] set device name, failed: %s", advertised_uuid);
                        return BT_ERR_STATUS_NOMEM;
                    }
                }
            }
            else
            {
                c4tv_setup_info[index].u.scan.advertised_uuid = malloc(strlen(advertised_uuid) + 1);
                if (NULL == c4tv_setup_info[index].u.scan.advertised_uuid)
                {
                    BT_DBG_ERROR(BT_DEBUG_GATT, "[GATTS] set device name, failed: %s", advertised_uuid);
                    return BT_ERR_STATUS_NOMEM;
                }
            }

            memcpy(c4tv_setup_info[index].u.scan.bt_advertised_uuid, adv_uuid, BT_GATT_MAX_UUID_LEN);
            strncpy(c4tv_setup_info[index].u.scan.advertised_uuid, advertised_uuid, strlen(advertised_uuid));
            c4tv_setup_info[index].u.scan.advertised_uuid[strlen(advertised_uuid)] = '\0';
        }

        if ((NULL == advertise_data) || (0 == strncmp(advertise_data, "", strlen(advertise_data))))
        {
            BT_DBG_ERROR(BT_DEBUG_GATT, "[GATTS] advertise_data is NULL");
        }
        else
        {
            if (NULL != c4tv_setup_info[index].u.scan.advertise_data)
            {
                if (strlen(advertise_data) > strlen(c4tv_setup_info[index].u.scan.advertise_data))
                {
                    free(c4tv_setup_info[index].u.scan.advertise_data);
                    c4tv_setup_info[index].u.scan.advertise_data = NULL;
                    c4tv_setup_info[index].u.scan.advertise_data = malloc(strlen(advertise_data) + 1);
                    if (NULL == c4tv_setup_info[index].u.scan.advertise_data)
                    {
                        BT_DBG_ERROR(BT_DEBUG_GATT, "[GATTS] set device name, failed: %s", advertise_data);
                        return BT_ERR_STATUS_NOMEM;
                    }
                }
            }
            else
            {
                c4tv_setup_info[index].u.scan.advertise_data = malloc(strlen(advertise_data) + 1);
                if (NULL == c4tv_setup_info[index].u.scan.advertise_data)
                {
                    BT_DBG_ERROR(BT_DEBUG_GATT, "[GATTS] set device name, failed: %s", advertise_data);
                    return BT_ERR_STATUS_NOMEM;
                }
            }

            strncpy(c4tv_setup_info[index].u.scan.advertise_data, advertise_data, strlen(advertise_data));
            c4tv_setup_info[index].u.scan.advertise_data[strlen(advertise_data)] = '\0';
        }

        if ((NULL == manufacturer_data) || (0 == strncmp(manufacturer_data, "", strlen(manufacturer_data))))
        {
            BT_DBG_ERROR(BT_DEBUG_GATT, "[GATTS] manufacturer_data is NULL");
        }
        else
        {
            if (NULL != c4tv_setup_info[index].u.scan.manufacturer_data)
            {
                if (strlen(manufacturer_data) > strlen(c4tv_setup_info[index].u.scan.manufacturer_data))
                {
                    free(c4tv_setup_info[index].u.scan.manufacturer_data);
                    c4tv_setup_info[index].u.scan.manufacturer_data = NULL;
                    c4tv_setup_info[index].u.scan.manufacturer_data = malloc(strlen(manufacturer_data) + 1);
                    if (NULL == c4tv_setup_info[index].u.scan.manufacturer_data)
                    {
                        BT_DBG_ERROR(BT_DEBUG_GATT, "[GATTS] set device name, failed: %s", manufacturer_data);
                        return BT_ERR_STATUS_NOMEM;
                    }
                }
            }
            else
            {
                c4tv_setup_info[index].u.scan.manufacturer_data = malloc(strlen(manufacturer_data) + 1);
                if (NULL == c4tv_setup_info[index].u.scan.manufacturer_data)
                {
                    BT_DBG_ERROR(BT_DEBUG_GATT, "[GATTS] set device name, failed: %s", manufacturer_data);
                    return BT_ERR_STATUS_NOMEM;
                }
            }

            strncpy(c4tv_setup_info[index].u.scan.manufacturer_data, manufacturer_data, strlen(manufacturer_data));
            c4tv_setup_info[index].u.scan.manufacturer_data[strlen(manufacturer_data)] = '\0';
        }
    }
    else
    {
//        char scan_advertise_data[32] = {0};
//        int scan_advertise_data_len = 0;

//        scan_advertise_data_len = ascii_2_hex(advertise_data, strlen(advertise_data), (unsigned char *)scan_advertise_data);
        c_btm_gattc_set_local_le_name(btm_client_if, p_deviceName);
        c_btm_gattc_set_local_disc_mode(btm_client_if, 6);

        usleep(100*1000);
#if 0
        ret = c_btm_bt_gattc_multi_adv_setdata(btm_client_if,
                                               TRUE,
                                               transmit_name,
                                               FALSE,
                                               0,
                                               strlen(manufacturer_data),
                                               manufacturer_data,
                                               0,       //  scan_advertise_data_len,
                                               NULL,    //  scan_advertise_data,
                                               0,       //  strlen(advertised_uuid),
                                               NULL);
#endif
        ret = c_btm_bt_gattc_multi_adv_setdata(btm_client_if,
                                               TRUE,
                                               transmit_name,
                                               FALSE,
                                               0,
                                               0,
                                               NULL,
                                               0,       //  scan_advertise_data_len,
                                               NULL,    //  scan_advertise_data,
                                               0,       //  strlen(advertised_uuid),
                                               NULL);
        if (BT_SUCCESS == ret)
        {
            BT_DBG_INFO(BT_DEBUG_GATT,"[GATTS] Set scan response for start server uuid:'%s'", service_uuid);
        }
        else
        {
            BT_DBG_ERROR(BT_DEBUG_GATT, "[GATTS] Set scan response for start server failed, uuid:'%s'", service_uuid);
        }
    }
    return ret;
}

/**
 * FUNCTION NAME: c_btm_gatt_addCharacteristic
 * PURPOSE:
 *      The function is used to create a GATT characteristic, this should be done prior to starting.
 * INPUT:
 *      service_uuid         -- the created service uuid
 *      attribute_uuid       -- the uuid of the characteristic to add.
 *      control_uuid         -- gatt characteristics have a max length of 512 btyes. For data longer than this,
 *                              there is an additional control_uuid to specify which 512 btye block of that data
 *                              should be displayed in th characteristic with attribute_id, it should be readable
 *                              and writable.
 *      option               -- list if options(read,write,notify).
 * OUTPUT:
 *      None
 * RETURN:
 *      BT_SUCCESS                    -- Operate success.
 *      BT_ERR_STATUS_NOMEM           -- no memory.
 *      BT_ERR_STATUS_PARM_INVALID    -- paramter is invalid.
 *      BT_ERR_STATUS_FAIL                -- Operate fail.
 * NOTES:
 *      None
 */
EXPORT_SYMBOL INT32 c_btm_gatt_addCharacteristic(CHAR * service_uuid, CHAR * attribute_uuid, CHAR * control_uuid, CHAR * option)
{
    //FUNC_ENTRY;

    BT_DBG_INFO(BT_DEBUG_GATT, "[GATT] service_uuid = %s, attribute_uuid = %s, control_uuid = %s, option = %s",
                service_uuid, attribute_uuid, control_uuid, option);

    UINT32 index = 0;
    c4tv_setup_info_t setup_info;
    c4tv_uuid_handle_t uuid_handle_info;
    //bt_uuid_t srvc_uuid;
    //bt_uuid_t attr_uuid;
    //bt_uuid_t ctrl_uuid;
    INT32 ret = BT_SUCCESS;
    CHAR srvc_uuid[BT_GATT_MAX_UUID_LEN] = {0};
    CHAR attr_uuid[BT_GATT_MAX_UUID_LEN] = {0};
    CHAR ctrl_uuid[BT_GATT_MAX_UUID_LEN] = {0};
    //linuxbt_uuid_stoh(service_uuid, &srvc_uuid);
    //linuxbt_uuid_stoh(attribute_uuid, &attr_uuid);
    //linuxbt_uuid_stoh(control_uuid, &ctrl_uuid);
    bluetooth_uuid_stos(service_uuid, srvc_uuid);
    bluetooth_uuid_stos(attribute_uuid, attr_uuid);
    bluetooth_uuid_stos(control_uuid, ctrl_uuid);

    setup_info.type = C4TV_SETUP_INFO_TYPE_E_CHAR;
    memcpy(setup_info.u.attr.bt_attribute_uuid, attr_uuid, BT_GATT_MAX_UUID_LEN);

    index = c_btm_gatt_findSetupInfo(&setup_info);
    if (MAX_C4TV_SETUP_INFO == index)
    {
        index = c_btm_gatt_allocNewSetupInfo();
        if (MAX_C4TV_SETUP_INFO == index)
        {
            BT_DBG_ERROR(BT_DEBUG_GATT, "[GATTS] add characteristic failed, uuid:'%s'", service_uuid);
            return BT_ERR_STATUS_FAIL;
        }
    }

    c4tv_setup_info[index].type = C4TV_SETUP_INFO_TYPE_E_CHAR;
    BT_DBG_INFO(BT_DEBUG_GATT, "%s,%d,index = %d", __FUNCTION__, __LINE__, index);
    if ((NULL == service_uuid) || (0 == strncmp(service_uuid, "", strlen(service_uuid))))
    {
        BT_DBG_ERROR(BT_DEBUG_GATT, "[GATTS] service_uuid is NULL");
    }
    else
    {
        if (NULL != c4tv_setup_info[index].u.attr.service_uuid)
        {
            if (strlen(service_uuid) > strlen(c4tv_setup_info[index].u.attr.service_uuid))
            {
                free(c4tv_setup_info[index].u.attr.service_uuid);
                c4tv_setup_info[index].u.attr.service_uuid = NULL;
                c4tv_setup_info[index].u.attr.service_uuid = malloc(strlen(service_uuid) + 1);
                if (NULL == c4tv_setup_info[index].u.attr.service_uuid)
                {
                    BT_DBG_ERROR(BT_DEBUG_GATT, "[GATTS] add char failed: %s", service_uuid);
                    return BT_ERR_STATUS_NOMEM;
                }
                memset(c4tv_setup_info[index].u.attr.service_uuid, 0, strlen(service_uuid) + 1);
            }
        }
        else
        {
            c4tv_setup_info[index].u.attr.service_uuid = malloc(strlen(service_uuid) + 1);
            if (NULL == c4tv_setup_info[index].u.attr.service_uuid)
            {
                BT_DBG_ERROR(BT_DEBUG_GATT, "[GATTS] add char failed: %s", service_uuid);
                return BT_ERR_STATUS_NOMEM;
            }
            memset(c4tv_setup_info[index].u.attr.service_uuid, 0, strlen(service_uuid) + 1);
        }

        memcpy(c4tv_setup_info[index].u.attr.bt_service_uuid, srvc_uuid, BT_GATT_MAX_UUID_LEN);
        strncpy(c4tv_setup_info[index].u.attr.service_uuid, service_uuid, strlen(service_uuid));
        c4tv_setup_info[index].u.attr.service_uuid[strlen(service_uuid)] = '\0';
    }

    if ((NULL == attribute_uuid) || (0 == strncmp(attribute_uuid, "", strlen(attribute_uuid))))
    {
        BT_DBG_ERROR(BT_DEBUG_GATT, "[GATTS] attribute_uuid is NULL");
    }
    else
    {
        if (NULL != c4tv_setup_info[index].u.attr.attribute_uuid)
        {
            if (strlen(attribute_uuid) > strlen(c4tv_setup_info[index].u.attr.attribute_uuid))
            {
                free(c4tv_setup_info[index].u.attr.attribute_uuid);
                c4tv_setup_info[index].u.attr.attribute_uuid = NULL;
                c4tv_setup_info[index].u.attr.attribute_uuid = malloc(strlen(attribute_uuid) + 1);
                if (NULL == c4tv_setup_info[index].u.attr.attribute_uuid)
                {
                    BT_DBG_ERROR(BT_DEBUG_GATT, "[GATTS] add char failed: %s", attribute_uuid);
                    return BT_ERR_STATUS_NOMEM;
                }
            }
        }
        else
        {
            c4tv_setup_info[index].u.attr.attribute_uuid = malloc(strlen(attribute_uuid) + 1);
            if (NULL == c4tv_setup_info[index].u.attr.attribute_uuid)
            {
                BT_DBG_ERROR(BT_DEBUG_GATT, "[GATTS] add char failed: %s", attribute_uuid);
                return BT_ERR_STATUS_NOMEM;
            }
        }

        memcpy(c4tv_setup_info[index].u.attr.bt_attribute_uuid, attr_uuid, BT_GATT_MAX_UUID_LEN);
        strncpy(c4tv_setup_info[index].u.attr.attribute_uuid, attribute_uuid, strlen(attribute_uuid));
        c4tv_setup_info[index].u.attr.attribute_uuid[strlen(attribute_uuid)] = '\0';
    }

    if ((NULL == control_uuid) || (0 == strncmp(control_uuid, "", strlen(control_uuid))))
    {
        BT_DBG_NOTICE(BT_DEBUG_GATT, "[GATTS] control_uuid is NULL");
    }
    else
    {
        if (NULL != c4tv_setup_info[index].u.attr.control_uuid)
        {
            if (strlen(control_uuid) > strlen(c4tv_setup_info[index].u.attr.control_uuid))
            {
                free(c4tv_setup_info[index].u.attr.control_uuid);
                c4tv_setup_info[index].u.attr.control_uuid = NULL;
                c4tv_setup_info[index].u.attr.control_uuid = malloc(strlen(control_uuid) + 1);
                if (NULL == c4tv_setup_info[index].u.attr.control_uuid)
                {
                    BT_DBG_ERROR(BT_DEBUG_GATT, "[GATTS] add char failed: %s", control_uuid);
                    return BT_ERR_STATUS_NOMEM;
                }
            }
        }
        else
        {
            c4tv_setup_info[index].u.attr.control_uuid = malloc(strlen(control_uuid) + 1);
            if (NULL == c4tv_setup_info[index].u.attr.control_uuid)
            {
                BT_DBG_ERROR(BT_DEBUG_GATT, "[GATTS] add char failed: %s", control_uuid);
                return BT_ERR_STATUS_NOMEM;
            }
        }

        memcpy(c4tv_setup_info[index].u.attr.bt_control_uuid, ctrl_uuid, BT_GATT_MAX_UUID_LEN);
        strncpy(c4tv_setup_info[index].u.attr.control_uuid, control_uuid, strlen(control_uuid));
        c4tv_setup_info[index].u.attr.control_uuid[strlen(control_uuid)] = '\0';
        c4tv_setup_info[index].u.attr.control_index = 0;
    }

    if (0 == strcmp(option, "read"))
    {
        c4tv_setup_info[index].u.attr.option |= 0x1;
    }
    else if (0 == strcmp(option, "write"))
    {
        c4tv_setup_info[index].u.attr.option |= 0x2;
    }
    else if (0 == strcmp(option, "notify"))
    {
        c4tv_setup_info[index].u.attr.option |= 0x4;
    }
    else
    {
        BT_DBG_ERROR(BT_DEBUG_GATT, "[GATTS] Add Characteristic Value failed, option is %s", option);
    }
    BT_DBG_INFO(BT_DEBUG_GATT, "%s,%d,c4tv_setup_info[%d].u.attr.bt_control_uuid = %s", __FUNCTION__,
                __LINE__, index, c4tv_setup_info[index].u.attr.bt_control_uuid);
    BT_DBG_INFO(BT_DEBUG_GATT, "%s,%d,c4tv_setup_info[%d].u.attr.bt_attribute_uuid = %s", __FUNCTION__,
                __LINE__, index, c4tv_setup_info[index].u.attr.bt_attribute_uuid);
    BT_DBG_INFO(BT_DEBUG_GATT, "%s,%d,c4tv_setup_info[%d].u.attr.bt_service_uuid = %s", __FUNCTION__,
                __LINE__, index, c4tv_setup_info[index].u.attr.bt_service_uuid);

    uuid_handle_info.type = C4TV_UUID_HANDLE_TYPE_E_CHAR;
    memcpy(uuid_handle_info.uuid, attr_uuid, BT_GATT_MAX_UUID_LEN);

    index = c_btm_gatt_findHandleByUuid(&uuid_handle_info);
    if (MAX_C4TV_UUID_HANDLE == index)
    {
        index = c_btm_gatt_allocNewUuidHandle();
        if (MAX_C4TV_UUID_HANDLE == index)
        {
            BT_DBG_ERROR(BT_DEBUG_GATT, "[GATTS] add char failed");
            return BT_ERR_STATUS_FAIL;
        }
    }

    c4tv_uuid_handle[index].type = C4TV_UUID_HANDLE_TYPE_E_CHAR;
    memcpy(c4tv_uuid_handle[index].uuid, attr_uuid, BT_GATT_MAX_UUID_LEN);

    if ((NULL != control_uuid) && (0 != strncmp(control_uuid, "", strlen(control_uuid))))
    {
        uuid_handle_info.type = C4TV_UUID_HANDLE_TYPE_E_CHAR;
        memcpy(uuid_handle_info.uuid, ctrl_uuid, BT_GATT_MAX_UUID_LEN);

        index = c_btm_gatt_findHandleByUuid(&uuid_handle_info);
        if (MAX_C4TV_UUID_HANDLE == index)
        {
            index = c_btm_gatt_allocNewUuidHandle();
            if (MAX_C4TV_UUID_HANDLE == index)
            {
                BT_DBG_ERROR(BT_DEBUG_GATT, "[GATTS] add char failed");
                return BT_ERR_STATUS_FAIL;
            }
        }
        c4tv_uuid_handle[index].type = C4TV_UUID_HANDLE_TYPE_E_CHAR;
        memcpy(c4tv_uuid_handle[index].uuid, ctrl_uuid, BT_GATT_MAX_UUID_LEN);
    }
    return ret;
}

/**
 * FUNCTION NAME: c_btm_gatt_setCharacteristicValue
 * PURPOSE:
 *      The function is used to set the value of a readable attribute.
 * INPUT:
 *      service_uuid         -- the created service uuid
 *      attribute_uuid       -- uthe uuid of the char to set value.
 *      value                -- new value to associate with the characteristic. The value may be truncated to 512 bytes
 *                              if a control_uuid was not specified when the attribute was created.
 *      length               -- the value length.
 * OUTPUT:
 *      None
 * RETURN:
 *      BT_SUCCESS                    -- Operate success.
 *      BT_ERR_STATUS_NOMEM           -- no memory.
 *      BT_ERR_STATUS_PARM_INVALID    -- paramter is invalid.
 *      BT_ERR_STATUS_FAIL                -- Operate fail.
 * NOTES:
 *      None
 */
EXPORT_SYMBOL INT32 c_btm_gatt_setCharacteristicValue(CHAR * service_uuid, CHAR  * attribute_uuid, CHAR * value, UINT32 length)
{
    //FUNC_ENTRY;
    //CC_TEMP;
    BT_DBG_INFO(BT_DEBUG_GATT, "[GATT] service_uuid = %s, attribute_uuid = %s, value = %p, length = %d",
                 service_uuid, attribute_uuid, value, length);
    //if (0 == strncmp(attribute_uuid, "0B7C6160FAB211E49FBB0002A5D5C51B", strlen(attribute_uuid)))
    //{
    //	BT_DBG_ERROR(BT_DEBUG_GATT, "[GATT] service_uuid = %s, attribute_uuid = %s, value = %s, length = %d, value size = %d",
    //                service_uuid, attribute_uuid, value, length, sizeof(value));

    //}

    //bt_uuid_t srvc_uuid;
    //bt_uuid_t attr_uuid;
    CHAR srvc_uuid[BT_GATT_MAX_UUID_LEN] = {0};
    CHAR attr_uuid[BT_GATT_MAX_UUID_LEN] = {0};
    INT32 ret = BT_SUCCESS;
    bluetooth_uuid_stos(service_uuid, srvc_uuid);
    bluetooth_uuid_stos(attribute_uuid, attr_uuid);
    //linuxbt_uuid_stoh(service_uuid, &srvc_uuid);
    //linuxbt_uuid_stoh(attribute_uuid, &attr_uuid);

    UINT32 index = 0;
    c4tv_setup_info_t info;
    info.type = C4TV_SETUP_INFO_TYPE_E_CHAR;
    c_btm_gatt_data_buf_lock();
    memcpy(info.u.attr.bt_attribute_uuid, attr_uuid, BT_GATT_MAX_UUID_LEN);

    index = c_btm_gatt_findSetupInfo(&info);
    if (MAX_C4TV_SETUP_INFO == index)
    {
        c_btm_gatt_data_buf_unlock();
        if (err_log_flag < 30)
        {
            BT_DBG_ERROR(BT_DEBUG_GATT, "[GATTS] set characteristic value failed, uuid:'%s'", service_uuid);
            err_log_flag++;
        }
        return BT_ERR_STATUS_FAIL;
    }

    if (NULL == value)
    {
        c_btm_gatt_data_buf_unlock();
        BT_DBG_ERROR(BT_DEBUG_GATT, "[GATTS] value is NULL");
        return BT_ERR_STATUS_PARM_INVALID;
    }
    else
    {
        if (NULL != c4tv_setup_info[index].u.attr.value)
        {
            if (length >= ATTRIBUTE_VALUE_MEM_SIZE)
            {
                //BT_DBG_ERROR(BT_DEBUG_GATT, "[GATTS] length = %d", length);
                free(c4tv_setup_info[index].u.attr.value);
                c4tv_setup_info[index].u.attr.value = NULL;
                c4tv_setup_info[index].u.attr.value = malloc(length);
                if (NULL == c4tv_setup_info[index].u.attr.value)
                {
                    c_btm_gatt_data_buf_unlock();
                    BT_DBG_ERROR(BT_DEBUG_GATT, "[GATTS] set Characteristic Value failed");
                    return BT_ERR_STATUS_NOMEM;
                }

                free(c4tv_setup_info[index].u.attr.read_value);
                c4tv_setup_info[index].u.attr.read_value = NULL;
                c4tv_setup_info[index].u.attr.read_value = malloc(length);
                if (NULL == c4tv_setup_info[index].u.attr.read_value)
                {
                    c_btm_gatt_data_buf_unlock();
                    BT_DBG_ERROR(BT_DEBUG_GATT, "[GATTS] set Characteristic Value failed");
                    return BT_ERR_STATUS_NOMEM;
                }
            }
        }
        else
        {
            c4tv_setup_info[index].u.attr.value = malloc(ATTRIBUTE_VALUE_MEM_SIZE);
            if (NULL == c4tv_setup_info[index].u.attr.value)
            {
                c_btm_gatt_data_buf_unlock();
                BT_DBG_ERROR(BT_DEBUG_GATT, "[GATTS] set Characteristic Value failed");
                return BT_ERR_STATUS_NOMEM;
            }

            c4tv_setup_info[index].u.attr.read_value = malloc(ATTRIBUTE_VALUE_MEM_SIZE);
            if (NULL == c4tv_setup_info[index].u.attr.read_value)
            {
                c_btm_gatt_data_buf_unlock();
                BT_DBG_ERROR(BT_DEBUG_GATT, "[GATTS] set Characteristic Value failed");
                return BT_ERR_STATUS_NOMEM;
            }
        }
        BT_DBG_INFO(BT_DEBUG_GATT, "c4tv_setup_info[%d].u.attr.value = %p, length = %d", index,
            c4tv_setup_info[index].u.attr.value, length);
        BT_DBG_INFO(BT_DEBUG_GATT, "c4tv_setup_info[%d].u.attr.bt_attribute_uuid = %s", index,
            c4tv_setup_info[index].u.attr.bt_attribute_uuid);
        memcpy(c4tv_setup_info[index].u.attr.value, value, length);
        c4tv_setup_info[index].u.attr.value_len = length;
        //c4tv_setup_info[index].u.attr.control_index = 0;
        c_btm_gatt_data_buf_unlock();
    }
    //CC_TEMP;
    return ret;
}
#if 0
VOID cq_test(VOID)
{
    INT32 index = 0;
    for (index = 0; index < MAX_C4TV_SETUP_INFO; index++)
    {
        switch (c4tv_setup_info[index].type)
        {
        case C4TV_SETUP_INFO_TYPE_E_ADV:
            DBG_LOG_PRINT(("c4tv_setup_info Index: %ld, type:%d, service_uuid: %s, advertised_uuid:%s",
                           (long)index, c4tv_setup_info[index].type,
                           c4tv_setup_info[index].u.adv.service_uuid,
                           c4tv_setup_info[index].u.adv.advertised_uuid));
            break;
        case C4TV_SETUP_INFO_TYPE_E_SCAN:
            DBG_LOG_PRINT(("c4tv_setup_info Index: %ld, type:%d, service_uuid: %s, advertised_uuid:%s",
                           (long)index, c4tv_setup_info[index].type,
                           c4tv_setup_info[index].u.scan.service_uuid,
                           c4tv_setup_info[index].u.scan.advertised_uuid));
            break;
        case C4TV_SETUP_INFO_TYPE_E_CHAR:
            DBG_LOG_PRINT(("c4tv_setup_info Index: %ld, type:%d, service_uuid: %s, advertised_uuid:%s, control_uuid:%s",
                           (long)index, c4tv_setup_info[index].type,
                           c4tv_setup_info[index].u.attr.service_uuid,
                           c4tv_setup_info[index].u.attr.attribute_uuid,
                           c4tv_setup_info[index].u.attr.control_uuid));
            break;
        default:
            continue;
        }
    }

    for (index = 0; index < MAX_C4TV_UUID_HANDLE; index++)
    {
        if ((c4tv_uuid_handle[index].type != C4TV_UUID_HANDLE_TYPE_E_SRVC) && (c4tv_uuid_handle[index].type != C4TV_UUID_HANDLE_TYPE_E_CHAR))
        {
            continue;
        }
    }

    return;
}
#endif
#endif



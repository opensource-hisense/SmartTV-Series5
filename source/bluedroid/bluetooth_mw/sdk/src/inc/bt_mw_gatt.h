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

/* FILE NAME:  bt_mw_gatt.h
 * AUTHOR: Xuemei Yang
 * PURPOSE:
 *      It provides GATTC and GATTS API to c_bt_mw_gatt and other mw layer modules.
 * NOTES:
 */


#ifndef __BT_MW_GATT_H__
#define __BT_MW_GATT_H__

#include "u_bt_mw_gatt.h"

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
INT32 bluetooth_gatt_base_init(BT_APP_GATTC_CB_FUNC_T *func);

/**
 * FUNCTION NAME: bluetooth_gattc_register_app_sync
 * PURPOSE:
 *      The function is used to synchronous register gatt client APP.
 * INPUT:
 *      uuid               -- app uuid
 * OUTPUT:
 *      client_if          -- registered app identifer
 * RETURN:
 *	   BT_SUCCESS					     -- Operate success.
 *      BT_ERR_STATUS_FAIL				-- Operate fail.
 *      BT_ERR_STATUS_PARM_INVALID		-- paramter is invalid.
 * NOTES:
 *      None
 */
INT32 bluetooth_gattc_register_app_sync(CHAR *pt_uuid, INT32 *client_if);

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
INT32 bluetooth_gattc_register_app(CHAR *pt_uuid);

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
INT32 bluetooth_gattc_unregister_app(INT32 client_if);

/**
 * FUNCTION NAME: bluetooth_gattc_scan
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
INT32 bluetooth_gattc_scan();

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
INT32 bluetooth_gattc_stop_scan();

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
INT32 bluetooth_gattc_open(INT32 client_if, CHAR *bt_addr, UINT8 is_direct, INT32 transport);

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
INT32 bluetooth_gattc_close(INT32 client_if, CHAR *bt_addr, INT32 conn_id);

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
INT32 bluetooth_gattc_listen(INT32 client_if);

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
INT32 bluetooth_gattc_refresh(INT32 client_if, CHAR *bt_addr);

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
INT32 bluetooth_gattc_search_service(INT32 conn_id, CHAR *pt_uuid);

INT32 bluetooth_gattc_get_gatt_db(INT32 conn_id);

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
                               INT32 start_inst_id);

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
                                INT32 start_inst_id);

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
                                       INT32 incl_inst_id);

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
                                INT32 auth_req);

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
                                 INT32 auth_req);

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
                                 CHAR *value);

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
                                  CHAR *value);

INT32 bluetooth_gattc_execute_write(INT32 conn_id,
                                    INT32 execute);


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
                               INT32 char_handle);

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
                                 INT32 char_handle);

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
INT32 bluetooth_gattc_read_rssi(INT32 client_if, CHAR *bt_addr);

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
VOID bluetooth_gattc_get_connect_result_info(BT_GATTC_CONNECT_RST_T *connect_rst_info);

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
VOID bluetooth_gattc_get_disconnect_result_info(BT_GATTC_CONNECT_RST_T *disconnect_rst_info);

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
VOID bluetooth_gattc_read_rssi_result_info(BT_GATTC_GET_REMOTE_RSSI_T *get_remote_rssi_info);

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
        INT32 timeout);

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
INT32 bluetooth_gattc_set_scan_parameters(INT32 client_if, INT32 scan_interval, INT32 scan_window);

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
                                       INT32 timeout_s);

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
        INT32 timeout_s);

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
                                       INT32 timeout_s);

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
                                        CHAR* service_uuid);

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
        CHAR* service_uuid);

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
INT32 bluetooth_gattc_multi_adv_disable(INT32 client_if);

INT32 bluetooth_gattc_batchscan_cfg_storage(INT32 client_if, INT32 batch_scan_full_max,
                                                INT32 batch_scan_trunc_max, INT32 batch_scan_notify_threshold);

INT32 bluetooth_gattc_batchscan_enb_batch_scan(INT32 client_if, INT32 scan_mode,
                        INT32 scan_interval, INT32 scan_window, INT32 addr_type, INT32 discard_rule);

INT32 bluetooth_gattc_batchscan_dis_batch_scan(INT32 client_if);

INT32 bluetooth_gattc_batchscan_read_reports(INT32 client_if, INT32 scan_mode);

INT32 bluetooth_gattc_configure_mtu(INT32 conn_id, INT32 mtu);

#if MTK_LINUX_GATTC_LE_NAME //== TRUE
INT32 bluetooth_gattc_set_local_le_name(INT32 client_if, CHAR *name);
#endif

#if MTK_LINUX_GATTC_RPA
INT32 bluetooth_gattc_get_local_adv_rpa(INT32 client_if);
#endif

#if MTK_LINUX_GATTC_DISC_MODE
INT32 bluetooth_gattc_set_local_disc_mode(INT32 client_if, INT32 disc_mode);
#endif

#if MTK_LINUX_GATTC_PTS_TEST
INT32 bluetooth_gattc_read_using_char_uuid(INT32 conn_id,
                                                 INT32 start_handle,
                                                 INT32 end_handle,
                                                 CHAR *uuid,
                                                 INT32 auth_req);

INT32 bluetooth_gattc_read_long_characteristic(INT32 conn_id,
                                                 INT32 handle,
                                                 INT32 offset,
                                                 INT32 auth_req);

INT32 bluetooth_gattc_read_multi_characteristic(INT32 conn_id,
                                                 INT32 num_attr,
                                                 UINT16 *handles,
                                                 INT32 auth_req);

INT32 bluetooth_gattc_read_long_descr(INT32 conn_id,
                                                 INT32 handle,
                                                 INT32 offset,
                                                 INT32 auth_req);

INT32 bluetooth_gattc_write_long_characteristic(INT32 conn_id,
                                                         INT32 char_handle,
                                                         INT32 write_type,
                                                         INT32 len,
                                                         INT32 offset,
                                                         INT32 auth_req,
                                                         CHAR *value);

INT32 bluetooth_gattc_write_long_descr(INT32 conn_id,
                                                 INT32 descr_handle,
                                                 INT32 write_type,
                                                 INT32 len,
                                                 INT32 offset,
                                                 INT32 auth_req,
                                                 CHAR *value);

INT32 bluetooth_gattc_set_pts_flag(UINT8 pts_flag);
#endif

INT32 bluetooth_gattc_test_command(INT32 command, BT_GATTC_TEST_PARAMS_T test_params);

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
VOID bluetooth_gatt_parse_adv_data (UINT8* adv_data);

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
VOID bluetooth_gattc_register_client_cbk(INT32 status, INT32 client_if, CHAR *app_uuid);

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
VOID bluetooth_gattc_scan_result_cbk(CHAR* bda, INT32 rssi, UINT8* adv_data);

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
VOID bluetooth_gattc_connect_cbk(INT32 conn_id, INT32 status, INT32 client_if, CHAR* bda);

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
VOID bluetooth_gattc_disconnect_cbk(INT32 conn_id, INT32 status, INT32 client_if, CHAR* bda);

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
VOID bluetooth_gattc_search_complete_cbk(INT32 conn_id, INT32 status);

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
VOID bluetooth_gattc_search_result_cbk(INT32 conn_id, BT_GATTS_SRVC_ID_T *srvc_id);

VOID bluetooth_gattc_get_gatt_db_cbk(INT32 conn_id, bluetooth_gatt_db_element_t *gatt_db, INT32 count);

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
        BT_GATT_ID_T *char_id, INT32 char_prop);

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
                                        BT_GATT_ID_T *char_id, BT_GATT_ID_T *descr_id);

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
        BT_GATTS_SRVC_ID_T *incl_srvc_id);

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
VOID bluetooth_gattc_register_for_notification_cbk(INT32 conn_id, INT32 registered, INT32 status, UINT16 handle);

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
VOID bluetooth_gattc_notify_cbk(INT32 conn_id, BT_GATT_NOTI_PARAMS_T *p_data);

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
VOID bluetooth_gattc_read_characteristic_cbk(INT32 conn_id, INT32 status, BT_GATT_READ_PARAMS_T *p_data);

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
VOID bluetooth_gattc_write_characteristic_cbk(INT32 conn_id, INT32 status, UINT16 handle);

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
VOID bluetooth_gattc_read_descriptor_cbk(INT32 conn_id, INT32 status, BT_GATT_READ_PARAMS_T *p_data);

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
VOID bluetooth_gattc_write_descriptor_cbk(INT32 conn_id, INT32 status, UINT16 handle);

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
VOID bluetooth_gattc_execute_write_cbk(INT32 conn_id, INT32 status);

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
VOID bluetooth_gattc_read_remote_rssi_cbk(INT32 client_if, CHAR* bda, INT32 rssi, INT32 status);

VOID bluetooth_gattc_scan_filter_cfg_cbk(INT32 action, INT32 client_if, INT32 status, INT32 filt_type, INT32 avbl_space);

VOID bluetooth_gattc_scan_filter_param_cbk(INT32 action, INT32 client_if, INT32 status, INT32 avbl_space);

VOID bluetooth_gattc_scan_filter_status_cbk(INT32 enable, INT32 client_if, INT32 status);

VOID bluetooth_gattc_multi_adv_enable_cbk(INT32 client_if, INT32 status);
VOID bluetooth_gattc_multi_adv_data_cbk(INT32 client_if, INT32 status);


/*GATT server*/
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
INT32 bluetooth_gatts_base_init(BT_APP_GATTS_CB_FUNC_T *func);

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
INT32 bluetooth_gatts_register_app_sync(CHAR *pt_uuid, INT32 *server_if);

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
INT32 bluetooth_gatts_register_server(CHAR *pt_uuid);

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
INT32 bluetooth_gatts_unregister_server(INT32 server_if);

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
INT32 bluetooth_gatts_open(INT32 server_if, CHAR *bt_addr, UINT8 is_direct, INT32 transport);

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
INT32 bluetooth_gatts_close(INT32 server_if, CHAR *bt_addr, INT32 conn_id);

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
                                       INT32 number, INT32 *srvc_handle);

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
INT32 bluetooth_gatts_add_service(INT32 server_if,
                                  CHAR *pt_service_uuid,
                                  UINT8 is_primary,
                                  INT32 number);

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
INT32 bluetooth_gatts_add_included_service(INT32 server_if,
        INT32 service_handle,
        INT32 included_handle);

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
                                    INT32 permissions, INT32 *char_handle);

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
                               INT32 permissions);

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
INT32 bluetooth_gatts_add_desc(INT32 server_if,
                               INT32 service_handle,
                               CHAR *uuid,
                               INT32 permissions);

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
INT32 bluetooth_gatts_start_service(INT32 server_if, INT32 service_handle, INT32 transport);

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
INT32 bluetooth_gatts_start_service_sync(INT32 server_if, INT32 service_handle, INT32 transport);

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
INT32 bluetooth_gatts_stop_service(INT32 server_if, INT32 service_handle);

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
INT32 bluetooth_gatts_delete_service(INT32 server_if, INT32 service_handle);

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
                                      INT32 value_len);

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
                                    INT32 auth_req);

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
VOID bluetooth_gatts_get_connect_result_info(BT_GATTS_CONNECT_RST_T *connect_rst_info);

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
VOID bluetooth_gatts_get_disconnect_result_info(BT_GATTS_CONNECT_RST_T *disconnect_rst_info);

INT32 bluetooth_gattc_scan_filter_param_setup(bluetooth_gatt_filt_param_setup_t scan_filt_param);

INT32 bluetooth_gattc_scan_filter_enable(INT32 client_if);

INT32 bluetooth_gattc_scan_filter_disable(INT32 client_if);

INT32 bluetooth_gattc_scan_filter_add_remove(INT32 client_if, INT32 action,
        INT32 filt_type, INT32 filt_index, INT32 company_id,
        INT32 company_id_mask, const CHAR *p_uuid,
        const CHAR *p_uuid_mask, const CHAR *bd_addr,
        CHAR addr_type, INT32 data_len, CHAR* p_data, INT32 mask_len,
        CHAR* p_mask);

INT32 bluetooth_gattc_scan_filter_clear(INT32 client_if, INT32 filt_index);

INT32 bluetooth_gattc_get_device_type(CHAR *bt_addr);

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
VOID bluetooth_gatts_register_server_callback(INT32 status, INT32 server_if, CHAR *app_uuid);

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
        BT_GATTS_SRVC_ID_T *srvc_id, INT32 srvc_handle);

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
        INT32 connected, CHAR *bda);

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
        INT32 incl_srvc_handle);

/**
 * FUNCTION NAME: bluetooth_gatts_characteristic_added_callback
 * PURPOSE:
 *      The function is callback function of gatt server add characteristic
 * INPUT:
 *      status              -- gatt error code status
 *      server_if           -- registered server app identifier
 *      uuid                -- added characteristic uuid
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
        INT32 char_handle);

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
        INT32 descr_handle);

/**
 * FUNCTION NAME: bluetooth_gatts_request_read_callback
 * PURPOSE:
 *      The function is callback function of gatt server request read operation
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
        BOOL is_long);

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
        UINT8* value);

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
        INT32 exec_write);

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
VOID bluetooth_gatts_response_confirmation_callback(INT32 status, INT32 handle);

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
VOID bluetooth_gatts_indication_sent_callback(INT32 conn_id, INT32 status);

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
VOID bluetooth_gatts_congestion_callback(INT32 conn_id, BOOL congested);

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
VOID bluetooth_gatts_mtu_changed_callback(INT32 conn_id, INT32 mtu);

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
VOID bluetooth_gatts_service_started_callback(INT32 status, INT32 server_if, INT32 srvc_handle);

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
VOID bluetooth_gatts_service_stopped_callback(INT32 status, INT32 server_if, INT32 srvc_handle);

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
VOID bluetooth_gatts_service_deleted_callback(INT32 status, INT32 server_if, INT32 srvc_handle);


#endif/* __BT_MW_GATT_H__ */

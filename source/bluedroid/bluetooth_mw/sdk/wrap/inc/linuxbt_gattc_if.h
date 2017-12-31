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

/* FILE NAME:  linuxbt_gattc_if.h
 * AUTHOR: Xuemei Yang
 * PURPOSE:
 *      It provides GATTC operation interface definition to MW higher layer.
 * NOTES:
 */


/*-----------------------------------------------------------------------------
                    include files
 ----------------------------------------------------------------------------*/

#ifndef __LINUXBT_GATTC_IF_H__
#define __LINUXBT_GATTC_IF_H__

#define LINUXBT_GATTC_APP_UUID  "49557E50-D815-11E4-8830-0800200C9A66"

#include "u_bt_mw_gatt.h"
#include "bt_gatt.h"
#if defined(MTK_LINUX_GATT) && (MTK_LINUX_GATT == TRUE)
#include "mtk_bt_gatt.h"
#include "mtk_bluetooth.h"
#endif

/**
 * FUNCTION NAME: linuxbt_gattc_init
 * PURPOSE:
 *      The function is used to initialize gatt client inteface.
 * INPUT:
 *      interface            -- gatt client infrace structure
 * OUTPUT:
 *      None
 * RETURN:
 *      BT_SUCCESS             -- Operate success.
 * NOTES:
 *      None
 */
int linuxbt_gattc_init(const btgatt_client_interface_t *interface);

#if defined(MTK_LINUX_GATT) && (MTK_LINUX_GATT == TRUE)
int linuxbt_gattc_ex_init(const btgatt_ex_client_interface_t *ex_interface);
#endif

/**
 * FUNCTION NAME: linuxbt_gattc_deinit
 * PURPOSE:
 *      The function is used to deinitialize gatt client.
 * INPUT:
 *      None
 * OUTPUT:
 *      None
 * RETURN:
 *      BT_SUCCESS             -- Operate success.
 * NOTES:
 *      None
 */
int linuxbt_gattc_deinit(void);

/**
 * FUNCTION NAME: linuxbt_gattc_register_app
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
int linuxbt_gattc_register_app(char *pt_uuid);

/**
 * FUNCTION NAME: linuxbt_gattc_unregister_app
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
int linuxbt_gattc_unregister_app(int client_if);

/**
 * FUNCTION NAME: linuxbt_gattc_scan
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
int linuxbt_gattc_scan();

/**
 * FUNCTION NAME: linuxbt_gattc_stop_scan
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
int linuxbt_gattc_stop_scan();

/**
 * FUNCTION NAME: linuxbt_gattc_open
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
int linuxbt_gattc_open(int client_if, char *bt_addr, UINT8 is_direct, int transport);

/**
 * FUNCTION NAME: linuxbt_gattc_close
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
int linuxbt_gattc_close(int client_if, char *bt_addr, int conn_id);

/**
 * FUNCTION NAME: linuxbt_gattc_listen
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
int linuxbt_gattc_listen(int client_if);

/**
 * FUNCTION NAME: linuxbt_gattc_refresh
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
int linuxbt_gattc_refresh(int client_if, char *bt_addr);

/**
 * FUNCTION NAME: linuxbt_gattc_search_service
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
int linuxbt_gattc_search_service(int conn_id, char *pt_uuid);

/**
 * FUNCTION NAME: linuxbt_gattc_get_char
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
int linuxbt_gattc_get_char(int conn_id,
                           char *pt_service_uuid,
                           int service_inst_id,
                           char *pt_start_uuid,
                           int start_inst_id);

/**
 * FUNCTION NAME: linuxbt_gattc_get_descr
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
int linuxbt_gattc_get_descr(int conn_id,
                            char *pt_service_uuid,
                            int service_inst_id,
                            char *pt_char_uuid,
                            int char_inst_id,
                            char *pt_start_uuid,
                            int start_inst_id);

/**
 * FUNCTION NAME: linuxbt_gattc_get_incl_service
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
int linuxbt_gattc_get_incl_service(int conn_id,
                                   char *pt_service_uuid,
                                   int service_inst_id,
                                   char *pt_incl_uuid,
                                   int incl_inst_id);

/**
 * FUNCTION NAME: linuxbt_gattc_read_char
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
int linuxbt_gattc_read_char(int conn_id,
                            int char_handle,
                            int auth_req);

/**
 * FUNCTION NAME: linuxbt_gattc_read_descr
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
int linuxbt_gattc_read_descr(int conn_id,
                             int descr_handle,
                             int auth_req);

/**
 * FUNCTION NAME: linuxbt_gattc_write_char
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
int linuxbt_gattc_write_char(int conn_id,
                             int char_handle,
                             int write_type,
                             int len,
                             int auth_req,
                             char *value);

/**
 * FUNCTION NAME: linuxbt_gattc_write_descr
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
int linuxbt_gattc_write_descr(int conn_id,
                              int descr_handle,
                              int write_type,
                              int len,
                              int auth_req,
                              char *value);

/**
 * FUNCTION NAME: linuxbt_gattc_execute_write
 * PURPOSE:
 *      The function is used for gatt client execute write operation
 * INPUT:
 *      conn_id              -- connection id
 *      execute              -- execute write or not
 * OUTPUT:
 *      None
 * RETURN:
 *      BT_SUCCESS                        -- Operate success.
 *      BT_ERR_STATUS_FAIL                -- Operate fail.
 *      BT_ERR_STATUS_PARM_INVALID        -- paramter is invalid.
 * NOTES:
 *      None
 */
int linuxbt_gattc_execute_write(int conn_id, int execute);

/**
 * FUNCTION NAME: linuxbt_gattc_reg_noti
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
int linuxbt_gattc_reg_noti(int client_if,
                           char *bt_addr,
                           int char_handle);

/**
 * FUNCTION NAME: linuxbt_gattc_dereg_noti
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
int linuxbt_gattc_dereg_noti(int client_if,
                             char *bt_addr,
                             int char_handle);

/**
 * FUNCTION NAME: linuxbt_gattc_read_rssi
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
int linuxbt_gattc_read_rssi(int client_if, char *bt_addr);

/**
 * FUNCTION NAME: linuxbt_gattc_conn_parameter_update
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
int linuxbt_gattc_conn_parameter_update(char *pbt_addr,
                                        int min_interval,
                                        int max_interval,
                                        int latency,
                                        int timeout);

/**
 * FUNCTION NAME: linuxbt_gattc_set_scan_parameters
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
int linuxbt_gattc_set_scan_parameters(int client_if, int scan_interval, int scan_window);

/**
 * FUNCTION NAME: linuxbt_gattc_scan_filter_param_setup
 * PURPOSE:
 *      The function is used for gatt client to setup scan filter parameters
 * INPUT:
 *      None, not used
 * OUTPUT:
 *      None
 * RETURN:
 *      BT_SUCCESS                        -- Operate success.
 *      BT_ERR_STATUS_FAIL                -- Operate fail.
 *      BT_ERR_STATUS_PARM_INVALID        -- paramter is invalid.
 * NOTES:
 *      None
 */
int linuxbt_gattc_scan_filter_param_setup(bluetooth_gatt_filt_param_setup_t scan_filt_param);

/**
 * FUNCTION NAME: linuxbt_gattc_scan_filter_enable
 * PURPOSE:
 *      The function is used for gatt client to enable scan filter
 * INPUT:
 *      None, not used
 * OUTPUT:
 *      None
 * RETURN:
 *      BT_SUCCESS                        -- Operate success.
 *      BT_ERR_STATUS_FAIL                -- Operate fail.
 *      BT_ERR_STATUS_PARM_INVALID        -- paramter is invalid.
 * NOTES:
 *      None
 */
int linuxbt_gattc_scan_filter_enable(int client_if);

/**
 * FUNCTION NAME: linuxbt_gattc_scan_filter_disable
 * PURPOSE:
 *      The function is used for gatt client to disable scan filter
 * INPUT:
 *      None, not used
 * OUTPUT:
 *      None
 * RETURN:
 *      BT_SUCCESS                        -- Operate success.
 *      BT_ERR_STATUS_FAIL                -- Operate fail.
 *      BT_ERR_STATUS_PARM_INVALID        -- paramter is invalid.
 * NOTES:
 *      None
 */
int linuxbt_gattc_scan_filter_disable(int client_if);

/**
 * FUNCTION NAME: linuxbt_gattc_scan_filter_add_remove
 * PURPOSE:
 *      The function is used for gatt client to add or remove device from scan filter
 * INPUT:
 *      None, not used
 * OUTPUT:
 *      None
 * RETURN:
 *      BT_SUCCESS                        -- Operate success.
 *      BT_ERR_STATUS_FAIL                -- Operate fail.
 *      BT_ERR_STATUS_PARM_INVALID        -- paramter is invalid.
 * NOTES:
 *      None
 */
int linuxbt_gattc_scan_filter_add_remove(int client_if, int action,
        int filt_type, int filt_index, int company_id,
        int company_id_mask, const char *p_uuid,
        const char *p_uuid_mask, const char *bd_addr,
        char addr_type, int data_len, char* p_data, int mask_len,
        char* p_mask);

/**
 * FUNCTION NAME: linuxbt_gattc_scan_filter_clear
 * PURPOSE:
 *      The function is used for gatt client to clear device from scan filter
 * INPUT:
 *      None, not used
 * OUTPUT:
 *      None
 * RETURN:
 *      BT_SUCCESS                        -- Operate success.
 *      BT_ERR_STATUS_FAIL                -- Operate fail.
 *      BT_ERR_STATUS_PARM_INVALID        -- paramter is invalid.
 * NOTES:
 *      None
 */
int linuxbt_gattc_scan_filter_clear(int client_if, int filt_index);

/**
 * FUNCTION NAME: linuxbt_gattc_get_device_type
 * PURPOSE:
 *      The function is used for gatt client to get device type
 * INPUT:
 *      None, not used
 * OUTPUT:
 *      None
 * RETURN:
 *      BT_SUCCESS                        -- Operate success.
 *      BT_ERR_STATUS_FAIL                -- Operate fail.
 *      BT_ERR_STATUS_PARM_INVALID        -- paramter is invalid.
 * NOTES:
 *      None
 */
int linuxbt_gattc_get_device_type(char *bt_addr);

/**
 * FUNCTION NAME: linuxbt_gattc_set_adv_data
 * PURPOSE:
 *      The function is used for gatt to set advertising data
 * INPUT:
 *      client_if              -- registered client app identifier
 *      set_scan_rsp           -- set scan response flag
 *      include_name           -- include device name flag
 *      include_txpower        -- include transmit power flag
 *      min_interval           -- min advertising interval
 *      max_interval           -- max advertising interval
 *      appearance             -- appearance data
 *      manufacturer_len       -- manufacturer data length
 *      manufacturer_data      -- manufacturer data
 *      service_data_len       -- service data length
 *      service_data           -- service data
 *      service_uuid_len       -- service uuid length
 *      service_uuid           -- service uuid
 * OUTPUT:
 *      None
 * RETURN:
 *      BT_SUCCESS                        -- Operate success.
 *      BT_ERR_STATUS_FAIL                -- Operate fail.
 *      BT_ERR_STATUS_PARM_INVALID        -- paramter is invalid.
 * NOTES:
 *      None
 */
int linuxbt_gattc_set_adv_data(int client_if,
                               uint8_t set_scan_rsp,
                               uint8_t include_name,
                               uint8_t include_txpower,
                               int min_interval,
                               int max_interval,
                               int appearance,
                               uint16_t manufacturer_len,
                               char* manufacturer_data,
                               uint16_t service_data_len,
                               char* service_data,
                               uint16_t service_uuid_len,
                               char* service_uuid);

#if MTK_LINUX_GATTC_LE_NAME //== TRUE
int linuxbt_gattc_set_local_le_name(int client_if, char *name);
#endif

#if MTK_LINUX_GATTC_RPA
int linuxbt_gattc_get_local_adv_rpa(int client_if);
#endif

#if MTK_LINUX_GATTC_DISC_MODE
int linuxbt_gattc_set_local_disc_mode(int client_if, int disc_mode);
#endif

#if MTK_LINUX_GATTC_PTS_TEST
int linuxbt_gattc_read_using_char_uuid(int conn_id,
                                                 int start_handle,
                                                 int end_handle,
                                                 char *uuid,
                                                 int auth_req);

int linuxbt_gattc_read_long_characteristic(int conn_id, uint16_t handle,
                                                     uint16_t offset, int auth_req);

int linuxbt_gattc_read_multi_characteristic(int conn_id, uint8_t num_attr,
                                                      uint16_t *handles, int auth_req);

int linuxbt_gattc_read_long_descr(int conn_id, uint16_t handle,
                                          uint16_t offset, int auth_req);

int linuxbt_gattc_write_long_characteristic(int conn_id, uint16_t handle, int write_type,
                                         int len, uint16_t offset, int auth_req, char* p_value);

int linuxbt_gattc_write_long_descr(int conn_id, uint16_t handle, int write_type,
                                         int len, uint16_t offset, int auth_req, char* p_value);

int linuxbt_gattc_set_pts_flag(uint8_t pts_flag);
#endif


/**
 * FUNCTION NAME: linuxbt_gattc_configure_mtu
 * PURPOSE:
 *      The function is used for gatt client to config mtu
 * INPUT:
 *      None, not used
 * OUTPUT:
 *      None
 * RETURN:
 *      BT_SUCCESS                        -- Operate success.
 *      BT_ERR_STATUS_FAIL                -- Operate fail.
 *      BT_ERR_STATUS_PARM_INVALID        -- paramter is invalid.
 * NOTES:
 *      None
 */
int linuxbt_gattc_configure_mtu(int conn_id, int mtu);

/**
 * FUNCTION NAME: linuxbt_gattc_multi_adv_enable
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
int linuxbt_gattc_multi_adv_enable(int client_if,
                                   int min_interval,
                                   int max_interval,
                                   int adv_type,
                                   int chnl_map,
                                   int tx_power,
                                   int timeout_s);

/**
 * FUNCTION NAME: linuxbt_gattc_multi_adv_update
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
int linuxbt_gattc_multi_adv_update(int client_if,
                                   int min_interval,
                                   int max_interval,
                                   int adv_type,
                                   int chnl_map,
                                   int tx_power,
                                   int timeout_s);

/**
 * FUNCTION NAME: linuxbt_gattc_multi_adv_setdata
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
int linuxbt_gattc_multi_adv_setdata(int client_if,
                                    uint8_t set_scan_rsp,
                                    uint8_t include_name,
                                    uint8_t include_txpower,
                                    int appearance,
                                    int manufacturer_len,
                                    char* manufacturer_data,
                                    int service_data_len,
                                    char* service_data,
                                    int service_uuid_len,
                                    char* service_uuid);

/**
 * FUNCTION NAME: linuxbt_gattc_multi_adv_disable
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
int linuxbt_gattc_multi_adv_disable(int client_if);

/**
 * FUNCTION NAME: linuxbt_gattc_batchscan_cfg_storage
 * PURPOSE:
 *      The function is used for gatt client to config batch scan storage paramter
 * INPUT:
 *      None, not used
 * OUTPUT:
 *      None
 * RETURN:
 *      BT_SUCCESS                        -- Operate success.
 *      BT_ERR_STATUS_FAIL                -- Operate fail.
 *      BT_ERR_STATUS_PARM_INVALID        -- paramter is invalid.
 * NOTES:
 *      None
 */
int linuxbt_gattc_batchscan_cfg_storage(int client_if,int batch_scan_full_max,
                                              int batch_scan_trunc_max, int batch_scan_notify_threshold);


/**
 * FUNCTION NAME: linuxbt_gattc_batchscan_enb_batch_scan
 * PURPOSE:
 *      The function is used for gatt client to enable batch scan
 * INPUT:
 *      None, not used
 * OUTPUT:
 *      None
 * RETURN:
 *      BT_SUCCESS                        -- Operate success.
 *      BT_ERR_STATUS_FAIL                -- Operate fail.
 *      BT_ERR_STATUS_PARM_INVALID        -- paramter is invalid.
 * NOTES:
 *      None
 */
int linuxbt_gattc_batchscan_enb_batch_scan(int client_if, int scan_mode,
                        int scan_interval, int scan_window, int addr_type, int discard_rule);

/**
 * FUNCTION NAME: linuxbt_gattc_batchscan_dis_batch_scan
 * PURPOSE:
 *      The function is used for gatt client to disable batch scan
 * INPUT:
 *      None, not used
 * OUTPUT:
 *      None
 * RETURN:
 *      BT_SUCCESS                        -- Operate success.
 *      BT_ERR_STATUS_FAIL                -- Operate fail.
 *      BT_ERR_STATUS_PARM_INVALID        -- paramter is invalid.
 * NOTES:
 *      None
 */
int linuxbt_gattc_batchscan_dis_batch_scan(int client_if);

/**
 * FUNCTION NAME: linuxbt_gattc_batchscan_read_reports
 * PURPOSE:
 *      The function is used for gatt client to batch scan read reports
 * INPUT:
 *      None, not used
 * OUTPUT:
 *      None
 * RETURN:
 *      BT_SUCCESS                        -- Operate success.
 *      BT_ERR_STATUS_FAIL                -- Operate fail.
 *      BT_ERR_STATUS_PARM_INVALID        -- paramter is invalid.
 * NOTES:
 *      None
 */
int linuxbt_gattc_batchscan_read_reports(int client_if, int scan_mode);

int linuxbt_gattc_test_command(int command, BT_GATTC_TEST_PARAMS_T test_params);
int linuxbt_gattc_get_gatt_db(int conn_id);




#endif /* __LINUXBT_GATTC_IF_H__ */

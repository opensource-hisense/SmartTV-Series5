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

/* FILE NAME:  bt_mw_gatt_cli.h
 * AUTHOR: Xuemei Yang
 * PURPOSE:
 *      It provides GATTC and GATTS operation command definition.
 * NOTES:
 */


/*-----------------------------------------------------------------------------
                    include files
 ----------------------------------------------------------------------------*/

#ifndef __BT_MW_GATT_CLI_H__
#define __BT_MW_GATT_CLI_H__

void _bluetooth_add_gatt_cli_table ();

/*gattc*/
/**
 * FUNCTION NAME: _bluetooth_gattc_register_app_cli
 * PURPOSE:
 *      The function is used to register gatt client APP.
 * INPUT:
 *      argc               -- paramter number
 *      argv[0]            -- gattc_register_app
 *      argv[1]            -- app uuid
 * OUTPUT:
 *      None
 * RETURN:
 *      BT_SUCCESS                        -- Operate success.
 *      BT_ERR_STATUS_FAIL                -- Operate fail.
 *      BT_ERR_STATUS_PARM_INVALID        -- paramter is invalid.
 *      BT_ERR_STATUS_INVALID_PARM_NUMS   -- param num invalid.
 * NOTES:
 *      None
 */
INT32 _bluetooth_gattc_register_app_cli (INT32 argc, const CHAR **argv);

/**
 * FUNCTION NAME: _bluetooth_gattc_unregister_app_cli
 * PURPOSE:
 *      The function is used to deregister gatt client APP.
 * INPUT:
 *      argc               -- paramter number
 *      argv[0]            -- gattc_register_app
 *      argv[1]            -- client identifer of register app
 * OUTPUT:
 *      None
 * RETURN:
 *      BT_SUCCESS                        -- Operate success.
 *      BT_ERR_STATUS_FAIL                -- Operate fail.
 *      BT_ERR_STATUS_PARM_INVALID        -- paramter is invalid.
 *      BT_ERR_STATUS_INVALID_PARM_NUMS   -- param num invalid.
 * NOTES:
 *      None
 */
INT32 _bluetooth_gattc_unregister_app_cli (INT32 argc, const CHAR **argv);

/**
 * FUNCTION NAME: _bluetooth_gattc_scan_cli
 * PURPOSE:
 *      The function is used for gatt client scan remote device.
 * INPUT:
 *      argc               -- paramter number
 *      argv[0]            -- gattc_start_scan
 *      argv[1]            -- client identifer of register app
 * OUTPUT:
 *      None
 * RETURN:
 *      BT_SUCCESS                        -- Operate success.
 *      BT_ERR_STATUS_FAIL                -- Operate fail.
 *      BT_ERR_STATUS_PARM_INVALID        -- paramter is invalid.
 *      BT_ERR_STATUS_INVALID_PARM_NUMS   -- param num invalid.
 * NOTES:
 *      None
 */
INT32 _bluetooth_gattc_scan_cli (INT32 argc, const CHAR **argv);

/**
 * FUNCTION NAME: _bluetooth_gattc_stop_scan_cli
 * PURPOSE:
 *      The function is used for gatt client stop scan remote device.
 * INPUT:
 *      argc               -- paramter number
 *      argv[0]            -- gattc_stop_scan
 *      argv[1]            -- client identifer of register app
 * OUTPUT:
 *      None
 * RETURN:
 *      BT_SUCCESS                        -- Operate success.
 *      BT_ERR_STATUS_FAIL                -- Operate fail.
 *      BT_ERR_STATUS_PARM_INVALID        -- paramter is invalid.
 *      BT_ERR_STATUS_INVALID_PARM_NUMS   -- param num invalid.
 * NOTES:
 *      None
 */
INT32 _bluetooth_gattc_stop_scan_cli (INT32 argc, const CHAR **argv);

/**
* FUNCTION NAME: _bluetooth_gattc_open_cli
* PURPOSE:
*      The function is used for gatt client connect with remote device.
* INPUT:
*      argc               -- paramter number
*      argv[0]            -- gattc_open
*      argv[1]            -- registered app identifer
*      argv[2]            -- remote device bt address
*      argv[3]            -- is direct connection or background connection
*      argv[4]            -- transport type:(0 : auto, 1 : BREDR, 2 : LE)
* OUTPUT:
*      None
* RETURN:
*      BT_SUCCESS                        -- Operate success.
*      BT_ERR_STATUS_FAIL                -- Operate fail.
*      BT_ERR_STATUS_PARM_INVALID        -- paramter is invalid.
*      BT_ERR_STATUS_INVALID_PARM_NUMS   -- param num invalid.
* NOTES:
*      None
*/
INT32 _bluetooth_gattc_open_cli (INT32 argc, const CHAR **argv);

/**
* FUNCTION NAME: _bluetooth_gattc_close_cli
* PURPOSE:
*      The function is used for gatt client disconnect with remote device.
* INPUT:
*      argc               -- paramter number
*      argv[0]            -- gattc_close
*      argv[1]            -- registered app identifer
*      argv[2]            -- remote device bt address
*      argv[3]            -- connection id
* OUTPUT:
*      None
* RETURN:
*      BT_SUCCESS                        -- Operate success.
*      BT_ERR_STATUS_FAIL                -- Operate fail.
*      BT_ERR_STATUS_PARM_INVALID        -- paramter is invalid.
*      BT_ERR_STATUS_INVALID_PARM_NUMS   -- param num invalid.
* NOTES:
*      None
*/
INT32 _bluetooth_gattc_close_cli (INT32 argc, const CHAR **argv);

/**
* FUNCTION NAME: _bluetooth_gattc_listen_cli
* PURPOSE:
*      The function is used for gatt client listening operation.
* INPUT:
*      argc               -- paramter number
*      argv[0]            -- gattc_listen
*      argv[1]          -- registered app identifer
* OUTPUT:
*      None
* RETURN:
*      BT_SUCCESS                        -- Operate success.
*      BT_ERR_STATUS_FAIL                -- Operate fail.
*      BT_ERR_STATUS_PARM_INVALID        -- paramter is invalid.
*      BT_ERR_STATUS_INVALID_PARM_NUMS   -- param num invalid.
* NOTES:
*      None
*/
INT32 _bluetooth_gattc_listen_cli (INT32 argc, const CHAR **argv);

/**
* FUNCTION NAME: _bluetooth_gattc_refresh_cli
* PURPOSE:
*      The function is used for gatt client refresh operation.
* INPUT:
*      argc               -- paramter number
*      argv[0]            -- gattc_refresh
*      argv[1]          -- registered app identifer
*      argv[2]            -- remote device bt address
* OUTPUT:
*      None
* RETURN:
*      BT_SUCCESS                        -- Operate success.
*      BT_ERR_STATUS_FAIL                -- Operate fail.
*      BT_ERR_STATUS_PARM_INVALID        -- paramter is invalid.
*      BT_ERR_STATUS_INVALID_PARM_NUMS   -- param num invalid.
* NOTES:
*      None
*/
INT32 _bluetooth_gattc_refresh_cli (INT32 argc, const CHAR **argv);

/**
* FUNCTION NAME: _bluetooth_gattc_search_service_cli
* PURPOSE:
*      The function is used for gatt client search service on the remote device.
* INPUT:
*      argc               -- paramter number
*      argv[0]            -- gattc_search
*      argv[1]            -- connection id
*      argv[2]            -- service uuid
* OUTPUT:
*      None
* RETURN:
*      BT_SUCCESS                        -- Operate success.
*      BT_ERR_STATUS_FAIL                -- Operate fail.
*      BT_ERR_STATUS_PARM_INVALID        -- paramter is invalid.
*      BT_ERR_STATUS_INVALID_PARM_NUMS   -- param num invalid.
* NOTES:
*      None
*/
INT32 _bluetooth_gattc_search_service_cli (INT32 argc, const CHAR **argv);

INT32 _bluetooth_gattc_get_gatt_db_cli (INT32 argc, const CHAR **argv);

/**
* FUNCTION NAME: _bluetooth_gattc_get_char_cli
* PURPOSE:
*      The function is used for gatt client get characiteristic on loacal cache database.
* INPUT:
*      argc               -- paramter number
*      argv[0]            -- gattc_get_char
*      argv[1]            -- connection id
*      argv[2]            -- service uuid
*      argv[3]            -- service instance id
*      argv[4]            -- first charateristic uuid
*      argv[5]            -- first charateristic instance id
* OUTPUT:
*      None
* RETURN:
*      BT_SUCCESS                        -- Operate success.
*      BT_ERR_STATUS_FAIL                -- Operate fail.
*      BT_ERR_STATUS_PARM_INVALID        -- paramter is invalid.
*      BT_ERR_STATUS_INVALID_PARM_NUMS   -- param num invalid.
* NOTES:
*      None
*/
INT32 _bluetooth_gattc_get_char_cli (INT32 argc, const CHAR **argv);

/**
* FUNCTION NAME: _bluetooth_gattc_get_descr_cli
* PURPOSE:
*      The function is used for gatt client get char description on loacal cache database.
* INPUT:
*      argc               -- paramter number
*      argv[0]            -- gattc_get_desc
*      argv[1]            -- connection id
*      argv[2]            -- service uuid
*      argv[3]            -- service instance id
*      argv[4]            -- first charateristic uuid
*      argv[5]            -- first charateristic instance id
*      argv[6]            -- first char description uuid
*      argv[7]            -- first char description instance id
* OUTPUT:
*      None
* RETURN:
*      BT_SUCCESS                        -- Operate success.
*      BT_ERR_STATUS_FAIL                -- Operate fail.
*      BT_ERR_STATUS_PARM_INVALID        -- paramter is invalid.
*      BT_ERR_STATUS_INVALID_PARM_NUMS   -- param num invalid.
* NOTES:
*      None
*/
INT32 _bluetooth_gattc_get_descr_cli (INT32 argc, const CHAR **argv);

/**
* FUNCTION NAME: _bluetooth_gattc_get_incl_service_cli
* PURPOSE:
*      The function is used for gatt client get included service on loacal cache database.
* INPUT:
*      argc               -- paramter number
*      argv[0]            -- gattc_get_incl_service
*      argv[1]            -- connection id
*      argv[2]            -- service uuid
*      argv[3]            -- service instance id
*      argv[4]            -- included service uuid
*      argv[5]            -- included service instance id
* OUTPUT:
*      None
* RETURN:
*      BT_SUCCESS                        -- Operate success.
*      BT_ERR_STATUS_FAIL                -- Operate fail.
*      BT_ERR_STATUS_PARM_INVALID        -- paramter is invalid.
*      BT_ERR_STATUS_INVALID_PARM_NUMS   -- param num invalid.
* NOTES:
*      None
*/
INT32 _bluetooth_gattc_get_incl_service_cli (INT32 argc, const CHAR **argv);

/**
* FUNCTION NAME: _bluetooth_gattc_read_char_cli
* PURPOSE:
*      The function is used for gatt client read characteristic from remote device
* INPUT:
*      argc               -- paramter number
*      argv[0]            -- gattc_read_char
*      argv[1]            -- connection id
*      argv[2]            -- service uuid
*      argv[3]            -- service instance id
*      argv[4]            -- characteristic uuid
*      argv[5]            -- characteristic instance id
*      argv[6]            -- authentication request flag
* OUTPUT:
*      None
* RETURN:
*      BT_SUCCESS                        -- Operate success.
*      BT_ERR_STATUS_FAIL                -- Operate fail.
*      BT_ERR_STATUS_PARM_INVALID        -- paramter is invalid.
*      BT_ERR_STATUS_INVALID_PARM_NUMS   -- param num invalid.
* NOTES:
*      None
*/
INT32 _bluetooth_gattc_read_char_cli (INT32 argc, const CHAR **argv);

/**
* FUNCTION NAME: _bluetooth_gattc_read_descr_cli
* PURPOSE:
*      The function is used for gatt client read char description from remote device
* INPUT:
*      argc               -- paramter number
*      argv[0]            -- gattc_read_descr
*      argv[1]            -- connection id
*      argv[2]            -- service uuid
*      argv[3]            -- service instance id
*      argv[4]            -- characteristic uuid
*      argv[5]            -- characteristic instance id
*      argv[6]            -- char descriptor uuid
*      argv[7]            -- char descriptor instance id
*      argv[8]            -- authentication request flag
* OUTPUT:
*      None
* RETURN:
*      BT_SUCCESS                        -- Operate success.
*      BT_ERR_STATUS_FAIL                -- Operate fail.
*      BT_ERR_STATUS_PARM_INVALID        -- paramter is invalid.
*      BT_ERR_STATUS_INVALID_PARM_NUMS   -- param num invalid.
* NOTES:
*      None
*/
INT32 _bluetooth_gattc_read_descr_cli (INT32 argc, const CHAR **argv);

/**
* FUNCTION NAME: _bluetooth_gattc_write_char_cli
* PURPOSE:
*      The function is used for gatt client write characteristic to remote device
* INPUT:
*      argc               -- paramter number
*      argv[0]            -- gattc_write_char
*      argv[1]            -- connection id
*      argv[2]            -- service uuid
*      argv[3]            -- service instance id
*      argv[4]            -- characteristic uuid
*      argv[5]            -- characteristic instance id
*      argv[6]            -- write type(1:WRITE_TYPE_NO_RSP  2:WRITE_TYPE_REQUEST  3:WRITE_TYPE_PREPARE)
*      argv[7]            -- authentication request flag
*      argv[8]            -- write characteristic value
* OUTPUT:
*      None
* RETURN:
*      BT_SUCCESS                        -- Operate success.
*      BT_ERR_STATUS_FAIL                -- Operate fail.
*      BT_ERR_STATUS_PARM_INVALID        -- paramter is invalid.
*      BT_ERR_STATUS_INVALID_PARM_NUMS   -- param num invalid.
* NOTES:
*      None
*/
INT32 _bluetooth_gattc_write_char_cli (INT32 argc, const CHAR **argv);

/**
* FUNCTION NAME: _bluetooth_gattc_write_descr_cli
* PURPOSE:
*      The function is used for gatt client write char description to remote device
* INPUT:
*      argc               -- paramter number
*      argv[0]            -- gattc_write_desc
*      argv[1]            -- connection id
*      argv[2]            -- service uuid
*      argv[3]            -- service instance id
*      argv[4]            -- characteristic uuid
*      argv[5]            -- characteristic instance id
*      argv[6]            -- char description uuid
*      argv[7]            -- char description instance id
*      argv[8]            -- write type(1:WRITE_TYPE_NO_RSP  2:WRITE_TYPE_REQUEST  3:WRITE_TYPE_PREPARE)
*      argv[9]           -- authentication request flag
*      argv[10]              -- write characteristic value
* OUTPUT:
*      None
* RETURN:
*      BT_SUCCESS                        -- Operate success.
*      BT_ERR_STATUS_FAIL                -- Operate fail.
*      BT_ERR_STATUS_PARM_INVALID        -- paramter is invalid.
*      BT_ERR_STATUS_INVALID_PARM_NUMS   -- param num invalid.
* NOTES:
*      None
*/
INT32 _bluetooth_gattc_write_descr_cli (INT32 argc, const CHAR **argv);

/**
* FUNCTION NAME: _bluetooth_gattc_reg_noti_cli
* PURPOSE:
*      The function is used for gatt client register notification
* INPUT:
*      argc               -- paramter number
*      argv[0]            -- gattc_reg_noti
*      argv[1]            -- registered client app identifier
*      argv[2]            -- remote device bt address
*      argv[3]            -- service uuid
*      argv[4]            -- service instance id
*      argv[5]            -- characteristic uuid
*      argv[6]            -- characteristic instance id
* OUTPUT:
*      None
* RETURN:
*      BT_SUCCESS                        -- Operate success.
*      BT_ERR_STATUS_FAIL                -- Operate fail.
*      BT_ERR_STATUS_PARM_INVALID        -- paramter is invalid.
*      BT_ERR_STATUS_INVALID_PARM_NUMS   -- param num invalid.
* NOTES:
*      None
*/
INT32 _bluetooth_gattc_reg_noti_cli (INT32 argc, const CHAR **argv);

/**
* FUNCTION NAME: _bluetooth_gattc_dereg_noti_cli
* PURPOSE:
*      The function is used for gatt client deregister notification
* INPUT:
*      argc               -- paramter number
*      argv[0]            -- gattc_dereg_noti
*      argv[1]            -- registered client app identifier
*      argv[2]            -- remote device bt address
*      argv[3]            -- service uuid
*      argv[4]            -- service instance id
*      argv[5]            -- characteristic uuid
*      argv[6]            -- characteristic instance id
* OUTPUT:
*      None
* RETURN:
*      BT_SUCCESS                        -- Operate success.
*      BT_ERR_STATUS_FAIL                -- Operate fail.
*      BT_ERR_STATUS_PARM_INVALID        -- paramter is invalid.
*      BT_ERR_STATUS_INVALID_PARM_NUMS   -- param num invalid.
* NOTES:
*      None
*/
INT32 _bluetooth_gattc_dereg_noti_cli (INT32 argc, const CHAR **argv);

/**
* FUNCTION NAME: _bluetooth_gattc_read_rssi_cli
* PURPOSE:
*      The function is used for gatt client read the received signal strength indicator
* INPUT:
*      argc               -- paramter number
*      argv[0]            -- gattc_rssi
*      argv[1]            -- registered client app identifier
*      argv[2]            -- remote device bt address
* OUTPUT:
*      None
* RETURN:
*      BT_SUCCESS                        -- Operate success.
*      BT_ERR_STATUS_FAIL                -- Operate fail.
*      BT_ERR_STATUS_PARM_INVALID        -- paramter is invalid.
*      BT_ERR_STATUS_INVALID_PARM_NUMS   -- param num invalid.
* NOTES:
*      None
*/
INT32 _bluetooth_gattc_read_rssi_cli (INT32 argc, const CHAR **argv);

/**
* FUNCTION NAME: _bluetooth_gatt_parse_adv_data_cli
* PURPOSE:
*      The function is used for gatt to parse advertising data.
* INPUT:
*      argc               -- paramter number
*      argv[0]            -- gattc_parse_adv_data
*      argv[1]            -- advertising data
* OUTPUT:
*      None
* RETURN:
*      BT_SUCCESS                        -- Operate success.
*      BT_ERR_STATUS_INVALID_PARM_NUMS   -- param num invalid.
* NOTES:
*      None
*/
INT32 _bluetooth_gatt_parse_adv_data_cli (INT32 argc, const CHAR **argv);

/**
* FUNCTION NAME: _bluetooth_gattc_multi_adv_enable_cli
* PURPOSE:
*      The function is used for gatt to enable multiple advertising
* INPUT:
*      argc               -- paramter number
*      argv[0]            -- gattc_adv_enable
*      argv[1]            -- registered client app identifier
*      argv[2]            -- minimum advertising interval
*      argv[3]            -- maximum advertising interval
*      argv[4]            -- advertising type(0:ADV_IND  1:ADV_DIRECT_IND   2:ADV_SCAN_IND
*                                                 3:ADV_NONCONN_IND  4:SCAN_RSP)
*      argv[5]            -- transmit power(unit in dBm)
*      argv[6]            -- time out.
* OUTPUT:
*      None
* RETURN:
*      BT_SUCCESS                        -- Operate success.
*      BT_ERR_STATUS_FAIL                -- Operate fail.
*      BT_ERR_STATUS_PARM_INVALID        -- paramter is invalid.
*      BT_ERR_STATUS_INVALID_PARM_NUMS   -- param num invalid.
* NOTES:
*      None
*/
INT32 _bluetooth_gattc_multi_adv_enable_cli (INT32 argc, const CHAR **argv);

/**
* FUNCTION NAME: _bluetooth_gattc_multi_adv_update_cli
* PURPOSE:
*      The function is used for gatt to update multiple advertising parameters
* INPUT:
*      argc               -- paramter number
*      argv[0]            -- gattc_adv_update
*      argv[1]            -- registered client app identifier
*      argv[3]            -- minimum advertising interval
*      argv[4]            -- maximum advertising interval
*      argv[5]            -- advertising type(0:ADV_IND  1:ADV_DIRECT_IND   2:ADV_SCAN_IND
*                                                 3:ADV_NONCONN_IND  4:SCAN_RSP)
*      argv[5]            -- transmit power(unit in dBm)
*      argv[6]            -- time out.
* OUTPUT:
*      None
* RETURN:
*      BT_SUCCESS                        -- Operate success.
*      BT_ERR_STATUS_FAIL                -- Operate fail.
*      BT_ERR_STATUS_PARM_INVALID        -- paramter is invalid.
*      BT_ERR_STATUS_INVALID_PARM_NUMS   -- param num invalid.
* NOTES:
*      None
*/
INT32 _bluetooth_gattc_multi_adv_update_cli (INT32 argc, const CHAR **argv);

/**
* FUNCTION NAME: _bluetooth_gattc_multi_adv_setdata_cli
* PURPOSE:
*      The function is used for gatt to set multiple advertising data
* INPUT:
*      argc               -- paramter number
*      argv[0]            -- gattc_adv_update_data
*      argv[1]            -- registered client app identifier
*      argv[2]            -- set_scan_rsp
*      argv[3]            -- set scan response flag
*      argv[4]            -- include_name
*      argv[5]            -- include device name flag
*      argv[6]            -- incl_txpower
*      argv[7]            -- include transmit power flag
*      argv[8]            -- appearance
*      argv[9]            -- appearance data
*      argv[10]            -- manufacturer_data
*      argv[11]            -- manufacturer data
*      argv[12]            -- service_data
*      argv[13]            -- service data
*      argv[14]            -- service_uuid
*      argv[15]            -- service uuid
*      argv[16]            -- le_name
*      argv[17]            -- device name
* OUTPUT:
*      None
* RETURN:
*      BT_SUCCESS                        -- Operate success.
*      BT_ERR_STATUS_FAIL                -- Operate fail.
*      BT_ERR_STATUS_PARM_INVALID        -- paramter is invalid.
*      BT_ERR_STATUS_INVALID_PARM_NUMS   -- param num invalid.
* NOTES:
*      None
*/
INT32 _bluetooth_gattc_multi_adv_setdata_cli (INT32 argc, const CHAR **argv);

/**
* FUNCTION NAME: _bluetooth_gattc_multi_adv_disable_cli
* PURPOSE:
*      The function is used for gatt to disable multiple advertising
* INPUT:
*      argc               -- paramter number
*      argv[0]            -- gattc_adv_disable
*      argv[1]            -- registered client app identifier
* OUTPUT:
*      None
* RETURN:
*      BT_SUCCESS                        -- Operate success.
*      BT_ERR_STATUS_FAIL                -- Operate fail.
*      BT_ERR_STATUS_PARM_INVALID        -- paramter is invalid.
*      BT_ERR_STATUS_INVALID_PARM_NUMS   -- param num invalid.
* NOTES:
*      None
*/
INT32 _bluetooth_gattc_multi_adv_disable_cli (INT32 argc, const CHAR **argv);


INT32 _bluetooth_gattc_scan_filter_param_setup_cli (INT32 argc, const CHAR **argv);

INT32 _bluetooth_gattc_scan_filter_enable_cli (INT32 argc, const CHAR **argv);

INT32 _bluetooth_gattc_scan_filter_disable_cli (INT32 argc, const CHAR **argv);

INT32 _bluetooth_gattc_scan_filter_add_remove_cli (INT32 argc, const CHAR **argv);

INT32 _bluetooth_gattc_scan_filter_clear_cli (INT32 argc, const CHAR **argv);
INT32 _bluetooth_gattc_set_local_le_name_cli (INT32 argc, const CHAR **argv);
INT32 _bluetooth_gattc_configure_mtu_cli (INT32 argc, const CHAR **argv);



/*gatts*/
/**
 * FUNCTION NAME: _bluetooth_gatts_register_server_cli
 * PURPOSE:
 *      The function is used to register gatt server APP.
 * INPUT:
 *      argc               -- paramter number
 *      argv[0]            -- gatts_register_server
 *      argv[1]            -- app uuid
 * OUTPUT:
 *      None
 * RETURN:
 *      BT_SUCCESS                        -- Operate success.
 *      BT_ERR_STATUS_FAIL                -- Operate fail.
 *      BT_ERR_STATUS_PARM_INVALID        -- paramter is invalid.
 *      BT_ERR_STATUS_INVALID_PARM_NUMS   -- param num invalid.
 * NOTES:
 *      None
 */
INT32 _bluetooth_gatts_register_server_cli (INT32 argc, const CHAR **argv);

/**
 * FUNCTION NAME: _bluetooth_gatts_unregister_server_cli
 * PURPOSE:
 *      The function is used to deregister gatt server APP.
 * INPUT:
 *      argc               -- paramter number
 *      argv[0]            -- gatts_unregister_server
 *      argv[1]            -- registered app identifer
 * OUTPUT:
 *      None
 * RETURN:
 *      BT_SUCCESS                        -- Operate success.
 *      BT_ERR_STATUS_FAIL                -- Operate fail.
 *      BT_ERR_STATUS_PARM_INVALID        -- paramter is invalid.
 *      BT_ERR_STATUS_INVALID_PARM_NUMS   -- param num invalid.
 * NOTES:
 *      None
 */
INT32 _bluetooth_gatts_unregister_server_cli (INT32 argc, const CHAR **argv);

/**
 * FUNCTION NAME: _bluetooth_gatts_open_cli
 * PURPOSE:
 *      The function is used for gatt server connect with remote device.
 * INPUT:
 *      argc               -- paramter number
 *      argv[0]            -- gatts_cc
 *      argv[1]            -- registered app identifer
 *      argv[2]            -- remote device bt address
 *      argv[3]            -- is direct connection or background connection
 *      argv[4]            -- transport type:(0 : auto, 1 : BREDR, 2 : LE)
 * OUTPUT:
 *      None
 * RETURN:
 *      BT_SUCCESS                        -- Operate success.
 *      BT_ERR_STATUS_FAIL                -- Operate fail.
 *      BT_ERR_STATUS_PARM_INVALID        -- paramter is invalid.
 *      BT_ERR_STATUS_INVALID_PARM_NUMS   -- param num invalid.
 * NOTES:
 *      None
 */
INT32 _bluetooth_gatts_open_cli (INT32 argc, const CHAR **argv);

/**
 * FUNCTION NAME: _bluetooth_gatts_close_cli
 * PURPOSE:
 *      The function is used for gatt server disconnect with remote device.
 * INPUT:
 *      argc               -- paramter number
 *      argv[0]            -- gatts_dc
 *      argv[1]            -- registered app identifer
 *      argv[2]            -- remote device bt address
 *      argv[3]            -- connection id
 * OUTPUT:
 *      None
 * RETURN:
 *      BT_SUCCESS                        -- Operate success.
 *      BT_ERR_STATUS_FAIL                -- Operate fail.
 *      BT_ERR_STATUS_PARM_INVALID        -- paramter is invalid.
 *      BT_ERR_STATUS_INVALID_PARM_NUMS   -- param num invalid.
 * NOTES:
 *      None
 */
INT32 _bluetooth_gatts_close_cli (INT32 argc, const CHAR **argv);

/**
 * FUNCTION NAME: _bluetooth_gatts_add_service_cli
 * PURPOSE:
 *      The function is used for gatt server add service to database.
 * INPUT:
 *      argc               -- paramter number
 *      argv[0]            -- gatts_add_service
 *      argv[1]            -- registered app identifer
 *      argv[2]            -- service uuid
 *      argv[3]            -- is primary service or not
 *      argv[4]            -- handle number
 * OUTPUT:
 *      None
 * RETURN:
 *      BT_SUCCESS                        -- Operate success.
 *      BT_ERR_STATUS_FAIL                -- Operate fail.
 *      BT_ERR_STATUS_PARM_INVALID        -- paramter is invalid.
 *      BT_ERR_STATUS_INVALID_PARM_NUMS   -- param num invalid.
 * NOTES:
 *      None
 */
INT32 _bluetooth_gatts_add_service_cli (INT32 argc, const CHAR **argv);

/**
 * FUNCTION NAME: _bluetooth_gatts_add_incl_service_cli
 * PURPOSE:
 *      The function is used for gatt server add included service to database.
 * INPUT:
 *      argc               -- paramter number
 *      argv[0]            -- gatts_add_incl_service
 *      argv[1]            -- registered app identifer
 *      argv[2]            -- added service start handle
 *      argv[3]            -- include service handle
 * OUTPUT:
 *      None
 * RETURN:
 *      BT_SUCCESS                        -- Operate success.
 *      BT_ERR_STATUS_FAIL                -- Operate fail.
 *      BT_ERR_STATUS_PARM_INVALID        -- paramter is invalid.
 *      BT_ERR_STATUS_INVALID_PARM_NUMS   -- param num invalid.
 * NOTES:
 *      None
 */
INT32 _bluetooth_gatts_add_incl_service_cli (INT32 argc, const CHAR **argv);

/**
 * FUNCTION NAME: _bluetooth_gatts_add_char_cli
 * PURPOSE:
 *      The function is used for gatt server add characteristic to database.
 * INPUT:
 *      argc               -- paramter number
 *      argv[0]            -- gatts_add_char
 *      argv[1]            -- registered app identifer
 *      argv[2]            -- added service start handle
 *      argv[3]            -- characteristic uuid
 *      argv[4]            -- characteristic properties
 *      argv[5]            -- access characteristic permissions
 * OUTPUT:
 *      None
 * RETURN:
 *      BT_SUCCESS                        -- Operate success.
 *      BT_ERR_STATUS_FAIL                -- Operate fail.
 *      BT_ERR_STATUS_PARM_INVALID        -- paramter is invalid.
 *      BT_ERR_STATUS_INVALID_PARM_NUMS   -- param num invalid.
 * NOTES:
 *      None
 */
INT32 _bluetooth_gatts_add_char_cli (INT32 argc, const CHAR **argv);

/**
 * FUNCTION NAME: _bluetooth_gatts_add_desc_cli
 * PURPOSE:
 *      The function is used for gatt server add char description to database.
 * INPUT:
 *      argc               -- paramter number
 *      argv[0]            -- gatts_add_desc
 *      argv[1]            -- registered app identifer
 *      argv[2]            -- added service start handle
 *      argv[3]            -- char description uuid
 *      argv[4]            -- access char description permissions
 * OUTPUT:
 *      None
 * RETURN:
 *      BT_SUCCESS                        -- Operate success.
 *      BT_ERR_STATUS_FAIL                -- Operate fail.
 *      BT_ERR_STATUS_PARM_INVALID        -- paramter is invalid.
 *      BT_ERR_STATUS_INVALID_PARM_NUMS   -- param num invalid.
 * NOTES:
 *      None
 */
INT32 _bluetooth_gatts_add_desc_cli (INT32 argc, const CHAR **argv);

/**
 * FUNCTION NAME: _bluetooth_gatts_start_service_cli
 * PURPOSE:
 *      The function is used for gatt server start service.
 * INPUT:
 *      argc               -- paramter number
 *      argv[0]            -- gatts_start_service
 *      argv[1]            -- registered app identifer
 *      argv[2]            -- added service start handle
 *      argv[3]            -- transport type:(0 : auto, 1 : BREDR, 2 : LE)
 * OUTPUT:
 *      None
 * RETURN:
 *      BT_SUCCESS                        -- Operate success.
 *      BT_ERR_STATUS_FAIL                -- Operate fail.
 *      BT_ERR_STATUS_PARM_INVALID        -- paramter is invalid.
 *      BT_ERR_STATUS_INVALID_PARM_NUMS   -- param num invalid.
 * NOTES:
 *      None
 */
INT32 _bluetooth_gatts_start_service_cli (INT32 argc, const CHAR **argv);

/**
 * FUNCTION NAME: _bluetooth_gatts_stop_service_cli
 * PURPOSE:
 *      The function is used for gatt server stop service.
 * INPUT:
 *      argc               -- paramter number
 *      argv[0]            -- gatts_stop_service
 *      argv[1]            -- registered app identifer
 *      argv[2]            -- added service start handle
 * OUTPUT:
 *      None
 * RETURN:
 *      BT_SUCCESS                        -- Operate success.
 *      BT_ERR_STATUS_FAIL                -- Operate fail.
 *      BT_ERR_STATUS_PARM_INVALID        -- paramter is invalid.
 *      BT_ERR_STATUS_INVALID_PARM_NUMS   -- param num invalid.
 * NOTES:
 *      None
 */
INT32 _bluetooth_gatts_stop_service_cli (INT32 argc, const CHAR **argv);


/**
 * FUNCTION NAME: _bluetooth_gatts_delete_service_cli
 * PURPOSE:
 *      The function is used for gatt server delete service.
 * INPUT:
 *      argc               -- paramter number
 *      argv[0]            -- gatts_delete_service
 *      argv[1]            -- registered app identifer
 *      argv[2]            -- added service start handle
 * OUTPUT:
 *      None
 * RETURN:
 *      BT_SUCCESS                        -- Operate success.
 *      BT_ERR_STATUS_FAIL                -- Operate fail.
 *      BT_ERR_STATUS_PARM_INVALID        -- paramter is invalid.
 *      BT_ERR_STATUS_INVALID_PARM_NUMS   -- param num invalid.
 * NOTES:
 *      None
 */
INT32 _bluetooth_gatts_delete_service_cli (INT32 argc, const CHAR **argv);

/**
 * FUNCTION NAME: _bluetooth_gatts_send_indication_cli
 * PURPOSE:
 *      The function is used for gatt server send indication to remote device.
 * INPUT:
 *      argc               -- paramter number
 *      argv[0]            -- gatts_send_indi
 *      argv[1]            -- registered app identifer
 *      argv[2]            -- send indication attribute handle
 *      argv[3]            -- connection id
 *      argv[4]            -- is need confirmation or not
 *      argv[5]            -- send indication value
 * OUTPUT:
 *      None
 * RETURN:
 *      BT_SUCCESS                        -- Operate success.
 *      BT_ERR_STATUS_FAIL                -- Operate fail.
 *      BT_ERR_STATUS_PARM_INVALID        -- paramter is invalid.
 *      BT_ERR_STATUS_INVALID_PARM_NUMS   -- param num invalid.
 * NOTES:
 *      None
 */

INT32 _bluetooth_gatts_send_indication_cli (INT32 argc, const CHAR **argv);

/**
 * FUNCTION NAME: _bluetooth_gatts_send_response_cli
 * PURPOSE:
 *      The function is used for gatt server send response to remote device.
 * INPUT:
 *      argc               -- paramter number
 *      argv[0]            -- gatts_send_rsp
 *      argv[1]            -- connection id
 *      argv[2]            -- transaction id
 *      argv[3]            -- send response status
 *      argv[4]            -- send response handle
 *      argv[5]            -- authentication request
 *      argv[6]            -- send response value
 * OUTPUT:
 *      None
 * RETURN:
 *      BT_SUCCESS                        -- Operate success.
 *      BT_ERR_STATUS_FAIL                -- Operate fail.
 *      BT_ERR_STATUS_PARM_INVALID        -- paramter is invalid.
 *      BT_ERR_STATUS_INVALID_PARM_NUMS   -- param num invalid.
 * NOTES:
 *      None
 */
INT32 _bluetooth_gatts_send_response_cli (INT32 argc, const CHAR **argv);

#endif /*__BT_MW_GATT_CLI_H__*/

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

/* FILE NAME:  bt_mw_hidh_cli.h
 * AUTHOR: zwei.chen
 * PURPOSE:
 *      It provides GATTC and GATTS operation command implementation.
 * NOTES:
 */

#ifndef __BT_MW_HID_CLI_H__
#define __BT_MW_HID_CLI_H__


/*hid*/

/*****************************************************************************************
 * FUNCTION NAME: _bluetooth_hid_connect_cli
 * PURPOSE:
 *      The function is used to connect hid device
 * INPUT:
 *      argc               -- paramter number
 *      argv[0]            -- hid_cc
 *      argv[1]            -- device address
 * OUTPUT:
 *      None
 * RETURN:
 *      BT_SUCCESS                        -- Operate success.
 *      BT_ERR_STATUS_FAIL                -- Operate fail.
 *      BT_ERR_STATUS_PARM_INVALID        -- paramter is invalid.
 *      BT_ERR_STATUS_INVALID_PARM_NUMS   -- param num invalid.
 * NOTES:
 *      None
 ***********************************************************************************************/
//INT32 _bluetooth_hid_connect_cli(INT32 argc, const CHAR **argv);


/*****************************************************************************************
 * FUNCTION NAME: _bluetooth_hid_disconnect_cli
 * PURPOSE:
 *      The function is used to disconnect hid device
 * INPUT:
 *      argc               -- paramter number
 *      argv[0]            -- hid_dc
 *      argv[1]            -- device address
 * OUTPUT:
 *      None
 * RETURN:
 *      BT_SUCCESS                        -- Operate success.
 *      BT_ERR_STATUS_FAIL                -- Operate fail.
 *      BT_ERR_STATUS_PARM_INVALID        -- paramter is invalid.
 *      BT_ERR_STATUS_INVALID_PARM_NUMS   -- param num invalid.
 * NOTES:
 *      None
 ***********************************************************************************************/
//INT32 _bluetooth_hid_disconnect_cli(INT32 argc, const CHAR **argv);

/*****************************************************************************************
 * FUNCTION NAME: _bluetooth_hid_get_input_report_cli
 * PURPOSE:
 *      The function is used to get input report
 * INPUT:
 *      argc               -- paramter number
 *      argv[0]            -- get_input_report
 *      argv[1]            -- device address
 *      arg[2]              --report_id
  *      arg[3]              --max_buffer_size
 * OUTPUT:
 *      None
 * RETURN:
 *      BT_SUCCESS                        -- Operate success.
 *      BT_ERR_STATUS_FAIL                -- Operate fail.
 *      BT_ERR_STATUS_PARM_INVALID        -- paramter is invalid.
 *      BT_ERR_STATUS_INVALID_PARM_NUMS   -- param num invalid.
 * NOTES:
 *      None
 ***********************************************************************************************/
//INT32 _bluetooth_hid_get_input_report_cli(INT32 argc, const CHAR **argv);


/*****************************************************************************************
 * FUNCTION NAME: _bluetooth_hid_get_output_report_cli
 * PURPOSE:
 *      The function is used to get output report
 * INPUT:
 *      argc               -- paramter number
 *      argv[0]            -- get_output_report
 *      argv[1]            -- device address
 *      arg[2]              --report_id
  *      arg[3]              --max_buffer_size
 * OUTPUT:
 *      None
 * RETURN:
 *      BT_SUCCESS                        -- Operate success.
 *      BT_ERR_STATUS_FAIL                -- Operate fail.
 *      BT_ERR_STATUS_PARM_INVALID        -- paramter is invalid.
 *      BT_ERR_STATUS_INVALID_PARM_NUMS   -- param num invalid.
 * NOTES:
 *      None
 ***********************************************************************************************/
//INT32 _bluetooth_hid_get_output_report_cli(INT32 argc, const CHAR **argv);


/*****************************************************************************************
 * FUNCTION NAME: _bluetooth_hid_set_input_report_cli
 * PURPOSE:
 *      The function is used to set input report
 * INPUT:
 *      argc               -- paramter number
 *      argv[0]            -- set_input_report
 *      argv[1]            -- device address
 *      arg[2]              --report_data
  *
 * OUTPUT:
 *      None
 * RETURN:
 *      BT_SUCCESS                        -- Operate success.
 *      BT_ERR_STATUS_FAIL                -- Operate fail.
 *      BT_ERR_STATUS_PARM_INVALID        -- paramter is invalid.
 *      BT_ERR_STATUS_INVALID_PARM_NUMS   -- param num invalid.
 * NOTES:
 *      None
 ***********************************************************************************************/
//INT32 _bluetooth_hid_set_input_report_cli(INT32 argc, const CHAR **argv);


/*****************************************************************************************
 * FUNCTION NAME: _bluetooth_hid_set_output_report_cli
 * PURPOSE:
 *      The function is used to set output report
 * INPUT:
 *      argc               -- paramter number
 *      argv[0]            -- set_output_report
 *      argv[1]            -- device address
 *      arg[2]              --report_data
  *
 * OUTPUT:
 *      None
 * RETURN:
 *      BT_SUCCESS                        -- Operate success.
 *      BT_ERR_STATUS_FAIL                -- Operate fail.
 *      BT_ERR_STATUS_PARM_INVALID        -- paramter is invalid.
 *      BT_ERR_STATUS_INVALID_PARM_NUMS   -- param num invalid.
 * NOTES:
 *      None
 ***********************************************************************************************/
//INT32 _bluetooth_hid_set_output_report_cli(INT32 argc, const CHAR **argv);


/*****************************************************************************************
 * FUNCTION NAME: _bluetooth_hid_get_feature_report_cli
 * PURPOSE:
 *      The function is used to get feature report
 * INPUT:
 *      argc               -- paramter number
 *      argv[0]            -- get_feature_report
 *      argv[1]            -- device address
 *      arg[2]              --report_id
  *      arg[3]              --buffer_size
 * OUTPUT:
 *      None
 * RETURN:
 *      BT_SUCCESS                        -- Operate success.
 *      BT_ERR_STATUS_FAIL                -- Operate fail.
 *      BT_ERR_STATUS_PARM_INVALID        -- paramter is invalid.
 *      BT_ERR_STATUS_INVALID_PARM_NUMS   -- param num invalid.
 * NOTES:
 *      None
 ***********************************************************************************************/
//INT32 _bluetooth_hid_get_feature_report_cli(INT32 argc, const CHAR **argv);


/*****************************************************************************************
 * FUNCTION NAME: _bluetooth_hid_set_feature_report_cli
 * PURPOSE:
 *      The function is used to set feature report
 * INPUT:
 *      argc               -- paramter number
 *      argv[0]            -- set_feature_report
 *      argv[1]            -- device address
 *      arg[2]              --report_data
  *
 * OUTPUT:
 *      None
 * RETURN:
 *      BT_SUCCESS                        -- Operate success.
 *      BT_ERR_STATUS_FAIL                -- Operate fail.
 *      BT_ERR_STATUS_PARM_INVALID        -- paramter is invalid.
 *      BT_ERR_STATUS_INVALID_PARM_NUMS   -- param num invalid.
 * NOTES:
 *      None
 ***********************************************************************************************/
//INT32 _bluetooth_hid_set_feature_report_cli(INT32 argc, const CHAR **argv);


/*****************************************************************************************
 * FUNCTION NAME: _bluetooth_hid_get_protocol_cli
 * PURPOSE:
 *      The function is used to get protocol mode
 * INPUT:
 *      argc               -- paramter number
 *      argv[0]            -- get_protocol
 *      argv[1]            -- device address
*
 * OUTPUT:
 *      None
 * RETURN:
 *      BT_SUCCESS                        -- Operate success.
 *      BT_ERR_STATUS_FAIL                -- Operate fail.
 *      BT_ERR_STATUS_PARM_INVALID        -- paramter is invalid.
 *      BT_ERR_STATUS_INVALID_PARM_NUMS   -- param num invalid.
 * NOTES:
 *      None
 ***********************************************************************************************/
//INT32 _bluetooth_hid_get_protocol_cli(INT32 argc, const CHAR **argv);


/*****************************************************************************************
 * FUNCTION NAME: _bluetooth_hid_set_protocol_cli
 * PURPOSE:
 *      The function is used to set protocol mode
 * INPUT:
 *      argc               -- paramter number
 *      argv[0]            -- set_protocol
 *      argv[1]            -- device address
*
 * OUTPUT:
 *      None
 * RETURN:
 *      BT_SUCCESS                        -- Operate success.
 *      BT_ERR_STATUS_FAIL                -- Operate fail.
 *      BT_ERR_STATUS_PARM_INVALID        -- paramter is invalid.
 *      BT_ERR_STATUS_INVALID_PARM_NUMS   -- param num invalid.
 * NOTES:
 *      None
 ***********************************************************************************************/
//INT32 _bluetooth_hid_set_protocol_cli(INT32 argc, const CHAR **argv);


/*****************************************************************************************
 * FUNCTION NAME: _bluetooth_hid_send_control_cli
 * PURPOSE:
 *      The function is used to send control command
 * INPUT:
 *      argc               -- paramter number
 *      argv[0]            -- send_control
 *      argv[1]            -- control mode
*
 * OUTPUT:
 *      None
 * RETURN:
 *      BT_SUCCESS                        -- Operate success.
 *      BT_ERR_STATUS_FAIL                -- Operate fail.
 *      BT_ERR_STATUS_PARM_INVALID        -- paramter is invalid.
 *      BT_ERR_STATUS_INVALID_PARM_NUMS   -- param num invalid.
 * NOTES:
 *      None
 ***********************************************************************************************/
//INT32 _bluetooth_hid_send_control_cli(INT32 argc, const CHAR **argv);



#if ENABLE_HID_PROFILE_H

INT32 _bluetooth_hid_connect_cli(INT32 argc, const CHAR **argv);


INT32 _bluetooth_hid_disconnect_cli(INT32 argc, const CHAR **argv);


INT32 _bluetooth_hid_get_input_report_cli(INT32 argc, const CHAR **argv);


INT32 _bluetooth_hid_get_output_report_cli(INT32 argc, const CHAR **argv);


INT32 _bluetooth_hid_set_input_report_cli(INT32 argc, const CHAR **argv);


INT32 _bluetooth_hid_set_output_report_cli(INT32 argc, const CHAR **argv);


INT32 _bluetooth_hid_set_feature_report_cli(INT32 argc, const CHAR **argv);


INT32 _bluetooth_hid_get_feature_report_cli(INT32 argc, const CHAR **argv);


INT32 _bluetooth_hid_set_protocol_cli(INT32 argc, const CHAR **argv);


INT32 _bluetooth_hid_get_protocol_cli(INT32 argc, const CHAR **argv);

INT32 _bluetooth_hid_virtual_unplug_cli(INT32 argc, const CHAR **argv);

INT32 _bluetooth_hid_send_data_cli(INT32 argc, const CHAR **argv);
//INT32 _bluetooth_hid_send_control_cli(INT32 argc, const CHAR **argv);


INT32 _bluetooth_scan_hid_a2dp_cli(INT32 argc, const CHAR **argv);

#endif /* ENABLE_HID_PROFILE_D */
#if ENABLE_HID_PROFILE_D

INT32 _bluetooth_hidd_connect_cli(INT32 argc, const CHAR **argv);


INT32 _bluetooth_hidd_disconnect_cli(INT32 argc, const CHAR **argv);


INT32 _bluetooth_hidd_send_keyboard_data_cli(INT32 argc, const CHAR **argv);


INT32 _bluetooth_hidd_send_mouse_data_cli(INT32 argc, const CHAR **argv);


INT32 _bluetooth_hidd_send_consumer_data_cli(INT32 argc, const CHAR **argv);


INT32 _bluetooth_hidd_send_data_cli(INT32 argc, const CHAR **argv);

#endif /* ENABLE_HID_PROFILE_H */


#endif


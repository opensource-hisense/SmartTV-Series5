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

/* FILE NAME:  bt_mw_hidh_cli.c
 * AUTHOR: zwei.chen
 * PURPOSE:
 *      It provides GATTC and GATTS operation command implementation.
 * NOTES:
 */


/*-----------------------------------------------------------------------------
                    include files
 ----------------------------------------------------------------------------*/

#include <pthread.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
//#include "x_cli.h"

#include "bt_mw_common.h"
#include "bluetooth_sync.h"
#include "bluetooth.h"
#include "bt_mw_hid_cli.h"
#include "bt_mw_hidh.h"
//#include "bt_mw_hidd.h"
#include "c_bt_mw_gap.h"
#include "c_bt_mw_hidh.h"
//#include "c_bt_mw_hidd.h"



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
INT32 _bluetooth_hid_connect_cli(INT32 argc, const CHAR **argv)
{
    CHAR bt_addr[18];
    if (2 == argc)
    {
        strncpy(bt_addr, argv[1], 17);
        bt_addr[17] = '\0';
        return bluetooth_hid_connect(bt_addr);
    }
    else
    {
        BT_DBG_ERROR(BT_DEBUG_HID, "param number error!");
        return BT_ERR_STATUS_INVALID_PARM_NUMS;
    }
}

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
INT32 _bluetooth_hid_disconnect_cli(INT32 argc, const CHAR **argv)
{
    CHAR bt_addr[18];
    if (2 == argc)
    {
        strncpy(bt_addr, argv[1], 17);
        bt_addr[17] = '\0';
        return bluetooth_hid_disconnect(bt_addr);
    }
    else
    {
        BT_DBG_ERROR(BT_DEBUG_HID, "param number error!");
        return BT_ERR_STATUS_INVALID_PARM_NUMS;
    }
}

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
INT32 _bluetooth_hid_get_input_report_cli(INT32 argc, const CHAR **argv)
{
    CHAR bt_addr[18];
    int report_id = 0;
    int buffer_size = 0;
    if (4 == argc)
    {
        strncpy(bt_addr, argv[1], 17);
        bt_addr[17] = '\0';
        report_id = atoi(argv[2]);
        buffer_size = atoi(argv[3]);
        return bluetooth_hid_get_input_report(bt_addr,report_id,buffer_size);
    }
    else
    {
        BT_DBG_ERROR(BT_DEBUG_HID, "SET_INPUT_REPROT [MAC_ADDR][report_id][max_buffer_size]");
        return BT_ERR_STATUS_INVALID_PARM_NUMS;
    }
}

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
INT32 _bluetooth_hid_get_output_report_cli(INT32 argc, const CHAR **argv)
{
    CHAR bt_addr[18];
    int report_id = 0;
    int buffer_size = 0;
    if (4 == argc)
    {
        strncpy(bt_addr, argv[1], 17);
        bt_addr[17] = '\0';
        report_id = atoi(argv[2]);
        buffer_size = atoi(argv[3]);
        return bluetooth_hid_get_output_report(bt_addr,report_id,buffer_size);
    }
    else
    {
        BT_DBG_ERROR(BT_DEBUG_HID, "GET_OUTPUT_REPROT [MAC_ADDR][report_id][max_buffer_size]");
        return BT_ERR_STATUS_INVALID_PARM_NUMS;
    }
}

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
INT32 _bluetooth_hid_set_input_report_cli(INT32 argc, const CHAR **argv)
{
    CHAR bt_addr[18];
    char report_data[1024];
    if (3 == argc)
    {
        strncpy(bt_addr, argv[1], 17);
        bt_addr[17] = '\0';
        strncpy(report_data,argv[2],strlen(argv[2]));
        report_data[strlen(argv[2])] = '\0';
        return bluetooth_hid_set_input_report(bt_addr,report_data);
    }
    else
    {
        BT_DBG_ERROR(BT_DEBUG_HID, "SET_INPUT_REPROT [MAC_ADDR][report_data(HEX)]");
        return BT_ERR_STATUS_INVALID_PARM_NUMS;
    }
}

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
INT32 _bluetooth_hid_set_output_report_cli(INT32 argc, const CHAR **argv)
{
    CHAR bt_addr[18];
    char report_data[1024];
    if (3 == argc)
    {
        strncpy(bt_addr, argv[1], 17);
        bt_addr[17] = '\0';
        strncpy(report_data,argv[2],strlen(argv[2]));
        report_data[strlen(argv[2])] = '\0';
        return bluetooth_hid_set_output_report(bt_addr,report_data);
    }
    else
    {
        BT_DBG_ERROR(BT_DEBUG_HID, "SET_OUTPUT_REPROT [MAC_ADDR][report_data(HEX)]");
        return BT_ERR_STATUS_INVALID_PARM_NUMS;
    }
}

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
INT32 _bluetooth_hid_get_feature_report_cli(INT32 argc, const CHAR **argv)
{
    CHAR bt_addr[18];
    int report_id = 0;
    int buffer_size = 0;;
    if (4 == argc)
    {
        strncpy(bt_addr, argv[1], 17);
        bt_addr[17] = '\0';
        report_id = atoi(argv[2]);
        buffer_size = atoi(argv[3]);
        return bluetooth_hid_get_feature_report(bt_addr,report_id,buffer_size);
    }
    else
    {
        BT_DBG_ERROR(BT_DEBUG_HID, "get_feature_report [MAC_ADDR][report_id][max_buffer_size]");
        return BT_ERR_STATUS_INVALID_PARM_NUMS;
    }
}

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
 *
 *  PTS test case: TC_HOS_HCT_BV_03_C
 *  cli: set_feature_report 00:1a:7d:da:71:07 05313233
 *       report_id = 05
 *       report_data( 3 bytes) = 313233
 ***********************************************************************************************/

INT32 _bluetooth_hid_set_feature_report_cli(INT32 argc, const CHAR **argv)
{
    CHAR bt_addr[18];
    char report_data[1024];
    if (3 == argc)
    {
        strncpy(bt_addr, argv[1], 17);
        bt_addr[17] = '\0';
        strncpy(report_data,argv[2],strlen(argv[2]));
        report_data[strlen(argv[2])] = '\0';
        return bluetooth_hid_set_feature_report(bt_addr,report_data);
    }
    else
    {
        BT_DBG_ERROR(BT_DEBUG_HID, "set_feature_report [MAC_ADDR][report_data]");
        return BT_ERR_STATUS_INVALID_PARM_NUMS;
    }
}
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
INT32 _bluetooth_hid_get_protocol_cli(INT32 argc, const CHAR **argv)
{
    CHAR bt_addr[18];
    if (2 == argc)
    {
        strncpy(bt_addr, argv[1], 17);
        bt_addr[17] = '\0';
        return bluetooth_hid_get_protocol(bt_addr);
    }
    else
    {
        BT_DBG_ERROR(BT_DEBUG_HID, "get_protocol [MAC_ADDR]");
        return BT_ERR_STATUS_INVALID_PARM_NUMS;
    }
}

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
INT32 _bluetooth_hid_set_protocol_cli(INT32 argc, const CHAR **argv)
{
    CHAR bt_addr[18];
    int protocol_mode = 0;
    if (3 == argc)
    {
        strncpy(bt_addr, argv[1], 17);
        bt_addr[17] = '\0';
        protocol_mode = atoi(argv[2]);
        return bluetooth_hid_set_protocol(bt_addr,protocol_mode);
    }
    else
    {
        BT_DBG_ERROR(BT_DEBUG_HID, "set_protocol [MAC_ADDR][1:BOOT 0:REPORT]");
        return BT_ERR_STATUS_INVALID_PARM_NUMS;
    }
}


INT32 _bluetooth_hid_virtual_unplug_cli(INT32 argc, const CHAR **argv){
    CHAR bt_addr[18];
    if (2 == argc)
    {
        strncpy(bt_addr, argv[1], 17);
        bt_addr[17] = '\0';
        return bluetooth_hid_virtual_unplug(bt_addr);
    }
    else
    {
        BT_DBG_ERROR(BT_DEBUG_HID, "param number error!");
        return BT_ERR_STATUS_INVALID_PARM_NUMS;
    }

}

INT32 _bluetooth_hid_send_data_cli(INT32 argc, const CHAR **argv)
{
    CHAR bt_addr[18];
    char send_data[1024];
    if (3 == argc)
    {
        strncpy(bt_addr, argv[1], 17);
        bt_addr[17] = '\0';
        strncpy(send_data, argv[2], strlen(argv[2]));
        send_data[strlen(argv[2])] = '\0';
        return bluetooth_hid_send_data(bt_addr, send_data);
    }
    else
    {
        BT_DBG_ERROR(BT_DEBUG_HID, "SEND_DATA [MAC_ADDR][send_data(HEX)]");
        return BT_ERR_STATUS_INVALID_PARM_NUMS;
    }
}
#if 0
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
INT32 _bluetooth_hid_send_control_cli(INT32 argc, const CHAR **argv)
{
    CHAR bt_addr[18];
    int pcontrol_mode = 0;
    if (3 == argc)
    {
        strncpy(bt_addr, argv[1], 17);
        bt_addr[17] = '\0';
        pcontrol_mode = atoi(argv[2]);
        return bluetooth_hid_send_control(bt_addr,pcontrol_mode);
    }
    else
    {
        BT_DBG_ERROR(BT_DEBUG_HID, "send_control [MAC_ADDR][3:SUSPEND 4:EXIT_SUSPEND 5:VIRTUAL_CABLE_UNPLUG]");
        return BT_ERR_STATUS_INVALID_PARM_NUMS;
    }
}
#endif
INT32 _bluetooth_scan_hid_a2dp_cli(INT32 argc, const CHAR **argv)
{
    INT32 a2dp_type = 0;
    INT32 hid_type = 0;
    if (argc != 3)
    {
        BT_DBG_NORMAL(BT_DEBUG_COMM, "A parameter is required!");
        return BT_ERR_STATUS_INVALID_PARM_NUMS;
    }
    else
    {
        a2dp_type = atoi(argv[1]);
        hid_type = atoi(argv[2]);
        if (( 1 == a2dp_type) && (0 == hid_type))
            c_btm_start_inquiry_scan(BT_INQUIRY_FILTER_TYPE_A2DP_SNK,NULL);
        else if ((0 == a2dp_type) && (1 == hid_type))
            c_btm_start_inquiry_scan(BT_INQUIRY_FILTER_TYPE_HID,NULL);
        else if ((1 == a2dp_type) && (1 == hid_type))
            c_btm_start_inquiry_scan(BT_INQUIRY_FILTER_TYPE_A2DP_HID,NULL);
        else
            BT_DBG_NORMAL(BT_DEBUG_COMM, "Invalid required!");
    }
    return BT_SUCCESS;
}

#if ENABLE_HID_PROFILE_D
/*hidd*/
/*****************************************************************************************
 * FUNCTION NAME: _bluetooth_hidd_connect_cli
 * PURPOSE:
 *      The function is used to connect hid host
 * INPUT:
 *      argc               -- paramter number
 *      argv[0]            -- hid_cc
 *      argv[1]            -- host address
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
INT32 _bluetooth_hidd_connect_cli(INT32 argc, const CHAR **argv)
{
    CHAR bt_addr[18];
    if (2 == argc)
    {
        strncpy(bt_addr, argv[1], 17);
        bt_addr[17] = '\0';
        return c_bt_hidd_connect(bt_addr);
    }
    else
    {
        BT_DBG_ERROR(BT_DEBUG_HID, "param number error!");
        return BT_ERR_STATUS_INVALID_PARM_NUMS;
    }
}

/*****************************************************************************************
 * FUNCTION NAME: _bluetooth_hidd_disconnect_cli
 * PURPOSE:
 *      The function is used to disconnect hid host
 * INPUT:
 *      argc               -- paramter number
 *      argv[0]            -- hid_dc
 *      argv[1]            -- host address
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
INT32 _bluetooth_hidd_disconnect_cli(INT32 argc, const CHAR **argv)
{
    CHAR bt_addr[18];
    if (2 == argc)
    {
        strncpy(bt_addr, argv[1], 17);
        bt_addr[17] = '\0';
        return c_bt_hidd_disconnect(bt_addr);
    }
    else
    {
        BT_DBG_ERROR(BT_DEBUG_HID, "param number error!");
        return BT_ERR_STATUS_INVALID_PARM_NUMS;
    }
}

/*****************************************************************************************
 * FUNCTION NAME: _bluetooth_hidd_send_keyboard_data_cli
 * PURPOSE:
 *      The function is used to send keyboard data to host
 * INPUT:
 *      argc               -- paramter number
 *      argv[0]            -- data
 *      argv[1]            -- dataSize
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

INT32 _bluetooth_hidd_send_keyboard_data_cli(INT32 argc, const CHAR **argv)
{
    CHAR report_data[1024];
    int dataSize = 0;
    if (3 == argc)
    {
        strncpy(report_data,argv[1],strlen(argv[1]));
        report_data[strlen(argv[1])] = '\0';
        dataSize = atoi(argv[2]);
        return c_bt_hidd_send_keyboard_data(report_data, dataSize);
    }
    else
    {
        BT_DBG_ERROR(BT_DEBUG_HID, "param number error!");
        return BT_ERR_STATUS_INVALID_PARM_NUMS;
    }
}

/*****************************************************************************************
 * FUNCTION NAME: _bluetooth_hidd_send_mouse_data_cli
 * PURPOSE:
 *      The function is used to send mosue data to host
 * INPUT:
 *      argc               -- paramter number
 *      argv[0]            -- data
 *      argv[1]            -- dataSize
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

INT32 _bluetooth_hidd_send_mouse_data_cli(INT32 argc, const CHAR **argv)
{
    CHAR report_data[1024];
    int dataSize = 0;
    if (3 == argc)
    {
        strncpy(report_data,argv[1],strlen(argv[1]));
        report_data[strlen(argv[1])] = '\0';
        dataSize = atoi(argv[2]);
        return c_bt_hidd_send_mouse_data(report_data, dataSize);
    }
    else
    {
        BT_DBG_ERROR(BT_DEBUG_HID, "param number error!");
        return BT_ERR_STATUS_INVALID_PARM_NUMS;
    }
}

/*****************************************************************************************
 * FUNCTION NAME: _bluetooth_hidd_send_consumer_data_cli
 * PURPOSE:
 *      The function is used to send mosue data to host
 * INPUT:
 *      argc               -- paramter number
 *      argv[0]            -- data
 *      argv[1]            -- dataSize
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

INT32 _bluetooth_hidd_send_consumer_data_cli(INT32 argc, const CHAR **argv)
{
    CHAR report_data[1024];
    int dataSize = 0;
    if (3 == argc)
    {
        strncpy(report_data,argv[1],strlen(argv[1]));
        report_data[strlen(argv[1])] = '\0';
        dataSize = atoi(argv[2]);
        return c_bt_hidd_send_consumer_data(report_data, dataSize);
    }
    else
    {
        BT_DBG_ERROR(BT_DEBUG_HID, "param number error!");
        return BT_ERR_STATUS_INVALID_PARM_NUMS;
    }
}

/*****************************************************************************************
 * FUNCTION NAME: _bluetooth_hidd_send_data_cli
 * PURPOSE:
 *      The function is used to send data to host
 * INPUT:
 *      argc               -- paramter number
 *      argv[0]            -- data
 *      argv[1]            -- dataSize
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

INT32 _bluetooth_hidd_send_data_cli(INT32 argc, const CHAR **argv)
{
    CHAR report_data[1024];
    int dataSize = 0;
    if (3 == argc)
    {
        strncpy(report_data,argv[1],strlen(argv[1]));
        report_data[strlen(argv[1])] = '\0';
        dataSize = atoi(argv[2]);
        return c_bt_hidd_send_data(report_data, dataSize);
    }
    else
    {
        BT_DBG_ERROR(BT_DEBUG_HID, "param number error!");
        return BT_ERR_STATUS_INVALID_PARM_NUMS;
    }
}


#endif




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

/* FILE NAME:  bt_mw_gatt_cli.c
 * AUTHOR: Xuemei Yang
 * PURPOSE:
 *      It provides GATTC and GATTS operation command implementation.
 * NOTES:
 */


/*-----------------------------------------------------------------------------
                    include files
 ----------------------------------------------------------------------------*/
#if 0
#include <pthread.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#if ENABLE_MTK_TV_PLATFORM
#include "inc/x_common.h"
#include "dbg/x_dbg.h"
#endif
//#include "x_cli.h"
#include "bt_mw_common.h"
#include "linuxbt_common.h"
#include "bt_mw_gatt.h"
#include "bt_mw_gatt_cli.h"


static CLI_EXEC_T at_bluetooth_gatt_cmd_tbl[] =
{
    {
        "gattc_register_app",
        "gattc_register_app",
        _bluetooth_gattc_register_app_cli,
        NULL,
        "gattc_register_app uuid",
        CLI_GUEST
    },
    {
        "gattc_unregister_app",
        "gattc_unregister_app",
        _bluetooth_gattc_unregister_app_cli,
        NULL,
        "gattc_unregister_app client_if",
        CLI_GUEST
    },
    {
        "gattc_start_scan",
        "gattc_start_scan",
        _bluetooth_gattc_scan_cli,
        NULL,
        "gattc_start_scan",
        CLI_GUEST
    },
    {
        "gattc_stop_scan",
        "gattc_stop_scan",
        _bluetooth_gattc_stop_scan_cli,
        NULL,
        "gattc_stop_scan",
        CLI_GUEST
    },
    {
        "gattc_open",
        "gattc_open",
        _bluetooth_gattc_open_cli,
        NULL,
        "gattc_open client_if BD_ADDR [isDirect:true|false] [transport]",
        CLI_GUEST
    },
    {
        "gattc_close",
        "gattc_close",
        _bluetooth_gattc_close_cli,
        NULL,
        "gattc_close client_if BD_ADDR conn_id",
        CLI_GUEST
    },
    {
        "gattc_listen",
        "gattc_listen",
        _bluetooth_gattc_listen_cli,
        NULL,
        "gattc_listen client_if",
        CLI_GUEST
    },
    {
        "gattc_refresh",
        "gattc_refresh",
        _bluetooth_gattc_refresh_cli,
        NULL,
        "gattc_refresh client_if BD_ADDR",
        CLI_GUEST
    },
    {
        "gattc_search",
        "gattc_search",
        _bluetooth_gattc_search_service_cli,
        NULL,
        "gattc_search conn_id [uuid]",
        CLI_GUEST
    },
    {
        "gattc_get_gatt_db",
        "gattc_get_gatt_db",
        _bluetooth_gattc_get_gatt_db_cli,
        NULL,
        "gattc_get_gatt_db conn_id",
        CLI_GUEST
    },
    {
        "gattc_get_char",
        "gattc_get_char",
        _bluetooth_gattc_get_char_cli,
        NULL,
        "gattc_get_char conn_id service_uuid inst_id [char_uuid] [char_inst_id]",
        CLI_GUEST
    },
    {
        "gattc_get_descr",
        "gattc_get_descr",
        _bluetooth_gattc_get_descr_cli,
        NULL,
        "gattc_get_descr conn_id service_uuid inst_id char_uuid char_inst_id [desc_uuid] [desc_inst_id]",
        CLI_GUEST
    },
    {
        "gattc_get_incl_service",
        "gattc_get_incl_service",
        _bluetooth_gattc_get_incl_service_cli,
        NULL,
        "gattc_get_incl_service conn_id service_uuid inst_id [included_service_uuid] [included_service_inst_id]",
        CLI_GUEST
    },
    {
        "gattc_read_char",
        "gattc_read_char",
        _bluetooth_gattc_read_char_cli,
        NULL,
        "gattc_read_char conn_id char_handle [auth_req]",
        CLI_GUEST
    },
    {
        "gattc_read_descr",
        "gattc_read_descr",
        _bluetooth_gattc_read_descr_cli,
        NULL,
        "gattc_read_descr conn_id descr_handle [auth_req]",
        CLI_GUEST
    },
    {
        "gattc_write_char",
        "gattc_write_char",
        _bluetooth_gattc_write_char_cli,
        NULL,
        "gattc_write_char conn_id char_handle write_type [auth_req] value",
        CLI_GUEST
    },
    {
        "gattc_write_descr",
        "gattc_write_descr",
        _bluetooth_gattc_write_descr_cli,
        NULL,
        "gattc_write_descr conn_id descr_handle write_type [auth_req] value",
        CLI_GUEST
    },
    {
        "gattc_reg_noti",
        "gattc_reg_noti",
        _bluetooth_gattc_reg_noti_cli,
        NULL,
        "gattc_reg_noti client_if BD_ADDR char_handle",
        CLI_GUEST
    },
    {
        "gattc_dereg_noti",
        "gattc_dereg_noti",
        _bluetooth_gattc_dereg_noti_cli,
        NULL,
        "gattc_dereg_noti client_if BD_ADDR char_handle",
        CLI_GUEST
    },
    {
        "gattc_rssi",
        "gattc_rssi",
        _bluetooth_gattc_read_rssi_cli,
        NULL,
        "gattc_rssi client_if BD_ADDR",
        CLI_GUEST
    },
    {
        "gattc_parse_adv_data",
        "gattc_parse_adv_data",
        _bluetooth_gatt_parse_adv_data_cli,
        NULL,
        "gattc_parse_adv_data [data]",
        CLI_GUEST
    },
    {
        "gattc_adv_enable",
        "gattc_adv_enable",
        _bluetooth_gattc_multi_adv_enable_cli,
        NULL,
        "gattc_adv_enable client_if min_interval max_interval adv_type tx_power timeout",
        CLI_GUEST
    },
    {
        "gattc_adv_update",
        "gattc_adv_update",
        _bluetooth_gattc_multi_adv_update_cli,
        NULL,
        "gattc_adv_update client_if min_interval max_interval adv_type tx_power timeout",
        CLI_GUEST
    },
    {
        "gattc_adv_update_data",
        "gattc_adv_update_data",
        _bluetooth_gattc_multi_adv_setdata_cli,
        NULL,
        "gattc_adv_update_data client_if [set_scan_rsp true|false] [include_name true|false] [incl_txpower true|false] [appearance int] [manufacturer_data hex_string] [service_data hex_string] [service_uuid hex_string] [le_name device_name]",
        CLI_GUEST
    },
    {
        "gattc_adv_disable",
        "gattc_adv_disable",
        _bluetooth_gattc_multi_adv_disable_cli,
        NULL,
        "gattc_adv_disable client_interface",
        CLI_GUEST
    },
    {
        "gattc_scan_filter_param_setup",
        "gattc_scan_filter_param_setup",
        _bluetooth_gattc_scan_filter_param_setup_cli,
        NULL,
        "gattc_can_filter_param_setup client_if action [filt_index int] [feat_seln int] [list_logic_type hex_string] [filt_logic_type int] [rssi_high_thres int] [rssi_low_thres int] [dely_mode int] [found_timeout int] [lost_timeout int] [found_timeout_cnt int] [num_of_tracking_entries int]",
        CLI_GUEST
    },
    {
        "gattc_scan_filter_enable",
        "gattc_scan_filter_enable",
        _bluetooth_gattc_scan_filter_enable_cli,
        NULL,
        "gattc_scan_filter_enable client_if",
        CLI_GUEST
    },
    {
        "gattc_scan_filter_disable",
        "gattc_scan_filter_disable",
        _bluetooth_gattc_scan_filter_disable_cli,
        NULL,
        "gattc_scan_filter_disable client_if",
        CLI_GUEST
    },
    {
        "gattc_scan_filter_add_remove",
        "gattc_scan_filter_add_remove",
        _bluetooth_gattc_scan_filter_add_remove_cli,
        NULL,
        "gattc_scan_filter_add_remove client_if action [filt_index int] [filt_type int] [company_id hex_string] [company_id_mask hex_string] [uuid hex_string] [uuid_mask hex_string] [bd_addr string] [addr_type int] [data hex_string/string] [data_mask hex_string/string]",
        CLI_GUEST
    },
    {
        "gattc_scan_filter_clear",
        "gattc_scan_filter_clear",
        _bluetooth_gattc_scan_filter_clear_cli,
        NULL,
        "gattc_scan_filter_clear client_if filt_index",
        CLI_GUEST
    },
    {
        "gattc_set_local_le_name",
        "gattc_set_local_le_name",
        _bluetooth_gattc_set_local_le_name_cli,
        NULL,
        "gattc_set_le_name client_if le_name",
        CLI_GUEST
    },
    {
        "gattc_config_mtu",
        "gattc_config_mtu",
        _bluetooth_gattc_configure_mtu_cli,
        NULL,
        "gattc_config_mtu conn_id mtu",
        CLI_GUEST
    },
    {
        "gatts_register_server",
        "gatts_register_server",
        _bluetooth_gatts_register_server_cli,
        NULL,
        "gatts_register_server uuid",
        CLI_GUEST
    },
    {
        "gatts_unregister_server",
        "gatts_unregister_server",
        _bluetooth_gatts_unregister_server_cli,
        NULL,
        "gatts_unregister_server server_if",
        CLI_GUEST
    },
    {
        "gatts_cc",
        "gatts_cc",
        _bluetooth_gatts_open_cli,
        NULL,
        "gatts_cc server_if BD_ADDR [is_direct] [transport]",
        CLI_GUEST
    },
    {
        "gatts_dc",
        "gatts_dc",
        _bluetooth_gatts_close_cli,
        NULL,
        "gatts_dc server_if BD_ADDR conn_id",
        CLI_GUEST
    },
    {
        "gatts_add_service",
        "gatts_add_service",
        _bluetooth_gatts_add_service_cli,
        NULL,
        "gatts_add_service server_if uuid [is_primary:true|false] [number_of_handles]",
        CLI_GUEST
    },
    {
        "gatts_add_incl_service",
        "gatts_add_incl_service",
        _bluetooth_gatts_add_incl_service_cli,
        NULL,
        "gatts_add_incl_service server_if service_handle included_handle",
        CLI_GUEST
    },
    {
        "gatts_add_char",
        "gatts_add_char",
        _bluetooth_gatts_add_char_cli,
        NULL,
        "gatts_add_char server_if service_handle uuid [properties] [permissions]",
        CLI_GUEST
    },
    {
        "gatts_add_desc",
        "gatts_add_desc",
        _bluetooth_gatts_add_desc_cli,
        NULL,
        "gatts_add_desc server_if service_handle uuid [permissions]",
        CLI_GUEST
    },
    {
        "gatts_start_service",
        "gatts_start_service",
        _bluetooth_gatts_start_service_cli,
        NULL,
        "gatts_start_service server_if service_handle [transport]",
        CLI_GUEST
    },
    {
        "gatts_stop_service",
        "gatts_stop_service",
        _bluetooth_gatts_stop_service_cli,
        NULL,
        "gatts_stop_service server_if service_handle",
        CLI_GUEST
    },
    {
        "gatts_delete_service",
        "gatts_delete_service",
        _bluetooth_gatts_delete_service_cli,
        NULL,
        "gatts_delete_service server_if service_handle",
        CLI_GUEST
    },
    {
        "gatts_send_indi",
        "gatts_send_indi",
        _bluetooth_gatts_send_indication_cli,
        NULL,
        "gatts_send_indi server_if attribute_handle conn_id [confirm] value",
        CLI_GUEST
    },
    {
        "gatts_send_rsp",
        "gatts_send_rsp",
        _bluetooth_gatts_send_response_cli,
        NULL,
        "gatts_send_rsp conn_id trans_id status handle [auth_req] value",
        CLI_GUEST
    },
    END_OF_CLI_CMD_TBL
};

/*gattc*/

/**
 * FUNCTION NAME: _bluetooth_add_gatt_cli_table
 * PURPOSE:
 *      The function is used to add gatt cli cmd table
 * INPUT:
 *      argc               -- paramter number
 *      argv[0]            -- gattc_register_app
 * OUTPUT:
 *      None
 * RETURN:
 *      at_bluetooth_gatt_cmd_tbl;
 * NOTES:
 *      None
 */

static CLI_EXEC_T at_ble_root_cmd_tbl[] =
{
    {
        "ble",
        NULL, NULL,
        at_bluetooth_gatt_cmd_tbl,
        "ble commands",
        CLI_GUEST
    },
    END_OF_CLI_CMD_TBL
};

INT32 x_ble_cli_init (VOID)
{
    BT_DBG_NORMAL(BT_DEBUG_GATT,"start x_ble_cli_init");
    /* Attach ble  CLI command table to MW CLI */
    return (x_cli_attach_cmd_tbl(at_ble_root_cmd_tbl, CLI_CAT_MW, CLI_GRP_NONE));

}


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

INT32 _bluetooth_gattc_register_app_cli (INT32 argc, const CHAR **argv)
{
    CHAR ptr[130];
    //bt_uuid_t uuid;
    memset(ptr,0,sizeof(ptr));
    if (argc < 2)
    {
        BT_DBG_ERROR(BT_DEBUG_GATT,"input error");
        BT_DBG_ERROR(BT_DEBUG_GATT,"please input gattc_register_app uuid");
        return BT_ERR_STATUS_INVALID_PARM_NUMS;
    }
    strncpy(ptr, argv[1], strlen(argv[1]));
    ptr[strlen(argv[1])] = '\0';
    //bt_gatt_uuid_stoh(ptr, &uuid);

    return bluetooth_gattc_register_app(ptr);
}

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
INT32 _bluetooth_gattc_unregister_app_cli (INT32 argc, const CHAR **argv)
{
    INT32 client_if = 0;
    if (argc < 2)
    {
        BT_DBG_ERROR(BT_DEBUG_GATT,"input error");
        BT_DBG_ERROR(BT_DEBUG_GATT,"please input gattc_unregister_app client_if");
        return BT_ERR_STATUS_INVALID_PARM_NUMS;
    }
    client_if = atoi(argv[1]);

    return bluetooth_gattc_unregister_app(client_if);
}

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

INT32 _bluetooth_gattc_scan_cli (INT32 argc, const CHAR **argv)
{
    if (1 == argc)
    {
        return bluetooth_gattc_scan();
    }
    else
    {
        BT_DBG_ERROR(BT_DEBUG_GATT,"input error");
        BT_DBG_ERROR(BT_DEBUG_GATT,"please input gattc_start_scan");
        return BT_ERR_STATUS_INVALID_PARM_NUMS;
    }
}

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
INT32 _bluetooth_gattc_stop_scan_cli (INT32 argc, const CHAR **argv)
{
    if (1 == argc)
    {
        return bluetooth_gattc_stop_scan();
    }
    else
    {
        BT_DBG_ERROR(BT_DEBUG_GATT,"input error");
        BT_DBG_ERROR(BT_DEBUG_GATT,"please input gattc_stop_scan");
        return BT_ERR_STATUS_INVALID_PARM_NUMS;
    }
}

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
INT32 _bluetooth_gattc_open_cli (INT32 argc, const CHAR **argv)
{
    CHAR ps_addr[MAX_BDADDR_LEN];
    UINT8 is_direct = 0;
    INT32 transport= 0;
    INT32 client_if = 0;
    if (argc < 3 )
    {
        BT_DBG_ERROR(BT_DEBUG_GATT,"please input gattc_open client_if BD_ADDR [isDirect:true|false] [transport]");
        return BT_ERR_STATUS_INVALID_PARM_NUMS;
    }
    client_if = atoi(argv[1]);
    strncpy(ps_addr,argv[2], MAX_BDADDR_LEN);
    ps_addr[MAX_BDADDR_LEN - 1] = '\0';
    if (argc >= 4)
    {
        is_direct = atoi(argv[3]);
        BT_DBG_INFO(BT_DEBUG_GATT,"is_direct:%d", is_direct);
    }
    if (argc >= 5)
    {
        transport = atoi(argv[4]);
        BT_DBG_INFO(BT_DEBUG_GATT,"transport:%ld", (long)transport);
    }

    BT_DBG_INFO(BT_DEBUG_GATT,"gattc_open %s %d %ld", ps_addr, is_direct,(long)transport);

    return bluetooth_gattc_open(client_if,ps_addr,is_direct,transport);
}

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
INT32 _bluetooth_gattc_close_cli (INT32 argc, const CHAR **argv)
{
    CHAR ps_addr[MAX_BDADDR_LEN];
    INT32 conn_id;
    INT32 client_if = 0;
    if (argc != 4 )
    {
        BT_DBG_ERROR(BT_DEBUG_GATT,"please input gattc_close client_if BD_ADDR conn_id");
        return BT_ERR_STATUS_INVALID_PARM_NUMS;
    }

    client_if = atoi(argv[1]);
    strncpy(ps_addr,argv[2], MAX_BDADDR_LEN);
    ps_addr[MAX_BDADDR_LEN - 1] = '\0';
    conn_id = atoi(argv[3]);
    BT_DBG_INFO(BT_DEBUG_GATT,"gattc_close %ld %s %ld", (long)client_if, ps_addr, (long)conn_id);

    return bluetooth_gattc_close(client_if,ps_addr,conn_id);
}

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
INT32 _bluetooth_gattc_listen_cli (INT32 argc, const CHAR **argv)
{
    INT32 client_if = 0;
    if (2 == argc)
    {
        client_if = atoi(argv[1]);
        return bluetooth_gattc_listen(client_if);
    }
    else
    {
        BT_DBG_ERROR(BT_DEBUG_GATT,"input error");
        BT_DBG_ERROR(BT_DEBUG_GATT,"please input gattc_listen client_if");
        return BT_ERR_STATUS_INVALID_PARM_NUMS;
    }
}

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
INT32 _bluetooth_gattc_refresh_cli (INT32 argc, const CHAR **argv)
{
    CHAR ps_addr[MAX_BDADDR_LEN];
    INT32 client_if = 0;
    if (3 == argc)
    {
        client_if = atoi(argv[1]);
        strncpy(ps_addr,argv[2], MAX_BDADDR_LEN);
        ps_addr[MAX_BDADDR_LEN - 1] = '\0';
        return bluetooth_gattc_refresh(client_if, ps_addr);
    }
    else
    {
        BT_DBG_ERROR(BT_DEBUG_GATT,"input error");
        BT_DBG_ERROR(BT_DEBUG_GATT,"please input gattc_refresh client_if BD_ADDR");
        return BT_ERR_STATUS_INVALID_PARM_NUMS;
    }
}

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
INT32 _bluetooth_gattc_search_service_cli (INT32 argc, const CHAR **argv)
{
    CHAR pt_uuid[130] = {0};
    CHAR *pt_uuid_ptr = NULL;
    INT32 conn_id = 0;
    //bt_uuid_t bt_uuid;
    if (argc < 2 )
    {
        BT_DBG_ERROR(BT_DEBUG_GATT,"please input gattc_search conn_id [uuid]");
        return BT_ERR_STATUS_INVALID_PARM_NUMS;
    }
    conn_id = atoi(argv[1]);
    if (argc == 3 )
    {
        strncpy(pt_uuid,argv[2], strlen(argv[2]));
        pt_uuid[strlen(argv[2])] = '\0';
        pt_uuid_ptr = pt_uuid;
        //bt_gatt_uuid_stoh(pt_uuid,&bt_uuid);
        BT_DBG_INFO(BT_DEBUG_GATT,"gattc_search %ld %s",(long)conn_id,pt_uuid);
    }
    else
    {
        BT_DBG_INFO(BT_DEBUG_GATT,"gattc_search %ld",(long)conn_id);
        pt_uuid_ptr = NULL;
    }

    return bluetooth_gattc_search_service(conn_id, pt_uuid_ptr);
}

INT32 _bluetooth_gattc_get_gatt_db_cli (INT32 argc, const CHAR **argv)
{
    INT32 conn_id = 0;
    if (2 == argc)
    {
        conn_id = atoi(argv[1]);
        return bluetooth_gattc_get_gatt_db(conn_id);
    }
    else
    {
        BT_DBG_ERROR(BT_DEBUG_GATT,"input error");
        BT_DBG_ERROR(BT_DEBUG_GATT,"please input gattc_get_gatt_db conn_id");
        return BT_ERR_STATUS_INVALID_PARM_NUMS;
    }
}

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
INT32 _bluetooth_gattc_get_char_cli (INT32 argc, const CHAR **argv)
{
    CHAR pt_srvc_uuid[130];
    CHAR pt_start_uuid[130];
    INT32 conn_id;
    //bt_uuid_t pt_service_uuid;
    INT32 service_inst_id;
    //bt_uuid_t pt_start_uuid;
    INT32 start_inst_id;
    if (argc < 4 )
    {
        BT_DBG_ERROR(BT_DEBUG_GATT,"please input gattc_get_char conn_id service_uuid inst_id [characteristic_uuid] [characteristic_inst_id]");
        return BT_ERR_STATUS_INVALID_PARM_NUMS;
    }
    conn_id = atoi(argv[1]);
    strncpy(pt_srvc_uuid, argv[2], strlen(argv[2]));
    pt_srvc_uuid[strlen(argv[2])] = '\0';
    //bt_gatt_uuid_stoh(pt_uuid,&pt_service_uuid);
    service_inst_id = atoi(argv[3]);
    if (argc >= 5 )
    {
        memset(pt_start_uuid,0,sizeof(pt_start_uuid));
        strncpy(pt_start_uuid, argv[4], strlen(argv[4]));
        pt_start_uuid[strlen(argv[4])] = '\0';
        //bt_gatt_uuid_stoh(pt_uuid,&pt_start_uuid);
    }
    if (argc >= 6 )
    {
        start_inst_id = atoi(argv[5]);
    }

    return bluetooth_gattc_get_char(conn_id,
                                    pt_srvc_uuid,
                                    service_inst_id,
                                    pt_start_uuid,
                                    start_inst_id);
}

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
INT32 _bluetooth_gattc_get_descr_cli (INT32 argc, const CHAR **argv)
{
    CHAR pt_service_uuid[130];
    CHAR pt_char_uuid[130];
    CHAR pt_start_uuid[130];
    INT32 conn_id;
    //bt_uuid_t pt_service_uuid;
    INT32 service_inst_id;
    //bt_uuid_t pt_char_uuid;
    INT32 char_inst_id;
    //bt_uuid_t pt_start_uuid;
    INT32 start_inst_id;
    if (argc < 6 )
    {
        BT_DBG_ERROR(BT_DEBUG_GATT,"please input gattc_get_desc conn_id service_uuid inst_id characteristic_uuid characteristic_inst_id [descriptor_uuid] [descriptor_inst_id]");
        return BT_ERR_STATUS_INVALID_PARM_NUMS;
    }
    conn_id = atoi(argv[1]);
    strncpy(pt_service_uuid, argv[2], strlen(argv[2]));
    pt_service_uuid[strlen(argv[2])] = '\0';
    //bt_gatt_uuid_stoh(pt_uuid,&pt_service_uuid);
    service_inst_id = atoi(argv[3]);
    //memset(pt_uuid,0,sizeof(pt_uuid));
    strncpy(pt_char_uuid, argv[4], strlen(argv[4]));
    pt_char_uuid[strlen(argv[4])] = '\0';
    //bt_gatt_uuid_stoh(pt_uuid,&pt_char_uuid);
    char_inst_id = atoi(argv[5]);
    if (argc >= 7 )
    {
        //memset(pt_uuid,0,sizeof(pt_uuid));
        strncpy(pt_start_uuid, argv[6], strlen(argv[6]));
        pt_start_uuid[strlen(argv[6])] = '\0';
        //bt_gatt_uuid_stoh(pt_uuid,&pt_start_uuid);
    }
    if (argc >= 8 )
    {
        start_inst_id = atoi(argv[7]);
    }
    return bluetooth_gattc_get_descr(conn_id,
                                     pt_service_uuid,
                                     service_inst_id,
                                     pt_char_uuid,
                                     char_inst_id,
                                     pt_start_uuid,
                                     start_inst_id);
}

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
INT32 _bluetooth_gattc_get_incl_service_cli (INT32 argc, const CHAR **argv)
{
    CHAR pt_service_uuid[130];
    CHAR pt_incl_uuid[130];
    INT32 conn_id;
    //bt_uuid_t pt_service_uuid;
    INT32 service_inst_id;
    //bt_uuid_t pt_incl_uuid;
    INT32 incl_inst_id;
    if (argc < 4 )
    {
        BT_DBG_ERROR(BT_DEBUG_GATT,"please input gattc_get_incl_service conn_id service_uuid inst_id [included_service_uuid] [included_service_inst_id]");
        return BT_ERR_STATUS_INVALID_PARM_NUMS;
    }
    conn_id = atoi(argv[1]);
    strncpy(pt_service_uuid, argv[2], strlen(argv[2]));
    pt_service_uuid[strlen(argv[2])] = '\0';
    //bt_gatt_uuid_stoh(pt_uuid,&pt_service_uuid);
    service_inst_id = atoi(argv[3]);
    if (argc >= 5 )
    {
        //memset(pt_uuid,0,sizeof(pt_uuid));
        strncpy(pt_incl_uuid, argv[4], strlen(argv[4]));
        pt_incl_uuid[strlen(argv[4])] = '\0';
        //bt_gatt_uuid_stoh(pt_uuid,&pt_incl_uuid);
    }
    if (argc >= 6 )
    {
        incl_inst_id = atoi(argv[5]);
    }
    return bluetooth_gattc_get_incl_service(conn_id,
                                            pt_service_uuid,
                                            service_inst_id,
                                            pt_incl_uuid,
                                            incl_inst_id);
}

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
INT32 _bluetooth_gattc_read_char_cli (INT32 argc, const CHAR **argv)
{

    INT32 conn_id;
    INT32 char_handle;
    INT32 auth_req = 0;
    if (argc < 3 )
    {
        BT_DBG_ERROR(BT_DEBUG_GATT,"please input gattc_read_char conn_id characteristic_handle [auth_req]");
        return BT_ERR_STATUS_INVALID_PARM_NUMS;
    }
    conn_id = atoi(argv[1]);
    char_handle = atoi(argv[2]);
    if (argc >= 4 )
    {
        auth_req = atoi(argv[3]);
    }
    return bluetooth_gattc_read_char(conn_id,
                                     char_handle,
                                     auth_req);
}

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
INT32 _bluetooth_gattc_read_descr_cli (INT32 argc, const CHAR **argv)
{
    INT32 conn_id = 0;
    INT32 descr_handle = 0;
    INT32 auth_req = 0;
    if (argc < 3 )
    {
        BT_DBG_ERROR(BT_DEBUG_GATT,"please input gattc_read_descr conn_id descr_handle [auth_req]");
        return BT_ERR_STATUS_INVALID_PARM_NUMS;
    }
    conn_id = atoi(argv[1]);
    descr_handle = atoi(argv[2]);
    if (4 == argc)
    {
        auth_req = atoi(argv[3]);
    }
    return bluetooth_gattc_read_descr(conn_id,
                                      descr_handle,
                                      auth_req);
}


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
INT32 _bluetooth_gattc_write_char_cli (INT32 argc, const CHAR **argv)
{
    INT32 conn_id = 0;
    INT32 char_handle= 0;
    INT32 auth_req= 0;
    INT32 write_type = 2;
    INT32 len = 0;
    CHAR value[260];
    if (argc < 5 )
    {
        BT_DBG_ERROR(BT_DEBUG_GATT,"please input gattc_write_char conn_id char_handle write_type [auth_req] value");
        return BT_ERR_STATUS_INVALID_PARM_NUMS;
    }
    conn_id = atoi(argv[1]);
    char_handle = atoi(argv[2]);
    write_type = atoi(argv[3]);
    if (argc >= 5)
    {
        strncpy(value,argv[4], strlen(argv[4]));
        value[strlen(argv[4])] = '\0';
        len = strlen(argv[4]);
    }
    if (argc >= 6)
    {
        auth_req = atoi(argv[4]);
        strncpy(value,argv[5], strlen(argv[5]));
        value[strlen(argv[5])] = '\0';
        len = strlen(argv[5]);
    }

    return bluetooth_gattc_write_char(conn_id,
                                      char_handle,
                                      write_type,
                                      len,
                                      auth_req,
                                      value);
}

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
INT32 _bluetooth_gattc_write_descr_cli (INT32 argc, const CHAR **argv)
{
    INT32 conn_id = 0;
    INT32 descr_handle = 0;
    INT32 auth_req = 0;
    INT32 write_type = 2;
    INT32 len = 0;
    CHAR value[260];
    if (argc < 5 )
    {
        BT_DBG_ERROR(BT_DEBUG_GATT,"please input gattc_write_desc conn_id descr_handle write_type [auth_req] value");
        return BT_ERR_STATUS_INVALID_PARM_NUMS;
    }
    conn_id = atoi(argv[1]);
    descr_handle = atoi(argv[2]);
    write_type = atoi(argv[3]);
    if (argc >= 5)
    {
        strncpy(value,argv[4], strlen(argv[4]));
        value[strlen(argv[4])] = '\0';
        len = strlen(argv[4]);
    }
    else if (argc >= 6)
    {
        auth_req = atoi(argv[4]);
        strncpy(value,argv[5], strlen(argv[5]));
        value[strlen(argv[5])] = '\0';
        len = strlen(argv[5]);
    }

    return bluetooth_gattc_write_descr(conn_id,
                                       descr_handle,
                                       write_type,
                                       len,
                                       auth_req,
                                       value);
}

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
INT32 _bluetooth_gattc_reg_noti_cli (INT32 argc, const CHAR **argv)
{
    CHAR bt_addr[MAX_BDADDR_LEN];
    INT32 char_handle = 0;
    INT32 client_if = 0;
    if (argc < 4 )
    {
        BT_DBG_ERROR(BT_DEBUG_GATT,"please input gattc_reg_noti client_if BD_ADDR char_handle");
        return BT_ERR_STATUS_INVALID_PARM_NUMS;
    }
    client_if = atoi(argv[1]);
    strncpy(bt_addr,argv[2], MAX_BDADDR_LEN);
    char_handle = atoi(argv[3]);
    return bluetooth_gattc_reg_noti(client_if,
                                    bt_addr,
                                    char_handle);
}

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
INT32 _bluetooth_gattc_dereg_noti_cli (INT32 argc, const CHAR **argv)
{
    CHAR bt_addr[MAX_BDADDR_LEN];
    INT32 char_handle = 0;
    INT32 client_if = 0;
    if (argc < 4 )
    {
        BT_DBG_ERROR(BT_DEBUG_GATT,"please input gattc_dereg_noti client_if BD_ADDR char_handle");
        return BT_ERR_STATUS_INVALID_PARM_NUMS;
    }
    client_if = atoi(argv[1]);
    strncpy(bt_addr, argv[2], MAX_BDADDR_LEN);
    char_handle = atoi(argv[3]);
    return bluetooth_gattc_dereg_noti(client_if,
                                      bt_addr,
                                      char_handle);
}

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
INT32 _bluetooth_gattc_read_rssi_cli (INT32 argc, const CHAR **argv)
{
    CHAR ps_addr[MAX_BDADDR_LEN];
    INT32 client_if;
    if (argc < 3)
    {
        BT_DBG_ERROR(BT_DEBUG_GATT,"please input gattc_rssi client_if BD_ADDR");
        return BT_ERR_STATUS_INVALID_PARM_NUMS;
    }

    client_if = atoi(argv[1]);

    strncpy(ps_addr, argv[2], MAX_BDADDR_LEN);
    ps_addr[MAX_BDADDR_LEN - 1] = '\0';
    BT_DBG_INFO(BT_DEBUG_GATT,"gattc_rssi %ld %s", (long)client_if, ps_addr);
    return bluetooth_gattc_read_rssi(client_if, ps_addr);
}

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
INT32 _bluetooth_gatt_parse_adv_data_cli (INT32 argc, const CHAR **argv)
{
    UINT8 *adv_data;
    if (argc < 2)
    {
        BT_DBG_ERROR(BT_DEBUG_GATT,"please input gattc_parse_adv_data [data]");
        return BT_ERR_STATUS_INVALID_PARM_NUMS;
    }
    adv_data = (UINT8 *)argv[1];
    bluetooth_gatt_parse_adv_data (adv_data);
    return BT_SUCCESS;
}

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
INT32 _bluetooth_gattc_multi_adv_enable_cli (INT32 argc, const CHAR **argv)
{
    INT32 client_if = 0;
    INT32 min_interval = 0;
    INT32 max_interval = 0;
    INT32 adv_type = 0;
    INT32 chnl_map = 0;
    INT32 tx_power = 0;
    INT32 timeout_s = 0;
    if (argc < 7)
    {
        BT_DBG_ERROR(BT_DEBUG_GATT,"please input gattc_adv_enable client_if min_interval max_interval adv_type tx_power timeout");
        return BT_ERR_STATUS_INVALID_PARM_NUMS;
    }
    client_if = atoi(argv[1]);
    min_interval = atoi(argv[2])*1000/625;
    max_interval = atoi(argv[3])*1000/625;
    adv_type = atoi(argv[4]);
    tx_power = atoi(argv[5]);
    timeout_s = atoi(argv[6]);
    return bluetooth_gattc_multi_adv_enable(client_if,
                                            min_interval,
                                            max_interval,
                                            adv_type,
                                            chnl_map,
                                            tx_power,
                                            timeout_s);
}

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
INT32 _bluetooth_gattc_multi_adv_update_cli (INT32 argc, const CHAR **argv)
{
    INT32 client_if = 0;
    INT32 min_interval = 0;
    INT32 max_interval = 0;
    INT32 adv_type = 0;
    INT32 chnl_map = 0;
    INT32 tx_power = 0;
    INT32 timeout_s = 0;
    if (argc < 7)
    {
        BT_DBG_ERROR(BT_DEBUG_GATT,"please input gattc_adv_update client_if min_interval max_interval adv_type tx_power timeout");
        return BT_ERR_STATUS_INVALID_PARM_NUMS;
    }
    client_if = atoi(argv[1]);
    min_interval = atoi(argv[2]);
    max_interval = atoi(argv[3]);
    adv_type = atoi(argv[4]);
    tx_power = atoi(argv[5]);
    timeout_s = atoi(argv[6]);
    return bluetooth_gattc_multi_adv_update(client_if,
                                            min_interval,
                                            max_interval,
                                            adv_type,
                                            chnl_map,
                                            tx_power,
                                            timeout_s);
}

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
INT32 _bluetooth_gattc_multi_adv_setdata_cli (INT32 argc, const CHAR **argv)
{
    INT32 client_if = 0;
    UINT8 set_scan_rsp = 0;
    UINT8 include_name = 0;
    UINT8 include_txpower = 0;
    INT32 appearance = 0;
    INT32 manufacturer_len = 0;
    CHAR* manufacturer_data = NULL;
    INT32 service_data_len = 0;
    CHAR* service_data = NULL;
    INT32 service_uuid_len = 0;
    CHAR* service_uuid = NULL;
    INT32 le_name_len = 0;
    CHAR* le_name = NULL;
    INT32 count = 0;
    CHAR *hex_buf = NULL;
    BT_ERR_STATUS_T ret = BT_SUCCESS;
    if (argc < 2)
    {
        BT_DBG_ERROR(BT_DEBUG_GATT,"please input gattc_adv_update_data client_if [set_scan_rsp true|false] [include_name true|false] [incl_txpower true|false] [appearance int] [manufacturer_data hex_string] [service_data hex_string] [service_uuid hex_string] [le_name device_name]");
        return BT_ERR_STATUS_INVALID_PARM_NUMS;
    }
    client_if = atoi(argv[1]);
    while (count < argc)
    {
        BT_DBG_NOTICE(BT_DEBUG_GATT,"[GATTC] %s()", argv[count]);
        if (strncmp(argv[count],"set_scan_rsp",strlen("set_scan_rsp")) == 0)
        {
            count++;
            if (strncmp(argv[count],"1",strlen("1")) == 0
                    ||strncmp(argv[count],"true",strlen("true")) == 0
                    ||strncmp(argv[count],"TRUE",strlen("TRUE")) == 0)
            {
                set_scan_rsp = TRUE;
            }
            else
            {
                set_scan_rsp = FALSE;
            }
            count++;
            continue;
        }
        else if (strncmp(argv[count],"include_name",strlen("include_name")) == 0)
        {
            count++;
            if (strncmp(argv[count],"1",strlen("1")) == 0
                    || strncmp(argv[count],"true",strlen("true")) == 0
                    || strncmp(argv[count],"TRUE",strlen("TRUE")) == 0)
            {
                include_name = TRUE;
            }
            else
            {
                include_name = FALSE;
            }
            count++;
            continue;
        }
        else if (strncmp(argv[count],"incl_txpower",strlen("incl_txpower")) == 0)
        {
            count++;
            if (strncmp(argv[count],"1",strlen("TRUE")) == 0
                    ||strncmp(argv[count],"true",strlen("TRUE")) == 0
                    ||strncmp(argv[count],"TRUE",strlen("TRUE")) == 0)
            {
                include_txpower = TRUE;
            }
            else
            {
                include_txpower = FALSE;
            }
            count++;
            continue;
        }
        else if (strncmp(argv[count],"appearance",strlen("appearance")) == 0)
        {
            count++;
            appearance = atoi(argv[count]);
            count++;
            continue;
        }
        else if (strncmp(argv[count],"manufacturer_data",strlen("manufacturer_data")) == 0)
        {
            count++;
            INT32 hex_len = (strlen(argv[count]) + 1) / 2;
            if (NULL != hex_buf)
            {
                free(hex_buf);
            }
            hex_buf = malloc(hex_len * sizeof(CHAR));
            ascii_2_hex((CHAR *)argv[count], hex_len, (UINT8*)hex_buf);
            manufacturer_data = hex_buf;
            manufacturer_len = hex_len;
            count++;
            BT_DBG_INFO(BT_DEBUG_GATT,"manufacturer_len : %ld" ,(long)manufacturer_len);
            continue;
        }
        else if (strncmp(argv[count],"service_data",strlen("service_data")) == 0)
        {
            count++;
            INT32 hex_len = (strlen(argv[count]) + 1) / 2;
            if (NULL != hex_buf)
            {
                free(hex_buf);
            }
            hex_buf = malloc(hex_len * sizeof(CHAR));
            ascii_2_hex((CHAR *)argv[count], hex_len, (UINT8*)hex_buf);
            service_data = hex_buf;
            service_data_len = hex_len;
            count++;
            BT_DBG_INFO(BT_DEBUG_GATT,"service_data_len : %ld" ,(long)service_data_len);
            continue;
        }
        else if (strncmp(argv[count],"service_uuid",strlen("service_uuid")) == 0)
        {
            count++;
            INT32 hex_len = (strlen(argv[count]) + 1) / 2;
            if (NULL != hex_buf)
            {
                free(hex_buf);
            }
            hex_buf = malloc(hex_len * sizeof(CHAR));
            ascii_2_hex((CHAR *)argv[count], hex_len, (UINT8*)hex_buf);
            service_uuid = hex_buf;
            service_uuid_len = hex_len;
            count++;
            BT_DBG_INFO(BT_DEBUG_GATT,"service_uuid_len : %ld" ,(long)service_uuid_len);
            continue;
        }
        else if (strncmp(argv[count],"le_name",strlen("le_name")) == 0)
        {
            count++;
            le_name_len = strlen(argv[count])+1;
            if (NULL != le_name)
            {
                free(le_name);
            }
            le_name = malloc(le_name_len * sizeof(CHAR));
            if (le_name)
            {
                memcpy(le_name, argv[count], le_name_len);
            }
            count++;
            BT_DBG_INFO(BT_DEBUG_GATT,"len = %ld,le_name : %s",(long)le_name_len ,le_name);
            continue;
        }
        count+=2;
    }

    ret = bluetooth_gattc_multi_adv_setdata(client_if,
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
    if (manufacturer_data)
        free(manufacturer_data);
    if (service_data)
        free(service_data);
    if (service_uuid)
        free(service_uuid);
    if (le_name)
        free(le_name);
    return ret;
}

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
INT32 _bluetooth_gattc_multi_adv_disable_cli (INT32 argc, const CHAR **argv)
{
    INT32 client_if = 0;
    if (2 == argc)
    {
        client_if = atoi(argv[1]);
        return bluetooth_gattc_multi_adv_disable(client_if);
    }
    else
    {
        BT_DBG_ERROR(BT_DEBUG_GATT,"input error");
        BT_DBG_ERROR(BT_DEBUG_GATT,"please input gattc_adv_disable client_interface");
        return BT_ERR_STATUS_INVALID_PARM_NUMS;
    }
}

INT32 _bluetooth_gattc_scan_filter_param_setup_cli (INT32 argc, const CHAR **argv)
{
    BT_DBG_NORMAL(BT_DEBUG_GATT,"[GATTC] _bluetooth_gattc_scan_filter_param_setup_cli");
    bluetooth_gatt_filt_param_setup_t scan_filt_param;
    memset(&scan_filt_param, 0, sizeof(bluetooth_gatt_filt_param_setup_t));
    if (argc < 2)
    {
        BT_DBG_ERROR(BT_DEBUG_GATT,"Usage :");
        BT_DBG_ERROR(BT_DEBUG_GATT," please input gattc_can_filter_param_setup client_if action [filt_index int] [feat_seln int] [list_logic_type hex_string] [filt_logic_type int] [rssi_high_thres int] [rssi_low_thres int] [dely_mode int] [found_timeout int] [lost_timeout int] [found_timeout_cnt int] [num_of_tracking_entries int]");
    }

    scan_filt_param.client_if = atoi(argv[1]);
    scan_filt_param.action = atoi(argv[2]);
    INT32 count = 3;
    while (count < argc)
    {
        if (strcmp(argv[count],"filt_index") == 0)
        {
            count++;
            scan_filt_param.filt_index = atoi(argv[count]);
            count++;
            BT_DBG_INFO(BT_DEBUG_GATT,"filt_index : %ld" ,(long)scan_filt_param.filt_index);
            continue;
        }
        else if (strcmp(argv[count],"feat_seln") == 0)
        {
            count++;
            scan_filt_param.feat_seln = atoi(argv[count]);
            count++;
            BT_DBG_INFO(BT_DEBUG_GATT,"feat_seln : %ld" ,(long)scan_filt_param.feat_seln);
            continue;
        }
        else if (strcmp(argv[count],"list_logic_type") == 0)
        {
            count++;
            scan_filt_param.list_logic_type = strtol(argv[count],NULL,16);
            count++;
            BT_DBG_INFO(BT_DEBUG_GATT,"list_logic_type : %ld" ,(long)scan_filt_param.list_logic_type);
            continue;
        }
        else if (strcmp(argv[count],"filt_logic_type") == 0)
        {
            count++;
            scan_filt_param.filt_logic_type = atoi(argv[count]);
            count++;
            BT_DBG_INFO(BT_DEBUG_GATT,"filt_logic_type : %ld" ,(long)scan_filt_param.filt_logic_type);
            continue;
        }
        else if (strcmp(argv[count],"rssi_high_thres") == 0)
        {
            count++;
            scan_filt_param.rssi_high_thres = atoi(argv[count]);
            count++;
            BT_DBG_INFO(BT_DEBUG_GATT,"rssi_high_thres : %ld" ,(long)scan_filt_param.rssi_high_thres);
            continue;
        }
        else if (strcmp(argv[count],"rssi_low_thres") == 0)
        {
            count++;
            scan_filt_param.rssi_low_thres = atoi(argv[count]);
            count++;
            BT_DBG_INFO(BT_DEBUG_GATT,"rssi_low_thres : %ld" ,(long)scan_filt_param.rssi_low_thres);
            continue;
        }
        else if (strcmp(argv[count],"dely_mode") == 0)
        {
            count++;
            scan_filt_param.dely_mode = atoi(argv[count]);
            count++;
            BT_DBG_INFO(BT_DEBUG_GATT,"dely_mode : %ld" ,(long)scan_filt_param.dely_mode);
            continue;
        }
        else if (strcmp(argv[count],"found_timeout") == 0)
        {
            count++;
            scan_filt_param.found_timeout = atoi(argv[count]);
            count++;
            BT_DBG_INFO(BT_DEBUG_GATT,"found_timeout : %ld" ,(long)scan_filt_param.found_timeout);
            continue;
        }
        else if (strcmp(argv[count],"lost_timeout") == 0)
        {
            count++;
            scan_filt_param.lost_timeout = atoi(argv[count]);
            count++;
            BT_DBG_INFO(BT_DEBUG_GATT,"lost_timeout : %ld" ,(long)scan_filt_param.lost_timeout);
            continue;
        }
        else if (strcmp(argv[count],"found_timeout_cnt") == 0)
        {
            count++;
            scan_filt_param.found_timeout_cnt = atoi(argv[count]);
            count++;
            BT_DBG_INFO(BT_DEBUG_GATT,"found_timeout_cnt : %ld" ,(long)scan_filt_param.found_timeout_cnt);
            continue;
        }
        else if (strcmp(argv[count],"num_of_tracking_entries") == 0)
        {
            count++;
            scan_filt_param.num_of_tracking_entries = atoi(argv[count]);
            count++;
            BT_DBG_INFO(BT_DEBUG_GATT,"num_of_tracking_entries : %ld" ,(long)scan_filt_param.num_of_tracking_entries);
            continue;
        }
        count+=2;
    }
    bluetooth_gattc_scan_filter_param_setup(scan_filt_param);
    return BT_SUCCESS;
}

INT32 _bluetooth_gattc_scan_filter_enable_cli (INT32 argc, const CHAR **argv)
{
    BT_DBG_NORMAL(BT_DEBUG_GATT,"[GATTC] _bluetooth_gattc_scan_filter_enable_cli");
    INT32 client_if;
    if (argc < 1)
    {
        BT_DBG_ERROR(BT_DEBUG_GATT,"Usage :");
        BT_DBG_ERROR(BT_DEBUG_GATT,"please input gattc_scan_filter_enable client_if");
    }
    client_if = atoi(argv[1]);
    bluetooth_gattc_scan_filter_enable(client_if);
    return BT_SUCCESS;
}

INT32 _bluetooth_gattc_scan_filter_disable_cli (INT32 argc, const CHAR **argv)
{
    BT_DBG_NORMAL(BT_DEBUG_GATT,"[GATTC] _bluetooth_gattc_scan_filter_disable_cli");
    INT32 client_if;
    if (argc < 1)
    {
        BT_DBG_ERROR(BT_DEBUG_GATT,"Usage :");
        BT_DBG_ERROR(BT_DEBUG_GATT,"please input gattc_scan_filter_disable client_if");
    }

    client_if = atoi(argv[1]);
    bluetooth_gattc_scan_filter_disable(client_if);
    return BT_SUCCESS;
}

INT32 _bluetooth_gattc_scan_filter_add_remove_cli (INT32 argc, const CHAR **argv)
{
    BT_DBG_NORMAL(BT_DEBUG_GATT,"[GATTC] _bluetooth_gattc_scan_filter_add_remove_cli");
    INT32 client_if,action;
    INT32 filt_type = 0;
    INT32 filt_index = 0;
    INT32 company_id = 0;
    INT32 company_id_mask = 0;
    //bt_uuid_t p_uuid,p_uuid_mask;
    //bt_bdaddr_t bd_addr;
    CHAR pt_uuid[130] = {0};
    CHAR *ptr_uuid = NULL;
    CHAR pt_uuid_mask[130] = {0};
    CHAR *ptr_uuid_mask = NULL;
    CHAR pt_addr[130] = {0};
    CHAR *ptr_addr = NULL;
    INT32 addr_type=0;
    INT32 data_len=0;
    char* p_data=NULL;
    INT32 mask_len=0;
    char* p_mask=NULL;

    if (argc < 2)
    {
        BT_DBG_ERROR(BT_DEBUG_GATT,"Usage :");
        BT_DBG_ERROR(BT_DEBUG_GATT,"please input gattc_scan_filter_add_remove client_if action [filt_index int] [filt_type int] [company_id hex_string] [company_id_mask hex_string] [uuid hex_string] [uuid_mask hex_string] [bd_addr string] [addr_type int] [data hex_string/string] [data_mask hex_string]");
    }
    client_if = atoi(argv[1]);
    action = atoi(argv[2]);
    int count = 3;
    while (count < argc)
    {
        if (strcmp(argv[count],"filt_index") == 0)
        {
            count++;
            filt_index = atoi(argv[count]);
            count++;
            BT_DBG_INFO(BT_DEBUG_GATT,"filt_index : %ld" ,(long)filt_index);
            continue;
        }
        else if (strcmp(argv[count],"filt_type") == 0)
        {
            count++;
            filt_type = atoi(argv[count]);
            count++;
            BT_DBG_INFO(BT_DEBUG_GATT,"filt_type : %ld" ,(long)filt_type);
            continue;
        }
        else if (strcmp(argv[count],"company_id") == 0)
        {
            count++;
            company_id = strtol(argv[count],NULL,16);
            count++;
            BT_DBG_INFO(BT_DEBUG_GATT,"company_id : %ld" ,(long)company_id);
            continue;
        }
        else if (strcmp(argv[count],"company_id_mask") == 0)
        {
            count++;
            company_id_mask = strtol(argv[count],NULL,16);
            count++;
            BT_DBG_INFO(BT_DEBUG_GATT,"company_id_mask : %ld" ,(long)company_id_mask);
            continue;
        }
        else if (strcmp(argv[count],"uuid") == 0)
        {
            count++;
            strncpy(pt_uuid,argv[count], strlen(argv[count]));
            pt_uuid[strlen(argv[count])-1] = '\0';
            ptr_uuid = pt_uuid;
            //char *ptr = argv[count];
            //bt_gatt_uuid_stoh(pt_uuid, &(p_uuid));
            count++;
            BT_DBG_INFO(BT_DEBUG_GATT,"uuid : %s" ,pt_uuid);
            continue;
        }
        else if (strcmp(argv[count],"uuid_mask") == 0)
        {
            count++;
            //char *ptr = argv[count];
            strncpy(pt_uuid_mask,argv[count], strlen(argv[count]));
            pt_uuid_mask[strlen(argv[count])-1] = '\0';
            ptr_uuid_mask = pt_uuid_mask;
            //bt_gatt_uuid_stoh(pt_uuid, &(p_uuid_mask));
            count++;
            BT_DBG_INFO(BT_DEBUG_GATT,"uuid_mask : %s" ,pt_uuid);
            continue;
        }
        else if (strcmp(argv[count],"bd_addr") == 0)
        {
            count++;
            //char *ptr = argv[count];
            strncpy(pt_addr,argv[count], strlen(argv[count]));
            pt_addr[strlen(argv[count])-1] = '\0';
            ptr_addr = pt_addr;
            //bt_gatt_btaddr_stoh(pt_addr, &bd_addr);
            count++;
            BT_DBG_INFO(BT_DEBUG_GATT,"bd_addr %s,", pt_addr);
            continue;
        }
        else if (strcmp(argv[count],"addr_type") == 0)
        {
            count++;
            addr_type = atoi(argv[count]);
            count++;
            BT_DBG_INFO(BT_DEBUG_GATT,"addr_type : %ld" ,(long)addr_type);
            continue;
        }
        else if (strcmp(argv[count],"data") == 0)
        {
            count++;
            switch (filt_type)
            {
            case 0: // BTM_BLE_PF_ADDR_FILTER
            case 2: // BTM_BLE_PF_SRVC_UUID
            case 3: // BTM_BLE_PF_SRVC_SOL_UUID
            {
                count++;
                BT_DBG_INFO(BT_DEBUG_GATT,"data : %d" ,0);
                continue;
                break;
            }
            case 1: // BTM_BLE_PF_SRVC_DATA
            case 5: // BTM_BLE_PF_MANU_DATA
            case 6: // BTM_BLE_PF_SRVC_DATA_PATTERN
            {
                short hex_len = (strlen(argv[count]) + 1) / 2;
                char *hex_buf = malloc(hex_len * sizeof(char));
                char p_argv[256] = {0};
                strncpy(p_argv, argv[count], strlen(argv[count]));
                ascii_2_hex(p_argv, hex_len, (unsigned char *)hex_buf);
                p_data = hex_buf;
                data_len = hex_len;
                count++;
                BT_DBG_INFO(BT_DEBUG_GATT,"data : %ld" ,(long)data_len);
                continue;
                break;
            }
            case 4: // BTM_BLE_PF_LOCAL_NAME
            {
                // p_data = argv[count];
                data_len = strlen(argv[count]);
                p_data = malloc((data_len + 1) * sizeof(char));
                memcpy(p_data,argv[count],data_len + 1);
                count++;
                BT_DBG_INFO(BT_DEBUG_GATT,"data : %ld" ,(long)data_len);
                break;
            }
            default:
                count++;
                break;
            }
            continue;
        }
        else if (strcmp(argv[count],"data_mask") == 0)
        {
            count++;
            switch (filt_type)
            {
            case 0: // BTM_BLE_PF_ADDR_FILTER
            case 2: // BTM_BLE_PF_SRVC_UUID
            case 3: // BTM_BLE_PF_SRVC_SOL_UUID
            {
                count++;
                BT_DBG_INFO(BT_DEBUG_GATT,"data_mask : %d" ,0);
                continue;
                break;
            }
            case 1: // BTM_BLE_PF_SRVC_DATA
            case 5: // BTM_BLE_PF_MANU_DATA
            case 6: // BTM_BLE_PF_SRVC_DATA_PATTERN
            {
                short hex_len = (strlen(argv[count]) + 1) / 2;
                char *hex_buf = malloc(hex_len * sizeof(char));
                char p_argv[256] = {0};
                strncpy(p_argv, argv[count], strlen(argv[count]));
                ascii_2_hex(p_argv, hex_len, (unsigned char *)hex_buf);
                p_mask = hex_buf;
                mask_len = hex_len;
                count++;
                BT_DBG_INFO(BT_DEBUG_GATT,"data_mask : %ld" ,(long)mask_len);
                continue;
                break;
            }
            case 4: // BTM_BLE_PF_LOCAL_NAME
            {
                //p_mask = argv[count];
#if 0
                mask_len = strlen(argv[count]);
                p_mask = malloc((mask_len + 1) * sizeof(char));
                memcpy(p_mask,argv[count],mask_len + 1);
                count++;
#endif
                short hex_len = (strlen(argv[count]) + 1) / 2;
                char *hex_buf = malloc(hex_len * sizeof(char));
                char p_argv[256] = {0};
                strncpy(p_argv, argv[count], strlen(argv[count]));
                ascii_2_hex(p_argv, hex_len, (unsigned char *)hex_buf);
                p_mask = hex_buf;
                mask_len = hex_len;
                count++;
                BT_DBG_INFO(BT_DEBUG_GATT,"data_mask : %ld" ,(long)mask_len);
                break;
            }
            default:
                count++;
                break;
            }
            continue;
        }
        count+=2;
    }
    bluetooth_gattc_scan_filter_add_remove(client_if,action,filt_type,
                                           filt_index,company_id,company_id_mask,ptr_uuid,
                                           ptr_uuid_mask,ptr_addr,addr_type,data_len,p_data,mask_len,p_mask);
    return BT_SUCCESS;
}

INT32 _bluetooth_gattc_scan_filter_clear_cli (INT32 argc, const CHAR **argv)
{
    BT_DBG_NORMAL(BT_DEBUG_GATT,"[GATTC] _bluetooth_gattc_scan_filter_add_remove_cli");
    INT32 client_if,filt_index;
    if (argc < 2)
    {
        BT_DBG_ERROR(BT_DEBUG_GATT,"Usage :");
        BT_DBG_ERROR(BT_DEBUG_GATT,"  please input gattc_scan_filter_clear client_if filt_index");
        return -1;
    }
    client_if = atoi(argv[1]);
    filt_index = atoi(argv[2]);
    bluetooth_gattc_scan_filter_clear(client_if,filt_index);
    return BT_SUCCESS;
}
INT32 _bluetooth_gattc_set_local_le_name_cli (INT32 argc, const CHAR **argv)
{
    BT_DBG_NORMAL(BT_DEBUG_GATT,"[GATTC] _bluetooth_gattc_set_local_le_name_cli");
    INT32 client_if = 0;
    CHAR p_le_name[32];
    if (argc < 3)
    {
        BT_DBG_ERROR(BT_DEBUG_GATT,"Usage :");
        BT_DBG_ERROR(BT_DEBUG_GATT,"  please input gattc_set_le_name client_if le_name");
        return -1;
    }

    if ((strlen(argv[1]) + 1) > 32)
    {
        BT_DBG_ERROR(BT_DEBUG_GATT,"max le name len is 31 bytes");
        return -1;
    }
    client_if = atoi(argv[1]);
    strncpy(p_le_name, argv[2], strlen(argv[2]));
    p_le_name[strlen(argv[2])] = '\0';
#if MTK_LINUX_GATTC_LE_NAME //== TRUE
    bluetooth_gattc_set_local_le_name(client_if,p_le_name);
#endif
    return BT_SUCCESS;
}

INT32 _bluetooth_gattc_configure_mtu_cli (INT32 argc, const CHAR **argv)
{
    BT_DBG_NORMAL(BT_DEBUG_GATT,"[GATTC] _bluetooth_gattc_configure_mtu_cli");
    INT32 conn_id = 0;
    INT32 mtu = 0;
    if (argc < 3)
    {
        BT_DBG_ERROR(BT_DEBUG_GATT,"Usage :");
        BT_DBG_ERROR(BT_DEBUG_GATT,"  please input gattc_config_mtu conn_id mtu");
        return -1;
    }
    conn_id = atoi(argv[1]);
    mtu = atoi(argv[2]);
    bluetooth_gattc_configure_mtu(conn_id,mtu);
    return BT_SUCCESS;
}

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

INT32 _bluetooth_gatts_register_server_cli (INT32 argc, const CHAR **argv)
{
    //bt_uuid_t pt_service_uuid;
    CHAR pt_service_uuid[130];

    if (2 == argc)
    {
        strncpy(pt_service_uuid,argv[1], strlen(argv[1]));
        pt_service_uuid[strlen(argv[1])] = '\0';
        //bt_gatt_uuid_stoh(pt_uuid,&pt_service_uuid);
        return bluetooth_gatts_register_server(pt_service_uuid);
    }
    else
    {
        BT_DBG_ERROR(BT_DEBUG_GATT,"input error");
        BT_DBG_ERROR(BT_DEBUG_GATT,"please input gatts_register_server [UUID]");
        return BT_ERR_STATUS_INVALID_PARM_NUMS;
    }
}

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
INT32 _bluetooth_gatts_unregister_server_cli (INT32 argc, const CHAR **argv)
{
    INT32 server_if = 0;
    if (2 == argc)
    {
        server_if = atoi(argv[1]);
        return bluetooth_gatts_unregister_server(server_if);
    }
    else
    {
        BT_DBG_ERROR(BT_DEBUG_GATT,"input error");
        BT_DBG_ERROR(BT_DEBUG_GATT,"please input gatts_unregister_server server_if");
        return BT_ERR_STATUS_INVALID_PARM_NUMS;
    }
}

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
INT32 _bluetooth_gatts_open_cli (INT32 argc, const CHAR **argv)
{
    CHAR ps_addr[MAX_BDADDR_LEN];
    UINT8 is_direct = 0;
    INT32 transport = 0;
    INT32 server_if = 0;
    if (argc < 3 )
    {
        BT_DBG_ERROR(BT_DEBUG_GATT,"please input gatts_cc server_if BD_ADDR [is_direct] [transport]");
        return BT_ERR_STATUS_INVALID_PARM_NUMS;
    }
    server_if = atoi(argv[1]);
    strncpy(ps_addr,argv[2], MAX_BDADDR_LEN);
    ps_addr[MAX_BDADDR_LEN - 1] = '\0';

    if (argc == 3 )
    {
        BT_DBG_INFO(BT_DEBUG_GATT,"gatts_cc %ld %s", (long)server_if, ps_addr);
    }
    if (argc >= 4 )
    {
        is_direct = atoi(argv[3]);
        BT_DBG_INFO(BT_DEBUG_GATT,"gatts_cc %ld %s %d", (long)server_if, ps_addr, is_direct);
    }
    if (argc >= 5 )
    {
        transport = atoi(argv[4]);
        BT_DBG_INFO(BT_DEBUG_GATT,"gatts_cc %ld %s %d %ld", (long)server_if, ps_addr, is_direct,(long)transport);
    }

    return bluetooth_gatts_open(server_if, ps_addr,is_direct,transport);
}

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
INT32 _bluetooth_gatts_close_cli (INT32 argc, const CHAR **argv)
{
    CHAR ps_addr[MAX_BDADDR_LEN];
    INT32 server_if = 0;
    INT32 conn_id = 0;

    if (argc != 4 )
    {
        BT_DBG_ERROR(BT_DEBUG_GATT,"gatts_dc server_if BD_ADDR conn_id");
        return BT_ERR_STATUS_INVALID_PARM_NUMS;
    }

    server_if = atoi(argv[1]);
    strncpy(ps_addr,argv[2], MAX_BDADDR_LEN);
    ps_addr[MAX_BDADDR_LEN - 1] = '\0';
    conn_id = atoi(argv[3]);
    BT_DBG_INFO(BT_DEBUG_GATT,"gatts_dc %ld %s %ld", (long)server_if, ps_addr, (long)conn_id);

    return bluetooth_gatts_close(server_if,ps_addr,conn_id);
}

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
INT32 _bluetooth_gatts_add_service_cli (INT32 argc, const CHAR **argv)
{
    CHAR pt_uuid[130];
    INT32 number = 0;
    UINT8 is_primary = 0;
    //bt_uuid_t bt_uuid;
    INT32 server_if = 0;
    if (argc < 3 )
    {
        BT_DBG_ERROR(BT_DEBUG_GATT,"please input gatts_add_service server_if uuid [is_primary:true|false] [number_of_handles]");
        return BT_ERR_STATUS_INVALID_PARM_NUMS;
    }

    server_if = atoi(argv[1]);
    strncpy(pt_uuid, argv[2], strlen(argv[2]));
    pt_uuid[strlen(argv[2])] = '\0';
    //bt_gatt_uuid_stoh(pt_uuid,&bt_uuid);
    BT_DBG_INFO(BT_DEBUG_GATT,"server_if %ld uuid %s", (long)server_if, pt_uuid);
    if (argc >= 4)
    {
        is_primary = atoi(argv[3]);
        BT_DBG_INFO(BT_DEBUG_GATT,"is_primary %d", is_primary);
    }
    if (argc >= 5)
    {
        number = atoi(argv[4]);
        BT_DBG_INFO(BT_DEBUG_GATT,"number %ld", (long)number);
    }

    return bluetooth_gatts_add_service(server_if,
                                       pt_uuid,
                                       is_primary,
                                       number);
}

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
INT32 _bluetooth_gatts_add_incl_service_cli (INT32 argc, const CHAR **argv)
{
    INT32 service_handle;
    INT32 included_handle;
    INT32 server_if = 0;

    if (argc == 4 )
    {
        server_if = atoi(argv[1]);
        service_handle = atoi(argv[2]);
        included_handle = atoi(argv[3]);
        BT_DBG_INFO(BT_DEBUG_GATT,"service_handle %ld", (long)service_handle);
        BT_DBG_INFO(BT_DEBUG_GATT,"included_handle %ld", (long)included_handle);
    }
    else
    {
        BT_DBG_ERROR(BT_DEBUG_GATT,"please input gatts_add_incl_service server_if service_handle included_handle");
        return BT_ERR_STATUS_INVALID_PARM_NUMS;
    }

    return bluetooth_gatts_add_included_service(server_if, service_handle, included_handle);
}

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
INT32 _bluetooth_gatts_add_char_cli (INT32 argc, const CHAR **argv)
{
    CHAR pt_uuid[130];
    INT32 service_handle = 0;
    INT32 properties = 6;
    INT32 permissions = 17;
    //bt_uuid_t bt_uuid;
    INT32 server_if = 0;
    if (argc < 4)
    {
        BT_DBG_ERROR(BT_DEBUG_GATT,"please input gatts_add_char server_if service_handle uuid [properties] [permissions]");
        return BT_ERR_STATUS_INVALID_PARM_NUMS;
    }

    server_if = atoi(argv[1]);
    BT_DBG_INFO(BT_DEBUG_GATT,"server_if %ld", (long)server_if);
    service_handle = atoi(argv[2]);
    BT_DBG_INFO(BT_DEBUG_GATT,"service_handle %ld", (long)service_handle);
    strncpy(pt_uuid,argv[3], strlen(argv[3]));
    pt_uuid[strlen(argv[3])] = '\0';
    //bt_gatt_uuid_stoh(pt_uuid,&bt_uuid);
    BT_DBG_INFO(BT_DEBUG_GATT,"uuid %s", pt_uuid);

    if (argc >= 5 )
    {
        properties = atoi(argv[4]);
        BT_DBG_INFO(BT_DEBUG_GATT,"properties %ld", (long)properties);
    }
    if (argc >= 6 )
    {
        permissions = atoi(argv[5]);
        BT_DBG_INFO(BT_DEBUG_GATT,"permissions %ld", (long)permissions);
    }

    return bluetooth_gatts_add_char(server_if,
                                    service_handle,
                                    pt_uuid,
                                    properties,
                                    permissions);
}

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
INT32 _bluetooth_gatts_add_desc_cli (INT32 argc, const CHAR **argv)
{
    CHAR pt_uuid[130];
    INT32 service_handle = 0;
    INT32 permissions = 0;
    //bt_uuid_t bt_uuid;
    INT32 server_if = 0;
    if (argc < 4)
    {
        BT_DBG_ERROR(BT_DEBUG_GATT,"please input gatts_add_desc server_if service_handle uuid [permissions]");
        return BT_ERR_STATUS_INVALID_PARM_NUMS;
    }
    server_if = atoi(argv[1]);
    BT_DBG_INFO(BT_DEBUG_GATT,"server_if %ld", (long)server_if);
    service_handle = atoi(argv[2]);
    BT_DBG_INFO(BT_DEBUG_GATT,"service_handle %ld", (long)service_handle);
    strncpy(pt_uuid,argv[3], strlen(argv[3]));
    pt_uuid[strlen(argv[3])] = '\0';
    //bt_gatt_uuid_stoh(pt_uuid,&bt_uuid);
    BT_DBG_INFO(BT_DEBUG_GATT,"uuid %s", pt_uuid);
    if (argc >= 5 )
    {
        permissions = atoi(argv[4]);
        BT_DBG_INFO(BT_DEBUG_GATT,"permissions %ld", (long)permissions);
    }

    return bluetooth_gatts_add_desc(server_if,
                                    service_handle,
                                    pt_uuid,
                                    permissions);
}

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
INT32 _bluetooth_gatts_start_service_cli (INT32 argc, const CHAR **argv)
{
    INT32 service_handle = 0;
    INT32 transport = 0;
    INT32 server_if = 0;
    if (argc < 3)
    {
        BT_DBG_ERROR(BT_DEBUG_GATT,"please input gatts_start_service server_if service_handle [transport]");
        return BT_ERR_STATUS_INVALID_PARM_NUMS;
    }
    server_if = atoi(argv[1]);
    BT_DBG_INFO(BT_DEBUG_GATT,"server_if %ld", (long)server_if);
    service_handle = atoi(argv[2]);
    BT_DBG_INFO(BT_DEBUG_GATT,"service_handle %ld", (long)service_handle);
    if (argc >= 4 )
    {
        transport = atoi(argv[3]);
        BT_DBG_INFO(BT_DEBUG_GATT,"transport %ld", (long)transport);
    }

    return bluetooth_gatts_start_service(server_if, service_handle, transport);
}

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
INT32 _bluetooth_gatts_stop_service_cli (INT32 argc, const CHAR **argv)
{
    INT32 service_handle = 0;
    INT32 server_if = 0;

    if (argc == 3 )
    {
        server_if = atoi(argv[1]);
        BT_DBG_INFO(BT_DEBUG_GATT,"server_if %ld", (long)server_if);
        service_handle = atoi(argv[2]);
        BT_DBG_INFO(BT_DEBUG_GATT,"service_handle %ld", (long)service_handle);
    }
    else
    {
        BT_DBG_ERROR(BT_DEBUG_GATT,"please input gatts_stop_service server_if service_handle");
        return BT_ERR_STATUS_INVALID_PARM_NUMS;
    }


    return bluetooth_gatts_stop_service(server_if, service_handle);
}

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
INT32 _bluetooth_gatts_delete_service_cli (INT32 argc, const CHAR **argv)
{
    INT32 service_handle = 0;
    INT32 server_if = 0;

    if (argc == 3 )
    {
        server_if = atoi(argv[1]);
        BT_DBG_INFO(BT_DEBUG_GATT,"server_if %ld", (long)server_if);
        service_handle = atoi(argv[2]);
        BT_DBG_INFO(BT_DEBUG_GATT,"service_handle %ld", (long)service_handle);
    }
    else
    {
        BT_DBG_ERROR(BT_DEBUG_GATT,"please input gatts_delete_service server_if service_handle");
        return BT_ERR_STATUS_INVALID_PARM_NUMS;
    }

    return bluetooth_gatts_delete_service(server_if, service_handle);
}

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
INT32 _bluetooth_gatts_send_indication_cli (INT32 argc, const CHAR **argv)
{
    CHAR p_value[260];
    INT32 server_if = 0;
    INT32 attribute_handle = 0;
    INT32 conn_id = 0;
    INT32 fg_confirm = 0;
    INT32 value_len = 0;

    if (argc < 4)
    {
        BT_DBG_ERROR(BT_DEBUG_GATT,"please input gatts_send_indi server_if attribute_handle conn_id [confirm] value");
        return BT_ERR_STATUS_INVALID_PARM_NUMS;
    }

    server_if = atoi(argv[1]);
    BT_DBG_INFO(BT_DEBUG_GATT,"server_if %ld", (long)server_if);

    attribute_handle = atoi(argv[2]);
    BT_DBG_INFO(BT_DEBUG_GATT,"attribute_handle %ld", (long)attribute_handle);
    conn_id = atoi(argv[3]);
    BT_DBG_INFO(BT_DEBUG_GATT,"conn_id %ld", (long)conn_id);

    if (argc >= 5 )
    {

        strncpy(p_value,argv[4], strlen(argv[4]));
        p_value[strlen(argv[4])] = '\0';
        BT_DBG_INFO(BT_DEBUG_GATT,"p_value %s", p_value);
        value_len = strlen(p_value);
        BT_DBG_INFO(BT_DEBUG_GATT,"value_len %ld", (long)value_len);
    }
    if (argc >= 6 )
    {
        fg_confirm = atoi(argv[4]);
        BT_DBG_INFO(BT_DEBUG_GATT,"fg_confirm %ld", (long)fg_confirm);
        strncpy(p_value,argv[5], strlen(argv[5]));
        p_value[strlen(argv[5])] = '\0';
        BT_DBG_INFO(BT_DEBUG_GATT,"p_value %s", p_value);
        value_len = strlen(p_value);
        BT_DBG_INFO(BT_DEBUG_GATT,"value_len %ld", (long)value_len);
    }

    return bluetooth_gatts_send_indication(server_if,
                                           attribute_handle,
                                           conn_id,
                                           fg_confirm,
                                           p_value,
                                           value_len);
}

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
INT32 _bluetooth_gatts_send_response_cli (INT32 argc, const CHAR **argv)
{
    CHAR p_value[260];
    INT32 conn_id = 0;
    INT32 trans_id = 0;
    INT32 status = 0;
    INT32 handle = 0;
    INT32 value_len = 0;
    INT32 auth_req = 0;

    if (argc < 6)
    {
        BT_DBG_ERROR(BT_DEBUG_GATT,"please input gatts_send_rsp conn_id trans_id status handle [auth_req] value");
        return BT_ERR_STATUS_INVALID_PARM_NUMS;
    }
    conn_id = atoi(argv[1]);
    BT_DBG_INFO(BT_DEBUG_GATT,"conn_id %ld", (long)conn_id);
    trans_id = atoi(argv[2]);
    BT_DBG_INFO(BT_DEBUG_GATT,"trans_id %ld", (long)trans_id);
    status = atoi(argv[3]);
    BT_DBG_INFO(BT_DEBUG_GATT,"status %ld", (long)status);
    handle = atoi(argv[4]);
    BT_DBG_INFO(BT_DEBUG_GATT,"handle %ld", (long)handle);
    if (argc == 6 )
    {

        strncpy(p_value,argv[5], strlen(argv[5]));
        p_value[strlen(argv[5])] = '\0';
        BT_DBG_INFO(BT_DEBUG_GATT,"p_value %s", p_value);
        value_len = strlen(p_value);
        BT_DBG_INFO(BT_DEBUG_GATT,"value_len %ld", (long)value_len);
    }
    else if (argc == 7 )
    {
        auth_req = atoi(argv[5]);
        BT_DBG_INFO(BT_DEBUG_GATT,"auth_req %ld", (long)auth_req);
        strncpy(p_value,argv[6], strlen(argv[6]));
        p_value[strlen(argv[6])] = '\0';
        BT_DBG_INFO(BT_DEBUG_GATT,"p_value %s", p_value);
        value_len = strlen(p_value);
        BT_DBG_INFO(BT_DEBUG_GATT,"value_len %ld", (long)value_len);
    }

    return bluetooth_gatts_send_response(conn_id,
                                         trans_id,
                                         status,
                                         handle,
                                         p_value,
                                         value_len,
                                         auth_req);
}
#endif


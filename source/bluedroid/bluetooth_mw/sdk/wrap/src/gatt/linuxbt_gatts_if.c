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

/* FILE NAME:  linuxbt_gatts_if.c
 * AUTHOR: Xuemei Yang
 * PURPOSE:
 *      It provides GATTS operation interface and callback function implementation to MW higher layer.
 * NOTES:
 */


/*-----------------------------------------------------------------------------
                    include files
 ----------------------------------------------------------------------------*/


#include <string.h>
#include <stdlib.h>
#include "bluetooth.h"
//#include "bt_gatt_types.h"
#include "bt_mw_common.h"
#include "bt_mw_gatt.h"
#include "linuxbt_gatts_if.h"
#include "linuxbt_gatt_if.h"
#include "u_bt_mw_gatt.h"
#include "linuxbt_common.h"

static int linuxbt_server_if = -1;
const btgatt_server_interface_t *linuxbt_gatts_interface = NULL;

/**
 * FUNCTION NAME: linuxbt_gatts_register_server
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
int linuxbt_gatts_register_server(char *uuid)
{
    //FUNC_ENTRY;
    bt_status_t ret= BT_STATUS_SUCCESS;
    if (NULL == linuxbt_gatts_interface)
    {
        BT_DBG_ERROR(BT_DEBUG_GATT,"[GATTS] linuxbt_gatts_interface not init.");
        return BT_ERR_STATUS_PARM_INVALID;
    }
    if (NULL == linuxbt_gatts_interface->register_server)
    {
        BT_DBG_ERROR(BT_DEBUG_GATT,"[GATTS] null pointer of register_server");
        return BT_ERR_STATUS_PARM_INVALID;
    }
    bt_uuid_t app_uuid;
    linuxbt_uuid_stoh(uuid, &app_uuid);
    ret = linuxbt_gatts_interface->register_server(&app_uuid);
    BT_DBG_MINOR(BT_DEBUG_GATT,"ret = %d", ret);
    return BT_STATUS_SUCCESS;//linuxbt_return_value_convert(ret);
}

/**
 * FUNCTION NAME: linuxbt_gatts_unregister_server
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
int linuxbt_gatts_unregister_server(int server_if)
{
    //FUNC_ENTRY;
    bt_status_t ret= BT_STATUS_SUCCESS;
    if (NULL == linuxbt_gatts_interface)
    {
        BT_DBG_ERROR(BT_DEBUG_GATT,"[GATTS] linuxbt_gatts_interface not init.");
        return BT_ERR_STATUS_PARM_INVALID;
    }
    ret = linuxbt_gatts_interface->unregister_server(server_if);
    BT_DBG_MINOR(BT_DEBUG_GATT,"ret = %d", ret);
    return BT_STATUS_SUCCESS;//llinuxbt_return_value_convert(ret);
}

/**
 * FUNCTION NAME: linuxbt_gatts_open
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
int linuxbt_gatts_open(int server_if, char *bt_addr, uint8_t is_direct, int transport)
{
    //FUNC_ENTRY;
    bt_status_t ret= BT_STATUS_SUCCESS;
    if (NULL == linuxbt_gatts_interface)
    {
        BT_DBG_ERROR(BT_DEBUG_GATT,"[GATTS] linuxbt_gatts_interface not init.");
        return BT_ERR_STATUS_PARM_INVALID;
    }
    if (NULL == bt_addr)
    {
        BT_DBG_ERROR(BT_DEBUG_GATT,"[GATTS] null pointer");
        return BT_ERR_STATUS_PARM_INVALID;
    }
    bt_bdaddr_t bdaddr;

    /*GATTS open BD_ADDR [isDirect:true|false] [transport]*/


    linuxbt_btaddr_stoh(bt_addr, &bdaddr);
    BT_DBG_INFO(BT_DEBUG_GATT,"GATTS connect to %02X:%02X:%02X:%02X:%02X:%02X ,sever_if:%d",
                bdaddr.address[0], bdaddr.address[1], bdaddr.address[2],
                bdaddr.address[3], bdaddr.address[4], bdaddr.address[5], server_if);

    ret = linuxbt_gatts_interface->connect(server_if,&bdaddr,is_direct,transport);
    BT_DBG_MINOR(BT_DEBUG_GATT,"ret = %d", ret);
    return BT_STATUS_SUCCESS;//llinuxbt_return_value_convert(ret);

}

/**
 * FUNCTION NAME: linuxbt_gatts_close
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
int linuxbt_gatts_close(int server_if, char *bt_addr, int conn_id)
{
    //FUNC_ENTRY;
    bt_status_t ret= BT_STATUS_SUCCESS;
    if (NULL == linuxbt_gatts_interface)
    {
        BT_DBG_ERROR(BT_DEBUG_GATT,"[GATTS] linuxbt_gatts_interface not init.");
        return BT_ERR_STATUS_PARM_INVALID;
    }
    if (NULL == bt_addr)
    {
        BT_DBG_ERROR(BT_DEBUG_GATT,"[GATTS] null pointer");
        return BT_ERR_STATUS_PARM_INVALID;
    }
    bt_bdaddr_t bdaddr;

    linuxbt_btaddr_stoh(bt_addr, &bdaddr);
    BT_DBG_INFO(BT_DEBUG_GATT,"GATTS disconnect to %02X:%02X:%02X:%02X:%02X:%02X ,server_if:%d",
                bdaddr.address[0], bdaddr.address[1], bdaddr.address[2],
                bdaddr.address[3], bdaddr.address[4], bdaddr.address[5], server_if);

    ret = linuxbt_gatts_interface->disconnect(server_if,&bdaddr,conn_id);
    BT_DBG_MINOR(BT_DEBUG_GATT,"ret = %d", ret);
    return BT_STATUS_SUCCESS;//llinuxbt_return_value_convert(ret);
}

/**
 * FUNCTION NAME: linuxbt_gatts_add_service
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
int linuxbt_gatts_add_service(int server_if,
                              char *pt_service_uuid,
                              uint8_t is_primary,
                              int number)
{
    //FUNC_ENTRY;
    bt_status_t ret= BT_STATUS_SUCCESS;
    if (NULL == linuxbt_gatts_interface)
    {
        BT_DBG_ERROR(BT_DEBUG_GATT,"[GATTS] linuxbt_gatts_interface not init.");
        return BT_ERR_STATUS_PARM_INVALID;
    }
    if (NULL == pt_service_uuid)
    {
        BT_DBG_ERROR(BT_DEBUG_GATT,"[GATTS] null pointer");
        return BT_ERR_STATUS_PARM_INVALID;
    }
    /* one service,one char and no desc*/
    /*GATTS add_service uuid [is_primary:true|false] [number_of_handles]*/
    btgatt_srvc_id_t srvc_id;

    bt_uuid_t svc_uuid;
    linuxbt_uuid_stoh(pt_service_uuid, &svc_uuid);
    memcpy(&srvc_id.id.uuid, &svc_uuid, sizeof(bt_uuid_t));
    srvc_id.is_primary = is_primary;

    ret = linuxbt_gatts_interface->add_service(server_if,&srvc_id,number);
    BT_DBG_MINOR(BT_DEBUG_GATT,"ret = %d", ret);
    return BT_STATUS_SUCCESS;//llinuxbt_return_value_convert(ret);
}

/**
 * FUNCTION NAME: linuxbt_gatts_add_char
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
int linuxbt_gatts_add_char(int server_if,
                           int service_handle,
                           char *uuid,
                           int properties,
                           int permissions)
{
    //FUNC_ENTRY;
    bt_status_t ret= BT_STATUS_SUCCESS;
    if (NULL == linuxbt_gatts_interface)
    {
        BT_DBG_ERROR(BT_DEBUG_GATT,"[GATTS] linuxbt_gatts_interface not init.");
        return BT_ERR_STATUS_PARM_INVALID;
    }

    bt_uuid_t char_uuid;
    linuxbt_uuid_stoh(uuid, &char_uuid);
    /*GATTS add_char service_handle uuid [properties] [permissions]*/
    ret = linuxbt_gatts_interface->add_characteristic(server_if,
            service_handle,
            &char_uuid,
            properties,
            permissions);
    BT_DBG_MINOR(BT_DEBUG_GATT,"ret = %d", ret);
    return BT_STATUS_SUCCESS;//llinuxbt_return_value_convert(ret);
}

/**
 * FUNCTION NAME: linuxbt_gatts_add_desc
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
int linuxbt_gatts_add_desc(int server_if,
                           int service_handle,
                           char *uuid,
                           int permissions)
{
    //FUNC_ENTRY;
    bt_status_t ret= BT_STATUS_SUCCESS;
    if (NULL == linuxbt_gatts_interface)
    {
        BT_DBG_ERROR(BT_DEBUG_GATT,"[GATTS] linuxbt_gatts_interface not init.");
        return BT_ERR_STATUS_PARM_INVALID;
    }
    /*GATTS add_desc service_handle uuid [permissions]*/

    bt_uuid_t desc_uuid;
    linuxbt_uuid_stoh(uuid, &desc_uuid);
    ret = linuxbt_gatts_interface->add_descriptor(server_if,service_handle,&desc_uuid,permissions);
    BT_DBG_MINOR(BT_DEBUG_GATT,"ret = %d", ret);
    return BT_STATUS_SUCCESS;//llinuxbt_return_value_convert(ret);
}

/**
 * FUNCTION NAME: linuxbt_gatts_add_included_service
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
int linuxbt_gatts_add_included_service(int server_if,
                                       int service_handle,
                                       int included_handle)
{
    //FUNC_ENTRY;
    bt_status_t ret= BT_STATUS_SUCCESS;
    if (NULL == linuxbt_gatts_interface)
    {
        BT_DBG_ERROR(BT_DEBUG_GATT,"[GATTS] linuxbt_gatts_interface not init.");
        return BT_ERR_STATUS_PARM_INVALID;
    }
    /*GATTS add_included_service service_handle included_handle*/

    ret = linuxbt_gatts_interface->add_included_service(server_if,service_handle,included_handle);
    BT_DBG_MINOR(BT_DEBUG_GATT,"ret = %d", ret);
    return BT_STATUS_SUCCESS;//llinuxbt_return_value_convert(ret);
}

/**
 * FUNCTION NAME: linuxbt_gatts_start_service
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
int linuxbt_gatts_start_service(int server_if, int service_handle, int transport)
{
    //FUNC_ENTRY;
    bt_status_t ret= BT_STATUS_SUCCESS;
    if (NULL == linuxbt_gatts_interface)
    {
        BT_DBG_ERROR(BT_DEBUG_GATT,"[GATTS] linuxbt_gatts_interface not init.");
        return BT_ERR_STATUS_PARM_INVALID;
    }

    ret = linuxbt_gatts_interface->start_service(server_if,service_handle,transport);
    BT_DBG_MINOR(BT_DEBUG_GATT,"ret = %d", ret);
    return BT_STATUS_SUCCESS;//llinuxbt_return_value_convert(ret);
}

/**
 * FUNCTION NAME: linuxbt_gatts_stop_service
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
int linuxbt_gatts_stop_service(int server_if, int service_handle)
{
    //FUNC_ENTRY;
    bt_status_t ret= BT_STATUS_SUCCESS;
    if (NULL == linuxbt_gatts_interface)
    {
        BT_DBG_ERROR(BT_DEBUG_GATT,"[GATTS] linuxbt_gatts_interface not init.");
        return BT_ERR_STATUS_PARM_INVALID;
    }

    ret = linuxbt_gatts_interface->stop_service(server_if, service_handle);
    BT_DBG_MINOR(BT_DEBUG_GATT,"ret = %d", ret);
    return BT_STATUS_SUCCESS;//llinuxbt_return_value_convert(ret);
}

/**
 * FUNCTION NAME: linuxbt_gatts_delete_service
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
int linuxbt_gatts_delete_service(int server_if, int service_handle)
{
    //FUNC_ENTRY;
    bt_status_t ret= BT_STATUS_SUCCESS;
    if (NULL == linuxbt_gatts_interface)
    {
        BT_DBG_ERROR(BT_DEBUG_GATT,"[GATTS] linuxbt_gatts_interface not init.");
        return BT_ERR_STATUS_PARM_INVALID;
    }
    ret = linuxbt_gatts_interface->delete_service(server_if,service_handle);
    BT_DBG_MINOR(BT_DEBUG_GATT,"ret = %d", ret);
    return BT_STATUS_SUCCESS;//llinuxbt_return_value_convert(ret);

}

/**
 * FUNCTION NAME: linuxbt_gatts_send_indication
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
int linuxbt_gatts_send_indication(int server_if,
                                  int attribute_handle,
                                  int conn_id,
                                  int fg_confirm,
                                  char* p_value,
                                  int value_len)
{
    //FUNC_ENTRY;
    bt_status_t ret= BT_STATUS_SUCCESS;
    if (NULL == linuxbt_gatts_interface)
    {
        BT_DBG_ERROR(BT_DEBUG_GATT,"[GATTS] linuxbt_gatts_interface not init.");
        return BT_ERR_STATUS_PARM_INVALID;
    }
    if (NULL == p_value)
    {
        BT_DBG_ERROR(BT_DEBUG_GATT,"[GATTS] null pointer");
        return BT_ERR_STATUS_PARM_INVALID;
    }
    /*GATTS send_indi attribute_handle conn_id [confirm] value*/


    ret = linuxbt_gatts_interface->send_indication(server_if,
            attribute_handle,
            conn_id,
            value_len,
            fg_confirm,
            p_value);
    BT_DBG_MINOR(BT_DEBUG_GATT,"ret = %d", ret);
    return BT_STATUS_SUCCESS;//llinuxbt_return_value_convert(ret);

}

/**
 * FUNCTION NAME: linuxbt_gatts_send_response
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
int linuxbt_gatts_send_response(int conn_id,
                                int trans_id,
                                int status,
                                int handle,
                                char *p_value,
                                int value_len,
                                int auth_req)
{
    //FUNC_ENTRY;
    bt_status_t ret= BT_STATUS_SUCCESS;
    if (NULL == linuxbt_gatts_interface)
    {
        BT_DBG_ERROR(BT_DEBUG_GATT,"[GATTS] linuxbt_gatts_interface not init.");
        return BT_ERR_STATUS_PARM_INVALID;
    }
    //if (NULL == p_value)
    //{
    //    BT_DBG_ERROR(BT_DEBUG_GATT,"[GATTS] null pointer");
    //    return BT_ERR_STATUS_PARM_INVALID;
    //}
    /*GATTS send_response conn_id trans_id status handle [auth_req] value*/

    btgatt_response_t response;
    if (value_len > 0)
    {
        memcpy(response.attr_value.value, p_value, value_len);
    }
    response.attr_value.handle = handle;
    response.attr_value.offset = 0;
    response.attr_value.len = value_len;
    response.attr_value.auth_req = auth_req;

    ret = linuxbt_gatts_interface->send_response(conn_id,trans_id,status,&response);
    BT_DBG_MINOR(BT_DEBUG_GATT,"ret = %d", ret);
    return BT_STATUS_SUCCESS;//llinuxbt_return_value_convert(ret);

}

/**
 * FUNCTION NAME: linuxbt_gatts_register_server_callback
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
static void linuxbt_gatts_register_server_callback(int status, int server_if, bt_uuid_t *app_uuid)
{
    //FUNC_ENTRY;
    BT_DBG_INFO(BT_DEBUG_GATT,"[GATTS] Register server callback server_if:%d, status:%d", server_if,status);
    if (0 == status)
    {
        linuxbt_server_if = server_if;
        //LE scan units(0.625ms)
        /*int scan_window = SCAN_MODE_BALANCED_WINDOW_MS*1000/625;
        int scan_interval = SCAN_MODE_BALANCED_INTERVAL_MS*1000/625;
        linuxbt_gattc_interface->set_scan_parameters(scan_interval,scan_window);*/
    }
    char uuid[BT_GATT_MAX_UUID_LEN] = {0};
    linuxbt_print_uuid(app_uuid, uuid);
    bluetooth_gatts_register_server_callback(status, server_if, uuid);

}

/**
 * FUNCTION NAME: linuxbt_gatts_connection_callback
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
static void linuxbt_gatts_connection_callback(int conn_id, int server_if, int connected, bt_bdaddr_t *bda)
{
    //FUNC_ENTRY;
    BT_DBG_INFO(BT_DEBUG_GATT, "%02X:%02X:%02X:%02X:%02X:%02X\n connected = %d, conn_id = %d",
                bda->address[0], bda->address[1], bda->address[2],
                bda->address[3], bda->address[4], bda->address[5],connected,conn_id);
    char btaddr_s[MAX_BDADDR_LEN];
    linuxbt_btaddr_htos(bda, btaddr_s);
    bluetooth_gatts_connection_callback(conn_id, server_if, connected, btaddr_s);

#if MTK_LINUX_GATTC_PTS_TEST
    if (connected == 1)
    {
        linuxbt_gatts_open(server_if, btaddr_s, 1, 2);
    }
#endif
}

/**
 * FUNCTION NAME: linuxbt_gatts_service_added_callback
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
static void linuxbt_gatts_service_added_callback(int status, int server_if, btgatt_srvc_id_t *srvc_id, int srvc_handle)
{
    bt_uuid_t uuid = srvc_id->id.uuid;
    char uuid_s[BT_GATT_MAX_UUID_LEN] = {0};
    linuxbt_print_uuid(&uuid,uuid_s);

    BT_DBG_INFO(BT_DEBUG_GATT,"add service uuid:%s handle = %d, status = %d",uuid_s,srvc_handle,status);
    BT_GATTS_SRVC_ID_T bt_srvc_id;
    bt_srvc_id.is_primary = srvc_id->is_primary;
    bt_srvc_id.id.inst_id = srvc_id->id.inst_id;
    linuxbt_print_uuid(&uuid,bt_srvc_id.id.uuid);
    bluetooth_gatts_service_added_callback(status, server_if, &bt_srvc_id, srvc_handle);
}

/**
 * FUNCTION NAME: linuxbt_gatts_included_service_added_callback
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
static void linuxbt_gatts_included_service_added_callback(int status, int server_if, int srvc_handle, int incl_srvc_handle)
{
    BT_DBG_INFO(BT_DEBUG_GATT, "add included service:%d in service: %d, status = %d",incl_srvc_handle, srvc_handle,status);
    bluetooth_gatts_included_service_added_callback(status, server_if, srvc_handle, incl_srvc_handle);
}

/**
 * FUNCTION NAME: linuxbt_gatts_characteristic_added_callback
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
static void linuxbt_gatts_characteristic_added_callback(int status, int server_if, bt_uuid_t *uuid, int srvc_handle, int char_handle)
{
    char uuid_s[BT_GATT_MAX_UUID_LEN] = {0};
    linuxbt_print_uuid(uuid,uuid_s);

    BT_DBG_INFO(BT_DEBUG_GATT, "add char uuid:%s in service:%d handle = %d, status = %d",uuid_s,srvc_handle,char_handle,status);
    bluetooth_gatts_characteristic_added_callback(status, server_if, uuid_s, srvc_handle, char_handle);

}

/**
 * FUNCTION NAME: linuxbt_gatts_descriptor_added_callback
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
static void linuxbt_gatts_descriptor_added_callback(int status, int server_if, bt_uuid_t *uuid, int srvc_handle, int descr_handle)
{
    char uuid_s[BT_GATT_MAX_UUID_LEN] = {0};
    linuxbt_print_uuid(uuid,uuid_s);

    BT_DBG_INFO(BT_DEBUG_GATT,"add descriptor uuid:%s in service:%d handle = %d, status = %d",uuid_s,srvc_handle,descr_handle,status);
    bluetooth_gatts_descriptor_added_callback(status, server_if, uuid_s, srvc_handle, descr_handle);
}

/**
 * FUNCTION NAME: linuxbt_gatts_service_started_callback
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
static void linuxbt_gatts_service_started_callback(int status, int server_if, int srvc_handle)
{
    //FUNC_ENTRY;
    BT_DBG_INFO(BT_DEBUG_GATT, "service started handle = %d, status = %d",srvc_handle,status);
    bluetooth_gatts_service_started_callback(status, server_if, srvc_handle);
}

/**
 * FUNCTION NAME: linuxbt_gatts_service_stopped_callback
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
static void linuxbt_gatts_service_stopped_callback(int status, int server_if, int srvc_handle)
{
    //FUNC_ENTRY;
    BT_DBG_INFO(BT_DEBUG_GATT,"service stopped handle = %d, status = %d",srvc_handle,status);
    bluetooth_gatts_service_stopped_callback(status, server_if, srvc_handle);
}

/**
 * FUNCTION NAME: linuxbt_gatts_service_deleted_callback
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
static void linuxbt_gatts_service_deleted_callback(int status, int server_if, int srvc_handle)
{
    //FUNC_ENTRY;
    BT_DBG_INFO(BT_DEBUG_GATT,"service stopped handle = %d, status = %d",srvc_handle,status);
    bluetooth_gatts_service_deleted_callback(status, server_if, srvc_handle);
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
static void linuxbt_gatts_request_read_callback(int conn_id, int trans_id, bt_bdaddr_t *bda, int attr_handle, int offset, bool is_long)
{
    BT_DBG_INFO(BT_DEBUG_GATT, "%02X:%02X:%02X:%02X:%02X:%02X\n request read, trans_id = %d , attr_handle = %d",
                bda->address[0], bda->address[1], bda->address[2],
                bda->address[3], bda->address[4], bda->address[5],trans_id,attr_handle);
    char btaddr_s[MAX_BDADDR_LEN];
    linuxbt_btaddr_htos(bda, btaddr_s);
    bluetooth_gatts_request_read_callback(conn_id, trans_id, btaddr_s, attr_handle, offset, is_long);
}

/**
 * FUNCTION NAME: linuxbt_gatts_request_write_callback
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
static void linuxbt_gatts_request_write_callback(int conn_id, int trans_id, bt_bdaddr_t *bda, int attr_handle, int offset, int length,
        bool need_rsp, bool is_prep, uint8_t* value)
{
    BT_DBG_INFO(BT_DEBUG_GATT, "%02X:%02X:%02X:%02X:%02X:%02X\n request write, need_respond = %d , trans_id = %d , attr_handle = %d, value:%s",
                bda->address[0], bda->address[1], bda->address[2],
                bda->address[3], bda->address[4], bda->address[5],need_rsp,trans_id,attr_handle,value);
#if 0
    if (need_rsp)
    {
        btgatt_response_t response;
        response.handle = attr_handle;
        response.attr_value.len = 0;
        if (NULL == linuxbt_gatts_interface)
        {
            BT_DBG_ERROR(BT_DEBUG_GATT,"[GATTS] linuxbt_gatts_interface not init.");
            return;
        }
        linuxbt_gatts_interface->send_response(conn_id,trans_id,BT_STATUS_SUCCESS,&response);
    }
#endif
    char btaddr_s[MAX_BDADDR_LEN];
    linuxbt_btaddr_htos(bda, btaddr_s);
    bluetooth_gatts_request_write_callback(conn_id,
                                           trans_id,
                                           btaddr_s,
                                           attr_handle,
                                           offset,
                                           length,
                                           need_rsp,
                                           is_prep,
                                           value);
}

/**
 * FUNCTION NAME: linuxbt_gatts_request_exec_write_callback
 * PURPOSE:
 *      The function is callback function of gatt client request execute write operation
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
static void linuxbt_gatts_request_exec_write_callback(int conn_id, int trans_id, bt_bdaddr_t *bda, int exec_write)
{
    //FUNC_ENTRY;
    BT_DBG_INFO(BT_DEBUG_GATT,"%02X:%02X:%02X:%02X:%02X:%02X\n request exec write, conn_id = %d,trans_id = %d , exec_write = %d",
                bda->address[0], bda->address[1], bda->address[2],
                bda->address[3], bda->address[4], bda->address[5],conn_id,trans_id,exec_write);

#if !MTK_LINUX_GATTC_PTS_TEST
    btgatt_response_t response;
    response.handle = conn_id;
    response.attr_value.len = 0;
    if (NULL == linuxbt_gatts_interface)
    {
        BT_DBG_ERROR(BT_DEBUG_GATT,"[GATTS] linuxbt_gatts_interface not init.");
        return;
    }
    linuxbt_gatts_interface->send_response(conn_id,trans_id,BT_STATUS_SUCCESS,&response);
#endif
    char btaddr_s[MAX_BDADDR_LEN];
    linuxbt_btaddr_htos(bda, btaddr_s);
    bluetooth_gatts_request_exec_write_callback(conn_id,
            trans_id,
            btaddr_s,
            exec_write);

}

/**
 * FUNCTION NAME: linuxbt_gatts_response_confirmation_callback
 * PURPOSE:
 *      The function is callback function of gatt client response confirmation operation
 * INPUT:
 *      status             -- gatt error code status
 *      handle             --response confirmation attribute handle
 * OUTPUT:
 *      None
 * RETURN:
 *      None
 * NOTES:
 *      None
 */
static void linuxbt_gatts_response_confirmation_callback(int status, int handle)
{
    //FUNC_ENTRY;
    BT_DBG_INFO(BT_DEBUG_GATT,"response confirmation handle = %d, status = %d",handle,status);

}

/**
 * FUNCTION NAME: linuxbt_gatts_indication_sent_callback
 * PURPOSE:
 *      The function is callback function of gatt server send indication operation
 * INPUT:
 *      conn_id            -- coonection id
 *      status             -- gatt error code status
 * OUTPUT:
 *      None
 * RETURN:
 *      None
 * NOTES:
 *      None
 */
static void linuxbt_gatts_indication_sent_callback(int conn_id, int status)
{
    //FUNC_ENTRY;
    BT_DBG_INFO(BT_DEBUG_GATT, "indication sent conn_id = %d, status = %d",conn_id,status);
    bluetooth_gatts_indication_sent_callback(conn_id, status);

}

/**
 * FUNCTION NAME: linuxbt_gatts_congestion_callback
 * PURPOSE:
 *      The function is callback function of gatt server congestion
 * INPUT:
 *      conn_id              -- connection id
 *      congested            -- congested or not
 * OUTPUT:
 *      None
 * RETURN:
 *      None
 * NOTES:
 *      None
 */
static void linuxbt_gatts_congestion_callback(int conn_id, bool congested)
{
    //FUNC_ENTRY;
    BT_DBG_INFO(BT_DEBUG_GATT,"conn_id = %d, congestion = %d",conn_id,congested);

}

/**
 * FUNCTION NAME: linuxbt_gatts_mtu_changed_callback
 * PURPOSE:
 *      The function is callback function of gatt server mtu changed operation
 * INPUT:
 *      conn_id              -- connection id
 *      mtu                  -- maximum transmit unit
 * OUTPUT:
 *      None
 * RETURN:
 *      None
 * NOTES:
 *      None
 */
static void linuxbt_gatts_mtu_changed_callback(int conn_id, int mtu)
{
    //FUNC_ENTRY;
    BT_DBG_INFO(BT_DEBUG_GATT, "conn_id = %d, mtu = %d",conn_id,mtu);
    bluetooth_gatts_mtu_changed_callback(conn_id,mtu);

}

const btgatt_server_callbacks_t linuxbt_gatts_callbacks =
{
    linuxbt_gatts_register_server_callback,
    linuxbt_gatts_connection_callback,
    linuxbt_gatts_service_added_callback,
    linuxbt_gatts_included_service_added_callback,
    linuxbt_gatts_characteristic_added_callback,
    linuxbt_gatts_descriptor_added_callback,
    linuxbt_gatts_service_started_callback,
    linuxbt_gatts_service_stopped_callback,
    linuxbt_gatts_service_deleted_callback,
    linuxbt_gatts_request_read_callback,
    linuxbt_gatts_request_write_callback,
    linuxbt_gatts_request_exec_write_callback,
    linuxbt_gatts_response_confirmation_callback,
    linuxbt_gatts_indication_sent_callback,
    linuxbt_gatts_congestion_callback,
    linuxbt_gatts_mtu_changed_callback,
};

/**
 * FUNCTION NAME: linuxbt_gatts_init
 * PURPOSE:
 *      The function is used to initialize gatt server inteface.
 * INPUT:
 *      interface            -- gatt server infrace structure
 * OUTPUT:
 *      None
 * RETURN:
 *      BT_SUCCESS             -- Operate success.
 * NOTES:
 *      None
 */
int linuxbt_gatts_init(const btgatt_server_interface_t *pt_interface)
{
    //FUNC_ENTRY;

    linuxbt_gatts_interface = pt_interface;

    bt_uuid_t uuid;
    linuxbt_uuid_stoh(LINUXBT_GATTS_SERVER_UUID, &uuid);
#if 0
    //old code for register server app in the initialization
    if (linuxbt_gatts_interface->register_server(&uuid))
    {
    }
#endif
    return BT_SUCCESS;
}

/**
 * FUNCTION NAME: linuxbt_gatts_deinit
 * PURPOSE:
 *      The function is used to deinitialize gatt server.
 * INPUT:
 *      None
 * OUTPUT:
 *      None
 * RETURN:
 *      BT_SUCCESS             -- Operate success.
 * NOTES:
 *      None
 */
int linuxbt_gatts_deinit(void)
{
    //FUNC_ENTRY;
    /*if (pxpm_callback != NULL) {
        linuxbt_pxpm_deinit();
    }*/
    return BT_SUCCESS;
}




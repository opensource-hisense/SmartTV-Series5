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

/* FILE NAME:  linuxbt_gattc_if.c
 * AUTHOR: Xuemei Yang
 * PURPOSE:
 *      It provides GATTC operation interface and callback function implementation to MW higher layer.
 * NOTES:
 */


/*-----------------------------------------------------------------------------
                    include files
 ----------------------------------------------------------------------------*/


#include <string.h>
#include "bluetooth.h"
#include "bt_mw_common.h"
#include "linuxbt_gattc_if.h"
#include "linuxbt_common.h"
#include "linuxbt_gatt_if.h"
#include "bt_mw_gatt.h"

static int linuxbt_client_if = -1;
const btgatt_client_interface_t *linuxbt_gattc_interface = NULL;

#if defined(MTK_LINUX_GATT) && (MTK_LINUX_GATT == TRUE)
const btgatt_ex_client_interface_t *linuxbt_gattc_ex_interface = NULL;
#endif

#if 0
static int SCAN_MODE_LOW_POWER_WINDOW_MS = 500;
static int SCAN_MODE_LOW_POWER_INTERVAL_MS = 5000;
static int SCAN_MODE_BALANCED_WINDOW_MS = 2000;
static int SCAN_MODE_BALANCED_INTERVAL_MS = 5000;
static int SCAN_MODE_LOW_LATENCY_WINDOW_MS = 5000;
static int SCAN_MODE_LOW_LATENCY_INTERVAL_MS = 5000;
#endif
static int ADVERTISING_CHANNEL_ALL = 7;  //ADVERTISING_CHANNEL_37 | ADVERTISING_CHANNEL_38 | ADVERTISING_CHANNEL_39;


//Basic Gatt Client function
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
int linuxbt_gattc_register_app(char *uuid)
{
    //FUNC_ENTRY;
    bt_uuid_t app_uuid;
    bt_status_t ret= BT_STATUS_SUCCESS;
    if (NULL == linuxbt_gattc_interface)
    {
        BT_DBG_ERROR(BT_DEBUG_GATT, "[GATTC] linuxbt_gattc_interface not init.");
        return BT_ERR_STATUS_PARM_INVALID;
    }
    if (NULL == linuxbt_gattc_interface->register_client)
    {
        BT_DBG_ERROR(BT_DEBUG_GATT, "[GATTC] null pointer of register_client");
        return BT_ERR_STATUS_PARM_INVALID;
    }
    linuxbt_uuid_stoh(uuid, &app_uuid);
    ret = linuxbt_gattc_interface->register_client(&app_uuid);
    BT_DBG_MINOR(BT_DEBUG_GATT,"ret = %d", ret);
    return BT_STATUS_SUCCESS;//linuxbt_return_value_convert(ret);
}

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
int linuxbt_gattc_unregister_app(int client_if)
{
    //FUNC_ENTRY;
    bt_status_t ret = BT_STATUS_SUCCESS;
    if (NULL == linuxbt_gattc_interface)
    {
        BT_DBG_ERROR(BT_DEBUG_GATT,"[GATTC] linuxbt_gattc_interface not init.");
        return BT_ERR_STATUS_PARM_INVALID;
    }
    if (client_if < 0 && linuxbt_client_if == -1)
    {
        BT_DBG_ERROR(BT_DEBUG_GATT, "[GATTC] Unregister client : no client app need to unregister.");
        return BT_ERR_STATUS_PARM_INVALID;
    }
    ret = linuxbt_gattc_interface->unregister_client(client_if);
    BT_DBG_MINOR(BT_DEBUG_GATT,"ret = %d", ret);
    linuxbt_client_if = -1;
    return BT_STATUS_SUCCESS;//linuxbt_return_value_convert(ret);
}

/**
 * FUNCTION NAME: linuxbt_gattc_scan
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
int linuxbt_gattc_scan()
{
    bt_status_t ret = BT_STATUS_SUCCESS;
    if (NULL == linuxbt_gattc_interface)
    {
        BT_DBG_ERROR(BT_DEBUG_GATT, "[GATTC] linuxbt_gattc_interface not init.");
        return BT_ERR_STATUS_PARM_INVALID;
    }
    ret = linuxbt_gattc_interface->scan(true);
    BT_DBG_MINOR(BT_DEBUG_GATT,"ret = %d", ret);
    return BT_STATUS_SUCCESS;//linuxbt_return_value_convert(ret);
}

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
int linuxbt_gattc_stop_scan()
{
    //FUNC_ENTRY;
    bt_status_t ret = BT_STATUS_SUCCESS;
    if (NULL == linuxbt_gattc_interface)
    {
        BT_DBG_ERROR(BT_DEBUG_GATT, "[GATTC] linuxbt_gattc_interface not init.");
        return BT_ERR_STATUS_PARM_INVALID;
    }
    ret = linuxbt_gattc_interface->scan(false);
    BT_DBG_MINOR(BT_DEBUG_GATT,"ret = %d", ret);
    return BT_STATUS_SUCCESS;//linuxbt_return_value_convert(ret);
}

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
int linuxbt_gattc_open(int client_if, char *bt_addr, UINT8 is_direct, int transport)
{
    //FUNC_ENTRY;
    bt_bdaddr_t bdaddr;
    bt_status_t ret = BT_STATUS_SUCCESS;

    if (NULL == linuxbt_gattc_interface)
    {
        BT_DBG_ERROR(BT_DEBUG_GATT, "[GATTC] linuxbt_gattc_interface not init.");
        return BT_ERR_STATUS_PARM_INVALID;
    }
    if (-1 == client_if)
    {
        BT_DBG_ERROR(BT_DEBUG_GATT, "[GATTC] Open : no client app registered now.");
        return BT_ERR_STATUS_PARM_INVALID;
    }
    if (NULL == bt_addr)
    {
        BT_DBG_ERROR(BT_DEBUG_GATT, "[GATTC] null pointer");
        return BT_ERR_STATUS_PARM_INVALID;
    }

    linuxbt_btaddr_stoh(bt_addr, &bdaddr);
    BT_DBG_INFO(BT_DEBUG_GATT, "GATTC connect to %02X:%02X:%02X:%02X:%02X:%02X, client_if:%d",
                bdaddr.address[0], bdaddr.address[1], bdaddr.address[2],
                bdaddr.address[3], bdaddr.address[4], bdaddr.address[5], client_if);

    ret = linuxbt_gattc_interface->connect(client_if,&bdaddr,is_direct,transport);
    BT_DBG_MINOR(BT_DEBUG_GATT,"ret = %d", ret);
    return BT_STATUS_SUCCESS;//linuxbt_return_value_convert(ret);

}

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
int linuxbt_gattc_close(int client_if, char *bt_addr, int conn_id)
{
    //FUNC_ENTRY;
    bt_bdaddr_t bdaddr;
    bt_status_t ret = BT_STATUS_SUCCESS;
    if (NULL == linuxbt_gattc_interface)
    {
        BT_DBG_ERROR(BT_DEBUG_GATT, "[GATTC] linuxbt_gattc_interface not init.");
        return BT_ERR_STATUS_PARM_INVALID;
    }
    if (-1 == client_if)
    {
        BT_DBG_ERROR(BT_DEBUG_GATT, "[GATTC] Close : no client app registered now.");
        return BT_ERR_STATUS_PARM_INVALID;
    }

    linuxbt_btaddr_stoh(bt_addr, &bdaddr);

    BT_DBG_INFO(BT_DEBUG_GATT, "GATTC disconnect to %02X:%02X:%02X:%02X:%02X:%02X, client_if:%d",
                bdaddr.address[0], bdaddr.address[1], bdaddr.address[2],
                bdaddr.address[3], bdaddr.address[4], bdaddr.address[5], client_if);

    ret = linuxbt_gattc_interface->disconnect(client_if,&bdaddr,conn_id);
    BT_DBG_MINOR(BT_DEBUG_GATT,"ret = %d", ret);
    return BT_STATUS_SUCCESS;//linuxbt_return_value_convert(ret);

}

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
int linuxbt_gattc_listen(int client_if)
{
    //FUNC_ENTRY;
    bt_status_t ret = BT_STATUS_SUCCESS;
    if (NULL == linuxbt_gattc_interface)
    {
        BT_DBG_ERROR(BT_DEBUG_GATT, "[GATTC] linuxbt_gattc_interface not init.");
        return BT_ERR_STATUS_PARM_INVALID;
    }
    if (-1 == client_if)
    {
        BT_DBG_ERROR(BT_DEBUG_GATT, "[GATTC] Listen : no client app registered now.");
        return BT_ERR_STATUS_PARM_INVALID;
    }
    BT_DBG_INFO(BT_DEBUG_GATT, "[GATTC] Listen client_if: %d", client_if);
    ret = linuxbt_gattc_interface->listen(client_if,true);
    BT_DBG_MINOR(BT_DEBUG_GATT,"ret = %d", ret);
    return BT_STATUS_SUCCESS;//linuxbt_return_value_convert(ret);

}

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
int linuxbt_gattc_refresh(int client_if, char *bt_addr)
{
    //FUNC_ENTRY;
    bt_bdaddr_t bdaddr;
    bt_status_t ret = BT_STATUS_SUCCESS;
    if (NULL == linuxbt_gattc_interface)
    {
        BT_DBG_ERROR(BT_DEBUG_GATT,"[GATTC] linuxbt_gattc_interface not init.");
        return BT_ERR_STATUS_PARM_INVALID;
    }
    if (-1 == client_if)
    {
        BT_DBG_ERROR(BT_DEBUG_GATT,"[GATTC] Refresh : no client app registered now.");
        return BT_ERR_STATUS_PARM_INVALID;
    }

    linuxbt_btaddr_stoh(bt_addr, &bdaddr);
    BT_DBG_INFO(BT_DEBUG_GATT, "GATTC refresh %02X:%02X:%02X:%02X:%02X:%02X , client_if:%d",
                bdaddr.address[0], bdaddr.address[1], bdaddr.address[2],
                bdaddr.address[3], bdaddr.address[4], bdaddr.address[5], client_if);
    ret = linuxbt_gattc_interface->refresh(client_if,&bdaddr);
    BT_DBG_MINOR(BT_DEBUG_GATT,"ret = %d", ret);
    return BT_STATUS_SUCCESS;//linuxbt_return_value_convert(ret);
}

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
int linuxbt_gattc_search_service(int conn_id, char *pt_uuid)
{
    //FUNC_ENTRY;
    bt_status_t ret = BT_STATUS_SUCCESS;
    bt_uuid_t uuid;
    bt_uuid_t *uuid_ptr = NULL;
    if (NULL == linuxbt_gattc_interface)
    {
        BT_DBG_ERROR(BT_DEBUG_GATT, "[GATTC] linuxbt_gattc_interface not init.");
        return BT_ERR_STATUS_PARM_INVALID;
    }
    if (NULL == pt_uuid)
    {
        BT_DBG_ERROR(BT_DEBUG_GATT, "[GATTC] pt_uuid is null.");
        uuid_ptr = NULL;
    }
    else
    {
        linuxbt_uuid_stoh(pt_uuid, &uuid);
        uuid_ptr = &uuid;
    }
    ret = linuxbt_gattc_interface->search_service(conn_id, uuid_ptr);
    BT_DBG_MINOR(BT_DEBUG_GATT,"ret = %d", ret);
    return BT_STATUS_SUCCESS;//linuxbt_return_value_convert(ret);
}

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
                           int start_inst_id)
{
#if 0
    //FUNC_ENTRY;
    btgatt_srvc_id_t service_uuid;
    btgatt_gatt_id_t start_uuid;
    bt_status_t ret = BT_STATUS_SUCCESS;
    if (NULL == linuxbt_gattc_interface)
    {
        BT_DBG_ERROR(BT_DEBUG_GATT, "[GATTC] linuxbt_gattc_interface not init.");
        return BT_ERR_STATUS_PARM_INVALID;
    }
    if (-1 == linuxbt_client_if)
    {
        BT_DBG_ERROR(BT_DEBUG_GATT, "[GATTC] no client app registered now.");
        return BT_ERR_STATUS_PARM_INVALID;
    }
    /* GATTC get_char conn_id service_uuid inst_id [characteristic_uuid] [characteristic_inst_id] */

    if (NULL != pt_start_uuid)
    {
        linuxbt_uuid_stoh(pt_start_uuid, &(start_uuid.uuid));
        //memcpy(&(start_uuid.uuid), pt_start_uuid, sizeof(bt_uuid_t));
        start_uuid.inst_id = start_inst_id;
    }
    if (NULL != pt_service_uuid)
    {
        linuxbt_uuid_stoh(pt_service_uuid, &(service_uuid.id.uuid));
        //memcpy(&(service_uuid.id.uuid), pt_service_uuid, sizeof(bt_uuid_t));
        service_uuid.is_primary = 1;
        service_uuid.id.inst_id = service_inst_id;
    }
#if 0
    if (NULL != pt_start_uuid)
    {
        ret = linuxbt_gattc_interface->get_characteristic(conn_id,&service_uuid,&start_uuid);
    }
    else
    {
        ret = linuxbt_gattc_interface->get_characteristic(conn_id,&service_uuid,NULL);
    }
#endif
#endif
    return BT_STATUS_SUCCESS;//linuxbt_return_value_convert(ret);

}

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
                            int start_inst_id)
{
#if 0
    //FUNC_ENTRY;
    btgatt_srvc_id_t service_uuid;
    btgatt_gatt_id_t char_uuid;
    btgatt_gatt_id_t start_uuid;
    bt_status_t ret = BT_STATUS_SUCCESS;
    if (NULL == linuxbt_gattc_interface)
    {
        BT_DBG_ERROR(BT_DEBUG_GATT,"[GATTC] linuxbt_gattc_interface not init.");
        return BT_ERR_STATUS_PARM_INVALID;
    }
    if (-1 == linuxbt_client_if)
    {
        BT_DBG_ERROR(BT_DEBUG_GATT,"[GATTC] no client app registered now.");
        return BT_ERR_STATUS_PARM_INVALID;
    }
    /* GATTC get_descr conn_id service_uuid inst_id characteristic_uuid characteristic_inst_id [descriptor_uuid] [descriptor_inst_id] */
    if (NULL != pt_start_uuid)
    {
        linuxbt_uuid_stoh(pt_start_uuid, &(start_uuid.uuid));
        //memcpy(&(start_uuid.uuid), pt_start_uuid, sizeof(bt_uuid_t));
        start_uuid.inst_id = start_inst_id;
    }
    if (NULL != pt_char_uuid)
    {
        linuxbt_uuid_stoh(pt_char_uuid, &(char_uuid.uuid));
        //memcpy(&(char_uuid.uuid), pt_char_uuid, sizeof(bt_uuid_t));
        char_uuid.inst_id = char_inst_id;
    }
    if (NULL != pt_service_uuid)
    {
        linuxbt_uuid_stoh(pt_service_uuid, &(service_uuid.id.uuid));
        //memcpy(&(service_uuid.id.uuid), pt_service_uuid, sizeof(bt_uuid_t));
        service_uuid.is_primary = 1;
        service_uuid.id.inst_id = service_inst_id;
    }

    conn_id = conn_id;
#if 0
    if (NULL != pt_start_uuid)
    {
        ret = linuxbt_gattc_interface->get_descriptor(conn_id,&service_uuid,&char_uuid,&start_uuid);
    }
    else
    {
        ret = linuxbt_gattc_interface->get_descriptor(conn_id,&service_uuid,&char_uuid,NULL);
    }
#endif
#endif
    return BT_STATUS_SUCCESS;//linuxbt_return_value_convert(ret);

}

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
                                   int incl_inst_id)
{
#if 0
    //FUNC_ENTRY;
    btgatt_srvc_id_t service_uuid;
    btgatt_srvc_id_t incl_service_uuid;
    bt_status_t ret = BT_STATUS_SUCCESS;
    if (NULL == linuxbt_gattc_interface)
    {
        BT_DBG_ERROR(BT_DEBUG_GATT, "[GATTC] linuxbt_gattc_interface not init.");
        return BT_ERR_STATUS_PARM_INVALID;
    }
    if (-1 == linuxbt_client_if)
    {
        BT_DBG_ERROR(BT_DEBUG_GATT,"[GATTC] no client app registered now.");
        return BT_ERR_STATUS_PARM_INVALID;
    }
    /* GATTC get_incl_service conn_id service_uuid inst_id [included_service_uuid] [included_service_inst_id] */
    if (NULL != pt_service_uuid)
    {
        linuxbt_uuid_stoh(pt_service_uuid, &(service_uuid.id.uuid));
        //memcpy(&(service_uuid.id.uuid), pt_service_uuid, sizeof(bt_uuid_t));
        service_uuid.is_primary = 1;
        service_uuid.id.inst_id = service_inst_id;
    }
    if (NULL != pt_incl_uuid)
    {
        linuxbt_uuid_stoh(pt_incl_uuid, &(incl_service_uuid.id.uuid));
        //memcpy(&(incl_service_uuid.id.uuid), pt_incl_uuid, sizeof(bt_uuid_t));
        incl_service_uuid.is_primary = 0;
        incl_service_uuid.id.inst_id = incl_inst_id;
    }
#if 0
    if (NULL != pt_incl_uuid)
    {
        ret = linuxbt_gattc_interface->get_included_service(conn_id,&service_uuid,&incl_service_uuid);
    }
    else
    {
        ret = linuxbt_gattc_interface->get_included_service(conn_id,&service_uuid,NULL);
    }
#endif
#endif
    return BT_STATUS_SUCCESS;//linuxbt_return_value_convert(ret);

}

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
                            int auth_req)
{
    FUNC_ENTRY;
    bt_status_t ret = BT_STATUS_SUCCESS;
    if (NULL == linuxbt_gattc_interface)
    {
        BT_DBG_ERROR(BT_DEBUG_GATT,"[GATTC] linuxbt_gattc_interface not init.");
        return BT_ERR_STATUS_PARM_INVALID;
    }
    ret = linuxbt_gattc_interface->read_characteristic(conn_id,char_handle,auth_req);
    BT_DBG_MINOR(BT_DEBUG_GATT,"ret = %d", ret);
    return BT_STATUS_SUCCESS;//linuxbt_return_value_convert(ret);


}

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
                             int auth_req)
{
    FUNC_ENTRY;
    bt_status_t ret = BT_STATUS_SUCCESS;
    if (NULL == linuxbt_gattc_interface)
    {
        BT_DBG_ERROR(BT_DEBUG_GATT,"[GATTC] linuxbt_gattc_interface not init.");
        return BT_ERR_STATUS_PARM_INVALID;
    }
    ret = linuxbt_gattc_interface->read_descriptor(conn_id,descr_handle,auth_req);
    BT_DBG_MINOR(BT_DEBUG_GATT,"ret = %d", ret);
    return BT_STATUS_SUCCESS;//linuxbt_return_value_convert(ret);

}

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
                             char *value)
{
    FUNC_ENTRY;
    bt_status_t ret = BT_STATUS_SUCCESS;
    //WRITE_TYPE_DEFAULT = 2, WRITE_TYPE_NO_RESPONSE = 1, WRITE_TYPE_SIGNED = 4
    if (NULL == linuxbt_gattc_interface)
    {
        BT_DBG_ERROR(BT_DEBUG_GATT,"[GATTC] linuxbt_gattc_interface not init.");
        return BT_ERR_STATUS_PARM_INVALID;
    }
    ret = linuxbt_gattc_interface->write_characteristic(conn_id,char_handle,write_type,len,auth_req,value);
    BT_DBG_MINOR(BT_DEBUG_GATT,"ret = %d", ret);
    return BT_STATUS_SUCCESS;//linuxbt_return_value_convert(ret);
}

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
                              char *value)
{
    FUNC_ENTRY;
    bt_status_t ret = BT_STATUS_SUCCESS;
    //WRITE_TYPE_DEFAULT = 2, WRITE_TYPE_NO_RESPONSE = 1, WRITE_TYPE_SIGNED = 4
    if (NULL == linuxbt_gattc_interface)
    {
        BT_DBG_ERROR(BT_DEBUG_GATT,"[GATTC] linuxbt_gattc_interface not init.");
        return BT_ERR_STATUS_PARM_INVALID;
    }
    ret = linuxbt_gattc_interface->write_descriptor(conn_id,descr_handle,write_type,len,auth_req,value);
    BT_DBG_MINOR(BT_DEBUG_GATT,"ret = %d", ret);
    return BT_STATUS_SUCCESS;//linuxbt_return_value_convert(ret);
}

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
int linuxbt_gattc_execute_write(int conn_id, int execute)
{
    //FUNC_ENTRY;
    bt_status_t ret = BT_STATUS_SUCCESS;
    if (NULL == linuxbt_gattc_interface)
    {
        BT_DBG_ERROR(BT_DEBUG_GATT,"[GATTC] linuxbt_gattc_interface not init.");
        return BT_ERR_STATUS_PARM_INVALID;
    }
    if (-1 == linuxbt_client_if)
    {
        BT_DBG_ERROR(BT_DEBUG_GATT,"[GATTC] no client app registered now.");
        return BT_ERR_STATUS_PARM_INVALID;
    }

    /* GATTC execute_write conn_id execute */

    ret = linuxbt_gattc_interface->execute_write(conn_id,execute);
    BT_DBG_MINOR(BT_DEBUG_GATT,"ret = %d", ret);
    return BT_STATUS_SUCCESS;//linuxbt_return_value_convert(ret);
}

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
                           int char_handle)
{
    //FUNC_ENTRY;
    //btgatt_srvc_id_t service_uuid;
    //btgatt_gatt_id_t char_uuid;
    bt_bdaddr_t bdaddr;
    bt_status_t ret = BT_STATUS_SUCCESS;
    if (NULL == linuxbt_gattc_interface)
    {
        BT_DBG_ERROR(BT_DEBUG_GATT,"[GATTC] linuxbt_gattc_interface not init.");
        return BT_ERR_STATUS_PARM_INVALID;
    }
    if (-1 == client_if)
    {
        BT_DBG_ERROR(BT_DEBUG_GATT,"[GATTC] Open : no client app registered now.");
        return BT_ERR_STATUS_PARM_INVALID;
    }
    linuxbt_btaddr_stoh(bt_addr, &bdaddr);
    ret = linuxbt_gattc_interface->register_for_notification(client_if,&bdaddr,char_handle);
    BT_DBG_MINOR(BT_DEBUG_GATT,"ret = %d", ret);
    return BT_STATUS_SUCCESS;//linuxbt_return_value_convert(ret);

}

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
                             int char_handle)
{
    FUNC_ENTRY;
    bt_bdaddr_t bdaddr;
    bt_status_t ret = BT_STATUS_SUCCESS;
    if (NULL == linuxbt_gattc_interface)
    {
        BT_DBG_ERROR(BT_DEBUG_GATT,"[GATTC] linuxbt_gattc_interface not init.");
        return BT_ERR_STATUS_PARM_INVALID;
    }
    if (-1 == client_if)
    {
        BT_DBG_ERROR(BT_DEBUG_GATT,"[GATTC] Open : no client app registered now.");
        return BT_ERR_STATUS_PARM_INVALID;
    }
    if (NULL == bt_addr) //|| NULL == pt_service_uuid || NULL == pt_char_uuid)
    {
        BT_DBG_ERROR(BT_DEBUG_GATT,"[GATTC] null pointer");
        return BT_ERR_STATUS_PARM_INVALID;
    }
    linuxbt_btaddr_stoh(bt_addr, &bdaddr);
    ret = linuxbt_gattc_interface->deregister_for_notification(client_if,&bdaddr,char_handle);
    BT_DBG_MINOR(BT_DEBUG_GATT,"ret = %d", ret);
    return BT_STATUS_SUCCESS;//linuxbt_return_value_convert(ret);
}

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
int linuxbt_gattc_read_rssi(int client_if, char *bt_addr)
{
    //FUNC_ENTRY;
    bt_status_t ret = BT_STATUS_SUCCESS;
    bt_bdaddr_t bdaddr;
    if (NULL == linuxbt_gattc_interface)
    {
        BT_DBG_ERROR(BT_DEBUG_GATT,"[GATTC] linuxbt_gattc_interface not init.");
        return BT_ERR_STATUS_PARM_INVALID;
    }
    if (-1 == client_if)
    {
        BT_DBG_ERROR(BT_DEBUG_GATT,"[GATTC] read rssi : no client app registered now.");
        return BT_ERR_STATUS_PARM_INVALID;
    }
    if (NULL == bt_addr)
    {
        BT_DBG_ERROR(BT_DEBUG_GATT,"[GATTC] null pointer");
        return BT_ERR_STATUS_PARM_INVALID;
    }
    /* GATTC read_rssi BD_ADDR */
    linuxbt_btaddr_stoh(bt_addr, &bdaddr);

    ret = linuxbt_gattc_interface->read_remote_rssi(client_if,&bdaddr);
    BT_DBG_MINOR(BT_DEBUG_GATT,"ret = %d", ret);
    return BT_STATUS_SUCCESS;//linuxbt_return_value_convert(ret);
}

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
                                        int timeout)
{
    //FUNC_ENTRY;
    bt_status_t ret = BT_STATUS_SUCCESS;
    bt_bdaddr_t bd_addr;
    if (NULL == linuxbt_gattc_interface)
    {
        BT_DBG_ERROR(BT_DEBUG_GATT,"[GATTC] linuxbt_gattc_interface not init.");
        return BT_ERR_STATUS_PARM_INVALID;
    }
    if (NULL == pbt_addr)
    {
        BT_DBG_ERROR(BT_DEBUG_GATT,"[GATTC] null pointer");
        return BT_ERR_STATUS_PARM_INVALID;
    }
    linuxbt_btaddr_stoh(pbt_addr, &bd_addr);
    BT_DBG_INFO(BT_DEBUG_GATT, "[GATTC] %02X:%02X:%02X:%02X:%02X:%02X ,",
                bd_addr.address[0], bd_addr.address[1], bd_addr.address[2],
                bd_addr.address[3], bd_addr.address[4], bd_addr.address[5]);
    ret = linuxbt_gattc_interface->conn_parameter_update(&bd_addr,
            min_interval,
            max_interval,
            latency,
            timeout);
    BT_DBG_MINOR(BT_DEBUG_GATT,"ret = %d", ret);
    return BT_STATUS_SUCCESS;//linuxbt_return_value_convert(ret);

}

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
int linuxbt_gattc_set_scan_parameters(int client_if, int scan_interval, int scan_window)
{
    //FUNC_ENTRY;
    bt_status_t ret = BT_STATUS_SUCCESS;
    if (NULL == linuxbt_gattc_interface)
    {
        BT_DBG_ERROR(BT_DEBUG_GATT, "[GATTC] linuxbt_gattc_interface not init.");
        return BT_ERR_STATUS_PARM_INVALID;
    }
    if (-1 == client_if)
    {
        BT_DBG_ERROR(BT_DEBUG_GATT,"[GATTC] set scan parameters : no client app registered now.");
        return BT_ERR_STATUS_PARM_INVALID;
    }
    ret = linuxbt_gattc_interface->set_scan_parameters(client_if, scan_interval,scan_window);
    BT_DBG_MINOR(BT_DEBUG_GATT,"ret = %d", ret);
    return BT_STATUS_SUCCESS;//linuxbt_return_value_convert(ret);

}

// Scan filter function
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
int linuxbt_gattc_scan_filter_param_setup(bluetooth_gatt_filt_param_setup_t scan_filt_param)
{
    //FUNC_ENTRY;
    if (NULL == linuxbt_gattc_interface)
    {
        BT_DBG_ERROR(BT_DEBUG_GATT,"[GATTC] linuxbt_gattc_interface not init.");
        return BT_ERR_STATUS_NOT_READY;
    }
    if (-1 == scan_filt_param.client_if)
    {
        BT_DBG_ERROR(BT_DEBUG_GATT,"[GATTC] scan filter param setup : no client app registered now.");
        return BT_ERR_STATUS_PARM_INVALID;
    }

    /*GATTC adv_enable min_interval max_interval adv_type tx_power timeout*/
    btgatt_filt_param_setup_t filt_param;
    filt_param.client_if = scan_filt_param.client_if;
    filt_param.action = scan_filt_param.action;
    filt_param.filt_index = scan_filt_param.filt_index;
    filt_param.feat_seln = scan_filt_param.feat_seln;
    filt_param.list_logic_type = scan_filt_param.list_logic_type;
    filt_param.filt_logic_type = scan_filt_param.filt_logic_type;
    filt_param.rssi_high_thres = scan_filt_param.rssi_high_thres;
    filt_param.rssi_low_thres = scan_filt_param.rssi_low_thres;
    filt_param.dely_mode = scan_filt_param.dely_mode;
    filt_param.found_timeout = scan_filt_param.found_timeout;
    filt_param.lost_timeout = scan_filt_param.lost_timeout;
    filt_param.found_timeout_cnt = scan_filt_param.found_timeout_cnt;
    filt_param.num_of_tracking_entries = scan_filt_param.num_of_tracking_entries;
    linuxbt_gattc_interface->scan_filter_param_setup(filt_param);
    return BT_SUCCESS;
}

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
int linuxbt_gattc_scan_filter_enable(int client_if)
{
    //FUNC_ENTRY;
    if (NULL == linuxbt_gattc_interface)
    {
        BT_DBG_ERROR(BT_DEBUG_GATT,"[GATTC] linuxbt_gattc_interface not init.");
        return BT_ERR_STATUS_NOT_READY;
    }
    if (-1 == client_if)
    {
        BT_DBG_ERROR(BT_DEBUG_GATT,"[GATTC] scan filter enable : no client app registered now.");
        return BT_ERR_STATUS_PARM_INVALID;
    }
    BT_DBG_NORMAL(BT_DEBUG_GATT,"client_if:%d",client_if);
    /*GATTC adv_enable min_interval max_interval adv_type tx_power timeout*/
    linuxbt_gattc_interface->scan_filter_enable(client_if,true);
    return BT_SUCCESS;
}

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
int linuxbt_gattc_scan_filter_disable(int client_if)
{
    //FUNC_ENTRY;
    if (NULL == linuxbt_gattc_interface)
    {
        BT_DBG_ERROR(BT_DEBUG_GATT,"[GATTC] linuxbt_gattc_interface not init.");
        return BT_ERR_STATUS_NOT_READY;
    }
    if (-1 == client_if)
    {
        BT_DBG_ERROR(BT_DEBUG_GATT,"[GATTC] scan filter disable : no client app registered now.");
        return BT_ERR_STATUS_PARM_INVALID;
    }
    BT_DBG_NORMAL(BT_DEBUG_GATT,"client_if:%d",client_if);
    /*GATTC adv_enable min_interval max_interval adv_type tx_power timeout*/
    linuxbt_gattc_interface->scan_filter_enable(client_if,false);

    return BT_SUCCESS;
}

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
        char* p_mask)
{
    //FUNC_ENTRY;
    bt_uuid_t uuid;
    bt_uuid_t uuid_mask;
    bt_bdaddr_t bdaddr;
    if (NULL == linuxbt_gattc_interface)
    {
        BT_DBG_ERROR(BT_DEBUG_GATT,"[GATTC] linuxbt_gattc_interface not init.");
        return BT_ERR_STATUS_NOT_READY;
    }
    if (-1 == client_if)
    {
        BT_DBG_ERROR(BT_DEBUG_GATT,"[GATTC] scan filter add_remove : no client app registered now.");
        return BT_ERR_STATUS_PARM_INVALID;
    }

    if (NULL == p_uuid)
    {
        BT_DBG_ERROR(BT_DEBUG_GATT, "[GATTC] p_uuid is null.");
        //return BT_ERR_STATUS_PARM_INVALID;
    }

    if (NULL == p_uuid_mask)
    {
        BT_DBG_ERROR(BT_DEBUG_GATT, "[GATTC] p_uuid is null.");
        //return BT_ERR_STATUS_PARM_INVALID;
    }

    if (NULL == bd_addr)
    {
        BT_DBG_ERROR(BT_DEBUG_GATT,"[GATTC] null pointer");
        //return BT_ERR_STATUS_PARM_INVALID;
    }

    linuxbt_uuid_stoh((CHAR*)p_uuid, &uuid);
    linuxbt_uuid_stoh((CHAR*)p_uuid_mask, &uuid_mask);
    linuxbt_btaddr_stoh((CHAR*)bd_addr, &bdaddr);

    linuxbt_gattc_interface->scan_filter_add_remove(client_if,action,filt_type,
            filt_index,company_id,company_id_mask,&uuid,
            &uuid_mask,&bdaddr,addr_type,data_len,p_data,mask_len,p_mask);
    return BT_SUCCESS;
}

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
int linuxbt_gattc_scan_filter_clear(int client_if, int filt_index)
{
    //FUNC_ENTRY;
    if (NULL == linuxbt_gattc_interface)
    {
        BT_DBG_ERROR(BT_DEBUG_GATT,"[GATTC] linuxbt_gattc_interface not init.");
        return BT_ERR_STATUS_NOT_READY;
    }
    if (-1 == client_if)
    {
        BT_DBG_ERROR(BT_DEBUG_GATT,"[GATTC] scan filter add_remove : no client app registered now.");
        return BT_ERR_STATUS_PARM_INVALID;
    }
    linuxbt_gattc_interface->scan_filter_clear(client_if,filt_index);
    return BT_SUCCESS;
}

#if MTK_LINUX_GATTC_LE_NAME //== TRUE
int linuxbt_gattc_set_local_le_name(int client_if, char *name)
{
    if (NULL == linuxbt_gattc_ex_interface)
    {
        BT_DBG_ERROR(BT_DEBUG_GATT,"[GATTC] linuxbt_gattc_interface not init.");
        return BT_ERR_STATUS_NOT_READY;
    }
    if (NULL == name)
    {
        BT_DBG_ERROR(BT_DEBUG_GATT,"[GATTC] input name is NULL.");
        return BT_ERR_STATUS_PARM_INVALID;
    }
    if (-1 == client_if)
    {
        BT_DBG_ERROR(BT_DEBUG_GATT,"[GATTC] set local lename : no client app registered now.");
        return BT_ERR_STATUS_PARM_INVALID;
    }
    linuxbt_gattc_ex_interface->set_local_le_name(client_if,name);
    return BT_SUCCESS;
}
#endif

#if MTK_LINUX_GATTC_RPA
int linuxbt_gattc_get_local_adv_rpa(int client_if)
{
    if (NULL == linuxbt_gattc_ex_interface)
    {
        BT_DBG_ERROR(BT_DEBUG_GATT,"[GATTC] linuxbt_gattc_interface not init.");
        return BT_ERR_STATUS_NOT_READY;
    }
    if (-1 == client_if)
    {
        BT_DBG_ERROR(BT_DEBUG_GATT,"[GATTC] get local adv rpa : no client app registered now.");
        return BT_ERR_STATUS_PARM_INVALID;
    }
    linuxbt_gattc_ex_interface->get_adv_rpa(client_if);
    return BT_SUCCESS;
}
#endif

#if MTK_LINUX_GATTC_DISC_MODE
int linuxbt_gattc_set_local_disc_mode(int client_if, int disc_mode)
{
    FUNC_ENTRY;
    if (NULL == linuxbt_gattc_ex_interface)
    {
        BT_DBG_ERROR(BT_DEBUG_GATT,"[GATTC] linuxbt_gattc_interface not init.");
        return BT_ERR_STATUS_NOT_READY;
    }
    if (-1 == client_if)
    {
        BT_DBG_ERROR(BT_DEBUG_GATT,"[GATTC] set local disc mode : no client app registered now.");
        return BT_ERR_STATUS_PARM_INVALID;
    }
    linuxbt_gattc_ex_interface->set_discoverable_mode(client_if,disc_mode);
    return BT_SUCCESS;
}

#endif

#if MTK_LINUX_GATTC_PTS_TEST
int linuxbt_gattc_read_using_char_uuid(int conn_id,
                                                 int start_handle,
                                                 int end_handle,
                                                 CHAR *uuid,
                                                 int auth_req)
{
    FUNC_ENTRY;
    bt_status_t ret = BT_STATUS_SUCCESS;
    bt_uuid_t char_uuid;
    if (NULL == linuxbt_gattc_ex_interface)
    {
        BT_DBG_ERROR(BT_DEBUG_GATT,"[GATTC] linuxbt_gattc_interface not init.");
        return BT_ERR_STATUS_PARM_INVALID;
    }
    linuxbt_uuid_stoh(uuid, &char_uuid);
    ret = linuxbt_gattc_ex_interface->read_using_characteristic_uuid(conn_id, start_handle, end_handle, &char_uuid, auth_req);
    BT_DBG_MINOR(BT_DEBUG_GATT,"ret = %d", ret);
    return BT_STATUS_SUCCESS;//linuxbt_return_value_convert(ret);
}

int linuxbt_gattc_read_long_characteristic(int conn_id, uint16_t handle,
                                                     uint16_t offset, int auth_req)
{
    FUNC_ENTRY;
    bt_status_t ret = BT_STATUS_SUCCESS;
    if (NULL == linuxbt_gattc_ex_interface)
    {
        BT_DBG_ERROR(BT_DEBUG_GATT,"[GATTC] linuxbt_gattc_interface not init.");
        return BT_ERR_STATUS_PARM_INVALID;
    }
    ret = linuxbt_gattc_ex_interface->read_long_characteristic(conn_id, handle, offset, auth_req);
    BT_DBG_MINOR(BT_DEBUG_GATT,"ret = %d", ret);
    return BT_STATUS_SUCCESS;//linuxbt_return_value_convert(ret);
}

int linuxbt_gattc_read_multi_characteristic(int conn_id, uint8_t num_attr,
                                                      uint16_t *handles, int auth_req)
{
    FUNC_ENTRY;
    bt_status_t ret = BT_STATUS_SUCCESS;
    if (NULL == linuxbt_gattc_ex_interface)
    {
        BT_DBG_ERROR(BT_DEBUG_GATT,"[GATTC] linuxbt_gattc_interface not init.");
        return BT_ERR_STATUS_PARM_INVALID;
    }
    if (NULL == handles)
    {
        BT_DBG_ERROR(BT_DEBUG_GATT,"[GATTC] handles is NULL.");
        return BT_ERR_STATUS_PARM_INVALID;
    }
    ret = linuxbt_gattc_ex_interface->read_multi_characteristic(conn_id, num_attr, handles, auth_req);
    BT_DBG_MINOR(BT_DEBUG_GATT,"ret = %d", ret);
    return BT_STATUS_SUCCESS;//linuxbt_return_value_convert(ret);
}

int linuxbt_gattc_read_long_descr(int conn_id, uint16_t handle,
                                          uint16_t offset, int auth_req)
{
    FUNC_ENTRY;
    bt_status_t ret = BT_STATUS_SUCCESS;
    if (NULL == linuxbt_gattc_ex_interface)
    {
        BT_DBG_ERROR(BT_DEBUG_GATT,"[GATTC] linuxbt_gattc_interface not init.");
        return BT_ERR_STATUS_PARM_INVALID;
    }
    ret = linuxbt_gattc_ex_interface->read_long_char_descr(conn_id, handle, offset, auth_req);
    BT_DBG_MINOR(BT_DEBUG_GATT,"ret = %d", ret);
    return BT_STATUS_SUCCESS;//linuxbt_return_value_convert(ret);
}

int linuxbt_gattc_write_long_characteristic(int conn_id, uint16_t handle, int write_type,
                                         int len, uint16_t offset, int auth_req, char* p_value)
{
    FUNC_ENTRY;
    bt_status_t ret = BT_STATUS_SUCCESS;
    if (NULL == linuxbt_gattc_ex_interface)
    {
        BT_DBG_ERROR(BT_DEBUG_GATT, "[GATTC] linuxbt_gattc_interface not init.");
        return BT_ERR_STATUS_PARM_INVALID;
    }
    ret = linuxbt_gattc_ex_interface->write_long_char(conn_id, handle, write_type,
        len, offset, auth_req, p_value);
    BT_DBG_MINOR(BT_DEBUG_GATT, "ret = %d", ret);
    return BT_STATUS_SUCCESS;//linuxbt_return_value_convert(ret);
}

int linuxbt_gattc_write_long_descr(int conn_id, uint16_t handle, int write_type,
                                         int len, uint16_t offset, int auth_req, char* p_value)
{
    FUNC_ENTRY;
    bt_status_t ret = BT_STATUS_SUCCESS;
    if (NULL == linuxbt_gattc_ex_interface)
    {
        BT_DBG_ERROR(BT_DEBUG_GATT, "[GATTC] linuxbt_gattc_interface not init.");
        return BT_ERR_STATUS_PARM_INVALID;
    }
    ret = linuxbt_gattc_ex_interface->write_long_char_descr(conn_id, handle, write_type,
        len, offset, auth_req, p_value);
    BT_DBG_MINOR(BT_DEBUG_GATT, "ret = %d", ret);
    return BT_STATUS_SUCCESS;//linuxbt_return_value_convert(ret);
}

int linuxbt_gattc_set_pts_flag(uint8_t pts_flag)
{
    FUNC_ENTRY;
    bt_status_t ret = BT_STATUS_SUCCESS;
    if (NULL == linuxbt_gattc_ex_interface)
    {
        BT_DBG_ERROR(BT_DEBUG_GATT, "[GATTC] linuxbt_gattc_interface not init.");
        return BT_ERR_STATUS_PARM_INVALID;
    }
    ret = linuxbt_gattc_ex_interface->set_pts_test_flag(pts_flag);
    BT_DBG_MINOR(BT_DEBUG_GATT, "ret = %d", ret);
    return BT_STATUS_SUCCESS;//linuxbt_return_value_convert(ret);
}
#endif

// Parameters function

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
int linuxbt_gattc_get_device_type(char *bt_addr)
{
    bt_bdaddr_t bdaddr;
    if (NULL == linuxbt_gattc_interface)
    {
        BT_DBG_ERROR(BT_DEBUG_GATT,"[GATTC] linuxbt_gattc_interface not init.");
        return BT_ERR_STATUS_PARM_INVALID;
    }

    if (NULL == bt_addr)
    {
        BT_DBG_ERROR(BT_DEBUG_GATT,"[GATTC] null pointer");
        return BT_ERR_STATUS_PARM_INVALID;
    }
    linuxbt_btaddr_stoh(bt_addr, &bdaddr);
    return linuxbt_gattc_interface->get_device_type(&bdaddr);
}

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
                               char* service_uuid)
{
    //FUNC_ENTRY;
    bt_status_t ret = BT_STATUS_SUCCESS;
    if (NULL == linuxbt_gattc_interface)
    {
        BT_DBG_ERROR(BT_DEBUG_GATT,"[GATTC] linuxbt_gattc_interface not init.");
        return BT_ERR_STATUS_PARM_INVALID;
    }
    if (-1 == client_if)
    {
        BT_DBG_ERROR(BT_DEBUG_GATT,"[GATTC] set adv data : no client app registered now.");
        return BT_ERR_STATUS_PARM_INVALID;
    }

    /*GATTC set_adv_data [set_scan_rsp true|false] [include_name true|false] [incl_txpower true|false] [min_interval int] [max_interval int] [appearance int] [manufacturer_data hex_string] [service_data hex_string] [service_uuid hex_string]*/
    BT_DBG_INFO(BT_DEBUG_GATT, "set_scan_rsp : %d" ,set_scan_rsp);
    BT_DBG_INFO(BT_DEBUG_GATT, "include_name : %d" ,include_name);
    BT_DBG_INFO(BT_DEBUG_GATT, "include_txpower : %d" ,include_txpower);
    BT_DBG_INFO(BT_DEBUG_GATT, "min_interval : %d" ,min_interval);
    //min_interval *= 1000/625;
    BT_DBG_INFO(BT_DEBUG_GATT, "max_interval : %d" ,max_interval);
    //max_interval *= 1000/625;
    BT_DBG_INFO(BT_DEBUG_GATT, "appearance : %d" ,appearance);
    BT_DBG_INFO(BT_DEBUG_GATT, "manufacturer_len : %d" ,manufacturer_len);
    BT_DBG_INFO(BT_DEBUG_GATT, "service_data_len : %d" ,service_data_len);
    BT_DBG_INFO(BT_DEBUG_GATT, "service_uuid_len : %d" ,service_uuid_len);


    ret = linuxbt_gattc_interface->set_adv_data(client_if,
            set_scan_rsp,
            include_name,
            include_txpower,
            min_interval,
            max_interval,
            appearance,
            manufacturer_len,
            manufacturer_data,
            service_data_len,
            service_data,
            service_uuid_len,
            service_uuid);
    BT_DBG_MINOR(BT_DEBUG_GATT,"ret = %d", ret);
    return BT_STATUS_SUCCESS;//linuxbt_return_value_convert(ret);

}

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
int linuxbt_gattc_configure_mtu(int conn_id, int mtu)
{
    //FUNC_ENTRY;
    bt_status_t ret = BT_STATUS_SUCCESS;
    if (NULL == linuxbt_gattc_interface)
    {
        BT_DBG_ERROR(BT_DEBUG_GATT,"[GATTC] linuxbt_gattc_interface not init.");
        return BT_ERR_STATUS_PARM_INVALID;
    }
    if ((mtu < BT_GATT_MIN_MTU_SIZE) || (mtu > BT_GATT_MAX_MTU_SIZE))
    {
        BT_DBG_ERROR(BT_DEBUG_GATT,"[GATTC] invalid mtu size %d.", mtu);
        return BT_ERR_STATUS_PARM_INVALID;
    }
    BT_DBG_INFO(BT_DEBUG_GATT, "gattc config mtu : %d", mtu);
    ret = linuxbt_gattc_interface->configure_mtu(conn_id, mtu);
    BT_DBG_MINOR(BT_DEBUG_GATT,"ret = %d", ret);
    return BT_SUCCESS;
}

// Multiple advertising function
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
                                   int timeout_s)
{
    //FUNC_ENTRY;
    bt_status_t ret = BT_STATUS_SUCCESS;
    if (NULL == linuxbt_gattc_interface)
    {
        BT_DBG_ERROR(BT_DEBUG_GATT,"[GATTC] linuxbt_gattc_interface not init.");
        return BT_ERR_STATUS_PARM_INVALID;
    }
    if (-1 == client_if)
    {
        BT_DBG_ERROR(BT_DEBUG_GATT,"[GATTC] adv enable : no client app registered now.");
        return BT_ERR_STATUS_PARM_INVALID;
    }
    BT_DBG_INFO(BT_DEBUG_GATT, "client_if:%d,min_interval:%d ,max_interval:%d, adv_type:%d,chnl_map:%d,tx_power:%d,timeout_s:%d",
                client_if,min_interval,max_interval,adv_type,ADVERTISING_CHANNEL_ALL,tx_power,timeout_s);
    /*GATTC adv_enable min_interval max_interval adv_type tx_power timeout*/
    ret = linuxbt_gattc_interface->multi_adv_enable(client_if,
            min_interval,
            max_interval,
            adv_type,
            ADVERTISING_CHANNEL_ALL,
            tx_power,
            timeout_s);
    BT_DBG_MINOR(BT_DEBUG_GATT,"ret = %d", ret);
    return BT_STATUS_SUCCESS;//linuxbt_return_value_convert(ret);

}

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
                                   int timeout_s)
{
    //FUNC_ENTRY;
    bt_status_t ret = BT_STATUS_SUCCESS;
    if (NULL == linuxbt_gattc_interface)
    {
        BT_DBG_ERROR(BT_DEBUG_GATT, "[GATTC] linuxbt_gattc_interface not init.");
        return BT_ERR_STATUS_PARM_INVALID;
    }
    if (-1 == client_if)
    {
        BT_DBG_ERROR(BT_DEBUG_GATT, "[GATTC] adv update : no client app registered now.");
        return BT_ERR_STATUS_PARM_INVALID;
    }
    BT_DBG_INFO(BT_DEBUG_GATT, "client_if:%d,min_interval:%d ,max_interval:%d, adv_type:%d,chnl_map:%d,tx_power:%d,timeout_s:%d",
                client_if,min_interval,max_interval,adv_type,ADVERTISING_CHANNEL_ALL,tx_power,timeout_s);

    /*GATTC adv_update min_interval max_interval adv_type tx_power timeout*/
    ret = linuxbt_gattc_interface->multi_adv_update(client_if,
            min_interval,
            max_interval,
            adv_type,
            ADVERTISING_CHANNEL_ALL,
            tx_power,
            timeout_s);
    BT_DBG_MINOR(BT_DEBUG_GATT,"ret = %d", ret);
    return BT_STATUS_SUCCESS;//linuxbt_return_value_convert(ret);
}

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
                                    char* service_uuid)
{
    //FUNC_ENTRY;
    bt_status_t ret = BT_STATUS_SUCCESS;
    char uuid[BT_GATT_UUID_ARRAY_SIZE] = {0};
    if (NULL == linuxbt_gattc_interface)
    {
        BT_DBG_ERROR(BT_DEBUG_GATT, "[GATTC] linuxbt_gattc_interface not init.");
        return BT_ERR_STATUS_PARM_INVALID;
    }
    if (-1 == client_if)
    {
        BT_DBG_ERROR(BT_DEBUG_GATT, "[GATTC] set adv data : no client app registered now.");
        return BT_ERR_STATUS_PARM_INVALID;
    }

    /*GATTC adv_update_data [set_scan_rsp true|false] [include_name true|false] [incl_txpower true|false] [appearance int] [manufacturer_data hex_string] [service_data hex_string] [service_uuid hex_string]*/

    BT_DBG_INFO(BT_DEBUG_GATT, "set_scan_rsp : %d" ,set_scan_rsp);
    BT_DBG_INFO(BT_DEBUG_GATT, "include_name : %d" ,include_name);
    BT_DBG_INFO(BT_DEBUG_GATT, "include_txpower : %d" ,include_txpower);
    BT_DBG_INFO(BT_DEBUG_GATT, "appearance : %d" ,appearance);
    BT_DBG_INFO(BT_DEBUG_GATT, "manufacturer_len : %d" ,manufacturer_len);
    BT_DBG_INFO(BT_DEBUG_GATT, "service_data_len : %d" ,service_data_len);
    BT_DBG_INFO(BT_DEBUG_GATT, "service_uuid_len : %d" ,service_uuid_len);
    if (service_uuid)
    {
        bluetooth_uuid_stoh(service_uuid, uuid);
        ret = linuxbt_gattc_interface->multi_adv_set_inst_data(client_if,
                set_scan_rsp,
                include_name,
                include_txpower,
                appearance,
                manufacturer_len,
                manufacturer_data,
                service_data_len,
                service_data,
                BT_GATT_UUID_ARRAY_SIZE,
                uuid);
    }
    else
    {

        ret = linuxbt_gattc_interface->multi_adv_set_inst_data(client_if,
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
    BT_DBG_MINOR(BT_DEBUG_GATT,"ret = %d", ret);
    return BT_STATUS_SUCCESS;//linuxbt_return_value_convert(ret);
}

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
int linuxbt_gattc_multi_adv_disable(int client_if)
{
    //FUNC_ENTRY;
    bt_status_t ret = BT_STATUS_SUCCESS;
    if (NULL == linuxbt_gattc_interface)
    {
        BT_DBG_ERROR(BT_DEBUG_GATT, "[GATTC] linuxbt_gattc_interface not init.");
        return BT_ERR_STATUS_PARM_INVALID;
    }
    if (-1 == client_if)
    {
        BT_DBG_ERROR(BT_DEBUG_GATT, "[GATTC] adv disable : no client app registered now.");
        return BT_ERR_STATUS_PARM_INVALID;
    }
    ret = linuxbt_gattc_interface->multi_adv_disable(client_if);
    BT_DBG_MINOR(BT_DEBUG_GATT,"ret = %d", ret);
    return BT_STATUS_SUCCESS;//linuxbt_return_value_convert(ret);
}

// Batch scan function
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
                                              int batch_scan_trunc_max, int batch_scan_notify_threshold)
{
    FUNC_ENTRY;
    bt_status_t ret = BT_STATUS_SUCCESS;
    if (NULL == linuxbt_gattc_interface)
    {
        BT_DBG_ERROR(BT_DEBUG_GATT, "[GATTC] linuxbt_gattc_interface not init.");
        return BT_ERR_STATUS_PARM_INVALID;
    }
    if (-1 == client_if)
    {
        BT_DBG_ERROR(BT_DEBUG_GATT, "[GATTC] adv disable : no client app registered now.");
        return BT_ERR_STATUS_PARM_INVALID;
    }
    ret = linuxbt_gattc_interface->batchscan_cfg_storage(client_if,
                batch_scan_full_max, batch_scan_trunc_max, batch_scan_notify_threshold);
    BT_DBG_MINOR(BT_DEBUG_GATT,"ret = %d", ret);
    return BT_STATUS_SUCCESS;//linuxbt_return_value_convert(ret);
}

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
                        int scan_interval, int scan_window, int addr_type, int discard_rule)
{
    FUNC_ENTRY;
    bt_status_t ret = BT_STATUS_SUCCESS;
    if (NULL == linuxbt_gattc_interface)
    {
        BT_DBG_ERROR(BT_DEBUG_GATT, "[GATTC] linuxbt_gattc_interface not init.");
        return BT_ERR_STATUS_PARM_INVALID;
    }
    if (-1 == client_if)
    {
        BT_DBG_ERROR(BT_DEBUG_GATT, "[GATTC] adv disable : no client app registered now.");
        return BT_ERR_STATUS_PARM_INVALID;
    }
    ret = linuxbt_gattc_interface->batchscan_enb_batch_scan(client_if,
                scan_mode, scan_interval, scan_window, addr_type, discard_rule);
    BT_DBG_MINOR(BT_DEBUG_GATT,"ret = %d", ret);
    return BT_STATUS_SUCCESS;

}

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
int linuxbt_gattc_batchscan_dis_batch_scan(int client_if)
{
    FUNC_ENTRY;
    bt_status_t ret = BT_STATUS_SUCCESS;
    if (NULL == linuxbt_gattc_interface)
    {
        BT_DBG_ERROR(BT_DEBUG_GATT, "[GATTC] linuxbt_gattc_interface not init.");
        return BT_ERR_STATUS_PARM_INVALID;
    }
    if (-1 == client_if)
    {
        BT_DBG_ERROR(BT_DEBUG_GATT, "[GATTC] adv disable : no client app registered now.");
        return BT_ERR_STATUS_PARM_INVALID;
    }
    ret = linuxbt_gattc_interface->batchscan_dis_batch_scan(client_if);
    BT_DBG_MINOR(BT_DEBUG_GATT,"ret = %d", ret);
    return BT_STATUS_SUCCESS;

}

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
int linuxbt_gattc_batchscan_read_reports(int client_if, int scan_mode)
{
    FUNC_ENTRY;
    bt_status_t ret = BT_STATUS_SUCCESS;
    if (NULL == linuxbt_gattc_interface)
    {
        BT_DBG_ERROR(BT_DEBUG_GATT, "[GATTC] linuxbt_gattc_interface not init.");
        return BT_ERR_STATUS_PARM_INVALID;
    }
    if (-1 == client_if)
    {
        BT_DBG_ERROR(BT_DEBUG_GATT, "[GATTC] adv disable : no client app registered now.");
        return BT_ERR_STATUS_PARM_INVALID;
    }
    ret = linuxbt_gattc_interface->batchscan_read_reports(client_if, scan_mode);
    BT_DBG_MINOR(BT_DEBUG_GATT,"ret = %d", ret);
    return BT_STATUS_SUCCESS;
}

int linuxbt_gattc_test_command(int command, BT_GATTC_TEST_PARAMS_T test_params)
{
    FUNC_ENTRY;
    bt_status_t ret = BT_STATUS_SUCCESS;
    if (NULL == linuxbt_gattc_interface)
    {
        BT_DBG_ERROR(BT_DEBUG_GATT,"[GATTC] linuxbt_gattc_interface not init.");
        return BT_ERR_STATUS_PARM_INVALID;
    }
    btgatt_test_params_t params;
    params.u1 = test_params.u1;
    params.u2 = test_params.u2;
    params.u3 = test_params.u3;
    params.u4 = test_params.u4;
    params.u5 = test_params.u5;
    linuxbt_btaddr_stoh(test_params.bda1, params.bda1);
    linuxbt_uuid_stoh(test_params.uuid1, params.uuid1);
    linuxbt_gattc_interface->test_command(command, &params);
    return ret;
}

int linuxbt_gattc_get_gatt_db(int conn_id)
{
    FUNC_ENTRY;
    bt_status_t ret = BT_STATUS_SUCCESS;
    if (NULL == linuxbt_gattc_interface)
    {
        BT_DBG_ERROR(BT_DEBUG_GATT,"[GATTC] linuxbt_gattc_interface not init.");
        return BT_ERR_STATUS_PARM_INVALID;
    }
    linuxbt_gattc_interface->get_gatt_db(conn_id);
    return ret;
}



/**
 * FUNCTION NAME: linuxbt_gattc_register_client_callback
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
static void linuxbt_gattc_register_client_callback(int status, int client_if, bt_uuid_t *app_uuid)
{
    //FUNC_ENTRY;
    BT_DBG_INFO(BT_DEBUG_GATT, "[GATTC] Register client callback client_if:%d, status:%d", client_if,status);
    if (0 == status)
    {
        linuxbt_client_if = client_if;
        //LE scan units(0.625ms)
        /*int scan_window = SCAN_MODE_BALANCED_WINDOW_MS*1000/625;
        int scan_interval = SCAN_MODE_BALANCED_INTERVAL_MS*1000/625;
        linuxbt_gattc_interface->set_scan_parameters(scan_interval,scan_window);*/
    }

    char uuid[BT_GATT_MAX_UUID_LEN] = {0};
    linuxbt_print_uuid(app_uuid, uuid);
    bluetooth_gattc_register_client_cbk(status, client_if, uuid);
}

/**
 * FUNCTION NAME: linuxbt_gattc_scan_result_callback
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
static void linuxbt_gattc_scan_result_callback(bt_bdaddr_t* bda, int rssi, uint8_t* adv_data)
{
    BT_DBG_INFO(BT_DEBUG_GATT, "Find %02X:%02X:%02X:%02X:%02X:%02X\n , rssi : %d",
                bda->address[0], bda->address[1], bda->address[2],
                bda->address[3], bda->address[4], bda->address[5],rssi);

    char btaddr_s[MAX_BDADDR_LEN];
    linuxbt_btaddr_htos(bda, btaddr_s);
    bluetooth_gattc_scan_result_cbk(btaddr_s, rssi, adv_data);

}

/**
 * FUNCTION NAME: linuxbt_gattc_connect_callback
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
static void linuxbt_gattc_connect_callback(int conn_id, int status, int client_if, bt_bdaddr_t* bda)
{
    //FUNC_ENTRY;
    BT_DBG_INFO(BT_DEBUG_GATT, "%02X:%02X:%02X:%02X:%02X:%02X\n connected, conn_id = %d , status = %d",
                bda->address[0], bda->address[1], bda->address[2],
                bda->address[3], bda->address[4], bda->address[5],conn_id,status);
    char btaddr_s[MAX_BDADDR_LEN];
    linuxbt_btaddr_htos(bda, btaddr_s);
    bluetooth_gattc_connect_cbk(conn_id, status, client_if, btaddr_s);
}

/**
 * FUNCTION NAME: linuxbt_gattc_disconnect_callback
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
static void linuxbt_gattc_disconnect_callback(int conn_id, int status, int client_if, bt_bdaddr_t* bda)
{
    //FUNC_ENTRY;
    BT_DBG_INFO(BT_DEBUG_GATT, "%02X:%02X:%02X:%02X:%02X:%02X\n disconnected, conn_id = %d , status = %d",
                bda->address[0], bda->address[1], bda->address[2],
                bda->address[3], bda->address[4], bda->address[5],conn_id,status);
    char btaddr_s[MAX_BDADDR_LEN];
    linuxbt_btaddr_htos(bda, btaddr_s);
    bluetooth_gattc_disconnect_cbk(conn_id, status, client_if, btaddr_s);
}

/**
 * FUNCTION NAME: linuxbt_gattc_search_complete_callback
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
static void linuxbt_gattc_search_complete_callback(int conn_id, int status)
{
    //FUNC_ENTRY;
    BT_DBG_INFO(BT_DEBUG_GATT,"search complete status = %d",status);
    bluetooth_gattc_search_complete_cbk(conn_id, status);
}

#if 0
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
static void linuxbt_gattc_search_result_callback( int conn_id, btgatt_srvc_id_t *srvc_id)
{
    bt_uuid_t uuid = srvc_id->id.uuid;
    //char uuid_s[37];
    BT_GATTS_SRVC_ID_T service_id;
    service_id.is_primary = srvc_id->is_primary;
    service_id.id.inst_id = srvc_id->id.inst_id;
    linuxbt_print_uuid(&uuid, service_id.id.uuid);
    //linuxbt_print_uuid(&uuid,uuid_s);

    BT_DBG_INFO(BT_DEBUG_GATT, "find service uuid:%s isPrimary = %d",service_id.id.uuid,srvc_id->is_primary);
    if (NULL == linuxbt_gattc_interface)
    {
        BT_DBG_ERROR(BT_DEBUG_GATT,"[GATTC] linuxbt_gattc_interface not init.");
        return;
    }
    linuxbt_gattc_interface->get_characteristic(conn_id,srvc_id,NULL);

    bluetooth_gattc_search_result_cbk(conn_id, &service_id);
}

/**
 * FUNCTION NAME: linuxbt_gattc_get_characteristic_callback
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
static void linuxbt_gattc_get_characteristic_callback(int conn_id, int status, btgatt_srvc_id_t *srvc_id, btgatt_gatt_id_t *char_id, int char_prop)
{
    bt_uuid_t uuid = srvc_id->id.uuid;
    //char uuid_s[37];
    BT_GATTS_SRVC_ID_T service_id;
    service_id.is_primary = srvc_id->is_primary;
    service_id.id.inst_id = srvc_id->id.inst_id;
    linuxbt_print_uuid(&uuid, service_id.id.uuid);
    //linuxbt_print_uuid(&uuid,uuid_s);
    bt_uuid_t char_uuid = char_id->uuid;
    //char char_uuid_s[37];
    BT_GATT_ID_T character_id;
    character_id.inst_id = char_id->inst_id;
    linuxbt_print_uuid(&char_uuid, character_id.uuid);
    //linuxbt_print_uuid(&char_uuid,char_uuid_s);

    BT_DBG_INFO(BT_DEBUG_GATT,"find char uuid:%s in service:%s, inst_id = %d, properties = %d ,status = %d",character_id.uuid,service_id.id.uuid,char_id->inst_id,char_prop,status);
    if (GATT_ERROR == status)
    {
        return; // GATT_ERROR
    }
    if (NULL == linuxbt_gattc_interface)
    {
        BT_DBG_ERROR(BT_DEBUG_GATT,"[GATTC] linuxbt_gattc_interface not init.");
        return;
    }
    linuxbt_gattc_interface->get_characteristic(conn_id,srvc_id,char_id);
    linuxbt_gattc_interface->get_descriptor(conn_id,srvc_id,char_id,NULL);
    bluetooth_gattc_get_characteristic_cbk(conn_id, status, &service_id, &character_id, char_prop);
}

/**
 * FUNCTION NAME: linuxbt_gattc_get_descriptor_callback
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
static void linuxbt_gattc_get_descriptor_callback(int conn_id, int status, btgatt_srvc_id_t *srvc_id, btgatt_gatt_id_t *char_id, btgatt_gatt_id_t *descr_id)
{
    bt_uuid_t uuid = srvc_id->id.uuid;
    //char uuid_s[37];
    BT_GATTS_SRVC_ID_T service_id;
    service_id.is_primary = srvc_id->is_primary;
    service_id.id.inst_id= srvc_id->id.inst_id;
    linuxbt_print_uuid(&uuid, service_id.id.uuid);
    //linuxbt_print_uuid(&uuid,uuid_s);
    bt_uuid_t char_uuid = char_id->uuid;
    //char char_uuid_s[37];
    BT_GATT_ID_T character_id;
    character_id.inst_id = char_id->inst_id;
    linuxbt_print_uuid(&char_uuid, character_id.uuid);
    //linuxbt_print_uuid(&char_uuid,char_uuid_s);
    bt_uuid_t descr_uuid = descr_id->uuid;
    //char descr_uuid_s[37];
    BT_GATT_ID_T descrip_id;
    descrip_id.inst_id = descr_id->inst_id;
    linuxbt_print_uuid(&descr_uuid, descrip_id.uuid);
    //linuxbt_print_uuid(&descr_uuid,descr_uuid_s);
    BT_DBG_INFO(BT_DEBUG_GATT,"find descr uuid:%s in char:%s service:%s, inst_id = %d, status = %d",descrip_id.uuid,character_id.uuid,service_id.id.uuid,descr_id->inst_id,status);
    if (GATT_ERROR == status)
    {
        return; // GATT_ERROR
    }
    if (NULL == linuxbt_gattc_interface)
    {
        BT_DBG_ERROR(BT_DEBUG_GATT,"[GATTC] linuxbt_gattc_interface not init.");
        return;
    }
    linuxbt_gattc_interface->get_descriptor(conn_id,srvc_id,char_id,descr_id);
    bluetooth_gattc_get_descriptor_cbk(conn_id, status, &service_id, &character_id, &descrip_id);
}

/**
 * FUNCTION NAME: linuxbt_gattc_get_included_service_callback
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
static void linuxbt_gattc_get_included_service_callback(int conn_id, int status, btgatt_srvc_id_t *srvc_id, btgatt_srvc_id_t *incl_srvc_id)
{
    bt_uuid_t uuid = srvc_id->id.uuid;
    //char uuid_s[37];
    BT_GATTS_SRVC_ID_T service_id;
    service_id.is_primary = srvc_id->is_primary;
    service_id.id.inst_id= srvc_id->id.inst_id;
    linuxbt_print_uuid(&uuid, service_id.id.uuid);
    //linuxbt_print_uuid(&uuid,uuid_s);
    bt_uuid_t incl_srvc_uuid = incl_srvc_id->id.uuid;
    //char incl_srvc_uuid_s[37];
    BT_GATTS_SRVC_ID_T incl_service_id;
    incl_service_id.is_primary = incl_srvc_id->is_primary;
    incl_service_id.id.inst_id= incl_srvc_id->id.inst_id;
    linuxbt_print_uuid(&incl_srvc_uuid, incl_service_id.id.uuid);
    //linuxbt_print_uuid(&incl_srvc_uuid,incl_srvc_uuid_s);
    BT_DBG_INFO(BT_DEBUG_GATT, "find included service uuid:%s in service:%s, inst_id = %d, status = %d",incl_service_id.id.uuid,service_id.id.uuid,incl_srvc_id->id.inst_id,status);
    if (GATT_ERROR == status)
    {
        return; // GATT_ERROR
    }

    if (NULL == linuxbt_gattc_interface)
    {
        BT_DBG_ERROR(BT_DEBUG_GATT,"[GATTC] linuxbt_gattc_interface not init.");
        return;
    }
    linuxbt_gattc_interface->get_included_service(conn_id,srvc_id,incl_srvc_id);
    bluetooth_gattc_get_included_service_cbk(conn_id, status, &service_id, &incl_service_id);
}
#endif

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
static void linuxbt_gattc_register_for_notification_callback(int conn_id, int registered, int status, uint16_t handle)
{
    FUNC_ENTRY;
    bluetooth_gattc_register_for_notification_cbk(conn_id, registered, status, handle);

}

/**
 * FUNCTION NAME: linuxbt_gattc_notify_callback
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
static void linuxbt_gattc_notify_callback(int conn_id, btgatt_notify_params_t *p_data)
{
    FUNC_ENTRY;
    BT_GATT_NOTI_PARAMS_T noti_params_data;
    memcpy(noti_params_data.value, p_data->value, BTGATT_MAX_ATTR_LEN);
    linuxbt_btaddr_htos(&(p_data->bda), noti_params_data.bda);
    noti_params_data.handle = p_data->handle;
    noti_params_data.len = p_data->len;
    noti_params_data.is_notify = p_data->is_notify;
    bluetooth_gattc_notify_cbk(conn_id, &noti_params_data);

}

/**
 * FUNCTION NAME: linuxbt_gattc_read_characteristic_callback
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
static void linuxbt_gattc_read_characteristic_callback(int conn_id, int status, btgatt_read_params_t *p_data)
{
    BT_GATT_READ_PARAMS_T read_params_data;
    memset(&read_params_data, 0 , sizeof(BT_GATT_READ_PARAMS_T));
    read_params_data.handle = p_data->handle;
    read_params_data.status = p_data->status;
    read_params_data.value_type = p_data->value_type;
    read_params_data.value.len = p_data->value.len;
    memcpy(read_params_data.value.value, p_data->value.value, p_data->value.len);

    BT_DBG_INFO(BT_DEBUG_GATT,"handle = %d", read_params_data.handle);
    BT_DBG_INFO(BT_DEBUG_GATT,"status = %d", read_params_data.status);
    BT_DBG_INFO(BT_DEBUG_GATT,"value_type = %d", read_params_data.value_type);
    BT_DBG_INFO(BT_DEBUG_GATT,"value len = %d", read_params_data.value.len);
    BT_DBG_INFO(BT_DEBUG_GATT,"value = %s", read_params_data.value.value);

    bluetooth_gattc_read_characteristic_cbk(conn_id, status, &read_params_data);
}

/**
 * FUNCTION NAME: linuxbt_gattc_write_characteristic_callback
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
static void linuxbt_gattc_write_characteristic_callback(int conn_id, int status, uint16_t handle)
{
    BT_DBG_INFO(BT_DEBUG_GATT,"handle = %d", handle);
    bluetooth_gattc_write_characteristic_cbk(conn_id, status, handle);
}

/**
 * FUNCTION NAME: linuxbt_gattc_read_descriptor_callback
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
static void linuxbt_gattc_read_descriptor_callback(int conn_id, int status, btgatt_read_params_t *p_data)
{
    BT_GATT_READ_PARAMS_T read_params_data;
    memset(&read_params_data, 0 , sizeof(BT_GATT_READ_PARAMS_T));
    read_params_data.handle = p_data->handle;
    read_params_data.status = p_data->status;
    read_params_data.value_type = p_data->value_type;
    read_params_data.value.len = p_data->value.len;
    memcpy(read_params_data.value.value, p_data->value.value, p_data->value.len);
    BT_DBG_INFO(BT_DEBUG_GATT,"handle = %d", read_params_data.handle);
    BT_DBG_INFO(BT_DEBUG_GATT,"status = %d", read_params_data.status);
    BT_DBG_INFO(BT_DEBUG_GATT,"value_type = %d", read_params_data.value_type);
    BT_DBG_INFO(BT_DEBUG_GATT,"value len = %d", read_params_data.value.len);
    BT_DBG_INFO(BT_DEBUG_GATT,"value = %s", read_params_data.value.value);
    bluetooth_gattc_read_descriptor_cbk(conn_id, status, &read_params_data);
}

/**
 * FUNCTION NAME: linuxbt_gattc_write_descriptor_callback
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
static void linuxbt_gattc_write_descriptor_callback(int conn_id, int status, uint16_t handle)
{
    BT_DBG_INFO(BT_DEBUG_GATT,"handle = %d", handle);
    bluetooth_gattc_write_descriptor_cbk(conn_id, status, handle);
}

/**
 * FUNCTION NAME: linuxbt_gattc_execute_write_callback
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
static void linuxbt_gattc_execute_write_callback(int conn_id, int status)
{
    //FUNC_ENTRY;
    BT_DBG_INFO(BT_DEBUG_GATT, "execute write status = %d",status);
    bluetooth_gattc_execute_write_cbk(conn_id, status);
}

/**
 * FUNCTION NAME: linuxbt_gattc_read_remote_rssi_callback
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
static void linuxbt_gattc_read_remote_rssi_callback(int client_if, bt_bdaddr_t* bda, int rssi, int status)
{
    //FUNC_ENTRY;
    BT_DBG_INFO(BT_DEBUG_GATT, "read %02X:%02X:%02X:%02X:%02X:%02X\n rssi = %d , status = %d",
                bda->address[0], bda->address[1], bda->address[2],
                bda->address[3], bda->address[4], bda->address[5],rssi,status);
    char btaddr_s[MAX_BDADDR_LEN];
    linuxbt_btaddr_htos(bda, btaddr_s);
    bluetooth_gattc_read_remote_rssi_cbk(client_if, btaddr_s, rssi, status);
}

/**
 * FUNCTION NAME: linuxbt_gattc_listen_callback
 * PURPOSE:
 *      The function is callback function of gatt client listen operation
 * INPUT:
 *      status               -- gatt error code status
 *      client_if            -- registered client app identifier
 * OUTPUT:
 *      None
 * RETURN:
 *      None
 * NOTES:
 *      None
 */
static void linuxbt_gattc_listen_callback(int status, int client_if)
{
    //FUNC_ENTRY;
}

/**
 * FUNCTION NAME: linuxbt_gattc_configure_mtu_callback
 * PURPOSE:
 *      The function is callback function of gatt client config mtu operation
 * INPUT:
 *      conn_id              -- connection id
 *      status               -- gatt error code status
 *      mtu                  -- maximum transmit unit
 * OUTPUT:
 *      None
 * RETURN:
 *      None
 * NOTES:
 *      None
 */
static void linuxbt_gattc_configure_mtu_callback(int conn_id, int status, int mtu)
{
    //FUNC_ENTRY;
    BT_DBG_INFO(BT_DEBUG_GATT,"configure mtu = %d, status = %d",mtu,status);

}

/**
 * FUNCTION NAME: linuxbt_gattc_scan_filter_cfg_callback
 * PURPOSE:
 *      The function is callback function of gatt client scan filter
 * INPUT:
 *      action               -- add, remove or clear a filter in scan filter
 *      client_if            -- registered client app identifier
 *      status               -- gatt error code status
 *      filt_type            -- the logical fulter type of feature selection
 *      avbl_space           -- number of available entries in the filters table
 * OUTPUT:
 *      None
 * RETURN:
 *      None
 * NOTES:
 *      None
 */
static void linuxbt_gattc_scan_filter_cfg_callback(int action, int client_if, int status, int filt_type, int avbl_space)
{
    BT_DBG_INFO(BT_DEBUG_GATT,"[GATTC] %s()", __FUNCTION__);
    BT_DBG_INFO(BT_DEBUG_GATT,"scan_filter_cfg action = %d, client_if = %d, filt_type = %d, status = %d, avbl_space = %d",
                action,client_if,filt_type,status,avbl_space);
    bluetooth_gattc_scan_filter_cfg_cbk(action, client_if, status, filt_type, avbl_space);
}

/**
 * FUNCTION NAME: linuxbt_gattc_scan_filter_param_callback
 * PURPOSE:
 *      The function is callback function of gatt client set scan filter paramter
 * INPUT:
 *      action               -- add, remove or clear a filter in scan filter
 *      client_if            -- registered client app identifier
 *      status               -- gatt error code status
 *      avbl_space           -- number of available entries in the filters table
 * OUTPUT:
 *      None
 * RETURN:
 *      None
 * NOTES:
 *      None
 */
static void linuxbt_gattc_scan_filter_param_callback(int action, int client_if, int status, int avbl_space)
{
    BT_DBG_INFO(BT_DEBUG_GATT,"[GATTC] %s()", __FUNCTION__);
    BT_DBG_INFO(BT_DEBUG_GATT,"scan_filter_param action = %d, status = %d, avbl_space = %d",action,status,avbl_space);
    bluetooth_gattc_scan_filter_param_cbk(action, client_if, status, avbl_space);
}

/**
 * FUNCTION NAME: linuxbt_gattc_scan_filter_status_callback
 * PURPOSE:
 *      The function is callback function of gatt client enable or disable scan filter
 * INPUT:
 *      enable               -- enable or disable scan filter
 *      client_if            -- registered client app identifier
 *      status               -- gatt error code status
 * OUTPUT:
 *      None
 * RETURN:
 *      None
 * NOTES:
 *      None
 */
static void linuxbt_gattc_scan_filter_status_callback(int enable, int client_if, int status)
{
    BT_DBG_INFO(BT_DEBUG_GATT,"[GATTC] %s()", __FUNCTION__);
    BT_DBG_INFO(BT_DEBUG_GATT,"scan_filter_status %d ,status = %d",enable,status);
    bluetooth_gattc_scan_filter_status_cbk(enable, client_if, status);
}

/**
 * FUNCTION NAME: linuxbt_gattc_multi_adv_enable_callback
 * PURPOSE:
 *      The function is callback function of gatt enable multiple advertising
 * INPUT:
 *      client_if            -- registered client app identifier
 *      status               -- gatt error code status
 * OUTPUT:
 *      None
 * RETURN:
 *      None
 * NOTES:
 *      None
 */
static void linuxbt_gattc_multi_adv_enable_callback(int client_if, int status)
{
    //BT_DBG_NORMAL(BT_DEBUG_GATT,"[GATTC] +++into");
    BT_DBG_INFO(BT_DEBUG_GATT,"status = %d",status);
    bluetooth_gattc_multi_adv_enable_cbk(client_if, status);
}

/**
 * FUNCTION NAME: linuxbt_gattc_multi_adv_update_callback
 * PURPOSE:
 *      The function is callback function of gatt update multiple advertising
 * INPUT:
 *      client_if            -- registered client app identifier
 *      status               -- gatt error code status
 * OUTPUT:
 *      None
 * RETURN:
 *      None
 * NOTES:
 *      None
 */
static void linuxbt_gattc_multi_adv_update_callback(int client_if, int status)
{
    //FUNC_ENTRY;
    BT_DBG_INFO(BT_DEBUG_GATT, "status = %d",status);
}

/**
 * FUNCTION NAME: linuxbt_gattc_multi_adv_data_callback
 * PURPOSE:
 *      The function is callback function of gatt set multiple advertising data
 * INPUT:
 *      client_if            -- registered client app identifier
 *      status               -- gatt error code status
 * OUTPUT:
 *      None
 * RETURN:
 *      None
 * NOTES:
 *      None
 */
static void linuxbt_gattc_multi_adv_data_callback(int client_if, int status)
{
    //FUNC_ENTRY;
    BT_DBG_INFO(BT_DEBUG_GATT,"status = %d",status);
    bluetooth_gattc_multi_adv_data_cbk(client_if, status);
}

/**
 * FUNCTION NAME: linuxbt_gattc_multi_adv_disable_callback
 * PURPOSE:
 *      The function is callback function of gatt disable multiple advertising
 * INPUT:
 *      client_if            -- registered client app identifier
 *      status               -- gatt error code status
 * OUTPUT:
 *      None
 * RETURN:
 *      None
 * NOTES:
 *      None
 */
static void linuxbt_gattc_multi_adv_disable_callback(int client_if, int status)
{
    //FUNC_ENTRY;
    BT_DBG_INFO(BT_DEBUG_GATT,"status = %d",status);
}

/**
 * FUNCTION NAME: linuxbt_gattc_congestion_callback
 * PURPOSE:
 *      The function is callback function of gatt client congestion
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
static void linuxbt_gattc_congestion_callback(int conn_id, bool congested)
{

}

/**
 * FUNCTION NAME: linuxbt_gattc_batchscan_cfg_storage_callback
 * PURPOSE:
 *      The function is callback function of gatt client to config batch scan storage paramter
 * INPUT:
 *      client_if            -- registered client app identifier
 *      status               -- gatt error code status
 * OUTPUT:
 *      None
 * RETURN:
 *      BT_SUCCESS             -- Operate success.
 *      BT_ERR_FAILED          -- Operate fail.
 * NOTES:
 *      None
 */
static void linuxbt_gattc_batchscan_cfg_storage_callback(int client_if, int status)
{
    BT_DBG_INFO(BT_DEBUG_GATT,"status = %d",status);
}

/**
 * FUNCTION NAME: linuxbt_gattc_batchscan_enable_disable_callback
 * PURPOSE:
 *      The function is callback function of gatt client to enable/disable batch scan
 * INPUT:
 *      action               -- enable or disable batch scan
 *      client_if            -- registered client app identifier
 *      status               -- gatt error code status
 * OUTPUT:
 *      None
 * RETURN:
 *      BT_SUCCESS             -- Operate success.
 *      BT_ERR_FAILED          -- Operate fail.
 * NOTES:
 *      None
 */
static void linuxbt_gattc_batchscan_enable_disable_callback(int action, int client_if, int status)
{
    BT_DBG_INFO(BT_DEBUG_GATT,"action= %d, status = %d", action, status);
}

/**
 * FUNCTION NAME: linuxbt_gattc_batchscan_reports_callback
 * PURPOSE:
 *      The function is callback function of gatt client to batch scan read reports
 * INPUT:
 *      client_if            -- registered client app identifier
 *      status               -- gatt error code status
 *      report_format        -- report format
 *      num_records          -- record number
 *      data_len             -- report data length
 *      rep_data             -- report data
 * OUTPUT:
 *      None
 * RETURN:
 *      BT_SUCCESS             -- Operate success.
 *      BT_ERR_FAILED          -- Operate fail.
 * NOTES:
 *      None
 */
static void linuxbt_gattc_batchscan_reports_callback(int client_if, int status, int report_format,
        int num_records, int data_len, uint8_t* rep_data)
{
    BT_DBG_INFO(BT_DEBUG_GATT,"report_format= %d, status = %d, num_records = %d, data_len = %d",
                report_format, status, num_records, data_len);
    for (int i = 0; i < data_len; i += 6)
    {
        if ((data_len - i) > 6)
        {
            BT_DBG_INFO(BT_DEBUG_GATT,"rep_data= %02X %02X %02X %02X %02X %02X ",
                rep_data[i], rep_data[i+1], rep_data[i+2], rep_data[i+3], rep_data[i+4], rep_data[i+5]);
        }
    }
}

/**
 * FUNCTION NAME: linuxbt_gattc_batchscan_threshold_callback
 * PURPOSE:
 *      The function is callback function of gatt client to set batch scan threshhold
 * INPUT:
 *      client_if            -- registered client app identifier
 * OUTPUT:
 *      None
 * RETURN:
 *      BT_SUCCESS             -- Operate success.
 *      BT_ERR_FAILED          -- Operate fail.
 * NOTES:
 *      None
 */
static void linuxbt_gattc_batchscan_threshold_callback(int client_if)
{
    BT_DBG_INFO(BT_DEBUG_GATT,"client_if= %d", client_if);
}

/**
 * FUNCTION NAME: linuxbt_gattc_track_adv_event_callback
 * PURPOSE:
 *      The function is callback function of gatt client to track advertising event
 * INPUT:
 *      client_if            -- registered client app identifier
 *      filt_index           -- filter index(0, max_filter-1)
 *      addr_type            -- public or random address type
 *      bda                  -- remote device bt address
 *      adv_state            -- found or lost adevertising event
 * OUTPUT:
 *      None
 * RETURN:
 *      BT_SUCCESS             -- Operate success.
 *      BT_ERR_FAILED          -- Operate fail.
 * NOTES:
 *      None
 */
static void linuxbt_gattc_track_adv_event_callback(btgatt_track_adv_info_t *p_track_adv_info)
{

}

static void linuxbt_gattc_scan_parameter_setup_completed_callback(int client_if,
        btgattc_error_t status)
{
    BT_DBG_INFO(BT_DEBUG_GATT,"client_if = %d, status = %d", client_if, status);

}

static void linuxbt_gattc_get_gatt_db_callback(int conn_id, btgatt_db_element_t *db, int count)
{
    bluetooth_gatt_db_element_t *gatt_db = malloc(count * sizeof(bluetooth_gatt_db_element_t));
    memset(gatt_db, 0, count * sizeof(bluetooth_gatt_db_element_t));
    bluetooth_gatt_db_element_t *curr_db_ptr = gatt_db;
    int i = 0;
    for (i = 0; i < count; i++)
    {
        curr_db_ptr->type = db->type;
        curr_db_ptr->attribute_handle = db->attribute_handle;
        curr_db_ptr->start_handle = db->start_handle;
        curr_db_ptr->end_handle = db->end_handle;
        curr_db_ptr->id = db->id;
        curr_db_ptr->properties = db->properties;
        linuxbt_print_uuid(&(db->uuid), curr_db_ptr->uuid);

        BT_DBG_INFO(BT_DEBUG_GATT,"type = %d, attribute_handle = %d",curr_db_ptr->type, curr_db_ptr->attribute_handle);
        BT_DBG_INFO(BT_DEBUG_GATT,"start_handle = %d, end_handle = %d",curr_db_ptr->start_handle, curr_db_ptr->end_handle);
        BT_DBG_INFO(BT_DEBUG_GATT,"id = %d, properties = %d",curr_db_ptr->id, curr_db_ptr->properties);
        BT_DBG_INFO(BT_DEBUG_GATT,"uuid = %s",curr_db_ptr->uuid);
        BT_DBG_INFO(BT_DEBUG_GATT,"\n");
        curr_db_ptr++;
        db++;
    }
    bluetooth_gattc_get_gatt_db_cbk(conn_id, gatt_db, count);
    free(gatt_db);
}

static void linuxbt_gattc_services_removed_callback(int conn_id, uint16_t start_handle, uint16_t end_handle)
{

}

static void linuxbt_gattc_services_added_callback(int conn_id, btgatt_db_element_t *added, int added_count)
{

}

#if MTK_LINUX_GATTC_RPA
static void linuxbt_gattc_get_adv_rpa_callback(int client_if, int status, bt_bdaddr_t* rpa)
{
    BT_DBG_INFO(BT_DEBUG_GATT, "get_adv_rpa %02X:%02X:%02X:%02X:%02X:%02X\n , status : %d, client_if : %d",
                rpa->address[0], rpa->address[1], rpa->address[2],
                rpa->address[3], rpa->address[4], rpa->address[5],status, client_if);
}
#endif
const btgatt_client_callbacks_t linuxbt_gattc_callbacks =
{
    linuxbt_gattc_register_client_callback,
    linuxbt_gattc_scan_result_callback,
    linuxbt_gattc_connect_callback,
    linuxbt_gattc_disconnect_callback,
    linuxbt_gattc_search_complete_callback,
    linuxbt_gattc_register_for_notification_callback,
    linuxbt_gattc_notify_callback,
    linuxbt_gattc_read_characteristic_callback,
    linuxbt_gattc_write_characteristic_callback,
    linuxbt_gattc_read_descriptor_callback,
    linuxbt_gattc_write_descriptor_callback,
    linuxbt_gattc_execute_write_callback,
    linuxbt_gattc_read_remote_rssi_callback,
    linuxbt_gattc_listen_callback,
    linuxbt_gattc_configure_mtu_callback,
    linuxbt_gattc_scan_filter_cfg_callback,
    linuxbt_gattc_scan_filter_param_callback,
    linuxbt_gattc_scan_filter_status_callback,
    linuxbt_gattc_multi_adv_enable_callback,
    linuxbt_gattc_multi_adv_update_callback,
    linuxbt_gattc_multi_adv_data_callback,
    linuxbt_gattc_multi_adv_disable_callback,
    linuxbt_gattc_congestion_callback,
    linuxbt_gattc_batchscan_cfg_storage_callback,
    linuxbt_gattc_batchscan_enable_disable_callback,
    linuxbt_gattc_batchscan_reports_callback,
    linuxbt_gattc_batchscan_threshold_callback,
    linuxbt_gattc_track_adv_event_callback,
    linuxbt_gattc_scan_parameter_setup_completed_callback,
    linuxbt_gattc_get_gatt_db_callback,
    linuxbt_gattc_services_removed_callback,
    linuxbt_gattc_services_added_callback,
};

#if defined(MTK_LINUX_GATT) && (MTK_LINUX_GATT == TRUE)
const btgatt_ex_client_callbacks_t linuxbt_gattc_ex_callbacks =
{
    linuxbt_gattc_get_adv_rpa_callback,
};
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
int linuxbt_gattc_init(const btgatt_client_interface_t *pt_interface)
{
    //FUNC_ENTRY;
    BT_ERR_STATUS_T ret = BT_SUCCESS;
    bt_uuid_t uuid;

    linuxbt_gattc_interface = pt_interface;
    linuxbt_uuid_stoh(LINUXBT_GATTC_APP_UUID, &uuid);
#if 0
    // old code for register client app in initialization
    if (linuxbt_gattc_interface->register_client(&uuid))
    {
        BT_DBG_INFO(BT_DEBUG_GATT,"[GATTC] Register client uuid:'%s'", LINUXBT_GATTC_APP_UUID);
    }
#endif

    return ret;
}

#if defined(MTK_LINUX_GATT) && (MTK_LINUX_GATT == TRUE)
int linuxbt_gattc_ex_init(const btgatt_ex_client_interface_t *pt_ex_interface)
{
    //FUNC_ENTRY;
    linuxbt_gattc_ex_interface = pt_ex_interface;
    return BT_SUCCESS;
}
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
int linuxbt_gattc_deinit(void)
{
    //FUNC_ENTRY;
    return BT_SUCCESS;
}



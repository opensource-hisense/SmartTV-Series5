/********************************************************************************************
 *     LEGAL DISCLAIMER
 *
 *     (Header of MediaTek Software/Firmware Release or Documentation)
 *
 *     BY OPENING OR USING THIS FILE, BUYER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND AGREES
 *     THAT THE SOFTWARE/FIRMWARE AND ITS DOCUMENTATIONS ("MEDIATEK SOFTWARE") RECEIVED
 *     FROM MEDIATEK AND/OR ITS REPRESENTATIVES ARE PROVIDED TO BUYER ON AN "AS-IS" BASIS
 *     ONLY. MEDIATEK EXPRESSLY DISCLAIMS ANY AND ALL WARRANTIES, EXPRESS OR IMPLIED,
 *     INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR
 *     A PARTICULAR PURPOSE OR NONINFRINGEMENT. NEITHER DOES MEDIATEK PROVIDE ANY WARRANTY
 *     WHATSOEVER WITH RESPECT TO THE SOFTWARE OF ANY THIRD PARTY WHICH MAY BE USED BY,
 *     INCORPORATED IN, OR SUPPLIED WITH THE MEDIATEK SOFTWARE, AND BUYER AGREES TO LOOK
 *     ONLY TO SUCH THIRD PARTY FOR ANY WARRANTY CLAIM RELATING THERETO. MEDIATEK SHALL ALSO
 *     NOT BE RESPONSIBLE FOR ANY MEDIATEK SOFTWARE RELEASES MADE TO BUYER'S SPECIFICATION
 *     OR TO CONFORM TO A PARTICULAR STANDARD OR OPEN FORUM.
 *
 *     BUYER'S SOLE AND EXCLUSIVE REMEDY AND MEDIATEK'S ENTIRE AND CUMULATIVE LIABILITY WITH
 *     RESPECT TO THE MEDIATEK SOFTWARE RELEASED HEREUNDER WILL BE, AT MEDIATEK'S OPTION,
 *     TO REVISE OR REPLACE THE MEDIATEK SOFTWARE AT ISSUE, OR REFUND ANY SOFTWARE LICENSE
 *     FEES OR SERVICE CHARGE PAID BY BUYER TO MEDIATEK FOR SUCH MEDIATEK SOFTWARE AT ISSUE.
 *
 *     THE TRANSACTION CONTEMPLATED HEREUNDER SHALL BE CONSTRUED IN ACCORDANCE WITH THE LAWS
 *     OF THE STATE OF CALIFORNIA, USA, EXCLUDING ITS CONFLICT OF LAWS PRINCIPLES.
 ************************************************************************************************/

#include "c_mw_config.h"
#include "bt_mw_common.h"
#include "bluetooth_sync.h"
#include "linuxbt_spp_if.h"

extern BtAppSppCbk      SppCbk;

/**
 * FUNCTION NAME: bt_spp_connect
 * PURPOSE:
 *      The function is used to SPP connect.
 * INPUT:
 *      addr -- the device address to connect.
 *      uuid -- the uuid to connect.
 * OUTPUT:
 *      None
 * RETURN:
 *      BT_SUCCESS           -- Operate success.
 *      BT_ERR_STATUS_FAIL   -- Operate fail.
 * NOTES:
 *      None
 */
INT32 bt_spp_connect(CHAR* addr, CHAR *uuid)
{
    FUNC_ENTRY;
    return linuxbt_spp_connect(addr, uuid);
}

/**
 * FUNCTION NAME: bt_spp_disconnect
 * PURPOSE:
 *      The function is used to SPP disconnect.
 * INPUT:
 *      addr -- the device address to disconnect.
 *      uuid -- the uuid to disconnect.
 * OUTPUT:
 *      None
 * RETURN:
 *      BT_SUCCESS           -- Operate success.
 *      BT_ERR_STATUS_FAIL   -- Operate fail.
 * NOTES:
 *      None
 */
INT32 bt_spp_disconnect(CHAR *addr, CHAR *uuid)
{
    FUNC_ENTRY;
    return linuxbt_spp_disconnect(addr, uuid);
}

/**
 * FUNCTION NAME: bt_spp_send_data
 * PURPOSE:
 *      The function is used to send data.
 * INPUT:
 *      bd_addr -- the device address to send.
 *      uuid -- the uuid to send.
 *      str -- the string to send.
 *      len -- the string length to send.
 * OUTPUT:
 *      None
 * RETURN:
 *      BT_SUCCESS           -- Operate success.
 *      BT_ERR_STATUS_FAIL   -- Operate fail.
 * NOTES:
 *      None
 */
INT32 bt_spp_send_data(CHAR* bd_addr, CHAR *uuid, CHAR* str, INT32 len)
{
    FUNC_ENTRY;
    linuxbt_spp_send_data(bd_addr, uuid, str, len);
/*    UINT8 port;
    INT32 i4_ret;
    INT16 spp_data_len = 0;
    INT16 len_count = 0;

    CHAR spp_data[SPP_ADP_UART_GET_BYTE_BUFFER_LENGTH];

    if (NULL == str || NULL == uuid)
    {
        BT_DBG_ERROR(BT_DEBUG_SPP, "null pointer of str or uuid");
        return BT_ERR_STATUS_PARM_INVALID;
    }
    i4_ret = bt_spp_get_port(uuid, &port);
    if (0 == i4_ret)
    {
        spp_data_len = len;
        while (0 < spp_data_len)
        {
            memset(spp_data, 0, sizeof(spp_data));
            if (127 < spp_data_len)
            {
                spp_data_len -= SPP_ADP_UART_GET_BYTE_BUFFER_LENGTH-1;
                memcpy(spp_data, str+len_count, SPP_ADP_UART_GET_BYTE_BUFFER_LENGTH-1);
                len_count += SPP_ADP_UART_GET_BYTE_BUFFER_LENGTH-1;
                linuxbt_spp_send_data(spp_data, SPP_ADP_UART_GET_BYTE_BUFFER_LENGTH-1, port);
                BT_DBG_NOTICE(BT_DEBUG_SPP, "spp_data:%s",spp_data);
                BT_DBG_NOTICE(BT_DEBUG_SPP, "spp_data_len:%d",SPP_ADP_UART_GET_BYTE_BUFFER_LENGTH-1);
            }
            else
            {
                memcpy(spp_data, str+len_count, spp_data_len);
                linuxbt_spp_send_data(spp_data, spp_data_len, port);
                spp_data[SPP_ADP_UART_GET_BYTE_BUFFER_LENGTH-1] = '\0';
                BT_DBG_NOTICE(BT_DEBUG_SPP, "spp_data:%s",spp_data);
                BT_DBG_NOTICE(BT_DEBUG_SPP, "spp_data_len:%d",spp_data_len);
                break;
            }
        }
    }
    else
    {
        BT_DBG_NORMAL(BT_DEBUG_COM,"get port error");
        return BT_ERR_STATUS_FAIL;
    }

    */
    return BT_SUCCESS;
}

/**
 * FUNCTION NAME: bt_spp_receive_data
 * PURPOSE:
 *      The function is used to receive data.
 * INPUT:
 *      bd_addr -- the data to receive from which device.
 *      uuid -- the data to receive from which uuid.
 *      uuid_len -- the uuid length.
 *      buffer -- the data to receive.
 *      length -- the data length to receive.
 * OUTPUT:
 *      None
 * RETURN:
 *      BT_SUCCESS           -- Operate success.
 *      BT_ERR_STATUS_FAIL   -- Operate fail.
 * NOTES:
 *      None
 */
INT32 bt_spp_receive_data(CHAR *bd_addr, CHAR *uuid, INT32 uuid_len, CHAR *buffer, INT16 length)
{
    FUNC_ENTRY;

    CHAR spp_data[SPP_ADP_UART_GET_BYTE_BUFFER_LENGTH];
    BT_SPP_CBK_STRUCT t_spp_struct_data;

    if (NULL == buffer)
    {
        BT_DBG_ERROR(BT_DEBUG_SPP, "null pointer of buffer");
        return BT_ERR_STATUS_PARM_INVALID;
    }

    memset(spp_data, 0, sizeof(spp_data));
    memset(&t_spp_struct_data, 0, sizeof(BT_SPP_CBK_STRUCT));

    strncpy(t_spp_struct_data.bd_addr, bd_addr, MAX_BDADDR_LEN);

    if (length <= SPP_ADP_UART_GET_BYTE_BUFFER_LENGTH)
    {
        t_spp_struct_data.spp_data_len = length;
        memcpy(spp_data, buffer, length);
    }
    else
    {
        t_spp_struct_data.spp_data_len = SPP_ADP_UART_GET_BYTE_BUFFER_LENGTH;
        memcpy(spp_data, buffer, SPP_ADP_UART_GET_BYTE_BUFFER_LENGTH);
    }

    t_spp_struct_data.spp_data = spp_data;
    t_spp_struct_data.uuid = uuid;
    t_spp_struct_data.uuid_len = uuid_len;
    t_spp_struct_data.bt_spp_event_type = BT_SPP_RECV_DATA;
    BT_DBG_NOTICE(BT_DEBUG_SPP, "bd_addr is: %s", t_spp_struct_data.bd_addr);
    BT_DBG_NOTICE(BT_DEBUG_SPP, "uuid is: %s", t_spp_struct_data.uuid);
    BT_DBG_NOTICE(BT_DEBUG_SPP, "uuid_len is: %d", t_spp_struct_data.uuid_len);
    BT_DBG_NOTICE(BT_DEBUG_SPP, "spp_data is:");
    if(t_spp_struct_data.spp_data_len > 0)
    {
        BT_DBG_NOTICE(BT_DEBUG_SPP, "Received Data:%s", t_spp_struct_data.spp_data);
    }

    BT_DBG_NOTICE(BT_DEBUG_SPP, "");
    BT_DBG_NOTICE(BT_DEBUG_SPP, "include spp_data_len is: %d", t_spp_struct_data.spp_data_len);

    if (NULL != SppCbk)
    {
        SppCbk(&t_spp_struct_data);
    }
    else
    {
        BT_DBG_ERROR(BT_DEBUG_SPP, "SppCbk NULL");
    }

    return BT_SUCCESS;
}

/**
 * FUNCTION NAME: bt_spp_event_cb
 * PURPOSE:
 *      The function is used to confirm spp event, eg. connect/disocnnect.
 * INPUT:
 *      bd_addr -- the event from which device.
 *      uuid -- the event from which uuid.
 *      uuid_len -- the uuid length.
 *      event -- the event type.
 * OUTPUT:
 *      None
 * RETURN:
 *      BT_SUCCESS           -- Operate success.
 *      BT_ERR_STATUS_FAIL   -- Operate fail.
 * NOTES:
 *      None
 */
INT32 bt_spp_event_cb(CHAR *bd_addr, CHAR *uuid, INT32 uuid_len, INT32 event)
{
    FUNC_ENTRY;
    BT_SPP_CBK_STRUCT t_spp_struct_data;
    memset(&t_spp_struct_data, 0, sizeof(BT_SPP_CBK_STRUCT));
    strncpy(t_spp_struct_data.bd_addr, bd_addr, MAX_BDADDR_LEN);
    t_spp_struct_data.uuid = uuid;
    t_spp_struct_data.uuid_len = uuid_len;
    t_spp_struct_data.bt_spp_event_type = event;
    BT_DBG_NOTICE(BT_DEBUG_SPP, "bd_addr is: %s", t_spp_struct_data.bd_addr);
    BT_DBG_NOTICE(BT_DEBUG_SPP, "uuid is: %s", t_spp_struct_data.uuid);
    BT_DBG_NOTICE(BT_DEBUG_SPP, "uuid_len is: %d", t_spp_struct_data.uuid_len);
    BT_DBG_NOTICE(BT_DEBUG_SPP, "event is: %d", t_spp_struct_data.bt_spp_event_type);

    if (NULL != SppCbk)
    {
        SppCbk(&t_spp_struct_data);
    }
    else
    {
        BT_DBG_ERROR(BT_DEBUG_SPP, "SppCbk NULL");
    }

    return BT_SUCCESS;
}

/**
 * FUNCTION NAME: bt_spp_enable_devB
 * PURPOSE:
 *      The function is used to enable deviceB, the platform is deviceB.
 * INPUT:
 *      servername -- the service name.
 *      uuid -- the uuid to connect/send/disconnect.
 * OUTPUT:
 *      None
 * RETURN:
 *      BT_SUCCESS           -- Operate success.
 *      BT_ERR_STATUS_FAIL   -- Operate fail.
 * NOTES:
 *      None
 */
INT32 bt_spp_enable_devB(CHAR *servername, CHAR *uuid)
{
    FUNC_ENTRY;

    if (NULL == servername)
    {
        BT_DBG_ERROR(BT_DEBUG_SPP, "null pointer of paddr");
        return BT_ERR_STATUS_PARM_INVALID;
    }

    if (NULL == uuid)
    {
        CHAR *uuid_128 = "00001101-0000-1000-8000-00805F9B34FB";
        uuid = uuid_128;
        BT_DBG_NOTICE(BT_DEBUG_SPP, "Using default UUID:%s", uuid);
    }

    linuxbt_spp_activate(servername, uuid);
    return BT_SUCCESS;
}

/**
 * FUNCTION NAME: bt_spp_disable_devB
 * PURPOSE:
 *      The function is used to disable deviceB, the platform is deviceB.
 * INPUT:
 *      uuid -- the uuid to disable.
 * OUTPUT:
 *      None
 * RETURN:
 *      BT_SUCCESS           -- Operate success.
 *      BT_ERR_STATUS_FAIL   -- Operate fail.
 * NOTES:
 *      None
 */
INT32 bt_spp_disable_devB(CHAR *uuid)
{
    FUNC_ENTRY;

    if (NULL == uuid)
    {
        CHAR *uuid_128 = "00001101-0000-1000-8000-00805F9B34FB";
        uuid = uuid_128;
        BT_DBG_NOTICE(BT_DEBUG_SPP, "Using default UUID:%s", uuid);
    }

    BT_DBG_NOTICE(BT_DEBUG_SPP, "Using Input UUID:%s", uuid);

    return linuxbt_spp_deactivate(uuid);
}


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

#ifndef _BT_MW_SPP_H_
#define _BT_MW_SPP_H_

#include "u_bluetooth.h"

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
INT32 bt_spp_connect(CHAR* addr, CHAR *uuid);

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
INT32 bt_spp_disconnect(CHAR *addr, CHAR *uuid);

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
INT32 bt_spp_send_data(CHAR* bd_addr, CHAR *uuid, CHAR* str, INT32 len);

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
INT32 bt_spp_receive_data(CHAR *bd_addr, CHAR *uuid, INT32 uuid_len, CHAR *buffer, INT16 length);

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
INT32 bt_spp_event_cb(CHAR *bd_addr, CHAR *uuid, INT32 uuid_len, INT32 event);

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
INT32 bt_spp_enable_devB(CHAR *servername, CHAR *uuid);

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
INT32 bt_spp_disable_devB(CHAR *uuid);

#endif /*  _BT_MW_SPP_H_ */

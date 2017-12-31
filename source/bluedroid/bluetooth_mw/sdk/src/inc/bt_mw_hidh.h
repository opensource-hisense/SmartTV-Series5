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

/* FILE NAME:  bt_mw_hid.h
 * AUTHOR: zwei chen
 * PURPOSE:
 *      It provides hid  interface to c_bt_mw_hid.c.
 * NOTES:
 */

#ifndef __BT_MW_HID_H__
#define __BT_MW_HID_H__

#include "bluetooth.h"

#if 0
typedef struct _BT_HOGP_Status
{
    UINT8              status;/*0-->idle; 1 -> connecting ; 2--> connected*/
    CHAR               bdAddr[MAX_BDADDR_LEN];/* Bluetooth Address */
}BT_HOGP_STATUS;
#endif
/****************************************************
 * FUNCTION NAME: bluetooth_hid_activate
 * PURPOSE:
 *      The function is used to activate hid
 * INPUT:
 *      None
 *
 * OUTPUT:
 *      None
 * RETURN:
 *      INT32:error code
 * NOTES:
 *      None
 */
//INT32 bluetooth_hid_activate(VOID);

/****************************************************
 * FUNCTION NAME: bluetooth_hid_deactivate
 * PURPOSE:
 *      The function is used to deactivate hid
 * INPUT:
 *      None
 *
 * OUTPUT:
 *      None
 * RETURN:
 *      INT32:error code
 * NOTES:
 *      None
 */
//INT32 bluetooth_hid_deactivate(VOID);

/****************************************************
 * FUNCTION NAME: bluetooth_hid_init
 * PURPOSE:
 *      The function is used to init hid profile
 * INPUT:
 *      None
 *
 * OUTPUT:
 *      None
 * RETURN:
 *      INT32:error code
 * NOTES:
 *      None
 */
INT32 bluetooth_hid_init(VOID);

/****************************************************
 * FUNCTION NAME: bluetooth_hid_deinit
 * PURPOSE:
 *      The function is used to deinit hid profile
 * INPUT:
 *      None
 *
 * OUTPUT:
 *      None
 * RETURN:
 *      INT32:error code
 * NOTES:
 *      None
 */
INT32 bluetooth_hid_deinit(VOID);

/****************************************************
 * FUNCTION NAME: bluetooth_hid_connect
 * PURPOSE:
 *      The function is used to connect  hid device
 * INPUT:
 *      pbt_addr:device address
 *
 * OUTPUT:
 *      None
 * RETURN:
 *      INT32:error code
 * NOTES:
 *      None
 */
INT32 bluetooth_hid_connect(CHAR *pbt_addr);

/****************************************************
 * FUNCTION NAME: bluetooth_hid_disconnect
 * PURPOSE:
 *      The function is used to disconnect  hid device
 * INPUT:
 *      pbt_addr:device address
 *
 * OUTPUT:
 *      None
 * RETURN:
 *      INT32:error code
 * NOTES:
 *      None
 */
INT32 bluetooth_hid_disconnect(CHAR *pbt_addr);

/****************************************************
 * FUNCTION NAME: bluetooth_hid_disconnect_sync
 * PURPOSE:
 *      The function is used to disconnect hid device
 * INPUT:
 *      no input
 *
 * OUTPUT:
 *      None
 * RETURN:
 *      INT32:error code
 * NOTES:
 *      None
 */

INT32 bluetooth_hid_disconnect_sync();

/****************************************************
 * FUNCTION NAME: bluetooth_hid_set_output_report
 * PURPOSE:
 *      The function is used to set output report
 * INPUT:
 *      pbt_addr:device address
 *      preport_data: output report data
 * OUTPUT:
 *      None
 * RETURN:
 *      INT32:error code
 * NOTES:
 *      None
 */
INT32 bluetooth_hid_set_output_report(char *pbt_addr, char *preport_data);

/****************************************************
 * FUNCTION NAME: bluetooth_hid_get_output_report
 * PURPOSE:
 *      The function is used to get output report
 * INPUT:
 *      pbt_addr:device address
 *      reportId: report id
 *     bufferSize : size of the buffer for output data
 * OUTPUT:
 *      None
 * RETURN:
 *      INT32:error code
 * NOTES:
 *      None
 */
INT32 bluetooth_hid_get_output_report(char *pbt_addr, int reportId, int bufferSize);


/****************************************************
 * FUNCTION NAME: bluetooth_hid_set_input_report
 * PURPOSE:
 *      The function is used to set input report
 * INPUT:
 *      pbt_addr:device address
 *      preport_data: input report data
 *
 * OUTPUT:
 *      None
 * RETURN:
 *      INT32:error code
 * NOTES:
 *      None
 */
INT32 bluetooth_hid_set_input_report(char *pbt_addr, char *preport_data);

/****************************************************
 * FUNCTION NAME: bluetooth_hid_get_input_report
 * PURPOSE:
 *      The function is used to get input report
 * INPUT:
 *      pbt_addr:device address
 *      reportId: report id
 *     bufferSize : size of the buffer for input data
 * OUTPUT:
 *      None
 * RETURN:
 *      INT32:error code
 * NOTES:
 *      None
 */
INT32 bluetooth_hid_get_input_report(char *pbt_addr, int reportId, int bufferSize);

/****************************************************
 * FUNCTION NAME: linuxbt_hid_get_feature_report_handler
 * PURPOSE:
 *      The function is used to get feature report
 * INPUT:
 *      pbt_addr:device address
 *      reportId: report id
 *     bufferSize : size of the buffer for feature data
 * OUTPUT:
 *      None
 * RETURN:
 *      INT32:error code
 * NOTES:
 *      None
 */
INT32 bluetooth_hid_get_feature_report(char *pbt_addr, int reportId, int bufferSize);

/****************************************************
 * FUNCTION NAME: bluetooth_hid_set_feature_report
 * PURPOSE:
 *      The function is used to set feature report
 * INPUT:
 *      pbt_addr:device address
 *      preport_data: input report data
 *
 * OUTPUT:
 *      None
 * RETURN:
 *      INT32:error code
 * NOTES:
 *      None
 */
INT32 bluetooth_hid_set_feature_report(char *pbt_addr, char *preport_data);

/****************************************************
 * FUNCTION NAME: btaudio_get_current_hid_target_dev_info
 * PURPOSE:
 *      The function is used to get curent hid device info
 * INPUT:
 *      None
 *
 *
 * OUTPUT:
 *      pt_target_dev_info: device info (address and name)
 * RETURN:
 *      INT32:error code
 * NOTES:
 *      None
 */
INT32 btaudio_get_current_hid_target_dev_info(BT_TARGET_DEV_INFO *pt_target_dev_info);

/****************************************************
 * FUNCTION NAME: bluetooth_hid_get_protocol
 * PURPOSE:
 *      The function is used to get  protocol
 * INPUT:
 *      pbt_addr:device address
 *
 *
 * OUTPUT:
 *      None
 * RETURN:
 *      INT32:error code
 * NOTES:
 *      None
 */
INT32 bluetooth_hid_get_protocol(char *pbt_addr);


/****************************************************
 * FUNCTION NAME: bluetooth_hid_set_protocol
 * PURPOSE:
 *      The function is used to set  protocol
 * INPUT:
 *      pbt_addr:device address
 *      protocol_mode: protocol mode:boot or report mode
 *
 * OUTPUT:
 *      None
 * RETURN:
 *      INT32:error code
 * NOTES:
 *      None
 */
INT32 bluetooth_hid_set_protocol(char *pbt_addr, int protocol_mode);

INT32 bluetooth_hid_virtual_unplug(char *pbt_addr);
INT32 bluetooth_hid_send_data(char *pbt_addr, char *data);
/****************************************************
 * FUNCTION NAME: bluetooth_hid_send_control
 * PURPOSE:
 *      The function is used to suspend or exit suspend mode
 * INPUT:
 *      pbt_addr:device address
 *      pcontrol_mode: suspend or exit suspend mode
 *
 * OUTPUT:
 *      None
 * RETURN:
 *      INT32:error code
 * NOTES:
 *      None
 */
//INT32 bluetooth_hid_send_control(char *pbt_addr, int pcontrol_mode);

/****************************************************
 * FUNCTION NAME: bluetooth_handle_hid_connect_cb
 * PURPOSE:
 *      The function is used for callback for connected
 * INPUT:
 *      bd_addr:device address
 *
 *
 * OUTPUT:
 *      None
 * RETURN:
 *      INT32:error code
 * NOTES:
 *      None
 */
INT32 bluetooth_handle_hid_connect_cb(bt_bdaddr_t *bd_addr);


/****************************************************
 * FUNCTION NAME: bluetooth_handle_hid_connect_fail_cb
 * PURPOSE:
 *      The function is used for callback for connect failed
 * INPUT:
 *      bd_addr: device address
 *      event: HID connected or disconnected related event
 *
 * OUTPUT:
 *      None
 * RETURN:
 *      INT32:error code
 * NOTES:
 *      None
 */
INT32 bluetooth_handle_hid_connect_fail_cb(bt_bdaddr_t *bd_addr, BT_HID_EVENT event);

/****************************************************
 * FUNCTION NAME: bluetooth_handle_hid_disconnect_cb
 * PURPOSE:
 *      The function is used for callback for disconnected
 * INPUT:
 *      bd_addr:device address
 *
 *
 * OUTPUT:
 *      None
 * RETURN:
 *      INT32:error code
 * NOTES:
 *      None
 */
INT32 bluetooth_handle_hid_disconnect_cb(bt_bdaddr_t *bd_addr);

#if 0
INT32 get_hogp_connect_state(CHAR *pbt_add);


void set_hogp_connect_state(UINT8 status,CHAR *pbt_add);
#endif

#endif /*__BT_MW_HID_H__*/

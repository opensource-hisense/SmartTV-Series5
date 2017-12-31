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
/* FILE NAME:  bt_mw_devices_info.h
 * PURPOSE:
 *  {1. What is covered in this file - function and scope.}
 *  {2. Related documents or hardware information}
 * NOTES:
 *  {Something must be known or noticed}
 *  {1. How to use these functions - Give an example.}
 *  {2. Sequence of messages if applicable.}
 *  {3. Any design limitation}
 *  {4. Any performance limitation}
 *  {5. Is it a reusable component}
 *
 *
 *
 */
#ifndef __BT_MW_DEVICES_INFO_H__
#define __BT_MW_DEVICES_INFO_H__
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>

#include "bluetooth.h"
#include "bt_mw_common.h"

/* this API get local device info at real time */
INT32 bluetooth_update_local_dev_info(VOID);
BOOL bluetooth_scan_device_info_ready(BT_DEV_MAPPING_INFO detected_map_info);
INT32 bluetooth_get_notify_mapping_name(BT_DEV_MAPPING_INFO *pmapping_dev, BT_DEV_MAPPING_LIST p_target_dev_info);
INT32 bluetooth_get_dev_mapping_name(BT_DEV_MAPPING_INFO *pmapping_dev);
INT32 bluetooth_gap_update_mapping_info(BT_DEV_MAPPING_INFO *pmapping_dev, BT_DEV_MAPPING_LIST *pall_target_dev_info);
INT32 bluetooth_gap_update_target_get_mapping_info(bt_bdaddr_t *bd_addr, INT32 num_properties, bt_property_t *properties);
INT32 bluetooth_gap_get_mapping_info(bt_bdaddr_t *bd_addr, INT32 num_properties, bt_property_t *properties, BT_DEV_MAPPING_LIST *target_device_list);
INT32 bluetooth_notify_device_info_app(bt_bdaddr_t *btaddr, BT_DEV_MAPPING_LIST *pall_target_dev_info);
INT32 bluetooth_notify_every_device_info_app(bt_bdaddr_t *btaddr, BT_DEV_MAPPING_LIST *pall_target_dev_info);
INT32 print_history_info(BT_TARGET_DEV_LIST * pt_device_history);
INT32 print_hid_history_info(BT_HID_TARGET_DEV_LIST *pt_device_history);
INT32 print_hid_status_info(BT_HID_STATUS_STRUCT_LIST *pt_device_history);
INT32 bluetooth_get_device_info(BT_TARGET_DEV_LIST *dev_list_info);
INT32 bluetooth_get_device_history(BT_TARGET_DEV_LIST *dev_history, BT_TARGET_DEV_LIST *pt_paired_dev_history);
INT32 bluetooth_get_hid_device_history(BT_HID_TARGET_DEV_LIST *dev_history, BT_HID_TARGET_DEV_LIST *pt_paired_dev_history);
INT32 bluetooth_get_num_of_history(UINT32 *u4_dev_num, BT_TARGET_DEV_LIST *pt_paired_dev_history);
VOID bluetooth_clear_device_info(VOID);
INT32 bluetooth_ReadHistoryFromFile(CHAR *filename, BT_TARGET_DEV_LIST *pt_paired_dev_history);
INT32 bluetooth_hid_ReadHistoryFromFile(CHAR *filename, BT_HID_TARGET_DEV_LIST *pt_paired_dev_history);
BT_TARGET_DEV_LIST* bluetooth_getHistoryList(BOOL fg_src_mode);
BT_HID_TARGET_DEV_LIST* bluetooth_getHIDHistoryList(VOID);
INT32 bluetooth_WriteHistoryToFile(BT_TARGET_DEV_LIST *pt_device_history, CHAR *pfilename);
INT32 bluetooth_hid_WriteHistoryToFile(BT_HID_TARGET_DEV_LIST *pt_device_history, CHAR *pfilename);
VOID bluetooth_setHistory(BT_TARGET_DEV_INFO *history, BT_TARGET_DEV_LIST *pt_paired_dev_history);
INT32 bluetooth_remove_av_history_one(CHAR* keyword, BT_TARGET_DEV_LIST *pt_paired_dev_history);
INT32 bluetooth_remove_av_history_all(VOID);
INT32 bluetooth_hid_removeHistory_one(CHAR* keyword, BT_HID_TARGET_DEV_LIST *pt_paired_dev_history);
INT32 bluetooth_hid_removeStatus_one(CHAR* keyword, BT_HID_STATUS_STRUCT_LIST *pt_paired_dev_history);
INT32 bluetooth_hid_removeHistory_all(VOID);
INT32 bluetooth_hid_removeStatus_all(VOID);
INT32 bluetooth_set_device_status(char *bdaddr, BT_STATUS status);
VOID bluetooth_hid_setHistory(BT_TARGET_DEV_INFO *history, BT_HID_TARGET_DEV_LIST *pt_paired_dev_history);
VOID bluetooth_hid_setStatus(BT_HID_STATUS_STRUCT *history, BT_HID_STATUS_STRUCT_LIST *pt_paired_dev_history);
INT32 bluetooth_hid_getStatus(CHAR* bdaddr, BT_STATUS *status);
INT32 bluetooth_ReadLocalInfoFromFile(CHAR *filename);
INT32 bluetooth_WriteLocalInfoToFile(BT_GAP_LOCAL_PROPERTIES_RESULT *pt_local_info);
INT32 bluetooth_init_local_dev_info (VOID);
BOOL bluetooth_local_dev_is_init(VOID);
VOID bluetooth_check_default_folder(VOID);
INT32 bluetooth_remove_av_history(BT_TARGET_DEV_LIST *pt_paired_dev_history);
INT32 bluetooth_hid_removeHistory_except_rc(CHAR* keyword, BT_HID_TARGET_DEV_LIST *pt_paired_dev_history);
INT32 bluetooth_get_namebyaddr(BT_TARGET_DEV_INFO *pt_dev_info, BT_DEV_LIST_IN_NAMES *pt_device_list);
INT32 bluetooth_get_local_dev_name(CHAR *name);
//app use this API
INT32 bluetooth_get_local_dev_info( BT_GAP_LOCAL_PROPERTIES_RESULT* ps_dev_info);
INT32 _bt_update_dev_history(BT_TARGET_DEV_INFO *pt_dev_info, BT_TARGET_DEV_LIST *pt_paired_dev_history);

#endif /* End of __BT_MW_DEVICES_INFO_H__ */


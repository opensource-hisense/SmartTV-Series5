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
 *     FEES OR SERVICE charGE PAID BY BUYER TO MEDIATEK FOR SUCH MEDIATEK SOFTWARE AT ISSUE.
 *
 *     THE TRANSACTION CONTEMPLATED HEREUNDER SHALL BE CONSTRUED IN ACCORDANCE WITH THE LAWS
 *     OF THE STATE OF CALIFORNIA, USA, EXCLUDING ITS CONFLICT OF LAWS PRINCIPLES.
 ************************************************************************************************/

#ifndef _MTK_BT_SERVICE_HIDH_WRAPPER_H_
#define _MTK_BT_SERVICE_HIDH_WRAPPER_H_

#include "u_rpcipc_types.h"
#include "u_bluetooth.h"


#ifdef  __cplusplus
extern "C" {
#endif


extern INT32 a_mtkapi_hidh_init(VOID);
extern INT32 a_mtkapi_hidh_deinit(VOID);
extern INT32 a_mtkapi_hidh_connect(CHAR *pbt_addr);
extern INT32 a_mtkapi_hidh_disconnect(CHAR *pbt_addr);
extern INT32 a_mtkapi_hidh_set_output_report(CHAR *pbt_addr, CHAR *preport_data);
extern INT32 a_mtkapi_hidh_get_input_report(CHAR *pbt_addr, UINT8 reportId, INT32 bufferSize);
extern INT32 a_mtkapi_hidh_get_output_report(CHAR *pbt_addr, UINT8 reportId, INT32 bufferSize);
extern INT32 a_mtkapi_hidh_set_input_report(CHAR *pbt_addr, CHAR *preport_data);
extern INT32 a_mtkapi_hidh_get_feature_report(CHAR *pbt_addr, UINT8 reportId, INT32 bufferSize);
extern INT32 a_mtkapi_hidh_set_feature_report(CHAR *pbt_addr, CHAR *preport_data);
extern INT32 a_mtkapi_hidh_get_protocol(CHAR *pbt_addr);
extern INT32 a_mtkapi_hidh_set_protocol(CHAR *pbt_addr, UINT8 protocol_mode);
extern INT32 a_mtkapi_hidh_send_control(CHAR *pbt_addr, UINT8 pcontrol_mode);
extern INT32 a_mtkapi_bluetooth_hidh_set_output_report(CHAR *pbt_addr, CHAR *preport_data);
extern INT32 a_mtkapi_get_paired_hidh_dev_list(BT_HID_TARGET_DEV_LIST *pt_device_list, BT_HID_STATUS_STRUCT_LIST *pt_hid_status_list);
extern INT32 a_mtkapi_del_paired_hidh_dev_one(BT_TARGET_DEV_INFO *pt_device_info);
extern INT32 a_mtkapi_del_paired_hidh_dev_all(VOID);
extern INT32 a_mtkapi_connect_hidh_sources(CHAR *addr);
extern INT32 a_mtkapi_disconnect_hidh_sources(CHAR *addr);
extern INT32 a_mtkapi_hidh_auto_disconnection(VOID);


#ifdef  __cplusplus
}
#endif
#endif



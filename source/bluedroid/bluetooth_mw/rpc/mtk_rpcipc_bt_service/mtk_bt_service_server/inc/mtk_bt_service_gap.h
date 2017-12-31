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

#ifndef _MTK_BT_SERVICE_GAP_H_
#define _MTK_BT_SERVICE_GAP_H_

#include "mtk_bt_service_gap_wrapper.h"

INT32 x_mtkapi_bt_base_init(MTKRPCAPI_BT_APP_CB_FUNC* func, void *pv_tag);

INT32 x_mtkapi_bt_gap_set_dbg_level(BT_DEBUG_LAYER_NAME_T layer, INT32 level);
INT32 x_mtkapi_bt_gap_btstart(const CHAR *ps_mode, UINT32 u4_overtime);
INT32 x_mtkapi_bt_gap_get_mode();
INT32 x_mtkapi_bt_gap_on_off(BOOL fg_on);
INT32 x_mtkapi_bt_gap_connecting(CHAR* ps_mac);
INT32 x_mtkapi_bt_gap_stop(BOOL fg_keep_conn, BOOL fg_discard_data);
INT32 x_mtkapi_bt_gap_clear_dev_info();
INT32 x_mtkapi_bt_bluetooth_factory_reset(CHAR *addr);
INT32 x_mtkapi_bt_gap_del_paired_av_dev_one(BT_TARGET_DEV_INFO *pt_device_info);
INT32 x_mtkapi_bt_gap_del_paired_av_dev_all();
INT32 x_mtkapi_bt_gap_auto_disconnection();
INT32 x_mtkapi_bt_gap_save_device_history();
INT32 x_mtkapi_bt_gap_set_name(CHAR *name);
INT32 x_mtkapi_bt_gap_set_connectable_and_discoverable(BOOL fg_conn, BOOL fg_disc);
INT32 x_mtkapi_bt_gap_get_paired_device_info();
INT32 x_mtkapi_bt_gap_get_local_dev_info(BT_GAP_LOCAL_PROPERTIES_RESULT *ps_dev_info);
INT32 x_mtkapi_bt_gap_get_target_info(BT_TARGET_DEV_INFO *pt_target_dev_info);
INT32 x_mtkapi_bt_gap_inquiry_one(CHAR *ps_dev_mac, UINT32 ui4_inqDuration);
INT32 x_mtkapi_bt_gap_start_inquiry_scan(UINT32 ui4_filter_type, BtAppGapInquiryResponseCbk pf_fct);
INT32 x_mtkapi_bt_gap_stop_inquiry_scan();
INT32 x_mtkapi_bt_gap_pair(CHAR *addr, int transport);

INT32 x_mtkapi_bt_gap_paired_dev_erase(CHAR *addr);
INT32 x_mtkapi_bt_gap_get_rssi(CHAR *address, INT16 *rssi_value);
INT32 x_mtkapi_bt_gap_set_virtual_sniffer(INT32 enable);
INT32 x_mtkapi_bt_gap_send_hci(CHAR *buffer);

#endif

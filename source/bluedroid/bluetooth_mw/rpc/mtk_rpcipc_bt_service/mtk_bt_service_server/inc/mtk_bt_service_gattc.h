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

#ifndef _MTK_BT_SERVICE_GATTC_H_
#define _MTK_BT_SERVICE_GATTC_H_

#include "mtk_bt_service_gattc_wrapper.h"

INT32 x_mtkapi_bt_gattc_register_app(char * app_uuid);
INT32 x_mtkapi_bt_gattc_unregister_app(INT32 client_if);
INT32 x_mtkapi_bt_gattc_scan();
INT32 x_mtkapi_bt_gattc_stop_scan();
INT32 x_mtkapi_bt_gattc_open(INT32 client_if, CHAR *bt_addr, UINT8 is_direct, INT32 transport);
INT32 x_mtkapi_bt_gattc_close(INT32 client_if, CHAR *bt_addr, INT32 conn_id);
INT32 x_mtkapi_bt_gattc_listen(INT32 client_if);
INT32 x_mtkapi_bt_gattc_refresh(INT32 client_if, CHAR *bt_addr);
INT32 x_mtkapi_bt_gattc_search_service(INT32 conn_id, CHAR *uuid);
INT32 x_mtkapi_bt_gattc_get_gatt_db(INT32 conn_id);
INT32 x_mtkapi_bt_gattc_read_char(INT32 conn_id, INT32 char_handle, INT32 auth_req);
INT32 x_mtkapi_bt_gattc_read_descr(INT32 conn_id, INT32 descr_handle, INT32 auth_req);
INT32 x_mtkapi_bt_gattc_write_char(INT32 conn_id, INT32 char_handle,
                                        INT32 write_type, INT32 len,
                                        INT32 auth_req, CHAR *value);
INT32 x_mtkapi_bt_gattc_write_descr(INT32 conn_id, INT32 descr_handle,
                                         INT32 write_type, INT32 len,
                                         INT32 auth_req, CHAR *value);
INT32 x_mtkapi_bt_gattc_execute_write(INT32 conn_id, INT32 execute);
INT32 x_mtkapi_bt_gattc_reg_noti(INT32 client_if, CHAR *bt_addr, INT32 char_handle);
INT32 x_mtkapi_bt_gattc_dereg_noti(INT32 client_if, CHAR *bt_addr, INT32 char_handle);
INT32 x_mtkapi_bt_gattc_read_rssi(INT32 client_if, CHAR *bt_addr);
INT32 x_mtkapi_bt_gattc_scan_filter_param_setup(bluetooth_gatt_filt_param_setup_t *scan_filt_param);
INT32 x_mtkapi_bt_gattc_scan_filter_enable(INT32 client_if);
INT32 x_mtkapi_bt_gattc_scan_filter_disable(INT32 client_if);
INT32 x_mtkapi_bt_gattc_scan_filter_add_remove(INT32 client_if, INT32 action,
                                                      INT32 filt_type, INT32 filt_index, INT32 company_id,
                                                      INT32 company_id_mask, const CHAR *p_uuid,
                                                      const CHAR *p_uuid_mask, const CHAR *bd_addr,
                                                      CHAR addr_type, INT32 data_len, CHAR *p_data, INT32 mask_len,
                                                      CHAR *p_mask);
INT32 x_mtkapi_bt_gattc_scan_filter_clear(INT32 client_if, INT32 filt_index);
INT32 x_mtkapi_bt_gattc_get_device_type(CHAR *bd_addr);
INT32 x_mtkapi_bt_gattc_configure_mtu(INT32 conn_id, INT32 mtu);
INT32 x_mtkapi_bt_gattc_conn_parameter_update(CHAR *bt_addr, INT32 min_interval, INT32 max_interval,
                                                       INT32 latency, INT32 timeout);
INT32 x_mtkapi_bt_gattc_set_scan_parameters(INT32 client_if, INT32 scan_interval, INT32 scan_window);
INT32 x_mtkapi_bt_gattc_multi_adv_enable(INT32 client_if, INT32 min_interval,
                                               INT32 max_interval, INT32 adv_type,
                                               INT32 chnl_map, INT32 tx_power, INT32 timeout_s);
INT32 x_mtkapi_bt_gattc_set_disc_mode(INT32 client_if, INT32 disc_mode);

INT32 x_mtkapi_bt_gattc_multi_adv_update(INT32 client_if, INT32 min_interval,
                                               INT32 max_interval, INT32 adv_type,
                                               INT32 chnl_map, INT32 tx_power, INT32 timeout_s);
INT32 x_mtkapi_bt_gattc_multi_adv_setdata(INT32 client_if,
                                                UINT8 set_scan_rsp,
                                                UINT8 include_name,
                                                UINT8 include_txpower,
                                                INT32 appearance,
                                                INT32 manufacturer_len,
                                                CHAR* manufacturer_data,
                                                INT32 service_data_len,
                                                CHAR* service_data,
                                                INT32 service_uuid_len,
                                                CHAR* service_uuid);
INT32 x_mtkapi_bt_gattc_multi_adv_disable(INT32 client_if);
INT32 x_mtkapi_bt_gattc_batchscan_cfg_storage(INT32 client_if, INT32 batch_scan_full_max,
                                                      INT32 batch_scan_trunc_max, INT32 batch_scan_notify_threshold);
INT32 x_mtkapi_bt_gattc_batchscan_enb_batch_scan(INT32 client_if, INT32 scan_mode,
                        INT32 scan_interval, INT32 scan_window, INT32 addr_type, INT32 discard_rule);
INT32 x_mtkapi_bt_gattc_batchscan_dis_batch_scan(INT32 client_if);
INT32 x_mtkapi_bt_gattc_batchscan_read_reports(INT32 client_if, INT32 scan_mode);

#if MTK_LINUX_GATTC_LE_NAME
INT32 x_mtkapi_bt_gattc_set_local_le_name(INT32 client_if, CHAR *le_name);
#endif
VOID x_mtkapi_bt_gattc_get_connect_result_info(BT_GATTC_CONNECT_RST_T *connect_rst_info);
VOID x_mtkapi_bt_gattc_get_disconnect_result_info(BT_GATTC_CONNECT_RST_T *disconnect_rst_info);
VOID x_mtkapi_bt_gattc_read_rssi_result_info(BT_GATTC_GET_REMOTE_RSSI_T *get_remote_rssi_info);
INT32 x_mtkapi_bt_gattc_base_init(MTKRPCAPI_BT_APP_GATTC_CB_FUNC_T *func, void *pv_tag);
INT32 mtkrpcapi_bt_gattc_base_init(MTKRPCAPI_BT_APP_GATTC_CB_FUNC_T *func,void *pv_tag);
INT32 mtkrpcapi_btm_gattc_register_cbk_fct(MTKRPCAPI_BT_APP_GATTC_CB_FUNC_T *func,void *pv_tag);


VOID MWBtAppGATTCEventCbk(BT_GATTC_EVENT_T bt_gatt_event);
VOID MWBtAppGATTCRegClientCbk(BT_GATTC_REG_CLIENT_T *pt_reg_client_result);

#endif

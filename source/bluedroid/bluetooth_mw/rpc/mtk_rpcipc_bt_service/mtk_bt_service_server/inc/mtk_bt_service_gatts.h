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

#ifndef _MTK_BT_SERVICE_GATTS_H_
#define _MTK_BT_SERVICE_GATTS_H_

#include "mtk_bt_service_gatts_wrapper.h"

INT32 x_mtkapi_bt_gatts_base_init(MTKRPCAPI_BT_APP_GATTS_CB_FUNC_T *func, void *pv_tag);

INT32 x_mtkapi_bt_gatts_register_server(char * app_uuid);
INT32 x_mtkapi_bt_gatts_unregister_server(INT32 server_if);
INT32 x_mtkapi_bt_gatts_open(INT32 server_if, CHAR *bt_addr, UINT8 is_direct, INT32 transport);
INT32 x_mtkapi_bt_gatts_close(INT32 server_if, CHAR *bt_addr, INT32 conn_id);
INT32 x_mtkapi_bt_gatts_add_service(INT32 server_if, CHAR *service_uuid,
                                         UINT8 is_primary, INT32 number);
INT32 x_mtkapi_bt_gatts_add_included_service(INT32 server_if, INT32 service_handle, INT32 included_handle);
INT32 x_mtkapi_bt_gatts_add_char(INT32 server_if, INT32 service_handle, CHAR *uuid,
                                      INT32 properties, INT32 permissions);
INT32 x_mtkapi_bt_gatts_add_desc(INT32 server_if, INT32 service_handle,
                                       CHAR *uuid, INT32 permissions);
INT32 x_mtkapi_bt_gatts_start_service(INT32 server_if, INT32 service_handle, INT32 transport);
INT32 x_mtkapi_bt_gatts_stop_service(INT32 server_if, INT32 service_handle);
INT32 x_mtkapi_bt_gatts_delete_service(INT32 server_if, INT32 service_handle);
INT32 x_mtkapi_bt_gatts_send_indication(INT32 server_if, INT32 attribute_handle,
                                              INT32 conn_id, INT32 fg_confirm,
                                              CHAR *p_value, INT32 value_len);
INT32 x_mtkapi_bt_gatts_send_response(INT32 conn_id, INT32 trans_id,
                                             INT32 status, INT32 handle,
                                             CHAR *p_value, INT32 value_len,
                                             INT32 auth_req);
VOID x_mtkapi_bt_gatts_get_connect_result_info(BT_GATTS_CONNECT_RST_T *connect_rst_info);
VOID x_mtkapi_bt_gatts_get_disconnect_result_info(BT_GATTS_CONNECT_RST_T *disconnect_rst_info);

#if MTK_BLE_GGL_SETUP_SUPPORT
INT32 x_mtkapi_bt_gatt_initialize();
INT32 x_mtkapi_bt_gatt_finalize();
INT32 x_mtkapi_bt_gatt_isSupported();
INT32 x_mtkapi_bt_gatt_setDelegate(INT32 delegate);
INT32 x_mtkapi_bt_gatt_onCharacteristicWrite(char * service_uuid, char * char_uuid, char * value);
INT32 x_mtkapi_bt_gatts_createService(char * service_uuid);
INT32 x_mtkapi_bt_gatts_destroyService(char * service_uuid);
INT32 x_mtkapi_bt_gatts_startService(char * service_uuid);
INT32 x_mtkapi_bt_gatts_stopService(char * service_uuid);
INT32 x_mtkapi_bt_gatts_setDeviceName(char * name);
INT32 x_mtkapi_bt_gatt_setAdvertisement(char * service_uuid, char ** advertised_uuids,
                                              UINT32 advertised_uuids_num, char * advertise_data,
                                              const char * manufacturer_data, bool transmit_name);
INT32 x_mtkapi_bt_gatt_setScanResponse(char * service_uuid, char ** advertised_uuids,
                                               UINT32 advertised_uuids_num, char * advertise_data,
                                               char * manufacturer_data, bool transmit_name);
INT32 x_mtkapi_bt_gatts_addCharacteristic(char * service_uuid, char * attribute_uuid,
                                                char * control_uuid, char ** options, UINT32 options_num);
INT32 x_mtkapi_bt_gatts_setCharacteristicValue(char * service_uuid, char * attribute_uuid,
                                                     void * value, UINT32 length,
                                                     UINT32 max_length, UINT32 flag);
INT32 x_mtkapi_bt_gatt_reg_nfy_fct(a_mtkapi_bt_gatt_nfy_fct_cbk pf_nfy, void *pv_tag);
#endif
#endif

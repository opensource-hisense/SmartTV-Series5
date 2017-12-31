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

/*-----------------------------------------------------------------------------
                            include files
-----------------------------------------------------------------------------*/
#include <stdio.h>

#include "mtk_bt_service_gattc_wrapper.h"
#include "mtk_bt_service_gattc.h"
#include "c_bt_mw_gatt.h"


#define BT_RC_LOG(_stmt...) \
        do{ \
            if(0){    \
                printf("[setup.c]Func:%s Line:%d--->: ", __FUNCTION__, __LINE__);   \
                printf(_stmt); \
                printf("\n"); \
            }        \
        }   \
        while(0)


void *g_gattc_pvtag = NULL;

mtkrpcapi_BtAppGATTCEventCbk mtkrpcapi_BtGATTCEventCbk = NULL;
mtkrpcapi_BtAppGATTCRegClientCbk mtkrpcapi_BtGATTCRegClientCbk = NULL;
mtkrpcapi_BtAppGATTCScanCbk mtkrpcapi_BtGATTCScanResultCbk = NULL;
mtkrpcapi_BtAppGATTCGetGattDbCbk mtkrpcapi_BtGATTCGetGattDbCbk = NULL;
mtkrpcapi_BtAppGATTCGetRegNotiCbk mtkrpcapi_BtGATTCGetRegNotiCbk = NULL;
mtkrpcapi_BtAppGATTCNotifyCbk mtkrpcapi_BtGATTCNotifyCbk = NULL;
mtkrpcapi_BtAppGATTCReadCharCbk mtkrpcapi_BtGATTCReadCharCbk = NULL;
mtkrpcapi_BtAppGATTCWriteCharCbk mtkrpcapi_BtGATTCWriteCharCbk = NULL;
mtkrpcapi_BtAppGATTCReadDescCbk mtkrpcapi_BtGATTCReadDescCbk = NULL;
mtkrpcapi_BtAppGATTCWriteDescCbk mtkrpcapi_BtGATTCWriteDescCbk = NULL;
mtkrpcapi_BtAppGATTScanFilterParamCbk mtkrpcapi_BtGATTCScanFilterParamCbk = NULL;
mtkrpcapi_BtAppGATTScanFilterStatusCbk mtkrpcapi_BtGATTCScanFilterStatusCbk = NULL;
mtkrpcapi_BtAppGATTScanFilterCfgCbk mtkrpcapi_BtGATTCScanFilterCfgCbk = NULL;
mtkrpcapi_BtAppGATTGetDeviceNameCbk mtkrpcapi_BtGATTCGetDeviceNameCbk = NULL;

INT32 x_mtkapi_bt_gattc_register_app(char * app_uuid)
{
    INT32 i4_ret = 0;
    i4_ret = c_btm_bt_gattc_register_app(app_uuid);
    if (i4_ret != 0)
    {
        BT_RC_LOG("[.c][%s]c_btm_bt_gattc_register_app fail\n", __FUNCTION__);
    }
    else
    {
        BT_RC_LOG("[.c][%s]c_btm_bt_gattc_register_app success\n", __FUNCTION__);
    }

    return i4_ret;

}

INT32 x_mtkapi_bt_gattc_unregister_app(INT32 client_if)
{
    INT32 i4_ret = 0;
    i4_ret = c_btm_bt_gattc_unregister_app(client_if);
    if (i4_ret != 0)
    {
        BT_RC_LOG("[.c][%s]c_btm_bt_gattc_unregister_app fail\n", __FUNCTION__);
    }
    else
    {
        BT_RC_LOG("[.c][%s]c_btm_bt_gattc_unregister_app success\n", __FUNCTION__);
    }

    return i4_ret;
}

INT32 x_mtkapi_bt_gattc_scan()
{
    INT32 i4_ret = 0;
    i4_ret = c_btm_bt_gattc_scan();
    if (i4_ret != 0)
    {
        BT_RC_LOG("[.c][%s]c_btm_bt_gattc_scan fail\n", __FUNCTION__);
    }
    else
    {
        BT_RC_LOG("[.c][%s]c_btm_bt_gattc_scan success\n", __FUNCTION__);
    }

    return i4_ret;

}

INT32 x_mtkapi_bt_gattc_stop_scan()
{
    INT32 i4_ret = 0;
    i4_ret = c_btm_bt_gattc_stop_scan();
    if (i4_ret != 0)
    {
        BT_RC_LOG("[.c][%s]c_btm_bt_gattc_stop_scan fail\n", __FUNCTION__);
    }
    else
    {
        BT_RC_LOG("[.c][%s]c_btm_bt_gattc_stop_scan success\n", __FUNCTION__);
    }

    return i4_ret;

}

INT32 x_mtkapi_bt_gattc_open(INT32 client_if, CHAR *bt_addr, UINT8 is_direct, INT32 transport)
{
    INT32 i4_ret = 0;
    i4_ret = c_btm_bt_gattc_open(client_if, bt_addr, is_direct, transport);
    if (i4_ret != 0)
    {
        BT_RC_LOG("[.c][%s]c_btm_bt_gattc_open fail\n", __FUNCTION__);
    }
    else
    {
        BT_RC_LOG("[.c][%s]c_btm_bt_gattc_open success\n", __FUNCTION__);
    }

    return i4_ret;
}

INT32 x_mtkapi_bt_gattc_close(INT32 client_if, CHAR *bt_addr, INT32 conn_id)
{
    INT32 i4_ret = 0;
    i4_ret = c_btm_bt_gattc_close(client_if, bt_addr, conn_id);
    if (i4_ret != 0)
    {
        BT_RC_LOG("[.c][%s]c_btm_bt_gattc_close fail\n", __FUNCTION__);
    }
    else
    {
        BT_RC_LOG("[.c][%s]c_btm_bt_gattc_close success\n", __FUNCTION__);
    }

    return i4_ret;
}

INT32 x_mtkapi_bt_gattc_listen(INT32 client_if)
{
    INT32 i4_ret = 0;
    i4_ret = c_btm_bt_gattc_listen(client_if);
    if (i4_ret != 0)
    {
        BT_RC_LOG("[.c][%s]c_btm_bt_gattc_listen fail\n", __FUNCTION__);
    }
    else
    {
        BT_RC_LOG("[.c][%s]c_btm_bt_gattc_listen success\n", __FUNCTION__);
    }

    return i4_ret;
}

INT32 x_mtkapi_bt_gattc_refresh(INT32 client_if, CHAR *bt_addr)
{
    INT32 i4_ret = 0;
    i4_ret = c_btm_bt_gattc_refresh(client_if, bt_addr);
    if (i4_ret != 0)
    {
        BT_RC_LOG("[.c][%s]c_btm_bt_gattc_refresh fail\n", __FUNCTION__);
    }
    else
    {
        BT_RC_LOG("[.c][%s]c_btm_bt_gattc_refresh success\n", __FUNCTION__);
    }

    return i4_ret;
}

INT32 x_mtkapi_bt_gattc_search_service(INT32 conn_id, CHAR *uuid)
{
    INT32 i4_ret = 0;
    i4_ret = c_btm_bt_gattc_search_service(conn_id, uuid);
    if (i4_ret != 0)
    {
        BT_RC_LOG("[.c][%s]c_btm_bt_gattc_search_service fail\n", __FUNCTION__);
    }
    else
    {
        BT_RC_LOG("[.c][%s]c_btm_bt_gattc_search_service success\n", __FUNCTION__);
    }

    return i4_ret;
}

INT32 x_mtkapi_bt_gattc_get_gatt_db(INT32 conn_id)
{
    INT32 i4_ret = 0;
    i4_ret = c_btm_bt_gattc_get_gatt_db(conn_id);
    if (i4_ret != 0)
    {
        BT_RC_LOG("[.c][%s]c_btm_bt_gattc_get_gatt_db fail\n", __FUNCTION__);
    }
    else
    {
        BT_RC_LOG("[.c][%s]c_btm_bt_gattc_get_gatt_db success\n", __FUNCTION__);
    }

    return i4_ret;
}

INT32 x_mtkapi_bt_gattc_read_char(INT32 conn_id, INT32 char_handle, INT32 auth_req)
{
    INT32 i4_ret = 0;
    i4_ret = c_btm_bt_gattc_read_char(conn_id, char_handle, auth_req);
    if (i4_ret != 0)
    {
        BT_RC_LOG("[.c][%s]c_btm_bt_gattc_read_char fail\n", __FUNCTION__);
    }
    else
    {
        BT_RC_LOG("[.c][%s]c_btm_bt_gattc_read_char success\n", __FUNCTION__);
    }

    return i4_ret;
}

INT32 x_mtkapi_bt_gattc_read_descr(INT32 conn_id, INT32 descr_handle, INT32 auth_req)
{
    INT32 i4_ret = 0;
    i4_ret = c_btm_bt_gattc_read_descr(conn_id, descr_handle, auth_req);
    if (i4_ret != 0)
    {
        BT_RC_LOG("[.c][%s]c_btm_bt_gattc_read_descr fail\n", __FUNCTION__);
    }
    else
    {
        BT_RC_LOG("[.c][%s]c_btm_bt_gattc_read_descr success\n", __FUNCTION__);
    }

    return i4_ret;
}

INT32 x_mtkapi_bt_gattc_write_char(INT32 conn_id, INT32 char_handle,
                                        INT32 write_type, INT32 len,
                                        INT32 auth_req, CHAR *value)
{
    INT32 i4_ret = 0;
    i4_ret = c_btm_bt_gattc_write_char(conn_id, char_handle, write_type, len, auth_req, value);
    if (i4_ret != 0)
    {
        BT_RC_LOG("[.c][%s]c_btm_bt_gattc_write_char fail\n", __FUNCTION__);
    }
    else
    {
        BT_RC_LOG("[.c][%s]c_btm_bt_gattc_write_char success\n", __FUNCTION__);
    }

    return i4_ret;
}

INT32 x_mtkapi_bt_gattc_write_descr(INT32 conn_id, INT32 descr_handle,
                                         INT32 write_type, INT32 len,
                                         INT32 auth_req, CHAR *value)
{
    INT32 i4_ret = 0;
    i4_ret = c_btm_bt_gattc_write_descr(conn_id, descr_handle, write_type, len, auth_req, value);
    if (i4_ret != 0)
    {
        BT_RC_LOG("[.c][%s]c_btm_bt_gattc_write_descr fail\n", __FUNCTION__);
    }
    else
    {
        BT_RC_LOG("[.c][%s]c_btm_bt_gattc_write_descr success\n", __FUNCTION__);
    }

    return i4_ret;
}

INT32 x_mtkapi_bt_gattc_execute_write(INT32 conn_id, INT32 execute)
{
    INT32 i4_ret = 0;
    i4_ret = c_btm_bt_gattc_execute_write(conn_id, execute);
    if (i4_ret != 0)
    {
        BT_RC_LOG("[.c][%s]c_btm_bt_gattc_execute_write fail\n", __FUNCTION__);
    }
    else
    {
        BT_RC_LOG("[.c][%s]c_btm_bt_gattc_execute_write success\n", __FUNCTION__);
    }

    return i4_ret;
}

INT32 x_mtkapi_bt_gattc_reg_noti(INT32 client_if, CHAR *bt_addr, INT32 char_handle)
{
    INT32 i4_ret = 0;
    i4_ret = c_btm_bt_gattc_reg_noti(client_if, bt_addr, char_handle);
    if (i4_ret != 0)
    {
        BT_RC_LOG("[.c][%s]c_btm_bt_gattc_reg_noti fail\n", __FUNCTION__);
    }
    else
    {
        BT_RC_LOG("[.c][%s]c_btm_bt_gattc_reg_noti success\n", __FUNCTION__);
    }

    return i4_ret;
}

INT32 x_mtkapi_bt_gattc_dereg_noti(INT32 client_if, CHAR *bt_addr, INT32 char_handle)
{
    INT32 i4_ret = 0;
    i4_ret = c_btm_bt_gattc_dereg_noti(client_if, bt_addr, char_handle);
    if (i4_ret != 0)
    {
        BT_RC_LOG("[.c][%s]c_btm_bt_gattc_dereg_noti fail\n", __FUNCTION__);
    }
    else
    {
        BT_RC_LOG("[.c][%s]c_btm_bt_gattc_dereg_noti success\n", __FUNCTION__);
    }

    return i4_ret;
}

INT32 x_mtkapi_bt_gattc_read_rssi(INT32 client_if, CHAR *bt_addr)
{
    INT32 i4_ret = 0;
    i4_ret = c_btm_bt_gattc_read_rssi(client_if, bt_addr);
    if (i4_ret != 0)
    {
        BT_RC_LOG("[.c][%s]c_btm_bt_gattc_read_rssi fail\n", __FUNCTION__);
    }
    else
    {
        BT_RC_LOG("[.c][%s]c_btm_bt_gattc_read_rssi success\n", __FUNCTION__);
    }

    return i4_ret;
}

INT32 x_mtkapi_bt_gattc_scan_filter_param_setup(bluetooth_gatt_filt_param_setup_t *scan_filt_param)
{
    INT32 i4_ret = 0;
    bluetooth_gatt_filt_param_setup_t filt_param;
    memcpy(&filt_param, scan_filt_param, sizeof(bluetooth_gatt_filt_param_setup_t));
    i4_ret = c_btm_gattc_scan_filter_param_setup(filt_param);
    if (i4_ret != 0)
    {
        BT_RC_LOG("[.c][%s]c_btm_gattc_scan_filter_param_setup fail\n", __FUNCTION__);
    }
    else
    {
        BT_RC_LOG("[.c][%s]c_btm_gattc_scan_filter_param_setup success\n", __FUNCTION__);
    }

    return i4_ret;
}

INT32 x_mtkapi_bt_gattc_scan_filter_enable(INT32 client_if)
{
    INT32 i4_ret = 0;
    i4_ret = c_btm_gattc_scan_filter_enable(client_if);
    if (i4_ret != 0)
    {
        BT_RC_LOG("[.c][%s]c_btm_gattc_scan_filter_enable fail\n", __FUNCTION__);
    }
    else
    {
        BT_RC_LOG("[.c][%s]c_btm_gattc_scan_filter_enable success\n", __FUNCTION__);
    }

    return i4_ret;
}

INT32 x_mtkapi_bt_gattc_scan_filter_disable(INT32 client_if)
{
    INT32 i4_ret = 0;
    i4_ret = c_btm_gattc_scan_filter_disable(client_if);
    if (i4_ret != 0)
    {
        BT_RC_LOG("[.c][%s]c_btm_gattc_scan_filter_disable fail\n", __FUNCTION__);
    }
    else
    {
        BT_RC_LOG("[.c][%s]c_btm_gattc_scan_filter_disable success\n", __FUNCTION__);
    }

    return i4_ret;
}

INT32 x_mtkapi_bt_gattc_scan_filter_add_remove(INT32 client_if, INT32 action,
                                                      INT32 filt_type, INT32 filt_index, INT32 company_id,
                                                      INT32 company_id_mask, const CHAR *p_uuid,
                                                      const CHAR *p_uuid_mask, const CHAR *bd_addr,
                                                      CHAR addr_type, INT32 data_len, CHAR *p_data, INT32 mask_len,
                                                      CHAR *p_mask)
{
    INT32 i4_ret = 0;
    i4_ret = c_btm_gattc_scan_filter_add_remove(client_if, action, filt_type, filt_index, company_id, company_id_mask,
                                                p_uuid, p_uuid_mask, bd_addr, addr_type, data_len, p_data,
                                                mask_len, p_mask);
    if (i4_ret != 0)
    {
        BT_RC_LOG("[.c][%s]c_btm_gattc_scan_filter_add_remove fail\n", __FUNCTION__);
    }
    else
    {
        BT_RC_LOG("[.c][%s]c_btm_gattc_scan_filter_add_remove success\n", __FUNCTION__);
    }

    return i4_ret;
}

INT32 x_mtkapi_bt_gattc_scan_filter_clear(INT32 client_if, INT32 filt_index)
{
    INT32 i4_ret = 0;
    i4_ret = c_btm_gattc_scan_filter_clear(client_if, filt_index);
    if (i4_ret != 0)
    {
        BT_RC_LOG("[.c][%s]c_btm_gattc_scan_filter_clear fail\n", __FUNCTION__);
    }
    else
    {
        BT_RC_LOG("[.c][%s]c_btm_gattc_scan_filter_clear success\n", __FUNCTION__);
    }

    return i4_ret;
}

INT32 x_mtkapi_bt_gattc_get_device_type(CHAR *bd_addr)
{
    INT32 i4_ret = 0;
    i4_ret = c_btm_gattc_get_device_type(bd_addr);
    if (i4_ret != 0)
    {
        BT_RC_LOG("[.c][%s]c_btm_gattc_get_device_type fail\n", __FUNCTION__);
    }
    else
    {
        BT_RC_LOG("[.c][%s]c_btm_gattc_get_device_type success\n", __FUNCTION__);
    }

    return i4_ret;
}

INT32 x_mtkapi_bt_gattc_configure_mtu(INT32 conn_id, INT32 mtu)
{
    INT32 i4_ret = 0;
    i4_ret = c_btm_gattc_configure_mtu(conn_id, mtu);
    if (i4_ret != 0)
    {
        BT_RC_LOG("[.c][%s]c_btm_gattc_configure_mtu fail\n", __FUNCTION__);
    }
    else
    {
        BT_RC_LOG("[.c][%s]c_btm_gattc_configure_mtu success\n", __FUNCTION__);
    }

    return i4_ret;
}

INT32 x_mtkapi_bt_gattc_conn_parameter_update(CHAR *bt_addr, INT32 min_interval, INT32 max_interval,
                                                       INT32 latency, INT32 timeout)
{
    INT32 i4_ret = 0;
    i4_ret = c_btm_bt_gattc_conn_parameter_update(bt_addr, min_interval, max_interval,
                                                  latency, timeout);
    if (i4_ret != 0)
    {
        BT_RC_LOG("[.c][%s]c_btm_bt_gattc_conn_parameter_update fail\n", __FUNCTION__);
    }
    else
    {
        BT_RC_LOG("[.c][%s]c_btm_bt_gattc_conn_parameter_update success\n", __FUNCTION__);
    }

    return i4_ret;
}

INT32 x_mtkapi_bt_gattc_set_scan_parameters(INT32 client_if, INT32 scan_interval, INT32 scan_window)
{
    INT32 i4_ret = 0;
    i4_ret = c_btm_bt_gattc_set_scan_parameters(client_if, scan_interval, scan_window);
    if (i4_ret != 0)
    {
        BT_RC_LOG("[.c][%s]c_btm_bt_gattc_set_scan_parameters fail\n", __FUNCTION__);
    }
    else
    {
        BT_RC_LOG("[.c][%s]c_btm_bt_gattc_set_scan_parameters success\n", __FUNCTION__);
    }

    return i4_ret;
}

INT32 x_mtkapi_bt_gattc_set_disc_mode(INT32 client_if, INT32 disc_mode)
{
    INT32 i4_ret = 0;
    i4_ret = c_btm_gattc_set_local_disc_mode(client_if, disc_mode);
    if (i4_ret != 0)
    {
        BT_RC_LOG("[.c][%s]c_btm_gattc_set_local_disc_mode fail\n", __FUNCTION__);
    }
    else
    {
        BT_RC_LOG("[.c][%s]c_btm_gattc_set_local_disc_mode success\n", __FUNCTION__);
    }

    return i4_ret;
}

INT32 x_mtkapi_bt_gattc_multi_adv_enable(INT32 client_if, INT32 min_interval,
                                               INT32 max_interval, INT32 adv_type,
                                               INT32 chnl_map, INT32 tx_power, INT32 timeout_s)
{
    INT32 i4_ret = 0;
    i4_ret = c_btm_bt_gattc_multi_adv_enable(client_if, min_interval, max_interval, adv_type,
                                             chnl_map, tx_power, timeout_s);
    if (i4_ret != 0)
    {
        BT_RC_LOG("[.c][%s]c_btm_bt_gattc_multi_adv_enable fail\n", __FUNCTION__);
    }
    else
    {
        BT_RC_LOG("[.c][%s]c_btm_bt_gattc_multi_adv_enable success\n", __FUNCTION__);
    }

    return i4_ret;
}


INT32 x_mtkapi_bt_gattc_multi_adv_update(INT32 client_if, INT32 min_interval,
                                               INT32 max_interval, INT32 adv_type,
                                               INT32 chnl_map, INT32 tx_power, INT32 timeout_s)
{
    INT32 i4_ret = 0;
    i4_ret = c_btm_bt_gattc_multi_adv_update(client_if, min_interval, max_interval, adv_type,
                                             chnl_map, tx_power, timeout_s);
    if (i4_ret != 0)
    {
        BT_RC_LOG("[.c][%s]c_btm_bt_gattc_multi_adv_update fail\n", __FUNCTION__);
    }
    else
    {
        BT_RC_LOG("[.c][%s]c_btm_bt_gattc_multi_adv_update success\n", __FUNCTION__);
    }

    return i4_ret;
}

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
                                                CHAR* service_uuid)
{
    INT32 i4_ret = 0;
    i4_ret = c_btm_bt_gattc_multi_adv_setdata(client_if, set_scan_rsp, include_name, include_txpower,
                                              appearance, manufacturer_len, manufacturer_data,
                                              service_data_len, service_data, service_uuid_len, service_uuid);
    if (i4_ret != 0)
    {
        BT_RC_LOG("[.c][%s]c_btm_bt_gattc_multi_adv_setdata fail\n", __FUNCTION__);
    }
    else
    {
        BT_RC_LOG("[.c][%s]c_btm_bt_gattc_multi_adv_setdata success\n", __FUNCTION__);
    }

    return i4_ret;
}

INT32 x_mtkapi_bt_gattc_multi_adv_disable(INT32 client_if)
{
    INT32 i4_ret = 0;
    i4_ret = c_btm_bt_gattc_multi_adv_disable(client_if);
    if (i4_ret != 0)
    {
        BT_RC_LOG("[.c][%s]c_btm_bt_gattc_multi_adv_disable fail\n", __FUNCTION__);
    }
    else
    {
        BT_RC_LOG("[.c][%s]c_btm_bt_gattc_multi_adv_disable success\n", __FUNCTION__);
    }

    return i4_ret;
}
INT32 x_mtkapi_bt_gattc_batchscan_cfg_storage(INT32 client_if, INT32 batch_scan_full_max,
                                                      INT32 batch_scan_trunc_max, INT32 batch_scan_notify_threshold)
{
    INT32 i4_ret = 0;
    i4_ret = c_btm_bt_gattc_batchscan_cfg_storage(client_if, batch_scan_full_max,
                                                  batch_scan_trunc_max, batch_scan_notify_threshold);
    if (i4_ret != 0)
    {
        BT_RC_LOG("[.c][%s]c_btm_bt_gattc_batchscan_cfg_storage fail\n", __FUNCTION__);
    }
    else
    {
        BT_RC_LOG("[.c][%s]c_btm_bt_gattc_batchscan_cfg_storage success\n", __FUNCTION__);
    }

    return i4_ret;
}

INT32 x_mtkapi_bt_gattc_batchscan_enb_batch_scan(INT32 client_if, INT32 scan_mode,
                        INT32 scan_interval, INT32 scan_window, INT32 addr_type, INT32 discard_rule)
{
    INT32 i4_ret = 0;
    i4_ret = c_btm_bt_gattc_batchscan_enb_batch_scan(client_if, scan_mode, scan_interval,
                                                     scan_window, addr_type, discard_rule);
    if (i4_ret != 0)
    {
        BT_RC_LOG("[.c][%s]c_btm_bt_gattc_batchscan_enb_batch_scan fail\n", __FUNCTION__);
    }
    else
    {
        BT_RC_LOG("[.c][%s]c_btm_bt_gattc_batchscan_enb_batch_scan success\n", __FUNCTION__);
    }

    return i4_ret;
}

INT32 x_mtkapi_bt_gattc_batchscan_dis_batch_scan(INT32 client_if)
{
    INT32 i4_ret = 0;
    i4_ret = c_btm_bt_gattc_batchscan_dis_batch_scan(client_if);
    if (i4_ret != 0)
    {
        BT_RC_LOG("[.c][%s]c_btm_bt_gattc_batchscan_dis_batch_scan fail\n", __FUNCTION__);
    }
    else
    {
        BT_RC_LOG("[.c][%s]c_btm_bt_gattc_batchscan_dis_batch_scan success\n", __FUNCTION__);
    }

    return i4_ret;
}

INT32 x_mtkapi_bt_gattc_batchscan_read_reports(INT32 client_if, INT32 scan_mode)
{
    INT32 i4_ret = 0;
    i4_ret = c_btm_bt_gattc_batchscan_read_reports(client_if, scan_mode);
    if (i4_ret != 0)
    {
        BT_RC_LOG("[.c][%s]c_btm_bt_gattc_batchscan_read_reports fail\n", __FUNCTION__);
    }
    else
    {
        BT_RC_LOG("[.c][%s]c_btm_bt_gattc_batchscan_read_reports success\n", __FUNCTION__);
    }

    return i4_ret;
}

#if MTK_LINUX_GATTC_LE_NAME
INT32 x_mtkapi_bt_gattc_set_local_le_name(INT32 client_if, CHAR *le_name)
{
    INT32 i4_ret = 0;
    i4_ret = c_btm_gattc_set_local_le_name(client_if, le_name);
    if (i4_ret != 0)
    {
        BT_RC_LOG("[.c][%s]c_btm_gattc_set_local_le_name fail\n", __FUNCTION__);
    }
    else
    {
        BT_RC_LOG("[.c][%s]c_btm_gattc_set_local_le_name success\n", __FUNCTION__);
    }

    return i4_ret;
}
#endif

VOID x_mtkapi_bt_gattc_get_connect_result_info(BT_GATTC_CONNECT_RST_T *connect_rst_info)
{
    c_btm_bt_gattc_get_connect_result_info(connect_rst_info);
}

VOID x_mtkapi_bt_gattc_get_disconnect_result_info(BT_GATTC_CONNECT_RST_T *disconnect_rst_info)
{
    c_btm_bt_gattc_get_disconnect_result_info(disconnect_rst_info);
}

VOID x_mtkapi_bt_gattc_read_rssi_result_info(BT_GATTC_GET_REMOTE_RSSI_T *get_remote_rssi_info)
{
    c_btm_bt_gattc_read_rssi_result_info(get_remote_rssi_info);
}

VOID MWBtAppGATTCEventCbk(BT_GATTC_EVENT_T bt_gatt_event)
{
    if (mtkrpcapi_BtGATTCEventCbk)
    {
        mtkrpcapi_BtGATTCEventCbk(bt_gatt_event, g_gattc_pvtag);
    }
}

VOID MWBtAppGATTCRegClientCbk(BT_GATTC_REG_CLIENT_T *pt_reg_client_result)
{
    if (NULL == pt_reg_client_result)
    {
        return;
    }
    BT_RC_LOG("[.c][%s], client_if = %ld\n", __FUNCTION__, (long)pt_reg_client_result->client_if);

    if (mtkrpcapi_BtGATTCRegClientCbk)
    {
        mtkrpcapi_BtGATTCRegClientCbk(pt_reg_client_result, g_gattc_pvtag);
    }
}

VOID MWBtAppGATTCScanResultCbk(BT_GATTC_SCAN_RST_T *pt_scan_result)
{
    if (NULL == pt_scan_result)
    {
        return;
    }
    BT_RC_LOG("[.c][%s], pt_scan_result->btaddr = %s\n", __FUNCTION__, pt_scan_result->btaddr);
    if (mtkrpcapi_BtGATTCScanResultCbk)
    {
        mtkrpcapi_BtGATTCScanResultCbk(pt_scan_result, g_gattc_pvtag);
    }
}

VOID MWBtAppGATTCGetGattDbCbk(BT_GATTC_GET_GATT_DB_T *pt_get_gatt_db_result)
{
    if (NULL == pt_get_gatt_db_result)
    {
        return;
    }
    BT_RC_LOG("[.c][%s], pt_get_gatt_db_result->count = %d\n", __FUNCTION__, pt_get_gatt_db_result->count);
    if (mtkrpcapi_BtGATTCGetGattDbCbk)
    {
        mtkrpcapi_BtGATTCGetGattDbCbk(pt_get_gatt_db_result, g_gattc_pvtag);
    }
}

VOID MWBtAppGATTCGetRegNotiCbk(BT_GATTC_GET_REG_NOTI_RST_T *pt_get_reg_noti_result)
{
    if (NULL == pt_get_reg_noti_result)
    {
        return;
    }
    BT_RC_LOG("[.c][%s], pt_get_reg_noti_result->registered = %d\n", __FUNCTION__, pt_get_reg_noti_result->registered);
    if (mtkrpcapi_BtGATTCGetRegNotiCbk)
    {
        mtkrpcapi_BtGATTCGetRegNotiCbk(pt_get_reg_noti_result, g_gattc_pvtag);
    }
}

VOID MWBtAppGATTCNotifyCbk(BT_GATTC_GET_NOTIFY_T *pt_notify)
{
    if (NULL == pt_notify)
    {
        return;
    }
    BT_RC_LOG("[.c][%s], bda = %s\n", __FUNCTION__, pt_notify->notify_data.bda);
    if (mtkrpcapi_BtGATTCNotifyCbk)
    {
        mtkrpcapi_BtGATTCNotifyCbk(pt_notify, g_gattc_pvtag);
    }
}

VOID MWBtAppGATTCReadCharCbk(BT_GATTC_READ_CHAR_RST_T *pt_read_char)
{
    if (NULL == pt_read_char)
    {
        return;
    }
    BT_RC_LOG("[.c][%s], value = %s\n", __FUNCTION__, pt_read_char->read_data.value.value);
    if (mtkrpcapi_BtGATTCReadCharCbk)
    {
        mtkrpcapi_BtGATTCReadCharCbk(pt_read_char, g_gattc_pvtag);
    }
}

VOID MWBtAppGATTCWriteCharCbk(BT_GATTC_WRITE_CHAR_RST_T *pt_write_char)
{
    if (NULL == pt_write_char)
    {
        return;
    }
    BT_RC_LOG("[.c][%s], handle = %ld\n", __FUNCTION__, (long)pt_write_char->handle);
    if (mtkrpcapi_BtGATTCWriteCharCbk)
    {
        mtkrpcapi_BtGATTCWriteCharCbk(pt_write_char, g_gattc_pvtag);
    }
}

VOID MWBtAppGATTCReadDescCbk(BT_GATTC_READ_DESCR_RST_T *pt_read_desc)
{
    if (NULL == pt_read_desc)
    {
        return;
    }
    BT_RC_LOG("[.c][%s], value = %s\n", __FUNCTION__, pt_read_desc->read_data.value.value);
    if (mtkrpcapi_BtGATTCReadDescCbk)
    {
        mtkrpcapi_BtGATTCReadDescCbk(pt_read_desc, g_gattc_pvtag);
    }
}

VOID MWBtAppGATTCWriteDescCbk(BT_GATTC_WRITE_DESCR_RST_T *pt_write_desc)
{
    if (NULL == pt_write_desc)
    {
        return;
    }
    BT_RC_LOG("[.c][%s], handle = %ld\n", __FUNCTION__, (long)pt_write_desc->handle);
    if (mtkrpcapi_BtGATTCWriteDescCbk)
    {
        mtkrpcapi_BtGATTCWriteDescCbk(pt_write_desc, g_gattc_pvtag);
    }
}

VOID MWBtAppGATTCScanFilterParamCbk(bluetooth_gatt_scan_filter_param_t *pt_scan_filter_param)
{
    if (NULL == pt_scan_filter_param)
    {
        return;
    }
    BT_RC_LOG("[.c][%s], action = %ld\n", __FUNCTION__, (long)pt_scan_filter_param->action);
    if (mtkrpcapi_BtGATTCScanFilterParamCbk)
    {
        mtkrpcapi_BtGATTCScanFilterParamCbk(pt_scan_filter_param, g_gattc_pvtag);
    }
}

VOID MWBtAppGATTCScanFilterStatusCbk(bluetooth_gatt_scan_filter_status_t *pt_scan_filter_status)
{
    if (NULL == pt_scan_filter_status)
    {
        return;
    }
    BT_RC_LOG("[.c][%s], enable = %ld\n", __FUNCTION__, (long)pt_scan_filter_status->enable);
    if (mtkrpcapi_BtGATTCScanFilterStatusCbk)
    {
        mtkrpcapi_BtGATTCScanFilterStatusCbk(pt_scan_filter_status, g_gattc_pvtag);
    }
}

VOID MWBtAppGATTCScanFilterCfgCbk(bluetooth_gatt_scan_filter_cfg_t *pt_scan_filter_cfg)
{
    if (NULL == pt_scan_filter_cfg)
    {
        return;
    }
    BT_RC_LOG("[.c][%s], action = %ld\n", __FUNCTION__, (long)pt_scan_filter_cfg->action);
    if (mtkrpcapi_BtGATTCScanFilterCfgCbk)
    {
        mtkrpcapi_BtGATTCScanFilterCfgCbk(pt_scan_filter_cfg, g_gattc_pvtag);
    }
}

VOID MWBtAppGATTCGetDeviceNameCbk(CHAR *pt_DeviceName)
{
    if (NULL == pt_DeviceName)
    {
        return;
    }
    BT_RC_LOG("[.c][%s], pt_DeviceName = %s\n", __FUNCTION__, pt_DeviceName);
    if (mtkrpcapi_BtGATTCGetDeviceNameCbk)
    {
        mtkrpcapi_BtGATTCGetDeviceNameCbk(pt_DeviceName, g_gattc_pvtag);
    }
}

/*register APP callback function*/
INT32 mtkrpcapi_btm_gattc_register_cbk_fct(MTKRPCAPI_BT_APP_GATTC_CB_FUNC_T *func,void *pv_tag)
{
    INT32 i4_ret = 0;

    g_gattc_pvtag = pv_tag;
    if(NULL == func)
    {
        BT_RC_LOG(("callback func is null!\n"));
        return BT_ERR_NULL_POINTER;
    }
    if(func->bt_gatt_event_cb)
    {
        mtkrpcapi_BtGATTCEventCbk = func->bt_gatt_event_cb;
    }
    else
    {
        BT_RC_LOG(("event callback func is null!\n"));
        i4_ret = BT_ERR_NULL_POINTER;
    }

    if(func->bt_gatt_reg_client_cb)
    {
        mtkrpcapi_BtGATTCRegClientCbk = func->bt_gatt_reg_client_cb;
    }
    else
    {
        BT_RC_LOG(("bt_gatt_reg_client_cb callback func is null!\n"));
        i4_ret = BT_ERR_NULL_POINTER;
    }
    if(func->bt_gatt_scan_cb)
    {
        mtkrpcapi_BtGATTCScanResultCbk = func->bt_gatt_scan_cb;
    }
    else
    {
        BT_RC_LOG(("bt_gatt_scan_cb callback func is null!\n"));
        i4_ret = BT_ERR_NULL_POINTER;
    }

    if(func->bt_gatt_get_gatt_db_cb)
    {
        mtkrpcapi_BtGATTCGetGattDbCbk = func->bt_gatt_get_gatt_db_cb;
    }
    else
    {
        BT_RC_LOG(("bt_gatt_get_gatt_db_cb callback func is null!\n"));
        i4_ret = BT_ERR_NULL_POINTER;
    }

    if(func->bt_gatt_get_reg_noti_cb)
    {
        mtkrpcapi_BtGATTCGetRegNotiCbk = func->bt_gatt_get_reg_noti_cb;
    }
    else
    {
        BT_RC_LOG(("bt_gatt_get_reg_noti_cb callback func is null!\n"));
        i4_ret = BT_ERR_NULL_POINTER;
    }

    if(func->bt_gatt_notify_cb)
    {
        mtkrpcapi_BtGATTCNotifyCbk = func->bt_gatt_notify_cb;
    }
    else
    {
        BT_RC_LOG(("bt_gatt_notify_cb callback func is null!\n"));
        i4_ret = BT_ERR_NULL_POINTER;
    }

    if(func->bt_gatt_read_char_cb)
    {
        mtkrpcapi_BtGATTCReadCharCbk = func->bt_gatt_read_char_cb;
    }
    else
    {
        BT_RC_LOG(("bt_gatt_read_char_cb callback func is null!\n"));
        i4_ret = BT_ERR_NULL_POINTER;
    }

    if(func->bt_gatt_write_char_cb)
    {
        mtkrpcapi_BtGATTCWriteCharCbk = func->bt_gatt_write_char_cb;
    }
    else
    {
        BT_RC_LOG(("bt_gatt_write_char_cb callback func is null!\n"));
        i4_ret = BT_ERR_NULL_POINTER;
    }

    if(func->bt_gatt_read_desc_cb)
    {
        mtkrpcapi_BtGATTCReadDescCbk = func->bt_gatt_read_desc_cb;
    }
    else
    {
        BT_RC_LOG(("bt_gatt_read_desc_cb callback func is null!\n"));
        i4_ret = BT_ERR_NULL_POINTER;
    }

    if(func->bt_gatt_write_desc_cb)
    {
        mtkrpcapi_BtGATTCWriteDescCbk = func->bt_gatt_write_desc_cb;
    }
    else
    {
        BT_RC_LOG(("bt_gatt_write_desc_cb callback func is null!\n"));
        i4_ret = BT_ERR_NULL_POINTER;
    }

    if(func->bt_gatt_scan_filter_param_cb)
    {
        mtkrpcapi_BtGATTCScanFilterParamCbk = func->bt_gatt_scan_filter_param_cb;
    }
    else
    {
        BT_RC_LOG(("bt_gatt_scan_filter_param_cb callback func is null!\n"));
        i4_ret = BT_ERR_NULL_POINTER;
    }

    if(func->bt_gatt_scan_filter_status_cb)
    {
        mtkrpcapi_BtGATTCScanFilterStatusCbk = func->bt_gatt_scan_filter_status_cb;
    }
    else
    {
        BT_RC_LOG(("bt_gatt_scan_filter_status_cb callback func is null!\n"));
        i4_ret = BT_ERR_NULL_POINTER;
    }

    if(func->bt_gatt_scan_filter_cfg_cb)
    {
        mtkrpcapi_BtGATTCScanFilterCfgCbk = func->bt_gatt_scan_filter_cfg_cb;
    }
    else
    {
        BT_RC_LOG(("bt_gatt_scan_filter_cfg_cb callback func is null!\n"));
        i4_ret = BT_ERR_NULL_POINTER;
    }

    if(func->bt_gatt_get_device_name_cb)
    {
        mtkrpcapi_BtGATTCGetDeviceNameCbk = func->bt_gatt_get_device_name_cb;
    }
    else
    {
        BT_RC_LOG(("bt_gatt_get_device_name_cb callback func is null!\n"));
        i4_ret = BT_ERR_NULL_POINTER;
    }

    return i4_ret;
}

INT32 mtkrpcapi_bt_gattc_base_init(MTKRPCAPI_BT_APP_GATTC_CB_FUNC_T *func,void *pv_tag)
{
    BT_RC_LOG("[.c][%s]\n", __FUNCTION__);

    INT32 i4_ret = 0;
    BT_APP_GATTC_CB_FUNC_T app_func;
    memset(&app_func, 0, sizeof(BT_APP_GATTC_CB_FUNC_T));

    app_func.bt_gatt_event_cb = MWBtAppGATTCEventCbk;
    app_func.bt_gatt_reg_client_cb = MWBtAppGATTCRegClientCbk;
    app_func.bt_gatt_scan_cb = MWBtAppGATTCScanResultCbk;
    app_func.bt_gatt_get_gatt_db_cb = MWBtAppGATTCGetGattDbCbk;
    app_func.bt_gatt_get_reg_noti_cb = MWBtAppGATTCGetRegNotiCbk;
    app_func.bt_gatt_notify_cb = MWBtAppGATTCNotifyCbk;
    app_func.bt_gatt_read_char_cb = MWBtAppGATTCReadCharCbk;
    app_func.bt_gatt_write_char_cb = MWBtAppGATTCWriteCharCbk;
    app_func.bt_gatt_read_desc_cb = MWBtAppGATTCReadDescCbk;
    app_func.bt_gatt_write_desc_cb = MWBtAppGATTCWriteDescCbk;
    app_func.bt_gatt_scan_filter_param_cb = MWBtAppGATTCScanFilterParamCbk;
    app_func.bt_gatt_scan_filter_status_cb = MWBtAppGATTCScanFilterStatusCbk;
    app_func.bt_gatt_scan_filter_cfg_cb = MWBtAppGATTCScanFilterCfgCbk;
    app_func.bt_gatt_get_device_name_cb = MWBtAppGATTCGetDeviceNameCbk;
    i4_ret = mtkrpcapi_btm_gattc_register_cbk_fct(func, pv_tag);
    if(0 != i4_ret)
    {
        BT_RC_LOG(("mtkrpcapi_btm_register_cbk_fct Error.\n"));
        return i4_ret;
    }

    return c_btm_bt_gattc_base_init(&app_func);
}


INT32 x_mtkapi_bt_gattc_base_init(MTKRPCAPI_BT_APP_GATTC_CB_FUNC_T *func, void *pv_tag)
{
    BT_RC_LOG("[.c][%s]\n", __FUNCTION__);
    return mtkrpcapi_bt_gattc_base_init(func,pv_tag);

}

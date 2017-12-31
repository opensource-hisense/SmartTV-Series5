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

#include "mtk_bt_service_hidh_wrapper.h"
#include "mtk_bt_service_hidh_ipcrpc_struct.h"
#include "client_common.h"

#define BT_HIDH_LOG(_stmt...) \
        do{ \
            if(0){    \
                printf("[HIDH]Func:%s Line:%d--->: ", __FUNCTION__, __LINE__);   \
                printf(_stmt); \
                printf("\n"); \
            }        \
        }   \
        while(0)


EXPORT_SYMBOL INT32 a_mtkapi_hidh_init(VOID)
{
    BT_HIDH_LOG("a_mtkapi_hidh_init");
    //Adding a invalid value for passing IPC/RPC, no other use
    INT32 i4_InvalidValue = 0;
    RPC_CLIENT_DECL(1, INT32);
    RPC_CLIENT_ARG_INP(ARG_TYPE_INT32, i4_InvalidValue);
    RPC_BT_SERVICE_CLIENT_DO_OP("x_mtkapi_hidh_init");
    RPC_CLIENT_RETURN(ARG_TYPE_INT32, -1);
}

EXPORT_SYMBOL INT32 a_mtkapi_hidh_deinit(VOID)
{
    BT_HIDH_LOG("a_mtkapi_hidh_deinit");
    //Adding a invalid value for passing IPC/RPC, no other use
    INT32 i4_InvalidValue = 0;
    RPC_CLIENT_DECL(1, INT32);
    RPC_CLIENT_ARG_INP(ARG_TYPE_INT32, i4_InvalidValue);
    RPC_BT_SERVICE_CLIENT_DO_OP("x_mtkapi_hidh_deinit");
    RPC_CLIENT_RETURN(ARG_TYPE_INT32, -1);
}

EXPORT_SYMBOL INT32 a_mtkapi_hidh_connect(CHAR *pbt_addr)
{
    BT_HIDH_LOG("a_mtkapi_hidh_connect [%s]", pbt_addr);
    RPC_CLIENT_DECL(1, INT32);
    RPC_CLIENT_ARG_INP(ARG_TYPE_REF_STR, pbt_addr);
    RPC_BT_SERVICE_CLIENT_DO_OP("x_mtkapi_hidh_connect");
    RPC_CLIENT_RETURN(ARG_TYPE_INT32, -1);
}

EXPORT_SYMBOL INT32 a_mtkapi_hidh_disconnect(CHAR *pbt_addr)
{
    BT_HIDH_LOG("a_mtkapi_hidh_disconnect [%s]", pbt_addr);
    RPC_CLIENT_DECL(1, INT32);
    RPC_CLIENT_ARG_INP(ARG_TYPE_REF_STR, pbt_addr);
    RPC_BT_SERVICE_CLIENT_DO_OP("x_mtkapi_hidh_disconnect");
    RPC_CLIENT_RETURN(ARG_TYPE_INT32, -1);
}

EXPORT_SYMBOL INT32 a_mtkapi_hidh_set_output_report(CHAR *pbt_addr, CHAR *preport_data)
{
    BT_HIDH_LOG("a_mtkapi_hidh_set_output_report");
    RPC_CLIENT_DECL(2, INT32);
    RPC_CLIENT_ARG_INP(ARG_TYPE_REF_STR, pbt_addr);
    RPC_CLIENT_ARG_INP(ARG_TYPE_REF_STR, preport_data);
    RPC_BT_SERVICE_CLIENT_DO_OP("x_mtkapi_hidh_set_output_report");
    RPC_CLIENT_RETURN(ARG_TYPE_INT32, -1);
}

EXPORT_SYMBOL INT32 a_mtkapi_hidh_get_input_report(CHAR *pbt_addr, UINT8 reportId, INT32 bufferSize)
{
    BT_HIDH_LOG("a_mtkapi_hidh_get_input_report");
    RPC_CLIENT_DECL(3, INT32);
    RPC_CLIENT_ARG_INP(ARG_TYPE_REF_STR, pbt_addr);
    RPC_CLIENT_ARG_INP(ARG_TYPE_UINT8, reportId);
    RPC_CLIENT_ARG_INP(ARG_TYPE_INT32, bufferSize);
    RPC_BT_SERVICE_CLIENT_DO_OP("x_mtkapi_hidh_get_input_report");
    RPC_CLIENT_RETURN(ARG_TYPE_INT32, -1);
}

EXPORT_SYMBOL INT32 a_mtkapi_hidh_get_output_report(CHAR *pbt_addr, UINT8 reportId, INT32 bufferSize)
{
    BT_HIDH_LOG("a_mtkapi_hidh_get_output_report");
    RPC_CLIENT_DECL(3, INT32);
    RPC_CLIENT_ARG_INP(ARG_TYPE_REF_STR, pbt_addr);
    RPC_CLIENT_ARG_INP(ARG_TYPE_UINT8, reportId);
    RPC_CLIENT_ARG_INP(ARG_TYPE_INT32, bufferSize);
    RPC_BT_SERVICE_CLIENT_DO_OP("x_mtkapi_hidh_get_output_report");
    RPC_CLIENT_RETURN(ARG_TYPE_INT32, -1);
}

EXPORT_SYMBOL INT32 a_mtkapi_hidh_set_input_report(CHAR *pbt_addr, CHAR *preport_data)
{
    BT_HIDH_LOG("a_mtkapi_hidh_set_input_report");
    RPC_CLIENT_DECL(2, INT32);
    RPC_CLIENT_ARG_INP(ARG_TYPE_REF_STR, pbt_addr);
    RPC_CLIENT_ARG_INP(ARG_TYPE_REF_STR, preport_data);
    RPC_BT_SERVICE_CLIENT_DO_OP("x_mtkapi_hidh_set_input_report");
    RPC_CLIENT_RETURN(ARG_TYPE_INT32, -1);
}

EXPORT_SYMBOL INT32 a_mtkapi_hidh_get_feature_report(CHAR *pbt_addr, UINT8 reportId, INT32 bufferSize)
{
    BT_HIDH_LOG("a_mtkapi_hidh_get_feature_report");
    RPC_CLIENT_DECL(3, INT32);
    RPC_CLIENT_ARG_INP(ARG_TYPE_REF_STR, pbt_addr);
    RPC_CLIENT_ARG_INP(ARG_TYPE_UINT8, reportId);
    RPC_CLIENT_ARG_INP(ARG_TYPE_INT32, bufferSize);
    RPC_BT_SERVICE_CLIENT_DO_OP("x_mtkapi_hidh_get_feature_report");
    RPC_CLIENT_RETURN(ARG_TYPE_INT32, -1);
}

EXPORT_SYMBOL INT32 a_mtkapi_hidh_set_feature_report(CHAR *pbt_addr, CHAR *preport_data)
{
    BT_HIDH_LOG("a_mtkapi_hidh_set_feature_report");
    RPC_CLIENT_DECL(2, INT32);
    RPC_CLIENT_ARG_INP(ARG_TYPE_REF_STR, pbt_addr);
    RPC_CLIENT_ARG_INP(ARG_TYPE_REF_STR, preport_data);
    RPC_BT_SERVICE_CLIENT_DO_OP("x_mtkapi_hidh_set_feature_report");
    RPC_CLIENT_RETURN(ARG_TYPE_INT32, -1);
}

EXPORT_SYMBOL INT32 a_mtkapi_hidh_get_protocol(CHAR *pbt_addr)
{
    BT_HIDH_LOG("a_mtkapi_hidh_get_protocol");
    RPC_CLIENT_DECL(1, INT32);
    RPC_CLIENT_ARG_INP(ARG_TYPE_REF_STR, pbt_addr);
    RPC_BT_SERVICE_CLIENT_DO_OP("x_mtkapi_hidh_get_protocol");
    RPC_CLIENT_RETURN(ARG_TYPE_INT32, -1);
}

EXPORT_SYMBOL INT32 a_mtkapi_hidh_set_protocol(CHAR *pbt_addr, UINT8 protocol_mode)
{
    BT_HIDH_LOG("a_mtkapi_hidh_set_protocol");
    RPC_CLIENT_DECL(2, INT32);
    RPC_CLIENT_ARG_INP(ARG_TYPE_REF_STR, pbt_addr);
    RPC_CLIENT_ARG_INP(ARG_TYPE_UINT8, protocol_mode);
    RPC_BT_SERVICE_CLIENT_DO_OP("x_mtkapi_hidh_set_protocol");
    RPC_CLIENT_RETURN(ARG_TYPE_INT32, -1);
}

EXPORT_SYMBOL INT32 a_mtkapi_hidh_send_control(CHAR *pbt_addr, UINT8 pcontrol_mode)
{
    BT_HIDH_LOG("a_mtkapi_hidh_send_control");
    RPC_CLIENT_DECL(2, INT32);
    RPC_CLIENT_ARG_INP(ARG_TYPE_REF_STR, pbt_addr);
    RPC_CLIENT_ARG_INP(ARG_TYPE_UINT8, pcontrol_mode);
    RPC_BT_SERVICE_CLIENT_DO_OP("x_mtkapi_hidh_send_control");
    RPC_CLIENT_RETURN(ARG_TYPE_INT32, -1);
}

EXPORT_SYMBOL INT32 a_mtkapi_bluetooth_hidh_set_output_report(CHAR *pbt_addr, CHAR *preport_data)
{
    BT_HIDH_LOG("a_mtkapi_bluetooth_hidh_set_output_report");
    RPC_CLIENT_DECL(2, INT32);
    RPC_CLIENT_ARG_INP(ARG_TYPE_REF_STR, pbt_addr);
    RPC_CLIENT_ARG_INP(ARG_TYPE_REF_STR, preport_data);
    RPC_BT_SERVICE_CLIENT_DO_OP("x_mtkapi_bluetooth_hidh_set_output_report");
    RPC_CLIENT_RETURN(ARG_TYPE_INT32, -1);
}

EXPORT_SYMBOL INT32 a_mtkapi_get_paired_hidh_dev_list(BT_HID_TARGET_DEV_LIST *pt_device_list, BT_HID_STATUS_STRUCT_LIST *pt_hid_status_list)
{
    BT_HIDH_LOG("a_mtkapi_get_paired_hidh_dev_list");
    RPC_CLIENT_DECL(2, INT32);
    RPC_CLIENT_CHECK(bt_rpc_add_ref_desc(RPC_DEFAULT_ID,
                    pt_device_list,
                    RPC_DESC_BT_HID_TARGET_DEV_LIST,
                    NULL));
    RPC_CLIENT_ARG_OUT(ARG_TYPE_REF_DESC, pt_device_list);
    RPC_CLIENT_CHECK(bt_rpc_add_ref_desc(RPC_DEFAULT_ID,
                    pt_hid_status_list,
                    RPC_DESC_BT_HID_STATUS_STRUCT_LIST,
                    NULL));
    RPC_CLIENT_ARG_OUT(ARG_TYPE_REF_DESC, pt_hid_status_list);
    RPC_BT_SERVICE_CLIENT_DO_OP("x_mtkapi_get_paired_hidh_dev_list");
    RPC_CLIENT_RETURN(ARG_TYPE_INT32, -1);
}

EXPORT_SYMBOL INT32 a_mtkapi_del_paired_hidh_dev_one(BT_TARGET_DEV_INFO *pt_device_info)
{
    BT_HIDH_LOG("a_mtkapi_del_paired_hidh_dev_one");
    RPC_CLIENT_DECL(1, INT32);
    RPC_CLIENT_CHECK(bt_rpc_add_ref_desc(RPC_DEFAULT_ID,
                    pt_device_info,
                    RPC_DESC_BT_TARGET_DEV_INFO,
                    NULL));
    RPC_CLIENT_ARG_INP(ARG_TYPE_REF_DESC, pt_device_info);
    RPC_BT_SERVICE_CLIENT_DO_OP("x_mtkapi_del_paired_hidh_dev_one");
    RPC_CLIENT_RETURN(ARG_TYPE_INT32, -1);
}

EXPORT_SYMBOL INT32 a_mtkapi_del_paired_hidh_dev_all(VOID)
{
    BT_HIDH_LOG("a_mtkapi_del_paired_hidh_dev_all");
    //Adding a invalid value for passing IPC/RPC, no other use
    INT32 i4_InvalidValue = 0;
    RPC_CLIENT_DECL(1, INT32);
    RPC_CLIENT_ARG_INP(ARG_TYPE_INT32, i4_InvalidValue);
    RPC_BT_SERVICE_CLIENT_DO_OP("x_mtkapi_del_paired_hidh_dev_all");
    RPC_CLIENT_RETURN(ARG_TYPE_INT32, -1);
}

EXPORT_SYMBOL INT32 a_mtkapi_connect_hidh_sources(CHAR *addr)
{
    BT_HIDH_LOG("a_mtkapi_connect_hidh_sources");
    RPC_CLIENT_DECL(1, INT32);
    RPC_CLIENT_ARG_INP(ARG_TYPE_REF_STR, addr);
    RPC_BT_SERVICE_CLIENT_DO_OP("x_mtkapi_connect_hidh_sources");
    RPC_CLIENT_RETURN(ARG_TYPE_INT32, -1);
}

EXPORT_SYMBOL INT32 a_mtkapi_disconnect_hidh_sources(CHAR *addr)
{
    BT_HIDH_LOG("a_mtkapi_disconnect_hidh_sources");
    RPC_CLIENT_DECL(1, INT32);
    RPC_CLIENT_ARG_INP(ARG_TYPE_REF_STR, addr);
    RPC_BT_SERVICE_CLIENT_DO_OP("x_mtkapi_disconnect_hidh_sources");
    RPC_CLIENT_RETURN(ARG_TYPE_INT32, -1);
}

EXPORT_SYMBOL INT32 a_mtkapi_hidh_auto_disconnection(VOID)
{
    BT_HIDH_LOG("a_mtkapi_hidh_auto_disconnection");
    //Adding a invalid value for passing IPC/RPC, no other use
    INT32 i4_InvalidValue = 0;
    RPC_CLIENT_DECL(1, INT32);
    RPC_CLIENT_ARG_INP(ARG_TYPE_INT32, i4_InvalidValue);
    RPC_BT_SERVICE_CLIENT_DO_OP("x_mtkapi_hidh_auto_disconnection");
    RPC_CLIENT_RETURN(ARG_TYPE_INT32, -1);
}

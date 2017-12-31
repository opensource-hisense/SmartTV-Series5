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

#include "mtk_bt_service_a2dp_wrapper.h"
#include "mtk_bt_service_a2dp_ipcrpc_struct.h"
#include "client_common.h"

#define BT_A2DP_LOG(_stmt...) \
        do{ \
            if(0){    \
                printf("[A2DP]Func:%s Line:%d--->: ", __FUNCTION__, __LINE__);   \
                printf(_stmt); \
                printf("\n"); \
            }        \
        }   \
        while(0)


EXPORT_SYMBOL INT32 a_mtkapi_a2dp_src_set_audio_hw_dbg_lvl(UINT8 log_level)
{
    BT_A2DP_LOG("a_mtkapi_a2dp_src_set_audio_hw_dbg_lvl");
    RPC_CLIENT_DECL(1, INT32);
    RPC_CLIENT_ARG_INP(ARG_TYPE_UINT8, log_level);
    RPC_BT_SERVICE_CLIENT_DO_OP("x_mtkapi_a2dp_src_set_audio_hw_dbg_lvl");
    RPC_CLIENT_RETURN(ARG_TYPE_INT32, -1);
}

EXPORT_SYMBOL INT32 a_mtkapi_a2dp_source_sink_on_off(BOOL fg_src_on, BOOL fg_sink_on)
{
    BT_A2DP_LOG("a_mtkapi_a2dp_source_sink_on_off");
    RPC_CLIENT_DECL(2, INT32);
    RPC_CLIENT_ARG_INP(ARG_TYPE_BOOL, fg_src_on);
    RPC_CLIENT_ARG_INP(ARG_TYPE_BOOL, fg_sink_on);
    RPC_BT_SERVICE_CLIENT_DO_OP("x_mtkapi_a2dp_source_sink_on_off");
    RPC_CLIENT_RETURN(ARG_TYPE_INT32, -1);
}

EXPORT_SYMBOL INT32 a_mtkapi_a2dp_connect_audio_sources(char *addr, audio_conn_type_t type)
{
    BT_A2DP_LOG("a_mtkapi_a2dp_connect_audio_sources");
    RPC_CLIENT_DECL(2, INT32);
    RPC_CLIENT_ARG_INP(ARG_TYPE_REF_STR, addr);
    RPC_CLIENT_ARG_INP(ARG_TYPE_INT32, type);
    RPC_BT_SERVICE_CLIENT_DO_OP("x_mtkapi_a2dp_connect_audio_sources");
    RPC_CLIENT_RETURN(ARG_TYPE_INT32, -1);
}


EXPORT_SYMBOL INT32 a_mtkapi_a2dp_disconnect_audio_sources(char *addr, audio_conn_type_t type)
{
    BT_A2DP_LOG("a_mtkapi_a2dp_disconnect_audio_sources");
    RPC_CLIENT_DECL(2, INT32);
    RPC_CLIENT_ARG_INP(ARG_TYPE_REF_STR, addr);
    RPC_CLIENT_ARG_INP(ARG_TYPE_INT32, type);
    RPC_BT_SERVICE_CLIENT_DO_OP("x_mtkapi_a2dp_disconnect_audio_sources");
    RPC_CLIENT_RETURN(ARG_TYPE_INT32, -1);
}

EXPORT_SYMBOL INT32 a_mtkapi_a2dp_send_stream_data(const CHAR *data, INT32 len)
{
    BT_A2DP_LOG("a_mtkapi_a2dp_send_stream_data");
    RPC_CLIENT_DECL(2, INT32);
    RPC_CLIENT_CHECK(bt_rpc_add_ref_buff(RPC_DEFAULT_ID, data, len));
    RPC_CLIENT_ARG_INP(ARG_TYPE_INT32, len);
    RPC_BT_SERVICE_CLIENT_DO_OP("x_mtkapi_a2dp_send_stream_data");
    RPC_CLIENT_RETURN(ARG_TYPE_INT32, -1);
}

EXPORT_SYMBOL INT32 a_mtkapi_a2dp_get_role(VOID)
{
    BT_A2DP_LOG("a_mtkapi_a2dp_get_role");
    INT32 i4_InvalidValue = 0;
    RPC_CLIENT_DECL(1, INT32);
    RPC_CLIENT_ARG_INP(ARG_TYPE_INT32, i4_InvalidValue);
    RPC_BT_SERVICE_CLIENT_DO_OP("x_mtkapi_a2dp_get_role");
    RPC_CLIENT_RETURN(ARG_TYPE_INT32, -1);
}

EXPORT_SYMBOL INT32 a_mtkapi_a2dp_stream_is_suspend(VOID)
{
    BT_A2DP_LOG("a_mtkapi_a2dp_stream_is_suspend");
    INT32 i4_InvalidValue = 0;
    RPC_CLIENT_DECL(1, INT32);
    RPC_CLIENT_ARG_INP(ARG_TYPE_INT32, i4_InvalidValue);
    RPC_BT_SERVICE_CLIENT_DO_OP("x_mtkapi_a2dp_stream_is_suspend");
    RPC_CLIENT_RETURN(ARG_TYPE_INT32, -1);
}

EXPORT_SYMBOL INT32 a_mtkapi_a2dp_get_connect_status(VOID)
{
    BT_A2DP_LOG("a_mtkapi_a2dp_get_connect_status");
    INT32 i4_InvalidValue = 0;
    RPC_CLIENT_DECL(1, INT32);
    RPC_CLIENT_ARG_INP(ARG_TYPE_INT32, i4_InvalidValue);
    RPC_BT_SERVICE_CLIENT_DO_OP("x_mtkapi_a2dp_get_connect_status");
    RPC_CLIENT_RETURN(ARG_TYPE_INT32, -1);
}

EXPORT_SYMBOL VOID a_mtkapi_a2dp_cmd(bt_a2dp_status_t status)
{
    BT_A2DP_LOG("a_mtkapi_a2dp_cmd");
    RPC_CLIENT_DECL_VOID(1);
    RPC_CLIENT_ARG_INP(ARG_TYPE_INT32, status);
    RPC_BT_SERVICE_CLIENT_DO_OP("x_mtkapi_a2dp_cmd");
}

EXPORT_SYMBOL INT32 a_mtkapi_a2dp_get_paired_sink_dev_list(BT_TARGET_DEV_LIST *pt_device_list)
{
    BT_A2DP_LOG("a_mtkapi_a2dp_get_paired_sink_dev_list");
    RPC_CLIENT_DECL(1, INT32);
    RPC_CLIENT_CHECK(bt_rpc_add_ref_desc(RPC_DEFAULT_ID,
                    pt_device_list,
                    RPC_DESC_BT_TARGET_DEV_LIST,
                    NULL));
    RPC_CLIENT_ARG_OUT(ARG_TYPE_REF_DESC, pt_device_list);
    RPC_BT_SERVICE_CLIENT_DO_OP("x_mtkapi_a2dp_get_paired_sink_dev_list");
    RPC_CLIENT_RETURN(ARG_TYPE_INT32, -1);
}

EXPORT_SYMBOL INT32 a_mtkapi_a2dp_get_paired_src_dev_list(BT_TARGET_DEV_LIST *pt_device_list)
{
    BT_A2DP_LOG("a_mtkapi_a2dp_get_paired_src_dev_list");
    RPC_CLIENT_DECL(1, INT32);
    RPC_CLIENT_CHECK(bt_rpc_add_ref_desc(RPC_DEFAULT_ID,
                    pt_device_list,
                    RPC_DESC_BT_TARGET_DEV_LIST,
                    NULL));
    RPC_CLIENT_ARG_OUT(ARG_TYPE_REF_DESC, pt_device_list);
    RPC_BT_SERVICE_CLIENT_DO_OP("x_mtkapi_a2dp_get_paired_src_dev_list");
    RPC_CLIENT_RETURN(ARG_TYPE_INT32, -1);
}

EXPORT_SYMBOL VOID a_mtkapi_a2dp_mute_device(UINT8 fg_mute)
{
    BT_A2DP_LOG("a_mtkapi_a2dp_mute_device");
    RPC_CLIENT_DECL_VOID(1);
    RPC_CLIENT_ARG_INP(ARG_TYPE_UINT8, fg_mute);
    RPC_BT_SERVICE_CLIENT_DO_OP("x_mtkapi_a2dp_mute_device");
}

INT32 c_rpc_reg_mtk_bt_service_a2dp_cb_hndlrs(VOID)
{
    return RPCR_OK;
}


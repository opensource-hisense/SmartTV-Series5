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

#include "mtk_bt_service_hfclient_wrapper.h"
#include "mtk_bt_service_hfclient_ipcrpc_struct.h"
#include "client_common.h"

#define BT_RW_LOG(_stmt...) \
        do{ \
            if(0){    \
                printf("[Client]Func:%s Line:%d--->: ", __FUNCTION__, __LINE__);   \
                printf(_stmt); \
                printf("\n"); \
            }        \
        }   \
        while(0)

EXPORT_SYMBOL INT32 a_mtkapi_bt_hfclient_connect(CHAR *bt_addr)
{
    BT_RW_LOG("a_mtkapi_bt_hfclient_connect");
    RPC_CLIENT_DECL(1, INT32);
    RPC_CLIENT_ARG_INP(ARG_TYPE_REF_STR, bt_addr);
    RPC_BT_SERVICE_CLIENT_DO_OP("x_mtkapi_bt_hfclient_connect");
    RPC_CLIENT_RETURN(ARG_TYPE_INT32, -1);
}

EXPORT_SYMBOL INT32 a_mtkapi_bt_hfclient_disconnect(CHAR *bt_addr)
{
    BT_RW_LOG("a_mtkapi_bt_hfclient_disconnect");
    RPC_CLIENT_DECL(1, INT32);
    RPC_CLIENT_ARG_INP(ARG_TYPE_REF_STR, bt_addr);
    RPC_BT_SERVICE_CLIENT_DO_OP("x_mtkapi_bt_hfclient_disconnect");
    RPC_CLIENT_RETURN(ARG_TYPE_INT32, -1);
}

EXPORT_SYMBOL INT32 a_mtkapi_bt_hfclient_connect_audio(CHAR *bt_addr)
{
    BT_RW_LOG("a_mtkapi_bt_hfclient_connect_audio");
    RPC_CLIENT_DECL(1, INT32);
    RPC_CLIENT_ARG_INP(ARG_TYPE_REF_STR, bt_addr);
    RPC_BT_SERVICE_CLIENT_DO_OP("x_mtkapi_bt_hfclient_connect_audio");
    RPC_CLIENT_RETURN(ARG_TYPE_INT32, -1);
}

EXPORT_SYMBOL INT32 a_mtkapi_bt_hfclient_disconnect_audio(CHAR *bt_addr)
{
    BT_RW_LOG("a_mtkapi_bt_hfclient_disconnect_audio");
    RPC_CLIENT_DECL(1, INT32);
    RPC_CLIENT_ARG_INP(ARG_TYPE_REF_STR, bt_addr);
    RPC_BT_SERVICE_CLIENT_DO_OP("x_mtkapi_bt_hfclient_disconnect_audio");
    RPC_CLIENT_RETURN(ARG_TYPE_INT32, -1);
}

EXPORT_SYMBOL INT32 a_mtkapi_bt_hfclient_start_voice_recognition(VOID)
{
    INT32 i4_InvalidValue = 0;
    BT_RW_LOG("a_mtkapi_bt_hfclient_start_voice_recognition");
    RPC_CLIENT_DECL(1, INT32);
    RPC_CLIENT_ARG_INP(ARG_TYPE_INT32, i4_InvalidValue);
    RPC_BT_SERVICE_CLIENT_DO_OP("x_mtkapi_bt_hfclient_start_voice_recognition");
    RPC_CLIENT_RETURN(ARG_TYPE_INT32, -1);
}

EXPORT_SYMBOL INT32 a_mtkapi_bt_hfclient_stop_voice_recognition(VOID)
{
    INT32 i4_InvalidValue = 0;
    BT_RW_LOG("a_mtkapi_bt_hfclient_stop_voice_recognition");
    RPC_CLIENT_DECL(1, INT32);
    RPC_CLIENT_ARG_INP(ARG_TYPE_INT32, i4_InvalidValue);
    RPC_BT_SERVICE_CLIENT_DO_OP("x_mtkapi_bt_hfclient_stop_voice_recognition");
    RPC_CLIENT_RETURN(ARG_TYPE_INT32, -1);
}

EXPORT_SYMBOL INT32 a_mtkapi_bt_hfclient_volume_control(BT_HFCLIENT_VOLUME_TYPE_T type, INT32 volume)
{
    BT_RW_LOG("a_mtkapi_bt_hfclient_volume_control");
    RPC_CLIENT_DECL(2, INT32);
    RPC_CLIENT_ARG_INP(ARG_TYPE_INT32, type);
    RPC_CLIENT_ARG_INP(ARG_TYPE_INT32, volume);
    RPC_BT_SERVICE_CLIENT_DO_OP("x_mtkapi_bt_hfclient_volume_control");
    RPC_CLIENT_RETURN(ARG_TYPE_INT32, -1);
}

EXPORT_SYMBOL INT32 a_mtkapi_bt_hfclient_dial(const CHAR *number)
{
    BT_RW_LOG("a_mtkapi_bt_hfclient_dial");
    RPC_CLIENT_DECL(1, INT32);
    RPC_CLIENT_ARG_INP(ARG_TYPE_REF_STR, number);
    RPC_BT_SERVICE_CLIENT_DO_OP("x_mtkapi_bt_hfclient_dial");
    RPC_CLIENT_RETURN(ARG_TYPE_INT32, -1);
}

EXPORT_SYMBOL INT32 a_mtkapi_bt_hfclient_dial_memory(INT32 location)
{
    BT_RW_LOG("a_mtkapi_bt_hfclient_dial_memory");
    RPC_CLIENT_DECL(1, INT32);
    RPC_CLIENT_ARG_INP(ARG_TYPE_INT32, location);
    RPC_BT_SERVICE_CLIENT_DO_OP("x_mtkapi_bt_hfclient_dial_memory");
    RPC_CLIENT_RETURN(ARG_TYPE_INT32, -1);
}

EXPORT_SYMBOL INT32 a_mtkapi_bt_hfclient_handle_call_action(BT_HFCLIENT_CALL_ACTION_T action, INT32 idx)
{
    BT_RW_LOG("a_mtkapi_bt_hfclient_handle_call_action");
    RPC_CLIENT_DECL(2, INT32);
    RPC_CLIENT_ARG_INP(ARG_TYPE_INT32, action);
    RPC_CLIENT_ARG_INP(ARG_TYPE_INT32, idx);
    RPC_BT_SERVICE_CLIENT_DO_OP("x_mtkapi_bt_hfclient_handle_call_action");
    RPC_CLIENT_RETURN(ARG_TYPE_INT32, -1);
}

EXPORT_SYMBOL INT32 a_mtkapi_bt_hfclient_query_current_calls(VOID)
{
    INT32 i4_InvalidValue = 0;
    BT_RW_LOG("a_mtkapi_bt_hfclient_query_current_calls");
    RPC_CLIENT_DECL(1, INT32);
    RPC_CLIENT_ARG_INP(ARG_TYPE_INT32, i4_InvalidValue);
    RPC_BT_SERVICE_CLIENT_DO_OP("x_mtkapi_bt_hfclient_query_current_calls");
    RPC_CLIENT_RETURN(ARG_TYPE_INT32, -1);
}

EXPORT_SYMBOL INT32 a_mtkapi_bt_hfclient_query_current_operator_name(VOID)
{
    INT32 i4_InvalidValue = 0;
    BT_RW_LOG("a_mtkapi_bt_hfclient_query_current_operator_name");
    RPC_CLIENT_DECL(1, INT32);
    RPC_CLIENT_ARG_INP(ARG_TYPE_INT32, i4_InvalidValue);
    RPC_BT_SERVICE_CLIENT_DO_OP("x_mtkapi_bt_hfclient_query_current_operator_name");
    RPC_CLIENT_RETURN(ARG_TYPE_INT32, -1);
}

EXPORT_SYMBOL INT32 a_mtkapi_bt_hfclient_retrieve_subscriber_info(VOID)
{
    INT32 i4_InvalidValue = 0;
    BT_RW_LOG("a_mtkapi_bt_hfclient_retrieve_subscriber_info");
    RPC_CLIENT_DECL(1, INT32);
    RPC_CLIENT_ARG_INP(ARG_TYPE_INT32, i4_InvalidValue);
    RPC_BT_SERVICE_CLIENT_DO_OP("x_mtkapi_bt_hfclient_retrieve_subscriber_info");
    RPC_CLIENT_RETURN(ARG_TYPE_INT32, -1);
}

EXPORT_SYMBOL INT32 a_mtkapi_bt_hfclient_send_dtmf(CHAR code)
{
    BT_RW_LOG("a_mtkapi_bt_hfclient_send_dtmf");
    RPC_CLIENT_DECL(1, INT32);
    RPC_CLIENT_ARG_INP(ARG_TYPE_CHAR, code);
    RPC_BT_SERVICE_CLIENT_DO_OP("x_mtkapi_bt_hfclient_send_dtmf");
    RPC_CLIENT_RETURN(ARG_TYPE_INT32, -1);
}

EXPORT_SYMBOL INT32 a_mtkapi_bt_hfclient_request_last_voice_tag_number(VOID)
{
    INT32 i4_InvalidValue = 0;
    BT_RW_LOG("a_mtkapi_bt_hfclient_request_last_voice_tag_number");
    RPC_CLIENT_DECL(1, INT32);
    RPC_CLIENT_ARG_INP(ARG_TYPE_INT32, i4_InvalidValue);
    RPC_BT_SERVICE_CLIENT_DO_OP("x_mtkapi_bt_hfclient_request_last_voice_tag_number");
    RPC_CLIENT_RETURN(ARG_TYPE_INT32, -1);
}

EXPORT_SYMBOL INT32 a_mtkapi_bt_hfclient_send_at_cmd(INT32 cmd, INT32 val1, INT32 val2, const CHAR *arg)
{
    BT_RW_LOG("a_mtkapi_bt_hfclient_send_at_cmd");
    RPC_CLIENT_DECL(4, INT32);
    RPC_CLIENT_ARG_INP(ARG_TYPE_INT32, cmd);
    RPC_CLIENT_ARG_INP(ARG_TYPE_INT32, val1);
    RPC_CLIENT_ARG_INP(ARG_TYPE_INT32, val2);
    RPC_CLIENT_ARG_INP(ARG_TYPE_REF_STR, arg);
    RPC_BT_SERVICE_CLIENT_DO_OP("x_mtkapi_bt_hfclient_send_at_cmd");
    RPC_CLIENT_RETURN(ARG_TYPE_INT32, -1);
}

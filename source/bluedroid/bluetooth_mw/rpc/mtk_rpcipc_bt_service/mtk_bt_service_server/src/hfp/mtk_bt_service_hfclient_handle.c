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
#include "mtk_bt_service_hfclient.h"
#include "mtk_bt_service_hfclient_handle.h"
#include "u_rpc.h"
#include "ri_common.h"

#define BT_RH_LOG(_stmt...) \
        do{ \
            if(0){    \
                printf("Func:%s Line:%d--->: ", __FUNCTION__, __LINE__);   \
                printf(_stmt); \
                printf("\n"); \
            }        \
        }   \
        while(0)

static INT32 _hndlr_x_mtkapi_bt_hfclient_connect(
                         RPC_ID_T     t_rpc_id,
                         const CHAR*  ps_cb_type,
                         UINT32       ui4_num_args,
                         ARG_DESC_T*  pt_args,
                         ARG_DESC_T*  pt_return)
{
    if (ui4_num_args != 1)
    {
        BT_RH_LOG("Invalid ARGS: %ld\n", ui4_num_args);
        return RPCR_INV_ARGS;
    }

    BT_RH_LOG("arg_1 = %s\n", pt_args[0].u.pc_arg);
    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_mtkapi_bt_hfclient_connect(pt_args[0].u.pc_arg);

    return RPCR_OK;
}

static INT32 _hndlr_x_mtkapi_bt_hfclient_disconnect(
                         RPC_ID_T     t_rpc_id,
                         const CHAR*  ps_cb_type,
                         UINT32       ui4_num_args,
                         ARG_DESC_T*  pt_args,
                         ARG_DESC_T*  pt_return)
{
    if (ui4_num_args != 1)
    {
        BT_RH_LOG("Invalid ARGS: %ld\n", ui4_num_args);
        return RPCR_INV_ARGS;
    }

    BT_RH_LOG("arg_1 = %s\n", pt_args[0].u.pc_arg);
    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_mtkapi_bt_hfclient_disconnect(pt_args[0].u.pc_arg);

    return RPCR_OK;
}

static INT32 _hndlr_x_mtkapi_bt_hfclient_connect_audio(
                         RPC_ID_T     t_rpc_id,
                         const CHAR*  ps_cb_type,
                         UINT32       ui4_num_args,
                         ARG_DESC_T*  pt_args,
                         ARG_DESC_T*  pt_return)
{
    if (ui4_num_args != 1)
    {
        BT_RH_LOG("Invalid ARGS: %ld\n", ui4_num_args);
        return RPCR_INV_ARGS;
    }

    BT_RH_LOG("arg_1 = %s\n", pt_args[0].u.pc_arg);
    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_mtkapi_bt_hfclient_connect_audio(pt_args[0].u.pc_arg);

    return RPCR_OK;
}

static INT32 _hndlr_x_mtkapi_bt_hfclient_disconnect_audio(
                         RPC_ID_T     t_rpc_id,
                         const CHAR*  ps_cb_type,
                         UINT32       ui4_num_args,
                         ARG_DESC_T*  pt_args,
                         ARG_DESC_T*  pt_return)
{
    if (ui4_num_args != 1)
    {
        BT_RH_LOG("Invalid ARGS: %ld\n", ui4_num_args);
        return RPCR_INV_ARGS;
    }

    BT_RH_LOG("arg_1 = %s\n", pt_args[0].u.pc_arg);
    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_mtkapi_bt_hfclient_disconnect_audio(pt_args[0].u.pc_arg);

    return RPCR_OK;
}

static INT32 _hndlr_x_mtkapi_bt_hfclient_start_voice_recognition(
                         RPC_ID_T     t_rpc_id,
                         const CHAR*  ps_cb_type,
                         UINT32       ui4_num_args,
                         ARG_DESC_T*  pt_args,
                         ARG_DESC_T*  pt_return)
{
    BT_RH_LOG("\n");
    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_mtkapi_bt_hfclient_start_voice_recognition();

    return RPCR_OK;
}

static INT32 _hndlr_x_mtkapi_bt_hfclient_stop_voice_recognition(
                         RPC_ID_T     t_rpc_id,
                         const CHAR*  ps_cb_type,
                         UINT32       ui4_num_args,
                         ARG_DESC_T*  pt_args,
                         ARG_DESC_T*  pt_return)
{
    BT_RH_LOG("\n");
    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_mtkapi_bt_hfclient_stop_voice_recognition();

    return RPCR_OK;
}

static INT32 _hndlr_x_mtkapi_bt_hfclient_volume_control(
                         RPC_ID_T     t_rpc_id,
                         const CHAR*  ps_cb_type,
                         UINT32       ui4_num_args,
                         ARG_DESC_T*  pt_args,
                         ARG_DESC_T*  pt_return)
{
    if (ui4_num_args != 2)
    {
        BT_RH_LOG("Invalid ARGS: %ld\n", ui4_num_args);
        return RPCR_INV_ARGS;
    }

    BT_RH_LOG("arg_1 = %ld, arg_2 = %ld\n", pt_args[0].u.i4_arg, pt_args[1].u.i4_arg);
    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_mtkapi_bt_hfclient_volume_control(pt_args[0].u.i4_arg, pt_args[1].u.i4_arg);

    return RPCR_OK;
}

static INT32 _hndlr_x_mtkapi_bt_hfclient_dial(
                         RPC_ID_T     t_rpc_id,
                         const CHAR*  ps_cb_type,
                         UINT32       ui4_num_args,
                         ARG_DESC_T*  pt_args,
                         ARG_DESC_T*  pt_return)
{
    if (ui4_num_args != 1)
    {
        BT_RH_LOG("Invalid ARGS: %ld\n", ui4_num_args);
        return RPCR_INV_ARGS;
    }

    BT_RH_LOG("arg_1 = %s\n", pt_args[0].u.pc_arg);
    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_mtkapi_bt_hfclient_dial(pt_args[0].u.pc_arg);

    return RPCR_OK;
}

static INT32 _hndlr_x_mtkapi_bt_hfclient_dial_memory(
                         RPC_ID_T     t_rpc_id,
                         const CHAR*  ps_cb_type,
                         UINT32       ui4_num_args,
                         ARG_DESC_T*  pt_args,
                         ARG_DESC_T*  pt_return)
{
    if (ui4_num_args != 1)
    {
        BT_RH_LOG("Invalid ARGS: %ld\n", ui4_num_args);
        return RPCR_INV_ARGS;
    }

    BT_RH_LOG("arg_1 = %ld\n", pt_args[0].u.i4_arg);
    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_mtkapi_bt_hfclient_dial_memory(pt_args[0].u.i4_arg);

    return RPCR_OK;
}

static INT32 _hndlr_x_mtkapi_bt_hfclient_handle_call_action(
                         RPC_ID_T     t_rpc_id,
                         const CHAR*  ps_cb_type,
                         UINT32       ui4_num_args,
                         ARG_DESC_T*  pt_args,
                         ARG_DESC_T*  pt_return)
{
    if (ui4_num_args != 2)
    {
        BT_RH_LOG("Invalid ARGS: %ld\n", ui4_num_args);
        return RPCR_INV_ARGS;
    }

    BT_RH_LOG("arg_1 = %ld, arg_2 = %ld\n", pt_args[0].u.i4_arg, pt_args[1].u.i4_arg);
    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_mtkapi_bt_hfclient_handle_call_action(pt_args[0].u.i4_arg, pt_args[1].u.i4_arg);

    return RPCR_OK;
}

static INT32 _hndlr_x_mtkapi_bt_hfclient_query_current_calls(
                         RPC_ID_T     t_rpc_id,
                         const CHAR*  ps_cb_type,
                         UINT32       ui4_num_args,
                         ARG_DESC_T*  pt_args,
                         ARG_DESC_T*  pt_return)
{
    BT_RH_LOG("\n");
    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_mtkapi_bt_hfclient_query_current_calls();

    return RPCR_OK;
}

static INT32 _hndlr_x_mtkapi_bt_hfclient_query_current_operator_name(
                         RPC_ID_T     t_rpc_id,
                         const CHAR*  ps_cb_type,
                         UINT32       ui4_num_args,
                         ARG_DESC_T*  pt_args,
                         ARG_DESC_T*  pt_return)
{
    BT_RH_LOG("\n");
    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_mtkapi_bt_hfclient_query_current_operator_name();

    return RPCR_OK;
}

static INT32 _hndlr_x_mtkapi_bt_hfclient_retrieve_subscriber_info(
                         RPC_ID_T     t_rpc_id,
                         const CHAR*  ps_cb_type,
                         UINT32       ui4_num_args,
                         ARG_DESC_T*  pt_args,
                         ARG_DESC_T*  pt_return)
{
    BT_RH_LOG("\n");
    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_mtkapi_bt_hfclient_retrieve_subscriber_info();

    return RPCR_OK;
}

static INT32 _hndlr_x_mtkapi_bt_hfclient_send_dtmf(
                         RPC_ID_T     t_rpc_id,
                         const CHAR*  ps_cb_type,
                         UINT32       ui4_num_args,
                         ARG_DESC_T*  pt_args,
                         ARG_DESC_T*  pt_return)
{
    if (ui4_num_args != 1)
    {
        BT_RH_LOG("Invalid ARGS: %ld\n", ui4_num_args);
        return RPCR_INV_ARGS;
    }

    BT_RH_LOG("arg_1 = %d\n", pt_args[0].u.c_arg);
    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_mtkapi_bt_hfclient_send_dtmf(pt_args[0].u.c_arg);

    return RPCR_OK;
}

static INT32 _hndlr_x_mtkapi_bt_hfclient_request_last_voice_tag_number(
                         RPC_ID_T     t_rpc_id,
                         const CHAR*  ps_cb_type,
                         UINT32       ui4_num_args,
                         ARG_DESC_T*  pt_args,
                         ARG_DESC_T*  pt_return)
{
    BT_RH_LOG("\n");
    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_mtkapi_bt_hfclient_request_last_voice_tag_number();

    return RPCR_OK;
}

static INT32 _hndlr_x_mtkapi_bt_hfclient_send_at_cmd(
                         RPC_ID_T     t_rpc_id,
                         const CHAR*  ps_cb_type,
                         UINT32       ui4_num_args,
                         ARG_DESC_T*  pt_args,
                         ARG_DESC_T*  pt_return)
{
    if (ui4_num_args != 4)
    {
        BT_RH_LOG("Invalid ARGS: %ld\n", ui4_num_args);
        return RPCR_INV_ARGS;
    }

    BT_RH_LOG("arg_1 = %ld, arg_2 = %ld, arg_3 = %ld, arg_4 = %s\n",
        pt_args[0].u.i4_arg, pt_args[1].u.i4_arg, pt_args[2].u.i4_arg, pt_args[3].u.pc_arg);
    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_mtkapi_bt_hfclient_send_at_cmd(pt_args[0].u.i4_arg, pt_args[1].u.i4_arg,
                                                           pt_args[2].u.i4_arg, pt_args[3].u.pc_arg);

    return RPCR_OK;
}

INT32 c_rpc_reg_mtk_bt_service_hfclient_op_hndlrs(VOID)
{
    RPC_REG_OP_HNDLR(x_mtkapi_bt_hfclient_connect);
    RPC_REG_OP_HNDLR(x_mtkapi_bt_hfclient_disconnect);
    RPC_REG_OP_HNDLR(x_mtkapi_bt_hfclient_connect_audio);
    RPC_REG_OP_HNDLR(x_mtkapi_bt_hfclient_disconnect_audio);
    RPC_REG_OP_HNDLR(x_mtkapi_bt_hfclient_start_voice_recognition);
    RPC_REG_OP_HNDLR(x_mtkapi_bt_hfclient_stop_voice_recognition);
    RPC_REG_OP_HNDLR(x_mtkapi_bt_hfclient_volume_control);
    RPC_REG_OP_HNDLR(x_mtkapi_bt_hfclient_dial);
    RPC_REG_OP_HNDLR(x_mtkapi_bt_hfclient_dial_memory);
    RPC_REG_OP_HNDLR(x_mtkapi_bt_hfclient_handle_call_action);
    RPC_REG_OP_HNDLR(x_mtkapi_bt_hfclient_query_current_calls);
    RPC_REG_OP_HNDLR(x_mtkapi_bt_hfclient_query_current_operator_name);
    RPC_REG_OP_HNDLR(x_mtkapi_bt_hfclient_retrieve_subscriber_info);
    RPC_REG_OP_HNDLR(x_mtkapi_bt_hfclient_send_dtmf);
    RPC_REG_OP_HNDLR(x_mtkapi_bt_hfclient_request_last_voice_tag_number);
    RPC_REG_OP_HNDLR(x_mtkapi_bt_hfclient_send_at_cmd);
    return RPCR_OK;
}

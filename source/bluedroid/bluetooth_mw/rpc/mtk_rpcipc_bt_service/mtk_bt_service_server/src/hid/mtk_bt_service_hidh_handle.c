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

#include "mtk_bt_service_hidh.h"
#include "mtk_bt_service_hidh_handle.h"
#include "u_rpc.h"
#include "ri_common.h"

#define BT_RH_LOG(_stmt...) \
        do{ \
            if(0){    \
                printf("[HIDH]Func:%s Line:%d--->: ", __FUNCTION__, __LINE__);   \
                printf(_stmt); \
                printf("\n"); \
            }        \
        }   \
        while(0)



static INT32 _hndlr_x_mtkapi_hidh_init(
                        RPC_ID_T     t_rpc_id,
                        const CHAR*  ps_cb_type,
                        UINT32       ui4_num_args,
                        ARG_DESC_T*  pt_args,
                        ARG_DESC_T*  pt_return)
{
    BT_RH_LOG("[_hndlr_]_hndlr_x_mtkapi_hidh_init, arg_1 = %ld\n", pt_args[0].u.i4_arg);

    if (ui4_num_args != 1)
    {
        BT_RH_LOG("Invalid ARGS: %ld\n", ui4_num_args);
        return RPCR_INV_ARGS;
    }

    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_mtkapi_hidh_init();

    return RPCR_OK;
}

static INT32 _hndlr_x_mtkapi_hidh_deinit(
                        RPC_ID_T     t_rpc_id,
                        const CHAR*  ps_cb_type,
                        UINT32       ui4_num_args,
                        ARG_DESC_T*  pt_args,
                        ARG_DESC_T*  pt_return)
{
    BT_RH_LOG("[_hndlr_]_hndlr_x_mtkapi_hidh_deinit, arg_1 = %ld\n", pt_args[0].u.i4_arg);

    if (ui4_num_args != 1)
    {
        BT_RH_LOG("Invalid ARGS: %ld\n", ui4_num_args);
        return RPCR_INV_ARGS;
    }

    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_mtkapi_hidh_deinit();

    return RPCR_OK;
}

static INT32 _hndlr_x_mtkapi_hidh_connect(
                        RPC_ID_T     t_rpc_id,
                        const CHAR*  ps_cb_type,
                        UINT32       ui4_num_args,
                        ARG_DESC_T*  pt_args,
                        ARG_DESC_T*  pt_return)
{
    BT_RH_LOG("[_hndlr_]_hndlr_x_mtkapi_hidh_connect, arg_1 = %s\n", pt_args[0].u.pc_arg);

    if (ui4_num_args != 1)
    {
        BT_RH_LOG("Invalid ARGS: %ld\n", ui4_num_args);
        return RPCR_INV_ARGS;
    }

    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_mtkapi_hidh_connect(pt_args[0].u.pc_arg);

    return RPCR_OK;
}

static INT32 _hndlr_x_mtkapi_hidh_disconnect(
                        RPC_ID_T     t_rpc_id,
                        const CHAR*  ps_cb_type,
                        UINT32       ui4_num_args,
                        ARG_DESC_T*  pt_args,
                        ARG_DESC_T*  pt_return)
{
    BT_RH_LOG("[_hndlr_]_hndlr_x_mtkapi_hidh_disconnect, arg_1 = %s\n", pt_args[0].u.pc_arg);

    if (ui4_num_args != 1)
    {
        BT_RH_LOG("Invalid ARGS: %ld\n", ui4_num_args);
        return RPCR_INV_ARGS;
    }

    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_mtkapi_hidh_disconnect(pt_args[0].u.pc_arg);

    return RPCR_OK;
}

static INT32 _hndlr_x_mtkapi_hidh_set_output_report(
                        RPC_ID_T     t_rpc_id,
                        const CHAR*  ps_cb_type,
                        UINT32       ui4_num_args,
                        ARG_DESC_T*  pt_args,
                        ARG_DESC_T*  pt_return)
{
    BT_RH_LOG("[_hndlr_]_hndlr_x_mtkapi_hidh_set_output_report, arg_1 = %s, arg_2 = %s\n",
        pt_args[0].u.pc_arg, pt_args[1].u.pc_arg);

    if (ui4_num_args != 2)
    {
        BT_RH_LOG("Invalid ARGS: %ld\n", ui4_num_args);
        return RPCR_INV_ARGS;
    }

    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_mtkapi_hidh_set_output_report(pt_args[0].u.pc_arg, pt_args[1].u.pc_arg);

    return RPCR_OK;
}

static INT32 _hndlr_x_mtkapi_hidh_get_input_report(
                        RPC_ID_T     t_rpc_id,
                        const CHAR*  ps_cb_type,
                        UINT32       ui4_num_args,
                        ARG_DESC_T*  pt_args,
                        ARG_DESC_T*  pt_return)
{
    BT_RH_LOG("[_hndlr_]_hndlr_x_mtkapi_hidh_get_input_report, arg_1 = %s, arg_2 = %d, arg_3 = %ld\n",
        pt_args[0].u.pc_arg, pt_args[1].u.ui1_arg, pt_args[2].u.i4_arg);

    if (ui4_num_args != 3)
    {
        BT_RH_LOG("Invalid ARGS: %ld\n", ui4_num_args);
        return RPCR_INV_ARGS;
    }

    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_mtkapi_hidh_get_input_report(pt_args[0].u.pc_arg,
                                                         pt_args[1].u.ui1_arg,
                                                         pt_args[2].u.i4_arg);

    return RPCR_OK;
}

static INT32 _hndlr_x_mtkapi_hidh_get_output_report(
                        RPC_ID_T     t_rpc_id,
                        const CHAR*  ps_cb_type,
                        UINT32       ui4_num_args,
                        ARG_DESC_T*  pt_args,
                        ARG_DESC_T*  pt_return)
{
    BT_RH_LOG("[_hndlr_]_hndlr_x_mtkapi_hidh_get_output_report, arg_1 = %s, arg_2 = %d, arg_3 = %ld\n",
        pt_args[0].u.pc_arg, pt_args[1].u.ui1_arg, pt_args[2].u.i4_arg);

    if (ui4_num_args != 3)
    {
        BT_RH_LOG("Invalid ARGS: %ld\n", ui4_num_args);
        return RPCR_INV_ARGS;
    }

    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_mtkapi_hidh_get_output_report(pt_args[0].u.pc_arg,
                                                          pt_args[1].u.ui1_arg,
                                                          pt_args[2].u.i4_arg);

    return RPCR_OK;
}

static INT32 _hndlr_x_mtkapi_hidh_set_input_report(
                        RPC_ID_T     t_rpc_id,
                        const CHAR*  ps_cb_type,
                        UINT32       ui4_num_args,
                        ARG_DESC_T*  pt_args,
                        ARG_DESC_T*  pt_return)
{
    BT_RH_LOG("[_hndlr_]_hndlr_x_mtkapi_hidh_set_input_report, arg_1 = %s, arg_2 = %s\n",
       pt_args[0].u.pc_arg, pt_args[1].u.pc_arg);

    if (ui4_num_args != 2)
    {
        BT_RH_LOG("Invalid ARGS: %ld\n", ui4_num_args);
        return RPCR_INV_ARGS;
    }

    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_mtkapi_hidh_set_input_report(pt_args[0].u.pc_arg,
                                                         pt_args[1].u.pc_arg);

    return RPCR_OK;
}

static INT32 _hndlr_x_mtkapi_hidh_get_feature_report(
                        RPC_ID_T     t_rpc_id,
                        const CHAR*  ps_cb_type,
                        UINT32       ui4_num_args,
                        ARG_DESC_T*  pt_args,
                        ARG_DESC_T*  pt_return)
{
    BT_RH_LOG("[_hndlr_]_hndlr_x_mtkapi_hidh_get_feature_report, arg_1 = %s, arg_2 = %d, arg_3 = %ld\n",
        pt_args[0].u.pc_arg, pt_args[1].u.ui1_arg, pt_args[2].u.i4_arg);

    if (ui4_num_args != 3)
    {
        BT_RH_LOG("Invalid ARGS: %ld\n", ui4_num_args);
        return RPCR_INV_ARGS;
    }

    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_mtkapi_hidh_get_feature_report(pt_args[0].u.pc_arg,
                                                           pt_args[1].u.ui1_arg,
                                                           pt_args[2].u.i4_arg);

    return RPCR_OK;
}

static INT32 _hndlr_x_mtkapi_hidh_set_feature_report(
                        RPC_ID_T     t_rpc_id,
                        const CHAR*  ps_cb_type,
                        UINT32       ui4_num_args,
                        ARG_DESC_T*  pt_args,
                        ARG_DESC_T*  pt_return)
{
    BT_RH_LOG("[_hndlr_]_hndlr_x_mtkapi_hidh_set_feature_report, arg_1 = %s, arg_2 = %s\n",
        pt_args[0].u.pc_arg, pt_args[1].u.pc_arg);

    if (ui4_num_args != 2)
    {
        BT_RH_LOG("Invalid ARGS: %ld\n", ui4_num_args);
        return RPCR_INV_ARGS;
    }

    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_mtkapi_hidh_set_feature_report(pt_args[0].u.pc_arg,
                                                           pt_args[1].u.pc_arg);

    return RPCR_OK;
}

static INT32 _hndlr_x_mtkapi_hidh_get_protocol(
                        RPC_ID_T     t_rpc_id,
                        const CHAR*  ps_cb_type,
                        UINT32       ui4_num_args,
                        ARG_DESC_T*  pt_args,
                        ARG_DESC_T*  pt_return)
{
    BT_RH_LOG("[_hndlr_]_hndlr_x_mtkapi_hidh_get_protocol, arg_1 = %s\n",
        pt_args[0].u.pc_arg);

    if (ui4_num_args != 1)
    {
        BT_RH_LOG("Invalid ARGS: %ld\n", ui4_num_args);
        return RPCR_INV_ARGS;
    }

    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_mtkapi_hidh_get_protocol(pt_args[0].u.pc_arg);

    return RPCR_OK;
}

static INT32 _hndlr_x_mtkapi_hidh_set_protocol(
                        RPC_ID_T     t_rpc_id,
                        const CHAR*  ps_cb_type,
                        UINT32       ui4_num_args,
                        ARG_DESC_T*  pt_args,
                        ARG_DESC_T*  pt_return)
{
    BT_RH_LOG("[_hndlr_]_hndlr_x_mtkapi_hidh_set_protocol, arg_1 = %s, arg_2 = %d\n",
        pt_args[0].u.pc_arg, pt_args[1].u.ui1_arg);

    if (ui4_num_args != 2)
    {
        BT_RH_LOG("Invalid ARGS: %ld\n", ui4_num_args);
        return RPCR_INV_ARGS;
    }

    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_mtkapi_hidh_set_protocol(pt_args[0].u.pc_arg,
                                                     pt_args[1].u.ui1_arg);

    return RPCR_OK;
}


static INT32 _hndlr_x_mtkapi_hidh_send_control(
                        RPC_ID_T     t_rpc_id,
                        const CHAR*  ps_cb_type,
                        UINT32       ui4_num_args,
                        ARG_DESC_T*  pt_args,
                        ARG_DESC_T*  pt_return)
{
    BT_RH_LOG("[_hndlr_]_hndlr_x_mtkapi_hidh_send_control, arg_1 = %s, arg_2 = %d\n",
        pt_args[0].u.pc_arg, pt_args[1].u.ui1_arg);

    if (ui4_num_args != 2)
    {
        BT_RH_LOG("Invalid ARGS: %ld\n", ui4_num_args);
        return RPCR_INV_ARGS;
    }

    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_mtkapi_hidh_send_control(pt_args[0].u.pc_arg,
                                                     pt_args[1].u.ui1_arg);

    return RPCR_OK;
}

static INT32 _hndlr_x_mtkapi_bluetooth_hidh_set_output_report(
                        RPC_ID_T     t_rpc_id,
                        const CHAR*  ps_cb_type,
                        UINT32       ui4_num_args,
                        ARG_DESC_T*  pt_args,
                        ARG_DESC_T*  pt_return)
{
    BT_RH_LOG("[_hndlr_]_hndlr_x_mtkapi_bluetooth_hidh_set_output_report, arg_1 = %s, arg_2 = %s\n",
        pt_args[0].u.pc_arg, pt_args[1].u.pc_arg);

    if (ui4_num_args != 2)
    {
        BT_RH_LOG("Invalid ARGS: %ld\n", ui4_num_args);
        return RPCR_INV_ARGS;
    }

    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_mtkapi_bluetooth_hidh_set_output_report(pt_args[0].u.pc_arg,
                                                                    pt_args[1].u.pc_arg);

    return RPCR_OK;
}

static INT32 _hndlr_x_mtkapi_get_paired_hidh_dev_list(
                        RPC_ID_T     t_rpc_id,
                        const CHAR*  ps_cb_type,
                        UINT32       ui4_num_args,
                        ARG_DESC_T*  pt_args,
                        ARG_DESC_T*  pt_return)
{
    BT_RH_LOG("[_hndlr_]_hndlr_x_mtkapi_get_paired_hidh_dev_list, arg_1 = %p, arg_2 = %p\n",
             pt_args[0].u.pv_desc, pt_args[1].u.pv_desc);

    if (ui4_num_args != 2)
    {
        BT_RH_LOG("Invalid ARGS: %ld\n", ui4_num_args);
        return RPCR_INV_ARGS;
    }

    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_mtkapi_get_paired_hidh_dev_list(pt_args[0].u.pv_desc,
                                                            pt_args[1].u.pv_desc);
    return RPCR_OK;
}

static INT32 _hndlr_x_mtkapi_del_paired_hidh_dev_one(
                        RPC_ID_T     t_rpc_id,
                        const CHAR*  ps_cb_type,
                        UINT32       ui4_num_args,
                        ARG_DESC_T*  pt_args,
                        ARG_DESC_T*  pt_return)
{
    BT_RH_LOG("[_hndlr_]_hndlr_x_mtkapi_del_paired_hidh_dev_one, arg_1 = %p\n",
             pt_args[0].u.pv_desc);

    if (ui4_num_args != 1)
    {
        BT_RH_LOG("Invalid ARGS: %ld\n", ui4_num_args);
        return RPCR_INV_ARGS;
    }

    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_mtkapi_del_paired_hidh_dev_one(pt_args[0].u.pv_desc);

    return RPCR_OK;
}

static INT32 _hndlr_x_mtkapi_del_paired_hidh_dev_all(
                        RPC_ID_T     t_rpc_id,
                        const CHAR*  ps_cb_type,
                        UINT32       ui4_num_args,
                        ARG_DESC_T*  pt_args,
                        ARG_DESC_T*  pt_return)
{
    BT_RH_LOG("[_hndlr_]_hndlr_x_mtkapi_del_paired_hidh_dev_all, arg_1 = %ld\n", pt_args[0].u.i4_arg);

    if (ui4_num_args != 1)
    {
        BT_RH_LOG("Invalid ARGS: %ld\n", ui4_num_args);
        return RPCR_INV_ARGS;
    }

    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_mtkapi_del_paired_hidh_dev_all();

    return RPCR_OK;
}

static INT32 _hndlr_x_mtkapi_connect_hidh_sources(
                        RPC_ID_T     t_rpc_id,
                        const CHAR*  ps_cb_type,
                        UINT32       ui4_num_args,
                        ARG_DESC_T*  pt_args,
                        ARG_DESC_T*  pt_return)
{
    BT_RH_LOG("[_hndlr_]_hndlr_x_mtkapi_connect_hidh_sources, arg_1 = %s\n", pt_args[0].u.pc_arg);

    if (ui4_num_args != 1)
    {
        BT_RH_LOG("Invalid ARGS: %ld\n", ui4_num_args);
        return RPCR_INV_ARGS;
    }

    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_mtkapi_connect_hidh_sources(pt_args[0].u.pc_arg);

    return RPCR_OK;
}

static INT32 _hndlr_x_mtkapi_disconnect_hidh_sources(
                        RPC_ID_T     t_rpc_id,
                        const CHAR*  ps_cb_type,
                        UINT32       ui4_num_args,
                        ARG_DESC_T*  pt_args,
                        ARG_DESC_T*  pt_return)
{
    BT_RH_LOG("[_hndlr_]_hndlr_x_mtkapi_disconnect_hidh_sources, arg_1 = %s\n", pt_args[0].u.pc_arg);

    if (ui4_num_args != 1)
    {
        BT_RH_LOG("Invalid ARGS: %ld\n", ui4_num_args);
        return RPCR_INV_ARGS;
    }

    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_mtkapi_disconnect_hidh_sources(pt_args[0].u.pc_arg);

    return RPCR_OK;
}

static INT32 _hndlr_x_mtkapi_hidh_auto_disconnection(
                        RPC_ID_T     t_rpc_id,
                        const CHAR*  ps_cb_type,
                        UINT32       ui4_num_args,
                        ARG_DESC_T*  pt_args,
                        ARG_DESC_T*  pt_return)
{
    BT_RH_LOG("[_hndlr_]_hndlr_x_mtkapi_hidh_auto_disconnection, arg_1 = %ld\n", pt_args[0].u.i4_arg);

    if (ui4_num_args != 1)
    {
        BT_RH_LOG("Invalid ARGS: %ld\n", ui4_num_args);
        return RPCR_INV_ARGS;
    }

    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_mtkapi_hidh_auto_disconnection();

    return RPCR_OK;
}


INT32 c_rpc_reg_mtk_bt_service_hidh_op_hndlrs(VOID)
{
    RPC_REG_OP_HNDLR(x_mtkapi_hidh_init);
    RPC_REG_OP_HNDLR(x_mtkapi_hidh_deinit);
    RPC_REG_OP_HNDLR(x_mtkapi_hidh_connect);
    RPC_REG_OP_HNDLR(x_mtkapi_hidh_disconnect);
    RPC_REG_OP_HNDLR(x_mtkapi_hidh_set_output_report);
    RPC_REG_OP_HNDLR(x_mtkapi_hidh_get_input_report);
    RPC_REG_OP_HNDLR(x_mtkapi_hidh_get_output_report);
    RPC_REG_OP_HNDLR(x_mtkapi_hidh_set_input_report);
    RPC_REG_OP_HNDLR(x_mtkapi_hidh_get_feature_report);
    RPC_REG_OP_HNDLR(x_mtkapi_hidh_set_feature_report);
    RPC_REG_OP_HNDLR(x_mtkapi_hidh_get_protocol);
    RPC_REG_OP_HNDLR(x_mtkapi_hidh_set_protocol);
    RPC_REG_OP_HNDLR(x_mtkapi_hidh_send_control);
    RPC_REG_OP_HNDLR(x_mtkapi_bluetooth_hidh_set_output_report);
    RPC_REG_OP_HNDLR(x_mtkapi_get_paired_hidh_dev_list);
    RPC_REG_OP_HNDLR(x_mtkapi_del_paired_hidh_dev_one);
    RPC_REG_OP_HNDLR(x_mtkapi_del_paired_hidh_dev_all);
    RPC_REG_OP_HNDLR(x_mtkapi_connect_hidh_sources);
    RPC_REG_OP_HNDLR(x_mtkapi_disconnect_hidh_sources);
    RPC_REG_OP_HNDLR(x_mtkapi_hidh_auto_disconnection);
    return RPCR_OK;
}

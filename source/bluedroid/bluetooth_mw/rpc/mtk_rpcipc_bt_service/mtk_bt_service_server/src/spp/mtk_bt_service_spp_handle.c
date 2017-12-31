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

#include "mtk_bt_service_spp.h"
#include "mtk_bt_service_spp_handle.h"
#include "u_rpc.h"
#include "ri_common.h"

#define BT_RH_LOG(_stmt...) \
        do{ \
            if(1){    \
                printf("[Handle]Func:%s Line:%d--->: ", __FUNCTION__, __LINE__);   \
                printf(_stmt); \
                printf("\n"); \
            }        \
        }   \
        while(0)

static INT32 _hndlr_x_mtkapi_spp_connect(
                         RPC_ID_T     t_rpc_id,
                         const CHAR*  ps_cb_type,
                         UINT32       ui4_num_args,
                         ARG_DESC_T*  pt_args,
                         ARG_DESC_T*  pt_return)
{
    BT_RH_LOG("[_hndlr_]spp_connect, arg_1 = %s, arg_2 = %s\n", pt_args[0].u.ps_str, pt_args[1].u.ps_str);

    if (ui4_num_args != 2)
    {
        BT_RH_LOG("Invalid ARGS: %ld\n", ui4_num_args);
        return RPCR_INV_ARGS;
    }

    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_mtkapi_spp_connect(pt_args[0].u.ps_str, pt_args[1].u.ps_str);

    return RPCR_OK;
}

static INT32 _hndlr_x_mtkapi_spp_disconnect(
                         RPC_ID_T     t_rpc_id,
                         const CHAR*  ps_cb_type,
                         UINT32       ui4_num_args,
                         ARG_DESC_T*  pt_args,
                         ARG_DESC_T*  pt_return)
{
    BT_RH_LOG("[_hndlr_]spp_disconnect, arg_1 = %s, arg_2 = %s\n", pt_args[0].u.ps_str, pt_args[1].u.ps_str);

    if (ui4_num_args != 2)
    {
        BT_RH_LOG("Invalid ARGS: %ld\n", ui4_num_args);
        return RPCR_INV_ARGS;
    }

    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_mtkapi_spp_disconnect(pt_args[0].u.ps_str, pt_args[1].u.ps_str);

    return RPCR_OK;
}

static INT32 _hndlr_x_mtkapi_spp_send_data(
                         RPC_ID_T     t_rpc_id,
                         const CHAR*  ps_cb_type,
                         UINT32       ui4_num_args,
                         ARG_DESC_T*  pt_args,
                         ARG_DESC_T*  pt_return)
{
    BT_RH_LOG("[_hndlr_]spp_send_data, arg_1 = %s, arg_2 = %s,\n arg_3=%s, \n arg_4=%d",
        pt_args[0].u.ps_str, pt_args[1].u.ps_str, pt_args[2].u.ps_str, pt_args[3].u.i4_arg);

    if (ui4_num_args != 4)
    {
        BT_RH_LOG("Invalid ARGS: %ld\n", ui4_num_args);
        return RPCR_INV_ARGS;
    }

    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_mtkapi_spp_send_data(pt_args[0].u.ps_str, pt_args[1].u.ps_str, pt_args[2].u.ps_str, pt_args[3].u.i4_arg);

    return RPCR_OK;
}

static INT32 _hndlr_x_mtkapi_spp_enable_devb(
                         RPC_ID_T     t_rpc_id,
                         const CHAR*  ps_cb_type,
                         UINT32       ui4_num_args,
                         ARG_DESC_T*  pt_args,
                         ARG_DESC_T*  pt_return)
{
    BT_RH_LOG("[_hndlr_]spp_enable_devb, arg_1 = %s, arg_2 = %s\n", pt_args[0].u.ps_str, pt_args[1].u.ps_str);

    if (ui4_num_args != 2)
    {
        BT_RH_LOG("Invalid ARGS: %ld\n", ui4_num_args);
        return RPCR_INV_ARGS;
    }

    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_mtkapi_spp_enable_devb(pt_args[0].u.ps_str, pt_args[1].u.ps_str);

    return RPCR_OK;
}

static INT32 _hndlr_x_mtkapi_spp_disable_devb(
                         RPC_ID_T     t_rpc_id,
                         const CHAR*  ps_cb_type,
                         UINT32       ui4_num_args,
                         ARG_DESC_T*  pt_args,
                         ARG_DESC_T*  pt_return)
{
    BT_RH_LOG("[_hndlr_]spp_disable_devb, arg_1 = %s\n", pt_args[0].u.ps_str);

    if (ui4_num_args != 1)
    {
        BT_RH_LOG("Invalid ARGS: %ld\n", ui4_num_args);
        return RPCR_INV_ARGS;
    }

    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_mtkapi_spp_disable_devb(pt_args[0].u.ps_str);

    return RPCR_OK;
}


INT32 c_rpc_reg_mtk_bt_service_spp_op_hndlrs(VOID)
{
    RPC_REG_OP_HNDLR(x_mtkapi_spp_connect);
    RPC_REG_OP_HNDLR(x_mtkapi_spp_disconnect);
    RPC_REG_OP_HNDLR(x_mtkapi_spp_send_data);
    RPC_REG_OP_HNDLR(x_mtkapi_spp_enable_devb);
    RPC_REG_OP_HNDLR(x_mtkapi_spp_disable_devb);

    return RPCR_OK;
}



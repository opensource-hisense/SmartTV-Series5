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

#include "mtk_bt_service_avrcp.h"
#include "mtk_bt_service_avrcp_handle.h"
#include "u_rpc.h"
#include "ri_common.h"

#define BT_RH_LOG(_stmt...) \
        do{ \
            if(0){    \
                printf("[Handle]Func:%s Line:%d--->: ", __FUNCTION__, __LINE__);   \
                printf(_stmt); \
                printf("\n"); \
            }        \
        }   \
        while(0)


static INT32 _hndlr_x_mtkapi_avrcp_passthrough_cmd(
                        RPC_ID_T     t_rpc_id,
                        const CHAR*  ps_cb_type,
                        UINT32       ui4_num_args,
                        ARG_DESC_T*  pt_args,
                        ARG_DESC_T*  pt_return)
{
    if (2 != ui4_num_args)
    {
        return RPCR_INV_ARGS;
    }

    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_mtkapi_avrcp_passthrough_cmd(pt_args[0].u.i4_arg, pt_args[1].u.i4_arg);

    return RPCR_OK;
}

static INT32 _hndlr_x_mtkapi_avrcp_get_mediaInfo(
                        RPC_ID_T     t_rpc_id,
                        const CHAR*  ps_cb_type,
                        UINT32       ui4_num_args,
                        ARG_DESC_T*  pt_args,
                        ARG_DESC_T*  pt_return)
{
    BT_RH_LOG("[_hndlr_]x_mtkapi_avrcp_get_mediaInfo, arg_1 = %p", pt_args[0].u.pv_desc);

    if (ui4_num_args != 1)
    {
        BT_RH_LOG("Invalid ARGS: %ld", ui4_num_args);
        return RPCR_INV_ARGS;
    }

    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_mtkapi_avrcp_get_mediaInfo(pt_args[0].u.pv_desc);

    return RPCR_OK;
}

static INT32 _hndlr_x_mtkapi_avrcp_get_play_status(
                        RPC_ID_T     t_rpc_id,
                        const CHAR*  ps_cb_type,
                        UINT32       ui4_num_args,
                        ARG_DESC_T*  pt_args,
                        ARG_DESC_T*  pt_return)
{
    BT_RH_LOG("[_hndlr_]x_mtkapi_avrcp_get_mediaInfo, arg_1 = %p", pt_args[0].u.pv_desc);

    if (ui4_num_args != 1)
    {
        BT_RH_LOG("Invalid ARGS: %ld", ui4_num_args);
        return RPCR_INV_ARGS;
    }

    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_mtkapi_avrcp_get_play_status(pt_args[0].u.pv_desc);

    return RPCR_OK;
}

static INT32 _hndlr_x_mtkapi_avrcp_set_volume(
                        RPC_ID_T     t_rpc_id,
                        const CHAR*  ps_cb_type,
                        UINT32       ui4_num_args,
                        ARG_DESC_T*  pt_args,
                        ARG_DESC_T*  pt_return)
{
    if (1 != ui4_num_args)
    {
        return RPCR_INV_ARGS;
    }

    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_mtkapi_avrcp_set_volume(pt_args[0].u.i4_arg);

    return RPCR_OK;
}


INT32 c_rpc_reg_mtk_bt_service_avrcp_op_hndlrs(VOID)
{
    RPC_REG_OP_HNDLR(x_mtkapi_avrcp_passthrough_cmd);
    RPC_REG_OP_HNDLR(x_mtkapi_avrcp_get_mediaInfo);
    RPC_REG_OP_HNDLR(x_mtkapi_avrcp_get_play_status);
    RPC_REG_OP_HNDLR(x_mtkapi_avrcp_set_volume);
    return RPCR_OK;
}



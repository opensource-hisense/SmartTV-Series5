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

#include "mtk_bt_service_gatts.h"
#include "mtk_bt_service_gatts_handle.h"
#include "mtk_bt_service_gatt_ipcrpc_struct.h"
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

static VOID bt_app_gatts_event_cbk_wrapper(BT_GATTS_EVENT_T bt_gatts_event, void* pv_tag)
{
    RPC_DECL_VOID(2);
    RPC_CB_NFY_TAG_T  *pt_nfy_tag = (RPC_CB_NFY_TAG_T*)pv_tag;
    RPC_ARG_INP(ARG_TYPE_INT32, bt_gatts_event);
    RPC_ARG_INP(ARG_TYPE_REF_VOID, pt_nfy_tag->pv_tag);

    RPC_DO_CB(pt_nfy_tag->t_id, "bt_app_gatts_event_cbk", pt_nfy_tag->apv_cb_addr_ex[0]);

    RPC_RETURN_VOID;
}

static VOID bt_app_gatts_reg_server_cbk_wrapper(BT_GATTS_REG_SERVER_RST_T *bt_gatts_reg_server, void* pv_tag)
{
    RPC_DECL_VOID(2);
    RPC_CB_NFY_TAG_T *pt_nfy_tag = (RPC_CB_NFY_TAG_T*)pv_tag;
    RPC_CHECK(bt_rpc_add_ref_desc(RPC_DEFAULT_ID, bt_gatts_reg_server, RPC_DESC_BT_GATTS_REG_SERVER_RST_T, NULL));
    RPC_CHECK(bt_rpc_add_ref_buff(RPC_DEFAULT_ID, bt_gatts_reg_server->app_uuid, BT_GATT_MAX_UUID_LEN));
    RPC_ARG_IO(ARG_TYPE_REF_DESC, bt_gatts_reg_server);
    RPC_ARG_INP(ARG_TYPE_REF_VOID, pt_nfy_tag->pv_tag);

    BT_RH_LOG("[_hndlr_]bt_app_gatts_reg_server_cbk_wrapper, server_if = %ld, pt_nfy_tag->apv_cb_addr_ex[1] = %p\n",
        (long)bt_gatts_reg_server->server_if, pt_nfy_tag->apv_cb_addr_ex[1]);

    RPC_DO_CB(pt_nfy_tag->t_id, "bt_app_gatts_reg_server_cbk", pt_nfy_tag->apv_cb_addr_ex[1]);

    RPC_RETURN_VOID;
}

static VOID bt_app_gatts_add_srvc_cbk_wrapper(BT_GATTS_ADD_SRVC_RST_T *bt_gatts_add_srvc, void* pv_tag)
{
    RPC_DECL_VOID(2);
    RPC_CB_NFY_TAG_T *pt_nfy_tag = (RPC_CB_NFY_TAG_T*)pv_tag;
    RPC_CHECK(bt_rpc_add_ref_desc(RPC_DEFAULT_ID, bt_gatts_add_srvc, RPC_DESC_BT_GATTS_ADD_SRVC_RST_T, NULL));
    RPC_ARG_IO(ARG_TYPE_REF_DESC, bt_gatts_add_srvc);
    RPC_ARG_INP(ARG_TYPE_REF_VOID, pt_nfy_tag->pv_tag);

    BT_RH_LOG("[_hndlr_]bt_app_gatts_add_srvc_cbk_wrapper, uuid = %s, pt_nfy_tag->apv_cb_addr_ex[2] = %p\n",
                bt_gatts_add_srvc->srvc_id.id.uuid, pt_nfy_tag->apv_cb_addr_ex[2]);

    RPC_DO_CB(pt_nfy_tag->t_id, "bt_app_gatts_add_srvc_cbk", pt_nfy_tag->apv_cb_addr_ex[2]);

    RPC_RETURN_VOID;
}

static VOID bt_app_gatts_add_incl_cbk_wrapper(BT_GATTS_ADD_INCL_RST_T *bt_gatts_add_incl, void* pv_tag)
{
    RPC_DECL_VOID(2);
    RPC_CB_NFY_TAG_T *pt_nfy_tag = (RPC_CB_NFY_TAG_T*)pv_tag;
    RPC_CHECK(bt_rpc_add_ref_desc(RPC_DEFAULT_ID, bt_gatts_add_incl, RPC_DESC_BT_GATTS_ADD_INCL_RST_T, NULL));
    RPC_ARG_IO(ARG_TYPE_REF_DESC, bt_gatts_add_incl);
    RPC_ARG_INP(ARG_TYPE_REF_VOID, pt_nfy_tag->pv_tag);

    BT_RH_LOG("[_hndlr_]bt_app_gatts_add_incl_cbk_wrapper, incl_srvc_handle = %ld, pt_nfy_tag->apv_cb_addr_ex[3] = %p\n",
                (long)bt_gatts_add_incl->incl_srvc_handle, pt_nfy_tag->apv_cb_addr_ex[3]);

    RPC_DO_CB(pt_nfy_tag->t_id, "bt_app_gatts_add_incl_cbk", pt_nfy_tag->apv_cb_addr_ex[3]);

    RPC_RETURN_VOID;
}

static VOID bt_app_gatts_add_char_cbk_wrapper(BT_GATTS_ADD_CHAR_RST_T *bt_gatts_add_char, void* pv_tag)
{
    RPC_DECL_VOID(2);
    RPC_CB_NFY_TAG_T *pt_nfy_tag = (RPC_CB_NFY_TAG_T*)pv_tag;
    RPC_CHECK(bt_rpc_add_ref_desc(RPC_DEFAULT_ID, bt_gatts_add_char, RPC_DESC_BT_GATTS_ADD_CHAR_RST_T, NULL));
    RPC_ARG_IO(ARG_TYPE_REF_DESC, bt_gatts_add_char);
    RPC_ARG_INP(ARG_TYPE_REF_VOID, pt_nfy_tag->pv_tag);

    BT_RH_LOG("[_hndlr_]bt_app_gatts_add_char_cbk_wrapper, uuid = %s, pt_nfy_tag->apv_cb_addr_ex[4] = %p\n",
                bt_gatts_add_char->uuid, pt_nfy_tag->apv_cb_addr_ex[4]);

    RPC_DO_CB(pt_nfy_tag->t_id, "bt_app_gatts_add_char_cbk", pt_nfy_tag->apv_cb_addr_ex[4]);

    RPC_RETURN_VOID;
}

static VOID bt_app_gatts_add_desc_cbk_wrapper(BT_GATTS_ADD_DESCR_RST_T *bt_gatts_add_desc, void* pv_tag)
{
    RPC_DECL_VOID(2);
    RPC_CB_NFY_TAG_T *pt_nfy_tag = (RPC_CB_NFY_TAG_T*)pv_tag;
    RPC_CHECK(bt_rpc_add_ref_desc(RPC_DEFAULT_ID, bt_gatts_add_desc, RPC_DESC_BT_GATTS_ADD_DESCR_RST_T, NULL));
    RPC_ARG_IO(ARG_TYPE_REF_DESC, bt_gatts_add_desc);
    RPC_ARG_INP(ARG_TYPE_REF_VOID, pt_nfy_tag->pv_tag);

    BT_RH_LOG("[_hndlr_]bt_app_gatts_add_desc_cbk_wrapper, uuid = %s, pt_nfy_tag->apv_cb_addr_ex[5] = %p\n",
                bt_gatts_add_desc->uuid, pt_nfy_tag->apv_cb_addr_ex[5]);

    RPC_DO_CB(pt_nfy_tag->t_id, "bt_app_gatts_add_desc_cbk", pt_nfy_tag->apv_cb_addr_ex[5]);

    RPC_RETURN_VOID;
}

static VOID bt_app_gatts_op_srvc_cbk_wrapper(BT_GATTS_SRVC_OP_TYPE_T op_type, BT_GATTS_SRVC_RST_T *bt_gatts_srvc, void* pv_tag)
{
    RPC_DECL_VOID(3);
    RPC_CB_NFY_TAG_T *pt_nfy_tag = (RPC_CB_NFY_TAG_T*)pv_tag;
    RPC_ARG_INP(ARG_TYPE_INT32, op_type);
    RPC_CHECK(bt_rpc_add_ref_desc(RPC_DEFAULT_ID, bt_gatts_srvc, RPC_DESC_BT_GATTS_SRVC_RST_T, NULL));
    RPC_ARG_IO(ARG_TYPE_REF_DESC, bt_gatts_srvc);
    RPC_ARG_INP(ARG_TYPE_REF_VOID, pt_nfy_tag->pv_tag);

    BT_RH_LOG("[_hndlr_]bt_app_gatts_op_srvc_cbk_wrapper, srvc_handle = %ld, pt_nfy_tag->apv_cb_addr_ex[6] = %p\n",
                (long)bt_gatts_srvc->srvc_handle, pt_nfy_tag->apv_cb_addr_ex[6]);

    RPC_DO_CB(pt_nfy_tag->t_id, "bt_app_gatts_op_srvc_cbk", pt_nfy_tag->apv_cb_addr_ex[6]);

    RPC_RETURN_VOID;
}

static VOID bt_app_gatts_req_read_cbk_wrapper(BT_GATTS_REQ_READ_RST_T *bt_gatts_read, void* pv_tag)
{
    RPC_DECL_VOID(2);
    RPC_CB_NFY_TAG_T *pt_nfy_tag = (RPC_CB_NFY_TAG_T*)pv_tag;
    RPC_CHECK(bt_rpc_add_ref_desc(RPC_DEFAULT_ID, bt_gatts_read, RPC_DESC_BT_GATTS_REQ_READ_RST_T, NULL));
    RPC_ARG_IO(ARG_TYPE_REF_DESC, bt_gatts_read);
    RPC_ARG_INP(ARG_TYPE_REF_VOID, pt_nfy_tag->pv_tag);

    BT_RH_LOG("[_hndlr_]bt_app_gatts_req_read_cbk_wrapper, attr_handle = %ld, pt_nfy_tag->apv_cb_addr_ex[7] = %p\n",
                (long)bt_gatts_read->attr_handle, pt_nfy_tag->apv_cb_addr_ex[7]);

    RPC_DO_CB(pt_nfy_tag->t_id, "bt_app_gatts_req_read_cbk", pt_nfy_tag->apv_cb_addr_ex[7]);

    RPC_RETURN_VOID;
}

static VOID bt_app_gatts_req_write_cbk_wrapper(BT_GATTS_REQ_WRITE_RST_T *bt_gatts_write, void* pv_tag)
{
    RPC_DECL_VOID(2);
    RPC_CB_NFY_TAG_T *pt_nfy_tag = (RPC_CB_NFY_TAG_T*)pv_tag;
    RPC_CHECK(bt_rpc_add_ref_desc(RPC_DEFAULT_ID, bt_gatts_write, RPC_DESC_BT_GATTS_REQ_WRITE_RST_T, NULL));
    RPC_ARG_IO(ARG_TYPE_REF_DESC, bt_gatts_write);
    RPC_ARG_INP(ARG_TYPE_REF_VOID, pt_nfy_tag->pv_tag);

    BT_RH_LOG("[_hndlr_]bt_app_gatts_req_write_cbk_wrapper, value = %s, pt_nfy_tag->apv_cb_addr_ex[8] = %p\n",
                bt_gatts_write->value, pt_nfy_tag->apv_cb_addr_ex[8]);

    RPC_DO_CB(pt_nfy_tag->t_id, "bt_app_gatts_req_write_cbk", pt_nfy_tag->apv_cb_addr_ex[8]);

    RPC_RETURN_VOID;
}

static VOID bt_app_gatts_ind_sent_cbk_wrapper(INT32 conn_id, INT32 status, void *pv_tag)
{
    RPC_DECL_VOID(3);
    RPC_CB_NFY_TAG_T *pt_nfy_tag = (RPC_CB_NFY_TAG_T*)pv_tag;
    RPC_ARG_INP(ARG_TYPE_INT32, conn_id);
    RPC_ARG_INP(ARG_TYPE_INT32, status);
    RPC_ARG_INP(ARG_TYPE_REF_VOID, pt_nfy_tag->pv_tag);

    BT_RH_LOG("[_hndlr_]bt_app_gatts_ind_sent_cbk_wrapper, conn_id = %ld, status = %ld, pt_nfy_tag->apv_cb_addr_ex[9] = %p\n",
               (long)conn_id, (long)status, pt_nfy_tag->apv_cb_addr_ex[9]);

    RPC_DO_CB(pt_nfy_tag->t_id, "bt_app_gatts_ind_sent_cbk", pt_nfy_tag->apv_cb_addr_ex[9]);

    RPC_RETURN_VOID;
}

static INT32 _hndlr_x_mtkapi_bt_gatts_base_init(
                        RPC_ID_T     t_rpc_id,
                        const CHAR*  ps_cb_type,
                        UINT32       ui4_num_args,
                        ARG_DESC_T*  pt_args,
                        ARG_DESC_T*  pt_return)
{
    MTKRPCAPI_BT_APP_GATTS_CB_FUNC_T *p_bt_app_cb_func = NULL;
    MTKRPCAPI_BT_APP_GATTS_CB_FUNC_T bt_app_cb_func;
    RPC_CB_NFY_TAG_T * pt_nfy_tag = NULL;
    VOID * apv_cb_addr[10] = {0};

    memset(&bt_app_cb_func,0,sizeof(MTKRPCAPI_BT_APP_GATTS_CB_FUNC_T));

    BT_RH_LOG("gatts_base_init, pt_args[0].u.pv_desc = %p\n", pt_args[0].u.pv_desc);

    if(ui4_num_args != 2)
    {
        return RPCR_INV_ARGS;
    }
    p_bt_app_cb_func = (MTKRPCAPI_BT_APP_GATTS_CB_FUNC_T*)pt_args[0].u.pv_desc;

    if(p_bt_app_cb_func->bt_gatts_event_cb != NULL)
    {
        apv_cb_addr[0] = p_bt_app_cb_func->bt_gatts_event_cb;
        bt_app_cb_func.bt_gatts_event_cb = bt_app_gatts_event_cbk_wrapper;
    }

    if(p_bt_app_cb_func->bt_gatts_reg_server_cb != NULL)
    {
        apv_cb_addr[1] = p_bt_app_cb_func->bt_gatts_reg_server_cb;
        bt_app_cb_func.bt_gatts_reg_server_cb = bt_app_gatts_reg_server_cbk_wrapper;
    }

    if(p_bt_app_cb_func->bt_gatts_add_srvc_cb != NULL)
    {
        apv_cb_addr[2] = p_bt_app_cb_func->bt_gatts_add_srvc_cb;
        bt_app_cb_func.bt_gatts_add_srvc_cb = bt_app_gatts_add_srvc_cbk_wrapper;
    }
    if (p_bt_app_cb_func->bt_gatts_add_incl_cb != NULL)
    {
        apv_cb_addr[3] = p_bt_app_cb_func->bt_gatts_add_incl_cb;
        bt_app_cb_func.bt_gatts_add_incl_cb = bt_app_gatts_add_incl_cbk_wrapper;
    }
    if (p_bt_app_cb_func->bt_gatts_add_char_cb != NULL)
    {
        apv_cb_addr[4] = p_bt_app_cb_func->bt_gatts_add_char_cb;
        bt_app_cb_func.bt_gatts_add_char_cb = bt_app_gatts_add_char_cbk_wrapper;
    }
    if (p_bt_app_cb_func->bt_gatts_add_desc_cb != NULL)
    {
        apv_cb_addr[5] = p_bt_app_cb_func->bt_gatts_add_desc_cb;
        bt_app_cb_func.bt_gatts_add_desc_cb = bt_app_gatts_add_desc_cbk_wrapper;
    }
    if (p_bt_app_cb_func->bt_gatts_op_srvc_cb != NULL)
    {
        apv_cb_addr[6] = p_bt_app_cb_func->bt_gatts_op_srvc_cb;
        bt_app_cb_func.bt_gatts_op_srvc_cb = bt_app_gatts_op_srvc_cbk_wrapper;
    }
    if (p_bt_app_cb_func->bt_gatts_req_read_cb != NULL)
    {
        apv_cb_addr[7] = p_bt_app_cb_func->bt_gatts_req_read_cb;
        bt_app_cb_func.bt_gatts_req_read_cb = bt_app_gatts_req_read_cbk_wrapper;
    }
    if (p_bt_app_cb_func->bt_gatts_req_write_cb != NULL)
    {
        apv_cb_addr[8] = p_bt_app_cb_func->bt_gatts_req_write_cb;
        bt_app_cb_func.bt_gatts_req_write_cb = bt_app_gatts_req_write_cbk_wrapper;
    }
    if (p_bt_app_cb_func->bt_gatts_ind_sent_cb != NULL)
    {
        apv_cb_addr[9] = p_bt_app_cb_func->bt_gatts_ind_sent_cb;
        bt_app_cb_func.bt_gatts_ind_sent_cb = bt_app_gatts_ind_sent_cbk_wrapper;
    }

    pt_nfy_tag  = ri_create_cb_tag(t_rpc_id, apv_cb_addr, 10, pt_args[1].u.pv_arg);

    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_mtkapi_bt_gatts_base_init(&bt_app_cb_func, pt_nfy_tag);
    if (pt_return->u.i4_arg && pt_nfy_tag != NULL)
    {
        ri_free_cb_tag(pt_nfy_tag);
    }
    return RPCR_OK;
}

static INT32 _hndlr_x_mtkapi_bt_gatts_register_server(
                         RPC_ID_T     t_rpc_id,
                         const CHAR*  ps_cb_type,
                         UINT32       ui4_num_args,
                         ARG_DESC_T*  pt_args,
                         ARG_DESC_T*  pt_return)
{
    BT_RH_LOG("[_hndlr_]x_mtkapi_bt_gatts_register_server, arg_1 = %s\n", pt_args[0].u.pc_arg);

    if (ui4_num_args != 1)
    {
        BT_RH_LOG("Invalid ARGS: %ld\n", ui4_num_args);
        return RPCR_INV_ARGS;
    }

    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_mtkapi_bt_gatts_register_server(pt_args[0].u.pc_arg);

    return RPCR_OK;
}

static INT32 _hndlr_x_mtkapi_bt_gatts_unregister_server(
                         RPC_ID_T     t_rpc_id,
                         const CHAR*  ps_cb_type,
                         UINT32       ui4_num_args,
                         ARG_DESC_T*  pt_args,
                         ARG_DESC_T*  pt_return)
{
    BT_RH_LOG("[_hndlr_]x_mtkapi_bt_gatts_unregister_server, arg_1 = %ld\n", pt_args[0].u.i4_arg);

    if (ui4_num_args != 1)
    {
        BT_RH_LOG("Invalid ARGS: %ld\n", ui4_num_args);
        return RPCR_INV_ARGS;
    }

    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_mtkapi_bt_gatts_unregister_server(pt_args[0].u.i4_arg);

    return RPCR_OK;
}


static INT32 _hndlr_x_mtkapi_bt_gatts_open(
                         RPC_ID_T     t_rpc_id,
                         const CHAR*  ps_cb_type,
                         UINT32       ui4_num_args,
                         ARG_DESC_T*  pt_args,
                         ARG_DESC_T*  pt_return)
{
    BT_RH_LOG("[_hndlr_]x_mtkapi_bt_gatts_open, arg_1 = %ld, arg_2 = %s, arg_3 = %d, arg_4 = %ld\n",
        pt_args[0].u.i4_arg, pt_args[1].u.pc_arg, pt_args[2].u.ui1_arg, pt_args[3].u.i4_arg);

    if (ui4_num_args != 4)
    {
        BT_RH_LOG("Invalid ARGS: %ld\n", ui4_num_args);
        return RPCR_INV_ARGS;
    }

    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_mtkapi_bt_gatts_open(pt_args[0].u.i4_arg, pt_args[1].u.pc_arg,
                          pt_args[2].u.ui1_arg, pt_args[3].u.i4_arg);

    return RPCR_OK;
}


static INT32 _hndlr_x_mtkapi_bt_gatts_close(
                         RPC_ID_T     t_rpc_id,
                         const CHAR*  ps_cb_type,
                         UINT32       ui4_num_args,
                         ARG_DESC_T*  pt_args,
                         ARG_DESC_T*  pt_return)
{
    BT_RH_LOG("[_hndlr_]x_mtkapi_bt_gatts_close, arg_1 = %ld, arg_2 = %s, arg_3 = %ld\n",
        pt_args[0].u.i4_arg, pt_args[1].u.pc_arg, pt_args[2].u.i4_arg);

    if (ui4_num_args != 3)
    {
        BT_RH_LOG("Invalid ARGS: %ld\n", ui4_num_args);
        return RPCR_INV_ARGS;
    }

    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_mtkapi_bt_gatts_close(pt_args[0].u.i4_arg, pt_args[1].u.pc_arg,
                          pt_args[2].u.i4_arg);

    return RPCR_OK;
}

static INT32 _hndlr_x_mtkapi_bt_gatts_add_service(
                         RPC_ID_T     t_rpc_id,
                         const CHAR*  ps_cb_type,
                         UINT32       ui4_num_args,
                         ARG_DESC_T*  pt_args,
                         ARG_DESC_T*  pt_return)
{
    BT_RH_LOG("[_hndlr_]x_mtkapi_bt_gatts_open, arg_1 = %ld, arg_2 = %s, arg_3 = %d, arg_4 = %ld\n",
        pt_args[0].u.i4_arg, pt_args[1].u.pc_arg, pt_args[2].u.ui1_arg, pt_args[3].u.i4_arg);

    if (ui4_num_args != 4)
    {
        BT_RH_LOG("Invalid ARGS: %ld\n", ui4_num_args);
        return RPCR_INV_ARGS;
    }

    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_mtkapi_bt_gatts_add_service(pt_args[0].u.i4_arg, pt_args[1].u.pc_arg,
                          pt_args[2].u.ui1_arg, pt_args[3].u.i4_arg);

    return RPCR_OK;
}


static INT32 _hndlr_x_mtkapi_bt_gatts_add_included_service(
                         RPC_ID_T     t_rpc_id,
                         const CHAR*  ps_cb_type,
                         UINT32       ui4_num_args,
                         ARG_DESC_T*  pt_args,
                         ARG_DESC_T*  pt_return)
{
    BT_RH_LOG("[_hndlr_]x_mtkapi_bt_gatts_add_included_service, arg_1 = %ld, arg_2 = %ld, arg_3 = %ld\n",
        pt_args[0].u.i4_arg, pt_args[1].u.i4_arg, pt_args[2].u.i4_arg);

    if (ui4_num_args != 3)
    {
        BT_RH_LOG("Invalid ARGS: %ld\n", ui4_num_args);
        return RPCR_INV_ARGS;
    }

    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_mtkapi_bt_gatts_add_included_service(pt_args[0].u.i4_arg, pt_args[1].u.i4_arg,
                          pt_args[2].u.i4_arg);

    return RPCR_OK;
}

static INT32 _hndlr_x_mtkapi_bt_gatts_add_char(
                         RPC_ID_T     t_rpc_id,
                         const CHAR*  ps_cb_type,
                         UINT32       ui4_num_args,
                         ARG_DESC_T*  pt_args,
                         ARG_DESC_T*  pt_return)
{
    BT_RH_LOG("[_hndlr_]x_mtkapi_bt_gatts_add_char, arg_1 = %ld, arg_2 = %ld, arg_3 = %s\n",
        pt_args[0].u.i4_arg, pt_args[1].u.i4_arg,pt_args[2].u.pc_arg);

    if (ui4_num_args != 5)
    {
        BT_RH_LOG("Invalid ARGS: %ld\n", ui4_num_args);
        return RPCR_INV_ARGS;
    }

    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_mtkapi_bt_gatts_add_char(pt_args[0].u.i4_arg, pt_args[1].u.i4_arg,
                          pt_args[2].u.pc_arg, pt_args[3].u.i4_arg, pt_args[4].u.i4_arg);

    return RPCR_OK;
}

static INT32 _hndlr_x_mtkapi_bt_gatts_add_desc(
                         RPC_ID_T     t_rpc_id,
                         const CHAR*  ps_cb_type,
                         UINT32       ui4_num_args,
                         ARG_DESC_T*  pt_args,
                         ARG_DESC_T*  pt_return)
{
    BT_RH_LOG("[_hndlr_]x_mtkapi_bt_gatts_add_desc, arg_1 = %ld, arg_2 = %ld, arg_3 = %s\n",
        pt_args[0].u.i4_arg, pt_args[1].u.i4_arg,pt_args[2].u.pc_arg);

    if (ui4_num_args != 4)
    {
        BT_RH_LOG("Invalid ARGS: %ld\n", ui4_num_args);
        return RPCR_INV_ARGS;
    }

    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_mtkapi_bt_gatts_add_desc(pt_args[0].u.i4_arg, pt_args[1].u.i4_arg,
                          pt_args[2].u.pc_arg, pt_args[3].u.i4_arg);

    return RPCR_OK;
}

static INT32 _hndlr_x_mtkapi_bt_gatts_start_service(
                         RPC_ID_T     t_rpc_id,
                         const CHAR*  ps_cb_type,
                         UINT32       ui4_num_args,
                         ARG_DESC_T*  pt_args,
                         ARG_DESC_T*  pt_return)
{
    BT_RH_LOG("[_hndlr_]x_mtkapi_bt_gatts_start_service, arg_1 = %ld, arg_2 = %ld, arg_3 = %ld\n",
        pt_args[0].u.i4_arg, pt_args[1].u.i4_arg,pt_args[2].u.i4_arg);

    if (ui4_num_args != 3)
    {
        BT_RH_LOG("Invalid ARGS: %ld\n", ui4_num_args);
        return RPCR_INV_ARGS;
    }

    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_mtkapi_bt_gatts_start_service(pt_args[0].u.i4_arg, pt_args[1].u.i4_arg,
                          pt_args[2].u.i4_arg);

    return RPCR_OK;
}

static INT32 _hndlr_x_mtkapi_bt_gatts_stop_service(
                         RPC_ID_T     t_rpc_id,
                         const CHAR*  ps_cb_type,
                         UINT32       ui4_num_args,
                         ARG_DESC_T*  pt_args,
                         ARG_DESC_T*  pt_return)
{
    BT_RH_LOG("[_hndlr_]x_mtkapi_bt_gatts_stop_service, arg_1 = %ld, arg_2 = %ld\n",
        pt_args[0].u.i4_arg, pt_args[1].u.i4_arg);

    if (ui4_num_args != 2)
    {
        BT_RH_LOG("Invalid ARGS: %ld\n", ui4_num_args);
        return RPCR_INV_ARGS;
    }

    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_mtkapi_bt_gatts_stop_service(pt_args[0].u.i4_arg, pt_args[1].u.i4_arg);

    return RPCR_OK;
}

static INT32 _hndlr_x_mtkapi_bt_gatts_delete_service(
                         RPC_ID_T     t_rpc_id,
                         const CHAR*  ps_cb_type,
                         UINT32       ui4_num_args,
                         ARG_DESC_T*  pt_args,
                         ARG_DESC_T*  pt_return)
{
    BT_RH_LOG("[_hndlr_]x_mtkapi_bt_gatts_delete_service, arg_1 = %ld, arg_2 = %ld\n",
        pt_args[0].u.i4_arg, pt_args[1].u.i4_arg);

    if (ui4_num_args != 2)
    {
        BT_RH_LOG("Invalid ARGS: %ld\n", ui4_num_args);
        return RPCR_INV_ARGS;
    }

    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_mtkapi_bt_gatts_delete_service(pt_args[0].u.i4_arg, pt_args[1].u.i4_arg);

    return RPCR_OK;
}

static INT32 _hndlr_x_mtkapi_bt_gatts_send_indication(
                         RPC_ID_T     t_rpc_id,
                         const CHAR*  ps_cb_type,
                         UINT32       ui4_num_args,
                         ARG_DESC_T*  pt_args,
                         ARG_DESC_T*  pt_return)
{
    BT_RH_LOG("[_hndlr_]x_mtkapi_bt_gatts_send_indication, arg_1 = %ld, arg_2 = %ld\n",
        pt_args[0].u.i4_arg, pt_args[1].u.i4_arg);

    if (ui4_num_args != 6)
    {
        BT_RH_LOG("Invalid ARGS: %ld\n", ui4_num_args);
        return RPCR_INV_ARGS;
    }

    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_mtkapi_bt_gatts_send_indication(pt_args[0].u.i4_arg, pt_args[1].u.i4_arg,
                          pt_args[2].u.i4_arg, pt_args[3].u.i4_arg, pt_args[4].u.pc_arg, pt_args[5].u.i4_arg);

    return RPCR_OK;
}

static INT32 _hndlr_x_mtkapi_bt_gatts_send_response(
                         RPC_ID_T     t_rpc_id,
                         const CHAR*  ps_cb_type,
                         UINT32       ui4_num_args,
                         ARG_DESC_T*  pt_args,
                         ARG_DESC_T*  pt_return)
{
    BT_RH_LOG("[_hndlr_]x_mtkapi_bt_gatts_send_response, arg_1 = %ld, arg_2 = %ld\n",
        pt_args[0].u.i4_arg, pt_args[1].u.i4_arg);

    if (ui4_num_args != 7)
    {
        BT_RH_LOG("Invalid ARGS: %ld\n", ui4_num_args);
        return RPCR_INV_ARGS;
    }

    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_mtkapi_bt_gatts_send_response(pt_args[0].u.i4_arg, pt_args[1].u.i4_arg,
                          pt_args[2].u.i4_arg, pt_args[3].u.i4_arg, pt_args[4].u.pc_arg, pt_args[5].u.i4_arg, pt_args[6].u.i4_arg);

    return RPCR_OK;
}

static INT32 _hndlr_x_mtkapi_bt_gatts_get_connect_result_info(
                         RPC_ID_T     t_rpc_id,
                         const CHAR*  ps_cb_type,
                         UINT32       ui4_num_args,
                         ARG_DESC_T*  pt_args,
                         ARG_DESC_T*  pt_return)
{
     BT_RH_LOG("[_hndlr_]bt_gatts_get_connect_result_info, arg_1 = %p\n", pt_args[0].u.pv_desc);

    if (ui4_num_args != 1)
    {
        BT_RH_LOG("Invalid ARGS: %ld\n", ui4_num_args);
        return RPCR_INV_ARGS;
    }

    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = RPCR_OK;
    x_mtkapi_bt_gatts_get_connect_result_info(pt_args[0].u.pv_desc);

    return RPCR_OK;
}

static INT32 _hndlr_x_mtkapi_bt_gatts_get_disconnect_result_info(
                         RPC_ID_T     t_rpc_id,
                         const CHAR*  ps_cb_type,
                         UINT32       ui4_num_args,
                         ARG_DESC_T*  pt_args,
                         ARG_DESC_T*  pt_return)
{
     BT_RH_LOG("[_hndlr_]bt_gatts_get_disconnect_result_info, arg_1 = %p\n", pt_args[0].u.pv_desc);

    if (ui4_num_args != 1)
    {
        BT_RH_LOG("Invalid ARGS: %ld\n", ui4_num_args);
        return RPCR_INV_ARGS;
    }

    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = RPCR_OK;
    x_mtkapi_bt_gatts_get_disconnect_result_info(pt_args[0].u.pv_desc);

    return RPCR_OK;
}

#if MTK_BLE_GGL_SETUP_SUPPORT
static INT32 _hndlr_x_mtkapi_bt_gatt_initialize(
                         RPC_ID_T     t_rpc_id,
                         const CHAR*  ps_cb_type,
                         UINT32       ui4_num_args,
                         ARG_DESC_T*  pt_args,
                         ARG_DESC_T*  pt_return)
{
    BT_RH_LOG("[_hndlr_]initialize, arg_1 = %ld\n", pt_args[0].u.i4_arg);

    if (ui4_num_args != 1)
    {
        BT_RH_LOG("Invalid ARGS: %ld\n", ui4_num_args);
        return RPCR_INV_ARGS;
    }

    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_mtkapi_bt_gatt_initialize();

    return RPCR_OK;
}

static INT32 _hndlr_x_mtkapi_bt_gatt_finalize(
                         RPC_ID_T     t_rpc_id,
                         const CHAR*  ps_cb_type,
                         UINT32       ui4_num_args,
                         ARG_DESC_T*  pt_args,
                         ARG_DESC_T*  pt_return)
{
    BT_RH_LOG("[_hndlr_]initialize, arg_1 = %ld\n", pt_args[0].u.i4_arg);

    if (ui4_num_args != 1)
    {
        BT_RH_LOG("Invalid ARGS: %ld\n", ui4_num_args);
        return RPCR_INV_ARGS;
    }

    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_mtkapi_bt_gatt_finalize();

    return RPCR_OK;
}

static INT32 _hndlr_x_mtkapi_bt_gatt_isSupported(
                         RPC_ID_T     t_rpc_id,
                         const CHAR*  ps_cb_type,
                         UINT32       ui4_num_args,
                         ARG_DESC_T*  pt_args,
                         ARG_DESC_T*  pt_return)
{
    BT_RH_LOG("[_hndlr_]initialize, arg_1 = %ld\n", pt_args[0].u.i4_arg);

    if (ui4_num_args != 1)
    {
        BT_RH_LOG("Invalid ARGS: %ld\n", ui4_num_args);
        return RPCR_INV_ARGS;
    }

    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_mtkapi_bt_gatt_isSupported();

    return RPCR_OK;
}

static INT32 _hndlr_x_mtkapi_bt_gatt_setDelegate(
                         RPC_ID_T     t_rpc_id,
                         const CHAR*  ps_cb_type,
                         UINT32       ui4_num_args,
                         ARG_DESC_T*  pt_args,
                         ARG_DESC_T*  pt_return)
{
    BT_RH_LOG("[_hndlr_]setDelegate, arg_1 = %ld\n", pt_args[0].u.i4_arg);

    if (ui4_num_args != 1)
    {
        BT_RH_LOG("Invalid ARGS: %ld\n", ui4_num_args);
        return RPCR_INV_ARGS;
    }

    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_mtkapi_bt_gatt_setDelegate(pt_args[0].u.i4_arg);

    return RPCR_OK;
}

static INT32 _hndlr_x_mtkapi_bt_gatt_onCharacteristicWrite(
                         RPC_ID_T     t_rpc_id,
                         const CHAR*  ps_cb_type,
                         UINT32       ui4_num_args,
                         ARG_DESC_T*  pt_args,
                         ARG_DESC_T*  pt_return)
{
    BT_RH_LOG("[_hndlr_]onCharacteristicWrite, arg_1 = %s, arg_2 = %s, arg_3 = %s\n",
        pt_args[0].u.pc_arg, pt_args[1].u.pc_arg, pt_args[2].u.pc_arg);

    if (ui4_num_args != 3)
    {
        BT_RH_LOG("Invalid ARGS: %ld\n", ui4_num_args);
        return RPCR_INV_ARGS;
    }

    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_mtkapi_bt_gatt_onCharacteristicWrite(pt_args[0].u.pc_arg,
                                                                 pt_args[1].u.pc_arg,
                                                                 pt_args[2].u.pc_arg);

    return RPCR_OK;
}

static INT32 _hndlr_x_mtkapi_bt_gatts_createService(
                         RPC_ID_T     t_rpc_id,
                         const CHAR*  ps_cb_type,
                         UINT32       ui4_num_args,
                         ARG_DESC_T*  pt_args,
                         ARG_DESC_T*  pt_return)
{
    BT_RH_LOG("[_hndlr_]createService, arg_1 = %s\n", pt_args[0].u.pc_arg);

    if (ui4_num_args != 1)
    {
        BT_RH_LOG("Invalid ARGS: %ld\n", ui4_num_args);
        return RPCR_INV_ARGS;
    }

    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_mtkapi_bt_gatts_createService(pt_args[0].u.pc_arg);

    return RPCR_OK;
}

static INT32 _hndlr_x_mtkapi_bt_gatts_destroyService(
                         RPC_ID_T     t_rpc_id,
                         const CHAR*  ps_cb_type,
                         UINT32       ui4_num_args,
                         ARG_DESC_T*  pt_args,
                         ARG_DESC_T*  pt_return)
{
    BT_RH_LOG("[_hndlr_]destroyService, arg_1 = %s\n", pt_args[0].u.pc_arg);

    if (ui4_num_args != 1)
    {
        BT_RH_LOG("Invalid ARGS: %ld\n", ui4_num_args);
        return RPCR_INV_ARGS;
    }

    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_mtkapi_bt_gatts_destroyService(pt_args[0].u.pc_arg);

    return RPCR_OK;
}

static INT32 _hndlr_x_mtkapi_bt_gatts_startService(
                         RPC_ID_T     t_rpc_id,
                         const CHAR*  ps_cb_type,
                         UINT32       ui4_num_args,
                         ARG_DESC_T*  pt_args,
                         ARG_DESC_T*  pt_return)
{
    BT_RH_LOG("[_hndlr_]startService, arg_1 = %s\n", pt_args[0].u.pc_arg);

    if (ui4_num_args != 1)
    {
        BT_RH_LOG("Invalid ARGS: %ld\n", ui4_num_args);
        return RPCR_INV_ARGS;
    }

    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_mtkapi_bt_gatts_startService(pt_args[0].u.pc_arg);

    return RPCR_OK;
}

static INT32 _hndlr_x_mtkapi_bt_gatts_stopService(
                         RPC_ID_T     t_rpc_id,
                         const CHAR*  ps_cb_type,
                         UINT32       ui4_num_args,
                         ARG_DESC_T*  pt_args,
                         ARG_DESC_T*  pt_return)
{
    BT_RH_LOG("[_hndlr_]stopService, arg_1 = %s\n", pt_args[0].u.pc_arg);

    if (ui4_num_args != 1)
    {
        BT_RH_LOG("Invalid ARGS: %ld\n", ui4_num_args);
        return RPCR_INV_ARGS;
    }

    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_mtkapi_bt_gatts_stopService(pt_args[0].u.pc_arg);

    return RPCR_OK;
}

static INT32 _hndlr_x_mtkapi_bt_gatts_setDeviceName(
                         RPC_ID_T     t_rpc_id,
                         const CHAR*  ps_cb_type,
                         UINT32       ui4_num_args,
                         ARG_DESC_T*  pt_args,
                         ARG_DESC_T*  pt_return)
{
    BT_RH_LOG("[_hndlr_]setDeviceName, arg_1 = %s\n", pt_args[0].u.pc_arg);

    if (ui4_num_args != 1)
    {
        BT_RH_LOG("Invalid ARGS: %ld\n", ui4_num_args);
        return RPCR_INV_ARGS;
    }

    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_mtkapi_bt_gatts_setDeviceName(pt_args[0].u.pc_arg);

    return RPCR_OK;
}


static INT32 _hndlr_x_mtkapi_bt_gatt_setAdvertisement(
                         RPC_ID_T     t_rpc_id,
                         const CHAR*  ps_cb_type,
                         UINT32       ui4_num_args,
                         ARG_DESC_T*  pt_args,
                         ARG_DESC_T*  pt_return)
{
    BT_RH_LOG("[_hndlr_]setAdvertisement, arg_1 = %s, arg_2 = %s, arg_3 = %s, arg_4 = %d, arg_5 = %ld\n", pt_args[0].u.pc_arg,
                                                                    pt_args[1].u.pc_arg,
                                                                    pt_args[2].u.pc_arg,
                                                                    pt_args[3].u.b_arg,
                                                                    pt_args[4].u.ui4_arg);

    UINT32 index = 0;
    CHAR  *pc_arg[16] = {0};

    if (ui4_num_args != 16)
    {
        BT_RH_LOG("Invalid ARGS: %ld\n", ui4_num_args);
        return RPCR_INV_ARGS;
    }

    for (index = 0; index < pt_args[4].u.ui4_arg; index++)
    {
        pc_arg[index] = pt_args[5 + index].u.pc_arg;
        BT_RH_LOG("arg_%ld = %s\n", 6 + index, pc_arg[index]);
    }

    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_mtkapi_bt_gatt_setAdvertisement(pt_args[0].u.pc_arg,
                                                               pc_arg,
                                                               pt_args[4].u.ui4_arg,
                                                               pt_args[1].u.pc_arg,
                                                               pt_args[2].u.pc_arg,
                                                               pt_args[3].u.b_arg);

    return RPCR_OK;
}

static INT32 _hndlr_x_mtkapi_bt_gatt_setScanResponse(
                         RPC_ID_T     t_rpc_id,
                         const CHAR*  ps_cb_type,
                         UINT32       ui4_num_args,
                         ARG_DESC_T*  pt_args,
                         ARG_DESC_T*  pt_return)
{
    BT_RH_LOG("[_hndlr_]setScanResponse, arg_1 = %s, arg_2 = %s, arg_3 = %s, arg_4 = %d, arg_5 = %ld\n", pt_args[0].u.pc_arg,
                                                                    pt_args[1].u.pc_arg,
                                                                    pt_args[2].u.pc_arg,
                                                                    pt_args[3].u.b_arg,
                                                                    pt_args[4].u.ui4_arg);

    UINT32 index = 0;
    CHAR  *pc_arg[16] = {0};

    if (ui4_num_args != 16)
    {
        BT_RH_LOG("Invalid ARGS: %ld\n", ui4_num_args);
        return RPCR_INV_ARGS;
    }

    for (index = 0; index < pt_args[4].u.ui4_arg; index++)
    {
        pc_arg[index] = pt_args[5 + index].u.pc_arg;
        BT_RH_LOG("arg_%ld = %s\n", 6 + index, pc_arg[index]);
    }

    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_mtkapi_bt_gatt_setScanResponse(pt_args[0].u.pc_arg,
                                                              pc_arg,
                                                              pt_args[4].u.ui4_arg,
                                                              pt_args[1].u.pc_arg,
                                                              pt_args[2].u.pc_arg,
                                                              pt_args[3].u.b_arg);

    return RPCR_OK;
}

static INT32 _hndlr_x_mtkapi_bt_gatts_addCharacteristic(
                         RPC_ID_T     t_rpc_id,
                         const CHAR*  ps_cb_type,
                         UINT32       ui4_num_args,
                         ARG_DESC_T*  pt_args,
                         ARG_DESC_T*  pt_return)
{
    BT_RH_LOG("[_hndlr_]addCharacteristic, arg_1 = %s, arg_2 = %s, arg_3 = %s, arg_4 = %ld\n", pt_args[0].u.pc_arg,
                                                                    pt_args[1].u.pc_arg,
                                                                    pt_args[2].u.pc_arg,
                                                                    pt_args[3].u.ui4_arg);

    UINT32 index = 0;
    CHAR  *pc_arg[16] = {0};

    if (ui4_num_args != 16)
    {
        BT_RH_LOG("Invalid ARGS: %ld\n", ui4_num_args);
        return RPCR_INV_ARGS;
    }

    for (index = 0; index < pt_args[3].u.ui4_arg; index++)
    {
        pc_arg[index] = pt_args[4 + index].u.pc_arg;
        BT_RH_LOG("arg_%ld = %s\n", 5 + index, pc_arg[index]);
    }

    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_mtkapi_bt_gatts_addCharacteristic(pt_args[0].u.pc_arg,
                                                   pt_args[1].u.pc_arg,
                                                   pt_args[2].u.pc_arg,
                                                   pc_arg,
                                                   pt_args[3].u.ui4_arg);

    return RPCR_OK;
}

static INT32 _hndlr_x_mtkapi_bt_gatts_setCharacteristicValue(
                         RPC_ID_T     t_rpc_id,
                         const CHAR*  ps_cb_type,
                         UINT32       ui4_num_args,
                         ARG_DESC_T*  pt_args,
                         ARG_DESC_T*  pt_return)
{
    BT_RH_LOG("[_hndlr_]setCharacteristicValue, arg_1 = %s, arg_2 = %s, arg_3 = %p, arg_4 = %ld, arg_5 = %ld, arg_6 = %ld\n",
                                                                    pt_args[0].u.pc_arg,
                                                                    pt_args[1].u.pc_arg,
                                                                    pt_args[2].u.pv_desc,
                                                                    pt_args[3].u.ui4_arg,
                                                                    pt_args[4].u.ui4_arg,
                                                                    pt_args[5].u.ui4_arg);

    if (ui4_num_args != 6)
    {
        BT_RH_LOG("Invalid number of ARGS: %ld", ui4_num_args);
        return RPCR_INV_ARGS;
    }

    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_mtkapi_bt_gatts_setCharacteristicValue(pt_args[0].u.pc_arg,
                                                                     pt_args[1].u.pc_arg,
                                                                     pt_args[2].u.pv_desc,
                                                                     pt_args[3].u.ui4_arg,
                                                                     pt_args[4].u.ui4_arg,
                                                                     pt_args[5].u.ui4_arg);

    BT_RH_LOG("[_hndlr_]x_mtkapi_bt_gatts_setCharacteristicValue return\n");
    return RPCR_OK;
}
#endif

VOID x_mtkapi_bt_gatt_nfy_fct_wrapper(CHAR *service_uuid, CHAR *characteristic_uuid,
                                            CHAR *value, VOID *pv_tag)
{
    BT_RH_LOG("nfy_fct_wrapper, service_uuid = %s, characteristic_uuid = %s, value = %s, pv_tag = %p\n",
                                                                            service_uuid,
                                                                            characteristic_uuid,
                                                                            value,
                                                                            pv_tag);

    RPC_CB_NFY_TAG_T * pt_nfy_tag = pv_tag;
    RPC_DECL_VOID(4);

    RPC_ARG_INP(ARG_TYPE_REF_STR, service_uuid);
    RPC_ARG_INP(ARG_TYPE_REF_STR, characteristic_uuid);
    RPC_ARG_INP(ARG_TYPE_REF_STR, value);
    RPC_ARG_INP(ARG_TYPE_REF_VOID, pt_nfy_tag->pv_tag);

    BT_RH_LOG("nfy_fct_wrapper, pt_nfy_tag->pv_cb_addr = %p\n", pt_nfy_tag->pv_cb_addr);

    RPC_DO_CB(pt_nfy_tag->t_id, "bt_app_gatt_nfy_fct_cbk", pt_nfy_tag->pv_cb_addr);

    RPC_RETURN_VOID;
}

#if MTK_BLE_GGL_SETUP_SUPPORT
static INT32 _hndlr_x_mtkapi_bt_gatt_reg_nfy_fct(
                        RPC_ID_T     t_rpc_id,
                        const CHAR*  ps_cb_type,
                        UINT32       ui4_num_args,
                        ARG_DESC_T*  pt_args,
                        ARG_DESC_T*  pt_return)
{

    RPC_CB_NFY_TAG_T * pt_nfy_tag;
    a_mtkapi_bt_gatt_nfy_fct_cbk pf_nfy;

    BT_RH_LOG("reg_nfy_fct, pt_args[0].u.pv_func = %p\n", pt_args[0].u.pv_func);

    if (ui4_num_args != 2)
    {
        BT_RH_LOG("Invalid ARGS: %ld\n", ui4_num_args);
        return RPCR_INV_ARGS;
    }

    if (pt_args[0].u.pv_func == NULL)
    {
        pf_nfy      = NULL;
        pt_nfy_tag  = NULL;
    }
    else
    {
        pf_nfy = (a_mtkapi_bt_gatt_nfy_fct_cbk)x_mtkapi_bt_gatt_nfy_fct_wrapper;
        //RPC_CB_TAG(t_rpc_id, pt_nfy_tag, pt_args[0].u.pv_func, pt_args[1].u.pv_arg);
        pt_nfy_tag = ri_create_cb_tag(t_rpc_id, &pt_args[0].u.pv_func, 1, pt_args[1].u.pv_arg);
    }

    BT_RH_LOG("reg_nfy_fct, pt_nfy_tag->pv_cb_addr = %p\n", pt_nfy_tag->pv_cb_addr);

    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_mtkapi_bt_gatt_reg_nfy_fct(pf_nfy, (void *)pt_nfy_tag);

    if (pt_return->u.i4_arg == 0)
    {
        return RPCR_OK;
    }
    else
    {
        return RPCR_ERROR;
    }

}
#endif

INT32 c_rpc_reg_mtk_bt_service_gatts_op_hndlrs(VOID)
{
    RPC_REG_OP_HNDLR(x_mtkapi_bt_gatts_base_init);
    RPC_REG_OP_HNDLR(x_mtkapi_bt_gatts_register_server);
    RPC_REG_OP_HNDLR(x_mtkapi_bt_gatts_unregister_server);
    RPC_REG_OP_HNDLR(x_mtkapi_bt_gatts_open);
    RPC_REG_OP_HNDLR(x_mtkapi_bt_gatts_close);
    RPC_REG_OP_HNDLR(x_mtkapi_bt_gatts_add_service);
    RPC_REG_OP_HNDLR(x_mtkapi_bt_gatts_add_included_service);
    RPC_REG_OP_HNDLR(x_mtkapi_bt_gatts_add_char);
    RPC_REG_OP_HNDLR(x_mtkapi_bt_gatts_add_desc);
    RPC_REG_OP_HNDLR(x_mtkapi_bt_gatts_start_service);
    RPC_REG_OP_HNDLR(x_mtkapi_bt_gatts_stop_service);
    RPC_REG_OP_HNDLR(x_mtkapi_bt_gatts_delete_service);
    RPC_REG_OP_HNDLR(x_mtkapi_bt_gatts_send_indication);
    RPC_REG_OP_HNDLR(x_mtkapi_bt_gatts_send_response);
    RPC_REG_OP_HNDLR(x_mtkapi_bt_gatts_get_connect_result_info);
    RPC_REG_OP_HNDLR(x_mtkapi_bt_gatts_get_disconnect_result_info);
#if MTK_BLE_GGL_SETUP_SUPPORT
    RPC_REG_OP_HNDLR(x_mtkapi_bt_gatt_initialize);
    RPC_REG_OP_HNDLR(x_mtkapi_bt_gatt_finalize);
    RPC_REG_OP_HNDLR(x_mtkapi_bt_gatt_isSupported);
    RPC_REG_OP_HNDLR(x_mtkapi_bt_gatt_setDelegate);
    RPC_REG_OP_HNDLR(x_mtkapi_bt_gatt_onCharacteristicWrite);
    RPC_REG_OP_HNDLR(x_mtkapi_bt_gatts_createService);
    RPC_REG_OP_HNDLR(x_mtkapi_bt_gatts_destroyService);
    RPC_REG_OP_HNDLR(x_mtkapi_bt_gatts_startService);
    RPC_REG_OP_HNDLR(x_mtkapi_bt_gatts_stopService);
    RPC_REG_OP_HNDLR(x_mtkapi_bt_gatts_setDeviceName);
    RPC_REG_OP_HNDLR(x_mtkapi_bt_gatt_setAdvertisement);
    RPC_REG_OP_HNDLR(x_mtkapi_bt_gatt_setScanResponse);
    RPC_REG_OP_HNDLR(x_mtkapi_bt_gatts_addCharacteristic);
    RPC_REG_OP_HNDLR(x_mtkapi_bt_gatts_setCharacteristicValue);
    RPC_REG_OP_HNDLR(x_mtkapi_bt_gatt_reg_nfy_fct);
#endif
    return RPCR_OK;
}



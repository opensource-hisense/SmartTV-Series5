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
#include "mtk_bt_service_gatts_wrapper.h"
#include "mtk_bt_service_gatts.h"
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


#define CHARACTERRISTIC_MAX_LENGTH 16*1024
static char * p_setCharValue = NULL;
static char * p_setCharValuePtr = NULL;
static INT32 setCharValueCnt = 0;


void *g_gatts_pvtag = NULL;

mtkrpcapi_BtAppGATTSEventCbk mtkrpcapi_BtGATTSEventCbk = NULL;
mtkrpcapi_BtAppGATTSRegServerCbk mtkrpcapi_BtGATTSRegServerCbk = NULL;
mtkrpcapi_BtAppGATTSAddSrvcCbk mtkrpcapi_BtGATTSAddSrvcCbk = NULL;
mtkrpcapi_BtAppGATTSAddInclCbk mtkrpcapi_BtGATTSAddInclCbk = NULL;
mtkrpcapi_BtAppGATTSAddCharCbk mtkrpcapi_BtGATTSAddCharCbk = NULL;
mtkrpcapi_BtAppGATTSAddDescCbk mtkrpcapi_BtGATTSAddDescCbk = NULL;
mtkrpcapi_BtAppGATTSOpSrvcCbk mtkrpcapi_BtGATTSOpSrvcCbk = NULL;
mtkrpcapi_BtAppGATTSReqReadCbk mtkrpcapi_BtGATTSReqReadCbk = NULL;
mtkrpcapi_BtAppGATTSReqWriteCbk mtkrpcapi_BtGATTSReqWriteCbk = NULL;
mtkrpcapi_BtAppGATTSIndSentCbk mtkrpcapi_BtGATTSIndSentCbk = NULL;

VOID MWBtAppGATTSEventCbk(BT_GATTS_EVENT_T bt_gatts_event)
{
    if (mtkrpcapi_BtGATTSEventCbk)
    {
        mtkrpcapi_BtGATTSEventCbk(bt_gatts_event, g_gatts_pvtag);
    }
}

VOID MWBtAppGATTSRegServerCbk(BT_GATTS_REG_SERVER_RST_T *bt_gatts_reg_server)
{
    if (NULL == bt_gatts_reg_server)
    {
        return;
    }
    BT_RC_LOG("[.c][%s], server_if = %ld\n", __FUNCTION__, (long)bt_gatts_reg_server->server_if);
    if (mtkrpcapi_BtGATTSRegServerCbk)
    {
        mtkrpcapi_BtGATTSRegServerCbk(bt_gatts_reg_server, g_gatts_pvtag);
    }
}

VOID MWBtAppGATTSAddSrvcCbk(BT_GATTS_ADD_SRVC_RST_T *bt_gatts_add_srvc)
{
    if (NULL == bt_gatts_add_srvc)
    {
        return;
    }
    BT_RC_LOG("[.c][%s], uuid = %s\n", __FUNCTION__,
    bt_gatts_add_srvc->srvc_id.id.uuid);
    if (mtkrpcapi_BtGATTSAddSrvcCbk)
    {
        mtkrpcapi_BtGATTSAddSrvcCbk(bt_gatts_add_srvc, g_gatts_pvtag);
    }
}

VOID MWBtAppGATTSAddInclCbk(BT_GATTS_ADD_INCL_RST_T *bt_gatts_add_incl)
{
    if (NULL == bt_gatts_add_incl)
    {
        return;
    }
    BT_RC_LOG("[.c][%s], incl_srvc_handle = %ld\n", __FUNCTION__, (long)bt_gatts_add_incl->incl_srvc_handle);
    if (mtkrpcapi_BtGATTSAddInclCbk)
    {
        mtkrpcapi_BtGATTSAddInclCbk(bt_gatts_add_incl, g_gatts_pvtag);
    }
}

VOID MWBtAppGATTSAddCharCbk(BT_GATTS_ADD_CHAR_RST_T *bt_gatts_add_char)
{
    if (NULL == bt_gatts_add_char)
    {
        return;
    }
    BT_RC_LOG("[.c][%s], uuid = %s\n", __FUNCTION__, bt_gatts_add_char->uuid);
    if (mtkrpcapi_BtGATTSAddCharCbk)
    {
        mtkrpcapi_BtGATTSAddCharCbk(bt_gatts_add_char, g_gatts_pvtag);
    }
}

VOID MWBtAppGATTSAddDescCbk(BT_GATTS_ADD_DESCR_RST_T *bt_gatts_add_desc)
{
    if (NULL == bt_gatts_add_desc)
    {
        return;
    }
    BT_RC_LOG("[.c][%s], uuid = %s\n", __FUNCTION__, bt_gatts_add_desc->uuid);
    if (mtkrpcapi_BtGATTSAddDescCbk)
    {
        mtkrpcapi_BtGATTSAddDescCbk(bt_gatts_add_desc, g_gatts_pvtag);
    }
}

VOID MWBtAppGATTSOpSrvcCbk(BT_GATTS_SRVC_OP_TYPE_T op_type, BT_GATTS_SRVC_RST_T *bt_gatts_srvc)
{
    if (NULL == bt_gatts_srvc)
    {
        return;
    }
    BT_RC_LOG("[.c][%s], op_type = %ld, srvc_handle = %ld\n", __FUNCTION__,
               (long)op_type, (long)bt_gatts_srvc->srvc_handle);
    if (mtkrpcapi_BtGATTSOpSrvcCbk)
    {
        mtkrpcapi_BtGATTSOpSrvcCbk(op_type, bt_gatts_srvc, g_gatts_pvtag);
    }
}

VOID MWBtAppGATTSReqReadCbk(BT_GATTS_REQ_READ_RST_T *bt_gatts_read)
{
    if (NULL == bt_gatts_read)
    {
        return;
    }
    BT_RC_LOG("[.c][%s], attr_handle = %ld\n", __FUNCTION__, (long)bt_gatts_read->attr_handle);
    if (mtkrpcapi_BtGATTSReqReadCbk)
    {
        mtkrpcapi_BtGATTSReqReadCbk(bt_gatts_read, g_gatts_pvtag);
    }
}

VOID MWBtAppGATTSReqWriteCbk(BT_GATTS_REQ_WRITE_RST_T *bt_gatts_write)
{
    if (NULL == bt_gatts_write)
    {
        return;
    }
    BT_RC_LOG("[.c][%s], attr_handle = %ld\n", __FUNCTION__, (long)bt_gatts_write->attr_handle);
    if (mtkrpcapi_BtGATTSReqWriteCbk)
    {
        mtkrpcapi_BtGATTSReqWriteCbk(bt_gatts_write, g_gatts_pvtag);
    }
}

VOID MWBtAppGATTSIndSentCbk(INT32 conn_id, INT32 status)
{
    BT_RC_LOG("[.c][%s], conn_id = %ld, status = %ld\n", __FUNCTION__, (long)conn_id, (long)status);
    if (mtkrpcapi_BtGATTSIndSentCbk)
    {
        mtkrpcapi_BtGATTSIndSentCbk(conn_id, status, g_gatts_pvtag);
    }
}

/*register APP callback function*/
INT32 mtkrpcapi_btm_gatts_register_cbk_fct(MTKRPCAPI_BT_APP_GATTS_CB_FUNC_T *func,void *pv_tag)
{
    INT32 i4_ret = 0;

    g_gatts_pvtag = pv_tag;
    if(NULL == func)
    {
        BT_RC_LOG(("callback func is null!\n"));
        return BT_ERR_NULL_POINTER;
    }
    if(func->bt_gatts_event_cb)
    {
        mtkrpcapi_BtGATTSEventCbk = func->bt_gatts_event_cb;
    }
    else
    {
        BT_RC_LOG(("event callback func is null!\n"));
        i4_ret = BT_ERR_NULL_POINTER;
    }

    if(func->bt_gatts_reg_server_cb)
    {
        mtkrpcapi_BtGATTSRegServerCbk = func->bt_gatts_reg_server_cb;
    }
    else
    {
        BT_RC_LOG(("bt_gatts_reg_server_cb callback func is null!\n"));
        i4_ret = BT_ERR_NULL_POINTER;
    }
    if(func->bt_gatts_add_srvc_cb)
    {
        mtkrpcapi_BtGATTSAddSrvcCbk = func->bt_gatts_add_srvc_cb;
    }
    else
    {
        BT_RC_LOG(("bt_gatts_add_srvc_cb callback func is null!\n"));
        i4_ret = BT_ERR_NULL_POINTER;
    }

    if(func->bt_gatts_add_incl_cb)
    {
        mtkrpcapi_BtGATTSAddInclCbk = func->bt_gatts_add_incl_cb;
    }
    else
    {
        BT_RC_LOG(("bt_gatts_add_incl_cb callback func is null!\n"));
        i4_ret = BT_ERR_NULL_POINTER;
    }

    if(func->bt_gatts_add_char_cb)
    {
        mtkrpcapi_BtGATTSAddCharCbk = func->bt_gatts_add_char_cb;
    }
    else
    {
        BT_RC_LOG(("bt_gatts_add_char_cb callback func is null!\n"));
        i4_ret = BT_ERR_NULL_POINTER;
    }

    if(func->bt_gatts_add_desc_cb)
    {
        mtkrpcapi_BtGATTSAddDescCbk = func->bt_gatts_add_desc_cb;
    }
    else
    {
        BT_RC_LOG(("bt_gatts_add_desc_cb callback func is null!\n"));
        i4_ret = BT_ERR_NULL_POINTER;
    }

    if(func->bt_gatts_op_srvc_cb)
    {
        mtkrpcapi_BtGATTSOpSrvcCbk = func->bt_gatts_op_srvc_cb;
    }
    else
    {
        BT_RC_LOG(("bt_gatts_op_srvc_cb callback func is null!\n"));
        i4_ret = BT_ERR_NULL_POINTER;
    }

    if(func->bt_gatts_req_read_cb)
    {
        mtkrpcapi_BtGATTSReqReadCbk = func->bt_gatts_req_read_cb;
    }
    else
    {
        BT_RC_LOG(("bt_gatts_req_read_cb callback func is null!\n"));
        i4_ret = BT_ERR_NULL_POINTER;
    }

    if(func->bt_gatts_req_write_cb)
    {
        mtkrpcapi_BtGATTSReqWriteCbk = func->bt_gatts_req_write_cb;
    }
    else
    {
        BT_RC_LOG(("bt_gatts_req_write_cb callback func is null!\n"));
        i4_ret = BT_ERR_NULL_POINTER;
    }

    if(func->bt_gatts_ind_sent_cb)
    {
        mtkrpcapi_BtGATTSIndSentCbk = func->bt_gatts_ind_sent_cb;
    }
    else
    {
        BT_RC_LOG(("bt_gatts_ind_sent_cb callback func is null!\n"));
        i4_ret = BT_ERR_NULL_POINTER;
    }
    return i4_ret;
}

INT32 mtkrpcapi_bt_gatts_base_init(MTKRPCAPI_BT_APP_GATTS_CB_FUNC_T *func,void *pv_tag)
{
    BT_RC_LOG("[.c][%s]\n", __FUNCTION__);

    INT32 i4_ret = 0;
    BT_APP_GATTS_CB_FUNC_T app_func;
    memset(&app_func, 0, sizeof(BT_APP_GATTS_CB_FUNC_T));

    app_func.bt_gatts_event_cb = MWBtAppGATTSEventCbk;
    app_func.bt_gatts_reg_server_cb = MWBtAppGATTSRegServerCbk;
    app_func.bt_gatts_add_srvc_cb = MWBtAppGATTSAddSrvcCbk;
    app_func.bt_gatts_add_incl_cb = MWBtAppGATTSAddInclCbk;
    app_func.bt_gatts_add_char_cb = MWBtAppGATTSAddCharCbk;
    app_func.bt_gatts_add_desc_cb = MWBtAppGATTSAddDescCbk;
    app_func.bt_gatts_op_srvc_cb = MWBtAppGATTSOpSrvcCbk;
    app_func.bt_gatts_req_read_cb = MWBtAppGATTSReqReadCbk;
    app_func.bt_gatts_req_write_cb = MWBtAppGATTSReqWriteCbk;
    app_func.bt_gatts_ind_sent_cb = MWBtAppGATTSIndSentCbk;
    i4_ret = mtkrpcapi_btm_gatts_register_cbk_fct(func, pv_tag);
    if(0 != i4_ret)
    {
        BT_RC_LOG(("mtkrpcapi_bt_gatts_base_init Error.\n"));
        return i4_ret;
    }

    return c_btm_bt_gatts_base_init(&app_func);
}

INT32 x_mtkapi_bt_gatts_base_init(MTKRPCAPI_BT_APP_GATTS_CB_FUNC_T *func, void *pv_tag)
{
    BT_RC_LOG("[.c][%s]\n", __FUNCTION__);
    return mtkrpcapi_bt_gatts_base_init(func,pv_tag);

}

INT32 x_mtkapi_bt_gatts_register_server(char * app_uuid)
{
    INT32 i4_ret = 0;
    i4_ret = c_btm_bt_gatts_register_server(app_uuid);
    if (i4_ret != 0)
    {
        BT_RC_LOG("[.c][%s]c_btm_bt_gatts_register_server fail\n", __FUNCTION__);
    }
    else
    {
        BT_RC_LOG("[.c][%s]c_btm_bt_gatts_register_server success\n", __FUNCTION__);
    }
    return i4_ret;
}
INT32 x_mtkapi_bt_gatts_unregister_server(INT32 server_if)
{
    INT32 i4_ret = 0;
    i4_ret = c_btm_bt_gatts_unregister_server(server_if);
    if (i4_ret != 0)
    {
        BT_RC_LOG("[.c][%s]c_btm_bt_gatts_unregister_server fail\n", __FUNCTION__);
    }
    else
    {
        BT_RC_LOG("[.c][%s]c_btm_bt_gatts_unregister_server success\n", __FUNCTION__);
    }
    return i4_ret;
}

INT32 x_mtkapi_bt_gatts_open(INT32 server_if, CHAR *bt_addr, UINT8 is_direct, INT32 transport)
{
    INT32 i4_ret = 0;
    i4_ret = c_btm_bt_gatts_open(server_if, bt_addr, is_direct, transport);
    if (i4_ret != 0)
    {
        BT_RC_LOG("[.c][%s]c_btm_bt_gatts_open fail\n", __FUNCTION__);
    }
    else
    {
        BT_RC_LOG("[.c][%s]c_btm_bt_gatts_open success\n", __FUNCTION__);
    }
    return i4_ret;
}

INT32 x_mtkapi_bt_gatts_close(INT32 server_if, CHAR *bt_addr, INT32 conn_id)
{
    INT32 i4_ret = 0;
    i4_ret = c_btm_bt_gatts_close(server_if, bt_addr, conn_id);
    if (i4_ret != 0)
    {
        BT_RC_LOG("[.c][%s]c_btm_bt_gatts_close fail\n", __FUNCTION__);
    }
    else
    {
        BT_RC_LOG("[.c][%s]c_btm_bt_gatts_close success\n", __FUNCTION__);
    }
    return i4_ret;
}

INT32 x_mtkapi_bt_gatts_add_service(INT32 server_if, CHAR *service_uuid,
                                         UINT8 is_primary, INT32 number)
{
    INT32 i4_ret = 0;
    i4_ret = c_btm_bt_gatts_add_service(server_if, service_uuid, is_primary, number);
    if (i4_ret != 0)
    {
        BT_RC_LOG("[.c][%s]c_btm_bt_gatts_add_service fail\n", __FUNCTION__);
    }
    else
    {
        BT_RC_LOG("[.c][%s]c_btm_bt_gatts_add_service success\n", __FUNCTION__);
    }
    return i4_ret;
}

INT32 x_mtkapi_bt_gatts_add_included_service(INT32 server_if, INT32 service_handle, INT32 included_handle)
{
    INT32 i4_ret = 0;
    i4_ret = c_btm_bt_gatts_add_included_service(server_if, service_handle, included_handle);
    if (i4_ret != 0)
    {
        BT_RC_LOG("[.c][%s]c_btm_bt_gatts_add_included_service fail\n", __FUNCTION__);
    }
    else
    {
        BT_RC_LOG("[.c][%s]c_btm_bt_gatts_add_included_service success\n", __FUNCTION__);
    }
    return i4_ret;
}

INT32 x_mtkapi_bt_gatts_add_char(INT32 server_if, INT32 service_handle, CHAR *uuid,
                                      INT32 properties, INT32 permissions)
{
    INT32 i4_ret = 0;
    i4_ret = c_btm_bt_gatts_add_char(server_if, service_handle, uuid, properties, permissions);
    if (i4_ret != 0)
    {
        BT_RC_LOG("[.c][%s]c_btm_bt_gatts_add_char fail\n", __FUNCTION__);
    }
    else
    {
        BT_RC_LOG("[.c][%s]c_btm_bt_gatts_add_char success\n", __FUNCTION__);
    }
    return i4_ret;
}

INT32 x_mtkapi_bt_gatts_add_desc(INT32 server_if, INT32 service_handle,
                                       CHAR *uuid, INT32 permissions)
{
    INT32 i4_ret = 0;
    i4_ret = c_btm_bt_gatts_add_desc(server_if, service_handle, uuid, permissions);
    if (i4_ret != 0)
    {
        BT_RC_LOG("[.c][%s]c_btm_bt_gatts_add_desc fail\n", __FUNCTION__);
    }
    else
    {
        BT_RC_LOG("[.c][%s]c_btm_bt_gatts_add_desc success\n", __FUNCTION__);
    }
    return i4_ret;
}

INT32 x_mtkapi_bt_gatts_start_service(INT32 server_if, INT32 service_handle, INT32 transport)
{
    INT32 i4_ret = 0;
    i4_ret = c_btm_bt_gatts_start_service(server_if, service_handle, transport);
    if (i4_ret != 0)
    {
        BT_RC_LOG("[.c][%s]c_btm_bt_gatts_start_service fail\n", __FUNCTION__);
    }
    else
    {
        BT_RC_LOG("[.c][%s]c_btm_bt_gatts_start_service success\n", __FUNCTION__);
    }
    return i4_ret;
}

INT32 x_mtkapi_bt_gatts_stop_service(INT32 server_if, INT32 service_handle)
{
    INT32 i4_ret = 0;
    i4_ret = c_btm_bt_gatts_stop_service(server_if, service_handle);
    if (i4_ret != 0)
    {
        BT_RC_LOG("[.c][%s]c_btm_bt_gatts_stop_service fail\n", __FUNCTION__);
    }
    else
    {
        BT_RC_LOG("[.c][%s]c_btm_bt_gatts_stop_service success\n", __FUNCTION__);
    }
    return i4_ret;
}

INT32 x_mtkapi_bt_gatts_delete_service(INT32 server_if, INT32 service_handle)
{
    INT32 i4_ret = 0;
    i4_ret = c_btm_bt_gatts_delete_service(server_if, service_handle);
    if (i4_ret != 0)
    {
        BT_RC_LOG("[.c][%s]c_btm_bt_gatts_delete_service fail\n", __FUNCTION__);
    }
    else
    {
        BT_RC_LOG("[.c][%s]c_btm_bt_gatts_delete_service success\n", __FUNCTION__);
    }
    return i4_ret;
}

INT32 x_mtkapi_bt_gatts_send_indication(INT32 server_if, INT32 attribute_handle,
                                              INT32 conn_id, INT32 fg_confirm,
                                              CHAR *p_value, INT32 value_len)
{
    INT32 i4_ret = 0;
    i4_ret = c_btm_bt_gatts_send_indication(server_if, attribute_handle, conn_id,
                                            fg_confirm, p_value, value_len);
    if (i4_ret != 0)
    {
        BT_RC_LOG("[.c][%s]c_btm_bt_gatts_send_indication fail\n", __FUNCTION__);
    }
    else
    {
        BT_RC_LOG("[.c][%s]c_btm_bt_gatts_send_indication success\n", __FUNCTION__);
    }
    return i4_ret;
}

INT32 x_mtkapi_bt_gatts_send_response(INT32 conn_id, INT32 trans_id,
                                             INT32 status, INT32 handle,
                                             CHAR *p_value, INT32 value_len,
                                             INT32 auth_req)
{
    INT32 i4_ret = 0;
    i4_ret = c_btm_bt_gatts_send_response(conn_id, trans_id, status, handle,
                                          p_value, value_len, auth_req);
    if (i4_ret != 0)
    {
        BT_RC_LOG("[.c][%s]c_btm_bt_gatts_send_response fail\n", __FUNCTION__);
    }
    else
    {
        BT_RC_LOG("[.c][%s]c_btm_bt_gatts_send_response success\n", __FUNCTION__);
    }
    return i4_ret;
}

VOID x_mtkapi_bt_gatts_get_connect_result_info(BT_GATTS_CONNECT_RST_T *connect_rst_info)
{
    c_btm_bt_gatts_get_connect_result_info(connect_rst_info);
}

VOID x_mtkapi_bt_gatts_get_disconnect_result_info(BT_GATTS_CONNECT_RST_T *disconnect_rst_info)
{
    c_btm_bt_gatts_get_disconnect_result_info(disconnect_rst_info);
}

#if MTK_BLE_GGL_SETUP_SUPPORT
INT32 x_mtkapi_bt_gatt_initialize()
{
    INT32 i4_ret = 0;
    i4_ret = c_btm_gatt_initialize();
    if (i4_ret != 0)
    {
        BT_RC_LOG("[.c][%s]c_btm_gatt_initialize fail\n", __FUNCTION__);
    }
    else
    {
        BT_RC_LOG("[.c][%s]c_btm_gatt_initialize success\n", __FUNCTION__);
    }

    p_setCharValue = malloc(ATTRIBUTE_VALUE_MEM_SIZE);
    if (NULL == p_setCharValue)
    {
        printf("[.c][%s]p_setCharValue mem_alloc fail\n", __FUNCTION__);
        return -2;
    }
    else
    {
        p_setCharValuePtr = p_setCharValue;
        printf("[.c][%s]p_setCharValue memlloc success[%p]\n", __FUNCTION__, p_setCharValue);
        memset(p_setCharValue, 0, ATTRIBUTE_VALUE_MEM_SIZE);
    }
    return i4_ret;
}

INT32 x_mtkapi_bt_gatt_finalize()
{
    INT32 i4_ret = 0;
    i4_ret = c_btm_gatt_finalize();
    if (i4_ret != 0)
    {
        BT_RC_LOG("[.c][%s]c_btm_gatt_finalize fail\n", __FUNCTION__);
    }
    else
    {
        BT_RC_LOG("[.c][%s]c_btm_gatt_finalize success\n", __FUNCTION__);
    }
    if (p_setCharValue != NULL)
    {
        free(p_setCharValue);
        p_setCharValue = NULL;
        p_setCharValuePtr = NULL;
        printf("[.c][%s]p_setCharValue memory free success\n", __FUNCTION__);
    }
    return i4_ret;
}

INT32 x_mtkapi_bt_gatt_isSupported()
{
    INT32 i4_ret = 0;
    i4_ret = c_btm_gatt_isSupported();
    if (i4_ret != 0)
    {
        BT_RC_LOG("[.c][%s]c_btm_gatt_isSupported fail\n", __FUNCTION__);
    }
    else
    {
        BT_RC_LOG("[.c][%s]c_btm_gatt_isSupported success\n", __FUNCTION__);
    }

    return i4_ret;
}

INT32 x_mtkapi_bt_gatt_setDelegate(INT32 delegate)
{
    INT32 i4_ret = 0;
    i4_ret = c_btm_gatt_setDelegate(delegate);
    if (i4_ret != 0)
    {
        BT_RC_LOG("[.c][%s]c_btm_gatt_setDelegate fail\n", __FUNCTION__);
    }
    else
    {
        BT_RC_LOG("[.c][%s]c_btm_gatt_setDelegate success\n", __FUNCTION__);
    }

    return i4_ret;
}


INT32 x_mtkapi_bt_gatt_onCharacteristicWrite(char * service_uuid, char * char_uuid, char * value)
{
    INT32 i4_ret = 0;
    i4_ret = c_btm_gatt_onCharacteristicWrite(service_uuid, char_uuid, value);
    if (i4_ret != 0)
    {
        BT_RC_LOG("[.c][%s]c_btm_gatt_onCharacteristicWrite fail\n", __FUNCTION__);
    }
    else
    {
        BT_RC_LOG("[.c][%s]c_btm_gatt_onCharacteristicWrite success\n", __FUNCTION__);
    }

    return i4_ret;
}

INT32 x_mtkapi_bt_gatts_createService(char * service_uuid)
{
    INT32 i4_ret = 0;
    i4_ret = c_btm_gatt_createService(service_uuid);
    if (i4_ret != 0)
    {
        BT_RC_LOG("[.c][%s]c_btm_gatt_createService fail\n", __FUNCTION__);
    }
    else
    {
        BT_RC_LOG("[.c][%s]c_btm_gatt_createService success\n", __FUNCTION__);
    }

    return i4_ret;
}

INT32 x_mtkapi_bt_gatts_destroyService(char * service_uuid)
{
    INT32 i4_ret = 0;
    i4_ret = c_btm_gatt_destroyService(service_uuid);
    if (i4_ret != 0)
    {
        BT_RC_LOG("[.c][%s]c_btm_gatt_destroyService fail\n", __FUNCTION__);
    }
    else
    {
        BT_RC_LOG("[.c][%s]c_btm_gatt_destroyService success\n", __FUNCTION__);
    }

    return i4_ret;
}

INT32 x_mtkapi_bt_gatts_startService(char * service_uuid)
{
    INT32 i4_ret = 0;
    i4_ret = c_btm_gatt_startService(service_uuid);
    if (i4_ret != 0)
    {
        BT_RC_LOG("[.c][%s]c_btm_gatt_startService fail\n", __FUNCTION__);
    }
    else
    {
        BT_RC_LOG("[.c][%s]c_btm_gatt_startService success\n", __FUNCTION__);
    }

    return i4_ret;
}

INT32 x_mtkapi_bt_gatts_stopService(char * service_uuid)
{
    INT32 i4_ret = 0;
    i4_ret = c_btm_gatt_stopService(service_uuid);
    if (i4_ret != 0)
    {
        BT_RC_LOG("[.c][%s]c_btm_gatt_stopService fail\n", __FUNCTION__);
    }
    else
    {
        BT_RC_LOG("[.c][%s]c_btm_gatt_stopService success\n", __FUNCTION__);
    }

    return i4_ret;
}

INT32 x_mtkapi_bt_gatts_setDeviceName(char * name)
{
    INT32 i4_ret = 0;
    i4_ret = c_btm_gatt_setDeviceName(name);
    if (i4_ret != 0)
    {
        BT_RC_LOG("[.c][%s]c_btm_gatt_setDeviceName fail\n", __FUNCTION__);
    }
    else
    {
        BT_RC_LOG("[.c][%s]c_btm_gatt_setDeviceName success\n", __FUNCTION__);
    }

    return i4_ret;
}

INT32 x_mtkapi_bt_gatt_setAdvertisement(char * service_uuid, char ** advertised_uuids,
                                              UINT32 advertised_uuids_num, char * advertise_data,
                                              const char * manufacturer_data, bool transmit_name)
{
    INT32 ret = 0;
    UINT32 index = 0;
    for (index = 0; index < advertised_uuids_num; index++)
    {
        ret = c_btm_gatt_setAdvertisement(service_uuid, advertised_uuids[index], advertise_data,
                                         (char *)manufacturer_data, transmit_name);
        if (0 != ret)
        {
            BT_RC_LOG("[.c][%s]c_btm_gatt_setAdvertisement[%d] fail\n", __FUNCTION__, index);
            break;
        }
        else
        {
            BT_RC_LOG("[.c][%s]c_btm_gatt_setAdvertisement[%d] success\n", __FUNCTION__, index);
        }
    }

    return ret;
}

INT32 x_mtkapi_bt_gatt_setScanResponse(char * service_uuid, char ** advertised_uuids,
                                               UINT32 advertised_uuids_num, char * advertise_data,
                                               char * manufacturer_data, bool transmit_name)
{
    INT32 ret = 0;
    UINT32 index = 0;
    if(0 == advertised_uuids_num)
    {
        ret = c_btm_gatt_setScanResponse(service_uuid, advertised_uuids[index], advertise_data, manufacturer_data, transmit_name);
    }
    else
    {
        for (index = 0; index < advertised_uuids_num; index++)
        {
            ret = c_btm_gatt_setScanResponse(service_uuid, advertised_uuids[index], advertise_data, manufacturer_data, transmit_name);
            if (0 != ret)
            {
                break;
            }
        }
    }

    return ret;

}

INT32 x_mtkapi_bt_gatts_addCharacteristic(char * service_uuid, char * attribute_uuid,
                                                char * control_uuid, char ** options, UINT32 options_num)
{
    INT32 ret = 0;
    UINT32 index = 0;

    for (index = 0; index < options_num; index++)
    {
        ret = c_btm_gatt_addCharacteristic(service_uuid, attribute_uuid, control_uuid, options[index]);
        if (0 != ret)
        {
            BT_RC_LOG("[.c][%s]c_btm_gatt_addCharacteristic[%d] fail\n", __FUNCTION__, index);
            break;
        }
        else
        {
            BT_RC_LOG("[.c][%s]c_btm_gatt_addCharacteristic[%d] success\n", __FUNCTION__, index);
        }
    }

    return ret;
}

INT32 x_mtkapi_bt_gatts_setCharacteristicValue(char * service_uuid, char * attribute_uuid,
                                                void * value, UINT32 length,
                                                UINT32 max_length, UINT32 flag)
{
    int ret = 0;

    if (0 == flag && 0 == setCharValueCnt)
    {
        if (max_length > ATTRIBUTE_VALUE_MEM_SIZE)
        {
            BT_RC_LOG("[.c][%s][L:%d][YF]max_length[%d] > 16k, memory alloc again!\n", __FUNCTION__, __LINE__, max_length);
            if(NULL != p_setCharValue)
            {
                free(p_setCharValue);
                p_setCharValue = NULL;
                p_setCharValuePtr = NULL;
                p_setCharValue = malloc(max_length + 1);
                p_setCharValuePtr = p_setCharValue;
            }
        }

        if (NULL == p_setCharValue)
        {
            BT_RC_LOG("[.c][%s][L:%d][YF]ERROR:x_mem_alloc p_setCharValue fail!\n", __FUNCTION__, __LINE__);
            return -1;
        }
    }

    if (NULL != p_setCharValue)
    {
        memcpy(p_setCharValuePtr, value, length);
        p_setCharValuePtr += length;
        setCharValueCnt++;
        if (0 == flag)
        {
            return 0;
        }
        else
        {
            p_setCharValue[max_length] = '\0';
            ret = c_btm_gatt_setCharacteristicValue(service_uuid, attribute_uuid, p_setCharValue, max_length);
            if (ret != 0)
            {
                BT_RC_LOG("[.c][%s][L:%d][YF]c_btm_gatt_setCharacteristicValue fail\n", __FUNCTION__, __LINE__);
            }
            else
            {
                BT_RC_LOG("[.c][%s][L:%d][YF]c_btm_gatt_setCharacteristicValue success\n", __FUNCTION__, __LINE__);
            }
            setCharValueCnt = 0;
            p_setCharValuePtr = p_setCharValue;
            memset(p_setCharValue, 0, max_length);
            return ret;
        }
    }

    return ret;
}

INT32 x_mtkapi_bt_gatt_reg_nfy_fct(a_mtkapi_bt_gatt_nfy_fct_cbk pf_nfy, void *pv_tag)
{
    INT32 i4_ret = 0;
    i4_ret = c_btm_gatt_reg_nfy(pf_nfy, pv_tag);
    if (i4_ret != 0)
    {
        BT_RC_LOG("[.c][%s]c_btm_gatt_reg_nfy fail\n", __FUNCTION__);
    }
    else
    {
        BT_RC_LOG("[.c][%s]c_btm_gatt_reg_nfy success\n", __FUNCTION__);
    }

    return i4_ret;
}
#endif


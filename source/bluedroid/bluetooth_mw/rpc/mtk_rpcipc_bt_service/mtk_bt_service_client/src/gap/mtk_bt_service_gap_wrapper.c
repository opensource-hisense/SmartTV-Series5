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

#include "mtk_bt_service_gap_wrapper.h"
#include "mtk_bt_service_gap_ipcrpc_struct.h"
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

static INT32 _hndlr_bt_app_gap_inquiry_response_cbk(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_cb_type,
    void          *pv_cb_addr,
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    BT_RW_LOG("bt_app_gap_inquiry_response_cbk, pv_cb_addr = %p", pv_cb_addr);

    if(ui4_num_args != 1)
    {
        return RPCR_INV_ARGS;
    }
    pt_return->e_type   = ARG_TYPE_VOID;

    ((mtkrpcapi_BtAppGapInquiryResponseCbk)pv_cb_addr)((BT_GAP_SCAN_RESULT *)pt_args[0].u.pv_desc);
    return RPCR_OK;
}

static INT32 _hndlr_bt_app_gap_event_cbk(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_cb_type,
    void          *pv_cb_addr,
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    BT_RW_LOG("bt_app_gap_event_cbk, pv_cb_addr = %p", pv_cb_addr);

    if(ui4_num_args != 3)
    {
        return RPCR_INV_ARGS;
    }
    pt_return->e_type   = ARG_TYPE_VOID;

    ((mtkrpcapi_BtAppEventCbk)pv_cb_addr)((BTEvent)pt_args[0].u.i4_arg, pt_args[1].u.ps_str, pt_args[2].u.pv_arg);
    return RPCR_OK;
}

static INT32 _hndlr_bt_app_gap_get_pairing_key_cbk(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_cb_type,
    void          *pv_cb_addr,
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    BT_RW_LOG("bt_app_gap_get_pairing_key_cbk, pv_cb_addr = %p", pv_cb_addr);

    if(ui4_num_args != 3)
    {
        return RPCR_INV_ARGS;
    }
    pt_return->e_type   = ARG_TYPE_VOID;

    ((mtkrpcapi_BtAppGapGetPairingKeyCbk)pv_cb_addr)((pairing_key_value_t *)pt_args[0].u.pv_desc, pt_args[1].u.pui1_arg, pt_args[2].u.pv_arg);
    return RPCR_OK;
}

static INT32 _hndlr_bt_app_gap_upload_info_cbk(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_cb_type,
    void          *pv_cb_addr,
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    BT_RW_LOG("bt_app_gap_upload_info_cbk, pv_cb_addr = %p", pv_cb_addr);

    if(ui4_num_args != 3)
    {
        return RPCR_INV_ARGS;
    }
    pt_return->e_type   = ARG_TYPE_VOID;

    ((mtkrpcapi_BtAppA2dpUploadInfoCbk)pv_cb_addr)((BT_A2DP_UPLOAD_INFO *)pt_args[0].u.pv_desc, pt_args[1].u.b_arg,  pt_args[2].u.pv_arg);
    return RPCR_OK;
}

static INT32 _hndlr_bt_app_gap_playback_ctrl_cbk(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_cb_type,
    void          *pv_cb_addr,
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    BT_RW_LOG("bt_app_gap_playback_ctrl_cbk, pv_cb_addr = %p", pv_cb_addr);

    if(ui4_num_args != 2)
    {
        return RPCR_INV_ARGS;
    }
    pt_return->e_type   = ARG_TYPE_VOID;

    ((mtkrpcapi_BtAppA2dpPlaybackCtrlCbk)pv_cb_addr)((BTPlayState)pt_args[0].u.i4_arg, pt_args[1].u.pv_arg);
    return RPCR_OK;
}

static INT32 _hndlr_bt_app_gap_reset_audio_cbk(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_cb_type,
    void          *pv_cb_addr,
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    BT_RW_LOG("bt_app_gap_reset_audio_cbk, pv_cb_addr = %p", pv_cb_addr);

    if(ui4_num_args != 1)
    {
        return RPCR_INV_ARGS;
    }
    pt_return->e_type   = ARG_TYPE_VOID;

    ((mtkrpcapi_BtAppA2dpResetAudioCbk)pv_cb_addr)(pt_args[0].u.pv_arg);
    return RPCR_OK;
}

static INT32 _hndlr_bt_app_gap_play_change_cbk(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_cb_type,
    void          *pv_cb_addr,
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    BT_RW_LOG("bt_app_gap_play_change_cbk, pv_cb_addr = %p", pv_cb_addr);

    if(ui4_num_args != 2)
    {
        return RPCR_INV_ARGS;
    }
    pt_return->e_type   = ARG_TYPE_VOID;

    ((mtkrpcapi_BtAppAvrcpPlayStateChangeCbk)pv_cb_addr)((BTPlayState)pt_args[0].u.i4_arg, pt_args[1].u.pv_arg);
    return RPCR_OK;
}

static INT32 _hndlr_bt_app_gap_volume_change_cbk(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_cb_type,
    void          *pv_cb_addr,
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    BT_RW_LOG("bt_app_gap_volume_change_cbk, pv_cb_addr = %p", pv_cb_addr);

    if(ui4_num_args != 2)
    {
        return RPCR_INV_ARGS;
    }
    pt_return->e_type   = ARG_TYPE_VOID;

    ((mtkrpcapi_BtAppAvrcpVolumeChangeCbk)pv_cb_addr)((BTVolType)pt_args[0].u.i4_arg, pt_args[1].u.pv_arg);
    return RPCR_OK;
}

static INT32 _hndlr_bt_app_gap_track_change_cbk(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_cb_type,
    void          *pv_cb_addr,
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    BT_RW_LOG("bt_app_gap_track_change_cbk, pv_cb_addr = %p", pv_cb_addr);

    if(ui4_num_args != 2)
    {
        return RPCR_INV_ARGS;
    }
    pt_return->e_type   = ARG_TYPE_VOID;

    ((mtkrpcapi_BtAppAvrcpTrackChangeCbk)pv_cb_addr)(pt_args[0].u.ui4_arg, pt_args[1].u.pv_arg);
    return RPCR_OK;
}

static INT32 _hndlr_bt_app_gap_media_playstatus_cbk(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_cb_type,
    void          *pv_cb_addr,
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    BT_RW_LOG("bt_app_gap_media_playstatus_cbk, pv_cb_addr = %p", pv_cb_addr);

    if(ui4_num_args != 2)
    {
        return RPCR_INV_ARGS;
    }
    pt_return->e_type   = ARG_TYPE_VOID;

    ((mtkrpcapi_BtAppAvrcpMediaStatusCbk)pv_cb_addr)((BTMediaPlayState)pt_args[0].u.i4_arg, pt_args[1].u.pv_arg);
    return RPCR_OK;
}

static INT32 _hndlr_bt_app_gap_absolute_volume_cbk(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_cb_type,
    void          *pv_cb_addr,
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    BT_RW_LOG("bt_app_gap_absolute_volume_cbk, pv_cb_addr = %p", pv_cb_addr);

    if(ui4_num_args != 3)
    {
        return RPCR_INV_ARGS;
    }
    pt_return->e_type   = ARG_TYPE_VOID;

    ((mtkrpcapi_BtAppAvrcpAbsoluteVolumeCbk)pv_cb_addr)((BTAbsoluteVolumeType)pt_args[0].u.i4_arg, pt_args[1].u.pui1_arg, pt_args[2].u.pv_arg);
    return RPCR_OK;
}

static INT32 _hndlr_bt_app_gap_avrcp_cmd_cbk(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_cb_type,
    void          *pv_cb_addr,
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    BT_RW_LOG("bt_app_gap_avrcp_cmd_cbk, pv_cb_addr = %p", pv_cb_addr);

    if(ui4_num_args != 3)
    {
        return RPCR_INV_ARGS;
    }
    pt_return->e_type   = ARG_TYPE_VOID;

    ((mtkrpcapi_BtAppAvrcpCmdCbk)pv_cb_addr)((BTCmdType)pt_args[0].u.i4_arg, pt_args[1].u.ui1_arg, pt_args[2].u.pv_arg);
    return RPCR_OK;
}

static INT32 _hndlr_bt_app_get_play_status_cbk(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_cb_type,
    void          *pv_cb_addr,
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    BT_RW_LOG("bt_app_get_play_status_cbk, pv_cb_addr = %p", pv_cb_addr);

    if(ui4_num_args != 2)
    {
        return RPCR_INV_ARGS;
    }
    pt_return->e_type   = ARG_TYPE_VOID;

    ((mtkrpcapi_BtAppAvrcpGetPlayStatusCbk)pv_cb_addr)((BTPlayState *)pt_args[0].u.pi4_arg, pt_args[1].u.pv_arg);
    return RPCR_OK;
}

static INT32 _hndlr_bt_app_gap_bt_hid_cbk(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_cb_type,
    void          *pv_cb_addr,
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    BT_RW_LOG("bt_app_gap_bt_hid_cbk, pv_cb_addr = %p", pv_cb_addr);

    if(ui4_num_args != 2)
    {
        return RPCR_INV_ARGS;
    }
    pt_return->e_type   = ARG_TYPE_VOID;

    ((mtkrpcapi_BtAppHidCbk)pv_cb_addr)((BT_HID_CBK_STRUCT *)pt_args[0].u.pv_desc, pt_args[1].u.pv_arg);
    return RPCR_OK;
}

static INT32 _hndlr_bt_app_gap_spp_cbk(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_cb_type,
    void          *pv_cb_addr,
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    BT_RW_LOG("bt_app_gap_spp_cbk, pv_cb_addr = %p", pv_cb_addr);

    if(ui4_num_args != 2)
    {
        return RPCR_INV_ARGS;
    }
    pt_return->e_type   = ARG_TYPE_VOID;

    ((mtkrpcapi_BtAppSppCbk)pv_cb_addr)((BT_SPP_CBK_STRUCT *)pt_args[0].u.pv_desc, pt_args[1].u.pv_arg);
    return RPCR_OK;
}

static INT32 _hndlr_bt_app_hfclient_cbk(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_cb_type,
    void          *pv_cb_addr,
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    BT_RW_LOG("bt_app_hfclient_cbk, pv_cb_addr = %p", pv_cb_addr);

    if (ui4_num_args != 2)
    {
        return RPCR_INV_ARGS;
    }
    pt_return->e_type   = ARG_TYPE_VOID;

    ((mtkrpcapi_BtAppHfclientCbk)pv_cb_addr)((BT_HFCLIENT_CBK_STRUCT *)pt_args[0].u.pv_desc, pt_args[1].u.pv_arg);
    return RPCR_OK;
}

static INT32 _hndlr_bt_app_bond_cbk(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_cb_type,
    void          *pv_cb_addr,
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    BT_RW_LOG("bt_app_bond_cbk, pv_cb_addr = %p", pv_cb_addr);

    if(ui4_num_args != 2)
    {
        return RPCR_INV_ARGS;
    }
    pt_return->e_type   = ARG_TYPE_VOID;

    ((mtkrpcapi_BtAppBondCbk)pv_cb_addr)((BT_BOND_STATUS_STRUCT *)pt_args[0].u.pv_desc, pt_args[1].u.pv_arg);
    return RPCR_OK;
}

EXPORT_SYMBOL INT32 a_mtkapi_gap_bt_base_init(MTKRPCAPI_BT_APP_CB_FUNC * func, VOID* pv_tag)
{
    BT_RW_LOG("a_mtkapi_gap_bt_base_init");
    RPC_CLIENT_DECL(2, INT32);
    RPC_CLIENT_CHECK(bt_rpc_add_ref_desc(RPC_DEFAULT_ID,
                    func,
                    RPC_DESC_MTKRPCAPI_BT_APP_CB_FUNC,
                    NULL));
    RPC_CLIENT_ARG_INP(ARG_TYPE_REF_DESC, func);
    RPC_CLIENT_ARG_INP(ARG_TYPE_REF_VOID, pv_tag);
    RPC_BT_SERVICE_CLIENT_DO_OP("x_mtkapi_bt_base_init");
    RPC_CLIENT_RETURN(ARG_TYPE_INT32, -1);
}

EXPORT_SYMBOL INT32 a_mtkapi_bt_gap_set_dbg_level(BT_DEBUG_LAYER_NAME_T layer, INT32 level)
{
    RPC_CLIENT_DECL(2, INT32);
    RPC_CLIENT_ARG_INP(ARG_TYPE_INT32, layer);
    RPC_CLIENT_ARG_INP(ARG_TYPE_INT32, level);
    RPC_BT_SERVICE_CLIENT_DO_OP("x_mtkapi_bt_gap_set_dbg_level");
    RPC_CLIENT_RETURN(ARG_TYPE_INT32, -1);
}

EXPORT_SYMBOL INT32 a_mtkapi_bt_gap_btstart(const CHAR *ps_mode, UINT32 u4_overtime)
{
    RPC_CLIENT_DECL(2, INT32);
    RPC_CLIENT_ARG_INP(ARG_TYPE_REF_STR, ps_mode);
    RPC_CLIENT_ARG_INP(ARG_TYPE_UINT32, u4_overtime);
    RPC_BT_SERVICE_CLIENT_DO_OP("x_mtkapi_bt_gap_btstart");
    RPC_CLIENT_RETURN(ARG_TYPE_INT32, -1);
}

EXPORT_SYMBOL BT_MODE_TYPE a_mtkapi_bt_gap_get_mode(VOID)
{
    //Adding a invalid value for passing IPC/RPC, no other use
    INT32 i4_InvalidValue = 0;
    RPC_CLIENT_DECL(1, INT32);
    RPC_CLIENT_ARG_INP(ARG_TYPE_INT32, i4_InvalidValue);
    RPC_BT_SERVICE_CLIENT_DO_OP("x_mtkapi_bt_gap_get_mode");
    RPC_CLIENT_RETURN(ARG_TYPE_INT32, -1);
}

EXPORT_SYMBOL INT32 a_mtkapi_bt_gap_on_off(BOOL fg_on)
{
    RPC_CLIENT_DECL(1, INT32);
    RPC_CLIENT_ARG_INP(ARG_TYPE_BOOL, fg_on);
    RPC_BT_SERVICE_CLIENT_DO_OP("x_mtkapi_bt_gap_on_off");
    RPC_CLIENT_RETURN(ARG_TYPE_INT32, -1);
}

EXPORT_SYMBOL INT32 a_mtkapi_bt_gap_connecting(CHAR* ps_mac)
{
    RPC_CLIENT_DECL(1, INT32);
    RPC_CLIENT_ARG_INP(ARG_TYPE_REF_STR, ps_mac);
    RPC_BT_SERVICE_CLIENT_DO_OP("x_mtkapi_bt_gap_connecting");
    RPC_CLIENT_RETURN(ARG_TYPE_INT32, -1);
}

EXPORT_SYMBOL INT32 a_mtkapi_bt_gap_stop(BOOL fg_keep_conn, BOOL fg_discard_data)
{
    RPC_CLIENT_DECL(2, INT32);
    RPC_CLIENT_ARG_INP(ARG_TYPE_BOOL, fg_keep_conn);
    RPC_CLIENT_ARG_INP(ARG_TYPE_BOOL, fg_discard_data);
    RPC_BT_SERVICE_CLIENT_DO_OP("x_mtkapi_bt_gap_stop");
    RPC_CLIENT_RETURN(ARG_TYPE_INT32, -1);
}

EXPORT_SYMBOL INT32 a_mtkapi_bt_gap_clear_dev_info(VOID)
{
    //Adding a invalid value for passing IPC/RPC, no other use
    INT32 i4_InvalidValue = 0;
    RPC_CLIENT_DECL(1, INT32);
    RPC_CLIENT_ARG_INP(ARG_TYPE_INT32, i4_InvalidValue);
    RPC_BT_SERVICE_CLIENT_DO_OP("x_mtkapi_bt_gap_clear_dev_info");
    RPC_CLIENT_RETURN(ARG_TYPE_INT32, -1);
}

EXPORT_SYMBOL INT32 a_mtkapi_bt_bluetooth_factory_reset(CHAR *addr)
{
    RPC_CLIENT_DECL(1, INT32);
    RPC_CLIENT_ARG_INP(ARG_TYPE_REF_STR, addr);
    RPC_BT_SERVICE_CLIENT_DO_OP("x_mtkapi_bt_bluetooth_factory_reset");
    RPC_CLIENT_RETURN(ARG_TYPE_INT32, -1);
}

EXPORT_SYMBOL INT32 a_mtkapi_bt_gap_del_paired_av_dev_one(BT_TARGET_DEV_INFO *pt_device_info)
{
    RPC_CLIENT_DECL(1, INT32);
    RPC_CLIENT_CHECK(bt_rpc_add_ref_desc(RPC_DEFAULT_ID,
                    pt_device_info,
                    RPC_DESC_BT_TARGET_DEV_INFO,
                    NULL));
    RPC_CLIENT_ARG_INP(ARG_TYPE_REF_DESC, pt_device_info);
    RPC_BT_SERVICE_CLIENT_DO_OP("x_mtkapi_bt_gap_del_paired_av_dev_one");
    RPC_CLIENT_RETURN(ARG_TYPE_INT32, -1);
}

EXPORT_SYMBOL INT32 a_mtkapi_bt_gap_del_paired_av_dev_all(VOID)
{
    //Adding a invalid value for passing IPC/RPC, no other use
    INT32 i4_InvalidValue = 0;
    RPC_CLIENT_DECL(1, INT32);
    RPC_CLIENT_ARG_INP(ARG_TYPE_INT32, i4_InvalidValue);
    RPC_BT_SERVICE_CLIENT_DO_OP("x_mtkapi_bt_gap_del_paired_av_dev_all");
    RPC_CLIENT_RETURN(ARG_TYPE_INT32, -1);
}

EXPORT_SYMBOL INT32 a_mtkapi_bt_gap_auto_disconnection(VOID)
{
    //Adding a invalid value for passing IPC/RPC, no other use
    INT32 i4_InvalidValue = 0;
    RPC_CLIENT_DECL(1, INT32);
    RPC_CLIENT_ARG_INP(ARG_TYPE_INT32, i4_InvalidValue);
    RPC_BT_SERVICE_CLIENT_DO_OP("x_mtkapi_bt_gap_auto_disconnection");
    RPC_CLIENT_RETURN(ARG_TYPE_INT32, -1);
}

EXPORT_SYMBOL INT32 a_mtkapi_bt_gap_save_device_history(VOID)
{
    //Adding a invalid value for passing IPC/RPC, no other use
    INT32 i4_InvalidValue = 0;
    RPC_CLIENT_DECL(1, INT32);
    RPC_CLIENT_ARG_INP(ARG_TYPE_INT32, i4_InvalidValue);
    RPC_BT_SERVICE_CLIENT_DO_OP("x_mtkapi_bt_gap_save_device_history");
    RPC_CLIENT_RETURN(ARG_TYPE_INT32, -1);
}

EXPORT_SYMBOL INT32 a_mtkapi_bt_gap_set_name(CHAR *name)
{
    RPC_CLIENT_DECL(1, INT32);
    RPC_CLIENT_ARG_INP(ARG_TYPE_REF_STR, name);
    RPC_BT_SERVICE_CLIENT_DO_OP("x_mtkapi_bt_gap_set_name");
    RPC_CLIENT_RETURN(ARG_TYPE_INT32, -1);
}

EXPORT_SYMBOL INT32 a_mtkapi_bt_gap_set_connectable_and_discoverable(BOOL fg_conn, BOOL fg_disc)
{
    RPC_CLIENT_DECL(2, INT32);
    RPC_CLIENT_ARG_INP(ARG_TYPE_BOOL, fg_conn);
    RPC_CLIENT_ARG_INP(ARG_TYPE_BOOL, fg_disc);
    RPC_BT_SERVICE_CLIENT_DO_OP("x_mtkapi_bt_gap_set_connectable_and_discoverable");
    RPC_CLIENT_RETURN(ARG_TYPE_INT32, -1);
}

EXPORT_SYMBOL INT32 a_mtkapi_bt_gap_get_paired_device_info(VOID)
{
    //Adding a invalid value for passing IPC/RPC, no other use
    INT32 i4_InvalidValue = 0;
    RPC_CLIENT_DECL(1, INT32);
    RPC_CLIENT_ARG_INP(ARG_TYPE_INT32, i4_InvalidValue);
    RPC_BT_SERVICE_CLIENT_DO_OP("x_mtkapi_bt_gap_get_paired_device_info");
    RPC_CLIENT_RETURN(ARG_TYPE_INT32, -1);
}

EXPORT_SYMBOL INT32 a_mtkapi_bt_gap_get_local_dev_info(BT_GAP_LOCAL_PROPERTIES_RESULT *ps_dev_info)
{
    RPC_CLIENT_DECL(1, INT32);
    RPC_CLIENT_CHECK(bt_rpc_add_ref_desc(RPC_DEFAULT_ID,
                    ps_dev_info,
                    RPC_DESC_BT_GAP_LOCAL_PROPERTIES_RESULT,
                    NULL));
    RPC_CLIENT_ARG_OUT(ARG_TYPE_REF_DESC, ps_dev_info);
    RPC_BT_SERVICE_CLIENT_DO_OP("x_mtkapi_bt_gap_get_local_dev_info");
    RPC_CLIENT_RETURN(ARG_TYPE_INT32, -1);
}

EXPORT_SYMBOL INT32 a_mtkapi_bt_gap_get_target_info(BT_TARGET_DEV_INFO *pt_target_dev_info)
{
    RPC_CLIENT_DECL(1, INT32);
    RPC_CLIENT_CHECK(bt_rpc_add_ref_desc(RPC_DEFAULT_ID,
                    pt_target_dev_info,
                    RPC_DESC_BT_TARGET_DEV_INFO,
                    NULL));
    RPC_CLIENT_ARG_OUT(ARG_TYPE_REF_DESC, pt_target_dev_info);
    RPC_BT_SERVICE_CLIENT_DO_OP("x_mtkapi_bt_gap_get_target_info");
    RPC_CLIENT_RETURN(ARG_TYPE_INT32, -1);
}


EXPORT_SYMBOL INT32 a_mtkapi_bt_gap_inquiry_one(CHAR *ps_dev_mac, UINT32 ui4_inqDuration)
{
    RPC_CLIENT_DECL(2, INT32);
    RPC_CLIENT_ARG_INP(ARG_TYPE_REF_STR, ps_dev_mac);
    RPC_CLIENT_ARG_INP(ARG_TYPE_UINT32, ui4_inqDuration);
    RPC_BT_SERVICE_CLIENT_DO_OP("x_mtkapi_bt_gap_inquiry_one");
    RPC_CLIENT_RETURN(ARG_TYPE_INT32, -1);
}

EXPORT_SYMBOL INT32 a_mtkapi_bt_gap_start_inquiry_scan(UINT32 ui4_filter_type, BtAppGapInquiryResponseCbk pf_fct)
{
    RPC_CLIENT_DECL(2, INT32);
    RPC_CLIENT_ARG_INP(ARG_TYPE_UINT32, ui4_filter_type);
    RPC_CLIENT_ARG_INP(ARG_TYPE_REF_FUNC, pf_fct);
    RPC_BT_SERVICE_CLIENT_DO_OP("x_mtkapi_bt_gap_start_inquiry_scan");
    RPC_CLIENT_RETURN(ARG_TYPE_INT32, -1);
}

EXPORT_SYMBOL INT32 a_mtkapi_bt_gap_stop_inquiry_scan(VOID)
{
    //Adding a invalid value for passing IPC/RPC, no other use
    INT32 i4_InvalidValue = 0;
    RPC_CLIENT_DECL(1, INT32);
    RPC_CLIENT_ARG_INP(ARG_TYPE_INT32, i4_InvalidValue);
    RPC_BT_SERVICE_CLIENT_DO_OP("x_mtkapi_bt_gap_stop_inquiry_scan");
    RPC_CLIENT_RETURN(ARG_TYPE_INT32, -1);
}

EXPORT_SYMBOL INT32 a_mtkapi_bt_gap_pair(CHAR *addr, int transport)
{
    RPC_CLIENT_DECL(2, INT32);
    RPC_CLIENT_ARG_INP(ARG_TYPE_REF_STR, addr);
    RPC_CLIENT_ARG_INP(ARG_TYPE_INT32, transport);
    RPC_BT_SERVICE_CLIENT_DO_OP("x_mtkapi_bt_gap_pair");
    RPC_CLIENT_RETURN(ARG_TYPE_INT32, -1);
}

EXPORT_SYMBOL INT32 a_mtkapi_bt_gap_paired_dev_erase(CHAR *addr)
{
    RPC_CLIENT_DECL(1, INT32);
    RPC_CLIENT_ARG_INP(ARG_TYPE_REF_STR, addr);
    RPC_BT_SERVICE_CLIENT_DO_OP("x_mtkapi_bt_gap_paired_dev_erase");
    RPC_CLIENT_RETURN(ARG_TYPE_INT32, -1);
}

EXPORT_SYMBOL INT32 a_mtkapi_bt_gap_get_rssi(CHAR *address, INT16 *rssi_value)
{
    RPC_CLIENT_DECL(2, INT32);
    RPC_CLIENT_ARG_INP(ARG_TYPE_REF_STR, address);
    RPC_CLIENT_ARG_OUT(ARG_TYPE_REF_INT16, rssi_value);
    RPC_BT_SERVICE_CLIENT_DO_OP("x_mtkapi_bt_gap_get_rssi");
    RPC_CLIENT_RETURN(ARG_TYPE_INT32, -1);
}

EXPORT_SYMBOL INT32 a_mtkapi_bt_gap_set_virtual_sniffer(INT32 enable)
{
    RPC_CLIENT_DECL(1, INT32);
    RPC_CLIENT_ARG_INP(ARG_TYPE_INT32, enable);
    RPC_BT_SERVICE_CLIENT_DO_OP("x_mtkapi_bt_gap_set_virtual_sniffer");
    RPC_CLIENT_RETURN(ARG_TYPE_INT32, -1);
}

EXPORT_SYMBOL INT32 a_mtkapi_bt_gap_send_hci(CHAR *buffer)
{
    RPC_CLIENT_DECL(1, INT32);
    RPC_CLIENT_ARG_INP(ARG_TYPE_REF_STR, buffer);
    RPC_BT_SERVICE_CLIENT_DO_OP("x_mtkapi_bt_gap_send_hci");
    RPC_CLIENT_RETURN(ARG_TYPE_INT32, -1);
}

INT32 c_rpc_reg_mtk_bt_service_gap_cb_hndlrs(VOID)
{
    int i4_ret = 0;
    RPC_REG_CB_HNDLR(bt_app_gap_inquiry_response_cbk);
    RPC_REG_CB_HNDLR(bt_app_gap_event_cbk);
    RPC_REG_CB_HNDLR(bt_app_gap_get_pairing_key_cbk);
    RPC_REG_CB_HNDLR(bt_app_gap_upload_info_cbk);
    RPC_REG_CB_HNDLR(bt_app_gap_playback_ctrl_cbk);
    RPC_REG_CB_HNDLR(bt_app_gap_reset_audio_cbk);
    RPC_REG_CB_HNDLR(bt_app_gap_play_change_cbk);
    RPC_REG_CB_HNDLR(bt_app_gap_volume_change_cbk);
    RPC_REG_CB_HNDLR(bt_app_gap_track_change_cbk);
    RPC_REG_CB_HNDLR(bt_app_gap_media_playstatus_cbk);
    RPC_REG_CB_HNDLR(bt_app_gap_absolute_volume_cbk);
    RPC_REG_CB_HNDLR(bt_app_gap_avrcp_cmd_cbk);
    RPC_REG_CB_HNDLR(bt_app_get_play_status_cbk);
    RPC_REG_CB_HNDLR(bt_app_gap_bt_hid_cbk);
    RPC_REG_CB_HNDLR(bt_app_gap_spp_cbk);
    RPC_REG_CB_HNDLR(bt_app_hfclient_cbk);
    RPC_REG_CB_HNDLR(bt_app_bond_cbk);
    return RPCR_OK;
}



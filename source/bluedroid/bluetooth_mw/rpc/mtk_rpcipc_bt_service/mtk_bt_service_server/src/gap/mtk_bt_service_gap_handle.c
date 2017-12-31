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

#include "mtk_bt_service_gap.h"
#include "mtk_bt_service_gap_ipcrpc_struct.h"
#include "mtk_bt_service_gap_handle.h"
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

void *g_gap_ptag = NULL;

static VOID bt_app_gap_inquiry_response_cbk_wrapper(BT_GAP_SCAN_RESULT* pt_result, void* pv_tag)
{
    RPC_DECL_VOID(1);
    RPC_CB_NFY_TAG_T *pt_nfy_tag = (RPC_CB_NFY_TAG_T*)pv_tag;
    RPC_CHECK(bt_rpc_add_ref_desc(RPC_DEFAULT_ID, pt_result, RPC_DESC_BT_GAP_SCAN_RESULT, NULL));
    RPC_ARG_IO(ARG_TYPE_REF_DESC, pt_result);

    BT_RH_LOG("[_hndlr_]bt_app_inquiry_response_cbk_wrapper, name = %s, pt_nfy_tag->pv_cb_addr = %p\n",
               pt_result->name, pt_nfy_tag->pv_cb_addr);

    RPC_DO_CB(pt_nfy_tag->t_id, "bt_app_gap_inquiry_response_cbk", pt_nfy_tag->pv_cb_addr);

    RPC_RETURN_VOID;
}

VOID MWBtGapInquiryResponseCbk(BT_GAP_SCAN_RESULT* pt_result)
{
    bt_app_gap_inquiry_response_cbk_wrapper(pt_result, g_gap_ptag);
}

static VOID bt_app_gap_event_cbk_wrapper(BTEvent bt_event, CHAR *bdAddr, void* pv_tag)
{
    RPC_DECL_VOID(3);

    if (NULL != bdAddr)
    {
        BT_RH_LOG("bt_app_gap_event_cbk_wrapper, bt_event = %d, bdAddr = %s\n",
            bt_event, bdAddr);
    }
    else
    {
        BT_RH_LOG("bt_app_gap_event_cbk_wrapper, bt_event = %d, bdAddr = NULL\n",
            bt_event);
    }

    RPC_CB_NFY_TAG_T  *pt_nfy_tag = (RPC_CB_NFY_TAG_T*)pv_tag;
    RPC_ARG_INP(ARG_TYPE_INT32, bt_event);
    RPC_CHECK(bt_rpc_add_ref_buff(RPC_DEFAULT_ID, bdAddr, MAX_BDADDR_LEN*sizeof(CHAR)));
    RPC_ARG_IO(ARG_TYPE_REF_DESC, bdAddr);
    RPC_ARG_INP(ARG_TYPE_REF_VOID, pt_nfy_tag->pv_tag);

    RPC_DO_CB(pt_nfy_tag->t_id, "bt_app_gap_event_cbk", pt_nfy_tag->apv_cb_addr_ex[APP_EVENT_CB_IDX]);

    RPC_RETURN_VOID;
}

static VOID bt_app_gap_get_pairing_key_cbk_wrapper(pairing_key_value_t *bt_pairing_key, UINT8 *fg_accept, void* pv_tag)
{
    RPC_DECL_VOID(3);
    RPC_CB_NFY_TAG_T *pt_nfy_tag = (RPC_CB_NFY_TAG_T*)pv_tag;
    RPC_CHECK(bt_rpc_add_ref_desc(RPC_DEFAULT_ID, bt_pairing_key, RPC_DESC_pairing_key_value_t, NULL));
    RPC_ARG_IO(ARG_TYPE_REF_DESC, bt_pairing_key);
    RPC_ARG_INP(ARG_TYPE_REF_UINT8, fg_accept);
    RPC_ARG_INP(ARG_TYPE_REF_VOID, pt_nfy_tag->pv_tag);

    BT_RH_LOG("[_hndlr_]bt_app_gap_get_pairing_key_cbk_wrapper, key_type = %d, pt_nfy_tag->apv_cb_addr_ex[%d] = %p\n",
        bt_pairing_key->key_type, APP_GAP_GET_PAIRING_KEY_CB_IDX, pt_nfy_tag->apv_cb_addr_ex[APP_GAP_GET_PAIRING_KEY_CB_IDX]);

    RPC_DO_CB(pt_nfy_tag->t_id, "bt_app_gap_get_pairing_key_cbk", pt_nfy_tag->apv_cb_addr_ex[APP_GAP_GET_PAIRING_KEY_CB_IDX]);

    RPC_RETURN_VOID;
}

static VOID bt_app_gap_upload_info_cbk_wrapper(BT_A2DP_UPLOAD_INFO* pt_upload_info, BOOL fg_only_check_mute, void* pv_tag)
{
    RPC_DECL_VOID(3);
    RPC_CB_NFY_TAG_T *pt_nfy_tag = (RPC_CB_NFY_TAG_T*)pv_tag;
    RPC_CHECK(bt_rpc_add_ref_desc(RPC_DEFAULT_ID, pt_upload_info, RPC_DESC_BT_A2DP_UPLOAD_INFO, NULL));
    RPC_ARG_IO(ARG_TYPE_REF_DESC, pt_upload_info);
    RPC_ARG_INP(ARG_TYPE_BOOL, fg_only_check_mute);
    RPC_ARG_INP(ARG_TYPE_REF_VOID, pt_nfy_tag->pv_tag);

    BT_RH_LOG("[_hndlr_]bt_app_gap_upload_info_cbk_wrapper, fg_only_check_mute = %d, pt_nfy_tag->apv_cb_addr_ex[%d] = %p\n",
                (int)fg_only_check_mute, APP_A2DP_UPLOAD_INFO_CB_IDX, pt_nfy_tag->apv_cb_addr_ex[APP_A2DP_UPLOAD_INFO_CB_IDX]);

    RPC_DO_CB(pt_nfy_tag->t_id, "bt_app_gap_upload_info_cbk", pt_nfy_tag->apv_cb_addr_ex[APP_A2DP_UPLOAD_INFO_CB_IDX]);

    RPC_RETURN_VOID;
}

static VOID bt_app_gap_playback_ctrl_cbk_wrapper(BTPlayState bt_playback_cmd, void* pv_tag)
{
    RPC_DECL_VOID(2);
    RPC_CB_NFY_TAG_T *pt_nfy_tag = (RPC_CB_NFY_TAG_T*)pv_tag;
    RPC_ARG_INP(ARG_TYPE_INT32, bt_playback_cmd);
    RPC_ARG_INP(ARG_TYPE_REF_VOID, pt_nfy_tag->pv_tag);

    BT_RH_LOG("[_hndlr_]bt_app_gap_playback_ctrl_cbk_wrapper, bt_playback_cmd = %d, pt_nfy_tag->apv_cb_addr_ex[%d] = %p\n",
                (int)bt_playback_cmd, APP_A2DP_PLAYBACK_CTRL_CB_IDX, pt_nfy_tag->apv_cb_addr_ex[APP_A2DP_PLAYBACK_CTRL_CB_IDX]);

    RPC_DO_CB(pt_nfy_tag->t_id, "bt_app_gap_playback_ctrl_cbk", pt_nfy_tag->apv_cb_addr_ex[APP_A2DP_PLAYBACK_CTRL_CB_IDX]);

    RPC_RETURN_VOID;
}

static VOID bt_app_gap_reset_audio_cbk_wrapper(void* pv_tag)
{
    RPC_DECL_VOID(1);
    RPC_CB_NFY_TAG_T *pt_nfy_tag = (RPC_CB_NFY_TAG_T*)pv_tag;
    RPC_ARG_INP(ARG_TYPE_REF_VOID, pt_nfy_tag->pv_tag);

    RPC_DO_CB(pt_nfy_tag->t_id, "bt_app_gap_reset_audio_cbk", pt_nfy_tag->apv_cb_addr_ex[APP_A2DP_RESET_AUDIO_CB_IDX]);

    RPC_RETURN_VOID;
}

static VOID bt_app_gap_play_change_cbk_wrapper(BTPlayState bt_playstate, void* pv_tag)
{
    RPC_DECL_VOID(2);
    RPC_CB_NFY_TAG_T *pt_nfy_tag = (RPC_CB_NFY_TAG_T*)pv_tag;
    RPC_ARG_INP(ARG_TYPE_INT32, bt_playstate);
    RPC_ARG_INP(ARG_TYPE_REF_VOID, pt_nfy_tag->pv_tag);

    BT_RH_LOG("[_hndlr_]bt_app_gap_play_change_cbk_wrapper, bt_playstate = %d, pt_nfy_tag->apv_cb_addr_ex[%d] = %p\n",
                (int)bt_playstate, APP_AVRCP_PLAY_STATE_CHANGE_CB_IDX, pt_nfy_tag->apv_cb_addr_ex[APP_AVRCP_PLAY_STATE_CHANGE_CB_IDX]);

    RPC_DO_CB(pt_nfy_tag->t_id, "bt_app_gap_play_change_cbk", pt_nfy_tag->apv_cb_addr_ex[APP_AVRCP_PLAY_STATE_CHANGE_CB_IDX]);

    RPC_RETURN_VOID;
}

static VOID bt_app_gap_volume_change_cbk_wrapper(BTVolType bt_volume, void* pv_tag)
{
    RPC_DECL_VOID(2);
    RPC_CB_NFY_TAG_T *pt_nfy_tag = (RPC_CB_NFY_TAG_T*)pv_tag;
    RPC_ARG_INP(ARG_TYPE_INT32, bt_volume);
    RPC_ARG_INP(ARG_TYPE_REF_VOID, pt_nfy_tag->pv_tag);

    BT_RH_LOG("[_hndlr_]bt_app_gap_volume_change_cbk_wrapper, bt_volume = %d, pt_nfy_tag->apv_cb_addr_ex[%d] = %p\n",
                (int)bt_volume, APP_AVRCP_VOLUME_CHANGE_CB_IDX, pt_nfy_tag->apv_cb_addr_ex[APP_AVRCP_VOLUME_CHANGE_CB_IDX]);

    RPC_DO_CB(pt_nfy_tag->t_id, "bt_app_gap_volume_change_cbk", pt_nfy_tag->apv_cb_addr_ex[APP_AVRCP_VOLUME_CHANGE_CB_IDX]);

    RPC_RETURN_VOID;
}

static VOID bt_app_gap_track_change_cbk_wrapper(UINT32 bt_track_index, void* pv_tag)
{
    RPC_DECL_VOID(2);
    RPC_CB_NFY_TAG_T *pt_nfy_tag = (RPC_CB_NFY_TAG_T*)pv_tag;
    RPC_ARG_INP(ARG_TYPE_UINT32, bt_track_index);
    RPC_ARG_INP(ARG_TYPE_REF_VOID, pt_nfy_tag->pv_tag);

    BT_RH_LOG("[_hndlr_]bt_app_gap_track_change_cbk_wrapper, bt_track_index = %d, pt_nfy_tag->apv_cb_addr_ex[%d] = %p\n",
                (int)bt_track_index, APP_AVRCP_TRACK_CHANGE_CB_IDX, pt_nfy_tag->apv_cb_addr_ex[APP_AVRCP_TRACK_CHANGE_CB_IDX]);

    RPC_DO_CB(pt_nfy_tag->t_id, "bt_app_gap_track_change_cbk", pt_nfy_tag->apv_cb_addr_ex[APP_AVRCP_TRACK_CHANGE_CB_IDX]);

    RPC_RETURN_VOID;
}

static VOID bt_app_gap_media_playstatus_cbk_wrapper(BTMediaPlayState bt_status, void* pv_tag)
{
    RPC_DECL_VOID(2);
    RPC_CB_NFY_TAG_T *pt_nfy_tag = (RPC_CB_NFY_TAG_T*)pv_tag;
    RPC_ARG_INP(ARG_TYPE_INT32, bt_status);
    RPC_ARG_INP(ARG_TYPE_REF_VOID, pt_nfy_tag->pv_tag);

    BT_RH_LOG("[_hndlr_]bt_app_gap_media_playstatus_cbk_wrapper, bt_status = %d, pt_nfy_tag->apv_cb_addr_ex[%d] = %p\n",
                (int)bt_status, APP_AVRCP_MEDIA_STATUS_CB_IDX, pt_nfy_tag->apv_cb_addr_ex[APP_AVRCP_MEDIA_STATUS_CB_IDX]);

    RPC_DO_CB(pt_nfy_tag->t_id, "bt_app_gap_media_playstatus_cbk", pt_nfy_tag->apv_cb_addr_ex[APP_AVRCP_MEDIA_STATUS_CB_IDX]);

    RPC_RETURN_VOID;
}

static VOID bt_app_gap_absolute_volume_cbk_wrapper(BTAbsoluteVolumeType flag, UINT8 *bt_absolute_volume, void* pv_tag)
{
    RPC_DECL_VOID(3);
    RPC_CB_NFY_TAG_T *pt_nfy_tag = (RPC_CB_NFY_TAG_T*)pv_tag;
    RPC_ARG_INP(ARG_TYPE_INT32, flag);
    RPC_ARG_IO(ARG_TYPE_REF_UINT8, bt_absolute_volume);
    RPC_ARG_INP(ARG_TYPE_REF_VOID, pt_nfy_tag->pv_tag);

    BT_RH_LOG("[_hndlr_]bt_app_gap_absolute_volume_cbk_wrapper, flag = %d, bt_absolute_volume = %p, pt_nfy_tag->apv_cb_addr_ex[%d] = %p\n",
               (int)flag, bt_absolute_volume, APP_AVRCP_ABSOLUTE_VOLUME_CB_IDX, pt_nfy_tag->apv_cb_addr_ex[APP_AVRCP_ABSOLUTE_VOLUME_CB_IDX]);

    RPC_DO_CB(pt_nfy_tag->t_id, "bt_app_gap_absolute_volume_cbk", pt_nfy_tag->apv_cb_addr_ex[APP_AVRCP_ABSOLUTE_VOLUME_CB_IDX]);

    RPC_RETURN_VOID;
}

static VOID bt_app_gap_avrcp_cmd_cbk_wrapper(BTCmdType bt_cmd_type, UINT8 press, void* pv_tag)
{
    RPC_DECL_VOID(3);
    RPC_CB_NFY_TAG_T *pt_nfy_tag = (RPC_CB_NFY_TAG_T*)pv_tag;
    RPC_ARG_INP(ARG_TYPE_INT32, bt_cmd_type);
    RPC_ARG_INP(ARG_TYPE_UINT8, press);
    RPC_ARG_INP(ARG_TYPE_REF_VOID, pt_nfy_tag->pv_tag);

    BT_RH_LOG("[_hndlr_]bt_app_gap_avrcp_cmd_cbk_wrapper, bt_cmd_type = %d, press = %d, pt_nfy_tag->apv_cb_addr_ex[%d] = %p\n",
               (int)bt_cmd_type, press, APP_AVRCP_CMD_CB_IDX, pt_nfy_tag->apv_cb_addr_ex[APP_AVRCP_CMD_CB_IDX]);

    RPC_DO_CB(pt_nfy_tag->t_id, "bt_app_gap_avrcp_cmd_cbk", pt_nfy_tag->apv_cb_addr_ex[APP_AVRCP_CMD_CB_IDX]);

    RPC_RETURN_VOID;
}

static VOID bt_app_get_play_status_cbk_wrapper(BTPlayState *bt_playstatus, void* pv_tag)
{
    RPC_DECL_VOID(2);
    RPC_CB_NFY_TAG_T *pt_nfy_tag = (RPC_CB_NFY_TAG_T*)pv_tag;
    RPC_ARG_INP(ARG_TYPE_REF_INT32, bt_playstatus);
    RPC_ARG_INP(ARG_TYPE_REF_VOID, pt_nfy_tag->pv_tag);

    BT_RH_LOG("[_hndlr_]bt_app_get_play_status_cbk_wrapper, bt_playstatus = %d, pt_nfy_tag->apv_cb_addr_ex[%d] = %p\n",
               (int)bt_playstatus, APP_AVRCP_GET_PLAY_STATUS_CB_IDX, pt_nfy_tag->apv_cb_addr_ex[APP_AVRCP_GET_PLAY_STATUS_CB_IDX]);

    RPC_DO_CB(pt_nfy_tag->t_id, "bt_app_get_play_status_cbk", pt_nfy_tag->apv_cb_addr_ex[APP_AVRCP_GET_PLAY_STATUS_CB_IDX]);

    RPC_RETURN_VOID;
}

static VOID bt_app_gap_bt_hid_cbk_wrapper(BT_HID_CBK_STRUCT *pt_hid_struct, void* pv_tag)
{
    RPC_DECL_VOID(2);
    RPC_CB_NFY_TAG_T *pt_nfy_tag = (RPC_CB_NFY_TAG_T*)pv_tag;
    RPC_CHECK(bt_rpc_add_ref_desc(RPC_DEFAULT_ID, pt_hid_struct, RPC_DESC_BT_HID_CBK_STRUCT, NULL));
    RPC_ARG_IO(ARG_TYPE_REF_DESC, pt_hid_struct);
    RPC_ARG_INP(ARG_TYPE_REF_VOID, pt_nfy_tag->pv_tag);

    BT_RH_LOG("[_hndlr_]bt_app_gap_bt_hid_cbk_wrapper, current_addr = %s, pt_nfy_tag->apv_cb_addr_ex[%d] = %p\n",
               pt_hid_struct->current_addr, APP_HID_CB_IDX, pt_nfy_tag->apv_cb_addr_ex[APP_HID_CB_IDX]);

    RPC_DO_CB(pt_nfy_tag->t_id, "bt_app_gap_bt_hid_cbk", pt_nfy_tag->apv_cb_addr_ex[APP_HID_CB_IDX]);

    RPC_RETURN_VOID;
}

static VOID bt_app_gap_spp_cbk_wrapper(BT_SPP_CBK_STRUCT *pt_spp_struct, void* pv_tag)
{
    RPC_DECL_VOID(2);
    RPC_CB_NFY_TAG_T *pt_nfy_tag = (RPC_CB_NFY_TAG_T*)pv_tag;
    RPC_CHECK(bt_rpc_add_ref_desc(RPC_DEFAULT_ID, pt_spp_struct, RPC_DESC_BT_SPP_CBK_STRUCT, NULL));
    RPC_ARG_IO(ARG_TYPE_REF_DESC, pt_spp_struct);
    RPC_ARG_INP(ARG_TYPE_REF_VOID, pt_nfy_tag->pv_tag);

    BT_RH_LOG("[_hndlr_]bt_app_gap_spp_cbk_wrapper, uuid = %s, pt_nfy_tag->apv_cb_addr_ex[%d] = %p\n",
               pt_spp_struct->uuid, APP_SPP_CB_IDX, pt_nfy_tag->apv_cb_addr_ex[APP_SPP_CB_IDX]);

    RPC_DO_CB(pt_nfy_tag->t_id, "bt_app_gap_spp_cbk", pt_nfy_tag->apv_cb_addr_ex[APP_SPP_CB_IDX]);

    RPC_RETURN_VOID;
}

static VOID bt_app_hfclient_cbk_wrapper(BT_HFCLIENT_CBK_STRUCT *pt_hfclient_struct, VOID *pv_tag)
{
    RPC_DECL_VOID(2);
    RPC_CB_NFY_TAG_T *pt_nfy_tag = (RPC_CB_NFY_TAG_T*)pv_tag;
    RPC_CHECK(bt_rpc_add_ref_desc(RPC_DEFAULT_ID, pt_hfclient_struct, RPC_DESC_BT_HFCLIENT_CBK_STRUCT, NULL));
    RPC_ARG_IO(ARG_TYPE_REF_DESC, pt_hfclient_struct);
    RPC_ARG_INP(ARG_TYPE_REF_VOID, pt_nfy_tag->pv_tag);
    RPC_DO_CB(pt_nfy_tag->t_id, "bt_app_hfclient_cbk", pt_nfy_tag->apv_cb_addr_ex[APP_HFCLIENT_CB_IDX]);
    RPC_RETURN_VOID;
}

static VOID bt_app_bond_cbk_wrapper(BT_BOND_STATUS_STRUCT *pt_bond_struct, void* pv_tag)
{
    RPC_DECL_VOID(2);
    RPC_CB_NFY_TAG_T *pt_nfy_tag = (RPC_CB_NFY_TAG_T*)pv_tag;
    RPC_CHECK(bt_rpc_add_ref_desc(RPC_DEFAULT_ID, pt_bond_struct, RPC_DESC_BT_BOND_STATUS_STRUCT, NULL));
    RPC_ARG_IO(ARG_TYPE_REF_DESC, pt_bond_struct);
    RPC_ARG_INP(ARG_TYPE_REF_VOID, pt_nfy_tag->pv_tag);

    BT_RH_LOG("[_hndlr_]bt_app_bond_cbk_wrapper, t_nfy_tag->apv_cb_addr_ex[%d] = %p\n",
               APP_BOND_CB_IDX, pt_nfy_tag->apv_cb_addr_ex[APP_BOND_CB_IDX]);

    RPC_DO_CB(pt_nfy_tag->t_id, "bt_app_bond_cbk", pt_nfy_tag->apv_cb_addr_ex[APP_BOND_CB_IDX]);

    RPC_RETURN_VOID;
}

static INT32 _hndlr_x_mtkapi_bt_base_init(
                         RPC_ID_T     t_rpc_id,
                         const CHAR*  ps_cb_type,
                         UINT32       ui4_num_args,
                         ARG_DESC_T*  pt_args,
                         ARG_DESC_T*  pt_return)
{
    BT_RH_LOG("[_hndlr_]bt_gap_base_init , arg_1 = %ld\n", pt_args[0].u.i4_arg);

    MTKRPCAPI_BT_APP_CB_FUNC *p_bt_app_cb_func = NULL;
    MTKRPCAPI_BT_APP_CB_FUNC bt_app_cb_func;
    RPC_CB_NFY_TAG_T * pt_nfy_tag = NULL;
    VOID * apv_cb_addr[APP_CB_IDX_NUM] = {0};

    memset(&bt_app_cb_func,0,sizeof(MTKRPCAPI_BT_APP_CB_FUNC));

    BT_RH_LOG("bt_base_init, pt_args[0].u.pv_desc = %p\n", pt_args[0].u.pv_desc);

    if(ui4_num_args != 2)
    {
        return RPCR_INV_ARGS;
    }
    p_bt_app_cb_func = (MTKRPCAPI_BT_APP_CB_FUNC*)pt_args[0].u.pv_desc;

    if(p_bt_app_cb_func->bt_event_cb != NULL)
    {
        apv_cb_addr[APP_EVENT_CB_IDX] = p_bt_app_cb_func->bt_event_cb;
        bt_app_cb_func.bt_event_cb = bt_app_gap_event_cbk_wrapper;
    }

    if(p_bt_app_cb_func->bt_get_pairing_key_cb != NULL)
    {
        apv_cb_addr[APP_GAP_GET_PAIRING_KEY_CB_IDX] = p_bt_app_cb_func->bt_get_pairing_key_cb;
        bt_app_cb_func.bt_get_pairing_key_cb = bt_app_gap_get_pairing_key_cbk_wrapper;
    }

    if(p_bt_app_cb_func->bt_upload_info_cb != NULL)
    {
        apv_cb_addr[APP_A2DP_UPLOAD_INFO_CB_IDX] = p_bt_app_cb_func->bt_upload_info_cb;
        bt_app_cb_func.bt_upload_info_cb = bt_app_gap_upload_info_cbk_wrapper;
    }
    if (p_bt_app_cb_func->bt_playback_ctrl_cb != NULL)
    {
        apv_cb_addr[APP_A2DP_PLAYBACK_CTRL_CB_IDX] = p_bt_app_cb_func->bt_playback_ctrl_cb;
        bt_app_cb_func.bt_playback_ctrl_cb = bt_app_gap_playback_ctrl_cbk_wrapper;
    }
    if (p_bt_app_cb_func->bt_reset_audio_cb != NULL)
    {
        apv_cb_addr[APP_A2DP_RESET_AUDIO_CB_IDX] = p_bt_app_cb_func->bt_reset_audio_cb;
        bt_app_cb_func.bt_reset_audio_cb = bt_app_gap_reset_audio_cbk_wrapper;
    }
    if (p_bt_app_cb_func->bt_play_change_cb != NULL)
    {
        apv_cb_addr[APP_AVRCP_PLAY_STATE_CHANGE_CB_IDX] = p_bt_app_cb_func->bt_play_change_cb;
        bt_app_cb_func.bt_play_change_cb = bt_app_gap_play_change_cbk_wrapper;
    }
    if (p_bt_app_cb_func->bt_volume_change_cb != NULL)
    {
        apv_cb_addr[APP_AVRCP_VOLUME_CHANGE_CB_IDX] = p_bt_app_cb_func->bt_volume_change_cb;
        bt_app_cb_func.bt_volume_change_cb = bt_app_gap_volume_change_cbk_wrapper;
    }
    if (p_bt_app_cb_func->bt_track_change_cb != NULL)
    {
        apv_cb_addr[APP_AVRCP_TRACK_CHANGE_CB_IDX] = p_bt_app_cb_func->bt_track_change_cb;
        bt_app_cb_func.bt_track_change_cb = bt_app_gap_track_change_cbk_wrapper;
    }
    if (p_bt_app_cb_func->bt_media_playstatus_cb != NULL)
    {
        apv_cb_addr[APP_AVRCP_MEDIA_STATUS_CB_IDX] = p_bt_app_cb_func->bt_media_playstatus_cb;
        bt_app_cb_func.bt_media_playstatus_cb = bt_app_gap_media_playstatus_cbk_wrapper;
    }
    if (p_bt_app_cb_func->bt_absolute_volume_cb != NULL)
    {
        apv_cb_addr[APP_AVRCP_ABSOLUTE_VOLUME_CB_IDX] = p_bt_app_cb_func->bt_absolute_volume_cb;
        bt_app_cb_func.bt_absolute_volume_cb = bt_app_gap_absolute_volume_cbk_wrapper;
    }
    if (p_bt_app_cb_func->bt_avrcp_cmd_cb != NULL)
    {
        apv_cb_addr[APP_AVRCP_CMD_CB_IDX] = p_bt_app_cb_func->bt_avrcp_cmd_cb;
        bt_app_cb_func.bt_avrcp_cmd_cb = bt_app_gap_avrcp_cmd_cbk_wrapper;
    }
    if (p_bt_app_cb_func->bt_get_play_status_cb != NULL)
    {
        apv_cb_addr[APP_AVRCP_GET_PLAY_STATUS_CB_IDX] = p_bt_app_cb_func->bt_get_play_status_cb;
        bt_app_cb_func.bt_get_play_status_cb = bt_app_get_play_status_cbk_wrapper;
    }
    if (p_bt_app_cb_func->bt_hid_cb != NULL)
    {
        apv_cb_addr[APP_HID_CB_IDX] = p_bt_app_cb_func->bt_hid_cb;
        bt_app_cb_func.bt_hid_cb = bt_app_gap_bt_hid_cbk_wrapper;
    }
    if (p_bt_app_cb_func->bt_spp_cb != NULL)
    {
        apv_cb_addr[APP_SPP_CB_IDX] = p_bt_app_cb_func->bt_spp_cb;
        bt_app_cb_func.bt_spp_cb = bt_app_gap_spp_cbk_wrapper;
    }
    if (p_bt_app_cb_func->bt_hfclient_cb != NULL)
    {
        apv_cb_addr[APP_HFCLIENT_CB_IDX] = p_bt_app_cb_func->bt_hfclient_cb;
        bt_app_cb_func.bt_hfclient_cb = bt_app_hfclient_cbk_wrapper;
    }
    if (p_bt_app_cb_func->bt_bond_cb != NULL)
    {
        apv_cb_addr[APP_BOND_CB_IDX] = p_bt_app_cb_func->bt_bond_cb;
        bt_app_cb_func.bt_bond_cb = bt_app_bond_cbk_wrapper;
    }

    pt_nfy_tag  = ri_create_cb_tag(t_rpc_id, apv_cb_addr, APP_CB_IDX_NUM, pt_args[1].u.pv_arg);

    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_mtkapi_bt_base_init(&bt_app_cb_func, pt_nfy_tag);
    if (pt_return->u.i4_arg && pt_nfy_tag != NULL)
    {
        ri_free_cb_tag(pt_nfy_tag);
    }
    return RPCR_OK;

}

static INT32 _hndlr_x_mtkapi_bt_gap_set_dbg_level(
                         RPC_ID_T     t_rpc_id,
                         const CHAR*  ps_cb_type,
                         UINT32       ui4_num_args,
                         ARG_DESC_T*  pt_args,
                         ARG_DESC_T*  pt_return)
{
    BT_RH_LOG("[_hndlr_]bt_gap_set_dbg_layer , arg_1 = %ld\n", pt_args[0].u.i4_arg);
    BT_RH_LOG("[_hndlr_]bt_gap_set_dbg_level , arg_2 = %ld\n", pt_args[1].u.i4_arg);

    if (ui4_num_args != 2)
    {
        BT_RH_LOG("Invalid ARGS: %ld\n", ui4_num_args);
        return RPCR_INV_ARGS;
    }

    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_mtkapi_bt_gap_set_dbg_level(pt_args[0].u.i4_arg, pt_args[1].u.i4_arg);

    return RPCR_OK;
}

static INT32 _hndlr_x_mtkapi_bt_gap_btstart(
                         RPC_ID_T     t_rpc_id,
                         const CHAR*  ps_cb_type,
                         UINT32       ui4_num_args,
                         ARG_DESC_T*  pt_args,
                         ARG_DESC_T*  pt_return)
{
    BT_RH_LOG("[_hndlr_]bt_gap_btstart , arg_1 = %s, arg_2 = %lu\n", pt_args[0].u.ps_str, pt_args[1].u.ui4_arg);

    if (ui4_num_args != 2)
    {
        BT_RH_LOG("Invalid ARGS: %ld\n", ui4_num_args);
        return RPCR_INV_ARGS;
    }

    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_mtkapi_bt_gap_btstart(pt_args[0].u.ps_str,
                                                  pt_args[1].u.ui4_arg);

    return RPCR_OK;
}

static INT32 _hndlr_x_mtkapi_bt_gap_get_mode(
                         RPC_ID_T     t_rpc_id,
                         const CHAR*  ps_cb_type,
                         UINT32       ui4_num_args,
                         ARG_DESC_T*  pt_args,
                         ARG_DESC_T*  pt_return)
{
    BT_RH_LOG("[_hndlr_]bt_gap_get_mode , arg_1 = %ld\n", pt_args[0].u.i4_arg);

    if (ui4_num_args != 1)
    {
        BT_RH_LOG("Invalid ARGS: %ld\n", ui4_num_args);
        return RPCR_INV_ARGS;
    }

    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_mtkapi_bt_gap_get_mode();

    return RPCR_OK;
}

static INT32 _hndlr_x_mtkapi_bt_gap_on_off(
                         RPC_ID_T     t_rpc_id,
                         const CHAR*  ps_cb_type,
                         UINT32       ui4_num_args,
                         ARG_DESC_T*  pt_args,
                         ARG_DESC_T*  pt_return)
{
    BT_RH_LOG("[_hndlr_]bt_gap_on_off , arg_1 = %d\n", pt_args[0].u.b_arg);

    if (ui4_num_args != 1)
    {
        BT_RH_LOG("Invalid ARGS: %ld\n", ui4_num_args);
        return RPCR_INV_ARGS;
    }

    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_mtkapi_bt_gap_on_off(pt_args[0].u.b_arg);

    return RPCR_OK;
}

static INT32 _hndlr_x_mtkapi_bt_gap_connecting(
                         RPC_ID_T     t_rpc_id,
                         const CHAR*  ps_cb_type,
                         UINT32       ui4_num_args,
                         ARG_DESC_T*  pt_args,
                         ARG_DESC_T*  pt_return)
{
    BT_RH_LOG("[_hndlr_]bt_gap_connecting , arg_1 = %s\n", pt_args[0].u.pc_arg);

    if (ui4_num_args != 1)
    {
        BT_RH_LOG("Invalid ARGS: %ld\n", ui4_num_args);
        return RPCR_INV_ARGS;
    }

    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_mtkapi_bt_gap_connecting(pt_args[0].u.pc_arg);

    return RPCR_OK;
}

static INT32 _hndlr_x_mtkapi_bt_gap_stop(
                         RPC_ID_T     t_rpc_id,
                         const CHAR*  ps_cb_type,
                         UINT32       ui4_num_args,
                         ARG_DESC_T*  pt_args,
                         ARG_DESC_T*  pt_return)
{
    BT_RH_LOG("[_hndlr_]bt_gap_stop , arg_1 = %d, arg_2 = %d\n", pt_args[0].u.b_arg, pt_args[1].u.b_arg);

    if (ui4_num_args != 2)
    {
        BT_RH_LOG("Invalid ARGS: %ld\n", ui4_num_args);
        return RPCR_INV_ARGS;
    }

    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_mtkapi_bt_gap_stop(pt_args[0].u.b_arg,
                                               pt_args[1].u.b_arg);

    return RPCR_OK;
}

static INT32 _hndlr_x_mtkapi_bt_gap_clear_dev_info(
                         RPC_ID_T     t_rpc_id,
                         const CHAR*  ps_cb_type,
                         UINT32       ui4_num_args,
                         ARG_DESC_T*  pt_args,
                         ARG_DESC_T*  pt_return)
{
    BT_RH_LOG("[_hndlr_]bt_gap_stop\n");

    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_mtkapi_bt_gap_clear_dev_info();

    return RPCR_OK;
}

static INT32 _hndlr_x_mtkapi_bt_bluetooth_factory_reset(
                         RPC_ID_T     t_rpc_id,
                         const CHAR*  ps_cb_type,
                         UINT32       ui4_num_args,
                         ARG_DESC_T*  pt_args,
                         ARG_DESC_T*  pt_return)
{
    BT_RH_LOG("[_hndlr_]bt_factory_reset , arg_1 = %s\n", pt_args[0].u.ps_str);

    if (ui4_num_args != 1)
    {
        BT_RH_LOG("Invalid ARGS: %ld\n", ui4_num_args);
        return RPCR_INV_ARGS;
    }

    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_mtkapi_bt_bluetooth_factory_reset(pt_args[0].u.ps_str);

    return RPCR_OK;
}

static INT32 _hndlr_x_mtkapi_bt_gap_del_paired_av_dev_one(
                         RPC_ID_T     t_rpc_id,
                         const CHAR*  ps_cb_type,
                         UINT32       ui4_num_args,
                         ARG_DESC_T*  pt_args,
                         ARG_DESC_T*  pt_return)
{
    BT_RH_LOG("[_hndlr_]bt_gap_del_paired_av_dev_one , arg_1 = %p\n", pt_args[0].u.pv_desc);

    if (ui4_num_args != 1)
    {
        BT_RH_LOG("Invalid ARGS: %ld\n", ui4_num_args);
        return RPCR_INV_ARGS;
    }

    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_mtkapi_bt_gap_del_paired_av_dev_one(pt_args[0].u.pv_desc);

    return RPCR_OK;
}

static INT32 _hndlr_x_mtkapi_bt_gap_del_paired_av_dev_all(
                         RPC_ID_T     t_rpc_id,
                         const CHAR*  ps_cb_type,
                         UINT32       ui4_num_args,
                         ARG_DESC_T*  pt_args,
                         ARG_DESC_T*  pt_return)
{
    BT_RH_LOG("[_hndlr_]bt_gap_del_paired_av_dev_all\n");

    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_mtkapi_bt_gap_del_paired_av_dev_all();

    return RPCR_OK;
}

static INT32 _hndlr_x_mtkapi_bt_gap_auto_disconnection(
                         RPC_ID_T     t_rpc_id,
                         const CHAR*  ps_cb_type,
                         UINT32       ui4_num_args,
                         ARG_DESC_T*  pt_args,
                         ARG_DESC_T*  pt_return)
{
    BT_RH_LOG("[_hndlr_]bt_gap_stop\n");

    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_mtkapi_bt_gap_auto_disconnection();

    return RPCR_OK;
}

static INT32 _hndlr_x_mtkapi_bt_gap_save_device_history(
                         RPC_ID_T     t_rpc_id,
                         const CHAR*  ps_cb_type,
                         UINT32       ui4_num_args,
                         ARG_DESC_T*  pt_args,
                         ARG_DESC_T*  pt_return)
{
    BT_RH_LOG("[_hndlr_]bt_gap_save_device_history\n");

    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_mtkapi_bt_gap_save_device_history();

    return RPCR_OK;
}

static INT32 _hndlr_x_mtkapi_bt_gap_set_name(
                         RPC_ID_T     t_rpc_id,
                         const CHAR*  ps_cb_type,
                         UINT32       ui4_num_args,
                         ARG_DESC_T*  pt_args,
                         ARG_DESC_T*  pt_return)
{
    BT_RH_LOG("[_hndlr_]bt_gap_set_name , arg_1 = %s\n", pt_args[0].u.ps_str);

    if (ui4_num_args != 1)
    {
        BT_RH_LOG("Invalid ARGS: %ld\n", ui4_num_args);
        return RPCR_INV_ARGS;
    }

    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_mtkapi_bt_gap_set_name(pt_args[0].u.ps_str);

    return RPCR_OK;
}

static INT32 _hndlr_x_mtkapi_bt_gap_set_connectable_and_discoverable(
                         RPC_ID_T     t_rpc_id,
                         const CHAR*  ps_cb_type,
                         UINT32       ui4_num_args,
                         ARG_DESC_T*  pt_args,
                         ARG_DESC_T*  pt_return)
{
    BT_RH_LOG("[_hndlr_]bt_gap_set_connectable_and_discoverable , arg_1 = %d, arg_2 = %d\n", pt_args[0].u.b_arg, pt_args[1].u.b_arg);

    if (ui4_num_args != 2)
    {
        BT_RH_LOG("Invalid ARGS: %ld\n", ui4_num_args);
        return RPCR_INV_ARGS;
    }

    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_mtkapi_bt_gap_set_connectable_and_discoverable(pt_args[0].u.b_arg, pt_args[1].u.b_arg);

    return RPCR_OK;
}

static INT32 _hndlr_x_mtkapi_bt_gap_get_paired_device_info(
                         RPC_ID_T     t_rpc_id,
                         const CHAR*  ps_cb_type,
                         UINT32       ui4_num_args,
                         ARG_DESC_T*  pt_args,
                         ARG_DESC_T*  pt_return)
{
    BT_RH_LOG("[_hndlr_]bt_gap_get_paired_device_info\n");

    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_mtkapi_bt_gap_get_paired_device_info();

    return RPCR_OK;
}

static INT32 _hndlr_x_mtkapi_bt_gap_get_local_dev_info(
                         RPC_ID_T     t_rpc_id,
                         const CHAR*  ps_cb_type,
                         UINT32       ui4_num_args,
                         ARG_DESC_T*  pt_args,
                         ARG_DESC_T*  pt_return)
{
    BT_RH_LOG("[_hndlr_]bt_gap_get_local_dev_info , arg_1 = %p\n", pt_args[0].u.pv_desc);

    if (ui4_num_args != 1)
    {
        BT_RH_LOG("Invalid ARGS: %ld\n", ui4_num_args);
        return RPCR_INV_ARGS;
    }

    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_mtkapi_bt_gap_get_local_dev_info(pt_args[0].u.pv_desc);

    return RPCR_OK;
}

static INT32 _hndlr_x_mtkapi_bt_gap_get_target_info(
                         RPC_ID_T     t_rpc_id,
                         const CHAR*  ps_cb_type,
                         UINT32       ui4_num_args,
                         ARG_DESC_T*  pt_args,
                         ARG_DESC_T*  pt_return)
{
    BT_RH_LOG("[_hndlr_]bt_gap_get_target_info , arg_1 = %p\n", pt_args[0].u.pv_desc);

    if (ui4_num_args != 1)
    {
        BT_RH_LOG("Invalid ARGS: %ld\n", ui4_num_args);
        return RPCR_INV_ARGS;
    }

    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_mtkapi_bt_gap_get_target_info(pt_args[0].u.pv_desc);

    return RPCR_OK;
}

static INT32 _hndlr_x_mtkapi_bt_gap_inquiry_one(
                         RPC_ID_T     t_rpc_id,
                         const CHAR*  ps_cb_type,
                         UINT32       ui4_num_args,
                         ARG_DESC_T*  pt_args,
                         ARG_DESC_T*  pt_return)
{
    BT_RH_LOG("[_hndlr_]bt_gap_inquiry_one , arg_1 = %s, arg_2 = %lu\n", pt_args[0].u.ps_str, pt_args[1].u.ui4_arg);

    if (ui4_num_args != 2)
    {
        BT_RH_LOG("Invalid ARGS: %ld\n", ui4_num_args);
        return RPCR_INV_ARGS;
    }

    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_mtkapi_bt_gap_inquiry_one(pt_args[0].u.ps_str, pt_args[1].u.ui4_arg);

    return RPCR_OK;
}

static INT32 _hndlr_x_mtkapi_bt_gap_start_inquiry_scan(
                         RPC_ID_T     t_rpc_id,
                         const CHAR*  ps_cb_type,
                         UINT32       ui4_num_args,
                         ARG_DESC_T*  pt_args,
                         ARG_DESC_T*  pt_return)
{
    BT_RH_LOG("[_hndlr_]bt_gap_start_inquiry_scan , arg_1 = %lu, arg_2 = %p\n", pt_args[0].u.ui4_arg, pt_args[1].u.pv_func);
    RPC_CB_NFY_TAG_T * pt_nfy_tag;
    BtAppGapInquiryResponseCbk pf_nfy;

    BT_RH_LOG("reg_nfy_fct, pt_args[0].u.pv_func = %p\n", pt_args[1].u.pv_func);

    if (ui4_num_args != 2)
    {
        BT_RH_LOG("Invalid ARGS: %ld\n", ui4_num_args);
        return RPCR_INV_ARGS;
    }

    if (pt_args[1].u.pv_func == NULL)
    {
        pf_nfy      = NULL;
        pt_nfy_tag  = NULL;
    }
    else
    {
        pf_nfy = (BtAppGapInquiryResponseCbk)MWBtGapInquiryResponseCbk;
        //RPC_CB_TAG(t_rpc_id, pt_nfy_tag, pt_args[0].u.pv_func, pt_args[1].u.pv_arg);
        pt_nfy_tag = ri_create_cb_tag(t_rpc_id, &pt_args[1].u.pv_func, 1, NULL);
        g_gap_ptag = pt_nfy_tag;
    }

    BT_RH_LOG("reg_nfy_fct, pt_nfy_tag->pv_cb_addr = %p\n", pt_nfy_tag->pv_cb_addr);

    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_mtkapi_bt_gap_start_inquiry_scan(pt_args[0].u.ui4_arg, pf_nfy);

    if (pt_return->u.i4_arg == 0)
    {
        return RPCR_OK;
    }
    else
    {
        return RPCR_ERROR;
    }
}

static INT32 _hndlr_x_mtkapi_bt_gap_stop_inquiry_scan(
                         RPC_ID_T     t_rpc_id,
                         const CHAR*  ps_cb_type,
                         UINT32       ui4_num_args,
                         ARG_DESC_T*  pt_args,
                         ARG_DESC_T*  pt_return)
{
    BT_RH_LOG("[_hndlr_]bt_gap_stop_inquiry_scan\n");

    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_mtkapi_bt_gap_stop_inquiry_scan();

    if (pt_return->u.i4_arg == 0)
    {
        return RPCR_OK;
    }
    else
    {
        return RPCR_ERROR;
    }

}

static INT32 _hndlr_x_mtkapi_bt_gap_pair(
                         RPC_ID_T     t_rpc_id,
                         const CHAR*  ps_cb_type,
                         UINT32       ui4_num_args,
                         ARG_DESC_T*  pt_args,
                         ARG_DESC_T*  pt_return)
{
    BT_RH_LOG("[_hndlr_]bt_gap_pair , arg_1 = %s, arg_2 = %ld\n", pt_args[0].u.ps_str, pt_args[1].u.i4_arg);

    if (ui4_num_args != 2)
    {
        BT_RH_LOG("Invalid ARGS: %ld\n", ui4_num_args);
        return RPCR_INV_ARGS;
    }

    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_mtkapi_bt_gap_pair(pt_args[0].u.ps_str, pt_args[1].u.i4_arg);

    return RPCR_OK;
}

static INT32 _hndlr_x_mtkapi_bt_gap_paired_dev_erase(
                         RPC_ID_T     t_rpc_id,
                         const CHAR*  ps_cb_type,
                         UINT32       ui4_num_args,
                         ARG_DESC_T*  pt_args,
                         ARG_DESC_T*  pt_return)
{
    BT_RH_LOG("[_hndlr_]bt_gap_paired_dev_erase , arg_1 = %s\n", pt_args[0].u.ps_str);

    if (ui4_num_args != 1)
    {
        BT_RH_LOG("Invalid ARGS: %ld\n", ui4_num_args);
        return RPCR_INV_ARGS;
    }

    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_mtkapi_bt_gap_paired_dev_erase(pt_args[0].u.ps_str);

    return RPCR_OK;
}

static INT32 _hndlr_x_mtkapi_bt_gap_get_rssi(
                         RPC_ID_T     t_rpc_id,
                         const CHAR*  ps_cb_type,
                         UINT32       ui4_num_args,
                         ARG_DESC_T*  pt_args,
                         ARG_DESC_T*  pt_return)
{
    BT_RH_LOG("[_hndlr_]bt_gap_get_rssi , arg_1 = %s, arg_2 = %p\n", pt_args[0].u.ps_str, pt_args[1].u.pi2_arg);

    if (ui4_num_args != 2)
    {
        BT_RH_LOG("Invalid ARGS: %ld\n", ui4_num_args);
        return RPCR_INV_ARGS;
    }

    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_mtkapi_bt_gap_get_rssi(pt_args[0].u.ps_str, pt_args[1].u.pi2_arg);

    return RPCR_OK;
}

static INT32 _hndlr_x_mtkapi_bt_gap_set_virtual_sniffer(
                         RPC_ID_T     t_rpc_id,
                         const CHAR*  ps_cb_type,
                         UINT32       ui4_num_args,
                         ARG_DESC_T*  pt_args,
                         ARG_DESC_T*  pt_return)
{
    BT_RH_LOG("[_hndlr_]bt_gap_set_virtual_sniffer , arg_1 = %ld\n", pt_args[0].u.i4_arg);

    if (ui4_num_args != 1)
    {
        BT_RH_LOG("Invalid ARGS: %ld\n", ui4_num_args);
        return RPCR_INV_ARGS;
    }

    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_mtkapi_bt_gap_set_virtual_sniffer(pt_args[0].u.i4_arg);

    return RPCR_OK;
}

static INT32 _hndlr_x_mtkapi_bt_gap_send_hci(
                         RPC_ID_T     t_rpc_id,
                         const CHAR*  ps_cb_type,
                         UINT32       ui4_num_args,
                         ARG_DESC_T*  pt_args,
                         ARG_DESC_T*  pt_return)
{
    BT_RH_LOG("[_hndlr_]bt_gap_send_hci , arg_1 = %s\n", pt_args[0].u.ps_str);

    if (ui4_num_args != 1)
    {
        BT_RH_LOG("Invalid ARGS: %ld\n", ui4_num_args);
        return RPCR_INV_ARGS;
    }

    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_mtkapi_bt_gap_send_hci(pt_args[0].u.ps_str);

    return RPCR_OK;
}


INT32 c_rpc_reg_mtk_bt_service_gap_op_hndlrs(VOID)
{
    RPC_REG_OP_HNDLR(x_mtkapi_bt_base_init);
    RPC_REG_OP_HNDLR(x_mtkapi_bt_gap_set_dbg_level);
    RPC_REG_OP_HNDLR(x_mtkapi_bt_gap_btstart);
    RPC_REG_OP_HNDLR(x_mtkapi_bt_gap_get_mode);
    RPC_REG_OP_HNDLR(x_mtkapi_bt_gap_on_off);
    RPC_REG_OP_HNDLR(x_mtkapi_bt_gap_connecting);
    RPC_REG_OP_HNDLR(x_mtkapi_bt_gap_stop);
    RPC_REG_OP_HNDLR(x_mtkapi_bt_gap_clear_dev_info);
    RPC_REG_OP_HNDLR(x_mtkapi_bt_bluetooth_factory_reset);
    RPC_REG_OP_HNDLR(x_mtkapi_bt_gap_del_paired_av_dev_one);
    RPC_REG_OP_HNDLR(x_mtkapi_bt_gap_del_paired_av_dev_all);
    RPC_REG_OP_HNDLR(x_mtkapi_bt_gap_auto_disconnection);
    RPC_REG_OP_HNDLR(x_mtkapi_bt_gap_save_device_history);
    RPC_REG_OP_HNDLR(x_mtkapi_bt_gap_set_name);
    RPC_REG_OP_HNDLR(x_mtkapi_bt_gap_set_connectable_and_discoverable);
    RPC_REG_OP_HNDLR(x_mtkapi_bt_gap_get_paired_device_info);
    RPC_REG_OP_HNDLR(x_mtkapi_bt_gap_get_local_dev_info);
    RPC_REG_OP_HNDLR(x_mtkapi_bt_gap_get_target_info);
    RPC_REG_OP_HNDLR(x_mtkapi_bt_gap_inquiry_one);
    RPC_REG_OP_HNDLR(x_mtkapi_bt_gap_start_inquiry_scan);
    RPC_REG_OP_HNDLR(x_mtkapi_bt_gap_stop_inquiry_scan);
    RPC_REG_OP_HNDLR(x_mtkapi_bt_gap_pair);
    RPC_REG_OP_HNDLR(x_mtkapi_bt_gap_paired_dev_erase);
    RPC_REG_OP_HNDLR(x_mtkapi_bt_gap_get_rssi);
    RPC_REG_OP_HNDLR(x_mtkapi_bt_gap_set_virtual_sniffer);
    RPC_REG_OP_HNDLR(x_mtkapi_bt_gap_send_hci);
    return RPCR_OK;
}



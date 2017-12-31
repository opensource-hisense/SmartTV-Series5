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
#include "mtk_bt_service_gap.h"
#include "c_bt_mw_gap.h"
#include "c_bt_mw_manager.h"

#define BT_RC_LOG(_stmt...) \
        do{ \
            if(0){    \
                printf("Func:%s Line:%d--->: ", __FUNCTION__, __LINE__);   \
                printf(_stmt); \
                printf("\n"); \
            }        \
        }   \
        while(0)

void *g_gap_pvtag = NULL;

mtkrpcapi_BtAppEventCbk mtkrpcapi_BtEventCbk = NULL;
mtkrpcapi_BtAppGapGetPairingKeyCbk mtkrpcapi_BtGapGetPairingKeyCbk = NULL;
mtkrpcapi_BtAppA2dpUploadInfoCbk mtkrpcapi_BtA2dpUploadInfoCbk = NULL;
mtkrpcapi_BtAppA2dpPlaybackCtrlCbk mtkrpcapi_BtA2dpPlaybackCtrlCbk = NULL;
mtkrpcapi_BtAppA2dpResetAudioCbk mtkrpcapi_BtA2dpResetAudioCbk = NULL;
mtkrpcapi_BtAppAvrcpPlayStateChangeCbk mtkrpcapi_BtAvrcpPlayStateChangeCbk = NULL;
mtkrpcapi_BtAppAvrcpVolumeChangeCbk mtkrpcapi_BtAvrcpVolumeChangeCbk = NULL;
mtkrpcapi_BtAppAvrcpTrackChangeCbk mtkrpcapi_BtAvrcpTrackChangeCbk = NULL;
mtkrpcapi_BtAppAvrcpMediaStatusCbk mtkrpcapi_BtAvrcpMediaStatusCbk = NULL;
mtkrpcapi_BtAppAvrcpAbsoluteVolumeCbk mtkrpcapi_BtAvrcpAbsoluteVolumeCbk = NULL;
mtkrpcapi_BtAppAvrcpCmdCbk mtkrpcapi_BtAvrcpCmdCbk =NULL;
mtkrpcapi_BtAppAvrcpGetPlayStatusCbk mtkrpcapi_BtAvrcpGetPlayStatusCbk =NULL;
mtkrpcapi_BtAppHidCbk mtkrpcapi_BtHidCbk = NULL;
mtkrpcapi_BtAppSppCbk mtkrpcapi_BtSppCbk = NULL;
mtkrpcapi_BtAppHfclientCbk mtkrpcapi_BtHfclientCbk = NULL;
mtkrpcapi_BtAppBondCbk mtkrpcapi_BtBondCbk = NULL;

VOID MWBtAppEventCbk(BTEvent bt_event, CHAR *bdAddr)
{
    BT_RC_LOG("mtkrpcapi_BtEventCbk\n");
    if (mtkrpcapi_BtEventCbk)
    {
        mtkrpcapi_BtEventCbk(bt_event, bdAddr, g_gap_pvtag);
    }
    else
    {
        BT_RC_LOG("mtkrpcapi_BtEventCbk is null\n");
    }
}

VOID MWBtAppSppCbk(BT_SPP_CBK_STRUCT *pt_spp_struct)
{
    if (NULL == pt_spp_struct)
    {
        BT_RC_LOG("pt_spp_struct is null\n");
        return;
    }
    BT_RC_LOG("uuid = %s\n", pt_spp_struct->uuid);
    if (mtkrpcapi_BtSppCbk)
    {
        mtkrpcapi_BtSppCbk(pt_spp_struct, g_gap_pvtag);
    }
    else
    {
        BT_RC_LOG("mtkrpcapi_BtSppCbk is null\n");
    }
}

VOID MWBtAppHidCbk(BT_HID_CBK_STRUCT *pt_hid_struct)
{
    if (NULL == pt_hid_struct)
    {
        BT_RC_LOG("pt_hid_struct is null\n");
        return;
    }
    BT_RC_LOG("current_addr = %s\n", pt_hid_struct->current_addr);
    if (mtkrpcapi_BtHidCbk)
    {
        mtkrpcapi_BtHidCbk(pt_hid_struct, g_gap_pvtag);
    }
    else
    {
        BT_RC_LOG("mtkrpcapi_BtHidCbk is null\n");
    }
}

VOID MWBtAppA2dpUploadInfoCbk(BT_A2DP_UPLOAD_INFO* pt_upload_info, BOOL fg_only_check_mute)
{
    if (NULL == pt_upload_info)
    {
        BT_RC_LOG("pt_upload_info is null\n");
        return;
    }
    BT_RC_LOG("sample_freq = %d, fg_only_check_mute = %d\n", pt_upload_info->sample_freq, (int)fg_only_check_mute);
    if (mtkrpcapi_BtA2dpUploadInfoCbk)
    {
        mtkrpcapi_BtA2dpUploadInfoCbk(pt_upload_info, fg_only_check_mute, g_gap_pvtag);
    }
    else
    {
        BT_RC_LOG("mtkrpcapi_BtA2dpUploadInfoCbk is null\n");
    }
}

VOID MWBtAppA2dpPlaybackCtrlCbk(BTPlayState bt_playback_cmd)
{
    BT_RC_LOG("bt_playback_cmd = %d\n", (int)bt_playback_cmd);
    if (mtkrpcapi_BtA2dpPlaybackCtrlCbk)
    {
        mtkrpcapi_BtA2dpPlaybackCtrlCbk(bt_playback_cmd, g_gap_pvtag);
    }
    else
    {
        BT_RC_LOG("mtkrpcapi_BtA2dpPlaybackCtrlCbk is null\n");
    }
}

VOID MWBtAppA2dpResetAudioCbk()
{
    BT_RC_LOG("\n");
    if (mtkrpcapi_BtA2dpResetAudioCbk)
    {
        mtkrpcapi_BtA2dpResetAudioCbk(g_gap_pvtag);
    }
    else
    {
        BT_RC_LOG("mtkrpcapi_BtA2dpResetAudioCbk is null\n");
    }
}

VOID MWBtAppGapGetPairingKeyCbk(pairing_key_value_t *bt_pairing_key, UINT8 *fg_accept)
{
    if ((NULL == bt_pairing_key) || (NULL == fg_accept))
    {
        if (NULL == bt_pairing_key)
        {
            BT_RC_LOG("bt_pairing_key is null\n");
        }
        if (NULL == fg_accept)
        {
            BT_RC_LOG("fg_accept is null\n");
        }
        return;
    }
    BT_RC_LOG("pin_code = %s, key_value= %u, fg_accept=%u\n", bt_pairing_key->pin_code, bt_pairing_key->key_value, *fg_accept);
    if (mtkrpcapi_BtGapGetPairingKeyCbk)
    {
        mtkrpcapi_BtGapGetPairingKeyCbk(bt_pairing_key, fg_accept, g_gap_pvtag);
    }
    else
    {
        BT_RC_LOG("mtkrpcapi_BtGapGetPairingKeyCbk is null\n");
    }
}

VOID MWBtAppAvrcpPlayStateChangeCbk(BTPlayState bt_playstate)
{
    BT_RC_LOG("bt_playstate = %d\n", (int)bt_playstate);
    if (mtkrpcapi_BtAvrcpPlayStateChangeCbk)
    {
        mtkrpcapi_BtAvrcpPlayStateChangeCbk(bt_playstate, g_gap_pvtag);
    }
    else
    {
        BT_RC_LOG("mtkrpcapi_BtAvrcpPlayStateChangeCbk is null\n");
    }
}

VOID MWBtAppAvrcpVolumeChangeCbk(BTVolType bt_volume)
{
    BT_RC_LOG("bt_volume = %d\n", (int)bt_volume);
    if (mtkrpcapi_BtAvrcpVolumeChangeCbk)
    {
        mtkrpcapi_BtAvrcpVolumeChangeCbk(bt_volume, g_gap_pvtag);
    }
    else
    {
        BT_RC_LOG("mtkrpcapi_BtAvrcpVolumeChangeCbk is null\n");
    }
}

VOID MWBtAppAvrcpTrackChangeCbk(UINT32 bt_track_index)
{
    BT_RC_LOG("bt_track_index = %u\n", bt_track_index);
    if (mtkrpcapi_BtAvrcpTrackChangeCbk)
    {
        mtkrpcapi_BtAvrcpTrackChangeCbk(bt_track_index, g_gap_pvtag);
    }
    else
    {
        BT_RC_LOG("mtkrpcapi_BtAvrcpTrackChangeCbk is null\n");
    }
}

VOID MWBtAppAvrcpMediaStatusCbk(BTMediaPlayState bt_status)
{
    BT_RC_LOG("bt_status = %d\n", (int)bt_status);
    if (mtkrpcapi_BtAvrcpMediaStatusCbk)
    {
        mtkrpcapi_BtAvrcpMediaStatusCbk(bt_status, g_gap_pvtag);
    }
    else
    {
        BT_RC_LOG("mtkrpcapi_BtAvrcpMediaStatusCbk is null\n");
    }
}

VOID MWBtAppAvrcpAbsoluteVolumeCbk(BTAbsoluteVolumeType flag, UINT8 *bt_absolute_volume)
{
    if (NULL == bt_absolute_volume)
    {
        BT_RC_LOG("bt_absolute_volume is null\n");
        return;
    }
    BT_RC_LOG("flag = %d, bt_absolute_volume = %p\n", (int)flag, bt_absolute_volume);
    if (mtkrpcapi_BtAvrcpAbsoluteVolumeCbk)
    {
        mtkrpcapi_BtAvrcpAbsoluteVolumeCbk(flag, bt_absolute_volume, g_gap_pvtag);
    }
    else
    {
        BT_RC_LOG("mtkrpcapi_BtAvrcpAbsoluteVolumeCbk is null\n");
    }
}

VOID MWBtAppAvrcpGetPlayStatusCbk(BTPlayState *bt_playstatus)
{
    if (NULL == bt_playstatus)
    {
        BT_RC_LOG("bt_playstatus is null\n");
        return;
    }
    BT_RC_LOG("bt_playstatus = %d\n", (int)bt_playstatus);
    if (mtkrpcapi_BtAvrcpGetPlayStatusCbk)
    {
        mtkrpcapi_BtAvrcpGetPlayStatusCbk(bt_playstatus, g_gap_pvtag);
    }
    else
    {
        BT_RC_LOG("mtkrpcapi_BtAvrcpGetPlayStatusCbk is null\n");
    }
}

VOID MWBtAppAvrcpCmdCbk(BTCmdType bt_cmd_type, UINT8 press)
{
    BT_RC_LOG("bt_cmd_type = %d, press = %d\n", (int)bt_cmd_type, press);
    if (mtkrpcapi_BtAvrcpCmdCbk)
    {
        mtkrpcapi_BtAvrcpCmdCbk(bt_cmd_type, press, g_gap_pvtag);
    }
    else
    {
        BT_RC_LOG("mtkrpcapi_BtAvrcpCmdCbk is null\n");
    }
}

VOID MWBtAppBondCbk(BT_BOND_STATUS_STRUCT *pt_bond_struct)
{
    if (NULL == pt_bond_struct)
    {
        BT_RC_LOG("pt_bond_struct is null\n");
        return;
    }

    if (mtkrpcapi_BtBondCbk)
    {
        mtkrpcapi_BtBondCbk(pt_bond_struct, g_gap_pvtag);
    }
    else
    {
        BT_RC_LOG("mtkrpcapi_BtBondCbk is null\n");
    }
}

VOID MWBtAppHfclientCbk(BT_HFCLIENT_CBK_STRUCT *pt_hfclient_struct)
{
    if (NULL == pt_hfclient_struct)
    {
        BT_RC_LOG("pt_hfclient_struct is null\n");
        return;
    }
    BT_RC_LOG("event = %d\n", (int)pt_hfclient_struct->event);
    if (mtkrpcapi_BtHfclientCbk)
    {
        mtkrpcapi_BtHfclientCbk(pt_hfclient_struct, g_gap_pvtag);
    }
    else
    {
        BT_RC_LOG("mtkrpcapi_BtHfclientCbk is null\n");
    }
}

/*register APP callback function*/
INT32 mtkrpcapi_btm_gap_register_cbk_fct(MTKRPCAPI_BT_APP_CB_FUNC *func, BT_APP_CB_FUNC *app_func, void *pv_tag)
{
    INT32 i4_ret = 0;

    g_gap_pvtag = pv_tag;
    if(NULL == func)
    {
        BT_RC_LOG(("callback func is null!\n"));
        return BT_ERR_NULL_POINTER;
    }
    if(func->bt_event_cb)
    {
        mtkrpcapi_BtEventCbk = func->bt_event_cb;
        app_func->bt_event_cb = MWBtAppEventCbk;
    }
    else
    {
        BT_RC_LOG(("event callback func is null!\n"));
        app_func->bt_event_cb = NULL;
    }

    if(func->bt_get_pairing_key_cb)
    {
        mtkrpcapi_BtGapGetPairingKeyCbk = func->bt_get_pairing_key_cb;
        app_func->bt_get_pairing_key_cb = MWBtAppGapGetPairingKeyCbk;
    }
    else
    {
        BT_RC_LOG(("bt_get_pairing_key_cb callback func is null!\n"));
        app_func->bt_get_pairing_key_cb = NULL;
    }
    if(func->bt_upload_info_cb)
    {
        mtkrpcapi_BtA2dpUploadInfoCbk = func->bt_upload_info_cb;
        app_func->bt_upload_info_cb = MWBtAppA2dpUploadInfoCbk;
    }
    else
    {
        BT_RC_LOG(("bt_upload_info_cb callback func is null!\n"));
        app_func->bt_upload_info_cb = NULL;
    }

    if(func->bt_playback_ctrl_cb)
    {
        mtkrpcapi_BtA2dpPlaybackCtrlCbk = func->bt_playback_ctrl_cb;
        app_func->bt_playback_ctrl_cb = MWBtAppA2dpPlaybackCtrlCbk;
    }
    else
    {
        BT_RC_LOG(("bt_playback_ctrl_cb callback func is null!\n"));
        app_func->bt_playback_ctrl_cb = NULL;
    }

    if(func->bt_reset_audio_cb)
    {
        mtkrpcapi_BtA2dpResetAudioCbk = func->bt_reset_audio_cb;
        app_func->bt_reset_audio_cb= MWBtAppA2dpResetAudioCbk;
    }
    else
    {
        BT_RC_LOG(("bt_reset_audio_cb callback func is null!\n"));
        app_func->bt_reset_audio_cb = NULL;
    }

    if(func->bt_play_change_cb)
    {
        mtkrpcapi_BtAvrcpPlayStateChangeCbk = func->bt_play_change_cb;
        app_func->bt_play_change_cb = MWBtAppAvrcpPlayStateChangeCbk;
    }
    else
    {
        BT_RC_LOG(("bt_play_change_cb callback func is null!\n"));
        app_func->bt_play_change_cb = NULL;
    }

    if(func->bt_volume_change_cb)
    {
        mtkrpcapi_BtAvrcpVolumeChangeCbk = func->bt_volume_change_cb;
        app_func->bt_volume_change_cb = MWBtAppAvrcpVolumeChangeCbk;
    }
    else
    {
        BT_RC_LOG(("bt_volume_change_cb callback func is null!\n"));
        app_func->bt_volume_change_cb = NULL;
    }

    if(func->bt_track_change_cb)
    {
        mtkrpcapi_BtAvrcpTrackChangeCbk = func->bt_track_change_cb;
        app_func->bt_track_change_cb = MWBtAppAvrcpTrackChangeCbk;
    }
    else
    {
        BT_RC_LOG(("bt_track_change_cb callback func is null!\n"));
        app_func->bt_track_change_cb = NULL;
    }

    if(func->bt_media_playstatus_cb)
    {
        mtkrpcapi_BtAvrcpMediaStatusCbk = func->bt_media_playstatus_cb;
        app_func->bt_media_playstatus_cb = MWBtAppAvrcpMediaStatusCbk;
    }
    else
    {
        BT_RC_LOG(("bt_media_playstatus_cb callback func is null!\n"));
        app_func->bt_media_playstatus_cb = NULL;
    }

    if(func->bt_absolute_volume_cb)
    {
        mtkrpcapi_BtAvrcpAbsoluteVolumeCbk = func->bt_absolute_volume_cb;
        app_func->bt_absolute_volume_cb = MWBtAppAvrcpAbsoluteVolumeCbk;
    }
    else
    {
        BT_RC_LOG(("bt_absolute_volume_cb callback func is null!\n"));
        app_func->bt_absolute_volume_cb = NULL;
    }

    if(func->bt_avrcp_cmd_cb)
    {
        mtkrpcapi_BtAvrcpCmdCbk = func->bt_avrcp_cmd_cb;
        app_func->bt_avrcp_cmd_cb = MWBtAppAvrcpCmdCbk;
    }
    else
    {
        BT_RC_LOG(("bt_avrcp_cmd_cb callback func is null!\n"));
        app_func->bt_avrcp_cmd_cb = NULL;
    }

    if(func->bt_get_play_status_cb)
    {
        mtkrpcapi_BtAvrcpGetPlayStatusCbk = func->bt_get_play_status_cb;
        app_func->bt_get_play_status_cb = MWBtAppAvrcpGetPlayStatusCbk;
    }
    else
    {
        BT_RC_LOG(("bt_get_play_status_cb callback func is null!\n"));
        app_func->bt_get_play_status_cb = NULL;
    }

    if(func->bt_hid_cb)
    {
        mtkrpcapi_BtHidCbk = func->bt_hid_cb;
        app_func->bt_hid_cb = MWBtAppHidCbk;
    }
    else
    {
        BT_RC_LOG(("bt_hid_cb callback func is null!\n"));
        app_func->bt_hid_cb = NULL;
    }

    if(func->bt_spp_cb)
    {
        mtkrpcapi_BtSppCbk = func->bt_spp_cb;
        app_func->bt_spp_cb = MWBtAppSppCbk;
    }
    else
    {
        BT_RC_LOG(("bt_spp_cb callback func is null!\n"));
        app_func->bt_spp_cb = NULL;
    }

    if(func->bt_hfclient_cb)
    {
        mtkrpcapi_BtHfclientCbk = func->bt_hfclient_cb;
        app_func->bt_hfclient_cb = MWBtAppHfclientCbk;
    }
    else
    {
        BT_RC_LOG(("bt_hfclient_cb callback func is null!\n"));
        app_func->bt_hfclient_cb = NULL;
    }

    if(func->bt_bond_cb)
    {
        mtkrpcapi_BtBondCbk = func->bt_bond_cb;
        app_func->bt_bond_cb = MWBtAppBondCbk;
    }
    else
    {
        BT_RC_LOG(("bt_bond_cb callback func is null!\n"));
        app_func->bt_spp_cb = NULL;
    }

    return i4_ret;
}


INT32 x_mtkapi_bt_base_init(MTKRPCAPI_BT_APP_CB_FUNC* func, void *pv_tag)
{
    INT32 i4_ret = 0;

    BT_APP_CB_FUNC app_func;
    memset(&app_func, 0, sizeof(BT_APP_CB_FUNC));

    i4_ret = mtkrpcapi_btm_gap_register_cbk_fct(func, &app_func, pv_tag);
    if(0 != i4_ret)
    {
        BT_RC_LOG(("x_mtkapi_bt_base_init Error.\n"));
        return i4_ret;
    }

    i4_ret = c_btm_bt_base_init(&app_func);
    if (i4_ret != 0)
    {
        BT_RC_LOG("c_btm_bt_base_init fail\n");
    }
    else
    {
        BT_RC_LOG("c_btm_bt_base_init success\n");
    }

    return i4_ret;
}

INT32 x_mtkapi_bt_gap_set_dbg_level(BT_DEBUG_LAYER_NAME_T layer, INT32 level)
{
    INT32 i4_ret = 0;
    i4_ret = c_btm_set_dbg_level(layer, level);
    if (i4_ret != 0)
    {
        BT_RC_LOG("c_btm_set_dbg_level fail\n");
    }
    else
    {
        BT_RC_LOG("c_btm_set_dbg_level success\n");
    }
    return i4_ret;
}

INT32 x_mtkapi_bt_gap_btstart(const CHAR *ps_mode, UINT32 u4_overtime)
{
    INT32 i4_ret = 0;
    i4_ret = c_btm_btstart(ps_mode, u4_overtime);
    if (i4_ret != 0)
    {
        BT_RC_LOG("c_btm_btstart fail\n");
    }
    else
    {
        BT_RC_LOG("c_btm_btstart success\n");
    }
    return i4_ret;

}

INT32 x_mtkapi_bt_gap_get_mode()
{
    BT_MODE_TYPE i4_ret = 0;
    i4_ret = c_btm_get_mode();
    return (INT32)i4_ret;
}

INT32 x_mtkapi_bt_gap_on_off(BOOL fg_on)
{
    INT32 i4_ret = 0;
    i4_ret = c_btm_bluetooth_on_off(fg_on);
    if (i4_ret != 0)
    {
        BT_RC_LOG("c_btm_bluetooth_on_off fail\n");
    }
    else
    {
        BT_RC_LOG("c_btm_bluetooth_on_off success\n");
    }
    return i4_ret;
}

INT32 x_mtkapi_bt_gap_connecting(CHAR* ps_mac)
{
    INT32 i4_ret = 0;
    i4_ret = c_btm_connecting(ps_mac);
    if (i4_ret != 0)
    {
        BT_RC_LOG("c_btm_connecting fail\n");
    }
    else
    {
        BT_RC_LOG("c_btm_connecting success\n");
    }
    return i4_ret;
}

INT32 x_mtkapi_bt_gap_stop(BOOL fg_keep_conn, BOOL fg_discard_data)
{
    INT32 i4_ret = 0;
    i4_ret = c_btm_stop(fg_keep_conn, fg_discard_data);
    if (i4_ret != 0)
    {
        BT_RC_LOG("c_btm_stop fail\n");
    }
    else
    {
        BT_RC_LOG("c_btm_stop success\n");
    }
    return i4_ret;
}

INT32 x_mtkapi_bt_gap_clear_dev_info()
{
    INT32 i4_ret = 0;
    i4_ret = c_btm_clear_dev_info();
    if (i4_ret != 0)
    {
        BT_RC_LOG("c_btm_clear_dev_info fail\n");
    }
    else
    {
        BT_RC_LOG("c_btm_clear_dev_info success\n");
    }
    return i4_ret;
}

INT32 x_mtkapi_bt_bluetooth_factory_reset(CHAR *addr)
{
    INT32 i4_ret = 0;
    i4_ret = c_btm_bluetooth_factory_reset(addr);
    if (i4_ret != 0)
    {
        BT_RC_LOG("c_btm_bluetooth_factory_reset fail\n");
    }
    else
    {
        BT_RC_LOG("c_btm_bluetooth_factory_reset success\n");
    }
    return i4_ret;
}

INT32 x_mtkapi_bt_gap_del_paired_av_dev_one(BT_TARGET_DEV_INFO *pt_device_info)
{
    INT32 i4_ret = 0;
    i4_ret = c_btm_del_paired_av_dev_one(pt_device_info);
    if (i4_ret != 0)
    {
        BT_RC_LOG("c_btm_del_paired_av_dev_one fail\n");
    }
    else
    {
        BT_RC_LOG("c_btm_del_paired_av_dev_one success\n");
    }
    return i4_ret;
}

INT32 x_mtkapi_bt_gap_del_paired_av_dev_all()
{
    INT32 i4_ret = 0;
    i4_ret = c_btm_del_paired_av_dev_all();
    if (i4_ret != 0)
    {
        BT_RC_LOG("c_btm_del_paired_av_dev_all fail\n");
    }
    else
    {
        BT_RC_LOG("c_btm_del_paired_av_dev_all success\n");
    }
    return i4_ret;
}

INT32 x_mtkapi_bt_gap_auto_disconnection()
{
    INT32 i4_ret = 0;
    i4_ret = c_btm_bt_auto_disconnection();
    if (i4_ret != 0)
    {
        BT_RC_LOG("c_btm_bt_auto_disconnection fail\n");
    }
    else
    {
        BT_RC_LOG("c_btm_bt_auto_disconnection success\n");
    }
    return i4_ret;
}

INT32 x_mtkapi_bt_gap_save_device_history()
{
    INT32 i4_ret = 0;
    i4_ret = c_btm_save_device_history();
    if (i4_ret != 0)
    {
        BT_RC_LOG("c_btm_save_device_history fail\n");
    }
    else
    {
        BT_RC_LOG("c_btm_save_device_history success\n");
    }
    return i4_ret;
}

INT32 x_mtkapi_bt_gap_set_name(CHAR *name)
{
    INT32 i4_ret = 0;
    i4_ret = c_btm_set_local_name(name);
    if (i4_ret != 0)
    {
        BT_RC_LOG("c_btm_set_local_name fail\n");
    }
    else
    {
        BT_RC_LOG("c_btm_set_local_name success\n");
    }
    return i4_ret;
}

INT32 x_mtkapi_bt_gap_set_connectable_and_discoverable(BOOL fg_conn, BOOL fg_disc)
{
    INT32 i4_ret = 0;
    i4_ret = c_btm_set_connectable_and_discoverable(fg_conn, fg_disc);
    if (i4_ret != 0)
    {
        BT_RC_LOG("c_btm_set_connectable_and_discoverable fail\n");
    }
    else
    {
        BT_RC_LOG("c_btm_set_connectable_and_discoverable success\n");
    }
    return i4_ret;
}

INT32 x_mtkapi_bt_gap_get_paired_device_info()
{
    INT32 i4_ret = 0;
    i4_ret = c_btm_bluetooth_get_bonded_device();
    if (i4_ret != 0)
    {
        BT_RC_LOG("c_btm_bluetooth_get_bonded_device fail\n");
    }
    else
    {
        BT_RC_LOG("c_btm_bluetooth_get_bonded_device success\n");
    }
    return i4_ret;
}

INT32 x_mtkapi_bt_gap_get_local_dev_info(BT_GAP_LOCAL_PROPERTIES_RESULT *ps_dev_info)
{
    INT32 i4_ret = 0;
    i4_ret = c_btm_get_local_dev_info(ps_dev_info);
    if (i4_ret != 0)
    {
        BT_RC_LOG("c_btm_get_local_dev_info fail\n");
    }
    else
    {
        BT_RC_LOG("c_btm_get_local_dev_info success\n");
    }
    return i4_ret;
}

INT32 x_mtkapi_bt_gap_get_target_info(BT_TARGET_DEV_INFO *pt_target_dev_info)
{
    INT32 i4_ret = 0;
    i4_ret = c_btm_get_target_info(pt_target_dev_info);
    if (i4_ret != 0)
    {
        BT_RC_LOG("c_btm_get_target_info fail\n");
    }
    else
    {
        BT_RC_LOG("c_btm_get_target_info success\n");
    }
    return i4_ret;
}

INT32 x_mtkapi_bt_gap_inquiry_one(CHAR *ps_dev_mac, UINT32 ui4_inqDuration)
{
    INT32 i4_ret = 0;
    i4_ret = c_btm_inquiry_one(ps_dev_mac, ui4_inqDuration);
    if (i4_ret != 0)
    {
        BT_RC_LOG("c_btm_inquiry_one fail\n");
    }
    else
    {
        BT_RC_LOG("c_btm_inquiry_one success\n");
    }
    return i4_ret;
}

INT32 x_mtkapi_bt_gap_start_inquiry_scan(UINT32 ui4_filter_type, BtAppGapInquiryResponseCbk pf_fct)
{
    INT32 i4_ret = 0;
    i4_ret = c_btm_start_inquiry_scan(ui4_filter_type, pf_fct);
    if (i4_ret != 0)
    {
        BT_RC_LOG("c_btm_start_inquiry_scan fail\n");
    }
    else
    {
        BT_RC_LOG("c_btm_start_inquiry_scan success\n");
    }
    return i4_ret;
}

INT32 x_mtkapi_bt_gap_stop_inquiry_scan()
{
    INT32 i4_ret = 0;
    i4_ret = c_btm_stop_inquiry_scan();
    if (i4_ret != 0)
    {
        BT_RC_LOG("c_btm_start_inquiry_scan fail\n");
    }
    else
    {
        BT_RC_LOG("c_btm_start_inquiry_scan success\n");
    }
    return i4_ret;
}

INT32 x_mtkapi_bt_gap_pair(CHAR *addr, int transport)
{
    INT32 i4_ret = 0;
    i4_ret = c_btm_pair(addr, transport);
    if (i4_ret != 0)
    {
        BT_RC_LOG("c_btm_pair fail\n");
    }
    else
    {
        BT_RC_LOG("c_btm_pair success\n");
    }
    return i4_ret;
}

INT32 x_mtkapi_bt_gap_paired_dev_erase(CHAR *addr)
{
    INT32 i4_ret = 0;
    i4_ret = c_btm_paired_dev_erase(addr);
    if (i4_ret != 0)
    {
        BT_RC_LOG("c_btm_paired_dev_erase fail\n");
    }
    else
    {
        BT_RC_LOG("c_btm_paired_dev_erase success\n");
    }
    return i4_ret;
}

INT32 x_mtkapi_bt_gap_get_rssi(CHAR *address, INT16 *rssi_value)
{
    INT32 i4_ret = 0;
    i4_ret = c_btm_get_rssi(address, rssi_value);
    if (i4_ret != 0)
    {
        BT_RC_LOG("c_btm_get_rssi fail\n");
    }
    else
    {
        BT_RC_LOG("c_btm_get_rssi success\n");
    }
    return i4_ret;
}

INT32 x_mtkapi_bt_gap_set_virtual_sniffer(INT32 enable)
{
    INT32 i4_ret = 0;
    i4_ret = c_btm_set_virtual_sniffer(enable);
    if (i4_ret != 0)
    {
        BT_RC_LOG("c_btm_set_virtual_sniffer fail\n");
    }
    else
    {
        BT_RC_LOG("c_btm_set_virtual_sniffer success\n");
    }
    return i4_ret;
}

INT32 x_mtkapi_bt_gap_send_hci(CHAR *buffer)
{
    INT32 i4_ret = 0;
    c_btm_send_hci(buffer);
    return i4_ret;
}



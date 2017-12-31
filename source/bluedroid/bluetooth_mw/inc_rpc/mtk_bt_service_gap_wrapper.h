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
 *     FEES OR SERVICE charGE PAID BY BUYER TO MEDIATEK FOR SUCH MEDIATEK SOFTWARE AT ISSUE.
 *
 *     THE TRANSACTION CONTEMPLATED HEREUNDER SHALL BE CONSTRUED IN ACCORDANCE WITH THE LAWS
 *     OF THE STATE OF CALIFORNIA, USA, EXCLUDING ITS CONFLICT OF LAWS PRINCIPLES.
 ************************************************************************************************/

#ifndef _MTK_BT_SERVICE_GAP_WRAPPER_H_
#define _MTK_BT_SERVICE_GAP_WRAPPER_H_

#include "u_rpcipc_types.h"
#include "u_bluetooth.h"
#include "u_bt_mw_avrcp.h"

#ifdef  __cplusplus
extern "C" {
#endif

typedef VOID (*mtkrpcapi_BtAppEventCbk)(BTEvent bt_event, CHAR* bdAddr, void* pv_tag);
typedef VOID (*mtkrpcapi_BtAppGapGetPairingKeyCbk)(pairing_key_value_t *bt_pairing_key, UINT8 *fg_accept, void* pv_tag);
typedef VOID (*mtkrpcapi_BtAppA2dpUploadInfoCbk)(BT_A2DP_UPLOAD_INFO* pt_upload_info, BOOL fg_only_check_mute, void* pv_tag);
typedef VOID (*mtkrpcapi_BtAppA2dpPlaybackCtrlCbk)(BTPlayState bt_playback_cmd, void* pv_tag);
typedef VOID (*mtkrpcapi_BtAppA2dpResetAudioCbk)(void* pv_tag);
typedef VOID (*mtkrpcapi_BtAppAvrcpPlayStateChangeCbk)(BTPlayState bt_playstate, void* pv_tag);
typedef VOID (*mtkrpcapi_BtAppAvrcpVolumeChangeCbk)(BTVolType bt_volume, void* pv_tag);
typedef VOID (*mtkrpcapi_BtAppAvrcpTrackChangeCbk)(UINT32 bt_track_index, void* pv_tag);
typedef VOID (*mtkrpcapi_BtAppAvrcpMediaStatusCbk)(BTMediaPlayState bt_status, void* pv_tag);
typedef VOID (*mtkrpcapi_BtAppAvrcpAbsoluteVolumeCbk)(BTAbsoluteVolumeType flag, UINT8 *bt_absolute_volume, void* pv_tag);
typedef VOID (*mtkrpcapi_BtAppAvrcpCmdCbk)(BTCmdType bt_cmd_type, UINT8 press, void* pv_tag);
typedef VOID (*mtkrpcapi_BtAppAvrcpGetPlayStatusCbk)(BTPlayState *bt_playstatus, void* pv_tag);
typedef VOID (*mtkrpcapi_BtAppHidCbk)(BT_HID_CBK_STRUCT *pt_hid_struct, void* pv_tag);
typedef VOID (*mtkrpcapi_BtAppSppCbk)(BT_SPP_CBK_STRUCT *pt_spp_struct, void* pv_tag);
typedef VOID (*mtkrpcapi_BtAppGapInquiryResponseCbk)(BT_GAP_SCAN_RESULT* pt_result);
typedef VOID (*mtkrpcapi_BtAppHfclientCbk)(BT_HFCLIENT_CBK_STRUCT *pt_hfclient_struct, VOID *pv_tag);
typedef VOID (*mtkrpcapi_BtAppBondCbk)(BT_BOND_STATUS_STRUCT *pt_spp_struct, void* pv_tag);

typedef struct
{
    mtkrpcapi_BtAppEventCbk bt_event_cb;
    mtkrpcapi_BtAppGapGetPairingKeyCbk bt_get_pairing_key_cb;
    mtkrpcapi_BtAppA2dpUploadInfoCbk bt_upload_info_cb;
    mtkrpcapi_BtAppA2dpPlaybackCtrlCbk bt_playback_ctrl_cb;
    mtkrpcapi_BtAppA2dpResetAudioCbk bt_reset_audio_cb;
    mtkrpcapi_BtAppAvrcpPlayStateChangeCbk bt_play_change_cb;
    mtkrpcapi_BtAppAvrcpVolumeChangeCbk bt_volume_change_cb;
    mtkrpcapi_BtAppAvrcpTrackChangeCbk bt_track_change_cb;
    mtkrpcapi_BtAppAvrcpMediaStatusCbk bt_media_playstatus_cb;
    mtkrpcapi_BtAppAvrcpAbsoluteVolumeCbk bt_absolute_volume_cb;
    mtkrpcapi_BtAppAvrcpCmdCbk bt_avrcp_cmd_cb;
    mtkrpcapi_BtAppAvrcpGetPlayStatusCbk bt_get_play_status_cb;
    mtkrpcapi_BtAppHidCbk bt_hid_cb;
    mtkrpcapi_BtAppSppCbk bt_spp_cb;
    mtkrpcapi_BtAppHfclientCbk bt_hfclient_cb;
    mtkrpcapi_BtAppBondCbk bt_bond_cb;
}MTKRPCAPI_BT_APP_CB_FUNC;

extern INT32 a_mtkapi_gap_bt_base_init(MTKRPCAPI_BT_APP_CB_FUNC * func, VOID* pv_tag);
extern INT32 a_mtkapi_bt_gap_set_dbg_level(BT_DEBUG_LAYER_NAME_T layer, INT32 level);
extern INT32 a_mtkapi_bt_gap_btstart(const CHAR *ps_mode, UINT32 u4_overtime);
extern BT_MODE_TYPE a_mtkapi_bt_gap_get_mode(VOID);
extern INT32 a_mtkapi_bt_gap_on_off(BOOL fg_on);
extern INT32 a_mtkapi_bt_gap_connecting(CHAR* ps_mac);
extern INT32 a_mtkapi_bt_gap_stop(BOOL fg_keep_conn, BOOL fg_discard_data);
extern INT32 a_mtkapi_bt_gap_clear_dev_info(VOID);
extern INT32 a_mtkapi_bt_bluetooth_factory_reset(CHAR *addr);
extern INT32 a_mtkapi_bt_gap_del_paired_av_dev_one(BT_TARGET_DEV_INFO *pt_device_info);
extern INT32 a_mtkapi_bt_gap_del_paired_av_dev_all(VOID);
extern INT32 a_mtkapi_bt_gap_auto_disconnection(VOID);
extern INT32 a_mtkapi_bt_gap_save_device_history(VOID);
extern INT32 a_mtkapi_bt_gap_set_name(CHAR *name);
extern INT32 a_mtkapi_bt_gap_set_connectable_and_discoverable(BOOL fg_conn, BOOL fg_disc);
extern INT32 a_mtkapi_bt_gap_get_paired_device_info(VOID);
extern INT32 a_mtkapi_bt_gap_get_local_dev_info(BT_GAP_LOCAL_PROPERTIES_RESULT *ps_dev_info);
extern INT32 a_mtkapi_bt_gap_get_target_info(BT_TARGET_DEV_INFO *pt_target_dev_info);
extern INT32 a_mtkapi_bt_gap_inquiry_one(CHAR *ps_dev_mac, UINT32 ui4_inqDuration);
extern INT32 a_mtkapi_bt_gap_start_inquiry_scan(UINT32 ui4_filter_type, mtkrpcapi_BtAppGapInquiryResponseCbk pf_fct);
extern INT32 a_mtkapi_bt_gap_stop_inquiry_scan(VOID);
extern INT32 a_mtkapi_bt_gap_pair(CHAR *addr, int transport);
extern INT32 a_mtkapi_bt_gap_paired_dev_erase(CHAR *addr);
extern INT32 a_mtkapi_bt_gap_get_rssi(CHAR *address, INT16 *rssi_value);
extern INT32 a_mtkapi_bt_gap_set_virtual_sniffer(INT32 enable);
extern INT32 a_mtkapi_bt_gap_send_hci(CHAR *buffer);

extern INT32 c_rpc_reg_mtk_bt_service_gap_cb_hndlrs(VOID);

#ifdef  __cplusplus
}
#endif
#endif

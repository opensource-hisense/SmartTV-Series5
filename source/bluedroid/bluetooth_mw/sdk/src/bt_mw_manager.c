/*-----------------------------------------------------------------------------
                    include files
 ----------------------------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include "c_mw_config.h"
#include "bluetooth.h"

#include "bt_mw_common.h"
#include "bt_mw_gap.h"
#include "bt_mw_manager.h"

#include "bluetooth_sync.h"

#include "bt_mw_gatt.h"
#include "c_bt_mw_gatt.h"
#include "u_bt_mw_gatt.h"



#include "bt_mw_a2dp_common.h"
#include "bt_mw_avrcp.h"
#include "bt_mw_devices_info.h"
#include "bt_mw_spp.h"
#include "c_bt_mw_a2dp_common.h"
#if ENABLE_A2DP_SRC
#include "c_bt_mw_a2dp_src.h"
#include "bt_mw_a2dp_src.h"
#endif
#if ENABLE_A2DP_SINK
#include "c_bt_mw_a2dp_snk.h"
#include "bt_mw_a2dp_snk.h"
#endif
#include "bt_mw_msg.h"
#include "linuxbt_common.h"
#include "linuxbt_msg.h"
#include "bt_mw_hidh.h"
#if ENABLE_MTK_TV_PLATFORM
//#include "dbg/x_dbg.h"
#else
//#include "x_ipcd.h"
#endif


#define DISCONNECT_MAX_LOOP  10
#define PAIR_MAX_LOOP  10
#define GET_MEDIA_INFO_MAX_LOOP  6
#define BT_COMMON_MAX_LOOP  6
#define BT_CONNECTING_TIMEOUT  15

#define AVRCP_CHECK

/*GAP callback*/
//BtGapCbk      GapCbk;
/*APP callback*/
BtAppEventCbk      BtAppCbk = NULL;
BtAppAvrcpPlayStateChangeCbk AppPlayStateCbk = NULL;
BtAppAvrcpVolumeChangeCbk AppVolumeCbk = NULL;
BtAppAvrcpTrackChangeCbk AppTrackChangeCbk = NULL;
BtAppAvrcpMediaStatusCbk AppMediaStatusCbk = NULL;
BtAppAvrcpAbsoluteVolumeCbk AppAbsoluteVolumeCbk = NULL;
BtAppAvrcpCmdCbk AppAvrcpcmdCbk = NULL;
BtAppSppCbk      SppCbk = NULL;
extern  BtAppHidCbk      HidCbk;
BtAppGapInquiryResponseCbk AppInquiryCbk = NULL;
BtAppA2dpUploadInfoCbk AppUploadInfoCbk = NULL;
BtAppAvrcpGetPlayStatusCbk AppGetPlayStatusCbk = NULL;
BtAppA2dpPlaybackCtrlCbk AppPlaybackCtrlCbk = NULL;
BtAppA2dpResetAudioCbk AppResetAudioCbk = NULL;
BtAppGapGetPairingKeyCbk AppGetPairingKeyCbk = NULL;
BtAppHfclientCbk AppHfclientCbk = NULL;
BtAppBondCbk AppBondCbk = NULL;

extern UINT8 g_scms_t_value;
extern BT_TARGET_DEV_LIST *g_pt_paired_src_dev_history;
extern BT_TARGET_DEV_LIST g_paired_src_dev_history;
#if ENABLE_A2DP_SRC
extern BT_TARGET_DEV_LIST g_paired_sink_dev_history;
extern BT_TARGET_DEV_LIST *g_pt_paired_sink_dev_history;
#endif
extern BT_HID_TARGET_DEV_LIST g_paired_hid_dev_history;
extern BT_HID_TARGET_DEV_LIST *g_pt_paired_hid_dev_history;
extern BT_HID_STATUS_STRUCT_LIST g_hid_status_list;
extern BT_HID_STATUS_STRUCT_LIST *g_pt_hid_status_list;

static BT_APP_CB_FUNC g_func;

#if MTK_BLE_GGL_SETUP_SUPPORT
extern BOOL g_fg_createdService;
extern BOOL g_fg_startedService;
extern char started_service_uuid[8][64];
extern char created_service_uuid[8][64];
extern BOOL g_fg_dcoff;
extern BOOL g_fg_charIsExist;
extern BOOL g_fg_serviceIsExist;
#endif



//first time enter into bluetooth function
//static BOOL fg_bt_first_init = TRUE;
//if bluetooth have init
//static BOOL g_fg_bt_init = FALSE;
INT32 g_i4restart_counter = 0;
static BOOL g_fg_bt_base_init = FALSE;
static BOOL g_fg_bt_bluetooth_on = FALSE;
extern BT_GAP_LOCAL_PROPERTIES_RESULT g_localProperty;
//extern HANDLE_T  h_bt_playback_mutex;

extern INT32 bluetooth_a2dp_sink_player_init(VOID);
extern BT_PLAYER_SNK_CB_FUNC player_func;
extern BT_AUDIO_SRC_CB_FUNC* bt_a2dp_alsa_getUploader(VOID);
typedef INT32 (*BtA2dpOutputCbk)(const CHAR *data, INT32 len);

extern VOID bt_a2dp_alsa_regOutputCallback(BtA2dpOutputCbk output_cb);
extern BT_PLAYER_SNK_CB_FUNC* bt_a2dp_playback_getPb(VOID);


/*register APP callback function*/
INT32 bluetooth_mgr_register_cbk_fct(BT_APP_CB_FUNC *func)
{
    INT32 i4_ret = BT_ERR_STATUS_FAIL;

    BT_DBG_NORMAL(BT_DEBUG_COMM, "start c_bt_manager_register_cbk_fct");
    if (NULL == func)
    {
        BT_DBG_ERROR(BT_DEBUG_COMM, "callback func is null!");
        return BT_ERR_STATUS_NULL_POINTER;
    }
    if (func->bt_event_cb)
    {
        BtAppCbk = func->bt_event_cb;
    }
    else
    {
        BT_DBG_ERROR(BT_DEBUG_COMM, "event callback func is null!");
        //i4_ret = BT_ERR_STATUS_NULL_POINTER;
    }

    if (func->bt_play_change_cb)
    {
        AppPlayStateCbk = func->bt_play_change_cb;
    }
    else
    {
        BT_DBG_ERROR(BT_DEBUG_COMM, "playback change callback func is null!");
        //i4_ret = BT_ERR_STATUS_NULL_POINTER;
    }
    if (func->bt_volume_change_cb)
    {
        AppVolumeCbk = func->bt_volume_change_cb;
    }
    else
    {
        BT_DBG_ERROR(BT_DEBUG_COMM, "volume change callback func is null!");
        //i4_ret = BT_ERR_STATUS_NULL_POINTER;
    }
    if (func->bt_track_change_cb)
    {
        AppTrackChangeCbk = func->bt_track_change_cb;
    }
    else
    {
        BT_DBG_ERROR(BT_DEBUG_COMM, "track change callback func is null!");
        //i4_ret = BT_ERR_STATUS_NULL_POINTER;
    }

    if (func->bt_media_playstatus_cb)
    {
        AppMediaStatusCbk = func->bt_media_playstatus_cb;
    }
    else
    {
        BT_DBG_ERROR(BT_DEBUG_COMM, "media playstatus callback func is null!");
        //i4_ret = BT_ERR_STATUS_NULL_POINTER;
    }

    if (func->bt_spp_cb)
    {
        SppCbk = func->bt_spp_cb;
    }
    else
    {
        BT_DBG_ERROR(BT_DEBUG_COMM, "spp callback func is null!");
        //i4_ret = BT_ERR_STATUS_NULL_POINTER;
    }

    if (func->bt_hid_cb)
    {
        HidCbk = func->bt_hid_cb;
        BT_DBG_NORMAL(BT_DEBUG_COMM, "hid callback func register successful");
    }
    else
    {
        BT_DBG_ERROR(BT_DEBUG_COMM, "hid callback func is null!");
        //i4_ret = BT_ERR_STATUS_NULL_POINTER;
    }

    if (func->bt_absolute_volume_cb)
    {
        AppAbsoluteVolumeCbk = func->bt_absolute_volume_cb;
    }
    else
    {
        BT_DBG_ERROR(BT_DEBUG_COMM, "absolute volume callback func is null!");
        //i4_ret = BT_ERR_STATUS_NULL_POINTER;
    }

    if (func->bt_avrcp_cmd_cb)
    {
        AppAvrcpcmdCbk = func->bt_avrcp_cmd_cb;
    }
    else
    {
        BT_DBG_ERROR(BT_DEBUG_COMM, "avrcp cmd func is null!");
        //i4_ret = BT_ERR_STATUS_NULL_POINTER;
    }

    if (func->bt_upload_info_cb)
    {
        AppUploadInfoCbk = func->bt_upload_info_cb;
    }
    else
    {
        BT_DBG_ERROR(BT_DEBUG_COMM, "upload info func is null!");
        //i4_ret = BT_ERR_STATUS_NULL_POINTER;
    }
    if (func->bt_get_play_status_cb)
    {
        AppGetPlayStatusCbk = func->bt_get_play_status_cb;
    }
    else
    {
        BT_DBG_ERROR(BT_DEBUG_COMM, "get play status func is null!");
        //i4_ret = BT_ERR_STATUS_NULL_POINTER;
    }
    if (func->bt_playback_ctrl_cb)
    {
        AppPlaybackCtrlCbk = func->bt_playback_ctrl_cb;
    }
    else
    {
        BT_DBG_ERROR(BT_DEBUG_COMM, "control playback func is null!");
        //i4_ret = BT_ERR_STATUS_NULL_POINTER;
    }
    if (func->bt_reset_audio_cb)
    {
        AppResetAudioCbk = func->bt_reset_audio_cb;
    }
    else
    {
        BT_DBG_ERROR(BT_DEBUG_COMM, "reset audio func is null!");
        //i4_ret = BT_ERR_STATUS_NULL_POINTER;
    }
    if (func->bt_get_pairing_key_cb)
    {
        AppGetPairingKeyCbk = func->bt_get_pairing_key_cb;
    }
    else
    {
        BT_DBG_ERROR(BT_DEBUG_COMM, "get pairing key func is null!");
        //i4_ret = BT_ERR_STATUS_NULL_POINTER;
    }
    if (func->bt_hfclient_cb)
    {
        AppHfclientCbk = func->bt_hfclient_cb;
    }
    else
    {
        BT_DBG_ERROR(BT_DEBUG_COMM, "hfp client func is null!");
        //i4_ret = BT_ERR_STATUS_NULL_POINTER;
    }

    if (func->bt_bond_cb)
    {
        AppBondCbk = func->bt_bond_cb;
    }
    else
    {
        BT_DBG_ERROR(BT_DEBUG_COMM, "bond func is null!\n");
        //i4_ret = BT_ERR_STATUS_NULL_POINTER;
    }

    return i4_ret;
}

INT32 bluetooth_disconnect_both_sync(VOID)
{
    FUNC_ENTRY;
    INT32 i4_ret = 0;
    /*need disconnect AVRCP first*/
    i4_ret = bluetooth_avrcp_disconnect_sync();
    if (0 != i4_ret)
    {
        BT_DBG_ERROR(BT_DEBUG_A2DP,"can't disconnect AVRCP");
        return -1;
    }
    /*maybe after avrcp disconnection a2dp automatically disconnected*/
    if (get_bt_a2dp_connect())
    {
        i4_ret = bluetooth_a2dp_disconnect_sync();

        if (0 != i4_ret)
        {
            BT_DBG_ERROR(BT_DEBUG_A2DP,"can't disconnect a2dp");
            return -1;
        }
    }

    return 0;
}

INT32 bluetooth_mgr_av_disconnect_all(VOID)
{
    FUNC_ENTRY;
    INT32 i4_ret = BT_ERR_STATUS_FAIL;

    if (get_bt_avrcp_connect() && get_bt_a2dp_connect())
    {
        //i4_ret = bluetooth_disconnect_both_sync();
        i4_ret = bluetooth_a2dp_disconnect_sync();
    }
    else if (get_bt_avrcp_connect())
    {
        //i4_ret = bluetooth_avrcp_disconnect_sync();
        BT_DBG_ERROR(BT_DEBUG_COMM, "should not go to this case!");
    }
    else if (get_bt_a2dp_connect())
    {
        i4_ret = bluetooth_a2dp_disconnect_sync();
    }
    else
    {
        BT_DBG_NORMAL(BT_DEBUG_COMM, "there no av connection");
        i4_ret = BT_SUCCESS;
    }
    BT_CHECK_RESULT(BT_DEBUG_COMM, i4_ret);

    return BT_SUCCESS;
}

/* currently implementation no restart mechanism for share library solution */
INT32 bluetooth_mgr_restart_bluetooth(VOID)
{
    return BT_SUCCESS;
}

INT32 bluetooth_mgr_connect_fail(CHAR *bdAddr)
{
    FUNC_ENTRY;

    /*Reset launch connection flag*/
    /*If platform trigger conection, but connection fail. It should reset the flag*/
    BtAppCbk(BT_CONNECT_TIMEOUT, bdAddr);
    //BtAppCbk(BT_PAIRING_MODE);
    //set_bt_mode(BT_MODE_PAIRING);

    return BT_SUCCESS;
}

/*bluetooth init function, before run this API should run stack first*/
INT32 bluetooth_mgr_base_init(BT_APP_CB_FUNC *func)
{
    FUNC_ENTRY;

    /*fix after stack restart codec info reset issue*/

    BT_DBG_NORMAL(BT_DEBUG_COMM, "the g_fg_bt_base_init is:%d!", g_fg_bt_base_init);
    if (!g_fg_bt_base_init)
    {
        bluetooth_check_default_folder();
        mw_log_init();

#if ENABLE_A2DP_SRC
        /*only for test, official should not call this API at here*/
        //register_upload_func();
        bluetooth_a2dp_register_upload_cb(bt_a2dp_alsa_getUploader());
        bt_a2dp_alsa_regOutputCallback(c_btm_a2dp_send_stream_data);
#endif
#if ENABLE_A2DP_SINK
        /*only for test, official should not call this API at here*/
        bluetooth_a2dp_register_player_cb(bt_a2dp_playback_getPb());
#endif

        /* reset some variable */
        i4bluetooth_sync_var_init();
        bluetooth_gap_init();
        bluetooth_a2dp_sink_player_init();

        /* register app callback function to mw */
        memcpy(&g_func, func, sizeof(g_func));
        bluetooth_mgr_register_cbk_fct( func );

        /* mw message queue for handle connection info */
        bt_msg_queue_init();
        /* wrap layer message queue for handle connection info */
        linuxbt_msg_queue_init();

        /* default only init src device list */
#if ENABLE_A2DP_SRC
        g_pt_paired_sink_dev_history = &g_paired_sink_dev_history;
        bluetooth_getHistoryList(TRUE);//GET A2DP SINK history
#endif
#if ENABLE_A2DP_SINK
        g_pt_paired_src_dev_history = &g_paired_src_dev_history;
        bluetooth_getHistoryList(FALSE);//GET A2DP SRC history
#endif
#if ENABLE_HID_PROFILE_H
        BT_DBG_NORMAL(BT_DEBUG_COMM, "Ininiate HID device");
        g_pt_paired_hid_dev_history = &g_paired_hid_dev_history;
        g_pt_hid_status_list = &g_hid_status_list;
        bluetooth_getHIDHistoryList();//GET HID history
#endif
#if ENABLE_SAVE_LOCAL_DEV_TO_FILE
        INT32 i4_ret;
        if (bluetooth_local_dev_is_init())
        {
            /*call mtkbt API need wait BT power on*/
            BT_DBG_NORMAL(BT_DEBUG_COMM, "local dev info have init");
            if (0 < strlen(g_localProperty.name))
            {
                bluetooth_set_local_dev_name(g_localProperty.name);
            }
            else
            {
                bluetooth_set_local_dev_name(BLUETOOTH_ORIGINAL_NAME);
            }
        }
        else
        {
            BT_DBG_INFO(BT_DEBUG_COMM, "should init dev name");
            i4_ret = bluetooth_init_local_dev_info();
            if (BT_SUCCESS != i4_ret)
            {
                BT_DBG_ERROR(BT_DEBUG_COMM, "bluetooth_init_local_dev_info is failed.");
                return BT_ERR_STATUS_FAIL;
            }
        }
#endif/*ENABLE_SAVE_LOCAL_DEV_TO_FILE*/

        g_fg_bt_base_init = TRUE;
        BT_DBG_NORMAL(BT_DEBUG_COMM, "after init the g_fg_bt_base_init is:%d!", g_fg_bt_base_init);
    }
    return BT_SUCCESS;
}


INT32 bluetooth_mgr_bluetooth_on_off(BOOL fg_on)
{
    FUNC_ENTRY;
    INT32 i4_ret = BT_ERR_STATUS_FAIL;
    INT32 i = 0;
    g_fg_bt_bluetooth_on = fg_on;
    BT_DBG_NORMAL(BT_DEBUG_COMM, "%s",fg_on?"on":"off");
    if (fg_on)
    {
        /* set bt enable */
        i4_ret = bluetooth_set_power_sync(TRUE);

#if ENABLE_SAVE_LOCAL_DEV_TO_FILE
        if (bluetooth_local_dev_is_init())
        {
            /* in case of after factory reset no device info */
            if ((strlen(g_localProperty.bdAddr)<17)||(0 == strlen(g_localProperty.name)))
            {
                BT_DBG_NORMAL(BT_DEBUG_COMM, "init device info after reset.");
                bluetooth_update_local_dev_info();
                strncpy(g_localProperty.name, BLUETOOTH_ORIGINAL_NAME, sizeof(BLUETOOTH_ORIGINAL_NAME));
            }
            /*BT power on can call stack API */
            //bluetooth_set_local_dev_name(g_localProperty.name);
        }
        else
        {
            BT_DBG_INFO(BT_DEBUG_COMM, "should init dev name");
            i4_ret = bluetooth_init_local_dev_info();
            if (BT_SUCCESS != i4_ret)
            {
                BT_DBG_ERROR(BT_DEBUG_COMM, "bluetooth_init_local_dev_info is failed.");
                return BT_ERR_STATUS_FAIL;
            }
            /*BT power on can call stack API */
            //bluetooth_set_local_dev_name(g_localProperty.name);
        }
#endif/* ENABLE_SAVE_LOCAL_DEV_TO_FILE */
        /*BT power on can call stack API */
        bluetooth_set_local_dev_name(g_localProperty.name);

        /*avoid codec type have not set successful before bt power on*/
        switch (get_bt_enabled_a2dp_role())
        {
        case 1:
            c_btm_source_sink_on_off(FALSE, TRUE);
            break;
        case 2:
            c_btm_source_sink_on_off(TRUE, FALSE);
            break;
        case 3:
            c_btm_source_sink_on_off(TRUE, TRUE);
            break;
        default:
            BT_DBG_NORMAL(BT_DEBUG_COMM, "default only enable sink");
            c_btm_source_sink_on_off(FALSE, TRUE);
            break;
        }
#if MTK_BLE_GGL_SETUP_SUPPORT
        if (g_fg_bt_base_init && g_fg_dcoff)
        {
            if (g_fg_createdService)
            {
                g_fg_serviceIsExist = FALSE;
                for (i = 0; i < 8; i++)
                {
                    if (created_service_uuid[i][0] != '\0')
                    {
                        c_btm_gatt_createService(created_service_uuid[i]);
                        BT_DBG_NORMAL(BT_DEBUG_COMM, "dc_on createservice uuid:%s!", created_service_uuid[i]);
                    }
                }
            }
            if (g_fg_startedService)
            {
                g_fg_charIsExist = FALSE;
                for (i = 0; i < 8; i++)
                {
                    if (started_service_uuid[i][0] != '\0')
                    {
                        c_btm_gatt_startService(started_service_uuid[i]);
                        BT_DBG_NORMAL(BT_DEBUG_COMM, "dc_on startservice uuid:%s!", started_service_uuid[i]);
                    }
                }
            }
        }
        g_fg_dcoff = FALSE;
#endif
    }
    else
    {
#if MTK_BLE_GGL_SETUP_SUPPORT
        /* set bt disable */
        g_fg_dcoff = TRUE;
#endif
#if ENABLE_ALWAYS_CONNECTABLE_MODE
        i4_ret = bluetooth_set_scanmode_sync(FALSE, FALSE);
#endif
        /*error happened, need restart stack, and reset resource*/
        if (BT_SUCCESS != i4_ret)
        {
            BT_DBG_ERROR(BT_DEBUG_COMM, "bluetooth_set_scanmode_sync failed!");
            return BT_ERR_STATUS_FAIL;
        }

        /*in case of before power off BDV don't do disconnection*/
        if (get_bt_avrcp_connect() && get_bt_a2dp_connect())
        {
            //i4_ret = bluetooth_disconnect_both_sync();
            i4_ret = bluetooth_a2dp_disconnect_sync();
        }
        if (get_bt_avrcp_connect())
        {
            //i4_ret = bluetooth_avrcp_disconnect_sync();
        }
        if (get_bt_a2dp_connect())
        {
            i4_ret = bluetooth_a2dp_disconnect_sync();
        }
        i4_ret = bluetooth_hid_disconnect_sync();
        i4_ret = bluetooth_set_power_sync(FALSE);
        /*profile deinit maybe should be here*/
        bluetooth_gap_deinit();
        /*x_thread_delay(500);*/
        //usleep(500*1000);

        /*error happened, need restart stack, and reset resource*/
        if (BT_SUCCESS != i4_ret)
        {
            BT_DBG_ERROR(BT_DEBUG_COMM, "bluetooth_set_power_sync failed!");
        }
        else
        {
            g_i4restart_counter = 0;
        }
    }

    BT_DBG_NORMAL(BT_DEBUG_COMM, "the g_fg_bt_bluetooth_on is:%d!", g_fg_bt_bluetooth_on);
    return BT_SUCCESS;
}

/*for MW CLI debug --uninit bluetooth function*/
EXPORT_SYMBOL INT32 bluetooth_mgr_btuninit(VOID)
{
    FUNC_ENTRY;

    INT32 i4_ret = BT_ERR_STATUS_FAIL;

    BT_DBG_NORMAL(BT_DEBUG_COMM, "the g_fg_bt_base_init is:%d!", g_fg_bt_base_init);
    if (g_fg_bt_base_init)
    {
        //from off mode to pairing mode no need power off
        if (BT_MODE_OFF == get_bt_mode())
        {
            if (g_fg_bt_bluetooth_on)
            {
                i4_ret = bluetooth_mgr_bluetooth_on_off(TRUE);
            }
        }
        else
        {
            //from pairing mode to exit
            if (BT_MODE_PAIRING == get_bt_mode())
            {
                /*in case of mode overlap*/
                i4_ret = bluetooth_mgr_av_disconnect_all();
            }
            else if (BT_MODE_PLAYING == get_bt_mode())
            {
                i4_ret = bluetooth_mgr_av_disconnect_all();
            }
            //from connecting mode to pairing mode
            else if (BT_MODE_CONNECTING == get_bt_mode())
            {
            }
            /*error happened, need restart stack, and reset resource*/
            if (BT_SUCCESS != i4_ret)
            {
                BT_DBG_ERROR(BT_DEBUG_COMM, "bluetooth_mgr_av_disconnect_all failed!");
            }
            else
            {
                i4_ret = bluetooth_mgr_bluetooth_on_off(FALSE);
            }

        }

        set_bt_mode(BT_MODE_CLOSE);
        BT_CHECK_RESULT(BT_DEBUG_COMM, i4_ret);
        BtAppCbk(BT_EXIT, NULL);


        g_fg_bt_base_init = FALSE;

        BT_DBG_NORMAL(BT_DEBUG_COMM, "after stop the g_fg_bt_base_init is:%d!", g_fg_bt_base_init);

    }
    return BT_SUCCESS;
}

INT32 bluetooth_mgr_pairing(VOID)
{
    FUNC_ENTRY;

    INT32 i4_ret = BT_ERR_STATUS_FAIL;

    if (get_bt_avrcp_connect() || get_bt_a2dp_connect())
    {
        /*fast reconnection have been established, return*/
        BT_DBG_NORMAL(BT_DEBUG_COMM,"A2DP or AVRCP connection established,return");
        return BT_SUCCESS;
    }

    set_bt_avrcp_support(FALSE);
    set_bt_a2dp_support(FALSE);
    bt_audio_set_stream_suspend(TRUE);
    bluetooth_reset_playstatus();
    set_bt_2nd_connect_comming(FALSE);
    //bluetooth_get_local_dev_info(&ps_dev_info);

    /* set bt connectable&discoverable on */
    if (get_bt_avrcp_connect() || get_bt_a2dp_connect())
    {
        /*fast reconnection have been established, return*/
        BT_DBG_NORMAL(BT_DEBUG_COMM,"A2DP or AVRCP connection established,return");
        return BT_SUCCESS;
    }
    i4_ret = bluetooth_set_scanmode_sync(TRUE, TRUE);
    if (BT_SUCCESS != i4_ret)
    {
        BT_DBG_ERROR(BT_DEBUG_COMM, "bluetooth_set_scanmode_sync failed!");
    }

    if (get_bt_avrcp_connect() || get_bt_a2dp_connect())
    {
        /*fast reconnection have been established, return*/
        BT_DBG_NORMAL(BT_DEBUG_COMM,"A2DP or AVRCP connection established,return");
        BT_DBG_INFO(BT_DEBUG_COMM,"A2DP:%s, AVRCP:%s",
                      get_bt_a2dp_connect()?"up":"down",
                      get_bt_avrcp_connect()?"up":"down");
        return BT_SUCCESS;
    }
    set_bt_mode(BT_MODE_PAIRING);

    return BT_SUCCESS;
}

INT32 bluetooth_mgr_connecting(CHAR* ps_mac)
{
    FUNC_ENTRY;
    //bluetooth_set_connectable(FALSE);

    set_bt_mode(BT_MODE_CONNECTING);
    BtAppCbk(BT_CONNECT_MODE, NULL);
    /*if connection is triggered by platform, it should trigger AVRCP*/
    c_btm_connect_audio_sources(ps_mac, CONN_TYPE_A2DP);

    return BT_SUCCESS;

}


INT32 bluetooth_mgr_playing(VOID)
{
    FUNC_ENTRY;
#if ENABLE_A2DP_SINK
    //bluetooth_a2dp_playback_init_ext();
    bluetooth_a2dp_playback_play();
#endif

    return BT_SUCCESS;
}

INT32 bluetooth_mgr_off(VOID)
{
    FUNC_ENTRY;
    INT32 i4_ret;

    set_bt_mode(BT_MODE_OFF);

    i4_ret = bluetooth_set_power_sync(FALSE);
    if (0 != i4_ret)
    {
        BT_DBG_ERROR(BT_DEBUG_COMM, "bluetooth_set_power_sync failed!");
    }

    return BT_SUCCESS;
}

INT32 bluetooth_mgr_pairing_mode(UINT32 u4_overtime)
{
    INT32 i4_ret = 0;

    BT_DBG_NORMAL(BT_DEBUG_COMM, "enter pairing mode!");
    BT_DBG_NORMAL(BT_DEBUG_COMM, "the last g_bt_mode:%d", get_bt_mode());

    if (get_bt_a2dp_be_sink())
    {
        /*SINK mode*/
        //from playing mode to pairing mode
        if (BT_MODE_PLAYING == get_bt_mode())
        {
            //maybe have disconnect done
            i4_ret = bluetooth_mgr_av_disconnect_all();
            if (BT_SUCCESS != i4_ret)
            {
                BT_DBG_ERROR(BT_DEBUG_COMM, "bluetooth_mgr_av_disconnect_all failed!");
            }
        }
        else if (BT_MODE_OFF == get_bt_mode())
        {
            //from off mode to pairing mode
            /* set bt enable */
            i4_ret = bluetooth_set_power_sync(TRUE);
            /*error happened, need restart stack, and reset resource*/
            if (BT_SUCCESS != i4_ret)
            {
                BT_DBG_ERROR(BT_DEBUG_COMM, "bluetooth_set_power_sync failed!");
            }
            else
            {
                bluetooth_set_local_dev_name(g_localProperty.name);
            }
        }
        else if (BT_MODE_CONNECTING == get_bt_mode())
        {
            //from connecting mode to pairing mode
        }
        else if (BT_MODE_CLOSE == get_bt_mode())
        {
            //from exit mode to pairing mode
            //x_thread_delay(2000);
        }

        /*avoid fast reconnection after disconnection*/
        if (get_bt_avrcp_connect() || get_bt_a2dp_connect())
        {
            BT_DBG_NORMAL(BT_DEBUG_COMM, "no need enter into WAITING mode");
            return BT_SUCCESS;
        }
        i4_ret = bluetooth_mgr_pairing();
        BT_CHECK_RESULT(BT_DEBUG_COMM, i4_ret);
    }
    else
    {
        /*SRC mode*/
        BT_DBG_NORMAL(BT_DEBUG_COMM, "enter pairing mode!");
        BT_DBG_NORMAL(BT_DEBUG_COMM, "the last bt_mode:%d", get_bt_mode());

        //bluetooth_set_pairable_sync(TRUE);
        bluetooth_set_scanmode_sync(TRUE, TRUE);

        set_bt_mode(BT_MODE_PAIRING);
    }
    return BT_SUCCESS;
}

INT32 bluetooth_mgr_connecting_mode(UINT32 u4_overtime)
{
    INT32 i4_ret = 0;
    BT_DBG_NORMAL(BT_DEBUG_COMM, "CONNECTING mode!");
    if (get_bt_a2dp_be_sink())
    {
        /*SINK mode*/
        //from PAIRING mode to CONNECTING mode
        if (BT_MODE_PAIRING == get_bt_mode())
        {
            i4_ret = bluetooth_set_scanmode_sync(FALSE,FALSE);
        }
        else if (BT_MODE_PLAYING == get_bt_mode())
        {
            BT_DBG_ERROR(BT_DEBUG_COMM, "shouldn't go here");
            return BT_ERR_STATUS_FAIL;
        }
        else if (BT_MODE_OFF == get_bt_mode())
        {
            BT_DBG_ERROR(BT_DEBUG_COMM, "OFF-->CONNECT");
            if (get_bt_power_on())
            {
                BT_DBG_NORMAL(BT_DEBUG_COMM, "BT power is on");
            }
            else
            {
                /* set bt enable */
                i4_ret = bluetooth_set_power_sync(TRUE);
                if (BT_SUCCESS != i4_ret)
                {
                    BT_DBG_ERROR(BT_DEBUG_COMM, "bluetooth_set_power_sync failed!");
                }
            }
        }
        else if (BT_MODE_CLOSE == get_bt_mode())
        {
            BT_DBG_ERROR(BT_DEBUG_COMM, "CLOSE-->CONNECT");
            if (get_bt_power_on())
            {
                BT_DBG_NORMAL(BT_DEBUG_COMM, "BT power is on");
            }
            else
            {
                /* set bt enable */
                i4_ret = bluetooth_set_power_sync(TRUE);
                if (BT_SUCCESS != i4_ret)
                {
                    BT_DBG_ERROR(BT_DEBUG_COMM, "bluetooth_set_power_sync failed!");
                }
            }
        }
        BtAppCbk(BT_CONNECT_MODE, NULL);
    }
    else
    {
        /*SRC mode*/
        BT_DBG_NORMAL(BT_DEBUG_COMM, "CONNECTING mode!");

        set_bt_mode(BT_MODE_CONNECTING);
        BtAppCbk(BT_CONNECT_MODE, NULL);
    }
    return BT_SUCCESS;
}

INT32 bluetooth_mgr_playing_mode(VOID)
{
    BT_DBG_NORMAL(BT_DEBUG_COMM, "into playing mode!");
    if (get_bt_a2dp_be_sink())
    {
        /*SINK mode*/
        set_bt_discard_data(FALSE);
        if (get_bt_a2dp_connect())
        {
#if ENABLE_A2DP_SINK
            bluetooth_mgr_playing();
#endif
            set_bt_mode(BT_MODE_PLAYING);
            BtAppCbk(BT_PLAYING_MODE, NULL);
        }
        else
        {
            BT_DBG_ERROR(BT_DEBUG_COMM, "a2dp disconnected no need into playing mode");
        }
    }
    else
    {
        /*SRC mode*/
        if (get_bt_a2dp_connect())
        {
            set_bt_mode(BT_MODE_PLAYING);
            BtAppCbk(BT_PLAYING_MODE, NULL);
        }
        else
        {
            BT_DBG_ERROR(BT_DEBUG_COMM, "a2dp disconnected no need into playing mode");
            return BT_SUCCESS;
        }
    }
    return BT_SUCCESS;
}

INT32 bluetooth_mgr_off_mode(VOID)
{
    INT32 i4_ret = 0;

    BT_DBG_NORMAL(BT_DEBUG_COMM, "enter off mode!");
    if (BT_MODE_PLAYING == get_bt_mode())
    {
#if defined(BTMTK_ON_V301)
        BT_DBG_NORMAL(BT_DEBUG_COMM, "only for v301!");
        //from playing mode to off mode
        if (BT_MODE_PLAYING == get_bt_mode())
        {
            //maybe have disconnect done
            i4_ret = bluetooth_mgr_av_disconnect_all();
        }
        /*avoid fast reconnection after disconnection*/
        if (get_bt_avrcp_connect() || get_bt_a2dp_connect())
        {
            BT_DBG_NORMAL(BT_DEBUG_COMM, "no need enter into off mode");
            return BT_SUCCESS;
        }
#else
        BT_DBG_NORMAL(BT_DEBUG_COMM, "should not go here!");
        return BT_SUCCESS;
#endif
    }
    else if (BT_MODE_CONNECTING == get_bt_mode())
    {
        BT_DBG_NORMAL(BT_DEBUG_COMM, "from connecting mode--> off mode");
    }
    else if (BT_MODE_PAIRING == get_bt_mode())
    {
        //from pairing mode to off mode
        BT_DBG_NORMAL(BT_DEBUG_COMM, "from pairing mode--> off mode");
    }
    else
    {
        BT_DBG_ERROR(BT_DEBUG_COMM, "unexpected mode ");
        return BT_ERR_STATUS_FAIL;
    }
    i4_ret = bluetooth_mgr_off();
    BT_CHECK_RESULT(BT_DEBUG_COMM, i4_ret);

    return BT_SUCCESS;
}

INT32 bluetooth_mgr_btstart(const CHAR *ps_mode, UINT32 u4_overtime)
{
    FUNC_ENTRY;

    BT_DBG_NORMAL(BT_DEBUG_COMM, "start g_bt_mode:%d", get_bt_mode());

    BT_DBG_INFO(BT_DEBUG_COMM, "I'm %s", !get_bt_a2dp_be_sink()?"SRC":"SINK");
    if (0 == strcmp(PAIRING, ps_mode))
    {
        //enter into PAIRING mode
        bluetooth_mgr_pairing_mode(u4_overtime);
    }
    else if (0 == strcmp(CONNECTING, ps_mode))
    {
        //enter into CONNECTING mode
        bluetooth_mgr_connecting_mode(u4_overtime);
    }
    else if (0 == strcmp(PLAYING, ps_mode))
    {
        //enter into PLAYING mode
        bluetooth_mgr_playing_mode();
    }
    else if (0 == strcmp(OFF, ps_mode))
    {
        //enter into OFF mode
        bluetooth_mgr_off_mode();
    }
    else
    {
        BT_DBG_ERROR(BT_DEBUG_COMM, "invalid parameter!");
        return BT_ERR_STATUS_PARM_INVALID;
    }
    BT_DBG_NORMAL(BT_DEBUG_COMM, "after g_bt_mode:%d", get_bt_mode());
    return BT_SUCCESS;
}

INT32 bluetooth_mgr_stop(BOOL fg_keep_conn, BOOL fg_discard_data)
{
    FUNC_ENTRY;

    INT32 i4_ret = BT_ERR_STATUS_FAIL;

    set_bt_keep_connection(fg_keep_conn);
    set_bt_discard_data(fg_discard_data);

    BT_DBG_INFO(BT_DEBUG_COMM, "I'm %s", !get_bt_a2dp_be_sink()?"SRC":"SINK");
    /*sink mode as before*/
    if (get_bt_a2dp_be_sink())
    {
        BT_DBG_NORMAL(BT_DEBUG_COMM, "the g_fg_bt_base_init is:%d!", g_fg_bt_base_init);
        if (g_fg_bt_base_init)
        {
            if (BT_MODE_OFF == get_bt_mode())
            {
                //should not goto this state?
                if (g_fg_bt_bluetooth_on)
                {
                    i4_ret = bluetooth_mgr_bluetooth_on_off(TRUE);
                }
            }
            else
            {
                if (BT_MODE_PAIRING == get_bt_mode())
                {
                    //from pairing mode to exit
                    BT_DBG_NORMAL(BT_DEBUG_COMM, "from PAIRING mode to exit mode!");
                    /*in case of mode overlap*/
                    i4_ret = bluetooth_mgr_av_disconnect_all();
                }
                else if (BT_MODE_PLAYING == get_bt_mode())
                {
                    BT_DBG_NORMAL(BT_DEBUG_COMM, "from PLAYING mode to exit mode!");
                    if (!get_bt_keep_connection())
                    {
                        i4_ret = bluetooth_mgr_av_disconnect_all();
                        /*x_thread_delay(500);*/
                        usleep(500*1000);
                    }
                    else
                    {
                        BT_DBG_NORMAL(BT_DEBUG_COMM, "keep connection!");
#if ENABLE_A2DP_SINK
                        bluetooth_a2dp_playback_deinit();
#endif
                    }
                }
                else if (BT_MODE_CONNECTING == get_bt_mode())
                {
                    //from connecting mode to exit mode
                    BT_DBG_NORMAL(BT_DEBUG_COMM, "from CONNECTING mode to exit mode!");
                }
                else if (BT_MODE_CLOSE == get_bt_mode() && !fg_keep_conn)
                {
                    /*from CLOSE mode to CLOSE mode*/
                    BT_DBG_NORMAL(BT_DEBUG_COMM, "from CLOSE mode to CLOSE mode!");
                    i4_ret = bluetooth_mgr_av_disconnect_all();
                }
                /*error happened, need restart stack, and reset resource*/
                if (BT_SUCCESS != i4_ret)
                {
                    BT_DBG_ERROR(BT_DEBUG_COMM, "bluetooth_mgr_av_disconnect_all failed!");
                    /* set bt discoverable&connectable off */
                    i4_ret = bluetooth_set_scanmode_sync(FALSE, FALSE);
                }
                else
                {
#if ENABLE_SYMPHONY
                    /* set bt discoverableoff & connectable on */
                    i4_ret = bluetooth_set_scanmode_sync(TRUE, FALSE);
#else
                    /* set bt discoverable&connectable off */
                    i4_ret = bluetooth_set_scanmode_sync(FALSE, FALSE);
#endif
                }
                if (BT_SUCCESS != i4_ret)
                {
                    BT_DBG_ERROR(BT_DEBUG_COMM, "bluetooth_set_scanmode_sync failed!");
                    /* set bt discoverable&connectable off */
                    i4_ret = bluetooth_set_scanmode_sync(FALSE, FALSE);
                }
            }

            set_bt_mode(BT_MODE_CLOSE);
            BtAppCbk(BT_EXIT, NULL);

        }
        else
        {
            BT_DBG_NORMAL(BT_DEBUG_COMM, "bluetooth no init yet");
        }
    }
#if ENABLE_A2DP_SRC
    else
    {
        /*src mode*/
        BT_DBG_NORMAL(BT_DEBUG_COMM, "A2DP SRC mode no need handle it");
    }
#endif

    return BT_SUCCESS;
}

INT32 bluetooth_mgr_av_auto_disconnection(VOID)
{
    FUNC_ENTRY;
    INT32 i4_ret = BT_ERR_STATUS_FAIL;

    /*avoid frequently press TX/RX, every connection, it should only response a TX/RX*/
    static BOOL fg_bt_send_request_successful_disconnect_status = TRUE;
    if (!fg_bt_send_request_successful_disconnect_status)
    {
        BT_DBG_NORMAL(BT_DEBUG_COMM, "Has been sent disconnect request yet, please wait for disconnection");
        return BT_SUCCESS;
    }
    fg_bt_send_request_successful_disconnect_status = FALSE;
    /*Avoid disconnection from opposite device, platform and opposite device trigger AVRCP disconnection simultaneously*/
    i4_ret = bluetooth_mgr_av_disconnect_all();
    if (BT_SUCCESS == i4_ret)
    {
        g_i4restart_counter = 0;
    }
    else
    {
        if (g_i4restart_counter < 3)
        {
            BT_DBG_ERROR(BT_DEBUG_COMM, "g_i4restart_counter < 3!");
        }
        else
        {
            BT_DBG_ERROR(BT_DEBUG_COMM, "restart bluetooth 3 times and all failed:%ld", (long)i4_ret);
            i4_ret = BT_ERR_STATUS_FAIL;
        }
    }
    set_bt_avrcp_support(FALSE);
    set_bt_connect(FALSE);
    BtAppCbk(BT_DISCONNECTED, bt_last_device_addr);
    fg_bt_send_request_successful_disconnect_status = TRUE;
    return i4_ret;
}

INT32 bluetooth_mgr_get_num_in_pairlist(UINT32 *u4_dev_num)
{
    FUNC_ENTRY;

    if (!get_bt_a2dp_be_sink())
    {
        BT_DBG_NORMAL(BT_DEBUG_COMM, "get paired sink device number");
#if ENABLE_A2DP_SRC
        return bluetooth_get_num_of_history(u4_dev_num, g_pt_paired_sink_dev_history);
#endif
    }
    else
    {
        BT_DBG_NORMAL(BT_DEBUG_COMM, "get paired src device number");
#if ENABLE_A2DP_SINK
        return bluetooth_get_num_of_history(u4_dev_num, g_pt_paired_src_dev_history);
#endif
    }
    BT_DBG_ERROR(BT_DEBUG_COMM, "should not go here.");
    return BT_SUCCESS;
}


INT32 bluetooth_mgr_clear_local_dev_info(VOID)
{
    FUNC_ENTRY;
#if ENABLE_SAVE_LOCAL_DEV_TO_FILE
    INT32 i4_ret;

    memset(&g_localProperty, 0, sizeof(g_localProperty));
    i4_ret = bluetooth_WriteLocalInfoToFile(NULL);
    if (BT_SUCCESS != i4_ret)
    {
        BT_DBG_ERROR(BT_DEBUG_COMM, "cannot clear the device info");
        return BT_ERR_STATUS_FAIL;
    }
#else
    if (0 != access(BLUETOOTH_LOCAL_INFO_FILE, F_OK))
    {
        BT_DBG_ERROR(BT_DEBUG_COMM, "file:%s not exist!", BLUETOOTH_LOCAL_INFO_FILE);
    }
    else
    {
        remove(BLUETOOTH_LOCAL_INFO_FILE);
    }
#endif
    return BT_SUCCESS;
}

INT32 bluetooth_mgr_factory_reset(CHAR *addr)
{
    FUNC_ENTRY;

    if(NULL != addr)   //factory reset except RC
    {
        BT_DBG_NORMAL(BT_DEBUG_COMM, "[GAP] RC address:%s\n", addr);


        bluetooth_remove_av_history(g_pt_paired_sink_dev_history);
        bluetooth_remove_av_history(g_pt_paired_src_dev_history);

        bluetooth_remove_av_history_all();

        bluetooth_hid_removeHistory_except_rc(addr, g_pt_paired_hid_dev_history);
    }
    else
    {
        system(BLUETOOTH_RM_BT_CONFIG_CMD);
        system(BLUETOOTH_RM_BT_SNOOP_HCI_CMD);
        system(BLUETOOTH_RM_BT_STACK_LOG_CMD);
        system(BLUETOOTH_RM_BAK_FILE_CMD);
        system(BLUETOOTH_RM_CACHE_FILE_CMD);
    }

    BT_DBG_NORMAL(BT_DEBUG_COMM, "rm file done!");
    return BT_SUCCESS;
}


/* remove one item from paired device histroy, not do unpair operation*/
INT32 bluetooth_mgr_del_paired_av_dev_one(BT_TARGET_DEV_INFO *pt_device_info)
{
    FUNC_ENTRY;
    INT32 i4_ret = BT_ERR_STATUS_FAIL;

    BT_CHECK_POINTER(BT_DEBUG_COMM, pt_device_info);
    /*unpair*/
    bluetooth_remove_paired_dev(pt_device_info->bdAddr);
    if (!get_bt_a2dp_be_sink())
    {
        BT_DBG_NORMAL(BT_DEBUG_COMM, "delete device:%s from sink device list", pt_device_info->bdAddr);
#if ENABLE_A2DP_SRC
        /*remove mw maintained device info*/
        i4_ret = bluetooth_remove_av_history_one(pt_device_info->bdAddr, g_pt_paired_sink_dev_history);
#endif
    }
    else
    {
        BT_DBG_NORMAL(BT_DEBUG_COMM, "delete device:%s from src device list", pt_device_info->bdAddr);
#if ENABLE_A2DP_SINK
        /*remove mw maintained device info*/
        i4_ret = bluetooth_remove_av_history_one(pt_device_info->bdAddr, g_pt_paired_src_dev_history);
#endif
    }

    if (BT_SUCCESS != i4_ret)
    {
        BT_DBG_ERROR(BT_DEBUG_COMM, "cannot delete the device:%s", pt_device_info->bdAddr);
        return BT_ERR_STATUS_FAIL;
    }

    return BT_SUCCESS;
}


INT32 bluetooth_mgr_del_paired_av_dev_all(VOID)
{
    FUNC_ENTRY;
    INT32 i4_ret;

    /*remove mw maintained device info*/
    i4_ret = bluetooth_remove_av_history_all();
    BT_CHECK_RESULT(BT_DEBUG_COMM, i4_ret);

    return BT_SUCCESS;
}

INT32 bluetooth_mgr_save_device_history(VOID)
{
    INT32 i4_ret;
#if ENABLE_SAVE_LOCAL_DEV_TO_FILE
    if (17 == strlen(g_localProperty.bdAddr) && (0 < strlen(g_localProperty.name)))
    {
        i4_ret = bluetooth_WriteLocalInfoToFile(&g_localProperty);
    }
    else
    {
        memset(&g_localProperty, 0x0, sizeof(g_localProperty));
        i4_ret = bluetooth_WriteLocalInfoToFile(NULL);
    }
    if ( i4_ret != BT_SUCCESS )
    {
        BT_DBG_NORMAL(BT_DEBUG_COMM, "save local device info failed. " );
        return BT_ERR_STATUS_FAIL;
    }
#else
    bluetooth_mgr_clear_local_dev_info();
#endif/*ENABLE_SAVE_LOCAL_DEV_TO_FILE*/

#if ENABLE_A2DP_SRC
    if (NULL != g_pt_paired_sink_dev_history)
    {
        i4_ret = bluetooth_WriteHistoryToFile(g_pt_paired_sink_dev_history, BLUETOOTH_SINK_HISTORY_FILE);
        if ( i4_ret != BT_SUCCESS )
        {
            BT_DBG_NORMAL(BT_DEBUG_COMM, "save device history failed. " );
            return BT_ERR_STATUS_FAIL;
        }
        BT_DBG_NORMAL(BT_DEBUG_COMM, "save sink device history sucess. " );
    }
#endif/* ENABLE_A2DP_SRC */

#if ENABLE_A2DP_SINK
    if (NULL != g_pt_paired_src_dev_history)
    {
        i4_ret = bluetooth_WriteHistoryToFile(g_pt_paired_src_dev_history, BLUETOOTH_SRC_HISTORY_FILE);
        if ( i4_ret != BT_SUCCESS )
        {
            BT_DBG_NORMAL(BT_DEBUG_COMM, "save device history failed. " );
            return BT_ERR_STATUS_FAIL;
        }
        BT_DBG_NORMAL(BT_DEBUG_COMM, "save src device history sucess. " );
    }
#endif /* ENABLE_A2DP_SINK */

#if ENABLE_HID_PROFILE_H
    if (NULL != g_pt_paired_hid_dev_history)
    {
        i4_ret = bluetooth_hid_WriteHistoryToFile(g_pt_paired_hid_dev_history, BLUETOOTH_HID_HISTORY_FILE);
        if ( i4_ret != BT_SUCCESS )
        {
            BT_DBG_NORMAL(BT_DEBUG_COMM, "save HID device history failed. " );
            return BT_ERR_STATUS_FAIL;
        }
        BT_DBG_NORMAL(BT_DEBUG_COMM, "save HID device history sucess. " );
    }
#endif /* ENABLE_HID_PROFILE_H */
    return BT_SUCCESS;
} /*c_btm_save_device_history*/

INT32 bluetooth_mgr_set_external_log_lvl(INT32 module, INT32 log_level)
{
    FUNC_ENTRY;
    return bluetooth_set_external_log_level(module, log_level);
}

INT32 bluetooth_mgr_Enable_external_log(INT32 log_switch)
{
    FUNC_ENTRY;
    return bluetooth_enable_external_log(log_switch);
}


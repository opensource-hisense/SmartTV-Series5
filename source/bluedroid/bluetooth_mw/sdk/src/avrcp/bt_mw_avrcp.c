#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <assert.h>
#include <fcntl.h>
#include <sys/prctl.h>
#include <sys/select.h>
#include <sys/time.h>

//#include "bt_mmi.h"
#include "bluetooth.h"
#include "bt_mw_common.h"
#include "bluetooth_sync.h"
//#include "bluetooth_avrcp_struct.h"
#include "c_bt_mw_manager.h"
#include "bt_mw_avrcp.h"
#include "bt_mw_msg.h"

#include "linuxbt_avrcp_if.h"
#include "linuxbt_avrcp_tg_if.h"
#include "bt_mw_avrcp_misc.h"
#include "bt_mw_devices_info.h"
#if defined(MTK_LINUX_AVRCP_PLUS) && (MTK_LINUX_AVRCP_PLUS == TRUE)
#include "mtk_bt_rc.h"
#else
#include "bt_rc.h"
#endif

UINT32 g_ui4_old_track_index;
static BOOL fg_launch_tc_from_ps = FALSE;
static BOOL fg_get_metadata_after_launch_tc = FALSE;

#if ENABLE_A2DP_SINK

//static sys_enable_t abortrsp_flag = FALSE;
extern BT_TARGET_DEV_LIST *g_pt_paired_src_dev_history;
#endif

#if ENABLE_A2DP_SRC
extern BT_TARGET_DEV_LIST g_paired_sink_dev_history;
extern BT_TARGET_DEV_LIST *g_pt_paired_sink_dev_history;
#endif

extern char bt_last_device_addr[18];
extern BT_TARGET_DEV_INFO g_bt_target_dev_info;

extern BtAppEventCbk      BtAppCbk;
extern BtAppAvrcpAbsoluteVolumeCbk AppAbsoluteVolumeCbk;
extern BtAppAvrcpPlayStateChangeCbk AppPlayStateCbk;
extern BtAppAvrcpTrackChangeCbk AppTrackChangeCbk;
extern BtAppAvrcpMediaStatusCbk AppMediaStatusCbk;
extern BT_PLAYER_SNK_CB_FUNC player_func;

UINT8 g_trackstatus = 0;
UINT8 g_absolute_volume = 50;


INT32 bluetooth_avrcp_init(VOID)
{
    FUNC_ENTRY;
    return linuxbt_rc_init();
}

INT32 bluetooth_avrcp_deinit(VOID)
{
    FUNC_ENTRY;
    return linuxbt_rc_deinit();
}

INT32 bluetooth_avrcp_disconnect(CHAR *addr)
{
    FUNC_ENTRY;

    BT_CHECK_POINTER(BT_DEBUG_AVRCP, addr);
    if ((strlen(addr) > BT_ADDR_STR_MAX_LEN))
    {
        BT_DBG_ERROR(BT_DEBUG_AVRCP, "invalid parameter");
        return BT_ERR_STATUS_PARM_INVALID;
    }

    return linuxbt_rc_disconnect_handler(addr);
} /*bluetooth_avrcp_disconnect*/

INT32 bluetooth_avrcp_disconnect_sync(VOID)
{
    FUNC_ENTRY;
    UINT32 ui4_loop = 10;
    INT32 i4_ret = -1;

    /*due to bluedroid not support launch avrcp connection/disconnection upper MW layer*/
    return 0;
    i4_ret = linuxbt_rc_disconnect_handler(bt_last_device_addr);
    BT_CHECK_RESULT(BT_DEBUG_AVRCP, i4_ret);
    while (get_bt_avrcp_connect() && (0 < ui4_loop))
    {
        BT_DBG_NOTICE(BT_DEBUG_AVRCP, "ui4_loop:%ld,avrcp_connect:%d!",
                      (long)ui4_loop,
                      get_bt_avrcp_connect()
                     );
        /*x_thread_delay(500);*/
        usleep(500*1000);
        ui4_loop --;
    }
    if (get_bt_avrcp_connect())
    {
        BT_DBG_ERROR(BT_DEBUG_AVRCP, "can't disconnect AVRCP");
        return BT_ERR_STATUS_FAIL;
    }
    return BT_SUCCESS;
} /*bluetooth_avrcp_disconnect_sync*/

INT32 bluetooth_avrcp_connect(CHAR *addr)
{
    FUNC_ENTRY;

    BT_CHECK_POINTER(BT_DEBUG_AVRCP, addr);
    if ((strlen(addr) > BT_ADDR_STR_MAX_LEN))
    {
        BT_DBG_ERROR(BT_DEBUG_AVRCP, "invalid parameter");
        return BT_ERR_STATUS_PARM_INVALID;
    }
    return linuxbt_rc_connect_handler(addr);
} /*bluetooth_avrcp_connect*/

INT32 bluetooth_avrcp_connect_sync(CHAR *addr)
{
    FUNC_ENTRY;
    UINT32 ui4_loop = 10;
    INT32 i4_ret = -1;

    /*due to bluedroid not support launch avrcp connection/disconnection upper MW layer*/
    return BT_SUCCESS;
    BT_CHECK_POINTER(BT_DEBUG_AVRCP, addr);
    if ((strlen(addr) > BT_ADDR_STR_MAX_LEN))
    {
        BT_DBG_ERROR(BT_DEBUG_AVRCP, "invalid parameter");
        return BT_ERR_STATUS_PARM_INVALID;
    }
    i4_ret = linuxbt_rc_connect_handler(addr);
    BT_CHECK_RESULT(BT_DEBUG_AVRCP, i4_ret);
    while (!get_bt_avrcp_connect() && (0 < ui4_loop))
    {
        BT_DBG_NOTICE(BT_DEBUG_AVRCP, "ui4_loop:%ld,avrcp_connect:%d!",
                      (long)ui4_loop,
                      get_bt_avrcp_connect()
                     );
        /*x_thread_delay(500);*/
        usleep(500*1000);
        ui4_loop --;
    }
    if (!get_bt_avrcp_connect())
    {
        BT_DBG_ERROR(BT_DEBUG_AVRCP, "can't establish AVRCP connection");
        return BT_ERR_STATUS_FAIL;
    }

    return BT_SUCCESS;
} /*bluetooth_avrcp_connect_sync*/

INT32 bluetooth_avrcp_send_cmd(BTCmdType bt_cmd_type, BTN_ACTION button_action)
{
    FUNC_ENTRY;
    static BOOL fg_need_send_release = FALSE;
    bt_player_status_values_t pt_playStatus;

    bluetooth_avrcp_get_playstatus(&pt_playStatus);

    switch (bt_cmd_type)
    {
    case BT_CMD_TYPE_PLAY:
        if (BT_MODE_PLAYING == get_bt_mode()
            && (BUTTON_ACT_PRESS == button_action
            || BUTTON_ACT_AUTO == button_action))
        {
            fg_need_send_release = TRUE;
            //bluetooth_a2dp_playback_init_ext();
            bluetooth_a2dp_playback_play();
            if (AVRCP_STATUS_PLAYING == pt_playStatus.play_status)
            {
                BT_DBG_WARNING(BT_DEBUG_AVRCP, "[AVRCP] in play state, ignore play command");
                fg_need_send_release = FALSE;
                return BT_SUCCESS;
            }
        }
        else if (BT_MODE_PLAYING == get_bt_mode()
            && BUTTON_ACT_RELEASE == button_action
            && fg_need_send_release == TRUE)
        {
            fg_need_send_release = FALSE;
        }
        else
        {
            BT_DBG_WARNING(BT_DEBUG_AVRCP, "[AVRCP] not in PLAYING mode, ignore avrcp play command");
            return BT_SUCCESS;
        }
        break;
    case BT_CMD_TYPE_PAUSE:
        if (BT_MODE_PLAYING == get_bt_mode()
            && (BUTTON_ACT_PRESS == button_action
            || BUTTON_ACT_AUTO == button_action))
        {
            fg_need_send_release = TRUE;
            bluetooth_a2dp_playback_pause();
            bluetooth_a2dp_playback_deinit();
            if (AVRCP_STATUS_PAUSED == pt_playStatus.play_status)
            {
                BT_DBG_WARNING(BT_DEBUG_AVRCP, "[AVRCP] in pause state, ignore pause command");
                fg_need_send_release = FALSE;
                return BT_SUCCESS;
            }
        }
        else if (BT_MODE_PLAYING == get_bt_mode()
            && BUTTON_ACT_RELEASE == button_action
            && fg_need_send_release == TRUE)
        {
            fg_need_send_release = FALSE;
        }
        else
        {
            BT_DBG_WARNING(BT_DEBUG_AVRCP, "[AVRCP] not in PLAYING mode, ignore avrcp pause command");
            return BT_SUCCESS;
        }
        break;
    default:
        break;
    }

    if (get_bt_avrcp_connect())
    {
        return linuxbt_rc_send_cmd_handler(bt_cmd_type, button_action);
    }
    else
    {
        BT_DBG_WARNING(BT_DEBUG_AVRCP,  "should create avrcp connection first");
    }

    return BT_SUCCESS;
} /*bluetooth_avrcp_send_cmd*/

INT32 bluetooth_avrcp_get_playstatus(bt_player_status_values_t *pt_playStatus)
{
    FUNC_ENTRY;

    BT_CHECK_POINTER(BT_DEBUG_AVRCP, pt_playStatus);
    linuxbt_get_player_status(pt_playStatus);
    return BT_SUCCESS;
}

INT32 bluetooth_avrcp_send_get_playstatus_cmd(VOID)
{
    FUNC_ENTRY;

    linuxbt_rc_send_get_playstatus_cmd_handler();
    return BT_SUCCESS;
}

INT32 bluetooth_avrcp_get_mediaInfo(avrcp_medioInfo_t *pMediaInfo)
{
    FUNC_ENTRY;

    BT_CHECK_POINTER(BT_DEBUG_AVRCP, pMediaInfo);
    linuxbt_get_media_info(pMediaInfo);
    return BT_SUCCESS;
}

INT32 bluetooth_reset_playstatus(VOID)
{
    return linuxbt_reset_playstatus();
}

/*For PTS test*/
INT32 btaudio_avrcp_set_trackstatus(char* pTrackStatus)
{

    BT_CHECK_POINTER(BT_DEBUG_AVRCP, pTrackStatus);
    if (get_bt_pts_enable())
    {
        if (pTrackStatus &&
                pTrackStatus[0] == 'n' &&
                pTrackStatus[1] == 'o' &&
                pTrackStatus[2] == 't' &&
                pTrackStatus[3] == 'r' &&
                pTrackStatus[4] == 'a' &&
                pTrackStatus[5] == 'c' &&
                pTrackStatus[6] == 'k' )
        {
            g_trackstatus = 0;
            BT_DBG_NOTICE(BT_DEBUG_AVRCP, "[AVRCP] Set track status : No Track");
        }
        else if (pTrackStatus &&
                 pTrackStatus[0] == 'p' &&
                 pTrackStatus[1] == 'l' &&
                 pTrackStatus[2] == 'a' &&
                 pTrackStatus[3] == 'y' &&
                 pTrackStatus[4] == 'i' &&
                 pTrackStatus[5] == 'n' &&
                 pTrackStatus[6] == 'g' )
        {
            g_trackstatus = 1;
            BT_DBG_NOTICE(BT_DEBUG_AVRCP, "[AVRCP] Set track status : Playing");
        }
        else if (pTrackStatus &&
                 pTrackStatus[0] == 's' &&
                 pTrackStatus[1] == 'e' &&
                 pTrackStatus[2] == 'l' &&
                 pTrackStatus[3] == 'e' &&
                 pTrackStatus[4] == 'c' &&
                 pTrackStatus[5] == 't' &&
                 pTrackStatus[6] == 'e' &&
                 pTrackStatus[7] == 'd' )
        {
            g_trackstatus = 2;
            BT_DBG_NOTICE(BT_DEBUG_AVRCP, "[AVRCP] Set track status : Selected");
        }
        else
        {
            BT_DBG_NOTICE(BT_DEBUG_AVRCP, "[AVRCP] track status should be [notrack | playing | selected]! ");
            return BT_ERR_STATUS_PARM_INVALID;
        }
    }
    return BT_SUCCESS;
}

INT32 bluetooth_avrcp_register_notification(CHAR *pNotificationStr)
{
    BT_CHECK_POINTER(BT_DEBUG_AVRCP, pNotificationStr);
    if (get_bt_is_call_power_off())
    {
        BT_DBG_NOTICE(BT_DEBUG_AVRCP, "have call power off, no need register %s", pNotificationStr);
        return BT_ERR_STATUS_NOT_READY;
    }
    if (!get_bt_avrcp_connect())
    {
        BT_DBG_NOTICE(BT_DEBUG_AVRCP, "avrcp disconnect no need register %s", pNotificationStr);
        return BT_ERR_STATUS_NOT_READY;
    }
    BT_DBG_NOTICE(BT_DEBUG_AVRCP, "<<< call BtAvrcpRegNotificationReq(%s) >>>", pNotificationStr);

    return linuxbt_rc_send_reg_notification_handler(pNotificationStr);
}

INT32 bluetooth_avrcp_set_absolute_volume(UINT8 u1volume)
{
    FUNC_ENTRY;
    g_absolute_volume = u1volume;
    return linuxbt_rc_send_volume_change_rsp_handler(u1volume);
}



INT32 bluetooth_avrcp_get_capabilities(avrcp_apability_type_t type)
{
    return 0;
    /*btrc_capability_id_t capability_id = 0;

    if (CAPABILITY_COMPANYID == type)
    {
        capability_id = BTRC_CBT_COMPANY_ID;
    }
    else if (CAPABILITY_EVENTS == type)
    {
        capability_id = BTRC_CBT_EVENTS_SUPPORTED;
    }
    else
    {
        BT_DBG_ERROR(BT_DEBUG_AVRCP, "invalid parameter:%d", capability_id);
        return BT_ERR_STATUS_PARM_INVALID;
    }
    BT_DBG_NOTICE(BT_DEBUG_AVRCP, "capability_id:%d", capability_id);

    return linuxbt_rc_send_get_capabilities_handler(capability_id);*/
} /*bluetooth_avrcp_get_capabilities*/

VOID bluetooth_avrcp_handle_connect_cb(CHAR *pbt_addr)
{
    FUNC_ENTRY;
    BT_MSG_T bt_avrcp_msg;
    BT_DEV_MAPPING_INFO target_mapping_dev;

    if (NULL != pbt_addr)
    {
        BT_DBG_NORMAL(BT_DEBUG_AVRCP, "connection Connect with %s", pbt_addr);
    }
    else
    {
        /*avoid the double connection notify from blueange, and ignore 2nd one (no address) */
        BT_DBG_NORMAL(BT_DEBUG_AVRCP, "connection Connect");
        return;
    }
    /*reset the global flags*/
    set_bt_avrcp_support(TRUE);
    set_bt_avrcp_connect(TRUE);
    fg_launch_tc_from_ps = FALSE;
    fg_get_metadata_after_launch_tc = FALSE;
    g_ui4_old_track_index = 0;
    memset(&bt_last_device_addr, 0, sizeof(bt_last_device_addr));
    memcpy(bt_last_device_addr,
           pbt_addr,
           sizeof(bt_last_device_addr)
          );
    memcpy(g_bt_target_dev_info.bdAddr,
           pbt_addr,
           sizeof(bt_last_device_addr)
          );
    strncpy(target_mapping_dev.bdAddr,
            pbt_addr,
            sizeof(target_mapping_dev.bdAddr)
           );
    if (0 == bluetooth_get_dev_mapping_name(&target_mapping_dev))
    {
        memcpy(g_bt_target_dev_info.name, target_mapping_dev.name, sizeof(g_bt_target_dev_info.name));
        g_bt_target_dev_info.cod = target_mapping_dev.cod;
    }
    else
    {
        strncpy(g_bt_target_dev_info.name, "no name", sizeof(g_bt_target_dev_info.name));
    }

    if (!get_bt_a2dp_be_sink())
    {
#if ENABLE_A2DP_SRC
        _bt_update_dev_history(&g_bt_target_dev_info, g_pt_paired_sink_dev_history);
#endif
    }
    else
    {
#if ENABLE_A2DP_SINK
        _bt_update_dev_history(&g_bt_target_dev_info, g_pt_paired_src_dev_history);
#endif
    }
    bt_avrcp_msg.tMsgType = BT_MSG_FROM_MW;
    bt_avrcp_msg.body.evtType = BT_AVRCP_EVENT_CONNECT_CNF;
    i4SendMsg(&bt_avrcp_msg);


}

extern char bt_2nd_device_addr[18];
VOID bluetooth_avrcp_handle_disconnect_cb(CHAR *pbt_addr)
{
    FUNC_ENTRY;

    if (NULL != pbt_addr)
    {
        BT_DBG_NORMAL(BT_DEBUG_AVRCP, "[AVRCP] connection Disconnected with %s", pbt_addr);
    }
    else
    {
        /*avoid AVRCP disconnection notify from blueange not set address */
        BT_DBG_NORMAL(BT_DEBUG_AVRCP, "[AVRCP] connection Disconnected ");
    }

    set_bt_avrcp_connect(FALSE);
    set_bt_avrcp_absolute_volume_support(FALSE);
    fg_get_metadata_after_launch_tc = FALSE;
    if (get_bt_a2dp_be_sink())
    {
        if (!get_bt_2nd_connect_comming())
        {
            if (get_bt_a2dp_support())
            {
                /* a2dp connection have been disconnected */
                if (!(get_bt_avrcp_connect() || get_bt_a2dp_connect()))
                {
                    BT_DBG_NOTICE(BT_DEBUG_AVRCP, "a2dp down, avrcp down");
                    set_bt_connect(FALSE);
                    /*simultaneously disconnect from phone and press HOME key*/
                    if (BT_MODE_PLAYING == get_bt_mode())
                    {
                        BT_DBG_NOTICE(BT_DEBUG_AVRCP, "from playing mode");
                        //set_bt_mode(BT_MODE_PAIRING);
                        set_bt_avrcp_support(FALSE);
                        BtAppCbk(BT_DISCONNECTED, pbt_addr);
                    }
                    else if (BT_MODE_CLOSE == get_bt_mode())
                    {
                        BT_DBG_NOTICE(BT_DEBUG_AVRCP, "from close mode");
                        set_bt_avrcp_support(FALSE);
                        BtAppCbk(BT_DISCONNECTED, pbt_addr);
                    }
                    else if ((BT_MODE_PAIRING == get_bt_mode()) || (BT_MODE_CONNECTING == get_bt_mode()))
                    {
                        BT_DBG_NORMAL(BT_DEBUG_AVRCP, "from PAIRING/CONNECTING mode disconnect");
                        set_bt_avrcp_support(FALSE);
                        BtAppCbk(BT_DISCONNECTED, pbt_addr);
                    }
                    else
                    {
                        BT_DBG_NOTICE(BT_DEBUG_AVRCP, "at invalid mode, no need send disconnection notify to APP");
                    }
                }
                else
                {
                    BT_DBG_NOTICE(BT_DEBUG_AVRCP, "a2dp disconnection does not down");
                }
            }
            else
            {
                BT_DBG_NOTICE(BT_DEBUG_AVRCP, "not support a2dp?");
                if (!get_bt_avrcp_connect())
                {
                    BT_DBG_NOTICE(BT_DEBUG_AVRCP, "avrcp down, not support a2dp");
                    set_bt_connect(FALSE);
                    /*simultaneously disconnect from phone and press HOME key*/
                    if (BT_MODE_PLAYING == get_bt_mode())
                    {
                        BT_DBG_NOTICE(BT_DEBUG_AVRCP, "from playing mode");
                        //set_bt_mode(BT_MODE_PAIRING);
                        set_bt_avrcp_support(FALSE);
                        BtAppCbk(BT_DISCONNECTED, pbt_addr);
                    }
                    else if (BT_MODE_CLOSE == get_bt_mode())
                    {
                        BT_DBG_NOTICE(BT_DEBUG_AVRCP, "from close mode");
                        set_bt_avrcp_support(FALSE);
                        BtAppCbk(BT_DISCONNECTED, pbt_addr);
                    }
                    else
                    {
                        BT_DBG_NOTICE(BT_DEBUG_AVRCP, "not at CLOSE mode, no need send disconnection notify to APP");
                    }
                }
            }
        }
        else
        {
            BT_DBG_NOTICE(BT_DEBUG_AVRCP, "bt_2nd_device_addr = %s", bt_2nd_device_addr);
            if (0)//(0 == strncmp(bt_2nd_device_addr, pbt_addr, strlen(pbt_addr)))
            {
                BT_DBG_NOTICE(BT_DEBUG_AVRCP, "2nd connection disconnect, send disconnect msg to app");
                /*aVOID to result bt_mode execption while simultaneously pressing HOME and disconnecting from SRC device*/
                if (BT_MODE_PLAYING != get_bt_mode())
                {
                    BT_DBG_NOTICE(BT_DEBUG_AVRCP, "Bt_mode has changed into non-playing mode, it can not be set pairing mode");
                }
                else
                {
                    //set_bt_mode(BT_MODE_PAIRING);
                }
                BtAppCbk(BT_2ND_CONNECT_FAIL, pbt_addr);
            }
            else
            {
                BT_DBG_NOTICE(BT_DEBUG_AVRCP, "for 2nd connection, no need send disconnet msg to app");
            }
        }
    }
    else /* SRC mode */
    {
        if (get_bt_a2dp_support())
        {
            /* a2dp connection have been disconnected */
            if (!(get_bt_avrcp_connect() || get_bt_a2dp_connect()))
            {
                BT_DBG_NOTICE(BT_DEBUG_AVRCP, "a2dp down, avrcp down");
                set_bt_connect(FALSE);
                //set_bt_mode(BT_MODE_PAIRING);
                set_bt_avrcp_support(FALSE);
                BtAppCbk(BT_DISCONNECTED, pbt_addr);
            }
            else
            {
                BT_DBG_NOTICE(BT_DEBUG_AVRCP, "a2dp disconnection does not down");
            }
        }
        else
        {
            if (!get_bt_avrcp_connect())
            {
                BT_DBG_NOTICE(BT_DEBUG_AVRCP, "avrcp down, not support a2dp");
                set_bt_connect(FALSE);
                //set_bt_mode(BT_MODE_PAIRING);
                set_bt_avrcp_support(FALSE);
                BtAppCbk(BT_DISCONNECTED, pbt_addr);
            }
        }
    }
}


VOID bluetooth_avrcp_get_mediaInfo_cbk(avrcp_medioInfo_t *media_info)
{
    if (NULL != AppMediaStatusCbk)
    {
        AppMediaStatusCbk(BT_MEDIA_INFO_OK);
    }
    else
    {
        BT_DBG_ERROR(BT_DEBUG_AVRCP, "AppMediaStatusCbk is null");
    }
}

VOID bluetooth_avrcp_track_change_cbk(UINT32 bt_track_index)
{
    if (NULL != AppTrackChangeCbk)
    {
        AppTrackChangeCbk(bt_track_index);
    }
    else
    {
        BT_DBG_ERROR(BT_DEBUG_AVRCP, "AppTrackChangeCbk is null");
    }
}

VOID bluetooth_avrcp_playstatus_change_cbk(bt_avrcp_playstatus_t avrcp_player_status)
{
    BTPlayState bt_play_state = avrcp_player_status;

    if (NULL != BtAppCbk)
    {
        BtAppCbk(BT_SUPPORT_AVRCP_1_3, NULL);
    }
    else
    {
        BT_DBG_ERROR(BT_DEBUG_AVRCP, "BtAppCbk is null");
    }

    if (NULL != AppPlayStateCbk)
    {
        AppPlayStateCbk(bt_play_state);
    }
    else
    {
        BT_DBG_ERROR(BT_DEBUG_AVRCP, "AppMediaStatusCbk is null");
    }
}

VOID bluetooth_avrcp_get_playstatus_cbk(bt_player_status_values_t *player_status)
{
    if (NULL != AppMediaStatusCbk)
    {
        AppMediaStatusCbk(BT_PLAY_STATUS_OK);
    }
    else
    {
        BT_DBG_ERROR(BT_DEBUG_AVRCP, "AppMediaStatusCbk is null");
    }
}

VOID bluetooth_rc_tg_register_notification_cb(UINT8 *pu1_volume)
{
    BTAbsoluteVolumeType flag = BT_GET_VOLUME;
    set_bt_avrcp_absolute_volume_support(TRUE);
    if (AppAbsoluteVolumeCbk && pu1_volume)
    {
        AppAbsoluteVolumeCbk(flag, pu1_volume);
        BT_DBG_NOTICE(BT_DEBUG_AVRCP, "volume = %d", *pu1_volume);
    }
    else
    {
        BT_DBG_ERROR(BT_DEBUG_AVRCP, "AppAbsoluteVolumeCbk or pu1_volume is null");
    }
}

VOID bluetooth_rc_tg_set_absolute_volume_ind_cb (UINT8 u1_volume)
{
    BT_DBG_INFO(BT_DEBUG_AVRCP, "[AVRCP] volume set to %x ", u1_volume);
    BTAbsoluteVolumeType flag = BT_SEND_VOLUME;
    set_bt_avrcp_absolute_volume_support(TRUE);
    if (AppAbsoluteVolumeCbk)
    {
        AppAbsoluteVolumeCbk(flag, &u1_volume);
    }
    else
    {
        BT_DBG_ERROR(BT_DEBUG_AVRCP, "AppAbsoluteVolumeCbk is null");
    }
}

/*for PTS test*/
INT32 bluetooth_rc_tg_send_volume_change_rsp_handler(UINT8 u1_volume)
{
    return linuxbt_rc_tg_send_volume_change_rsp_handler(0, u1_volume);
}

INT32 bluetooth_rc_tg_send_track_change_rsp_handler(UINT8 u1track_selected)
{
    return linuxbt_rc_tg_send_track_change_rsp_handler(u1track_selected);
}

INT32 bluetooth_rc_tg_set_large_element_attribute_response_handler(UINT8 u1enable)
{
    return linuxbt_rc_tg_set_large_element_attribute_response_handler(u1enable);
}

INT32 bluetooth_rc_tg_send_pts_test_handler(UINT8 fgenable)
{
    return linuxbt_rc_tg_send_pts_test_handler(fgenable);
}

INT32 bluetooth_rc_set_avrcp_delay(INT32 delay)
{
    return linuxbt_rc_set_avrcp_delay(delay);
}


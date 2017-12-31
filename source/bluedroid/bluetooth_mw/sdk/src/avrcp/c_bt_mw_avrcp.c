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

#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <linux/input.h>
#include <linux/uinput.h>

#include "bluetooth.h"

#include "c_mw_config.h"
#include "c_bt_mw_manager.h"
#include "c_bt_mw_avrcp.h"
#include "bt_mw_common.h"
#include "bt_mw_avrcp.h"
#include "bluetooth_sync.h"

static VOID print_key_information(BTN_ACTION action)
{
    if (BUTTON_ACT_PRESS == action)
    {
        BT_DBG_NORMAL(BT_DEBUG_AVRCP, "press command");
    }
    else if (BUTTON_ACT_RELEASE== action)
    {
        BT_DBG_NORMAL(BT_DEBUG_AVRCP, "release command");
    }
    else if (BUTTON_ACT_AUTO== action)
    {
        BT_DBG_NORMAL(BT_DEBUG_AVRCP, "auto command");
    }
    else
    {
        BT_DBG_ERROR(BT_DEBUG_AVRCP, "invalid value");
    }
}

EXPORT_SYMBOL INT32 c_btm_avrcp_passthrough_cmd(BTCmdType bt_cmd_type, BTN_ACTION button_action)
{
    FUNC_ENTRY;

    if (get_bt_is_call_power_off())
    {
        BT_DBG_WARNING(BT_DEBUG_AVRCP,  "have call power off");
        return BT_ERR_STATUS_NOT_READY;
    }
    print_key_information(button_action);
    /* for fix only establish a2dp connection, but app still use avrcp cmd notify to release ALSA  */
    /* and check avrcp connection @ bluetooth_avrcp_send_cmd() */
    if (1)
    //if (get_bt_avrcp_connect() )
    {
        bluetooth_avrcp_send_cmd(bt_cmd_type, button_action);
    }
    else
    {
        BT_DBG_WARNING(BT_DEBUG_AVRCP,  "should create avrcp connection first");
    }
    return BT_SUCCESS;
}

/* after recive the get mediainfo done message should call this API to get value */
EXPORT_SYMBOL INT32 c_btm_avrcp_get_mediaInfo(avrcp_medioInfo_t *mediaInfo)
{
    FUNC_ENTRY;

    BT_CHECK_POINTER(BT_DEBUG_AVRCP, mediaInfo);

    if (get_bt_avrcp_connect())
    {
        bluetooth_avrcp_get_mediaInfo(mediaInfo);
    }
    else
    {
        BT_DBG_ERROR(BT_DEBUG_AVRCP,  "should create avrcp connection first");
        return BT_ERR_STATUS_NOT_READY;
    }

    return BT_SUCCESS;
}

/* after recive the get playstatus done message should call this API to get value */
EXPORT_SYMBOL INT32 c_btm_avrcp_get_play_status(bt_player_status_values_t *pt_playStatus)
{
    BT_DBG_NORMAL(BT_DEBUG_AVRCP,"+++into %s", __FUNCTION__);
    BT_CHECK_POINTER(BT_DEBUG_AVRCP, pt_playStatus);

    if (get_bt_avrcp_connect())
    {
        bluetooth_avrcp_get_playstatus(pt_playStatus);
        BT_DBG_INFO(BT_DEBUG_AVRCP, "song_length:%ld", (long)pt_playStatus->song_length);
        BT_DBG_INFO(BT_DEBUG_AVRCP, "song_position:%ld", (long)pt_playStatus->song_position);
        BT_DBG_INFO(BT_DEBUG_AVRCP, "play_status:%d", pt_playStatus->play_status);
    }
    else
    {
        /*have not send get playstatus cmd*/
        BT_DBG_ERROR(BT_DEBUG_AVRCP,  "should create avrcp connection first");
        return BT_ERR_STATUS_NOT_READY;
    }

    return BT_SUCCESS;
}

/* just to send the get_playstatus_cmd */
EXPORT_SYMBOL INT32 c_btm_avrcp_send_get_playstatus_cmd(VOID)
{
    BT_DBG_NORMAL(BT_DEBUG_AVRCP,"+++into %s", __FUNCTION__);

    if (get_bt_avrcp_connect())
    {
        bluetooth_avrcp_send_get_playstatus_cmd();
    }
    else
    {
        BT_DBG_ERROR(BT_DEBUG_AVRCP,  "should create avrcp connection first");
        return BT_ERR_STATUS_NOT_READY;
    }

    return BT_SUCCESS;
}

EXPORT_SYMBOL INT32 c_btm_set_volume(UINT8 u1volume)
{
    FUNC_ENTRY;

    if (!get_bt_avrcp_absolute_volume_support() && !get_bt_pts_enable())
    {
        BT_DBG_ERROR(BT_DEBUG_A2DP,"not support absolute volume");
        return BT_ERR_STATUS_UNSUPPORTED;
    }
    if (get_bt_is_call_power_off())
    {
        BT_DBG_NORMAL(BT_DEBUG_AVRCP, "have call power off");
        return BT_ERR_STATUS_NOT_READY;
    }
    if (get_bt_avrcp_connect())
    {

        bluetooth_avrcp_set_absolute_volume(u1volume);
    }
    else
    {
        BT_DBG_ERROR(BT_DEBUG_A2DP,"should create avrcp connection first");
        return BT_ERR_STATUS_NOT_READY;
    }
    return BT_SUCCESS;
}



/*for PTS check*/
EXPORT_SYMBOL INT32 c_btm_set_trackstatus(CHAR *ptrack_status)
{
    FUNC_ENTRY;
    return btaudio_avrcp_set_trackstatus(ptrack_status);
}

/*call by MW, register notification*/
EXPORT_SYMBOL INT32 c_btm_register_avrcp_notification(CHAR* pNotificationStr)
{
    FUNC_ENTRY;
    BT_CHECK_POINTER(BT_DEBUG_AVRCP, pNotificationStr);

    if (get_bt_is_call_power_off())
    {
        BT_DBG_WARNING(BT_DEBUG_AVRCP, "have call power off");
        return BT_ERR_STATUS_NOT_READY;
    }
    if (get_bt_avrcp_connect())
    {
        bluetooth_avrcp_register_notification(pNotificationStr);
    }
    else
    {
        BT_DBG_ERROR(BT_DEBUG_AVRCP,"should create avrcp connection first");
        return BT_ERR_STATUS_NOT_READY;
    }
    return BT_SUCCESS;
}


/*for PTS test*/
EXPORT_SYMBOL INT32 c_btm_send_volume_change_rsp_handler(UINT8 u1_volume)
{
    return bluetooth_rc_tg_send_volume_change_rsp_handler(u1_volume);
}

EXPORT_SYMBOL INT32 c_btm_send_track_change_rsp_handler(UINT8 u1track_selected)
{
    return bluetooth_rc_tg_send_track_change_rsp_handler(u1track_selected);
}

EXPORT_SYMBOL INT32 c_btm_set_large_element_attribute_response_handler(UINT8 u1enable)
{
    return bluetooth_rc_tg_set_large_element_attribute_response_handler(u1enable);
}

EXPORT_SYMBOL INT32 c_btm_send_pts_test_handler(UINT8 fgenable)
{
    set_bt_pts_enable(fgenable);
    return bluetooth_rc_tg_send_pts_test_handler(fgenable);
}

EXPORT_SYMBOL INT32 c_btm_bt_avrcp_get_capabilities(avrcp_apability_type_t type)
{
    FUNC_ENTRY;
    BT_CHECK_RESULT(BT_DEBUG_AVRCP, bluetooth_avrcp_get_capabilities(type));
    return BT_SUCCESS;
}



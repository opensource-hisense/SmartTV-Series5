#include "bt_mw_common.h"
#include "linuxbt_hfclient_if.h"

extern BtAppEventCbk BtAppCbk;
extern BtAppHfclientCbk AppHfclientCbk;

INT32 bluetooth_hfclient_connect(CHAR *bt_addr)
{
    return linuxbt_hfclient_connect(bt_addr);
}

INT32 bluetooth_hfclient_disconnect(CHAR *bt_addr)
{
    return linuxbt_hfclient_disconnect(bt_addr);
}

INT32 bluetooth_hfclient_connect_audio(CHAR *bt_addr)
{
    return linuxbt_hfclient_connect_audio(bt_addr);
}

INT32 bluetooth_hfclient_disconnect_audio(CHAR *bt_addr)
{
    return linuxbt_hfclient_disconnect_audio(bt_addr);
}

INT32 bluetooth_hfclient_start_voice_recognition(VOID)
{
    return linuxbt_hfclient_start_voice_recognition();
}

INT32 bluetooth_hfclient_stop_voice_recognition(VOID)
{
    return linuxbt_hfclient_stop_voice_recognition();
}

INT32 bluetooth_hfclient_volume_control(BT_HFCLIENT_VOLUME_TYPE_T type, INT32 volume)
{
    return linuxbt_hfclient_volume_control(type, volume);
}

INT32 bluetooth_hfclient_dial(const CHAR *number)
{
    return linuxbt_hfclient_dial(number);
}

INT32 bluetooth_hfclient_dial_memory(INT32 location)
{
    return linuxbt_hfclient_dial_memory(location);
}

INT32 bluetooth_hfclient_handle_call_action(BT_HFCLIENT_CALL_ACTION_T action, INT32 idx)
{
    return linuxbt_hfclient_handle_call_action(action, idx);
}

INT32 bluetooth_hfclient_query_current_calls(VOID)
{
    return linuxbt_hfclient_query_current_calls();
}

INT32 bluetooth_hfclient_query_current_operator_name(VOID)
{
    return linuxbt_hfclient_query_current_operator_name();
}

INT32 bluetooth_hfclient_retrieve_subscriber_info(VOID)
{
    return linuxbt_hfclient_retrieve_subscriber_info();
}

INT32 bluetooth_hfclient_send_dtmf(CHAR code)
{
    return linuxbt_hfclient_send_dtmf(code);
}

INT32 bluetooth_hfclient_request_last_voice_tag_number(VOID)
{
    return linuxbt_hfclient_request_last_voice_tag_number();
}

INT32 bluetooth_hfclient_send_at_cmd(INT32 cmd, INT32 val1, INT32 val2, const CHAR *arg)
{
    return linuxbt_hfclient_send_at_cmd(cmd, val1, val2, arg);
}

VOID bluetooth_hfclient_connection_state_cb(BT_HFCLIENT_CONNECTION_STATE_T state,
                                                      UINT32 peer_feat, UINT32 chld_feat, CHAR *bt_addr)
{
    BT_DBG_NORMAL(BT_DEBUG_HFP, "state=%d, peer_feat=%d, chld_feat=%d, bt_addr=%s",
        state, peer_feat, chld_feat, bt_addr);

    if (NULL == BtAppCbk)
    {
        return;
    }

    switch (state)
    {
    case BT_HFCLIENT_CONNECTION_STATE_DISCONNECTED:
        BtAppCbk(BT_HFCLIENT_DISCONNECTED, bt_addr);
        break;
    case BT_HFCLIENT_CONNECTION_STATE_SLC_CONNECTED:
        BtAppCbk(BT_HFCLIENT_CONNECTED, bt_addr);
        break;
    default:
        break;
    }
}

VOID bluetooth_hfclient_audio_state_cb(BT_HFCLIENT_AUDIO_STATE_T state, CHAR *bt_addr)
{
    BT_DBG_NORMAL(BT_DEBUG_HFP, "state=%d, bt_addr=%s", state, bt_addr);

    if (NULL == BtAppCbk)
    {
        return;
    }

    switch (state)
    {
    case BT_HFCLIENT_AUDIO_STATE_DISCONNECTED:
        BtAppCbk(BT_HFCLIENT_AUDIO_DISCONNECTED, bt_addr);
        break;
    case BT_HFCLIENT_AUDIO_STATE_CONNECTED:
        BtAppCbk(BT_HFCLIENT_AUDIO_CONNECTED_CVSD, bt_addr);
        break;
    case BT_HFCLIENT_AUDIO_STATE_CONNECTED_MSBC:
        BtAppCbk(BT_HFCLIENT_AUDIO_CONNECTED_MSBC, bt_addr);
        break;
    default:
        break;
    }
}

VOID bluetooth_hfclient_vr_cmd_cb(BT_HFCLIENT_VR_STATE_T state)
{
    BT_HFCLIENT_CBK_STRUCT param;

    BT_DBG_NORMAL(BT_DEBUG_HFP, "state=%d", state);
    if (AppHfclientCbk)
    {
        memset(&param, 0, sizeof(param));
        param.event = BT_HFCLIENT_VR_CMD;
        param.u.vr_state = state;
        AppHfclientCbk(&param);
    }
}

VOID bluetooth_hfclient_network_state_cb(BT_HFCLIENT_NETWORK_STATE_T state)
{
    BT_HFCLIENT_CBK_STRUCT param;

    BT_DBG_NORMAL(BT_DEBUG_HFP, "state=%d", state);
    if (AppHfclientCbk)
    {
        memset(&param, 0, sizeof(param));
        param.event = BT_HFCLIENT_NETWORK_STATE;
        param.u.network_state = state;
        AppHfclientCbk(&param);
    }
}

VOID bluetooth_hfclient_network_roaming_cb(BT_HFCLIENT_SERVICE_TYPE_T type)
{
    BT_HFCLIENT_CBK_STRUCT param;

    BT_DBG_NORMAL(BT_DEBUG_HFP, "type=%d", type);
    if (AppHfclientCbk)
    {
        memset(&param, 0, sizeof(param));
        param.event = BT_HFCLIENT_NETWORK_ROAMING;
        param.u.service_type = type;
        AppHfclientCbk(&param);
    }
}

VOID bluetooth_hfclient_network_signal_cb(int signal_strength)
{
    BT_HFCLIENT_CBK_STRUCT param;

    BT_DBG_NORMAL(BT_DEBUG_HFP, "signal_strength=%d", signal_strength);
    if (AppHfclientCbk)
    {
        memset(&param, 0, sizeof(param));
        param.event = BT_HFCLIENT_NETWORK_SIGNAL;
        param.u.signal_strength = signal_strength;
        AppHfclientCbk(&param);
    }
}

VOID bluetooth_hfclient_battery_level_cb(int battery_level)
{
    BT_HFCLIENT_CBK_STRUCT param;

    BT_DBG_NORMAL(BT_DEBUG_HFP, "battery_level=%d", battery_level);
    if (AppHfclientCbk)
    {
        memset(&param, 0, sizeof(param));
        param.event = BT_HFCLIENT_BATTERY_LEVEL;
        param.u.battery_level = battery_level;
        AppHfclientCbk(&param);
    }
}

VOID bluetooth_hfclient_current_operator_cb(const CHAR *name)
{
    BT_HFCLIENT_CBK_STRUCT param;

    BT_DBG_NORMAL(BT_DEBUG_HFP, "name=%s", name);
    if (AppHfclientCbk)
    {
        memset(&param, 0, sizeof(param));
        param.event = BT_HFCLIENT_CURRENT_OPERATOR;
        memcpy(param.u.operator_name, name, strlen(name));
        param.u.operator_name[strlen(name)] = '\0';
        AppHfclientCbk(&param);
    }
}

VOID bluetooth_hfclient_call_cb(BT_HFCLIENT_CALL_T call)
{
    BT_HFCLIENT_CBK_STRUCT param;

    BT_DBG_NORMAL(BT_DEBUG_HFP, "call=%d", call);
    if (AppHfclientCbk)
    {
        memset(&param, 0, sizeof(param));
        param.event = BT_HFCLIENT_CALL;
        param.u.call = call;
        AppHfclientCbk(&param);
    }
}

VOID bluetooth_hfclient_callsetup_cb(BT_HFCLIENT_CALLSETUP_T callsetup)
{
    BT_HFCLIENT_CBK_STRUCT param;

    BT_DBG_NORMAL(BT_DEBUG_HFP, "callsetup=%d", callsetup);
    if (AppHfclientCbk)
    {
        memset(&param, 0, sizeof(param));
        param.event = BT_HFCLIENT_CALLSETUP;
        param.u.callsetup = callsetup;
        AppHfclientCbk(&param);
    }
}

VOID bluetooth_hfclient_callheld_cb(BT_HFCLIENT_CALLHELD_T callheld)
{
    BT_HFCLIENT_CBK_STRUCT param;

    BT_DBG_NORMAL(BT_DEBUG_HFP, "callheld=%d", callheld);
    if (AppHfclientCbk)
    {
        memset(&param, 0, sizeof(param));
        param.event = BT_HFCLIENT_CALLHELD;
        param.u.callheld = callheld;
        AppHfclientCbk(&param);
    }
}

VOID bluetooth_hfclient_resp_and_hold_cb(BT_HFCLIENT_RESP_AND_HOLD_T resp_and_hold)
{
    BT_HFCLIENT_CBK_STRUCT param;

    BT_DBG_NORMAL(BT_DEBUG_HFP, "resp_and_hold=%d", resp_and_hold);
    if (AppHfclientCbk)
    {
        memset(&param, 0, sizeof(param));
        param.event = BT_HFCLIENT_RESP_AND_HOLD;
        param.u.resp_and_hold = resp_and_hold;
        AppHfclientCbk(&param);
    }
}

VOID bluetooth_hfclient_clip_cb(const CHAR *number)
{
    BT_HFCLIENT_CBK_STRUCT param;

    BT_DBG_NORMAL(BT_DEBUG_HFP, "number=%s", (number != NULL) ? number : "");
    if (AppHfclientCbk)
    {
        memset(&param, 0, sizeof(param));
        param.event = BT_HFCLIENT_CLIP;
        if (number)
        {
            memcpy(param.u.clip_number, number, strlen(number));
            param.u.clip_number[strlen(number)] = '\0';
        }
        else
        {
            param.u.clip_number[0] = '\0';
        }
        AppHfclientCbk(&param);
    }
}

VOID bluetooth_hfclient_call_waiting_cb(const CHAR *number)
{
    BT_HFCLIENT_CBK_STRUCT param;

    BT_DBG_NORMAL(BT_DEBUG_HFP, "number=%s", (number != NULL) ? number : "");
    if (AppHfclientCbk)
    {
        memset(&param, 0, sizeof(param));
        param.event = BT_HFCLIENT_CALL_WAITING;
        if (number)
        {
            memcpy(param.u.call_waiting_number, number, strlen(number));
            param.u.call_waiting_number[strlen(number)] = '\0';
        }
        else
        {
            param.u.call_waiting_number[0] = '\0';
        }
        AppHfclientCbk(&param);
    }
}

VOID bluetooth_hfclient_current_calls_cb(int index, BT_HFCLIENT_CALL_DIRECTION_T dir, BT_HFCLIENT_CALL_STATE_T state,
                                                 BT_HFCLIENT_CALL_MPTY_TYPE_T mpty, const CHAR *number)
{
    BT_HFCLIENT_CBK_STRUCT param;

    BT_DBG_NORMAL(BT_DEBUG_HFP, "index=%d, dir=%d, state=%d, mpty=%d, number=%s", index, dir, state, mpty, (number != NULL) ? number : "");
    if (AppHfclientCbk)
    {
        memset(&param, 0, sizeof(param));
        param.event = BT_HFCLIENT_CURRENT_CALLS;
        param.u.current_calls.index = index;
        param.u.current_calls.dir = dir;
        param.u.current_calls.state = state;
        param.u.current_calls.mpty = mpty;
        if (number)
        {
            memcpy(param.u.current_calls.number, number, strlen(number));
            param.u.current_calls.number[strlen(number)] = '\0';
        }
        else
        {
            param.u.current_calls.number[0] = '\0';
        }
        AppHfclientCbk(&param);
    }
}

VOID bluetooth_hfclient_volume_change_cb(BT_HFCLIENT_VOLUME_TYPE_T type, int volume)
{
    BT_HFCLIENT_CBK_STRUCT param;

    BT_DBG_NORMAL(BT_DEBUG_HFP, "type=%d, volume=%d", type, volume);
    if (AppHfclientCbk)
    {
        memset(&param, 0, sizeof(param));
        param.event = BT_HFCLIENT_VOLUME_CHANGE;
        param.u.volume_change.type = type;
        param.u.volume_change.volume = volume;
        AppHfclientCbk(&param);
    }
}

VOID bluetooth_hfclient_cmd_complete_cb(BT_HFCLIENT_CMD_COMPLETE_TYPE_T type, int cme)
{
    BT_HFCLIENT_CBK_STRUCT param;

    BT_DBG_NORMAL(BT_DEBUG_HFP, "type=%d, cme=%d", type, cme);
    if (AppHfclientCbk)
    {
        memset(&param, 0, sizeof(param));
        param.event = BT_HFCLIENT_CMD_COMPLETE;
        param.u.cmd_complete.type = type;
        param.u.cmd_complete.cme = cme;
        AppHfclientCbk(&param);
    }
}

VOID bluetooth_hfclient_subscriber_info_cb(const CHAR *number, BT_HFCLIENT_SUBSCRIBER_SERVICE_TYPE_T type)
{
    BT_HFCLIENT_CBK_STRUCT param;

    BT_DBG_NORMAL(BT_DEBUG_HFP, "number=%s, type=%d", (number != NULL) ? number : "", type);
    if (AppHfclientCbk)
    {
        memset(&param, 0, sizeof(param));
        param.event = BT_HFCLIENT_SUBSCRIBER_INFO;
        if (number)
        {
            memcpy(param.u.subscriber_info.number, number, strlen(number));
            param.u.subscriber_info.number[strlen(number)] = '\0';
        }
        else
        {
            param.u.subscriber_info.number[0] = '\0';
        }
        param.u.subscriber_info.type = type;
        AppHfclientCbk(&param);
    }
}

VOID bluetooth_hfclient_in_band_ring_tone_cb(BT_HFCLIENT_IN_BAND_RING_STATE_T state)
{
    BT_HFCLIENT_CBK_STRUCT param;

    BT_DBG_NORMAL(BT_DEBUG_HFP, "state=%d", state);
    if (AppHfclientCbk)
    {
        memset(&param, 0, sizeof(param));
        param.event = BT_HFCLIENT_INBAND_RINGTONE;
        param.u.inband_ring_state = state;
        AppHfclientCbk(&param);
    }
}

VOID bluetooth_hfclient_last_voice_tag_number_cb(const CHAR *number)
{
    BT_HFCLIENT_CBK_STRUCT param;

    BT_DBG_NORMAL(BT_DEBUG_HFP, "number=%s", (number != NULL) ? number : "");
    if (AppHfclientCbk)
    {
        memset(&param, 0, sizeof(param));
        param.event = BT_HFCLIENT_LAST_VOICE_TAG_NUMBER;
        if (number)
        {
            memcpy(param.u.voice_tag_number, number, strlen(number));
            param.u.voice_tag_number[strlen(number)] = '\0';
        }
        else
        {
            param.u.voice_tag_number[0] = '\0';
        }
        AppHfclientCbk(&param);
    }
}

VOID bluetooth_hfclient_ring_indication_cb(VOID)
{
    BT_HFCLIENT_CBK_STRUCT param;

    BT_DBG_NORMAL(BT_DEBUG_HFP, "");
    if (AppHfclientCbk)
    {
        memset(&param, 0, sizeof(param));
        param.event = BT_HFCLIENT_RING_INDICATION;
        AppHfclientCbk(&param);
    }
}

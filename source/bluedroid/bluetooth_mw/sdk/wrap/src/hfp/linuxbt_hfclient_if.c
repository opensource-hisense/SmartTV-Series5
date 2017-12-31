#include "linuxbt_common.h"
#include "linuxbt_hfclient_if.h"
#include "bt_mw_common.h"
#include "bt_mw_hfclient.h"

extern void *linuxbt_gap_get_profile_interface(const char *profile_id);
static void linuxbt_hfclient_connection_state_cb(bthf_client_connection_state_t state,
    unsigned int peer_feat, unsigned int chld_feat, bt_bdaddr_t *bd_addr);
static void linuxbt_hfclient_audio_state_cb(bthf_client_audio_state_t state, bt_bdaddr_t *bd_addr);
static void linuxbt_hfclient_vr_cmd_cb(bthf_client_vr_state_t state);
static void linuxbt_hfclient_network_state_cb(bthf_client_network_state_t state);
static void linuxbt_hfclient_network_roaming_cb(bthf_client_service_type_t type);
static void linuxbt_hfclient_network_signal_cb(int signal_strength);
static void linuxbt_hfclient_battery_level_cb(int battery_level);
static void linuxbt_hfclient_current_operator_cb(const char *name);
static void linuxbt_hfclient_call_cb(bthf_client_call_t call);
static void linuxbt_hfclient_callsetup_cb(bthf_client_callsetup_t callsetup);
static void linuxbt_hfclient_callheld_cb(bthf_client_callheld_t callheld);
static void linuxbt_hfclient_resp_and_hold_cb(bthf_client_resp_and_hold_t resp_and_hold);
static void linuxbt_hfclient_clip_cb(const char *number);
static void linuxbt_hfclient_call_waiting_cb(const char *number);
static void linuxbt_hfclient_current_calls_cb(int index, bthf_client_call_direction_t dir,
    bthf_client_call_state_t state, bthf_client_call_mpty_type_t mpty, const char *number);
static void linuxbt_hfclient_volume_change_cb(bthf_client_volume_type_t type, int volume);
static void linuxbt_hfclient_cmd_complete_cb(bthf_client_cmd_complete_t type, int cme);
static void linuxbt_hfclient_subscriber_info_cb(const char *name, bthf_client_subscriber_service_type_t type);
static void linuxbt_hfclient_in_band_ring_tone_cb(bthf_client_in_band_ring_state_t state);
static void linuxbt_hfclient_last_voice_tag_number_cb(const char *number);
static void linuxbt_hfclient_ring_indication_cb(void);

static bthf_client_interface_t *g_bt_hfclient_interface = NULL;
static bthf_client_callbacks_t g_bt_hfclient_callbacks =
{
    sizeof(bthf_client_callbacks_t),
    linuxbt_hfclient_connection_state_cb,
    linuxbt_hfclient_audio_state_cb,
    linuxbt_hfclient_vr_cmd_cb,
    linuxbt_hfclient_network_state_cb,
    linuxbt_hfclient_network_roaming_cb,
    linuxbt_hfclient_network_signal_cb,
    linuxbt_hfclient_battery_level_cb,
    linuxbt_hfclient_current_operator_cb,
    linuxbt_hfclient_call_cb,
    linuxbt_hfclient_callsetup_cb,
    linuxbt_hfclient_callheld_cb,
    linuxbt_hfclient_resp_and_hold_cb,
    linuxbt_hfclient_clip_cb,
    linuxbt_hfclient_call_waiting_cb,
    linuxbt_hfclient_current_calls_cb,
    linuxbt_hfclient_volume_change_cb,
    linuxbt_hfclient_cmd_complete_cb,
    linuxbt_hfclient_subscriber_info_cb,
    linuxbt_hfclient_in_band_ring_tone_cb,
    linuxbt_hfclient_last_voice_tag_number_cb,
    linuxbt_hfclient_ring_indication_cb,
};

int linuxbt_hfclient_init(void)
{
    INT32 ret = 0;

    g_bt_hfclient_interface = (bthf_client_interface_t *)linuxbt_gap_get_profile_interface(BT_PROFILE_HANDSFREE_CLIENT_ID);
    if (NULL == g_bt_hfclient_interface)
    {
        BT_DBG_ERROR(BT_DEBUG_HFP, "Failed to get HFP Client interface");
        return BT_ERR_STATUS_FAIL;
    }

    ret = g_bt_hfclient_interface->init(&g_bt_hfclient_callbacks);
    BT_DBG_MINOR(BT_DEBUG_HFP, "init ret=%d", ret);
    return BT_STATUS_SUCCESS;
}

int linuxbt_hfclient_deinit(void)
{
    if (NULL == g_bt_hfclient_interface)
    {
        BT_DBG_ERROR(BT_DEBUG_HFP, "g_bt_hfclient_interface not init");
        return BT_ERR_STATUS_PARM_INVALID;
    }

    g_bt_hfclient_interface->cleanup();
    return BT_SUCCESS;
}

int linuxbt_hfclient_connect(char *bt_addr)
{
    bt_bdaddr_t bdaddr;
    bt_status_t ret = BT_STATUS_SUCCESS;

    if (NULL == g_bt_hfclient_interface)
    {
        BT_DBG_ERROR(BT_DEBUG_HFP, "g_bt_hfclient_interface not init");
        return BT_ERR_STATUS_PARM_INVALID;
    }

    if (NULL == bt_addr)
    {
        BT_DBG_ERROR(BT_DEBUG_HFP, "null pointer");
        return BT_ERR_STATUS_PARM_INVALID;
    }

    linuxbt_btaddr_stoh(bt_addr, &bdaddr);
    BT_DBG_INFO(BT_DEBUG_HFP, "HFClient connect to %02X:%02X:%02X:%02X:%02X:%02X",
                bdaddr.address[0], bdaddr.address[1], bdaddr.address[2],
                bdaddr.address[3], bdaddr.address[4], bdaddr.address[5]);

    ret = g_bt_hfclient_interface->connect(&bdaddr);
    BT_DBG_MINOR(BT_DEBUG_HFP, "connect ret=%d", ret);
    return BT_STATUS_SUCCESS;
}

int linuxbt_hfclient_disconnect(char *bt_addr)
{
    bt_bdaddr_t bdaddr;
    bt_status_t ret = BT_STATUS_SUCCESS;

    if (NULL == g_bt_hfclient_interface)
    {
        BT_DBG_ERROR(BT_DEBUG_HFP, "g_bt_hfclient_interface not init");
        return BT_ERR_STATUS_PARM_INVALID;
    }

    if (NULL == bt_addr)
    {
        BT_DBG_ERROR(BT_DEBUG_HFP, "null pointer");
        return BT_ERR_STATUS_PARM_INVALID;
    }

    linuxbt_btaddr_stoh(bt_addr, &bdaddr);
    BT_DBG_INFO(BT_DEBUG_HFP, "HFClient disconnect to %02X:%02X:%02X:%02X:%02X:%02X",
                bdaddr.address[0], bdaddr.address[1], bdaddr.address[2],
                bdaddr.address[3], bdaddr.address[4], bdaddr.address[5]);

    ret = g_bt_hfclient_interface->disconnect(&bdaddr);
    BT_DBG_MINOR(BT_DEBUG_HFP, "disconnect ret=%d", ret);
    return BT_STATUS_SUCCESS;
}

int linuxbt_hfclient_connect_audio(char *bt_addr)
{
    bt_bdaddr_t bdaddr;
    bt_status_t ret = BT_STATUS_SUCCESS;

    if (NULL == g_bt_hfclient_interface)
    {
        BT_DBG_ERROR(BT_DEBUG_HFP, "g_bt_hfclient_interface not init");
        return BT_ERR_STATUS_PARM_INVALID;
    }

    if (NULL == bt_addr)
    {
        BT_DBG_ERROR(BT_DEBUG_HFP, "null pointer");
        return BT_ERR_STATUS_PARM_INVALID;
    }

    linuxbt_btaddr_stoh(bt_addr, &bdaddr);
    BT_DBG_INFO(BT_DEBUG_HFP, "HFClient connect audio to %02X:%02X:%02X:%02X:%02X:%02X",
                bdaddr.address[0], bdaddr.address[1], bdaddr.address[2],
                bdaddr.address[3], bdaddr.address[4], bdaddr.address[5]);

    ret = g_bt_hfclient_interface->connect_audio(&bdaddr);
    BT_DBG_MINOR(BT_DEBUG_HFP, "connect_audio ret=%d", ret);
    return BT_STATUS_SUCCESS;
}

int linuxbt_hfclient_disconnect_audio(char *bt_addr)
{
    bt_bdaddr_t bdaddr;
    bt_status_t ret = BT_STATUS_SUCCESS;

    if (NULL == g_bt_hfclient_interface)
    {
        BT_DBG_ERROR(BT_DEBUG_HFP, "g_bt_hfclient_interface not init");
        return BT_ERR_STATUS_PARM_INVALID;
    }

    if (NULL == bt_addr)
    {
        BT_DBG_ERROR(BT_DEBUG_HFP, "null pointer");
        return BT_ERR_STATUS_PARM_INVALID;
    }

    linuxbt_btaddr_stoh(bt_addr, &bdaddr);
    BT_DBG_INFO(BT_DEBUG_HFP, "HFClient disconnect audio to %02X:%02X:%02X:%02X:%02X:%02X",
                bdaddr.address[0], bdaddr.address[1], bdaddr.address[2],
                bdaddr.address[3], bdaddr.address[4], bdaddr.address[5]);

    ret = g_bt_hfclient_interface->disconnect_audio(&bdaddr);
    BT_DBG_MINOR(BT_DEBUG_HFP, "disconnect_audio ret=%d", ret);
    return BT_STATUS_SUCCESS;
}

int linuxbt_hfclient_start_voice_recognition(void)
{
    bt_status_t ret = BT_STATUS_SUCCESS;

    if (NULL == g_bt_hfclient_interface)
    {
        BT_DBG_ERROR(BT_DEBUG_HFP, "g_bt_hfclient_interface not init");
        return BT_ERR_STATUS_PARM_INVALID;
    }

    ret = g_bt_hfclient_interface->start_voice_recognition();
    BT_DBG_MINOR(BT_DEBUG_HFP, "start_voice_recognition ret=%d", ret);
    return BT_STATUS_SUCCESS;
}

int linuxbt_hfclient_stop_voice_recognition(void)
{
    bt_status_t ret = BT_STATUS_SUCCESS;

    if (NULL == g_bt_hfclient_interface)
    {
        BT_DBG_ERROR(BT_DEBUG_HFP, "g_bt_hfclient_interface not init");
        return BT_ERR_STATUS_PARM_INVALID;
    }

    ret = g_bt_hfclient_interface->stop_voice_recognition();
    BT_DBG_MINOR(BT_DEBUG_HFP, "stop_voice_recognition ret=%d", ret);
    return BT_STATUS_SUCCESS;
}

int linuxbt_hfclient_volume_control(bthf_client_volume_type_t type, int volume)
{
    bt_status_t ret = BT_STATUS_SUCCESS;

    if (NULL == g_bt_hfclient_interface)
    {
        BT_DBG_ERROR(BT_DEBUG_HFP, "g_bt_hfclient_interface not init");
        return BT_ERR_STATUS_PARM_INVALID;
    }

    ret = g_bt_hfclient_interface->volume_control(type, volume);
    BT_DBG_MINOR(BT_DEBUG_HFP, "volume_control(type=%d, volume=%d) ret=%d", type, volume, ret);
    return BT_STATUS_SUCCESS;
}

int linuxbt_hfclient_dial(const char *number)
{
    bt_status_t ret = BT_STATUS_SUCCESS;

    if (NULL == g_bt_hfclient_interface)
    {
        BT_DBG_ERROR(BT_DEBUG_HFP, "g_bt_hfclient_interface not init");
        return BT_ERR_STATUS_PARM_INVALID;
    }

    ret = g_bt_hfclient_interface->dial(number);
    BT_DBG_MINOR(BT_DEBUG_HFP, "dial(number=%s) ret=%d", (number != NULL) ? number : "", ret);
    return BT_STATUS_SUCCESS;
}

int linuxbt_hfclient_dial_memory(int location)
{
    bt_status_t ret = BT_STATUS_SUCCESS;

    if (NULL == g_bt_hfclient_interface)
    {
        BT_DBG_ERROR(BT_DEBUG_HFP, "g_bt_hfclient_interface not init");
        return BT_ERR_STATUS_PARM_INVALID;
    }

    ret = g_bt_hfclient_interface->dial_memory(location);
    BT_DBG_MINOR(BT_DEBUG_HFP, "dial_memory(location=%d) ret=%d", location, ret);
    return BT_STATUS_SUCCESS;
}

int linuxbt_hfclient_handle_call_action(bthf_client_call_action_t action, int idx)
{
    bt_status_t ret = BT_STATUS_SUCCESS;

    if (NULL == g_bt_hfclient_interface)
    {
        BT_DBG_ERROR(BT_DEBUG_HFP, "g_bt_hfclient_interface not init");
        return BT_ERR_STATUS_PARM_INVALID;
    }

    ret = g_bt_hfclient_interface->handle_call_action(action, idx);
    BT_DBG_MINOR(BT_DEBUG_HFP, "handle_call_action(action=%d, idx=%d) ret=%d", action, idx, ret);
    return BT_STATUS_SUCCESS;
}

int linuxbt_hfclient_query_current_calls(void)
{
    bt_status_t ret = BT_STATUS_SUCCESS;

    if (NULL == g_bt_hfclient_interface)
    {
        BT_DBG_ERROR(BT_DEBUG_HFP, "g_bt_hfclient_interface not init");
        return BT_ERR_STATUS_PARM_INVALID;
    }

    ret = g_bt_hfclient_interface->query_current_calls();
    BT_DBG_MINOR(BT_DEBUG_HFP, "query_current_calls ret=%d", ret);
    return BT_STATUS_SUCCESS;
}

int linuxbt_hfclient_query_current_operator_name(void)
{
    bt_status_t ret = BT_STATUS_SUCCESS;

    if (NULL == g_bt_hfclient_interface)
    {
        BT_DBG_ERROR(BT_DEBUG_HFP, "g_bt_hfclient_interface not init");
        return BT_ERR_STATUS_PARM_INVALID;
    }

    ret = g_bt_hfclient_interface->query_current_operator_name();
    BT_DBG_MINOR(BT_DEBUG_HFP, "query_current_operator_name ret=%d", ret);
    return BT_STATUS_SUCCESS;
}

int linuxbt_hfclient_retrieve_subscriber_info(void)
{
    bt_status_t ret = BT_STATUS_SUCCESS;

    if (NULL == g_bt_hfclient_interface)
    {
        BT_DBG_ERROR(BT_DEBUG_HFP, "g_bt_hfclient_interface not init");
        return BT_ERR_STATUS_PARM_INVALID;
    }

    ret = g_bt_hfclient_interface->retrieve_subscriber_info();
    BT_DBG_MINOR(BT_DEBUG_HFP, "retrieve_subscriber_info ret=%d", ret);
    return BT_STATUS_SUCCESS;
}

int linuxbt_hfclient_send_dtmf(char code)
{
    bt_status_t ret = BT_STATUS_SUCCESS;

    if (NULL == g_bt_hfclient_interface)
    {
        BT_DBG_ERROR(BT_DEBUG_HFP, "g_bt_hfclient_interface not init");
        return BT_ERR_STATUS_PARM_INVALID;
    }

    ret = g_bt_hfclient_interface->send_dtmf(code);
    BT_DBG_MINOR(BT_DEBUG_HFP, "send_dtmf(code=%d) ret=%d", code, ret);
    return BT_STATUS_SUCCESS;
}

int linuxbt_hfclient_request_last_voice_tag_number(void)
{
    bt_status_t ret = BT_STATUS_SUCCESS;

    if (NULL == g_bt_hfclient_interface)
    {
        BT_DBG_ERROR(BT_DEBUG_HFP, "g_bt_hfclient_interface not init");
        return BT_ERR_STATUS_PARM_INVALID;
    }

    ret = g_bt_hfclient_interface->request_last_voice_tag_number();
    BT_DBG_MINOR(BT_DEBUG_HFP, "request_last_voice_tag_number ret=%d", ret);
    return BT_STATUS_SUCCESS;
}

int linuxbt_hfclient_send_at_cmd(int cmd, int val1, int val2, const char *arg)
{
    bt_status_t ret = BT_STATUS_SUCCESS;

    if (NULL == g_bt_hfclient_interface)
    {
        BT_DBG_ERROR(BT_DEBUG_HFP, "g_bt_hfclient_interface not init");
        return BT_ERR_STATUS_PARM_INVALID;
    }

    ret = g_bt_hfclient_interface->send_at_cmd(cmd, val1, val2, arg);
    BT_DBG_MINOR(BT_DEBUG_HFP, "send_at_cmd(cmd=%d, val1=%d, val2=%d, arg=%s) ret=%d", cmd, val1, val2, arg, ret);
    return BT_STATUS_SUCCESS;
}

static void linuxbt_hfclient_connection_state_cb(bthf_client_connection_state_t state,
    unsigned int peer_feat, unsigned int chld_feat, bt_bdaddr_t *bd_addr)
{
    char btaddr_s[MAX_BDADDR_LEN];

    BT_DBG_INFO(BT_DEBUG_HFP, "state=%d, peer_feat=%d, chld_feat=%d, addr=%02X:%02X:%02X:%02X:%02X:%02X",
                state, peer_feat, chld_feat,
                bd_addr->address[0], bd_addr->address[1], bd_addr->address[2],
                bd_addr->address[3], bd_addr->address[4], bd_addr->address[5]);
    linuxbt_btaddr_htos(bd_addr, btaddr_s);
    bluetooth_hfclient_connection_state_cb((BT_HFCLIENT_CONNECTION_STATE_T)state, peer_feat, chld_feat, btaddr_s);
}

static void linuxbt_hfclient_audio_state_cb(bthf_client_audio_state_t state, bt_bdaddr_t *bd_addr)
{
    char btaddr_s[MAX_BDADDR_LEN];

    BT_DBG_INFO(BT_DEBUG_HFP, "state=%d, addr=%02X:%02X:%02X:%02X:%02X:%02X", state,
                bd_addr->address[0], bd_addr->address[1], bd_addr->address[2],
                bd_addr->address[3], bd_addr->address[4], bd_addr->address[5]);
    linuxbt_btaddr_htos(bd_addr, btaddr_s);
    bluetooth_hfclient_audio_state_cb((BT_HFCLIENT_AUDIO_STATE_T)state, btaddr_s);
}

static void linuxbt_hfclient_vr_cmd_cb(bthf_client_vr_state_t state)
{
    BT_DBG_INFO(BT_DEBUG_HFP, "state=%d", state);
    bluetooth_hfclient_vr_cmd_cb((BT_HFCLIENT_VR_STATE_T)state);
}

static void linuxbt_hfclient_network_state_cb(bthf_client_network_state_t state)
{
    BT_DBG_INFO(BT_DEBUG_HFP, "state=%d", state);
    bluetooth_hfclient_network_state_cb((BT_HFCLIENT_NETWORK_STATE_T)state);
}

static void linuxbt_hfclient_network_roaming_cb(bthf_client_service_type_t type)
{
    BT_DBG_INFO(BT_DEBUG_HFP, "type=%d", type);
    bluetooth_hfclient_network_roaming_cb((BT_HFCLIENT_SERVICE_TYPE_T)type);
}

static void linuxbt_hfclient_network_signal_cb(int signal_strength)
{
    BT_DBG_INFO(BT_DEBUG_HFP, "signal_strength=%d", signal_strength);
    bluetooth_hfclient_network_signal_cb(signal_strength);
}

static void linuxbt_hfclient_battery_level_cb(int battery_level)
{
    BT_DBG_INFO(BT_DEBUG_HFP, "battery_level=%d", battery_level);
    bluetooth_hfclient_battery_level_cb(battery_level);
}

static void linuxbt_hfclient_current_operator_cb(const char *name)
{
    BT_DBG_INFO(BT_DEBUG_HFP, "name=%s", name);
    bluetooth_hfclient_current_operator_cb(name);
}

static void linuxbt_hfclient_call_cb(bthf_client_call_t call)
{
    BT_DBG_INFO(BT_DEBUG_HFP, "call=%d", call);
    bluetooth_hfclient_call_cb((BT_HFCLIENT_CALL_T)call);
}

static void linuxbt_hfclient_callsetup_cb(bthf_client_callsetup_t callsetup)
{
    BT_DBG_INFO(BT_DEBUG_HFP, "callsetup=%d", callsetup);
    bluetooth_hfclient_callsetup_cb((BT_HFCLIENT_CALLSETUP_T)callsetup);
}

static void linuxbt_hfclient_callheld_cb(bthf_client_callheld_t callheld)
{
    BT_DBG_INFO(BT_DEBUG_HFP, "callheld=%d", callheld);
    bluetooth_hfclient_callheld_cb((BT_HFCLIENT_CALLHELD_T)callheld);
}

static void linuxbt_hfclient_resp_and_hold_cb(bthf_client_resp_and_hold_t resp_and_hold)
{
    BT_DBG_INFO(BT_DEBUG_HFP, "resp_and_hold=%d", resp_and_hold);
    bluetooth_hfclient_resp_and_hold_cb((BT_HFCLIENT_RESP_AND_HOLD_T)resp_and_hold);
}

static void linuxbt_hfclient_clip_cb(const char *number)
{
    BT_DBG_INFO(BT_DEBUG_HFP, "number=%s", (number != NULL) ? number : "");
    bluetooth_hfclient_clip_cb(number);
}

static void linuxbt_hfclient_call_waiting_cb(const char *number)
{
    BT_DBG_INFO(BT_DEBUG_HFP, "number=%s", (number != NULL) ? number : "");
    bluetooth_hfclient_call_waiting_cb(number);
}

static void linuxbt_hfclient_current_calls_cb(int index, bthf_client_call_direction_t dir,
                                           bthf_client_call_state_t state,
                                           bthf_client_call_mpty_type_t mpty,
                                           const char *number)
{
    BT_DBG_INFO(BT_DEBUG_HFP, "index=%d, dir=%d, state=%d, mpty=%d, number=%s", index, dir, state, mpty, number);
    bluetooth_hfclient_current_calls_cb(index, (BT_HFCLIENT_CALL_DIRECTION_T)dir,
        (BT_HFCLIENT_CALL_STATE_T)state, (BT_HFCLIENT_CALL_MPTY_TYPE_T)mpty, (number != NULL) ? number : "");
}

static void linuxbt_hfclient_volume_change_cb(bthf_client_volume_type_t type, int volume)
{
    BT_DBG_INFO(BT_DEBUG_HFP, "type=%d, volume=%d", type, volume);
    bluetooth_hfclient_volume_change_cb((BT_HFCLIENT_VOLUME_TYPE_T)type, volume);
}

static void linuxbt_hfclient_cmd_complete_cb(bthf_client_cmd_complete_t type, int cme)
{
    BT_DBG_INFO(BT_DEBUG_HFP, "type=%d, cme=%d", type, cme);
    bluetooth_hfclient_cmd_complete_cb((BT_HFCLIENT_CMD_COMPLETE_TYPE_T)type, cme);
}

static void linuxbt_hfclient_subscriber_info_cb(const char *number,
                                                       bthf_client_subscriber_service_type_t type)
{
    BT_DBG_INFO(BT_DEBUG_HFP, "number=%s, type=%d", (number != NULL) ? number : "", type);
    bluetooth_hfclient_subscriber_info_cb(number, (BT_HFCLIENT_SUBSCRIBER_SERVICE_TYPE_T)type);
}

static void linuxbt_hfclient_in_band_ring_tone_cb(bthf_client_in_band_ring_state_t state)
{
    BT_DBG_INFO(BT_DEBUG_HFP, "state=%d", state);
    bluetooth_hfclient_in_band_ring_tone_cb((BT_HFCLIENT_IN_BAND_RING_STATE_T)state);
}

static void linuxbt_hfclient_last_voice_tag_number_cb(const char *number)
{
    BT_DBG_INFO(BT_DEBUG_HFP, "number=%s", (number != NULL) ? number : "");
    bluetooth_hfclient_last_voice_tag_number_cb(number);
}

static void linuxbt_hfclient_ring_indication_cb(void)
{
    BT_DBG_INFO(BT_DEBUG_HFP, "");
    bluetooth_hfclient_ring_indication_cb();
}

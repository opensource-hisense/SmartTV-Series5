#include "bt_mw_hfclient.h"

EXPORT_SYMBOL INT32 c_btm_bt_hfclient_connect(CHAR *bt_addr)
{
    return bluetooth_hfclient_connect(bt_addr);
}

EXPORT_SYMBOL INT32 c_btm_bt_hfclient_disconnect(CHAR *bt_addr)
{
    return bluetooth_hfclient_disconnect(bt_addr);
}

EXPORT_SYMBOL INT32 c_btm_bt_hfclient_connect_audio(CHAR *bt_addr)
{
    return bluetooth_hfclient_connect_audio(bt_addr);
}

EXPORT_SYMBOL INT32 c_btm_bt_hfclient_disconnect_audio(CHAR *bt_addr)
{
    return bluetooth_hfclient_disconnect_audio(bt_addr);
}

EXPORT_SYMBOL INT32 c_btm_bt_hfclient_start_voice_recognition(VOID)
{
    return bluetooth_hfclient_start_voice_recognition();
}

EXPORT_SYMBOL INT32 c_btm_bt_hfclient_stop_voice_recognition(VOID)
{
    return bluetooth_hfclient_stop_voice_recognition();
}

EXPORT_SYMBOL INT32 c_btm_bt_hfclient_volume_control(BT_HFCLIENT_VOLUME_TYPE_T type, INT32 volume)
{
    return bluetooth_hfclient_volume_control(type, volume);
}

EXPORT_SYMBOL INT32 c_btm_bt_hfclient_dial(const CHAR *number)
{
    return bluetooth_hfclient_dial(number);
}

EXPORT_SYMBOL INT32 c_btm_bt_hfclient_dial_memory(INT32 location)
{
    return bluetooth_hfclient_dial_memory(location);
}

EXPORT_SYMBOL INT32 c_btm_bt_hfclient_handle_call_action(BT_HFCLIENT_CALL_ACTION_T action, INT32 idx)
{
    return bluetooth_hfclient_handle_call_action(action, idx);
}

EXPORT_SYMBOL INT32 c_btm_bt_hfclient_query_current_calls(VOID)
{
    return bluetooth_hfclient_query_current_calls();
}

EXPORT_SYMBOL INT32 c_btm_bt_hfclient_query_current_operator_name(VOID)
{
    return bluetooth_hfclient_query_current_operator_name();
}

EXPORT_SYMBOL INT32 c_btm_bt_hfclient_retrieve_subscriber_info(VOID)
{
    return bluetooth_hfclient_retrieve_subscriber_info();
}

EXPORT_SYMBOL INT32 c_btm_bt_hfclient_send_dtmf(CHAR code)
{
    return bluetooth_hfclient_send_dtmf(code);
}

EXPORT_SYMBOL INT32 c_btm_bt_hfclient_request_last_voice_tag_number(VOID)
{
    return bluetooth_hfclient_request_last_voice_tag_number();
}

EXPORT_SYMBOL INT32 c_btm_bt_hfclient_send_at_cmd(INT32 cmd, INT32 val1, INT32 val2, const CHAR *arg)
{
    return bluetooth_hfclient_send_at_cmd(cmd, val1, val2, arg);
}

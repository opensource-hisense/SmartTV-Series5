#ifndef _BT_MW_HFCLIENT_H_
#define _BT_MW_HFCLIENT_H_

#include "u_bt_mw_hfclient.h"

INT32 bluetooth_hfclient_connect(CHAR *bt_addr);
INT32 bluetooth_hfclient_disconnect(CHAR *bt_addr);
INT32 bluetooth_hfclient_connect_audio(CHAR *bt_addr);
INT32 bluetooth_hfclient_disconnect_audio(CHAR *bt_addr);
INT32 bluetooth_hfclient_start_voice_recognition(VOID);
INT32 bluetooth_hfclient_stop_voice_recognition(VOID);
INT32 bluetooth_hfclient_volume_control(BT_HFCLIENT_VOLUME_TYPE_T type, INT32 volume);
INT32 bluetooth_hfclient_dial(const CHAR *number);
INT32 bluetooth_hfclient_dial_memory(INT32 location);
INT32 bluetooth_hfclient_handle_call_action(BT_HFCLIENT_CALL_ACTION_T action, INT32 idx);
INT32 bluetooth_hfclient_query_current_calls(VOID);
INT32 bluetooth_hfclient_query_current_operator_name(VOID);
INT32 bluetooth_hfclient_retrieve_subscriber_info(VOID);
INT32 bluetooth_hfclient_send_dtmf(CHAR code);
INT32 bluetooth_hfclient_request_last_voice_tag_number(VOID);
INT32 bluetooth_hfclient_send_at_cmd(INT32 cmd, INT32 val1, INT32 val2, const CHAR *arg);
VOID bluetooth_hfclient_connection_state_cb(BT_HFCLIENT_CONNECTION_STATE_T state,
                                                      UINT32 peer_feat, UINT32 chld_feat, CHAR *bt_addr);
VOID bluetooth_hfclient_audio_state_cb(BT_HFCLIENT_AUDIO_STATE_T state, CHAR *bt_addr);
VOID bluetooth_hfclient_vr_cmd_cb(BT_HFCLIENT_VR_STATE_T state);
VOID bluetooth_hfclient_network_state_cb(BT_HFCLIENT_NETWORK_STATE_T state);
VOID bluetooth_hfclient_network_roaming_cb(BT_HFCLIENT_SERVICE_TYPE_T type);
VOID bluetooth_hfclient_network_signal_cb(int signal_strength);
VOID bluetooth_hfclient_battery_level_cb(int battery_level);
VOID bluetooth_hfclient_current_operator_cb(const CHAR *name);
VOID bluetooth_hfclient_call_cb(BT_HFCLIENT_CALL_T call);
VOID bluetooth_hfclient_callsetup_cb(BT_HFCLIENT_CALLSETUP_T callsetup);
VOID bluetooth_hfclient_callheld_cb(BT_HFCLIENT_CALLHELD_T callheld);
VOID bluetooth_hfclient_resp_and_hold_cb(BT_HFCLIENT_RESP_AND_HOLD_T resp_and_hold);
VOID bluetooth_hfclient_clip_cb(const CHAR *number);
VOID bluetooth_hfclient_call_waiting_cb(const CHAR *number);
VOID bluetooth_hfclient_current_calls_cb(int index, BT_HFCLIENT_CALL_DIRECTION_T dir, BT_HFCLIENT_CALL_STATE_T state,
                                                 BT_HFCLIENT_CALL_MPTY_TYPE_T mpty, const CHAR *number);
VOID bluetooth_hfclient_volume_change_cb(BT_HFCLIENT_VOLUME_TYPE_T type, int volume);
VOID bluetooth_hfclient_cmd_complete_cb(BT_HFCLIENT_CMD_COMPLETE_TYPE_T type, int cme);
VOID bluetooth_hfclient_subscriber_info_cb(const CHAR *number, BT_HFCLIENT_SUBSCRIBER_SERVICE_TYPE_T type);
VOID bluetooth_hfclient_in_band_ring_tone_cb(BT_HFCLIENT_IN_BAND_RING_STATE_T state);
VOID bluetooth_hfclient_last_voice_tag_number_cb(const CHAR *number);
VOID bluetooth_hfclient_ring_indication_cb(VOID);

#endif /* _BT_MW_HFCLIENT_H_ */

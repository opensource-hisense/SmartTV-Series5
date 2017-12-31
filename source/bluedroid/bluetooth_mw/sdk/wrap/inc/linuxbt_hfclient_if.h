#ifndef __LINUXBT_HFCLIENT_IF_H__
#define __LINUXBT_HFCLIENT_IF_H__

#include "bluetooth.h"
#include "bt_hf_client.h"

int linuxbt_hfclient_init(void);
int linuxbt_hfclient_deinit(void);
int linuxbt_hfclient_connect(char *bt_addr);
int linuxbt_hfclient_connect(char *bt_addr);
int linuxbt_hfclient_disconnect(char *bt_addr);
int linuxbt_hfclient_connect_audio(char *bt_addr);
int linuxbt_hfclient_disconnect_audio(char *bt_addr);
int linuxbt_hfclient_start_voice_recognition(void);
int linuxbt_hfclient_stop_voice_recognition(void);
int linuxbt_hfclient_volume_control(bthf_client_volume_type_t type, int volume);
int linuxbt_hfclient_dial(const char *number);
int linuxbt_hfclient_dial_memory(int location);
int linuxbt_hfclient_handle_call_action(bthf_client_call_action_t action, int idx);
int linuxbt_hfclient_query_current_calls(void);
int linuxbt_hfclient_query_current_operator_name(void);
int linuxbt_hfclient_retrieve_subscriber_info(void);
int linuxbt_hfclient_send_dtmf(char code);
int linuxbt_hfclient_request_last_voice_tag_number(void);
int linuxbt_hfclient_send_at_cmd(int cmd, int val1, int val2, const char *arg);

#endif /* __LINUXBT_HFCLIENT_IF_H__ */

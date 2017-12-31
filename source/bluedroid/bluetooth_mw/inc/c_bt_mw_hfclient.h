#ifndef _C_BT_MW_HFCLIENT_H_
#define _C_BT_MW_HFCLIENT_H_

/* INCLUDE FILE DECLARATIONS
 */


/* NAMING CONSTANT DECLARATIONS
 */

/* MACRO FUNCTION DECLARATIONS
 */


/* DATA TYPE DECLARATIONS
 */
extern INT32 c_btm_bt_hfclient_connect(CHAR *bt_addr);
extern INT32 c_btm_bt_hfclient_disconnect(CHAR *bt_addr);
extern INT32 c_btm_bt_hfclient_connect_audio(CHAR *bt_addr);
extern INT32 c_btm_bt_hfclient_disconnect_audio(CHAR *bt_addr);
extern INT32 c_btm_bt_hfclient_start_voice_recognition(VOID);
extern INT32 c_btm_bt_hfclient_stop_voice_recognition(VOID);
extern INT32 c_btm_bt_hfclient_volume_control(BT_HFCLIENT_VOLUME_TYPE_T type, INT32 volume);
extern INT32 c_btm_bt_hfclient_dial(const CHAR *number);
extern INT32 c_btm_bt_hfclient_dial_memory(INT32 location);
extern INT32 c_btm_bt_hfclient_handle_call_action(BT_HFCLIENT_CALL_ACTION_T action, INT32 idx);
extern INT32 c_btm_bt_hfclient_query_current_calls(VOID);
extern INT32 c_btm_bt_hfclient_query_current_operator_name(VOID);
extern INT32 c_btm_bt_hfclient_retrieve_subscriber_info(VOID);
extern INT32 c_btm_bt_hfclient_send_dtmf(CHAR code);
extern INT32 c_btm_bt_hfclient_request_last_voice_tag_number(VOID);
extern INT32 c_btm_bt_hfclient_send_at_cmd(INT32 cmd, INT32 val1, INT32 val2, const CHAR *arg);

#endif /*  _C_BT_MW_HFCLIENT_H_  */


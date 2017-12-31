#ifndef __BT_MW_AVRCP_H__
#define __BT_MW_AVRCP_H__
#include "u_bluetooth.h"

INT32 bluetooth_avrcp_init(VOID);
INT32 bluetooth_avrcp_deinit(VOID);
INT32 bluetooth_avrcp_disconnect(CHAR *addr);
INT32 bluetooth_avrcp_disconnect_sync(VOID);
INT32 bluetooth_avrcp_connect(CHAR *addr);
INT32 bluetooth_avrcp_connect_sync(CHAR *addr);
INT32 bluetooth_avrcp_send_cmd(BTCmdType bt_cmd_type, BTN_ACTION button_action);

INT32 bluetooth_reset_playstatus(VOID);

INT32 bluetooth_avrcp_get_mediaInfo(avrcp_medioInfo_t *pMediaInfo);
INT32 bluetooth_avrcp_get_playstatus(bt_player_status_values_t *pPlayStatus);
INT32 bluetooth_avrcp_send_get_playstatus_cmd(VOID);
INT32 bluetooth_avrcp_register_notification(CHAR *pNotificationStr);
INT32 bluetooth_avrcp_get_capabilities(avrcp_apability_type_t type);
VOID bluetooth_avrcp_handle_connect_cb(CHAR *pbt_addr);
VOID bluetooth_avrcp_handle_disconnect_cb(CHAR *pbt_addr);
VOID bluetooth_avrcp_get_playstatus_cbk(bt_player_status_values_t *player_status);
VOID bluetooth_avrcp_playstatus_change_cbk(bt_avrcp_playstatus_t avrcp_player_status);
VOID bluetooth_avrcp_track_change_cbk(UINT32 bt_track_index);
VOID bluetooth_avrcp_get_mediaInfo_cbk(avrcp_medioInfo_t *media_info);
VOID bluetooth_rc_tg_set_absolute_volume_ind_cb (UINT8 u1_volume);
VOID bluetooth_rc_tg_register_notification_cb(UINT8 *pu1_volume);
INT32 bluetooth_avrcp_set_absolute_volume(UINT8 u1volume);

/*For PTS test*/
INT32 btaudio_avrcp_set_trackstatus(char* pTrackStatus);
INT32 bluetooth_rc_tg_send_volume_change_rsp_handler(UINT8 u1_volume);
INT32 bluetooth_rc_tg_send_track_change_rsp_handler(UINT8 u1track_selected);
INT32 bluetooth_rc_tg_set_large_element_attribute_response_handler(UINT8 u1enable);
INT32 bluetooth_rc_tg_send_pts_test_handler(UINT8 fgenable);
INT32 bluetooth_rc_set_avrcp_delay(INT32 delay);



#endif /*__BT_MW_AVRCP_H__*/


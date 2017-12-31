#ifndef __LINUXBT_AVRCP_TG_IF_H__
#define __LINUXBT_AVRCP_TG_IF_H__


int linuxbt_rc_tg_init(void);
int linuxbt_rc_tg_deinit(void);


int linuxbt_rc_tg_connect_handler(char *pbt_addr);
int linuxbt_rc_tg_disconnect_handler(char *pbt_addr);
int linuxbt_rc_tg_register_notification_test_mode_handler(int enable);
int linuxbt_rc_tg_send_volume_change_rsp_handler(uint8_t u1interim, uint8_t u1volume);
int linuxbt_rc_tg_send_track_change_rsp_handler(uint8_t u1track_selected);
int linuxbt_rc_tg_set_large_element_attribute_response_handler(uint8_t u1enable);
int linuxbt_rc_tg_send_pts_test_handler(int enable);
int linuxbt_rc_set_avrcp_delay(int delay);

#endif /* __LINUXBT_AVRCP_TG_IF_H__ */

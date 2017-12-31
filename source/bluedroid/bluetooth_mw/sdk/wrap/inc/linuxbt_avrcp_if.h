#ifndef __LINUXBT_AVRCP_IF_H__
#define __LINUXBT_AVRCP_IF_H__

#if defined(MTK_LINUX_AVRCP_PLUS) && (MTK_LINUX_AVRCP_PLUS == TRUE)
#include "mtk_bt_rc.h"
#else
#include "bt_rc.h"
#endif

#define AVRCP_CT_MAX_ATTR_NO 7

int linuxbt_rc_init(void);
int linuxbt_rc_deinit(void);
int linuxbt_rc_connect_handler(char *pbt_addr);
int linuxbt_rc_disconnect_handler(char *pbt_addr);

int linuxbt_rc_send_cmd_handler(BTCmdType bt_cmd_type, BTN_ACTION button_action);
int linuxbt_rc_send_get_playstatus_cmd_handler(VOID);
//int linuxbt_rc_send_get_capabilities_handler(btrc_capability_id_t capability_id);
int linuxbt_rc_send_volume_change_handler(void);
void linuxbt_get_player_status(bt_player_status_values_t *pt_player_status);
void linuxbt_get_media_info(avrcp_medioInfo_t *pt_media_info);
int linuxbt_reset_playstatus(void);
int linuxbt_rc_send_reg_notification_handler(char *pNotificationStr);

int linuxbt_rc_send_volume_change_rsp_handler(uint8_t u1volume);


#endif /* __LINUXBT_AVRCP_IF_H__ */

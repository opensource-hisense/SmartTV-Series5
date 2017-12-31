#ifndef __BTMW_TEST_AVRCP_IF_H__
#define __BTMW_TEST_AVRCP_IF_H__

#include "bluetooth.h"
#if defined(MTK_LINUX_AVRCP_PLUS) && (MTK_LINUX_AVRCP_PLUS == TRUE)
#include "mtk_bt_rc.h"
#else
#include "bt_rc.h"
#endif
#include "btmw_test_cli.h"
#include "btmw_test_debug.h"

#define BTMW_TEST_CMD_KEY_AVRCP_CT     "MW_AVRCP_CT"
#define AVRCP_CT_MAX_ATTR_NO 7

int btmw_test_rc_init(void);
int btmw_test_rc_deinit(void);

void btmw_test_rc_passthrough_rsp_cb(int id, int key_state);
void btmw_test_rc_groupnavigation_rsp_cb(int id, int key_state);
void btmw_test_rc_connection_state_cb(bool state, bt_bdaddr_t *bd_addr);
void btmw_test_rc_getrcfeatures_cb(bt_bdaddr_t *bd_addr, int features);
void btmw_test_rc_setplayerappsetting_rsp_cb(bt_bdaddr_t *bd_addr, uint8_t accepted);
void btmw_test_rc_playerapplicationsetting_cb(bt_bdaddr_t *bd_addr,
                                                                 uint8_t num_attr,
                                                                 btrc_player_app_attr_t *app_attrs,
                                                                 uint8_t num_ext_attr,
                                                                 btrc_player_app_ext_attr_t *ext_attrs);
void btmw_test_rc_playerapplicationsetting_changed_cb(bt_bdaddr_t *bd_addr,
                                                                          btrc_player_settings_t *p_vals);
void btmw_test_rc_setabsvol_cmd_cb(bt_bdaddr_t *bd_addr, uint8_t abs_vol, uint8_t label);
void btmw_test_rc_registernotification_absvol_cb(bt_bdaddr_t *bd_addr, uint8_t label);
void btmw_test_rc_track_changed_cb(bt_bdaddr_t *bd_addr, uint8_t num_attr,
                                                     btrc_element_attr_val_t *p_attrs);
void btmw_test_rc_play_position_changed_cb(bt_bdaddr_t *bd_addr,
                                                              uint32_t song_len, uint32_t song_pos);
void btmw_test_rc_play_status_changed_cb(bt_bdaddr_t *bd_addr,
                                                            btrc_play_status_t play_status);

extern void *btmw_test_gap_get_profile_interface(const char *profile_id);


#define AVRCP_STATE_PRESSED 0
#define AVRCP_STATE_RELEASED 1

#define AVRCP_POP_POWER             0x40
#define AVRCP_POP_VOLUME_UP         0x41
#define AVRCP_POP_VOLUME_DOWN       0x42
#define AVRCP_POP_MUTE              0x43
#define AVRCP_POP_PLAY              0x44
#define AVRCP_POP_STOP              0x45
#define AVRCP_POP_PAUSE             0x46
#define AVRCP_POP_RECORD            0x47
#define AVRCP_POP_REWIND            0x48
#define AVRCP_POP_FAST_FORWARD      0x49
#define AVRCP_POP_EJECT             0x4A
#define AVRCP_POP_FORWARD           0x4B
#define AVRCP_POP_BACKWARD          0x4C
#define AVRCP_POP_NEXT_GROUP        0x0000
#define AVRCP_POP_PREV_GROUP        0x0001

#define AVRCP_CHARSET_UTF8          0x006A

#endif /* __BTUT_AVRCP_IF_H__ */

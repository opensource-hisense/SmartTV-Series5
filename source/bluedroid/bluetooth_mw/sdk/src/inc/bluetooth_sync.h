
#ifndef _BLUETOOTH_SYNC_H_
#define _BLUETOOTH_SYNC_H_

INT32 i4bluetooth_sync_var_init(VOID);

VOID set_bt_mode(BT_MODE_TYPE ps_mode);
BT_MODE_TYPE get_bt_mode(VOID);
VOID set_bt_power_on(BOOL fg_connect);
BOOL get_bt_power_on(VOID);
VOID set_bt_is_call_power_off(BOOL fg_done);
BOOL get_bt_is_call_power_off(VOID);
VOID set_bt_stream_closed(BOOL fg_connect);
BOOL get_bt_stream_closed(VOID);
VOID set_bt_set_scanmode_done(BOOL fg_done);
BOOL get_bt_set_scanmode_done(VOID);
VOID set_bt_get_dev_info_done(BOOL fg_done);
BOOL get_bt_get_dev_info_done(VOID);
VOID set_bt_avrcp_connect(BOOL fg_done);
BOOL get_bt_avrcp_connect(VOID);
VOID set_bt_a2dp_connect(BOOL fg_done);
BOOL get_bt_a2dp_connect(VOID);
VOID set_bt_a2dp_disconnecting(BOOL fg_done);
BOOL get_bt_a2dp_disconnecting(VOID);
VOID set_bt_stream_suspend(BOOL fg_done);
BOOL get_bt_stream_suspend(VOID);
VOID set_bt_connect(BOOL fg_done);
BOOL get_bt_connect(VOID);
VOID set_bt_2nd_connect_comming(BOOL fg_done);
BOOL get_bt_2nd_connect_comming(VOID);
VOID set_bt_avrcp_support(BOOL fg_done);
BOOL get_bt_avrcp_support(VOID);
VOID set_bt_avrcp_absolute_volume_support(BOOL fg_done);
BOOL get_bt_avrcp_absolute_volume_support(VOID);
VOID set_bt_a2dp_support(BOOL fg_done);
BOOL get_bt_a2dp_support(VOID);
VOID set_bt_get_rssi_done(BOOL fg_done);
BOOL get_bt_get_rssi_done(VOID);
VOID set_bt_pts_enable(BOOL fg_done);
BOOL get_bt_pts_enable(VOID);
VOID set_bt_spp_connect(BOOL fg_done);
BOOL get_bt_spp_connect(VOID);
VOID set_bt_a2dp_be_sink(BOOL fg_sink);
BOOL get_bt_a2dp_be_sink(VOID);
VOID set_bt_a2dp_rx_mode(BOOL fg_tx);
BOOL get_bt_a2dp_rx_mode(VOID);
VOID set_bt_a2dp_pause(BOOL fg_done);
BOOL get_bt_a2dp_pause(VOID);
VOID set_bt_a2dp_pause_done(BOOL fg_done);
BOOL get_bt_a2dp_pause_done(VOID);
VOID set_bt_keep_connection(BOOL fg_done);
BOOL get_bt_keep_connection(VOID);
VOID set_bt_discard_data(BOOL fg_done);
BOOL get_bt_discard_data(VOID);
VOID set_bt_enabled_codec_type(BT_AUDIO_CODEC_TYPE codec_type, BOOL fg_enable);
UINT8 get_bt_enabled_codec_type(VOID);
VOID set_bt_enabled_a2dp_role(BOOL fg_src, BOOL fg_enable);
UINT8 get_bt_enabled_a2dp_role(VOID);
VOID set_bt_support_scms_t(BOOL fg_done);
BOOL get_bt_support_scms_t(VOID);
VOID set_bt_is_mute(BOOL fg_done);
BOOL get_bt_is_mute(VOID);
VOID set_bt_send_zero_data(BOOL fg_done);
BOOL get_bt_send_zero_data(VOID);
VOID set_bt_hid_support(BOOL fg_done);
BOOL get_bt_hid_support(VOID);

#endif /* _BLUETOOTH_SYNC_H_# */

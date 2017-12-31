#ifndef __LINUXBT_A2DP_IF_H__
#define __LINUXBT_A2DP_IF_H__


INT32 linuxbt_a2dp_set_audio_hw_log_lvl(UINT8 log_level);
VOID init_audio_path(VOID);
VOID uninit_audio_path(VOID);
VOID linuxbt_a2dp_src_handle_connected_event(VOID);
VOID linuxbt_a2dp_src_handle_disconnected_event(VOID);
VOID linuxbt_a2dp_sink_handle_connected_event(VOID);
VOID linuxbt_a2dp_sink_handle_disconnected_event(VOID);

INT32 linuxbt_a2dp_sink_init(VOID);
INT32 linuxbt_a2dp_src_init(VOID);
VOID linuxbt_a2dp_src_deinit(VOID);
VOID linuxbt_a2dp_sink_deinit(VOID);
INT32 linuxbt_a2dp_connect_int_handler(CHAR *pbt_addr, UINT8 local_role);
INT32 linuxbt_a2dp_disconnect_handler(CHAR *pbt_addr, UINT8 local_role);

INT32 linuxbt_a2dp_src_enable_handler(UINT8 src_enable);
INT32 linuxbt_a2dp_sink_enable_handler(UINT8 sink_enable);
INT32 linuxbt_a2dp_aac_enable_handler(UINT8 fg_enable);
INT32 linuxbt_a2dp_ldac_enable_handler(UINT8 fg_enable);
INT32 linuxbt_a2dp_aptx_enable_handler(UINT8 fg_enable);
INT32 linuxbt_a2dp_2nd_con_enable_handler(UINT8 fg_enable);
INT32 linuxbt_a2dp_start_handler(CHAR *pbt_addr);
INT32 linuxbt_a2dp_suspend_handler(CHAR *pbt_addr);
INT32 linuxbt_a2dp_abort_handler(CHAR *pbt_addr);
INT32 linuxbt_a2dp_reconfig_handler(CHAR *pbt_addr,
                                INT32 sample_rate,
                                INT32 channel_num);
INT32 linuxbt_send_stream_data(const CHAR *data, INT32 len);
INT32 linuxbt_a2dp_get_sample_rate(VOID);
INT32 linuxbt_a2dp_get_channel_num(VOID);
INT32 linuxbt_a2dp_get_status( INT32* sample_rate,
                               INT32* channel_num,
                               INT32* prefer_size,
                               INT32* bitpool,
                               INT32* allocation_method,
                               INT32* block_len,
                               INT32* sub_bands);
INT32 linuxbt_a2dp_get_prefer_size(VOID);

#endif /* __LINUXBT_A2DP_IF_H__ */


#ifndef _BT_DBG_CONFIG_A2DP_H_
#define _BT_DBG_CONFIG_A2DP_H_

#if defined(BT_RPC_DBG_SERVER) || defined(BT_RPC_DBG_CLIENT)
enum
{
    A2DP_CMD_START = DBG_A2DP << MOD_LOCATION & 0xFF000000,
    A2DP_CMD_TARGET_DEV_INFO,       // g_bt_target_dev_info
    A2DP_CMD_LAST_DEVICE_ADDR,      // bt_last_device_addr
    A2DP_CMD_2ND_DEVICE_ADDR,       // bt_2nd_device_addr
    A2DP_CMD_SCMS_T_VALUE,          // g_scms_t_value
    A2DP_CMD_SAMPLERATE,            // gi4SampleRate
    A2DP_CMD_CHANNELCNT,            // gi4ChannelCnt
    A2DP_CMD_LOCAL_ROLE,            // g_local_role
    A2DP_CMD_CURRENT_CODEC_TYPE,    // g_current_codec_type

    A2DP_CMD_END
};

#if defined(BT_RPC_DBG_SERVER)
extern int dbg_a2dp_get_g_bt_target_dev_info(int array_index, int offset, char *name, char *data, int length);
extern int dbg_a2dp_get_bt_last_device_addr(int array_index, int offset, char *name, char *data, int length);
extern int dbg_a2dp_get_bt_2nd_device_addr(int array_index, int offset, char *name, char *data, int length);
extern int dbg_a2dp_get_g_scms_t_value(int array_index, int offset, char *name, char *data, int length);
extern int dbg_a2dp_get_gi4SampleRate(int array_index, int offset, char *name, char *data, int length);
extern int dbg_a2dp_get_gi4ChannelCnt(int array_index, int offset, char *name, char *data, int length);
extern int dbg_a2dp_get_g_local_role(int array_index, int offset, char *name, char *data, int length);
extern int dbg_a2dp_get_g_current_codec_type(int array_index, int offset, char *name, char *data, int length);

#endif

#if defined(BT_RPC_DBG_SERVER)
#define A2DP_DATA_TARGET_DEV_INFO       (dbg_a2dp_get_g_bt_target_dev_info)
#define A2DP_DATA_LAST_DEVICE_ADDR      (dbg_a2dp_get_bt_last_device_addr)
#define A2DP_DATA_2ND_DEVICE_ADDR       (dbg_a2dp_get_bt_2nd_device_addr)
#define A2DP_DATA_SCMS_T_VALUE          (dbg_a2dp_get_g_scms_t_value)
#define A2DP_DATA_SAMPLERATE            (dbg_a2dp_get_gi4SampleRate)
#define A2DP_DATA_CHANNELCNT            (dbg_a2dp_get_gi4ChannelCnt)
#define A2DP_DATA_LOCAL_ROLE            (dbg_a2dp_get_g_local_role)
#define A2DP_DATA_CURRENT_CODEC_TYPE    (dbg_a2dp_get_g_current_codec_type)


#endif
#if defined(BT_RPC_DBG_CLIENT)
#define A2DP_DATA_TARGET_DEV_INFO       (HEAD"A2DP"TYPE"STR"NAME"g_bt_target_dev_info"TAIL)
#define A2DP_DATA_LAST_DEVICE_ADDR      (HEAD"A2DP"TYPE"VAR"ARRY"18"NAME"bt_last_device_addr"TAIL)
#define A2DP_DATA_2ND_DEVICE_ADDR       (HEAD"A2DP"TYPE"VAR"ARRY"18"NAME"bt_2nd_device_addr"TAIL)
#define A2DP_DATA_SCMS_T_VALUE          (HEAD"A2DP"TYPE"VAR"NAME"g_scms_t_value"TAIL)
#define A2DP_DATA_SAMPLERATE            (HEAD"A2DP"TYPE"VAR"NAME"gi4SampleRate"TAIL)
#define A2DP_DATA_CHANNELCNT            (HEAD"A2DP"TYPE"VAR"NAME"gi4ChannelCnt"TAIL)
#define A2DP_DATA_LOCAL_ROLE            (HEAD"A2DP"TYPE"VAR"NAME"g_local_role"TAIL)
#define A2DP_DATA_CURRENT_CODEC_TYPE    (HEAD"A2DP"TYPE"VAR"NAME"g_current_codec_type"TAIL)


#endif


#define A2DP_CMD_NUM    (A2DP_CMD_END - A2DP_CMD_START)

static SUB_CMD a2dp_cmd[A2DP_CMD_NUM] =
{
    {A2DP_CMD_TARGET_DEV_INFO,      A2DP_DATA_TARGET_DEV_INFO},
    {A2DP_CMD_LAST_DEVICE_ADDR,     A2DP_DATA_LAST_DEVICE_ADDR},
    {A2DP_CMD_2ND_DEVICE_ADDR,      A2DP_DATA_2ND_DEVICE_ADDR},
    {A2DP_CMD_SCMS_T_VALUE,         A2DP_DATA_SCMS_T_VALUE},
    {A2DP_CMD_SAMPLERATE,           A2DP_DATA_SAMPLERATE},
    {A2DP_CMD_CHANNELCNT,           A2DP_DATA_CHANNELCNT},
    {A2DP_CMD_LOCAL_ROLE,           A2DP_DATA_LOCAL_ROLE},
    {A2DP_CMD_CURRENT_CODEC_TYPE,   A2DP_DATA_CURRENT_CODEC_TYPE},

    {0,   NULL}
};
#endif

#endif


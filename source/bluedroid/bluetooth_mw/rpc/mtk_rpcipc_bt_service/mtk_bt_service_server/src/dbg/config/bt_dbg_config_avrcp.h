
#ifndef _BT_DBG_CONFIG_AVRCP_H_
#define _BT_DBG_CONFIG_AVRCP_H_

#if defined(BT_RPC_DBG_SERVER) || defined(BT_RPC_DBG_CLIENT)
enum
{
    AVRCP_CMD_START = DBG_AVRCP << MOD_LOCATION & 0xFF000000,
    AVRCP_CMD_MEDIA_INFO,   // g_media_info
    AVRCP_CMD_PLAYER_STATUS,  // g_player_status

    AVRCP_CMD_END
};

#if defined(BT_RPC_DBG_SERVER)
extern int dbg_avrcp_get_g_media_info(int array_index, int offset, char *name, char *data, int length);
extern int dbg_avrcp_get_g_player_status(int array_index, int offset, char *name, char *data, int length);

#endif

#if defined(BT_RPC_DBG_SERVER)
#define AVRCP_DATA_MEDIA_INFO    (dbg_avrcp_get_g_media_info)
#define AVRCP_DATA_PLAYER_STATUS (dbg_avrcp_get_g_player_status)

#endif
#if defined(BT_RPC_DBG_CLIENT)
#define AVRCP_DATA_MEDIA_INFO     (HEAD"AVRCP"TYPE"STR"NAME"g_media_info"TAIL)
#define AVRCP_DATA_PLAYER_STATUS  (HEAD"AVRCP"TYPE"STR"NAME"g_player_status"TAIL)

#endif

#define AVRCP_CMD_NUM   (AVRCP_CMD_END - AVRCP_CMD_START)

static SUB_CMD avrcp_cmd[AVRCP_CMD_NUM] =
{
    {AVRCP_CMD_MEDIA_INFO,      AVRCP_DATA_MEDIA_INFO},
    {AVRCP_CMD_PLAYER_STATUS,   AVRCP_DATA_PLAYER_STATUS},

    {0,   NULL}
};
#endif

#endif


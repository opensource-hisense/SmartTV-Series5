
#ifndef _BT_DBG_CONFIG_L2CAP_H_
#define _BT_DBG_CONFIG_L2CAP_H_

#if defined(BT_RPC_DBG_SERVER) || defined(BT_RPC_DBG_CLIENT)
enum
{
    L2CAP_CMD_START = DBG_L2CAP << MOD_LOCATION & 0xFF000000,

    L2CAP_CMD_END
};


#define L2CAP_CMD_NUM   (L2CAP_CMD_END - L2CAP_CMD_START)

static SUB_CMD l2cap_cmd[L2CAP_CMD_NUM] =
{

    {0,   NULL}
};
#endif

#endif


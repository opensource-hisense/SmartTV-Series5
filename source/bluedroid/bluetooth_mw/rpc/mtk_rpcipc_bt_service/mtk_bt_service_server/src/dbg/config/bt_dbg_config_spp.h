
#ifndef _BT_DBG_CONFIG_SPP_H_
#define _BT_DBG_CONFIG_SPP_H_

#if defined(BT_RPC_DBG_SERVER) || defined(BT_RPC_DBG_CLIENT)
enum
{
    SPP_CMD_START = DBG_SPP << MOD_LOCATION & 0xFF000000,

    SPP_CMD_END
};


#define SPP_CMD_NUM     (SPP_CMD_END - SPP_CMD_START)

static SUB_CMD spp_cmd[SPP_CMD_NUM] =
{

    {0,   NULL}
};
#endif

#endif


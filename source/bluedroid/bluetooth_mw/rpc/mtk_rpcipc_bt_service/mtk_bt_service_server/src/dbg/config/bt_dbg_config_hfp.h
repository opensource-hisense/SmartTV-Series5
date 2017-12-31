
#ifndef _BT_DBG_CONFIG_HFP_H_
#define _BT_DBG_CONFIG_HFP_H_

#if defined(BT_RPC_DBG_SERVER) || defined(BT_RPC_DBG_CLIENT)
enum
{
    HFP_CMD_START = DBG_HFP << MOD_LOCATION & 0xFF000000,

    HFP_CMD_END
};


#define HFP_CMD_NUM     (HFP_CMD_END - HFP_CMD_START)

static SUB_CMD hfp_cmd[HFP_CMD_NUM] =
{

    {0,   NULL}
};
#endif

#endif


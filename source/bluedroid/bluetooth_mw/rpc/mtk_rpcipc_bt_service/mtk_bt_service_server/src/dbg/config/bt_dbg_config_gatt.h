
#ifndef _BT_DBG_CONFIG_GATT_H_
#define _BT_DBG_CONFIG_GATT_H_

#if defined(BT_RPC_DBG_SERVER) || defined(BT_RPC_DBG_CLIENT)
enum
{
    GATT_CMD_START = DBG_GATT << MOD_LOCATION & 0xFF000000,

    GATT_CMD_END
};


#define GATT_CMD_NUM    (GATT_CMD_END - GATT_CMD_START)

static SUB_CMD gatt_cmd[GATT_CMD_NUM] =
{

    {0,   NULL}
};
#endif

#endif


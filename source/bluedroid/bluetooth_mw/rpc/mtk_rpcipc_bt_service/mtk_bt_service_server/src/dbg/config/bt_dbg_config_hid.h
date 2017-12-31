
#ifndef _BT_DBG_CONFIG_HID_H_
#define _BT_DBG_CONFIG_HID_H_

#if defined(BT_RPC_DBG_SERVER) || defined(BT_RPC_DBG_CLIENT)
enum
{
    HID_CMD_START = DBG_HID << MOD_LOCATION & 0xFF000000,

    HID_CMD_END
};


#define HID_CMD_NUM     (HID_CMD_END - HID_CMD_START)

static SUB_CMD hid_cmd[HID_CMD_NUM] =
{

    {0,   NULL}
};
#endif

#endif


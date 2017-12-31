
#ifndef _BT_DBG_CONFIG_H_
#define _BT_DBG_CONFIG_H_

#if defined(BT_RPC_DBG_SERVER) || defined(BT_RPC_DBG_CLIENT)
#include "bt_dbg_def.h"
#include "bt_dbg_config_gap.h"
#include "bt_dbg_config_l2cap.h"
#include "bt_dbg_config_gatt.h"
#include "bt_dbg_config_a2dp.h"
#include "bt_dbg_config_hid.h"
#include "bt_dbg_config_avrcp.h"
#include "bt_dbg_config_hfp.h"
#include "bt_dbg_config_spp.h"

#define VERSION "1.0"

static MAIN_CMD m_cmd[DBG_END - DBG_START] =
{
    {"GAP",     DBG_GAP,    gap_cmd,    GAP_CMD_NUM,    "GAP rpc debug"},
    {"L2CAP",   DBG_L2CAP,  l2cap_cmd,  L2CAP_CMD_NUM,  "L2CAP rpc debug"},
    {"GATT",    DBG_GATT,   gatt_cmd,   GATT_CMD_NUM,   "GATT rpc debug"},
    {"A2DP",    DBG_A2DP,   a2dp_cmd,   A2DP_CMD_NUM,   "A2DP rpc debug"},
    {"HID",     DBG_HID,    hid_cmd,    HID_CMD_NUM,    "HID rpc debug"},
    {"AVRCP",   DBG_AVRCP,  avrcp_cmd,  AVRCP_CMD_NUM,  "AVRCP rpc debug"},
    {"HFP",     DBG_HFP,    hfp_cmd,    HFP_CMD_NUM,    "HFP rpc debug"},
    {"SPP",     DBG_SPP,    spp_cmd,    SPP_CMD_NUM,    "SPP rpc debug"},

    {NULL,      0,          NULL,       0,              NULL }
};
#endif

#endif


/* This source file was automatically created by the */
/* tool 'MTK RPC Description tool', 'Version 1.10' on 'Wed Nov 15 16:45:27 2017'. */
/* Do NOT modify this source file. */



/* Start of source pre-amble file 'src_header_file.h'. */

#include "u_bt_mw_hfclient.h"
#include "mtk_bt_service_hfclient_ipcrpc_struct.h"

/* End of source pre-amble file 'src_header_file.h'. */

static const RPC_DESC_T t_rpc_decl_BT_HFCLIENT_CURRENT_CALLS_T;
static const RPC_DESC_T t_rpc_decl_BT_HFCLIENT_VOLUME_CHANGE_T;
static const RPC_DESC_T t_rpc_decl_BT_HFCLIENT_CMD_COMPLETE_T;
static const RPC_DESC_T t_rpc_decl_BT_HFCLIENT_SUBSCRIBER_INFO_T;



static const RPC_DESC_T t_rpc_decl_BT_HFCLIENT_CURRENT_CALLS_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (BT_HFCLIENT_CURRENT_CALLS_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_BT_HFCLIENT_VOLUME_CHANGE_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (BT_HFCLIENT_VOLUME_CHANGE_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_BT_HFCLIENT_CMD_COMPLETE_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (BT_HFCLIENT_CMD_COMPLETE_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_BT_HFCLIENT_SUBSCRIBER_INFO_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (BT_HFCLIENT_SUBSCRIBER_INFO_T),
    .ui4_num_entries = 0
};


static const RPC_DESC_T* at_rpc_desc_list [] =
{
    &t_rpc_decl_BT_HFCLIENT_CURRENT_CALLS_T,
    &t_rpc_decl_BT_HFCLIENT_VOLUME_CHANGE_T,
    &t_rpc_decl_BT_HFCLIENT_CMD_COMPLETE_T,
    &t_rpc_decl_BT_HFCLIENT_SUBSCRIBER_INFO_T
};

EXPORT_SYMBOL const RPC_DESC_T* __rpc_get_hfclient_desc__ (UINT32  ui4_idx)
{
  return ((ui4_idx < 4) ? at_rpc_desc_list [ui4_idx] : NULL);
}



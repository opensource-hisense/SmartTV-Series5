/* This source file was automatically created by the */
/* tool 'MTK RPC Description tool', 'Version 1.10' on 'Wed Nov 15 16:44:47 2017'. */
/* Do NOT modify this source file. */



/* Start of source pre-amble file 'src_header_file.h'. */

#include "mtk_bt_service_avrcp_ipcrpc_struct.h"
#include "u_bt_mw_avrcp.h"


/* End of source pre-amble file 'src_header_file.h'. */

static const RPC_DESC_T t_rpc_decl_avrcp_medioInfo_t;
static const RPC_DESC_T t_rpc_decl_bt_player_status_values_t;
static const RPC_DESC_T t_rpc_decl_bt_player_settings_t;
static const RPC_DESC_T t_rpc_decl_bt_register_notification_t;



static const RPC_DESC_T t_rpc_decl_avrcp_medioInfo_t =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (avrcp_medioInfo_t),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_bt_player_status_values_t =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (bt_player_status_values_t),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_bt_player_settings_t =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (bt_player_settings_t),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_bt_register_notification_t =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (bt_register_notification_t),
    .ui4_num_entries = 0
};


static const RPC_DESC_T* at_rpc_desc_list [] =
{
    &t_rpc_decl_avrcp_medioInfo_t,
    &t_rpc_decl_bt_player_status_values_t,
    &t_rpc_decl_bt_player_settings_t,
    &t_rpc_decl_bt_register_notification_t
};

EXPORT_SYMBOL const RPC_DESC_T* __rpc_get_avrcp_desc__ (UINT32  ui4_idx)
{
  return ((ui4_idx < 4) ? at_rpc_desc_list [ui4_idx] : NULL);
}



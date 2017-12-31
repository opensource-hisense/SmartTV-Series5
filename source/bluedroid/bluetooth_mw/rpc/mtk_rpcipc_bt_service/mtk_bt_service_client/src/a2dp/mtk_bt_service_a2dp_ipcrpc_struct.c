/* This source file was automatically created by the */
/* tool 'MTK RPC Description tool', 'Version 1.10' on 'Wed Nov 15 16:44:42 2017'. */
/* Do NOT modify this source file. */



/* Start of source pre-amble file 'src_header_file.h'. */

#include "mtk_bt_service_a2dp_ipcrpc_struct.h"
#include "u_bluetooth.h"


/* End of source pre-amble file 'src_header_file.h'. */

static const RPC_DESC_T t_rpc_decl_avrcp_medioInfo_t;
static const RPC_DESC_T t_rpc_decl_bt_player_status_values_t;
static const RPC_DESC_T t_rpc_decl_bt_player_settings_t;
static const RPC_DESC_T t_rpc_decl_bt_register_notification_t;
static const RPC_DESC_T t_rpc_decl_BT_HFCLIENT_CURRENT_CALLS_T;
static const RPC_DESC_T t_rpc_decl_BT_HFCLIENT_VOLUME_CHANGE_T;
static const RPC_DESC_T t_rpc_decl_BT_HFCLIENT_CMD_COMPLETE_T;
static const RPC_DESC_T t_rpc_decl_BT_HFCLIENT_SUBSCRIBER_INFO_T;
static const RPC_DESC_T t_rpc_decl_BT_SPP_CBK_STRUCT;
static const RPC_DESC_T t_rpc_decl_BT_HID_CBK_STRUCT;
static const RPC_DESC_T t_rpc_decl_BT_HFCLIENT_CBK_STRUCT;
static const RPC_DESC_T t_rpc_decl_BT_HID_STATUS_STRUCT;
static const RPC_DESC_T t_rpc_decl_BT_HID_STATUS_STRUCT_LIST;
static const RPC_DESC_T t_rpc_decl_BT_TARGET_DEV_INFO;
static const RPC_DESC_T t_rpc_decl_BT_TARGET_DEV_LIST;
static const RPC_DESC_T t_rpc_decl_BT_HID_TARGET_DEV_LIST;
static const RPC_DESC_T t_rpc_decl_BT_DEV_MAPPING_INFO;
static const RPC_DESC_T t_rpc_decl_BT_DEV_MAPPING_LIST;
static const RPC_DESC_T t_rpc_decl_BT_DEV_LIST_IN_NAMES;
static const RPC_DESC_T t_rpc_decl_BT_GAP_SCAN_RESULT;
static const RPC_DESC_T t_rpc_decl_BT_GAP_SCAN_RESULT_S;
static const RPC_DESC_T t_rpc_decl_BT_GAP_SDP_RESULT;
static const RPC_DESC_T t_rpc_decl_BT_GAP_LOCAL_PROPERTIES_RESULT;
static const RPC_DESC_T t_rpc_decl_BT_GAP_CBK_PARMS;
static const RPC_DESC_T t_rpc_decl_BT_A2DP_UPLOAD_INFO;
static const RPC_DESC_T t_rpc_decl_pairing_key_value_t;
static const RPC_DESC_T t_rpc_decl_BT_BOND_STATUS_STRUCT;
static const RPC_DESC_T t_rpc_decl_BT_APP_CB_FUNC;
static const RPC_DESC_T t_rpc_decl_BT_AUDIO_SRC_CB_FUNC;
static const RPC_DESC_T t_rpc_decl_BT_PLAYER_SNK_CB_FUNC;



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

static const RPC_DESC_T t_rpc_decl_BT_SPP_CBK_STRUCT =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (BT_SPP_CBK_STRUCT),
    .ui4_num_entries = 2,
    {
        {
            .e_type          = ARG_TYPE_REF_DESC,
            .pt_desc         = NULL,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (BT_SPP_CBK_STRUCT, uuid)
            }
        },
        {
            .e_type          = ARG_TYPE_REF_DESC,
            .pt_desc         = NULL,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (BT_SPP_CBK_STRUCT, spp_data)
            }
        }
    }
};

static const RPC_DESC_T t_rpc_decl_BT_HID_CBK_STRUCT =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (BT_HID_CBK_STRUCT),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_BT_HFCLIENT_CBK_STRUCT =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (BT_HFCLIENT_CBK_STRUCT),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_BT_HID_STATUS_STRUCT =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (BT_HID_STATUS_STRUCT),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_BT_HID_STATUS_STRUCT_LIST =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (BT_HID_STATUS_STRUCT_LIST),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_BT_TARGET_DEV_INFO =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (BT_TARGET_DEV_INFO),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_BT_TARGET_DEV_LIST =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (BT_TARGET_DEV_LIST),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_BT_HID_TARGET_DEV_LIST =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (BT_HID_TARGET_DEV_LIST),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_BT_DEV_MAPPING_INFO =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (BT_DEV_MAPPING_INFO),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_BT_DEV_MAPPING_LIST =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (BT_DEV_MAPPING_LIST),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_BT_DEV_LIST_IN_NAMES =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (BT_DEV_LIST_IN_NAMES),
    .ui4_num_entries = 1,
    {
        {
            .e_type          = ARG_TYPE_REF_DESC,
            .pt_desc         = &t_rpc_decl_BT_TARGET_DEV_INFO,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (BT_DEV_LIST_IN_NAMES, device_list)
            }
        }
    }
};

static const RPC_DESC_T t_rpc_decl_BT_GAP_SCAN_RESULT =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (BT_GAP_SCAN_RESULT),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_BT_GAP_SCAN_RESULT_S =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (BT_GAP_SCAN_RESULT_S),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_BT_GAP_SDP_RESULT =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (BT_GAP_SDP_RESULT),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_BT_GAP_LOCAL_PROPERTIES_RESULT =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (BT_GAP_LOCAL_PROPERTIES_RESULT),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_BT_GAP_CBK_PARMS =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (BT_GAP_CBK_PARMS),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_BT_A2DP_UPLOAD_INFO =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (BT_A2DP_UPLOAD_INFO),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_pairing_key_value_t =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (pairing_key_value_t),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_BT_BOND_STATUS_STRUCT =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (BT_BOND_STATUS_STRUCT),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_BT_APP_CB_FUNC =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (BT_APP_CB_FUNC),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_BT_AUDIO_SRC_CB_FUNC =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (BT_AUDIO_SRC_CB_FUNC),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_BT_PLAYER_SNK_CB_FUNC =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (BT_PLAYER_SNK_CB_FUNC),
    .ui4_num_entries = 0
};


static const RPC_DESC_T* at_rpc_desc_list [] =
{
    &t_rpc_decl_avrcp_medioInfo_t,
    &t_rpc_decl_bt_player_status_values_t,
    &t_rpc_decl_bt_player_settings_t,
    &t_rpc_decl_bt_register_notification_t,
    &t_rpc_decl_BT_HFCLIENT_CURRENT_CALLS_T,
    &t_rpc_decl_BT_HFCLIENT_VOLUME_CHANGE_T,
    &t_rpc_decl_BT_HFCLIENT_CMD_COMPLETE_T,
    &t_rpc_decl_BT_HFCLIENT_SUBSCRIBER_INFO_T,
    &t_rpc_decl_BT_SPP_CBK_STRUCT,
    &t_rpc_decl_BT_HID_CBK_STRUCT,
    &t_rpc_decl_BT_HFCLIENT_CBK_STRUCT,
    &t_rpc_decl_BT_HID_STATUS_STRUCT,
    &t_rpc_decl_BT_HID_STATUS_STRUCT_LIST,
    &t_rpc_decl_BT_TARGET_DEV_INFO,
    &t_rpc_decl_BT_TARGET_DEV_LIST,
    &t_rpc_decl_BT_HID_TARGET_DEV_LIST,
    &t_rpc_decl_BT_DEV_MAPPING_INFO,
    &t_rpc_decl_BT_DEV_MAPPING_LIST,
    &t_rpc_decl_BT_DEV_LIST_IN_NAMES,
    &t_rpc_decl_BT_GAP_SCAN_RESULT,
    &t_rpc_decl_BT_GAP_SCAN_RESULT_S,
    &t_rpc_decl_BT_GAP_SDP_RESULT,
    &t_rpc_decl_BT_GAP_LOCAL_PROPERTIES_RESULT,
    &t_rpc_decl_BT_GAP_CBK_PARMS,
    &t_rpc_decl_BT_A2DP_UPLOAD_INFO,
    &t_rpc_decl_pairing_key_value_t,
    &t_rpc_decl_BT_BOND_STATUS_STRUCT,
    &t_rpc_decl_BT_APP_CB_FUNC,
    &t_rpc_decl_BT_AUDIO_SRC_CB_FUNC,
    &t_rpc_decl_BT_PLAYER_SNK_CB_FUNC
};

EXPORT_SYMBOL const RPC_DESC_T* __rpc_get_a2dp_desc__ (UINT32  ui4_idx)
{
  return ((ui4_idx < 30) ? at_rpc_desc_list [ui4_idx] : NULL);
}



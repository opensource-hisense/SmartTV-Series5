/* This source file was automatically created by the */
/* tool 'MTK RPC Description tool', 'Version 1.10' on 'Wed Nov 15 16:45:21 2017'. */
/* Do NOT modify this source file. */



/* Start of source pre-amble file 'src_header_file.h'. */

#include "u_bt_mw_gatt.h"
#include "mtk_bt_service_gattc_wrapper.h"
#include "mtk_bt_service_gatts_wrapper.h"
#include "mtk_bt_service_gatt_ipcrpc_struct.h"



/* End of source pre-amble file 'src_header_file.h'. */

static const RPC_DESC_T t_rpc_decl_BT_GATT_ID_T;
static const RPC_DESC_T t_rpc_decl_BT_GATTS_SRVC_ID_T;
static const RPC_DESC_T t_rpc_decl_BT_GATTC_REG_CLIENT_T;
static const RPC_DESC_T t_rpc_decl_BT_GATTC_SCAN_RST_T;
static const RPC_DESC_T t_rpc_decl_BT_GATTC_CONNECT_RST_T;
static const RPC_DESC_T t_rpc_decl_BT_GATTC_SEARCH_RST_T;
static const RPC_DESC_T t_rpc_decl_BT_GATTC_GET_CHAR_RST_T;
static const RPC_DESC_T t_rpc_decl_BT_GATTC_GET_DESCR_RST_T;
static const RPC_DESC_T t_rpc_decl_BT_GATTC_GET_INCL_RST_T;
static const RPC_DESC_T t_rpc_decl_BT_GATTC_GET_REG_NOTI_RST_T;
static const RPC_DESC_T t_rpc_decl_BT_GATT_NOTI_PARAMS_T;
static const RPC_DESC_T t_rpc_decl_BT_GATTC_GET_NOTIFY_T;
static const RPC_DESC_T t_rpc_decl_BT_GATT_UNFORMATTED_VALUE_T;
static const RPC_DESC_T t_rpc_decl_BT_GATT_READ_PARAMS_T;
static const RPC_DESC_T t_rpc_decl_BT_GATTC_READ_CHAR_RST_T;
static const RPC_DESC_T t_rpc_decl_BT_GATTC_WRITE_CHAR_RST_T;
static const RPC_DESC_T t_rpc_decl_BT_GATTC_READ_DESCR_RST_T;
static const RPC_DESC_T t_rpc_decl_BT_GATTC_WRITE_DESCR_RST_T;
static const RPC_DESC_T t_rpc_decl_BT_GATTC_GET_REMOTE_RSSI_T;
static const RPC_DESC_T t_rpc_decl_bluetooth_gatt_scan_filter_param_t;
static const RPC_DESC_T t_rpc_decl_bluetooth_gatt_scan_filter_status_t;
static const RPC_DESC_T t_rpc_decl_bluetooth_gatt_scan_filter_cfg_t;
static const RPC_DESC_T t_rpc_decl_bluetooth_gatt_filt_param_setup_t;
static const RPC_DESC_T t_rpc_decl_bluetooth_gatt_db_element_t;
static const RPC_DESC_T t_rpc_decl_BT_GATTC_GET_GATT_DB_T;
static const RPC_DESC_T t_rpc_decl_BT_GATTC_TEST_PARAMS_T;
static const RPC_DESC_T t_rpc_decl_BT_APP_GATTC_CB_FUNC_T;
static const RPC_DESC_T t_rpc_decl_BT_GATTS_REG_SERVER_RST_T;
static const RPC_DESC_T t_rpc_decl_BT_GATTS_CONNECT_RST_T;
static const RPC_DESC_T t_rpc_decl_BT_GATTS_ADD_SRVC_RST_T;
static const RPC_DESC_T t_rpc_decl_BT_GATTS_ADD_INCL_RST_T;
static const RPC_DESC_T t_rpc_decl_BT_GATTS_ADD_CHAR_RST_T;
static const RPC_DESC_T t_rpc_decl_BT_GATTS_ADD_DESCR_RST_T;
static const RPC_DESC_T t_rpc_decl_BT_GATTS_SRVC_RST_T;
static const RPC_DESC_T t_rpc_decl_BT_GATTS_REQ_READ_RST_T;
static const RPC_DESC_T t_rpc_decl_BT_GATTS_REQ_WRITE_RST_T;
static const RPC_DESC_T t_rpc_decl_BT_GATTS_EXEC_WRITE_RST_T;
static const RPC_DESC_T t_rpc_decl_BT_APP_GATTS_CB_FUNC_T;
static const RPC_DESC_T t_rpc_decl_c4tv_setup_info_adv_t;
static const RPC_DESC_T t_rpc_decl_c4tv_setup_info_scan_t;
static const RPC_DESC_T t_rpc_decl_c4tv_setup_info_attr_t;
static const RPC_DESC_T t_rpc_decl_c4tv_setup_info_t_u;
static const RPC_DESC_T t_rpc_decl_c4tv_setup_info_t;
static const RPC_DESC_T t_rpc_decl_c4tv_uuid_handle_t;
static const RPC_DESC_T t_rpc_decl_MTKRPCAPI_BT_APP_GATTC_CB_FUNC_T;
static const RPC_DESC_T t_rpc_decl_MTKRPCAPI_BT_APP_GATTS_CB_FUNC_T;



static const RPC_DESC_T t_rpc_decl_BT_GATT_ID_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (BT_GATT_ID_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_BT_GATTS_SRVC_ID_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (BT_GATTS_SRVC_ID_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_BT_GATTC_REG_CLIENT_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (BT_GATTC_REG_CLIENT_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_BT_GATTC_SCAN_RST_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (BT_GATTC_SCAN_RST_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_BT_GATTC_CONNECT_RST_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (BT_GATTC_CONNECT_RST_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_BT_GATTC_SEARCH_RST_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (BT_GATTC_SEARCH_RST_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_BT_GATTC_GET_CHAR_RST_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (BT_GATTC_GET_CHAR_RST_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_BT_GATTC_GET_DESCR_RST_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (BT_GATTC_GET_DESCR_RST_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_BT_GATTC_GET_INCL_RST_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (BT_GATTC_GET_INCL_RST_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_BT_GATTC_GET_REG_NOTI_RST_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (BT_GATTC_GET_REG_NOTI_RST_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_BT_GATT_NOTI_PARAMS_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (BT_GATT_NOTI_PARAMS_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_BT_GATTC_GET_NOTIFY_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (BT_GATTC_GET_NOTIFY_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_BT_GATT_UNFORMATTED_VALUE_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (BT_GATT_UNFORMATTED_VALUE_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_BT_GATT_READ_PARAMS_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (BT_GATT_READ_PARAMS_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_BT_GATTC_READ_CHAR_RST_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (BT_GATTC_READ_CHAR_RST_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_BT_GATTC_WRITE_CHAR_RST_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (BT_GATTC_WRITE_CHAR_RST_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_BT_GATTC_READ_DESCR_RST_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (BT_GATTC_READ_DESCR_RST_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_BT_GATTC_WRITE_DESCR_RST_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (BT_GATTC_WRITE_DESCR_RST_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_BT_GATTC_GET_REMOTE_RSSI_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (BT_GATTC_GET_REMOTE_RSSI_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_bluetooth_gatt_scan_filter_param_t =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (bluetooth_gatt_scan_filter_param_t),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_bluetooth_gatt_scan_filter_status_t =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (bluetooth_gatt_scan_filter_status_t),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_bluetooth_gatt_scan_filter_cfg_t =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (bluetooth_gatt_scan_filter_cfg_t),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_bluetooth_gatt_filt_param_setup_t =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (bluetooth_gatt_filt_param_setup_t),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_bluetooth_gatt_db_element_t =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (bluetooth_gatt_db_element_t),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_BT_GATTC_GET_GATT_DB_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (BT_GATTC_GET_GATT_DB_T),
    .ui4_num_entries = 1,
    {
        {
            .e_type          = ARG_TYPE_REF_DESC,
            .pt_desc         = &t_rpc_decl_bluetooth_gatt_db_element_t,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (BT_GATTC_GET_GATT_DB_T, gatt_db_element)
            }
        }
    }
};

static const RPC_DESC_T t_rpc_decl_BT_GATTC_TEST_PARAMS_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (BT_GATTC_TEST_PARAMS_T),
    .ui4_num_entries = 2,
    {
        {
            .e_type          = ARG_TYPE_REF_DESC,
            .pt_desc         = NULL,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (BT_GATTC_TEST_PARAMS_T, bda1)
            }
        },
        {
            .e_type          = ARG_TYPE_REF_DESC,
            .pt_desc         = NULL,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (BT_GATTC_TEST_PARAMS_T, uuid1)
            }
        }
    }
};

static const RPC_DESC_T t_rpc_decl_BT_APP_GATTC_CB_FUNC_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (BT_APP_GATTC_CB_FUNC_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_BT_GATTS_REG_SERVER_RST_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (BT_GATTS_REG_SERVER_RST_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_BT_GATTS_CONNECT_RST_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (BT_GATTS_CONNECT_RST_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_BT_GATTS_ADD_SRVC_RST_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (BT_GATTS_ADD_SRVC_RST_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_BT_GATTS_ADD_INCL_RST_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (BT_GATTS_ADD_INCL_RST_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_BT_GATTS_ADD_CHAR_RST_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (BT_GATTS_ADD_CHAR_RST_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_BT_GATTS_ADD_DESCR_RST_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (BT_GATTS_ADD_DESCR_RST_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_BT_GATTS_SRVC_RST_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (BT_GATTS_SRVC_RST_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_BT_GATTS_REQ_READ_RST_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (BT_GATTS_REQ_READ_RST_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_BT_GATTS_REQ_WRITE_RST_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (BT_GATTS_REQ_WRITE_RST_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_BT_GATTS_EXEC_WRITE_RST_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (BT_GATTS_EXEC_WRITE_RST_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_BT_APP_GATTS_CB_FUNC_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (BT_APP_GATTS_CB_FUNC_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_c4tv_setup_info_adv_t =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (c4tv_setup_info_adv_t),
    .ui4_num_entries = 4,
    {
        {
            .e_type          = ARG_TYPE_REF_DESC,
            .pt_desc         = NULL,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (c4tv_setup_info_adv_t, service_uuid)
            }
        },
        {
            .e_type          = ARG_TYPE_REF_DESC,
            .pt_desc         = NULL,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (c4tv_setup_info_adv_t, advertised_uuid)
            }
        },
        {
            .e_type          = ARG_TYPE_REF_DESC,
            .pt_desc         = NULL,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (c4tv_setup_info_adv_t, advertise_data)
            }
        },
        {
            .e_type          = ARG_TYPE_REF_DESC,
            .pt_desc         = NULL,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (c4tv_setup_info_adv_t, manufacturer_data)
            }
        }
    }
};

static const RPC_DESC_T t_rpc_decl_c4tv_setup_info_scan_t =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (c4tv_setup_info_scan_t),
    .ui4_num_entries = 4,
    {
        {
            .e_type          = ARG_TYPE_REF_DESC,
            .pt_desc         = NULL,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (c4tv_setup_info_scan_t, service_uuid)
            }
        },
        {
            .e_type          = ARG_TYPE_REF_DESC,
            .pt_desc         = NULL,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (c4tv_setup_info_scan_t, advertised_uuid)
            }
        },
        {
            .e_type          = ARG_TYPE_REF_DESC,
            .pt_desc         = NULL,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (c4tv_setup_info_scan_t, advertise_data)
            }
        },
        {
            .e_type          = ARG_TYPE_REF_DESC,
            .pt_desc         = NULL,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (c4tv_setup_info_scan_t, manufacturer_data)
            }
        }
    }
};

static const RPC_DESC_T t_rpc_decl_c4tv_setup_info_attr_t =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (c4tv_setup_info_attr_t),
    .ui4_num_entries = 5,
    {
        {
            .e_type          = ARG_TYPE_REF_DESC,
            .pt_desc         = NULL,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (c4tv_setup_info_attr_t, service_uuid)
            }
        },
        {
            .e_type          = ARG_TYPE_REF_DESC,
            .pt_desc         = NULL,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (c4tv_setup_info_attr_t, attribute_uuid)
            }
        },
        {
            .e_type          = ARG_TYPE_REF_DESC,
            .pt_desc         = NULL,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (c4tv_setup_info_attr_t, control_uuid)
            }
        },
        {
            .e_type          = ARG_TYPE_REF_DESC,
            .pt_desc         = NULL,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (c4tv_setup_info_attr_t, value)
            }
        },
        {
            .e_type          = ARG_TYPE_REF_DESC,
            .pt_desc         = NULL,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (c4tv_setup_info_attr_t, read_value)
            }
        }
    }
};

static const RPC_DESC_T t_rpc_decl_c4tv_setup_info_t_u =
{
    .e_type          = ARG_TYPE_UNION,
    .z_size          = sizeof (((c4tv_setup_info_t*) NULL)->u),
    .ui4_num_entries = 3,
    {
        {
            .e_type          = ARG_TYPE_DESC,
            .pt_desc         = &t_rpc_decl_c4tv_setup_info_adv_t,
            .ui4_num_entries = 1,

            {
                .ps_field_name = "u.adv"
            }
        },
        {
            .e_type          = ARG_TYPE_DESC,
            .pt_desc         = &t_rpc_decl_c4tv_setup_info_scan_t,
            .ui4_num_entries = 1,

            {
                .ps_field_name = "u.scan"
            }
        },
        {
            .e_type          = ARG_TYPE_DESC,
            .pt_desc         = &t_rpc_decl_c4tv_setup_info_attr_t,
            .ui4_num_entries = 1,

            {
                .ps_field_name = "u.attr"
            }
        }
    }
};

static const RPC_DESC_T t_rpc_decl_c4tv_setup_info_t =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (c4tv_setup_info_t),
    .ui4_num_entries = 1,
    {
        {
            .e_type          = ARG_TYPE_DESC,
            .pt_desc         = &t_rpc_decl_c4tv_setup_info_t_u,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (c4tv_setup_info_t, u)
            }
        }
    }
};

static const RPC_DESC_T t_rpc_decl_c4tv_uuid_handle_t =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (c4tv_uuid_handle_t),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_MTKRPCAPI_BT_APP_GATTC_CB_FUNC_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (MTKRPCAPI_BT_APP_GATTC_CB_FUNC_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_MTKRPCAPI_BT_APP_GATTS_CB_FUNC_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (MTKRPCAPI_BT_APP_GATTS_CB_FUNC_T),
    .ui4_num_entries = 0
};


static const RPC_DESC_T* at_rpc_desc_list [] =
{
    &t_rpc_decl_BT_GATT_ID_T,
    &t_rpc_decl_BT_GATTS_SRVC_ID_T,
    &t_rpc_decl_BT_GATTC_REG_CLIENT_T,
    &t_rpc_decl_BT_GATTC_SCAN_RST_T,
    &t_rpc_decl_BT_GATTC_CONNECT_RST_T,
    &t_rpc_decl_BT_GATTC_SEARCH_RST_T,
    &t_rpc_decl_BT_GATTC_GET_CHAR_RST_T,
    &t_rpc_decl_BT_GATTC_GET_DESCR_RST_T,
    &t_rpc_decl_BT_GATTC_GET_INCL_RST_T,
    &t_rpc_decl_BT_GATTC_GET_REG_NOTI_RST_T,
    &t_rpc_decl_BT_GATT_NOTI_PARAMS_T,
    &t_rpc_decl_BT_GATTC_GET_NOTIFY_T,
    &t_rpc_decl_BT_GATT_UNFORMATTED_VALUE_T,
    &t_rpc_decl_BT_GATT_READ_PARAMS_T,
    &t_rpc_decl_BT_GATTC_READ_CHAR_RST_T,
    &t_rpc_decl_BT_GATTC_WRITE_CHAR_RST_T,
    &t_rpc_decl_BT_GATTC_READ_DESCR_RST_T,
    &t_rpc_decl_BT_GATTC_WRITE_DESCR_RST_T,
    &t_rpc_decl_BT_GATTC_GET_REMOTE_RSSI_T,
    &t_rpc_decl_bluetooth_gatt_scan_filter_param_t,
    &t_rpc_decl_bluetooth_gatt_scan_filter_status_t,
    &t_rpc_decl_bluetooth_gatt_scan_filter_cfg_t,
    &t_rpc_decl_bluetooth_gatt_filt_param_setup_t,
    &t_rpc_decl_bluetooth_gatt_db_element_t,
    &t_rpc_decl_BT_GATTC_GET_GATT_DB_T,
    &t_rpc_decl_BT_GATTC_TEST_PARAMS_T,
    &t_rpc_decl_BT_APP_GATTC_CB_FUNC_T,
    &t_rpc_decl_BT_GATTS_REG_SERVER_RST_T,
    &t_rpc_decl_BT_GATTS_CONNECT_RST_T,
    &t_rpc_decl_BT_GATTS_ADD_SRVC_RST_T,
    &t_rpc_decl_BT_GATTS_ADD_INCL_RST_T,
    &t_rpc_decl_BT_GATTS_ADD_CHAR_RST_T,
    &t_rpc_decl_BT_GATTS_ADD_DESCR_RST_T,
    &t_rpc_decl_BT_GATTS_SRVC_RST_T,
    &t_rpc_decl_BT_GATTS_REQ_READ_RST_T,
    &t_rpc_decl_BT_GATTS_REQ_WRITE_RST_T,
    &t_rpc_decl_BT_GATTS_EXEC_WRITE_RST_T,
    &t_rpc_decl_BT_APP_GATTS_CB_FUNC_T,
    &t_rpc_decl_c4tv_setup_info_adv_t,
    &t_rpc_decl_c4tv_setup_info_scan_t,
    &t_rpc_decl_c4tv_setup_info_attr_t,
    &t_rpc_decl_c4tv_setup_info_t_u,
    &t_rpc_decl_c4tv_setup_info_t,
    &t_rpc_decl_c4tv_uuid_handle_t,
    &t_rpc_decl_MTKRPCAPI_BT_APP_GATTC_CB_FUNC_T,
    &t_rpc_decl_MTKRPCAPI_BT_APP_GATTS_CB_FUNC_T
};

EXPORT_SYMBOL const RPC_DESC_T* __rpc_get_gatt_desc__ (UINT32  ui4_idx)
{
  return ((ui4_idx < 46) ? at_rpc_desc_list [ui4_idx] : NULL);
}



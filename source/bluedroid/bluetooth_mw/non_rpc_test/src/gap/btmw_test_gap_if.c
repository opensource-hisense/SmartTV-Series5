#include <stdlib.h>
#include <string.h>
#include <signal.h>

#include "bluetooth.h"
#include "u_bt_mw_types.h"
#include "u_bluetooth.h"
#include "u_bt_mw_common.h"
#include "u_bt_mw_gap.h"
#include "c_bt_mw_gap.h"
#include "c_bt_mw_manager.h"
#include "btmw_test_cli.h"
#include "btmw_test_debug.h"
#include "btmw_test_gap_if.h"

#define BLUETOOTH_TEST_NAME             "tv_bt"
#define BT_NAME_SUF_TEST_LEN              5

/*-----------------------------------------------------------------------------
                            variable declarations
 -----------------------------------------------------------------------------*/
static BT_APP_CB_FUNC g_btmw_test_gap_callbacks;
extern INT32 g_a2dp_connection_established;
extern VOID register_player_func(VOID);
extern CHAR btmw_test_default_pcm_file[128];
extern pthread_t btmw_test_stream_handle;
extern INT32 g_stream_status;
extern UINT8 g_new_PCM_file_flag;
extern VOID btmw_test_send_audio_data(VOID);
extern void bt_mw_log_setStackLevel(const char *trc_name, int level);

CHAR* print_the_event(BTEvent bt_event)
{
    switch (bt_event)
    {
    case BT_OFF_MODE:
        return "BT_OFF_MODE";
    case BT_CONNECT_MODE:
        return "BT_CONNECT_MODE";
    case BT_PAIRING_MODE:
        return "BT_PAIRING_MODE";
    case BT_PLAYING_MODE:
        return "BT_PLAYING_MODE";
    case BT_EXIT:
        return "BT_EXIT";
    case BT_GAP_STATE_BONDED:
        return "BT_GAP_STATE_BONDED";
    case BT_PAIRING_TIMEOUT:
        return "BT_PAIRING_TIMEOUT";
    case BT_GAP_STATE_NONE:
        return "BT_GAP_STATE_NONE";
    case BT_CONNECT_SUCCESS:
        return "BT_CONNECT_SUCCESS";
    case BT_CONNECT_MISSING:
        return "BT_CONNECT_MISSING";
    case BT_DISCONNECTED:
        return "BT_DISCONNECTED";
    case BT_CONNECT_TIMEOUT:
        return "BT_CONNECT_TIMEOUT";
    case BT_2ND_CONNECT_COMMING:
        return "BT_2ND_CONNECT_COMMING";
    case BT_2ND_CONNECT_FAIL:
        return "BT_2ND_CONNECT_FAIL";
    case BT_GET_DEV_HISTORY_DONE:
        return "BT_GET_DEV_HISTORY_DONE";
    case BT_SUPPORT_SCMS_T:
        return "BT_SUPPORT_SCMS_T";
    case BT_AVRCP_CONNECTION_SUCCESS:
        return "BT_SUPPORT_SCMS_T";
    case BT_A2DP_CONNECTION_SUCCESS:
        return "BT_A2DP_CONNECTION_SUCCESS";
    case BT_SUPPORT_AVRCP_1_3:
        return "BT_SUPPORT_AVRCP_1_3";
    case BT_NOT_SUPPORT_AVRCP_1_3:
        return "BT_NOT_SUPPORT_AVRCP_1_3";
    case BT_REGISTER_PS_SUCCESS:
        return "BT_REGISTER_PS_SUCCESS";
    case BT_REGISTER_PS_FAIL:
        return "BT_REGISTER_PS_FAIL";
    case BT_CONNECT_COMING:
        return "BT_CONNECT_COMING";
    case BT_GET_CODEC_INFORMATION_SUCCESS:
        return "BT_GET_CODEC_INFORMATION_SUCCESS";
    case BT_A2DP_UP_SAMPLE_2FS:
        return "BT_A2DP_UP_SAMPLE_2FS";
    case BT_HIDD_CONNECTION_SUCCESS:
        return "BT_HIDD_CONNECTION_SUCCESS";
    case BT_HIDD_CONNECTION_FAIL:
        return "BT_HIDD_CONNECTION_FAIL";
    case BT_HIDD_DISCONNECTION_SUCCESS:
        return "BT_HIDD_DISCONNECTION_SUCCESS";
    case BT_HIDD_REJECT_3ND_CONNECTION:
        return "BT_HIDD_REJECT_3ND_CONNECTION";
    default:
        break;
    }

    return "";
}

CHAR* print_hid_event(BT_HID_EVENT bt_event)
{
    switch (bt_event)
    {
    case BT_HID_CONNECTED:
        return "BT_HID_CONNECTED";
    case BT_HID_CONNECTING:
        return "BT_HID_CONNECTING";
    case BT_HID_DISCONNECTED:
        return "BT_HID_DISCONNECTED";
    case BT_HID_DISCONNECTING:
        return "BT_HID_DISCONNECTING";
    case BT_HID_CONNECT_FAIL:
        return "BT_HID_CONNECT_FAIL";
    case BT_HID_DISCONNECT_FAIL:
        return "BT_HID_DISCONNECT_FAIL";
    case BT_HID_CONNECTION_REJECT:
        return "BT_HID_CONNECTION_REJECT";
    default:
        break;
    }

    return "";
}

CHAR* print_spp_event(BT_SPP_EVENT bt_event)
{
    switch (bt_event)
    {
        case BT_SPP_CONNECT:
            return "BT_SPP_CONNECT";
        case BT_SPP_DISCONNECT:
            return "BT_SPP_DISCONNECT";
        case BT_SPP_RECV_DATA:
            return "BT_SPP_RECV_DATA";
        case BT_SPP_CONNECT_FAIL:
            return "BT_SPP_CONNECT_FAIL";
        case BT_SPP_DISCONNECT_FAIL:
            return "BT_SPP_DISCONNECT_FAIL";
        default:
            break;
    }

    return "";
}

VOID btmw_test_gap_app_event_callback(BTEvent bt_event)
{
    BTMW_TEST_Logd("bt_event: %s\n", print_the_event(bt_event));

    switch (bt_event)
    {
    case BT_CONNECT_SUCCESS:
        break;
    case BT_A2DP_CONNECTION_SUCCESS:
        c_btm_btstart(PLAYING, 0);
        g_a2dp_connection_established = 1;
        if (BT_A2DP_ROLE_SRC == c_btm_get_a2dp_role())
        {
            btmw_test_send_audio_data(); // lauch the thread used to send audio data to the remote device
        }
        break;
    case BT_DISCONNECTED:
        g_stream_status = 0;
        g_new_PCM_file_flag = 0;
        g_a2dp_connection_established = 0;
        strncpy(btmw_test_default_pcm_file, "/data/usb/music/48000/input_48000.pcm", 127);
        btmw_test_default_pcm_file[127] = 0;/*
        if (btmw_test_stream_handle)
        {
            pthread_kill(btmw_test_stream_handle, 0);
            btmw_test_stream_handle = -1;
        }*/
        c_btm_set_connectable_and_discoverable(TRUE,TRUE);
        break;
    case BT_EXIT:
        break;
    default:
        break;
    }
}

VOID btmw_test_hid_app_event_callback(BT_HID_CBK_STRUCT *pt_hid_struct)
{
    //FUNC_ENTRY;
    BTMW_TEST_Logv("pt_hid_struct.addr=%s\n", pt_hid_struct->current_addr);
    BTMW_TEST_Logv("pt_hid_struct.bt_get_dev_hid_history_done=%d\n", pt_hid_struct->bt_get_dev_hid_history_done);
    BTMW_TEST_Logv("pt_hid_struct.bt_hid_event_type=%d, %s.\n", pt_hid_struct->bt_hid_event_type,
    print_hid_event(pt_hid_struct->bt_hid_event_type));
}

VOID btmw_test_get_pairing_key_callback(pairing_key_value_t *bt_pairing_key, UINT8 *fg_accept)
{
    //FUNC_ENTRY;
    if (bt_pairing_key->key_type == PIN_CODE)
    {
        BTMW_TEST_Logv("PIN CODE\n");
        BTMW_TEST_Logv("pin code:%s\n", bt_pairing_key->pin_code);
    }
    else if (bt_pairing_key->key_type == PASSKEY)
    {
        BTMW_TEST_Logv("PASS KEY\n");
        BTMW_TEST_Logv("pass key:%d\n", bt_pairing_key->key_value);
    }
    *fg_accept = 1;
    BTMW_TEST_Logv("%s\n", *fg_accept ? "accept" : "reject");
}

VOID btmw_test_spp_callback(BT_SPP_CBK_STRUCT *pt_spp_struct)
{
    BTMW_TEST_Logd("[SPP] event: %s\n", print_spp_event(pt_spp_struct->bt_spp_event_type));
    BTMW_TEST_Logv("[SPP]bd_addr:%s\n", pt_spp_struct->bd_addr);
    BTMW_TEST_Logv("[SPP]uuid:%s\n", pt_spp_struct->uuid);
    BTMW_TEST_Logv("[SPP]spp_data:%s\n", pt_spp_struct->spp_data);
    BTMW_TEST_Logv("[SPP]uuid_len:%d\n", pt_spp_struct->uuid_len);
    BTMW_TEST_Logv("[SPP]spp_data_len:%d\n", pt_spp_struct->spp_data_len);
}

VOID btmw_test_gap_app_inquiry_callback(BT_GAP_SCAN_RESULT *pt_result)
{
    if (NULL == pt_result)
    {
        return;
    }

    BTMW_TEST_Logv("name:     %s\n", pt_result->name);
    BTMW_TEST_Logv("cod:      0x%2lx\n", (UINT32)pt_result->cod);
    BTMW_TEST_Logv("isPaired: %d\n", pt_result->isPaired);
    BTMW_TEST_Logv("bdAddr:   %s\n", pt_result->bdAddr);
    BTMW_TEST_Logv("service:  %ld\n", (long)pt_result->service);
    BTMW_TEST_Logv("rssi:     %d\n", pt_result->rssi);
}

INT32 btmw_test_gap_set_dbg_level_cli(int argc, char *argv[])
{
    if (argc != 2)
    {
        BTMW_TEST_Loge("Usage :\n");
        BTMW_TEST_Loge("layer <0~8> means set target layer to level\n");
        BTMW_TEST_Loge("if layer is 9 means set all layer to level\n");
        BTMW_TEST_Loge("level <0~5>\n");
        return -1;
    }
    c_btm_set_dbg_level(atoi(argv[0]), atoi(argv[1]));
    return 0;
}

INT32 btmw_test_gap_mode_cli(int argc, char *argv[])
{
    UINT32 u4_overtime = 300;

    if (argc != 1)
    {
        BTMW_TEST_Loge("please attach mode name after command just append one string\n");
        return -1;
    }

    if (strcmp("PAIRING", argv[0]) && strcmp("CONNECTING", argv[0]) &&
        strcmp("PLAYING", argv[0]) && strcmp("OFF", argv[0]))
    {
        BTMW_TEST_Loge("input error, please input again, eg. mode PAIRING\n");
        return -1;
    }
    BTMW_TEST_Logi("%s MODE\n", argv[0]);
    c_btm_btstart(argv[0], u4_overtime);
    return 0;
}

INT32 btmw_test_gap_get_mode(int argc, char *argv[])
{
    BT_MODE_TYPE mode;
    mode = c_btm_get_mode();
    BTMW_TEST_Logd("mode is %d \n", mode);
    return BT_SUCCESS;
}

INT32 btmw_test_gap_connecting(int argc, char *argv[])
{
    CHAR *ps_address;

    if (argc != 1)
    {
        BTMW_TEST_Logd("parameter num error,please enter one parameter addr\n");
        return BT_ERR_STATUS_INVALID_PARM_NUMS;
    }
    if (NULL == argv[0])
    {
        BTMW_TEST_Logd("addr is null \n");
        return BT_ERR_STATUS_NULL_POINTER;
    }
    ps_address = strdup(argv[0]);
    if (NULL == ps_address)
    {
        BTMW_TEST_Logd("strdup failed\n");
        return BT_ERR_STATUS_PARM_INVALID;
    }
    c_btm_btstart("CONNECTING", 300);
    return c_btm_connecting(ps_address);
}

INT32 btmw_test_gap_set_power_cli(int argc, char *argv[])
{
    if (argc != 1)
    {
        BTMW_TEST_Loge("arg num should be 1 \n");
        return -1;
    }

    if (0 == strcmp("0", argv[0]))
    {
        g_stream_status = 0;
        return c_btm_bluetooth_on_off(FALSE);
    }
    else if (0 == strcmp("1", argv[0]))
    {
        return c_btm_bluetooth_on_off(TRUE);
    }

    return 0;
}

INT32 btmw_test_gap_stop_cli(int argc, char *argv[])
{
    if (argc != 0)
    {
        BTMW_TEST_Loge("no args \n");
        return -1;
    }

    return c_btm_stop(FALSE, FALSE);
}

INT32 btmw_test_gap_clear_devinfo_cli(int argc, char *argv[])
{
    if (argc != 0)
    {
        BTMW_TEST_Loge("no args \n");
        return -1;
    }

    c_btm_clear_dev_info();
    return 0;
}

INT32 btmw_test_gap_set_name_cli(int argc, char *argv[])
{
    INT32 i4_ret;
    CHAR *ps_name;

    if (argc != 1)
    {
        BTMW_TEST_Logd("arg num should be 1\n");
    }
    if (NULL == argv[0])
    {
        BTMW_TEST_Logd("please attach name after commond just append one string\n");
    }

    ps_name = strdup(argv[0]);
    if (NULL == ps_name)
    {
        BTMW_TEST_Logd("strdup failed\n");
        return BT_ERR_STATUS_FAIL;
    }
    BTMW_TEST_Logd("name is: %s\n", ps_name);
    i4_ret = c_btm_set_local_name(ps_name);
    if (BT_SUCCESS == i4_ret)
    {
        BTMW_TEST_Logd("set name ok!\n");
    }

    return BT_SUCCESS;
}

INT32 btmw_test_gap_set_conn_disc_cli(int argc, char *argv[])
{
    if (2 == argc)
    {
        if (0 == strcmp("1" , argv[0]))
        {
            if (0 == strcmp("1" , argv[1]))
            {
                return c_btm_set_connectable_and_discoverable(TRUE, TRUE);
            }
            else if (0 == strcmp("0" , argv[1]))
            {
                return c_btm_set_connectable_and_discoverable(TRUE, FALSE);
            }
            else
            {
                BTMW_TEST_Loge("input error, 0 means off, 1 means on\n");
                return -1;
            }
        }
        else if (0 == strcmp("0" , argv[0]))
        {
            if (0 == strcmp("1" , argv[1]))
            {
                return c_btm_set_connectable_and_discoverable(FALSE, TRUE);
            }
            else if (0 == strcmp("0" , argv[1]))
            {
                return c_btm_set_connectable_and_discoverable(FALSE, FALSE);
            }
            else
            {
                BTMW_TEST_Loge("input error, 0 means off, 1 means on\n");
                return BT_ERR_STATUS_PARM_INVALID;
            }
        }
    }
    else
    {
        BTMW_TEST_Loge("arg num should be 2 \n");
        return BT_ERR_STATUS_INVALID_PARM_NUMS;
    }
    return BT_SUCCESS;
}

INT32 btmw_test_gap_get_paired_device_info_cli(int argc, char *argv[])
{
    if (argc != 0)
    {
        BTMW_TEST_Loge("no args \n");
        return -1;
    }

    c_btm_bluetooth_get_bonded_device();
    return BT_SUCCESS;
}

INT32 btmw_test_gap_get_local_dev_info_cli(int argc, char *argv[])
{
    BT_GAP_LOCAL_PROPERTIES_RESULT ps_dev_info;
    INT32 i4_ret;
    if (argc != 0)
    {
        BTMW_TEST_Loge("no args \n");
        return -1;
    }

    i4_ret = c_btm_get_local_dev_info(&ps_dev_info);

    if (BT_SUCCESS == i4_ret)
    {
        BTMW_TEST_Logd("get local device info ok!\n");
        BTMW_TEST_Logd("ps_dev_info.bdAddr:      %s\n", ps_dev_info.bdAddr);
        BTMW_TEST_Logd("ps_dev_info.name:        %s\n", ps_dev_info.name);
        BTMW_TEST_Logd("ps_dev_info.powered:     %s\n", ps_dev_info.powered ? "on" : "off");
        BTMW_TEST_Logd("ps_dev_info.scan_mode:   %ld\n", (unsigned long)ps_dev_info.scan_mode);
        BTMW_TEST_Logd("ps_dev_info.disctimeout: %ld\n", (unsigned long)ps_dev_info.disctimeout);
    }
    else
    {
        BTMW_TEST_Logd("get local device info failed!\n");
    }

    return BT_SUCCESS;
}

INT32 btmw_test_gap_inquiry_one_cli(int argc, char *argv[])
{
    CHAR ps_target_mac[18];
    UINT32 ui4_inqDuration = 10;

    if (2 == argc)
    {
        memset(ps_target_mac, 0, sizeof(ps_target_mac));
        if (17 > strlen(argv[0]))
        {
            BTMW_TEST_Logd("mac length should be 17\n");
            return BT_ERR_STATUS_PARM_INVALID;
        }
        strncpy(ps_target_mac, argv[0], sizeof(ps_target_mac)-1);
        ui4_inqDuration = atol(argv[1]);
        BTMW_TEST_Logd("inquiry %s:%ld s\n", ps_target_mac, (unsigned long)ui4_inqDuration);
    }
    else if (1 == argc)
    {
        memset(ps_target_mac, 0, sizeof(ps_target_mac));
        if (17 > strlen(argv[0]))
        {
            BTMW_TEST_Logd("mac length should be 17\n");
            return BT_ERR_STATUS_PARM_INVALID;
        }
        strncpy(ps_target_mac,argv[0], sizeof(ps_target_mac)-1);
        BTMW_TEST_Logd("inquiry %s:%ld s\n", ps_target_mac, (unsigned long)ui4_inqDuration);
    }
    else
    {
        BTMW_TEST_Logd("should input the parameter\n");
        return BT_ERR_STATUS_INVALID_PARM_NUMS;
    }
    return c_btm_inquiry_one(ps_target_mac, ui4_inqDuration);

}

INT32 btmw_test_gap_inquiry_cli(int argc, char *argv[])
{
    if (argc != 0)
    {
        BTMW_TEST_Loge("no args \n");
        return -1;
    }

    c_btm_start_inquiry_scan(0, btmw_test_gap_app_inquiry_callback);
    return 0;
}

INT32 btmw_test_gap_stop_inquiry_cli(int argc, char *argv[])
{
    if (argc != 0)
    {
        BTMW_TEST_Loge("no args \n");
        return -1;
    }

    return c_btm_stop_inquiry_scan();
}

INT32 btmw_test_gap_pair_cli(int argc, char *argv[])
{
    CHAR *ps_address;
    UINT32 transport;

    if (argc != 2)
    {
        BTMW_TEST_Logd("parameter num error,please enter two parameter: addr and transport \n");
        return BT_ERR_STATUS_INVALID_PARM_NUMS;
    }
    if (NULL == argv[0])
    {
        BTMW_TEST_Logd("addr is null \n");
        return BT_ERR_STATUS_NULL_POINTER;
    }

    ps_address = strdup(argv[0]);
    transport = atol(argv[1]);
    if (NULL == ps_address)
    {
        BTMW_TEST_Logd("strdup failed\n");
        return BT_ERR_STATUS_PARM_INVALID;
    }

    if (transport >= 3)
    {
        BTMW_TEST_Logd("transport error should < 3\n");
        return BT_ERR_STATUS_PARM_INVALID;
    }
    return c_btm_pair(ps_address, transport);
}

INT32 btmw_test_gap_remove_paired_dev_cli(int argc, char *argv[])
{
    CHAR *ps_address;

    if (argc != 1)
    {
        BTMW_TEST_Logd("arg num should be 1\n");
        return BT_ERR_STATUS_INVALID_PARM_NUMS;
    }
    if (NULL == argv[0])
    {
        BTMW_TEST_Logd("please attach MAC after commond just append one string\n");
        return BT_ERR_STATUS_NULL_POINTER;
    }

    ps_address = strdup(argv[0]);
    if (NULL == ps_address)
    {
        BTMW_TEST_Logd("strdup failed\n");
        return BT_ERR_STATUS_PARM_INVALID;
    }

    return c_btm_paired_dev_erase(ps_address);
}

INT32 btmw_test_gap_get_rssi_cli(int argc, char *argv[])
{
    CHAR ps_target_mac[18];
    INT16 rssi_value;

    if (1 == argc)
    {
        memset(ps_target_mac, 0, sizeof(ps_target_mac));
        if (17 > strlen(argv[0]))
        {
            BTMW_TEST_Logd("mac length should be 17\n");
            return BT_ERR_STATUS_PARM_INVALID;
        }
        strncpy(ps_target_mac,argv[0],sizeof(ps_target_mac)-1);
    }
    else
    {
        BTMW_TEST_Logd("arg num should be 1! \n");
        return BT_ERR_STATUS_INVALID_PARM_NUMS;
    }

    return c_btm_get_rssi(ps_target_mac, &rssi_value);;
}

INT32 btmw_test_gap_set_virtual_sniffer_cli(int argc, char *argv[])
{
    INT32 flag;

    if (argc != 1)
    {
        BTMW_TEST_Logd("A parameter is required!\n");
        return BT_ERR_STATUS_INVALID_PARM_NUMS;
    }

    flag = atoi(argv[0]);

    if (flag != 1 && flag != 0)
    {
        BTMW_TEST_Logd("flag should be 1 or 0 !\n");
        return BT_ERR_STATUS_PARM_INVALID;
    }
    c_btm_set_virtual_sniffer(flag);
    return BT_SUCCESS;
}

INT32 btmw_test_gap_send_hci(int argc, char *argv[])
{
    CHAR *hci_cmd;

    if (argc != 1)
        return BT_ERR_STATUS_INVALID_PARM_NUMS;

    hci_cmd = (CHAR *)argv[0];
    c_btm_send_hci(hci_cmd);
    return BT_SUCCESS;
}

INT32 btmw_test_gap_enable_stack_log(int argc, char *argv[])
{
    CHAR *section_name;
    UINT8 trace_level = 0;

    if (argc != 2)
    {
        return BT_ERR_STATUS_INVALID_PARM_NUMS;
    }

    section_name = (CHAR *)argv[0];
    trace_level = atoi(argv[1]);
    bt_mw_log_setStackLevel((const char*)section_name, trace_level);
    return BT_SUCCESS;
}


static BTMW_TEST_CLI btmw_test_gap_cli_commands[] = {
    { "dbg_level",            btmw_test_gap_set_dbg_level_cli,          " = dbg_level <0~9> <0~5>"},
    { "mode",                 btmw_test_gap_mode_cli,                   " = enter into some mode:PAIRING/CONNECTING/PLAYING/OFF"},
    { "get_current_mode",     btmw_test_gap_get_mode,                   " = get current a2dp mode CLOSE-0/PAIRING-1/CONNECTING-2/PLAYING-3/OFF-4"},
    { "a2dp_connecting",      btmw_test_gap_connecting,                 " = a2dp connecting"},
    { "power",                btmw_test_gap_set_power_cli,              " = power_on local device, 0:off, 1:on"},
    { "stop",                 btmw_test_gap_stop_cli,                   " = bluetooth stop"},
    { "clear_local",          btmw_test_gap_clear_devinfo_cli,          " = clear local device info"},
    { "name",                 btmw_test_gap_set_name_cli,               " = bt set local dev name, name <xxx>"},
    { "set_conn_disc",        btmw_test_gap_set_conn_disc_cli,          " = set device connectable and discoverable, set_conn_disc <1|0> <1|0>, 1:enable, 0: unable"},
    { "get_device_list",      btmw_test_gap_get_paired_device_info_cli, " = get paired device list "},
    { "info",                 btmw_test_gap_get_local_dev_info_cli,     " = info <local|update>"},
    { "inquiry_one",          btmw_test_gap_inquiry_one_cli,            " = inquiry_one <addr> 10"},
    { "inquiry",              btmw_test_gap_inquiry_cli,                " = start device discovery"},
    { "stop_inquiry",         btmw_test_gap_stop_inquiry_cli,           " = Stop device discovery"},
    { "pair",                 btmw_test_gap_pair_cli,                   " = Pair a remote device <addr> <transport> 0:unknown,1:BR/EDR,2:BLE"},
    { "unpair",               btmw_test_gap_remove_paired_dev_cli,      " = remove paired device <addr>"},
    { "get_rssi",             btmw_test_gap_get_rssi_cli,               " = get_rssi <addr>"},
    { "enable_hci_log",       btmw_test_gap_set_virtual_sniffer_cli,    " = enable_hci_log <1|0>. save bluetooth hci snoop log to btsnoop_hci.log"},
    { "send_hci",             btmw_test_gap_send_hci,                   " = send_hci <buffer>"},
    { "enable_stack_log",     btmw_test_gap_enable_stack_log,           " = enable_stack_log <trace_name> <level:0~6>"},

    { NULL, NULL, NULL }
};

int btmw_test_gap_cmd_handler(int argc, char **argv)
{
    BTMW_TEST_CLI *cmd, *match = NULL;
    int ret = 0;
    int count;

    count = 0;
    cmd = btmw_test_gap_cli_commands;

    BTMW_TEST_Logi("[GAP] argc: %d, argv[0]: %s\n", argc, argv[0]);

    while (cmd->cmd)
    {
        if (!strcmp(cmd->cmd, argv[0]))
        {
            match = cmd;
            count = 1;
            break;
        }
        cmd++;
    }

    if (count == 0)
    {
        btmw_test_print_cmd_help(BTMW_TEST_CMD_KEY_GAP, btmw_test_gap_cli_commands);
        ret = -1;
    }
    else
    {
        match->handler(argc - 1, &argv[1]);
    }

    return ret;
}

int btmw_test_gap_set_local_name()
{
    INT32 i4_ret;
    CHAR ac_bdAddr[MAX_BDADDR_LEN]={0};
    CHAR ac_name[MAX_NAME_LEN] = {0};
    CHAR bt_mac_suf[MAX_BDADDR_LEN] = {0};
    BT_GAP_LOCAL_PROPERTIES_RESULT t_local_info;
    memset(&t_local_info, 0, sizeof(BT_GAP_LOCAL_PROPERTIES_RESULT));

    //get local mac address
    i4_ret = c_btm_get_local_dev_info(&t_local_info);
    strncpy(ac_bdAddr, t_local_info.bdAddr, MAX_BDADDR_LEN);

     //only use the last 5 characters of the bluetooth device mac address
    strncpy(bt_mac_suf, ac_bdAddr + (strlen(ac_bdAddr)-BT_NAME_SUF_TEST_LEN), BT_NAME_SUF_TEST_LEN);

    strncpy(ac_name, BLUETOOTH_TEST_NAME, strlen(BLUETOOTH_TEST_NAME));

    //the bluetooth name: ac_name = BLUETOOTH_TEST_NAME + (bt_mac_suf)
    strcat(ac_name,"(");
    strcat(ac_name,bt_mac_suf);
    strcat(ac_name,")");
    c_btm_set_local_name(ac_name);
    return i4_ret;
}

int btmw_test_gap_reg(void)
{
    int ret = 0;
    BTMW_TEST_MOD btmw_test_gap_mod = {0};

    btmw_test_gap_mod.mod_id = BTMW_TEST_MOD_GAP;
    strncpy(btmw_test_gap_mod.cmd_key, BTMW_TEST_CMD_KEY_GAP, sizeof(btmw_test_gap_mod.cmd_key));
    btmw_test_gap_mod.cmd_handler = btmw_test_gap_cmd_handler;
    btmw_test_gap_mod.cmd_tbl = btmw_test_gap_cli_commands;

    ret = btmw_test_register_mod(&btmw_test_gap_mod);
    BTMW_TEST_Logi("[GAP] btmw_test_register_mod() returns: %d\n", ret);
    if (ret != 0)
    {
        return ret;
    }

    return 0;
}


int btmw_test_gap_init()
{
    btmw_test_gap_reg();

    memset(&g_btmw_test_gap_callbacks, 0, sizeof(BT_APP_CB_FUNC));
    g_btmw_test_gap_callbacks.bt_event_cb = btmw_test_gap_app_event_callback;
    g_btmw_test_gap_callbacks.bt_hid_cb = btmw_test_hid_app_event_callback;
    g_btmw_test_gap_callbacks.bt_get_pairing_key_cb = btmw_test_get_pairing_key_callback;
    g_btmw_test_gap_callbacks.bt_spp_cb = btmw_test_spp_callback;
    c_btm_bt_base_init(&g_btmw_test_gap_callbacks);
    //register_player_func();
    c_btm_source_sink_on_off(FALSE, TRUE);
    c_btm_bluetooth_on_off(TRUE);
    btmw_test_gap_set_local_name();
    c_btm_set_connectable_and_discoverable(TRUE, TRUE);

    return 0;
}

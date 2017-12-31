#include <string.h>

#include "btmw_rpc_test_cli.h"
#include "btmw_rpc_test_debug.h"
#include "btmw_rpc_test_gap_if.h"
#include "mtk_bt_service_gap_wrapper.h"

#define BLUETOOTH_RPC_TEST_NAME             "audio_bt"
#define BT_NAME_SUF_RPC_TEST_LEN              5

static void btmw_rpc_test_gap_event_callback(BTEvent bt_event, CHAR *addr, void* pv_tag)
{
    BTMW_RPC_TEST_Logi("[GAP] %s(), bt_gap_event =%d\n",  __func__, bt_event);

    if(NULL != addr)
    {
        BTMW_RPC_TEST_Logi("[GAP] %s(), addr =%s\n",  __func__, addr);
    }
    else
    {
        BTMW_RPC_TEST_Logi("[GAP] %s(), addr = NULL\n",  __func__);
    }

    switch (bt_event)
    {
    case BT_CONNECT_SUCCESS:
        break;
    case BT_A2DP_CONNECTION_SUCCESS:
        break;
    case BT_DISCONNECTED:
        a_mtkapi_bt_gap_set_connectable_and_discoverable(TRUE, TRUE);
        break;
    case BT_EXIT:
        break;
    case BT_A2DP_STREAM_START:
        a_mtkapi_bt_gap_btstart(PLAYING, 0);
        break;
    default:
        break;
    }
    return;
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

static void btmw_rpc_test_gap_get_pairing_key_callback(pairing_key_value_t *bt_pairing_key, UINT8 *fg_accept, void* pv_tag)
{
    BTMW_RPC_TEST_Logi("[GAP] %s(), bt_pairing_key->pin_code =%s,  key_value = %lu, fg_accept = %u\n",  __func__, bt_pairing_key->pin_code, bt_pairing_key->key_value, *fg_accept);
    return;
}

static void btmw_rpc_test_a2dp_upload_info_callback(BT_A2DP_UPLOAD_INFO* pt_upload_info, BOOL fg_only_check_mute, void* pv_tag)
{
    BTMW_RPC_TEST_Logi("[GAP] %s(), pt_upload_info->sample_freq =%d\n",  __func__, pt_upload_info->sample_freq);
    return;
}

static void btmw_rpc_test_a2dp_playback_ctrl_callback(BTPlayState bt_playback_cmd, void* pv_tag)
{
    BTMW_RPC_TEST_Logi("[GAP] %s(), bt_playback_cmd =%d\n",  __func__, bt_playback_cmd);
    return;
}

static void btmw_rpc_test_a2dp_reset_audio_callback(void* pv_tag)
{
    BTMW_RPC_TEST_Logi("[GAP] %s()\n",  __func__);
    return;
}

static void btmw_rpc_test_avrcp_playstate_change_callback(BTPlayState bt_playstate, void* pv_tag)
{
    BTMW_RPC_TEST_Logi("[GAP] %s(), bt_playstate =%d\n",  __func__, (int)bt_playstate);
    return;
}

static void btmw_rpc_test_avrcp_volume_change_callback(BTVolType bt_volume, void* pv_tag)
{
    BTMW_RPC_TEST_Logi("[GAP] %s(), bt_volume =%d\n",  __func__, (int)bt_volume);
    return;
}

static void btmw_rpc_test_avrcp_track_change_callback(UINT32 bt_track_index, void* pv_tag)
{
    BTMW_RPC_TEST_Logi("[GAP] %s(), bt_track_index =%lu\n",  __func__, bt_track_index);
    return;
}

static void btmw_rpc_test_avrcp_media_status_callback(BTMediaPlayState bt_status, void* pv_tag)
{
    BTMW_RPC_TEST_Logi("[GAP] %s(), bt_status =%d\n",  __func__, (int)bt_status);
    return;
}

static void btmw_rpc_test_avrcp_absolute_volume_callback(BTAbsoluteVolumeType flag, UINT8 *bt_absolute_volume, void* pv_tag)
{
    BTMW_RPC_TEST_Logi("[GAP] %s(), flag =%d\n",  __func__, (int)flag);
    return;
}

static void btmw_rpc_test_avrcp_cmd_callback(BTCmdType bt_cmd_type, UINT8 press, void* pv_tag)
{
    BTMW_RPC_TEST_Logi("[GAP] %s(), bt_cmd_type =%d\n",  __func__, (int)bt_cmd_type);
    return;
}

static void btmw_rpc_test_avrcp_get_playstatus_callback(BTPlayState *bt_playstatus, void* pv_tag)
{
    BTMW_RPC_TEST_Logi("[GAP] %s(), bt_playstatus =%d\n",  __func__, (int)bt_playstatus);
    return;
}

static void btmw_rpc_test_gap_hid_callback(BT_HID_CBK_STRUCT *pt_hid_struct, void* pv_tag)
{
    BTMW_RPC_TEST_Logi("[GAP] %s(), pt_hid_struct->current_addr =%s\n",  __func__, pt_hid_struct->current_addr);
    BTMW_RPC_TEST_Logi("[GAP] %s(), pt_hid_struct->bt_hid_event_type =%d\n",  __func__, pt_hid_struct->bt_hid_event_type);
    BTMW_RPC_TEST_Logi("[GAP] %s(), pt_hid_struct->bt_get_dev_hid_history_done =%d\n",  __func__, pt_hid_struct->bt_get_dev_hid_history_done);
    BTMW_RPC_TEST_Logi("[GAP] %s(), pt_hid_struct->name =%s\n",  __func__, pt_hid_struct->name);
    BTMW_RPC_TEST_Logi("[GAP] %s(), pt_hid_struct->cod =%d\n",  __func__, pt_hid_struct->cod);
    return;
}

static void btmw_rpc_test_gap_spp_callback(BT_SPP_CBK_STRUCT *pt_spp_struct, void* pv_tag)
{
    BTMW_RPC_TEST_Logi("[GAP] %s()\n",  __func__);
    BTMW_RPC_TEST_Logi("[SPP] event: %s\n", print_spp_event(pt_spp_struct->bt_spp_event_type));
    BTMW_RPC_TEST_Logi("[SPP]bd_addr:%s\n", pt_spp_struct->bd_addr);
    BTMW_RPC_TEST_Logi("[SPP]uuid:%s\n", pt_spp_struct->uuid);
    BTMW_RPC_TEST_Logi("[SPP]spp_data:%s\n", pt_spp_struct->spp_data);
    BTMW_RPC_TEST_Logi("[SPP]uuid_len:%d\n", pt_spp_struct->uuid_len);
    BTMW_RPC_TEST_Logi("[SPP]spp_data_len:%d\n", pt_spp_struct->spp_data_len);
    return;
}

static void btmw_rpc_test_gap_app_inquiry_callback(BT_GAP_SCAN_RESULT *pt_result)
{
    if (NULL == pt_result)
    {
        return;
    }

    BTMW_RPC_TEST_Logi("[GAP] %s() \n", __func__);
    BTMW_RPC_TEST_Logi("name:     %s\n", pt_result->name);
    BTMW_RPC_TEST_Logi("cod:      0x%2lx\n", (UINT32)pt_result->cod);
    BTMW_RPC_TEST_Logi("isPaired: %d\n", pt_result->isPaired);
    BTMW_RPC_TEST_Logi("bdAddr:   %s\n", pt_result->bdAddr);
    BTMW_RPC_TEST_Logi("service:  %ld\n", (long)pt_result->service);
    BTMW_RPC_TEST_Logi("rssi:     %d\n", pt_result->rssi);
}

static void btmw_rpc_test_hfclient_callback(BT_HFCLIENT_CBK_STRUCT *pt_hfclient_struct, VOID *pv_tag)
{
    if (NULL == pt_hfclient_struct)
    {
        return;
    }

    switch (pt_hfclient_struct->event)
    {
    case BT_HFCLIENT_VR_CMD:
        BTMW_RPC_TEST_Logi("[GAP] %s(), BT_HFCLIENT_VR_CMD vr_state=%d\n",
            __func__, pt_hfclient_struct->u.vr_state);
        break;
    case BT_HFCLIENT_NETWORK_STATE:
        BTMW_RPC_TEST_Logi("[GAP] %s(), BT_HFCLIENT_NETWORK_STATE network_state=%d\n",
            __func__, pt_hfclient_struct->u.network_state);
        break;
    case BT_HFCLIENT_NETWORK_ROAMING:
        BTMW_RPC_TEST_Logi("[GAP] %s(), BT_HFCLIENT_NETWORK_ROAMING service_type=%d\n",
            __func__, pt_hfclient_struct->u.service_type);
        break;
    case BT_HFCLIENT_NETWORK_SIGNAL:
        BTMW_RPC_TEST_Logi("[GAP] %s(), BT_HFCLIENT_NETWORK_SIGNAL signal_strength=%d\n",
            __func__, pt_hfclient_struct->u.signal_strength);
        break;
    case BT_HFCLIENT_BATTERY_LEVEL:
        BTMW_RPC_TEST_Logi("[GAP] %s(), BT_HFCLIENT_BATTERY_LEVEL battery_level=%d\n",
            __func__, pt_hfclient_struct->u.battery_level);
        break;
    case BT_HFCLIENT_CURRENT_OPERATOR:
        BTMW_RPC_TEST_Logi("[GAP] %s(), BT_HFCLIENT_CURRENT_OPERATOR operator_name=%s\n",
            __func__, pt_hfclient_struct->u.operator_name);
        break;
    case BT_HFCLIENT_CALL:
        BTMW_RPC_TEST_Logi("[GAP] %s(), BT_HFCLIENT_CALL call=%d\n",
            __func__, pt_hfclient_struct->u.call);
        break;
    case BT_HFCLIENT_CALLSETUP:
        BTMW_RPC_TEST_Logi("[GAP] %s(), BT_HFCLIENT_CALLSETUP callsetup=%d\n",
            __func__, pt_hfclient_struct->u.callsetup);
        break;
    case BT_HFCLIENT_CALLHELD:
        BTMW_RPC_TEST_Logi("[GAP] %s(), BT_HFCLIENT_CALLHELD callheld=%d\n",
            __func__, pt_hfclient_struct->u.callheld);
        break;
    case BT_HFCLIENT_RESP_AND_HOLD:
        BTMW_RPC_TEST_Logi("[GAP] %s(), BT_HFCLIENT_RESP_AND_HOLD resp_and_hold=%d\n",
            __func__, pt_hfclient_struct->u.resp_and_hold);
        break;
    case BT_HFCLIENT_CLIP:
        BTMW_RPC_TEST_Logi("[GAP] %s(), BT_HFCLIENT_CLIP clip_number=%s\n",
            __func__, pt_hfclient_struct->u.clip_number);
        break;
    case BT_HFCLIENT_CALL_WAITING:
        BTMW_RPC_TEST_Logi("[GAP] %s(), BT_HFCLIENT_CALL_WAITING call_waiting_number=%s\n",
            __func__, pt_hfclient_struct->u.call_waiting_number);
        break;
    case BT_HFCLIENT_CURRENT_CALLS:
        BTMW_RPC_TEST_Logi("[GAP] %s(), BT_HFCLIENT_CURRENT_CALLS index=%d, dir=%d, state=%d, mpty=%d, number=%s\n",
            __func__, pt_hfclient_struct->u.current_calls.index, pt_hfclient_struct->u.current_calls.dir,
                      pt_hfclient_struct->u.current_calls.state, pt_hfclient_struct->u.current_calls.mpty,
                      pt_hfclient_struct->u.current_calls.number);
        break;
    case BT_HFCLIENT_VOLUME_CHANGE:
        BTMW_RPC_TEST_Logi("[GAP] %s(), BT_HFCLIENT_VOLUME_CHANGE type=%d, volume=%d\n",
            __func__, pt_hfclient_struct->u.volume_change.type, pt_hfclient_struct->u.volume_change.volume);
        break;
    case BT_HFCLIENT_CMD_COMPLETE:
        BTMW_RPC_TEST_Logi("[GAP] %s(), BT_HFCLIENT_CMD_COMPLETE type=%d, cme=%d\n",
            __func__, pt_hfclient_struct->u.cmd_complete.type, pt_hfclient_struct->u.cmd_complete.cme);
        break;
    case BT_HFCLIENT_SUBSCRIBER_INFO:
        BTMW_RPC_TEST_Logi("[GAP] %s(), BT_HFCLIENT_SUBSCRIBER_INFO number=%s, type=%d\n",
            __func__, pt_hfclient_struct->u.subscriber_info.number, pt_hfclient_struct->u.subscriber_info.type);
        break;
    case BT_HFCLIENT_INBAND_RINGTONE:
        BTMW_RPC_TEST_Logi("[GAP] %s(), BT_HFCLIENT_INBAND_RINGTONE inband_ring_state=%d\n",
            __func__, pt_hfclient_struct->u.inband_ring_state);
        break;
    case BT_HFCLIENT_LAST_VOICE_TAG_NUMBER:
        BTMW_RPC_TEST_Logi("[GAP] %s(), BT_HFCLIENT_LAST_VOICE_TAG_NUMBER voice_tag_number=%s\n",
            __func__, pt_hfclient_struct->u.voice_tag_number);
        break;
    case BT_HFCLIENT_RING_INDICATION:
        BTMW_RPC_TEST_Logi("[GAP] %s(), BT_HFCLIENT_RING_INDICATION\n", __func__);
        break;
    default:
        BTMW_RPC_TEST_Logi("[GAP] %s(), unknown event=%d\n", __func__, pt_hfclient_struct->event);
        break;
    }
}

static void btmw_rpc_test_bond_callback(BT_BOND_STATUS_STRUCT *pt_bond_struct, void* pv_tag)
{
    BTMW_RPC_TEST_Logi("[GAP] %s()\n",  __func__);
    BTMW_RPC_TEST_Logi("[GAP] status: %d\n", pt_bond_struct->status);
    BTMW_RPC_TEST_Logi("[GAP] bd_addr:%s\n", pt_bond_struct->bdAddr);
    return;
}

int btmw_rpc_test_gap_bt_base_init(MTKRPCAPI_BT_APP_CB_FUNC *func, void *pv_tag)
{
    BTMW_RPC_TEST_Logi("[GAP] btmw_rpc_test_gap_bt_base_init\n");
    return a_mtkapi_gap_bt_base_init(func, pv_tag);
}

int btmw_rpc_test_gap_set_local_name(VOID)
{
    INT32 i4_ret;
    CHAR ac_bdAddr[MAX_BDADDR_LEN]={0};
    CHAR ac_name[MAX_NAME_LEN] = {0};
    CHAR bt_mac_suf[MAX_BDADDR_LEN] = {0};
    BT_GAP_LOCAL_PROPERTIES_RESULT t_local_info;
    memset(&t_local_info, 0, sizeof(BT_GAP_LOCAL_PROPERTIES_RESULT));

    //get local mac address
    i4_ret = a_mtkapi_bt_gap_get_local_dev_info(&t_local_info);
    strncpy(ac_bdAddr, t_local_info.bdAddr, MAX_BDADDR_LEN);

     //only use the last 5 characters of the bluetooth device mac address
    strncpy(bt_mac_suf, ac_bdAddr + (strlen(ac_bdAddr)-BT_NAME_SUF_RPC_TEST_LEN), BT_NAME_SUF_RPC_TEST_LEN);

    strncpy(ac_name, BLUETOOTH_RPC_TEST_NAME, strlen(BLUETOOTH_RPC_TEST_NAME));

    //the bluetooth name: ac_name = BLUETOOTH_RPC_TEST_NAME + (bt_mac_suf)
    strcat(ac_name,"(");
    strcat(ac_name,bt_mac_suf);
    strcat(ac_name,")");
    a_mtkapi_bt_gap_set_name(ac_name);
    return i4_ret;
}

int btmw_rpc_test_gap_set_dbg_level_cli(int argc, char **argv)
{
    if (argc != 2)
    {
        BTMW_RPC_TEST_Loge("Usage :\n");
        BTMW_RPC_TEST_Loge("layer <0~8> means set target layer to level\n");
        BTMW_RPC_TEST_Loge("if layer is 9 means set all layer to level\n");
        BTMW_RPC_TEST_Loge("level <0~5>\n");
        return BT_ERR_STATUS_INVALID_PARM_NUMS;
    }

    a_mtkapi_bt_gap_set_dbg_level(atoi(argv[0]), atoi(argv[1]));
    return BT_SUCCESS;
}

int btmw_rpc_test_gap_set_mode_cli(int argc, char **argv)
{
    CHAR ptr[130];
    memset(ptr,0,sizeof(ptr));

    BTMW_RPC_TEST_Logi("[GAP] %s()\n", __func__);

    UINT32 u4_overtime = 300;

    if (argc != 1)
    {
        BTMW_RPC_TEST_Loge("please attach mode name after command just append one string\n");
        return BT_ERR_STATUS_INVALID_PARM_NUMS;
    }

    if (strcmp("PAIRING", argv[0]) && strcmp("CONNECTING", argv[0]) &&
        strcmp("PLAYING", argv[0]) && strcmp("OFF", argv[0]))
    {
        BTMW_RPC_TEST_Loge("input error, please input again, eg. mode PAIRING\n");
        return BT_ERR_STATUS_PARM_INVALID;
    }

    BTMW_RPC_TEST_Logi("%s MODE\n", argv[0]);

    strncpy(ptr, argv[0], strlen(argv[0]));

    return a_mtkapi_bt_gap_btstart(argv[0], u4_overtime);
}

int btmw_rpc_test_gap_get_mode_cli(int argc, char **argv)
{
    BT_MODE_TYPE bt_mode;

    if (argc != 0)
    {
        BTMW_RPC_TEST_Loge("no parameter in this command\n");
        return BT_ERR_STATUS_INVALID_PARM_NUMS;
    }

    bt_mode = a_mtkapi_bt_gap_get_mode();

    BTMW_RPC_TEST_Logi("%ld MODE: 0: CLOSE 1:PAIRING 2:CONNECTING 3:PLAYING 4:OFF\n", (int)bt_mode);

    return BT_SUCCESS;
}

int btmw_rpc_test_gap_set_power_cli(int argc, char **argv)
{
    if (argc != 1)
    {
        BTMW_RPC_TEST_Loge("0 means off, 1 means on\n");
        return BT_ERR_STATUS_INVALID_PARM_NUMS;
    }

    if (0 == strcmp("0" , argv[0]))
    {
        return a_mtkapi_bt_gap_on_off(FALSE);
    }
    else if (0 == strcmp("1" , argv[0]))
    {
        return a_mtkapi_bt_gap_on_off(TRUE);
    }
    else
    {
        return BT_ERR_STATUS_PARM_INVALID;
    }

    return BT_SUCCESS;
}

int btmw_rpc_test_gap_stop_cli(int argc, char **argv)
{
    if (argc != 0)
    {
        BTMW_RPC_TEST_Loge("no parameter in this command\n");
        return BT_ERR_STATUS_INVALID_PARM_NUMS;
    }

    a_mtkapi_bt_gap_stop(FALSE, FALSE);
    return BT_SUCCESS;
}

int btmw_rpc_test_gap_connecting_cli(int argc, char **argv)
{
    if (argc != 1)
    {
        BTMW_RPC_TEST_Loge("pleae input BD address need connecting to \n");
        return BT_ERR_STATUS_INVALID_PARM_NUMS;
    }

    BTMW_RPC_TEST_Logd("BD address is: %s\n", argv[0]);

    a_mtkapi_bt_gap_connecting(argv[0]);
    return BT_SUCCESS;
}

int btmw_rpc_test_gap_auto_disconnection_cli(int argc, char **argv)
{
    if (argc != 0)
    {
        BTMW_RPC_TEST_Loge("no parameter in this command\n");
        return BT_ERR_STATUS_INVALID_PARM_NUMS;
    }

    a_mtkapi_bt_gap_auto_disconnection();
    return BT_SUCCESS;
}

int btmw_rpc_test_save_device_history_cli(int argc, char **argv)
{
    if (argc != 0)
    {
        BTMW_RPC_TEST_Loge("no parameter in this command\n");
        return BT_ERR_STATUS_INVALID_PARM_NUMS;
    }

    a_mtkapi_bt_gap_save_device_history();
    return BT_SUCCESS;
}

int btmw_rpc_test_gap_del_paired_av_dev_one_cli(int argc, char **argv)
{
    BT_TARGET_DEV_INFO device_info;
    memset(&device_info, 0, sizeof(device_info));
    if (argc != 1)
    {
        BTMW_RPC_TEST_Loge("pleae input BD address need connecting to \n");
        return BT_ERR_STATUS_INVALID_PARM_NUMS;
    }

    if (17 != strlen(argv[0]))
    {
        BTMW_RPC_TEST_Logd("mac length should be 17\n");
        return BT_ERR_STATUS_PARM_INVALID;
    }

    BTMW_RPC_TEST_Logd("BD address is: %s\n", argv[0]);
    strncpy(device_info.bdAddr, argv[0], MAX_BDADDR_LEN - 1);

    a_mtkapi_bt_gap_del_paired_av_dev_one(&device_info);
    return BT_SUCCESS;
}

int btmw_rpc_test_gap_del_paired_av_dev_all_cli(int argc, char **argv)
{
    if (argc != 0)
    {
        BTMW_RPC_TEST_Loge("no parameter in this command\n");
        return BT_ERR_STATUS_INVALID_PARM_NUMS;
    }

    a_mtkapi_bt_gap_del_paired_av_dev_all();
    return BT_SUCCESS;
}

int btmw_rpc_test_gap_clear_devinfo_cli(int argc, char **argv)
{
    if (argc != 0)
    {
        BTMW_RPC_TEST_Loge("no parameter in this command\n");
        return BT_ERR_STATUS_INVALID_PARM_NUMS;
    }

    a_mtkapi_bt_gap_clear_dev_info();
    return BT_SUCCESS;
}

int btmw_rpc_test_gap_set_name_cli(int argc, char **argv)
{
    INT32 i4_ret = BT_SUCCESS;

    if (argc != 1)
    {
        BTMW_RPC_TEST_Logd("please attach name after commond just append one string\n");
        return BT_ERR_STATUS_INVALID_PARM_NUMS;
    }
    if (NULL == argv[0])
    {
        BTMW_RPC_TEST_Logd("please attach name after commond just append one string\n");
        return BT_ERR_STATUS_NULL_POINTER;
    }

    BTMW_RPC_TEST_Logd("name is: %s\n", argv[0]);
    i4_ret = a_mtkapi_bt_gap_set_name(argv[0]);
    if (BT_SUCCESS == i4_ret)
    {
        BTMW_RPC_TEST_Logd("set name ok!\n");
    }

    return BT_SUCCESS;
}

int btmw_rpc_test_gap_set_conn_disc_cli(int argc, char **argv)
{
    if (2 == argc)
    {
        if (0 == strcmp("1" , argv[0]))
        {
            if (0 == strcmp("1" , argv[1]))
            {
                return a_mtkapi_bt_gap_set_connectable_and_discoverable(TRUE, TRUE);
            }
            else if (0 == strcmp("0" , argv[1]))
            {
                return a_mtkapi_bt_gap_set_connectable_and_discoverable(TRUE, FALSE);
            }
            else
            {
                BTMW_RPC_TEST_Loge("input error, 0 means off, 1 means on\n");
                return BT_ERR_STATUS_PARM_INVALID;
            }
        }
        else if (0 == strcmp("0" , argv[0]))
        {
            if (0 == strcmp("1" , argv[1]))
            {
                return a_mtkapi_bt_gap_set_connectable_and_discoverable(FALSE, TRUE);
            }
            else if (0 == strcmp("0" , argv[1]))
            {
                return a_mtkapi_bt_gap_set_connectable_and_discoverable(FALSE, FALSE);
            }
            else
            {
                BTMW_RPC_TEST_Loge("input error, 0 means off, 1 means on\n");
                return BT_ERR_STATUS_PARM_INVALID;
            }
        }
    }
    else
    {
        BTMW_RPC_TEST_Loge("0 means off, other integer:on");
        return BT_ERR_STATUS_INVALID_PARM_NUMS;
    }
    return BT_SUCCESS;
}

int btmw_rpc_test_gap_get_paired_device_info_cli(int argc, char **argv)
{
    if (argc != 0)
    {
        BTMW_RPC_TEST_Loge("no parameter in this command\n");
        return BT_ERR_STATUS_INVALID_PARM_NUMS;
    }

    a_mtkapi_bt_gap_get_paired_device_info();
    return BT_SUCCESS;
}

int btmw_rpc_test_gap_get_local_dev_info_cli(int argc, char **argv)
{
    BT_GAP_LOCAL_PROPERTIES_RESULT ps_dev_info;
    INT32 i4_ret = BT_SUCCESS;
    if (argc != 0)
    {
        BTMW_RPC_TEST_Loge("no parameter in this command\n");
        return BT_ERR_STATUS_INVALID_PARM_NUMS;
    }

    a_mtkapi_bt_gap_get_local_dev_info(&ps_dev_info);

    if (BT_SUCCESS == i4_ret)
    {
        BTMW_RPC_TEST_Logd("get local device info ok!\n");
        BTMW_RPC_TEST_Logd("ps_dev_info.bdAddr:      %s\n", ps_dev_info.bdAddr);
        BTMW_RPC_TEST_Logd("ps_dev_info.name:        %s\n", ps_dev_info.name);
        BTMW_RPC_TEST_Logd("ps_dev_info.powered:     %s\n", ps_dev_info.powered ? "on" : "off");
        BTMW_RPC_TEST_Logd("ps_dev_info.scan_mode:   %d\n", ps_dev_info.scan_mode);
        BTMW_RPC_TEST_Logd("ps_dev_info.disctimeout: %lu\n", ps_dev_info.disctimeout);
    }
    else
    {
        BTMW_RPC_TEST_Logd("get local device info failed!\n");
    }

    return BT_SUCCESS;
}

int btmw_rpc_test_gap_get_target_info_cli(int argc, char **argv)
{
    BT_TARGET_DEV_INFO ps_device_info;
    INT32 i4_ret = BT_SUCCESS;
    if (argc != 0)
    {
        BTMW_RPC_TEST_Loge("no parameter in this command\n");
        return BT_ERR_STATUS_INVALID_PARM_NUMS;
    }

    a_mtkapi_bt_gap_get_target_info(&ps_device_info);

    if (BT_SUCCESS == i4_ret)
    {
        BTMW_RPC_TEST_Logd("get device info ok!\n");
        BTMW_RPC_TEST_Logd("ps_device_info.bdAddr:      %s\n", ps_device_info.bdAddr);
        BTMW_RPC_TEST_Logd("ps_device_info.name:        %s\n", ps_device_info.name);
        BTMW_RPC_TEST_Logd("ps_device_info.cod:     %lu\n", ps_device_info.cod);
        BTMW_RPC_TEST_Logd("ps_device_info.device_type:   %lu\n", ps_device_info.device_type);
        BTMW_RPC_TEST_Logd("ps_device_info.fg_istv: %d\n", ps_device_info.fg_istv);
        BTMW_RPC_TEST_Logd("ps_device_info.u4delay_value: %lu\n", ps_device_info.u4delay_value);
    }
    else
    {
        BTMW_RPC_TEST_Logd("get device info failed!\n");
    }

    return BT_SUCCESS;
}

int btmw_rpc_test_gap_inquiry_one_cli(int argc, char **argv)
{
    CHAR ps_target_mac[18];
    UINT32 ui4_inqDuration = 10;

    if (2 == argc)
    {
        memset(ps_target_mac, 0, sizeof(ps_target_mac));
        if (17 > strlen(argv[0]))
        {
            BTMW_RPC_TEST_Logd("mac length should be 17\n");
            return BT_ERR_STATUS_PARM_INVALID;
        }
        strncpy(ps_target_mac,argv[0],sizeof(ps_target_mac)-1);
        ui4_inqDuration = atol(argv[1]);
        BTMW_RPC_TEST_Logd("inquiry %s:%ld s\n", ps_target_mac, (unsigned long)ui4_inqDuration);
    }
    else if (1 == argc)
    {
        memset(ps_target_mac, 0, sizeof(ps_target_mac));
        if (17 > strlen(argv[0]))
        {
            BTMW_RPC_TEST_Logd("mac length should be 17\n");
            return BT_ERR_STATUS_PARM_INVALID;
        }
        strncpy(ps_target_mac,argv[0],sizeof(ps_target_mac)-1);
        BTMW_RPC_TEST_Logd("inquiry %s:%ld s\n", ps_target_mac, (unsigned long)ui4_inqDuration);
    }
    else
    {
        BTMW_RPC_TEST_Logd("should input the parameter\n");
        return BT_ERR_STATUS_INVALID_PARM_NUMS;
    }

    a_mtkapi_bt_gap_inquiry_one(ps_target_mac, ui4_inqDuration);
    return BT_SUCCESS;

}

int btmw_rpc_test_gap_inquiry_cli(int argc, char **argv)
{
    int flag = BT_INQUIRY_FILTER_TYPE_A2DP_HID;

    if (argc == 0)
    {
        BTMW_RPC_TEST_Logd("No Input parameter!\n");
    }
    else if (argc == 1)
    {
        BTMW_RPC_TEST_Logd("Input A parameter!\n");
        flag = atoi(argv[0]);
    }
    else
    {
        BTMW_RPC_TEST_Logd("parameter number is error!\n");
        return BT_ERR_STATUS_INVALID_PARM_NUMS;
    }

    a_mtkapi_bt_gap_start_inquiry_scan(flag, btmw_rpc_test_gap_app_inquiry_callback);
    return 0;
}

int btmw_rpc_test_gap_stop_inquiry_cli(int argc, char **argv)
{
    if (argc != 0)
    {
        BTMW_RPC_TEST_Loge("no parameter in this command\n");
        return BT_ERR_STATUS_INVALID_PARM_NUMS;
    }

    return a_mtkapi_bt_gap_stop_inquiry_scan();
}

int btmw_rpc_test_gap_pair_cli (int argc, char **argv)
{
    UINT32 transport;

    if (argc != 2)
    {
        BTMW_RPC_TEST_Logd("parameter num error,please enter two parameter: addr and transport \n");
        return BT_ERR_STATUS_INVALID_PARM_NUMS;
    }
    if (NULL == argv[0])
    {
        BTMW_RPC_TEST_Logd("addr is null \n");
        return BT_ERR_STATUS_NULL_POINTER;
    }

    if (17 != strlen(argv[0]))
    {
        BTMW_RPC_TEST_Logd("mac length should be 17\n");
        return BT_ERR_STATUS_PARM_INVALID;
    }
    transport = atol(argv[1]);
    return a_mtkapi_bt_gap_pair(argv[0], transport);

}

int btmw_rpc_test_gap_remove_paired_dev_cli(int argc, char **argv)
{
    if (argc != 1)
    {
        BTMW_RPC_TEST_Logd("please attach MAC after commond just append one string\n");
        return BT_ERR_STATUS_INVALID_PARM_NUMS;
    }
    if (NULL == argv[0])
    {
        BTMW_RPC_TEST_Logd("please attach MAC after commond just append one string\n");
        return BT_ERR_STATUS_NULL_POINTER;
    }
    if (17 != strlen(argv[0]))
    {
        BTMW_RPC_TEST_Logd("mac length should be 17\n");
        return BT_ERR_STATUS_PARM_INVALID;
    }

    return a_mtkapi_bt_gap_paired_dev_erase(argv[0]);
}

int btmw_rpc_test_gap_get_rssi_cli (int argc, char **argv)
{
    CHAR ps_target_mac[18];
    INT16 rssi_value;

    if (1 == argc)
    {
        memset(ps_target_mac, 0, sizeof(ps_target_mac));
        if (17 != strlen(argv[0]))
        {
            BTMW_RPC_TEST_Logd("mac length should be 17\n");
            return BT_ERR_STATUS_PARM_INVALID;
        }
        strncpy(ps_target_mac,argv[0],sizeof(ps_target_mac)-1);
    }
    else
    {
        BTMW_RPC_TEST_Logd("please input get_rssi [MAC address]\n");
        return BT_ERR_STATUS_INVALID_PARM_NUMS;
    }

    a_mtkapi_bt_gap_get_rssi(ps_target_mac, &rssi_value);
    return BT_SUCCESS;
}

int btmw_rpc_test_gap_set_virtual_sniffer_cli(int argc, char **argv)
{

    INT32 flag;

    if (argc != 1)
    {
        BTMW_RPC_TEST_Logd("A parameter is required!\n");
        return BT_ERR_STATUS_INVALID_PARM_NUMS;
    }

    flag = atoi(argv[0]);
    a_mtkapi_bt_gap_set_virtual_sniffer(flag);

    return BT_SUCCESS;
}

int btmw_rpc_test_gap_send_hci(int argc, char **argv)
{
    CHAR *hci_cmd;

    if (argc != 1)
        return BT_ERR_STATUS_INVALID_PARM_NUMS;

    hci_cmd = (CHAR *)argv[0];
    a_mtkapi_bt_gap_send_hci(hci_cmd);
    return BT_SUCCESS;
}

int btmw_rpc_test_gap_factory_reset(int argc, char **argv)
{
    BTMW_RPC_TEST_Logd("btmw_rpc_test_gap_factory_reset, argc=%d.\n", argc);
    if (0 == argc)
    {
        BTMW_RPC_TEST_Logd("Not input address\n");
        return a_mtkapi_bt_bluetooth_factory_reset(NULL);
    }
    else if (1 == argc)
    {
        BTMW_RPC_TEST_Logd("BD address is: %s\n", argv[0]);
        char bdaddr[MAX_BDADDR_LEN];
        memset(bdaddr, 0, MAX_BDADDR_LEN);
        BTMW_RPC_TEST_Logd("input address:%s\n", argv[0]);
        strncpy(bdaddr, argv[0], MAX_BDADDR_LEN);
        bdaddr[MAX_BDADDR_LEN - 1] = '\0';
        return a_mtkapi_bt_bluetooth_factory_reset(bdaddr);
    }
    else
    {
        BTMW_RPC_TEST_Logd("please attach MAC after commond just append one string\n");
        return BT_ERR_STATUS_INVALID_PARM_NUMS;
    }
}

static BTMW_RPC_TEST_CLI btmw_rpc_test_gap_cli_commands[] = {
    { (const char *)"dbg_level",            btmw_rpc_test_gap_set_dbg_level_cli,          (const char *)" = dbg_level <0~9> <0~5>"},
    { (const char *)"mode",                 btmw_rpc_test_gap_set_mode_cli,               (const char *)" = enter into some mode:PAIRING/CONNECTING/PLAYING/OFF"},
    { (const char *)"current_mode",         btmw_rpc_test_gap_get_mode_cli,               (const char *)" = get current mode:PAIRING/CONNECTING/PLAYING/OFF"},
    { (const char *)"power",                btmw_rpc_test_gap_set_power_cli,              (const char *)" = power_on local device, 0:off, 1:on"},
    { (const char *)"stop",                 btmw_rpc_test_gap_stop_cli,                   (const char *)" = bluetooth stop"},
    { (const char *)"connect",              btmw_rpc_test_gap_connecting_cli,             (const char *)" = bluetooth connection to a2dp device <addr> "},
    { (const char *)"dis_connect",          btmw_rpc_test_gap_auto_disconnection_cli,     (const char *)" = bluetooth auto disconncetiong"},
    { (const char *)"save_history",         btmw_rpc_test_save_device_history_cli,        (const char *)" = save history to local file"},
    { (const char *)"delete_one",           btmw_rpc_test_gap_del_paired_av_dev_one_cli,  (const char *)" = remove one item from paired device histroy"},
    { (const char *)"delete_all",           btmw_rpc_test_gap_del_paired_av_dev_all_cli,  (const char *)" = remove all items from paired device histroy"},
    { (const char *)"clear_local",          btmw_rpc_test_gap_clear_devinfo_cli,          (const char *)" = clear local device info"},
    { (const char *)"name",                 btmw_rpc_test_gap_set_name_cli,               (const char *)" = bt set local dev name, name <xxx>"},
    { (const char *)"set_conn_disc",        btmw_rpc_test_gap_set_conn_disc_cli,          (const char *)" = set device connectable and discoverable, set_conn_disc <1|0> <1|0>, 1:enable, 0: unable"},
    { (const char *)"get_device_list",      btmw_rpc_test_gap_get_paired_device_info_cli, (const char *)" = get paired device list "},
    { (const char *)"info",                 btmw_rpc_test_gap_get_local_dev_info_cli,     (const char *)" = info <local|update>"},
    { (const char *)"current_dev_info",     btmw_rpc_test_gap_get_target_info_cli,        (const char *)" = get current connected device info"},
    { (const char *)"inquiry_one",          btmw_rpc_test_gap_inquiry_one_cli,            (const char *)" = inquiry_one <addr> 10"},
    { (const char *)"inquiry",              btmw_rpc_test_gap_inquiry_cli,                (const char *)" = start device discovery"},
    { (const char *)"stop_inquiry",         btmw_rpc_test_gap_stop_inquiry_cli,           (const char *)" = stop device discovery"},
    { (const char *)"pair",                 btmw_rpc_test_gap_pair_cli,                   (const char *)" = pair a remote device <addr> <transport> 0:unknown,1:BR/EDR,2:BLE"},
    { (const char *)"unpair",               btmw_rpc_test_gap_remove_paired_dev_cli,      (const char *)" = remove paired device <addr>"},
    { (const char *)"get_rssi",             btmw_rpc_test_gap_get_rssi_cli,               (const char *)" = get_rssi <addr>"},
    { (const char *)"config_hci_snoop_log", btmw_rpc_test_gap_set_virtual_sniffer_cli,    (const char *)" = config_hci_snoop_log <1|0>. save bluetooth hci snoop log to btsnoop_hci.log"},
    { (const char *)"send_hci",             btmw_rpc_test_gap_send_hci,                   (const char *)" = send_hci <buffer>"},
    { (const char *)"factory_reset",        btmw_rpc_test_gap_factory_reset,              (const char *)" = delete bluetooth files for factory reset"},

    { NULL, NULL, NULL }
};

int btmw_rpc_test_gap_cmd_handler(int argc, char **argv)
{
    BTMW_RPC_TEST_CLI *cmd, *match = NULL;
    int ret = 0;
    int count;

    count = 0;
    cmd = btmw_rpc_test_gap_cli_commands;

    BTMW_RPC_TEST_Logi("[GAP] argc: %d, argv[0]: %s\n", argc, argv[0]);

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
        btmw_rpc_test_print_cmd_help(BTMW_RPC_TEST_CMD_KEY_GAP, btmw_rpc_test_gap_cli_commands);
        ret = -1;
    }
    else
    {
        match->handler(argc - 1, &argv[1]);
    }

    return ret;
}

int btmw_rpc_test_gap_init()
{
    int ret = 0;
    BTMW_RPC_TEST_MOD btmw_rpc_test_gap_mod = {0};

    btmw_rpc_test_gap_mod.mod_id = BTMW_RPC_TEST_MOD_GAP;
    strncpy(btmw_rpc_test_gap_mod.cmd_key, BTMW_RPC_TEST_CMD_KEY_GAP, sizeof(btmw_rpc_test_gap_mod.cmd_key));
    btmw_rpc_test_gap_mod.cmd_handler = btmw_rpc_test_gap_cmd_handler;
    btmw_rpc_test_gap_mod.cmd_tbl = btmw_rpc_test_gap_cli_commands;

    ret = btmw_rpc_test_register_mod(&btmw_rpc_test_gap_mod);
    BTMW_RPC_TEST_Logi("[GAP] btmw_rpc_test_register_mod() returns: %d\n", ret);

    if (!g_cli_pts_mode)
    {
        MTKRPCAPI_BT_APP_CB_FUNC func;
        char pv_tag[2] = {0};
        memset(&func, 0, sizeof(MTKRPCAPI_BT_APP_CB_FUNC));
        func.bt_event_cb = btmw_rpc_test_gap_event_callback;
        func.bt_get_pairing_key_cb = btmw_rpc_test_gap_get_pairing_key_callback;
        func.bt_upload_info_cb = btmw_rpc_test_a2dp_upload_info_callback;
        func.bt_playback_ctrl_cb = btmw_rpc_test_a2dp_playback_ctrl_callback;
        func.bt_reset_audio_cb = btmw_rpc_test_a2dp_reset_audio_callback;
        func.bt_play_change_cb = btmw_rpc_test_avrcp_playstate_change_callback;
        func.bt_volume_change_cb = btmw_rpc_test_avrcp_volume_change_callback;
        func.bt_track_change_cb = btmw_rpc_test_avrcp_track_change_callback;
        func.bt_media_playstatus_cb = btmw_rpc_test_avrcp_media_status_callback;
        func.bt_absolute_volume_cb = btmw_rpc_test_avrcp_absolute_volume_callback;
        func.bt_avrcp_cmd_cb = btmw_rpc_test_avrcp_cmd_callback;
        func.bt_get_play_status_cb = btmw_rpc_test_avrcp_get_playstatus_callback;
        func.bt_hid_cb = btmw_rpc_test_gap_hid_callback;
        func.bt_spp_cb = btmw_rpc_test_gap_spp_callback;
        func.bt_hfclient_cb = btmw_rpc_test_hfclient_callback;
        func.bt_bond_cb = btmw_rpc_test_bond_callback;

        btmw_rpc_test_gap_bt_base_init(&func, (void *)pv_tag);
        a_mtkapi_a2dp_source_sink_on_off(FALSE, TRUE);
        a_mtkapi_bt_gap_on_off(TRUE);
        btmw_rpc_test_gap_set_local_name();
        a_mtkapi_bt_gap_set_connectable_and_discoverable(TRUE, TRUE);
    }
    return 0;
}


#include "btmw_rpc_test_cli.h"
#include "btmw_rpc_test_debug.h"
#include "btmw_rpc_test_avrcp_if.h"
#include "mtk_bt_service_avrcp_wrapper.h"

static BTMW_RPC_TEST_CLI btmw_rpc_test_rc_ct_cli_commands[];


#if 0
static void btmw_rpc_test_rc_set_abs_vol_cb(BTAbsoluteVolumeType flag, UINT8 *bt_absolute_volume)
{
    BTMW_RPC_TEST_Logd("[AVRCP] %s() flag=%d, vol=%u", __func__, *bt_absolute_volume);
    *bt_absolute_volume = 100;
}

BT_APP_CB_FUNC g_btmw_rpc_test_rc_callbacks =
{
    NULL, /* BtAppEventCbk bt_event_cb; */
    NULL, /* BtAppPlayStateChangeCbk bt_play_change_cb; */
    NULL, /* BtAppVolumeChangeCbk bt_volume_change_cb; */
    NULL, /* BtAppTrackChangeCbk bt_track_change_cb */
    NULL, /* BtAppMediaStatusCbk bt_media_playstatus_cb; */
    NULL, /* BtAppSppCbk bt_spp_cb; */
    NULL, /* BtAppHidCbk bt_hid_cb; */
    btmw_rpc_test_rc_set_abs_vol_cb, /* BtAppAbsoluteVolumeCbk bt_absolute_volume_cb; */
    NULL, /* BtAppAVRCPCMDCbk bt_avrcp_cmd_cb; */
    NULL, /* BtAppUploadInfoCbk bt_upload_info_cb; */
    NULL, /* BtAppGetPlayStatusCbk bt_get_play_status_cb; */
    NULL, /* BtAppPlaybackCtrlCbk bt_playback_ctrl_cb; */
    NULL, /* BtAppResetAudioCbk bt_reset_audio_cb; */
    NULL, /* BtAppGetPairingKeyCbk bt_get_pairing_key_cb; */
};
#endif

int btmw_rpc_test_rc_ct_cmd_handler(int argc, char **argv)
{
    BTMW_RPC_TEST_CLI *cmd, *match = NULL;
    int ret = 0;
    int count = 0;

    BTMW_RPC_TEST_Logi("[AVRCP] CT argc: %d, argv[0]: %s\n", argc, argv[0]);

    cmd = btmw_rpc_test_rc_ct_cli_commands;
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
        btmw_rpc_test_print_cmd_help(BTMW_RPC_TEST_CMD_KEY_AVRCP_CT, btmw_rpc_test_rc_ct_cli_commands);
        ret = -1;
    }
    else
    {
        match->handler(argc - 1, &argv[1]);
    }

    return ret;
}

static int btmw_rpc_test_rc_send_play_handler(int argc, char *argv[])
{
    BTMW_RPC_TEST_Logi("[AVRCP] %s() \n", __func__);

    a_mtkapi_avrcp_passthrough_cmd(BT_CMD_TYPE_PLAY, BUTTON_ACT_PRESS);
    sleep(1);
    a_mtkapi_avrcp_passthrough_cmd(BT_CMD_TYPE_PLAY, BUTTON_ACT_RELEASE);

    return 0;
}

static int btmw_rpc_test_rc_send_pause_handler(int argc, char *argv[])
{
    BTMW_RPC_TEST_Logi("[AVRCP] %s() \n", __func__);

    a_mtkapi_avrcp_passthrough_cmd(BT_CMD_TYPE_PAUSE, BUTTON_ACT_PRESS);
    sleep(1);
    a_mtkapi_avrcp_passthrough_cmd(BT_CMD_TYPE_PAUSE, BUTTON_ACT_RELEASE);

    return 0;
}

static int btmw_rpc_test_rc_send_stop_handler(int argc, char *argv[])
{
    BTMW_RPC_TEST_Logi("[AVRCP] %s() \n", __func__);

    a_mtkapi_avrcp_passthrough_cmd(BT_CMD_TYPE_STOP, BUTTON_ACT_PRESS);
    sleep(1);
    a_mtkapi_avrcp_passthrough_cmd(BT_CMD_TYPE_STOP, BUTTON_ACT_RELEASE);

    return 0;
}

static int btmw_rpc_test_rc_send_fwd_handler(int argc, char *argv[])
{
    BTMW_RPC_TEST_Logi("[AVRCP] %s() \n", __func__);

    a_mtkapi_avrcp_passthrough_cmd(BT_CMD_TYPE_FWD, BUTTON_ACT_PRESS);
    sleep(1);
    a_mtkapi_avrcp_passthrough_cmd(BT_CMD_TYPE_FWD, BUTTON_ACT_RELEASE);
    return 0;
}

static int btmw_rpc_test_rc_send_bwd_handler(int argc, char *argv[])
{
    BTMW_RPC_TEST_Logi("[AVRCP] %s() \n", __func__);

    a_mtkapi_avrcp_passthrough_cmd(BT_CMD_TYPE_BWD, BUTTON_ACT_PRESS);
    sleep(1);
    a_mtkapi_avrcp_passthrough_cmd(BT_CMD_TYPE_BWD, BUTTON_ACT_RELEASE);
    return 0;
}

static int btmw_rpc_test_rc_send_ffwd_handler(int argc, char *argv[])
{
    BTMW_RPC_TEST_Logi("[AVRCP] %s() \n", __func__);

    a_mtkapi_avrcp_passthrough_cmd(BT_CMD_TYPE_FFWD, BUTTON_ACT_PRESS);
    sleep(1);
    a_mtkapi_avrcp_passthrough_cmd(BT_CMD_TYPE_FFWD, BUTTON_ACT_RELEASE);
    return 0;
}

static int btmw_rpc_test_rc_send_rwd_handler(int argc, char *argv[])
{
    BTMW_RPC_TEST_Logi("[AVRCP] %s() \n", __func__);
    a_mtkapi_avrcp_passthrough_cmd(BT_CMD_TYPE_RWD, BUTTON_ACT_PRESS);
    sleep(1);
    a_mtkapi_avrcp_passthrough_cmd(BT_CMD_TYPE_RWD, BUTTON_ACT_RELEASE);

    return 0;
}

static int btmw_rpc_test_rc_send_volumeup_handler(int argc, char *argv[])
{
    BTMW_RPC_TEST_Logi("[AVRCP] %s() \n", __func__);

    a_mtkapi_avrcp_passthrough_cmd(BT_CMD_TYPE_VOLUME_UP, BUTTON_ACT_PRESS);
    sleep(1);
    a_mtkapi_avrcp_passthrough_cmd(BT_CMD_TYPE_VOLUME_UP, BUTTON_ACT_RELEASE);
    return 0;
}

static int btmw_rpc_test_rc_send_volumedown_handler(int argc, char *argv[])
{
    BTMW_RPC_TEST_Logi("[AVRCP] %s() \n", __func__);

    a_mtkapi_avrcp_passthrough_cmd(BT_CMD_TYPE_VOLUME_DOWN, BUTTON_ACT_PRESS);
    sleep(1);
    a_mtkapi_avrcp_passthrough_cmd(BT_CMD_TYPE_VOLUME_DOWN, BUTTON_ACT_RELEASE);
    return 0;
}

static int btmw_rpc_test_rc_send_next_group_handler(int argc, char *argv[])
{
    BTMW_RPC_TEST_Logi("[AVRCP] %s() \n", __func__);

    a_mtkapi_avrcp_passthrough_cmd(BT_CMD_TYPE_VOLUME_DOWN, BUTTON_ACT_PRESS);
    sleep(1);
    a_mtkapi_avrcp_passthrough_cmd(BT_CMD_TYPE_VOLUME_DOWN, BUTTON_ACT_RELEASE);
    return 0;
}

static int btmw_rpc_test_rc_send_prev_group_handler(int argc, char *argv[])
{
    BTMW_RPC_TEST_Logi("[AVRCP] %s() \n", __func__);

    return 0;
}

static int btmw_rpc_test_rc_send_pass_through_cmd_handler(int argc, char *argv[])
{
    return 0;
}

static int btmw_rpc_test_rc_get_mediaInfo_cmd_handler(int argc, char *argv[])
{
    avrcp_medioInfo_t mediaInfo;
    BTMW_RPC_TEST_Logi("[AVRCP] %s() \n", __func__);
    a_mtkapi_avrcp_get_mediaInfo(&mediaInfo);
    BTMW_RPC_TEST_Logi("media info: \n", __func__);
    BTMW_RPC_TEST_Logi("Title: %s\n", mediaInfo.title);
    BTMW_RPC_TEST_Logi("artist: %s\n", mediaInfo.artist);
    BTMW_RPC_TEST_Logi("album: %s\n", mediaInfo.album);
    BTMW_RPC_TEST_Logi("current_track_number: %d\n", mediaInfo.current_track_number);
    BTMW_RPC_TEST_Logi("number_of_tracks: %d\n", mediaInfo.number_of_tracks);
    BTMW_RPC_TEST_Logi("genre: %s\n", mediaInfo.genre);
    BTMW_RPC_TEST_Logi("position: %d\n", mediaInfo.position);
    return 0;
}

static int btmw_rpc_test_rc_get_play_status_cmd_handler(int argc, char *argv[])
{
    bt_player_status_values_t playStatus;
    BTMW_RPC_TEST_Logi("[AVRCP] %s() \n", __func__);
    a_mtkapi_avrcp_get_play_status(&playStatus);
    BTMW_RPC_TEST_Logi("playStatus: \n", __func__);
    BTMW_RPC_TEST_Logi("song_length: %d\n", playStatus.song_length);
    BTMW_RPC_TEST_Logi("song_position: %d\n", playStatus.song_position);
    BTMW_RPC_TEST_Logi("play_status: %d\n", playStatus.play_status);

    return 0;
}

static int btmw_rpc_test_rc_set_volume_cmd_handler(int argc, char *argv[])
{
    UINT8 u1volume = 0;
    u1volume = atoi(argv[0]);
    BTMW_RPC_TEST_Logi("[AVRCP] %s() volume=%d\n", __func__, u1volume);
    a_mtkapi_avrcp_set_volume(u1volume);
    return 0;
}

static BTMW_RPC_TEST_CLI btmw_rpc_test_rc_ct_cli_commands[] =
{
    {"play",             btmw_rpc_test_rc_send_play_handler,             " = send play command <addr>"},
    {"pause",            btmw_rpc_test_rc_send_pause_handler,            " = send pause command <addr>"},
    {"stop",             btmw_rpc_test_rc_send_stop_handler,             " = send stop command <addr>"},
    {"fwd",              btmw_rpc_test_rc_send_fwd_handler,              " = send fwd command <addr>"},
    {"bwd",              btmw_rpc_test_rc_send_bwd_handler,              " = send bwd command <addr>"},
    {"ffwd",             btmw_rpc_test_rc_send_ffwd_handler,             " = send ffwd command <addr>"},
    {"rwd",              btmw_rpc_test_rc_send_rwd_handler,              " = send rwd command <addr>"},
    {"volume_up",        btmw_rpc_test_rc_send_volumeup_handler,         " = send volume up command <addr>"},
    {"volume_down",      btmw_rpc_test_rc_send_volumedown_handler,       " = send volume down command <addr>"},
    {"next_group",       btmw_rpc_test_rc_send_next_group_handler,       " = send next group command <addr>"},
    {"prev_group",       btmw_rpc_test_rc_send_prev_group_handler,       " = send previous group command <addr>"},
    {"pass_through_cmd", btmw_rpc_test_rc_send_pass_through_cmd_handler, " = pass_through_cmd <addr> <key_code> enter 0 for Next and 1 for Previous navigation command"},
    {"get_media_info",   btmw_rpc_test_rc_get_mediaInfo_cmd_handler,     " = get_media_info"},
    {"get_play_status",  btmw_rpc_test_rc_get_play_status_cmd_handler,   " = get_play_status"},
    {"set_volume",       btmw_rpc_test_rc_set_volume_cmd_handler,        " = set_volume <volume:0~127> "},
    {NULL, NULL, NULL},
};

int btmw_rpc_test_rc_init(void)
{
    int ret = 0;
    BTMW_RPC_TEST_MOD avrcp_ct_mode = {0};

    avrcp_ct_mode.mod_id = BTMW_RPC_TEST_MOD_AVRCP_CT;
    strncpy(avrcp_ct_mode.cmd_key, BTMW_RPC_TEST_CMD_KEY_AVRCP_CT, sizeof(avrcp_ct_mode.cmd_key));
    avrcp_ct_mode.cmd_handler = btmw_rpc_test_rc_ct_cmd_handler;
    avrcp_ct_mode.cmd_tbl = btmw_rpc_test_rc_ct_cli_commands;

    ret = btmw_rpc_test_register_mod(&avrcp_ct_mode);
    BTMW_RPC_TEST_Logd("[AVRCP][CT] btmw_rpc_test_register_mod() for returns: %d\n", ret);

    return ret;
}

int btmw_rpc_test_rc_deinit(void)
{
    return 0;
}


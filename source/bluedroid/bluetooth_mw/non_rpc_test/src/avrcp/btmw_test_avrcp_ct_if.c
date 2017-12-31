#include <unistd.h>
#include "bt_mw_common.h"
#include "btmw_test_avrcp_if.h"
#include "c_bt_mw_avrcp.h"
#include "u_bt_mw_avrcp.h"

extern void *btmw_test_gap_get_profile_interface(const char *profile_id);
#if 0

static void btmw_test_rc_set_abs_vol_cb(BTAbsoluteVolumeType flag, UINT8 *bt_absolute_volume)
{
    BTMW_TEST_Logd("[AVRCP] %s() flag=%d, vol=%u", __func__, *bt_absolute_volume);
    *bt_absolute_volume = 100;
}


BT_APP_CB_FUNC g_btmw_test_rc_callbacks =
{
    NULL, /* BtAppEventCbk bt_event_cb; */
    NULL, /* BtAppPlayStateChangeCbk bt_play_change_cb; */
    NULL, /* BtAppVolumeChangeCbk bt_volume_change_cb; */
    NULL, /* BtAppTrackChangeCbk bt_track_change_cb */
    NULL, /* BtAppMediaStatusCbk bt_media_playstatus_cb; */
    NULL, /* BtAppSppCbk bt_spp_cb; */
    NULL, /* BtAppHidCbk bt_hid_cb; */
    btmw_test_rc_set_abs_vol_cb, /* BtAppAbsoluteVolumeCbk bt_absolute_volume_cb; */
    NULL, /* BtAppAVRCPCMDCbk bt_avrcp_cmd_cb; */
    NULL, /* BtAppUploadInfoCbk bt_upload_info_cb; */
    NULL, /* BtAppGetPlayStatusCbk bt_get_play_status_cb; */
    NULL, /* BtAppPlaybackCtrlCbk bt_playback_ctrl_cb; */
    NULL, /* BtAppResetAudioCbk bt_reset_audio_cb; */
    NULL, /* BtAppGetPairingKeyCbk bt_get_pairing_key_cb; */
};
#endif

static int btmw_test_rc_send_play_handler(int argc, char *argv[]);
static int btmw_test_rc_send_pause_handler(int argc, char *argv[]);
static int btmw_test_rc_send_stop_handler(int argc, char *argv[]);
static int btmw_test_rc_send_fwd_handler(int argc, char *argv[]);
static int btmw_test_rc_send_bwd_handler(int argc, char *argv[]);
static int btmw_test_rc_send_ffwd_handler(int argc, char *argv[]);
static int btmw_test_rc_send_rwd_handler(int argc, char *argv[]);
static int btmw_test_rc_send_volumeup_handler(int argc, char *argv[]);
static int btmw_test_rc_send_volumedown_handler(int argc, char *argv[]);
static int btmw_test_rc_send_next_group_handler(int argc, char *argv[]);
static int btmw_test_rc_send_prev_group_handler(int argc, char *argv[]);
static int btmw_test_rc_send_pass_through_cmd_handler(int argc, char *argv[]);
static int btmw_test_rc_send_get_playstatus_cmd_handler(int argc, char *argv[]);
static int btmw_test_rc_pts_enable_handler(int argc, char *argv[]);
static int btmw_test_rc_track_change_handler(int argc, char *argv[]);
static int btmw_test_rc_enable_large_element_attr_handler(int argc, char *argv[]);

static BTMW_TEST_CLI btmw_test_rc_ct_cli_commands[] =
{
    {"play",             btmw_test_rc_send_play_handler,             " = send play command <addr>"},
    {"pause",            btmw_test_rc_send_pause_handler,            " = send pause command <addr>"},
    {"stop",             btmw_test_rc_send_stop_handler,             " = send stop command <addr>"},
    {"fwd",              btmw_test_rc_send_fwd_handler,              " = send fwd command <addr>"},
    {"bwd",              btmw_test_rc_send_bwd_handler,              " = send bwd command <addr>"},
    {"ffwd",             btmw_test_rc_send_ffwd_handler,             " = send ffwd command <addr>"},
    {"rwd",              btmw_test_rc_send_rwd_handler,              " = send rwd command <addr>"},
    {"volume_up",        btmw_test_rc_send_volumeup_handler,         " = send volume up command <addr>"},
    {"volume_down",      btmw_test_rc_send_volumedown_handler,       " = send volume down command <addr>"},
    {"next_group",       btmw_test_rc_send_next_group_handler,       " = send next group command <addr>"},
    {"prev_group",       btmw_test_rc_send_prev_group_handler,       " = send previous group command <addr>"},
    {"pass_through_cmd", btmw_test_rc_send_pass_through_cmd_handler, " = pass_through_cmd <addr> <key_code> enter 0 for Next and 1 for Previous navigation command"},
    {"get_playstatus",   btmw_test_rc_send_get_playstatus_cmd_handler,   " = get_playstatus <addr>"},
    {"pts_enable",       btmw_test_rc_pts_enable_handler,            " = avrcp pts enable, 0-disable, 1-enable"},
    {"track_change",     btmw_test_rc_track_change_handler,          " = track_change <selected:0/1>"},
    {"large_element",    btmw_test_rc_enable_large_element_attr_handler, " = large_element <enable:0/1>"},
    {NULL, NULL, NULL},
};

int btmw_test_rc_ct_cmd_handler(int argc, char **argv)
{
    BTMW_TEST_CLI *cmd, *match = NULL;
    int ret = 0;
    int count = 0;

    BTMW_TEST_Logi("[AVRCP] CT argc: %d, argv[0]: %s\n", argc, argv[0]);

    cmd = btmw_test_rc_ct_cli_commands;
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
        btmw_test_print_cmd_help(BTMW_TEST_CMD_KEY_AVRCP_CT, btmw_test_rc_ct_cli_commands);
        ret = -1;
    }
    else
    {
        match->handler(argc - 1, &argv[1]);
    }

    return ret;
}

static int btmw_test_rc_send_play_handler(int argc, char *argv[])
{
    BTMW_TEST_Logi("[AVRCP] %s() \n", __func__);
    if (argc > 0)
    {
        BTMW_TEST_Loge("[USAGE] %s play", BTMW_TEST_CMD_KEY_AVRCP_CT);
        return -1;
    }

    c_btm_avrcp_passthrough_cmd(BT_CMD_TYPE_PLAY, BUTTON_ACT_PRESS);
    sleep(1);
    c_btm_avrcp_passthrough_cmd(BT_CMD_TYPE_PLAY, BUTTON_ACT_RELEASE);

    return 0;
}

static int btmw_test_rc_send_pause_handler(int argc, char *argv[])
{
    BTMW_TEST_Logi("[AVRCP] %s() \n", __func__);
    if (argc > 0)
    {
        BTMW_TEST_Loge("[USAGE] %s pause", BTMW_TEST_CMD_KEY_AVRCP_CT);
        return -1;
    }

    c_btm_avrcp_passthrough_cmd(BT_CMD_TYPE_PAUSE, BUTTON_ACT_PRESS);
    sleep(1);
    c_btm_avrcp_passthrough_cmd(BT_CMD_TYPE_PAUSE, BUTTON_ACT_RELEASE);

    return 0;
}

static int btmw_test_rc_send_stop_handler(int argc, char *argv[])
{
    BTMW_TEST_Logi("[AVRCP] %s() \n", __func__);
    if (argc > 0)
    {
        BTMW_TEST_Loge("[USAGE] %s stop", BTMW_TEST_CMD_KEY_AVRCP_CT);
        return -1;
    }

    c_btm_avrcp_passthrough_cmd(BT_CMD_TYPE_STOP, BUTTON_ACT_PRESS);
    sleep(1);
    c_btm_avrcp_passthrough_cmd(BT_CMD_TYPE_STOP, BUTTON_ACT_RELEASE);

    return 0;
}

static int btmw_test_rc_send_fwd_handler(int argc, char *argv[])
{
    BTMW_TEST_Logi("[AVRCP] %s() \n", __func__);
    if (argc > 0)
    {
        BTMW_TEST_Loge("[USAGE] %s fwd", BTMW_TEST_CMD_KEY_AVRCP_CT);
        return -1;
    }

    c_btm_avrcp_passthrough_cmd(BT_CMD_TYPE_FWD, BUTTON_ACT_PRESS);
    sleep(1);
    c_btm_avrcp_passthrough_cmd(BT_CMD_TYPE_FWD, BUTTON_ACT_RELEASE);
    return 0;
}

static int btmw_test_rc_send_bwd_handler(int argc, char *argv[])
{
    BTMW_TEST_Logi("[AVRCP] %s() \n", __func__);
    if (argc > 0)
    {
        BTMW_TEST_Loge("[USAGE] %s bwd", BTMW_TEST_CMD_KEY_AVRCP_CT);
        return -1;
    }

    c_btm_avrcp_passthrough_cmd(BT_CMD_TYPE_BWD, BUTTON_ACT_PRESS);
    sleep(1);
    c_btm_avrcp_passthrough_cmd(BT_CMD_TYPE_BWD, BUTTON_ACT_RELEASE);
    return 0;
}

static int btmw_test_rc_send_ffwd_handler(int argc, char *argv[])
{
    BTMW_TEST_Logi("[AVRCP] %s() \n", __func__);
    if (argc > 0)
    {
        BTMW_TEST_Loge("[USAGE] %s ffwd", BTMW_TEST_CMD_KEY_AVRCP_CT);
        return -1;
    }

    c_btm_avrcp_passthrough_cmd(BT_CMD_TYPE_FFWD, BUTTON_ACT_PRESS);
    sleep(1);
    c_btm_avrcp_passthrough_cmd(BT_CMD_TYPE_FFWD, BUTTON_ACT_RELEASE);
    return 0;
}

static int btmw_test_rc_send_rwd_handler(int argc, char *argv[])
{
    BTMW_TEST_Logi("[AVRCP] %s() \n", __func__);
    if (argc > 0)
    {
        BTMW_TEST_Loge("[USAGE] %s rwd", BTMW_TEST_CMD_KEY_AVRCP_CT);
        return -1;
    }

    c_btm_avrcp_passthrough_cmd(BT_CMD_TYPE_RWD, BUTTON_ACT_PRESS);
    sleep(1);
    c_btm_avrcp_passthrough_cmd(BT_CMD_TYPE_RWD, BUTTON_ACT_RELEASE);

    return 0;
}

static int btmw_test_rc_send_volumeup_handler(int argc, char *argv[])
{
    BTMW_TEST_Logi("[AVRCP] %s() \n", __func__);
    if (argc > 0)
    {
        BTMW_TEST_Loge("[USAGE] %s volume_up", BTMW_TEST_CMD_KEY_AVRCP_CT);
        return -1;
    }

    c_btm_avrcp_passthrough_cmd(BT_CMD_TYPE_VOLUME_UP, BUTTON_ACT_PRESS);
    sleep(1);
    c_btm_avrcp_passthrough_cmd(BT_CMD_TYPE_VOLUME_UP, BUTTON_ACT_RELEASE);
    return 0;
}

static int btmw_test_rc_send_volumedown_handler(int argc, char *argv[])
{

    BTMW_TEST_Logi("[AVRCP] %s() \n", __func__);
    if (argc > 0)
    {
        BTMW_TEST_Loge("[USAGE] %s volume_down", BTMW_TEST_CMD_KEY_AVRCP_CT);
        return -1;
    }

    c_btm_avrcp_passthrough_cmd(BT_CMD_TYPE_VOLUME_DOWN, BUTTON_ACT_PRESS);
    sleep(1);
    c_btm_avrcp_passthrough_cmd(BT_CMD_TYPE_VOLUME_DOWN, BUTTON_ACT_RELEASE);
    return 0;
}

static int btmw_test_rc_send_next_group_handler(int argc, char *argv[])
{
    BTMW_TEST_Logi("[AVRCP] %s() \n", __func__);
    if (1)
    {
        BTMW_TEST_Loge("this CLI not supported!");
        return -1;
    }

    c_btm_avrcp_passthrough_cmd(BT_CMD_TYPE_VOLUME_DOWN, BUTTON_ACT_PRESS);
    sleep(1);
    c_btm_avrcp_passthrough_cmd(BT_CMD_TYPE_VOLUME_DOWN, BUTTON_ACT_RELEASE);
    return 0;
}

static int btmw_test_rc_send_prev_group_handler(int argc, char *argv[])
{
    BTMW_TEST_Logi("[AVRCP] %s() \n", __func__);
    if (1)
    {
        BTMW_TEST_Loge("this CLI not supported!");
        return -1;
    }

    return 0;
}

static int btmw_test_rc_send_pass_through_cmd_handler(int argc, char *argv[])
{
    return 0;
}

static int btmw_test_rc_send_get_playstatus_cmd_handler(int argc, char *argv[])
{
    c_btm_avrcp_send_get_playstatus_cmd();
    return 0;
}

static int btmw_test_rc_pts_enable_handler(int argc, char *argv[])
{
    c_btm_send_pts_test_handler(atoi(argv[0]));
    return 0;
}

static int btmw_test_rc_track_change_handler(int argc, char *argv[])
{
    c_btm_send_track_change_rsp_handler(atoi(argv[0]));
    return 0;
}

static int btmw_test_rc_enable_large_element_attr_handler(int argc, char *argv[])
{
    c_btm_set_large_element_attribute_response_handler(atoi(argv[0]));
    return 0;
}





int btmw_test_rc_init(void)
{
    int ret = 0;
    BTMW_TEST_MOD avrcp_ct_mode = {0};

    avrcp_ct_mode.mod_id = BTMW_TEST_MOD_AVRCP_CT;
    strncpy(avrcp_ct_mode.cmd_key, BTMW_TEST_CMD_KEY_AVRCP_CT, sizeof(avrcp_ct_mode.cmd_key));
    avrcp_ct_mode.cmd_handler = btmw_test_rc_ct_cmd_handler;
    avrcp_ct_mode.cmd_tbl = btmw_test_rc_ct_cli_commands;

    ret = btmw_test_register_mod(&avrcp_ct_mode);
    BTMW_TEST_Logd("[AVRCP][CT] btmw_test_register_mod() for returns: %d\n", ret);

    return ret;
}

int btmw_test_rc_deinit(void)
{
    return 0;
}


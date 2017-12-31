#include <string.h>
#include "btmw_rpc_test_cli.h"
#include "btmw_rpc_test_debug.h"
#include "btmw_rpc_test_avrcp_tg_if.h"

#if 0

static void btmw_rpc_test_rc_tg_set_abs_vol_cb(BTAbsoluteVolumeType flag, UINT8 *bt_absolute_volume);
static void btmw_rpc_test_rc_tg_get_play_status_cb(BTPlayState *bt_playstatus);
static void btmw_rpc_test_rc_tg_volume_change_cb(BTVolType bt_volume);

BT_APP_CB_FUNC g_btmw_rpc_test_rc_tg_callbacks =
{
    NULL, /* BtAppEventCbk bt_event_cb; */
    NULL, /* BtAppPlayStateChangeCbk bt_play_change_cb; */
    btmw_rpc_test_rc_tg_volume_change_cb, /* BtAppVolumeChangeCbk bt_volume_change_cb; */
    NULL, /* BtAppTrackChangeCbk bt_track_change_cb */
    NULL, /* BtAppMediaStatusCbk bt_media_playstatus_cb; */
    NULL, /* BtAppSppCbk bt_spp_cb; */
    NULL, /* BtAppHidCbk bt_hid_cb; */
    btmw_rpc_test_rc_tg_set_abs_vol_cb, /* BtAppAbsoluteVolumeCbk bt_absolute_volume_cb; */
    NULL, /* BtAppAVRCPCMDCbk bt_avrcp_cmd_cb; */
    NULL, /* BtAppUploadInfoCbk bt_upload_info_cb; */
    btmw_rpc_test_rc_tg_get_play_status_cb, /* BtAppGetPlayStatusCbk bt_get_play_status_cb; */
    NULL, /* BtAppPlaybackCtrlCbk bt_playback_ctrl_cb; */
    NULL, /* BtAppResetAudioCbk bt_reset_audio_cb; */
    NULL, /* BtAppGetPairingKeyCbk bt_get_pairing_key_cb; */
};

static void btmw_rpc_test_rc_tg_set_abs_vol_cb(BTAbsoluteVolumeType flag, UINT8 *bt_absolute_volume)
{
    BTMW_RPC_TEST_Logd("[AVRCP] %s() flag=%d, vol=%u", __func__, *bt_absolute_volume);
    *bt_absolute_volume = 100;
}

static void btmw_rpc_test_rc_tg_get_play_status_cb(BTPlayState *bt_playstatus)
{
    BTMW_RPC_TEST_Logd("[AVRCP] %s() ",__func__);
    *bt_playstatus = BT_PLAY_STATUS_FWD_SEEK;
}

static void btmw_rpc_test_rc_tg_volume_change_cb(BTVolType bt_volume)
{
    BTMW_RPC_TEST_Logd("[AVRCP] %s() volume = %d", __func__, bt_volume);
}
#endif

static BTMW_RPC_TEST_CLI btmw_rpc_test_rc_tg_cli_commands[] =
{
    {NULL, NULL, NULL},
};

int btmw_rpc_test_rc_tg_cmd_handler(int argc, char **argv)
{
    BTMW_RPC_TEST_CLI *cmd, *match = NULL;
    int ret = 0;
    int count = 0;

    BTMW_RPC_TEST_Logd("[AVRCP] argc: %d, argv[0]: %s\n", argc, argv[0]);

    cmd = btmw_rpc_test_rc_tg_cli_commands;
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
        BTMW_RPC_TEST_Logd("[AVRCP] Unknown command '%s'\n", argv[0]);
        btmw_rpc_test_print_cmd_help(BTMW_RPC_TEST_CMD_KEY_AVRCP_TG, btmw_rpc_test_rc_tg_cli_commands);
        ret = -1;
    }
    else
    {
        match->handler(argc - 1, &argv[1]);
    }

    return ret;
}


int btmw_rpc_test_rc_tg_init()
{
    int ret = 0;
    BTMW_RPC_TEST_MOD rc_mod = {0};

    BTMW_RPC_TEST_Logd("[AVRCP] %s() \n", __func__);

    rc_mod.mod_id = BTMW_RPC_TEST_MOD_AVRCP_TG;
    strncpy(rc_mod.cmd_key, BTMW_RPC_TEST_CMD_KEY_AVRCP_TG, sizeof(rc_mod.cmd_key));
    rc_mod.cmd_handler = btmw_rpc_test_rc_tg_cmd_handler;
    rc_mod.cmd_tbl = btmw_rpc_test_rc_tg_cli_commands;

    ret = btmw_rpc_test_register_mod(&rc_mod);
    BTMW_RPC_TEST_Logd("[AVRCP] btmw_rpc_test_register_mod() for TG returns: %d\n", ret);


    return ret;
}

int btmw_rpc_test_rc_tg_deinit()
{
    BTMW_RPC_TEST_Logd("[AVRCP] %s() \n", __func__);

    return 0;
}

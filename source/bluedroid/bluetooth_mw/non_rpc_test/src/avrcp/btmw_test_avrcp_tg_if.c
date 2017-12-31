#include <string.h>
#include <stdlib.h>

#include "btmw_test_avrcp_tg_if.h"
#include "u_bluetooth.h"
#include "c_bt_mw_avrcp.h"

static int btmw_test_rc_tg_set_volume_handler(int argc, char *argv[])
{
    UINT8 volume = 0;

    BTMW_TEST_Logi("[AVRCP] %s() \n", __func__);

    if (1 == argc)
    {
        volume = atoi(argv[0]);
        BTMW_TEST_Logi("[AVRCP] set the volume:%d\n", volume);
    }
    else
    {
        volume = 50;
        BTMW_TEST_Logi("[AVRCP] use the default volume value:50\n");
    }

    c_btm_set_volume(volume);

    return 0;
}

static BTMW_TEST_CLI btmw_test_rc_tg_cli_commands[] =
{
    {"set_volume",             btmw_test_rc_tg_set_volume_handler,             " = send set_volume command <addr>"},
    {NULL, NULL, NULL},
};

int btmw_test_rc_tg_cmd_handler(int argc, char **argv)
{
    BTMW_TEST_CLI *cmd, *match = NULL;
    int ret = 0;
    int count = 0;

    BTMW_TEST_Logd("[AVRCP] argc: %d, argv[0]: %s\n", argc, argv[0]);

    cmd = btmw_test_rc_tg_cli_commands;
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
        BTMW_TEST_Logd("[AVRCP] Unknown command '%s'\n", argv[0]);
        btmw_test_print_cmd_help(BTMW_TEST_CMD_KEY_AVRCP_TG, btmw_test_rc_tg_cli_commands);
        ret = -1;
    }
    else
    {
        match->handler(argc - 1, &argv[1]);
    }

    return ret;
}


int btmw_test_rc_tg_init()
{
    int ret = 0;
    BTMW_TEST_MOD rc_mod = {0};

    BTMW_TEST_Logd("[AVRCP] %s() \n", __func__);

    rc_mod.mod_id = BTMW_TEST_MOD_AVRCP_TG;
    strncpy(rc_mod.cmd_key, BTMW_TEST_CMD_KEY_AVRCP_TG, sizeof(rc_mod.cmd_key));
    rc_mod.cmd_handler = btmw_test_rc_tg_cmd_handler;
    rc_mod.cmd_tbl = btmw_test_rc_tg_cli_commands;

    ret = btmw_test_register_mod(&rc_mod);
    BTMW_TEST_Logd("[AVRCP] btmw_test_register_mod() for TG returns: %d\n", ret);

    return ret;
}

int btmw_test_rc_tg_deinit()
{
    BTMW_TEST_Logd("[AVRCP] %s() \n", __func__);

    return 0;
}

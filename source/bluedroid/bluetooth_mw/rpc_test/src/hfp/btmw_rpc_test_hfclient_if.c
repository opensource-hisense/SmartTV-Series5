#include <string.h>

#include "btmw_rpc_test_cli.h"
#include "btmw_rpc_test_debug.h"
#include "btmw_rpc_test_hfclient_if.h"
#include "mtk_bt_service_hfclient_wrapper.h"

#define MAX_HFCLIENT_NUMBER_LEN 32

static int btmw_rpc_test_hfclient_connect(int argc, char **argv);
static int btmw_rpc_test_hfclient_disconnect(int argc, char **argv);
static int btmw_rpc_test_hfclient_connect_audio(int argc, char **argv);
static int btmw_rpc_test_hfclient_disconnect_audio(int argc, char **argv);
static int btmw_rpc_test_hfclient_start_vr(int argc, char **argv);
static int btmw_rpc_test_hfclient_stop_vr(int argc, char **argv);
static int btmw_rpc_test_hfclient_volume_control(int argc, char **argv);
static int btmw_rpc_test_hfclient_dial(int argc, char **argv);
static int btmw_rpc_test_hfclient_dial_memory(int argc, char **argv);
static int btmw_rpc_test_hfclient_call_action(int argc, char **argv);
static int btmw_rpc_test_hfclient_query_calls(int argc, char **argv);
static int btmw_rpc_test_hfclient_query_operator(int argc, char **argv);
static int btmw_rpc_test_hfclient_retrieve_subscriber(int argc, char **argv);
static int btmw_rpc_test_hfclient_dtmf(int argc, char **argv);
static int btmw_rpc_test_hfclient_request_voice_tag(int argc, char **argv);
static int btmw_rpc_test_hfclient_atcmd(int argc, char **argv);

static BTMW_RPC_TEST_CLI btmw_rpc_test_hfclient_cli_commands[] =
{
    {(const char *)"connect",              btmw_rpc_test_hfclient_connect,              (const char *)" = connect <addr>"},
    {(const char *)"disconnect",           btmw_rpc_test_hfclient_disconnect,           (const char *)" = disconnect <addr>"},
    {(const char *)"connect_audio",        btmw_rpc_test_hfclient_connect_audio,        (const char *)" = connect_audio <addr>"},
    {(const char *)"disconnect_audio",     btmw_rpc_test_hfclient_disconnect_audio,     (const char *)" = disconnect_audio <addr>"},
    {(const char *)"start_vr",             btmw_rpc_test_hfclient_start_vr,             (const char *)" = start_vr"},
    {(const char *)"stop_vr",              btmw_rpc_test_hfclient_stop_vr,              (const char *)" = stop_vr"},
    {(const char *)"volume_control",       btmw_rpc_test_hfclient_volume_control,       (const char *)" = volume_control <type> <volume>"},
    {(const char *)"dial",                 btmw_rpc_test_hfclient_dial,                 (const char *)" = dial [<number>]"},
    {(const char *)"dial_memory",          btmw_rpc_test_hfclient_dial_memory,          (const char *)" = dial_memory <location>"},
    {(const char *)"call_action",          btmw_rpc_test_hfclient_call_action,          (const char *)" = call_action <action> <idx>"},
    {(const char *)"query_calls",          btmw_rpc_test_hfclient_query_calls,          (const char *)" = query_calls"},
    {(const char *)"query_operator",       btmw_rpc_test_hfclient_query_operator,       (const char *)" = query_operator"},
    {(const char *)"retrieve_subscriber",  btmw_rpc_test_hfclient_retrieve_subscriber,  (const char *)" = retrieve_subscriber"},
    {(const char *)"dtmf",                 btmw_rpc_test_hfclient_dtmf,                 (const char *)" = dtmf <code>"},
    {(const char *)"request_voice_tag",    btmw_rpc_test_hfclient_request_voice_tag,    (const char *)" = request_voice_tag"},
    {(const char *)"atcmd",                btmw_rpc_test_hfclient_atcmd,                (const char *)" = atcmd <cmd> <val1> <val2> [<arg>]"},
    {NULL, NULL, NULL},
};

int btmw_rpc_test_hfclient_cmd_handler(int argc, char **argv)
{
    BTMW_RPC_TEST_CLI *cmd, *match = NULL;
    int ret = 0;
    int count;

    count = 0;
    cmd = btmw_rpc_test_hfclient_cli_commands;

    BTMW_RPC_TEST_Logi("[HFCLIENT] argc: %d, argv[0]: %s\n", argc, argv[0]);
    if (NULL == argv[0])
    {
        BTMW_RPC_TEST_Logi("[HFCLIENT] Unknown command\n");
        btmw_rpc_test_print_cmd_help(BTMW_RPC_TEST_CMD_KEY_HFCLIENT, btmw_rpc_test_hfclient_cli_commands);
        return -1;
    }

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
        BTMW_RPC_TEST_Logi("[HFCLIENT] Unknown command '%s'\n", argv[0]);
        btmw_rpc_test_print_cmd_help(BTMW_RPC_TEST_CMD_KEY_HFCLIENT, btmw_rpc_test_hfclient_cli_commands);
        ret = -1;
    }
    else
    {
        match->handler(argc - 1, &argv[1]);
    }

    return ret;
}

int btmw_rpc_test_hfclient_init()
{
    int ret = 0;
    BTMW_RPC_TEST_MOD hfclient_mod = {0};

    hfclient_mod.mod_id = BTMW_RPC_TEST_MOD_HFCLIENT;
    strncpy(hfclient_mod.cmd_key, BTMW_RPC_TEST_CMD_KEY_HFCLIENT, sizeof(hfclient_mod.cmd_key));
    hfclient_mod.cmd_handler = btmw_rpc_test_hfclient_cmd_handler;
    hfclient_mod.cmd_tbl = btmw_rpc_test_hfclient_cli_commands;

    ret = btmw_rpc_test_register_mod(&hfclient_mod);
    BTMW_RPC_TEST_Logi("[HFCLIENT] btmw_rpc_test_register_mod() returns: %d\n", ret);
    return ret;
}

int btmw_rpc_test_hfclient_deinit()
{
    BTMW_RPC_TEST_Logi("%s", __func__);
    return 0;
}

static int btmw_rpc_test_hfclient_connect(int argc, char **argv)
{
    CHAR bt_addr[MAX_BDADDR_LEN] = {0};

    BTMW_RPC_TEST_Logi("[HFCLIENT] %s()\n", __func__);

    if (argc < 1)
    {
        BTMW_RPC_TEST_Logi("Usage :\n");
        BTMW_RPC_TEST_Logi("  HFCLIENT connect <addr>\n");
        return -1;
    }

    strncpy(bt_addr, argv[0], MAX_BDADDR_LEN);
    bt_addr[MAX_BDADDR_LEN - 1] = '\0';
    return a_mtkapi_bt_hfclient_connect(bt_addr);
}

static int btmw_rpc_test_hfclient_disconnect(int argc, char **argv)
{
    CHAR bt_addr[MAX_BDADDR_LEN] = {0};

    BTMW_RPC_TEST_Logi("[HFCLIENT] %s()\n", __func__);

    if (argc < 1)
    {
        BTMW_RPC_TEST_Logi("Usage :\n");
        BTMW_RPC_TEST_Logi("  HFCLIENT disconnect <addr>\n");
        return -1;
    }

    strncpy(bt_addr, argv[0], MAX_BDADDR_LEN);
    bt_addr[MAX_BDADDR_LEN - 1] = '\0';
    return a_mtkapi_bt_hfclient_disconnect(bt_addr);
}

static int btmw_rpc_test_hfclient_connect_audio(int argc, char **argv)
{
    CHAR bt_addr[MAX_BDADDR_LEN] = {0};

    BTMW_RPC_TEST_Logi("[HFCLIENT] %s()\n", __func__);

    if (argc < 1)
    {
        BTMW_RPC_TEST_Logi("Usage :\n");
        BTMW_RPC_TEST_Logi("  HFCLIENT connect_audio <addr>\n");
        return -1;
    }

    strncpy(bt_addr, argv[0], MAX_BDADDR_LEN);
    bt_addr[MAX_BDADDR_LEN - 1] = '\0';
    return a_mtkapi_bt_hfclient_connect_audio(bt_addr);
}

static int btmw_rpc_test_hfclient_disconnect_audio(int argc, char **argv)
{
    CHAR bt_addr[MAX_BDADDR_LEN] = {0};

    BTMW_RPC_TEST_Logi("[HFCLIENT] %s()\n", __func__);

    if (argc < 1)
    {
        BTMW_RPC_TEST_Logi("Usage :\n");
        BTMW_RPC_TEST_Logi("  HFCLIENT disconnect_audio <addr>\n");
        return -1;
    }

    strncpy(bt_addr, argv[0], MAX_BDADDR_LEN);
    bt_addr[MAX_BDADDR_LEN - 1] = '\0';
    return a_mtkapi_bt_hfclient_disconnect_audio(bt_addr);
}

static int btmw_rpc_test_hfclient_start_vr(int argc, char **argv)
{
    BTMW_RPC_TEST_Logi("[HFCLIENT] %s()\n", __func__);
    return a_mtkapi_bt_hfclient_start_voice_recognition();
}

static int btmw_rpc_test_hfclient_stop_vr(int argc, char **argv)
{
    BTMW_RPC_TEST_Logi("[HFCLIENT] %s()\n", __func__);
    return a_mtkapi_bt_hfclient_stop_voice_recognition();
}

static int btmw_rpc_test_hfclient_volume_control(int argc, char **argv)
{
    INT32 type = 0;
    INT32 volume = 0;

    BTMW_RPC_TEST_Logi("[HFCLIENT] %s()\n", __func__);

    if (argc != 2)
    {
        BTMW_RPC_TEST_Logi("Usage :\n");
        BTMW_RPC_TEST_Logi("  HFCLIENT volume_control <type> <volume>\n");
        return -1;
    }

    type = atoi(argv[0]);
    volume = atoi(argv[1]);
    return a_mtkapi_bt_hfclient_volume_control(type, volume);
}

static int btmw_rpc_test_hfclient_dial(int argc, char **argv)
{
    CHAR number[MAX_HFCLIENT_NUMBER_LEN + 1] = {0};

    BTMW_RPC_TEST_Logi("[HFCLIENT] %s()\n", __func__);

    if (0 == argc)
    {
        return a_mtkapi_bt_hfclient_dial(NULL);
    }

    if (argc != 1)
    {
        BTMW_RPC_TEST_Logi("Usage :\n");
        BTMW_RPC_TEST_Logi("  HFCLIENT dial <number>\n");
        return -1;
    }

    strncpy(number, argv[0], MAX_HFCLIENT_NUMBER_LEN);
    number[MAX_HFCLIENT_NUMBER_LEN] = '\0';
    return a_mtkapi_bt_hfclient_dial(number);
}

static int btmw_rpc_test_hfclient_dial_memory(int argc, char **argv)
{
    INT32 location = 0;

    BTMW_RPC_TEST_Logi("[HFCLIENT] %s()\n", __func__);

    if (argc != 1)
    {
        BTMW_RPC_TEST_Logi("Usage :\n");
        BTMW_RPC_TEST_Logi("  HFCLIENT dial_memory <location>\n");
        return -1;
    }

    location = atoi(argv[0]);
    return a_mtkapi_bt_hfclient_dial_memory(location);
}

static int btmw_rpc_test_hfclient_call_action(int argc, char **argv)
{
    INT32 action = 0;
    INT32 idx = 0;

    BTMW_RPC_TEST_Logi("[HFCLIENT] %s()\n", __func__);

    if (argc != 2)
    {
        BTMW_RPC_TEST_Logi("Usage :\n");
        BTMW_RPC_TEST_Logi("  HFCLIENT call_action <action> <idx>\n");
        return -1;
    }

    action = atoi(argv[0]);
    idx = atoi(argv[1]);
    return a_mtkapi_bt_hfclient_handle_call_action(action, idx);
}

static int btmw_rpc_test_hfclient_query_calls(int argc, char **argv)
{
    BTMW_RPC_TEST_Logi("[HFCLIENT] %s()\n", __func__);
    return a_mtkapi_bt_hfclient_query_current_calls();
}

static int btmw_rpc_test_hfclient_query_operator(int argc, char **argv)
{
    BTMW_RPC_TEST_Logi("[HFCLIENT] %s()\n", __func__);
    return a_mtkapi_bt_hfclient_query_current_operator_name();
}

static int btmw_rpc_test_hfclient_retrieve_subscriber(int argc, char **argv)
{
    BTMW_RPC_TEST_Logi("[HFCLIENT] %s()\n", __func__);
    return a_mtkapi_bt_hfclient_retrieve_subscriber_info();
}

static int btmw_rpc_test_hfclient_dtmf(int argc, char **argv)
{
    CHAR code = 0;

    BTMW_RPC_TEST_Logi("[HFCLIENT] %s()\n", __func__);

    if (argc != 1)
    {
        BTMW_RPC_TEST_Logi("Usage :\n");
        BTMW_RPC_TEST_Logi("  HFCLIENT dtmf <code>\n");
        return -1;
    }

    code = argv[0][0];
    return a_mtkapi_bt_hfclient_send_dtmf(code);
}

static int btmw_rpc_test_hfclient_request_voice_tag(int argc, char **argv)
{
    BTMW_RPC_TEST_Logi("[HFCLIENT] %s()\n", __func__);
    return a_mtkapi_bt_hfclient_request_last_voice_tag_number();
}

static int btmw_rpc_test_hfclient_atcmd(int argc, char **argv)
{
    INT32 len = 0;
    INT32 cmd = 0;
    INT32 val1 = 0;
    INT32 val2 = 0;
    CHAR arg[MAX_HFCLIENT_NUMBER_LEN + 1] = {0};

    BTMW_RPC_TEST_Logi("[HFCLIENT] %s()\n", __func__);

    if ((argc != 3) && (argc != 4))
    {
        BTMW_RPC_TEST_Logi("Usage :\n");
        BTMW_RPC_TEST_Logi("  HFCLIENT atcmd <cmd> <val1> <val2> [<arg>]\n");
        return -1;
    }

    cmd = atoi(argv[0]);
    val1 = atoi(argv[1]);
    val2 = atoi(argv[2]);
    if (argv[3])
    {
        len = strlen(argv[3]);
        if (len > MAX_HFCLIENT_NUMBER_LEN)
        {
            BTMW_RPC_TEST_Logi("  HFCLIENT max <arg> length is %d\n", MAX_HFCLIENT_NUMBER_LEN);
            return -1;
        }
        strncpy(arg, argv[3], len);
        arg[len] = '\0';
    }

    return a_mtkapi_bt_hfclient_send_at_cmd(cmd, val1, val2, arg);
}

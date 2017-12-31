#ifndef __BTMW_RPC_TEST_CLI_H__
#define __BTMW_RPC_TEST_CLI_H__

#define BTMW_RPC_TEST_SUCCESS 0
#define BTMW_RPC_TEST_FAILED (-1)

#define BTMW_RPC_TEST_MAX_ARGS 25
#define BTMW_RPC_TEST_MAX_KEY_LEN    32

#define BTMW_RPC_TEST_MAX_PATH_LEN   256
#define BTMW_RPC_TEST_HISTORY_FILE   ".btmw_rpc_test_history"

#define BTMW_RPC_TEST_MAX_MODULES    40

typedef struct btmw_rpc_test_cli_t
{
    const char *cmd;
    int (*handler)(int argc, char *argv[]);
    const char *usage;
} BTMW_RPC_TEST_CLI;

typedef int (*BTMW_RPC_TEST_CMD_HANDLER)(int argc, char **argv);

enum
{
    BTMW_RPC_TEST_MOD_GAP = 0,
    BTMW_RPC_TEST_MOD_HFCLIENT,
    BTMW_RPC_TEST_MOD_A2DP_SINK,
    BTMW_RPC_TEST_MOD_A2DP_SRC,
    BTMW_RPC_TEST_MOD_GATT_CLIENT,
    BTMW_RPC_TEST_MOD_GATT_SERVER,
    BTMW_RPC_TEST_MOD_HID,
    BTMW_RPC_TEST_MOD_AVRCP_CT,
    BTMW_RPC_TEST_MOD_AVRCP_TG,
    BTMW_RPC_TEST_MOD_SPP,
    BTMW_RPC_TEST_MOD_CONF,
    BTMW_RPC_TEST_MOD_TOOLS,
    BTMW_RPC_TEST_MOD_NUM,
};

typedef struct btmw_rpc_test_mod_t
{
    int mod_id;
    char cmd_key[BTMW_RPC_TEST_MAX_KEY_LEN];
    BTMW_RPC_TEST_CMD_HANDLER cmd_handler;
    BTMW_RPC_TEST_CLI *cmd_tbl;
} BTMW_RPC_TEST_MOD;

extern void btmw_rpc_test_print_cmd_help(const char* prefix, BTMW_RPC_TEST_CLI *tbl);
extern int btmw_rpc_test_register_mod(BTMW_RPC_TEST_MOD *mod);
extern int g_cli_pts_mode;

#endif

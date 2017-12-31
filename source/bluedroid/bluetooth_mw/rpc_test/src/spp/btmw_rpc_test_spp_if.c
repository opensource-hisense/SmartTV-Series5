#include <string.h>

#include "btmw_rpc_test_cli.h"
#include "btmw_rpc_test_debug.h"
#include "btmw_rpc_test_spp_if.h"
#include "mtk_bt_service_spp_wrapper.h"
#include "u_bt_mw_common.h"

int btmw_rpc_test_spp_connect(int argc, char **argv)
{
    CHAR ps_target_mac[18];
    CHAR uuid[48];
    int i4_ret = 0;

    if (2 == argc)
    {
        memset(ps_target_mac, 0, sizeof(ps_target_mac));
        memset(uuid, 0, sizeof(uuid));
        if (17 != strlen(argv[0]))
        {
            BTMW_RPC_TEST_Logd("mac length should be 17\n");
            return BT_ERR_STATUS_PARM_INVALID;
        }
        strncpy(ps_target_mac, argv[0], sizeof(ps_target_mac)-1);
        strncpy(uuid, argv[1], strlen(argv[1]));
    }
    else
    {
        BTMW_RPC_TEST_Logd("please input spp_connect [MAC address][UUID]\n");
        return BT_ERR_STATUS_INVALID_PARM_NUMS;
    }

    i4_ret = a_mtkapi_spp_connect(ps_target_mac, uuid);
    BTMW_RPC_TEST_Logd("i4_ret=%d\n", i4_ret);
    return BT_SUCCESS;
}

int btmw_rpc_test_spp_disconnect(int argc, char **argv)
{
    CHAR ps_target_mac[18];
    CHAR uuid[48];
    int i4_ret = 0;

    if (2 == argc)
    {
        memset(ps_target_mac, 0, sizeof(ps_target_mac));
        memset(uuid, 0, sizeof(uuid));
        if (17 != strlen(argv[0]))
        {
            BTMW_RPC_TEST_Logd("mac length should be 17\n");
            return BT_ERR_STATUS_PARM_INVALID;
        }
        strncpy(ps_target_mac, argv[0], sizeof(ps_target_mac)-1);
        strncpy(uuid, argv[1], strlen(argv[1]));
    }
    else
    {
        BTMW_RPC_TEST_Logd("please input spp_disconnect [MAC address][UUID]\n");
        return BT_ERR_STATUS_INVALID_PARM_NUMS;
    }

    i4_ret = a_mtkapi_spp_disconnect(ps_target_mac, uuid);
    BTMW_RPC_TEST_Logd("i4_ret=%d\n", i4_ret);
    return BT_SUCCESS;
}

int btmw_rpc_test_spp_send_data(int argc, char **argv)
{
    CHAR ps_target_mac[18];
    CHAR uuid[48];
    CHAR str[128];
    int i4_ret = 0;

    if (3 == argc)
    {
        memset(ps_target_mac, 0, sizeof(ps_target_mac));
        memset(uuid, 0, sizeof(uuid));
        if (17 != strlen(argv[0]))
        {
            BTMW_RPC_TEST_Logd("mac length should be 17\n");
            return BT_ERR_STATUS_PARM_INVALID;
        }

        if (strlen(argv[2]) > 127)
        {
            BTMW_RPC_TEST_Logd("string length shoud be < 128\n");
            return BT_ERR_STATUS_PARM_INVALID;
        }

        strncpy(ps_target_mac, argv[0], sizeof(ps_target_mac)-1);
        strncpy(uuid, argv[1], strlen(argv[1]));
        strncpy(str, argv[2], strlen(argv[2]));
    }
    else
    {
        BTMW_RPC_TEST_Logd("please input spp_send_data [MAC address][UUID][String]\n");
        return BT_ERR_STATUS_INVALID_PARM_NUMS;
    }

    i4_ret = a_mtkapi_spp_send_data(ps_target_mac, uuid, str, strlen(argv[2]));
    BTMW_RPC_TEST_Logd("i4_ret=%d\n", i4_ret);
    return BT_SUCCESS;
}

int btmw_rpc_test_spp_enable_devb(int argc, char **argv)
{
    CHAR server_name[255];
    CHAR uuid[48];
    int i4_ret = 0;

    if (2 == argc)
    {
        memset(server_name, 0, sizeof(server_name));
        memset(uuid, 0, sizeof(uuid));
        if (strlen(argv[1]) > 254)
        {
            BTMW_RPC_TEST_Logd("name length should be < 255\n");
            return BT_ERR_STATUS_PARM_INVALID;
        }
        strncpy(server_name, argv[0], strlen(argv[0]));
        strncpy(uuid, argv[1], strlen(argv[1]));
    }
    else
    {
        BTMW_RPC_TEST_Logd("please input spp_enable_devb [server name][UUID]\n");
        return BT_ERR_STATUS_INVALID_PARM_NUMS;
    }

    i4_ret = a_mtkapi_spp_enable_devb(server_name, uuid);
    BTMW_RPC_TEST_Logd("i4_ret=%d\n", i4_ret);
    return BT_SUCCESS;
}


int btmw_rpc_test_spp_disable_devb(int argc, char **argv)
{
    CHAR uuid[48];
    int i4_ret = 0;

    if (1 == argc)
    {
        memset(uuid, 0, sizeof(uuid));
        if (strlen(argv[0]) > 254)
        {
            BTMW_RPC_TEST_Logd("name length should be < 255\n");
            return BT_ERR_STATUS_PARM_INVALID;
        }

        strncpy(uuid, argv[0], strlen(argv[0]));
    }
    else
    {
        BTMW_RPC_TEST_Logd("please input spp_disable_devb [UUID]\n");
        return BT_ERR_STATUS_INVALID_PARM_NUMS;
    }

    i4_ret = a_mtkapi_spp_disable_devb(uuid);
    BTMW_RPC_TEST_Logd("i4_ret=%d\n", i4_ret);
    return BT_SUCCESS;
}


static BTMW_RPC_TEST_CLI btmw_rpc_test_spp_cli_commands[] = {
    { (const char *)"spp_connect",            btmw_rpc_test_spp_connect,          (const char *)" = input addr and uuid"},
    { (const char *)"spp_disconnect",         btmw_rpc_test_spp_disconnect,       (const char *)" = input addr and uuid"},
    { (const char *)"spp_send_data",          btmw_rpc_test_spp_send_data,        (const char *)" = input addr, uuid and data"},
    { (const char *)"spp_enable_devb",        btmw_rpc_test_spp_enable_devb,      (const char *)" = input server name and uuid"},
    { (const char *)"spp_disable_devb",       btmw_rpc_test_spp_disable_devb,     (const char *)" = input uuid"},

    { NULL, NULL, NULL }
};

int btmw_rpc_test_spp_cmd_handler(int argc, char **argv)
{
    BTMW_RPC_TEST_CLI *cmd, *match = NULL;
    int ret = 0;
    int count;

    count = 0;
    cmd = btmw_rpc_test_spp_cli_commands;

    BTMW_RPC_TEST_Logi("[SPP] argc: %d, argv[0]: %s\n", argc, argv[0]);

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
        btmw_rpc_test_print_cmd_help(BTMW_RPC_TEST_CMD_KEY_SPP, btmw_rpc_test_spp_cli_commands);
        ret = -1;
    }
    else
    {
        match->handler(argc - 1, &argv[1]);
    }

    return ret;
}

int btmw_rpc_test_spp_init()
{
    INT32 ret = 0;
    BTMW_RPC_TEST_MOD spp_mod = {0};

    spp_mod.mod_id = BTMW_RPC_TEST_MOD_SPP;
    strncpy(spp_mod.cmd_key, BTMW_RPC_TEST_CMD_KEY_SPP, sizeof(spp_mod.cmd_key));
    spp_mod.cmd_handler = btmw_rpc_test_spp_cmd_handler;
    spp_mod.cmd_tbl = btmw_rpc_test_spp_cli_commands;

    ret = btmw_rpc_test_register_mod(&spp_mod);
    BTMW_RPC_TEST_Logd("btmw_rpc_test_register_mod() for SPP returns: %d\n", ret);
    return 0;
}


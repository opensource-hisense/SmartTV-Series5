#include <string.h>
#include <dlfcn.h>

// CLI
#include "btmw_rpc_test_cli.h"
#include "btmw_rpc_test_debug.h"
#include "btmw_rpc_test_tools_if.h"

static int driver_test(int argc, char *argv[]);

static BTMW_RPC_TEST_CLI btmw_rpc_test_tools_cli_commands[] =
{
    { "driver_test", driver_test,
      " = driver enable disable write open test" },
    { NULL, NULL, NULL }
};

// For handling incoming commands from CLI.
int btmw_rpc_test_tools_cmd_handler(int argc, char **argv)
{
    BTMW_RPC_TEST_CLI *cmd, *match = NULL;
    int ret = 0;
    int count;

    count = 0;
    cmd = btmw_rpc_test_tools_cli_commands;

    BTMW_RPC_TEST_Logd("[Tools] argc: %d, argv[0]: %s\n", argc, argv[0]);

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
        BTMW_RPC_TEST_Logd("Unknown command '%s'\n", argv[0]);

        btmw_rpc_test_print_cmd_help(BTMW_RPC_TEST_CMD_KEY_TOOLS, btmw_rpc_test_tools_cli_commands);
        ret = -1;
    }
    else
    {
        match->handler(argc - 1, &argv[1]);
    }

    return ret;
}

int btmw_rpc_test_tools_init()
{
    int ret = 0;
    BTMW_RPC_TEST_MOD tools_mod = {0};

    // Register to btmw_rpc_test_cli.
    tools_mod.mod_id = BTMW_RPC_TEST_MOD_TOOLS;
    strncpy(tools_mod.cmd_key, BTMW_RPC_TEST_CMD_KEY_TOOLS, sizeof(tools_mod.cmd_key));
    tools_mod.cmd_handler = btmw_rpc_test_tools_cmd_handler;
    tools_mod.cmd_tbl = btmw_rpc_test_tools_cli_commands;

    ret = btmw_rpc_test_register_mod(&tools_mod);
    BTMW_RPC_TEST_Logd("[Tools] btmw_rpc_test_register_mod() returns: %d\n", ret);

    return ret;
}

/*
 * bluetooth driver test
 * enable disable write
 */
static int driver_test(int argc, char *argv[])
{
    typedef void (*rx_callback)();

    typedef int (*bt_enable)(int flag, rx_callback func_cb);
    bt_enable mtk_bt_enable = NULL;

    typedef int (*bt_write)(int bt_fd, unsigned char *buffer, unsigned short length);
    bt_write mtk_bt_write = NULL;

    typedef int (*bt_disable)(int bt_fd);
    bt_disable mtk_bt_disable = NULL;
    void *glib_handle = NULL;
    unsigned char buffer[51] = {0x03, 0x0, 0xe, 0x30 };
    int fd=0;

    glib_handle = dlopen("libbt-vendor.so", RTLD_LAZY);
    if (!glib_handle)
    {
        printf("%s\n", dlerror());
        goto error;
    }

    dlerror(); /* Clear any existing error */

    mtk_bt_enable = dlsym(glib_handle, "mtk_bt_enable");
    if (mtk_bt_enable)
    {
        fd = mtk_bt_enable(0, NULL);
    }

    mtk_bt_write= dlsym(glib_handle, "mtk_bt_write");
    if (mtk_bt_write)
    {
        mtk_bt_write(fd, buffer, sizeof (buffer));
    }

    mtk_bt_disable = dlsym(glib_handle, "mtk_bt_disable");
    if (mtk_bt_disable)
    {
        mtk_bt_disable(fd);
    }

error:
    if (glib_handle)
    {
        dlclose(glib_handle);
        glib_handle = NULL;
    }

    return 0;
}

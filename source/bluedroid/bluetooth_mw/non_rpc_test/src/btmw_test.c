// System header files
#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/un.h>
#include <sys/wait.h>
#include <unistd.h>
#include <getopt.h>

#include "btmw_test_cli.h"

static void usage(void)
{
    printf(
"Usage: btcli [OPTION]...\n"
"\n"
"-h, --help              help\n"
"-c, --cli=#             choose cli mode. 0=disable, 1=enable\n"
"    --cli_debug=#       choose cli debug bitmap. #=2 byte hex number\n"
        );
}


int main(int argc, char **argv)
{
    int option_index;
    static const char short_options[] = "hc:";
    static const struct option long_options[] = {
        {"help", 0, 0, 'h'},
        {"cli", 1, 0, 'c'},
        {"cli_debug", 1, 0, OPT_CLI_DEBUG},
        {0, 0, 0, 0}
    };
    int c;
    int cli_mode = 1;
    int cli_debug = 0;
    char *cli_debug_bitmap;
    int opt_argc;
    char *opt_argv[2];

    btmw_test_log_init();

    while ((c = getopt_long(argc, argv, short_options, long_options, &option_index)) != -1)
    {
        switch (c)
        {
        case 'h':
            usage();
            return 0;
        case 'c':
            cli_mode = strtol(optarg, NULL, 0);
            break;
        case OPT_CLI_DEBUG:
            cli_debug = 1;
            cli_debug_bitmap = optarg;
            break;
        default:
            fprintf(stderr, "Try --help' for more information.\n");
            return 1;
        }
    }

    if (cli_debug)
    {
        opt_argc = 2;
        opt_argv[0] = cli_debug_bitmap;
        opt_argv[1] = "-1";

        btmw_test_set_btcli_handler(opt_argc, &opt_argv[0]);
    }

    if (btmw_test_init())
    {
        return -1;
    }

    if (cli_mode)
    {
        btmw_test_run();
        btmw_test_deinit();
    }
    else
    {
        pause();
    }

    return 0;
}

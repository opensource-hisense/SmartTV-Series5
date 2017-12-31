#include <stdlib.h>
#include <pthread.h>
#include <stdio.h>
#include <string.h>

#include "u_bluetooth.h"
#include "btmw_test_cli.h"
#include "btmw_test_debug.h"
#include "c_bt_mw_a2dp_common.h"
#include "c_bt_mw_a2dp_snk.h"
#include "c_bt_mw_manager.h"
#include "bt_a2dp_alsa_playback.h"

#define BTMW_TEST_CMD_KEY_A2DP_SINK        "MW_A2DP_SINK"

// CLI handler
static INT32 btmw_test_a2dp_sink_connect_int_handler(INT32 argc, CHAR *argv[]);
static INT32 btmw_test_a2dp_sink_disconnect_handler(INT32 argc, CHAR *argv[]);
static INT32 btmw_test_a2dp_sink_start_play_test(INT32 argc, CHAR *argv[]);
static INT32 btmw_test_a2dp_sink_stop_play_test(INT32 argc, CHAR *argv[]);
static INT32 btmw_test_a2dp_sink_active_handler(INT32 argc, CHAR *argv[]);

extern BT_PLAYER_SNK_CB_FUNC player_func;

static BTMW_TEST_CLI btmw_test_a2dp_sink_cli_commands[] =
{
    {"connect",    btmw_test_a2dp_sink_connect_int_handler, " = connect <addr>"},
    {"disconnect", btmw_test_a2dp_sink_disconnect_handler,  " = disconnect <addr>"},
    {"local_play", btmw_test_a2dp_sink_start_play_test,     " = local_play <sampling rate> <channel num> <data path>"},
    {"local_stop", btmw_test_a2dp_sink_stop_play_test,      " = local_stop"},
    {"active_sink", btmw_test_a2dp_sink_active_handler,     " = active_sink <1:enable|0:disable>"},
    {NULL, NULL, NULL},
};



typedef struct _LOCAL_PCM_DATA
{
    INT32 sampleRate;
    INT32 channelNum;
    CHAR pcmPath[64];
}LOCAL_PCM_DATA;
static pthread_t local_test_thread_id = (pthread_t)NULL;
static LOCAL_PCM_DATA g_pcm_data = {0};
static INT32 g_stop_test = 0;

VOID *data_receive_thread(VOID *p_args)
{
    INT32 ret;
    CHAR buf[4096*4];
    CHAR in_file[64] = "/data/usb/music/input_44100.pcm";
    FILE *fp = NULL;
    INT32 n_read_bytes = 0;
    LOCAL_PCM_DATA *pcm_data = (LOCAL_PCM_DATA *)p_args;

    if (NULL == p_args)
    {
        BTMW_TEST_Loge("p_args NULL pointer\n");
        return NULL;
    }
    ret = player_func.player_init_cb(pcm_data->sampleRate, pcm_data->channelNum);
    if (ret < 0)
    {
        BTMW_TEST_Loge("bluetooth_playback_init error.\n");
        return NULL;
    }
    BTMW_TEST_Logi("bluetooth_playback_init fs:%d channel num:%d. OK\n",
                   pcm_data->sampleRate, pcm_data->channelNum);

    if (0 < strlen(pcm_data->pcmPath))
    {
        BTMW_TEST_Logd("pcmPath:%s\n", pcm_data->pcmPath);
        strncpy(in_file, pcm_data->pcmPath, sizeof(in_file));
        in_file[sizeof(in_file)-1] = '\0';
    }
    if ((fp = fopen(in_file, "rb")) == (FILE *)NULL)
    {
        BTMW_TEST_Loge("cannot open file:%s\n", in_file);
        return NULL;
    }
    while (!g_stop_test)
    {
        n_read_bytes = fread(buf, sizeof(UINT8), 4096, fp);
        if (n_read_bytes <= 0)
        {
            BTMW_TEST_Logi("read finished!\n");
            break;
        }
        BTMW_TEST_Logi("read %d bytes\n", n_read_bytes);
        player_func.player_push_data_cb((VOID *)buf, (INT32)n_read_bytes);
    }

    fclose(fp);
    fp = NULL;
    player_func.player_deinit_cb();
    local_test_thread_id = (pthread_t)NULL;

    return NULL;
}

INT32 init_player_thread(INT32 sample_rate, INT32 channle_num, CHAR *path)
{

    BTMW_TEST_Logw("thread_id=0x%x\n", (INT32)local_test_thread_id);
    if (local_test_thread_id != (pthread_t)NULL)
    {
        BTMW_TEST_Logw("thread already was created\n");
        return 0;
    }

    g_pcm_data.sampleRate = sample_rate;
    g_pcm_data.channelNum = channle_num;
    if (NULL != path)
    {
        strncpy(g_pcm_data.pcmPath, path, sizeof(g_pcm_data.pcmPath));
    }
    BTMW_TEST_Logd("init_player_thread %d %d %s\n", g_pcm_data.sampleRate, g_pcm_data.channelNum, g_pcm_data.pcmPath);
    if (pthread_create(&local_test_thread_id, NULL , data_receive_thread , (VOID*)&g_pcm_data) != 0)
    {
        BTMW_TEST_Loge("Create thread failed\n");
        return -1;
    }
    return 0;
}

static INT32 btmw_test_a2dp_sink_start_play_test(INT32 argc, CHAR *argv[])
{
    INT32 fs = 44100;
    INT32 channel_num = 2;
    CHAR path[64] = {0};

    BTMW_TEST_Logi("%s() argc:%d, argv[0]:%s\n", __func__, argc, argv[0]);

    if (argc < 3)
    {
        BTMW_TEST_Loge("local_play <sampling rate> <channel num> <data path>");
        BTMW_TEST_Loge("eg. local_play 44100 2 /data/usb/music/input_44100.pcm");
        return -1;
    }

    if (!strncmp(argv[0], "44100", strlen("44100")))
    {
        fs = 44100;
    }
    else if (!strncmp(argv[0], "48000", strlen("48000")))
    {
        fs = 48000;
    }

    if (!strncmp(argv[1], "2", strlen("2")))
    {
        channel_num = 2;
    }
    else if (!strncmp(argv[1], "1", strlen("1")))
    {
        channel_num = 1;
    }

    strncpy(path, argv[2], sizeof(path));
    g_stop_test = 0;

    BTMW_TEST_Logd("local_play %d %d %s\n", fs, channel_num, path);

    init_player_thread(fs, channel_num, path);
    BTMW_TEST_Logi("A2DP play %s\n", path);

    return 0;
}

static INT32 btmw_test_a2dp_sink_stop_play_test(INT32 argc, CHAR *argv[])
{
    BTMW_TEST_Logi("A2DP stop play test\n");
    g_stop_test = 1;
    return 0;
}

static INT32 btmw_test_a2dp_sink_active_handler(INT32 argc, CHAR *argv[])
{
    BTMW_TEST_Logi("%s()\n", __func__);
    UINT8 u1_enable = 0;

    if (argc != 1)
    {
        BTMW_TEST_Loge("[USAGE] active_sink [1:enable|0:disable]");
        return -1;
    }

    u1_enable = atoi(argv[0]);

    c_btm_btstart("PAIRING", 300);

    if (1 == u1_enable)
    {
        c_btm_source_sink_on_off(FALSE, TRUE);
    }
    else if (0 == u1_enable)
    {
        c_btm_source_sink_on_off(TRUE, FALSE);
    }
    else
    {
        BTMW_TEST_Loge("input error\n");
        BTMW_TEST_Loge("please input active_sink <1:enable|0:disable>\n");
        BTMW_TEST_Loge("just remind:at the same time only src or sink role can be enabled\n");
    }

    return 0;
}


static INT32 btmw_test_a2dp_sink_connect_int_handler(INT32 argc, CHAR *argv[])
{
    CHAR *ptr = NULL;

    BTMW_TEST_Logi("%s()\n", __func__);

    if (argc != 1)
    {
        BTMW_TEST_Loge("[USAGE] connect <addr>");
        return -1;
    }
    if (strlen(argv[0]) < 17)
    {
        BTMW_TEST_Loge("<addr> invalid. Good example is \"AA:BB:CC:DD:EE:FF\"");
        BTMW_TEST_Loge("[USAGE] connect <addr>");
        return -1;
    }

    ptr = argv[0];
    BTMW_TEST_Logi("A2DP connected to %s\n", ptr);
    c_btm_connect_audio_sources(ptr, CONN_TYPE_A2DP);

    return 0;
}

static INT32 btmw_test_a2dp_sink_disconnect_handler(INT32 argc, CHAR *argv[])
{
    CHAR *ptr;
    BTMW_TEST_Logi("%s()\n", __func__);

    if (argc != 1)
    {
        BTMW_TEST_Loge("[USAGE] disconnect <addr>");
        return -1;
    }
    if (strlen(argv[0]) < 17)
    {
        BTMW_TEST_Loge("<addr> invalid. Good example is \"AA:BB:CC:DD:EE:FF\"");
        BTMW_TEST_Loge("[USAGE] disconnect <addr>");
    }

    ptr = argv[0];
    BTMW_TEST_Logi("A2DP disconnected to %s\n", ptr);
    c_btm_bt_disconnect_audio_sources(ptr, CONN_TYPE_A2DP);
    return 0;
}

INT32 btmw_test_a2dp_sink_cmd_handler(INT32 argc, CHAR **argv)
{
    BTMW_TEST_CLI *cmd, *match = NULL;
    INT32 ret = 0;
    INT32 count = 0;

    BTMW_TEST_Logd("%s argc: %d, argv[0]: %s\n", __func__, argc, argv[0]);

    cmd = btmw_test_a2dp_sink_cli_commands;
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
        BTMW_TEST_Logd("Unknown command '%s'\n", argv[0]);

        btmw_test_print_cmd_help(BTMW_TEST_CMD_KEY_A2DP_SINK, btmw_test_a2dp_sink_cli_commands);
        ret = -1;
    }
    else
    {
        match->handler(argc - 1, &argv[1]);
    }

    return ret;
}

INT32 btmw_test_a2dp_sink_init(VOID)
{
    INT32 ret = 0;
    BTMW_TEST_MOD a2dp_sink_mod = {0};

    a2dp_sink_mod.mod_id = BTMW_TEST_MOD_A2DP_SINK;
    strncpy(a2dp_sink_mod.cmd_key, BTMW_TEST_CMD_KEY_A2DP_SINK, sizeof(a2dp_sink_mod.cmd_key));
    a2dp_sink_mod.cmd_handler = btmw_test_a2dp_sink_cmd_handler;
    a2dp_sink_mod.cmd_tbl = btmw_test_a2dp_sink_cli_commands;

    //bluetooth_a2dp_register_player_cb(bt_a2dp_playback_getPb());
    
    ret = btmw_test_register_mod(&a2dp_sink_mod);
    BTMW_TEST_Logd("btmw_test_register_mod() for SINK returns: %d\n", ret);

    return ret;
}



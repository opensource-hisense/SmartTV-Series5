#include <string.h>

#include "btmw_rpc_test_cli.h"
#include "btmw_rpc_test_debug.h"
#include "btmw_rpc_test_a2dp_sink_if.h"
#include "mtk_bt_service_a2dp_wrapper.h"

#define BTMW_RPC_TEST_CMD_KEY_A2DP_SINK        "MW_RPC_A2DP_SINK"

// CLI handler
static int btmw_rpc_test_a2dp_sink_connect_int_handler(int argc, char *argv[]);
static int btmw_rpc_test_a2dp_sink_disconnect_handler(int argc, char *argv[]);
//static int btmw_rpc_test_a2dp_sink_start_play_test(int argc, char *argv[]);
//static int btmw_rpc_test_a2dp_sink_stop_play_test(int argc, char *argv[]);
static int btmw_rpc_test_a2dp_sink_active_handler(int argc, char *argv[]);
static int btmw_rpc_test_a2dp_get_role_handler(int argc, char *argv[]);
static int btmw_rpc_test_a2dp_stream_is_suspend_handler(int argc, char *argv[]);
static int btmw_rpc_test_a2dp_get_src_dev_list_handler(int argc, char *argv[]);
//static int btmw_rpc_test_a2dp_reg_player(int argc, char *argv[]);


static BTMW_RPC_TEST_CLI btmw_rpc_test_a2dp_sink_cli_commands[] =
{
    {"connect",         btmw_rpc_test_a2dp_sink_connect_int_handler, " = connect <addr>"},
    {"disconnect",      btmw_rpc_test_a2dp_sink_disconnect_handler,  " = disconnect <addr>"},
    //{"local_play",      btmw_rpc_test_a2dp_sink_start_play_test,     " = local_play <sampling rate> <channel num> <data path>"},
    //{"local_stop",      btmw_rpc_test_a2dp_sink_stop_play_test,      " = local_stop"},
    {"active_sink",     btmw_rpc_test_a2dp_sink_active_handler,      " = active_sink <1:enable|0:disable>"},
    {"get_role",        btmw_rpc_test_a2dp_get_role_handler,         " = get_role"},
    {"stream_status",   btmw_rpc_test_a2dp_stream_is_suspend_handler," = stream_status"},
    {"get_paired_dev",  btmw_rpc_test_a2dp_get_src_dev_list_handler, " = get_paired_dev"},
    //{"reg_player",      btmw_rpc_test_a2dp_reg_player,               " = reg_player"},
    {NULL, NULL, NULL},
};
#if 0

extern BT_PLAYER_SNK_CB_FUNC player_func;


typedef struct _LOCAL_PCM_DATA
{
    INT32 sampleRate;
    INT32 channelNum;
    CHAR pcmPath[64];
}LOCAL_PCM_DATA;
static pthread_t local_test_thread_id = (pthread_t)NULL;
static LOCAL_PCM_DATA g_pcm_data = {0};
static INT32 g_stop_test = 0;

VOID *btmw_rpc_test_data_receive_thread(VOID *p_args)
{
    INT32 ret;
    CHAR buf[4096*4];
    CHAR in_file[64] = "/data/usb/music/input_44100.pcm";
    FILE *fp = NULL;
    INT32 n_read_bytes = 0;
    LOCAL_PCM_DATA *pcm_data = (LOCAL_PCM_DATA *)p_args;

    if (NULL == p_args)
    {
        BTMW_RPC_TEST_Loge("p_args NULL pointer\n");
        return NULL;
    }
    ret = player_func.player_init_cb(pcm_data->sampleRate, pcm_data->channelNum);
    if (ret < 0)
    {
        BTMW_RPC_TEST_Loge("bluetooth_playback_init error.\n");
        return NULL;
    }
    BTMW_RPC_TEST_Logi("bluetooth_playback_init fs:%d channel num:%d. OK\n",
                pcm_data->sampleRate, pcm_data->channelNum);

    if (0 < strlen(pcm_data->pcmPath))
    {
        BTMW_RPC_TEST_Logd("pcmPath:%s\n", pcm_data->pcmPath);
        strncpy(in_file, pcm_data->pcmPath, sizeof(in_file));
        in_file[sizeof(in_file)-1] = '\0';
    }
    if ((fp = fopen(in_file, "rb")) == (FILE *)NULL)
    {
        BTMW_RPC_TEST_Loge("cannot open file:%s\n", in_file);
        return NULL;
    }
    while (!g_stop_test)
    {
        n_read_bytes = fread(buf, sizeof(UINT8), 4096, fp);
        if (n_read_bytes <= 0)
        {
            BTMW_RPC_TEST_Logi("read finished!\n");
            break;
        }
        BTMW_RPC_TEST_Logi("read %d bytes\n", n_read_bytes);
        player_func.player_push_data_cb((VOID *)buf, (INT32)n_read_bytes);
    }

    fclose(fp);
    fp = NULL;
    player_func.player_deinit_cb();
    local_test_thread_id = (pthread_t)NULL;

    return NULL;
}

INT32 btmw_rpc_test_init_player_thread(INT32 sample_rate, INT32 channle_num, CHAR *path)
{

    BTMW_RPC_TEST_Logw("thread_id=0x%x\n", (INT32)local_test_thread_id);
    if (local_test_thread_id != (pthread_t)NULL)
    {
        BTMW_RPC_TEST_Logw("thread already was created\n");
        return 0;
    }

    g_pcm_data.sampleRate = sample_rate;
    g_pcm_data.channelNum = channle_num;
    if(NULL != path)
    {
        strncpy(g_pcm_data.pcmPath, path, sizeof(g_pcm_data.pcmPath));
    }
    BTMW_RPC_TEST_Logd("init_player_thread %d %d %s\n", g_pcm_data.sampleRate, g_pcm_data.channelNum, g_pcm_data.pcmPath);
    if (pthread_create(&local_test_thread_id, NULL , btmw_rpc_test_data_receive_thread , (VOID*)&g_pcm_data) != 0)
    {
        BTMW_RPC_TEST_Loge("Create thread failed\n");
        return -1;
    }
    return 0;
}

static int btmw_rpc_test_a2dp_sink_start_play_test(int argc, char *argv[])
{
    INT32 fs = 44100;
    INT32 channel_num = 2;
    CHAR path[64] = {0};

    BTMW_RPC_TEST_Logi("%s() argc:%d, argv[0]:%s\n", __func__, argc, argv[0]);

    if (argc < 3)
    {
        BTMW_RPC_TEST_Loge("local_play <sampling rate> <channel num> <data path>");
        BTMW_RPC_TEST_Loge("eg. local_play 44100 2 /data/usb/music/input_44100.pcm");
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
    //g_stop_test = 0;

    BTMW_RPC_TEST_Logd("local_play %d %d %s\n", fs, channel_num, path);

    //btmw_rpc_test_init_player_thread(fs, channel_num, path);
    BTMW_RPC_TEST_Logi("A2DP play %s\n", path);

    return 0;
}

static int btmw_rpc_test_a2dp_sink_stop_play_test(int argc, char *argv[])
{
    BTMW_RPC_TEST_Logi("A2DP stop play test\n");
    //g_stop_test = 1;
    return 0;
}
#endif

static int btmw_rpc_test_a2dp_sink_active_handler(int argc, char *argv[])
{
    UINT8 u1_enable = 0;
    BTMW_RPC_TEST_Logi("%s()\n", __func__);

    if (argc != 1)
    {
        BTMW_RPC_TEST_Loge("[USAGE] active_sink [1:enable|0:disable]");
        return -1;
    }

    u1_enable = atoi(argv[0]);
    if (1 == u1_enable)
    {
        a_mtkapi_a2dp_source_sink_on_off(FALSE, TRUE);
    }
    else if (0 == u1_enable)
    {
        a_mtkapi_a2dp_source_sink_on_off(TRUE, FALSE);
    }
    else
    {
        BTMW_RPC_TEST_Loge("input error\n");
        BTMW_RPC_TEST_Loge("please input active_sink <1:enable|0:disable>\n");
        BTMW_RPC_TEST_Loge("just remind:at the same time only src or sink role can be enabled\n");
    }

    return 0;
}


static int btmw_rpc_test_a2dp_get_role_handler(int argc, char *argv[])
{
    BT_A2DP_ROLE role;
    BTMW_RPC_TEST_Logi("%s()\n", __func__);

    role = a_mtkapi_a2dp_get_role();

    BTMW_RPC_TEST_Logi("role is %s\n", role==BT_A2DP_ROLE_SRC?"src":(role==BT_A2DP_ROLE_SINK?"sink":"unknown"));

    return 0;
}

static int btmw_rpc_test_a2dp_stream_is_suspend_handler(int argc, char *argv[])
{
    INT32 status = 0;
    BTMW_RPC_TEST_Logi("%s()\n", __func__);

    status = a_mtkapi_a2dp_stream_is_suspend();

    BTMW_RPC_TEST_Logi("stream is %s\n", status==0?"not suspend":"suspend");

    return 0;
}


static int btmw_rpc_test_a2dp_get_src_dev_list_handler(int argc, char *argv[])
{
    BT_TARGET_DEV_LIST device_list;
    INT32 ret = 0;
    BTMW_RPC_TEST_Logi("%s()\n", __func__);

    memset((void*)&device_list, 0, sizeof(device_list));

    ret = a_mtkapi_a2dp_get_paired_src_dev_list(&device_list);
    BTMW_RPC_TEST_Logi("get paired src device list result:\n", __func__);
    if(BT_SUCCESS == ret)
    {
        if (0 == device_list.dev_num)
        {
            BTMW_RPC_TEST_Logi("no paired src device\n");
        }
        else
        {
            INT32 i = 0;
            for(i=0;i<device_list.dev_num;i++)
            {
                BTMW_RPC_TEST_Logi("device index: %d\n", (i+device_list.head_idx)%MAX_DEV_NUM);
                BTMW_RPC_TEST_Logi("name: %s\n", device_list.device_list[(i+device_list.head_idx)%MAX_DEV_NUM].name);
                BTMW_RPC_TEST_Logi("is TV: %d\n", device_list.device_list[(i+device_list.head_idx)%MAX_DEV_NUM].fg_istv);
                BTMW_RPC_TEST_Logi("delay: %d\n", device_list.device_list[(i+device_list.head_idx)%MAX_DEV_NUM].u4delay_value);
                BTMW_RPC_TEST_Logi("cod: 0x%x\n", device_list.device_list[(i+device_list.head_idx)%MAX_DEV_NUM].cod);
                BTMW_RPC_TEST_Logi("device_type: 0x%x\n", device_list.device_list[(i+device_list.head_idx)%MAX_DEV_NUM].device_type);
                BTMW_RPC_TEST_Logi("bdAddr: %s\n",
                    device_list.device_list[(i+device_list.head_idx)%MAX_DEV_NUM].bdAddr);
                BTMW_RPC_TEST_Logi("======================================\n");
            }
        }
    }
    else
    {
        BTMW_RPC_TEST_Logi("get paired sink device failed: %d\n", ret);
    }

    return 0;
}

#if 0

static INT32 btmw_rpc_test_a2dp_BtPlayerInit(INT32 trackFreq, INT32 channelType, VOID *pv_tag)
{
    BTMW_RPC_TEST_Logi("%s() trackFreq: %d, channelType: %d, %p\n",
        __func__, trackFreq, channelType, pv_tag);
    return 0;
}

static INT32 btmw_rpc_test_a2dp_BtPlayerStart(VOID *pv_tag)
{
    BTMW_RPC_TEST_Logi("%s() %p\n", __func__, pv_tag);
    return 0;
}

static INT32 btmw_rpc_test_a2dp_BtPlayerPause(VOID *pv_tag)
{
    BTMW_RPC_TEST_Logi("%s() %p\n", __func__, pv_tag);
    return 0;
}

static INT32 btmw_rpc_test_a2dp_BtPlayerStop(VOID *pv_tag)
{
    BTMW_RPC_TEST_Logi("%s() %p\n", __func__, pv_tag);
    return 0;
}

static INT32 btmw_rpc_test_a2dp_BtPlayerDeinit(VOID *pv_tag)
{
    BTMW_RPC_TEST_Logi("%s() %p\n", __func__, pv_tag);
    return 0;
}

static INT32 btmw_rpc_test_a2dp_BtPlayerPushData(VOID *audioBuffer, INT32 bufferLen, VOID *pv_tag)
{
    BTMW_RPC_TEST_Logi("%s() audioBuffer: %p, bufferLen: %d, %p\n",
        __func__, audioBuffer, bufferLen, pv_tag);
    return 0;
}

static int btmw_rpc_test_a2dp_reg_player(int argc, char *argv[])
{
    MTKRPCAPI_BT_APP_A2DP_PLAYER_SNK_CB_FUNC func =
    {
        btmw_rpc_test_a2dp_BtPlayerInit,
        btmw_rpc_test_a2dp_BtPlayerStart,
        btmw_rpc_test_a2dp_BtPlayerPause,
        btmw_rpc_test_a2dp_BtPlayerStop,
        btmw_rpc_test_a2dp_BtPlayerDeinit,
        btmw_rpc_test_a2dp_BtPlayerPushData,
    };
    INT32 ret = 0;
    BTMW_RPC_TEST_Logi("%s()\n", __func__);


    ret = a_mtkapi_a2dp_register_player_cb(&func, NULL);
    BTMW_RPC_TEST_Logi("%s() ret=%d\n", __func__, ret);

    return 0;
}
#endif


static int btmw_rpc_test_a2dp_sink_connect_int_handler(int argc, char *argv[])
{
    CHAR *ptr = NULL;

    BTMW_RPC_TEST_Logi("%s()\n", __func__);

    if (argc != 1)
    {
        BTMW_RPC_TEST_Loge("[USAGE] connect <addr>");
        return -1;
    }
    if (strlen(argv[0]) < 17)
    {
        BTMW_RPC_TEST_Loge("<addr> invalid. Good example is \"AA:BB:CC:DD:EE:FF\"");
        BTMW_RPC_TEST_Loge("[USAGE] connect <addr>");
        return -1;
    }

    ptr = argv[0];
    BTMW_RPC_TEST_Logi("A2DP connected to %s\n", ptr);
    a_mtkapi_a2dp_connect_audio_sources(ptr, CONN_TYPE_A2DP);

    return 0;
}

static int btmw_rpc_test_a2dp_sink_disconnect_handler(int argc, char *argv[])
{
    CHAR *ptr;
    BTMW_RPC_TEST_Logi("%s()\n", __func__);

    if (argc != 1)
    {
        BTMW_RPC_TEST_Loge("[USAGE] disconnect <addr>");
        return -1;
    }
    if (strlen(argv[0]) < 17)
    {
        BTMW_RPC_TEST_Loge("<addr> invalid. Good example is \"AA:BB:CC:DD:EE:FF\"");
        BTMW_RPC_TEST_Loge("[USAGE] disconnect <addr>");
    }

    ptr = argv[0];
    BTMW_RPC_TEST_Logi("A2DP disconnected to %s\n", ptr);
    a_mtkapi_a2dp_disconnect_audio_sources(ptr, CONN_TYPE_A2DP);
    return 0;
}

static int  btmw_rpc_test_a2dp_sink_cmd_handler(int argc, char *argv[])
{
    BTMW_RPC_TEST_CLI *cmd, *match = NULL;
    INT32 ret = 0;
    INT32 count = 0;

    BTMW_RPC_TEST_Logd("%s argc: %d, argv[0]: %s\n", __func__, argc, argv[0]);

    cmd = btmw_rpc_test_a2dp_sink_cli_commands;
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

        btmw_rpc_test_print_cmd_help(BTMW_RPC_TEST_CMD_KEY_A2DP_SINK, btmw_rpc_test_a2dp_sink_cli_commands);
        ret = -1;
    }
    else
    {
        match->handler(argc - 1, &argv[1]);
    }

    return ret;
}

INT32 btmw_rpc_test_a2dp_sink_init(VOID)
{
    INT32 ret = 0;
    BTMW_RPC_TEST_MOD a2dp_sink_mod = {0};

    a2dp_sink_mod.mod_id = BTMW_RPC_TEST_MOD_A2DP_SINK;
    strncpy(a2dp_sink_mod.cmd_key, BTMW_RPC_TEST_CMD_KEY_A2DP_SINK, sizeof(a2dp_sink_mod.cmd_key));
    a2dp_sink_mod.cmd_handler = btmw_rpc_test_a2dp_sink_cmd_handler;
    a2dp_sink_mod.cmd_tbl = btmw_rpc_test_a2dp_sink_cli_commands;

    ret = btmw_rpc_test_register_mod(&a2dp_sink_mod);
    BTMW_RPC_TEST_Logd("btmw_rpc_test_register_mod() for SINK returns: %d\n", ret);

    return ret;
}



#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <time.h>

#include "mtk_audio.h"
#include "btmw_rpc_test_cli.h"
#include "btmw_rpc_test_debug.h"
#include "btmw_rpc_test_a2dp_src_if.h"
#include "mtk_bt_service_a2dp_wrapper.h"

#define BTMW_RPC_TEST_CMD_KEY_A2DP_SRC        "MW_RPC_A2DP_SRC"
#define PCM_BUFFER_SIZE 512*8     // at least 512
/* 0:pause, 1: play */
static INT32 g_stream_status;
static pthread_t btmw_rpc_test_stream_handle = -1;
static CHAR btmw_rpc_test_pcm_file[128] = "/data/usb/music/48000/input_48000.pcm";


INT32 g_a2dp_connection_established = 0;
UINT32 g_u4last_tx_time = 0;



// CLI handler
static int btmw_rpc_test_a2dp_src_connect_int_handler(int argc, char *argv[]);
static int btmw_rpc_test_a2dp_src_disconnect_handler(int argc, char *argv[]);
static int btmw_rpc_test_a2dp_src_active_handler(int argc, char *argv[]);
static int btmw_rpc_test_a2dp_src_set_audio_hw_dbg_lvl_handler(int argc, char *argv[]);
static int btmw_rpc_test_a2dp_src_stream_handler(int argc, char *argv[]);
static int btmw_rpc_test_a2dp_get_sink_dev_list_handler(int argc, char *argv[]);
static int btmw_rpc_test_a2dp_src_write_stream_handler(int argc, char *argv[]);
static int btmw_rpc_test_a2dp_get_connect_status(int argc, char *argv[]);
static int btmw_rpc_test_a2dp_src_mute_device(int argc, char *argv[]);

//static int btmw_rpc_test_a2dp_reg_uploader(int argc, char *argv[]);

static BTMW_RPC_TEST_CLI btmw_rpc_test_a2dp_src_cli_commands[] =
{
    {"audio_hw_log",  btmw_rpc_test_a2dp_src_set_audio_hw_dbg_lvl_handler,      " = audio_hw_log <0~6>"},
    {"connect",       btmw_rpc_test_a2dp_src_connect_int_handler,               " = connect <addr>"},
    {"disconnect",    btmw_rpc_test_a2dp_src_disconnect_handler,                " = disconnect <addr>"},
    {"stream",        btmw_rpc_test_a2dp_src_stream_handler,                    " = stream <start|suspend>"},
    {"active_src",    btmw_rpc_test_a2dp_src_active_handler,                    " = active_src <1:enable|0:disable>"},
    {"get_paired_dev",btmw_rpc_test_a2dp_get_sink_dev_list_handler,             " = get_paired_dev"},
    {"write_stream",  btmw_rpc_test_a2dp_src_write_stream_handler,             " = write_stream"},
    {"connect_status",btmw_rpc_test_a2dp_get_connect_status,                 " = get connect status"},
    {"mute",          btmw_rpc_test_a2dp_src_mute_device,                 " = mute <1:mute|0:don't mute>"},
    //{"reg_uploader",  btmw_rpc_test_a2dp_reg_uploader,                          " = reg_uploader"},
    {NULL, NULL, NULL},
};

UINT32 bt_get_microseconds(VOID)
{
    struct timespec now;
    clock_gettime(CLOCK_MONOTONIC, &now);
    return (now.tv_sec * 1000000 + now.tv_nsec / 1000);
}

static int btmw_rpc_test_a2dp_src_set_audio_hw_dbg_lvl_handler(int argc, char *argv[])
{
    UINT8 u1_log_lvl = 0;
    BTMW_RPC_TEST_Logi("%s()\n", __func__);

    if (argc != 1)
    {
        BTMW_RPC_TEST_Loge("[USAGE] audio_hw_log <0~6>");
        return -1;
    }

    u1_log_lvl = atoi(argv[0]);
    if (0 <= u1_log_lvl && 6 >= u1_log_lvl)
    {
        a_mtkapi_a2dp_src_set_audio_hw_dbg_lvl(u1_log_lvl);
    }
    else
    {
        BTMW_RPC_TEST_Loge("input error\n");
        BTMW_RPC_TEST_Loge("please input audio_hw_log <0~6>\n");
    }
    return 0;
}

static int btmw_rpc_test_a2dp_src_active_handler(int argc, char *argv[])
{
    UINT8 u1_enable = 0;
    BTMW_RPC_TEST_Logi("%s()\n", __func__);

    if (argc != 1)
    {
        BTMW_RPC_TEST_Loge("[USAGE] active_src [1:enable|0:disable]");
        return -1;
    }

    u1_enable = atoi(argv[0]);
    if (1 == u1_enable)
    {
        a_mtkapi_a2dp_source_sink_on_off(TRUE, FALSE);
    }
    else if (0 == u1_enable)
    {
        a_mtkapi_a2dp_source_sink_on_off(FALSE, TRUE);
    }
    else
    {
        BTMW_RPC_TEST_Loge("input error\n");
        BTMW_RPC_TEST_Loge("please input active_src <1:enable|0:disable>\n");
        BTMW_RPC_TEST_Loge("just remind:at the same time only src or sink role can be enabled\n");
    }

    return 0;
}



static int btmw_rpc_test_a2dp_get_sink_dev_list_handler(int argc, char *argv[])
{
    BT_TARGET_DEV_LIST device_list;
    INT32 ret = 0;
    BTMW_RPC_TEST_Logi("%s()\n", __func__);

    memset((void*)&device_list, 0, sizeof(device_list));

    ret = a_mtkapi_a2dp_get_paired_sink_dev_list(&device_list);
    BTMW_RPC_TEST_Logi("get paired sink device list result:\n", __func__);
    if(BT_SUCCESS == ret)
    {
        if (0 == device_list.dev_num)
        {
            BTMW_RPC_TEST_Logi("no paired sink device\n");
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

static INT32 btmw_rpc_test_a2dp_BtUploadInit(void* pv_tag)
{
    BTMW_RPC_TEST_Logi("%s() %p\n", __func__, pv_tag);
    return 0;
}

static INT32 btmw_rpc_test_a2dp_mtkrpcapi_BtUploadStart(void* pv_tag)
{
    BTMW_RPC_TEST_Logi("%s() %p\n", __func__, pv_tag);
    return 0;
}

static INT32 btmw_rpc_test_a2dp_mtkrpcapi_BtUploadStop(void* pv_tag)
{
    BTMW_RPC_TEST_Logi("%s() %p\n", __func__, pv_tag);
    return 0;
}

static INT32 btmw_rpc_test_a2dp_mtkrpcapi_BtUploadDeinit(void* pv_tag)
{
    BTMW_RPC_TEST_Logi("%s() %p\n", __func__, pv_tag);
    return 0;
}

static int btmw_rpc_test_a2dp_reg_uploader(int argc, char *argv[])
{
    MTKRPCAPI_BT_APP_A2DP_SRC_CB_FUNC_T func =
    {
        btmw_rpc_test_a2dp_BtUploadInit,
        btmw_rpc_test_a2dp_mtkrpcapi_BtUploadStart,
        btmw_rpc_test_a2dp_mtkrpcapi_BtUploadStop,
        btmw_rpc_test_a2dp_mtkrpcapi_BtUploadDeinit,
    };
    INT32 ret = 0;
    BTMW_RPC_TEST_Logi("%s()\n", __func__);


    ret = a_mtkapi_a2dp_register_uploader_cb(&func, NULL);
    BTMW_RPC_TEST_Logi("%s() ret=%d\n", __func__, ret);

    return 0;
}
#endif


static int btmw_rpc_test_a2dp_src_connect_int_handler(int argc, char *argv[])
{
    CHAR *ptr;

    BTMW_RPC_TEST_Logd("%s()\n", __func__);

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
    a_mtkapi_a2dp_connect_audio_sources(ptr, CONN_TYPE_A2DP);

    return 0;
}

static int btmw_rpc_test_a2dp_src_disconnect_handler(int argc, char *argv[])
{
    CHAR *ptr;

    BTMW_RPC_TEST_Logd("%s()\n", __func__);

    if (argc != 1)
    {
        BTMW_RPC_TEST_Loge("[USAGE] disconnect <addr>");
        return -1;
    }
    if (strlen(argv[0]) < 17)
    {
        BTMW_RPC_TEST_Loge("<addr> invalid. Good example is \"AA:BB:CC:DD:EE:FF\"");
        BTMW_RPC_TEST_Loge("[USAGE] disconnect <addr>");
        return -1;
    }

    ptr = argv[0];
    a_mtkapi_a2dp_disconnect_audio_sources(ptr, CONN_TYPE_A2DP);
    return 0;
}

static VOID* btmw_rpc_test_send_audio_data_thread(VOID *arg)
{
    UINT8 *pcm_buffer = NULL;
    CHAR *local_test_pcm_file = (CHAR *)arg;
    FILE *fInputPCM;
    INT32 result=0;
    INT32 pcm_frame_len = PCM_BUFFER_SIZE;
    INT32 total_pcm_len;
    INT32 now = 0;
    UINT32 interval = 0;
    INT32 wait = 0;
    UINT32 wlength = 2;
    UINT32 channel_num = 2;
    UINT32 sample_rate = 44100;

    BTMW_RPC_TEST_Logd("Input file name  : %s\n", local_test_pcm_file);
    sleep(10);
    /* open file & allocate memory */
    fInputPCM = fopen(local_test_pcm_file, "rb");
    if (fInputPCM == NULL)
    {
        BTMW_RPC_TEST_Loge("Can't open input PCM file!\n");
        return NULL;
    }
    BTMW_RPC_TEST_Loge("open input PCM file success!\n");

    pcm_buffer = (UINT8 *)malloc(PCM_BUFFER_SIZE);
    if (pcm_buffer == NULL)
    {
        fclose(fInputPCM);
        BTMW_RPC_TEST_Loge("Can't allocat buffer\n");
        return NULL;
    }

    while(1)
    {
        memset(pcm_buffer, 0, PCM_BUFFER_SIZE);

        if(1 == g_stream_status)
        {
            total_pcm_len = fread(pcm_buffer, sizeof(UINT8), pcm_frame_len, fInputPCM );
            if (total_pcm_len == 0)
            {
                BTMW_RPC_TEST_Loge("total_pcm_len==0\n");
                goto send_audio_data_end;
            }

            /*for control pcm data send speed from uploader, avoid buffer
                    overflow due to bluedroid handle data slowly*/
            now = bt_get_microseconds();

            if (0 == g_u4last_tx_time)
            {
                g_u4last_tx_time = now;
            }
            else
            {
                interval = 1000 * (1000 * total_pcm_len  /
                            (wlength * channel_num * sample_rate));

                wait = g_u4last_tx_time + interval - now;
                BTMW_RPC_TEST_Logd("wait %ld!\n", wait);
                BTMW_RPC_TEST_Logd("interval %ld!\n", interval);
                BTMW_RPC_TEST_Logd("last_tx_time %ld!\n", g_u4last_tx_time);
                if (wait > 0)
                {
                    if ( wait > 50000 )
                    {
                        BTMW_RPC_TEST_Logd("wait more than 50ms!, skip!\n");
                    }
                    else
                    {
                        BTMW_RPC_TEST_Logd("wait %ld ms!\n", wait/1000);
                        usleep(wait);
                    }
                }
                if (wait < 0)
                {
                    g_u4last_tx_time = 0;
                }
                else
                {
                    g_u4last_tx_time += (interval + wait);
                }
            }
            /*end of speed control*/

            result = a_mtkapi_a2dp_send_stream_data((const CHAR*)pcm_buffer, total_pcm_len);
            if (result != 0)
            {
                BTMW_RPC_TEST_Logd("BtA2dpSendData Failed!\n");
                //goto send_audio_data_end;
            }
            else
            {
                BTMW_RPC_TEST_Logd("send data successful. result:%ld   total_pcm_len:%ld\n", (long)result, (long)total_pcm_len);
                //usleep(15000);
            }
        }
        else
        {
            usleep(500000);
        }
    }
send_audio_data_end:

    fclose(fInputPCM);
    fInputPCM = NULL;
    free(pcm_buffer);
    pcm_buffer = NULL;
    btmw_rpc_test_stream_handle = -1;
    BTMW_RPC_TEST_Logd("Send audio finished\n");
    return NULL;
}

static VOID* btmw_rpc_test_write_audio_data_thread(VOID *arg)
{
    UINT8 *pcm_buffer = NULL;
    CHAR *local_test_pcm_file = (CHAR *)arg;
    FILE *fInputPCM;
    INT32 result=0;
    INT32 pcm_frame_len = PCM_BUFFER_SIZE;
    INT32 total_pcm_len;
    INT32 now = 0;
    UINT32 interval = 0;
    INT32 wait = 0;
    UINT32 wlength = 2;
    UINT32 channel_num = 2;
    UINT32 sample_rate = 48000;
    int read_cnt = 0;
    hw_device_t* device = NULL;
    struct audio_hw_device *hw_dev = NULL;
    struct audio_stream_out *stream = NULL;

    BTMW_RPC_TEST_Logd("Input file name  : %s\n", local_test_pcm_file);
    /* open file & allocate memory */
    fInputPCM = fopen(local_test_pcm_file, "rb");
    if (fInputPCM == NULL)
    {
        BTMW_RPC_TEST_Loge("Can't open input PCM file!\n");
        btmw_rpc_test_stream_handle = -1;
        return NULL;
    }
    BTMW_RPC_TEST_Logd("open input PCM file success!\n");

    //fInputDumpPCM = fopen("/data/sda1/send_before.pcm", "wb+");

    pcm_buffer = (UINT8 *)malloc(PCM_BUFFER_SIZE);
    if (pcm_buffer == NULL)
    {
        fclose(fInputPCM);
        BTMW_RPC_TEST_Loge("Can't allocat buffer\n");
        btmw_rpc_test_stream_handle = -1;
        return NULL;
    }

    if (0 > adev_open(NULL, AUDIO_HARDWARE_INTERFACE, &device))
    {
        BTMW_RPC_TEST_Loge("open bt adev fail\n");
        goto send_audio_data_end;
    }
    hw_dev = (struct audio_hw_device *)device;
    if (0 > hw_dev->open_output_stream(hw_dev, 0, 0, 0, 0, &stream, 0))
    {
        BTMW_RPC_TEST_Loge("open out stream fail\n");
        goto send_audio_data_end;
    }

    while(read_cnt < 1)
    {
        memset(pcm_buffer, 0, PCM_BUFFER_SIZE);
        total_pcm_len = fread(pcm_buffer, sizeof(UINT8), pcm_frame_len, fInputPCM );
        if (total_pcm_len == 0)
        {
            BTMW_RPC_TEST_Loge("total_pcm_len==0\n");
            fseek(fInputPCM, 0L, SEEK_SET);
            read_cnt++;
            continue;
        }
#if 0
        /*for control pcm data send speed from uploader, avoid buffer
                    overflow due to bluedroid handle data slowly*/
        now = bt_get_microseconds();

        if (0 == g_u4last_tx_time)
        {
            g_u4last_tx_time = now;
        }
        else
        {
            interval = 1000 * (1000 * total_pcm_len  /
                        (wlength * channel_num * sample_rate));

            wait = g_u4last_tx_time + interval - now;
            //BTMW_RPC_TEST_Logd("wait %ld!\n", wait);
            //BTMW_RPC_TEST_Logd("interval %ld!\n", interval);
            //BTMW_RPC_TEST_Logd("last_tx_time %ld!\n", g_u4last_tx_time);
            if (wait > 0)
            {
                if ( wait > 50000 )
                {
                    //BTMW_RPC_TEST_Logd("wait more than 50ms!, skip!\n");
                }
                else
                {
                    //BTMW_RPC_TEST_Logd("wait %ld ms!\n", wait/1000);
                    usleep(wait);
                }
            }
            if (wait < 0)
            {
                g_u4last_tx_time = 0;
            }
            else
            {
                g_u4last_tx_time += (interval + wait);
            }
        }
        /*end of speed control*/
#endif

        stream->write(stream, pcm_buffer, total_pcm_len);
    }
send_audio_data_end:

    if (NULL != hw_dev)
    {
        hw_dev->close_output_stream(hw_dev, stream);
    }

    if (NULL != device)
    {
        device->close(device);
    }
    fclose(fInputPCM);
    fInputPCM = NULL;
    free(pcm_buffer);
    pcm_buffer = NULL;
    btmw_rpc_test_stream_handle = -1;
    BTMW_RPC_TEST_Logd("Send audio finished\n");
    return NULL;
}


static VOID btmw_rpc_test_send_audio_data(VOID)
{
    INT32 result;

    BTMW_RPC_TEST_Logd("%s()\n", __func__);

    if(-1 == btmw_rpc_test_stream_handle)
    {
        result = pthread_create(&btmw_rpc_test_stream_handle, NULL, btmw_rpc_test_send_audio_data_thread, btmw_rpc_test_pcm_file);
        if (result)
        {
            BTMW_RPC_TEST_Logd("pthread_create failed! (%d)\n", result);
        }
    }
    else
    {
        BTMW_RPC_TEST_Logw("streaming thread has been created!\n");
    }
}

static int btmw_rpc_test_a2dp_src_stream_handler(int argc, char *argv[])
{
    CHAR ptr[16];
    bt_a2dp_status_t status = BT_A2DP_SUSPEND;

    BTMW_RPC_TEST_Logd("%s()\n", __func__);

    if (argc != 1)
    {
        strcpy(btmw_rpc_test_pcm_file, argv[1]);
    }

    strncpy(ptr, argv[0], sizeof(ptr));
    if (!strncmp(ptr, "start", strlen("start")))
    {
        status = BT_A2DP_START;
        a_mtkapi_a2dp_cmd(status);
        g_stream_status = 1;
        btmw_rpc_test_send_audio_data();
    }
    else if (!strncmp(ptr, "suspend", strlen("suspend")))
    {
        status = BT_A2DP_SUSPEND;
        a_mtkapi_a2dp_cmd(status);
        g_stream_status = 0;
    }
    else
    {
        BTMW_RPC_TEST_Loge("input error\n");
        BTMW_RPC_TEST_Loge("please input stream <start|suspend>\n");
        return -1;
    }
    return 0;
}

static int btmw_rpc_test_a2dp_src_write_stream_handler(int argc, char *argv[])
{
    INT32 result;

    BTMW_RPC_TEST_Logd("%s(), file:%s\n", __func__, argv[0]);
    strcpy(btmw_rpc_test_pcm_file, argv[0]);

    if(-1 == btmw_rpc_test_stream_handle)
    {
        result = pthread_create(&btmw_rpc_test_stream_handle, NULL,
            btmw_rpc_test_write_audio_data_thread, btmw_rpc_test_pcm_file);
        if (result)
        {
            BTMW_RPC_TEST_Logd("pthread_create failed! (%d)\n", result);
        }
    }
    else
    {
        BTMW_RPC_TEST_Logw("streaming thread has been created!\n");
    }

    return 0;
}

static int btmw_rpc_test_a2dp_get_connect_status(int argc, char *argv[])
{
    BOOL status = FALSE;

    BTMW_RPC_TEST_Logd("%s()\n", __func__);

    if (argc != 0)
    {
        BTMW_RPC_TEST_Loge("[USAGE] connect <addr>");
        return -1;
    }

    status = a_mtkapi_a2dp_get_connect_status();
    BTMW_RPC_TEST_Loge("Get status:%d", status);
    return 0;
}

static int btmw_rpc_test_a2dp_src_mute_device(int argc, char *argv[])
{
    UINT8 fg_mute = 0;
    BTMW_RPC_TEST_Logd("%s()\n", __func__);

    if (argc != 1)
    {
        BTMW_RPC_TEST_Loge("[USAGE] mute <1:mute|0:don't mute>");
        return -1;
    }

    fg_mute = atoi(argv[0]);
    BTMW_RPC_TEST_Logd("%s() %s the device\n", __func__, fg_mute != 0 ? "try to mute" : "don't mute");

    a_mtkapi_a2dp_mute_device(fg_mute);

    return 0;

}

int btmw_rpc_test_a2dp_src_cmd_handler(int argc, char **argv)
{
    BTMW_RPC_TEST_CLI *cmd, *match = NULL;
    INT32 ret = 0;
    INT32 count = 0;

    BTMW_RPC_TEST_Logd("%s argc: %d, argv[0]: %s\n", __func__, argc, argv[0]);

    cmd = btmw_rpc_test_a2dp_src_cli_commands;
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

        btmw_rpc_test_print_cmd_help(BTMW_RPC_TEST_CMD_KEY_A2DP_SRC, btmw_rpc_test_a2dp_src_cli_commands);
        ret = -1;
    }
    else
    {
        match->handler(argc - 1, &argv[1]);
    }

    return ret;
}

INT32 btmw_rpc_test_a2dp_src_init(VOID)
{
    INT32 ret = 0;
    BTMW_RPC_TEST_MOD a2dp_src_mod = {0};

    a2dp_src_mod.mod_id = BTMW_RPC_TEST_MOD_A2DP_SRC;
    strncpy(a2dp_src_mod.cmd_key, BTMW_RPC_TEST_CMD_KEY_A2DP_SRC, sizeof(a2dp_src_mod.cmd_key));
    a2dp_src_mod.cmd_handler = btmw_rpc_test_a2dp_src_cmd_handler;
    a2dp_src_mod.cmd_tbl = btmw_rpc_test_a2dp_src_cli_commands;

    ret = btmw_rpc_test_register_mod(&a2dp_src_mod);
    BTMW_RPC_TEST_Logd("btmw_rpc_test_register_mod() for SRC returns: %d\n", ret);

    return ret;
}



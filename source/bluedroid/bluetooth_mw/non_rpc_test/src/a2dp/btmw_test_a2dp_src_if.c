#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>
//#include <sys/time.h>

#include "bluetooth.h"
#include "u_bluetooth.h"
#include "c_bt_mw_a2dp_common.h"
#include "c_bt_mw_a2dp_src.h"
#include "c_bt_mw_manager.h"
#include "btmw_test_cli.h"
#include "btmw_test_debug.h"
#include "bt_a2dp_alsa_uploader.h"

#define BTMW_TEST_CMD_KEY_A2DP_SRC        "MW_A2DP_SRC"
#define PCM_BUFFER_SIZE 512*8     // at least 512

/* 0:pause, 1: play */
INT32 g_stream_status;
UINT8 g_new_PCM_file_flag = 0; // set to 1 when a new PCM file is specified
pthread_t btmw_test_stream_handle = -1;
CHAR btmw_test_default_pcm_file[128] = "/data/usb/music/48000/input_48000.pcm";
INT32 g_a2dp_connection_established = 0;
UINT32 g_u4last_tx_time = 0;
static UINT8 g_a2dp_aging_test = 0;
VOID btmw_test_send_audio_data(VOID);

extern BT_AUDIO_SRC_CB_FUNC upload_func;
extern VOID *btmw_test_gap_get_profile_interface(const CHAR *profile_id);
extern VOID bt_a2dp_alsa_setDebugFlag(INT32 flag, UINT32 value);

// CLI handler
static INT32 btmw_test_a2dp_src_connect_int_handler(INT32 argc, CHAR *argv[]);
static INT32 btmw_test_a2dp_src_disconnect_handler(INT32 argc, CHAR *argv[]);
static INT32 btmw_test_a2dp_src_passthrough_cmd_handler(INT32 argc, CHAR *argv[]);
static INT32 btmw_test_a2dp_src_send_data_handler(INT32 argc, CHAR *argv[]);
static INT32 btmw_test_a2dp_src_aging_test_handler(INT32 argc, CHAR *argv[]);
static INT32 btmw_test_a2dp_src_active_handler(INT32 argc, CHAR *argv[]);
static INT32 btmw_test_a2dp_src_set_audio_hw_dbg_lvl_handler(INT32 argc, CHAR *argv[]);
static INT32 btmw_test_a2dp_set_upl_wait_handler(INT32 argc, CHAR *argv[]);
static INT32 btmw_test_a2dp_set_read_frames_handler(INT32 argc, CHAR *argv[]);
static INT32 btmw_test_a2dp_enable_dump_handler(INT32 argc, CHAR *argv[]);
static INT32 btmw_test_a2dp_set_push_len_handler(INT32 argc, CHAR *argv[]);
static INT32 btmw_test_a2dp_set_send_wait_handler(INT32 argc, CHAR *argv[]);
static INT32 btmw_test_a2dp_set_drop_time_handler(INT32 argc, CHAR *argv[]);

static BTMW_TEST_CLI btmw_test_a2dp_src_cli_commands[] =
{
    {"audio_hw_log",  btmw_test_a2dp_src_set_audio_hw_dbg_lvl_handler,      " = audio_hw_log <0~6>"},
    {"connect",       btmw_test_a2dp_src_connect_int_handler,               " = connect <addr>"},
    {"disconnect",    btmw_test_a2dp_src_disconnect_handler,                " = disconnect <addr>"},
    {"stream",        btmw_test_a2dp_src_passthrough_cmd_handler,           " = stream <start|suspend>"},
    {"send_data",     btmw_test_a2dp_src_send_data_handler,                 " = send_data  <1:start|0:stop>"},
    {"aging_test",    btmw_test_a2dp_src_aging_test_handler,                " = aging_test <1:enable|0:disable>"},
    {"active_src",    btmw_test_a2dp_src_active_handler,                    " = active_src <1:enable|0:disable>"},
    {"set_upl_wait",  btmw_test_a2dp_set_upl_wait_handler,                  " = set_upl_wait <us>"},
    {"set_rd_frm",    btmw_test_a2dp_set_read_frames_handler,               " = set_rd_frm <count>"},
    {"enable_dump",   btmw_test_a2dp_enable_dump_handler,                   " = enable_dump <1:enable|0:disable>"},
    {"set_push_len",  btmw_test_a2dp_set_push_len_handler,                  " = set_push_len <count>"},
    {"set_send_wait", btmw_test_a2dp_set_send_wait_handler,                 " = set_send_wait <us>"},
    {"drop_time",     btmw_test_a2dp_set_drop_time_handler,                 " = drop_time <us>"},
    {NULL, NULL, NULL},
};

UINT32 bt_get_microseconds(VOID)
{
    struct timespec now;
    clock_gettime(CLOCK_MONOTONIC, &now);
    return (now.tv_sec * 1000000 + now.tv_nsec / 1000);
}

static INT32 btmw_test_a2dp_src_set_audio_hw_dbg_lvl_handler(INT32 argc, CHAR *argv[])
{
    UINT8 u1_log_lvl = 0;
    BTMW_TEST_Logi("%s()\n", __func__);

    if (argc != 1)
    {
        BTMW_TEST_Loge("[USAGE] audio_hw_log <0~6>");
        return -1;
    }

    u1_log_lvl = atoi(argv[0]);
    if (0 <= u1_log_lvl && 6 >= u1_log_lvl)
    {
        c_btm_a2dp_set_audio_hw_log_lvl(u1_log_lvl);
    }
    else
    {
        BTMW_TEST_Loge("input error\n");
        BTMW_TEST_Loge("please input audio_hw_log <0~6>\n");
    }
    return 0;
}

static INT32 btmw_test_a2dp_src_active_handler(INT32 argc, CHAR *argv[])
{
    BTMW_TEST_Logi("%s()\n", __func__);
    UINT8 u1_enable = 0;
    if (argc != 1)
    {
        BTMW_TEST_Loge("[USAGE] active_src [1/0]\n");
        return -1;
    }

    u1_enable = atoi(argv[0]);

    c_btm_btstart("PAIRING", 300);

    if (1 == u1_enable)
    {
        c_btm_source_sink_on_off(TRUE, FALSE);
    }
    else if (0 == u1_enable)
    {
        c_btm_source_sink_on_off(FALSE, TRUE);
    }
    else
    {
        BTMW_TEST_Loge("input error\n");
        BTMW_TEST_Loge("please input active_src <1:enable|0:disable>\n");
        BTMW_TEST_Loge("just remind:at the same time only src or sink role can be enabled\n");
    }
    g_stream_status = 0;

    return 0;
}

static INT32 btmw_test_a2dp_set_upl_wait_handler(INT32 argc, CHAR *argv[])
{
    UINT32 wait_ms = 0;
    BTMW_TEST_Logi("%s()\n", __func__);

    wait_ms = atoi(argv[0]);

    bt_a2dp_alsa_setDebugFlag(0, wait_ms);

    return 0;
}

static INT32 btmw_test_a2dp_set_read_frames_handler(INT32 argc, CHAR *argv[])
{
    UINT32 frames = 0;
    BTMW_TEST_Logi("%s()\n", __func__);

    frames = atoi(argv[0]);

    bt_a2dp_alsa_setDebugFlag(1, frames);

    return 0;
}

static INT32 btmw_test_a2dp_enable_dump_handler(INT32 argc, CHAR *argv[])
{
    UINT32 enable = 0;
    BTMW_TEST_Logi("%s()\n", __func__);

    enable = atoi(argv[0]);

    bt_a2dp_alsa_setDebugFlag(2, enable);

    return 0;
}

static INT32 btmw_test_a2dp_set_push_len_handler(INT32 argc, CHAR *argv[])
{
    UINT32 len = 0;
    BTMW_TEST_Logi("%s()\n", __func__);

    len = atoi(argv[0]);

    bt_a2dp_alsa_setDebugFlag(3, len);

    return 0;
}

static INT32 btmw_test_a2dp_set_send_wait_handler(INT32 argc, CHAR *argv[])
{
    UINT32 send_wait_us = 0;
    BTMW_TEST_Logi("%s()\n", __func__);

    send_wait_us = atoi(argv[0]);

    bt_a2dp_alsa_setDebugFlag(4, send_wait_us);

    return 0;
}

static INT32 btmw_test_a2dp_set_drop_time_handler(INT32 argc, CHAR *argv[])
{
    UINT32 send_wait_us = 0;
    BTMW_TEST_Logi("%s()\n", __func__);

    send_wait_us = atoi(argv[0]);

    bt_a2dp_alsa_setDebugFlag(5, send_wait_us);

    return 0;
}

static INT32 btmw_test_a2dp_src_connect_int_handler(INT32 argc, CHAR *argv[])
{
    CHAR *ptr;

    BTMW_TEST_Logd("%s()\n", __func__);

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
    c_btm_connect_audio_sources(ptr, CONN_TYPE_A2DP);

    return 0;
}

static INT32 btmw_test_a2dp_src_disconnect_handler(INT32 argc, CHAR *argv[])
{
    CHAR *ptr;

    BTMW_TEST_Logd("%s()\n", __func__);

    if (argc != 1)
    {
        BTMW_TEST_Loge("[USAGE] disconnect <addr>");
        return -1;
    }
    if (strlen(argv[0]) < 17)
    {
        BTMW_TEST_Loge("<addr> invalid. Good example is \"AA:BB:CC:DD:EE:FF\"");
        BTMW_TEST_Loge("[USAGE] disconnect <addr>");
        return -1;
    }

    ptr = argv[0];
    c_btm_bt_disconnect_audio_sources(ptr, CONN_TYPE_A2DP);
    return 0;
}

static VOID* btmw_test_send_audio_data_thread(VOID *arg)
{
    UINT8 *pcm_buffer = NULL;
    CHAR *local_test_pcm_file = btmw_test_default_pcm_file;
    FILE *fInputPCM = NULL;
    INT32 result=0;
    INT32 pcm_frame_len = PCM_BUFFER_SIZE;
    INT32 total_pcm_len;
    INT32 now = 0;
    UINT32 interval = 0;
    INT32 wait = 0;
    UINT32 wlength = 2;
    UINT32 channel_num = 2;
    UINT32 sample_rate = 44100;
    UINT16 counter = 0;

    BTMW_TEST_Logd("Input file name  : %s\n", local_test_pcm_file);

    /* open file & allocate memory */
    fInputPCM = fopen(local_test_pcm_file, "rb");
    if (NULL == fInputPCM)
    {
        BTMW_TEST_Logw("Can't open input PCM file!\n");
        // return NULL;
    }

    pcm_buffer = (UINT8 *)malloc(PCM_BUFFER_SIZE);
    if (NULL == pcm_buffer)
    {
        BTMW_TEST_Loge("Can't allocat buffer\n");
        if (NULL != fInputPCM)
        {
            fclose(fInputPCM);
        }
        return NULL;
    }
    memset(pcm_buffer, 0, PCM_BUFFER_SIZE);

    while (1)
    {
        if (0 == g_a2dp_connection_established)
        {
            BTMW_TEST_Logd("A2DP is disconnected!\n");
            goto send_audio_data_end;
        }
        if (1 == g_stream_status)
        {
            if (1 == g_new_PCM_file_flag)
            {
                if (NULL != fInputPCM)
                {
                    fclose(fInputPCM);
                    fInputPCM = NULL;
                }
                fInputPCM = fopen(local_test_pcm_file, "rb");
                if (NULL == fInputPCM)
                {
                    BTMW_TEST_Loge("Can't open the new PCM file!\n");
                    goto send_audio_data_end;
                }
                g_new_PCM_file_flag = 0;
            }
            if (NULL == fInputPCM)
            {
                usleep(500000);
                counter++;
                if (counter % 1000 == 0)
                {
                    BTMW_TEST_Logi("cannot open PCM file! you can specify it using the CLI \"MW_A2DP_SRC send_data 1 pcm_file_path\"\n");
                }
                continue;
            }
            total_pcm_len = fread(pcm_buffer, sizeof(UINT8), pcm_frame_len, fInputPCM);
            if (total_pcm_len == 0)
            {
                BTMW_TEST_Loge("total_pcm_len==0\n");
                if (0 != feof(fInputPCM))
                {
                    BTMW_TEST_Logd("PCM file has completed playing!\n");
                }
                if (0 == g_a2dp_aging_test)
                {
                    goto send_audio_data_end;
                }
                else if (1 == g_a2dp_aging_test)
                {
                    fseek(fInputPCM, 0, SEEK_SET);
                    BTMW_TEST_Logd("play again.\n");
                    continue;
                }
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
                // BTMW_TEST_Logd("wait %ld!\n", wait);
                // BTMW_TEST_Logd("interval %ld!\n", interval);
                // BTMW_TEST_Logd("last_tx_time %ld!\n", g_u4last_tx_time);
                if (wait > 0)
                {
                    if ( wait > 50000 )
                    {
                        //BTMW_TEST_Logd("wait more than 50ms!, skip!\n");
                    }
                    else
                    {
                        //BTMW_TEST_Logd("wait %ld ms!\n", wait/1000);
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

            result = c_btm_a2dp_send_stream_data((const CHAR*)pcm_buffer, total_pcm_len);
            if (result != 0)
            {
                BTMW_TEST_Loge("BtA2dpSendData Failed!\n");
                //goto send_audio_data_end;
            }
            else
            {
                counter++;
                if (counter % 300 == 0)
                {
                    BTMW_TEST_Logd("send data successful. result:%ld   total_pcm_len:%ld\n", (long)result, (long)total_pcm_len);
                }
            }
        }
        else
        {
            usleep(500000);
            if (g_a2dp_connection_established && BT_A2DP_ROLE_SRC == c_btm_get_a2dp_role())
            {
                BTMW_TEST_Logd("[A2DP SRC]waiting for audio data......\n");
            }
        }
    }
send_audio_data_end:

    if (NULL != fInputPCM)
    {
        fclose(fInputPCM);
        fInputPCM = NULL;
    }
    if (NULL != pcm_buffer)
    {
        free(pcm_buffer);
        pcm_buffer = NULL;
    }
    g_stream_status = 0;
    g_new_PCM_file_flag = 0;
    btmw_test_stream_handle = -1;
    BTMW_TEST_Logd("Exit this send audio data thread.\n");
    return NULL;
}

VOID btmw_test_send_audio_data(VOID)
{
    INT32 result;

    BTMW_TEST_Logd("%s()\n", __func__);

    if (-1 == btmw_test_stream_handle)
    {
        result = pthread_create(&btmw_test_stream_handle, NULL, btmw_test_send_audio_data_thread, NULL);
        if (result)
        {
            BTMW_TEST_Logd("pthread_create failed! (%d)\n", result);
        }
    }
    else
    {
        BTMW_TEST_Logw("streaming thread has been created!\n");
    }
}

static INT32 btmw_test_a2dp_src_passthrough_cmd_handler(INT32 argc, CHAR *argv[])
{
    CHAR ptr[16];
    bt_a2dp_status_t status = BT_A2DP_SUSPEND;

    BTMW_TEST_Logd("%s()\n", __func__);

    if (!((argc == 1)))
    {
        BTMW_TEST_Loge("stream <start|suspend>\n");
        return -1;
    }

    strncpy(ptr, argv[0], sizeof(ptr));
    if (!strncmp(ptr, "start", strlen("start")))
    {
        status = BT_A2DP_START;
        /* call c_btm_a2dp_cmd(BT_A2DP_START) to set the A2DP stream state
                if (out->common.state == AUDIO_A2DP_STATE_SUSPENDED)
                    out->common.state = AUDIO_A2DP_STATE_STANDBY;
             */
        c_btm_a2dp_cmd(status);
        c_btm_a2dp_send_stream_data(NULL, 0); // to trigger A2DP START
    }
    else if (!strncmp(ptr, "suspend", strlen("suspend")))
    {
        status = BT_A2DP_SUSPEND;
        g_stream_status = 0;
        c_btm_a2dp_cmd(status);
    }
    else
    {
        BTMW_TEST_Loge("input error\n");
        BTMW_TEST_Loge("please input stream <start|suspend>\n");
        return -1;
    }
    // c_btm_a2dp_cmd(status);
    return 0;
}

static INT32 btmw_test_a2dp_src_send_data_handler(INT32 argc, CHAR *argv[])
{
    UINT8 u1_state = 0;
    BTMW_TEST_Logi("%s()\n", __func__);

    if ((argc != 1) && (argc != 2))
    {
        BTMW_TEST_Loge("[USAGE] send_data [1:start|0:stop] [PCM_file_path]\n");
        BTMW_TEST_Loge("the 1st parameter:enable/disable send A2DP data, the 2nd parameter set the PCM file path if A2DP send data enable.\n");
        return -1;
    }

    if (argc == 2)
    {
        if (access(argv[1], F_OK) == 0)
        {
            strncpy(btmw_test_default_pcm_file, argv[1], 127);
            btmw_test_default_pcm_file[127] = '\0';
            BTMW_TEST_Logd("PCM file path is: %s\n", btmw_test_default_pcm_file);
            g_new_PCM_file_flag = 1;
        }
        else
        {
            BTMW_TEST_Logw("file %s not existed, please check the path.\n", argv[1]);
        }
    }
    else
    {
        BTMW_TEST_Logd("use the default PCM file: %s\n", btmw_test_default_pcm_file);
    }

    u1_state = atoi(argv[0]);
    BTMW_TEST_Logi("send_data %s.\n", u1_state ? "start" : "stop");
    if (1 == u1_state)
    {
        if (g_a2dp_connection_established)
        {
            g_stream_status = u1_state;
        }
        else
        {
            BTMW_TEST_Logw("A2DP not connected, so no need send audio!\n");
            return -1;
        }
    }
    else if (0 == u1_state)
    {
        g_stream_status = u1_state;
    }
    else
    {
        BTMW_TEST_Loge("input error\n");
        BTMW_TEST_Loge("please input send_data [1:start|0:stop]\n");
        return -1;
    }


    return 0;
}
static INT32 btmw_test_a2dp_src_aging_test_handler(INT32 argc, CHAR *argv[])
{
    UINT8 u1_enable = 0;
    BTMW_TEST_Logi("%s()\n", __func__);

    if (argc != 1)
    {
        BTMW_TEST_Loge("[USAGE] aging_test [1:enable|0:disable]");
        return -1;
    }

    u1_enable = atoi(argv[0]);

    if ((1 == u1_enable) || (0 == u1_enable))
    {
        g_a2dp_aging_test = u1_enable;
    }
    else
    {
        BTMW_TEST_Loge("input error\n");
        BTMW_TEST_Loge("please input aging_test <1:enable|0:disable>\n");
        return -1;
    }

    BTMW_TEST_Loge("g_a2dp_aging_test %s.\n", g_a2dp_aging_test ? "enabled" : "disabled");

    return 0;

}

INT32 btmw_test_a2dp_src_cmd_handler(INT32 argc, CHAR **argv)
{
    BTMW_TEST_CLI *cmd, *match = NULL;
    INT32 ret = 0;
    INT32 count = 0;

    BTMW_TEST_Logd("%s argc: %d, argv[0]: %s\n", __func__, argc, argv[0]);

    cmd = btmw_test_a2dp_src_cli_commands;
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

        btmw_test_print_cmd_help(BTMW_TEST_CMD_KEY_A2DP_SRC, btmw_test_a2dp_src_cli_commands);
        ret = -1;
    }
    else
    {
        match->handler(argc - 1, &argv[1]);
    }

    return ret;
}

INT32 btmw_test_a2dp_src_init(VOID)
{
    INT32 ret = 0;
    BTMW_TEST_MOD a2dp_src_mod = {0};

    a2dp_src_mod.mod_id = BTMW_TEST_MOD_A2DP_SRC;
    strncpy(a2dp_src_mod.cmd_key, BTMW_TEST_CMD_KEY_A2DP_SRC, sizeof(a2dp_src_mod.cmd_key));
    a2dp_src_mod.cmd_handler = btmw_test_a2dp_src_cmd_handler;
    a2dp_src_mod.cmd_tbl = btmw_test_a2dp_src_cli_commands;

    ret = btmw_test_register_mod(&a2dp_src_mod);
    BTMW_TEST_Logd("btmw_test_register_mod() for SRC returns: %d\n", ret);
    
    //c_btm_a2dp_register_uploader_cb(bt_a2dp_alsa_getUploader());
    //bt_a2dp_alsa_regOutputCallback(c_btm_a2dp_send_stream_data);

    return ret;
}



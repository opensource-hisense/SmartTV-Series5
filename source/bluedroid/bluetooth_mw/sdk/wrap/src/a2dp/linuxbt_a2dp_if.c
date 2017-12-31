#include <string.h>
#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <sys/un.h>
#include <errno.h>
#include <time.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <pthread.h>

#include "bluetooth.h"
#include "bt_av.h"
#if defined(MTK_LINUX_AVRCP_PLUS) && (MTK_LINUX_AVRCP_PLUS == TRUE)
#include "mtk_bt_rc.h"
#else
#include "bt_rc.h"
#endif
#include "bt_audio_track.h"
#include "mtk_audio.h"

#include "bt_mw_common.h"
#include "bluetooth_sync.h"
#include "linuxbt_gap_if.h"
#include "linuxbt_a2dp_if.h"
#include "linuxbt_common.h"
#include "bt_mw_msg.h"


#include "bt_mw_a2dp_common.h"
#include "linuxbt_common.h"
#include "linuxbt_msg.h"
#if ENABLE_A2DP_SRC
//#include "bt_a2dp_uploader.h"
#endif


#if ENABLE_A2DP_SINK
//static BtifAvrcpAudioTrack *g_pt_AudioTrackCb = NULL;
#endif

#if ENABLE_A2DP_SRC
extern BT_AUDIO_SRC_CB_FUNC upload_func;
#endif

struct audio_hw_device g_audio_dev;
struct audio_stream_out *g_stream_out;
static BOOL g_fg_audio_path_init = FALSE;
extern int adev_open_output_stream(struct audio_hw_device *dev,
                                       audio_io_handle_t handle,
                                       audio_devices_t devices,
                                       audio_output_flags_t flags,
                                       struct audio_config *config,
                                       struct audio_stream_out **stream_out,
                                       const char *address);
extern void adev_close_output_stream(struct audio_hw_device *dev,
                                         struct audio_stream_out *stream);
extern uint8_t audio_set_trace_level(uint8_t new_level);

VOID init_audio_path(VOID)
{
    BT_DBG_NORMAL(BT_DEBUG_A2DP, "[A2DP] Enter+++ %s()", __FUNCTION__);
    memset(&g_audio_dev, 0x0, sizeof(g_audio_dev));
    g_stream_out = NULL;
    adev_open_output_stream(&g_audio_dev, 0, 0, 0, NULL, &g_stream_out, NULL);
    g_fg_audio_path_init = TRUE;
    BT_DBG_NORMAL(BT_DEBUG_A2DP, "[A2DP] Exit--- %s()", __FUNCTION__);
}
//g_stream_out->common.set_parameters(g_stream_out->common, );
VOID uninit_audio_path(VOID)
{
    BT_DBG_NORMAL(BT_DEBUG_A2DP, "[A2DP] Enter+++ %s()", __FUNCTION__);
    if (NULL != g_stream_out)
    {
        g_stream_out->common.set_parameters(&g_stream_out->common, "closing=true");
        g_stream_out->common.standby(&g_stream_out->common);
        adev_close_output_stream(&g_audio_dev, g_stream_out);
        g_stream_out = NULL;
        g_fg_audio_path_init = FALSE;
    }
    BT_DBG_NORMAL(BT_DEBUG_A2DP, "[A2DP] Exit--- %s()", __FUNCTION__);
}


#define MSG_ID_BT_A2DP_STREAM_DATA_SEND_REQ 401
#define SBC_BUFFER_SIZE 1024
#define PCM_BUFFER_SIZE 4096

static BOOL g_fg_sink_enable = FALSE;
static BOOL g_fg_src_enable = FALSE;

CHAR current_a2dp_addr[18]={0};
static BOOL g_scmst_support;
static UINT32 g_remote_supported_sampling_rate;
static UINT8 g_stream_type;
static UINT32 g_sample_rate;
static UINT8 g_channel_mode;

//static btav_audio_cap_struct local_audio_cap;
extern BtAppEventCbk      BtAppCbk;
extern BtAppA2dpPlaybackCtrlCbk AppPlaybackCtrlCbk;
extern INT32 bluetooth_mgr_connect_fail(CHAR *bdAddr);

UINT8 g_local_role;

extern VOID *linuxbt_gap_get_profile_interface(const CHAR *profile_id);

// Callback functions
static void linuxbt_a2dp_src_connection_state_cb(btav_connection_state_t state,
                                                        bt_bdaddr_t *bd_addr);
static void linuxbt_a2dp_src_audio_state_cb(btav_audio_state_t state,
                                                   bt_bdaddr_t* bd_addr);
static void linuxbt_a2dp_src_audio_config_cb(bt_bdaddr_t *bd_addr,
                                                    uint32_t sample_rate,
                                                    uint8_t channel_count);


static void linuxbt_a2dp_sink_connection_state_cb(btav_connection_state_t state,
                                                        bt_bdaddr_t*bd_addr);

static void linuxbt_a2dp_sink_audio_config_cb(bt_bdaddr_t *bd_addr,
                                                    uint32_t sample_rate,
                                                    uint8_t channel_count);
static void linuxbt_a2dp_sink_audio_state_cb(btav_audio_state_t state,
                                                bt_bdaddr_t *bd_addr);

static btav_interface_t *g_bt_a2dp_src_interface = NULL;
static btav_callbacks_t g_bt_a2dp_src_callbacks =
{
    sizeof(btav_callbacks_t),
    linuxbt_a2dp_src_connection_state_cb,
    linuxbt_a2dp_src_audio_state_cb,
    linuxbt_a2dp_src_audio_config_cb,
};

static btav_interface_t *g_bt_a2dp_sink_interface = NULL;
static btav_callbacks_t g_bt_a2dp_sink_callbacks =
{
    sizeof(btav_callbacks_t),
    linuxbt_a2dp_sink_connection_state_cb,
    linuxbt_a2dp_sink_audio_state_cb,
    linuxbt_a2dp_sink_audio_config_cb,
};

VOID linuxbt_a2dp_src_handle_connected_event(VOID)
{
    BT_DBG_NORMAL(BT_DEBUG_A2DP, "[A2DP] %s()", __FUNCTION__);

    btaudio_handle_a2dp_stream_open_cb(current_a2dp_addr);
    init_audio_path();
}

VOID linuxbt_a2dp_src_handle_disconnected_event(VOID)
{
    BT_DBG_NORMAL(BT_DEBUG_A2DP, "[A2DP] %s()", __FUNCTION__);

    btaudio_handle_a2dp_stream_close_cb(current_a2dp_addr);
    uninit_audio_path();
}

static void linuxbt_a2dp_src_connection_state_cb(btav_connection_state_t state,
                                                        bt_bdaddr_t *bd_addr)
{
    CHAR bt_address[18];
    LINUXBT_MSG_T linuxbt_message = { 0 };

    BT_DBG_NORMAL(BT_DEBUG_A2DP, "[A2DP] src state: %s(%d) ",
                  state==BTAV_CONNECTION_STATE_DISCONNECTED?"DISCONNECTED":
                  state==BTAV_CONNECTION_STATE_CONNECTING?"CONNECTING":
                  state==BTAV_CONNECTION_STATE_CONNECTED?"CONNECTED":
                  state==BTAV_CONNECTION_STATE_DISCONNECTING?"DISCONNECTING":
                  "UNKNOWN",
                  state);

    memset(bt_address, 0x0, sizeof(bt_address));

    if (NULL != bd_addr)
    {
        linuxbt_btaddr_htos(bd_addr, bt_address);
        BT_DBG_NORMAL(BT_DEBUG_A2DP, "address:%02X:%02X:%02X:%02X:%02X:%02X",
                    bd_addr->address[0],
                    bd_addr->address[1],
                    bd_addr->address[2],
                    bd_addr->address[3],
                    bd_addr->address[4],
                    bd_addr->address[5]);
        strncpy(current_a2dp_addr, bt_address, strlen(bt_address));
    }
    else
    {
        BT_DBG_WARNING(BT_DEBUG_A2DP, "invalid address");
    }
    set_bt_a2dp_pause_done(TRUE);
    linuxbt_message.tMsgType = LINUXBT_MSG_FROM_WRAP;

    if (state == BTAV_CONNECTION_STATE_DISCONNECTED)
    {
        g_scmst_support = FALSE;
        g_remote_supported_sampling_rate = 0;
        g_stream_type = 100;
        set_bt_a2dp_disconnecting(FALSE);
        if (get_bt_a2dp_connect())
        {
            linuxbt_message.body.evtType = LINUXBT_A2DP_SRC_EVENT_STREAM_CLOSED;
            set_bt_a2dp_connect(FALSE);
            set_bt_stream_closed(TRUE);
            i4LinuxBtSendMsg(&linuxbt_message);
        }
        else
        {
            /*connection failure*/
            BT_DBG_ERROR(BT_DEBUG_A2DP, "a2dp stream open fail");
            BT_DBG_NORMAL(BT_DEBUG_A2DP, "notify APP connection timeout");
            bluetooth_mgr_connect_fail(current_a2dp_addr);
        }
    }
    else if (state == BTAV_CONNECTION_STATE_CONNECTED)
    {
        g_sample_rate  = 0x02;
        g_channel_mode = 0x01;
        set_bt_a2dp_disconnecting(FALSE);
        if (!get_bt_a2dp_connect())
        {
            linuxbt_message.body.evtType = LINUXBT_A2DP_SRC_EVENT_STREAM_OPEN;
            set_bt_stream_closed(FALSE);
            set_bt_a2dp_connect(TRUE);
            i4LinuxBtSendMsg(&linuxbt_message);
        }
        else
        {
            /*connection has been established before this point*/
            BT_DBG_INFO(BT_DEBUG_A2DP, "a2dp stream open before this point");
        }
    }
    else if (state == BTAV_CONNECTION_STATE_DISCONNECTING)
    {
        set_bt_a2dp_disconnecting(TRUE);
#if ENABLE_A2DP_SRC
        if (NULL != upload_func.bt_upd_stop_cb)
        {
            upload_func.bt_upd_stop_cb();
        }
        else
        {
            BT_DBG_WARNING(BT_DEBUG_COMM, "bt_upd_stop_cb func is null!");
        }
        if (NULL != upload_func.bt_upd_deinit_cb)
        {
            upload_func.bt_upd_deinit_cb();
        }
        else
        {
            BT_DBG_WARNING(BT_DEBUG_COMM, "bt_upd_deinit_cb func is null!");
        }
#endif
        //uninit_audio_path();
        linuxbt_message.body.evtType = LINUXBT_A2DP_SRC_EVENT_STREAM_CLOSING;
        i4LinuxBtSendMsg(&linuxbt_message);
    }

}

static void linuxbt_a2dp_src_audio_state_cb(btav_audio_state_t state,
                                                bt_bdaddr_t *bd_addr)
{
    BT_DBG_NORMAL(BT_DEBUG_A2DP, "[A2DP] state: %s(%d) ",
                  state==BTAV_AUDIO_STATE_STARTED?"STARTED":
                  state==BTAV_AUDIO_STATE_STOPPED?"STOPPED":
                  state==BTAV_AUDIO_STATE_REMOTE_SUSPEND?"SUSPEND":"UNKNOWN",
                  state);
    if (BTAV_AUDIO_STATE_STARTED == state)
    {
        btaudio_handle_a2dp_stream_start_cb();
    }
    else if (BTAV_AUDIO_STATE_REMOTE_SUSPEND == state)
    {
        btaudio_handle_a2dp_stream_suspend_cb();
    }
    else if (BTAV_AUDIO_STATE_STOPPED == state)
    {
        btaudio_handle_a2dp_stream_suspend_cb();
    }

}

static void linuxbt_a2dp_src_audio_config_cb(bt_bdaddr_t *bd_addr,
                                                uint32_t sample_rate,
                                                uint8_t channel_count)
{
    BT_DBG_NOTICE(BT_DEBUG_A2DP, "+++into");
    if ( sample_rate != 0 )
        g_sample_rate = sample_rate;
    if ( channel_count != 0 )
        g_channel_mode = channel_count;
    BT_DBG_INFO(BT_DEBUG_A2DP, "[A2DP] sample_rate: %d, channel_count: %d",
                sample_rate, channel_count);

    bluetooth_a2dp_save_audio_cap_info(sample_rate, channel_count);

    btaudio_handle_a2dp_stream_reconfig_cb(sample_rate, channel_count);
}

VOID linuxbt_a2dp_sink_handle_connected_event(VOID)
{
    BT_DBG_NORMAL(BT_DEBUG_A2DP, "[A2DP] %s()", __FUNCTION__);

    btaudio_handle_a2dp_stream_open_cb(current_a2dp_addr);
}

VOID linuxbt_a2dp_sink_handle_disconnected_event(VOID)
{
    BT_DBG_NORMAL(BT_DEBUG_A2DP, "[A2DP] %s()", __FUNCTION__);

    btaudio_handle_a2dp_stream_close_cb(current_a2dp_addr);
}

static void linuxbt_a2dp_sink_connection_state_cb(btav_connection_state_t state,
                                                        bt_bdaddr_t *bd_addr)
{
    CHAR bt_address[MAX_BDADDR_LEN];
    LINUXBT_MSG_T linuxbt_message = { 0 };

    BT_DBG_NORMAL(BT_DEBUG_A2DP, "[A2DP] sink state: %s(%d) ",
                  state==BTAV_CONNECTION_STATE_DISCONNECTED?"DISCONNECTED":
                  state==BTAV_CONNECTION_STATE_CONNECTING?"CONNECTING":
                  state==BTAV_CONNECTION_STATE_CONNECTED?"CONNECTED":
                  state==BTAV_CONNECTION_STATE_DISCONNECTING?"DISCONNECTING":"UNKNOWN",
                  state);

    memset(bt_address, 0x0, sizeof(bt_address));
    linuxbt_btaddr_htos(bd_addr, bt_address);
    BT_DBG_NORMAL(BT_DEBUG_GAP,"bt addr: %s", bt_address);
    strncpy(current_a2dp_addr,bt_address,strlen(bt_address));
    set_bt_a2dp_pause_done(TRUE);
    linuxbt_message.tMsgType = LINUXBT_MSG_FROM_WRAP;

    if (state == BTAV_CONNECTION_STATE_DISCONNECTED)
    {
        g_scmst_support = false;
        g_stream_type = 100;
        if (get_bt_a2dp_connect())
        {
            linuxbt_message.body.evtType = LINUXBT_A2DP_SINK_EVENT_STREAM_CLOSED;
            set_bt_a2dp_connect(FALSE);
            set_bt_stream_closed(TRUE);
            i4LinuxBtSendMsg(&linuxbt_message);
        }
        else
        {
            /*connection failure*/
            BT_DBG_ERROR(BT_DEBUG_A2DP, "a2dp stream open fail");
            BT_DBG_NORMAL(BT_DEBUG_A2DP, "notify APP connection timeout");
            bluetooth_mgr_connect_fail(bt_address);
        }
    }
    else if (state == BTAV_CONNECTION_STATE_CONNECTED)
    {
        g_sample_rate  = 0x02;
        g_channel_mode = 0x01;
        if (!get_bt_a2dp_connect())
        {
            linuxbt_message.body.evtType = LINUXBT_A2DP_SINK_EVENT_STREAM_OPEN;
            set_bt_stream_closed(FALSE);
            set_bt_a2dp_connect(TRUE);
            i4LinuxBtSendMsg(&linuxbt_message);
        }
        else
        {
            /*connection has been established before this point*/
            BT_DBG_INFO(BT_DEBUG_A2DP, "a2dp stream open before this point");
        }
    }
    else if (state == BTAV_CONNECTION_STATE_CONNECTING)
    {
        //BtifAvrcpAudioTrackInit(g_pt_AudioTrackCb);
    }
}

static void linuxbt_a2dp_sink_audio_config_cb(bt_bdaddr_t *bd_addr,
                                                    uint32_t sample_rate,
                                                    uint8_t channel_count)
{
    BT_DBG_NOTICE(BT_DEBUG_A2DP, "+++into");
    if ( sample_rate != 0 )
        g_sample_rate = sample_rate;
    if ( channel_count != 0 )
        g_channel_mode = channel_count;
    BT_DBG_INFO(BT_DEBUG_A2DP, "[A2DP] sample_rate: %d, channel_count: %d",
                sample_rate, channel_count);

    bluetooth_a2dp_save_audio_cap_info(sample_rate, channel_count);

    btaudio_handle_a2dp_stream_reconfig_cb(sample_rate, channel_count);
}

static void linuxbt_a2dp_sink_audio_state_cb(btav_audio_state_t state,
                                                bt_bdaddr_t *bd_addr)
{
    BT_DBG_NORMAL(BT_DEBUG_A2DP, "[A2DP] state: %s(%d) ",
                  state==BTAV_AUDIO_STATE_STARTED?"STARTED":
                  state==BTAV_AUDIO_STATE_STOPPED?"STOPPED":
                  state==BTAV_AUDIO_STATE_REMOTE_SUSPEND?"SUSPEND":"UNKNOWN",
                  state);
    if (BTAV_AUDIO_STATE_STARTED == state)
    {
        btaudio_handle_a2dp_stream_start_cb();
    }
    else if (BTAV_AUDIO_STATE_REMOTE_SUSPEND == state)
    {
        btaudio_handle_a2dp_stream_suspend_cb();
    }
    else if (BTAV_AUDIO_STATE_STOPPED == state)
    {
        btaudio_handle_a2dp_stream_suspend_cb();
    }
}

INT32 linuxbt_a2dp_connect_int_handler(CHAR *pbt_addr, UINT8 local_role)
{
    INT32 status = BT_SUCCESS;
    bt_bdaddr_t bdaddr;

    BT_DBG_INFO(BT_DEBUG_A2DP, "[A2DP] %s()", __FUNCTION__);
    if (NULL == pbt_addr)
    {
        BT_DBG_ERROR(BT_DEBUG_A2DP, "null pointer");
        return BT_ERR_STATUS_FAIL;
    }
    linuxbt_btaddr_stoh(pbt_addr, &bdaddr);
    BT_DBG_INFO(BT_DEBUG_A2DP, "A2DP connected to %02X:%02X:%02X:%02X:%02X:%02X",
                bdaddr.address[0],
                bdaddr.address[1],
                bdaddr.address[2],
                bdaddr.address[3],
                bdaddr.address[4],
                bdaddr.address[5]);
    BT_DBG_INFO(BT_DEBUG_A2DP, "[A2DP] Connecting as %s.....",
                (local_role == A2DP_LOCAL_ROLE_SRC)? "SOURCE":"SINK" );
    if (local_role == A2DP_LOCAL_ROLE_SNK && g_fg_sink_enable == TRUE)
    {
        if (g_bt_a2dp_sink_interface && g_bt_a2dp_sink_interface->connect)
        {
            g_bt_a2dp_sink_interface->connect(&bdaddr);
        }
        else
        {
            BT_DBG_ERROR(BT_DEBUG_A2DP, "[A2DP] A2DP SINK is not enable");
        }
    }
    else if (local_role == A2DP_LOCAL_ROLE_SRC && g_fg_src_enable == TRUE)
    {

        if (g_bt_a2dp_src_interface && g_bt_a2dp_src_interface->connect)
        {
            g_bt_a2dp_src_interface->connect(&bdaddr);
        }
        else
        {
            BT_DBG_ERROR(BT_DEBUG_A2DP, "[A2DP] A2DP SRC is not enable");
        }
    }
    else
    {
        BT_DBG_ERROR(BT_DEBUG_A2DP, "[A2DP] role is not enabled");
        return BT_ERR_STATUS_FAIL;
    }

    return status;
}

INT32 linuxbt_a2dp_disconnect_handler(CHAR *pbt_addr, UINT8 local_role)
{
    INT32 status = BT_SUCCESS;
    bt_bdaddr_t bdaddr;
    if ((g_bt_a2dp_src_interface == NULL) && (g_bt_a2dp_sink_interface == NULL))
    {
        BT_DBG_ERROR(BT_DEBUG_A2DP, "[A2DP] Failed to get A2DP SRC interface");
        return BT_ERR_STATUS_NOT_READY;
    }

    BT_DBG_NOTICE(BT_DEBUG_A2DP, "[A2DP] linuxbt_a2dp_snk_disconnect_handler");

    linuxbt_btaddr_stoh(pbt_addr, &bdaddr);
    BT_DBG_NORMAL(BT_DEBUG_A2DP, "A2DP disconnected to %02X:%02X:%02X:%02X:%02X:%02X",
                  bdaddr.address[0], bdaddr.address[1], bdaddr.address[2],
                  bdaddr.address[3], bdaddr.address[4], bdaddr.address[5]);

    if ((local_role == A2DP_LOCAL_ROLE_SNK) && (g_fg_sink_enable == TRUE))
    {
        if (g_bt_a2dp_sink_interface && g_bt_a2dp_sink_interface->disconnect)
        {
            g_bt_a2dp_sink_interface->disconnect(&bdaddr);
        }
        else
        {
            BT_DBG_ERROR(BT_DEBUG_A2DP, "[A2DP] A2DP SINK is not enable");
        }
    }
    else if ((local_role == A2DP_LOCAL_ROLE_SRC) && (g_fg_src_enable == TRUE))
    {

        if (g_bt_a2dp_src_interface && g_bt_a2dp_src_interface->disconnect)
        {
            g_bt_a2dp_src_interface->disconnect(&bdaddr);
        }
        else
        {
            BT_DBG_ERROR(BT_DEBUG_A2DP, "[A2DP] A2DP SRC is not enable");
        }
    }
    else
    {
        BT_DBG_ERROR(BT_DEBUG_A2DP, "[A2DP] role is not enabled");
        return BT_ERR_STATUS_FAIL;
    }

    return status;
}

INT32 linuxbt_a2dp_start_handler(CHAR *pbt_addr)
{
    bt_bdaddr_t bdaddr;

    BT_DBG_INFO(BT_DEBUG_A2DP, "[A2DP] %s()", __FUNCTION__);
    if (g_bt_a2dp_src_interface == NULL)
    {
        BT_DBG_ERROR(BT_DEBUG_A2DP, "[A2DP] Failed to get A2DP interface");
        return BT_ERR_STATUS_NOT_READY;
    }

    linuxbt_btaddr_stoh(pbt_addr, &bdaddr);
    BT_DBG_INFO(BT_DEBUG_A2DP, "A2DP start stream with %02X:%02X:%02X:%02X:%02X:%02X",
                bdaddr.address[0], bdaddr.address[1], bdaddr.address[2],
                bdaddr.address[3], bdaddr.address[4], bdaddr.address[5]);

    //g_bt_a2dp_src_interface->start(&bdaddr);
    if (g_fg_audio_path_init && NULL != g_stream_out)
    {
        g_stream_out->common.set_parameters(&g_stream_out->common, "A2dpSuspended=false");
    }
    return BT_SUCCESS;
}


INT32 linuxbt_a2dp_suspend_handler(CHAR *pbt_addr)
{
    bt_bdaddr_t bdaddr;

    BT_DBG_INFO(BT_DEBUG_A2DP, "[A2DP] %s()", __FUNCTION__);
    if (g_bt_a2dp_src_interface == NULL)
    {
        BT_DBG_ERROR(BT_DEBUG_A2DP, "[A2DP] Failed to get A2DP interface");
        return BT_ERR_STATUS_NOT_READY;
    }

    linuxbt_btaddr_stoh(pbt_addr, &bdaddr);
    BT_DBG_INFO(BT_DEBUG_A2DP, "A2DP suspend stream with %02X:%02X:%02X:%02X:%02X:%02X",
                bdaddr.address[0], bdaddr.address[1], bdaddr.address[2],
                bdaddr.address[3], bdaddr.address[4], bdaddr.address[5]);

    //g_bt_a2dp_src_interface->suspend(&bdaddr);
    if (g_fg_audio_path_init && NULL != g_stream_out)
    {
        g_stream_out->common.set_parameters(&g_stream_out->common, "A2dpSuspended=true");
    }
    return BT_SUCCESS;
}


INT32 linuxbt_a2dp_abort_handler(CHAR *pbt_addr)
{
    bt_bdaddr_t bdaddr;

    BT_DBG_INFO(BT_DEBUG_A2DP, "[A2DP] %s()", __FUNCTION__);

    if (g_bt_a2dp_src_interface == NULL)
    {
        BT_DBG_ERROR(BT_DEBUG_A2DP, "[A2DP] Failed to get A2DP interface");
        return BT_ERR_STATUS_NOT_READY;
    }
    linuxbt_btaddr_stoh(pbt_addr, &bdaddr);
    BT_DBG_INFO(BT_DEBUG_A2DP, "A2DP abort stream with %02X:%02X:%02X:%02X:%02X:%02X",
                bdaddr.address[0], bdaddr.address[1], bdaddr.address[2],
                bdaddr.address[3], bdaddr.address[4], bdaddr.address[5]);

    //g_bt_a2dp_src_interface->abort(&bdaddr);
    return BT_SUCCESS;
}

INT32 linuxbt_a2dp_reconfig_handler(CHAR *pbt_addr,
                                    INT32 sample_rate,
                                    INT32 channel_num)
{
    bt_bdaddr_t bdaddr;

    BT_DBG_INFO(BT_DEBUG_A2DP, "[A2DP] %s()", __FUNCTION__);
    if (g_bt_a2dp_src_interface == NULL)
    {
        BT_DBG_ERROR(BT_DEBUG_A2DP, "[A2DP] Failed to get A2DP interface");
        return BT_ERR_STATUS_NOT_READY;
    }

    linuxbt_btaddr_stoh(pbt_addr, &bdaddr);
    BT_DBG_INFO(BT_DEBUG_A2DP, "A2DP reconfigure stream with %02X:%02X:%02X:%02X:%02X:%02X",
                bdaddr.address[0], bdaddr.address[1], bdaddr.address[2],
                bdaddr.address[3], bdaddr.address[4], bdaddr.address[5]);

    BT_DBG_INFO(BT_DEBUG_A2DP, "A2DP reconfig sample rate = %ld, channel_num = %ld", (long)sample_rate, (long)channel_num);

    //g_bt_a2dp_src_interface->reconfig(&bdaddr, sample_rate, channel_num);
    return BT_SUCCESS;
}

INT32 linuxbt_a2dp_src_enable_handler(UINT8 src_enable)
{
    BT_DBG_NOTICE(BT_DEBUG_A2DP, "SRC %s", g_fg_src_enable==1?"enable":"disable");
    g_fg_src_enable = (src_enable == 1) ? TRUE : FALSE;

    return BT_SUCCESS;
}

INT32 linuxbt_a2dp_sink_enable_handler(UINT8 sink_enable)
{
    BT_DBG_NOTICE(BT_DEBUG_A2DP, "SNK %s", sink_enable==1?"enable":"disable");
    g_fg_sink_enable = (sink_enable == 1) ? TRUE : FALSE;

    return BT_SUCCESS;
}

INT32 linuxbt_a2dp_aac_enable_handler(UINT8 fg_enable)
{

    BT_DBG_INFO(BT_DEBUG_A2DP, "[A2DP] %s() AAC %s", __FUNCTION__,fg_enable?"Enable":"Disable");
    return BT_SUCCESS;
}

INT32 linuxbt_a2dp_ldac_enable_handler(UINT8 fg_enable)
{
    BT_DBG_INFO(BT_DEBUG_A2DP, "[A2DP] %s() LDAC %s", __FUNCTION__,fg_enable?"Enable":"Disable");
    return BT_SUCCESS;
}

INT32 linuxbt_a2dp_aptx_enable_handler(UINT8 fg_enable)
{
    BT_DBG_INFO(BT_DEBUG_A2DP, "[A2DP] %s() APTX %s", __FUNCTION__,fg_enable?"Enable":"Disable");
    return BT_SUCCESS;
}

INT32 linuxbt_a2dp_mp3_enable_handler(UINT8 fg_enable)
{
    BT_DBG_INFO(BT_DEBUG_A2DP, "[A2DP] %s() MP3 %s", __FUNCTION__,fg_enable?"Enable":"Disable");
    return BT_SUCCESS;
}

INT32 linuxbt_a2dp_2nd_con_enable_handler(UINT8 fg_enable)
{
    BT_DBG_INFO(BT_DEBUG_A2DP, "[A2DP] %s() %s 2nd connection", __FUNCTION__,fg_enable?"Accept":"Reject");
    return BT_SUCCESS;
}

INT32 linuxbt_a2dp_src_init(VOID)
{
    BT_DBG_INFO(BT_DEBUG_A2DP, "[A2DP] %s()", __FUNCTION__);
    INT32 ret = 0;

    // Get A2DP SRC interface
    g_bt_a2dp_src_interface = (btav_interface_t *) linuxbt_gap_get_profile_interface(BT_PROFILE_ADVANCED_AUDIO_ID);
    if (g_bt_a2dp_src_interface == NULL)
    {
        BT_DBG_ERROR(BT_DEBUG_A2DP, "[A2DP] Failed to get A2DP interface");
        return BT_ERR_STATUS_FAIL;
    }
    g_scmst_support = false;
    g_remote_supported_sampling_rate = 0;
    g_stream_type = 100;


    // Init A2DP interface
    ret = g_bt_a2dp_src_interface->init(&g_bt_a2dp_src_callbacks);
    if (ret == BT_STATUS_SUCCESS)
    {
        BT_DBG_NORMAL(BT_DEBUG_A2DP, "[A2DP] success to init A2DP interface");
    }
    else if (ret == BT_STATUS_DONE)
    {
        BT_DBG_NORMAL(BT_DEBUG_A2DP, "[A2DP] already init A2DP interface");
    }
    else
    {
        BT_DBG_ERROR(BT_DEBUG_A2DP, "[A2DP] Failed to init A2DP interface");
    }

    g_local_role = A2DP_LOCAL_ROLE_SRC; // By default, local role will be SRC

    return linuxbt_return_value_convert(ret);
}

INT32 linuxbt_a2dp_sink_init(VOID)
{
    BT_DBG_INFO(BT_DEBUG_A2DP, "[A2DP] %s()", __FUNCTION__);
    INT32 ret = 0;

    // Get A2DP SINK interface
    g_bt_a2dp_sink_interface = (btav_interface_t *)linuxbt_gap_get_profile_interface(BT_PROFILE_ADVANCED_AUDIO_SINK_ID);
    if (g_bt_a2dp_sink_interface == NULL)
    {
        BT_DBG_ERROR(BT_DEBUG_A2DP, "[A2DP] Failed to get A2DP interface");
        return BT_ERR_STATUS_FAIL;
    }
    g_scmst_support = false;

    // Init A2DP SINK interface
    ret = g_bt_a2dp_sink_interface->init(&g_bt_a2dp_sink_callbacks);
    BT_DBG_NORMAL(BT_DEBUG_A2DP, "[A2DP] g_bt_a2dp_sink_callbacks:%p", &g_bt_a2dp_sink_callbacks);
    if (ret == BT_STATUS_SUCCESS)
    {
        BT_DBG_NORMAL(BT_DEBUG_A2DP, "[A2DP] success to init A2DP interface");
    }
    else if (ret == BT_STATUS_DONE)
    {
        BT_DBG_NORMAL(BT_DEBUG_A2DP, "[A2DP] already init A2DP interface");
    }
    else
    {
        BT_DBG_ERROR(BT_DEBUG_A2DP, "[A2DP] Failed to init A2DP interface");
    }

    g_local_role = A2DP_LOCAL_ROLE_SNK;

    return linuxbt_return_value_convert(ret);
}

VOID linuxbt_a2dp_src_deinit(VOID)
{
    FUNC_ENTRY;

    if (g_bt_a2dp_src_interface != NULL)
    {
        g_bt_a2dp_src_interface->cleanup();
    }
    FUNC_EXIT;
}

VOID linuxbt_a2dp_sink_deinit(VOID)
{
    FUNC_ENTRY;

    if (g_bt_a2dp_sink_interface != NULL)
    {
        g_bt_a2dp_sink_interface->cleanup();
    }
    FUNC_EXIT;
}

INT32 linuxbt_send_stream_data(const CHAR *data, INT32 len)
{
    INT32 i4_ret = 0;
    if (g_bt_a2dp_src_interface == NULL)
    {
        BT_DBG_ERROR(BT_DEBUG_A2DP, "[A2DP] Failed to get A2DP interface");
        return BT_ERR_STATUS_FAIL;
    }
    BT_DBG_INFO(BT_DEBUG_A2DP, "[A2DP] send data length:%ld begin", (long)len);

    if (g_fg_audio_path_init && (NULL != g_stream_out) && (NULL != g_stream_out->write))
    {
        i4_ret = g_stream_out->write(g_stream_out, data, len);
        BT_DBG_INFO(BT_DEBUG_A2DP, "[A2DP] i4_ret:%ld", (long)i4_ret);
    }
    BT_DBG_INFO(BT_DEBUG_A2DP, "[A2DP] send data length:%ld end", (long)len);
    return BT_SUCCESS;
}

INT32 linuxbt_a2dp_get_sample_rate(VOID)
{
#if 0
    INT32 sample_rate = 0;
    //btav_audio_cap_struct *current_config = &local_audio_cap;
    switch ( current_config->codec_type )
    {
        case BT_A2DP_SBC:
            sample_rate = current_config->codec_cap.sbc.sample_rate;
            if ( sample_rate == 0x1 )
            {
                return 48000;
            }
            else if ( sample_rate == 0x2 )
            {
                return 44100;
            }
            else if ( sample_rate == 0x4 )
            {
                return 32000;
            }
            else if ( sample_rate == 0x8 )
            {
                return 16000;
            }
        case BT_A2DP_AAC:
            sample_rate = current_config->codec_cap.aac.sample_rate;
            BT_DBG_MINOR(BT_DEBUG_A2DP, "sample_rate:%ld",(long)sample_rate);
            if ( sample_rate == 0x800 )
            {
                return 8000;
            }
            else if ( sample_rate == 0x400 )
            {
                return 11024;
            }
            else if ( sample_rate == 0x200 )
            {
                return 12000;
            }
            else if ( sample_rate == 0x100 )
            {
                return 16000;
            }
            else if ( sample_rate == 0x80 )
            {
                return 22050;
            }
            else if ( sample_rate == 0x40 )
            {
                return 24000;
            }
            else if ( sample_rate == 0x20 )
            {
                return 32000;
            }
            else if ( sample_rate == 0x10 )
            {
                return 44100;
            }
            else if ( sample_rate == 0x8 )
            {
                return 48000;
            }
            else if ( sample_rate == 0x4 )
            {
                return 64000;
            }
            else if ( sample_rate == 0x2 )
            {
                return 88200;
            }
            else if ( sample_rate == 0x1 )
            {
                return 96000;
            }
#if ENABLE_LDAC_CODEC
        case BT_A2DP_NON_A2DP:
            sample_rate = current_config->codec_cap.vend.vendor_codec.cap.ldac_cap.sample_rate;
            BT_DBG_MINOR(BT_DEBUG_A2DP, "sample_rate:%ld",(long)sample_rate);
            //sample_rate = 0x10;
            if ( sample_rate == 0x01 )
            {
                return 192000;
            }
            else if ( sample_rate == 0x02 )
            {
                return 176400;
            }
            else if ( sample_rate == 0x04 )
            {
                return 96000;
            }
            else if ( sample_rate == 0x08 )
            {
                return 88200;
            }
            else if ( sample_rate == 0x10 )
            {
                return 48000;
            }
            else if ( sample_rate == 0x20 )
            {
                return 44100;
            }
            else
            {
                return 48000;
            }
#endif // ENABLE_LDAC_CODEC
        default:
            break;
    }
#endif
    return 0;
}

INT32 linuxbt_a2dp_get_channel_num(VOID)
{
#if 0
    INT32 channel_mode = 0;
    btav_audio_cap_struct *current_config = &local_audio_cap;
    switch ( current_config->codec_type )
    {
        case BT_A2DP_SBC:
            channel_mode = current_config->codec_cap.sbc.channel_mode;
            if ( channel_mode == 0x1 )
            {
                return 2;   //"Joint Stereo"
            }
            else if ( channel_mode == 0x2 )
            {
                return 2;   // Stereo
            }
            else if ( channel_mode == 0x4 )
            {
                return 2;   // Dual Channel
            }
            else if ( channel_mode == 0x8 )
            {
                return 1;   // Mono
            }
            else if ( channel_mode == 0x9 )
            {
                return 0;   // None
            }
            else if ( channel_mode == 0xa )
            {
                return 1;   // Channel 1
            }
            else if ( channel_mode == 0xb )
            {
                return 1;   // Channel 2
            }
            else if ( channel_mode == 0xc )
            {
                return 2;   // Channel 1& 2
            }
        case BT_A2DP_AAC:
            channel_mode = current_config->codec_cap.aac.channels;
            if ( channel_mode == 0x1 )
            {
                return 2;
            }
            else if ( channel_mode == 0x2 )
            {
                return 1;
            }
#if ENABLE_LDAC_CODEC
        case BT_A2DP_NON_A2DP:
            channel_mode = current_config->codec_cap.vend.vendor_codec.cap.ldac_cap.channel_mode;
            if ( channel_mode == 1 )
            {
                return 2;
            }
            else if ( channel_mode == 2 )
            {
                return 2;
            }
            else if ( channel_mode == 4 )
            {
                return 1;
            }
            else
            {
                return 2;
            }
#endif /* ENABLE_LDAC_CODEC */
        default:
            break;
    }
#endif
    return 0;
}

INT32 linuxbt_a2dp_get_prefer_size(VOID)
{
    return 668;
}

INT32 linuxbt_a2dp_get_bitpool(VOID)
{

    return 51;

}

INT32 linuxbt_a2dp_get_allocation_method(VOID)
{
    INT32 allocateion_method = 0;

    if ( allocateion_method == 0x1 )
    {
        return 0;   //LOUDNESS
    }
    else if ( allocateion_method == 0x2 )
    {
        return 1;   //SNR
    }

    return 0;
}

INT32 linuxbt_a2dp_get_block_len(VOID)
{
    INT32 block_length = 0;
    if ( block_length == 0x1 )
    {
        return 16;
    }
    else if ( block_length == 0x2 )
    {
        return 12;
    }
    else if ( block_length == 0x4 )
    {
        return 8;
    }
    else if ( block_length == 0x8 )
    {
        return 4;
    }

    return 0;
}

INT32 linuxbt_a2dp_get_sub_bands(VOID)
{
    INT32 subbands = 0;
    if ( subbands == 0x1 )
    {
        return 8;
    }
    else if ( subbands == 0x2 )
    {
        return 4;
    }

    return 0;
}


INT32 linuxbt_a2dp_get_status( INT32* sample_rate,
                               INT32* channel_num,
                               INT32* prefer_size,
                               INT32* bitpool,
                               INT32* allocation_method,
                               INT32* block_len,
                               INT32* sub_bands)
{
    *sample_rate = linuxbt_a2dp_get_sample_rate();
    *channel_num = linuxbt_a2dp_get_channel_num();
    *prefer_size = linuxbt_a2dp_get_prefer_size();
    *bitpool = linuxbt_a2dp_get_bitpool();
    *allocation_method = linuxbt_a2dp_get_allocation_method();
    *block_len = linuxbt_a2dp_get_block_len();
    *sub_bands = linuxbt_a2dp_get_sub_bands();
    return BT_SUCCESS;
}

INT32 linuxbt_a2dp_set_audio_hw_log_lvl(UINT8 log_level)
{
    BT_DBG_NORMAL(BT_DEBUG_A2DP, "[A2DP] %s(),level:%d", __FUNCTION__, log_level);
    return audio_set_trace_level(log_level);
}


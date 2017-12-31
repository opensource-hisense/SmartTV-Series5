
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<pthread.h>

#include "bt_mw_common.h"
#include "bluetooth_sync.h"

BT_SYNC_RST bt_sync_result = {0};

pthread_rwlock_t sync_lock = PTHREAD_RWLOCK_INITIALIZER;

INT32 i4bluetooth_sync_var_init(VOID)
{
    INT32 i4_ret;
    memset(&bt_sync_result, 0, sizeof(BT_SYNC_RST));

    i4_ret = pthread_rwlock_init(&sync_lock, NULL);
    if(i4_ret != 0)
    {
        BT_DBG_ERROR(BT_DEBUG_COMM, "fail to init rwlock:%ld", (long)i4_ret);
        return BT_ERR_STATUS_FAIL;
    }
    return BT_SUCCESS;
}

EXPORT_SYMBOL VOID set_bt_mode(BT_MODE_TYPE ps_mode)
{
    switch (ps_mode)
    {
    case BT_MODE_CLOSE:
    {
        BT_DBG_NOTICE(BT_DEBUG_COMM, "change to CLOSE mode");
        break;
    }
    case BT_MODE_PAIRING:
    {
        BT_DBG_NOTICE(BT_DEBUG_COMM, "change to PAIRING mode");
        break;
    }
    case BT_MODE_CONNECTING:
    {
        BT_DBG_NOTICE(BT_DEBUG_COMM, "change to CONNECTING mode");
        break;
    }
    case BT_MODE_PLAYING:
    {
        BT_DBG_NOTICE(BT_DEBUG_COMM, "change to PLAYING mode");
        break;
    }
    case BT_MODE_OFF:
    {
        BT_DBG_NOTICE(BT_DEBUG_COMM, "change to OFF mode");
        break;
    }
    default:
        BT_DBG_ERROR(BT_DEBUG_COMM, "not support this mode:%d", ps_mode);
        return;
    }
    pthread_rwlock_wrlock(&sync_lock);
    bt_sync_result.bt_mode = ps_mode;
    pthread_rwlock_unlock(&sync_lock);

}

BT_MODE_TYPE get_bt_mode(VOID)
{
    BT_MODE_TYPE bt_mode;

    pthread_rwlock_rdlock(&sync_lock);
    bt_mode = bt_sync_result.bt_mode;
    pthread_rwlock_unlock(&sync_lock);

    return bt_mode;
}

VOID set_bt_power_on(BOOL fg_connect)
{
    pthread_rwlock_wrlock(&sync_lock);
    bt_sync_result.fg_bt_power_on = fg_connect;
    pthread_rwlock_unlock(&sync_lock);
}

BOOL get_bt_power_on(VOID)
{
    BOOL fg_power_on;

    pthread_rwlock_rdlock(&sync_lock);
    fg_power_on = bt_sync_result.fg_bt_power_on;
    pthread_rwlock_unlock(&sync_lock);

    return fg_power_on;
}

VOID set_bt_is_call_power_off(BOOL fg_done)
{
    pthread_rwlock_wrlock(&sync_lock);
    bt_sync_result.fg_bt_is_call_power_off = fg_done;
    pthread_rwlock_unlock(&sync_lock);
}

BOOL get_bt_is_call_power_off(VOID)
{
    BOOL fg_is_call_power_on;

    pthread_rwlock_rdlock(&sync_lock);
    fg_is_call_power_on = bt_sync_result.fg_bt_is_call_power_off;
    pthread_rwlock_unlock(&sync_lock);

    return fg_is_call_power_on;
}

VOID set_bt_stream_closed(BOOL fg_connect)
{
    pthread_rwlock_wrlock(&sync_lock);
    bt_sync_result.fg_bt_stream_closed = fg_connect;
    pthread_rwlock_unlock(&sync_lock);
}

BOOL get_bt_stream_closed(VOID)
{
    BOOL fg_stream_closed;

    pthread_rwlock_rdlock(&sync_lock);
    fg_stream_closed = bt_sync_result.fg_bt_stream_closed;
    pthread_rwlock_unlock(&sync_lock);

    return fg_stream_closed;
}

VOID set_bt_set_scanmode_done(BOOL fg_done)
{
    pthread_rwlock_wrlock(&sync_lock);
    bt_sync_result.fg_bt_set_scanmode_done = fg_done;
    pthread_rwlock_unlock(&sync_lock);
}

BOOL get_bt_set_scanmode_done(VOID)
{
    BOOL fg_set_scanmode_done;

    pthread_rwlock_rdlock(&sync_lock);
    fg_set_scanmode_done = bt_sync_result.fg_bt_set_scanmode_done;
    pthread_rwlock_unlock(&sync_lock);

    return fg_set_scanmode_done;
}

VOID set_bt_get_dev_info_done(BOOL fg_done)
{
    pthread_rwlock_wrlock(&sync_lock);
    bt_sync_result.fg_bt_get_dev_info_done = fg_done;
    pthread_rwlock_unlock(&sync_lock);
}

BOOL get_bt_get_dev_info_done(VOID)
{
    BOOL fg_get_dev_info_done;

    pthread_rwlock_rdlock(&sync_lock);
    fg_get_dev_info_done = bt_sync_result.fg_bt_get_dev_info_done;
    pthread_rwlock_unlock(&sync_lock);

    return fg_get_dev_info_done;
}

VOID set_bt_avrcp_connect(BOOL fg_done)
{
    pthread_rwlock_wrlock(&sync_lock);
    bt_sync_result.fg_bt_avrcp_connect = fg_done;
    pthread_rwlock_unlock(&sync_lock);
}

BOOL get_bt_avrcp_connect(VOID)
{
    BOOL fg_avrcp_connect;

    pthread_rwlock_rdlock(&sync_lock);
    fg_avrcp_connect = bt_sync_result.fg_bt_avrcp_connect;
    pthread_rwlock_unlock(&sync_lock);

    return fg_avrcp_connect;
}

VOID set_bt_a2dp_connect(BOOL fg_done)
{
    pthread_rwlock_wrlock(&sync_lock);
    bt_sync_result.fg_bt_a2dp_connect = fg_done;
    pthread_rwlock_unlock(&sync_lock);
}

BOOL get_bt_a2dp_connect(VOID)
{
    BOOL fg_a2dp_connect;

    pthread_rwlock_rdlock(&sync_lock);
    fg_a2dp_connect = bt_sync_result.fg_bt_a2dp_connect;
    pthread_rwlock_unlock(&sync_lock);

    return fg_a2dp_connect;
}

VOID set_bt_a2dp_disconnecting(BOOL fg_done)
{
    pthread_rwlock_wrlock(&sync_lock);
    bt_sync_result.fg_bt_a2dp_disconnecting = fg_done;
    pthread_rwlock_unlock(&sync_lock);
}

BOOL get_bt_a2dp_disconnecting(VOID)
{
    BOOL fg_a2dp_disconnecting;

    pthread_rwlock_rdlock(&sync_lock);
    fg_a2dp_disconnecting = bt_sync_result.fg_bt_a2dp_disconnecting;
    pthread_rwlock_unlock(&sync_lock);

    return fg_a2dp_disconnecting;
}

VOID set_bt_connect(BOOL fg_done)
{
    pthread_rwlock_wrlock(&sync_lock);
    bt_sync_result.fg_bt_connect = fg_done;
    pthread_rwlock_unlock(&sync_lock);
}

BOOL get_bt_connect(VOID)
{
    BOOL fg_bt_connect;

    pthread_rwlock_rdlock(&sync_lock);
    fg_bt_connect = bt_sync_result.fg_bt_connect;
    pthread_rwlock_unlock(&sync_lock);

    return fg_bt_connect;
}

/*after receive MSG: MSG_ID_BT_SECOND_STREAM_IND -->2nd connection start*/
/*after receive MSG: MSG_ID_BT_AVRCP_CONNECT_CNF -->2nd connection end*/
VOID set_bt_2nd_connect_comming(BOOL fg_done)
{
    pthread_rwlock_wrlock(&sync_lock);
    bt_sync_result.fg_bt_2nd_connect_comming = fg_done;
    pthread_rwlock_unlock(&sync_lock);
}

BOOL get_bt_2nd_connect_comming(VOID)
{
    BOOL fg_2nd_connect_comming;

    pthread_rwlock_rdlock(&sync_lock);
    fg_2nd_connect_comming = bt_sync_result.fg_bt_2nd_connect_comming;
    pthread_rwlock_unlock(&sync_lock);

    return fg_2nd_connect_comming;
}

VOID set_bt_avrcp_support(BOOL fg_done)
{
    pthread_rwlock_wrlock(&sync_lock);
    bt_sync_result.fg_bt_avrcp_support = fg_done;
    pthread_rwlock_unlock(&sync_lock);
}

BOOL get_bt_avrcp_support(VOID)
{
    BOOL fg_avrcp_support;

    pthread_rwlock_rdlock(&sync_lock);
    fg_avrcp_support = bt_sync_result.fg_bt_avrcp_support;
    pthread_rwlock_unlock(&sync_lock);

    return fg_avrcp_support;
}

VOID set_bt_avrcp_absolute_volume_support(BOOL fg_done)
{
    BT_DBG_NOTICE(BT_DEBUG_COMM, "absolute volume %s", fg_done?"true":"false");
    pthread_rwlock_wrlock(&sync_lock);
    bt_sync_result.fg_bt_avrcp_absolute_volume_support = fg_done;
    pthread_rwlock_unlock(&sync_lock);
}

BOOL get_bt_avrcp_absolute_volume_support(VOID)
{
    BOOL fg_absolute_volume_support;

    pthread_rwlock_rdlock(&sync_lock);
    fg_absolute_volume_support = bt_sync_result.fg_bt_avrcp_absolute_volume_support;
    pthread_rwlock_unlock(&sync_lock);
    BT_DBG_NORMAL(BT_DEBUG_COMM, "absolute volume %s", fg_absolute_volume_support?"true":"false");

    return fg_absolute_volume_support;
}

VOID set_bt_a2dp_support(BOOL fg_done)
{
    pthread_rwlock_wrlock(&sync_lock);
    bt_sync_result.fg_bt_a2dp_support = fg_done;
    pthread_rwlock_unlock(&sync_lock);
}

BOOL get_bt_a2dp_support(VOID)
{
    BOOL fg_a2dp_support;

    pthread_rwlock_rdlock(&sync_lock);
    fg_a2dp_support = bt_sync_result.fg_bt_a2dp_support;
    pthread_rwlock_unlock(&sync_lock);

    return fg_a2dp_support;
}

VOID set_bt_stream_suspend(BOOL fg_done)
{
    pthread_rwlock_wrlock(&sync_lock);
    bt_sync_result.fg_bt_stream_suspend = fg_done;
    pthread_rwlock_unlock(&sync_lock);
}

BOOL get_bt_stream_suspend(VOID)
{
    BOOL fg_a2dp_stream_suspend;

    pthread_rwlock_rdlock(&sync_lock);
    fg_a2dp_stream_suspend = bt_sync_result.fg_bt_stream_suspend;
    pthread_rwlock_unlock(&sync_lock);

    return fg_a2dp_stream_suspend;
}

VOID set_bt_get_rssi_done(BOOL fg_done)
{
    pthread_rwlock_wrlock(&sync_lock);
    bt_sync_result.fg_bt_get_rssi_done = fg_done;
    pthread_rwlock_unlock(&sync_lock);
}

BOOL get_bt_get_rssi_done(VOID)
{
    BOOL fg_get_rssi_done;

    pthread_rwlock_rdlock(&sync_lock);
    fg_get_rssi_done = bt_sync_result.fg_bt_get_rssi_done;
    pthread_rwlock_unlock(&sync_lock);

    return fg_get_rssi_done;
}

VOID set_bt_pts_enable(BOOL fg_done)
{
    BT_DBG_NORMAL(BT_DEBUG_COMM, "set pts %s", fg_done?"enable":"disable");
    pthread_rwlock_wrlock(&sync_lock);
    bt_sync_result.fg_bt_pts_test = fg_done;
    pthread_rwlock_unlock(&sync_lock);
}

BOOL get_bt_pts_enable(VOID)
{
    BOOL fg_pts_enable;

    pthread_rwlock_rdlock(&sync_lock);
    fg_pts_enable = bt_sync_result.fg_bt_pts_test;
    pthread_rwlock_unlock(&sync_lock);
    BT_DBG_NORMAL(BT_DEBUG_COMM, "pts %s", fg_pts_enable?"enable":"disable");

    return fg_pts_enable;
}

VOID set_bt_spp_connect(BOOL fg_done)
{
    pthread_rwlock_wrlock(&sync_lock);
    bt_sync_result.fg_bt_spp_connect = fg_done;
    pthread_rwlock_unlock(&sync_lock);
}

BOOL get_bt_spp_connect(VOID)
{
    BOOL fg_spp_connect;

    pthread_rwlock_rdlock(&sync_lock);
    fg_spp_connect = bt_sync_result.fg_bt_spp_connect;
    pthread_rwlock_unlock(&sync_lock);

    return fg_spp_connect;
}

VOID set_bt_a2dp_be_sink(BOOL fg_sink)
{
    BT_DBG_NORMAL(BT_DEBUG_COMM, "set I'm %s ", fg_sink?"SINK":"SRC");
    pthread_rwlock_wrlock(&sync_lock);
    bt_sync_result.fg_bt_a2dp_src = !fg_sink;
    pthread_rwlock_unlock(&sync_lock);
}

BOOL get_bt_a2dp_be_sink(VOID)
{
    BOOL fg_a2dp_be_sink;

    pthread_rwlock_rdlock(&sync_lock);
    fg_a2dp_be_sink = !bt_sync_result.fg_bt_a2dp_src;
    pthread_rwlock_unlock(&sync_lock);

    return fg_a2dp_be_sink;
}

VOID set_bt_a2dp_rx_mode(BOOL fg_rx)
{
    BT_DBG_NORMAL(BT_DEBUG_COMM, "I'm %s mode", fg_rx?"RX":"TX");
    pthread_rwlock_wrlock(&sync_lock);
    bt_sync_result.fg_bt_a2dp_RX_mode = fg_rx;
    pthread_rwlock_unlock(&sync_lock);
}

BOOL get_bt_a2dp_rx_mode(VOID)
{
    BOOL fg_a2dp_rx_mode;

    pthread_rwlock_rdlock(&sync_lock);
    fg_a2dp_rx_mode = bt_sync_result.fg_bt_a2dp_RX_mode;
    pthread_rwlock_unlock(&sync_lock);

    return fg_a2dp_rx_mode;
}

VOID set_bt_a2dp_pause(BOOL fg_done)
{
    pthread_rwlock_wrlock(&sync_lock);
    bt_sync_result.fg_bt_a2dp_pause = fg_done;
    pthread_rwlock_unlock(&sync_lock);
}

BOOL get_bt_a2dp_pause(VOID)
{
    BOOL fg_a2dp_pause;

    pthread_rwlock_rdlock(&sync_lock);
    fg_a2dp_pause = bt_sync_result.fg_bt_a2dp_pause;
    pthread_rwlock_unlock(&sync_lock);

    return fg_a2dp_pause;
}

VOID set_bt_a2dp_pause_done(BOOL fg_done)
{
    pthread_rwlock_wrlock(&sync_lock);
    bt_sync_result.fg_bt_a2dp_pause_done = fg_done;
    pthread_rwlock_unlock(&sync_lock);
}

BOOL get_bt_a2dp_pause_done(VOID)
{
    BOOL fg_a2dp_pause_done;

    pthread_rwlock_rdlock(&sync_lock);
    fg_a2dp_pause_done = bt_sync_result.fg_bt_a2dp_pause_done;
    pthread_rwlock_unlock(&sync_lock);

    return fg_a2dp_pause_done;
}

VOID set_bt_keep_connection(BOOL fg_done)
{
    pthread_rwlock_wrlock(&sync_lock);
    bt_sync_result.fg_bt_keep_connection = fg_done;
    pthread_rwlock_unlock(&sync_lock);
}

BOOL get_bt_keep_connection(VOID)
{
    BOOL fg_keep_connection;

    pthread_rwlock_rdlock(&sync_lock);
    fg_keep_connection = bt_sync_result.fg_bt_keep_connection;
    pthread_rwlock_unlock(&sync_lock);

    return fg_keep_connection;
}

/*For keep A2DP connection and discard audio steaming data*/
VOID set_bt_discard_data(BOOL fg_done)
{
    pthread_rwlock_wrlock(&sync_lock);
    bt_sync_result.fg_bt_discard_data = fg_done;
    pthread_rwlock_unlock(&sync_lock);
}

BOOL get_bt_discard_data(VOID)
{
    BOOL fg_discard_data;

    pthread_rwlock_rdlock(&sync_lock);
    fg_discard_data = bt_sync_result.fg_bt_discard_data;
    pthread_rwlock_unlock(&sync_lock);

    return fg_discard_data;
}

VOID set_bt_enabled_codec_type(BT_AUDIO_CODEC_TYPE codec_type, BOOL fg_enable)
{
    UINT8 enabled_codec_type = 0;
    if (fg_enable)
    {
        switch (codec_type)
        {
        case BT_AUDIO_SBC:
            enabled_codec_type |= 0x1;
            break;
        case BT_AUDIO_AAC:
            enabled_codec_type |= 0x1<<1;
            break;
        case BT_AUDIO_LDAC:
            enabled_codec_type |= 0x1<<2;
            break;
        default:
            BT_DBG_ERROR(BT_DEBUG_COMM, "invalid codec type: %d", codec_type);
            return;
        }
    }
    else
    {
        switch (codec_type)
        {
        case BT_AUDIO_SBC:
            enabled_codec_type &= ~0x1;
            break;
        case BT_AUDIO_AAC:
            enabled_codec_type &= ~(0x1<<1);
            break;
        case BT_AUDIO_LDAC:
            enabled_codec_type &= ~(0x1<<2);
            break;
        default:
            BT_DBG_ERROR(BT_DEBUG_COMM, "invalid codec type: %d", codec_type);
            return;
        }
    }
    pthread_rwlock_wrlock(&sync_lock);
    bt_sync_result.enabled_codec_type = enabled_codec_type;
    pthread_rwlock_unlock(&sync_lock);
    BT_DBG_NORMAL(BT_DEBUG_COMM, "enabled codec type: %d", enabled_codec_type);
}

UINT8 get_bt_enabled_codec_type(VOID)
{
    UINT8 fg_enabled_codec_type;

    pthread_rwlock_rdlock(&sync_lock);
    fg_enabled_codec_type = bt_sync_result.enabled_codec_type;
    pthread_rwlock_unlock(&sync_lock);

    return fg_enabled_codec_type;
}

VOID set_bt_enabled_a2dp_role(BOOL fg_src, BOOL fg_enable)
{
    UINT8 a2dp_enabled_role = 0;
    pthread_rwlock_wrlock(&sync_lock);
    a2dp_enabled_role = bt_sync_result.a2dp_enabled_role;
    pthread_rwlock_unlock(&sync_lock);
    if (fg_src)
    {
        if (fg_enable)
        {
            a2dp_enabled_role |= 0x1<<1;
        }
        else
        {
            a2dp_enabled_role &= ~(0x1<<1);
        }
    }
    else
    {
        if (fg_enable)
        {
            a2dp_enabled_role |= 0x1;
        }
        else
        {
            a2dp_enabled_role &= ~0x1;
        }
    }
    pthread_rwlock_wrlock(&sync_lock);
    bt_sync_result.a2dp_enabled_role = a2dp_enabled_role;
    pthread_rwlock_unlock(&sync_lock);
    BT_DBG_NORMAL(BT_DEBUG_A2DP,"enabled a2dp role: %s(%x)",
        a2dp_enabled_role==1?"SINK":a2dp_enabled_role==2?"SRC":a2dp_enabled_role==3?"SINK&SRC":"Unknow",
        a2dp_enabled_role);
}

UINT8 get_bt_enabled_a2dp_role(VOID)
{
    UINT8 fg_enabled_a2dp_role;

    pthread_rwlock_rdlock(&sync_lock);
    fg_enabled_a2dp_role = bt_sync_result.a2dp_enabled_role;
    pthread_rwlock_unlock(&sync_lock);

    return fg_enabled_a2dp_role;
}

VOID set_bt_support_scms_t(BOOL fg_done)
{
    BT_DBG_NORMAL(BT_DEBUG_COMM, "set SCMS-T: %s", fg_done?"YES":"NO");
    pthread_rwlock_wrlock(&sync_lock);
    bt_sync_result.fg_bt_target_support_scms_t = fg_done;
    pthread_rwlock_unlock(&sync_lock);
}

BOOL get_bt_support_scms_t(VOID)
{
    BOOL fg_support_scms_t;

    pthread_rwlock_rdlock(&sync_lock);
    fg_support_scms_t = bt_sync_result.fg_bt_target_support_scms_t;
    pthread_rwlock_unlock(&sync_lock);
    BT_DBG_NOTICE(BT_DEBUG_COMM, "SCMS-T: %s", fg_support_scms_t?"YES":"NO");

    return fg_support_scms_t;
}

VOID set_bt_is_mute(BOOL fg_done)
{
    BT_DBG_NORMAL(BT_DEBUG_COMM, "mute: %s", fg_done?"YES":"NO");
    pthread_rwlock_wrlock(&sync_lock);
    bt_sync_result.fg_bt_is_mute = fg_done;
    pthread_rwlock_unlock(&sync_lock);
}

BOOL get_bt_is_mute(VOID)
{
    BOOL fg_bt_is_mute;

    pthread_rwlock_rdlock(&sync_lock);
    fg_bt_is_mute = bt_sync_result.fg_bt_is_mute;
    pthread_rwlock_unlock(&sync_lock);
    BT_DBG_NOTICE(BT_DEBUG_COMM, "fg_bt_is_mute: %s", fg_bt_is_mute?"YES":"NO");

    return fg_bt_is_mute;
}

VOID set_bt_send_zero_data(BOOL fg_done)
{
    BT_DBG_NORMAL(BT_DEBUG_COMM, "should send zero data: %s", fg_done?"YES":"NO");
    pthread_rwlock_wrlock(&sync_lock);
    bt_sync_result.fg_bt_send_zero_data = fg_done;
    pthread_rwlock_unlock(&sync_lock);
}

BOOL get_bt_send_zero_data(VOID)
{
    BOOL fg_bt_send_zero_data;

    pthread_rwlock_rdlock(&sync_lock);
    fg_bt_send_zero_data = bt_sync_result.fg_bt_send_zero_data;
    pthread_rwlock_unlock(&sync_lock);

    return fg_bt_send_zero_data;
}

VOID set_bt_hid_support(BOOL fg_done)
{
    pthread_rwlock_wrlock(&sync_lock);
    bt_sync_result.fg_bt_hid_support = fg_done;
    pthread_rwlock_unlock(&sync_lock);
}

BOOL get_bt_hid_support(VOID)
{
    BOOL fg_bt_hid_support;

    pthread_rwlock_rdlock(&sync_lock);
    fg_bt_hid_support = bt_sync_result.fg_bt_hid_support;
    pthread_rwlock_unlock(&sync_lock);

    return fg_bt_hid_support;
}


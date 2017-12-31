
#include "bluetooth.h"
#include "u_bt_mw_avrcp.h"
#if defined(MTK_LINUX_AVRCP_PLUS) && (MTK_LINUX_AVRCP_PLUS == TRUE)
#include "mtk_bt_rc.h"
#else
#include "bt_rc.h"
#endif
#include <unistd.h>
#include "bt_mw_common.h"
#include "linuxbt_common.h"
#include "linuxbt_avrcp_if.h"
#if ENABLE_MTK_TV_PLATFORM
#include "inc/x_dbg.h"
#include "inc/x_os.h"
#include "x_ipcd.h"
#endif
#include "bluetooth_sync.h"
#include "bt_mw_avrcp.h"
#include "mtk_bluetooth.h"


#define AVRCP_STATE_PRESSED 0
#define AVRCP_STATE_RELEASED 1

#define AVRCP_POP_POWER             0x40
#define AVRCP_POP_VOLUME_UP         0x41
#define AVRCP_POP_VOLUME_DOWN       0x42
#define AVRCP_POP_MUTE              0x43
#define AVRCP_POP_PLAY              0x44
#define AVRCP_POP_STOP              0x45
#define AVRCP_POP_PAUSE             0x46
#define AVRCP_POP_RECORD            0x47
#define AVRCP_POP_REWIND            0x48
#define AVRCP_POP_FAST_FORWARD      0x49
#define AVRCP_POP_EJECT             0x4A
#define AVRCP_POP_FORWARD           0x4B
#define AVRCP_POP_BACKWARD          0x4C

#define AVRCP_CHARSET_UTF8          0x006A

#if defined(MTK_LINUX_AVRCP_PLUS) && (MTK_LINUX_AVRCP_PLUS == TRUE)
#define AVRC_ID_VOL_UP      0x41    /* volume up */
#define AVRC_ID_VOL_DOWN    0x42    /* volume down */
extern BtAppAvrcpCmdCbk AppAvrcpcmdCbk;
extern BtAppAvrcpVolumeChangeCbk AppVolumeCbk;
#endif

extern void *linuxbt_gap_get_profile_interface(const char *profile_id);

static bt_player_status_values_t g_player_status;
static avrcp_medioInfo_t g_media_info;

static bt_bdaddr_t g_avrcp_btaddr;
static uint8_t g_avrcp_abs_vol_label;
static btrc_ctrl_interface_t *g_bt_rc_ct_interface = NULL;
#if defined(MTK_LINUX_AVRCP_PLUS) && (MTK_LINUX_AVRCP_PLUS == TRUE)
static const btrc_ctrl_ex_interface_t *g_bt_avrcp_ex_interface = NULL;
#endif
void linuxbt_rc_passthrough_cmd_cb(int id, int key_state);
void linuxbt_rc_connection_state_cb (bool state, bt_bdaddr_t *bd_addr);
void linuxbt_rc_set_volume_cb (uint8_t volume);
void linuxbt_rc_playback_status_cb(uint32_t song_len, uint32_t song_pos , uint8_t play_status);
void linuxbt_rc_get_element_attr_cb (uint8_t num_attr, btrc_element_attr_val_t* attrs);
//void linuxbt_rc_get_capabilities_cb (uint8_t capid, uint8_t count, btrc_get_capabilities_t *getcap);
void linuxbt_rc_connect_browse_cb(uint8_t connectid, uint16_t res, uint16_t mtu);
void linuxbt_rc_disconnect_browse_cb (uint8_t channel_num);
void linuxbt_rc_set_addressedplayer_cb (uint8_t channel_num, uint8_t status);
//void linuxbt_rc_set_browsedplayer_cb (uint8_t status, btrc_set_browsed_player_t *browsed_folder);
void linuxbt_rc_get_folderitems_cb (uint8_t status, uint16_t counter, uint16_t uid_counter, uint16_t data_len, uint8_t* data);
void linuxbt_rc_change_path_cb (uint8_t status, uint8_t chnl_num, uint32_t num_of_items);
void linuxbt_rc_get_item_attributes_cb (uint8_t status, uint8_t chnl_num, uint8_t counter, uint32_t *attr_id, uint16_t *charset, uint16_t *attr_val_len,  uint8_t **attr_val_string);
void linuxbt_rc_search_cb (uint8_t status, uint8_t chnl_num, uint16_t uid_counter, uint32_t found_num_of_item);
void linuxbt_rc_passthrough_frame_cb (uint8_t *frame_data, int len);
void linuxbt_rc_ctrl_groupnavigation_rsp_cb(int id, int key_state);
void linuxbt_rc_ctrl_setabsvol_cmd_cb(bt_bdaddr_t *bd_addr, uint8_t abs_vol, uint8_t label);
void linuxbt_rc_ctrl_registernotification_abs_vol_cb (bt_bdaddr_t *bd_addr, uint8_t label);
void linuxbt_rc_ctrl_play_position_changed_cb(bt_bdaddr_t *bd_addr, uint32_t song_len, uint32_t song_pos);
void linuxbt_rc_ctrl_play_status_changed_cb(bt_bdaddr_t *bd_addr, btrc_play_status_t play_status);
#if defined(MTK_LINUX_AVRCP_PLUS) && (MTK_LINUX_AVRCP_PLUS == TRUE)
void linuxbt_rc_tg_passthrough_volume_cmd_cb (int id, int key_state);
#endif
void linuxbt_rc_ctrl_remote_features_cb(bt_bdaddr_t *bd_addr, int features);
void linuxbt_rc_ctrl_setplayerapplicationsetting_rsp_cb(bt_bdaddr_t *bd_addr, uint8_t accepted);
void linuxbt_rc_ctrl_playerapplicationsetting_cb(bt_bdaddr_t *bd_addr,
                                                         uint8_t num_attr,
                                                         btrc_player_app_attr_t *app_attrs,
                                                         uint8_t num_ext_attr,
                                                         btrc_player_app_ext_attr_t *ext_attrs);

void linuxbt_rc_ctrl_playerapplicationsetting_changed_cb(bt_bdaddr_t *bd_addr, btrc_player_settings_t *p_vals);
void linuxbt_rc_ctrl_track_changed_cb(bt_bdaddr_t *bd_addr, uint8_t num_attr, btrc_element_attr_val_t *p_attrs);

static btrc_ctrl_callbacks_t g_bt_rc_callbacks =
{
    sizeof(btrc_ctrl_callbacks_t),
    linuxbt_rc_passthrough_cmd_cb,
    linuxbt_rc_ctrl_groupnavigation_rsp_cb, //btrc_groupnavigation_rsp_callback
    linuxbt_rc_connection_state_cb,
    linuxbt_rc_ctrl_remote_features_cb, //btrc_ctrl_getrcfeatures_callback
    linuxbt_rc_ctrl_setplayerapplicationsetting_rsp_cb, //btrc_ctrl_setplayerapplicationsetting_rsp_callback
    linuxbt_rc_ctrl_playerapplicationsetting_cb, //btrc_ctrl_playerapplicationsetting_callback
    linuxbt_rc_ctrl_playerapplicationsetting_changed_cb, //btrc_ctrl_playerapplicationsetting_changed_callback
    linuxbt_rc_ctrl_setabsvol_cmd_cb, //btrc_ctrl_setabsvol_cmd_callback
    linuxbt_rc_ctrl_registernotification_abs_vol_cb, //btrc_ctrl_registernotification_abs_vol_callback
    linuxbt_rc_ctrl_track_changed_cb, //btrc_ctrl_track_changed_callback
    linuxbt_rc_ctrl_play_position_changed_cb, //btrc_ctrl_play_position_changed_callback
    linuxbt_rc_ctrl_play_status_changed_cb, //btrc_ctrl_play_status_changed_callback
};

#if defined(MTK_LINUX_AVRCP_PLUS) && (MTK_LINUX_AVRCP_PLUS == TRUE)
static btrc_ctrl_ext_callbacks_t g_bt_rc_ext_callbacks =
{
    sizeof(btrc_ctrl_ext_callbacks_t),
    linuxbt_rc_tg_passthrough_volume_cmd_cb,// local device (A2DP SNK) works as AVRCP TG, remote device (A2DP SRC ,AVRCP CT) send VOLUME UP/DOWN passthrough cmd
};
#endif

void linuxbt_get_player_status(bt_player_status_values_t *pt_player_status)
{
    memcpy(pt_player_status, &g_player_status, sizeof(bt_player_status_values_t));
}

void linuxbt_get_media_info(avrcp_medioInfo_t *pt_media_info)
{
    memset(pt_media_info, 0, sizeof(avrcp_medioInfo_t));
    memcpy(pt_media_info, &g_media_info, sizeof(avrcp_medioInfo_t));

    BT_DBG_NORMAL(BT_DEBUG_AVRCP, "Title  : %s", pt_media_info->title);
    BT_DBG_NORMAL(BT_DEBUG_AVRCP, "Artist : %s", pt_media_info->artist);
    BT_DBG_NORMAL(BT_DEBUG_AVRCP, "Album  : %s", pt_media_info->album);
    BT_DBG_NORMAL(BT_DEBUG_AVRCP, "genre  : %s", pt_media_info->genre);
    BT_DBG_NORMAL(BT_DEBUG_AVRCP, "current_track_number  : %ld", (long)pt_media_info->current_track_number);
    BT_DBG_NORMAL(BT_DEBUG_AVRCP, "number_of_tracks  : %ld", (long)pt_media_info->number_of_tracks);
    BT_DBG_NORMAL(BT_DEBUG_AVRCP, "position  : %ld", (long)pt_media_info->position);
    //BT_DBG_NORMAL(BT_DEBUG_AVRCP, "track_duration  : %ld", pt_media_info->track_duration);
    /*clear media information*/
    //memset(&g_media_info, 0, sizeof(avrcp_medioInfo_t));

}

void linuxbt_rc_passthrough_frame_cb (uint8_t *frame_data, int len)
{
    BT_DBG_NOTICE(BT_DEBUG_AVRCP,  " passthrough_frame ");
    int i = 0;
    for (i = 0 ; i < len ; i++) {
        BT_DBG_NOTICE(BT_DEBUG_AVRCP, "frame[%d] = %d",i,frame_data[i]);
    }
}

void linuxbt_rc_passthrough_cmd_cb(int id, int key_state)
{
    BT_DBG_NORMAL(BT_DEBUG_AVRCP, "%s() id = %x, key state = %x ", __FUNCTION__, id, key_state);
}

void linuxbt_rc_connection_state_cb(bool state, bt_bdaddr_t *bd_addr)
{
    char bt_address[18];

    if (NULL == bd_addr)
    {
        BT_DBG_ERROR(BT_DEBUG_AVRCP, "null pointer");
        return;
    }
    BT_DBG_NOTICE(BT_DEBUG_AVRCP, "state = %x bd_addr = %2x:%2x:%2x:%2x:%2x:%2x", state, bd_addr->address[0], bd_addr->address[1], bd_addr->address[2], bd_addr->address[3], bd_addr->address[4], bd_addr->address[5]);
    memcpy(&g_avrcp_btaddr, bd_addr, sizeof(bt_bdaddr_t));// For further AVRCP operations
    linuxbt_btaddr_htos(&g_avrcp_btaddr, bt_address);
    if(FALSE == state)
    {
        /*avrcp disconnect*/
        if(get_bt_avrcp_connect())
        {
            bluetooth_avrcp_handle_disconnect_cb(bt_address);
        }
        else
        {
            /*avrcp before this point disconnected*/
            BT_DBG_INFO(BT_DEBUG_AVRCP, "avrcp disconnected before this point");
        }
    }
    else if(TRUE == state)
    {
        /*avrcp connect*/
        if(!get_bt_avrcp_connect())
        {
            bluetooth_avrcp_handle_connect_cb(bt_address);
        }
        else
        {
            /*avrcp before this point connected*/
            BT_DBG_INFO(BT_DEBUG_AVRCP, "avrcp connected before this point");
        }
    }
}
void linuxbt_rc_set_volume_cb(uint8_t volume)
{
    BT_DBG_NORMAL(BT_DEBUG_AVRCP, "%s() volume = %x ", __FUNCTION__, volume);
}

int linuxbt_reset_playstatus(void)
{
    memset(&g_player_status, 0, sizeof(g_player_status));
    return 0;
}

void linuxbt_rc_playback_status_cb(uint32_t song_len, uint32_t song_pos , uint8_t play_status)
{
    BT_DBG_NORMAL(BT_DEBUG_AVRCP, " %s() song_len = %x song_pos = %x play_status = %x", __FUNCTION__, song_len, song_pos, play_status);
    g_player_status.song_length = song_len;
    g_player_status.song_position = song_pos;
    g_player_status.play_status = play_status;
}

void linuxbt_rc_ctrl_groupnavigation_rsp_cb(int id, int key_state)
{
    BT_DBG_NORMAL(BT_DEBUG_AVRCP, "id = %d key_state = %d ", id, key_state);
}

void linuxbt_rc_ctrl_remote_features_cb(bt_bdaddr_t *bd_addr, int features)
{
    BT_DBG_NORMAL(BT_DEBUG_AVRCP, "%s() %02X:%02X:%02X:%02X:%02X:%02X",
           __FUNCTION__,
           bd_addr->address[0], bd_addr->address[1], bd_addr->address[2],
           bd_addr->address[3], bd_addr->address[4], bd_addr->address[5]);
    BT_DBG_NORMAL(BT_DEBUG_AVRCP, "[AVRCP] TG features = 0x%x ", features);
}

void linuxbt_rc_ctrl_setplayerapplicationsetting_rsp_cb(bt_bdaddr_t *bd_addr, uint8_t accepted)
{
    BT_DBG_NORMAL(BT_DEBUG_AVRCP, "%s() %02X:%02X:%02X:%02X:%02X:%02X",
           __FUNCTION__,
           bd_addr->address[0], bd_addr->address[1], bd_addr->address[2],
           bd_addr->address[3], bd_addr->address[4], bd_addr->address[5]);
    BT_DBG_NORMAL(BT_DEBUG_AVRCP, "accepted = %d ", accepted);
}

void linuxbt_rc_ctrl_playerapplicationsetting_cb(bt_bdaddr_t *bd_addr,
                                                         uint8_t num_attr,
                                                         btrc_player_app_attr_t *app_attrs,
                                                         uint8_t num_ext_attr,
                                                         btrc_player_app_ext_attr_t *ext_attrs)
{
    BT_DBG_NORMAL(BT_DEBUG_AVRCP, "%02X:%02X:%02X:%02X:%02X:%02X",
           bd_addr->address[0], bd_addr->address[1], bd_addr->address[2],
           bd_addr->address[3], bd_addr->address[4], bd_addr->address[5]);
    BT_DBG_NORMAL(BT_DEBUG_AVRCP, "num_attr = %d ", num_attr);
    BT_DBG_NORMAL(BT_DEBUG_AVRCP, "num_ext_attr = %d ", num_ext_attr);
}

void linuxbt_rc_ctrl_playerapplicationsetting_changed_cb(bt_bdaddr_t *bd_addr, btrc_player_settings_t *p_vals)
{
    BT_DBG_NORMAL(BT_DEBUG_AVRCP, "%02X:%02X:%02X:%02X:%02X:%02X ",
           bd_addr->address[0], bd_addr->address[1], bd_addr->address[2],
           bd_addr->address[3], bd_addr->address[4], bd_addr->address[5]);
    BT_DBG_NORMAL(BT_DEBUG_AVRCP, "num_attr = %d ", p_vals->num_attr);
}


void linuxbt_rc_ctrl_setabsvol_cmd_cb(bt_bdaddr_t *bd_addr, uint8_t abs_vol, uint8_t label) {
    BT_DBG_NORMAL(BT_DEBUG_AVRCP, "%02X:%02X:%02X:%02X:%02X:%02X , label = %x volume = %d",
           bd_addr->address[0], bd_addr->address[1], bd_addr->address[2],
           bd_addr->address[3], bd_addr->address[4], bd_addr->address[5],
           label,abs_vol);
    bluetooth_rc_tg_set_absolute_volume_ind_cb(abs_vol);
    g_bt_rc_ct_interface->set_volume_rsp(bd_addr, abs_vol, label);
}

void linuxbt_rc_ctrl_registernotification_abs_vol_cb (bt_bdaddr_t *bd_addr, uint8_t label)
{
    BT_DBG_NORMAL(BT_DEBUG_AVRCP, "%02X:%02X:%02X:%02X:%02X:%02X , label = %x",
            bd_addr->address[0], bd_addr->address[1], bd_addr->address[2],
            bd_addr->address[3], bd_addr->address[4], bd_addr->address[5],
           label);
    BT_DBG_NOTICE(BT_DEBUG_AVRCP, "support absolute volume");
    g_avrcp_abs_vol_label = label;
    UINT8 u1_volume = 0;
    bluetooth_rc_tg_register_notification_cb(&u1_volume);
    g_bt_rc_ct_interface->register_abs_vol_rsp( bd_addr, BTRC_NOTIFICATION_TYPE_INTERIM, u1_volume, label);
}

void linuxbt_rc_ctrl_track_changed_cb(bt_bdaddr_t *bd_addr, uint8_t num_attr, btrc_element_attr_val_t *p_attrs)
{
    BT_DBG_NORMAL(BT_DEBUG_AVRCP, "%02X:%02X:%02X:%02X:%02X:%02X",
            bd_addr->address[0], bd_addr->address[1], bd_addr->address[2],
            bd_addr->address[3], bd_addr->address[4], bd_addr->address[5]);
    BT_DBG_NORMAL(BT_DEBUG_AVRCP, "num_attr = %d ", num_attr);
    linuxbt_rc_get_element_attr_cb(num_attr, p_attrs);
}


void linuxbt_rc_ctrl_play_position_changed_cb(bt_bdaddr_t *bd_addr, uint32_t song_len, uint32_t song_pos)
{
    BT_DBG_NORMAL(BT_DEBUG_AVRCP, "%02X:%02X:%02X:%02X:%02X:%02X",
            bd_addr->address[0], bd_addr->address[1], bd_addr->address[2],
            bd_addr->address[3], bd_addr->address[4], bd_addr->address[5]);
    BT_DBG_NOTICE(BT_DEBUG_AVRCP, "play position changed, song_len=%d, song_pos=%d", song_len, song_pos);
    g_player_status.song_length = song_len;
    g_player_status.song_position = song_pos;
    bluetooth_avrcp_get_playstatus_cbk(&g_player_status);
}

void linuxbt_rc_ctrl_play_status_changed_cb(bt_bdaddr_t *bd_addr, btrc_play_status_t play_status)
{
    BT_DBG_NORMAL(BT_DEBUG_AVRCP, "%s() %02X:%02X:%02X:%02X:%02X:%02X",
           __FUNCTION__,
            bd_addr->address[0], bd_addr->address[1], bd_addr->address[2],
            bd_addr->address[3], bd_addr->address[4], bd_addr->address[5]);
    BT_DBG_NOTICE(BT_DEBUG_AVRCP, "play status changed, play_status=%d", play_status);
    g_player_status.play_status = play_status;
    bluetooth_avrcp_playstatus_change_cbk(g_player_status.play_status);
}

#if defined(MTK_LINUX_AVRCP_PLUS) && (MTK_LINUX_AVRCP_PLUS == TRUE)
/* just for PTS:AVRCP_TG_PTT_BV_02_I
local role:     A2DP SNK, AVRCP TG
remote role:  A2DP SRC, AVRCP CT
remote send AVRCP VOLUME UP/DOWN passthrough command to local
*/
void linuxbt_rc_tg_passthrough_volume_cmd_cb (int id, int key_state)
{
    BT_DBG_NORMAL(BT_DEBUG_AVRCP, "[AVRCP] %s() id = %x key_state = %x", __FUNCTION__, id, key_state);

    unsigned char cmd_type;
    BTCmdType bt_cmd_type;
    cmd_type = (unsigned char)id;

    BT_DBG_NORMAL(BT_DEBUG_AVRCP, "[AVRCP] %s(), bt_power_on = %d", __FUNCTION__, get_bt_power_on());

    switch (cmd_type)
    {
        case AVRC_ID_VOL_DOWN:
        {
            BT_DBG_NORMAL(BT_DEBUG_AVRCP, "KEY_VOLUMEDOWN");

            bt_cmd_type = BT_VOLUME_TYPE_DOWN;
            if (1 == key_state)
            {
                if (NULL != AppVolumeCbk)
                {
                    AppVolumeCbk(bt_cmd_type);
                }
                else
                {
                    BT_DBG_ERROR(BT_DEBUG_AVRCP, "null pointer AppVolumeCbk");
                }
            }
            return;
        }
        case AVRC_ID_VOL_UP:
        {
            BT_DBG_NORMAL(BT_DEBUG_AVRCP, "KEY_VOLUMEUP");
            bt_cmd_type = BT_VOLUME_TYPE_UP;
            if (1 == key_state)
            {
                if (NULL != AppVolumeCbk)
                {
                    AppVolumeCbk(bt_cmd_type);
                }
                else
                {
                    BT_DBG_ERROR(BT_DEBUG_AVRCP, "null pointer AppVolumeCbk");
                }
            }
            return;
        }
        default:
            break;
    }

    if (!AppAvrcpcmdCbk)
    {
        BT_DBG_ERROR(BT_DEBUG_AVRCP, "[AVRCP] AppAvrcpcmdCbk not init");
        return;
    }
    BT_DBG_NORMAL(BT_DEBUG_AVRCP, "[AVRCP] AppAvrcpcmdCbk");
    AppAvrcpcmdCbk(bt_cmd_type, key_state);
}
#endif

void linuxbt_rc_get_element_attr_cb(uint8_t num_attr, btrc_element_attr_val_t* attrs)
{
    int i;
    UINT8 *ptr_data = NULL;
    BT_DBG_NORMAL(BT_DEBUG_AVRCP, " %s() num_attr = %x ", __FUNCTION__, num_attr);
    if(attrs[0].attr_id == 0xFF)
    {
        BT_DBG_ERROR(BT_DEBUG_AVRCP,  " ERROR %d", attrs[1].attr_id);
        return;
    }

    for ( i = 0 ; i < 7 ; i++ )
    {
        ptr_data = (UINT8 *)&attrs[i].text;

        switch ((int)attrs[i].attr_id)
        {
            case BTRC_MEDIA_ATTR_TITLE://BTRC_MEDIA_ATTR_TITLE = 0x01
            {
                memset(g_media_info.title, 0, sizeof(g_media_info.title));
                strncpy((char *)g_media_info.title, (char *)ptr_data, MAX_NAME_STR_LEN-1);
                BT_DBG_NOTICE(BT_DEBUG_AVRCP, "Attribute_ID %d(title):%s", (int)attrs[i].attr_id, g_media_info.title);
                break;
            }
            case BTRC_MEDIA_ATTR_ARTIST://BTRC_MEDIA_ATTR_ARTIST = 0x02
            {
                memset(g_media_info.artist, 0, sizeof(g_media_info.artist));
                strncpy((char *)g_media_info.artist, (char *)ptr_data, MAX_NAME_STR_LEN-1);
                BT_DBG_NOTICE(BT_DEBUG_AVRCP, "Attribute_ID %d(artist):%s", (int)attrs[i].attr_id, g_media_info.artist);
                break;
            }
            case BTRC_MEDIA_ATTR_ALBUM://BTRC_MEDIA_ATTR_ALBUM = 0x03
            {
                memset(g_media_info.album, 0, sizeof(g_media_info.album));
                strncpy((char *)g_media_info.album, (char *)ptr_data, MAX_NAME_STR_LEN-1);
                BT_DBG_NOTICE(BT_DEBUG_AVRCP, "Attribute_ID %d(album):%s", (int)attrs[i].attr_id, g_media_info.album);
                break;
            }
            case BTRC_MEDIA_ATTR_TRACK_NUM://BTRC_MEDIA_ATTR_TRACK_NUM = 0x04
            {
                BT_DBG_NOTICE(BT_DEBUG_AVRCP, "Attribute_ID %d(current_track_number):%s", (int)attrs[i].attr_id, (char *)ptr_data);
                g_media_info.current_track_number = atol((char *)ptr_data);
                BT_DBG_NOTICE(BT_DEBUG_AVRCP, "current_track_number:%ld", (long)g_media_info.current_track_number);
                break;
            }
            case BTRC_MEDIA_ATTR_NUM_TRACKS://BTRC_MEDIA_ATTR_NUM_TRACKS = 0x05
            {
                BT_DBG_NOTICE(BT_DEBUG_AVRCP, "Attribute_ID %d(total_track_num):%s", (int)attrs[i].attr_id, (char *)ptr_data);
                g_media_info.number_of_tracks = atol((char *)ptr_data);
                BT_DBG_NOTICE(BT_DEBUG_AVRCP, "number_of_tracks:%ld", (long)g_media_info.number_of_tracks);
                break;
            }
            case BTRC_MEDIA_ATTR_GENRE://BTRC_MEDIA_ATTR_GENRE = 0x06
            {
                memset(g_media_info.genre, 0, sizeof(g_media_info.genre));
                strncpy((char *)g_media_info.genre, (char *)ptr_data, MAX_NAME_STR_LEN-1);
                BT_DBG_NOTICE(BT_DEBUG_AVRCP, "Attribute_ID %d(genre):%s", (int)attrs[i].attr_id, g_media_info.genre);
                break;
            }
            case BTRC_MEDIA_ATTR_PLAYING_TIME://BTRC_MEDIA_ATTR_PLAYING_TIME = 0x07
            {
                BT_DBG_NOTICE(BT_DEBUG_AVRCP, "Attribute_ID %d(play_time_in_millisecond):%s", (int)attrs[i].attr_id, (char *)ptr_data);
                g_media_info.position = atol((char *)ptr_data);
                BT_DBG_NOTICE(BT_DEBUG_AVRCP, "position:%ld", (long)g_media_info.position);
                break;
            }
            default:
            {
                BT_DBG_WARNING(BT_DEBUG_AVRCP, "Unknown Attribute_ID : %d", (int)attrs[i].attr_id);
                break;
            }
        }
    }
    bluetooth_avrcp_track_change_cbk(g_media_info.current_track_number);
    bluetooth_avrcp_get_mediaInfo_cbk(&g_media_info);
}

/*void linuxbt_rc_get_capabilities_cb(uint8_t capid, uint8_t count, btrc_get_capabilities_t *getcap)
{
    BT_DBG_NORMAL(BT_DEBUG_AVRCP, " %s() capid = %x count = %x ", __FUNCTION__, capid, count);
}/*/

void linuxbt_rc_connect_browse_cb(uint8_t connectid, uint16_t res, uint16_t mtu)
{
    BT_DBG_NORMAL(BT_DEBUG_AVRCP, " %s() connectid = %x res = %x mtu = %x", __FUNCTION__, connectid, res, mtu);
}

void linuxbt_rc_disconnect_browse_cb(uint8_t channel_num)
{
    BT_DBG_NORMAL(BT_DEBUG_AVRCP, " %s() channel_num = %x ", __FUNCTION__, channel_num);
}

void linuxbt_rc_set_addressedplayer_cb(uint8_t channel_num, uint8_t status)
{
    BT_DBG_NORMAL(BT_DEBUG_AVRCP, " %s() channel_num = %x status = %x ", __FUNCTION__, channel_num, status);
}

/*void linuxbt_rc_set_browsedplayer_cb(uint8_t status, btrc_set_browsed_player_t *browsed_folder)
{
    BT_DBG_NORMAL(BT_DEBUG_AVRCP, " %s() status = %x ", __FUNCTION__, status);
}*/

void linuxbt_rc_get_folderitems_cb(uint8_t status, uint16_t counter, uint16_t uid_counter, uint16_t data_len, uint8_t* data)
{
    BT_DBG_NORMAL(BT_DEBUG_AVRCP, " %s() status = %x counter = %x uid_counter = %x, ", __FUNCTION__, status, counter, uid_counter);
}

void linuxbt_rc_change_path_cb(uint8_t status, uint8_t chnl_num, uint32_t num_of_items)
{
    BT_DBG_NORMAL(BT_DEBUG_AVRCP, " %s() status = %x chnl_num = %x num_of_items = %x", __FUNCTION__, status, chnl_num, num_of_items);
}

void linuxbt_rc_get_item_attributes_cb(uint8_t status, uint8_t chnl_num, uint8_t counter, uint32_t *attr_id, uint16_t *charset, uint16_t *attr_val_len,  uint8_t **attr_val_string)
{
    BT_DBG_NORMAL(BT_DEBUG_AVRCP, " %s() status = %x chnl_num = %x counter = %x", __FUNCTION__, status, chnl_num, counter);
}

void linuxbt_rc_search_cb(uint8_t status, uint8_t chnl_num, uint16_t uid_counter, uint32_t found_num_of_item)
{
    BT_DBG_NORMAL(BT_DEBUG_AVRCP, " %s() status = %x chnl_num = %x  uid_counter = %x found_num_of_item = %x", __FUNCTION__, status, chnl_num, uid_counter, found_num_of_item);
}

int linuxbt_rc_init(void)
{
    int ret = 0;

    g_bt_rc_ct_interface = (btrc_ctrl_interface_t *) linuxbt_gap_get_profile_interface(BT_PROFILE_AV_RC_CTRL_ID);

    if (g_bt_rc_ct_interface == NULL)
    {
        BT_DBG_ERROR(BT_DEBUG_AVRCP,  " Failed to get AVRCP interface");
        return -1;
    }
#if defined(MTK_LINUX_AVRCP_PLUS) && (MTK_LINUX_AVRCP_PLUS == TRUE)
    g_bt_avrcp_ex_interface = (btrc_ctrl_ex_interface_t *) linuxbt_gap_get_profile_interface(BT_PROFILE_AVRCP_EX_ID);
    if (NULL == g_bt_avrcp_ex_interface)
    {
        BT_DBG_WARNING(BT_DEBUG_AVRCP, "Failed to get AVRCP extended interface");
    }
    // Init AVRCP ext interface
    ret = g_bt_avrcp_ex_interface->init(&g_bt_rc_ext_callbacks);
    if (ret == BT_STATUS_SUCCESS)
    {
        BT_DBG_WARNING(BT_DEBUG_AVRCP, " success to init AVRCP CT ext interface");
    }
    else if(ret == BT_STATUS_DONE)
    {
        BT_DBG_WARNING(BT_DEBUG_AVRCP, " already init AVRCP CT ext interface");
    }
#endif

    // Init AVRCP interface
    ret = g_bt_rc_ct_interface->init(&g_bt_rc_callbacks);
    if (ret == BT_STATUS_SUCCESS)
    {
        BT_DBG_NORMAL(BT_DEBUG_AVRCP, " success to init AVRCP CT interface");
        return 0;
    }
    else if(ret == BT_STATUS_DONE)
    {
        BT_DBG_NORMAL(BT_DEBUG_AVRCP, " already init AVRCP CT interface");
        return 0;
    }
    BT_DBG_ERROR(BT_DEBUG_AVRCP,  " Failed to init AVRCP CT interface");

    return ret;
}

//static int linuxbt_rc_disable_handler(int argc, char *argv[])
int linuxbt_rc_deinit(void)
{
    FUNC_ENTRY;

    if (g_bt_rc_ct_interface == NULL)
    {
        BT_DBG_ERROR(BT_DEBUG_AVRCP,  " Failed to get AVRCP interface");
        return -1;
    }

    g_bt_rc_ct_interface->cleanup();

    //btmtk_avrcp_send_deactivate_req(0, 1);
    //btmtk_profile_unregister(PROFILE_AVRCP);

    return 0;

}

int linuxbt_rc_connect_handler(char *pbt_addr)
{
    FUNC_ENTRY;
    bt_bdaddr_t bdaddr;

    if (NULL == pbt_addr)
    {
        BT_DBG_ERROR(BT_DEBUG_AVRCP, "null pointer of pbt_addr");
        return -1;
    }
    if (g_bt_rc_ct_interface == NULL)
    {
        BT_DBG_ERROR(BT_DEBUG_AVRCP,  " Failed to get AVRCP interface");
        return -1;
    }

    linuxbt_btaddr_stoh(pbt_addr, &bdaddr);

    BT_DBG_NORMAL(BT_DEBUG_AVRCP, "AVRCP connect to %02X:%02X:%02X:%02X:%02X:%02X",
           bdaddr.address[0], bdaddr.address[1], bdaddr.address[2],
           bdaddr.address[3], bdaddr.address[4], bdaddr.address[5]);

    memcpy(&g_avrcp_btaddr, &bdaddr, sizeof(bt_bdaddr_t));// For further AVRCP operations
    //linuxbt_reverse_bd_addr(&g_avrcp_btaddr);
#ifdef __CE_MTK_IF_API__
    //btrc_connect_tg(&bdaddr);
    //g_bt_rc_ct_interface->connect_ctrl(&bdaddr);
#endif

    return 0;
}

int linuxbt_rc_disconnect_handler(char *pbt_addr)
{
    FUNC_ENTRY;

    if (NULL == pbt_addr)
    {
        BT_DBG_ERROR(BT_DEBUG_AVRCP, "null pointer of pbt_addr");
        return -1;
    }
    if (g_bt_rc_ct_interface == NULL)
    {
        BT_DBG_ERROR(BT_DEBUG_AVRCP,  " Failed to get AVRCP interface");
        return -1;
    }

#ifdef __CE_MTK_IF_API__
    //btrc_disconnect_tg();
    BT_DBG_NOTICE(BT_DEBUG_AVRCP,  " bd_addr = %2x:%2x:%2x:%2x:%2x:%2x",
        g_avrcp_btaddr.address[0], g_avrcp_btaddr.address[1],
        g_avrcp_btaddr.address[2], g_avrcp_btaddr.address[3],
        g_avrcp_btaddr.address[4], g_avrcp_btaddr.address[5]);
    //g_bt_rc_ct_interface->disconnect_ctrl(&g_avrcp_btaddr);
#endif

    return 0;
}

static int linuxbt_rc_send_play_handler(BTN_ACTION button_action)
{
    FUNC_ENTRY;

    if (g_bt_rc_ct_interface == NULL)
    {
        BT_DBG_ERROR(BT_DEBUG_AVRCP,  " Failed to get AVRCP interface");
        return -1;
    }

    if (BUTTON_ACT_PRESS == button_action)
    {
        g_bt_rc_ct_interface->send_pass_through_cmd(&g_avrcp_btaddr,AVRCP_POP_PLAY, BUTTON_ACT_PRESS);
    }
    else if (BUTTON_ACT_RELEASE == button_action)
    {
        g_bt_rc_ct_interface->send_pass_through_cmd(&g_avrcp_btaddr,AVRCP_POP_PLAY, BUTTON_ACT_RELEASE);
    }
    else
    {
        g_bt_rc_ct_interface->send_pass_through_cmd(&g_avrcp_btaddr,AVRCP_POP_PLAY, BUTTON_ACT_PRESS);
        /*x_thread_delay(200);*/
        usleep(200*1000);
        g_bt_rc_ct_interface->send_pass_through_cmd(&g_avrcp_btaddr,AVRCP_POP_PLAY, BUTTON_ACT_RELEASE);
    }

    g_player_status.play_status = AVRCP_STATUS_PLAYING;

    return 0;
}

static int linuxbt_rc_send_pause_handler(BTN_ACTION button_action)
{
    FUNC_ENTRY;

    if (g_bt_rc_ct_interface == NULL)
    {
        BT_DBG_ERROR(BT_DEBUG_AVRCP,  " Failed to get AVRCP interface");
        return -1;
    }

    if (BUTTON_ACT_PRESS == button_action)
    {
        g_bt_rc_ct_interface->send_pass_through_cmd(&g_avrcp_btaddr,AVRCP_POP_PAUSE, BUTTON_ACT_PRESS);
    }
    else if (BUTTON_ACT_RELEASE == button_action)
    {
        g_bt_rc_ct_interface->send_pass_through_cmd(&g_avrcp_btaddr,AVRCP_POP_PAUSE, BUTTON_ACT_RELEASE);
    }
    else
    {
        g_bt_rc_ct_interface->send_pass_through_cmd(&g_avrcp_btaddr,AVRCP_POP_PAUSE, BUTTON_ACT_PRESS);
        /*x_thread_delay(200);*/
        usleep(200*1000);
        g_bt_rc_ct_interface->send_pass_through_cmd(&g_avrcp_btaddr,AVRCP_POP_PAUSE, BUTTON_ACT_RELEASE);
    }

    g_player_status.play_status = AVRCP_STATUS_PAUSED;

    return 0;
}


static int linuxbt_rc_send_stop_handler(BTN_ACTION button_action)
{
    FUNC_ENTRY;

    if (g_bt_rc_ct_interface == NULL)
    {
        BT_DBG_ERROR(BT_DEBUG_AVRCP,  " Failed to get AVRCP interface");
        return -1;
    }

    if (BUTTON_ACT_PRESS == button_action)
    {
        g_bt_rc_ct_interface->send_pass_through_cmd(&g_avrcp_btaddr,AVRCP_POP_STOP, BUTTON_ACT_PRESS);
    }
    else if (BUTTON_ACT_RELEASE == button_action)
    {
        g_bt_rc_ct_interface->send_pass_through_cmd(&g_avrcp_btaddr,AVRCP_POP_STOP, BUTTON_ACT_RELEASE);
    }
    else
    {
        g_bt_rc_ct_interface->send_pass_through_cmd(&g_avrcp_btaddr,AVRCP_POP_STOP, BUTTON_ACT_PRESS);
        /*x_thread_delay(200);*/
        usleep(200*1000);
        g_bt_rc_ct_interface->send_pass_through_cmd(&g_avrcp_btaddr,AVRCP_POP_STOP, BUTTON_ACT_RELEASE);
    }

    g_player_status.play_status = AVRCP_STATUS_STOPPED;

    return 0;
}

static int linuxbt_rc_send_fwd_handler(BTN_ACTION button_action)
{
    FUNC_ENTRY;

    if (g_bt_rc_ct_interface == NULL)
    {
        BT_DBG_ERROR(BT_DEBUG_AVRCP,  " Failed to get AVRCP interface");
        return -1;
    }

    if (BUTTON_ACT_PRESS == button_action)
    {
        g_bt_rc_ct_interface->send_pass_through_cmd(&g_avrcp_btaddr,AVRCP_POP_FORWARD, BUTTON_ACT_PRESS);
    }
    else if (BUTTON_ACT_RELEASE == button_action)
    {
        g_bt_rc_ct_interface->send_pass_through_cmd(&g_avrcp_btaddr,AVRCP_POP_FORWARD, BUTTON_ACT_RELEASE);
    }
    else
    {
        g_bt_rc_ct_interface->send_pass_through_cmd(&g_avrcp_btaddr,AVRCP_POP_FORWARD, BUTTON_ACT_PRESS);
        /*x_thread_delay(200);*/
        usleep(200*1000);
        g_bt_rc_ct_interface->send_pass_through_cmd(&g_avrcp_btaddr,AVRCP_POP_FORWARD, BUTTON_ACT_RELEASE);
    }

    return 0;
}

static int linuxbt_rc_send_bwd_handler(BTN_ACTION button_action)
{
    FUNC_ENTRY;

    if (g_bt_rc_ct_interface == NULL)
    {
        BT_DBG_ERROR(BT_DEBUG_AVRCP,  " Failed to get AVRCP interface");
        return -1;
    }

    if (BUTTON_ACT_PRESS == button_action)
    {
        g_bt_rc_ct_interface->send_pass_through_cmd(&g_avrcp_btaddr,AVRCP_POP_BACKWARD, BUTTON_ACT_PRESS);
    }
    else if (BUTTON_ACT_RELEASE == button_action)
    {
        g_bt_rc_ct_interface->send_pass_through_cmd(&g_avrcp_btaddr,AVRCP_POP_BACKWARD, BUTTON_ACT_RELEASE);
    }
    else
    {
        g_bt_rc_ct_interface->send_pass_through_cmd(&g_avrcp_btaddr,AVRCP_POP_BACKWARD, BUTTON_ACT_PRESS);
        /*x_thread_delay(200);*/
        usleep(200*1000);
        g_bt_rc_ct_interface->send_pass_through_cmd(&g_avrcp_btaddr,AVRCP_POP_BACKWARD, BUTTON_ACT_RELEASE);
    }

    return 0;
}

static int linuxbt_rc_send_ffwd_handler(BTN_ACTION button_action)
{
    FUNC_ENTRY;

    if (g_bt_rc_ct_interface == NULL)
    {
        BT_DBG_ERROR(BT_DEBUG_AVRCP,  " Failed to get AVRCP interface");
        return -1;
    }

    if (BUTTON_ACT_PRESS == button_action)
    {
        g_bt_rc_ct_interface->send_pass_through_cmd(&g_avrcp_btaddr,AVRCP_POP_FAST_FORWARD, BUTTON_ACT_PRESS);
    }
    else if (BUTTON_ACT_RELEASE == button_action)
    {
        g_bt_rc_ct_interface->send_pass_through_cmd(&g_avrcp_btaddr,AVRCP_POP_FAST_FORWARD, BUTTON_ACT_RELEASE);
    }
    else
    {
        g_bt_rc_ct_interface->send_pass_through_cmd(&g_avrcp_btaddr,AVRCP_POP_FAST_FORWARD, BUTTON_ACT_PRESS);
        /*x_thread_delay(200);*/
        usleep(200*1000);
        g_bt_rc_ct_interface->send_pass_through_cmd(&g_avrcp_btaddr,AVRCP_POP_FAST_FORWARD, BUTTON_ACT_RELEASE);
    }

    return 0;
}

static int linuxbt_rc_send_rwd_handler(BTN_ACTION button_action)
{
    FUNC_ENTRY;

    if (g_bt_rc_ct_interface == NULL)
    {
        BT_DBG_ERROR(BT_DEBUG_AVRCP,  " Failed to get AVRCP interface");
        return -1;
    }

    if (BUTTON_ACT_PRESS == button_action)
    {
        g_bt_rc_ct_interface->send_pass_through_cmd(&g_avrcp_btaddr,AVRCP_POP_REWIND, BUTTON_ACT_PRESS);
    }
    else if (BUTTON_ACT_RELEASE == button_action)
    {
        g_bt_rc_ct_interface->send_pass_through_cmd(&g_avrcp_btaddr,AVRCP_POP_REWIND, BUTTON_ACT_RELEASE);
    }
    else
    {
        g_bt_rc_ct_interface->send_pass_through_cmd(&g_avrcp_btaddr,AVRCP_POP_REWIND, BUTTON_ACT_PRESS);
        /*x_thread_delay(200);*/
        usleep(200*1000);
        g_bt_rc_ct_interface->send_pass_through_cmd(&g_avrcp_btaddr,AVRCP_POP_REWIND, BUTTON_ACT_RELEASE);
    }

    return 0;
}

static int linuxbt_rc_send_volumeup_handler(BTN_ACTION button_action)
{
    FUNC_ENTRY;

    if (g_bt_rc_ct_interface == NULL)
    {
        BT_DBG_ERROR(BT_DEBUG_AVRCP,  " Failed to get AVRCP interface");
        return -1;
    }

    if (BUTTON_ACT_PRESS == button_action)
    {
        g_bt_rc_ct_interface->send_pass_through_cmd(&g_avrcp_btaddr,AVRCP_POP_VOLUME_UP, BUTTON_ACT_PRESS);
    }
    else if (BUTTON_ACT_RELEASE == button_action)
    {
        g_bt_rc_ct_interface->send_pass_through_cmd(&g_avrcp_btaddr,AVRCP_POP_VOLUME_UP, BUTTON_ACT_RELEASE);
    }
    else
    {
        g_bt_rc_ct_interface->send_pass_through_cmd(&g_avrcp_btaddr,AVRCP_POP_VOLUME_UP, BUTTON_ACT_PRESS);
        /*x_thread_delay(200);*/
        usleep(200*1000);
        g_bt_rc_ct_interface->send_pass_through_cmd(&g_avrcp_btaddr,AVRCP_POP_VOLUME_UP, BUTTON_ACT_RELEASE);
    }

    return 0;
}

static int linuxbt_rc_send_volumedown_handler(BTN_ACTION button_action)
{
    FUNC_ENTRY;

    if (g_bt_rc_ct_interface == NULL)
    {
        BT_DBG_ERROR(BT_DEBUG_AVRCP,  " Failed to get AVRCP interface");
        return -1;
    }

    if (BUTTON_ACT_PRESS == button_action)
    {
        g_bt_rc_ct_interface->send_pass_through_cmd(&g_avrcp_btaddr,AVRCP_POP_VOLUME_DOWN, BUTTON_ACT_PRESS);
    }
    else if (BUTTON_ACT_RELEASE == button_action)
    {
        g_bt_rc_ct_interface->send_pass_through_cmd(&g_avrcp_btaddr,AVRCP_POP_VOLUME_DOWN, BUTTON_ACT_RELEASE);
    }
    else
    {
        g_bt_rc_ct_interface->send_pass_through_cmd(&g_avrcp_btaddr,AVRCP_POP_VOLUME_DOWN, BUTTON_ACT_PRESS);
        /*x_thread_delay(200);*/
        usleep(200*1000);
        g_bt_rc_ct_interface->send_pass_through_cmd(&g_avrcp_btaddr,AVRCP_POP_VOLUME_DOWN, BUTTON_ACT_RELEASE);
    }

    return 0;
}

int linuxbt_rc_send_cmd_handler(BTCmdType bt_cmd_type, BTN_ACTION button_action)
{
    FUNC_ENTRY;

    BT_DBG_NOTICE(BT_DEBUG_AVRCP,  " bd_addr = %2x:%2x:%2x:%2x:%2x:%2x",
                    g_avrcp_btaddr.address[0], g_avrcp_btaddr.address[1],
                    g_avrcp_btaddr.address[2], g_avrcp_btaddr.address[3],
                    g_avrcp_btaddr.address[4], g_avrcp_btaddr.address[5]);
    switch (bt_cmd_type)
    {
    case BT_CMD_TYPE_PLAY:
        BT_DBG_NORMAL(BT_DEBUG_AVRCP, " send play cmd");
        linuxbt_rc_send_play_handler(button_action);
        break;
    case BT_CMD_TYPE_PAUSE:
        BT_DBG_NORMAL(BT_DEBUG_AVRCP, " send pause cmd");
        linuxbt_rc_send_pause_handler(button_action);
        break;
    case BT_CMD_TYPE_FWD:
        BT_DBG_NORMAL(BT_DEBUG_AVRCP, " send forward cmd");
        linuxbt_rc_send_fwd_handler(button_action);
        break;
    case BT_CMD_TYPE_BWD:
        BT_DBG_NORMAL(BT_DEBUG_AVRCP, " send backward cmd");
        linuxbt_rc_send_bwd_handler(button_action);
        break;
    case BT_CMD_TYPE_FFWD:
        BT_DBG_NORMAL(BT_DEBUG_AVRCP, " send fast forward cmd");
        linuxbt_rc_send_ffwd_handler(button_action);
        break;
    case BT_CMD_TYPE_RWD:
        BT_DBG_NORMAL(BT_DEBUG_AVRCP, " send rewind cmd");
        linuxbt_rc_send_rwd_handler(button_action);
        break;
    case BT_CMD_TYPE_STOP:
        BT_DBG_NORMAL(BT_DEBUG_AVRCP, " send stop cmd");
        linuxbt_rc_send_stop_handler(button_action);
        break;
    case BT_CMD_TYPE_VOLUME_UP:
        BT_DBG_NORMAL(BT_DEBUG_AVRCP, " send volume up cmd");
        linuxbt_rc_send_volumeup_handler(button_action);
        break;
    case BT_CMD_TYPE_VOLUME_DOWN:
        BT_DBG_NORMAL(BT_DEBUG_AVRCP, " send volume down cmd");
        linuxbt_rc_send_volumedown_handler(button_action);
        break;
    default:
        BT_DBG_ERROR(BT_DEBUG_AVRCP,  " send invalid cmd");
        return -1;
    }

    return 0;
}

int linuxbt_rc_send_get_playstatus_cmd_handler(VOID)
{
    FUNC_ENTRY;

#if defined(MTK_LINUX_AVRCP_PLUS) && (MTK_LINUX_AVRCP_PLUS == TRUE)
    if (g_bt_avrcp_ex_interface == NULL)
    {
        BT_DBG_ERROR(BT_DEBUG_AVRCP,  " Failed to get AVRCP interface");
        return -1;
    }

    g_bt_avrcp_ex_interface->send_get_playstatus_cmd();
#endif

    return 0;
}

int linuxbt_rc_send_reg_notification_handler(char *pNotificationStr)
{
    //btrc_event_id_t event_id;
    //unsigned int interval = 0;

    FUNC_ENTRY;

    if (g_bt_rc_ct_interface == NULL)
    {
        BT_DBG_ERROR(BT_DEBUG_AVRCP,  " Failed to get AVRCP interface");
        return -1;
    }

#if 0
    BT_DBG_NORMAL(BT_DEBUG_AVRCP, " Registering notification for event %s", pNotificationStr);

    if (strcmp(pNotificationStr, "playStatus") == 0)
    {
        event_id = BTRC_EVT_PLAY_STATUS_CHANGED;
    }
    else if (strcmp(pNotificationStr, "trackChange") == 0)
    {
        event_id = BTRC_EVT_TRACK_CHANGE;
    }
    else if (strcmp(pNotificationStr, "trackEnd") == 0)
    {
        event_id = BTRC_EVT_TRACK_REACHED_END;
    }
    else if (strcmp(pNotificationStr, "trackStart") == 0)
    {
        event_id = BTRC_EVT_TRACK_REACHED_START;
    }
    else if (strcmp(pNotificationStr, "position") == 0)
    {
        event_id = BTRC_EVT_PLAY_POS_CHANGED;
        //No need this notification, or else need set interval value.
    }
    else if (strcmp(pNotificationStr, "setting") == 0)
    {
        event_id = BTRC_EVT_APP_SETTINGS_CHANGED;
    }
    else if (strcmp(pNotificationStr, "volume") == 0)
    {
        event_id = BTRC_EVT_VOLUME_CHANGED;
    }
    else if (strcmp(pNotificationStr, "batteryStatus") == 0)
    {
        event_id = BTRC_EVT_BATT_STATUS_CHANGED;
    }
    else if (strcmp(pNotificationStr, "systemStatus") == 0)
    {
        event_id = BTRC_EVT_SYSTEM_STATUS_CHANGED;
    }
    else if (strcmp(pNotificationStr, "nowPlaying") == 0)
    {
        event_id = BTRC_EVT_NOW_PLAYING_CONTENT_CHANGED;
    }
    else if (strcmp(pNotificationStr, "availablePlayers") == 0)
    {
        event_id = BTRC_EVT_AVAILABLE_PLAYERS_CHANGED;
    }
    else if (strcmp(pNotificationStr, "addressedPlayer") == 0)
    {
        event_id = BTRC_EVT_ADDRESSED_PLAYER_CHANGED;
    }
    else if (strcmp(pNotificationStr, "uids") == 0)
    {
        event_id = BTRC_EVT_UIDS_CHANGED;
    }
    else
    {
        BT_DBG_ERROR(BT_DEBUG_AVRCP, "not implement this notification: %s", pNotificationStr);
        return -1;
    }

    BT_DBG_NOTICE(BT_DEBUG_AVRCP,  " bd_addr = %2x:%2x:%2x:%2x:%2x:%2x",
                    g_avrcp_btaddr.address[0], g_avrcp_btaddr.address[1],
                    g_avrcp_btaddr.address[2], g_avrcp_btaddr.address[3],
                    g_avrcp_btaddr.address[4], g_avrcp_btaddr.address[5]);
    g_bt_rc_ct_interface->register_notification(&g_avrcp_btaddr, event_id, interval);

#endif

    return 0;
}

int linuxbt_rc_send_set_playstatus_handler(char *pstatus)
{

    //int status;

    FUNC_ENTRY;

    if (g_bt_rc_ct_interface == NULL)
    {
        BT_DBG_ERROR(BT_DEBUG_AVRCP,  " Failed to get AVRCP interface");
        return -1;
    }

    /*if(!strcmp(pstatus, "stopped"))
        status = AVRCP_PLAYSTATUS_STOPPED;
    else if(!strcmp(pstatus, "playing"))
        status = AVRCP_PLAYSTATUS_PLAYING;
    else if(!strcmp(pstatus, "paused"))
        status = AVRCP_PLAYSTATUS_PAUSED;
    else if(!strcmp(pstatus, "fwd_seek"))
        status = AVRCP_PLAYSTATUS_FWD_SEEK;
    else if(!strcmp(pstatus, "rev_seek"))
        status = AVRCP_PLAYSTATUS_REV_SEEK;
    else if(!strcmp(pstatus, "error"))
        status = AVRCP_PLAYSTATUS_ERROR;
    else
    {
        BT_DBG_ERROR(BT_DEBUG_AVRCP,  " Parameter should be [stopped | playing | paused | fwd_seek | rev_seek | error]");
        return -1;
    }*/

    #ifdef __CE_MTK_IF_API__
    //g_bt_rc_ct_interface->set_playback_status(status);
    #endif
    return 0;
}

/*int linuxbt_rc_send_get_capabilities_handler(btrc_capability_id_t capability_id)
{
    FUNC_ENTRY;

    if(g_bt_rc_ct_interface == NULL)
    {
        BT_DBG_ERROR(BT_DEBUG_AVRCP,  " Failed to get AVRCP interface");
        return -1;
    }

    BT_DBG_NOTICE(BT_DEBUG_AVRCP,  " bd_addr = %2x:%2x:%2x:%2x:%2x:%2x",
                    g_avrcp_btaddr.address[0], g_avrcp_btaddr.address[1],
                    g_avrcp_btaddr.address[2], g_avrcp_btaddr.address[3],
                    g_avrcp_btaddr.address[4], g_avrcp_btaddr.address[5]);
#ifdef __CE_MTK_IF_API__
    //g_bt_rc_ct_interface->get_capabilities(&g_avrcp_btaddr, capability_id);
#endif
    return 0;
}*/

int linuxbt_rc_send_volume_change_handler(void)
{
    //btrc_event_id_t event_id;
    //unsigned int interval = 0;

    BT_DBG_NORMAL(BT_DEBUG_AVRCP, " %s() ", __FUNCTION__);

    if (g_bt_rc_ct_interface == NULL)
    {
        BT_DBG_ERROR(BT_DEBUG_AVRCP,  " Failed to get AVRCP interface");
        return -1;
    }

    BT_DBG_NOTICE(BT_DEBUG_AVRCP,  " bd_addr = %2x:%2x:%2x:%2x:%2x:%2x",
                    g_avrcp_btaddr.address[0], g_avrcp_btaddr.address[1],
                    g_avrcp_btaddr.address[2], g_avrcp_btaddr.address[3],
                    g_avrcp_btaddr.address[4], g_avrcp_btaddr.address[5]);
#ifdef __CE_MTK_IF_API__
    //event_id = BTRC_EVT_VOLUME_CHANGED;
    //g_bt_rc_ct_interface->register_notification(&g_avrcp_btaddr, event_id, interval);
#endif
    return 0;
}

int linuxbt_rc_send_set_volume_handler(uint8_t volume)
{
    BT_DBG_NORMAL(BT_DEBUG_AVRCP, " %s() ", __FUNCTION__);

    if (g_bt_rc_ct_interface == NULL)
    {
        BT_DBG_ERROR(BT_DEBUG_AVRCP,  " Failed to get AVRCP interface");
        return -1;
    }

#ifdef __CE_MTK_IF_API__
    //BT_DBG_NORMAL(BT_DEBUG_AVRCP, " volume = %d", volume);
    //g_bt_rc_ct_interface->set_volume_ctrl(volume);
#endif
    return 0;
}

int linuxbt_rc_send_connect_browse_handler(void)
{
    BT_DBG_NORMAL(BT_DEBUG_AVRCP, " %s() ", __FUNCTION__);

    if (g_bt_rc_ct_interface == NULL)
    {
        BT_DBG_ERROR(BT_DEBUG_AVRCP,  " Failed to get AVRCP interface");
        return -1;
    }

#ifdef __CE_MTK_IF_API__
    //g_bt_rc_ct_interface->connect_browse(1, 1);
#endif

    return 0;
}

int linuxbt_rc_send_disconnect_browse_handler(void)
{
    BT_DBG_NORMAL(BT_DEBUG_AVRCP, " %s() ", __FUNCTION__);

    if (g_bt_rc_ct_interface == NULL)
    {
        BT_DBG_ERROR(BT_DEBUG_AVRCP,  " Failed to get AVRCP interface");
        return -1;
    }

#ifdef __CE_MTK_IF_API__
    //g_bt_rc_ct_interface->disconnect_browse(1, 1);
#endif

    return 0;
}

int linuxbt_rc_send_set_addressed_player_handler(uint32_t player_id)
{
    BT_DBG_NORMAL(BT_DEBUG_AVRCP, " %s() ", __FUNCTION__);

    if (g_bt_rc_ct_interface == NULL)
    {
        BT_DBG_ERROR(BT_DEBUG_AVRCP,  " Failed to get AVRCP interface");
        return -1;
    }

#ifdef __CE_MTK_IF_API__
    //g_bt_rc_ct_interface->set_addressed_player(&g_avrcp_btaddr, player_id);
#endif

    return 0;
}


int linuxbt_rc_send_set_browsed_player_handler(uint32_t player_id)
{
    BT_DBG_NORMAL(BT_DEBUG_AVRCP, " %s() ", __FUNCTION__);

    if (g_bt_rc_ct_interface == NULL)
    {
        BT_DBG_ERROR(BT_DEBUG_AVRCP,  " Failed to get AVRCP interface");
        return -1;
    }

#ifdef __CE_MTK_IF_API__
    //g_bt_rc_ct_interface->set_browsed_player(&g_avrcp_btaddr, player_id);
#endif

    return 0;
}

int get_attribute_id(char *attr_str)
{
    if (0 == strncmp(attr_str, "title", sizeof("title")))
    {
        return 0x1;
    }
    else if (0 == strncmp(attr_str, "artist_name", sizeof("artist_name")))
    {
        return 0x2;
    }
    else if (0 == strncmp(attr_str, "album_name", sizeof("album_name")))
    {
        return 0x3;
    }
    else if (0 == strncmp(attr_str, "track_number", sizeof("track_number")))
    {
        return 0x4;
    }
    else if (0 == strncmp(attr_str, "num_of_tracks", sizeof("num_of_tracks")))
    {
        return 0x5;
    }
    else if (0 == strncmp(attr_str, "genre", sizeof("genre")))
    {
        return 0x6;
    }
    else if (0 == strncmp(attr_str, "playing_time", sizeof("playing_time")))
    {
        return 0x7;
    }
    else
    {
        return -1;
    }


}

int linuxbt_rc_send_get_folder_items_handler(uint8_t scope,
        uint32_t start_item,
        uint32_t end_item,
        uint8_t count,
        uint32_t *attribute_ids)
{
    BT_DBG_NORMAL(BT_DEBUG_AVRCP, " %s() ", __FUNCTION__);

    if (g_bt_rc_ct_interface == NULL)
    {
        BT_DBG_ERROR(BT_DEBUG_AVRCP,  " Failed to get AVRCP interface");
        return -1;
    }

#ifdef __CE_MTK_IF_API__
    //g_bt_rc_ct_interface->get_folder_items(&g_avrcp_btaddr, scope, start_item, end_item, count, attribute_ids);
#endif

    return 0;
}

int linuxbt_rc_send_change_path_handler (uint16_t uid_counter,uint8_t direction, uint8_t *folder_uid)
{
    BT_DBG_NORMAL(BT_DEBUG_AVRCP, " %s() ", __FUNCTION__);

    if (g_bt_rc_ct_interface == NULL)
    {
        BT_DBG_ERROR(BT_DEBUG_AVRCP,  " Failed to get AVRCP interface");
        return -1;
    }

#ifdef __CE_MTK_IF_API__
    //direction:up-->0, down-->1
    //g_bt_rc_ct_interface->change_path(&g_avrcp_btaddr, uid_counter, direction, folder_uid);
#endif

    return 0;
}

int linuxbt_rc_send_get_item_attributes_handler(uint8_t scope,
        uint8_t *uuid,
        uint16_t uid_counter,
        uint8_t count,
        uint32_t *attribute_ids)
{
    BT_DBG_NORMAL(BT_DEBUG_AVRCP, " %s() ", __FUNCTION__);

    if (g_bt_rc_ct_interface == NULL)
    {
        BT_DBG_ERROR(BT_DEBUG_AVRCP,  " Failed to get AVRCP interface");
        return -1;
    }

#ifdef __CE_MTK_IF_API__
    //g_bt_rc_ct_interface->get_item_attributes(&g_avrcp_btaddr, scope, uuid, uid_counter, count, attribute_ids);
#endif

    return 0;
}


int linuxbt_rc_send_search_handler (uint16_t len, uint8_t *search_str)
{

    BT_DBG_NORMAL(BT_DEBUG_AVRCP, " %s() ", __FUNCTION__);

    if (g_bt_rc_ct_interface == NULL)
    {
        BT_DBG_ERROR(BT_DEBUG_AVRCP,  " Failed to get AVRCP interface");
        return -1;
    }
    if (search_str == NULL)
    {
        BT_DBG_ERROR(BT_DEBUG_AVRCP,  " null pointer of search_str");
        return -1;
    }

#ifdef __CE_MTK_IF_API__
    //g_bt_rc_ct_interface->search(&g_avrcp_btaddr, AVRCP_CHARSET_UTF8, len, search_str);
#endif
    return 0;
}

int linuxbt_rc_send_volume_change_rsp_handler(uint8_t u1volume)
{
    BT_DBG_NORMAL(BT_DEBUG_AVRCP,  "[AVRCP] send volume:%x", u1volume);

    if (g_bt_rc_ct_interface == NULL)
    {
        BT_DBG_NOTICE(BT_DEBUG_AVRCP,  "[AVRCP] Failed to get AVRCP interface");
        return -1;
    }

    g_bt_rc_ct_interface->register_abs_vol_rsp(&g_avrcp_btaddr, BTRC_NOTIFICATION_TYPE_CHANGED, u1volume, g_avrcp_abs_vol_label);

    return 0;
}


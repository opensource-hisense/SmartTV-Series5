#include <unistd.h>
#include "bluetooth.h"

#include "bt_mw_common.h"
#include "linuxbt_common.h"
#include "linuxbt_avrcp_tg_if.h"
#if ENABLE_MTK_TV_PLATFORM
#include "inc/x_dbg.h"
#include "inc/x_os.h"
#include "x_ipcd.h"
#endif
#include "bluetooth_sync.h"
#include "bt_mw_avrcp.h"
//#include "bt_avrcp_api.h"
//#include "bt_avrcp_key.h"
#if defined(MTK_LINUX_AVRCP_PLUS) && (MTK_LINUX_AVRCP_PLUS == TRUE)
#include "mtk_bt_rc.h"
#else
#include "bt_rc.h"
#endif

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

static bt_bdaddr_t g_rc_bdaddr;
static btrc_interface_t *g_bt_rc_tg_interface = NULL;
static bool test_mode = 0;
extern void *linuxbt_gap_get_profile_interface(const char *profile_id);
static int g_pts_track_selected = 0;
static int g_pts_LargeGetElementRsp = 0;

extern BtAppAvrcpCmdCbk AppAvrcpcmdCbk;
extern BtAppAvrcpVolumeChangeCbk AppVolumeCbk;
extern BtAppAvrcpGetPlayStatusCbk AppGetPlayStatusCbk;
//extern void avrcp_input_sendkey(uint16_t avrcpkey, int pressed);
//extern void btmtk_avrcp_send_get_playstatus_rsp(AvrcpChannelContext *cntx, U8 error, U32 song_length, U32 song_position, U8 status);

void linuxbt_rc_tg_remote_features_cb (bt_bdaddr_t *bd_addr, btrc_remote_features_t features)
{
    BT_DBG_NOTICE(BT_DEBUG_AVRCP,"[AVRCP] %s() features = %x ",__FUNCTION__, features);
}

void linuxbt_rc_tg_get_play_status_cb(void)
{
    BT_DBG_NORMAL(BT_DEBUG_AVRCP,"Enter [AVRCP] %s()",__FUNCTION__);

    BTPlayState bt_playstatus = BT_PLAY_STATE_STOP;
    btrc_play_status_t status = BTRC_PLAYSTATE_ERROR;

    /*callback to app to get status*/
    if(AppGetPlayStatusCbk)
    {
        AppGetPlayStatusCbk(&bt_playstatus);
    }
    BT_DBG_INFO(BT_DEBUG_AVRCP,"play status:%d", bt_playstatus);
    switch(bt_playstatus)
    {
        case BT_PLAY_STATE_STOP:
        {
            status = BTRC_PLAYSTATE_STOPPED;
            break;
        }
        case BT_PLAY_STATE_PLAY:
        {
            status = BTRC_PLAYSTATE_PLAYING;
            break;
        }
        case BT_PLAY_STATE_PAUSE:
        {
            status = BTRC_PLAYSTATE_PAUSED;
            break;
        }
        case BT_PLAY_STATUS_FWD_SEEK:
        {
            status = BTRC_PLAYSTATE_FWD_SEEK;
            break;
        }
        case BT_PLAY_STATUS_REV_SEEK:
        {
            status = BTRC_PLAYSTATE_REV_SEEK;
            break;
        }
        default:
        {
            status = BTRC_PLAYSTATE_ERROR;
            break;
        }
    }
    g_bt_rc_tg_interface->get_play_status_rsp(status, 0xffffffff, 0xffffffff);
    if (test_mode == 1)
    {
        btrc_register_notification_t p_param;
        p_param.play_status = BTRC_PLAYSTATE_STOPPED;

        g_bt_rc_tg_interface->get_play_status_rsp(BTRC_PLAYSTATE_STOPPED, 0, 0);
        g_bt_rc_tg_interface->register_notification_rsp(BTRC_EVT_PLAY_STATUS_CHANGED, BTRC_NOTIFICATION_TYPE_CHANGED, &p_param);

        test_mode = 0;
    }
    BT_DBG_NORMAL(BT_DEBUG_AVRCP,"Exit [AVRCP] %s()",__FUNCTION__);
}

void linuxbt_rc_tg_list_player_app_attr_cb(void)
{
    BT_DBG_NORMAL(BT_DEBUG_AVRCP,"[AVRCP] %s() ",__FUNCTION__);
}

void linuxbt_rc_tg_list_player_app_values_cb (btrc_player_attr_t attr_id)
{
    BT_DBG_NORMAL(BT_DEBUG_AVRCP,"[AVRCP] %s() attr_id = %x",__FUNCTION__, attr_id);
}

void linuxbt_rc_tg_get_player_app_value_cb (uint8_t num_attr, btrc_player_attr_t *p_attrs)
{
    BT_DBG_NORMAL(BT_DEBUG_AVRCP,"[AVRCP] %s() p_attrs = %x",__FUNCTION__, *p_attrs);
}

void linuxbt_rc_tg_get_player_app_attrs_text_cb (uint8_t num_attr, btrc_player_attr_t *p_attrs)
{
    BT_DBG_NORMAL(BT_DEBUG_AVRCP,"[AVRCP] %s() p_attrs = %x",__FUNCTION__, *p_attrs);
}

void linuxbt_rc_tg_get_player_app_values_text_cb (uint8_t attr_id, uint8_t num_val, uint8_t *p_vals)
{
    BT_DBG_NORMAL(BT_DEBUG_AVRCP,"[AVRCP] %s() attr_id = %x num_val = %x p_vals = %x",__FUNCTION__, attr_id, num_val,  *p_vals);
}

void linuxbt_rc_tg_set_player_app_value_cb (btrc_player_settings_t *p_vals)
{
    BT_DBG_NORMAL(BT_DEBUG_AVRCP,"[AVRCP] %s() num_attr:%d, attr_ids:%s, attr_values:%s",__FUNCTION__,p_vals->num_attr, p_vals->attr_ids, p_vals->attr_values);
}



void linuxbt_rc_tg_get_element_attr_cb (uint8_t num_attr, btrc_media_attr_t *p_attrs)
{
    BT_DBG_NORMAL(BT_DEBUG_AVRCP,"[AVRCP] %s() num_attr = %x, p_attrs = %x",__FUNCTION__, num_attr, *p_attrs);
    if (test_mode)
    {
        btrc_element_attr_val_t attrs[BTRC_MAX_ELEM_ATTR_SIZE];
        int i = 0;
        int cnt = 0;

        for ( i = 0 ; i < num_attr ; i++ )
        {
            attrs[i].attr_id = p_attrs[i];
            switch (p_attrs[i])
            {
                case BTRC_MEDIA_ATTR_TITLE: // Title
                    memcpy(attrs[i].text, "TITLE", 5);
                    if (g_pts_LargeGetElementRsp)
                    {
                        memset(&attrs[i].text, 'a', sizeof(attrs[0].text));
                        attrs[i].text[BTRC_MAX_ATTR_STR_LEN-1] = 0;
                    }
                    cnt++;
                    break;
                case BTRC_MEDIA_ATTR_ARTIST: // Artist
                    memcpy(attrs[i].text, "Artist", 6);
                    if (g_pts_LargeGetElementRsp)
                    {
                        memset(&attrs[i].text, 'a', sizeof(attrs[0].text));
                        attrs[i].text[BTRC_MAX_ATTR_STR_LEN-1] = 0;
                    }
                    cnt++;
                    break;
                case BTRC_MEDIA_ATTR_ALBUM: // Album
                    memcpy(attrs[i].text, "Album", 5);
                    cnt++;
                    break;
                case BTRC_MEDIA_ATTR_TRACK_NUM: // NumOfMedia
                    memcpy(attrs[i].text, "11", 2);
                    cnt++;
                    break;
                case BTRC_MEDIA_ATTR_NUM_TRACKS: // TotalNumOfMedia
                    memcpy(attrs[i].text, "22", 2);
                    cnt++;
                    break;
                case BTRC_MEDIA_ATTR_GENRE: // Genre
                    memcpy(attrs[i].text, "Genre", 5);
                    cnt++;
                    break;
                case BTRC_MEDIA_ATTR_PLAYING_TIME: // PlayingTimeinMillisecond
                    memcpy(attrs[i].text, "123456", 6);
                    cnt++;
                    break;
                default:
                    break;
            }
        }
        if (0 < cnt)
        {
            g_bt_rc_tg_interface->get_element_attr_rsp(cnt, attrs);
        }
        g_pts_LargeGetElementRsp = 0;
    }
    return;
}

void linuxbt_rc_tg_register_notification_cb (btrc_event_id_t event_id, uint32_t param)
{
    BT_DBG_NORMAL(BT_DEBUG_AVRCP,"[AVRCP] %s() event_id = %x, param = %x",__FUNCTION__, event_id, param);
    btrc_register_notification_t p_param;
    memset(&p_param, 0x0, sizeof(btrc_register_notification_t));

    if(test_mode == 1)
    {
        switch(event_id)
        {
        case BTRC_EVT_PLAY_STATUS_CHANGED:
            p_param.play_status = param;
            g_bt_rc_tg_interface->register_notification_rsp( event_id, BTRC_NOTIFICATION_TYPE_INTERIM, &p_param);
            break;
        case BTRC_EVT_TRACK_CHANGE:
            if (0 == g_pts_track_selected)
            {
                memset(&p_param, 0xFF, sizeof(btrc_register_notification_t));
            }
            g_bt_rc_tg_interface->register_notification_rsp( event_id, BTRC_NOTIFICATION_TYPE_INTERIM, &p_param);
            sleep(1);
            memset(&p_param, 0, sizeof(btrc_register_notification_t));
            g_bt_rc_tg_interface->register_notification_rsp( event_id, BTRC_NOTIFICATION_TYPE_CHANGED, &p_param);
            break;
        default:
            break;
        }
    }
}

void linuxbt_rc_tg_volume_change_cb (uint8_t volume, uint8_t ctype)
{
    BT_DBG_NORMAL(BT_DEBUG_AVRCP,"[AVRCP] %s() volume = %x ctype = %x",__FUNCTION__, volume, ctype);
}

/* Operation ID list for Passthrough commands
*/
#define AVRC_ID_SELECT      0x00    /* select */
#define AVRC_ID_UP          0x01    /* up */
#define AVRC_ID_DOWN        0x02    /* down */
#define AVRC_ID_LEFT        0x03    /* left */
#define AVRC_ID_RIGHT       0x04    /* right */
#define AVRC_ID_RIGHT_UP    0x05    /* right-up */
#define AVRC_ID_RIGHT_DOWN  0x06    /* right-down */
#define AVRC_ID_LEFT_UP     0x07    /* left-up */
#define AVRC_ID_LEFT_DOWN   0x08    /* left-down */
#define AVRC_ID_ROOT_MENU   0x09    /* root menu */
#define AVRC_ID_SETUP_MENU  0x0A    /* setup menu */
#define AVRC_ID_CONT_MENU   0x0B    /* contents menu */
#define AVRC_ID_FAV_MENU    0x0C    /* favorite menu */
#define AVRC_ID_EXIT        0x0D    /* exit */
#define AVRC_ID_0           0x20    /* 0 */
#define AVRC_ID_1           0x21    /* 1 */
#define AVRC_ID_2           0x22    /* 2 */
#define AVRC_ID_3           0x23    /* 3 */
#define AVRC_ID_4           0x24    /* 4 */
#define AVRC_ID_5           0x25    /* 5 */
#define AVRC_ID_6           0x26    /* 6 */
#define AVRC_ID_7           0x27    /* 7 */
#define AVRC_ID_8           0x28    /* 8 */
#define AVRC_ID_9           0x29    /* 9 */
#define AVRC_ID_DOT         0x2A    /* dot */
#define AVRC_ID_ENTER       0x2B    /* enter */
#define AVRC_ID_CLEAR       0x2C    /* clear */
#define AVRC_ID_CHAN_UP     0x30    /* channel up */
#define AVRC_ID_CHAN_DOWN   0x31    /* channel down */
#define AVRC_ID_PREV_CHAN   0x32    /* previous channel */
#define AVRC_ID_SOUND_SEL   0x33    /* sound select */
#define AVRC_ID_INPUT_SEL   0x34    /* input select */
#define AVRC_ID_DISP_INFO   0x35    /* display information */
#define AVRC_ID_HELP        0x36    /* help */
#define AVRC_ID_PAGE_UP     0x37    /* page up */
#define AVRC_ID_PAGE_DOWN   0x38    /* page down */
#define AVRC_ID_POWER       0x40    /* power */
#define AVRC_ID_VOL_UP      0x41    /* volume up */
#define AVRC_ID_VOL_DOWN    0x42    /* volume down */
#define AVRC_ID_MUTE        0x43    /* mute */
#define AVRC_ID_PLAY        0x44    /* play */
#define AVRC_ID_STOP        0x45    /* stop */
#define AVRC_ID_PAUSE       0x46    /* pause */
#define AVRC_ID_RECORD      0x47    /* record */
#define AVRC_ID_REWIND      0x48    /* rewind */
#define AVRC_ID_FAST_FOR    0x49    /* fast forward */
#define AVRC_ID_EJECT       0x4A    /* eject */
#define AVRC_ID_FORWARD     0x4B    /* forward */
#define AVRC_ID_BACKWARD    0x4C    /* backward */
#define AVRC_ID_ANGLE       0x50    /* angle */
#define AVRC_ID_SUBPICT     0x51    /* subpicture */
#define AVRC_ID_F1          0x71    /* F1 */
#define AVRC_ID_F2          0x72    /* F2 */
#define AVRC_ID_F3          0x73    /* F3 */
#define AVRC_ID_F4          0x74    /* F4 */
#define AVRC_ID_F5          0x75    /* F5 */
#define AVRC_ID_VENDOR      0x7E    /* vendor unique */
#define AVRC_KEYPRESSED_RELEASE 0x80

void linuxbt_rc_tg_passthrough_cmd_cb (int id, int key_state)
{
    BT_DBG_NORMAL(BT_DEBUG_AVRCP,"[AVRCP] %s() id = %x key_state = %x",__FUNCTION__, id, key_state);

    unsigned char cmd_type;
    BTCmdType bt_cmd_type = BT_CMD_TYPE_PLAY;

    BT_DBG_NORMAL(BT_DEBUG_AVRCP,"[AVRCP] %s(), bt_power_on = %d",__FUNCTION__, get_bt_power_on());
    cmd_type = (unsigned char)id;

    switch (cmd_type)
    {
        case AVRC_ID_PLAY:
        {
            BT_DBG_NOTICE(BT_DEBUG_AVRCP, "KEY_PLAYCD");
            bt_cmd_type = BT_CMD_TYPE_PLAY;
            break;
        }
        case AVRC_ID_STOP:
        {
            BT_DBG_NOTICE(BT_DEBUG_AVRCP, "KEY_STOPCD");
            bt_cmd_type = BT_CMD_TYPE_STOP;
            break;
        }
        case AVRC_ID_PAUSE:
        {
            BT_DBG_NOTICE(BT_DEBUG_AVRCP, "KEY_PAUSECD");
            bt_cmd_type = BT_CMD_TYPE_PAUSE;
            break;
        }
        case AVRC_ID_FORWARD:
        {
            BT_DBG_NOTICE(BT_DEBUG_AVRCP, "KEY_NEXTSONG");
            bt_cmd_type = BT_CMD_TYPE_FWD;
            break;
        }
        case AVRC_ID_BACKWARD:
        {
            bt_cmd_type = BT_CMD_TYPE_BWD;
            BT_DBG_NOTICE(BT_DEBUG_AVRCP, "KEY_PREVIOUSSONG");
            break;
        }
        case AVRC_ID_VOL_DOWN:
        {
            BT_DBG_NOTICE(BT_DEBUG_AVRCP, "KEY_VOLUMEDOWN");

            bt_cmd_type = BT_VOLUME_TYPE_DOWN;
            if (1 == key_state) {
                if(NULL != AppVolumeCbk)
                {
                    AppVolumeCbk(bt_cmd_type);
                }
                else
                {
                    BT_DBG_ERROR(BT_DEBUG_AVRCP, "null pointer AppVolumeCbk");
                }
            }
            return;
            break;
        }
        case AVRC_ID_VOL_UP:
        {
            BT_DBG_NOTICE(BT_DEBUG_AVRCP, "KEY_VOLUMEUP");
            bt_cmd_type = BT_VOLUME_TYPE_UP;
            if (1 == key_state) {
                if(NULL != AppVolumeCbk)
                {
                    AppVolumeCbk(bt_cmd_type);
                }
                else
                {
                    BT_DBG_ERROR(BT_DEBUG_AVRCP, "null pointer AppVolumeCbk");
                }
            }
            return;
            break;
        }
        case AVRC_ID_FAST_FOR:
            bt_cmd_type = BT_CMD_TYPE_FFWD;
            break;
        case AVRC_ID_REWIND:
            bt_cmd_type = BT_CMD_TYPE_RWD;
            break;
        default:
            //accept = 0;
        break;
    }

    if(!AppAvrcpcmdCbk)
    {
     BT_DBG_ERROR(BT_DEBUG_AVRCP,"[AVRCP] AppAvrcpcmdCbk not init");
     return;
    }
    AppAvrcpcmdCbk(bt_cmd_type, key_state);

}

#ifdef __CE_MTK_IF_API__
#ifdef __BT_AVRCP_V14__
void linuxbt_rc_tg_set_absolute_volume_ind_cb (uint8_t volume)
{
    UINT8 u1_volume = volume;
    BT_DBG_INFO(BT_DEBUG_AVRCP,"[AVRCP] volume set to %x ", volume);
    BT_DBG_INFO(BT_DEBUG_AVRCP,"[AVRCP] send volume response immediately.  volume = %x ", volume);
    g_bt_rc_tg_interface->set_volume_ind_res(volume);
    bluetooth_rc_tg_set_absolute_volume_ind_cb(u1_volume);
}
#endif // __BT_AVRCP_V14__
#endif //__CE_MTK_IF_API__

static btrc_callbacks_t g_bt_rc_tg_callbacks =
{
    sizeof(btrc_callbacks_t),
    linuxbt_rc_tg_remote_features_cb,
    linuxbt_rc_tg_get_play_status_cb,
    linuxbt_rc_tg_list_player_app_attr_cb,
    linuxbt_rc_tg_list_player_app_values_cb,
    linuxbt_rc_tg_get_player_app_value_cb,
    linuxbt_rc_tg_get_player_app_attrs_text_cb,
    linuxbt_rc_tg_get_player_app_values_text_cb,
    linuxbt_rc_tg_set_player_app_value_cb,
    linuxbt_rc_tg_get_element_attr_cb,
    linuxbt_rc_tg_register_notification_cb,
    linuxbt_rc_tg_volume_change_cb,
    linuxbt_rc_tg_passthrough_cmd_cb,
};

int linuxbt_rc_tg_init(void)
{
    int ret = 0;

    BT_DBG_NORMAL(BT_DEBUG_AVRCP,"[AVRCP] %s() ", __FUNCTION__);

    g_bt_rc_tg_interface = (btrc_interface_t *) linuxbt_gap_get_profile_interface(BT_PROFILE_AV_RC_ID);

    if (g_bt_rc_tg_interface == NULL)
    {
        BT_DBG_ERROR(BT_DEBUG_AVRCP,"[AVRCP] Failed to get AVRCP TG interface");
        return -1;
    }

    // Init AVRCP interface
    ret = g_bt_rc_tg_interface->init(&g_bt_rc_tg_callbacks);
    if (ret == BT_STATUS_SUCCESS)
    {
        BT_DBG_NOTICE(BT_DEBUG_AVRCP,"[AVRCP] success to init AVRCP TG interface");
        return 0;
    }
    else if(ret == BT_STATUS_DONE)
    {
        BT_DBG_NOTICE(BT_DEBUG_AVRCP,"[AVRCP] already init AVRCP TG interface");
        return 0;
    }
    BT_DBG_ERROR(BT_DEBUG_AVRCP,"[AVRCP] Failed to init AVRCP TG interface");

    return ret;
}

int linuxbt_rc_tg_deinit(void)
{
    BT_DBG_NORMAL(BT_DEBUG_AVRCP,"[AVRCP] %s() ", __FUNCTION__);

    if (g_bt_rc_tg_interface == NULL)
    {
    BT_DBG_ERROR(BT_DEBUG_AVRCP,"[AVRCP] Failed to get AVRCP TG interface");
    return -1;
    }

    g_bt_rc_tg_interface->cleanup();

    return 0;
}

int linuxbt_rc_tg_connect_handler(char *pbt_addr)
{
    bt_bdaddr_t bdaddr;

    BT_DBG_NORMAL(BT_DEBUG_AVRCP,"[AVRCP] %s() ", __FUNCTION__);

    if (g_bt_rc_tg_interface == NULL)
    {
        BT_DBG_ERROR(BT_DEBUG_AVRCP,"[AVRCP] Failed to get AVRCP interface");
        return -1;
    }

    linuxbt_btaddr_stoh(pbt_addr, &bdaddr);

    BT_DBG_INFO(BT_DEBUG_AVRCP,"AVRCP connect to %02X:%02X:%02X:%02X:%02X:%02X",
    bdaddr.address[0], bdaddr.address[1], bdaddr.address[2],
    bdaddr.address[3], bdaddr.address[4], bdaddr.address[5]);

    memcpy(&g_rc_bdaddr, &bdaddr, sizeof(bt_bdaddr_t));// For further AVRCP operations

    #ifdef __CE_MTK_IF_API__
    //g_bt_rc_tg_interface->connect(&bdaddr);
    #endif

    return 0;
}

int linuxbt_rc_tg_disconnect_handler(char *pbt_addr)
{
    BT_DBG_NORMAL(BT_DEBUG_AVRCP,"[AVRCP] %s() ", __FUNCTION__);

    if (g_bt_rc_tg_interface == NULL)
    {
        BT_DBG_ERROR(BT_DEBUG_AVRCP,"[AVRCP] Failed to get AVRCP TG interface");
        return -1;
    }

    #ifdef __CE_MTK_IF_API__
    BT_DBG_INFO(BT_DEBUG_AVRCP,"AVRCP connect to %02X:%02X:%02X:%02X:%02X:%02X",
                    g_rc_bdaddr.address[0], g_rc_bdaddr.address[1], g_rc_bdaddr.address[2],
                    g_rc_bdaddr.address[3], g_rc_bdaddr.address[4], g_rc_bdaddr.address[5]);
    //g_bt_rc_tg_interface->disconnect(&g_rc_bdaddr);
    #endif
    return 0;
}

int linuxbt_rc_tg_register_notification_test_mode_handler(int enable)
{
    BT_DBG_NORMAL(BT_DEBUG_AVRCP,"[AVRCP] %s() ", __FUNCTION__);

    test_mode = (enable == 1) ? 1 : 0;
    BT_DBG_INFO(BT_DEBUG_AVRCP,"[AVRCP] test mode %s", (1 == test_mode)?"Yes":"No");

    return 0;
}

//#ifdef FOR_PTS_TEST
int linuxbt_rc_tg_send_volume_change_rsp_handler(uint8_t u1interim, uint8_t u1volume)
{
    BT_DBG_NORMAL(BT_DEBUG_AVRCP,"[AVRCP] interim:%x send volume:%x", u1interim, u1volume);

    if (g_bt_rc_tg_interface == NULL)
    {
        BT_DBG_NOTICE(BT_DEBUG_AVRCP,"[AVRCP] Failed to get AVRCP interface");
        return -1;
    }

    //g_bt_rc_tg_interface->send_volume_change_rsp(u1interim, u1volume);

    return 0;
}

int linuxbt_rc_tg_send_track_change_rsp_handler(uint8_t u1track_selected)
{
    BT_DBG_NORMAL(BT_DEBUG_AVRCP,"[AVRCP] +++into ");

    if (test_mode == 0)
    {
        BT_DBG_INFO(BT_DEBUG_AVRCP,"[AVRCP] not test mode");
        return 0;
    }
    g_pts_track_selected = u1track_selected;

    return 0;
}

int linuxbt_rc_tg_set_large_element_attribute_response_handler(uint8_t u1enable)
{
    BT_DBG_NORMAL(BT_DEBUG_AVRCP,"[AVRCP] +++into ");
    if (test_mode == 0)
    {
        BT_DBG_INFO(BT_DEBUG_AVRCP,"[AVRCP] not test mode");
        return 0;
    }
    if (g_bt_rc_tg_interface == NULL)
    {
        BT_DBG_ERROR(BT_DEBUG_AVRCP,"[AVRCP] Failed to get AVRCP interface");
        return -1;
    }

    g_pts_LargeGetElementRsp = u1enable;

    return 0;
}
//#endif //PTS_TEST

int linuxbt_rc_tg_send_pts_test_handler(int enable)
{
    test_mode = enable ? 1 : 0;
    BT_DBG_INFO(BT_DEBUG_AVRCP,"[AVRCP] test mode %s", (1 == test_mode)?"Yes":"No");
    //btif_set_pts_test_mode(enable);
    return 0;
}

int linuxbt_rc_set_avrcp_delay(int delay)
{
    BT_DBG_NORMAL(BT_DEBUG_AVRCP,"[AVRCP] set delay: %d", delay);
    //btif_set_avrcp_delay(delay);
    return 0;
}


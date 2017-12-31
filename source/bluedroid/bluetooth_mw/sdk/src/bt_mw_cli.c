#if 0 // cc_temp

#include <pthread.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
//#include "bt_mmi.h"
#include "mtk_bluetooth.h"

#if ENABLE_MTK_TV_PLATFORM
#include "inc/x_common.h"
#include "dbg/x_dbg.h"
#endif
//#include "x_cli.h"
#include "bt_mw_common.h"
#include "bluetooth_sync.h"
#include "c_bt_mw_gap.h"
#include "bt_mw_gap.h"
#include "bt_mw_manager.h"
#if ENABLE_A2DP_SINK
#include <hardware/bt_audio_track.h>
#endif
#include "c_bt_mw_manager.h"

#include "bt_mw_gap_cli.h"
#if ENABLE_AVRCP_PROFILE
#include "bt_mw_avrcp_cli.h"
#endif
//#if ENABLE_GATT_PROFILE
//#include "bt_mw_gatt_cli.h"
//#endif
#if ENABLE_HID_PROFILE_H
#include "bt_mw_hid_cli.h"
#endif
#if (ENABLE_A2DP_SRC || ENABLE_A2DP_SINK)
#include "bt_mw_a2dp_cli.h"
#endif
#if ENABLE_SPP_PROFILE
#include "bt_mw_spp_cli.h"
#endif


/*-----------------------------------------------------------------------------
                    variable declarations
 -----------------------------------------------------------------------------*/
extern INT32 g_bt_debug_level;

UINT32 bt_mw_log_switch = 0xffff;
UINT32 bt_ext_log_switch = 0xffff;

CHAR* print_the_event(BTEvent bt_event)
{
    switch (bt_event)
    {
    case BT_OFF_MODE:
        return "BT_OFF_MODE";
        break;
    case BT_CONNECT_MODE:
        return "BT_CONNECT_MODE";
        break;
    case BT_PAIRING_MODE:
        return "BT_PAIRING_MODE";
        break;
    case BT_PLAYING_MODE:
        return "BT_PLAYING_MODE";
        break;
    case BT_EXIT:
        return "BT_EXIT";
        break;
    case BT_GAP_STATE_BONDED:
        return "BT_GAP_STATE_BONDED";
        break;
    case BT_PAIRING_TIMEOUT:
        return "BT_PAIRING_TIMEOUT";
        break;
    case BT_CONNECT_SUCCESS:
        return "BT_CONNECT_SUCCESS";
        break;
    case BT_CONNECT_MISSING:
        return "BT_CONNECT_MISSING";
        break;
    case BT_DISCONNECTED:
        return "BT_DISCONNECTED";
        break;
    case BT_CONNECT_TIMEOUT:
        return "BT_DISCONNECTED";
        break;
    case BT_2ND_CONNECT_COMMING:
        return "BT_2ND_CONNECT_COMMING";
        break;
    case BT_2ND_CONNECT_FAIL:
        return "BT_2ND_CONNECT_FAIL";
        break;
    case BT_GET_DEV_HISTORY_DONE:
        return "BT_GET_DEV_HISTORY_DONE";
        break;
    case BT_SUPPORT_SCMS_T:
        return "BT_SUPPORT_SCMS_T";
        break;
    case BT_AVRCP_CONNECTION_SUCCESS:
        return "BT_SUPPORT_SCMS_T";
        break;
    case BT_A2DP_CONNECTION_SUCCESS:
        return "BT_A2DP_CONNECTION_SUCCESS";
        break;
    case BT_SUPPORT_AVRCP_1_3:
        return "BT_SUPPORT_AVRCP_1_3";
        break;
    case BT_NOT_SUPPORT_AVRCP_1_3:
        return "BT_NOT_SUPPORT_AVRCP_1_3";
        break;
    case BT_REGISTER_PS_SUCCESS:
        return "BT_REGISTER_PS_SUCCESS";
        break;
    case BT_REGISTER_PS_FAIL:
        return "BT_REGISTER_PS_FAIL";
        break;
    case BT_CONNECT_COMING:
        return "BT_CONNECT_COMING";
        break;
    case BT_GET_CODEC_INFORMATION_SUCCESS:
        return "BT_GET_CODEC_INFORMATION_SUCCESS";
        break;
    case BT_A2DP_UP_SAMPLE_2FS:
        return "BT_A2DP_UP_SAMPLE_2FS";
        break;
    case BT_HIDD_CONNECTION_SUCCESS:
        return "BT_HIDD_CONNECTION_SUCCESS";
        break;
    case BT_HIDD_CONNECTION_FAIL:
        return "BT_HIDD_CONNECTION_FAIL";
        break;
    case BT_HIDD_DISCONNECTION_SUCCESS:
        return "BT_HIDD_DISCONNECTION_SUCCESS";
        break;
    case BT_HIDD_REJECT_3ND_CONNECTION:
        return "BT_HIDD_REJECT_3ND_CONNECTION";
        break;
    default:
        break;
    }

    return "";
}

VOID BtAppEventCallback(BTEvent bt_event)
{
    FUNC_ENTRY;
    BT_DBG_NORMAL(BT_DEBUG_COMM, "bt_event is: %s", print_the_event(bt_event));

    BT_TARGET_DEV_LIST pt_device_list;
    memset(&pt_device_list,0,sizeof(BT_TARGET_DEV_LIST));

    // need a sample state machine here
    switch (bt_event)
    {
    case BT_GET_CODEC_INFORMATION_SUCCESS:
        if (get_bt_a2dp_connect())
        {
            //c_btm_btstart("PLAYING",300);
            set_bt_mode(BT_MODE_PLAYING);
        }
        break;
    case BT_CONNECT_SUCCESS:
        break;
    case BT_DISCONNECTED:
        //c_btm_btstart("PAIRING",300);
        //set_bt_mode(BT_MODE_PAIRING);
        //c_btm_get_paired_dev_list(&pt_device_list);
        c_btm_set_connectable_and_discoverable(TRUE,TRUE);
        break;
    case BT_EXIT:
        break;
    default:
        break;
    }
}

VOID BtAppInquiryResponseCallback(BT_GAP_SCAN_RESULT* pt_result)
{
    FUNC_ENTRY;
    BT_DBG_INFO(BT_DEBUG_COMM, "name:%s",pt_result->name);
    BT_DBG_INFO(BT_DEBUG_COMM, "cod:0x%2lx",(UINT32)pt_result->cod);
    BT_DBG_INFO(BT_DEBUG_COMM, "isPaired:%d",pt_result->isPaired);
    BT_DBG_INFO(BT_DEBUG_COMM, "bdAddr:%s",pt_result->bdAddr);
    BT_DBG_INFO(BT_DEBUG_COMM, "service:%ld",(long)pt_result->service);
    BT_DBG_INFO(BT_DEBUG_COMM, "rssi:%d",pt_result->rssi);
}

#if ENABLE_AVRCP_PROFILE
VOID BtAppPlayStateCallback(BTPlayState bt_play_state)
{
    FUNC_ENTRY;
    BT_DBG_NORMAL(BT_DEBUG_COMM, "bt_play_state is: %d", bt_play_state);
}

VOID BtAppVolumeCallback(BTVolType bt_volume)
{
    FUNC_ENTRY;
    BT_DBG_NORMAL(BT_DEBUG_COMM, "bt_volume is: %s", bt_volume?"DOWN":"UP");
}

VOID BtAppTrackChangeCallback(UINT32 i4_track_index)
{
    FUNC_ENTRY;
    BT_DBG_NORMAL(BT_DEBUG_COMM, "i4_track_index is: %ld", (long)i4_track_index);
}

VOID BtAppMediaStatusCallback(BTMediaPlayState bt_status)
{
    FUNC_ENTRY;
    BT_DBG_NORMAL(BT_DEBUG_COMM, "media play status is: %d", bt_status);
}

VOID BtAppAbsoluteVolumeCallback(BTAbsoluteVolumeType flag, UINT8 *bt_absolute_volume)
{
    FUNC_ENTRY;
}

VOID BtAppAVRCPCMDCallback(BTCmdType bt_cmd_type, UINT8 press)
{
    FUNC_ENTRY;
}

VOID BtAppGetPlayStatusCallback(BTPlayState *bt_playstatus)
{
    FUNC_ENTRY;
}

#endif

#if ENABLE_SPP_PROFILE
VOID BtSppCallback(BT_SPP_CBK_STRUCT *pt_spp_struct)
{
    FUNC_ENTRY;
}

#endif
#if ENABLE_HID_PROFILE_H
VOID BtHidCallback(BT_HID_CBK_STRUCT *pt_hid_struct)
{
    FUNC_ENTRY;
}

#endif


#if (ENABLE_A2DP_SRC || ENABLE_A2DP_SINK)
VOID BtAppUploadInfoCallback(BT_A2DP_UPLOAD_INFO* pt_upload_info, BOOL fg_only_check_mute)
{
    FUNC_ENTRY;
}

VOID BtAppPlaybackCtrlCallback(BTPlayState bt_playback_cmd)
{
    FUNC_ENTRY;
}
#endif


/*-----------------------------------------------------------------------------
                    private methods implementations
 -----------------------------------------------------------------------------*/

INT32 _bluetooth_set_dbg_level_cli (INT32 argc, const CHAR **argv)
{
    INT32 mw_log_level = BT_DEBUG_LEVEL_NORMAL;
    if (argc != 2)
    {
        BT_DBG_ERROR(BT_DEBUG_COMM,"Two parameter is required!");
        BT_DBG_ERROR(BT_DEBUG_COMM,"Usage :");
        BT_DBG_ERROR(BT_DEBUG_COMM,"dbg_level [0~5] [0~5]");
        BT_DBG_ERROR(BT_DEBUG_COMM,"paramter 1 is mw log level 2 is external log level");
        return -1;
    }
    mw_log_level = atoi(argv[1]);
    g_bt_debug_level = mw_log_level;
    return 0;
}

/*-----------------------------------------------------------------------------
                    private methods implementations
 -----------------------------------------------------------------------------*/

INT32 _bluetooth_enable_dbg_log_cli (INT32 argc, const CHAR **argv)
{
    INT32 module_index;
    INT32 mw_log_switch = bt_mw_log_switch;
    INT32 ext_log_switch = bt_ext_log_switch;
    if (4 != argc)
    {
        BT_DBG_ERROR(BT_DEBUG_COMM,"input error");
    }
    if (BT_LOG_ENABLE == atoi(argv[2]))
    {
        for (module_index = 0; module_index < sizeof(BT_Debug_Module_Name)/sizeof(CHAR*); module_index++)
        {
            if (strcmp(BT_Debug_Module_Name[module_index],argv[1]) == 0)
            {
                mw_log_switch |= 1 << module_index;
            }
        }
    }
    else
    {
        for (module_index = 0; module_index < sizeof(BT_Debug_Module_Name)/sizeof(CHAR*); module_index++)
        {
            if (strcmp(BT_Debug_Module_Name[module_index],argv[1]) == 0)
            {
                mw_log_switch &= ~(1 << module_index);
            }
        }
    }
    if (BT_LOG_ENABLE == atoi(argv[3]))
    {
        for (module_index = 0; module_index < sizeof(BT_Debug_Module_Name)/sizeof(CHAR*); module_index++)
        {
            if (strcmp(BT_Debug_Module_Name[module_index],argv[1]) == 0)
            {
                ext_log_switch |= 1 << module_index;
            }
        }
    }
    else
    {
        for (module_index = 0; module_index < sizeof(BT_Debug_Module_Name)/sizeof(CHAR*); module_index++)
        {
            if (strcmp(BT_Debug_Module_Name[module_index],argv[1]) == 0)
            {
                ext_log_switch &= ~(1 << module_index);
            }
        }
    }
    bt_mw_log_switch = mw_log_switch;
    bt_ext_log_switch = ext_log_switch;
    bluetooth_mgr_Enable_external_log(ext_log_switch);
    BT_DBG_NORMAL(BT_DEBUG_COMM, "mw_log is %ld \n external_log is %ld",(long)mw_log_switch,(long)ext_log_switch);
    return 0;
}

VOID otherInitAfterBtInit(VOID)
{
    c_btm_bluetooth_on_off(TRUE);
    /*x_thread_delay(100);*/
    usleep(100*1000);
    c_btm_source_sink_on_off(TRUE,TRUE);
    /*x_thread_delay(100);*/
    usleep(100*1000);
    c_btm_set_local_name("TV_TURNKEY_1501");
    /*x_thread_delay(100);*/
    usleep(100*1000);
    /*x_thread_delay(100);*/
    usleep(100*1000);
    c_btm_set_connectable_and_discoverable(TRUE,TRUE);
    /*x_thread_delay(100);*/
    usleep(100*1000);
}

/*-----------------------------------------------------------------------------
 * Name: _bluetooth_init_cli
 *
 * Description: -
 *
 * Inputs:  -
 *
 * Outputs: -
 *
 * Returns: -
 *
 ----------------------------------------------------------------------------*/
INT32 _bluetooth_init_cli (INT32 argc, const CHAR **argv)
{
    BT_APP_CB_FUNC app_func;
    memset(&app_func, 0x0, sizeof(BT_APP_CB_FUNC));
    app_func.bt_event_cb = BtAppEventCallback;
#if ENABLE_AVRCP_PROFILE
    app_func.bt_play_change_cb = BtAppPlayStateCallback;
    app_func.bt_volume_change_cb = BtAppVolumeCallback;
    app_func.bt_track_change_cb = BtAppTrackChangeCallback;
    app_func.bt_media_playstatus_cb = BtAppMediaStatusCallback;
    app_func.bt_absolute_volume_cb = BtAppAbsoluteVolumeCallback;
    app_func.bt_avrcp_cmd_cb = BtAppAVRCPCMDCallback;
    app_func.bt_get_play_status_cb = BtAppGetPlayStatusCallback;
#endif
#if ENABLE_SPP_PROFILE
    app_func.bt_spp_cb = BtSppCallback;
#endif
#if (ENABLE_A2DP_SRC || ENABLE_A2DP_SINK)
    app_func.bt_upload_info_cb = BtAppUploadInfoCallback;
    app_func.bt_playback_ctrl_cb = BtAppPlaybackCtrlCallback;
#endif
#if ENABLE_HID_PROFILE_H
    app_func.bt_hid_cb = BtHidCallback;
#endif


    if (argc != 2 )
    {
        BT_DBG_ERROR(BT_DEBUG_COMM, "please attach name after commond just append one string");
        return -1;
    }
    if (0 == strcmp("1" , argv[1]))
    {
        BT_DBG_NOTICE(BT_DEBUG_COMM, "normal init");
        c_btm_bt_base_init(&app_func);
    }
    else
    {
        BT_DBG_NOTICE(BT_DEBUG_COMM, "init in putty");
        c_btm_bt_base_init(&app_func);
    }
    otherInitAfterBtInit();
    return 0;
}

INT32 _bluetooth_uninit_cli (INT32 argc, const CHAR **argv)
{
    return bluetooth_mgr_btuninit();
}

INT32 _bluetooth_mode_cli (INT32 argc, const CHAR **argv)
{
    UINT32 u4_overtime = 300;
    //CHAR *ps_addr;
    if (argc != 2 )
    {
        BT_DBG_ERROR(BT_DEBUG_COMM, "please attach mode name after commond just append one string");
        return -1;
    }

    //ps_addr = strdup(argv[2]);

    if (0 == strcmp("PAIRING" , argv[1]))
    {
        BT_DBG_NOTICE(BT_DEBUG_COMM, "PAIRING MODE");
        c_btm_btstart(argv[1], u4_overtime);
    }
    else if (0 == strcmp("CONNECTING" , argv[1]))
    {
        BT_DBG_NOTICE(BT_DEBUG_COMM, "CONNECTING MODE");
        c_btm_btstart(argv[1], u4_overtime);
    }
    else if (0 == strcmp("PLAYING" , argv[1]))
    {
        BT_DBG_NOTICE(BT_DEBUG_COMM, "PLAYING MODE");
        c_btm_btstart(argv[1], u4_overtime);
    }
    else if (0 == strcmp("OFF" , argv[1]))
    {
        BT_DBG_NOTICE(BT_DEBUG_COMM, "OFF MODE");
        c_btm_btstart(argv[1], u4_overtime);
    }
    else
    {
        BT_DBG_NOTICE(BT_DEBUG_COMM, "input error, please input again,eg. mode PAIRING");
    }
    return 0;
}


INT32 _bluetooth_show_current_mode_cli(INT32 argc, const CHAR **argv)
{
    if (BT_MODE_CLOSE == get_bt_mode())
    {
        BT_DBG_NORMAL(BT_DEBUG_COMM, "current BT mode is :close");
    }
    else if (BT_MODE_PAIRING == get_bt_mode())
    {
        BT_DBG_NORMAL(BT_DEBUG_COMM, "current BT mode is :pairing");
    }
    else if (BT_MODE_CONNECTING == get_bt_mode())
    {
        BT_DBG_NORMAL(BT_DEBUG_COMM, "current BT mode is :connecting");
    }
    else if (BT_MODE_PLAYING == get_bt_mode())
    {
        BT_DBG_NORMAL(BT_DEBUG_COMM, "current BT mode is :playing");
    }

    return 0;
}

/*-----------------------------------------------------------------------------
 * Name: _bluetooth_set_power_cli
 *
 * Description: -
 *
 * Inputs:  -
 *
 * Outputs: -
 *
 * Returns: -
 *
 ----------------------------------------------------------------------------*/

INT32 _bluetooth_set_power_cli(INT32 argc, const CHAR **argv)
{
    if (2 == argc)
    {
        if (0 == strcmp("0" , argv[1]))
        {
            return c_btm_bluetooth_on_off(FALSE);
        }
        else
        {
            return c_btm_bluetooth_on_off(TRUE);
        }
    }
    else
    {
        BT_DBG_ERROR(BT_DEBUG_COMM,"0 means off, other integer:on");
        return -1;
    }

}



/*-----------------------------------------------------------------------------
 * Name: _bluetooth_remove_paired_dev_cli
 *
 * Description: -
 *
 * Inputs:  -
 *
 * Outputs: -
 *
 * Returns: -
 *
 ----------------------------------------------------------------------------*/

INT32 _bluetooth_stop_cli (INT32 argc, const CHAR **argv)
{
    return c_btm_stop(FALSE, FALSE);
}

INT32 _bluetooth_inquiry_cli (INT32 argc, const CHAR **argv)
{
    FUNC_ENTRY;
    BT_GAP_SCAN_RESULT_S *pt_scan_rst = NULL;
    UINT32 ui4_inqDuration = 10;
    if (1 < argc)
    {
        ui4_inqDuration = atol(argv[1]);
        BT_DBG_NOTICE(BT_DEBUG_COMM, "inquiry %ld s", (long)ui4_inqDuration);
    }
    pt_scan_rst = (BT_GAP_SCAN_RESULT_S *)malloc(sizeof(BT_GAP_SCAN_RESULT_S));
    if (NULL == pt_scan_rst)
    {
        BT_DBG_ERROR(BT_DEBUG_COMM, "strdup failed");
        return -1;
    }
    memset(pt_scan_rst, 0x0, sizeof(BT_GAP_SCAN_RESULT_S));
    c_btm_start_inquiry_scan(0, &BtAppInquiryResponseCallback);
    free(pt_scan_rst);
    return 0;

}


INT32 _bluetooth_get_devlist_cli(INT32 argc, const CHAR **argv)
{
    FUNC_ENTRY;
    BT_TARGET_DEV_LIST pt_device_list;
    memset(&pt_device_list, 0, sizeof(BT_TARGET_DEV_LIST));
#if ENABLE_A2DP_SINK
    c_btm_get_paired_dev_list(&pt_device_list);
#endif
    return 0;

}

INT32 _bluetooth_clear_devinfo_cli (INT32 argc, const CHAR **argv)
{
    FUNC_ENTRY;

    c_btm_clear_dev_info();
    return 0;

}


INT32 _bluetooth_del_paired_dev_one_cli (INT32 argc, const CHAR **argv)
{
    FUNC_ENTRY;
    BT_TARGET_DEV_INFO pt_device_info;
    memset(&pt_device_info, 0, sizeof(BT_TARGET_DEV_INFO));
    if (1 < argc)
    {
        if (17 > strlen(argv[1]))
        {
            BT_DBG_NOTICE(BT_DEBUG_COMM, "mac length should be 17");
            return -1;
        }
        strncpy(pt_device_info.bdAddr, argv[1],17);
        BT_DBG_NOTICE(BT_DEBUG_COMM, "pt_device_info.bdAddr: %s", pt_device_info.bdAddr);
    }
    c_btm_del_paired_av_dev_one(&pt_device_info);
    return 0;

}

INT32 _bluetooth_del_paired_dev_all_cli (INT32 argc, const CHAR **argv)
{
    FUNC_ENTRY;

    c_btm_del_paired_av_dev_all();
    return 0;

}

INT32 _bluetooth_sdp_search_cli(INT32 argc, const CHAR **argv)
{
    CHAR *szaddress;
    if (2 == argc)
    {
        szaddress = strdup(argv[1]);
        if (NULL == szaddress)
        {
            BT_DBG_ERROR(BT_DEBUG_COMM, "strdup failed");
            return -1;
        }
        BT_DBG_INFO(BT_DEBUG_COMM,"sdp mac:%s", szaddress);
        bluetooth_sdp_request(szaddress);
        free(szaddress);
    }
    else
    {
        BT_DBG_ERROR(BT_DEBUG_COMM,"please input mac address");
        return -1;
    }
    return 0;
}


#if CONFIG_MW_SUPPORT_BT_USB_LIB
INT32 _bluetooth_load_usb_lib_cli(INT32 argc, const CHAR **argv)
{
    INT32 fg_on;
    if (1 < argc)
    {
        fg_on = atoi(argv[1]);
        if (0 < fg_on)
        {
            bluetooth_mgr_load_usb_lib(TRUE);
        }
        else
        {
            bluetooth_mgr_load_usb_lib(FALSE);

        }
    }
    else
    {
        BT_DBG_ERROR(BT_DEBUG_COMM, "input error,cp_so [1(copy)|0(get usb path)]");
    }

    return 0;
}


INT32 _bluetooth_rollback_usb_lib_cli(INT32 argc, const CHAR **argv)
{

    bluetooth_mgr_rollback_usb_lib();
    return 0;
}
#endif

/*-----------------------------------------------------------------------------
                    public methods implementations
 -----------------------------------------------------------------------------*/

#if (ENABLE_A2DP_SINK || ENABLE_A2DP_SRC)
static CLI_EXEC_T at_bluetooth_a2dp_cmd_tbl[] =
{
    {
        "set_aud_log_lvl",
        "set_aud_log_lvl",
        _bluetooth_a2dp_set_audio_log_lvl_cli,
        NULL,
        "set audio hw log level, eg: set_aud_log_lvl [0~6]",
        CLI_GUEST
    },
    {
        "get_current_codec",
        "get_current_codec",
        _bluetooth_get_codec_type_cli,
        NULL,
        "get current codec type",
        CLI_GUEST
    },
    {
        "set_codec",
        "set_codec",
        _bluetooth_set_codec_type_cli,
        NULL,
        "get codec type, scodec [sbc|aac|ldac]",
        CLI_GUEST
    },
    {
        "codec_type",
        "ctype",
        _bluetooth_get_audio_codec_type_cli,
        NULL,
        "get codec type",
        CLI_GUEST
    },
    {
        "cc",
        "audio_connect",
        _bluetooth_audio_connect_cli,
        NULL,
        "cc [MAC] [a2dp|avrcp]",
        CLI_GUEST
    },
    {
        "dc",
        "audio_disconnect",
        _bluetooth_audio_disconnect_cli,
        NULL,
        "dc [MAC] [a2dp|avrcp]",
        CLI_GUEST
    },
    {
        "a2dp",
        "a2dp",
        _bluetooth_a2dp_cmd_cli,
        NULL,
        "a2dp [MAC] [start|suspend|abort|reconfig][sample rate][channel]",
        CLI_GUEST
    },
    {
        "a2dp_src_sink",
        "a2dp_src_sink",
        _bluetooth_a2dp_src_sink_enable_cli,
        NULL,
        "enable a2dp role [src:1|0] [sink:1|0]",
        CLI_GUEST
    },
#if ENABLE_BUFFER_CONTROL
    {
        "sbc_drift",
        "sbc_drift",
        _bluetooth_enable_SBC_AAC_drift_compensation_cli,
        NULL,
        "sbc_drift [1|0]:set SBC/AAC drift compensation [ON/OFF],1:on,0:off",
        CLI_GUEST
    },
    {
        "detect",
        "detect",
        _bluetooth_enable_detection_cli,
        NULL,
        "detect [0|1]",
        CLI_GUEST
    },
    {
        "ncomp",
        "ncomp",
        _bluetooth_set_ncomp_cli,
        NULL,
        "ncomp value (-96,96)",
        CLI_GUEST
    },
    {
        "set_drift",
        "set_drift",
        _bluetooth_set_drift_cli,
        NULL,
        "set_drift [integer]",
        CLI_GUEST
    },
    {
        "set_g1",
        "set_g1",
        _bluetooth_set_g1_cli,
        NULL,
        "set_g1 [integer] [integer]",
        CLI_GUEST
    },
    {
        "set_g2",
        "set_g2",
        _bluetooth_set_g2_cli,
        NULL,
        "set_g2 [integer] [integer]",
        CLI_GUEST
    },
#if ENABLE_LDAC_CODEC
    {
        "ldac_switch",
        "ldac_switch",
        _bluetooth_ldac_frame_status_log_switch_cli,
        NULL,
        "ldac_switch [1|0]:for ldac compensation frame status log,1:on,0:off",
        CLI_GUEST
    },
    {
        "ldac_repeat",
        "ldac_repeat",
        _bluetooth_repeat_LDAC_frame_cli,
        NULL,
        "ldac_repeat [integer](1~15)",
        CLI_GUEST
    },
#endif  /* ENABLE_LDAC_CODEC */
#endif  /* ENABLE_BUFFER_CONTROL */
#if ENABLE_A2DP_SINK
    {
        "areset",
        "audio_reset",
        _bluetooth_audio_reset_cli,
        NULL,
        "audio reset",
        CLI_GUEST
    },
    {
        "playback",
        "pb",
        _bluetooth_pb_cmd_cli,
        NULL,
        "pb [init|uninit|play|pause|stop]",
        CLI_GUEST
    },
    {
        "dec_dump_enable",
        "dec_dump_enable",
        _bluetooth_dec_dump_data_enable_cli,
        NULL,
        "dump a2dp file to path, eg: dec_dump_enable [1|0]",
        CLI_GUEST
    },
    {
        "optimise_latency",
        "optimise_latency",
        _bluetooth_optimise_latency_cli,
        NULL,
        "optimise_latency [1|0](1:ON 2:OFF)",
        CLI_GUEST
    },
#if ENABLE_INTERNAL_SUSPEND
    {
        "set_in_time",
        "set_in_inter_timegap",
        _bluetooth_set_in_inter_timegap_cli,
        NULL,
        "set_in_time [integer] *10ms",
        CLI_GUEST
    },
    {
        "set_out_time",
        "set_out_inter_timegap",
        _bluetooth_set_out_inter_timegap_cli,
        NULL,
        "set_out_time [integer] *10ms",
        CLI_GUEST
    },
#endif

#endif /* ENABLE_A2DP_SINK */
#if ENABLE_A2DP_SRC
    {
        "set_aac_bitrate",
        "set_aac_bitrate",
        _bluetooth_a2dp_set_aac_bitrate_cli,
        NULL,
        "set aac encoder bitrate, eg: set_aac_bitrate [integter]",
        CLI_GUEST
    },
    {
        "enc_dump_enable",
        "enc_dump_enable",
        _bluetooth_enc_dump_data_enable_cli,
        NULL,
        "dump enc file to path, eg: enc_dump_enable [1|0]",
        CLI_GUEST
    },
    {
        "a2dp_send_data",
        "a2dp_send_data",
        _bluetooth_a2dp_send_data_cli,
        NULL,
        "a2dp_send_data [stream_handle][input data path]",
        CLI_GUEST
    },
#if ENABLE_MTK_TV_PLATFORM
    {
        "mute_speaker",
        "mute_speaker",
        _bluetooth_mute_speaker_cli,
        NULL,
        "mute_speaker [0|1]",
        CLI_GUEST
    },
#endif
#if ENABLE_TRACK_EXCHANGE
    {
        "set_track_change_value",
        "set_track_change_value",
        _bluetooth_set_track_exchange_delay__cli,
        NULL,
        "set_track_change_value [integer]",
        CLI_GUEST
    },
#endif
#endif /* ENABLE_A2DP_SRC */
    {
        "set_avdtp_start_delay",
        "set_avdtp_start_delay",
        _bluetooth_set_avdtp_start_delay__cli,
        NULL,
        "set_avdtp_start_delay [integer] ms",
        CLI_GUEST
    },

#if ENABLE_LDAC_CODEC
    {
        "a2dp_ldac_enable",
        "a2dp_ldac_enable",
        _bluetooth_a2dp_ldac_enable_cli,
        NULL,
        "a2dp_ldac_enable [1:enable|0:disable]",
        CLI_GUEST
    },
#if ENABLE_A2DP_SINK
    {
        "a2dp_start_ldac",
        "a2dp_start_ldac",
        _bluetooth_a2dp_test_ldac_cli,
        NULL,
        "a2dp_start_ldac [/mnt/sda1/test.ldac]",
        CLI_GUEST
    },
    {
        "a2dp_decode_ldac",
        "a2dp_decode_ldac",
        _bluetooth_a2dp_decode_ldac_cli,
        NULL,
        "a2dp_decode_ldac [channel_idx] [sample_rate] [file_full_name]",
        CLI_GUEST
    },
#endif
#if ENABLE_A2DP_SINK
    {
        "ldac_quality_threshold",
        "ldac_quality_threshold",
        _bluetooth_ldac_set_quality_threshold,
        NULL,
        "ldac_quality_threshold [good:INT32][bad:INT32]",
        CLI_GUEST
    },
#endif
    {
        "a2dp_set_qmode",
        "a2dp_set_qmode",
        _bluetooth_a2dp_set_qmode,
        NULL,
        "a2dp_set_qmode [0:H|1:S|2:M]",
        CLI_GUEST
    },
    {
        "a2dp_set_frame_head",
        "a2dp_set_frame_head",
        _bluetooth_a2dp_set_frameHeader,
        NULL,
        "a2dp_set_frame_head [integer]",
        CLI_GUEST
    },
#endif
#ifdef ENABLE_AAC_LOCAL_TEST
#if ENABLE_AAC_CODEC
    {
        "local_aac_enc",
        "local_aac_enc",
        _bluetooth_local_thread_aac_enc,
        NULL,
        "local_aac_enc [in file][out file][AOT][TRANSMUX][BITRATE][sample rate][channel num]",
        CLI_GUEST
    },
#endif
#endif /* ENABLE_AAC_LOCAL_TEST */
    {
        "codec",
        "codec",
        _bluetooth_a2dp_codec_enable_cli,
        NULL,
#if ENABLE_LDAC_CODEC
#if ENABLE_APTX_CODEC
        "codec [aac|ldac|aptx] [1|0]",
#else
        "codec [aac|ldac] [1|0]",
#endif
#else
#if ENABLE_APTX_CODEC
        "codec [aac|aptx] [1|0]",
#else
        "codec [aac] [1|0]",
#endif
#endif
        CLI_GUEST
    },
#if ENABLE_SYMPHONY
    {
        "get_enabled_codec",
        "get_enabled_codec",
        _bluetooth_a2dp_enabled_codec_cli,
        NULL,
        "print enabled codec type",
        CLI_GUEST
    },
#endif
    END_OF_CLI_CMD_TBL
};
#endif /* (ENABLE_A2DP_SINK || ENABLE_A2DP_SRC) */

#if ENABLE_AVRCP_PROFILE
static CLI_EXEC_T at_bluetooth_avrcp_cmd_tbl[] =
{
    {
        "avrcp",
        "avrcp",
        _bluetooth_avrcp_passthrough_cmd_cli,
        NULL,
        "avrcp [play|pause|stop|fwd|bwd|ffwd|rwd|volume_up|volume_down] [press|release|auto]",
        CLI_GUEST
    },
    {
        "avrcp_get",
        "avrcp_get",
        _bluetooth_avrcp_get_info_cli,
        NULL,
        "avrcp_get [ps|mi]",
        CLI_GUEST
    },
    {
        "register_notification",
        "register_notification",
        _bluetooth_avrcp_register_notification_cli,
        NULL,
        "register_notification [ps|tc]",
        CLI_GUEST
    },
    {
        "get_capabilities",
        "get_capabilities",
        _bluetooth_avrcp_get_capabilities_cli,
        NULL,
        "get_capabilities [companyid|events]",
        CLI_GUEST
    },
    {
        "track_change_rsp",
        "track_change_rsp",
        _bluetooth_avrcp_track_change_enable_cli,
        NULL,
        "track_change_rsp [integer]",
        CLI_GUEST
    },
    {
        "large_element_rsp",
        "large_element_rsp",
        _bluetooth_avrcp_large_element_enable_cli,
        NULL,
        "large_element_rsp [0|1]",
        CLI_GUEST
    },
    {
        "pts_enable",
        "pts_enable",
        _bluetooth_avrcp_pts_enable_cli,
        NULL,
        "pts_enable [0|1]",
        CLI_GUEST
    },
    {
        "absolute_volume_rsp",
        "absolute_volume_rsp",
        _bluetooth_avrcp_absolute_volume_rsp_cli,
        NULL,
        "absolute_volume_rsp [integer]",
        CLI_GUEST
    },
    {
        "avrcp_delay",
        "avrcp_delay",
        _bluetooth_avrcp_avrcp_delay_cli,
        NULL,
        "avrcp_delay [integer]",
        CLI_GUEST
    },
    END_OF_CLI_CMD_TBL
};
#endif /* ENABLE_AVRCP_PROFILE */

#if (ENABLE_HID_PROFILE_H || ENABLE_HID_PROFILE_D)
static CLI_EXEC_T at_bluetooth_hid_cmd_tbl[] =
{
#if ENABLE_HID_PROFILE_H
    {
        "hid_cc",
        "hid_connect",
        _bluetooth_hid_connect_cli,
        NULL,
        "eg. hid_cc address. hid connect with remote device",
        CLI_GUEST
    },
    {
        "hid_dc",
        "hid_disconnect",
        _bluetooth_hid_disconnect_cli,
        NULL,
        "eg. hid_dc address. hid disconnect with remote device",
        CLI_GUEST
    },
    {
        "get_input_report",
        "get_input_report",
        _bluetooth_hid_get_input_report_cli,
        NULL,
        "get_input_report [MAC_ADDR][report_id][max_buffer_size]",
        CLI_GUEST
    },
    {
        "get_output_report",
        "get_output_report",
        _bluetooth_hid_get_output_report_cli,
        NULL,
        "get_output_report [MAC_ADDR][report_id][max_buffer_size]",
        CLI_GUEST
    },
    {
        "set_input_report",
        "set_input_report",
        _bluetooth_hid_set_input_report_cli,
        NULL,
        "set_input_report [MAC_ADDR][report_data(HEX)]",
        CLI_GUEST
    },
    {
        "set_output_report",
        "set_output_report",
        _bluetooth_hid_set_output_report_cli,
        NULL,
        "set_output_report [MAC_ADDR][report_data(HEX)]",
        CLI_GUEST
    },
    {
        "set_feature_report",
        "set_feature_report",
        _bluetooth_hid_set_feature_report_cli,
        NULL,
        "set_feature_report [MAC] [report_data]",
        CLI_GUEST
    },
    {
        "get_feature_report",
        "get_feature_report",
        _bluetooth_hid_get_feature_report_cli,
        NULL,
        "get_feature_report [MAC] [report_id] [buffer_size]",
        CLI_GUEST
    },
    {
        "set_protocol",
        "set_protocol",
        _bluetooth_hid_set_protocol_cli,
        NULL,
        "set_protocol [MAC_ADDR][1:BOOT 0:REPORT]",
        CLI_GUEST
    },
    {
        "get_protocol",
        "get_protocol",
        _bluetooth_hid_get_protocol_cli,
        NULL,
        "get_protocol [MAC_ADDR]",
        CLI_GUEST
    },
    {
        "virtual_up",
        "virtual_up",
        _bluetooth_hid_virtual_unplug_cli,
        NULL,
        "virtual unplug [MAC_ADDR]",
        CLI_GUEST
    },
    {
        "send_data",
        "send_data",
        _bluetooth_hid_send_data_cli,
        NULL,
        "send data [MAC_ADDR][Data]",
        CLI_GUEST
    },
#if 0
    {
        "send_control",
        "send_control",
        _bluetooth_hid_send_control_cli,
        NULL,
        "send_control [MAC_ADDR][3:SUSPEND 4:EXIT_SUSPEND 5:VIRTUAL_CABLE_UNPLUG]",
        CLI_GUEST
    },
#endif
    {
        "scan_hid_a2dp",
        "scan_hid_a2dp",
        _bluetooth_scan_hid_a2dp_cli,
        NULL,
        "scan_hid_a2dp [(1 0):A2DP SINK&&keyboard&&mouse (0 1):]",
        CLI_GUEST
    },
#endif
#if ENABLE_HID_PROFILE_D
    {
        "hidd_cc",
        "hidd_connect",
        _bluetooth_hidd_connect_cli,
        NULL,
        "eg. hidd_cc address. hidd connect with remote device",
        CLI_GUEST
    },
    {
        "hidd_dc",
        "hidd_disconnect",
        _bluetooth_hidd_disconnect_cli,
        NULL,
        "eg. hidd_dc address. hidd disconnect with remote device",
        CLI_GUEST
    },
    {
        "hidd_send_keyboard_data",
        "hidd_send_keyboard_data",
        _bluetooth_hidd_send_keyboard_data_cli,
        NULL,
        "eg. hidd_send_keyboard_data [report_data][data_len].",
        CLI_GUEST
    },
    {
        "hidd_send_mouse_data",
        "hidd_send_mouse_data",
        _bluetooth_hidd_send_mouse_data_cli,
        NULL,
        "eg. hidd_send_mouse_data [report_data][data_len]",
        CLI_GUEST
    },
    {
        "hidd_send_consumer_data",
        "hidd_send_consumer_data",
        _bluetooth_hidd_send_consumer_data_cli,
        NULL,
        "eg. hidd_send_consumer_data [report_data][data_len]",
        CLI_GUEST
    },
    {
        "hidd_send_data",
        "hidd_send_c_data",
        _bluetooth_hidd_send_data_cli,
        NULL,
        "eg. hidd_send_data [report_data][data_len]",
        CLI_GUEST
    },
#endif
    END_OF_CLI_CMD_TBL
};
#endif /* (ENABLE_HID_PROFILE_H || ENABLE_HID_PROFILE_D) */

#if ENABLE_SPP_PROFILE
static CLI_EXEC_T at_bluetooth_spp_cmd_tbl[] =
{
    {
        "spp_connect",
        "spp_connect",
        _bluetooth_bt_spp_connect_cli,
        NULL,
        "spp_connect [MAC ADDRESS][optional:00001101-0000-1000-8000-00805F9B34FB]",
        CLI_GUEST
    },
    {
        "spp_disconnect",
        "spp_disconnect",
        _bluetooth_bt_spp_disconnect_cli,
        NULL,
        "spp_disconnect [PortNum]",
        CLI_GUEST
    },
    {
        "spp_send_data",
        "spp_send_data",
        _bluetooth_bt_spp_send_data_cli,
        NULL,
        "spp_send_data [PortNum][STRING]",
        CLI_GUEST
    },
    {
        "spp_send_data_from_file",
        "spp_send_data_from_file",
        _bluetooth_bt_spp_send_data_fromfilet_cli,
        NULL,
        "spp_send_data_from_file [PortNum] [FILE_NAME]",
        CLI_GUEST
    },
    {
        "spp_enable_devb",
        "spp_enable_devb",
        _bluetooth_bt_spp_enable_devB_cli,
        NULL,
        "spp_enable_devb, enable SPP acceptor",
        CLI_GUEST
    },
    {
        "spp_disable_devb",
        "spp_disable_devb",
        _bluetooth_bt_spp_disable_devB_cli,
        NULL,
        "spp_disable_devb [PortNum], disable SPP acceptor",
        CLI_GUEST
    },
    END_OF_CLI_CMD_TBL
};
#endif /* ENABLE_SPP_PROFILE */

static CLI_EXEC_T at_bluetooth_mw_cmd_tbl[] =
{
    {
        "dbg_level",
        "dbg_level",
        _bluetooth_set_dbg_level_cli,
        NULL,
        "dbg_level [mw_log_level:0~5]",
        CLI_GUEST
    },
    {
        "dbg_log_enable",
        "dbg_log_enable",
        _bluetooth_enable_dbg_log_cli,
        NULL,
        "dbg_log_enable [[GAP,A2DP,AVRCP,HID,GATT,SPP]mw_log:0~1,external_log:0~1]",
        CLI_GUEST
    },
    {
        "gdev",
        "get_dev",
        _bluetooth_get_devlist_cli,
        NULL,
        "get all target device info",
        CLI_GUEST
    },
    {
        "bt_init",
        "init",
        _bluetooth_init_cli,
        NULL,
        "bluetooth init 1:on, 0:off",
        CLI_GUEST
    },
    {
        "bt_uninit",
        "uninit",
        _bluetooth_uninit_cli,
        NULL,
        "bluetooth uninit",
        CLI_GUEST
    },
    {
        "enter_a_mode",
        "mode",
        _bluetooth_mode_cli,
        NULL,
        "enter into some mode:PAIRING/CONNECTING/PLAYING/OFF: mac",
        CLI_GUEST
    },
    {
        "current_mode",
        "current_mode",
        _bluetooth_show_current_mode_cli,
        NULL,
        "usage:current_mode",
        CLI_GUEST
    },
    {
        "power_on",
        "po",
        _bluetooth_set_power_cli,
        NULL,
        "power_on local device, 0:off, 1:on",
        CLI_GUEST
    },
    {
        "stop",
        "st",
        _bluetooth_stop_cli,
        NULL,
        "bluetooth stop",
        CLI_GUEST
    },
    {
        "clear_local",
        "clear_local",
        _bluetooth_clear_devinfo_cli,
        NULL,
        "clear local device name and mac",
        CLI_GUEST
    },
    {
        "rm_one",
        "remove_one",
        _bluetooth_del_paired_dev_one_cli,
        NULL,
        "rm_one 11:22:33:44:55:66",
        CLI_GUEST
    },
    {
        "rm_all",
        "remove_all",
        _bluetooth_del_paired_dev_all_cli,
        NULL,
        "remove_all",
        CLI_GUEST
    },
#if CONFIG_MW_SUPPORT_BT_USB_LIB
    {
        "cp_so",
        "copy_so",
        _bluetooth_load_usb_lib_cli,
        NULL,
        "copy usb so/ko",
        CLI_GUEST
    },
    {
        "rb",
        "rollback",
        _bluetooth_rollback_usb_lib_cli,
        NULL,
        "rollback usb so/ko/bin",
        CLI_GUEST
    },
#endif
    {
        "inquiry_one",
        "inquiry_one",
        _bluetooth_inquiry_one_cli,
        NULL,
        "inquiry mac 10",
        CLI_GUEST
    },
    {
        "setname",
        "name",
        _bluetooth_set_name_cli,
        NULL,
        "bt set local dev name, name xxx",
        CLI_GUEST
    },
    {
        "get_local_dev_info",
        "info",
        _bluetooth_get_local_dev_info_cli,
        NULL,
        "info [local|update]",
        CLI_GUEST
    },
    {
        "get_rssi",
        "get_rssi",
        _bluetooth_get_rssi_cli,
        NULL,
        "get_rssi [MAC addr]",
        CLI_GUEST
    },
    {
        "scan",
        "s",
        _bluetooth_scan_cli,
        NULL,
        "Start device discovery",
        CLI_GUEST
    },
    {
        "stop_scan",
        "ss",
        _bluetooth_stop_scan_cli,
        NULL,
        "stop device discovery",
        CLI_GUEST
    },
    {
        "stop_inquiry",
        "stop_inquiry",
        _bluetooth_stop_inquiry_cli,
        NULL,
        "Stop device discovery",
        CLI_GUEST
    },
    {
        "pair",
        "p",
        _bluetooth_pair_cli,
        NULL,
        "Pair a remote device, p MAC",
        CLI_GUEST
    },
    {
        "unpair",
        "unpair",
        _bluetooth_remove_paired_dev_cli,
        NULL,
        "remove paired device, rp MAC",
        CLI_GUEST
    },
    {
        "set_conn_disc",
        "set_conn_disc",
        _bluetooth_set_connectable_and_discoverable_cli,
        NULL,
        "set device connectable and discoverable, set_conn_disc[1|0][1|0], 1:enable, 0: unable",
        CLI_GUEST
    },
    {
        "vslog2file",
        "vslog2file",
        _bluetooth_set_virtual_sniffer_cli,
        NULL,
        "eg. vslog2file [1|0]. save virtual sniffer log to /tmp/hci_sniffer_logXXX.log",
        CLI_GUEST
    },
    {
        "set_sniff_mode",
        "set_sniff_mode",
        NULL,
        NULL,
        "set sniff mode ([addr][1:START | 0:STOP ][Max Interval][Min Interval][Sniff Attempts][Sniff Timeout])",
        CLI_GUEST
    },
    {
        "send_hci",
        "send_hci",
        _bluetooth_send_hci,
        NULL,
        "send_hci [buffer]",
        CLI_GUEST
    },
    {
        "gap_pts_enable",
        "gap_pts_enable",
        _bluetooth_gap_pts_enable_cli,
        NULL,
        "gap_pts_enable [integer]",
        CLI_GUEST
    },
    {
        "set_afh",
        "set_afh",
        _bluetooth_set_afh_cli,
        NULL,
        "set_afh mask",
        CLI_GUEST
    },
#ifdef __MTK_LINUX_API__
    {
        "config_clear",
        "config_clear",
        _bluetooth_config_clear,
        NULL,
        "config_clear",
        CLI_GUEST
    },
    {
        "dump",
        "dump",
        _bluetooth_dump,
        NULL,
        "dump [mode] mode is (text) or (bin)",
        CLI_GUEST
    },
    {
        "interop_add",
        "interop_add",
        _bluetooth_interop_database_add,
        NULL,
        "interop_add [feature] [addr] [len]",
        CLI_GUEST
    },
    {
        "interop_clear",
        "interop_clear",
        _bluetooth_interop_database_clear,
        NULL,
        "interop_clear",
        CLI_GUEST
    },
    {
        "ble_pair",
        "ble_pair",
        _bluetooth_ble_pair,
        NULL,
        "ble_pair addr",
        CLI_GUEST
    },
#endif
#if (ENABLE_A2DP_SINK || ENABLE_A2DP_SRC)
    {
        "a2dp",
        NULL,
        NULL,
        at_bluetooth_a2dp_cmd_tbl,
        "a2dp cmd",
        CLI_GUEST
    },
#endif /* (ENABLE_A2DP_SINK || ENABLE_A2DP_SRC) */
#if ENABLE_AVRCP_PROFILE
    {
        "avrcp",
        NULL,
        NULL,
        at_bluetooth_avrcp_cmd_tbl,
        "avrcp cmd",
        CLI_GUEST
    },
#endif /* ENABLE_AVRCP_PROFILE */
#if (ENABLE_HID_PROFILE_H || ENABLE_HID_PROFILE_D)
    {
        "hid",
        NULL,
        NULL,
        at_bluetooth_hid_cmd_tbl,
        "hid cmd",
        CLI_GUEST
    },
#endif /* (ENABLE_HID_PROFILE_H || ENABLE_HID_PROFILE_D) */
#if ENABLE_SPP_PROFILE
    {
        "spp",
        NULL,
        NULL,
        at_bluetooth_spp_cmd_tbl,
        "spp cmd",
        CLI_GUEST
    },
#endif /*ENABLE_SPP_PROFILE*/
    END_OF_CLI_CMD_TBL
};

CLI_EXEC_T at_bluetooth_root_cmd_tbl[] =
{
    {
        "bluetooth",
        NULL, NULL,
        at_bluetooth_mw_cmd_tbl,
        "bluetooth commands",
        CLI_GUEST
    },
    END_OF_CLI_CMD_TBL
};



/*-----------------------------------------------------------------------------
 * Name: x_bluetooth_cli_init
 *
 * Description: -
 *
 * Inputs:  -
 *
 * Outputs: -
 *
 * Returns: -
 *
 ----------------------------------------------------------------------------*/

INT32 x_bluetooth_cli_init (VOID)
{
    BT_DBG_NORMAL(BT_DEBUG_COMM, "start x_bluetooth_cli_init");
    /* Attach bluetooth  CLI command table to MW CLI */

    return (x_cli_attach_cmd_tbl(at_bluetooth_root_cmd_tbl, CLI_CAT_MW, CLI_GRP_NONE));
}


//#endif /* CLI_LVL_GUEST */
#endif


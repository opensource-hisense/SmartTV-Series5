/*******************************************************************************
 * LEGAL DISCLAIMER
 *
 * (Header of MediaTek Software/Firmware Release or Documentation)
 *
 * BY OPENING OR USING THIS FILE, BUYER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND
 * AGREES THAT THE SOFTWARE/FIRMWARE AND ITS DOCUMENTATIONS ("MEDIATEK
 * SOFTWARE") RECEIVED FROM MEDIATEK AND/OR ITS REPRESENTATIVES ARE PROVIDED TO
 * BUYER ON AN "AS-IS" BASISONLY. MEDIATEK EXPRESSLY DISCLAIMS ANY AND ALL
 * WARRANTIES, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE OR
 * NONINFRINGEMENT. NEITHER DOES MEDIATEK PROVIDE ANY WARRANTY WHATSOEVER WITH
 * RESPECT TO THE SOFTWARE OF ANY THIRD PARTY WHICH MAY BE USED BY,
 * INCORPORATED IN, OR SUPPLIED WITH THE MEDIATEK SOFTWARE, AND BUYER AGREES TO
 * LOOK ONLY TO SUCH THIRD PARTY FOR ANY WARRANTY CLAIM RELATING THERETO.
 * MEDIATEK SHALL ALSO NOT BE RESPONSIBLE FOR ANY MEDIATEK SOFTWARE RELEASES
 * MADE TO BUYER'S SPECIFICATION OR TO CONFORM TO A PARTICULAR STANDARD OR OPEN
 * FORUM.
 *
 * BUYER'S SOLE AND EXCLUSIVE REMEDY AND MEDIATEK'S ENTIRE AND CUMULATIVE
 * LIABILITY WITH RESPECT TO THE MEDIATEK SOFTWARE RELEASED HEREUNDER WILL BE,
 * AT MEDIATEK'S OPTION, TO REVISE OR REPLACE THE MEDIATEK SOFTWARE AT ISSUE,
 * OR REFUND ANY SOFTWARE LICENSE FEES OR SERVICE CHARGE PAID BY BUYER TO
 * MEDIATEK FOR SUCH MEDIATEK SOFTWARE AT ISSUE.
 *
 * THE TRANSACTION CONTEMPLATED HEREUNDER SHALL BE CONSTRUED IN ACCORDANCE WITH
 * THE LAWS OF THE STATE OF CALIFORNIA, USA, EXCLUDING ITS CONFLICT OF LAWS
 * PRINCIPLES.
 ******************************************************************************/

/* FILE NAME:  bt_mw_common.h
 * AUTHOR: Changlu Yi
 * PURPOSE:
 *      It provides bluetooth common structure to MW.
 * NOTES:
 */


#ifndef _BT_MW_COMMON_H_
#define _BT_MW_COMMON_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <assert.h>
#include <signal.h>
//#include <x_typedef.h>
#include "u_bluetooth.h"
//#include "x_os.h"
#if ENABLE_MTK_TV_PLATFORM
//#include "dbg/x_dbg.h"
//#include "inc/x_common.h"
#endif
#include "bt_mw_log.h"


#define BT_ADDR_STR_MAX_LEN (17)
#define BLUETOOTH_MW_LOCAL_FOLDER  "/3rd_rw/bluetooth"
#define BLUETOOTH_STACK_LOCAL_FOLDER  "/3rd_rw/bluedroid"

#define BLUETOOTH_HCIDUMP_EXEC_CMD      "./usr/sbin/hcidump "
#define BLUETOOTH_HCICONFIG_CMD         "./usr/sbin/hciconfig "
#define BLUETOOTH_HCITOOL_CMD           "./usr/bin/hcitool "

#define BLUETOOTH_HCI_DOWN_CMD          "/usr/sbin/hciconfig hci0 down"
#define BLUETOOTH_SET_HCI_CLASS_CMD     "/usr/sbin/hciconfig hci0 class 0x200428"

#define BLUETOOTH_LOAD_USB_LIB          "sh /etc/bluetooth.script/mtk_load_usb_lib.sh"
#define BLUETOOTH_GET_USB_PATH          "sh /etc/bluetooth.script/mtk_get_usb_path.sh"
#define BLUETOOTH_ROLLBACK_USB_LIB      "sh /etc/bluetooth.script/mtk_rb_usb_lib.sh"

#define BLUETOOTH_LOCAL_INFO_FILE       BLUETOOTH_MW_LOCAL_FOLDER"/bt_local_device_info.txt"
#define BLUETOOTH_SRC_HISTORY_FILE      BLUETOOTH_MW_LOCAL_FOLDER"/bt_paired_src_device_history.txt"
#define BLUETOOTH_SINK_HISTORY_FILE     BLUETOOTH_MW_LOCAL_FOLDER"/bt_paired_sink_device_history.txt"
#define BLUETOOTH_HID_HISTORY_FILE      BLUETOOTH_MW_LOCAL_FOLDER"/bt_paired_hid_device_history.txt"

#define BLUETOOTH_RM_BT_CONFIG_CMD      "rm -rf /3rd_rw/bluedroid/bt_config.*"
#define BLUETOOTH_RM_BT_SNOOP_HCI_CMD   "rm -rf /3rd_rw/bluedroid/btsnoop*"
#define BLUETOOTH_RM_BT_STACK_LOG_CMD   "rm -rf /3rd_rw/bluedroid/bt_stack.log*"
#define BLUETOOTH_RM_BAK_FILE_CMD       "rm -rf /3rd_rw/bluedroid/*.bak"
#define BLUETOOTH_RM_CACHE_FILE_CMD       "rm -rf /3rd_rw/bluetooth/*"

#define BLUETOOTH_UINPUT_PATH           "/dev/input/"
#define BLUETOOTH_ORIGINAL_NAME         "BLUETOOTH_TV"


#ifndef MTK_LINUX_GAP
#define MTK_LINUX_GAP    TRUE
#endif

#define SPP_ADP_UART_GET_BYTE_BUFFER_LENGTH 128
#define BT_GATT_UUID_ARRAY_SIZE         16

typedef enum
{
    BT_TIMER_FLAG_REPEAT = 0,
    BT_TIMER_FLAG_ONCE
}BT_TIMER_INTERVAL_FLAG;

typedef struct bt_error_str_s
{
    BT_ERR_TYPE errorId;
    CHAR *name;
}bt_error_str_t;


typedef enum
{
    BT_DEBUG_LEVEL_ERROR = 0,
    BT_DEBUG_LEVEL_WARNING,
    BT_DEBUG_LEVEL_NORMAL,
    BT_DEBUG_LEVEL_NOTICE,
    BT_DEBUG_LEVEL_INFO,
    BT_DEBUG_LEVEL_MINOR = 5,

    BT_DEBUG_LEVEL_MAX
}BT_DEBUG_LEVEL_T;

#define BT_DBG_COMM   "<BT><COM>"
#define BT_DBG_GAP    "<BT><GAP>"
#define BT_DBG_A2DP   "<BT><A2DP>"
#define BT_DBG_AVRCP  "<BT><AVRCP>"
#define BT_DBG_HID    "<BT><HID>"
#define BT_DBG_SPP    "<BT><SPP>"
#define BT_DBG_GATT   "<BT><GATT>"
#define BT_DBG_HFP    "<BT><HFP>"
#define BT_DBG_PB     "<BT><PB>"
#define BT_DBG_UPL    "<BT><UPL>"


typedef struct _BT_SYNC_RST
{
    UINT8 enabled_codec_type;
    BTEvent bt_event;
    BT_MODE_TYPE bt_mode;
    BTVolType bt_volume_type;
    BOOL fg_bt_stream_closed;
    BOOL fg_bt_pair_done;
    BOOL fg_bt_del_paired_dev_done;
    BOOL fg_bt_power_on;
    BOOL fg_bt_get_devlist_done; //get paired device name&adress info
    BOOL fg_bt_get_dev_info_done; //get local device info
    BOOL fg_bt_2nd_connect_comming;
    BOOL fg_bt_connect;
    BOOL fg_bt_set_scanmode_done;
    BOOL fg_bt_avrcp_connect;
    BOOL fg_bt_avrcp_support;
    BOOL fg_bt_a2dp_support;
    BOOL fg_bt_a2dp_connect;
    BOOL fg_bt_a2dp_disconnecting;
    BOOL fg_bt_is_call_power_off;
    BOOL fg_bt_stream_suspend;
    BOOL fg_bt_get_rssi_done;
    BOOL fg_bt_spp_connect;
    BOOL fg_bt_pts_test;
    BOOL fg_bt_a2dp_src;
    BOOL fg_bt_a2dp_pause;
    BOOL fg_bt_a2dp_pause_done;

    BOOL fg_bt_keep_connection; /*For keep A2DP connection when out of bt function*/
    BOOL fg_bt_discard_data; /*For when keep A2DP connection and discard audio steaming data*/
    BOOL fg_bt_target_support_scms_t;
    BOOL fg_bt_is_mute;
    BOOL fg_bt_send_zero_data;
    /* fg_bt_a2dp_RX_mode */
    /* 0: TX mode(src enable), 1: RX mode(sink enable)*/
    BOOL fg_bt_a2dp_RX_mode;
    BOOL fg_bt_avrcp_absolute_volume_support;
    /* a2dp_enabled_role */
    /* 0: both disable, 1: only sink enable, 2: only src enable, 3: both sink&src enable*/
    UINT8 a2dp_enabled_role;

    BOOL fg_bt_hid_support;

}BT_SYNC_RST;

CHAR* bt_get_log_prefix(VOID);
INT32 bt_get_dbg_level(BT_DEBUG_LAYER_NAME_T layer);
VOID bt_set_dbg_level(BT_DEBUG_LAYER_NAME_T layer, INT32 level);


#ifdef  DBG_MODULE_NAME_PREFIX
#undef  DBG_MODULE_NAME_PREFIX
#endif
#define DBG_MODULE_NAME_PREFIX bt_get_log_prefix()      /* log prefix */

#define FUNC_ENTRY BT_DBG_NORMAL(BT_DEBUG_COMM, "+++ Enter %s +++", __FUNCTION__)
#define FUNC_EXIT BT_DBG_NORMAL(BT_DEBUG_COMM, "--- Exit %s ---", __FUNCTION__)


///< check the NULL pointer
#define BT_CHECK_POINTER(module, p)                                        \
        do                                                              \
        {                                                               \
            if (NULL == p)                                              \
            {                                                           \
                BT_DBG_ERROR(module, "Invalid Pointer!");        \
                return BT_ERR_STATUS_NULL_POINTER;                                \
            }                                                           \
        }while(0)

#define BT_CHECK_RESULT(module, result)                                        \
        do                                                              \
        {                                                               \
            if (BT_SUCCESS != result)                                              \
            {                                                           \
                BT_DBG_ERROR(module,"result:%ld", (long)result);        \
                return result;                                \
            }                                                           \
        }while(0)

#define BT_CHECK_PARAM(module, op, ret)\
        do {\
            if (op){\
                BT_DBG_ERROR(module,"return:%d", ret);\
                return (ret);\
            }\
        }while(0)




CHAR* print_error_str(BT_ERR_TYPE errorId);

INT32 BluetoothInitPre ( VOID );

INT32 BluetoothExitPre ( VOID );

VOID bluetooth_uuid_stos(CHAR *uuid_s,  CHAR *uuid);
VOID bluetooth_uuid_stoh(CHAR *uuid_s,  CHAR *uuid);



#endif /* _BT_MW_COMMON_H_ */


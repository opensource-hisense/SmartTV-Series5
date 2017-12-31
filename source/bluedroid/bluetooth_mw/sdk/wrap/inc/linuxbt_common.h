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

/* FILE NAME:  linuxbt_common.h
 * AUTHOR: Hongliang Hu
 * PURPOSE:
 *      It provides bluetooth common structure to wrap.
 * NOTES:
 */


#ifndef __LINUXBT_COMMON_H__
#define __LINUXBT_COMMON_H__
#include "u_bluetooth.h"
#include "bluetooth.h"


#ifndef MIN
#define MIN(a,b) ((a < b) ? a : b)
#endif
#define LINUXBT_FREE(x)    { if(x != NULL)  free(x); x=NULL; }


#define BTMTK_ANDROID_DEFAULT_REMOTE_NAME "UNKNOWN DEVICE"
#define BTMTK_ANDROID_ADAPTER_PREFIX "MTKBT"
#define BTMTK_MAX_DEVICE_NAME_SIZE 80
#define BTMTK_MAX_PAIRED_LIST_NO   20
#define BTMTK_MAX_OBJECT_PATH_SIZE 32 /* "MTKBT/dev_XX_XX_XX_XX_XX_XX" + '\0' */
#define BTMTK_MAX_ADDR_STR_SIZE 20 /* "XX:XX:XX:XX:XX:XX" + '\0' */
#define BTMTK_MAX_UUID_STR_SIZE 37 /* "XXXXXXXX-XXXX-XXXX-XXXX-XXXXXXXXXXXX" + '\0' */
#define BTMTK_SDP_UUID_16_BIT_SIZE 2
#define BTMTK_SDP_UUID_32_BIT_SIZE 4
#define BTMTK_SDP_UUID_128_BIT_SIZE 16
#define BTMTK_MAX_STACK_TIMEOUT 3000
#define BTMTK_MAX_REMOTE_TIMEOUT 90000
#define BTMTK_MAX_HDP_CHANNEL_PATH_SIZE 40 /* "XX/MTKDEV_XX_XX_XX_XX_XX_XX" + '\0' */

typedef enum
{
    LINUXBT_GAP_POWERED_ON   = 0,       /* Init Cnf */
    LINUXBT_GAP_POWERED_OFF,            /* Deinit Cnf */
    LINUXBT_GAP_INQ_CANCELLED,          /* Inquiry canclled */
    LINUXBT_GAP_INQ_COMPLETE,           /* Inquiry Complete */
    LINUXBT_GAP_SDP_RESPONSE,           /* SDP response */
    LINUXBT_GAP_PINCODE_IND,            /* Pincode indication from bluez to MMI */
    LINUXBT_GAP_PASSKEY_IND,            /* Passkey indication from bluez to MMI */
    LINUXBT_GAP_PAIR_CNF,               /* Pairing Confirm indication */
    LINUXBT_GAP_DEVICE_FOUND,           /* Device found during inquiry */
    LINUXBT_GAP_LOCAL_PROP,             /* Local bluetooth device properties */
    LINUXBT_GAP_PAIR_DEV_CNF,           /* Indicate end of Paired device list */
    LINUXBT_GAP_PAIR_MODE_CHANGED,      /* Indicate Pairing mode set */
    LINUXBT_GAP_DISC_MODE_CHANGED,      /* Indicate Discovery mode set */
    LINUXBT_GAP_NAME_CHANGED,
    LINUXBT_OFF,

    LINUXBT_AVRCP_EVENT_CONNECT,


    /*This Event is sent to indicate that stream is opened, union parameter stream_info will be having stream specific parameters.*/
    LINUXBT_A2DP_SRC_EVENT_STREAM_OPEN,

    /* This Event is sent to pass the remote Codec Configuration Information to MMI in case GET_Capabilites is initiated by MMI*/
    LINUXBT_A2DP_EVENT_STREAM_CONFIG,

    /*This Event is sent to indicate MMI the remote configuration Event. MMI needs to call SET Configuration response after matching capabilities*/
    LINUXBT_A2DP_GET_CONFIG,

    /*This Event is sent to indicate GET Capability Request is completed*/
    //#define BLUETOOTH_A2DP_GET_CAP_CNF 0x05
    LINUXBT_A2DP_SRC_EVENT_STREAM_CLOSING,

    /*This Event is sent to indicate that Stream is Closed,union parameter stream_info will be having stream specific parameters.*/
    LINUXBT_A2DP_SRC_EVENT_STREAM_CLOSED,
    /*This Event is to indicate to MMI that AVDTP START Command is received from remote. MMI Can accept or reject the Request*/
    //#define A2DP_EVENT_STREAM_START_IND 0x07

    /*This Event is to indicate to MMI that AVDTP Stream has Started*/
    LINUXBT_A2DP_EVENT_STREAM_STARTED,

    /*This Event is to indicate to MMI that AVDTP Stream is suspended*/
    LINUXBT_A2DP_EVENT_STREAM_SUSPENDED,

    /*This Event is to indicate to MMI that stream is Aborted*/
    LINUXBT_A2DP_EVENT_STREAM_ABORTED,
    LINUXBT_A2DP_SINK_EVENT_STREAM_OPEN,
    LINUXBT_A2DP_SINK_EVENT_STREAM_CLOSED,

    LINUXBT_EVT_MAX
}LINUXBT_EVT_TYPE;

/* The number of paired records for peer devices. */
#ifndef BT_MAX_DEVICE_RECORDS
#define BT_MAX_DEVICE_RECORDS  32
#endif

#ifndef BT_MAX_NUM_UUIDS
#define BT_MAX_NUM_UUIDS  32
#endif

/* The max length of string uuid. */
#ifndef BT_MAX_UUID_LEN
#define BT_MAX_UUID_LEN 37
#endif

/** Bluetooth Address */
typedef struct _BT_ADDR_T
{
    CHAR address[MAX_BDADDR_LEN];
}BT_ADDR_T;

/** Bluetooth 128-bit string UUID */
typedef struct _BT_UUIDS_T
{
   CHAR uuid[BT_MAX_UUID_LEN];
}BT_UUIDS_T;

typedef struct _BT_GAP_BONDED_DEV_T
{
    UINT32 num_devices;
    BT_ADDR_T devices[BT_MAX_DEVICE_RECORDS];
} BT_GAP_BONDED_DEV_T;

typedef struct _BT_GAP_LOCAL_PROP_INFO
{
    CHAR              bdAddr[MAX_BDADDR_LEN];
    CHAR              name[MAX_NAME_LEN];
    UINT8             powered;
    UINT8             scan_mode;
    UINT32            disctimeout;
    UINT8             uuid_num;
    BT_UUIDS_T local_uuids[BT_MAX_NUM_UUIDS];
    BT_GAP_BONDED_DEV_T paired_devices;
}BT_GAP_LOCAL_PROP_INFO;

INT32 linuxbt_btaddr_stoh(CHAR *btaddr_s, bt_bdaddr_t *bdaddr_h);
INT32 linuxbt_btaddr_htos(bt_bdaddr_t *bdaddr_h, CHAR *btaddr_s);
INT32 linuxbt_reverse_bd_addr(bt_bdaddr_t *addr);
/**
 * FUNCTION NAME: linuxbt_print_uuid
 * PURPOSE:
 *      The function is used to convert uuid from bt_uuid_t* to char*.
 * INPUT:
 *      uuid        -- array to store uuid
 *      uuid_s      -- string to sore uuid
 * OUTPUT:
 *      None
 * RETURN:
 *      None
 * NOTES:
 *      None
 */
void linuxbt_print_uuid(bt_uuid_t* uuid, char* uuid_s);

VOID  linuxbt_uuid_stoh(CHAR *uuid_s,  bt_uuid_t *uuid);
INT32 ascii_2_hex(CHAR *p_ascii, INT32 len, UINT8 *p_hex);
INT32 linuxbt_reverse_bd_addr(bt_bdaddr_t *addr);
UINT32 bt_array2uint(UINT8 *ps_array, INT32 len);
UINT8* get_memory(INT32 lenth);
BT_ERR_STATUS_T linuxbt_return_value_convert(bt_status_t ret);


#endif /* __LINUXBT_COMMON_H__ */

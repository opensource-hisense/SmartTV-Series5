/********************************************************************************************
 *     LEGAL DISCLAIMER
 *
 *     (Header of MediaTek Software/Firmware Release or Documentation)
 *
 *     BY OPENING OR USING THIS FILE, BUYER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND AGREES
 *     THAT THE SOFTWARE/FIRMWARE AND ITS DOCUMENTATIONS ("MEDIATEK SOFTWARE") RECEIVED
 *     FROM MEDIATEK AND/OR ITS REPRESENTATIVES ARE PROVIDED TO BUYER ON AN "AS-IS" BASIS
 *     ONLY. MEDIATEK EXPRESSLY DISCLAIMS ANY AND ALL WARRANTIES, EXPRESS OR IMPLIED,
 *     INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR
 *     A PARTICULAR PURPOSE OR NONINFRINGEMENT. NEITHER DOES MEDIATEK PROVIDE ANY WARRANTY
 *     WHATSOEVER WITH RESPECT TO THE SOFTWARE OF ANY THIRD PARTY WHICH MAY BE USED BY,
 *     INCORPORATED IN, OR SUPPLIED WITH THE MEDIATEK SOFTWARE, AND BUYER AGREES TO LOOK
 *     ONLY TO SUCH THIRD PARTY FOR ANY WARRANTY CLAIM RELATING THERETO. MEDIATEK SHALL ALSO
 *     NOT BE RESPONSIBLE FOR ANY MEDIATEK SOFTWARE RELEASES MADE TO BUYER'S SPECIFICATION
 *     OR TO CONFORM TO A PARTICULAR STANDARD OR OPEN FORUM.
 *
 *     BUYER'S SOLE AND EXCLUSIVE REMEDY AND MEDIATEK'S ENTIRE AND CUMULATIVE LIABILITY WITH
 *     RESPECT TO THE MEDIATEK SOFTWARE RELEASED HEREUNDER WILL BE, AT MEDIATEK'S OPTION,
 *     TO REVISE OR REPLACE THE MEDIATEK SOFTWARE AT ISSUE, OR REFUND ANY SOFTWARE LICENSE
 *     FEES OR SERVICE CHARGE PAID BY BUYER TO MEDIATEK FOR SUCH MEDIATEK SOFTWARE AT ISSUE.
 *
 *     THE TRANSACTION CONTEMPLATED HEREUNDER SHALL BE CONSTRUED IN ACCORDANCE WITH THE LAWS
 *     OF THE STATE OF CALIFORNIA, USA, EXCLUDING ITS CONFLICT OF LAWS PRINCIPLES.
 ************************************************************************************************/
/* FILE NAME:  bt_mw_gap.c
 * PURPOSE:
 *  {1. What is covered in this file - function and scope.}
 *  {2. Related documents or hardware information}
 * NOTES:
 *  {Something must be known or noticed}
 *  {1. How to use these functions - Give an example.}
 *  {2. Sequence of messages if applicable.}
 *  {3. Any design limitation}
 *  {4. Any performance limitation}
 *  {5. Is it a reusable component}
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>

//#include "bt_types.h"
#include "bluetooth.h"
#include "bt_hh.h"
#if defined(MTK_LINUX_AVRCP_PLUS) && (MTK_LINUX_AVRCP_PLUS == TRUE)
#include "mtk_bt_rc.h"
#else
#include "bt_rc.h"
#endif
#include "bt_av.h"

#include "bt_mw_common.h"
#include "linuxbt_common.h"
#include "linuxbt_gap_if.h"
#include "bt_mw_gap.h"
#include "bluetooth_sync.h"

#include "bt_mw_devices_info.h"
#if ENABLE_MTK_TV_PLATFORM
//#include "dbg/x_dbg.h"
#else
//#include "x_ipcd.h"
#endif

#define BT_DEFAULT_DISCOVERABLE_TIMEOUT 0 /*default discoverable timeout value*/
#define BT_DEFAULT_PAIRABLE_TIMEOUT 0   /*default pairable timeout value*/

BT_GAP_CBK_PARMS    GapCbkParms;
extern BtAppEventCbk      BtAppCbk;
extern BT_DEV_MAPPING_LIST scan_target_dev_info;
extern UINT32 fg_bt_inquiry_type;
extern BT_GAP_LOCAL_PROPERTIES_RESULT g_localProperty;
extern BtAppGapGetPairingKeyCbk AppGetPairingKeyCbk;
extern BtAppBondCbk AppBondCbk;
extern BT_HID_TARGET_DEV_LIST *g_pt_paired_hid_dev_history;
extern BT_HID_STATUS_STRUCT_LIST *g_pt_hid_status_list;

CHAR g_ps_target_mac[18];
volatile BOOL g_fg_bt_inquiry_done = FALSE;
static INT32 g_rssi_value = 0;
/*discovered device information*/
BT_DEV_MAPPING_LIST all_target_dev_info = {0};
static INT32 g_pt_result_number = 0;
BT_DEV_MAPPING_LIST bond_target_dev_info = {0};
extern BOOL fg_bt_scan_ongoning;

/* FUNCTION NAME: bluetooth_set_local_dev_name
 * PURPOSE:
 *      { set_local_dev_name }
 * INPUT:
 *      parameter-1  --  name : device name
 * OUTPUT:
 *      parameter-n  --
 * RETURN:
 *      return-value-1 --
 * NOTES:
 (1) this API will sendMsg to stack to set local device name
 (2) local device name will also stored in the variable g_localProperty
 *
 */
INT32 bluetooth_set_local_dev_name(CHAR *name)
{
    FUNC_ENTRY;
    BT_CHECK_POINTER(BT_DEBUG_GAP, name);

    BT_DBG_NORMAL(BT_DEBUG_GAP, "<<< call bt_setlocal_name >>>");
    linuxbt_gap_set_device_name_handler(name);
    strncpy(g_localProperty.name, name, sizeof(g_localProperty.name));
#if ENABLE_SAVE_LOCAL_DEV_TO_FILE
    INT32 i4_ret;
    i4_ret = bluetooth_WriteLocalInfoToFile(&g_localProperty);
    if (0 != i4_ret)
    {
        BT_DBG_ERROR(BT_DEBUG_GAP, "bluetooth_WriteLocalInfoToFile fail");
        return BT_ERR_STATUS_FAIL;
    }
#endif
    return BT_SUCCESS;
} /*bt_set_name*/

/* FUNCTION NAME: bluetooth_get_properties
 * PURPOSE:
 *      { get the information of local device }
 * INPUT:
 *      parameter-1  --
 * OUTPUT:
 *      parameter-n  --
 * RETURN:
 *      return-value-1 --
 * NOTES:
 *
 */
INT32 bluetooth_get_properties(VOID)
{
    FUNC_ENTRY;

    set_bt_get_dev_info_done(FALSE);
    linuxbt_gap_get_adapter_properties_handler();

    return BT_SUCCESS;
} /*bluetooth_get_properties*/

/* FUNCTION NAME: bluetooth_set_power
 * PURPOSE:
 *      { set the power on or off}
 * INPUT:
 *      parameter-1  --
 * OUTPUT:
 *      parameter-n  --
 * RETURN:
 *      return-value-1 --
 * NOTES:
 *
 */
INT32 bluetooth_set_power(BOOL fg_on)
{
    FUNC_ENTRY;

    if (fg_on)
    {
        if (!get_bt_power_on())
        {
            BT_DBG_NORMAL(BT_DEBUG_GAP, "<<< call btenable >>>");
            return linuxbt_gap_enable_handler();
        }
        else
        {
            BT_DBG_NORMAL(BT_DEBUG_GAP, "already power on");
            return BT_ERR_STATUS_FAIL;
        }
    }
    else
    {
        if (get_bt_power_on())
        {
            BT_DBG_NORMAL(BT_DEBUG_GAP, "<<< call btdisable >>>");
            return linuxbt_gap_disable_handler();
        }
        else
        {
            BT_DBG_NORMAL(BT_DEBUG_GAP, "already power off");
            return BT_ERR_STATUS_FAIL;
        }
    }

    return BT_SUCCESS;
} /*bt_set_power_on*/

/* FUNCTION NAME: bluetooth_set_power_sync
 * PURPOSE:
 *      { set the power on or off ,sync mode }
 * INPUT:
 *      parameter-1  --
 * OUTPUT:
 *      parameter-n  --
 * RETURN:
 *      return-value-1 --
 * NOTES:
 *
 */
INT32 bluetooth_set_power_sync(BOOL fg_on)
{
    FUNC_ENTRY;
    UINT32 ui4_loop;
    INT32 i4_ret = 0;
    i4_ret = bluetooth_set_power(fg_on);
    if (BT_ERR_STATUS_FAIL == i4_ret)
    {
        BT_DBG_NORMAL(BT_DEBUG_GAP, "no need wait power %s", fg_on?"on":"off");
        return BT_SUCCESS;
    }
    /*waiting for power off event*/
    ui4_loop = 10;
    while (0 < ui4_loop)
    {
        BT_DBG_NORMAL(BT_DEBUG_GAP, "ui4_loop:%ld,power is %s:",
                      (long)ui4_loop,
                      get_bt_power_on()? "on":"off"
                     );
        if (get_bt_power_on() == fg_on)
        {
            break;
        }
        /*x_thread_delay(500);*/
        usleep(500*1000);
        ui4_loop--;
    }
    if (fg_on != get_bt_power_on())
    {
        BT_DBG_ERROR(BT_DEBUG_GAP, "can't power %s", fg_on?"on":"off");
        return BT_ERR_STATUS_FAIL;
    }
    FUNC_EXIT;
    return BT_SUCCESS;
} /*bluetooth_set_power_sync*/

/* FUNCTION NAME: bluetooth_set_connectable_and_discoverable
 * PURPOSE:
 *      {set the state of local device connectable_and_discoverable }
 * INPUT:
 *      parameter-1  --
 * OUTPUT:
 *      parameter-n  --
 * RETURN:
 *      return-value-1 --
 * NOTES:
 *
 */
INT32 bluetooth_set_connectable_and_discoverable(BOOL fg_conn, BOOL fg_disc)
{
    FUNC_ENTRY;

    return bluetooth_set_scanmode_sync(fg_conn, fg_disc);
} /*bluetooth_set_connectable_and_discoverable*/

/* FUNCTION NAME: bluetooth_set_scanmode_sync
 * PURPOSE:
 *      { set the state of local device connectable_and_discoverable  }
 * INPUT:
 *      parameter-1  --
 * OUTPUT:
 *      parameter-n  --
 * RETURN:
 *      return-value-1 --
 * NOTES: this function works the same as bluetooth_set_connectable_and_discoverable
 *
 */
INT32 bluetooth_set_scanmode_sync(BOOL fg_conn, BOOL fg_disc)
{
    FUNC_ENTRY;
    bt_scan_mode_t scan_mode;
    INT32 i4_ret = BT_ERR_STATUS_FAIL;
    INT32 i4_loop = 10;

    if (!get_bt_power_on())
    {
        BT_DBG_NORMAL(BT_DEBUG_GAP, "power is off");
        return BT_SUCCESS;
    }

    set_bt_set_scanmode_done(FALSE);
    /*pscan_on --> connectable on*/
    if (fg_conn)
    {
        /*iscan_on --> discoverable on*/
        if (fg_disc)
        {
            BT_DBG_NORMAL(BT_DEBUG_GAP, "<<< call bt_setscan_mode(%ld) >>>", (long)BT_SCAN_MODE_CONNECTABLE_DISCOVERABLE);
            scan_mode = BT_SCAN_MODE_CONNECTABLE_DISCOVERABLE;
        }
        else
        {
            BT_DBG_NORMAL(BT_DEBUG_GAP, "<<< call bt_setscan_mode(%ld) >>>", (long)BT_SCAN_MODE_CONNECTABLE);
            scan_mode = BT_SCAN_MODE_CONNECTABLE;
        }
    }
    else
    {
        BT_DBG_NORMAL(BT_DEBUG_GAP, "<<< call bt_setscan_mode(%ld) >>>", (long)BT_SCAN_MODE_NONE);
        scan_mode = BT_SCAN_MODE_NONE;
    }
    i4_ret = linuxbt_gap_set_scan_mode(scan_mode);

    if (BT_SUCCESS == i4_ret)
    {
        while (!get_bt_set_scanmode_done() && 0 < i4_loop)
        {
            BT_DBG_NOTICE(BT_DEBUG_GAP, "wait set scan mode done:%ld", (long)i4_loop);
            /*x_thread_delay(200);*/
            usleep(200*1000);
            i4_loop --;
        }
        if (!get_bt_set_scanmode_done())
        {
            BT_DBG_ERROR(BT_DEBUG_GAP, "set scan mode failed");
            i4_ret = BT_ERR_STATUS_FAIL;
        }
    }
    else
    {
        BT_DBG_ERROR(BT_DEBUG_GAP, "set bt connectable & discoverable failed");
        i4_ret = BT_ERR_STATUS_FAIL;
    }

    FUNC_EXIT;
    return i4_ret;
}

/* FUNCTION NAME: bluetooth_scan
 * PURPOSE:
 *      { to scan the remote device}
 * INPUT:
 *      parameter-1  --
 * OUTPUT:
 *      parameter-n  --
 * RETURN:
 *      return-value-1 --
 * NOTES:
 *
 */
EXPORT_SYMBOL INT32 bluetooth_scan(VOID)
{
    FUNC_ENTRY;

    BT_DBG_NORMAL(BT_DEBUG_GAP, "<<< call btstartDiscovery >>>");
    linuxbt_gap_start_discovery_handler();
    FUNC_EXIT;

    return BT_SUCCESS;
}

/* FUNCTION NAME: bluetooth_send_get_rssi
 * PURPOSE:
 *      { to get the rssi  }
 * INPUT:
 *      parameter-1  --
 * OUTPUT:
 *      parameter-n  --
 * RETURN:
 *      return-value-1 --
 * NOTES:
 *
 */
INT32 bluetooth_send_get_rssi(CHAR *address)
{
    FUNC_ENTRY;

    set_bt_get_rssi_done(FALSE);
    BT_DBG_NORMAL(BT_DEBUG_GAP, "<<< call bt_get_rssi >>>");
    linuxbt_gap_get_rssi_handler(address);
    FUNC_EXIT;

    return BT_SUCCESS;
}

/* FUNCTION NAME: bluetooth_set_rssi_value
 * PURPOSE:
 *      { to set the rssi value }
 * INPUT:
 *      parameter-1  --
 * OUTPUT:
 *      parameter-n  --
 * RETURN:
 *      return-value-1 --
 * NOTES:
 *
 */
INT32 bluetooth_set_rssi_value(INT16 rssi_value)
{
    g_rssi_value = rssi_value;

    return BT_SUCCESS;
}

/* FUNCTION NAME: bluetooth_get_rssi_value
 * PURPOSE:
 *      { to get the rssi value }
 * INPUT:
 *      parameter-1  --
 * OUTPUT:
 *      parameter-n  --
 * RETURN:
 *      return-value-1 --
 * NOTES:
 *
 */
INT32 bluetooth_get_rssi_value(INT16 *prssi_value)
{
    *prssi_value = g_rssi_value;

    return BT_SUCCESS;
}

/* FUNCTION NAME: bluetooth_gap_start_sniff_mode
 * PURPOSE:
 *      { to start the mode sniffer mode  }
 * INPUT:
 *      parameter-1  --
 * OUTPUT:
 *      parameter-n  --
 * RETURN:
 *      return-value-1 --
 * NOTES:
 *
 */
VOID bluetooth_gap_start_sniff_mode(UINT8 fg_start,
                                    CHAR *ptr_addr,
                                    INT32 i4maxInterval,
                                    INT32 i4minInterval,
                                    INT32 i4attempt,
                                    INT32 i4timeout)
{
    FUNC_ENTRY;
 /*   linuxbt_gap_start_sniff_mode_handler(fg_start,
                                         ptr_addr,
                                         i4maxInterval,
                                         i4minInterval,
                                         i4attempt,
                                         i4timeout);*/
}

/* FUNCTION NAME: bluetooth_inquiry_one
 * PURPOSE:
 *      {to inquiry only one device}
 * INPUT:
 *      parameter-1  --
 * OUTPUT:
 *      parameter-n  --
 * RETURN:
 *      return-value-1 --
 * NOTES:
 *
 */
INT32 bluetooth_inquiry_one(CHAR * ps_dev_mac, UINT32 ui4_inqDuration)
{
    FUNC_ENTRY;

    UINT32     ui4_tryCount = 0;


    if (NULL == ps_dev_mac)
    {
        BT_DBG_ERROR(BT_DEBUG_GAP, "the point of ps_dev_mac is null!");
        return BT_ERR_STATUS_NULL_POINTER;
    }
    BT_CHECK_PARAM(BT_DEBUG_GAP, (strlen(ps_dev_mac) > BT_ADDR_STR_MAX_LEN), BT_ERR_STATUS_PARM_INVALID);
    memset(&g_ps_target_mac, 0, sizeof(g_ps_target_mac));
    strncpy(g_ps_target_mac, ps_dev_mac, sizeof(g_ps_target_mac));
    g_ps_target_mac[17] = '\0';
    BT_DBG_NORMAL(BT_DEBUG_GAP, "start scan for %s/n", g_ps_target_mac);
    g_fg_bt_inquiry_done = FALSE;
    BT_DBG_NORMAL(BT_DEBUG_GAP, "<<< call btstartDiscovery >>>");
    linuxbt_gap_start_discovery_handler();

    ui4_tryCount = ui4_inqDuration;
    while (!g_fg_bt_inquiry_done && (0 < ui4_tryCount))
    {
        BT_DBG_NORMAL(BT_DEBUG_GAP, "ui4_tryCount:%ld, g_fg_bt_inquiry_done:%ld!",
                      (long)ui4_tryCount,
                      (long)g_fg_bt_inquiry_done
                     );
        /*x_thread_delay(1000);*/
        usleep(1000*1000);
        ui4_tryCount--;
    }
    linuxbt_gap_cancel_discovery_handler();
    if (!g_fg_bt_inquiry_done)
    {
        BT_DBG_NORMAL(BT_DEBUG_GAP, "can't find the target device");
        return BT_ERR_STATUS_FAIL;
    }
    g_fg_bt_inquiry_done = FALSE;

    FUNC_EXIT;
    return BT_SUCCESS;
}


/* FUNCTION NAME: bluetooth_stop_scan
 * PURPOSE:
 *      {to stop scan }
 * INPUT:
 *      parameter-1  --
 * OUTPUT:
 *      parameter-n  --
 * RETURN:
 *      return-value-1 --
 * NOTES:
 *
 */
EXPORT_SYMBOL INT32 bluetooth_stop_scan(VOID)
{
    FUNC_ENTRY;
    BT_DBG_NORMAL(BT_DEBUG_GAP, "<<< call btstopDiscovery >>>");
    linuxbt_gap_cancel_discovery_handler();

    return BT_SUCCESS;
} /*bt_cancel_inquire*/

/* FUNCTION NAME: bluetooth_sdp_request
 * PURPOSE:
 *      { to requst SDP }
 * INPUT:
 *      parameter-1  --
 * OUTPUT:
 *      parameter-n  --
 * RETURN:
 *      return-value-1 --
 * NOTES:
 *
 */
INT32 bluetooth_sdp_request(CHAR *addr)
{
    FUNC_ENTRY;
    BT_DBG_NORMAL(BT_DEBUG_GAP, "<<< call btstartSdp >>>");
    //btstartSdp(addr);

    return BT_SUCCESS;
} /*bt_sdp_request*/

/* FUNCTION NAME: bluetooth_pair
 * PURPOSE:
 *      { to pair }
 * INPUT:
 *      parameter-1  --
 * OUTPUT:
 *      parameter-n  --
 * RETURN:
 *      return-value-1 --
 * NOTES:
 *
 */
INT32 bluetooth_pair(CHAR *addr, int transport)
{
    FUNC_ENTRY;
    if (NULL == addr)
    {
        BT_DBG_ERROR(BT_DEBUG_GAP, "null pointer of addr!");
        return BT_ERR_STATUS_FAIL;
    }
    BT_DBG_NORMAL(BT_DEBUG_GAP, "<<< call btstartpaired >>>");
    linuxbt_gap_create_bond_handler(addr, transport);

    return BT_SUCCESS;
} /*bt_pairing_request*/

/* FUNCTION NAME: bluetooth_remove_paired_dev
 * PURPOSE:
 *      {to remove paired device}
 * INPUT:
 *      parameter-1  --
 * OUTPUT:
 *      parameter-n  --
 * RETURN:
 *      return-value-1 --
 * NOTES:
 *
 */
INT32 bluetooth_remove_paired_dev(CHAR *addr)
{
    FUNC_ENTRY;
    BT_CHECK_POINTER(BT_DEBUG_GAP, addr);
    BT_DBG_INFO(BT_DEBUG_GAP, "the address will delete is: %s", addr);
    //linuxbt_gap_detrust_one_handler(addr);
    linuxbt_gap_remove_bond_handler(addr);
    return BT_SUCCESS;
} /*bt_delete_pairing_request*/


/* FUNCTION NAME: bluetooth_pre_init
 * PURPOSE:
 *      {pre_init}
 * INPUT:
 *      parameter-1  --
 * OUTPUT:
 *      parameter-n  --
 * RETURN:
 *      return-value-1 --
 * NOTES:
 *
 */
INT32 bluetooth_pre_init( VOID )
{
    FUNC_ENTRY;

    return BluetoothInitPre();
}

/* FUNCTION NAME: bluetooth_gap_init
 * PURPOSE:
 *      {gap profile initial}
 * INPUT:
 *      parameter-1  --
 * OUTPUT:
 *      parameter-n  --
 * RETURN:
 *      return-value-1 --
 * NOTES:
 *
 */
INT32 bluetooth_gap_init(VOID)
{
    FUNC_ENTRY;
    memset(&all_target_dev_info, 0x0, sizeof(BT_DEV_MAPPING_LIST));
    return linuxbt_gap_init();

} /*bluetooth_gap_init*/

/* FUNCTION NAME: bluetooth_gap_deinit
 * PURPOSE:
 *      {gap profile deinitial}
 * INPUT:
 *      parameter-1  --
 * OUTPUT:
 *      parameter-n  --
 * RETURN:
 *      return-value-1 --
 * NOTES:
 *
 */
INT32 bluetooth_gap_deinit(VOID)
{
    FUNC_ENTRY;
    fg_bt_scan_ongoning = FALSE;
    linuxbt_gap_deinit_profiles();

    return BT_SUCCESS;
} /*bluetooth_gap_init*/
#if 0
extern bt_callbacks_t *bt_hal_cbacks;
//extern kal_bool g_force_exit_btif_task;
extern btav_callbacks_t *bt_av_callbacks;
extern btrc_ctrl_callbacks_t *bt_rc_ctrl_callbacks;
extern btrc_callbacks_t* bt_rc_callbacks;
extern bthh_callbacks_t *bt_hh_callbacks;
#endif

/* FUNCTION NAME: bluetooth_clear_external_resource
 * PURPOSE:
 *      {clear_external_resource when restart BT}
 * INPUT:
 *      parameter-1  --
 * OUTPUT:
 *      parameter-n  --
 * RETURN:
 *      return-value-1 --
 * NOTES:
 *
 */
VOID bluetooth_clear_external_resource(VOID)
{
    FUNC_ENTRY;
  #if 0
    bt_hal_cbacks = NULL;
    //g_force_exit_btif_task = TRUE;
    bt_av_callbacks = NULL;
    bt_rc_ctrl_callbacks = NULL;
    bt_rc_callbacks = NULL;
    bt_hh_callbacks = NULL;
	//bt_hd_callbacks = NULL;
   #endif
}
/* FUNCTION NAME: bluetooth_set_virtual_sniffer
 * PURPOSE:
 *      {to enable/disable sniffer log}
 * INPUT:
 *      parameter-1  --
 * OUTPUT:
 *      parameter-n  --
 * RETURN:
 *      return-value-1 --
 * NOTES:
 *
 */
EXPORT_SYMBOL INT32 bluetooth_set_virtual_sniffer(INT32 enable)
{
    FUNC_ENTRY;
    linuxbt_gap_config_hci_snoop_log_handler(enable);

    return BT_SUCCESS;
} /*bluetooth_set_virtual_sniffer*/

VOID bt_gap_handle_every_result_ind(INT32 num_properties, bt_property_t *properties)
{
    INT32 i;
    BT_GAP_SCAN_RESULT g_pt_result;
    bt_property_t *property = NULL;
    CHAR *name= NULL;
    bt_bdaddr_t *btaddr = NULL;
    UINT32 *cod= NULL;
    UINT32 *devicetype= NULL;
    UINT32 *disc_timeout= NULL;
    INT16 *rssi= NULL;
    g_pt_result_number = num_properties;
    BT_DBG_INFO(BT_DEBUG_GAP, "[GAP] num_properties = %ld", (long)num_properties);
    memset(&g_pt_result, 0x0, sizeof(BT_GAP_SCAN_RESULT));

    for (i = 0; i < num_properties; i++)
    {
        property = &properties[i];

        switch (property->type)
        {
            /* 1 */
        case BT_PROPERTY_BDNAME:
            name = (CHAR *)property->val;
            if (property->len < MAX_NAME_LEN)
            {
                memcpy(g_pt_result.name, name, property->len);
                g_pt_result.name[property->len] = '\0';
            }
            else
            {
                memcpy(g_pt_result.name, name, MAX_NAME_LEN);
            }
            BT_DBG_INFO(BT_DEBUG_GAP,"name = %s\n", g_pt_result.name);
            /*save the properties number*/
            g_pt_result_number = num_properties -1 ;
            break;
            /* 2 */
        case BT_PROPERTY_BDADDR:
            btaddr = (bt_bdaddr_t *)property->val;
            linuxbt_btaddr_htos(btaddr, g_pt_result.bdAddr);
            BT_DBG_INFO(BT_DEBUG_GAP,"bt addr: %s\n", g_pt_result.bdAddr);

            break;
            /* 3 */
        case BT_PROPERTY_UUIDS:
            //uuid = (bt_uuid_t*)property->val;
            //len = property->len;
            break;
            /* 4 */
        case BT_PROPERTY_CLASS_OF_DEVICE:
            cod = (UINT32 *)property->val;
            g_pt_result.cod = *cod;
            BT_DBG_NORMAL(BT_DEBUG_GAP,"cod = 0x%6x\n", (unsigned int)g_pt_result.cod);
            break;
            /* 5 */
        case BT_PROPERTY_TYPE_OF_DEVICE:
            devicetype= (UINT32 *)property->val;

            /* 0 - BLE, 1 - BT, 2 - DUAL MODE */
            BT_DBG_INFO(BT_DEBUG_GAP,"devicetype = %ld\n", (unsigned long)*devicetype);
            break;
            /* 6 */
        case BT_PROPERTY_SERVICE_RECORD:
            break;
            /* 7 */
        case BT_PROPERTY_ADAPTER_SCAN_MODE:
            //mode = (bt_scan_mode_t *)property->val;
            break;
            /* 8 */
        case BT_PROPERTY_ADAPTER_BONDED_DEVICES:
            btaddr = (bt_bdaddr_t *)property->val;

            BT_DBG_NORMAL(BT_DEBUG_GAP,"[GAP] type = %ld, len = %ld, bonded_addr = %02X:%02X:%02X:%02X:%02X:%02X\n", (long)property->type, (long)property->len,
                          btaddr->address[0], btaddr->address[1], btaddr->address[2],
                          btaddr->address[3], btaddr->address[4], btaddr->address[5]);
            break;
            /* 9 */
        case BT_PROPERTY_ADAPTER_DISCOVERY_TIMEOUT:
            disc_timeout = (UINT32 *)property->val;

            BT_DBG_NORMAL(BT_DEBUG_GAP,"[GAP] type = %ld, len = %ld, disc_timeout = %ld\n", (long)property->type, (long)property->len, (unsigned long)*disc_timeout);
            break;
            /* 10 */
        case BT_PROPERTY_REMOTE_FRIENDLY_NAME:
            name = (CHAR *)property->val;
            BT_DBG_NORMAL(BT_DEBUG_GAP,"[GAP] type = %ld, len = %ld, bdname = %s\n", (long)property->type, (long)property->len, name);

            break;
            /* 11 */
        case BT_PROPERTY_REMOTE_RSSI:
            rssi = (INT16 *)property->val;
            *rssi -= 256;
            g_pt_result.rssi = *rssi - 256;
            BT_DBG_INFO(BT_DEBUG_GAP,"rssi = %ld\n", (long)g_pt_result.rssi);
            break;
            /* 12 */
        case BT_PROPERTY_REMOTE_VERSION_INFO:
            BT_DBG_NORMAL(BT_DEBUG_GAP,"[GAP] type = %ld, len = %ld, Others.\n", (long)property->type, (long)property->len);
            break;
            /* FF */
        case BT_PROPERTY_REMOTE_DEVICE_TIMESTAMP:
            BT_DBG_NORMAL(BT_DEBUG_GAP,"[GAP] type = %ld, len = %ld, Others.\n", (long)property->type, (long)property->len);
            break;
        default:
            BT_DBG_INFO(BT_DEBUG_GAP,"[GAP] type = %ld, len = %ld, Others.\n", (long)property->type, (long)property->len);
            break;
        }
    }

    if (NULL != btaddr)
    {
        bluetooth_gap_get_mapping_info(btaddr, num_properties, properties,&all_target_dev_info);
        bluetooth_gap_get_mapping_info(btaddr, num_properties, properties,&scan_target_dev_info);

        if (fg_bt_scan_ongoning)
            bluetooth_notify_every_device_info_app(btaddr, &scan_target_dev_info);
    }
}


/* FUNCTION NAME: bt_gap_handle_discovery_result_ind
 * PURPOSE:
 *      { gap discover result }
 * INPUT:
 *      parameter-1  --
 * OUTPUT:
 *      parameter-n  --
 * RETURN:
 *      return-value-1 --
 * NOTES:
 *
 */
VOID bt_gap_handle_discovery_result_ind(INT32 num_properties, bt_property_t *properties)
{
    INT32 i;
    INT32 inquiry_type = 0;
    BT_GAP_SCAN_RESULT g_pt_result;
    bt_property_t *property = NULL;
    CHAR *name= NULL;
    bt_bdaddr_t *btaddr = NULL;
    UINT32 *cod= NULL;
    UINT32 *devicetype= NULL;
    UINT32 *disc_timeout= NULL;
    INT16 *rssi= NULL;
    BOOL cod_valid = FALSE;
    BOOL is_audio = FALSE;
    g_pt_result_number = num_properties;
    BT_DBG_INFO(BT_DEBUG_GAP, "[GAP] num_properties = %ld", (long)num_properties);
    memset(&g_pt_result, 0x0, sizeof(BT_GAP_SCAN_RESULT));

    if (BT_INQUIRY_FILTER_TYPE_DS4 == fg_bt_inquiry_type)//only DS4
    {
        inquiry_type = 1;
    }
    else if (BT_INQUIRY_FILTER_TYPE_HID== fg_bt_inquiry_type)//keyboard mouse
    {
        inquiry_type = 2;
    }
    else if (BT_INQUIRY_FILTER_TYPE_A2DP_SNK== fg_bt_inquiry_type)//A2DP SINK
    {
        inquiry_type = 4;
    }
    else if (BT_INQUIRY_FILTER_TYPE_A2DP_HID== fg_bt_inquiry_type)//A2DP SINK && keyboard mouse&&GamePad
    {
        inquiry_type = 8;
    }

    if (0 == inquiry_type)
    {
        BT_DBG_INFO(BT_DEBUG_GAP,"Do not have any filter.\n");
        bt_gap_handle_every_result_ind(num_properties, properties);
        return;
    }
    if (inquiry_type > 1)//[A2DP SINK] [keyboard mouse]
    {
        BT_DBG_INFO(BT_DEBUG_GAP, "A2DP SINK discover result");
        for (i = 0; i < num_properties; i++)
        {
            property = &properties[i];

            switch (property->type)
            {
                /* 1 */
            case BT_PROPERTY_BDNAME:
                name = (CHAR *)property->val;
                if (property->len < MAX_NAME_LEN)
                {
                    memcpy(g_pt_result.name, name, property->len);
                    g_pt_result.name[property->len] = '\0';
                }
                else
                {
                    memcpy(g_pt_result.name, name, MAX_NAME_LEN);
                }
                BT_DBG_INFO(BT_DEBUG_GAP, "name = %s", g_pt_result.name);
                g_pt_result_number = num_properties - 1;
                break;
                /* 2 */
            case BT_PROPERTY_BDADDR:
                btaddr = (bt_bdaddr_t *)property->val;
                linuxbt_btaddr_htos(btaddr, g_pt_result.bdAddr);
                BT_DBG_INFO(BT_DEBUG_GAP, "bt addr: %s", g_pt_result.bdAddr);

                break;
                /* 3 */
            case BT_PROPERTY_UUIDS:
                //uuid = (bt_uuid_t*)property->val;
                //len = property->len;
                break;
                /* 4 */
            case BT_PROPERTY_CLASS_OF_DEVICE:
                cod = (UINT32 *)property->val;
                g_pt_result.cod = *cod;
                BT_DBG_NORMAL(BT_DEBUG_GAP, "cod = 0x%6x", (unsigned int)g_pt_result.cod);
                if ((inquiry_type == 4) ||(inquiry_type == 8)) //A2DP
                {
                    /* Audio&Rending */
                    if (((g_pt_result.cod&0x00240400) == 0x00240400) ||
                        ((g_pt_result.cod&0x00040400) == 0x000040400))
                    {
                        BT_DBG_NORMAL(BT_DEBUG_GAP, "Audio Rending cod valid");
                        cod_valid = TRUE;
                        is_audio = TRUE;
                    }
                    else
                    {
                        BT_DBG_ERROR(BT_DEBUG_GAP, "Audio Rending cod invalid");
                    }
                    if (((g_pt_result.cod&0x00200400) == 0x00200400) && !((g_pt_result.cod&0x00080000) == 0x00080000))
                    {
                        BT_DBG_NORMAL(BT_DEBUG_GAP, "Audio device and not capturing cod valid");
                        cod_valid = TRUE;
                        is_audio = TRUE;
                    }
                    else
                    {
                        BT_DBG_ERROR(BT_DEBUG_GAP, "Audio device and not capturing cod invalid");
                    }
                }
                if ((inquiry_type == 2) ||(inquiry_type == 8)) //Keyboard&&mouse
                {
                    if ((g_pt_result.cod&0x0500) == 0x0500)
                    {
                        /* keyboard&&mouse */
                        if (((g_pt_result.cod&0x40) == 0x40)||((g_pt_result.cod&0x80) == 0x80)||((g_pt_result.cod&0xC0) == 0xC0))
                        {
                            BT_DBG_NORMAL(BT_DEBUG_GAP, "keyboard&&mouse cod valid");
                            cod_valid = TRUE;
                        }
                        else
                        {
                            BT_DBG_ERROR(BT_DEBUG_GAP, "keyboard&&mouse cod invalid");
                        }
                    }
                }
                if (inquiry_type == 8) //full HID device
                {
                    if ((g_pt_result.cod & 0x0500) == 0x0500)
                    {
                        /* keyboard&&mouse */
                        if (((g_pt_result.cod & 0x40) == 0x40) || ((g_pt_result.cod & 0x80) == 0x80) || ((g_pt_result.cod & 0xC0) == 0xC0))
                        {
                            BT_DBG_NORMAL(BT_DEBUG_GAP, "keyboard && mouse cod valid");
                            cod_valid = TRUE;
                        }
                        else if ((g_pt_result.cod & 0x4) == 0x4)
                        {
                            BT_DBG_NORMAL(BT_DEBUG_GAP, "Joystick");
                            cod_valid = TRUE;
                        }
                        else if ((g_pt_result.cod & 0x8) == 0x8)
                        {
                            BT_DBG_NORMAL(BT_DEBUG_GAP, "gamepad");
                            cod_valid = TRUE;
                        }
                        else if ((g_pt_result.cod & 0xC) == 0xC)
                        {
                            BT_DBG_NORMAL(BT_DEBUG_GAP, "Remote control");
                            cod_valid = TRUE;
                        }
                        else if ((g_pt_result.cod & 0x10) == 0x10)
                        {
                            BT_DBG_NORMAL(BT_DEBUG_GAP, "Sensing device");
                            cod_valid = TRUE;
                        }
                        else
                        {
                            BT_DBG_ERROR(BT_DEBUG_GAP, "HID cod invalid");
                        }
                    }
                }
                break;
                /* 5 */
            case BT_PROPERTY_TYPE_OF_DEVICE:
                devicetype= (UINT32 *)property->val;

                /* 0 - BLE, 1 - BT, 2 - DUAL MODE */
                BT_DBG_INFO(BT_DEBUG_GAP, "devicetype = %ld", (unsigned long)*devicetype);
                break;
                /* 6 */
            case BT_PROPERTY_SERVICE_RECORD:
                break;
                /* 7 */
            case BT_PROPERTY_ADAPTER_SCAN_MODE:
                //mode = (bt_scan_mode_t *)property->val;
                break;
                /* 8 */
            case BT_PROPERTY_ADAPTER_BONDED_DEVICES:
                btaddr = (bt_bdaddr_t *)property->val;

                BT_DBG_NORMAL(BT_DEBUG_GAP, "[GAP] type = %ld, len = %ld, bonded_addr = %02X:%02X:%02X:%02X:%02X:%02X", (long)property->type, (long)property->len,
                              btaddr->address[0], btaddr->address[1], btaddr->address[2],
                              btaddr->address[3], btaddr->address[4], btaddr->address[5]);
                break;
                /* 9 */
            case BT_PROPERTY_ADAPTER_DISCOVERY_TIMEOUT:
                disc_timeout = (UINT32 *)property->val;

                BT_DBG_NORMAL(BT_DEBUG_GAP, "[GAP] type = %ld, len = %ld, disc_timeout = %ld", (long)property->type, (long)property->len, (unsigned long)*disc_timeout);
                break;
                /* 10 */
            case BT_PROPERTY_REMOTE_FRIENDLY_NAME:
                name = (CHAR *)property->val;
                BT_DBG_NORMAL(BT_DEBUG_GAP, "[GAP] type = %ld, len = %ld, bdname = %s", (long)property->type, (long)property->len, name);

                break;
                /* 11 */
            case BT_PROPERTY_REMOTE_RSSI:
                rssi = (INT16 *)property->val;
                *rssi -= 256;
                g_pt_result.rssi = *rssi - 256;
                BT_DBG_INFO(BT_DEBUG_GAP, "rssi = %ld", (long)g_pt_result.rssi);
                break;
                /* 12 */
            case BT_PROPERTY_REMOTE_VERSION_INFO:
                BT_DBG_NORMAL(BT_DEBUG_GAP, "[GAP] type = %ld, len = %ld, Others.", (long)property->type, (long)property->len);
                break;
                /* FF */
            case BT_PROPERTY_REMOTE_DEVICE_TIMESTAMP:
                BT_DBG_NORMAL(BT_DEBUG_GAP, "[GAP] type = %ld, len = %ld, Others.", (long)property->type, (long)property->len);
                break;
            default:
                BT_DBG_INFO(BT_DEBUG_GAP, "[GAP] type = %ld, len = %ld, Others.", (long)property->type, (long)property->len);
                break;
            }
        }
    }
    else//DS4 device
    {
        for (i = 0; i < num_properties; i++)
        {
            property = &properties[i];

            switch (property->type)
            {
                /* 1 */
            case BT_PROPERTY_BDNAME:
                name = (CHAR *)property->val;
                if (property->len < MAX_NAME_LEN)
                {
                    memcpy(g_pt_result.name, name, property->len);
                    g_pt_result.name[property->len] = '\0';
                }
                BT_DBG_INFO(BT_DEBUG_GAP, "name = %s", g_pt_result.name);
                break;
                /* 2 */
            case BT_PROPERTY_BDADDR:
                btaddr = (bt_bdaddr_t *)property->val;
                linuxbt_btaddr_htos(btaddr, g_pt_result.bdAddr);
                BT_DBG_INFO(BT_DEBUG_GAP, "bt addr: %s", g_pt_result.bdAddr);

                break;
                /* 3 */
            case BT_PROPERTY_UUIDS:
                //uuid = (bt_uuid_t*)property->val;
                //len = property->len;
                break;
                /* 4 */
            case BT_PROPERTY_CLASS_OF_DEVICE:
                cod = (UINT32 *)property->val;
                g_pt_result.cod = *cod;
                BT_DBG_NORMAL(BT_DEBUG_GAP, "cod = 0x%6x", (unsigned int)g_pt_result.cod);
                /* Peripheral */
                if ((g_pt_result.cod&0x2508) == 0x2508)
                {
                    BT_DBG_NORMAL(BT_DEBUG_GAP, "Peripheral Dualstock4 cod valid");
                    cod_valid = TRUE;
                }
                //uuid_valid = TRUE;
                break;
                /* 5 */
            case BT_PROPERTY_TYPE_OF_DEVICE:
                devicetype= (UINT32 *)property->val;

                /* 0 - BLE, 1 - BT, 2 - DUAL MODE */
                BT_DBG_INFO(BT_DEBUG_GAP, "devicetype = %ld", (unsigned long)*devicetype);
                break;
                /* 6 */
            case BT_PROPERTY_SERVICE_RECORD:
                break;
                /* 7 */
            case BT_PROPERTY_ADAPTER_SCAN_MODE:
                //mode = (bt_scan_mode_t *)property->val;
                break;
                /* 8 */
            case BT_PROPERTY_ADAPTER_BONDED_DEVICES:
                btaddr = (bt_bdaddr_t *)property->val;

                BT_DBG_NORMAL(BT_DEBUG_GAP, "[GAP] type = %ld, len = %ld, bonded_addr = %02X:%02X:%02X:%02X:%02X:%02X", (long)property->type, (long)property->len,
                              btaddr->address[0], btaddr->address[1], btaddr->address[2],
                              btaddr->address[3], btaddr->address[4], btaddr->address[5]);
                break;
                /* 9 */
            case BT_PROPERTY_ADAPTER_DISCOVERY_TIMEOUT:
                disc_timeout = (UINT32 *)property->val;

                BT_DBG_NORMAL(BT_DEBUG_GAP, "[GAP] type = %ld, len = %ld, disc_timeout = %ld", (long)property->type, (long)property->len, (unsigned long)*disc_timeout);
                break;
                /* 10 */
            case BT_PROPERTY_REMOTE_FRIENDLY_NAME:
                name = (CHAR *)property->val;
                BT_DBG_NORMAL(BT_DEBUG_GAP, "[GAP] type = %ld, len = %ld, bdname = %s", (long)property->type, (long)property->len, name);

                break;
                /* 11 */
            case BT_PROPERTY_REMOTE_RSSI:
                rssi = (INT16 *)property->val;
                g_pt_result.rssi = *rssi - 256;
                BT_DBG_INFO(BT_DEBUG_GAP, "rssi = %ld", (long)g_pt_result.rssi);
                break;
                /* 12 */
            case BT_PROPERTY_REMOTE_VERSION_INFO:
                BT_DBG_NORMAL(BT_DEBUG_GAP, "[GAP] type = %ld, len = %ld, Others.", (long)property->type, (long)property->len);
                break;
                /* FF */
            case BT_PROPERTY_REMOTE_DEVICE_TIMESTAMP:
                BT_DBG_NORMAL(BT_DEBUG_GAP, "[GAP] type = %ld, len = %ld, Others.", (long)property->type, (long)property->len);
                break;
            default:
                BT_DBG_INFO(BT_DEBUG_GAP, "[GAP] type = %ld, len = %ld, Others.", (long)property->type, (long)property->len);
                break;
            }
        }
    }
    //for scan target device
    if (0 < strlen(g_ps_target_mac))
    {
        if (0 == strncmp(g_pt_result.bdAddr, g_ps_target_mac, 17))
        {
            BT_DBG_NORMAL(BT_DEBUG_GAP, "found the target device:%s", g_pt_result.bdAddr);
            g_fg_bt_inquiry_done = TRUE;
            return;
        }
        BT_DBG_NORMAL(BT_DEBUG_GAP, "it isn't the target device:%s", g_pt_result.bdAddr);
    }
    //for scan all device
    else
    {
        BT_DBG_NORMAL(BT_DEBUG_GAP, "for scan all device");
    }

    if (!cod_valid)
    {
        BT_DBG_NORMAL(BT_DEBUG_GAP, "cod is invalid, update all_target_dev_info.");
        if (NULL !=btaddr)
        {
            bluetooth_gap_get_mapping_info(btaddr, num_properties, properties,&all_target_dev_info);
            bluetooth_gap_get_mapping_info(btaddr, num_properties, properties,&scan_target_dev_info);
        }
        return;
    }
    /*the discovered information is enough*/
    if (0 < strlen(g_pt_result.name))
    {
        if (is_audio)
        {
            BT_DBG_NORMAL(BT_DEBUG_GAP, "cod 0x%6x is audio device", (unsigned int)g_pt_result.cod);
        }
        else
        {
            BT_DBG_NORMAL(BT_DEBUG_GAP, "cod 0x%6x is not audio device", (unsigned int)g_pt_result.cod);
        }
    }
    else
    {
        BT_DBG_INFO(BT_DEBUG_GAP, "wait for discovery update");
    }
    if (NULL != btaddr)
    {
        bluetooth_gap_get_mapping_info(btaddr, num_properties, properties,&all_target_dev_info);
        bluetooth_gap_get_mapping_info(btaddr, num_properties, properties,&scan_target_dev_info);
        if(fg_bt_scan_ongoning)
            bluetooth_notify_device_info_app(btaddr, &scan_target_dev_info);
    }

}

/* FUNCTION NAME: bt_gap_handle_discovery_update_ind
 * PURPOSE:
 *      {handle_discovery_update}
 * INPUT:
 *      parameter-1  --
 * OUTPUT:
 *      parameter-n  --
 * RETURN:
 *      return-value-1 --
 * NOTES:
 *
 */
VOID bt_gap_handle_discovery_update_ind(bt_bdaddr_t *bd_addr, INT32 num_properties,bt_property_t *properties)
{
    INT32 i;
    INT32 inquiry_type = 0;
    BT_GAP_SCAN_RESULT g_pt_result;
    CHAR *name= NULL;
    bt_bdaddr_t *btaddr = NULL;
    bt_property_t *property= NULL;
    INT16 *rssi= NULL;
    UINT32 *cod= NULL;
    UINT32 *devicetype= NULL;
    UINT32 *disc_timeout= NULL;
    CHAR current_addr[MAX_BDADDR_LEN];
    BOOL is_audio = FALSE;
    name = (CHAR *)properties->val;

    memset(current_addr,0,MAX_BDADDR_LEN);
    memset(&g_pt_result, 0x0, sizeof(BT_GAP_SCAN_RESULT));
    linuxbt_btaddr_htos(bd_addr,current_addr);

    if (properties->len < MAX_NAME_LEN)
    {
        memset(g_pt_result.name,0,MAX_NAME_LEN);
        memcpy(g_pt_result.name, name, properties->len);
        linuxbt_btaddr_htos(bd_addr,g_pt_result.bdAddr);
        BT_DBG_INFO(BT_DEBUG_GAP, "name = %s", g_pt_result.name);
        BT_DBG_INFO(BT_DEBUG_GAP, "address = %s", g_pt_result.bdAddr);
        g_pt_result_number++;
    }

    if (BT_INQUIRY_FILTER_TYPE_DS4 == fg_bt_inquiry_type)//only DS4
    {
        inquiry_type = 1;
    }
    else if (BT_INQUIRY_FILTER_TYPE_HID== fg_bt_inquiry_type)//keyboard mouse
    {
        inquiry_type = 2;
    }
    else if (BT_INQUIRY_FILTER_TYPE_A2DP_SNK== fg_bt_inquiry_type)//A2DP SINK
    {
        inquiry_type = 4;
    }
    else if (BT_INQUIRY_FILTER_TYPE_A2DP_HID== fg_bt_inquiry_type)//A2DP SINK && keyboard mouse
    {
        inquiry_type = 8;
    }
    if (inquiry_type > 1)//[A2DP SINK] [keyboard mouse]
    {
        for (i = 0; i < num_properties; i++)
        {
            property = &properties[i];

            switch (property->type)
            {
                /* 1 */
            case BT_PROPERTY_BDNAME:
                name = (CHAR *)property->val;
                if (property->len < MAX_NAME_LEN)
                {
                    memcpy(g_pt_result.name, name, property->len);
                    g_pt_result.name[property->len] = '\0';
                }
                else
                {
                    memcpy(g_pt_result.name, name, MAX_NAME_LEN-1);
                    g_pt_result.name[MAX_NAME_LEN - 1] = '\0';
                }
                BT_DBG_INFO(BT_DEBUG_GAP, "name = %s", g_pt_result.name);

                break;
                /* 2 */
            case BT_PROPERTY_BDADDR:
                btaddr = (bt_bdaddr_t *)property->val;
                linuxbt_btaddr_htos(btaddr, g_pt_result.bdAddr);
                BT_DBG_INFO(BT_DEBUG_GAP, "bt addr: %s", g_pt_result.bdAddr);
                g_pt_result_number = num_properties - 1;
                break;
                /* 3 */
            case BT_PROPERTY_UUIDS:
                //uuid = (bt_uuid_t*)property->val;
                //len = property->len;
                break;
                /* 4 */
            case BT_PROPERTY_CLASS_OF_DEVICE:
                cod = (UINT32 *)property->val;
                g_pt_result.cod = *cod;
                BT_DBG_NORMAL(BT_DEBUG_GAP, "cod = 0x%6x", (unsigned int)g_pt_result.cod);
                if ((inquiry_type == 4) ||(inquiry_type == 8)) //A2DP
                {
                    /* Audio&Rending */
                    if ((g_pt_result.cod&0x00240400) == 0x00240400)
                    {
                        BT_DBG_NORMAL(BT_DEBUG_GAP, "Audio Rending cod valid");
                        //cod_valid = TRUE;
                        is_audio = TRUE;
                    }
                    else
                    {
                        BT_DBG_ERROR(BT_DEBUG_GAP, "Audio Rending cod invalid");
                    }
                    if (((g_pt_result.cod&0x00200400) == 0x00200400) && !((g_pt_result.cod&0x00080000) == 0x00080000))
                    {
                        BT_DBG_NORMAL(BT_DEBUG_GAP, "Audio device and not capturing cod valid");
                        //cod_valid = TRUE;
                        is_audio = TRUE;
                    }
                    else
                    {
                        BT_DBG_ERROR(BT_DEBUG_GAP, "Audio device and not capturing cod invalid");
                    }
                }
                if ((inquiry_type == 2) ||(inquiry_type == 8)) //Keyboard&&mouse
                {
                    /* keyboard&&mouse */
                    if ((g_pt_result.cod&0x0500) == 0x0500)
                    {
                        if (((g_pt_result.cod&0x40) == 0x40)||((g_pt_result.cod&0x80) == 0x80)||((g_pt_result.cod&0xC0) == 0xC0))
                        {
                            BT_DBG_NORMAL(BT_DEBUG_GAP, "keyboard&&mouse cod valid");
                            //cod_valid = TRUE;
                        }
                        else
                        {
                            BT_DBG_ERROR(BT_DEBUG_GAP, "keyboard&&mouse cod invalid");
                        }
                    }
                }
                break;
                /* 5 */
            case BT_PROPERTY_TYPE_OF_DEVICE:
                devicetype= (UINT32 *)property->val;

                /* 0 - BLE, 1 - BT, 2 - DUAL MODE */
                BT_DBG_INFO(BT_DEBUG_GAP, "devicetype = %ld", (unsigned long)*devicetype);
                break;
                /* 6 */
            case BT_PROPERTY_SERVICE_RECORD:
                break;
                /* 7 */
            case BT_PROPERTY_ADAPTER_SCAN_MODE:
                //mode = (bt_scan_mode_t *)property->val;
                break;
                /* 8 */
            case BT_PROPERTY_ADAPTER_BONDED_DEVICES:
                btaddr = (bt_bdaddr_t *)property->val;

                BT_DBG_NORMAL(BT_DEBUG_GAP, "[GAP] type = %ld, len = %ld, bonded_addr = %02X:%02X:%02X:%02X:%02X:%02X", (long)property->type, (long)property->len,
                              btaddr->address[0], btaddr->address[1], btaddr->address[2],
                              btaddr->address[3], btaddr->address[4], btaddr->address[5]);
                break;
                /* 9 */
            case BT_PROPERTY_ADAPTER_DISCOVERY_TIMEOUT:
                disc_timeout = (UINT32 *)property->val;

                BT_DBG_NORMAL(BT_DEBUG_GAP, "[GAP] type = %ld, len = %ld, disc_timeout = %ld", (long)property->type, (long)property->len, (unsigned long)*disc_timeout);
                break;
                /* 10 */
            case BT_PROPERTY_REMOTE_FRIENDLY_NAME:
                name = (CHAR *)property->val;
                BT_DBG_NORMAL(BT_DEBUG_GAP, "[GAP] type = %ld, len = %ld, bdname = %s", (long)property->type, (long)property->len, name);

                break;
                /* 11 */
            case BT_PROPERTY_REMOTE_RSSI:
                rssi = (INT16 *)property->val;
                g_pt_result.rssi = *rssi - 256;
                BT_DBG_INFO(BT_DEBUG_GAP, "rssi = %ld", (long)g_pt_result.rssi);
                break;
                /* 12 */
            case BT_PROPERTY_REMOTE_VERSION_INFO:
                BT_DBG_NORMAL(BT_DEBUG_GAP, "[GAP] type = %ld, len = %ld, Others.", (long)property->type, (long)property->len);
                break;
                /* FF */
            case BT_PROPERTY_REMOTE_DEVICE_TIMESTAMP:
                BT_DBG_NORMAL(BT_DEBUG_GAP, "[GAP] type = %ld, len = %ld, Others.", (long)property->type, (long)property->len);
                break;
            default:
                BT_DBG_INFO(BT_DEBUG_GAP, "[GAP] type = %ld, len = %ld, Others.", (long)property->type, (long)property->len);
                break;
            }
        }
    }
    else//DS4
    {
        for (i = 0; i < num_properties; i++)
        {
            property = &properties[i];

            switch (property->type)
            {
                /* 1 */
            case BT_PROPERTY_BDNAME:
                name = (CHAR *)property->val;
                if (property->len < MAX_NAME_LEN)
                {
                    memcpy(g_pt_result.name, name, property->len);
                    g_pt_result.name[property->len] = '\0';
                }
                else
                {
                    memcpy(g_pt_result.name, name, MAX_NAME_LEN-1);
                    g_pt_result.name[MAX_NAME_LEN - 1] = '\0';
                }

                BT_DBG_INFO(BT_DEBUG_GAP, "name = %s", g_pt_result.name);
                break;
                /* 2 */
            case BT_PROPERTY_BDADDR:
                btaddr = (bt_bdaddr_t *)property->val;
                linuxbt_btaddr_htos(btaddr, g_pt_result.bdAddr);
                BT_DBG_INFO(BT_DEBUG_GAP, "bt addr: %s", g_pt_result.bdAddr);

                break;
                /* 3 */
            case BT_PROPERTY_UUIDS:
                //uuid = (bt_uuid_t*)property->val;
                //len = property->len;
                break;
                /* 4 */
            case BT_PROPERTY_CLASS_OF_DEVICE:
                cod = (UINT32 *)property->val;
                g_pt_result.cod = *cod;
                BT_DBG_NORMAL(BT_DEBUG_GAP, "cod = 0x%6x", (unsigned int)g_pt_result.cod);
                /* Peripheral */
                if ((g_pt_result.cod&0x2508) == 0x2508)
                {
                    BT_DBG_NORMAL(BT_DEBUG_GAP, "Peripheral Dualstock4 cod valid");
                    //cod_valid = TRUE;
                }
                //uuid_valid = TRUE;
                break;
                /* 5 */
            case BT_PROPERTY_TYPE_OF_DEVICE:
                devicetype= (UINT32 *)property->val;

                /* 0 - BLE, 1 - BT, 2 - DUAL MODE */
                BT_DBG_INFO(BT_DEBUG_GAP, "devicetype = %ld", (unsigned long)*devicetype);
                break;
                /* 6 */
            case BT_PROPERTY_SERVICE_RECORD:
                break;
                /* 7 */
            case BT_PROPERTY_ADAPTER_SCAN_MODE:
                //mode = (bt_scan_mode_t *)property->val;
                break;
                /* 8 */
            case BT_PROPERTY_ADAPTER_BONDED_DEVICES:
                btaddr = (bt_bdaddr_t *)property->val;

                BT_DBG_NORMAL(BT_DEBUG_GAP, "[GAP] type = %ld, len = %ld, bonded_addr = %02X:%02X:%02X:%02X:%02X:%02X", (long)property->type, (long)property->len,
                              btaddr->address[0], btaddr->address[1], btaddr->address[2],
                              btaddr->address[3], btaddr->address[4], btaddr->address[5]);
                break;
                /* 9 */
            case BT_PROPERTY_ADAPTER_DISCOVERY_TIMEOUT:
                disc_timeout = (UINT32 *)property->val;

                BT_DBG_NORMAL(BT_DEBUG_GAP, "[GAP] type = %ld, len = %ld, disc_timeout = %ld", (long)property->type, (long)property->len, (unsigned long)*disc_timeout);
                break;
                /* 10 */
            case BT_PROPERTY_REMOTE_FRIENDLY_NAME:
                name = (CHAR *)property->val;
                BT_DBG_NORMAL(BT_DEBUG_GAP, "[GAP] type = %ld, len = %ld, bdname = %s", (long)property->type, (long)property->len, name);

                break;
                /* 11 */
            case BT_PROPERTY_REMOTE_RSSI:
                rssi = (INT16 *)property->val;
                g_pt_result.rssi = *rssi - 256;
                BT_DBG_INFO(BT_DEBUG_GAP, "rssi = %ld", (long)g_pt_result.rssi);
                break;
                /* 12 */
            case BT_PROPERTY_REMOTE_VERSION_INFO:
                BT_DBG_NORMAL(BT_DEBUG_GAP, "[GAP] type = %ld, len = %ld, Others.", (long)property->type, (long)property->len);
                break;
                /* FF */
            case BT_PROPERTY_REMOTE_DEVICE_TIMESTAMP:
                BT_DBG_NORMAL(BT_DEBUG_GAP, "[GAP] type = %ld, len = %ld, Others.", (long)property->type, (long)property->len);
                break;
            default:
                BT_DBG_INFO(BT_DEBUG_GAP, "[GAP] type = %ld, len = %ld, Others.", (long)property->type, (long)property->len);
                break;
            }
        }
    }

    /*the discovered information is enough*/
    if (0 < strlen(g_pt_result.name))
    {
        if (is_audio)
        {
            BT_DBG_NORMAL(BT_DEBUG_GAP, "cod 0x%6x is audio device", (unsigned int)g_pt_result.cod);
        }
        else
        {
            BT_DBG_NORMAL(BT_DEBUG_GAP, "cod 0x%6x is not audio device", (unsigned int)g_pt_result.cod);
        }
    }
    else
    {
        BT_DBG_INFO(BT_DEBUG_GAP, "wait for discovery update");
    }
    if (NULL != bd_addr)
    {
        bluetooth_gap_get_mapping_info(bd_addr, num_properties, properties, &all_target_dev_info);
        bluetooth_gap_get_mapping_info(bd_addr, num_properties, properties, &scan_target_dev_info);
        if (fg_bt_scan_ongoning)
            bluetooth_notify_device_info_app(bd_addr, &scan_target_dev_info);
    }
}

/* FUNCTION NAME: bt_gap_acl_disconnect_reason_ck
 * PURPOSE:
 *      { to print the reason of disconnect }
 * INPUT:
 *      parameter-1  --
 * OUTPUT:
 *      parameter-n  --
 * RETURN:
 *      return-value-1 --
 * NOTES:
 *
 */
VOID bt_gap_acl_disconnect_reason_ck(bt_bdaddr_t *bd_addr, UINT8 reason)
{
    BT_DBG_NORMAL(BT_DEBUG_GAP, "reason:%ld, get_bt_power_on() = %d", (unsigned long)reason, get_bt_power_on());

    CHAR str_bt_addr[MAX_BDADDR_LEN];
    memset(str_bt_addr, 0, MAX_BDADDR_LEN);
    linuxbt_btaddr_htos(bd_addr, str_bt_addr);

    if (0x8 == reason)
    {
        BT_DBG_ERROR(BT_DEBUG_GAP, "connection lost");
        /*link supervision timeout has expired for a given connection*/
        BtAppCbk(BT_CONNECT_MISSING, str_bt_addr);
    }
    else if (0x13 == reason)
    {
        /*user on the remote device terminate the connection*/
    }
    else if (0x16 == reason)
    {
        /*the local device terminate the connection*/
    }
    else
    {
        /*other case terminate the connection, should handle this case due to IOP test,
                 in case of some device set ACL disconnect reason not correct.*/

    }
}

/* FUNCTION NAME: bt_gap_handle_power_on_cnf
 * PURPOSE:
 *      { to handle power on }
 * INPUT:
 *      parameter-1  --
 * OUTPUT:
 *      parameter-n  --
 * RETURN:
 *      return-value-1 --
 * NOTES:
 *
 */
VOID bt_gap_handle_power_on_cnf(VOID)
{
    FUNC_ENTRY;
    /*for clear disconnection notify flag*/
    set_bt_power_on(TRUE);
    set_bt_is_call_power_off(FALSE);

    BtAppCbk(BT_POWER_ON, NULL);
}

/* FUNCTION NAME: bt_gap_handle_power_off_cnf
 * PURPOSE:
 *      {to handle gap power off}
 * INPUT:
 *      parameter-1  --
 * OUTPUT:
 *      parameter-n  --
 * RETURN:
 *      return-value-1 --
 * NOTES:
 *
 */
VOID bt_gap_handle_power_off_cnf(VOID)
{
    FUNC_ENTRY;
    /*for clear disconnection notify flag*/
    set_bt_power_on(FALSE);
    set_bt_is_call_power_off(TRUE);
    set_bt_mode(BT_MODE_CLOSE);
    BtAppCbk(BT_EXIT, NULL);
}

/* FUNCTION NAME: bt_gap_handle_get_properties_cnf
 * PURPOSE:
 *      { handle_get_properties }
 * INPUT:
 *      parameter-1  --
 * OUTPUT:
 *      parameter-n  --
 * RETURN:
 *      return-value-1 --
 * NOTES:
 *
 */
VOID bt_gap_handle_get_properties_cnf(BT_GAP_LOCAL_PROPERTIES_RESULT *localProperty)
{
    BT_DBG_INFO(BT_DEBUG_GAP, "+++into");
    memcpy(&g_localProperty, localProperty, sizeof(BT_GAP_LOCAL_PROPERTIES_RESULT));
    BT_DBG_INFO(BT_DEBUG_GAP, "bdaddr %s", g_localProperty.bdAddr);
    BT_DBG_INFO(BT_DEBUG_GAP, "name %s", g_localProperty.name);
    BT_DBG_INFO(BT_DEBUG_GAP, "powered %s", g_localProperty.powered?"on":"off");
    BT_DBG_INFO(BT_DEBUG_GAP, "scan_mode %ld", (unsigned long)g_localProperty.scan_mode);
    BT_DBG_INFO(BT_DEBUG_GAP, "disctimeout %ld", (long)g_localProperty.disctimeout);
    set_bt_get_dev_info_done(TRUE);
}

/* FUNCTION NAME: bt_gap_handle_set_scanmode_cnf
 * PURPOSE:
 *      { handle_set_scanmode }
 * INPUT:
 *      parameter-1  --
 * OUTPUT:
 *      parameter-n  --
 * RETURN:
 *      return-value-1 --
 * NOTES:
 *
 */
VOID bt_gap_handle_set_scanmode_cnf(bt_status_t status, bt_property_t *properties)
{
    BT_DBG_INFO(BT_DEBUG_GAP, "+++into");
    bt_scan_mode_t *mode;

    if (NULL == properties)
    {
        BT_DBG_ERROR(BT_DEBUG_GAP, "null pointer of properties");
        return;
    }

    if (BT_PROPERTY_ADAPTER_SCAN_MODE == properties->type)
    {
        if (BT_STATUS_SUCCESS == status)
        {
            mode = (bt_scan_mode_t *)properties->val;
            BT_DBG_NORMAL(BT_DEBUG_GAP, "[GAP]scan mode = %ld", (long)*mode);
            set_bt_set_scanmode_done(TRUE);
        }
        else
        {
            BT_DBG_ERROR(BT_DEBUG_GAP, "set scan mode failed");
        }
    }

}

/* FUNCTION NAME: bt_gap_handle_link_state_ind
 * PURPOSE:
 *      { the link state state indicate }
 * INPUT:
 *      parameter-1  --
 * OUTPUT:
 *      parameter-n  --
 * RETURN:
 *      return-value-1 --
 * NOTES:
 *
 */
VOID bt_gap_handle_link_state_ind(bt_status_t status,
                                  bt_bdaddr_t *remote_bd_addr,
                                  bt_acl_state_t state)
{
    CHAR address[MAX_BDADDR_LEN];

    memset(address, 0, sizeof(address));
    linuxbt_btaddr_htos(remote_bd_addr, address);
    BT_DBG_NORMAL(BT_DEBUG_GAP, "address=%s", address);

    switch (state)
    {
    case BT_ACL_STATE_CONNECTED:
        BT_DBG_NORMAL(BT_DEBUG_GAP, "acl is connected.");
        if (!get_bt_a2dp_connect() && BtAppCbk)
        {
            BtAppCbk(BT_CONNECT_COMING, address);
        }
        else
        {
        }
        break;
    case BT_ACL_STATE_DISCONNECTED:
        BT_DBG_NORMAL(BT_DEBUG_GAP, "acl is disconnected.");
        break;
    default:
        break;
    }

}

/* FUNCTION NAME: bt_gap_get_rssi_result_cb
 * PURPOSE:
 *      { get rssi result callback }
 * INPUT:
 *      parameter-1  --
 * OUTPUT:
 *      parameter-n  --
 * RETURN:
 *      return-value-1 --
 * NOTES:
 *
 */
VOID bt_gap_get_rssi_result_cb(INT16 rssi_value)
{

    BT_DBG_NORMAL(BT_DEBUG_GAP, "rssi value: %ld", (long)rssi_value);

    set_bt_get_rssi_done(TRUE);
    bluetooth_set_rssi_value(rssi_value);
}


/* FUNCTION NAME: bt_gap_get_pin_code_cb
 * PURPOSE:
 *      { get pin code callback }
 * INPUT:
 *      parameter-1  -- pin code
 * OUTPUT:
 *      parameter-n  -- NULL
 * RETURN:
 *      return-value-1 -- NULL
 * NOTES:
 *
 */
#if ENABLE_HERCULES
void bt_gap_get_pin_code_cb(bt_bdaddr_t *remote_bd_addr, CHAR *name, bt_pin_code_t *pin, UINT8 *fg_accept)
#else
VOID bt_gap_get_pin_code_cb(bt_pin_code_t *pin, UINT8 *fg_accept)
#endif
{
    FUNC_ENTRY;
    pairing_key_value_t t_pairing_key_struct;
    memset(&t_pairing_key_struct, 0, sizeof(pairing_key_value_t));

#if ENABLE_HERCULES
    CHAR address[18];
    memset(address, 0, sizeof(address));
    linuxbt_btaddr_htos(remote_bd_addr, address);
    BT_DBG_NORMAL(BT_DEBUG_GAP, "address=%s\n", address);
    BT_DBG_NORMAL(BT_DEBUG_GAP, "name=%s\n", name);
    strncpy(t_pairing_key_struct.bd_addr, address, MAX_BDADDR_LEN);
    strncpy(t_pairing_key_struct.bd_name, name, strlen(name));
#endif

    t_pairing_key_struct.key_type = PIN_CODE;

    BT_DBG_NORMAL(BT_DEBUG_GAP, "pin=%s.\n", pin);
    memcpy(t_pairing_key_struct.pin_code, pin, sizeof(t_pairing_key_struct.pin_code));
    BT_DBG_NORMAL(BT_DEBUG_GAP, "t_pairing_key_struct.pin_code=%s.\n", t_pairing_key_struct.pin_code);

    if(AppGetPairingKeyCbk != NULL)
    {
        AppGetPairingKeyCbk(&t_pairing_key_struct, fg_accept);
    }
    else
    {
        BT_DBG_NORMAL(BT_DEBUG_GAP, "AppGetPairingKeyCbk not register.\n");
    }
}


/* FUNCTION NAME: bt_gap_get_passkey_cb
 * PURPOSE:
 *      { get passkey callback }
 * INPUT:
 *      parameter-1  -- passkey
 * OUTPUT:
 *      parameter-n  -- NULL
 * RETURN:
 *      return-value-1 -- NULL
 * NOTES:
 *
 */
#if ENABLE_HERCULES
void bt_gap_get_pass_key_cb(bt_bdaddr_t *remote_bd_addr, CHAR *name, UINT32 pass_key, UINT8 *fg_accept)
#else
void bt_gap_get_pass_key_cb(UINT32 pass_key, UINT8 *fg_accept)
#endif
{
    FUNC_ENTRY;
    pairing_key_value_t t_pairing_key_struct;
    memset(&t_pairing_key_struct, 0, sizeof(pairing_key_value_t));

#if ENABLE_HERCULES
    CHAR address[18];
    memset(address, 0, sizeof(address));
    linuxbt_btaddr_htos(remote_bd_addr, address);
    BT_DBG_NORMAL(BT_DEBUG_GAP, "address=%s\n", address);
    BT_DBG_NORMAL(BT_DEBUG_GAP, "name=%s\n", name);

    strncpy(t_pairing_key_struct.bd_addr, address, MAX_BDADDR_LEN);
    strncpy(t_pairing_key_struct.bd_name, name, strlen(name));
#endif

    t_pairing_key_struct.key_type = PASSKEY;
    t_pairing_key_struct.key_value = pass_key;
    if(AppGetPairingKeyCbk != NULL)
    {
        AppGetPairingKeyCbk(&t_pairing_key_struct, fg_accept);
    }
    else
    {
        BT_DBG_NORMAL(BT_DEBUG_GAP, "AppGetPairingKeyCbk not register.\n");
    }
}


VOID bluetooth_history_remove_unpair_device(bt_bdaddr_t *remote_bd_addr)
{
    FUNC_ENTRY;
    INT32 i4_temp = BT_SUCCESS;
    CHAR address[18];

    memset(address, 0, sizeof(address));
    linuxbt_btaddr_htos(remote_bd_addr, address);
    BT_DBG_NORMAL(BT_DEBUG_GAP, "address=%s", address);
#if ENABLE_HID_PROFILE_H
    i4_temp = bluetooth_hid_removeHistory_one(address, g_pt_paired_hid_dev_history);
    if (BT_SUCCESS != i4_temp)
    {
        BT_DBG_ERROR(BT_DEBUG_HID, "HID paired list cannot delete the device:%s", address);
    }

    i4_temp = bluetooth_hid_removeStatus_one(address, g_pt_hid_status_list);
    if (BT_SUCCESS != i4_temp)
    {
        BT_DBG_ERROR(BT_DEBUG_HID, "HID status list cannot delete the device:%s", address);
    }
#endif

}

VOID bt_gap_handle_bond_state_cb(bt_bond_state_t state, bt_bdaddr_t *remote_bd_addr)
{
    CHAR address[18];
    BT_BOND_STATUS_STRUCT bond_struct;
    memset(&bond_struct, 0, sizeof(BT_BOND_STATUS_STRUCT));
    memset(address, 0, sizeof(address));

    if (NULL == remote_bd_addr)
    {
        BT_DBG_ERROR(BT_DEBUG_GAP, "address = NULL\n");
    }

    linuxbt_btaddr_htos(remote_bd_addr, address);
    BT_DBG_NORMAL(BT_DEBUG_GAP, "address=%s\n", address);

    strncpy(bond_struct.bdAddr, address, MAX_BDADDR_LEN);

    switch (state)
    {
    case BT_BOND_STATE_NONE:
        bond_struct.status = BT_UNBOND;
        break;
    case BT_BOND_STATE_BONDING:
        bond_struct.status = BT_BONDING;
        break;
    case BT_BOND_STATE_BONDED:
        bond_struct.status = BT_BONDED;
        break;
    default:
        BT_DBG_ERROR(BT_DEBUG_GAP, "Unknown bond state=%d\n", state);
        break;
    }

    if(AppBondCbk != NULL)
    {
        AppBondCbk(&bond_struct);
    }
    else
    {
        BT_DBG_ERROR(BT_DEBUG_GAP, "AppBondCbk not register.\n");
    }
}

UINT8 hci_res_event[128]= {0};
EXPORT_SYMBOL BOOL hci_res_flag = FALSE;
INT32 hci_cmd_flag = 0;

/* FUNCTION NAME: bt_gap_hci_response_cb
 * PURPOSE:
 *      {hci_response_cb}
 * INPUT:
 *      parameter-1  --
 * OUTPUT:
 *      parameter-n  --
 * RETURN:
 *      return-value-1 --
 * NOTES:
 *
 */
VOID bt_gap_hci_response_cb(UINT8 *event, INT32 event_length)
{
    FUNC_ENTRY;
    memcpy(hci_res_event,(CHAR *)event, event_length);
    hci_res_flag = TRUE;
    if (1 == hci_cmd_flag)
    {
#if 0
        if (!strncmp(hci_res_event,"0E 04 01 03 0C 00",strlen("0E 04 01 03 0C 00")))
        {
            /*notify APP OK*/
            BtAppCbk(BT_RF_TEST_INITIALIZE_OK);
        }
        else
        {
            /*notify APP NG*/
            BtAppCbk(BT_RF_TEST_INITIALIZE_NG);
        }
#endif
    }
}

/* FUNCTION NAME: bt_gap_get_self_rssi_value
 * PURPOSE:
 *      { get_self_rssi_value }
 * INPUT:
 *      parameter-1  --
 * OUTPUT:
 *      parameter-n  --
 * RETURN:
 *      return-value-1 --
 * NOTES:
 *
 */
INT32 bt_gap_get_self_rssi_value(INT16 *rssi_value)
{
    if (NULL == rssi_value)
    {
        BT_DBG_ERROR(BT_DEBUG_GAP, "the null pointer of rssi_value");
        return BT_ERR_STATUS_FAIL;
    }
    else if (TRUE == hci_res_flag)
    {
        UINT8 i = 0;
        for (i=0;i<5;i++)
        {
            BT_DBG_INFO(BT_DEBUG_GAP, "%2d", hci_res_event[i]);
        }
        BT_DBG_ERROR(BT_DEBUG_A2DP,"");
        if (0x00 == hci_res_event[0]
                && 0xB9 == hci_res_event[1]
                && 0xFC == hci_res_event[2]
                && 0x00 == hci_res_event[3])
        {
            *rssi_value = (UINT8)hci_res_event[4]-256;
            BT_DBG_NORMAL(BT_DEBUG_GAP, "the self rssi value is: %ld", (long)*rssi_value);
        }
        else
        {
            BT_DBG_ERROR(BT_DEBUG_GAP, "cannot get self rssi value");
            return BT_ERR_STATUS_FAIL;
        }
    }
    else
    {
        BT_DBG_ERROR(BT_DEBUG_GAP, "should send get self rssi value first");
        return BT_ERR_STATUS_FAIL;
    }
    return BT_SUCCESS;
}

/* FUNCTION NAME: send_buffer_to_hci
 * PURPOSE:
 *      { send buffer to hci }
 * INPUT:
 *      parameter-1  --
 * OUTPUT:
 *      parameter-n  --
 * RETURN:
 *      return-value-1 --
 * NOTES:
 *
 */
INT32 send_buffer_to_hci(CHAR* buffer)
{
    INT32 time = 10;
    hci_res_flag = FALSE;

    linuxbtgap_send_hci_handler(buffer);
    while ((FALSE == hci_res_flag) && time)
    {
        BT_DBG_INFO(BT_DEBUG_GAP, "Wait for HCI command reponse");
        usleep(500);
        time--;
    }
    return 1;
}



/* FUNCTION NAME: bluetooth_set_afh
 * PURPOSE:
 *      { set afh }
 * INPUT:
 *      parameter-1  --
 * OUTPUT:
 *      parameter-n  --
 * RETURN:
 *      return-value-1 --
 * NOTES:
 *
 */
INT32 bluetooth_set_afh(UINT8* mask)
{
    FUNC_ENTRY;
    BT_CHECK_POINTER(BT_DEBUG_GAP, mask);
    BT_DBG_NORMAL(BT_DEBUG_GAP, "<<< call bt_set_afh >>>");
    linuxbt_gap_set_afh(mask);

    return BT_SUCCESS;
}

INT32 bluetooth_get_bonded_device(VOID)
{
    FUNC_ENTRY;
    memset(&bond_target_dev_info, 0, sizeof(bond_target_dev_info));
    bluetooth_get_properties();
    return BT_SUCCESS;
}

#if defined(MTK_LINUX_GAP) && (MTK_LINUX_GAP == TRUE)
/* FUNCTION NAME: bluetooth_config_clear
 * PURPOSE:
 *      { clear file bluetooth bt_config.conf }
 * INPUT:
 *      parameter-1  --
 * OUTPUT:
 *      parameter-n  --
 * RETURN:
 *      return-value-1 --
 * NOTES:
 *
 */
INT32 bluetooth_config_clear(VOID)
{
    FUNC_ENTRY;
    linuxbt_gap_config_clear();
    return BT_SUCCESS;
}

/* FUNCTION NAME: bluetooth_dump
 * PURPOSE:
 *      { generate dump.txt in 3rd_rw/bluedroid/ }
 * INPUT:
 *      parameter-1  --
 * OUTPUT:
 *      parameter-n  --
 * RETURN:
 *      return-value-1 --
 * NOTES:
 *
 */
INT32 bluetooth_dump(CHAR *mode)
{
    FUNC_ENTRY;
    linuxbt_dump(mode);
    return BT_SUCCESS;
}

/* FUNCTION NAME: bluetooth_database_add
 * PURPOSE:
 *      { add interop_database }
 * INPUT:
 *      parameter-1  --
 * OUTPUT:
 *      parameter-n  --
 * RETURN:
 *      return-value-1 --
 * NOTES:
 *
 */
INT32 bluetooth_database_add(UINT16 feature, bt_bdaddr_t *remote_bd_addr, size_t len)
{
    FUNC_ENTRY;
    linuxbt_interop_database_add((uint16_t)feature, remote_bd_addr, len);
    return BT_SUCCESS;
}

/* FUNCTION NAME: bluetooth_database_clear
 * PURPOSE:
 *      { clear interop_database }
 * INPUT:
 *      parameter-1  --
 * OUTPUT:
 *      parameter-n  --
 * RETURN:
 *      return-value-1 --
 * NOTES:
 *
 */
INT32 bluetooth_database_clear(VOID)
{
    FUNC_ENTRY;
    linuxbt_interop_database_clear();
    return BT_SUCCESS;
}

/* FUNCTION NAME: bluetooth_ble_pair
 * PURPOSE:
 *      { ble pair }
 * INPUT:
 *      parameter-1  --
 * OUTPUT:
 *      parameter-n  --
 * RETURN:
 *      return-value-1 --
 * NOTES:
 *
 */
INT32 bluetooth_ble_pair(CHAR *pbt_addr)
{
    FUNC_ENTRY;
    linuxbt_ble_pair(pbt_addr);
    return BT_SUCCESS;
}

/* FUNCTION NAME: bluetooth_get_device_type
 * PURPOSE:
 *      { ble pair }
 * INPUT:
 *      parameter-1  --
 * OUTPUT:
 *      parameter-n  --
 * RETURN:
 *      return-value-1 --
 * NOTES:
 *
 */
INT32 bluetooth_get_device_type(CHAR *pbt_addr)
{
    FUNC_ENTRY;
    INT32 i = 0;
    for (i = 0; i < all_target_dev_info.dev_num; i++)
    {
        if (strncmp(pbt_addr, all_target_dev_info.mapping_list[i].bdAddr, strlen(all_target_dev_info.mapping_list[i].bdAddr)))
        {
            continue;
        }
        else
        {
            BT_DBG_NORMAL(BT_DEBUG_COMM,"bt dev_type = 0x%02X", (unsigned int)all_target_dev_info.mapping_list[i].devicetype);
            return all_target_dev_info.mapping_list[i].devicetype;
        }
    }
    return BT_ERR_STATUS_FAIL;
}

#endif

INT32 bt_gap_handle_discovery_stop_ind()
{
    FUNC_ENTRY;
    fg_bt_scan_ongoning = FALSE;
    BT_DBG_NORMAL(BT_DEBUG_GAP,"discovery_stop\n");

    if (NULL != BtAppCbk)
    {
        BtAppCbk(BT_DISCOVERY_STOP, NULL);
    }
    else
    {
        BT_DBG_ERROR(BT_DEBUG_GAP, "BtAppCbk NULL!\n");
    }

    return BT_SUCCESS;
}


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
/* FILE NAME:  c_bt_mw_gap.c
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
 *
 *
 */
/* INCLUDE FILE DECLARATIONS
 */
/* NAMING CONSTANT DECLARATIONS
 */
/* MACRO FUNCTION DECLARATIONS
 */
/* DATA TYPE DECLARATIONS
 */
/* GLOBAL VARIABLE DECLARATIONS
 */
/* LOCAL SUBPROGRAM DECLARATIONS
 */
/* STATIC VARIABLE DECLARATIONS
 */
/* EXPORTED SUBPROGRAM BODIES
 */
/* LOCAL SUBPROGRAM BODIES
 */
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <linux/input.h>
#include <linux/uinput.h>

#include "bluetooth.h"

#include "bt_mw_common.h"
#include "bt_mw_gap.h"
#include "bt_mw_devices_info.h"
#include "c_bt_mw_manager.h"
#include "c_bt_mw_gap.h"
#include "c_mw_config.h"
#include "bluetooth_sync.h"

extern INT32 g_rssi_value; // seem no used
extern INT32 g_i4restart_counter;
extern BtAppEventCbk      BtAppCbk;
extern BT_GAP_LOCAL_PROPERTIES_RESULT g_localProperty;
extern BtAppGapInquiryResponseCbk AppInquiryCbk;

extern BT_DEV_MAPPING_LIST all_target_dev_info;
BT_DEV_MAPPING_LIST scan_target_dev_info = {0};
/*notify to app*/
BT_DEV_MAPPING_LIST notify_target_dev_info = {0};


/* FUNCTION NAME: c_btm_set_dbg_level
 * PURPOSE:
 *      { set mw debug level }
 * INPUT:
 *      layer -- debug layer 0~8 means single layer, 9 means all layer
 *      level  -- debug level 0~5
 * OUTPUT:
 *      NONE
 * RETURN:
 *      VOID
 * NOTES:
 *
 */
EXPORT_SYMBOL INT32 c_btm_set_dbg_level(BT_DEBUG_LAYER_NAME_T layer, INT32 level)
{
    bt_set_dbg_level(layer, level);
    return BT_SUCCESS;
}

/* FUNCTION NAME: c_btm_set_local_name
 * PURPOSE:
 *      { set local device name, need power on bluetooth first }
 * INPUT:
 *      parameter-1  --
 * OUTPUT:
 *      parameter-n  --
 * RETURN:
 *      return-value-1 --
 * NOTES:
 *
 */
EXPORT_SYMBOL INT32 c_btm_set_local_name(CHAR *name)
{
    FUNC_ENTRY;
    if (NULL == name)
    {
        BT_DBG_ERROR(BT_DEBUG_GAP, "null pointer of name!");
        return BT_ERR_STATUS_FAIL;
    }
    BT_DBG_NORMAL(BT_DEBUG_GAP, "the name is: %s", name);
    strncpy(g_localProperty.name, name, sizeof(g_localProperty.name));
    bluetooth_set_local_dev_name(name);
    return BT_SUCCESS;
}

/* FUNCTION NAME: c_btm_pair
 * PURPOSE:
 *      {start to pair with SRC device(eg. cell phone)}
 * INPUT:
 *      parameter-1  --
 * OUTPUT:
 *      parameter-n  --
 * RETURN:
 *      return-value-1 --
 * NOTES:
 *
 */
EXPORT_SYMBOL INT32 c_btm_pair(CHAR *addr, int transport)
{
    FUNC_ENTRY;

    bluetooth_pair(addr, transport);
    BT_DBG_NORMAL(BT_DEBUG_GAP, "the MAC is: %s", addr);
    return BT_SUCCESS;
}

/* FUNCTION NAME: c_btm_paired_dev_erase
 * PURPOSE:
 *      { remove the target  paired device information(eg. cell phone), actually it's unpair }
 * INPUT:
 *      parameter-1  --
 * OUTPUT:
 *      parameter-n  --
 * RETURN:
 *      return-value-1 --
 * NOTES:
 *
 */
EXPORT_SYMBOL INT32 c_btm_paired_dev_erase(CHAR *addr)
{
    FUNC_ENTRY;
    BT_DBG_NORMAL(BT_DEBUG_GAP, "the MAC is: %s", addr);
    bluetooth_remove_paired_dev(addr);
    return BT_SUCCESS;
}

/* FUNCTION NAME: c_btm_get_rssi
 * PURPOSE:
 *      { get rssi }
 * INPUT:
 *      parameter-1  --
 * OUTPUT:
 *      parameter-n  --
 * RETURN:
 *      return-value-1 --
 * NOTES:
 *
 */
EXPORT_SYMBOL INT32 c_btm_get_rssi(CHAR *address, INT16 *rssi_value)
{
    BT_DBG_MINOR(BT_DEBUG_GAP, "+++into %s", __FUNCTION__);
    UINT32 ui4_loop = 5;

    if (get_bt_is_call_power_off())
    {
        BT_DBG_NORMAL(BT_DEBUG_GAP, "have call power off");
        return BT_SUCCESS;
    }
    bluetooth_send_get_rssi(address);
    /*wait response from stack*/
    while (!get_bt_get_rssi_done() && (0 < ui4_loop))
    {
        /*x_thread_delay(200);*/
        usleep(200*1000);
        ui4_loop--;
    }
    if (get_bt_get_rssi_done())
    {
        bluetooth_get_rssi_value(rssi_value);
        BT_DBG_INFO(BT_DEBUG_GAP, "rssi value:%ld", (long)*rssi_value);
    }
    else
    {
        BT_DBG_WARNING(BT_DEBUG_GAP, "not receive get rssi cbk!");
        return BT_ERR_STATUS_FAIL;
    }
    return BT_SUCCESS;
}

UINT32 fg_bt_inquiry_type = 0;
BOOL fg_bt_scan_ongoning = FALSE;
/* FUNCTION NAME: c_btm_start_inquiry_scan
 * PURPOSE:
 *      { scan all devices(ps_dev_mac), timeless }
 * INPUT:
 *      parameter-1  --
 * OUTPUT:
 *      parameter-n  --
 * RETURN:
 *      return-value-1 --
 * NOTES:
 *
 */
EXPORT_SYMBOL INT32 c_btm_start_inquiry_scan(UINT32 ui4_filter_type, BtAppGapInquiryResponseCbk pf_fct)
{
    FUNC_ENTRY;
    if (fg_bt_scan_ongoning)
    {
        BT_DBG_ERROR(BT_DEBUG_GAP, "Inquiry is ongoing,it doesn't need to scan again");
        return BT_ERR_STATUS_BUSY;
    }
    else
    {
        fg_bt_scan_ongoning = TRUE;
        BT_DBG_NORMAL(BT_DEBUG_GAP, "START SCAN");
    }

    fg_bt_inquiry_type = ui4_filter_type;
    if (NULL != pf_fct)
    {
        AppInquiryCbk = pf_fct;
    }
    else
    {
        BT_DBG_ERROR(BT_DEBUG_GAP, "null pointer of pf_fct");
        //return BT_ERR_STATUS_FAIL;
    }
    if (get_bt_power_on())
    {
        bluetooth_scan();
        memset(&scan_target_dev_info, 0x0, sizeof(BT_DEV_MAPPING_LIST));
        memset(&notify_target_dev_info, 0x0, sizeof(BT_DEV_MAPPING_LIST));
    }
    else
    {
        fg_bt_scan_ongoning = FALSE;
        BT_DBG_ERROR(BT_DEBUG_GAP, "need power on bluetooth first");
        return BT_ERR_STATUS_NOT_READY;
    }
    return BT_SUCCESS;
}

/* FUNCTION NAME: c_btm_stop_inquiry_scan
 * PURPOSE:
 *      { stop scan }
 * INPUT:
 *      parameter-1  --
 * OUTPUT:
 *      parameter-n  --
 * RETURN:
 *      return-value-1 --
 * NOTES:
 *
 */
EXPORT_SYMBOL INT32 c_btm_stop_inquiry_scan(VOID)
{
    FUNC_ENTRY;
    if (!fg_bt_scan_ongoning)
    {
        BT_DBG_ERROR(BT_DEBUG_GAP, "Inquiry is not started,it doesn't need to stop again");
        return BT_ERR_STATUS_NOT_READY;
    }
    else
    {
        fg_bt_scan_ongoning = FALSE;
        BT_DBG_NORMAL(BT_DEBUG_GAP, "STOP SCAN");
    }

    if (get_bt_power_on())
    {
        return bluetooth_stop_scan();
    }
    else
    {
        BT_DBG_ERROR(BT_DEBUG_GAP, "need power on bluetooth first");
        return BT_ERR_STATUS_FAIL;
    }
}

/* FUNCTION NAME: c_btm_inquiry_one
 * PURPOSE:
 *      { scan target device(ps_dev_mac) in the during time(ui4_inqDuration) }
 * INPUT:
 *      parameter-1  --
 * OUTPUT:
 *      parameter-n  --
 * RETURN:
 *      return-value-1 --
 * NOTES:
 *
 */
EXPORT_SYMBOL INT32 c_btm_inquiry_one(CHAR *ps_dev_mac, UINT32 ui4_inqDuration)
{
    FUNC_ENTRY;
    INT32 i4_ret = BT_ERR_STATUS_FAIL;
    if (NULL == ps_dev_mac)
    {
        BT_DBG_ERROR(BT_DEBUG_GAP, "null pointer of ps_dev_mac");
        return BT_ERR_STATUS_FAIL;
    }
    if (!get_bt_power_on())
    {
        BT_DBG_ERROR(BT_DEBUG_GAP, "need power on bluetooth first");
        return BT_ERR_STATUS_FAIL;
    }
    i4_ret = bluetooth_inquiry_one(ps_dev_mac, ui4_inqDuration);

    return i4_ret;
}

/* FUNCTION NAME: c_btm_set_connectable_and_discoverable
 * PURPOSE:
 *      { set the mode of device connectable_and_discoverable}
 * INPUT:
 *      parameter-1  --
 * OUTPUT:
 *      parameter-n  --
 * RETURN:
 *      return-value-1 --
 * NOTES:
 *
 */
EXPORT_SYMBOL INT32 c_btm_set_connectable_and_discoverable(BOOL fg_conn, BOOL fg_disc)
{
    FUNC_ENTRY;
    return bluetooth_set_connectable_and_discoverable(fg_conn, fg_disc);
}

/* FUNCTION NAME: c_btm_set_virtual_sniffer
 * PURPOSE:
 *      { set virtual_sniffer enable/disable}
 * INPUT:
 *      parameter-1  --
 * OUTPUT:
 *      parameter-n  --
 * RETURN:
 *      return-value-1 --
 * NOTES:
 *
 */
EXPORT_SYMBOL INT32 c_btm_set_virtual_sniffer(INT32 enable)
{
    FUNC_ENTRY;
    return bluetooth_set_virtual_sniffer(enable);
}

/*for get rssi*/
/* FUNCTION NAME: c_btm_send_hci
 * PURPOSE:
 *      { send hci }
 * INPUT:
 *      parameter-1  --
 * OUTPUT:
 *      parameter-n  --
 * RETURN:
 *      return-value-1 --
 * NOTES:
 *
 */
EXPORT_SYMBOL VOID c_btm_send_hci(CHAR *buffer)
{
    FUNC_ENTRY;
    send_buffer_to_hci(buffer);
}

EXPORT_SYMBOL INT32 c_btm_set_afh(UINT8* mask)
{
    FUNC_ENTRY;
    return bluetooth_set_afh(mask);
}

EXPORT_SYMBOL INT32 c_btm_bluetooth_get_bonded_device(VOID)
{
    FUNC_ENTRY;
    return bluetooth_get_bonded_device();
}




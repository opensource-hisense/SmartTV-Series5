/*-----------------------------------------------------------------------------
                    include files
 ----------------------------------------------------------------------------*/
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <linux/input.h>
#include <linux/uinput.h>
#include "c_mw_config.h"
#include "bluetooth.h"

#if ENABLE_MTK_TV_PLATFORM
#include "x_ipcd.h"
#endif
#include "bt_mw_common.h"
#include "bt_mw_gap.h"
#include "bt_mw_manager.h"

#include "bluetooth_sync.h"

#include "bt_mw_a2dp_snk.h"
#include "bt_mw_devices_info.h"
#include "bt_mw_avrcp.h"

#include "c_bt_mw_manager.h"

/*bluetooth init function, before run this API should run stack first*/
EXPORT_SYMBOL INT32 c_btm_bt_base_init(BT_APP_CB_FUNC *func)
{
    FUNC_ENTRY;
    return bluetooth_mgr_base_init(func);
}

EXPORT_SYMBOL INT32 c_btm_bluetooth_on_off(BOOL fg_on)
{
    FUNC_ENTRY;
    return bluetooth_mgr_bluetooth_on_off(fg_on);
}

EXPORT_SYMBOL INT32 c_btm_connecting(CHAR* ps_mac)
{
    FUNC_ENTRY;
    return bluetooth_mgr_connecting(ps_mac);
}

EXPORT_SYMBOL INT32 c_btm_btstart(const CHAR *ps_mode, UINT32 u4_overtime)
{
    FUNC_ENTRY;
    return bluetooth_mgr_btstart(ps_mode, u4_overtime);
}

EXPORT_SYMBOL BT_MODE_TYPE c_btm_get_mode(VOID)
{
    BT_DBG_NOTICE(BT_DEBUG_COMM, "+++into:");
    return get_bt_mode();
}

EXPORT_SYMBOL INT32 c_btm_stop(BOOL fg_keep_conn, BOOL fg_discard_data)
{
    FUNC_ENTRY;
    return bluetooth_mgr_stop(fg_keep_conn, fg_discard_data);
}


/*get local device information, eg. MAC address, name*/
EXPORT_SYMBOL INT32 c_btm_get_local_dev_info(BT_GAP_LOCAL_PROPERTIES_RESULT* ps_dev_info)
{
    FUNC_ENTRY;
    bluetooth_update_local_dev_info();
    return bluetooth_get_local_dev_info(ps_dev_info);
}

/*get local device name, no need power on bluetooth*/
EXPORT_SYMBOL INT32 c_btm_get_local_name(CHAR *name)
{
    FUNC_ENTRY;
    if (NULL == name)
    {
        BT_DBG_ERROR(BT_DEBUG_COMM, "null pointer of name!");
        return BT_ERR_STATUS_FAIL;
    }
    bluetooth_get_local_dev_name(name);
    BT_DBG_NORMAL(BT_DEBUG_COMM, "the name is: %s", name);
    return BT_SUCCESS;
}

/*get current connected device(eg. cell phone) MAC address*/
EXPORT_SYMBOL INT32 c_btm_get_target_addr(CHAR *addr)
{
    FUNC_ENTRY;

    BT_CHECK_POINTER(BT_DEBUG_COMM, addr);
    return bt_get_last_device_addr(addr);
}

/*get current connected device(eg. cell phone) MAC address and name*/
EXPORT_SYMBOL INT32 c_btm_get_target_info(BT_TARGET_DEV_INFO *pt_target_dev_info)
{
    FUNC_ENTRY;
    return btaudio_get_current_target_dev_info(pt_target_dev_info);
}

EXPORT_SYMBOL INT32 c_btm_clear_dev_info(VOID)
{
    FUNC_ENTRY;
    return bluetooth_mgr_clear_local_dev_info();
}

EXPORT_SYMBOL INT32 c_btm_bluetooth_factory_reset(CHAR *addr)
{
    FUNC_ENTRY;
    return bluetooth_mgr_factory_reset(addr);
}

/* remove one item from paired device histroy, not do unpair operation*/
EXPORT_SYMBOL INT32 c_btm_del_paired_av_dev_one(BT_TARGET_DEV_INFO *pt_device_info)
{
    FUNC_ENTRY;

    return bluetooth_mgr_del_paired_av_dev_one(pt_device_info);
}

/* used by APP layer
**can support disconnection A2DP/AVRCP update to connection state**
*/
EXPORT_SYMBOL INT32 c_btm_del_paired_av_dev_all(VOID)
{
    FUNC_ENTRY;

    return bluetooth_mgr_del_paired_av_dev_all();
}

EXPORT_SYMBOL INT32 c_btm_bt_auto_disconnection(VOID)
{
    FUNC_ENTRY;

    return bluetooth_mgr_av_auto_disconnection();
}

UINT8 c_btm_bt_get_scms_t(VOID)
{
    FUNC_ENTRY;
    return get_bt_support_scms_t();
}

EXPORT_SYMBOL INT32 c_btm_save_device_history(VOID)
{
    FUNC_ENTRY;
    return bluetooth_mgr_save_device_history();
} /*c_btm_save_device_history*/


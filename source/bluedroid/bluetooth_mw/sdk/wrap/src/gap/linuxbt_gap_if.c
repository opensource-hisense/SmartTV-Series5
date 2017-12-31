
#include <stdlib.h>
#include <string.h>

#include "bt_mw_common.h"
#include "bluetooth.h"

#include "linuxbt_common.h"
#include "linuxbt_gap_if.h"
#if (ENABLE_A2DP_SRC || ENABLE_A2DP_SINK)
#include "linuxbt_a2dp_if.h"
#endif
#if ENABLE_AVRCP_PROFILE
#include "linuxbt_avrcp_if.h"
#include "linuxbt_avrcp_tg_if.h"
#endif
#if ENABLE_GATT_PROFILE
#include "linuxbt_gatt_if.h"
#endif
#if ENABLE_SPP_PROFILE
#include "linuxbt_spp_if.h"
#endif
#if ENABLE_HID_PROFILE_H
#include "linuxbt_hid_if.h"
#include "bt_mw_hidh.h"
#endif
#if ENABLE_HID_PROFILE_D
#include "linuxbt_hidd_if.h"
#endif
#if ENABLE_HFP_CLIENT
#include "linuxbt_hfclient_if.h"
#endif
#include "bluetooth_sync.h"
#include "bt_mw_gap.h"
#include "bt_mw_devices_info.h"

#if defined(MTK_LINUX_GAP) && (MTK_LINUX_GAP == TRUE)
#include "mtk_bluetooth.h"
#endif

//extern void set_loglv(int module, int lv);
//extern void set_Enablelog(int log_switch);
/*
extern int open_bluetooth_stack (const struct hw_module_t* module,
                                     char const* name,
                                     struct hw_device_t** abstraction);
*/
#define UNUSED_ATTR __attribute__((unused))

BT_GAP_LOCAL_PROP_INFO g_local_property;

extern int open_bluetooth_stack(const struct hw_module_t *module,
                                UNUSED_ATTR char const *name,
                                struct hw_device_t **abstraction) ;

extern BT_DEV_MAPPING_LIST all_target_dev_info;
extern BT_DEV_MAPPING_LIST bond_target_dev_info;

// Callback functions declaration
void linuxbt_gap_state_changed_cb(bt_state_t state);

void linuxbt_gap_properties_cb(bt_status_t status,
                               int num_properties,
                               bt_property_t *properties);

void linuxbt_gap_remote_device_properties_cb(bt_status_t status,
        bt_bdaddr_t *bd_addr,
        int num_properties,
        bt_property_t *properties);

void linuxbt_gap_device_found_cb(int num_properties,
                                 bt_property_t *properties);

void linuxbt_gap_discovery_state_changed_cb(bt_discovery_state_t state);

void linuxbt_gap_pin_request_cb(bt_bdaddr_t *remote_bd_addr,
                                bt_bdname_t *bd_name, uint32_t cod,bool min_16_digit);


void linuxbt_gap_ssp_request_cb(bt_bdaddr_t *remote_bd_addr,
                                bt_bdname_t *bd_name,
                                uint32_t cod,
                                bt_ssp_variant_t pairing_variant,
                                uint32_t pass_key);

void linuxbt_gap_bond_state_changed_cb(bt_status_t status,
                                       bt_bdaddr_t *remote_bd_addr,
                                       bt_bond_state_t state);

void linuxbt_gap_acl_state_changed_cb(bt_status_t status,
                                      bt_bdaddr_t *remote_bd_addr,
                                      bt_acl_state_t state);

void linuxbt_gap_acl_disconnect_reason_callback(bt_bdaddr_t *remote_bd_addr, uint8_t reason);


//void linuxbt_gap_reset_ind_cb(void);

void linuxbt_gap_thread_event_cb(bt_cb_thread_evt evt);

void linuxbt_gap_dut_mode_recv_cb(uint16_t opcode, uint8_t *buf, uint8_t len);

void linuxbt_gap_le_test_mode_cb(bt_status_t status, uint16_t num_packets);
void linuxbt_gap_get_rssi_cb(bt_status_t status, bt_bdaddr_t *remote_bd_addr , int rssi_value);
void  linuxbtgap_send_hci_cmd_cb(bt_status_t status, int event_length , uint8_t * event);
//void  linuxbtgap_generic_fail_cb(bt_status_t status, uint8_t gap_cmd);
void linuxbt_gap_adapter_init_cb(VOID);
void linuxbt_gap_get_bonded_device_cb();

bool bt_set_wake_alarm(uint64_t delay_millis, bool should_wake, alarm_cb cb, void *data)
{
    return true;
}

int bt_acquire_wake_lock(const char *lock_name)
{
    return 0;
}

int bt_release_wake_lock(const char *lock_name)
{
    return 0;
}

static bt_os_callouts_t g_callouts =
{
    sizeof(bt_os_callouts_t),
    bt_set_wake_alarm,
    bt_acquire_wake_lock,
    bt_release_wake_lock,
};

static bt_state_t power_state = BT_STATE_OFF;

static bluetooth_device_t *g_bt_device = NULL;
static const bt_interface_t *g_bt_interface = NULL;
static bt_callbacks_t g_bt_callbacks =
{
    sizeof(bt_callbacks_t),
    linuxbt_gap_state_changed_cb,
    linuxbt_gap_properties_cb,
    linuxbt_gap_remote_device_properties_cb,
    linuxbt_gap_device_found_cb,
    linuxbt_gap_discovery_state_changed_cb,
    linuxbt_gap_pin_request_cb,
    linuxbt_gap_ssp_request_cb,
    linuxbt_gap_bond_state_changed_cb,
    linuxbt_gap_acl_state_changed_cb,
    linuxbt_gap_thread_event_cb,
   // linuxbt_gap_reset_ind_cb,
    linuxbt_gap_dut_mode_recv_cb,
    linuxbt_gap_le_test_mode_cb,
    NULL,
    //linuxbtgap_send_hci_cmd_cb,
    //linuxbtgap_generic_fail_cb,
#ifdef __P_BENTEN__
    //linuxbt_gap_get_device_list_cb,
#endif
    //linuxbt_gap_adapter_init_cb,
    // NULL,
};


#if defined(MTK_LINUX_GAP) && (MTK_LINUX_GAP == TRUE)
static const btgap_ex_interface_t *linuxbt_gap_ex_interface = NULL;

static btgap_ex_callbacks_t linuxbt_gap_ex_callbacks =
{
    sizeof(btgap_ex_callbacks_t),
    linuxbt_gap_get_rssi_cb,
    linuxbt_gap_acl_disconnect_reason_callback,
};

#endif
/*
int linuxbt_gap_start_sniff_mode_handler(UINT8 fg_start,
        char *ptr_addr,
        int i4maxInterval,
        int i4minInterval,
        int i4attempt,
        int i4timeout)
{
    bt_bdaddr_t bdaddr;
    bt_sniff_info info;
    bt_status_t ret = BT_STATUS_SUCCESS;
    memset(&info, 0, sizeof(bt_sniff_info));
    memset(&bdaddr,0,sizeof(bt_bdaddr_t));
    if (NULL == ptr_addr)
    {
        BT_DBG_ERROR(BT_DEBUG_GAP, " null pointer of ptr_addr");
        return BT_ERR_STATUS_PARM_INVALID;
    }
    if (NULL == g_bt_interface)
    {
        BT_DBG_ERROR(BT_DEBUG_GAP, " Failed to get GAP interface");
        return BT_ERR_STATUS_FAIL;
    }
    linuxbt_btaddr_stoh(ptr_addr, &bdaddr);
    if (0 == fg_start)
    {
        BT_DBG_INFO(BT_DEBUG_GAP, "stop Sniff Mode:%s", ptr_addr);
       // ret = g_bt_interface->stop_sniff(&bdaddr);
    }
    else
    {
        info.maxInterval = i4maxInterval;
        info.minInterval = i4minInterval;
        info.attempt = i4attempt;
        info.timeout = i4timeout;
        BT_DBG_INFO(BT_DEBUG_GAP, "start Sniff Mode:%s %ld %ld %ld %ld",
                      ptr_addr,
                      (long)i4maxInterval,
                      (long)i4minInterval,
                      (long)i4attempt,
                      (long)i4timeout);
       // ret = g_bt_interface->start_sniff(&bdaddr, &info);
    }
    return linuxbt_return_value_convert(ret);
}
*/

#if defined(MTK_LINUX_GAP) && (MTK_LINUX_GAP == TRUE)
int linuxbtgap_send_hci_handler(char *ptr)
{
    int i = 0;
    uint8_t   rpt_size = 0;
    uint8_t   hex_bytes_filled;
    uint8_t hex_buf[200] = {0};
    uint16_t   hex_len = 0;
    bt_status_t ret = BT_STATUS_SUCCESS;

    if (NULL == ptr)
    {
        BT_DBG_ERROR(BT_DEBUG_GAP, "[HID] Usage : linuxbt_gap_send_hci_handler ([command hex)])");
        return BT_ERR_STATUS_PARM_INVALID;
    }
    if (NULL == linuxbt_gap_ex_interface)
    {
        BT_DBG_ERROR(BT_DEBUG_GAP, "Failed to get GAP interface");
        return BT_ERR_STATUS_FAIL;
    }
    rpt_size = strlen(ptr);
    hex_len = (strlen(ptr) + 1) / 2;

    BT_DBG_INFO(BT_DEBUG_GAP, "rpt_size=%ld, hex_len=%ld", (unsigned long)rpt_size, (unsigned long)hex_len);
    hex_bytes_filled = ascii_2_hex(ptr, hex_len, hex_buf);
    BT_DBG_INFO(BT_DEBUG_GAP, "hex_bytes_filled=%ld", (unsigned long)hex_bytes_filled);
    for (i=0;i<hex_len;i++)
    {
        BT_DBG_NOTICE(BT_DEBUG_GAP, "hex values= %02X",hex_buf[i]);
    }
    if (hex_bytes_filled)
    {
        ret = linuxbt_gap_ex_interface->send_hci((uint8_t*)hex_buf, hex_bytes_filled);
        BT_DBG_INFO(BT_DEBUG_GAP, "send_hci");
        return linuxbt_return_value_convert(ret);
    }
    else
    {
        BT_DBG_ERROR(BT_DEBUG_GAP, "hex_bytes_filled <= 0");
        return BT_ERR_STATUS_PARM_INVALID;
    }
    // return 0;

}
#endif

/*void linuxbtgap_generic_fail_cb(bt_status_t status, uint8_t msg)
{

    switch (msg)
    {
    case GAP_SERVICE_SEARCH_FAILED:
        BT_DBG_ERROR(BT_DEBUG_GAP, "GAP Start_Sdp Failed:");
        break;
    case GAP_GET_REMOTE_DEVICE_NAME_FAILED:
        BT_DBG_ERROR(BT_DEBUG_GAP, "GAP Get Remote Device Name Failed :");
        break;
    case GAP_WRITE_LOCAL_NAME_FAILED:
        BT_DBG_ERROR(BT_DEBUG_GAP, "GAP Write Local Name Failed :");
        break;
    case GAP_READ_LOCAL_UUID_FAILED:
        BT_DBG_ERROR(BT_DEBUG_GAP, "GAP Read Local UUID Failed :");
        break;
    case GAP_SERVICE_SEARCH_RESULT_FAILED:
        BT_DBG_ERROR(BT_DEBUG_GAP, "GAP Service Search Result Failed :");
        break;
    case GAP_WRITE_SCANNABLE_MODE_FAILED:
        BT_DBG_ERROR(BT_DEBUG_GAP, "GAP Service Search Result Failed :");
        break;
    case GAP_BONDING_CANCEL_FAIL:
        BT_DBG_ERROR(BT_DEBUG_GAP, "GAP Cancel_bond Failed :");
        break;
    case GAP_DISCOVERY_FAILED:
        BT_DBG_ERROR(BT_DEBUG_GAP, "GAP Start_discovery Failed :");
        break;
    case GAP_DISCOVERY_CANCEL_FAILED:
        BT_DBG_ERROR(BT_DEBUG_GAP, "GAP Cancel_discovery :");
        break;
    case GAP_DEL_TRUST_FAILED:
        BT_DBG_ERROR(BT_DEBUG_GAP, "GAP Delete Trust :");
        break;

    default :
        break;
    }
}
*/

void linuxbtgap_send_hci_cmd_cb(bt_status_t status, int event_length , uint8_t * event)
{

    INT16 i=0;
    BT_DBG_NORMAL(BT_DEBUG_GAP, "send_hci_call_back result start ");

    for (i=0; i< event_length;i++)
    {
        printf("%02X ", event[i]);
    }
    printf("");
    bt_gap_hci_response_cb(event,event_length);
    BT_DBG_NORMAL(BT_DEBUG_GAP, "send_hci_call_back result End ");
}

#if defined(MTK_LINUX_GAP) && (MTK_LINUX_GAP == TRUE)
int linuxbt_gap_get_rssi_handler(char *pbt_addr)
{
    BT_DBG_NORMAL(BT_DEBUG_GAP, "%s()", __FUNCTION__);
    bt_bdaddr_t bdaddr;
    bt_status_t ret = BT_STATUS_SUCCESS;
    memset(&bdaddr, 0, sizeof(bt_bdaddr_t));

    if (NULL == linuxbt_gap_ex_interface)
    {
        BT_DBG_ERROR(BT_DEBUG_GAP, "Failed to get GAP extended interface");
        return BT_ERR_STATUS_FAIL;
    }
    linuxbt_btaddr_stoh(pbt_addr, &bdaddr);
    BT_DBG_NORMAL(BT_DEBUG_GAP, "BTADDR = %02X:%02X:%02X:%02X:%02X:%02X",
                  bdaddr.address[0], bdaddr.address[1], bdaddr.address[2],
                  bdaddr.address[3], bdaddr.address[4], bdaddr.address[5]);
    ret = linuxbt_gap_ex_interface->get_rssi(&bdaddr);
    return linuxbt_return_value_convert(ret);
}
#endif

int linuxbt_gap_detrust_one_handler(char *pbt_addr)
{
    BT_DBG_NORMAL(BT_DEBUG_GAP, "%s()", __FUNCTION__);
    bt_bdaddr_t bdaddr;
    bt_status_t ret = BT_STATUS_SUCCESS;
    memset(&bdaddr, 0, sizeof(bt_bdaddr_t));

    if (NULL == g_bt_interface)
    {
        BT_DBG_ERROR(BT_DEBUG_GAP, "Failed to get GAP interface");
        return BT_ERR_STATUS_FAIL;
    }
    linuxbt_btaddr_stoh(pbt_addr, &bdaddr);
    BT_DBG_NORMAL(BT_DEBUG_GAP, "BTADDR = %02X:%02X:%02X:%02X:%02X:%02X",
                  bdaddr.address[0], bdaddr.address[1], bdaddr.address[2],
                  bdaddr.address[3], bdaddr.address[4], bdaddr.address[5]);
   // ret = g_bt_interface->detrust_one(&bdaddr);
    return linuxbt_return_value_convert(ret);
}
int linuxbt_gap_cancel_sdp_services_handler(char *pbt_addr)
{
    FUNC_ENTRY;

    bt_bdaddr_t bdaddr;
    bt_status_t ret = BT_STATUS_SUCCESS;
    memset(&bdaddr, 0, sizeof(bt_bdaddr_t));

    if (NULL == g_bt_interface)
    {
        BT_DBG_ERROR(BT_DEBUG_GAP, "Failed to get GAP interface");
        return BT_ERR_STATUS_FAIL;
    }
    linuxbt_btaddr_stoh(pbt_addr, &bdaddr);
    BT_DBG_NORMAL(BT_DEBUG_GAP, "BTADDR = %02X:%02X:%02X:%02X:%02X:%02X",
                  bdaddr.address[0], bdaddr.address[1], bdaddr.address[2],
                  bdaddr.address[3], bdaddr.address[4], bdaddr.address[5]);
   // ret = g_bt_interface->cancel_remote_services(&bdaddr);
    return linuxbt_return_value_convert(ret);
}

int linuxbt_gap_enable_handler(void)
{
    FUNC_ENTRY;

    bt_status_t ret = BT_STATUS_SUCCESS;
    if (NULL == g_bt_interface)
    {
        BT_DBG_ERROR(BT_DEBUG_GAP, "Failed to get GAP interface");
        return BT_ERR_STATUS_FAIL;
    }
    ret = g_bt_interface->enable(false);
    return linuxbt_return_value_convert(ret);
}

int linuxbt_gap_disable_handler(void)
{
    FUNC_ENTRY;

    bt_status_t ret = BT_STATUS_SUCCESS;
    if (NULL == g_bt_interface)
    {
        BT_DBG_ERROR(BT_DEBUG_GAP, "Failed to get GAP interface");
        return BT_ERR_STATUS_FAIL;
    }
    ret = g_bt_interface->disable();
    return linuxbt_return_value_convert(ret);
}

int linuxbt_gap_get_adapter_properties_handler(void)
{
    FUNC_ENTRY;

    bt_status_t ret = BT_STATUS_SUCCESS;
    if (power_state == BT_STATE_OFF)
    {
        BT_DBG_NORMAL(BT_DEBUG_GAP, "Bluetooth state = OFF");
    }
    if (NULL == g_bt_interface)
    {
        BT_DBG_ERROR(BT_DEBUG_GAP, "Failed to get GAP interface");
        return BT_ERR_STATUS_FAIL;
    }
    ret = g_bt_interface->get_adapter_properties();
    return linuxbt_return_value_convert(ret);
}

int linuxbt_gap_get_adapter_property_handler(bt_property_type_t num)
{
    FUNC_ENTRY;

    bt_status_t ret = BT_STATUS_SUCCESS;
    if (NULL == g_bt_interface)
    {
        BT_DBG_ERROR(BT_DEBUG_GAP, "Failed to get GAP interface");
        return BT_ERR_STATUS_FAIL;
    }
    ret = g_bt_interface->get_adapter_property(num);
    return linuxbt_return_value_convert(ret);
}

int linuxbt_gap_set_adapter_property_handler(int i4index, char *ptr)
{
    FUNC_ENTRY;

    bt_status_t ret = BT_STATUS_SUCCESS;
    unsigned int disc_timeout;
    bt_property_t property;
    bt_property_t *property_p;
    bt_scan_mode_t scan_mode;
    unsigned int cod;

    property_p = &property;

    if (NULL == g_bt_interface)
    {
        BT_DBG_ERROR(BT_DEBUG_GAP, "Failed to get GAP interface");
        return BT_ERR_STATUS_FAIL;
    }
    switch (i4index)
    {
        /* 1 */
    case BT_PROPERTY_BDNAME:
        property_p->type = BT_PROPERTY_BDNAME;
        property_p->len = strlen(ptr);
        property_p->val = ptr;

        break;
        /* 4 */
    case BT_PROPERTY_CLASS_OF_DEVICE:
        cod = atoi(ptr);
        property_p->type = BT_PROPERTY_CLASS_OF_DEVICE;
        property_p->len = sizeof(unsigned int);
        property_p->val = &cod;

        break;

        /* 7 */
    case BT_PROPERTY_ADAPTER_SCAN_MODE:
        scan_mode = (bt_scan_mode_t)atoi(ptr);;

        property_p->type = BT_PROPERTY_ADAPTER_SCAN_MODE;
        property_p->len = sizeof(bt_scan_mode_t);
        property_p->val = (void*)&scan_mode;

        break;
        /* 9 */
    case BT_PROPERTY_ADAPTER_DISCOVERY_TIMEOUT:
        disc_timeout = atoi(ptr);

        property_p->type = BT_PROPERTY_ADAPTER_DISCOVERY_TIMEOUT;
        property_p->len = sizeof(unsigned int);
        property_p->val = &disc_timeout;

        break;
    default:
        BT_DBG_ERROR(BT_DEBUG_GAP, "We don't support the format.");
        break;
    }

    ret = g_bt_interface->set_adapter_property(property_p);
    return linuxbt_return_value_convert(ret);
}

int linuxbt_gap_set_device_name_handler(char *pname)
{
    bt_property_t property;
    bt_property_t *property_p;
    bt_status_t ret = BT_STATUS_SUCCESS;

    memset(&property, 0, sizeof(bt_property_t));
    if (NULL == pname)
    {
        BT_DBG_ERROR(BT_DEBUG_GAP, "null pointer of pname");
        return BT_ERR_STATUS_PARM_INVALID;
    }
    if (NULL == g_bt_interface)
    {
        BT_DBG_ERROR(BT_DEBUG_GAP, "Failed to get GAP interface");
        return BT_ERR_STATUS_FAIL;
    }

    property_p = &property;

    property_p->type = BT_PROPERTY_BDNAME;
    property_p->len = strlen(pname);
    property_p->val = pname;

    ret = g_bt_interface->set_adapter_property(property_p);
    return linuxbt_return_value_convert(ret);
}

int linuxbt_gap_set_scan_mode(int mode)
{
    bt_property_t property;
    bt_property_t *property_p;
    bt_scan_mode_t scan_mode;
    bt_status_t ret = BT_STATUS_SUCCESS;

    memset(&property, 0, sizeof(bt_property_t));
    scan_mode = (bt_scan_mode_t)mode;

    if (NULL == g_bt_interface)
    {
        BT_DBG_ERROR(BT_DEBUG_GAP, "Failed to get GAP interface");
        return BT_ERR_STATUS_FAIL;
    }
    property_p = &property;
    property_p->type = BT_PROPERTY_ADAPTER_SCAN_MODE;
    property_p->len = sizeof(bt_scan_mode_t);
    property_p->val = (void*)&scan_mode;

    ret = g_bt_interface->set_adapter_property(property_p);

    return linuxbt_return_value_convert(ret);
}

int linuxbt_gap_set_discovery_timeout_handler(unsigned int disc_timeout)
{
    bt_property_t property;
    bt_property_t *property_p;
    bt_status_t ret = BT_STATUS_SUCCESS;

    memset(&property, 0, sizeof(bt_property_t));
    property_p = &property;

    if (NULL == g_bt_interface)
    {
        BT_DBG_ERROR(BT_DEBUG_GAP, "Failed to get GAP interface");
        return BT_ERR_STATUS_FAIL;
    }
    property_p->type = BT_PROPERTY_ADAPTER_DISCOVERY_TIMEOUT;
    property_p->len = sizeof(unsigned int);
    property_p->val = &disc_timeout;

    ret = g_bt_interface->set_adapter_property(property_p);

    return linuxbt_return_value_convert(ret);
}

int linuxbt_gap_get_remote_device_properties_handler(char *pbt_addr)
{
    FUNC_ENTRY;
    bt_bdaddr_t bdaddr;
    bt_status_t ret = BT_STATUS_SUCCESS;

    memset(&bdaddr, 0, sizeof(bt_bdaddr_t));
    if (NULL == pbt_addr)
    {
        BT_DBG_ERROR(BT_DEBUG_GAP, "null pointer of pbt_addr");
        return BT_ERR_STATUS_PARM_INVALID;
    }
    if (NULL == g_bt_interface)
    {
        BT_DBG_ERROR(BT_DEBUG_GAP, "Failed to get GAP interface");
        return BT_ERR_STATUS_FAIL;
    }

    linuxbt_btaddr_stoh(pbt_addr, &bdaddr);
    BT_DBG_NORMAL(BT_DEBUG_GAP, "BTADDR = %02X:%02X:%02X:%02X:%02X:%02X",
                  bdaddr.address[0], bdaddr.address[1], bdaddr.address[2],
                  bdaddr.address[3], bdaddr.address[4], bdaddr.address[5]);

    ret = g_bt_interface->get_remote_device_properties(&bdaddr);

    return linuxbt_return_value_convert(ret);
}

int linuxbt_gap_get_remote_device_property_handler(bt_property_type_t num, char *pbt_addr)
{
    FUNC_ENTRY;
    bt_bdaddr_t bdaddr;
    bt_status_t ret = BT_STATUS_SUCCESS;

    memset(&bdaddr, 0, sizeof(bt_bdaddr_t));
    if (NULL == pbt_addr)
    {
        BT_DBG_ERROR(BT_DEBUG_GAP, "null pointer of pbt_addr");
        return BT_ERR_STATUS_PARM_INVALID;
    }
    if (NULL == g_bt_interface)
    {
        BT_DBG_ERROR(BT_DEBUG_GAP, "Failed to get GAP interface");
        return BT_ERR_STATUS_FAIL;
    }
    linuxbt_btaddr_stoh(pbt_addr, &bdaddr);
    BT_DBG_NORMAL(BT_DEBUG_GAP, "BTADDR = %02X:%02X:%02X:%02X:%02X:%02X",
                  bdaddr.address[0], bdaddr.address[1], bdaddr.address[2],
                  bdaddr.address[3], bdaddr.address[4], bdaddr.address[5]);

    ret = g_bt_interface->get_remote_device_property(&bdaddr, num);

    return linuxbt_return_value_convert(ret);
}

int linuxbt_gap_set_remote_device_property_handler(void)
{
    BT_DBG_NORMAL(BT_DEBUG_GAP, "%s()", __FUNCTION__);

    //g_bt_interface->set_remote_device_property();

    return BT_STATUS_SUCCESS;
}
int linuxbt_gap_get_remote_service_record_handler(char *pbd_addr)
{
    bt_bdaddr_t bdaddr;
    bt_status_t ret = BT_STATUS_SUCCESS;

    memset(&bdaddr, 0, sizeof(bt_bdaddr_t));
    BT_DBG_NORMAL(BT_DEBUG_GAP, "%s()", __FUNCTION__);
    if (NULL == g_bt_interface)
    {
        BT_DBG_ERROR(BT_DEBUG_GAP, "Failed to get GAP interface");
        return BT_ERR_STATUS_FAIL;
    }

    /*get_remote_device_properties [BT_ADDR]*/
    linuxbt_btaddr_stoh(pbd_addr, &bdaddr);
    BT_DBG_NORMAL(BT_DEBUG_GAP, "CBTADDR = %02X:%02X:%02X:%02X:%02X:%02X",
                  bdaddr.address[0], bdaddr.address[1], bdaddr.address[2],
                  bdaddr.address[3], bdaddr.address[4], bdaddr.address[5]);

    ret = g_bt_interface->get_remote_services(&bdaddr);

    return linuxbt_return_value_convert(ret);
}
int linuxbt_gap_get_remote_services_handler(char *pbt_addr)
{
    FUNC_ENTRY;
    bt_bdaddr_t bdaddr;
    bt_status_t ret = BT_STATUS_SUCCESS;

    memset(&bdaddr, 0, sizeof(bt_bdaddr_t));
    if (NULL == pbt_addr)
    {
        BT_DBG_ERROR(BT_DEBUG_GAP, "null pointer of pbt_addr");
        return BT_ERR_STATUS_PARM_INVALID;
    }
    if (NULL == g_bt_interface)
    {
        BT_DBG_ERROR(BT_DEBUG_GAP, "Failed to get GAP interface");
        return BT_ERR_STATUS_FAIL;
    }
    linuxbt_btaddr_stoh(pbt_addr, &bdaddr);
    BT_DBG_NORMAL(BT_DEBUG_GAP, "CBTADDR = %02X:%02X:%02X:%02X:%02X:%02X",
                  bdaddr.address[0], bdaddr.address[1], bdaddr.address[2],
                  bdaddr.address[3], bdaddr.address[4], bdaddr.address[5]);

    ret = g_bt_interface->get_remote_services(&bdaddr);

    return linuxbt_return_value_convert(ret);
}

int linuxbt_gap_start_discovery_handler(void)
{
    FUNC_ENTRY;

    bt_status_t ret = BT_STATUS_SUCCESS;
    if (NULL == g_bt_interface)
    {
        BT_DBG_ERROR(BT_DEBUG_GAP, "Failed to get GAP interface");
        return BT_ERR_STATUS_FAIL;
    }
    ret = g_bt_interface->start_discovery();

    return linuxbt_return_value_convert(ret);
}

int linuxbt_gap_cancel_discovery_handler(void)
{
    FUNC_ENTRY;

    bt_status_t ret = BT_STATUS_SUCCESS;
    if (NULL == g_bt_interface)
    {
        BT_DBG_ERROR(BT_DEBUG_GAP, "Failed to get GAP interface");
        return BT_ERR_STATUS_FAIL;
    }
    ret = g_bt_interface->cancel_discovery();

    return linuxbt_return_value_convert(ret);
}

int linuxbt_gap_create_bond_handler(char *pbt_addr, int transport)
{
    FUNC_ENTRY;
    bt_bdaddr_t bdaddr;
    bt_status_t ret = BT_STATUS_SUCCESS;

    memset(&bdaddr, 0, sizeof(bt_bdaddr_t));

    if (NULL == pbt_addr)
    {
        BT_DBG_ERROR(BT_DEBUG_GAP, "null pointer of pbt_addr");
        return BT_ERR_STATUS_PARM_INVALID;
    }
    if (NULL == g_bt_interface)
    {
        BT_DBG_ERROR(BT_DEBUG_GAP, "Failed to get GAP interface");
        return BT_ERR_STATUS_FAIL;
    }
    linuxbt_btaddr_stoh(pbt_addr, &bdaddr);
    BT_DBG_NORMAL(BT_DEBUG_GAP, "start pairing to %02X:%02X:%02X:%02X:%02X:%02X",
                  bdaddr.address[0], bdaddr.address[1], bdaddr.address[2],
                  bdaddr.address[3], bdaddr.address[4], bdaddr.address[5]);

    ret = g_bt_interface->create_bond(&bdaddr, transport);

    return linuxbt_return_value_convert(ret);
}

/*needless*/
int linuxbt_gap_remove_bond_handler(char *pbt_addr)
{
    FUNC_ENTRY;

    bt_bdaddr_t bdaddr;
    bt_status_t ret = BT_STATUS_SUCCESS;

    memset(&bdaddr, 0, sizeof(bt_bdaddr_t));
    if (NULL == pbt_addr)
    {
        BT_DBG_ERROR(BT_DEBUG_GAP, "null pointer of pbt_addr");
        return BT_ERR_STATUS_PARM_INVALID;
    }
    if (NULL == g_bt_interface)
    {
        BT_DBG_ERROR(BT_DEBUG_GAP, "Failed to get GAP interface");
        return BT_ERR_STATUS_FAIL;
    }
    linuxbt_btaddr_stoh(pbt_addr, &bdaddr);
    BT_DBG_NORMAL(BT_DEBUG_GAP, "Cancel pairing to %02X:%02X:%02X:%02X:%02X:%02X",
                  bdaddr.address[0], bdaddr.address[1], bdaddr.address[2],
                  bdaddr.address[3], bdaddr.address[4], bdaddr.address[5]);

    ret = g_bt_interface->remove_bond(&bdaddr);

    return linuxbt_return_value_convert(ret);
}



//////////////////////////////////////////    haahahhhahahhah     /////////////////////////////////////////


int linuxbt_gap_cancel_bond_handler(char *pbt_addr)
{
    FUNC_ENTRY;
    bt_bdaddr_t bdaddr;
    bt_status_t ret = BT_STATUS_SUCCESS;

    memset(&bdaddr, 0, sizeof(bt_bdaddr_t));

    if (NULL == pbt_addr)
    {
        BT_DBG_ERROR(BT_DEBUG_GAP, "null pointer of pbt_addr");
        return BT_ERR_STATUS_PARM_INVALID;
    }
    if (NULL == g_bt_interface)
    {
        BT_DBG_ERROR(BT_DEBUG_GAP, "Failed to get GAP interface");
        return BT_ERR_STATUS_FAIL;
    }
    linuxbt_btaddr_stoh(pbt_addr, &bdaddr);
    BT_DBG_NORMAL(BT_DEBUG_GAP, "Cancel pairing to %02X:%02X:%02X:%02X:%02X:%02X",
                  bdaddr.address[0], bdaddr.address[1], bdaddr.address[2],
                  bdaddr.address[3], bdaddr.address[4], bdaddr.address[5]);

    ret = g_bt_interface->cancel_bond(&bdaddr);

    return linuxbt_return_value_convert(ret);
}

int linuxbt_gap_dut_mode_configure_handler(uint8_t enable)
{
    BT_DBG_NORMAL(BT_DEBUG_GAP, "%s()", __FUNCTION__);

    g_bt_interface->dut_mode_configure(enable);

    return BT_SUCCESS;
}

int linuxbt_gap_le_test_mode_handler(uint16_t opcode, uint8_t *buf, uint8_t len)
{
    BT_DBG_NORMAL(BT_DEBUG_GAP, "%s()", __FUNCTION__);

    g_bt_interface->le_test_mode(opcode, buf, len);

    return BT_SUCCESS;
}

int linuxbt_gap_stop_sniff_mode_handler(char *ptr)
{
    bt_bdaddr_t bdaddr;
    bt_status_t ret = BT_STATUS_SUCCESS;

    memset(&bdaddr, 0, sizeof(bt_bdaddr_t));
    if (NULL == g_bt_interface)
    {
        BT_DBG_ERROR(BT_DEBUG_GAP, "Failed to get GAP interface");
        return BT_ERR_STATUS_FAIL;
    }

    {
        linuxbt_btaddr_stoh(ptr, &bdaddr);
        BT_DBG_NORMAL(BT_DEBUG_GAP, "BTADDR = %02X:%02X:%02X:%02X:%02X:%02X",
                      bdaddr.address[0], bdaddr.address[1], bdaddr.address[2],
                      bdaddr.address[3], bdaddr.address[4], bdaddr.address[5]);
//        ret = g_bt_interface->stop_sniff(&bdaddr);
    }
    return linuxbt_return_value_convert(ret);
}

int linuxbt_gap_config_hci_snoop_log_handler(unsigned char enable)
{
    FUNC_ENTRY;

    bt_status_t ret = BT_STATUS_SUCCESS;

    if (NULL == g_bt_interface)
    {
        BT_DBG_ERROR(BT_DEBUG_GAP, "Failed to get GAP interface");
        return BT_ERR_STATUS_FAIL;
    }
    ret = g_bt_interface->config_hci_snoop_log(enable);

    return linuxbt_return_value_convert(ret);
}

int linuxbt_set_log_lv_handler(int module, int log_level)
{

    BT_DBG_NORMAL(BT_DEBUG_GAP, "%s()", __FUNCTION__);

    if (log_level < 0 || log_level > 5)
    {
        BT_DBG_NORMAL(BT_DEBUG_GAP, "level range is 0~5!!!");
        return BT_ERR_STATUS_PARM_INVALID;
    }

    //set_loglv(module, log_level);
    return BT_SUCCESS;
}

int linuxbt_enable_external_log(int log_switch)
{

    BT_DBG_NORMAL(BT_DEBUG_GAP, "%s()", __FUNCTION__);

    //set_Enablelog(log_switch);
    return BT_SUCCESS;
}
void linuxbt_gap_show_properties_debug(int num_properties, bt_property_t *properties)
{
    int i;
    int j;
    int len;
    char *name;
    bt_scan_mode_t *mode;
    bt_bdaddr_t *btaddr;
    bt_uuid_t *uuid;
    bt_property_t *property;
    unsigned int *cod;
    unsigned int *devicetype;
    unsigned int *disc_timeout;
    char *rssi;

    BT_DBG_NORMAL(BT_DEBUG_GAP, "=======================================Properties==============================================");
    BT_DBG_NORMAL(BT_DEBUG_GAP, "num_properties = %ld", (long)num_properties);
    for (i = 0; i < num_properties; i++)
    {
        property = &properties[i];

        switch (property->type)
        {
            /* 1 */
        case BT_PROPERTY_BDNAME:
            name = (char *)property->val;
            BT_DBG_NORMAL(BT_DEBUG_GAP, "type = %ld, len = %ld, bdname = %s", (long)property->type, (long)property->len, name);

            break;
            /* 2 */
        case BT_PROPERTY_BDADDR:
            btaddr = (bt_bdaddr_t *)property->val;

            BT_DBG_NORMAL(BT_DEBUG_GAP, "type = %ld, len = %ld, bdaddr = %02X:%02X:%02X:%02X:%02X:%02X", (long)property->type, (long)property->len,
                          btaddr->address[0], btaddr->address[1], btaddr->address[2],
                          btaddr->address[3], btaddr->address[4], btaddr->address[5]);

            break;
            /* 3 */
        case BT_PROPERTY_UUIDS:
            uuid = (bt_uuid_t*)property->val;
            len = property->len;

            for (j=0; j<len; j+=16)
            {
                BT_DBG_NORMAL(BT_DEBUG_GAP, "type = %ld, len = %ld, uuid = %02X%02X%02X%02X-%02X%02X-%02X%02X-%02X%02X-%02X%02X%02X%02X%02X%02X",
                                (long)property->type, (long)property->len,
                                uuid->uu[j+0], uuid->uu[j+1], uuid->uu[j+2], uuid->uu[j+3],
                                uuid->uu[j+4], uuid->uu[j+5], uuid->uu[j+6], uuid->uu[j+7],
                                uuid->uu[j+8], uuid->uu[j+9], uuid->uu[j+10], uuid->uu[j+11],
                                uuid->uu[j+12], uuid->uu[j+13], uuid->uu[j+14], uuid->uu[j+15]
                               );
            }

            break;
            /* 4 */
        case BT_PROPERTY_CLASS_OF_DEVICE:
            cod = (unsigned int *)property->val;
            BT_DBG_NORMAL(BT_DEBUG_GAP, "type = %ld, len = %ld, cod = 0x%X", (long)property->type, (long)property->len, *cod);
            break;
            /* 5 */
        case BT_PROPERTY_TYPE_OF_DEVICE:
            devicetype= (unsigned int *)property->val;

            /* 0 - BLE, 1 - BT, 2 - DUAL MODE */
            BT_DBG_NORMAL(BT_DEBUG_GAP, "type = %ld, len = %ld, devicetype = %ld", (long)property->type, (long)property->len, (unsigned long)*devicetype);
            break;
            /* 6 */
        case BT_PROPERTY_SERVICE_RECORD:
            BT_DBG_NORMAL(BT_DEBUG_GAP, "type = %ld, len = %ld, Others.", (long)property->type, (long)property->len);
            break;
            /* 7 */
        case BT_PROPERTY_ADAPTER_SCAN_MODE:
            mode = (bt_scan_mode_t *)property->val;

            BT_DBG_NORMAL(BT_DEBUG_GAP, "type = %ld, len = %ld, scan mode = %ld", (long)property->type, (long)property->len, (unsigned long)*mode);
            break;
            /* 8 */
        case BT_PROPERTY_ADAPTER_BONDED_DEVICES:
            btaddr = (bt_bdaddr_t *)property->val;

            BT_DBG_NORMAL(BT_DEBUG_GAP, "type = %ld, len = %ld, bonded_addr = %02X:%02X:%02X:%02X:%02X:%02X", (long)property->type, (long)property->len,
                          btaddr->address[0], btaddr->address[1], btaddr->address[2],
                          btaddr->address[3], btaddr->address[4], btaddr->address[5]);
            break;
            /* 9 */
        case BT_PROPERTY_ADAPTER_DISCOVERY_TIMEOUT:
            disc_timeout = (unsigned int *)property->val;

            BT_DBG_NORMAL(BT_DEBUG_GAP, "type = %ld, len = %ld, disc_timeout = %ld", (long)property->type, (long)property->len, (unsigned long)*disc_timeout);
            break;
            /* 10 */
        case BT_PROPERTY_REMOTE_FRIENDLY_NAME:
            name = (char *)property->val;
            BT_DBG_NORMAL(BT_DEBUG_GAP, "type = %ld, len = %ld, friendly bdname = %s", (long)property->type, (long)property->len, name);

            break;
            /* 11 */
        case BT_PROPERTY_REMOTE_RSSI:
            rssi = (char *)property->val;
            BT_DBG_NORMAL(BT_DEBUG_GAP, "type = %ld, len = %ld, rssi = %ld", (long)property->type, (long)property->len, (long)(*rssi));
            break;
            /* 12 */
        case BT_PROPERTY_REMOTE_VERSION_INFO:
            BT_DBG_NORMAL(BT_DEBUG_GAP, "type = %ld, len = %ld, Others.", (long)property->type, (long)property->len);
            break;
            /* FF */
        case BT_PROPERTY_REMOTE_DEVICE_TIMESTAMP:
            BT_DBG_NORMAL(BT_DEBUG_GAP, "type = %ld, len = %ld, Others.", (long)property->type, (long)property->len);
            break;
        default:
            BT_DBG_NORMAL(BT_DEBUG_GAP, "type = %ld, len = %ld, Others.", (long)property->type, (long)property->len);
            break;
        }
    }
}

void linuxbt_gap_show_properties_info(int num_properties, bt_property_t *properties)
{
    int i;
    int j;
    int len;
    char *name;
    bt_scan_mode_t *mode;
    bt_bdaddr_t *btaddr;
    bt_uuid_t *uuid;
    bt_property_t *property;
    unsigned int *devicetype;
    unsigned int *disc_timeout;
    char *rssi;
    BT_GAP_LOCAL_PROPERTIES_RESULT localProperty;

    memset(&localProperty,0,sizeof(BT_GAP_LOCAL_PROPERTIES_RESULT));
    BT_DBG_INFO(BT_DEBUG_GAP, "=======================================Properties==============================================");
    if ( power_state == BT_STATE_ON )
    {
        BT_DBG_NOTICE(BT_DEBUG_GAP, "Bluetooth state = POWER ON");
        localProperty.powered = 1;
    }
    else
    {
        BT_DBG_NOTICE(BT_DEBUG_GAP, "Bluetooth state = POWER OFF");
        localProperty.powered = 0;
    }
    BT_DBG_INFO(BT_DEBUG_GAP, "num_properties = %ld", (long)num_properties);

    for (i = 0; i < num_properties; i++)
    {
        property = &properties[i];

        switch (property->type)
        {
            /* 1 */
        case BT_PROPERTY_BDNAME:
            name = (char *)property->val;
            BT_DBG_INFO(BT_DEBUG_GAP, "type = %ld, len = %ld, bdname = %s", (long)property->type, (long)property->len, name);
            strncpy(localProperty.name, name, MAX_NAME_LEN);
            break;
            /* 2 */
        case BT_PROPERTY_BDADDR:
            btaddr = (bt_bdaddr_t *)property->val;

            BT_DBG_INFO(BT_DEBUG_GAP, "type = %ld, len = %ld, bdaddr = %02X:%02X:%02X:%02X:%02X:%02X", (long)property->type, (long)property->len,
                          btaddr->address[0], btaddr->address[1], btaddr->address[2],
                          btaddr->address[3], btaddr->address[4], btaddr->address[5]);
            linuxbt_btaddr_htos(btaddr, localProperty.bdAddr);
            break;
            /* 3 */
        case BT_PROPERTY_UUIDS:
            uuid = (bt_uuid_t*)property->val;
            len = property->len;

            for (j=0; j<len; j+=16)
            {
                BT_DBG_INFO(BT_DEBUG_GAP, "type = %ld, len = %ld, uuid = %02X%02X%02X%02X-%02X%02X-%02X%02X-%02X%02X-%02X%02X%02X%02X%02X%02X",
                                (long)property->type, (long)property->len,
                                uuid->uu[j+0], uuid->uu[j+1], uuid->uu[j+2], uuid->uu[j+3],
                                uuid->uu[j+4], uuid->uu[j+5], uuid->uu[j+6], uuid->uu[j+7],
                                uuid->uu[j+8], uuid->uu[j+9], uuid->uu[j+10], uuid->uu[j+11],
                                uuid->uu[j+12], uuid->uu[j+13], uuid->uu[j+14], uuid->uu[j+15]
                               );
            }

            break;
            /* 5 */
        case BT_PROPERTY_TYPE_OF_DEVICE:
            devicetype= (unsigned int *)property->val;

            /* 0 - BLE, 1 - BT, 2 - DUAL MODE */
            BT_DBG_INFO(BT_DEBUG_GAP, "type = %ld, len = %ld, devicetype = %ld", (long)property->type, (long)property->len, (unsigned long)*devicetype);
            break;
            /* 6 */
        case BT_PROPERTY_SERVICE_RECORD:
            BT_DBG_INFO(BT_DEBUG_GAP, "type = %ld, len = %ld, Others.", (long)property->type, (long)property->len);
            break;
            /* 7 */
        case BT_PROPERTY_ADAPTER_SCAN_MODE:
            mode = (bt_scan_mode_t *)property->val;

            BT_DBG_INFO(BT_DEBUG_GAP, "type = %ld, len = %ld, scan mode = %ld", (long)property->type, (long)property->len, (unsigned long)*mode);
            localProperty.scan_mode = (UINT8)*mode;
            break;
            /* 8 */
        case BT_PROPERTY_ADAPTER_BONDED_DEVICES:
            btaddr = (bt_bdaddr_t *)property->val;

            BT_DBG_INFO(BT_DEBUG_GAP, "type = %ld, len = %ld, bonded_addr = %02X:%02X:%02X:%02X:%02X:%02X", (long)property->type, (long)property->len,
                          btaddr->address[0], btaddr->address[1], btaddr->address[2],
                          btaddr->address[3], btaddr->address[4], btaddr->address[5]);
            break;
            /* 9 */
        case BT_PROPERTY_ADAPTER_DISCOVERY_TIMEOUT:
            disc_timeout = (unsigned int *)property->val;

            BT_DBG_INFO(BT_DEBUG_GAP, "type = %ld, len = %ld, disc_timeout = %ld", (long)property->type, (long)property->len, (unsigned long)*disc_timeout);
            localProperty.disctimeout = (UINT32)*disc_timeout;
            break;
            /* 10 */
        case BT_PROPERTY_REMOTE_FRIENDLY_NAME:
            name = (char *)property->val;
            BT_DBG_INFO(BT_DEBUG_GAP, "type = %ld, len = %ld, bdname = %s", (long)property->type, (long)property->len, name);

            break;
            /* 11 */
        case BT_PROPERTY_REMOTE_RSSI:
            rssi = (char *)property->val;
            BT_DBG_INFO(BT_DEBUG_GAP, "type = %ld, len = %ld, rssi = %ld", (long)property->type, (long)property->len, (long)(*rssi));
            break;
            /* 12 */
        case BT_PROPERTY_REMOTE_VERSION_INFO:
            BT_DBG_INFO(BT_DEBUG_GAP, "type = %ld, len = %ld, Others.", (long)property->type, (long)property->len);
            break;
            /* FF */
        case BT_PROPERTY_REMOTE_DEVICE_TIMESTAMP:
            BT_DBG_INFO(BT_DEBUG_GAP, "type = %ld, len = %ld, Others.", (long)property->type, (long)property->len);
            break;
        default:
            BT_DBG_INFO(BT_DEBUG_GAP, "type = %ld, len = %ld, Others.", (long)property->type, (long)property->len);
            break;
        }
    }

    BT_DBG_INFO(BT_DEBUG_GAP, "==========================================End==================================================");
    if (5 <= num_properties)
    {
        bt_gap_handle_get_properties_cnf(&localProperty);
    }

}

void linuxbt_gap_parse_properties_info(int num_properties, bt_property_t *properties)
{
    int i;
    int j;
    int k;
    int len;
    char *name;
    bt_scan_mode_t *mode;
    bt_bdaddr_t *btaddr;
    bt_uuid_t *uuid;
    bt_property_t *property;
    unsigned int *devicetype;
    unsigned int *disc_timeout;
    char *rssi;
    BT_UUIDS_T local_uuid;
    int uuid_exist = 0;
    int uuid_idx = 0;
    BT_GAP_BONDED_DEV_T bonded_devices;

    g_local_property.powered = (power_state == BT_STATE_ON)?1:0;
    BT_DBG_NORMAL(BT_DEBUG_GAP, "num_properties = %ld", (long)num_properties);

    for (i = 0; i < num_properties; i++)
    {
        property = &properties[i];

        switch (property->type)
        {
            /* 1 */
        case BT_PROPERTY_BDNAME:
            name = (char *)property->val;
            BT_DBG_INFO(BT_DEBUG_GAP, "type = %ld, len = %ld, bdname = %s", (long)property->type, (long)property->len, name);
            strncpy(g_local_property.name, name, MAX_NAME_LEN);
            break;
            /* 2 */
        case BT_PROPERTY_BDADDR:
            btaddr = (bt_bdaddr_t *)property->val;
            BT_DBG_INFO(BT_DEBUG_GAP, "type = %ld, len = %ld, bdaddr = %02X:%02X:%02X:%02X:%02X:%02X", (long)property->type, (long)property->len,
                          btaddr->address[0], btaddr->address[1], btaddr->address[2],
                          btaddr->address[3], btaddr->address[4], btaddr->address[5]);
            linuxbt_btaddr_htos(btaddr, g_local_property.bdAddr);
            break;
            /* 3 */
        case BT_PROPERTY_UUIDS:
            uuid = (bt_uuid_t*)property->val;
            len = property->len;
            for (j=0; j<(len/sizeof(bt_uuid_t)); j++)
            {
                /* print uuid */
                BT_DBG_INFO(BT_DEBUG_GAP, "type = %ld, len = %ld, uuid = %02X%02X%02X%02X-%02X%02X-%02X%02X-%02X%02X-%02X%02X%02X%02X%02X%02X",
                                (long)property->type, (long)property->len,
                                uuid[j].uu[0], uuid[j].uu[1], uuid[j].uu[2], uuid[j].uu[3],
                                uuid[j].uu[4], uuid[j].uu[5], uuid[j].uu[6], uuid[j].uu[7],
                                uuid[j].uu[8], uuid[j].uu[9], uuid[j].uu[10], uuid[j].uu[11],
                                uuid[j].uu[12], uuid[j].uu[13], uuid[j].uu[14], uuid[j].uu[15]
                               );
                linuxbt_print_uuid(&uuid[j], local_uuid.uuid);
                /* if current uuid not in list add it */
                for (k=0; k<g_local_property.uuid_num; k++)
                {
                    if (!strncmp(local_uuid.uuid, g_local_property.local_uuids[k].uuid, strlen(local_uuid.uuid)))
                    {
                        BT_DBG_INFO(BT_DEBUG_GAP, "uuid in list no need add it");
                        uuid_exist = 1;
                    }
                }
                if (!uuid_exist)
                {
                    uuid_idx = g_local_property.uuid_num;
                    strncpy(g_local_property.local_uuids[uuid_idx].uuid, local_uuid.uuid, sizeof(local_uuid.uuid));
                    g_local_property.uuid_num++;
                }
            }

            break;
            /* 5 */
        case BT_PROPERTY_TYPE_OF_DEVICE:
            devicetype= (unsigned int *)property->val;

            /* 0 - BLE, 1 - BT, 2 - DUAL MODE */
            BT_DBG_INFO(BT_DEBUG_GAP, "type = %ld, len = %ld, devicetype = %ld", (long)property->type, (long)property->len, (unsigned long)*devicetype);
            break;
            /* 6 */
        case BT_PROPERTY_SERVICE_RECORD:
            BT_DBG_INFO(BT_DEBUG_GAP, "type = %ld, len = %ld, Others.", (long)property->type, (long)property->len);
            break;
            /* 7 */
        case BT_PROPERTY_ADAPTER_SCAN_MODE:
            mode = (bt_scan_mode_t *)property->val;

            BT_DBG_INFO(BT_DEBUG_GAP, "type = %ld, len = %ld, scan mode = %ld", (long)property->type, (long)property->len, (unsigned long)*mode);
            g_local_property.scan_mode = (UINT8)*mode;
            break;
            /* 8 */
        case BT_PROPERTY_ADAPTER_BONDED_DEVICES:
            btaddr = (bt_bdaddr_t *)property->val;
            memset(&bonded_devices, 0x0, sizeof(bonded_devices));
            bonded_devices.num_devices = property->len / sizeof(bt_bdaddr_t);
            BT_DBG_INFO(BT_DEBUG_GAP, "type = %ld, len = %ld", (long)property->type, (long)property->len);
            for (k=0; k<bonded_devices.num_devices; k++)
            {
                BT_DBG_INFO(BT_DEBUG_GAP, "bonded_addr = %02X:%02X:%02X:%02X:%02X:%02X",
                          btaddr[k].address[0], btaddr->address[1], btaddr->address[2],
                          btaddr[k].address[3], btaddr->address[4], btaddr->address[5]);
                linuxbt_btaddr_htos(&btaddr[k], bonded_devices.devices[k].address);
                BT_DBG_INFO(BT_DEBUG_GAP, "bonded_devices[%d]:%s", k, bonded_devices.devices[k].address);
            }
            memcpy(&g_local_property.paired_devices, &bonded_devices, sizeof(bonded_devices));
            break;
            /* 9 */
        case BT_PROPERTY_ADAPTER_DISCOVERY_TIMEOUT:
            disc_timeout = (unsigned int *)property->val;

            BT_DBG_INFO(BT_DEBUG_GAP, "type = %ld, len = %ld, disc_timeout = %ld", (long)property->type, (long)property->len, (unsigned long)*disc_timeout);
            g_local_property.disctimeout = (UINT32)*disc_timeout;
            break;
            /* 10 */
        case BT_PROPERTY_REMOTE_FRIENDLY_NAME:
            name = (char *)property->val;
            BT_DBG_INFO(BT_DEBUG_GAP, "type = %ld, len = %ld, bdname = %s", (long)property->type, (long)property->len, name);

            break;
            /* 11 */
        case BT_PROPERTY_REMOTE_RSSI:
            rssi = (char *)property->val;
            BT_DBG_INFO(BT_DEBUG_GAP, "type = %ld, len = %ld, rssi = %ld", (long)property->type, (long)property->len, (long)(*rssi));
            break;
            /* 12 */
        case BT_PROPERTY_REMOTE_VERSION_INFO:
            BT_DBG_INFO(BT_DEBUG_GAP, "type = %ld, len = %ld, Others.", (long)property->type, (long)property->len);
            break;
            /* FF */
        case BT_PROPERTY_REMOTE_DEVICE_TIMESTAMP:
            BT_DBG_INFO(BT_DEBUG_GAP, "type = %ld, len = %ld, Others.", (long)property->type, (long)property->len);
            break;
        default:
            BT_DBG_INFO(BT_DEBUG_GAP, "type = %ld, len = %ld, Others.", (long)property->type, (long)property->len);
            break;
        }
    }

}

#if defined(MTK_LINUX_GAP_PTS_TEST) && (MTK_LINUX_GAP_PTS_TEST == TRUE)
void linuxbt_gap_set_bond_target_dev_info(BT_DEV_MAPPING_INFO * info, int num_properties, bt_property_t *properties)
{
    int i;
    bt_property_t *property;
    BT_DBG_NORMAL(BT_DEBUG_GAP, "=======================================Properties==============================================");
    BT_DBG_NORMAL(BT_DEBUG_GAP, "[GAP] num_properties = %ld", (long)num_properties);
    for (i = 0; i < num_properties; i++)
    {
        property = &properties[i];
        switch (property->type)
        {
            /* 1 */
        case BT_PROPERTY_BDNAME:
            memcpy(info->name, (char*)property->val, sizeof(info->name));
            BT_DBG_NORMAL(BT_DEBUG_GAP, "[GAP] type = %ld, len = %ld, bdname = %s", (long)property->type, (long)property->len, info->name);
            break;
            /* 4 */
        case BT_PROPERTY_CLASS_OF_DEVICE:
            info->cod = (unsigned int *)property->val;
            BT_DBG_NORMAL(BT_DEBUG_GAP, "[GAP] type = %ld, len = %ld, cod = 0x%X", (long)property->type, (long)property->len, info->cod);
            break;
            /* 5 */
        case BT_PROPERTY_TYPE_OF_DEVICE:
            info->devicetype= (unsigned int *)property->val;
            /* 0 - BLE, 1 - BT, 2 - DUAL MODE */
            BT_DBG_NORMAL(BT_DEBUG_GAP, "[GAP] type = %ld, len = %ld, devicetype = %ld", (long)property->type, (long)property->len, (unsigned long)(info->devicetype));
            break;
        default:
            BT_DBG_NORMAL(BT_DEBUG_GAP, "[GAP] type = %ld, len = %ld, Others.", (long)property->type, (long)property->len);
            break;
        }
    }
}
#endif
// Callback functions implementation
void linuxbt_gap_state_changed_cb(bt_state_t state)
{
    BT_DBG_NORMAL(BT_DEBUG_GAP, "%s() state: %ld", __FUNCTION__, (long)state);

    switch (state)
    {
    case BT_STATE_OFF:
        BT_DBG_NOTICE(BT_DEBUG_GAP, "BT STATE OFF");
        power_state = BT_STATE_OFF;
        set_bt_power_on(FALSE);
        bt_gap_handle_power_off_cnf();
        break;
    case BT_STATE_ON:
        BT_DBG_NOTICE(BT_DEBUG_GAP, "BT STATE ON");
        power_state = BT_STATE_ON;
#if ENABLE_A2DP_SRC
        if (!get_bt_a2dp_be_sink())
        {
            linuxbt_a2dp_src_init();
        }
#endif
#if ENABLE_A2DP_SINK
        if (get_bt_a2dp_be_sink())
        {
            linuxbt_a2dp_sink_init();
        }
#endif

#if ENABLE_HID_PROFILE_H
        linuxbt_hid_init();
#endif //__BT_HID_PROFILE__
#if ENABLE_HID_PROFILE_D
        linuxbt_hidd_init();
#endif //__BT_HIDD_PROFILE__

#if ENABLE_AVRCP_PROFILE
        if (get_bt_a2dp_be_sink())
        {
            linuxbt_rc_init();
        }
        else
        {
            linuxbt_rc_tg_init();
        }
#endif
        //linuxbt_pan_init();
#if ENABLE_GATT_PROFILE
        linuxbt_gatt_init();
#endif
#if ENABLE_SPP_PROFILE
        linuxbt_spp_init();
#endif
#if ENABLE_HFP_CLIENT
        linuxbt_hfclient_init();
#endif

        bt_gap_handle_power_on_cnf();
        break;
    default:
        break;
    }
}

void linuxbt_gap_properties_cb(bt_status_t status,
                               int num_properties,
                               bt_property_t *properties)
{
    BT_DBG_NORMAL(BT_DEBUG_GAP, "%s() status: %ld", __FUNCTION__, (long)status);
    int i = 0;
    bt_bdaddr_t bonded_devices[MAX_DEV_NUM];
    char bdAddr[MAX_BDADDR_LEN];
    memset(bdAddr, 0, MAX_BDADDR_LEN);
    memset(bonded_devices, 0, sizeof(bonded_devices));
    linuxbt_gap_show_properties_info(num_properties, properties);
    linuxbt_gap_parse_properties_info(num_properties, properties);
    if ((2 == num_properties &&
        (BT_PROPERTY_ADAPTER_SCAN_MODE == properties[0].type
        || (BT_PROPERTY_ADAPTER_SCAN_MODE == properties[1].type)))
        || ((1 == num_properties) &&(BT_PROPERTY_ADAPTER_SCAN_MODE ==
        properties[0].type)))
    {
        bt_gap_handle_set_scanmode_cnf(status,properties);
    }

    if ((6 == num_properties) && (BT_PROPERTY_ADAPTER_BONDED_DEVICES == properties[4].type))
    {
        memcpy(bonded_devices, properties[4].val, sizeof(bonded_devices));
        bond_target_dev_info.dev_num = properties[4].len / 6;
        if (bond_target_dev_info.dev_num > MAX_DEV_NUM)
        {
            bond_target_dev_info.dev_num = 0;
            BT_DBG_ERROR(BT_DEBUG_GAP, "dev num over max num");
            return;
        }
        BT_DBG_ERROR(BT_DEBUG_GAP, "bdaddr = dev num %d", bond_target_dev_info.dev_num);
        for (i = 0; i < bond_target_dev_info.dev_num; i++)
        {
            BT_DBG_ERROR(BT_DEBUG_GAP, "bdaddr = %02X:%02X:%02X:%02X:%02X:%02X",
            bonded_devices[i].address[0], bonded_devices[i].address[1], bonded_devices[i].address[2],
            bonded_devices[i].address[3], bonded_devices[i].address[4], bonded_devices[i].address[5]);
            if (bonded_devices[i].address[0] == 0 &&
                bonded_devices[i].address[1] == 0 &&
                bonded_devices[i].address[2] == 0 &&
                bonded_devices[i].address[3] == 0 &&
                bonded_devices[i].address[4] == 0 &&
                bonded_devices[i].address[5] == 0 )
            {
                break;
            }
            else
            {
                linuxbt_btaddr_htos(&bonded_devices[i], bdAddr);
                memcpy(&bond_target_dev_info.mapping_list[i].bdAddr, bdAddr, strlen(bdAddr) + 1);
                linuxbt_gap_get_remote_device_properties_handler(bond_target_dev_info.mapping_list[i].bdAddr);
            }
        }
    }
}

void linuxbt_gap_remote_device_properties_cb(bt_status_t status,
        bt_bdaddr_t *bd_addr,
        int num_properties,
        bt_property_t *properties)
{
    FUNC_ENTRY;
    int i = 0;
    int bond_device_num = 0;
    char str_bt_addr[MAX_BDADDR_LEN];
    memset(str_bt_addr, 0, MAX_BDADDR_LEN);
    if (0 < num_properties && BT_PROPERTY_BDNAME == properties->type)
    {
        if (5 == num_properties)
        {
            for (i = 0; i < bond_target_dev_info.dev_num; i++)
            {
                linuxbt_btaddr_htos(bd_addr, str_bt_addr);
                if (0 == strncmp(bond_target_dev_info.mapping_list[i].bdAddr, str_bt_addr, strlen(bond_target_dev_info.mapping_list[i].bdAddr)))
                {
#if defined(MTK_LINUX_GAP_PTS_TEST) && (MTK_LINUX_GAP_PTS_TEST == TRUE)
                    linuxbt_gap_set_bond_target_dev_info(&bond_target_dev_info.mapping_list[i], num_properties, properties);
#else
                    memcpy(bond_target_dev_info.mapping_list[i].name, (char*)properties[0].val, sizeof(bond_target_dev_info.mapping_list[i].name));
                    bond_target_dev_info.mapping_list[i].cod = *(UINT32*)properties[2].val;
                    bond_target_dev_info.mapping_list[i].devicetype = *(UINT32*)properties[3].val;
#endif
                    break;
                }
            }

            for (i = 0; i < bond_target_dev_info.dev_num; i++)
            {
                if (strlen(bond_target_dev_info.mapping_list[i].name) != 0)
                {
                    bond_device_num++;
                }
            }

            if (bond_device_num == bond_target_dev_info.dev_num)
            {
                linuxbt_gap_get_bonded_device_cb();
            }
            else
            {
                BT_DBG_ERROR(BT_DEBUG_GAP, "device num is  is %d total is %d", bond_device_num,bond_target_dev_info.dev_num);
            }
        }
        BT_DBG_NOTICE(BT_DEBUG_GAP, "discovery_update cbk");
        bt_gap_handle_discovery_update_ind(bd_addr,num_properties, properties);
    }
    else if (1 == num_properties && BT_PROPERTY_UUIDS == properties->type)
    {
        /* Added for BLE device discovery and update icon according to uuid in advertising packet */
        // update remote uuid
        BT_DBG_NOTICE(BT_DEBUG_GAP, "update remote uuid");
        return;
    }
    else
    {
        linuxbt_gap_show_properties_debug(num_properties, properties);
    }
    bluetooth_gap_update_target_get_mapping_info(bd_addr, num_properties, properties);
}


void linuxbt_gap_device_found_cb(int num_properties,
                                 bt_property_t *properties)
{
    BT_DBG_NORMAL(BT_DEBUG_GAP, "%s()", __FUNCTION__);

    linuxbt_gap_show_properties_debug(num_properties, properties);
    bt_gap_handle_discovery_result_ind(num_properties, properties);
}


void linuxbt_gap_discovery_state_changed_cb(bt_discovery_state_t state)
{
    BT_DBG_NORMAL(BT_DEBUG_GAP, "%s() state: %ld", __FUNCTION__, (long)state);

    switch (state)
    {
    case BT_DISCOVERY_STOPPED:
        bt_gap_handle_discovery_stop_ind();
        BT_DBG_NOTICE(BT_DEBUG_GAP, "BT Search Device Stop.");
        break;
    case BT_DISCOVERY_STARTED:
        BT_DBG_NOTICE(BT_DEBUG_GAP, "BT Search Device Start...");
        break;
    default:
        break;
    }
}

void linuxbt_gap_pin_request_cb(bt_bdaddr_t *remote_bd_addr,
                                bt_bdname_t *bd_name, uint32_t cod,bool min_16_digit)
{
    BT_DBG_NORMAL(BT_DEBUG_GAP, "%s()", __FUNCTION__);

    bt_pin_code_t pin;
    char str_pin[16];
    uint32_t pin_code;
    bt_status_t ret = BT_STATUS_SUCCESS;
    uint8_t fg_accept = 1;

    if (NULL == g_bt_interface)
    {
        BT_DBG_ERROR(BT_DEBUG_GAP, "Failed to get GAP interface");
        return;
    }
    memset(&pin, 0, 16);
    pin_code = 100000 + (uint32_t)(random() % 900000 - 1);

    BT_DBG_ERROR(BT_DEBUG_GAP, "pin code = %d\n", pin_code);


    snprintf(str_pin, sizeof(str_pin), "%d", pin_code);
    memcpy(pin.pin, str_pin, sizeof(pin.pin));

    BT_DBG_ERROR(BT_DEBUG_GAP, "pin code = %s\n", str_pin);
    BT_DBG_ERROR(BT_DEBUG_GAP, "pin.pin code = %s\n", pin.pin);

    bt_gap_get_pin_code_cb(remote_bd_addr, (CHAR*)bd_name->name, &pin, (UINT8 *)&fg_accept);

    ret = g_bt_interface->pin_reply(remote_bd_addr, (uint8_t)fg_accept, 6, &pin);
    if (BT_SUCCESS != linuxbt_return_value_convert(ret))
    {
        BT_DBG_ERROR(BT_DEBUG_GAP, "pin_reply error!\n");
    }
}


void linuxbt_gap_ssp_request_cb(bt_bdaddr_t *remote_bd_addr,
                                bt_bdname_t *bd_name, uint32_t cod,
                                bt_ssp_variant_t pairing_variant,
                                uint32_t passkey)
{
    BT_DBG_NORMAL(BT_DEBUG_GAP, "%s()", __FUNCTION__);
    bt_status_t ret = BT_STATUS_SUCCESS;
    uint8_t fg_accept = 1;

    if (remote_bd_addr)
    {
        bt_bdaddr_t *btaddr = remote_bd_addr;

        BT_DBG_INFO(BT_DEBUG_GAP, "REMOTE BDADDR = %02X:%02X:%02X:%02X:%02X:%02X",
                      btaddr->address[0], btaddr->address[1], btaddr->address[2],
                      btaddr->address[3], btaddr->address[4], btaddr->address[5]);
    }
    else
    {
        BT_DBG_ERROR(BT_DEBUG_GAP, "remote_bd_addr is NULL!");
    }
    if (NULL == g_bt_interface)
    {
        BT_DBG_ERROR(BT_DEBUG_GAP, "Failed to get GAP interface");
        return;
    }
    if (bd_name)
    {
        BT_DBG_NORMAL(BT_DEBUG_GAP, "BDNAME = %s", bd_name->name);
    }
    BT_DBG_INFO(BT_DEBUG_GAP, "cod = 0x%08X, pairing_variant = %ld, passkey = %ld.", cod, (long)pairing_variant, (unsigned long)passkey);
    BT_DBG_INFO(BT_DEBUG_GAP, "passkey = %ld.", (unsigned long)passkey);

     // Just Works
    if (BT_SSP_VARIANT_CONSENT == pairing_variant)
    {
        BT_DBG_WARNING(BT_DEBUG_GAP, "Just Works not need notify upper layer!\n\n");
    }
    else
    {
        bt_gap_get_pass_key_cb(remote_bd_addr, (CHAR*)bd_name->name, passkey, (UINT8 *)&fg_accept);
    }

    ret = g_bt_interface->ssp_reply(remote_bd_addr, pairing_variant, fg_accept, passkey);
    if (BT_SUCCESS != linuxbt_return_value_convert(ret))
    {
        BT_DBG_ERROR(BT_DEBUG_GAP, "ssp_reply error!\n");
    }
}

void linuxbt_gap_bond_state_changed_cb(bt_status_t status,
                                       bt_bdaddr_t *remote_bd_addr,
                                       bt_bond_state_t state)
{
    //CHAR   bdAddr[MAX_BDADDR_LEN];
    BT_DBG_NORMAL(BT_DEBUG_GAP, "%s(), status = %ld, state = %ld", __FUNCTION__, (long)status, (long)state);

    switch (status)
    {
    case BT_STATUS_SUCCESS:
        BT_DBG_NOTICE(BT_DEBUG_GAP, "BT bond status is successful(%ld), ", (long)status);
        break;
    default:
        BT_DBG_NOTICE(BT_DEBUG_GAP, "BT bond status is failed(%ld), ", (long)status);
        break;
    }

    switch (state)
    {
    case BT_BOND_STATE_NONE:
        BT_DBG_NOTICE(BT_DEBUG_GAP, "state is none.");
        break;
    case BT_BOND_STATE_BONDING:
        BT_DBG_NOTICE(BT_DEBUG_GAP, "state is bonding.");
        break;
    case BT_BOND_STATE_BONDED:
        BT_DBG_NOTICE(BT_DEBUG_GAP, "state is bonded.");
        break;
    default:
        break;
    }
#if 0
    /*special proccess for HOGP */
    if ((status == BT_STATUS_SUCCESS) && (state == BT_BOND_STATE_BONDED))
    {
        BT_DBG_NORMAL(BT_DEBUG_GAP, "%s(), special", __FUNCTION__);

        linuxbt_btaddr_htos(remote_bd_addr, bdAddr);

        if(1 ==  get_hogp_connect_state(bdAddr))
        {
            linuxbt_hid_connect_int_handler(bdAddr);
            set_hogp_connect_state(2, bdAddr);
        }

    }
#endif
    if (remote_bd_addr)
    {
        bt_bdaddr_t *btaddr = remote_bd_addr;

        if ( (status == BT_STATUS_SUCCESS) && (state == BT_BOND_STATE_BONDED) )
        {
            BT_DBG_NOTICE(BT_DEBUG_GAP, "REMOTE BDADDR = %02X:%02X:%02X:%02X:%02X:%02X",
                          btaddr->address[0], btaddr->address[1], btaddr->address[2],
                          btaddr->address[3], btaddr->address[4], btaddr->address[5]);
        }
        else
        {
            BT_DBG_NOTICE(BT_DEBUG_GAP, "REMOTE BDADDR = %02X:%02X:%02X:%02X:%02X:%02X",
                          btaddr->address[0], btaddr->address[1], btaddr->address[2],
                          btaddr->address[3], btaddr->address[4], btaddr->address[5]);
        }
    }
    else
    {
        BT_DBG_ERROR(BT_DEBUG_GAP, "remote_bd_addr is NULL!");
    }
    bt_gap_handle_bond_state_cb(state, remote_bd_addr);
    //linuxbt_btaddr_htos(remote_bd_addr, bt_addr);
}

void linuxbt_gap_acl_state_changed_cb(bt_status_t status, bt_bdaddr_t *remote_bd_addr,
                                      bt_acl_state_t state)
{
    BT_DBG_NORMAL(BT_DEBUG_GAP, "%s(), status = %ld, state = %ld", __FUNCTION__, (long)status, (long)state);

    switch (status)
    {
    case BT_STATUS_SUCCESS:
        BT_DBG_NOTICE(BT_DEBUG_GAP, "BT bond status is successful(%ld), ", (long)status);
        break;
    default:
        BT_DBG_NOTICE(BT_DEBUG_GAP, "BT bond status is failed(%ld), ", (long)status);
        break;
    }

    switch (state)
    {
    case BT_ACL_STATE_CONNECTED:
        BT_DBG_NOTICE(BT_DEBUG_GAP, "acl is connected.");

        break;
    case BT_ACL_STATE_DISCONNECTED:
        BT_DBG_NOTICE(BT_DEBUG_GAP, "acl is disconnected.");
        break;
    default:
        break;
    }

    if (remote_bd_addr)
    {
        bt_bdaddr_t *btaddr = remote_bd_addr;

        BT_DBG_NORMAL(BT_DEBUG_GAP, "REMOTE BDADDR = %02X:%02X:%02X:%02X:%02X:%02X",
                      btaddr->address[0], btaddr->address[1], btaddr->address[2],
                      btaddr->address[3], btaddr->address[4], btaddr->address[5]);
    }
    else
    {
        BT_DBG_ERROR(BT_DEBUG_GAP, "remote_bd_addr is NULL!");
    }
    bt_gap_handle_link_state_ind(status, remote_bd_addr, state);
}

void linuxbt_gap_get_bonded_device_cb()
{
    BT_DBG_NORMAL(BT_DEBUG_GAP, "%s() ", __FUNCTION__);
    for (int i = 0; i < bond_target_dev_info.dev_num; i++)
    {
        BT_DBG_NORMAL(BT_DEBUG_GAP, "bonded bdAddr = %s", bond_target_dev_info.mapping_list[i].bdAddr);
        BT_DBG_NORMAL(BT_DEBUG_GAP, "name = %s", bond_target_dev_info.mapping_list[i].name);
        BT_DBG_NORMAL(BT_DEBUG_GAP, "device_types = %d", bond_target_dev_info.mapping_list[i].devicetype);
        BT_DBG_NORMAL(BT_DEBUG_GAP, "cod = 0x%6x ", bond_target_dev_info.mapping_list[i].cod);
    }
    return;
}

void linuxbt_gap_acl_disconnect_reason_callback(bt_bdaddr_t *remote_bd_addr, uint8_t reason)
{
    if (NULL == remote_bd_addr)
    {
        BT_DBG_ERROR(BT_DEBUG_GAP, "null pointer of remote_bd_addr");
        return;
    }

    BT_DBG_NORMAL(BT_DEBUG_GAP, "REMOTE BDADDR = %02X:%02X:%02X:%02X:%02X:%02X",
                remote_bd_addr->address[0], remote_bd_addr->address[1], remote_bd_addr->address[2],
                remote_bd_addr->address[3], remote_bd_addr->address[4], remote_bd_addr->address[5]);
    BT_DBG_NORMAL(BT_DEBUG_GAP, "ACL disconnect reason = %ld",(unsigned long)reason);
    bt_gap_acl_disconnect_reason_ck(remote_bd_addr, reason);

}

void linuxbt_gap_adapter_init_cb()
{
    FUNC_ENTRY;
}
/*
void linuxbt_gap_reset_ind_cb(void)
{
    BT_DBG_NORMAL(BT_DEBUG_GAP, "%s()", __FUNCTION__);
}*/

void linuxbt_gap_thread_event_cb(bt_cb_thread_evt evt)
{
    BT_DBG_NORMAL(BT_DEBUG_GAP, "%s()", __FUNCTION__);
}

void linuxbt_gap_dut_mode_recv_cb(uint16_t opcode, uint8_t *buf, uint8_t len)
{
    BT_DBG_NORMAL(BT_DEBUG_GAP, "%s()", __FUNCTION__);
}

void linuxbt_gap_le_test_mode_cb(bt_status_t status, uint16_t num_packets)
{
    BT_DBG_NORMAL(BT_DEBUG_GAP, "%s()", __FUNCTION__);
}

void linuxbt_gap_get_rssi_cb(bt_status_t status, bt_bdaddr_t *remote_bd_addr , int rssi_value)
{
    BT_DBG_NORMAL(BT_DEBUG_GAP, "%s()  %ld ", __FUNCTION__, (long)(rssi_value));
    bt_gap_get_rssi_result_cb(rssi_value);
}

const void *linuxbt_gap_get_profile_interface(const char *profile_id)
{
    if (NULL != g_bt_interface)
    {
        return g_bt_interface->get_profile_interface(profile_id);
    }
    else
    {
        BT_DBG_ERROR(BT_DEBUG_GAP, "Failed to get GAP interface");
        return NULL;
    }
}

int linuxbt_gap_init(void)
{
    FUNC_ENTRY;

    bt_status_t ret = BT_STATUS_SUCCESS;
    // Init bluetooth interface.
    open_bluetooth_stack(NULL, "Stack Linux", (struct hw_device_t**)&g_bt_device);
    if (NULL == g_bt_device)
    {
        BT_DBG_ERROR(BT_DEBUG_GAP, "Failed to open Bluetooth stack.");
        return BT_ERR_STATUS_NOT_READY;
    }

    g_bt_interface = g_bt_device->get_bluetooth_interface();
    if (NULL == g_bt_interface)
    {
        BT_DBG_ERROR(BT_DEBUG_GAP, "Failed to get Bluetooth interface");
        return BT_ERR_STATUS_FAIL;
    }

    g_bt_interface->set_os_callouts(&g_callouts);

    ret = g_bt_interface->init(&g_bt_callbacks);
    if (BT_SUCCESS != linuxbt_return_value_convert(ret))
    {
        BT_DBG_ERROR(BT_DEBUG_GAP, "gap init error!");
    }

#if defined(MTK_LINUX_GAP) && (MTK_LINUX_GAP == TRUE)
    linuxbt_gap_ex_interface = (btgap_ex_interface_t *) linuxbt_gap_get_profile_interface(BT_PROFILE_GAP_EX_ID);
    if (NULL == linuxbt_gap_ex_interface)
    {
        BT_DBG_ERROR(BT_DEBUG_GAP, "Failed to get Bluetooth extended interface");
        return BT_ERR_STATUS_FAIL;
    }

    ret = linuxbt_gap_ex_interface->init(&linuxbt_gap_ex_callbacks);
    if (BT_SUCCESS != linuxbt_return_value_convert(ret))
    {
        BT_DBG_ERROR(BT_DEBUG_GAP, "gap init error!");
    }
#endif

    //default gap disable
    return linuxbt_return_value_convert(ret);//g_bt_interface->enable();
}

int linuxbt_gap_deinit_profiles(void)
{
    BT_DBG_NORMAL(BT_DEBUG_GAP, "%s()", __FUNCTION__);
#if ENABLE_A2DP_SRC
    linuxbt_a2dp_src_deinit();
#endif
#if ENABLE_A2DP_SINK
    linuxbt_a2dp_sink_deinit();
#endif
#if ENABLE_AVRCP_PROFILE
    linuxbt_rc_deinit();
    linuxbt_rc_tg_deinit();
#endif

#if ENABLE_HID_PROFILE_H
	linuxbt_hid_deinit();
#endif //__BT_HID_PROFILE__
#if ENABLE_HID_PROFILE_D
	linuxbt_hidd_deinit();
#endif //__BT_HIDD_PROFILE__

    //linuxbt_pan_deinit();
#if ENABLE_GATT_PROFILE
    linuxbt_gatt_deinit();
#endif
#if ENABLE_SPP_PROFILE
    linuxbt_spp_deinit();
#endif
#if ENABLE_HFP_CLIENT
    linuxbt_hfclient_deinit();
#endif

    return 0;
}

int linuxbt_gap_deinit(void)
{
    //bt_status_t ret = BT_STATUS_SUCCESS;

    if (NULL != g_bt_interface)
    {
        //g_bt_interface->disable();
        g_bt_interface->cleanup();
    }
    else
    {
        BT_DBG_ERROR(BT_DEBUG_GAP, "Failed to get GAP interface");
        return BT_ERR_STATUS_FAIL;
    }


    return BT_SUCCESS;//linuxbt_return_value_convert(ret);
}

int linuxbt_gap_set_bt_wifi_ratio(uint8_t bt_ratio, uint8_t wifi_ratio)
{
    uint8_t cmd[5] = {0};

    cmd[0] = 0xf1;
    cmd[1] = 0xfc;
    cmd[2] = 0x02;
    cmd[3] = bt_ratio;
    cmd[4] = wifi_ratio;

#if defined(MTK_LINUX_GAP) && (MTK_LINUX_GAP == TRUE)
    linuxbt_gap_ex_interface->send_hci(cmd, sizeof(cmd));
#endif

    return BT_SUCCESS;
}

#if defined(MTK_LINUX_GAP) && (MTK_LINUX_GAP == TRUE)
int linuxbt_gap_set_afh(uint8_t *mask)
{//  mask work as a array,the index is 0~10
    int i;
    unsigned char cmd[14]={0};
    cmd[0] = 0x3f;
    cmd[1] = 0x0c;
    cmd[2] = 0x0a;
    for ( i = 0 ; i < 10 ; i++ )
    {
        cmd[i+3] = mask[i];
    }
    linuxbt_gap_ex_interface->send_hci((uint8_t*)cmd,13);
    return BT_SUCCESS;
}

int linuxbt_gap_config_clear()
{
    if(NULL == g_bt_interface)
    {
        BT_DBG_ERROR(BT_DEBUG_GAP, "Failed to get GAP interface");
        return BT_ERR_STATUS_FAIL;
    }
    g_bt_interface->config_clear();
    return BT_SUCCESS;
}

int linuxbt_dump(char *mode)
{
    char mode_transfer[1][20];
    FILE *dump_file = fopen("/3rd_rw/bluedroid/dump.txt", "w+");
    if (dump_file == NULL)
    {
        BT_DBG_ERROR(BT_DEBUG_GAP, "Failed to open dump_file");
        return 0;
    }
    int fd = fileno(dump_file);
    if(NULL == g_bt_interface)
    {
        BT_DBG_ERROR(BT_DEBUG_GAP, "Failed to get GAP interface");
        return BT_ERR_STATUS_FAIL;
    }
    if (strncmp(mode,"text",strlen(mode)) == 0)
    {
        strncpy(mode_transfer[0],"--proto-text",12);
    }
    else if (strncmp(mode,"bin",strlen(mode)) == 0)
    {
        strncpy(mode_transfer[0],"--proto-bin",11);
    }
    g_bt_interface->dump(fd ,(const char**)mode_transfer);
    return BT_SUCCESS;
}

int linuxbt_interop_database_add(uint16_t feature, bt_bdaddr_t *remote_bd_addr,size_t len)
{
    if(NULL == g_bt_interface)
    {
        BT_DBG_ERROR(BT_DEBUG_GAP, "Failed to get GAP interface");
        return BT_ERR_STATUS_FAIL;
    }
    g_bt_interface->interop_database_add(feature, remote_bd_addr,len);
    return BT_SUCCESS;
}

int linuxbt_interop_database_clear()
{
    if(NULL == g_bt_interface)
    {
        BT_DBG_ERROR(BT_DEBUG_GAP, "Failed to get GAP interface");
        return BT_ERR_STATUS_FAIL;
    }
    g_bt_interface->interop_database_clear();
    return BT_SUCCESS;
}


int linuxbt_ble_pair(char *pbt_addr)
{
    FUNC_ENTRY;
    bt_bdaddr_t bdaddr;
    bt_status_t ret = BT_STATUS_SUCCESS;

    memset(&bdaddr, 0, sizeof(bt_bdaddr_t));

    if (NULL == pbt_addr)
    {
        BT_DBG_ERROR(BT_DEBUG_GAP, "null pointer of pbt_addr");
        return BT_ERR_STATUS_PARM_INVALID;
    }
    if (NULL == g_bt_interface)
    {
        BT_DBG_ERROR(BT_DEBUG_GAP, "Failed to get GAP interface");
        return BT_ERR_STATUS_FAIL;
    }
    linuxbt_btaddr_stoh(pbt_addr, &bdaddr);
    BT_DBG_NORMAL(BT_DEBUG_GAP, "start pairing to %02X:%02X:%02X:%02X:%02X:%02X",
                  bdaddr.address[0], bdaddr.address[1], bdaddr.address[2],
                  bdaddr.address[3], bdaddr.address[4], bdaddr.address[5]);

    ret = g_bt_interface->create_bond(&bdaddr,2);

    return linuxbt_return_value_convert(ret);
}
#endif

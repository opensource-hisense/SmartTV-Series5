#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <assert.h>
#include <fcntl.h>
#include <sys/prctl.h>
#include <pthread.h>

#include "bluetooth.h"
#include "bt_hh.h"
#include "u_bt_mw_hid.h"
#if defined(MTK_LINUX_HID) && (MTK_LINUX_HID == TRUE)
#include "mtk_bt_hh.h"
#endif
#include "linuxbt_hid_if.h"
#include "linuxbt_common.h"
#include "bt_mw_common.h"
#include "bt_mw_hidh.h"
#include "bt_mw_gap.h"

extern void *linuxbt_gap_get_profile_interface(const char *profile_id);

static void linuxbt_hid_connection_state_cb(bt_bdaddr_t *bd_addr, bthh_connection_state_t state)
{
    FUNC_ENTRY;
    switch (state) {
        case BTHH_CONN_STATE_CONNECTED:
            BT_DBG_NORMAL(BT_DEBUG_HID, "[HID] Connection State : '%d' connected", state);
            bluetooth_handle_hid_connect_cb(bd_addr);
            //usleep(1000000);
            //CHAR BT_ADDR[18] = {0};
            //linuxbt_btaddr_htos(bd_addr, BT_ADDR);
            //bluetooth_gap_start_sniff_mode(TRUE,BT_ADDR,18,18,2,0);
            break;
        case BTHH_CONN_STATE_CONNECTING:
            BT_DBG_NORMAL(BT_DEBUG_HID, "[HID] Connection State : '%d' connecting", state);
            break;
        case BTHH_CONN_STATE_DISCONNECTED:
            BT_DBG_NORMAL(BT_DEBUG_HID, "[HID] Connection State : '%d' disconnected", state);
            bluetooth_handle_hid_disconnect_cb(bd_addr);
            break;
        case BTHH_CONN_STATE_DISCONNECTING:
            BT_DBG_NORMAL(BT_DEBUG_HID, "[HID] Connection State : '%d' disconnecting", state);
            break;
#if 0
        case BTHH_CONN_STATE_REJECT:
            BT_DBG_NORMAL(BT_DEBUG_HID, "[HID] Reject connection State : '%d' reject", state);
            bluetooth_handle_hid_connect_fail_cb(bd_addr, BT_HID_CONNECTION_REJECT);
            break;
#endif
        case BTHH_CONN_STATE_FAILED_GENERIC:
            BT_DBG_NORMAL(BT_DEBUG_HID, "[HID] Connection State : '%d' connnect fail", state);
            bluetooth_handle_hid_connect_fail_cb(bd_addr, BT_HID_CONNECT_FAIL);
            break;
        default:
            BT_DBG_NORMAL(BT_DEBUG_HID, "[HID] Connection State : '%d'", state);
    }
}


static void linuxbt_hid_virtual_unplug_cb(bt_bdaddr_t *bd_addr, bthh_status_t hh_status)
{
    BT_DBG_NORMAL(BT_DEBUG_HID, "[HID] %s() state: %s(%d)", __FUNCTION__, (hh_status == 0) ? "SUCCESS" : "FAILED", hh_status);
}

static void linuxbt_hid_info_cb(bt_bdaddr_t *bd_addr, bthh_hid_info_t hid_info)
{
    BT_DBG_NORMAL(BT_DEBUG_HID, "[HID] %s() ", __FUNCTION__);
    BT_DBG_NORMAL(BT_DEBUG_HID, "[HID] attr_mask = 0x%x", hid_info.attr_mask);
    BT_DBG_NORMAL(BT_DEBUG_HID, "[HID] sub_class = 0x%x", hid_info.sub_class);
    BT_DBG_NORMAL(BT_DEBUG_HID, "[HID] app_id = 0x%x", hid_info.app_id);
    BT_DBG_NORMAL(BT_DEBUG_HID, "[HID] vendor_id = 0x%x", hid_info.vendor_id);
    BT_DBG_NORMAL(BT_DEBUG_HID, "[HID] product_id = 0x%x", hid_info.product_id);
    BT_DBG_NORMAL(BT_DEBUG_HID, "[HID] version = %d", hid_info.version);
    BT_DBG_NORMAL(BT_DEBUG_HID, "[HID] ctry_code = %d", hid_info.ctry_code);
    BT_DBG_NORMAL(BT_DEBUG_HID, "[HID] dl_len = %d", hid_info.dl_len);
    BT_DBG_NORMAL(BT_DEBUG_HID, "[HID] dsc_list = %s", hid_info.dsc_list);
}

static void linuxbt_hid_protocol_mode_cb(bt_bdaddr_t *bd_addr, bthh_status_t hh_status, bthh_protocol_mode_t mode)
{
    BT_DBG_NORMAL(BT_DEBUG_HID, "[HID] %s() state: %s", __FUNCTION__, hh_status==0?"SUCCESS":"FAILED");
    BT_DBG_INFO(BT_DEBUG_HID, "[HID] mode = %s(%d)", mode==BTHH_REPORT_MODE?"REPORT_MODE":"BOOT_MODE", mode);
}

static void linuxbt_hid_idle_time_cb(bt_bdaddr_t *bd_addr, bthh_status_t hh_status, int idle_rate)
{
    BT_DBG_NORMAL(BT_DEBUG_HID, "[HID] %s() state: %s", __FUNCTION__, hh_status==0?"SUCCESS":"FAILED");
}

static void linuxbt_hid_get_report_cb(bt_bdaddr_t *bd_addr, bthh_status_t hh_status, uint8_t* rpt_data, int rpt_size)
{
    BT_DBG_NORMAL(BT_DEBUG_HID, "[HID] %s() state: %s", __FUNCTION__, hh_status==0?"SUCCESS":"FAILED");
    BT_DBG_NORMAL(BT_DEBUG_HID, "Data Len = %d", rpt_size);

    BT_DBG_NORMAL(BT_DEBUG_HID, "Data = 0x%02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X ",
                rpt_size>0?rpt_data[0]:0,
                rpt_size>1?rpt_data[1]:0,
                rpt_size>2?rpt_data[2]:0,
                rpt_size>3?rpt_data[3]:0,
                rpt_size>4?rpt_data[4]:0,
                rpt_size>5?rpt_data[5]:0,
                rpt_size>6?rpt_data[6]:0,
                rpt_size>7?rpt_data[7]:0,
                rpt_size>9?rpt_data[8]:0,
                rpt_size>10?rpt_data[9]:0,
                rpt_size>11?rpt_data[10]:0,
                rpt_size>12?rpt_data[11]:0,
                rpt_size>13?rpt_data[12]:0,
                rpt_size>14?rpt_data[13]:0,
                rpt_size>15?rpt_data[14]:0,
                rpt_size>16?rpt_data[15]:0);
  if (rpt_data[0] == 0)
  {
    BT_DBG_NORMAL(BT_DEBUG_HID, "Report ID is NULL.Report ID cannot be NULL. Invalid HID report recieved ");
  }
}

static void linuxbt_hid_handshake_cb(bt_bdaddr_t *bd_addr, bthh_status_t hh_status)
{
    BT_DBG_NORMAL(BT_DEBUG_HID, "[HID] %s() state: %s", __FUNCTION__, (hh_status == 0) ? "SUCCESS" : "FAILED" );
}


static bthh_interface_t *g_bt_hid_interface = NULL;
static bthh_callbacks_t g_bt_hid_callbacks =
{
    sizeof(bthh_callbacks_t),
    linuxbt_hid_connection_state_cb,
    linuxbt_hid_info_cb,
    linuxbt_hid_protocol_mode_cb,
    linuxbt_hid_idle_time_cb,
    linuxbt_hid_get_report_cb,
    linuxbt_hid_virtual_unplug_cb,
    linuxbt_hid_handshake_cb,
};
extern INT32 bluetooth_get_dev_mapping_name(BT_DEV_MAPPING_INFO *pmapping_dev);

static void linuxbt_hid_get_remote_name_cb(bt_bdaddr_t *bd_addr, char *name)
{
    BT_DBG_NORMAL(BT_DEBUG_HID, "[HID] ");
    BT_DEV_MAPPING_INFO target_mapping_dev;
    CHAR bt_address[MAX_BDADDR_LEN];
    INT32 i4_ret;

    if ((NULL == bd_addr) || (NULL == name))
    {
        BT_DBG_ERROR(BT_DEBUG_HID, "[HID] null pointer of parameter");
        return;
    }

    i4_ret = linuxbt_btaddr_htos(bd_addr, bt_address);
    if (BT_SUCCESS != i4_ret)
    {
        BT_DBG_ERROR(BT_DEBUG_HID, "[HID] invalid parameter");
        return;
    }
    strncpy(target_mapping_dev.bdAddr,
            bt_address,
            sizeof(target_mapping_dev.bdAddr)
           );
    if (0 == bluetooth_get_dev_mapping_name(&target_mapping_dev))
    {
        memcpy(name, target_mapping_dev.name, MAX_NAME_LEN);
    }
    else
    {
        memcpy(name, "MTK HID", MAX_NAME_LEN);
    }
    BT_DBG_NORMAL(BT_DEBUG_HID, "[HID] name = %s", name);
}

#if defined(MTK_LINUX_HID) && (MTK_LINUX_HID == TRUE)
static const bthh_ex_interface_t *g_bt_hid_ex_interface = NULL;

static bthh_ex_callbacks_t g_bt_hid_ex_callbacks =
{
    sizeof(bthh_ex_callbacks_t),
    linuxbt_hid_get_remote_name_cb,
};
#endif

#if 0
int linuxbt_hid_activate_handler(void)
{
    BT_DBG_NORMAL(BT_DEBUG_HID, "[HID] %s()", __FUNCTION__);
    bt_status_t ret = BT_STATUS_SUCCESS;
    if (NULL == g_bt_hid_interface)
    {
        BT_DBG_ERROR(BT_DEBUG_HID, "[HID] Failed to get HID interface");
        return BT_ERR_STATUS_FAIL;
    }

    ret = g_bt_hid_interface->activate();
    return linuxbt_return_value_convert(ret);
}

int linuxbt_hid_deactivate_handler(void)
{
    BT_DBG_NORMAL(BT_DEBUG_HID, "[HID] %s()", __FUNCTION__);
    bt_status_t ret = BT_STATUS_SUCCESS;
    if (NULL == g_bt_hid_interface)
    {
        BT_DBG_ERROR(BT_DEBUG_HID, "[HID] Failed to get HID interface");
        return BT_ERR_STATUS_FAIL;
    }

    ret = g_bt_hid_interface->deactivate();
    return linuxbt_return_value_convert(ret);
}
#endif
int linuxbt_hid_connect_int_handler(char *pbt_addr)
{
    BT_DBG_NORMAL(BT_DEBUG_HID, "[HID] ");
    bt_bdaddr_t bdaddr;
    bt_status_t ret = BT_STATUS_SUCCESS;
    memset(&bdaddr, 0, sizeof(bt_bdaddr_t));
    if (NULL == pbt_addr)
    {
        BT_DBG_ERROR(BT_DEBUG_HID, "null pointer of pbt_addr");
        return BT_ERR_STATUS_PARM_INVALID;
    }
    if (g_bt_hid_interface == NULL)
    {
        BT_DBG_ERROR(BT_DEBUG_HID, "[HID] Failed to get HID interface");
        return BT_ERR_STATUS_FAIL;
    }

    linuxbt_btaddr_stoh(pbt_addr, &bdaddr);
    BT_DBG_NORMAL(BT_DEBUG_HID, "HID connect to %02X:%02X:%02X:%02X:%02X:%02X",
        bdaddr.address[0], bdaddr.address[1], bdaddr.address[2],
        bdaddr.address[3], bdaddr.address[4], bdaddr.address[5]);

    ret = g_bt_hid_interface->connect(&bdaddr);
    BT_DBG_NORMAL(BT_DEBUG_HID, "[HID] connect ret=%d", ret);
    return linuxbt_return_value_convert(ret);
}

int linuxbt_hid_disconnect_handler(char *pbt_addr)
{
    BT_DBG_NORMAL(BT_DEBUG_HID, "[HID] ");
    bt_bdaddr_t bdaddr;
    bt_status_t ret = BT_STATUS_SUCCESS;
    memset(&bdaddr, 0, sizeof(bt_bdaddr_t));
    if (NULL == g_bt_hid_interface)
    {
        BT_DBG_ERROR(BT_DEBUG_HID, "[HID] Failed to get HID interface");
        return BT_ERR_STATUS_FAIL;
    }
    if (NULL == pbt_addr)
    {
        BT_DBG_ERROR(BT_DEBUG_HID, "null pointer of pbt_addr");
        return BT_ERR_STATUS_PARM_INVALID;
    }
    linuxbt_btaddr_stoh(pbt_addr, &bdaddr);
    BT_DBG_NORMAL(BT_DEBUG_HID, "HID disconnect %02X:%02X:%02X:%02X:%02X:%02X",
        bdaddr.address[0], bdaddr.address[1], bdaddr.address[2],
        bdaddr.address[3], bdaddr.address[4], bdaddr.address[5]);

    ret = g_bt_hid_interface->disconnect(&bdaddr);
    return linuxbt_return_value_convert(ret);
}

int linuxbt_hid_set_output_report_handler(char *pbt_addr, char *preport_data)
{
    BT_DBG_NORMAL(BT_DEBUG_HID, "[HID] ");
    bt_status_t ret = BT_STATUS_SUCCESS;
    bt_bdaddr_t bdaddr;
    memset(&bdaddr, 0, sizeof(bt_bdaddr_t));
    linuxbt_btaddr_stoh(pbt_addr, &bdaddr);
    if (NULL == pbt_addr || NULL == preport_data)
    {
        BT_DBG_ERROR(BT_DEBUG_HID, "null pointer of pbt_addr or preport_data");
        return BT_ERR_STATUS_PARM_INVALID;
    }
    if(NULL == g_bt_hid_interface)
    {
        BT_DBG_ERROR(BT_DEBUG_HID, "[HID] Failed to get HID interface");
        return BT_ERR_STATUS_FAIL;
    }

    ret = g_bt_hid_interface->set_report(&bdaddr, BTHH_OUTPUT_REPORT, preport_data);
/*
    rpt_size = strlen(preport_data);
    hex_len = (strlen(preport_data) + 1) / 2;
    linuxbt_btaddr_stoh(pbt_addr, &bdaddr);
    BT_DBG_NORMAL(BT_DEBUG_HID, "HID set_report %02X:%02X:%02X:%02X:%02X:%02X",
            bdaddr.address[0], bdaddr.address[1], bdaddr.address[2],
            bdaddr.address[3], bdaddr.address[4], bdaddr.address[5]);
    BT_DBG_NORMAL(BT_DEBUG_HID, "%s:rpt_size=%d, hex_len=%d", __FUNCTION__, rpt_size, hex_len);
    hex_bytes_filled = ascii_2_hex(preport_data, hex_len, hex_buf);
    BT_DBG_NORMAL(BT_DEBUG_HID, "%s:hex_bytes_filled=%d", __FUNCTION__, hex_bytes_filled);
    for(i=0;i<hex_len;i++)
    {
        BT_DBG_NORMAL(BT_DEBUG_HID, "hex values= %02X",hex_buf[i]);
    }
    if (hex_bytes_filled)
    {
       ret = g_bt_hid_interface->set_report(&bdaddr, BTHH_OUTPUT_REPORT, (char*)hex_buf);
       BT_DBG_NORMAL(BT_DEBUG_HID, "set_output_report");
       return linuxbt_return_value_convert(ret);
    }
    else
    {
       BT_DBG_NORMAL(BT_DEBUG_HID, "%s:hex_bytes_filled <= 0", __FUNCTION__);
       return BT_ERR_STATUS_FAIL;
    }
*/
    return linuxbt_return_value_convert(ret);
}

int linuxbt_hid_get_input_report_handler(char *pbt_addr, int reportId, int bufferSize)
{
    BT_DBG_NORMAL(BT_DEBUG_HID, "[HID] ");
    bt_status_t ret = BT_STATUS_SUCCESS;
    bt_bdaddr_t bdaddr;
    memset(&bdaddr, 0, sizeof(bt_bdaddr_t));
    if (NULL == g_bt_hid_interface)
    {
        BT_DBG_ERROR(BT_DEBUG_HID, "[HID] Failed to get HID interface");
        return BT_ERR_STATUS_FAIL;
    }
    if (NULL == pbt_addr)
    {
        BT_DBG_ERROR(BT_DEBUG_HID, "null pointer of pbt_addr");
        return BT_ERR_STATUS_PARM_INVALID;
    }
    linuxbt_btaddr_stoh(pbt_addr, &bdaddr);
    BT_DBG_NORMAL(BT_DEBUG_HID, "HID get_report %02X:%02X:%02X:%02X:%02X:%02X",
                        bdaddr.address[0], bdaddr.address[1], bdaddr.address[2],
                        bdaddr.address[3], bdaddr.address[4], bdaddr.address[5]);
    ret = g_bt_hid_interface->get_report(&bdaddr,BTHH_INPUT_REPORT,reportId,bufferSize);
    return linuxbt_return_value_convert(ret);
}

int linuxbt_hid_get_output_report_handler(char *pbt_addr, int reportId, int bufferSize)
{
    BT_DBG_NORMAL(BT_DEBUG_HID, "[HID] ");
    bt_status_t ret = BT_STATUS_SUCCESS;
    bt_bdaddr_t bdaddr;
    memset(&bdaddr, 0, sizeof(bt_bdaddr_t));
    linuxbt_btaddr_stoh(pbt_addr, &bdaddr);
    BT_DBG_NORMAL(BT_DEBUG_HID, "HID get_report %02X:%02X:%02X:%02X:%02X:%02X",
        bdaddr.address[0], bdaddr.address[1], bdaddr.address[2],
        bdaddr.address[3], bdaddr.address[4], bdaddr.address[5]);
    ret = g_bt_hid_interface->get_report(&bdaddr,BTHH_OUTPUT_REPORT,reportId,bufferSize);
    return linuxbt_return_value_convert(ret);

}

int linuxbt_hid_set_input_report_handler(char *pbt_addr, char *preport_data)
{
    BT_DBG_NORMAL(BT_DEBUG_HID, "[HID] ");
    bt_bdaddr_t bdaddr;
    bt_status_t ret = BT_STATUS_SUCCESS;
    memset(&bdaddr, 0, sizeof(bt_bdaddr_t));
    if (NULL == pbt_addr || NULL == preport_data)
    {
        BT_DBG_ERROR(BT_DEBUG_HID, "null pointer of pbt_addr or preport_data");
        return BT_ERR_STATUS_PARM_INVALID;
    }
    if(NULL == g_bt_hid_interface)
    {
        BT_DBG_ERROR(BT_DEBUG_HID, "[HID] Failed to get HID interface");
        return BT_ERR_STATUS_FAIL;
    }
    linuxbt_btaddr_stoh(pbt_addr, &bdaddr);
    ret = g_bt_hid_interface->set_report(&bdaddr, BTHH_INPUT_REPORT, preport_data);
/*
    rpt_size = strlen(preport_data);
    hex_len = (strlen(preport_data) + 1) / 2;
    linuxbt_btaddr_stoh(pbt_addr, &bdaddr);
    BT_DBG_NORMAL(BT_DEBUG_HID, "HID set_report %02X:%02X:%02X:%02X:%02X:%02X",
            bdaddr.address[0], bdaddr.address[1], bdaddr.address[2],
            bdaddr.address[3], bdaddr.address[4], bdaddr.address[5]);
    BT_DBG_NORMAL(BT_DEBUG_HID, "%s:rpt_size=%d, hex_len=%d", __FUNCTION__, rpt_size, hex_len);
    hex_bytes_filled = ascii_2_hex(preport_data, hex_len, hex_buf);
    BT_DBG_NORMAL(BT_DEBUG_HID, "%s:hex_bytes_filled=%d", __FUNCTION__, hex_bytes_filled);
    for(i=0;i<hex_len;i++)
    {
       BT_DBG_NORMAL(BT_DEBUG_HID, "hex values= %02X",hex_buf[i]);
    }
    if (hex_bytes_filled)
    {
       ret = g_bt_hid_interface->set_report(&bdaddr, BTHH_INPUT_REPORT, (char*)hex_buf);
       BT_DBG_NORMAL(BT_DEBUG_HID, "set_input_report|");
    }
    else
    {
       BT_DBG_NORMAL(BT_DEBUG_HID, "%s:hex_bytes_filled <= 0", __FUNCTION__);
       ret = BT_STATUS_FAIL;
    }
*/
    return linuxbt_return_value_convert(ret);
}



int linuxbt_hid_get_feature_report_handler(char *pbt_addr, int reportId, int bufferSize)
{
    BT_DBG_NORMAL(BT_DEBUG_HID, "[HID] ");
    bt_bdaddr_t bdaddr;
    bt_status_t ret = BT_STATUS_SUCCESS;
    memset(&bdaddr, 0, sizeof(bt_bdaddr_t));
    if (NULL == g_bt_hid_interface)
    {
        BT_DBG_ERROR(BT_DEBUG_HID, "[HID] Failed to get HID interface");
        return BT_ERR_STATUS_FAIL;
    }
    if (NULL == pbt_addr)
    {
        BT_DBG_ERROR(BT_DEBUG_HID, "null pointer of pbt_addr");
        return BT_ERR_STATUS_PARM_INVALID;
    }
    linuxbt_btaddr_stoh(pbt_addr, &bdaddr);
    BT_DBG_NORMAL(BT_DEBUG_HID, "HID get_report %02X:%02X:%02X:%02X:%02X:%02X",
        bdaddr.address[0], bdaddr.address[1], bdaddr.address[2],
        bdaddr.address[3], bdaddr.address[4], bdaddr.address[5]);
    ret = g_bt_hid_interface->get_report(&bdaddr,BTHH_FEATURE_REPORT,reportId,bufferSize);
    return linuxbt_return_value_convert(ret);
}

int linuxbt_hid_set_protocol_handler(char *pbt_addr, int protocol_mode)
{
    BT_DBG_NORMAL(BT_DEBUG_HID, "[HID] ");
    bt_bdaddr_t bdaddr;
    bt_status_t ret = BT_STATUS_SUCCESS;
    memset(&bdaddr, 0, sizeof(bt_bdaddr_t));
    if (NULL == g_bt_hid_interface)
    {
        BT_DBG_ERROR(BT_DEBUG_HID, "[HID] Failed to get HID interface");
        return BT_ERR_STATUS_FAIL;
    }
    if (NULL == pbt_addr)
    {
        BT_DBG_ERROR(BT_DEBUG_HID, "null pointer of pbt_addr");
        return BT_ERR_STATUS_PARM_INVALID;
    }
    /* protocol mode ( 0:boot protocol, 1:report protocol)  */
    linuxbt_btaddr_stoh(pbt_addr, &bdaddr);
    BT_DBG_INFO(BT_DEBUG_HID, "HID set_protocol %02X:%02X:%02X:%02X:%02X:%02X",
        bdaddr.address[0], bdaddr.address[1], bdaddr.address[2],
        bdaddr.address[3], bdaddr.address[4], bdaddr.address[5]);
    ret = g_bt_hid_interface->set_protocol(&bdaddr,protocol_mode);
    return linuxbt_return_value_convert(ret);
}




#if 0
int linuxbt_hid_send_control_handler(char *pbt_addr, int pcontrol_mode)
{
    BT_DBG_NORMAL(BT_DEBUG_HID, "[HID] %s()", __FUNCTION__);
    bt_bdaddr_t bdaddr;
    bt_status_t ret = BT_STATUS_SUCCESS;
    memset(&bdaddr, 0, sizeof(bt_bdaddr_t));
    if (NULL == g_bt_hid_interface)
    {
        BT_DBG_ERROR(BT_DEBUG_HID, "[HID] Failed to get HID interface");
        return BT_ERR_STATUS_FAIL;
    }
    if (NULL == pbt_addr)
    {
        BT_DBG_ERROR(BT_DEBUG_HID, "null pointer of pbt_addr");
        return BT_ERR_STATUS_PARM_INVALID;
    }
    /*[control, 3:SUSPEND | 4:EXIT_SUSPEND | 5:VIRTUAL_CABLE_UNPLUG]*/
    linuxbt_btaddr_stoh(pbt_addr, &bdaddr);
    BT_DBG_INFO(BT_DEBUG_HID, "[HID] send_control %02X:%02X:%02X:%02X:%02X:%02X",
        bdaddr.address[0], bdaddr.address[1], bdaddr.address[2],
        bdaddr.address[3], bdaddr.address[4], bdaddr.address[5]);
    ret = g_bt_hid_interface->send_control(&bdaddr,pcontrol_mode);
    return linuxbt_return_value_convert(ret);
}
#endif

int linuxbt_hid_get_protocol_handler(char *pbt_addr)
{
    BT_DBG_NORMAL(BT_DEBUG_HID, "[HID] ");
    bt_bdaddr_t bdaddr;
    bt_status_t ret = BT_STATUS_SUCCESS;
    memset(&bdaddr, 0, sizeof(bt_bdaddr_t));
    if (NULL == g_bt_hid_interface)
    {
        BT_DBG_ERROR(BT_DEBUG_HID, "[HID] Failed to get HID interface");
        return BT_ERR_STATUS_FAIL;
    }
    if (NULL == pbt_addr)
    {
        BT_DBG_ERROR(BT_DEBUG_HID, "null pointer of pbt_addr");
        return BT_ERR_STATUS_PARM_INVALID;
    }
    linuxbt_btaddr_stoh(pbt_addr, &bdaddr);
    BT_DBG_INFO(BT_DEBUG_HID, "HID get_protocol %02X:%02X:%02X:%02X:%02X:%02X",
        bdaddr.address[0], bdaddr.address[1], bdaddr.address[2],
        bdaddr.address[3], bdaddr.address[4], bdaddr.address[5]);
    ret = g_bt_hid_interface->get_protocol(&bdaddr, 0);
    return linuxbt_return_value_convert(ret);
}


int linuxbt_hid_virtual_unplug_handler(char *pbt_addr)
{
    BT_DBG_NORMAL(BT_DEBUG_HID, "[HID] ");
    bt_bdaddr_t bdaddr;
    bt_status_t ret = BT_STATUS_SUCCESS;
    memset(&bdaddr, 0, sizeof(bt_bdaddr_t));
    if (NULL == g_bt_hid_interface)
    {
        BT_DBG_ERROR(BT_DEBUG_HID, "[HID] Failed to get HID interface");
        return BT_ERR_STATUS_FAIL;
    }
    if (NULL == pbt_addr)
    {
        BT_DBG_ERROR(BT_DEBUG_HID, "null pointer of pbt_addr");
        return BT_ERR_STATUS_PARM_INVALID;
    }
    linuxbt_btaddr_stoh(pbt_addr, &bdaddr);
    BT_DBG_INFO(BT_DEBUG_HID, "HID virtual_upl %02X:%02X:%02X:%02X:%02X:%02X",
        bdaddr.address[0], bdaddr.address[1], bdaddr.address[2],
        bdaddr.address[3], bdaddr.address[4], bdaddr.address[5]);
    ret = g_bt_hid_interface->virtual_unplug(&bdaddr);
    return linuxbt_return_value_convert(ret);
}

int linuxbt_hid_set_feature_report_handler(char *pbt_addr, char *preport_data)
{
    BT_DBG_NORMAL(BT_DEBUG_HID, "[HID] ");
    bt_bdaddr_t bdaddr;
    bt_status_t ret = BT_STATUS_SUCCESS;
    memset(&bdaddr, 0, sizeof(bt_bdaddr_t));
    if (NULL == pbt_addr || NULL == preport_data)
    {
        BT_DBG_ERROR(BT_DEBUG_HID, "null pointer of pbt_addr or preport_data");
        return BT_ERR_STATUS_PARM_INVALID;
    }
    if(g_bt_hid_interface == NULL)
    {
        BT_DBG_ERROR(BT_DEBUG_HID, "[HID] Failed to get HID interface");
        return BT_ERR_STATUS_FAIL;
    }
    linuxbt_btaddr_stoh(pbt_addr, &bdaddr);
    ret = g_bt_hid_interface->set_report(&bdaddr, BTHH_FEATURE_REPORT, preport_data);
/*
    rpt_size = strlen(preport_data);
    hex_len = (strlen(preport_data) + 1) / 2;
    linuxbt_btaddr_stoh(pbt_addr, &bdaddr);
    BT_DBG_INFO(BT_DEBUG_HID, "HID set_report %02X:%02X:%02X:%02X:%02X:%02X",
        bdaddr.address[0], bdaddr.address[1], bdaddr.address[2],
        bdaddr.address[3], bdaddr.address[4], bdaddr.address[5]);
    BT_DBG_INFO(BT_DEBUG_HID, "rpt_size:%d, hex_len=%d", rpt_size, hex_len);
    hex_bytes_filled = ascii_2_hex(preport_data, hex_len, hex_buf);
    BT_DBG_INFO(BT_DEBUG_HID, "hex_bytes_filled=%d", hex_bytes_filled);
    for(i=0;i<hex_len;i++)
    {
        BT_DBG_INFO(BT_DEBUG_HID, "hex values= %02X",hex_buf[i]);
    }
    if (hex_bytes_filled)
    {
        ret = g_bt_hid_interface->set_report(&bdaddr, BTHH_FEATURE_REPORT, (char*)hex_buf);
        BT_DBG_INFO(BT_DEBUG_HID, "set_feature_report");
    }
    else
    {
        BT_DBG_INFO(BT_DEBUG_HID, "hex_bytes_filled <= 0");
        ret = BT_STATUS_FAIL;
    }
*/
    return linuxbt_return_value_convert(ret);
}

int linuxbt_hid_send_data_handler(char *pbt_addr, char *data)
{
    BT_DBG_NORMAL(BT_DEBUG_HID, "[HID] ");
    bt_status_t ret = BT_STATUS_SUCCESS;
    bt_bdaddr_t bdaddr;

    memset(&bdaddr, 0, sizeof(bt_bdaddr_t));

    if (NULL == pbt_addr || NULL == data)
    {
        BT_DBG_ERROR(BT_DEBUG_HID, "null pointer of pbt_addr or preport_data");
        return BT_ERR_STATUS_PARM_INVALID;
    }
    if (NULL == g_bt_hid_interface)
    {
        BT_DBG_ERROR(BT_DEBUG_HID, "[HID] Failed to get HID interface");
        return BT_ERR_STATUS_FAIL;
    }
    linuxbt_btaddr_stoh(pbt_addr, &bdaddr);
    ret = g_bt_hid_interface->send_data(&bdaddr, data);

    return linuxbt_return_value_convert(ret);
}

int linuxbt_hid_init(void)
{
    BT_DBG_NORMAL(BT_DEBUG_HID, "[HID] ");
    bt_status_t ret = BT_STATUS_SUCCESS;

    // Get HID interface
    g_bt_hid_interface = (bthh_interface_t *) linuxbt_gap_get_profile_interface(BT_PROFILE_HIDHOST_ID);
    if (NULL == g_bt_hid_interface)
    {
        BT_DBG_ERROR(BT_DEBUG_HID, "[HID] Failed to get HID interface");
        return BT_ERR_STATUS_FAIL;
    }

    // Init HID interface
    ret = g_bt_hid_interface->init(&g_bt_hid_callbacks);
    if (ret == BT_STATUS_SUCCESS)
    {
        BT_DBG_NORMAL(BT_DEBUG_HID, "[HID] success to init HID interface");
    }
    else if(ret == BT_STATUS_DONE)
    {
        BT_DBG_NORMAL(BT_DEBUG_HID, "[HID] already init HID interface");
    }
    else
    {
        BT_DBG_ERROR(BT_DEBUG_HID, "[HID] Failed to init HID interface");
    }

#if defined(MTK_LINUX_HID) && (MTK_LINUX_HID == TRUE)
    g_bt_hid_ex_interface = (bthh_ex_interface_t *) linuxbt_gap_get_profile_interface(BT_PROFILE_HIDHOST_EX_ID);
    if (NULL == g_bt_hid_ex_interface)
    {
        BT_DBG_ERROR(BT_DEBUG_HID, "Failed to get HID extended interface");
        return BT_ERR_STATUS_FAIL;
    }

    ret = g_bt_hid_ex_interface->btif_hh_init_ex(&g_bt_hid_ex_callbacks);
    if (BT_SUCCESS != linuxbt_return_value_convert(ret))
    {
        BT_DBG_ERROR(BT_DEBUG_HID, "gap init error!");
    }
#endif
    return linuxbt_return_value_convert(ret);
}

int linuxbt_hid_deinit(void)
{
    BT_DBG_NORMAL(BT_DEBUG_HID, "[HID] ");
    bt_status_t ret = BT_STATUS_SUCCESS;
    // Deinit HID interface
    if (g_bt_hid_interface != NULL)
    {
        g_bt_hid_interface->cleanup();
    }

    return linuxbt_return_value_convert(ret);
}

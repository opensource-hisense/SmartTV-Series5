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
/* FILE NAME:  bt_devices_info.c
 * PURPOSE:
 *  {1. this file is extracted from original bt_mw_gap.c, in order to store BT devices information,
The following BT devices are included:
(1) Local BT device;
(2) A2dp remote SRC devices,history list and current connected BT device;
(3) A2dp remote SNK devices,history list and current connected BT device;
(4) Hid device
(5) BT scan result
*/

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>

#include "bluetooth.h"
#include "bt_mw_common.h"
#include "linuxbt_common.h"
#include "bluetooth_sync.h"
#include "bt_mw_devices_info.h"
#include "bt_mw_gap.h"

extern BtAppGapInquiryResponseCbk AppInquiryCbk;
extern BtAppEventCbk      BtAppCbk;
BT_GAP_LOCAL_PROPERTIES_RESULT g_localProperty;
static BT_TARGET_DEV_LIST g_paired_dev_info;
/*for a2dp sink role*/
BT_TARGET_DEV_LIST g_paired_src_dev_history;
BT_TARGET_DEV_LIST *g_pt_paired_src_dev_history = NULL;

/*for hid host role*/
BT_HID_TARGET_DEV_LIST g_paired_hid_dev_history;
BT_HID_TARGET_DEV_LIST *g_pt_paired_hid_dev_history = NULL;
BT_HID_STATUS_STRUCT_LIST g_hid_status_list;
BT_HID_STATUS_STRUCT_LIST *g_pt_hid_status_list = NULL;

/*for a2dp source role*/
BT_TARGET_DEV_LIST g_paired_sink_dev_history;
BT_TARGET_DEV_LIST *g_pt_paired_sink_dev_history = NULL;
static BOOL g_fghistorylist_init = FALSE;
static BOOL g_fg_local_dev_init = FALSE;
extern BT_DEV_MAPPING_LIST scan_target_dev_info;
extern BT_DEV_MAPPING_LIST all_target_dev_info;
extern UINT32 fg_bt_inquiry_type;
extern BT_DEV_MAPPING_LIST notify_target_dev_info;

BOOL bt_SRC_is_SONY_TV = FALSE;
BOOL bluetooth_scan_device_info_ready(BT_DEV_MAPPING_INFO detected_map_info)
{
    FUNC_ENTRY;
    BT_DBG_INFO(BT_DEBUG_COMM, "addr:%s", detected_map_info.bdAddr);
    BT_DBG_INFO(BT_DEBUG_COMM, "name:%s", detected_map_info.name);
    BT_DBG_INFO(BT_DEBUG_COMM, "cod:0x%02X", (unsigned int)detected_map_info.cod);
    if ((0 < strlen(detected_map_info.bdAddr)) &&( 0 < strlen(detected_map_info.name)) && ( 0 != detected_map_info.cod))
    {
        return TRUE;
    }
    return FALSE;
}

INT32 bluetooth_get_notify_mapping_name(BT_DEV_MAPPING_INFO *pmapping_dev, BT_DEV_MAPPING_LIST p_target_dev_info)
{
    FUNC_ENTRY;
    INT32 i = 0;
    BT_CHECK_POINTER(BT_DEBUG_COMM, pmapping_dev);

    for (i = 0; i < p_target_dev_info.dev_num; i++)
    {
        /* find target devie */
        if (0 == strcmp(pmapping_dev->bdAddr, p_target_dev_info.mapping_list[i].bdAddr))
        {
            BT_DBG_NOTICE(BT_DEBUG_COMM, "find target device");
            /*update target device*/
            if (0 != strlen(p_target_dev_info.mapping_list[i].name))
            {
                strncpy(pmapping_dev->name, p_target_dev_info.mapping_list[i].name, MAX_NAME_LEN);
            }
            else
            {
                BT_DBG_WARNING(BT_DEBUG_COMM, "but target device have no name in the saved information!");
            }
            if (0 != p_target_dev_info.mapping_list[i].cod)
            {
                pmapping_dev->cod = p_target_dev_info.mapping_list[i].cod;
            }
            else
            {
                pmapping_dev->cod = 0;
                BT_DBG_WARNING(BT_DEBUG_COMM, "but target device's cod is 0 in the saved information!");
            }
            BT_DBG_INFO(BT_DEBUG_COMM, "[%ld] cod:%ld", (long)i, (long)pmapping_dev->cod);
            BT_DBG_INFO(BT_DEBUG_COMM, "name:%s", pmapping_dev->name);
            return BT_SUCCESS;
        }
        BT_DBG_INFO(BT_DEBUG_COMM, "[%ld] addr:%s", (long)i, p_target_dev_info.mapping_list[i].bdAddr);
        BT_DBG_INFO(BT_DEBUG_COMM, "[%ld] name:%s", (long)i, p_target_dev_info.mapping_list[i].name);
        BT_DBG_INFO(BT_DEBUG_COMM, "[%ld] cod:%ld", (long)i, (long)p_target_dev_info.mapping_list[i].cod);
    }
    FUNC_EXIT;
    return BT_ERR_STATUS_FAIL;
}

INT32 bluetooth_get_dev_mapping_name(BT_DEV_MAPPING_INFO *pmapping_dev)
{
    FUNC_ENTRY;
    INT32 i4_ret= 0;
    BT_CHECK_POINTER(BT_DEBUG_COMM, pmapping_dev);

    i4_ret = bluetooth_get_notify_mapping_name(pmapping_dev, notify_target_dev_info);

    if (BT_SUCCESS != i4_ret)
    {
        BT_DBG_WARNING(BT_DEBUG_COMM, "Not found in notify table, find in all table");
        i4_ret = bluetooth_get_notify_mapping_name(pmapping_dev, all_target_dev_info);
    }

    FUNC_EXIT;
    return i4_ret;
}
INT32 bluetooth_gap_update_mapping_info(BT_DEV_MAPPING_INFO *pmapping_dev, BT_DEV_MAPPING_LIST *pall_target_dev_info)
{
    INT32 i = 0;
    BT_CHECK_POINTER(BT_DEBUG_COMM, pmapping_dev);
    BT_CHECK_POINTER(BT_DEBUG_COMM, pall_target_dev_info);

    for (i = 0; i < pall_target_dev_info->dev_num; i++)
    {
        /* find target devie */
        if (0 == strncmp(pmapping_dev->bdAddr, pall_target_dev_info->mapping_list[i].bdAddr,MAX_BDADDR_LEN-1))
        {
            BT_DBG_NOTICE(BT_DEBUG_COMM, "find target devie");
            /*update target device*/

            /* 1. device name */
            if (0 == strlen(pall_target_dev_info->mapping_list[i].name))
            {
                BT_DBG_NORMAL(BT_DEBUG_COMM, "existed device name is null,so update it.");
                if (0 < strlen(pmapping_dev->name) /* && (0 != strncmp(pmapping_dev->name, "no name", strlen("no name"))) */)
                {
                    strncpy(pall_target_dev_info->mapping_list[i].name, pmapping_dev->name, MAX_NAME_LEN);
                }
                else
                {
                    BT_DBG_ERROR(BT_DEBUG_COMM, "device name is null!");
                }
            }
            else
            {
                BT_DBG_NORMAL(BT_DEBUG_COMM, "existed device name is old ,so update it with the new one.");
                if (0 < strlen(pmapping_dev->name) && (0 != strncmp(pmapping_dev->name, pall_target_dev_info->mapping_list[i].name, MAX_NAME_LEN)))
                {
                    strncpy(pall_target_dev_info->mapping_list[i].name, pmapping_dev->name, MAX_NAME_LEN);
                }
                else
                {
                    BT_DBG_ERROR(BT_DEBUG_COMM, "pmapping_dev->name:%s, pall_target_dev_info->mapping_list[i].name:%s", pmapping_dev->name, pall_target_dev_info->mapping_list[i].name);
                }
            }

            /* 2. device cod */
            if (pmapping_dev->cod != pall_target_dev_info->mapping_list[i].cod)
            {
                BT_DBG_WARNING(BT_DEBUG_COMM, "the same MAC addr, but different cod value! one is 0x%02X,another is 0x%02X",(unsigned int)(pmapping_dev->cod),(unsigned int)(pall_target_dev_info->mapping_list[i].cod));
                if (((0 == pall_target_dev_info->mapping_list[i].cod || 0x1F00 == pall_target_dev_info->mapping_list[i].cod))
                      &&((0 < pmapping_dev->cod) && (0x1F00 != pmapping_dev->cod)))
                {
                    pall_target_dev_info->mapping_list[i].cod = pmapping_dev->cod;
                }
                else
                {
                    BT_DBG_ERROR(BT_DEBUG_COMM, "device cod is 0!");
                }
            }
            else
            {
                BT_DBG_WARNING(BT_DEBUG_COMM, "pmapping_dev cod:0x%02X, pall_target_dev_info->mapping_list[i].cod:0x%02X",(unsigned int)pmapping_dev->cod, (unsigned int)pall_target_dev_info->mapping_list[i].cod);
            }

            /* 3. device rssi */
            if ((pmapping_dev->rssi != pall_target_dev_info->mapping_list[i].rssi)
                    && (0 != pmapping_dev->rssi)
                    && (0 != pall_target_dev_info->mapping_list[i].rssi))
            {
                pall_target_dev_info->mapping_list[i].rssi = pmapping_dev->rssi;
            }
            else
            {
                BT_DBG_WARNING(BT_DEBUG_COMM, "pmapping_dev rssi:%ld,pall_target_dev_info->mapping_list[i] rssi:%ld",(long)pmapping_dev->rssi, (long)pall_target_dev_info->mapping_list[i].rssi);
            }
            /*4. device type */
            if ((pmapping_dev->devicetype!= pall_target_dev_info->mapping_list[i].devicetype)
                    && (0 != pmapping_dev->devicetype)
                    && (0 != pall_target_dev_info->mapping_list[i].devicetype))
            {
                pall_target_dev_info->mapping_list[i].devicetype = pmapping_dev->devicetype;
            }
            else
            {
                BT_DBG_WARNING(BT_DEBUG_COMM, "pmapping_dev device type :%ld,pall_target_dev_info->mapping_list[i] rssi:%ld",(long)pmapping_dev->devicetype, (long)pall_target_dev_info->mapping_list[i].devicetype);
            }
            BT_DBG_INFO(BT_DEBUG_COMM, "[%ld] addr:%s", (long)i, pall_target_dev_info->mapping_list[i].bdAddr);
            BT_DBG_INFO(BT_DEBUG_COMM, "[%ld] name:%s", (long)i, pall_target_dev_info->mapping_list[i].name);
            BT_DBG_INFO(BT_DEBUG_COMM, "[%ld] cod:0x%02X", (long)i, (unsigned int)pall_target_dev_info->mapping_list[i].cod);
            BT_DBG_INFO(BT_DEBUG_COMM, "[%ld] rssi:%ld", (long)i, (long)pall_target_dev_info->mapping_list[i].rssi);
            return BT_SUCCESS;
        }
    }
    /* new target devie */
    BT_DBG_NOTICE(BT_DEBUG_COMM, "new target devie");
    if (MAX_MAPPING_SCAN_NUM == pall_target_dev_info->dev_num)
    {
        /*replace the oldest one*/
        BT_DBG_NOTICE(BT_DEBUG_COMM, "target devie is full, replace the oldest one, pall_target_dev_info->head_idx: %ld", (long)pall_target_dev_info->head_idx);
        i = pall_target_dev_info->head_idx;
        memset(&pall_target_dev_info->mapping_list[i],0,sizeof(BT_DEV_MAPPING_INFO));
        pall_target_dev_info->head_idx++;
        if (MAX_MAPPING_SCAN_NUM == pall_target_dev_info->head_idx)
        {
            pall_target_dev_info->head_idx = 0;
        }
    }
    else
    {
        pall_target_dev_info->dev_num++;
        BT_DBG_NOTICE(BT_DEBUG_COMM, "have %ld target devie", (long)pall_target_dev_info->dev_num);
    }
    memcpy(pall_target_dev_info->mapping_list[i].bdAddr, pmapping_dev->bdAddr, sizeof(pmapping_dev->bdAddr));
    if (0 < strlen(pmapping_dev->name))
    {
        strncpy(pall_target_dev_info->mapping_list[i].name, pmapping_dev->name, MAX_NAME_LEN);
    }
    if ((0 == pall_target_dev_info->mapping_list[i].cod)
            &&(0 < pmapping_dev->cod))
    {
        pall_target_dev_info->mapping_list[i].cod = pmapping_dev->cod;
    }
    if ((0 == pall_target_dev_info->mapping_list[i].rssi)
            &&(0 != pmapping_dev->rssi))
    {
        pall_target_dev_info->mapping_list[i].rssi = pmapping_dev->rssi;
    }
    if ((0 == pall_target_dev_info->mapping_list[i].devicetype)
            &&(0 < pmapping_dev->devicetype))
    {
        pall_target_dev_info->mapping_list[i].devicetype= pmapping_dev->devicetype;
    }
    BT_DBG_INFO(BT_DEBUG_COMM, "[%ld] addr:%s", (long)i, pall_target_dev_info->mapping_list[i].bdAddr);
    BT_DBG_INFO(BT_DEBUG_COMM, "[%ld] name:%s", (long)i, pall_target_dev_info->mapping_list[i].name);
    BT_DBG_INFO(BT_DEBUG_COMM, "[%ld] cod:0x%02X", (long)i, (unsigned int)pall_target_dev_info->mapping_list[i].cod);
    BT_DBG_INFO(BT_DEBUG_COMM, "[%ld] rssi:%ld", (long)i, (long)pall_target_dev_info->mapping_list[i].rssi);
    BT_DBG_INFO(BT_DEBUG_COMM, "[%ld] device type :%ld", (long)i, (long)pall_target_dev_info->mapping_list[i].devicetype);

    return BT_SUCCESS;
}

INT32 bluetooth_gap_update_target_get_mapping_info(bt_bdaddr_t *bd_addr, INT32 num_properties, bt_property_t *properties)
{
    INT32 i;
    CHAR *name;
    CHAR btaddr[18];
    bt_property_t *property;
    BT_DEV_MAPPING_INFO mapping_dev;


    BT_CHECK_POINTER(BT_DEBUG_COMM, bd_addr);
    BT_CHECK_POINTER(BT_DEBUG_COMM, properties);
    memset(&mapping_dev, 0x0, sizeof(BT_DEV_MAPPING_INFO));
    for (i = 0; i < num_properties; i++)
    {
        property = &properties[i];
        /* 1 */
        if (BT_PROPERTY_BDNAME == property->type)
        {
            name = (CHAR *)property->val;
            BT_DBG_INFO(BT_DEBUG_COMM, "[GAP] type = %ld, len = %ld, bdname = %s", (long)property->type, (long)property->len, name);
            strncpy(mapping_dev.name, name, property->len);
            mapping_dev.name[property->len] = '\0';
        }
    }
    linuxbt_btaddr_htos(bd_addr, btaddr);
    memcpy(mapping_dev.bdAddr, btaddr, sizeof(mapping_dev.bdAddr));
    BT_DBG_INFO(BT_DEBUG_COMM, "bt name = %s", mapping_dev.name);
    BT_DBG_INFO(BT_DEBUG_COMM, "bt addr = %s", mapping_dev.bdAddr);
    bluetooth_gap_update_mapping_info(&mapping_dev, &all_target_dev_info);
    return BT_SUCCESS;
}

INT32 bluetooth_gap_get_mapping_info(bt_bdaddr_t *bd_addr, INT32 num_properties, bt_property_t *properties, BT_DEV_MAPPING_LIST *target_device_list)
{
    INT32 i;
    CHAR *name;
    UINT32 * cod;
    bt_bdaddr_t* btaddr;
    INT16 *rssi = NULL;
    bt_property_t *property;
    UINT32 *devicetype;
    BT_DEV_MAPPING_INFO mapping_dev;


    BT_CHECK_POINTER(BT_DEBUG_COMM, bd_addr);
    BT_CHECK_POINTER(BT_DEBUG_COMM, properties);
    memset(&mapping_dev, 0x0, sizeof(BT_DEV_MAPPING_INFO));
    for (i = 0; i < num_properties; i++)
    {
        property = &properties[i];
        if (BT_PROPERTY_BDNAME == property->type)
        {
            name = (CHAR *)property->val;
            if (strlen(name) > 0)
            {
                BT_DBG_INFO(BT_DEBUG_COMM, "[GAP] type = %ld, len = %ld, bdname = %s", (long)property->type, (long)property->len, name);
                strncpy(mapping_dev.name, name, property->len);
                mapping_dev.name[property->len] = '\0';
            }
            else
            {
                BT_DBG_NORMAL(BT_DEBUG_COMM, "device name is null");
            }
        }
        if (BT_PROPERTY_BDADDR == property->type)
        {
            btaddr = (bt_bdaddr_t *)property->val;
            linuxbt_btaddr_htos(btaddr, mapping_dev.bdAddr);
            BT_DBG_INFO(BT_DEBUG_COMM, "[GAP] type = %ld, len = %ld, bdaddr = %s", (long)property->type, (long)property->len, mapping_dev.bdAddr);
        }
        if (BT_PROPERTY_CLASS_OF_DEVICE == property->type)
        {
            cod = (UINT32 *)property->val;
            BT_DBG_INFO(BT_DEBUG_COMM, "[GAP] type = %ld, len = %ld, cod = 0x%02X", (long)property->type, (long)property->len, (unsigned int)*cod);
            mapping_dev.cod = *cod;
        }
        if (BT_PROPERTY_REMOTE_RSSI == property->type)
        {
            rssi = (INT16 *)property->val;
            BT_DBG_INFO(BT_DEBUG_COMM, "[GAP] type = %ld, len = %ld, rssi = 0x%02X", (long)property->type, (long)property->len, *rssi);
            mapping_dev.rssi = *rssi;
        }
        if (BT_PROPERTY_TYPE_OF_DEVICE == property->type)
        {
            devicetype = (UINT32 *)property->val;
            BT_DBG_INFO(BT_DEBUG_COMM, "[GAP] type = %ld, len = %ld, devtype = 0x%02X", (long)property->type, (long)property->len, (unsigned int)*devicetype);
            mapping_dev.devicetype = *devicetype;
        }
    }
    if (0 == strlen(mapping_dev.bdAddr))
    {
        BT_DBG_INFO(BT_DEBUG_COMM, "addr = %02X:%02X:%02X:%02X:%02X:%02X",
                    bd_addr->address[0], bd_addr->address[1], bd_addr->address[2],
                    bd_addr->address[3], bd_addr->address[4], bd_addr->address[5]);
        linuxbt_btaddr_htos(bd_addr, mapping_dev.bdAddr);
    }

    BT_DBG_INFO(BT_DEBUG_COMM, "bt name = %s", mapping_dev.name);
    BT_DBG_INFO(BT_DEBUG_COMM, "bt addr = %s", mapping_dev.bdAddr);
    BT_DBG_INFO(BT_DEBUG_COMM, "bt cod = 0x%02X", (unsigned int)mapping_dev.cod);
    BT_DBG_INFO(BT_DEBUG_COMM, "bt rssi = 0x%02X", (unsigned int)mapping_dev.rssi);
    BT_DBG_INFO(BT_DEBUG_COMM, "bt dev_type = 0x%02X", (unsigned int)mapping_dev.devicetype);
    bluetooth_gap_update_mapping_info(&mapping_dev, target_device_list);
    return BT_SUCCESS;
}

INT32 bluetooth_notify_device_info_app(bt_bdaddr_t *btaddr, BT_DEV_MAPPING_LIST *pall_target_dev_info)
{
    FUNC_ENTRY;

    INT32 i = 0;
    CHAR current_addr[MAX_BDADDR_LEN];

    BT_GAP_SCAN_RESULT result_info;
    BT_CHECK_POINTER(BT_DEBUG_COMM, pall_target_dev_info);

    if (NULL == AppInquiryCbk)
    {
        BT_DBG_ERROR(BT_DEBUG_COMM, "AppInquiryCbk is null");
        return BT_SUCCESS;
    }

    linuxbt_btaddr_htos(btaddr, current_addr);
    for (i = 0; i < pall_target_dev_info->dev_num; i++)
    {
        if (strncmp(current_addr, pall_target_dev_info->mapping_list[i].bdAddr, strlen(pall_target_dev_info->mapping_list[i].bdAddr)))
        {  /* current_addr is not pall_target_dev_info->mapping_list[i].bdAddr */
            continue;
        }
        if (bluetooth_scan_device_info_ready(pall_target_dev_info->mapping_list[i]))
        {
            pall_target_dev_info->mapping_list[i].ready = TRUE;

            if (NULL != AppInquiryCbk)
            {
                if (((pall_target_dev_info->mapping_list[i].cod&0x2508) == 0x2508))
                {
                    /*not notify APP*/
                    BT_DBG_ERROR(BT_DEBUG_COMM, "Inquiry is stop. For DS4, it should not notify to APP; Or else, it will connect automatically");
                }
                else
                {
                    /*with the default scan mode, if scan the non-a2dp sink device*/
                    if ((BT_INQUIRY_FILTER_TYPE_A2DP_SNK == fg_bt_inquiry_type)
                            &&(!(((pall_target_dev_info->mapping_list[i].cod&0x00240400) == 0x00240400))
                               &&!(((pall_target_dev_info->mapping_list[i].cod&0x00200400) == 0x00200400) && !((pall_target_dev_info->mapping_list[i].cod&0x00080000) == 0x00080000))))
                    {
                        BT_DBG_ERROR(BT_DEBUG_COMM, "In default a2dp scan mode, non-a2dp sink device should not be notify to APP");
                        return BT_ERR_STATUS_FAIL;
                    }
                    if (FALSE == pall_target_dev_info->mapping_list[i].is_notify)
                    {
                        memset(&result_info,0,sizeof(BT_GAP_SCAN_RESULT));
                        memcpy(result_info.bdAddr,pall_target_dev_info->mapping_list[i].bdAddr,MAX_BDADDR_LEN);
                        result_info.cod = pall_target_dev_info->mapping_list[i].cod;
                        result_info.rssi = pall_target_dev_info->mapping_list[i].rssi;
                        result_info.devicetype = pall_target_dev_info->mapping_list[i].devicetype;
                        memcpy(result_info.name,pall_target_dev_info->mapping_list[i].name,MAX_NAME_LEN);
                        // When scan BLE device, cod=0x1f00 means unclassified, BTM_COD_MAJOR_UNCLASSIFIED(btm_api.h)
                        if ((0 < strlen(result_info.name)) && (0 < strlen(result_info.bdAddr)) &&
                            ((0 != result_info.cod) && (0x1F00 != result_info.cod)))
                        {
                            if (notify_target_dev_info.dev_num <= MAX_MAPPING_SCAN_NUM)
                            {
                                AppInquiryCbk(&result_info);
                                pall_target_dev_info->mapping_list[i].is_notify = TRUE;
                                bluetooth_gap_update_mapping_info(&pall_target_dev_info->mapping_list[i], &notify_target_dev_info);
                            }
                            else
                            {
                                BT_DBG_ERROR(BT_DEBUG_COMM, "Notify Table full.");
                            }
                        }
                        else
                        {
                            BT_DBG_WARNING(BT_DEBUG_COMM, "(Need get more information)name:%s,addr:%s,cod:0x%02X",result_info.name,result_info.bdAddr,(unsigned int)result_info.cod);
                        }
                    }
                    else
                    {
                        BT_DBG_ERROR(BT_DEBUG_COMM, "Have notified to APP layer");
                    }
                }
            }
            else
            {
                BT_DBG_ERROR(BT_DEBUG_COMM, "AppInquiryCbk is null");
            }
        }
        else
        {
            BT_DBG_ERROR(BT_DEBUG_COMM, "Need get more information");
        }
        break;
    }
    return BT_SUCCESS;
}

INT32 bluetooth_notify_every_device_info_app(bt_bdaddr_t *btaddr, BT_DEV_MAPPING_LIST *pall_target_dev_info)
{
    FUNC_ENTRY;

    INT32 i = 0;
    CHAR current_addr[MAX_BDADDR_LEN];

    BT_GAP_SCAN_RESULT result_info;
    BT_CHECK_POINTER(BT_DEBUG_COMM, pall_target_dev_info);

    linuxbt_btaddr_htos(btaddr, current_addr);
    for (i = 0; i < pall_target_dev_info->dev_num; i++)
    {
        if (strncmp(current_addr, pall_target_dev_info->mapping_list[i].bdAddr, strlen(pall_target_dev_info->mapping_list[i].bdAddr)))
        {
            continue;
        }

        BT_DBG_INFO(BT_DEBUG_COMM, "bdAddr=%s,bdAddr_len=%d,name=%s,name_len=%d.\n",
            pall_target_dev_info->mapping_list[i].bdAddr,
            strlen(pall_target_dev_info->mapping_list[i].bdAddr),
            pall_target_dev_info->mapping_list[i].name,
            strlen(pall_target_dev_info->mapping_list[i].name));

        if ((0 < strlen(pall_target_dev_info->mapping_list[i].bdAddr)) &&( 0 < strlen(pall_target_dev_info->mapping_list[i].name)))
        {
            pall_target_dev_info->mapping_list[i].ready = TRUE;

            if (NULL != AppInquiryCbk)
            {
                if (FALSE == pall_target_dev_info->mapping_list[i].is_notify)
                {
                    memset(&result_info,0,sizeof(BT_GAP_SCAN_RESULT));
                    memcpy(result_info.bdAddr,pall_target_dev_info->mapping_list[i].bdAddr,MAX_BDADDR_LEN);
                    result_info.cod = pall_target_dev_info->mapping_list[i].cod;
                    result_info.rssi = pall_target_dev_info->mapping_list[i].rssi;
                    memcpy(result_info.name,pall_target_dev_info->mapping_list[i].name,MAX_NAME_LEN);

                    BT_DBG_NORMAL(BT_DEBUG_COMM, "Notify to APP layer.\n");
                    AppInquiryCbk(&result_info);
                    pall_target_dev_info->mapping_list[i].is_notify = TRUE;
                }
                else
                {
                    BT_DBG_ERROR(BT_DEBUG_COMM, "Have notified to APP layer.\n");
                }
            }
            else
            {
                BT_DBG_ERROR(BT_DEBUG_COMM, "AppInquiryCbk is null.\n");
            }
        }
        else
        {
            BT_DBG_ERROR(BT_DEBUG_COMM, "bdAddr or name is null.\n");
        }
        break;
    }
    return BT_SUCCESS;
}

INT32 print_history_info(BT_TARGET_DEV_LIST * pt_device_history)
{
    FUNC_ENTRY;
    UINT32 i = 0;
    UINT32 j = 0;/* j is for count device num */
    BT_CHECK_POINTER(BT_DEBUG_COMM, pt_device_history);
    i = pt_device_history->head_idx;
    BT_DBG_INFO(BT_DEBUG_COMM, "head_idx:%ld", (long)i);
    BT_DBG_INFO(BT_DEBUG_COMM, "dev num:%ld", (long)pt_device_history->dev_num);
    while (j < pt_device_history->dev_num)
    {
        BT_DBG_NORMAL(BT_DEBUG_COMM, "device_list[%ld].bdAddr:%s, name:%s, cod:%lx",
                      (long)i,
                      pt_device_history->device_list[i].bdAddr,
                      pt_device_history->device_list[i].name,
                      (long)pt_device_history->device_list[i].cod
                     );
        i = (i + 1) % MAX_DEV_NUM;
        j++;
    }
    return BT_SUCCESS;
}

INT32 print_hid_history_info(BT_HID_TARGET_DEV_LIST *pt_device_history)
{
    FUNC_ENTRY;
    UINT32 i = 0;
    UINT32 j = 0;/* j is for count device num */
    BT_CHECK_POINTER(BT_DEBUG_COMM, pt_device_history);
    i = pt_device_history->head_idx;
    BT_DBG_INFO(BT_DEBUG_COMM, "head_idx:%ld", (long)i);
    BT_DBG_INFO(BT_DEBUG_COMM, "dev_num:%ld", (long)pt_device_history->dev_num);
    while (j < pt_device_history->dev_num)
    {
        BT_DBG_INFO(BT_DEBUG_COMM, "device_list[%ld].bdAddr:%s device_list[%ld].name:%s device_list[%ld].cod:0x%02lX",
                        (long)i,
                        pt_device_history->device_list[i].bdAddr,
                        (long)i,
                        pt_device_history->device_list[i].name,
                        (long)i,
                        (long)pt_device_history->device_list[i].cod
                       );
        i = (i + 1) % MAX_HID_DEV_NUM;
        j++;
    }
    return BT_SUCCESS;
}

INT32 print_hid_status_info(BT_HID_STATUS_STRUCT_LIST *pt_device_history)
{
    FUNC_ENTRY;
    UINT32 i = 0;
    UINT32 j = 0;/* j is for count device num */
    BT_CHECK_POINTER(BT_DEBUG_COMM, pt_device_history);
    i = pt_device_history->head_idx;
    BT_DBG_INFO(BT_DEBUG_COMM, "head_idx:%ld", (long)i);
    BT_DBG_INFO(BT_DEBUG_COMM, "dev_num:%ld", (long)pt_device_history->dev_num);
    while (j < pt_device_history->dev_num)
    {
        BT_DBG_INFO(BT_DEBUG_COMM, "device_list[%ld].bdaddr:%s device_list[%ld].status:%d.",
                        (long)i,
                        pt_device_history->device_list[i].bdAddr,
                        (long)i,
                        pt_device_history->device_list[i].status
                       );
        i = (i + 1) % MAX_HID_DEV_NUM;
        j++;
    }
    return BT_SUCCESS;
}

INT32 bluetooth_get_device_info(BT_TARGET_DEV_LIST *dev_list_info)
{
    FUNC_ENTRY;
    if (g_paired_dev_info.dev_num > 0)
    {
        memcpy(dev_list_info,
               &g_paired_dev_info,
               sizeof(BT_TARGET_DEV_LIST)
              );
        return BT_SUCCESS;
    }
    BT_DBG_ERROR(BT_DEBUG_COMM, "NO paired device!");
    return BT_ERR_STATUS_FAIL;
}

INT32 bluetooth_get_device_history(BT_TARGET_DEV_LIST *dev_history, BT_TARGET_DEV_LIST *pt_paired_dev_history)
{
    FUNC_ENTRY;

    if (!pt_paired_dev_history || !dev_history)
    {
        BT_DBG_ERROR(BT_DEBUG_COMM, "The pointer is null");
        return BT_ERR_STATUS_FAIL;
    }

    if (pt_paired_dev_history->dev_num > 0)
    {
        memcpy(dev_history,
               pt_paired_dev_history,
               sizeof(BT_TARGET_DEV_LIST)
              );
        return BT_SUCCESS;
    }
    BT_DBG_ERROR(BT_DEBUG_COMM, "NO paired device!");
    return BT_ERR_STATUS_FAIL;
}

INT32 bluetooth_get_hid_device_history(BT_HID_TARGET_DEV_LIST *dev_history, BT_HID_TARGET_DEV_LIST *pt_paired_dev_history)
{
    FUNC_ENTRY;

    if (!pt_paired_dev_history || !dev_history)
    {
        BT_DBG_ERROR(BT_DEBUG_COMM, "The pointer is null");
        return BT_ERR_STATUS_FAIL;
    }
    if (NULL == pt_paired_dev_history)
    {
        BT_DBG_ERROR(BT_DEBUG_COMM, "input pt_paired_dev_history NULL!");
        pt_paired_dev_history = &g_paired_hid_dev_history;

        if (NULL == pt_paired_dev_history)
        {
            BT_DBG_ERROR(BT_DEBUG_COMM, "NULL pointer!");
            return BT_ERR_STATUS_FAIL;
        }
    }

    if (pt_paired_dev_history->dev_num > 0)
    {
        memcpy(dev_history,
               pt_paired_dev_history,
               sizeof(BT_HID_TARGET_DEV_LIST)
              );

        print_hid_history_info(dev_history);
        return BT_SUCCESS;
    }
    BT_DBG_ERROR(BT_DEBUG_COMM, "NO paired device!");
    return BT_ERR_STATUS_FAIL;
}

INT32 bluetooth_get_num_of_history(UINT32 *u4_dev_num, BT_TARGET_DEV_LIST *pt_paired_dev_history)
{
    FUNC_ENTRY;
    INT32 i4_ret = BT_SUCCESS;

    if (NULL == u4_dev_num)
    {
        BT_DBG_ERROR(BT_DEBUG_COMM, "null pointer of u4_dev_num");
        return BT_ERR_STATUS_FAIL;
    }
    if (g_fghistorylist_init)
    {
        *u4_dev_num = pt_paired_dev_history->dev_num;
        BT_DBG_NORMAL(BT_DEBUG_COMM,  "paired device number:%ld", (long)*u4_dev_num);
        i4_ret = BT_SUCCESS;
    }
    else
    {
        BT_DBG_ERROR(BT_DEBUG_COMM, "paired device list does not init");
        i4_ret = BT_ERR_STATUS_FAIL;
    }

    return i4_ret;
}

VOID bluetooth_clear_device_info(VOID)
{
    FUNC_ENTRY;
    memset(&g_paired_dev_info, 0, sizeof(BT_TARGET_DEV_LIST));
}

INT32 bluetooth_ReadHistoryFromFile(CHAR *filename, BT_TARGET_DEV_LIST *pt_paired_dev_history)
{
    FUNC_ENTRY;

    FILE *fp_local_dev_info = NULL;


    if (NULL == filename)
    {
        BT_DBG_ERROR(BT_DEBUG_COMM, "filename is null in lines:%ld @ %s!", (long)__LINE__, __FUNCTION__);
        return BT_ERR_STATUS_FAIL;
    }
    if (NULL == pt_paired_dev_history)
    {
        BT_DBG_ERROR(BT_DEBUG_COMM, "pt_paired_dev_history is null in lines:%ld @ %s!", (long)__LINE__, __FUNCTION__);
        return BT_ERR_STATUS_FAIL;
    }
    if (0 != access(filename, F_OK))
    {
        BT_DBG_ERROR(BT_DEBUG_COMM, "file:%s not exist!", filename);
        //pt_paired_dev_history->head_idx = 0;
        memset(pt_paired_dev_history, 0, sizeof(BT_TARGET_DEV_LIST));
        return BT_SUCCESS;
    }
    fp_local_dev_info = fopen(filename, "r");

    if (fp_local_dev_info == NULL)
    {
        BT_DBG_ERROR(BT_DEBUG_COMM, "Failed to Read %s:%ld", filename, (long)errno);
        remove(filename);
        return BT_ERR_STATUS_FAIL;
    }
    /*read history from txt file*/
    fread(pt_paired_dev_history, sizeof(BT_TARGET_DEV_LIST), 1, fp_local_dev_info);
    print_history_info(pt_paired_dev_history);
    fclose(fp_local_dev_info);

    return BT_SUCCESS;
}


BT_TARGET_DEV_LIST* bluetooth_getHistoryList(BOOL fg_src_mode)
{
    FUNC_ENTRY;
    INT32 i4_ret = BT_ERR_STATUS_FAIL;

    if (!fg_src_mode)
    {
        i4_ret = bluetooth_ReadHistoryFromFile(BLUETOOTH_SRC_HISTORY_FILE, g_pt_paired_src_dev_history);
        if (BT_SUCCESS != i4_ret)
        {
            BT_DBG_ERROR(BT_DEBUG_COMM, "read history from src file is failed.");
            return NULL;
        }
        BT_DBG_NORMAL(BT_DEBUG_COMM, "read history from src file success.");
        g_fghistorylist_init = TRUE;
        return g_pt_paired_src_dev_history;
    }
    else
    {
#if ENABLE_A2DP_SRC
        i4_ret = bluetooth_ReadHistoryFromFile(BLUETOOTH_SINK_HISTORY_FILE, g_pt_paired_sink_dev_history);
        if (BT_SUCCESS != i4_ret)
        {
            BT_DBG_ERROR(BT_DEBUG_COMM, "read history from sink file is failed.");
            return NULL;
        }
        BT_DBG_NORMAL(BT_DEBUG_COMM, "read history from sink file success.");
        g_fghistorylist_init = TRUE;
        return g_pt_paired_sink_dev_history;
#endif
    }

    BT_DBG_ERROR(BT_DEBUG_COMM, "should not go here.");
    return NULL;

}

BT_HID_TARGET_DEV_LIST* bluetooth_getHIDHistoryList(VOID)
{
    FUNC_ENTRY;
#if ENABLE_HID_PROFILE_H
    INT32 i4_ret = BT_SUCCESS;

    i4_ret = bluetooth_hid_ReadHistoryFromFile(BLUETOOTH_HID_HISTORY_FILE, g_pt_paired_hid_dev_history);
    if (BT_SUCCESS != i4_ret)
    {
        BT_DBG_ERROR(BT_DEBUG_HID, "read history from hid file is failed.");
        return NULL;
    }

    memset(g_pt_hid_status_list, 0, sizeof(BT_HID_STATUS_STRUCT_LIST));

    if (NULL != g_pt_paired_hid_dev_history)
    {
        UINT32 i = 0;
        UINT32 j = 0;/* j is for count device num */

        i = g_pt_paired_hid_dev_history->head_idx;
        BT_DBG_INFO(BT_DEBUG_COMM, "head_idx:%d", i);
        g_pt_hid_status_list->head_idx = i;
        g_pt_hid_status_list->dev_num = g_pt_paired_hid_dev_history->dev_num;
        BT_DBG_INFO(BT_DEBUG_COMM, "dev_num:%d", g_pt_hid_status_list->dev_num);

        while (j < g_pt_paired_hid_dev_history->dev_num)
        {
            BT_DBG_INFO(BT_DEBUG_COMM, "device_list[%ld].bdAddr:%s device_list[%ld].name:%s device_list[%ld].cod:0x%02lX",
                            (long)i,
                            g_pt_paired_hid_dev_history->device_list[i].bdAddr,
                            (long)i,
                            g_pt_paired_hid_dev_history->device_list[i].name,
                            (long)i,
                            (long)g_pt_paired_hid_dev_history->device_list[i].cod
                           );
            memcpy(g_pt_hid_status_list->device_list[i].bdAddr,
                g_pt_paired_hid_dev_history->device_list[i].bdAddr,
                MAX_BDADDR_LEN);

            // when platform boots, all device status shall be paired.
            g_pt_hid_status_list->device_list[i].status = BT_PAIRED_STATUS;
            i = (i + 1) % MAX_HID_DEV_NUM;
            j++;
        }

        print_hid_status_info(g_pt_hid_status_list);
    }

    BT_DBG_NORMAL(BT_DEBUG_HID, "read history from hid file success.");
    g_fghistorylist_init = TRUE;
    return g_pt_paired_hid_dev_history;

#endif
    return NULL;

}

INT32 bluetooth_WriteHistoryToFile(BT_TARGET_DEV_LIST *pt_device_history, CHAR *pfilename)
{
    FUNC_ENTRY;
    BT_CHECK_POINTER(BT_DEBUG_COMM, pt_device_history);
    INT32 i4_fd = -1;



    //BT_CHECK_POINTER(BT_DEBUG_COMM, pt_device_history);
    if (!g_fghistorylist_init)
    {
        BT_DBG_INFO(BT_DEBUG_COMM, "device list does not init, so no need to save it");
        return BT_SUCCESS;
    }
    if (NULL == pfilename)
    {
        BT_DBG_ERROR(BT_DEBUG_COMM, "null pointer of pfilename");
        return BT_ERR_STATUS_FAIL;
    }
    if (0 != access(BLUETOOTH_MW_LOCAL_FOLDER, F_OK))
    {
        BT_DBG_ERROR(BT_DEBUG_COMM, "should create:%s", BLUETOOTH_MW_LOCAL_FOLDER);
        if (mkdir(BLUETOOTH_MW_LOCAL_FOLDER, 0777) == 0)
        {
            BT_DBG_INFO(BT_DEBUG_COMM, "create:%s success", BLUETOOTH_MW_LOCAL_FOLDER);
        }
        else
        {
            BT_DBG_ERROR(BT_DEBUG_COMM, "create:%s fail:%ld", BLUETOOTH_MW_LOCAL_FOLDER, (long)errno);
        }
        return BT_ERR_STATUS_FAIL;
    }
    i4_fd = open(pfilename, O_RDWR|O_CREAT|O_SYNC, 0666);
    if (-1 == i4_fd)
    {
        BT_DBG_ERROR(BT_DEBUG_COMM, "Fail to open file:%ld", (long)errno);
        return BT_ERR_STATUS_FAIL;
    }
    //remove all paired device info
    if (NULL != pt_device_history && 0 == pt_device_history->dev_num)
    {
        BT_DBG_NORMAL(BT_DEBUG_COMM, "clear device history info");

        write(i4_fd, pt_device_history, sizeof(BT_TARGET_DEV_LIST));
        close(i4_fd);
        sync();

        return BT_SUCCESS;
    }

    print_history_info(pt_device_history);
    /*save the txt file*/
    BT_DBG_NORMAL(BT_DEBUG_COMM, "save device list to %s", pfilename);
    write(i4_fd, pt_device_history, sizeof(BT_TARGET_DEV_LIST));
    close(i4_fd);
    sync();
    FUNC_EXIT;
    return BT_SUCCESS;
}

INT32 bluetooth_hid_ReadHistoryFromFile(CHAR *filename, BT_HID_TARGET_DEV_LIST *pt_paired_dev_history)
{
    FUNC_ENTRY;

    FILE *fp_local_dev_info = NULL;


    if (NULL == filename)
    {
        BT_DBG_ERROR(BT_DEBUG_COMM, "filename is null in lines:%ld @ %s!", (long)__LINE__, __FUNCTION__);
        return BT_ERR_STATUS_FAIL;
    }
    if (NULL == pt_paired_dev_history)
    {
        BT_DBG_ERROR(BT_DEBUG_COMM, "pt_paired_dev_history is null in lines:%ld @ %s!", (long)__LINE__, __FUNCTION__);
        return BT_ERR_STATUS_FAIL;
    }
    if (0 != access(filename, F_OK))
    {
        BT_DBG_ERROR(BT_DEBUG_COMM, "file:%s not exist!", filename);
        //pt_paired_dev_history->head_idx = 0;
        memset(pt_paired_dev_history, 0, sizeof(BT_HID_TARGET_DEV_LIST));
        return BT_SUCCESS;
    }
    fp_local_dev_info = fopen(filename, "r");

    if (fp_local_dev_info == NULL)
    {
        BT_DBG_ERROR(BT_DEBUG_COMM, "Failed to Read %s:%ld", filename, (long)errno);
        remove(filename);
        return BT_ERR_STATUS_FAIL;
    }
    /*read history from txt file*/
    fread(pt_paired_dev_history, sizeof(BT_HID_TARGET_DEV_LIST), 1, fp_local_dev_info);
    print_hid_history_info(pt_paired_dev_history);
    fclose(fp_local_dev_info);

    return BT_SUCCESS;
}

INT32 bluetooth_hid_WriteHistoryToFile(BT_HID_TARGET_DEV_LIST *pt_device_history, CHAR *pfilename)
{
    FUNC_ENTRY;
    BT_CHECK_POINTER(BT_DEBUG_COMM, pt_device_history);
    INT32 i4_fd = -1;

    //BT_CHECK_POINTER(BT_DEBUG_COMM, pt_device_history);
    if (!g_fghistorylist_init)
    {
        BT_DBG_INFO(BT_DEBUG_COMM, "device list does not init, so no need to save it");
        return BT_SUCCESS;
    }
    if (NULL == pfilename)
    {
        BT_DBG_ERROR(BT_DEBUG_COMM, "null pointer of pfilename");
        return BT_ERR_STATUS_FAIL;
    }
    if (0 != access(BLUETOOTH_MW_LOCAL_FOLDER, F_OK))
    {
        BT_DBG_ERROR(BT_DEBUG_COMM, "should create:%s", BLUETOOTH_MW_LOCAL_FOLDER);
        if (mkdir(BLUETOOTH_MW_LOCAL_FOLDER, 0777) == 0)
        {
            BT_DBG_INFO(BT_DEBUG_COMM, "create:%s success", BLUETOOTH_MW_LOCAL_FOLDER);
        }
        else
        {
            BT_DBG_ERROR(BT_DEBUG_COMM, "create:%s fail:%ld", BLUETOOTH_MW_LOCAL_FOLDER, (long)errno);
        }
        return BT_ERR_STATUS_FAIL;
    }
    i4_fd = open(pfilename, O_RDWR|O_CREAT|O_SYNC, 0666);
    if (-1 == i4_fd)
    {
        BT_DBG_ERROR(BT_DEBUG_COMM, "Fail to open file:%ld", (long)errno);
        return BT_ERR_STATUS_FAIL;
    }
    //remove all paired device info
    if (NULL != pt_device_history && 0 == pt_device_history->dev_num)
    {
        BT_DBG_NORMAL(BT_DEBUG_COMM, "clear device history info");

        write(i4_fd, pt_device_history, sizeof(BT_HID_TARGET_DEV_LIST));
        close(i4_fd);
        sync();

        return BT_SUCCESS;
    }

    print_hid_history_info(pt_device_history);
    /*save the txt file*/
    BT_DBG_NORMAL(BT_DEBUG_COMM, "save device list to %s", pfilename);
    write(i4_fd, pt_device_history, sizeof(BT_HID_TARGET_DEV_LIST));
    close(i4_fd);
    sync();
    FUNC_EXIT;
    return BT_SUCCESS;
}

VOID bluetooth_setHistory(BT_TARGET_DEV_INFO *history, BT_TARGET_DEV_LIST *pt_paired_dev_history)
{
    FUNC_ENTRY;
    UINT32 i;
    UINT32 j;
    UINT32 k;

    BT_TARGET_DEV_INFO temp_dev = {0};

    if (NULL == history)
    {
        BT_DBG_ERROR(BT_DEBUG_COMM, "history is null pointer");
        return ;
    }
    if (NULL == pt_paired_dev_history)
    {
        BT_DBG_ERROR(BT_DEBUG_COMM, "pt_paired_dev_history is null pointer");
        return ;
    }

    if (pt_paired_dev_history->head_idx > (MAX_DEV_NUM-1))
    {
        BT_DBG_ERROR(BT_DEBUG_COMM, "recover head index:%ld to 0", (long)pt_paired_dev_history->head_idx);
        pt_paired_dev_history->head_idx = 0;

    }
    i = pt_paired_dev_history->head_idx;
    BT_DBG_NORMAL(BT_DEBUG_COMM, " history address:%s name:%s", history->bdAddr, history->name);
    /*when history items equal to MAX_DEV_NUM*/
    if (MAX_DEV_NUM == pt_paired_dev_history->dev_num)
    {
        BT_DBG_NORMAL(BT_DEBUG_COMM, "have MAX_DEV_NUM devices");
        /*find duplicated item*/
        j = 0;
        while (j < MAX_DEV_NUM)
        {
            /*have duplicated item*/
            if (0 == strncmp(history->bdAddr, pt_paired_dev_history->device_list[i].bdAddr, 17))
            {
                /*the duplicated item is the newest one, no need move*/
                if (i == (pt_paired_dev_history->head_idx - 1 + MAX_DEV_NUM)%MAX_DEV_NUM)
                {
                    BT_DBG_NORMAL(BT_DEBUG_COMM, "the item in the list and is the newest one");
                    /*in case of the same mac address not the same name
                                    but if the device name is no name should not update to it*/
                    if (0<strlen(history->name)/* && (0 != (strncmp(history->name,"no name",strlen("no name"))))*/)
                    {
                        memset(pt_paired_dev_history->device_list[i].name,
                               0,
                               sizeof(pt_paired_dev_history->device_list[i].name));
                        strncpy(pt_paired_dev_history->device_list[i].name,
                                history->name,
                                strlen(history->name)
                               );
                    }
                }
                else
                {
                    /* for MSA4G (support TV connection (SPP connection)).
                       just  change the sequcence, and don't replace the duplicated one  with the "history" (parameter 1).
                       this is aimed at covering the possiable case: when SONY TV trigger AVRCP prior to A2DP.
                    */
                    /* in this time i is the duplicated one */
                    k = i;
                    memcpy(&temp_dev,
                           &pt_paired_dev_history->device_list[i],
                           sizeof(BT_TARGET_DEV_INFO)
                          );
                    /*change the sequcence:  every item need move one step backforword*/
                    while ((k != (pt_paired_dev_history->head_idx - 1 + MAX_DEV_NUM) % MAX_DEV_NUM)
                            && (0 < strlen(pt_paired_dev_history->device_list[k].bdAddr))
                          )
                    {
                        memcpy(&pt_paired_dev_history->device_list[k],
                               &pt_paired_dev_history->device_list[(k + 1) % MAX_DEV_NUM],
                               sizeof(BT_TARGET_DEV_INFO)
                              );
                        k = (k + 1) % MAX_DEV_NUM;

                    }

                    memcpy(&pt_paired_dev_history->device_list[k],
                           &temp_dev,
                           sizeof(BT_TARGET_DEV_INFO)
                          );
                }
                /*complete handle duplicated item*/
                goto save_file;
                //return;
            }
            else
            {
                i = (i + 1) % MAX_DEV_NUM;
            }
            j++;
        }
        /*no duplicated item, replace the old one with the newest one and modify the index*/
        bluetooth_remove_paired_dev(pt_paired_dev_history->device_list[pt_paired_dev_history->head_idx].bdAddr);
        memset(&pt_paired_dev_history->device_list[pt_paired_dev_history->head_idx],
               0,
               sizeof(BT_TARGET_DEV_INFO));
        memcpy(&pt_paired_dev_history->device_list[pt_paired_dev_history->head_idx],
               history,
               sizeof(BT_TARGET_DEV_INFO)
              );
        pt_paired_dev_history->head_idx = (pt_paired_dev_history->head_idx + 1) % MAX_DEV_NUM;

    }
    /*the number of history item in list less than MAX_DEV_NUM*/
    else
    {
        BT_DBG_NORMAL(BT_DEBUG_COMM, "have %ld devices", (long)pt_paired_dev_history->dev_num);
        /*find duplicated item*/
        /*  j just for count device num*/
        j = 0;
        while (j < pt_paired_dev_history->dev_num)
        {
            /*have duplicated item*/
            if (0 == strncmp(history->bdAddr, pt_paired_dev_history->device_list[i].bdAddr, 17))
            {
                /*the duplicated item is the newest one, no need move*/
                if (0 == strlen(pt_paired_dev_history->device_list[(i+1)%MAX_DEV_NUM].bdAddr))
                {
                    BT_DBG_NORMAL(BT_DEBUG_COMM, "the item in the list and is the newest one");
                    /*in case of the same mac address not the same name
                                    but if the device name is no name should not update to it*/
                    if (0<strlen(history->name)/* && (0 != (strncmp(history->name,"no name",strlen("no name"))))*/)
                    {
                        memset(pt_paired_dev_history->device_list[i].name,
                               0,
                               sizeof(pt_paired_dev_history->device_list[i].name));
                        strncpy(pt_paired_dev_history->device_list[i].name,
                                history->name,
                                strlen(history->name)
                               );
                    }
                }
                else
                {
                    /* for MSA4G (support TV connection (SPP connection) ).
                       just  change the sequcence, and don't replace the duplicated one  with the "history" (parameter 1).
                       this is aimed at covering the possiable case: when SONY TV trigger AVRCP prior to A2DP.
                    */
                    /* in this time i is the duplicated one */
                    k = i;
                    memcpy(&temp_dev,
                           &pt_paired_dev_history->device_list[i],
                           sizeof(BT_TARGET_DEV_INFO)
                          );
                    /*change the sequcence:  every item need move one step backforword*/
                    while ((k != (pt_paired_dev_history->head_idx+pt_paired_dev_history->dev_num-1)%MAX_DEV_NUM)
                            && (0 < strlen(pt_paired_dev_history->device_list[k].bdAddr))
                          )
                    {
                        memcpy(&pt_paired_dev_history->device_list[k],
                               &pt_paired_dev_history->device_list[(k + 1) % MAX_DEV_NUM],
                               sizeof(BT_TARGET_DEV_INFO)
                              );
                        k = (k + 1) % MAX_DEV_NUM;

                    }

                    memcpy(&pt_paired_dev_history->device_list[k],
                           &temp_dev,
                           sizeof(BT_TARGET_DEV_INFO)
                          );
                }
                /*complete handle duplicated item*/
                goto save_file;
                //return;
            }
            else
            {
                i = (i + 1) % MAX_DEV_NUM;
            }
            j++;
        }
        /*no duplicated item, add new item to last of history list*/
        i = pt_paired_dev_history->head_idx;
        /* this time j is for the last pos in dev list */
        j = (i + pt_paired_dev_history->dev_num) % MAX_DEV_NUM;
        BT_DBG_NORMAL(BT_DEBUG_COMM, "head_idx:%ld, dev_num:%ld", (long)i, (long)j);
        BT_DBG_NORMAL(BT_DEBUG_COMM, "new item, add to the last position:%ld", (long)j);
        memset(&pt_paired_dev_history->device_list[j],
               0,
               sizeof(pt_paired_dev_history->device_list[j]));
        memcpy(&pt_paired_dev_history->device_list[j],
               history,
               sizeof(BT_TARGET_DEV_INFO)
              );
        pt_paired_dev_history->dev_num++;
    }
save_file:
    //print the paired device list
    print_history_info(pt_paired_dev_history);

    /*save the history list to /tmp*/
    INT32 i4_ret;
    if (!get_bt_a2dp_be_sink())
    {
        i4_ret = bluetooth_WriteHistoryToFile(pt_paired_dev_history, BLUETOOTH_SINK_HISTORY_FILE);
        if ( i4_ret != 0 )
        {
            BT_DBG_NORMAL(BT_DEBUG_COMM, "save sink device history failed. " );
            return;
        }
    }
    else
    {
        i4_ret = bluetooth_WriteHistoryToFile(pt_paired_dev_history, BLUETOOTH_SRC_HISTORY_FILE);
        if ( i4_ret != 0 )
        {
            BT_DBG_NORMAL(BT_DEBUG_COMM, "save src device history failed. " );
            return;
        }
    }
}


INT32 bluetooth_remove_av_history_one(CHAR* keyword, BT_TARGET_DEV_LIST *pt_paired_dev_history)
{
    FUNC_ENTRY;
    UINT32 i;
    UINT32 j = 0;
    //CHAR *ptemp = NULL;
    if (NULL == pt_paired_dev_history || NULL == keyword)
    {
        BT_DBG_ERROR(BT_DEBUG_COMM, "pt_paired_dev_history is null pointer");
        return BT_ERR_STATUS_FAIL;
    }
    if (pt_paired_dev_history->head_idx > (MAX_DEV_NUM-1))
    {
        return BT_ERR_STATUS_FAIL;
    }
    else
    {
        i = pt_paired_dev_history->head_idx;
    }
    while ((i != (pt_paired_dev_history->head_idx - 1 + MAX_DEV_NUM) % MAX_DEV_NUM)
            && (0 < strlen(pt_paired_dev_history->device_list[i].bdAddr)))
    {
        //find the target history item
        BT_DBG_NORMAL(BT_DEBUG_COMM, "device_list[%ld].bdAddr is:%s", (long)i, pt_paired_dev_history->device_list[i].bdAddr);
        if (0 == strncmp(keyword, pt_paired_dev_history->device_list[i].bdAddr, 17))
        {
            BT_DBG_NORMAL(BT_DEBUG_COMM, "success to find the target item is history[%ld]:%s!", (long)i, keyword);
            memset(&pt_paired_dev_history->device_list[i], 0, sizeof(BT_TARGET_DEV_INFO));

            BT_DBG_NORMAL(BT_DEBUG_COMM, "success to delete the target item -- history[%ld]:%s!", (long)i, keyword);
            //record forward one item
            for (j=i; j!= (pt_paired_dev_history->head_idx - 1 + MAX_DEV_NUM) % MAX_DEV_NUM;)
            {
                //if next item is null stop move
                if (0 == strlen(pt_paired_dev_history->device_list[(j + 1) % MAX_DEV_NUM].bdAddr))
                {
                    memset(&pt_paired_dev_history->device_list[j], 0, sizeof(BT_TARGET_DEV_INFO));
                    break ;
                }

                memcpy(&pt_paired_dev_history->device_list[j],
                       &pt_paired_dev_history->device_list[(j + 1) % MAX_DEV_NUM],
                       sizeof(BT_TARGET_DEV_INFO)
                      );
                j = (j + 1) % MAX_DEV_NUM;
            }
            //the last item should be null
            memset(&pt_paired_dev_history->device_list[j], 0, sizeof(BT_TARGET_DEV_INFO));
            //pt_paired_dev_history->szhistory[j] = NULL;
            pt_paired_dev_history->dev_num--;
            BT_DBG_NORMAL(BT_DEBUG_COMM, "success to move the historylist!");
            //save the history to FLASH
#if 1
            INT32 i4_ret = BT_ERR_STATUS_FAIL;
            if (!get_bt_a2dp_be_sink())
            {
                i4_ret = bluetooth_WriteHistoryToFile(pt_paired_dev_history, BLUETOOTH_SINK_HISTORY_FILE);
                if ( i4_ret != BT_SUCCESS )
                {
                    BT_DBG_NORMAL(BT_DEBUG_COMM, "save sink device history failed. " );
                    return BT_ERR_STATUS_FAIL;
                }
            }
            else
            {
                i4_ret = bluetooth_WriteHistoryToFile(pt_paired_dev_history, BLUETOOTH_SRC_HISTORY_FILE);
                if ( i4_ret != BT_SUCCESS)
                {
                    BT_DBG_NORMAL(BT_DEBUG_COMM, "save src device history failed. " );
                    return BT_ERR_STATUS_FAIL;
                }
            }
#endif
            return BT_SUCCESS;
        }
        else
        {
            i = (i + 1) % MAX_DEV_NUM;
        }
    }
    if (i == (pt_paired_dev_history->head_idx - 1 + MAX_DEV_NUM) % MAX_DEV_NUM)
    {
        //find the target history item
        if (0 == strncmp(keyword, pt_paired_dev_history->device_list[i].bdAddr,17))
        {
            BT_DBG_NORMAL(BT_DEBUG_COMM, "success to find the target item is history[%ld]:%s!", (long)i, pt_paired_dev_history->device_list[i].bdAddr);
            //free(pt_paired_dev_history->szhistory[i]);
            memset(&pt_paired_dev_history->device_list[i], 0, sizeof(BT_TARGET_DEV_INFO));
            //pt_paired_dev_history->szhistory[i] = NULL;
            //record forward one item
            for (j=i; j!= (pt_paired_dev_history->head_idx - 1 + MAX_DEV_NUM) % MAX_DEV_NUM;)
            {
                //if next item is null stop move
                if (0 == strlen(pt_paired_dev_history->device_list[(j + 1) % MAX_DEV_NUM].bdAddr))
                {
                    //pt_paired_dev_history->szhistory[j] = NULL;
                    memset(&pt_paired_dev_history->device_list[j], 0, sizeof(BT_TARGET_DEV_INFO));
                    break;
                }

                memcpy(&pt_paired_dev_history->device_list[j],
                       &pt_paired_dev_history->device_list[(j + 1) % MAX_DEV_NUM],
                       sizeof(BT_TARGET_DEV_INFO)
                      );
                j = (j + 1) % MAX_DEV_NUM;
            }
            //the last item should be null
            //pt_paired_dev_history->szhistory[j] = NULL;
            memset(&pt_paired_dev_history->device_list[j], 0, sizeof(BT_TARGET_DEV_INFO));
            pt_paired_dev_history->dev_num--;
            BT_DBG_NORMAL(BT_DEBUG_COMM, "success to move the historylist!");
            /*save the history to tmp*/
#if 1
            INT32 i4_ret = BT_ERR_STATUS_FAIL;
            if (!get_bt_a2dp_be_sink())
            {
                i4_ret = bluetooth_WriteHistoryToFile(pt_paired_dev_history, BLUETOOTH_SINK_HISTORY_FILE);
                if ( i4_ret != BT_SUCCESS)
                {
                    BT_DBG_NORMAL(BT_DEBUG_COMM, "save sink device history failed. " );
                    return BT_ERR_STATUS_FAIL;
                }
            }
            else
            {
                i4_ret = bluetooth_WriteHistoryToFile(pt_paired_dev_history, BLUETOOTH_SRC_HISTORY_FILE);
                if ( i4_ret != BT_SUCCESS )
                {
                    BT_DBG_NORMAL(BT_DEBUG_COMM, "save src device history failed. " );
                    return BT_ERR_STATUS_FAIL;
                }
            }

#endif
            return BT_SUCCESS;
        }
    }
    BT_DBG_NORMAL(BT_DEBUG_COMM, "can't find the target history item!");
    return BT_SUCCESS;
}


INT32 bluetooth_remove_av_history_all(VOID)
{
    FUNC_ENTRY;
    INT32 i4_ret = 0;

#if ENABLE_A2DP_SRC
    /*clear all sink paired device info*/
    memset(&g_paired_sink_dev_history, 0, sizeof(BT_TARGET_DEV_LIST));
    g_pt_paired_sink_dev_history = &g_paired_sink_dev_history;
    i4_ret = bluetooth_WriteHistoryToFile(g_pt_paired_sink_dev_history, BLUETOOTH_SINK_HISTORY_FILE);
    if ( i4_ret != 0 )
    {
        BT_DBG_NORMAL(BT_DEBUG_COMM, "save sink device history failed. " );
        return BT_ERR_STATUS_FAIL;
    }
#endif
#if ENABLE_A2DP_SINK
    /*clear all source paired device info*/
    memset(&g_paired_src_dev_history, 0, sizeof(BT_TARGET_DEV_LIST));
    g_pt_paired_src_dev_history = &g_paired_src_dev_history;
    i4_ret = bluetooth_WriteHistoryToFile(g_pt_paired_src_dev_history, BLUETOOTH_SRC_HISTORY_FILE);
    if ( i4_ret != 0 )
    {
        BT_DBG_NORMAL(BT_DEBUG_COMM, "save src device history failed. " );
        return BT_ERR_STATUS_FAIL;
    }
#endif
    g_fghistorylist_init = TRUE;
    return i4_ret;
}

INT32 bluetooth_hid_removeHistory_one(CHAR* keyword, BT_HID_TARGET_DEV_LIST *pt_paired_dev_history)
{
    FUNC_ENTRY;
    INT32 i4_ret = BT_SUCCESS;
    UINT32 i;
    UINT32 j = 0;
    //CHAR *ptemp = NULL;
    if (NULL == pt_paired_dev_history || NULL == keyword)
    {
        BT_DBG_ERROR(BT_DEBUG_HID, "pt_paired_dev_history is null pointer");
        return BT_ERR_STATUS_PARM_INVALID;
    }
    if (pt_paired_dev_history->head_idx > (MAX_HID_DEV_NUM-1))
    {
        return BT_ERR_STATUS_PARM_INVALID;
    }
    else
    {
        i = pt_paired_dev_history->head_idx;
    }
    while ((i != (pt_paired_dev_history->head_idx - 1 + MAX_HID_DEV_NUM) % MAX_HID_DEV_NUM)
            && (0 < strlen(pt_paired_dev_history->device_list[i].bdAddr)))
    {
        //find the target history item
        BT_DBG_NORMAL(BT_DEBUG_HID, "device_list[%ld].bdAddr is:%s", (long)i, pt_paired_dev_history->device_list[i].bdAddr);
        if (0 == strncmp(keyword, pt_paired_dev_history->device_list[i].bdAddr, 17))
        {
            BT_DBG_NORMAL(BT_DEBUG_HID, "success to find the target item is history[%ld]:%s!", (long)i, keyword);
            memset(&pt_paired_dev_history->device_list[i], 0, sizeof(BT_TARGET_DEV_INFO));

            BT_DBG_NORMAL(BT_DEBUG_HID, "success to delete the target item -- history[%ld]:%s!", (long)i, keyword);
            //record forward one item
            for (j=i; j!= (pt_paired_dev_history->head_idx - 1 + MAX_HID_DEV_NUM) % MAX_HID_DEV_NUM;)
            {
                //if next item is null stop move
                if (0 == strlen(pt_paired_dev_history->device_list[(j + 1) % MAX_HID_DEV_NUM].bdAddr))
                {
                    memset(&pt_paired_dev_history->device_list[j], 0, sizeof(BT_TARGET_DEV_INFO));
                    break ;
                }

                memcpy(&pt_paired_dev_history->device_list[j],
                       &pt_paired_dev_history->device_list[(j + 1) % MAX_HID_DEV_NUM],
                       sizeof(BT_TARGET_DEV_INFO)
                      );
                j = (j + 1) % MAX_HID_DEV_NUM;
            }
            //the last item should be null
            memset(&pt_paired_dev_history->device_list[j], 0, sizeof(BT_TARGET_DEV_INFO));
            //pt_paired_dev_history->szhistory[j] = NULL;
            pt_paired_dev_history->dev_num--;
            BT_DBG_NORMAL(BT_DEBUG_HID, "success to move the historylist!");

            if (0 == pt_paired_dev_history->dev_num)
            {
                BT_DBG_NORMAL(BT_DEBUG_HID, "HID historylist is null!");
                pt_paired_dev_history->head_idx = 0;
            }
            //save the history to FLASH
            i4_ret = BT_SUCCESS;
            i4_ret = bluetooth_hid_WriteHistoryToFile(pt_paired_dev_history, BLUETOOTH_HID_HISTORY_FILE);
            if ( i4_ret != BT_SUCCESS )
            {
                BT_DBG_NORMAL(BT_DEBUG_HID, "save history failed. " );
                return BT_ERR_STATUS_FAIL;
            }

            return BT_SUCCESS;
        }
        else
        {
            i = (i + 1) % MAX_HID_DEV_NUM;
        }
    }
    if (i == (pt_paired_dev_history->head_idx - 1 + MAX_HID_DEV_NUM) % MAX_HID_DEV_NUM)
    {
        //find the target history item
        if (0 == strncmp(keyword, pt_paired_dev_history->device_list[i].bdAddr,17))
        {
            BT_DBG_NORMAL(BT_DEBUG_HID, "success to find the target item is history[%ld]:%s!",
                (long)i, pt_paired_dev_history->device_list[i].bdAddr);
            memset(&pt_paired_dev_history->device_list[i], 0, sizeof(BT_TARGET_DEV_INFO));

            pt_paired_dev_history->dev_num--;
            if (0 == pt_paired_dev_history->dev_num)
            {
                BT_DBG_NORMAL(BT_DEBUG_HID, "HID historylist is null!");
                pt_paired_dev_history->head_idx = 0;
            }

            /*save the history to tmp*/
            i4_ret = BT_SUCCESS;
            i4_ret = bluetooth_hid_WriteHistoryToFile(pt_paired_dev_history, BLUETOOTH_HID_HISTORY_FILE);
            if ( i4_ret != BT_SUCCESS )
            {
                BT_DBG_NORMAL(BT_DEBUG_HID, "save HID device history failed. " );
                return i4_ret;
            }
            return BT_SUCCESS;
        }
    }
    BT_DBG_NORMAL(BT_DEBUG_HID, "can't find the target history item, it have been removed!");
    return BT_SUCCESS;
}

INT32 bluetooth_hid_removeStatus_one(CHAR* keyword, BT_HID_STATUS_STRUCT_LIST *pt_paired_dev_history)
{
    FUNC_ENTRY;
    UINT32 i;
    UINT32 j = 0;
    //CHAR *ptemp = NULL;
    if (NULL == pt_paired_dev_history || NULL == keyword)
    {
        BT_DBG_ERROR(BT_DEBUG_HID, "pt_paired_dev_history is null pointer");
        return BT_ERR_STATUS_PARM_INVALID;
    }
    if (pt_paired_dev_history->head_idx > (MAX_HID_DEV_NUM-1))
    {
        return BT_ERR_STATUS_PARM_INVALID;
    }
    else
    {
        i = pt_paired_dev_history->head_idx;
    }
    while ((i != (pt_paired_dev_history->head_idx - 1 + MAX_HID_DEV_NUM) % MAX_HID_DEV_NUM)
            && (0 < strlen(pt_paired_dev_history->device_list[i].bdAddr)))
    {
        //find the target history item
        BT_DBG_NORMAL(BT_DEBUG_HID, "device_list[%ld].bdAddr is:%s", (long)i, pt_paired_dev_history->device_list[i].bdAddr);
        if (0 == strncmp(keyword, pt_paired_dev_history->device_list[i].bdAddr, MAX_BDADDR_LEN-1))
        {
            BT_DBG_NORMAL(BT_DEBUG_HID, "success to find the target item is history[%ld]:%s!", (long)i, keyword);
            memset(&pt_paired_dev_history->device_list[i], 0, sizeof(BT_HID_STATUS_STRUCT));

            BT_DBG_NORMAL(BT_DEBUG_HID, "success to delete the target item -- history[%ld]:%s!", (long)i, keyword);
            //record forward one item
            for (j=i; j!= (pt_paired_dev_history->head_idx - 1 + MAX_HID_DEV_NUM) % MAX_HID_DEV_NUM;)
            {
                //if next item is null stop move
                if (0 == strlen(pt_paired_dev_history->device_list[(j + 1) % MAX_HID_DEV_NUM].bdAddr))
                {
                    memset(&pt_paired_dev_history->device_list[j], 0, sizeof(BT_HID_STATUS_STRUCT));
                    break ;
                }

                memcpy(&pt_paired_dev_history->device_list[j],
                       &pt_paired_dev_history->device_list[(j + 1) % MAX_HID_DEV_NUM],
                       sizeof(BT_HID_STATUS_STRUCT)
                      );
                j = (j + 1) % MAX_HID_DEV_NUM;
            }
            //the last item should be null
            memset(&pt_paired_dev_history->device_list[j], 0, sizeof(BT_HID_STATUS_STRUCT));
            //pt_paired_dev_history->szhistory[j] = NULL;
            pt_paired_dev_history->dev_num--;
            BT_DBG_NORMAL(BT_DEBUG_HID, "success to move the historylist!");

            if (0 == pt_paired_dev_history->dev_num)
            {
                BT_DBG_NORMAL(BT_DEBUG_HID, "HID historylist is null!");
                pt_paired_dev_history->head_idx = 0;
            }

            return BT_SUCCESS;
        }
        else
        {
            i = (i + 1) % MAX_HID_DEV_NUM;
        }
    }
    if (i == (pt_paired_dev_history->head_idx - 1 + MAX_HID_DEV_NUM) % MAX_HID_DEV_NUM)
    {
        //find the target history item
        if (0 == strncmp(keyword, pt_paired_dev_history->device_list[i].bdAddr, MAX_BDADDR_LEN-1))
        {
            BT_DBG_NORMAL(BT_DEBUG_HID, "success to find the target item is history[%ld]:%s!",
                (long)i, pt_paired_dev_history->device_list[i].bdAddr);
            memset(&pt_paired_dev_history->device_list[i], 0, sizeof(BT_HID_STATUS_STRUCT));

            pt_paired_dev_history->dev_num--;
            if (0 == pt_paired_dev_history->dev_num)
            {
                BT_DBG_NORMAL(BT_DEBUG_HID, "HID historylist is null!");
                pt_paired_dev_history->head_idx = 0;
            }

            return BT_SUCCESS;
        }
    }
    BT_DBG_NORMAL(BT_DEBUG_HID, "can't find the target history item, it have been removed!");
    return BT_SUCCESS;
}

INT32 bluetooth_hid_removeHistory_all(VOID)
{
    FUNC_ENTRY;
    INT32 i4_ret = BT_SUCCESS;

#if ENABLE_HID_PROFILE_H
    /*clear all hid paired device info*/
    memset(&g_paired_hid_dev_history, 0, sizeof(BT_HID_TARGET_DEV_LIST));
    g_pt_paired_hid_dev_history = &g_paired_hid_dev_history;
    memset(&g_hid_status_list, 0, sizeof(BT_HID_STATUS_STRUCT_LIST));
    g_pt_hid_status_list = &g_hid_status_list;
    i4_ret = bluetooth_hid_WriteHistoryToFile(g_pt_paired_hid_dev_history, BLUETOOTH_HID_HISTORY_FILE);
    if ( i4_ret != BT_SUCCESS )
    {
        BT_DBG_NORMAL(BT_DEBUG_HID, "save sink device history failed. " );
        return BT_ERR_STATUS_FAIL;
    }
#endif

    g_fghistorylist_init = TRUE;
    return i4_ret;
}

INT32 bluetooth_hid_removeStatus_all(VOID)
{
    FUNC_ENTRY;
    INT32 i4_ret = BT_SUCCESS;

#if ENABLE_HID_PROFILE_H
    /*clear all sink paired device info*/
    memset(&g_hid_status_list, 0, sizeof(BT_HID_STATUS_STRUCT_LIST));
    g_pt_hid_status_list = &g_hid_status_list;
#endif

    return i4_ret;
}

/****************************************************
 * FUNCTION NAME: bluetooth_set_device_status
 * PURPOSE:
 *      The function is used to set bt status, paired or connected.
 * INPUT:
 *      bdaddr: bt address
 *      status: bt status
 * OUTPUT:
 *      None
 * RETURN:
 *      INT32:error code
 * NOTES:
 *      None
 */
INT32 bluetooth_set_device_status(char *bdaddr, BT_STATUS status)
{
    FUNC_ENTRY;
    BT_CHECK_POINTER(BT_DEBUG_COMM, bdaddr);

    BT_DBG_INFO(BT_DEBUG_COMM,  "bdaddr:%s, status:%d", bdaddr, status);
    print_hid_status_info(g_pt_hid_status_list);

    UINT32 i = 0;
    UINT32 j = 0;/* j is for count device num */
    BT_CHECK_POINTER(BT_DEBUG_COMM, g_pt_hid_status_list);
    i = g_pt_hid_status_list->head_idx;
    BT_DBG_INFO(BT_DEBUG_COMM, "head_idx:%ld", (long)i);
    while (j < g_pt_hid_status_list->dev_num)
    {
        /* find target devie */
        if (0 == strcmp(bdaddr, g_pt_hid_status_list->device_list[i].bdAddr))
        {
            BT_DBG_INFO(BT_DEBUG_COMM, "find device, i:%d", i);
            g_pt_hid_status_list->device_list[i].status = status;
            break;
        }
        i = (i + 1) % MAX_HID_DEV_NUM;
        j++;
    }

    /*not find target devie */
    if (j == g_pt_hid_status_list->dev_num)
    {
        BT_DBG_NORMAL(BT_DEBUG_COMM, "not find device, i:%d", i);
    }

    return BT_SUCCESS;
}

VOID bluetooth_hid_setHistory(BT_TARGET_DEV_INFO *history, BT_HID_TARGET_DEV_LIST *pt_paired_dev_history)
{
    FUNC_ENTRY;
    INT32 i4_ret = BT_SUCCESS;
    UINT32 i;
    UINT32 j;
    UINT32 k;

    if (NULL == history)
    {
        BT_DBG_ERROR(BT_DEBUG_COMM, "history is null pointer");
        return ;
    }
    if (NULL == pt_paired_dev_history)
    {
        BT_DBG_ERROR(BT_DEBUG_COMM, "pt_paired_dev_history is null pointer");
        return ;
    }

    print_hid_history_info(pt_paired_dev_history);

    if (pt_paired_dev_history->head_idx > (MAX_HID_DEV_NUM-1))
    {
        BT_DBG_ERROR(BT_DEBUG_COMM, "recover head index:%ld to 0", (long)pt_paired_dev_history->head_idx);
        pt_paired_dev_history->head_idx = 0;

    }
    i = pt_paired_dev_history->head_idx;
    BT_DBG_NORMAL(BT_DEBUG_COMM, " history address:%s name:%s", history->bdAddr, history->name);
    /*when history items equal to MAX_DEV_NUM*/
    if (MAX_HID_DEV_NUM == pt_paired_dev_history->dev_num)
    {
        BT_DBG_NORMAL(BT_DEBUG_COMM, "have MAX_HID_DEV_NUM devices");
        /*find duplicated item*/
        j = 0;
        while (j < MAX_HID_DEV_NUM)
        {
            /*have duplicated item*/
            if (0 == strncmp(history->bdAddr, pt_paired_dev_history->device_list[i].bdAddr, MAX_BDADDR_LEN-1))
            {
                /*the duplicated item is the newest one, no need move*/
                if (i == (pt_paired_dev_history->head_idx - 1 + MAX_HID_DEV_NUM)%MAX_HID_DEV_NUM)
                {
                    BT_DBG_NORMAL(BT_DEBUG_COMM, "the item in the list and is the newest one");
                    /*in case of the same mac address not the same name
                                    but if the device name is no name should not update to it*/
                    if (0<strlen(history->name) && (0 != (strncmp(history->name,"no name",strlen("no name")))))
                    {
                        memset(pt_paired_dev_history->device_list[i].name,
                               0,
                               sizeof(pt_paired_dev_history->device_list[i].name));
                        strncpy(pt_paired_dev_history->device_list[i].name,
                                history->name,
                                strlen(history->name)
                               );
                        if(0 != history->cod)
                        {
                            pt_paired_dev_history->device_list[i].cod = history->cod;
                        }
                    }
                }
                else
                {
                    k = i;
                    /*change the sequcence:  every item need move one step backforword*/
                    while ((k != (pt_paired_dev_history->head_idx - 1 + MAX_HID_DEV_NUM) % MAX_HID_DEV_NUM)
                            && (0 < strlen(pt_paired_dev_history->device_list[k].bdAddr))
                          )
                    {
                        memcpy(&pt_paired_dev_history->device_list[k],
                               &pt_paired_dev_history->device_list[(k + 1) % MAX_HID_DEV_NUM],
                               sizeof(BT_TARGET_DEV_INFO)
                              );
                        k = (k + 1) % MAX_HID_DEV_NUM;

                    }
                    /*set history item to the newest one*/
                    memcpy(&pt_paired_dev_history->device_list[k],
                           history,
                           sizeof(BT_TARGET_DEV_INFO)
                          );
                }
                /*complete handle duplicated item*/

                goto save_file;
                //return;
            }
            else
            {
                i = (i + 1) % MAX_HID_DEV_NUM;
            }
            j++;
        }
        /*no duplicated item, replace the old one with the newest one and modify the index*/
        bluetooth_remove_paired_dev(pt_paired_dev_history->device_list[pt_paired_dev_history->head_idx].bdAddr);
        memset(&pt_paired_dev_history->device_list[pt_paired_dev_history->head_idx],
               0,
               sizeof(BT_TARGET_DEV_INFO));
        memcpy(&pt_paired_dev_history->device_list[pt_paired_dev_history->head_idx],
               history,
               sizeof(BT_TARGET_DEV_INFO)
              );
        pt_paired_dev_history->head_idx = (pt_paired_dev_history->head_idx + 1) % MAX_HID_DEV_NUM;
    }
    /*the number of history item in list less than MAX_HID_DEV_NUM*/
    else
    {
        BT_DBG_NORMAL(BT_DEBUG_HID, "have %ld devices", (long)pt_paired_dev_history->dev_num);
        /*find duplicated item*/
        /*  j just for count device num*/
        j = 0;
        while (j < pt_paired_dev_history->dev_num)
        {
            /*have duplicated item*/
            if (0 == strncmp(history->bdAddr, pt_paired_dev_history->device_list[i].bdAddr, 17))
            {
                /*the duplicated item is the newest one*/
                if (0 == strlen(pt_paired_dev_history->device_list[(i+1)%MAX_HID_DEV_NUM].bdAddr))
                {
                    /*only 1st device can into this case*/
                    BT_DBG_NORMAL(BT_DEBUG_COMM, "the item in the list and is the newest one");
                    /*in case of the same mac address not the same name
                                    but if the device name is no name should not update to it*/
                    if (0<strlen(history->name) && (0 != (strncmp(history->name,"no name",strlen("no name")))))
                    {
                        memset(pt_paired_dev_history->device_list[i].name,
                               0,
                               sizeof(pt_paired_dev_history->device_list[i].name));
                        strncpy(pt_paired_dev_history->device_list[i].name,
                                history->name,
                                strlen(history->name)
                               );
                        if(0 != history->cod)
                        {
                            pt_paired_dev_history->device_list[i].cod = history->cod;
                        }
                    }
                }
                else
                {
                    /* in this time i is the duplicated one */
                    k = i;
                    /*change the sequcence:  every item need move one step backforword*/
                    while ((k != (pt_paired_dev_history->head_idx+pt_paired_dev_history->dev_num-1)%MAX_HID_DEV_NUM)
                            && (0 < strlen(pt_paired_dev_history->device_list[k].bdAddr))
                          )
                    {
                        memcpy(&pt_paired_dev_history->device_list[k],
                               &pt_paired_dev_history->device_list[(k + 1) % MAX_HID_DEV_NUM],
                               sizeof(BT_TARGET_DEV_INFO)
                              );
                        k = (k + 1) % MAX_HID_DEV_NUM;

                    }
                    /*set history item to the newest one*/
                    memcpy(&pt_paired_dev_history->device_list[k],
                           history,
                           sizeof(BT_TARGET_DEV_INFO)
                          );
                }
                /*complete handle duplicated item*/
                goto save_file;
                //return;
            }
            else
            {
                i = (i + 1) % MAX_HID_DEV_NUM;
            }
            j++;
        }
        /*no duplicated item, add new item to last of history list*/
        i = pt_paired_dev_history->head_idx;
        /* this time j is for the last pos in dev list */
        j = (i + pt_paired_dev_history->dev_num) % MAX_HID_DEV_NUM;
        BT_DBG_NORMAL(BT_DEBUG_COMM, "head_idx:%ld,dev_num:%ld", (long)i, (long)j);
        BT_DBG_NORMAL(BT_DEBUG_COMM, "new item,add to the last position:%ld", (long)j);
        memset(&pt_paired_dev_history->device_list[j],
               0,
               sizeof(pt_paired_dev_history->device_list[j]));
        memcpy(&pt_paired_dev_history->device_list[j],
               history,
               sizeof(BT_TARGET_DEV_INFO)
              );
        pt_paired_dev_history->dev_num++;
    }
save_file:
    //print the paired device list
    print_hid_history_info(pt_paired_dev_history);

    /*save the history list to FLASH*/
    i4_ret = BT_SUCCESS;
    i4_ret = bluetooth_hid_WriteHistoryToFile(pt_paired_dev_history, BLUETOOTH_HID_HISTORY_FILE);
    if ( i4_ret != BT_SUCCESS )
    {
        BT_DBG_ERROR(BT_DEBUG_HID, "save history failed. " );
    }
    return;

}

VOID bluetooth_hid_setStatus(BT_HID_STATUS_STRUCT *history, BT_HID_STATUS_STRUCT_LIST *pt_paired_dev_status)
{
    FUNC_ENTRY;
    UINT32 i;
    UINT32 j;
    UINT32 k;

    if (NULL == history)
    {
        BT_DBG_ERROR(BT_DEBUG_COMM, "history is null pointer");
        return ;
    }

    BT_DBG_INFO(BT_DEBUG_COMM, "history.bdaddr=%s, status=%d.", history->bdAddr, history->status);

    if (NULL == pt_paired_dev_status)
    {
        BT_DBG_ERROR(BT_DEBUG_COMM, "pt_paired_dev_status is null pointer");
        return ;
    }

    print_hid_status_info(pt_paired_dev_status);

    if (pt_paired_dev_status->head_idx > (MAX_HID_DEV_NUM-1))
    {
        BT_DBG_ERROR(BT_DEBUG_COMM, "recover head index:%ld to 0", (long)pt_paired_dev_status->head_idx);
        pt_paired_dev_status->head_idx = 0;

    }
    i = pt_paired_dev_status->head_idx;
    /*when history items equal to MAX_DEV_NUM*/
    if (MAX_HID_DEV_NUM == pt_paired_dev_status->dev_num)
    {
        BT_DBG_NORMAL(BT_DEBUG_COMM, "have MAX_HID_DEV_NUM devices");
        /*find duplicated item*/
        j = 0;
        while (j < MAX_HID_DEV_NUM)
        {
            /*have duplicated item*/
            if (0 == strncmp(history->bdAddr, pt_paired_dev_status->device_list[i].bdAddr, 17))
            {
                /*the duplicated item is the newest one, no need move*/
                if (i == (pt_paired_dev_status->head_idx - 1 + MAX_HID_DEV_NUM)%MAX_HID_DEV_NUM)
                {
                    BT_DBG_NORMAL(BT_DEBUG_COMM, "the item in the list and is the newest one");
                    BT_DBG_INFO(BT_DEBUG_COMM, "pt_paired_dev_status->device_list[%d].bdAddr=%s, status=%d.",
                        i, pt_paired_dev_status->device_list[i].bdAddr, pt_paired_dev_status->device_list[i].status);
                    pt_paired_dev_status->device_list[i].status = history->status;
                }
                else
                {
                    k = i;
                    /*change the sequcence:  every item need move one step backforword*/
                    while ((k != (pt_paired_dev_status->head_idx - 1 + MAX_HID_DEV_NUM) % MAX_HID_DEV_NUM)
                            && (0 < strlen(pt_paired_dev_status->device_list[k].bdAddr))
                          )
                    {
                        memcpy(&pt_paired_dev_status->device_list[k],
                               &pt_paired_dev_status->device_list[(k + 1) % MAX_HID_DEV_NUM],
                               sizeof(BT_HID_STATUS_STRUCT)
                              );
                        k = (k + 1) % MAX_HID_DEV_NUM;

                    }
                    /*set history item to the newest one*/
                    memcpy(&pt_paired_dev_status->device_list[k],
                           history,
                           sizeof(BT_HID_STATUS_STRUCT)
                          );
                }
                /*complete handle duplicated item*/
                goto save_file;
                //return;
            }
            else
            {
                i = (i + 1) % MAX_HID_DEV_NUM;
            }
            j++;
        }
        /*no duplicated item, replace the old one with the newest one and modify the index*/
        //bluetooth_remove_paired_dev(pt_paired_dev_status->device_list[pt_paired_dev_status->head_idx].bdAddr);
        BT_DBG_NORMAL(BT_DEBUG_COMM, "the item in the list and is the newest one");
        memset(&pt_paired_dev_status->device_list[pt_paired_dev_status->head_idx],
               0,
               sizeof(BT_HID_STATUS_STRUCT));
        memcpy(&pt_paired_dev_status->device_list[pt_paired_dev_status->head_idx],
               history,
               sizeof(BT_HID_STATUS_STRUCT)
              );
        pt_paired_dev_status->head_idx = (pt_paired_dev_status->head_idx + 1) % MAX_HID_DEV_NUM;

    }
    /*the number of history item in list less than MAX_DEV_NUM*/
    else
    {
        BT_DBG_NORMAL(BT_DEBUG_COMM, "have %ld devices", (long)pt_paired_dev_status->dev_num);
        /*find duplicated item*/
        /*  j just for count device num*/
        j = 0;
        while (j < pt_paired_dev_status->dev_num)
        {
            /*have duplicated item*/
            if (0 == strncmp(history->bdAddr, pt_paired_dev_status->device_list[i].bdAddr, 17))
            {
                /*the duplicated item is the newest one, no need move*/
                if (0 == strlen(pt_paired_dev_status->device_list[(i+1)%MAX_HID_DEV_NUM].bdAddr))
                {
                    BT_DBG_NORMAL(BT_DEBUG_COMM, "the item in the list and is the newest one");
                    BT_DBG_INFO(BT_DEBUG_COMM, "pt_paired_dev_status->device_list[%d].bdAddr=%s, status=%d.",
                        i, pt_paired_dev_status->device_list[i].bdAddr, pt_paired_dev_status->device_list[i].status);
                    pt_paired_dev_status->device_list[i].status = history->status;
                }
                else
                {
                    /* in this time i is the duplicated one */
                    k = i;
                    /*change the sequcence:  every item need move one step backforword*/
                    while ((k != (pt_paired_dev_status->head_idx+pt_paired_dev_status->dev_num-1)%MAX_HID_DEV_NUM)
                            && (0 < strlen(pt_paired_dev_status->device_list[k].bdAddr))
                          )
                    {
                        memcpy(&pt_paired_dev_status->device_list[k],
                               &pt_paired_dev_status->device_list[(k + 1) % MAX_HID_DEV_NUM],
                               sizeof(BT_HID_STATUS_STRUCT)
                              );
                        k = (k + 1) % MAX_HID_DEV_NUM;

                    }
                    /*set history item to the newest one*/
                    memcpy(&pt_paired_dev_status->device_list[k],
                           history,
                           sizeof(BT_HID_STATUS_STRUCT)
                          );
                }
                /*complete handle duplicated item*/
                goto save_file;
                //return;
            }
            else
            {
                i = (i + 1) % MAX_HID_DEV_NUM;
            }
            j++;
        }
        /*no duplicated item, add new item to last of history list*/
        i = pt_paired_dev_status->head_idx;
        /* this time j is for the last pos in dev list */
        j = (i + pt_paired_dev_status->dev_num) % MAX_HID_DEV_NUM;
        BT_DBG_NORMAL(BT_DEBUG_COMM, "head_idx:%ld, dev_num:%ld", (long)i, (long)j);
        BT_DBG_NORMAL(BT_DEBUG_COMM, "new item,add to the last position:%ld", (long)j);
        memset(&pt_paired_dev_status->device_list[j],
               0,
               sizeof(pt_paired_dev_status->device_list[j]));
        memcpy(&pt_paired_dev_status->device_list[j],
               history,
               sizeof(BT_HID_STATUS_STRUCT)
              );
        pt_paired_dev_status->dev_num++;
    }
save_file:
    //print the paired device list
    print_hid_status_info(pt_paired_dev_status);

    return;
}

INT32 bluetooth_hid_getStatus(CHAR* bdaddr, BT_STATUS *status)
{
    FUNC_ENTRY;
    UINT32 i = 0;
    UINT32 j = 0;/* j is for count device num */
    BT_STATUS ret_status = BT_PAIRED_STATUS;
    BT_CHECK_POINTER(BT_DEBUG_COMM, g_pt_hid_status_list);
    i = g_pt_hid_status_list->head_idx;
    BT_DBG_INFO(BT_DEBUG_COMM, "head_idx:%d, dev_num:%d", i, g_pt_hid_status_list->dev_num);
    while (j < g_pt_hid_status_list->dev_num)
    {

        if(0 == strncmp(bdaddr, g_pt_hid_status_list->device_list[i].bdAddr, MAX_BDADDR_LEN))
        {
            BT_DBG_NORMAL(BT_DEBUG_COMM,  "Find address index:%d.", i);
            ret_status = g_pt_hid_status_list->device_list[i].status;
            status = &ret_status;
            break;
        }
        i = (i + 1) % MAX_HID_DEV_NUM;
        j++;
    }

    if (j == g_pt_hid_status_list->dev_num)
    {
        BT_DBG_ERROR(BT_DEBUG_HID, "Error HID address.");
        return BT_ERR_STATUS_FAIL;
    }

    return BT_SUCCESS;
}

#if 1


INT32 bluetooth_ReadLocalInfoFromFile(CHAR *filename)
{
    FUNC_ENTRY;

    BT_GAP_LOCAL_PROPERTIES_RESULT pt_local_info;
    FILE *fp_local_dev_info = NULL;

    if (NULL == filename)
    {
        BT_DBG_ERROR(BT_DEBUG_COMM, "file_name is null in lines:%ld @ %s!", (long)__LINE__, __FUNCTION__);
        return BT_ERR_STATUS_FAIL;
    }
    if (0 != access(filename, F_OK))
    {
        BT_DBG_ERROR(BT_DEBUG_COMM, "file:%s not exist!", filename);
        //g_pt_paired_dev_history->head_idx = 0;
        return BT_SUCCESS;
    }
    fp_local_dev_info = fopen(filename, "r");

    if (fp_local_dev_info == NULL)
    {
        BT_DBG_ERROR(BT_DEBUG_COMM, "Failed to Read %s:%ld", filename, (long)errno);
        remove(filename);
        return BT_ERR_STATUS_FAIL;
    }
    fread(&pt_local_info, sizeof(BT_GAP_LOCAL_PROPERTIES_RESULT), 1, fp_local_dev_info);
    memset(g_localProperty.bdAddr, 0, sizeof(CHAR)*MAX_BDADDR_LEN);
    memset(g_localProperty.name, 0, sizeof(CHAR)*MAX_NAME_LEN);
    strncpy(g_localProperty.bdAddr, pt_local_info.bdAddr, strlen(pt_local_info.bdAddr));
    g_localProperty.bdAddr[17] = 0x0;
    strncpy(g_localProperty.name, pt_local_info.name, strlen(pt_local_info.name));
    g_localProperty.name[strlen(pt_local_info.name)] = 0x0;
    BT_DBG_INFO(BT_DEBUG_COMM, "local addr %s", g_localProperty.bdAddr);
    BT_DBG_INFO(BT_DEBUG_COMM, "local name %s", g_localProperty.name);

    fclose(fp_local_dev_info);
    FUNC_EXIT;
    return BT_SUCCESS;
}


INT32 bluetooth_WriteLocalInfoToFile(BT_GAP_LOCAL_PROPERTIES_RESULT *pt_local_info)
{
    FUNC_ENTRY;
    INT32 i4_fd = -1;
    BT_GAP_LOCAL_PROPERTIES_RESULT t_local_info;

    if (0 != access(BLUETOOTH_MW_LOCAL_FOLDER, F_OK))
    {
        BT_DBG_ERROR(BT_DEBUG_COMM, "should create:%s", BLUETOOTH_MW_LOCAL_FOLDER);
        if (mkdir(BLUETOOTH_MW_LOCAL_FOLDER, 0777) == 0)
        {
            BT_DBG_INFO(BT_DEBUG_COMM, "create:%s success", BLUETOOTH_MW_LOCAL_FOLDER);
        }
        else
        {
            BT_DBG_ERROR(BT_DEBUG_COMM, "create:%s fail:%ld", BLUETOOTH_MW_LOCAL_FOLDER, (long)errno);
        }
        return BT_ERR_STATUS_FAIL;
    }
    /*for clear local device info*/
    if (NULL == pt_local_info)
    {
        BT_DBG_NORMAL(BT_DEBUG_COMM, "clear local device info");
        memset(&t_local_info, 0, sizeof(BT_GAP_LOCAL_PROPERTIES_RESULT));
        i4_fd = open(BLUETOOTH_LOCAL_INFO_FILE, O_RDWR|O_CREAT|O_SYNC, 0666);
        if (-1 == i4_fd)
        {
            BT_DBG_NORMAL(BT_DEBUG_COMM, "Fail to save file:%ld", (long)errno);
            return BT_ERR_STATUS_FAIL;
        }
        write(i4_fd, &t_local_info, sizeof(BT_GAP_LOCAL_PROPERTIES_RESULT));
        close(i4_fd);
        sync();
        return BT_SUCCESS;
    }

    //save the txt file
    i4_fd = open(BLUETOOTH_LOCAL_INFO_FILE, O_RDWR|O_CREAT|O_SYNC, 0666);
    if (-1 == i4_fd)
    {
        BT_DBG_NORMAL(BT_DEBUG_COMM, "Fail to save file:%ld", (long)errno);
        return BT_ERR_STATUS_FAIL;
    }
    write(i4_fd, pt_local_info, sizeof(BT_GAP_LOCAL_PROPERTIES_RESULT));
    close(i4_fd);
    sync();
    return BT_SUCCESS;
}


INT32 bluetooth_init_local_dev_info (VOID)
{
    FUNC_ENTRY;
    INT32 i4_ret;

    //memset(&g_localProperty, 0, sizeof(g_localProperty));
    i4_ret = bluetooth_ReadLocalInfoFromFile(BLUETOOTH_LOCAL_INFO_FILE);
    if (0 != i4_ret)
    {
        BT_DBG_NORMAL(BT_DEBUG_COMM, "bluetooth_ReadLocalInfoFromFile is failed.");
        if (TRUE == get_bt_power_on())
        {
            memset(&g_localProperty, 0, sizeof(g_localProperty));
            bluetooth_update_local_dev_info();
        }
        else
        {
            BT_DBG_ERROR(BT_DEBUG_COMM, "need power on bluetooth first");
            return BT_SUCCESS;
        }
    }
    /* in case of after factory reset no device info */
    if ((17 != strlen(g_localProperty.bdAddr)) && (0 == strlen(g_localProperty.name)))
    {
        if (TRUE == get_bt_power_on())
        {
            memset(&g_localProperty, 0, sizeof(g_localProperty));

            bluetooth_set_local_dev_name(BLUETOOTH_ORIGINAL_NAME);
            bluetooth_update_local_dev_info();
        }
        else
        {
            BT_DBG_ERROR(BT_DEBUG_COMM, "need power on bluetooth first");
            return BT_SUCCESS;
        }
    }
    g_fg_local_dev_init = TRUE;

    return BT_SUCCESS;
}

BOOL bluetooth_local_dev_is_init(VOID)
{
    BT_DBG_INFO(BT_DEBUG_COMM, "dev name is init:%s", g_fg_local_dev_init?"yes":"no");
    return g_fg_local_dev_init;

}


#endif

VOID bluetooth_check_default_folder(VOID)
{
    FUNC_ENTRY;
    struct stat fileStat;

    /* Create MW folder */
    if (stat(BLUETOOTH_MW_LOCAL_FOLDER,&fileStat) < 0)
    {
        BT_DBG_ERROR(BT_DEBUG_COMM, "[GAP] %s %ld", BLUETOOTH_MW_LOCAL_FOLDER, (long)errno);
        BT_DBG_NORMAL(BT_DEBUG_COMM, "[GAP] Create Folder: %s", BLUETOOTH_MW_LOCAL_FOLDER);
        if (mkdir((CHAR *)BLUETOOTH_MW_LOCAL_FOLDER, 0777) != 0)
        {
            BT_DBG_ERROR(BT_DEBUG_COMM, "[GAP] Create Folder Failed! (%s)(%ld)", BLUETOOTH_MW_LOCAL_FOLDER, (long)errno);
            return;
        }
    }
    else
    {
        BT_DBG_NORMAL(BT_DEBUG_COMM, "[GAP] have Folder: %s", BLUETOOTH_MW_LOCAL_FOLDER);
    }
    /* Create MW logs folder */
    if (stat(BLUETOOTH_MW_LOCAL_FOLDER"/logs",&fileStat) < 0)
    {
        BT_DBG_ERROR(BT_DEBUG_COMM, "[GAP] %s %ld", BLUETOOTH_MW_LOCAL_FOLDER"/logs", (long)errno);
        if (mkdir((CHAR *)BLUETOOTH_MW_LOCAL_FOLDER"/logs", 0777) != 0)
        {
            BT_DBG_ERROR(BT_DEBUG_COMM, "[GAP] Create Folder Failed! (%s)(%ld)", BLUETOOTH_MW_LOCAL_FOLDER"/logs", (long)errno);
            return;
        }
    }
    else
    {
        BT_DBG_NORMAL(BT_DEBUG_COMM, "[GAP] have Folder: %s", BLUETOOTH_MW_LOCAL_FOLDER"/logs");
    }
    /* Create Stack folder */
    if (stat(BLUETOOTH_STACK_LOCAL_FOLDER,&fileStat) < 0)
    {
        BT_DBG_ERROR(BT_DEBUG_COMM, "[GAP] %s %ld", BLUETOOTH_STACK_LOCAL_FOLDER, (long)errno);
        BT_DBG_NORMAL(BT_DEBUG_COMM, "[GAP] Create Folder: %s", BLUETOOTH_STACK_LOCAL_FOLDER);
        if (mkdir((CHAR *)BLUETOOTH_STACK_LOCAL_FOLDER, 0777) != 0)
        {
            BT_DBG_ERROR(BT_DEBUG_COMM, "[GAP] Create Folder Failed! (%s)(%ld)", BLUETOOTH_STACK_LOCAL_FOLDER, (long)errno);
            return;
        }
    }
    else
    {
        BT_DBG_NORMAL(BT_DEBUG_COMM, "[GAP] have Folder: %s", BLUETOOTH_STACK_LOCAL_FOLDER);
    }
}

INT32 bluetooth_remove_av_history(BT_TARGET_DEV_LIST *pt_paired_dev_history)
{
    FUNC_ENTRY;
    UINT32 i;

    if (NULL == pt_paired_dev_history)
    {
        BT_DBG_ERROR(BT_DEBUG_COMM, "pt_paired_dev_history is null pointer\n");
        return BT_ERR_STATUS_FAIL;
    }
    if (pt_paired_dev_history->head_idx > (MAX_DEV_NUM-1))
    {
        return BT_ERR_STATUS_FAIL;
    }
    else
    {
        i = pt_paired_dev_history->head_idx;
    }
    while ((i != (pt_paired_dev_history->head_idx - 1 + MAX_DEV_NUM) % MAX_DEV_NUM)
            && (0 < strlen(pt_paired_dev_history->device_list[i].bdAddr)))
    {
        BT_DBG_NORMAL(BT_DEBUG_COMM, "device_list[%ld].bdAddr is:%s\n", i, pt_paired_dev_history->device_list[i].bdAddr);
        bluetooth_remove_paired_dev(pt_paired_dev_history->device_list[i].bdAddr);
        i = (i + 1) % MAX_DEV_NUM;
    }

    if ((i == (pt_paired_dev_history->head_idx - 1 + MAX_DEV_NUM) % MAX_DEV_NUM)
        && (0 < strlen(pt_paired_dev_history->device_list[i].bdAddr)))
    {
        BT_DBG_NORMAL(BT_DEBUG_COMM, "last device_list[%ld].bdAddr is:%s\n", i, pt_paired_dev_history->device_list[i].bdAddr);
        bluetooth_remove_paired_dev(pt_paired_dev_history->device_list[i].bdAddr);
    }

    return BT_SUCCESS;
}


INT32 bluetooth_hid_removeHistory_except_rc(CHAR* keyword, BT_HID_TARGET_DEV_LIST *pt_paired_dev_history)
{
    FUNC_ENTRY;
    INT32 i4_ret = BT_SUCCESS;
    UINT32 i;
    if (NULL == pt_paired_dev_history || NULL == keyword)
    {
        BT_DBG_ERROR(BT_DEBUG_HID, "pt_paired_dev_history is null pointer\n");
        return BT_ERR_STATUS_PARM_INVALID;
    }
    if (pt_paired_dev_history->head_idx > (MAX_HID_DEV_NUM-1))
    {
        return BT_ERR_STATUS_PARM_INVALID;
    }
    else
    {
        i = pt_paired_dev_history->head_idx;
    }

    while ((i != (pt_paired_dev_history->head_idx - 1 + MAX_HID_DEV_NUM) % MAX_HID_DEV_NUM)
            && (0 < strlen(pt_paired_dev_history->device_list[i].bdAddr)))
    {
        //find the target history item
        BT_DBG_NORMAL(BT_DEBUG_HID, "device_list[%ld].bdAddr is:%s\n", i, pt_paired_dev_history->device_list[i].bdAddr);
        if (0 != strncmp(keyword, pt_paired_dev_history->device_list[i].bdAddr, MAX_BDADDR_LEN-1))
        {
            BT_DBG_NORMAL(BT_DEBUG_HID, "success to find the target item is history[%ld]:%s!\n", i, keyword);

            bluetooth_remove_paired_dev(pt_paired_dev_history->device_list[i].bdAddr);

            memset(&pt_paired_dev_history->device_list[i], 0, sizeof(BT_TARGET_DEV_INFO));

            BT_DBG_NORMAL(BT_DEBUG_HID, "success to delete the target item -- history[%ld]:%s!\n", i, keyword);

            pt_paired_dev_history->dev_num--;
            BT_DBG_NORMAL(BT_DEBUG_HID, "dev_num=%d\n", pt_paired_dev_history->dev_num);

        }
        else
        {
            BT_DBG_NORMAL(BT_DEBUG_HID, "Its RC address=%s\n", keyword);
        }

        i = (i + 1) % MAX_HID_DEV_NUM;
    }

    if (i == (pt_paired_dev_history->head_idx - 1 + MAX_HID_DEV_NUM) % MAX_HID_DEV_NUM)
    {
        if (0 != strncmp(keyword, pt_paired_dev_history->device_list[i].bdAddr, MAX_BDADDR_LEN-1))
        {
            BT_DBG_NORMAL(BT_DEBUG_HID, "success to find the last target item is history[%ld]:%s!\n",
                i, pt_paired_dev_history->device_list[i].bdAddr);

            bluetooth_remove_paired_dev(pt_paired_dev_history->device_list[i].bdAddr);

            memset(&pt_paired_dev_history->device_list[i], 0, sizeof(BT_TARGET_DEV_INFO));

            pt_paired_dev_history->dev_num--;
        }
    }

    //save the history to FLASH
    i4_ret = bluetooth_hid_WriteHistoryToFile(pt_paired_dev_history, BLUETOOTH_HID_HISTORY_FILE);
    if ( i4_ret != BT_SUCCESS )
    {
        BT_DBG_ERROR(BT_DEBUG_HID, "save hid history failed. \n" );
        return BT_ERR_STATUS_FAIL;
    }

    return BT_SUCCESS;
}

INT32 bluetooth_get_namebyaddr(BT_TARGET_DEV_INFO *pt_dev_info, BT_DEV_LIST_IN_NAMES *pt_device_list)
{
    FUNC_ENTRY;
    INT32 i = 0;
    BT_CHECK_POINTER(BT_DEBUG_COMM, pt_dev_info);
    BT_CHECK_POINTER(BT_DEBUG_COMM, pt_device_list);
    for (i=0; i < pt_device_list->dev_num; i++)
    {
        BT_DBG_NORMAL(BT_DEBUG_COMM, "pt_dev_info->bdAddr: %s,pt_device_list->device_list[%ld].bdAddr: %s",
                      pt_dev_info->bdAddr,
                      (long)i,
                      pt_device_list->device_list[i].bdAddr
                     );
        if (0 == strncmp(pt_dev_info->bdAddr, pt_device_list->device_list[i].bdAddr, 17))
        {
            BT_DBG_NORMAL(BT_DEBUG_COMM, "find out the devcie addr:%s name:%s",
                          pt_device_list->device_list[i].bdAddr,
                          pt_device_list->device_list[i].name
                         );
            strncpy(pt_dev_info->name, pt_device_list->device_list[i].name, sizeof(pt_dev_info->name));
            return BT_SUCCESS;
        }
    }
    BT_DBG_NORMAL(BT_DEBUG_COMM, "can't find device");
    return BT_ERR_STATUS_FAIL;

}

INT32 bluetooth_get_local_dev_name(CHAR *name)
{
    FUNC_ENTRY;
    BT_CHECK_POINTER(BT_DEBUG_COMM, name);
    strncpy(name, g_localProperty.name, strlen(g_localProperty.name));

    return BT_SUCCESS;
} /*bt_set_name*/

//app use this API
EXPORT_SYMBOL INT32 bluetooth_get_local_dev_info( BT_GAP_LOCAL_PROPERTIES_RESULT* ps_dev_info)
{
    FUNC_ENTRY;
    INT32 i4_ret;
    if (ps_dev_info == NULL)
    {
        BT_DBG_NORMAL(BT_DEBUG_COMM, "please specify a ps_dev_info");

        return BT_ERR_STATUS_NULL_POINTER;
    }
    if ((0 < strlen(g_localProperty.bdAddr)) && (0 < strlen(g_localProperty.name)))
    {
        memcpy(ps_dev_info, &g_localProperty, sizeof(BT_GAP_LOCAL_PROPERTIES_RESULT));
    }
    else
    {
        //x_thread_delay(1000);
        i4_ret = bluetooth_update_local_dev_info();
        if (0 == i4_ret)
        {
            memcpy(ps_dev_info, &g_localProperty, sizeof(BT_GAP_LOCAL_PROPERTIES_RESULT));
        }
        else
        {
            BT_DBG_ERROR(BT_DEBUG_COMM, "can't get local device info now");
            return BT_ERR_STATUS_FAIL;
        }
    }
    BT_DBG_INFO(BT_DEBUG_COMM, "g_localProperty.bdaddr %s", g_localProperty.bdAddr);
    BT_DBG_INFO(BT_DEBUG_COMM, "g_localProperty.name %s", g_localProperty.name);
    BT_DBG_INFO(BT_DEBUG_COMM, "bdaddr %s", ps_dev_info->bdAddr);
    BT_DBG_INFO(BT_DEBUG_COMM, "name %s", ps_dev_info->name);

    return BT_SUCCESS;
}

/* this API get local device info at real time */
EXPORT_SYMBOL INT32 bluetooth_update_local_dev_info(VOID)
{
    UINT32 ui4_loop = 10;

    bluetooth_get_properties();
    while (!get_bt_get_dev_info_done() && (0 < ui4_loop))
    {
        BT_DBG_NORMAL(BT_DEBUG_COMM, "ui4_loop:%ld, bt_get_dev_info_done:%d!",
                      (long)ui4_loop,
                      get_bt_get_dev_info_done()
                     );
        /*x_thread_delay(500);*/
        usleep(500*1000);
        ui4_loop--;
    }
    if (!get_bt_get_dev_info_done())
    {
        BT_DBG_ERROR(BT_DEBUG_COMM, "can't get device info");
        return BT_ERR_STATUS_FAIL;
    }
    return BT_SUCCESS;
}


INT32 _bt_update_dev_history(BT_TARGET_DEV_INFO *pt_dev_info, BT_TARGET_DEV_LIST *pt_paired_dev_history)
{
    BT_DBG_NORMAL(BT_DEBUG_COMM, "Enter %s", __FUNCTION__);
    if (NULL == pt_dev_info)
    {
        BT_DBG_ERROR(BT_DEBUG_COMM, "null pointer of pt_dev_info!");
        return -1;
    }
    bluetooth_setHistory(pt_dev_info, pt_paired_dev_history);
    BtAppCbk(BT_GET_DEV_HISTORY_DONE, pt_dev_info->bdAddr);

    BT_DBG_NORMAL(BT_DEBUG_COMM, "Exit %s", __FUNCTION__);
    return 0;
}



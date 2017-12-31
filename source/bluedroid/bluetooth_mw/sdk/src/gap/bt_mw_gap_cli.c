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
/* FILE NAME:  bt_mw_gap_cli.c
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

#include <pthread.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include "bt_mw_common.h"
#include "bluetooth.h"

#if ENABLE_MTK_TV_PLATFORM
//#include "inc/x_common.h"
//#include "dbg/x_dbg.h"
#endif
//#include "x_cli.h"

#include "bluetooth_sync.h"
#include "c_bt_mw_gap.h"
#include "bt_mw_gap.h"
#include "bt_mw_gap_cli.h"
#include "bt_mw_devices_info.h"


INT32 _bluetooth_inquiry_one_cli (INT32 argc, const CHAR **argv)
{
    FUNC_ENTRY;
    CHAR ps_target_mac[18];
    UINT32 ui4_inqDuration = 10;
    if (2 < argc)
    {
        memset(ps_target_mac, 0, sizeof(ps_target_mac));
        if (17 > strlen(argv[1]))
        {
            BT_DBG_NORMAL(BT_DEBUG_COMM, "mac length should be 17");
            return BT_ERR_STATUS_PARM_INVALID;
        }
        strncpy(ps_target_mac,argv[1],sizeof(ps_target_mac)-1);
        ui4_inqDuration = atol(argv[2]);
        BT_DBG_NORMAL(BT_DEBUG_COMM, "inquiry %s:%ld s", ps_target_mac, (unsigned long)ui4_inqDuration);
    }
    else if (2 == argc)
    {
        memset(ps_target_mac, 0, sizeof(ps_target_mac));
        if (17 > strlen(argv[1]))
        {
            BT_DBG_NORMAL(BT_DEBUG_COMM, "mac length should be 17");
            return BT_ERR_STATUS_PARM_INVALID;
        }
        strncpy(ps_target_mac,argv[1],sizeof(ps_target_mac)-1);
        BT_DBG_NORMAL(BT_DEBUG_COMM, "inquiry %s:%ld s", ps_target_mac, (unsigned long)ui4_inqDuration);
    }
    else
    {
        BT_DBG_NORMAL(BT_DEBUG_COMM, "should input the parameter");
        return BT_ERR_STATUS_INVALID_PARM_NUMS;
    }

    c_btm_inquiry_one(ps_target_mac, ui4_inqDuration);
    return BT_SUCCESS;

}

INT32 _bluetooth_set_name_cli (INT32 argc, const CHAR **argv)
{
    FUNC_ENTRY;
    INT32 i4_ret;
    CHAR *ps_name;
    if (argc != 2 )
    {
        BT_DBG_NORMAL(BT_DEBUG_COMM, "please attach name after commond just append one string");
    }
    if (NULL == argv[1])
    {
        BT_DBG_NORMAL(BT_DEBUG_COMM, "please attach name after commond just append one string");
    }
    ps_name = strdup(argv[1]);
    if (NULL == ps_name)
    {
        BT_DBG_NORMAL(BT_DEBUG_COMM, "strdup failed");
        return BT_ERR_STATUS_FAIL;
    }
    BT_DBG_NORMAL(BT_DEBUG_COMM, "name is: %s", ps_name);
    i4_ret = c_btm_set_local_name(ps_name);
    if (BT_SUCCESS == i4_ret)
    {
        BT_DBG_NORMAL(BT_DEBUG_COMM, "set name ok!");
    }

    return BT_SUCCESS;
}

/*-----------------------------------------------------------------------------
 * Name: _bluetooth_scan_cli
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
INT32 _bluetooth_scan_cli (INT32 argc, const CHAR **argv)
{
    FUNC_ENTRY;
    //BT_GAP_SCAN_RESULT_S ps_scan_rst;
    return bluetooth_scan();

    FUNC_EXIT;
}

INT32 _bluetooth_stop_scan_cli (INT32 argc, const CHAR **argv)
{
    FUNC_ENTRY;
    //BT_GAP_SCAN_RESULT_S ps_scan_rst;
    return bluetooth_stop_scan();

    FUNC_EXIT;
}


/*-----------------------------------------------------------------------------
 * Name: _bluetooth_set_connectable_and_discoverable_cli
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
INT32 _bluetooth_set_connectable_and_discoverable_cli(INT32 argc, const CHAR **argv)
{
    FUNC_ENTRY;

    if (3 == argc)
    {
        if (0 == strcmp("1" , argv[1]))
        {
            if (0 == strcmp("1" , argv[2]))
            {
                return c_btm_set_connectable_and_discoverable(TRUE, TRUE);
            }
            else if (0 == strcmp("0" , argv[2]))
            {
                return c_btm_set_connectable_and_discoverable(TRUE, FALSE);
            }
            else
            {
                BT_DBG_ERROR(BT_DEBUG_GAP,"input error, 0 means off, 1 means on");
                return -1;
            }
        }
        else if (0 == strcmp("0" , argv[1]))
        {
            if (0 == strcmp("1" , argv[2]))
            {
                return c_btm_set_connectable_and_discoverable(FALSE, TRUE);
            }
            else if (0 == strcmp("0" , argv[2]))
            {
                return c_btm_set_connectable_and_discoverable(FALSE, FALSE);
            }
            else
            {
                BT_DBG_ERROR(BT_DEBUG_GAP,"input error, 0 means off, 1 means on");
                return BT_ERR_STATUS_PARM_INVALID;
            }
        }
    }
    else
    {
        BT_DBG_ERROR(BT_DEBUG_GAP,"0 means off, other integer:on");
        return BT_ERR_STATUS_INVALID_PARM_NUMS;
    }
    FUNC_EXIT;
    return BT_SUCCESS;
}

INT32 _bluetooth_get_local_dev_info_cli(INT32 argc, const CHAR **argv)
{
    BT_GAP_LOCAL_PROPERTIES_RESULT ps_dev_info;
    INT32 i4_ret;
    CHAR info_type[8];
    if (argc != 2 )
    {
        BT_DBG_NORMAL(BT_DEBUG_COMM, "please input info [local|update]");
    }
    if (NULL == argv[1])
    {
        BT_DBG_NORMAL(BT_DEBUG_COMM, "null pointer of [local|update]");
        BT_DBG_NORMAL(BT_DEBUG_COMM, "please input info [local|update]");
    }
    strncpy(info_type, argv[1], sizeof(info_type));
    if (0 == strcmp(info_type, "local"))
    {
        i4_ret = bluetooth_get_local_dev_info(&ps_dev_info);
    }
    else if (0 == strcmp(info_type, "update"))
    {
        i4_ret = bluetooth_update_local_dev_info();
        bluetooth_get_local_dev_info(&ps_dev_info);
    }
    else
    {
        BT_DBG_NORMAL(BT_DEBUG_COMM, "use default type: update");
        i4_ret = bluetooth_update_local_dev_info();
        bluetooth_get_local_dev_info(&ps_dev_info);
    }

    if (BT_SUCCESS == i4_ret)
    {
        BT_DBG_NORMAL(BT_DEBUG_COMM, "get local device info ok!");
        BT_DBG_NORMAL(BT_DEBUG_COMM, "ps_dev_info.bdAddr:%s", ps_dev_info.bdAddr);
        BT_DBG_NORMAL(BT_DEBUG_COMM, "ps_dev_info.name:%s", ps_dev_info.name);
        BT_DBG_NORMAL(BT_DEBUG_COMM, "ps_dev_info.powered:%s", ps_dev_info.powered?"on":"off");
        BT_DBG_NORMAL(BT_DEBUG_COMM, "ps_dev_info.scan_mode:%ld", (unsigned long)ps_dev_info.scan_mode);
        BT_DBG_NORMAL(BT_DEBUG_COMM, "ps_dev_info.disctimeout:%ld", (unsigned long)ps_dev_info.disctimeout);
    }
    else
    {
        BT_DBG_NORMAL(BT_DEBUG_COMM, "get local device info failed!");
    }

    return BT_SUCCESS;
}

INT32 _bluetooth_get_rssi_cli (INT32 argc, const CHAR **argv)
{
    FUNC_ENTRY;
    CHAR ps_target_mac[18];
    INT16 rssi_value;

    if (2 == argc)
    {
        memset(ps_target_mac, 0, sizeof(ps_target_mac));
        if (17 > strlen(argv[1]))
        {
            BT_DBG_NORMAL(BT_DEBUG_COMM, "mac length should be 17");
            return BT_ERR_STATUS_PARM_INVALID;
        }
        strncpy(ps_target_mac,argv[1],sizeof(ps_target_mac)-1);
    }
    else
    {
        BT_DBG_NORMAL(BT_DEBUG_COMM, "please input get_rssi [MAC address]");
        return BT_ERR_STATUS_INVALID_PARM_NUMS;
    }

    c_btm_get_rssi(ps_target_mac, &rssi_value);
    return BT_SUCCESS;
}

INT32 _bluetooth_stop_inquiry_cli (INT32 argc, const CHAR **argv)
{
    FUNC_ENTRY;

    return c_btm_stop_inquiry_scan();
    FUNC_EXIT;
}

INT32 _bluetooth_pair_cli (INT32 argc, const CHAR **argv)
{
    CHAR *ps_name;
    if (argc != 2 )
    {
        BT_DBG_NORMAL(BT_DEBUG_COMM, "please attach MAC after commond just append one string");
    }
    if (NULL == argv[1])
    {
        BT_DBG_NORMAL(BT_DEBUG_COMM, "please attach MAC after commond just append one string");
    }
    ps_name = strdup(argv[1]);
    if (NULL == ps_name)
    {
        BT_DBG_NORMAL(BT_DEBUG_COMM, "strdup failed");
        return BT_ERR_STATUS_PARM_INVALID;
    }
    return c_btm_pair(ps_name, 1);
}

INT32 _bluetooth_remove_paired_dev_cli(INT32 argc, const CHAR **argv)
{
    CHAR *ps_address;
    if (argc != 2 )
    {
        BT_DBG_NORMAL(BT_DEBUG_COMM, "please attach MAC after commond just append one string");
    }
    if (NULL == argv[1])
    {
        BT_DBG_NORMAL(BT_DEBUG_COMM, "please attach MAC after commond just append one string");
    }

    ps_address = strdup(argv[1]);
    if (NULL == ps_address)
    {
        BT_DBG_NORMAL(BT_DEBUG_COMM, "strdup failed");
        return BT_ERR_STATUS_PARM_INVALID;
    }
    if (strncmp(ps_address, "all", 3))
    {
        return 0;//c_btm_paired_dev_erase_all();
    }
    else
    {
        return c_btm_paired_dev_erase(ps_address);
    }
    free(ps_address);
}

INT32 _bluetooth_set_virtual_sniffer_cli(INT32 argc, const CHAR **argv)
{
    INT32 flag;
    if (argc < 1)
    {
        BT_DBG_NORMAL(BT_DEBUG_COMM, "A parameter is required!");
        return BT_ERR_STATUS_INVALID_PARM_NUMS;
    }
    flag = atoi(argv[1]);

    bluetooth_set_virtual_sniffer(flag);
    return BT_SUCCESS;
}
/*
INT32 _bluetooth_gap_start_sniff_mode_handler(INT32 argc, const CHAR **argv)
{
    FUNC_ENTRY;
    CHAR bt_addr[18];
    bt_sniff_info info = {0};

    if (argc <=1 )
    {
        BT_DBG_NORMAL(BT_DEBUG_GAP,"Usage : To start Sniff Mode : start_sniff_mode ([addr] 1 [Max Interval][Min Interval][Sniff Attempts][Sniff Timeout])");
        BT_DBG_NORMAL(BT_DEBUG_GAP,"Usage : To stop Sniff Mode : start_sniff_mode [addr] 0");
        return BT_ERR_STATUS_INVALID_PARM_NUMS;
    }
    else if ((argc>=2) && atoi(argv[2])==0)
    {
        strncpy(bt_addr, argv[1], 17);
        bt_addr[17] = '\0';
        bluetooth_gap_start_sniff_mode(FALSE,bt_addr,info.maxInterval,info.minInterval,info.attempt,info.timeout);
    }
    else if (argc >=6 && atoi(argv[2])==1)
    {
        strncpy(bt_addr, argv[1], 17);
        bt_addr[17] = '\0';
        info.maxInterval  =  strtoul(argv[3], NULL, 16);
        info.minInterval   =  strtoul(argv[4], NULL, 16);
        info.attempt        =   strtoul(argv[5], NULL, 16);
        info.timeout        =   strtoul(argv[6], NULL, 16);
        bluetooth_gap_start_sniff_mode(TRUE,bt_addr,info.maxInterval,info.minInterval,info.attempt,info.timeout);
    }
    return BT_SUCCESS;

}*/

extern BOOL hci_res_flag;
INT32 _bluetooth_send_hci(INT32 argc, const CHAR **argv)
{
    FUNC_ENTRY;
    CHAR *hci_cmd;
    if (argc < 2)
        return BT_ERR_STATUS_INVALID_PARM_NUMS;
    hci_cmd = (CHAR *)argv[1];
    hci_res_flag = TRUE;
    c_btm_send_hci(hci_cmd);
    FUNC_EXIT;
    return BT_SUCCESS;
}

INT32 _bluetooth_gap_pts_enable_cli (INT32 argc, const CHAR **argv)
{
    UINT8 i4_enable;

    if (2 == argc)
    {
        i4_enable = atoi(argv[1]);
        BT_DBG_NORMAL(BT_DEBUG_GAP,"GAP ENABLE:%ld",(unsigned long)i4_enable);

        //bluetooth_gap_pts_enable(i4_enable);
    }
    else
    {
        BT_DBG_NORMAL(BT_DEBUG_COMM, "input error");
        BT_DBG_NORMAL(BT_DEBUG_COMM, "please input gap_pts_enable [integer]");
    }
    return BT_SUCCESS;
}

/*
set_afh 7FFFFFFFFFFFFFFFFFFF : do not maskany BT channel
set_afh 7FFFFFFFFFFFFFFFFFFE : only mask BT ch0
set_afh 7FFFFFFFFFFFFFFFFFFD : only mask BT ch1
set_afh 3FFFFFFFFFFFFFFFFFFF : only mask BT ch79
set_afh 00000000000000000000 : mask all the BT channels
*/
INT32 _bluetooth_set_afh_cli (INT32 argc, const CHAR **argv)
{
    if (0) // in MSA3G this CLI should not used, but will apply in MSA4G
    {
        CHAR mask[20] = {'\0'};
        UINT8 output[sizeof(mask)/2] = {0};
        if (argc!=2)
        {
            printf("the parameter is necessary!");
            return -1;
        }
        if (strlen(argv[1])!=20)
        {
            printf("input error %d",(int)strlen(argv[1]));
            return -1;
        }
        strncpy(mask,argv[1],sizeof(mask));
        printf("you input is %s",mask);
        CHAR buf[3] = {0};
        unsigned long int ret = 0;
        UINT8 i = 0;
        UINT8 j = 0;
        for (i=0;i<sizeof(mask) && j<sizeof(output);i+=2)
        {
            strncpy(buf,mask+i,2);
            //printf("buf:%s	-> ",buf);
            ret = strtoul(buf,NULL,16);
            //printf("ret:%ld",ret);
            output[j++] = (unsigned char)ret;
        }

        printf("output:");
        for (j=0;j<sizeof(output);j++)
            printf("%x",output[j]);
        printf("");

        bluetooth_set_afh(output);
    }
    return 0;
}

#if defined(MTK_LINUX_GAP) && (MTK_LINUX_GAP == TRUE)
INT32 _bluetooth_config_clear(INT32 argc, const CHAR **argv)
{
    FUNC_ENTRY;
    bluetooth_config_clear();
    FUNC_EXIT;
    return BT_SUCCESS;
}

INT32 _bluetooth_dump(INT32 argc, const CHAR **argv)
{
    FUNC_ENTRY;
    char mode[10];
    if (2 == argc)
    {
        strncpy(mode,argv[1],strlen(argv[1]));
        bluetooth_dump(mode);
    }
    else
    {
        BT_DBG_NORMAL(BT_DEBUG_COMM, "input error");
    }
    FUNC_EXIT;
    return BT_SUCCESS;
}

INT32 _bluetooth_interop_database_add(INT32 argc, const CHAR **argv)
{
    FUNC_ENTRY;
    uint16_t feature;
    bt_bdaddr_t *remote_bd_addr = NULL;
    uint16_t len;
    if (4 == argc)
    {
        feature = atoi(argv[1]);
        strncpy((char *)remote_bd_addr,argv[2],sizeof(bt_bdaddr_t));
        len = atoi(argv[3]);
        bluetooth_database_add(feature,remote_bd_addr,len);
    }
    else
    {
        BT_DBG_NORMAL(BT_DEBUG_COMM, "input error");
    }
    FUNC_EXIT;
    return BT_SUCCESS;
}

INT32 _bluetooth_interop_database_clear(INT32 argc, const CHAR **argv)
{
    FUNC_ENTRY;
    bluetooth_database_clear();
    FUNC_EXIT;
    return BT_SUCCESS;
}

INT32 _bluetooth_ble_pair(INT32 argc, const CHAR **argv)
{
    FUNC_ENTRY;
    char  remote_bd_addr[6];
    if (2 == argc)
    {
        strncpy(remote_bd_addr,argv[1],sizeof(bt_bdaddr_t));
        bluetooth_ble_pair(remote_bd_addr);
    }
    else
    {
        BT_DBG_NORMAL(BT_DEBUG_COMM, "input error");
    }
    FUNC_EXIT;
    return BT_SUCCESS;
}
#endif



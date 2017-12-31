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

/*-----------------------------------------------------------------------------
                            include files
-----------------------------------------------------------------------------*/
#include <stdio.h>

#include "mtk_bt_service_hidh.h"
#include "c_bt_mw_hidh.h"

#define BT_RC_LOG(_stmt...) \
        do{ \
            if(1){    \
                printf("[HIDH IPC Server]Func:%s Line:%d--->: ", __FUNCTION__, __LINE__);   \
                printf(_stmt); \
                printf("\n"); \
            }        \
        }   \
        while(0)


INT32 x_mtkapi_hidh_init(VOID)
{
    BT_RC_LOG("[.c][%s] begin\n", __FUNCTION__);
    INT32 i4_ret = 0;
    i4_ret = c_bt_hid_init();
    if (i4_ret != 0)
    {
        BT_RC_LOG("[.c][%s]c_bt_hid_init fail\n", __FUNCTION__);
    }
    else
    {
        BT_RC_LOG("[.c][%s]c_bt_hid_init success\n", __FUNCTION__);
    }

    return i4_ret;
}

INT32 x_mtkapi_hidh_deinit(VOID)
{
    BT_RC_LOG("[.c][%s] begin\n", __FUNCTION__);
    INT32 i4_ret = 0;
    i4_ret = c_bt_hid_deinit();
    if (i4_ret != 0)
    {
        BT_RC_LOG("[.c][%s]c_bt_hid_deinit fail\n", __FUNCTION__);
    }
    else
    {
        BT_RC_LOG("[.c][%s]c_bt_hid_deinit success\n", __FUNCTION__);
    }

    return i4_ret;
}

INT32 x_mtkapi_hidh_connect(CHAR *pbt_addr)
{
    BT_RC_LOG("[.c][%s] begin\n", __FUNCTION__);
    INT32 i4_ret = 0;
    i4_ret = c_bt_hid_connect(pbt_addr);
    if (i4_ret != 0)
    {
        BT_RC_LOG("[.c][%s]c_bt_hid_connect fail\n", __FUNCTION__);
    }
    else
    {
        BT_RC_LOG("[.c][%s]c_bt_hid_connect success\n", __FUNCTION__);
    }

    return i4_ret;
}

INT32 x_mtkapi_hidh_disconnect(CHAR *pbt_addr)
{
    BT_RC_LOG("[.c][%s] begin\n", __FUNCTION__);
    INT32 i4_ret = 0;
    i4_ret = c_bt_hid_disconnect(pbt_addr);
    if (i4_ret != 0)
    {
        BT_RC_LOG("[.c][%s]c_bt_hid_disconnect fail\n", __FUNCTION__);
    }
    else
    {
        BT_RC_LOG("[.c][%s]c_bt_hid_disconnect success\n", __FUNCTION__);
    }

    return i4_ret;
}

INT32 x_mtkapi_hidh_set_output_report(CHAR *pbt_addr, CHAR *preport_data)
{
    BT_RC_LOG("[.c][%s] begin\n", __FUNCTION__);
    INT32 i4_ret = 0;
    i4_ret = c_bt_hid_set_output_report(pbt_addr, preport_data);
    if (i4_ret != 0)
    {
        BT_RC_LOG("[.c][%s]c_bt_hid_set_output_report fail\n", __FUNCTION__);
    }
    else
    {
        BT_RC_LOG("[.c][%s]c_bt_hid_set_output_report success\n", __FUNCTION__);
    }

    return i4_ret;
}

INT32 x_mtkapi_hidh_get_input_report(CHAR *pbt_addr, UINT8 reportId, INT32 bufferSize)
{
    BT_RC_LOG("[.c][%s] begin\n", __FUNCTION__);
    INT32 i4_ret = 0;
    i4_ret = c_bt_hid_get_input_report(pbt_addr, reportId, bufferSize);
    if (i4_ret != 0)
    {
        BT_RC_LOG("[.c][%s]c_bt_hid_get_input_report fail\n", __FUNCTION__);
    }
    else
    {
        BT_RC_LOG("[.c][%s]c_bt_hid_get_input_report success\n", __FUNCTION__);
    }

    return i4_ret;
}

INT32 x_mtkapi_hidh_get_output_report(CHAR *pbt_addr, UINT8 reportId, INT32 bufferSize)
{
    BT_RC_LOG("[.c][%s] begin\n", __FUNCTION__);
    INT32 i4_ret = 0;
    i4_ret = c_bt_hid_get_output_report(pbt_addr, reportId, bufferSize);
    if (i4_ret != 0)
    {
        BT_RC_LOG("[.c][%s]c_bt_hid_get_output_report fail\n", __FUNCTION__);
    }
    else
    {
        BT_RC_LOG("[.c][%s]c_bt_hid_get_output_report success\n", __FUNCTION__);
    }

    return i4_ret;
}

INT32 x_mtkapi_hidh_set_input_report(CHAR *pbt_addr, CHAR *preport_data)
{
    BT_RC_LOG("[.c][%s] begin\n", __FUNCTION__);
    INT32 i4_ret = 0;
    i4_ret = c_bt_hid_set_input_report(pbt_addr, preport_data);
    if (i4_ret != 0)
    {
        BT_RC_LOG("[.c][%s]c_bt_hid_set_input_report fail\n", __FUNCTION__);
    }
    else
    {
        BT_RC_LOG("[.c][%s]c_bt_hid_set_input_report success\n", __FUNCTION__);
    }

    return i4_ret;
}

INT32 x_mtkapi_hidh_get_feature_report(CHAR *pbt_addr, UINT8 reportId, INT32 bufferSize)
{
    BT_RC_LOG("[.c][%s] begin\n", __FUNCTION__);
    INT32 i4_ret = 0;
    i4_ret = c_bt_hid_get_feature_report(pbt_addr, reportId, bufferSize);
    if (i4_ret != 0)
    {
        BT_RC_LOG("[.c][%s]c_bt_hid_get_feature_report fail\n", __FUNCTION__);
    }
    else
    {
        BT_RC_LOG("[.c][%s]c_bt_hid_get_feature_report success\n", __FUNCTION__);
    }

    return i4_ret;
}

INT32 x_mtkapi_hidh_set_feature_report(CHAR *pbt_addr, CHAR *preport_data)
{
    BT_RC_LOG("[.c][%s] begin\n", __FUNCTION__);
    INT32 i4_ret = 0;
    i4_ret = c_bt_hid_set_feature_report(pbt_addr, preport_data);
    if (i4_ret != 0)
    {
        BT_RC_LOG("[.c][%s]c_bt_hid_set_feature_report fail\n", __FUNCTION__);
    }
    else
    {
        BT_RC_LOG("[.c][%s]c_bt_hid_set_feature_report success\n", __FUNCTION__);
    }

    return i4_ret;
}

INT32 x_mtkapi_hidh_get_protocol(CHAR *pbt_addr)
{
    BT_RC_LOG("[.c][%s] begin\n", __FUNCTION__);
    INT32 i4_ret = 0;
    i4_ret = c_bt_hid_get_protocol(pbt_addr);
    if (i4_ret != 0)
    {
        BT_RC_LOG("[.c][%s]c_bt_hid_get_protocol fail\n", __FUNCTION__);
    }
    else
    {
        BT_RC_LOG("[.c][%s]c_bt_hid_get_protocol success\n", __FUNCTION__);
    }

    return i4_ret;
}

INT32 x_mtkapi_hidh_set_protocol(CHAR *pbt_addr, UINT8 protocol_mode)
{
    BT_RC_LOG("[.c][%s] begin\n", __FUNCTION__);
    INT32 i4_ret = 0;
    i4_ret = c_bt_hid_set_protocol(pbt_addr, protocol_mode);
    if (i4_ret != 0)
    {
        BT_RC_LOG("[.c][%s]c_bt_hid_set_protocol fail\n", __FUNCTION__);
    }
    else
    {
        BT_RC_LOG("[.c][%s]c_bt_hid_set_protocol success\n", __FUNCTION__);
    }

    return i4_ret;
}

INT32 x_mtkapi_hidh_send_control(CHAR *pbt_addr, UINT8 pcontrol_mode)
{
    INT32 i4_ret = 0;
    return i4_ret;
}

INT32 x_mtkapi_bluetooth_hidh_set_output_report(CHAR *pbt_addr, CHAR *preport_data)
{
    BT_RC_LOG("[.c][%s] begin\n", __FUNCTION__);
    INT32 i4_ret = 0;
    i4_ret = c_bt_hid_set_output_report(pbt_addr, preport_data);
    if (i4_ret != 0)
    {
        BT_RC_LOG("[.c][%s]c_bt_hid_set_output_report fail\n", __FUNCTION__);
    }
    else
    {
        BT_RC_LOG("[.c][%s]c_bt_hid_set_output_report success\n", __FUNCTION__);
    }

    return i4_ret;
}

INT32 x_mtkapi_get_paired_hidh_dev_list(BT_HID_TARGET_DEV_LIST *pt_device_list, BT_HID_STATUS_STRUCT_LIST *pt_hid_status_list)
{
    BT_RC_LOG("[.c][%s] begin\n", __FUNCTION__);
    INT32 i4_ret = 0;
    i4_ret = c_btm_get_paired_hid_dev_list(pt_device_list, pt_hid_status_list);
    if (i4_ret != 0)
    {
        BT_RC_LOG("[.c][%s]c_btm_get_paired_hid_dev_list fail\n", __FUNCTION__);
    }
    else
    {
        BT_RC_LOG("[.c][%s]c_btm_get_paired_hid_dev_list success\n", __FUNCTION__);
    }

    return i4_ret;
}

INT32 x_mtkapi_del_paired_hidh_dev_one(BT_TARGET_DEV_INFO *pt_device_info)
{
    BT_RC_LOG("[.c][%s] begin\n", __FUNCTION__);
    INT32 i4_ret = 0;
    i4_ret = c_btm_del_paired_hid_dev_one(pt_device_info);
    if (i4_ret != 0)
    {
        BT_RC_LOG("[.c][%s]c_btm_del_paired_hid_dev_one fail\n", __FUNCTION__);
    }
    else
    {
        BT_RC_LOG("[.c][%s]c_btm_del_paired_hid_dev_one success\n", __FUNCTION__);
    }

    return i4_ret;
}

INT32 x_mtkapi_del_paired_hidh_dev_all(VOID)
{
    BT_RC_LOG("[.c][%s] begin\n", __FUNCTION__);
    INT32 i4_ret = 0;
    i4_ret = c_btm_del_paired_hid_dev_all();
    if (i4_ret != 0)
    {
        BT_RC_LOG("[.c][%s]c_btm_del_paired_hid_dev_all fail\n", __FUNCTION__);
    }
    else
    {
        BT_RC_LOG("[.c][%s]c_btm_del_paired_hid_dev_all success\n", __FUNCTION__);
    }

    return i4_ret;
}

INT32 x_mtkapi_connect_hidh_sources(CHAR *addr)
{
    BT_RC_LOG("[.c][%s] begin\n", __FUNCTION__);
    INT32 i4_ret = 0;
    i4_ret = c_btm_connect_hid_sources(addr);
    if (i4_ret != 0)
    {
        BT_RC_LOG("[.c][%s]c_btm_connect_hid_sources fail\n", __FUNCTION__);
    }
    else
    {
        BT_RC_LOG("[.c][%s]c_btm_connect_hid_sources success\n", __FUNCTION__);
    }

    return i4_ret;
}

INT32 x_mtkapi_disconnect_hidh_sources(CHAR *addr)
{
    BT_RC_LOG("[.c][%s] begin\n", __FUNCTION__);
    INT32 i4_ret = 0;
    i4_ret = c_btm_disconnect_hid_sources(addr);
    if (i4_ret != 0)
    {
        BT_RC_LOG("[.c][%s]c_btm_disconnect_hid_sources fail\n", __FUNCTION__);
    }
    else
    {
        BT_RC_LOG("[.c][%s]c_btm_disconnect_hid_sources success\n", __FUNCTION__);
    }

    return i4_ret;
}

INT32 x_mtkapi_hidh_auto_disconnection(VOID)
{
    BT_RC_LOG("[.c][%s] begin\n", __FUNCTION__);
    INT32 i4_ret = 0;
    i4_ret = c_btm_hid_auto_disconnection();
    if (i4_ret != 0)
    {
        BT_RC_LOG("[.c][%s]c_btm_hid_auto_disconnection fail\n", __FUNCTION__);
    }
    else
    {
        BT_RC_LOG("[.c][%s]c_btm_hid_auto_disconnection success\n", __FUNCTION__);
    }

    return i4_ret;
}


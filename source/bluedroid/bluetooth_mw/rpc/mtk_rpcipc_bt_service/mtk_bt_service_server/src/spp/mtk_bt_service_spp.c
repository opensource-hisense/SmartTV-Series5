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

#include "mtk_bt_service_spp.h"
#include "c_bt_mw_spp.h"

#define BT_RC_LOG(_stmt...) \
        do{ \
            if(0){    \
                printf("Func:%s Line:%d--->: ", __FUNCTION__, __LINE__);   \
                printf(_stmt); \
                printf("\n"); \
            }        \
        }   \
        while(0)


INT32 x_mtkapi_spp_connect(CHAR* addr, CHAR* uuid)
{
    INT32 i4_ret = 0;
    i4_ret = c_btm_spp_connect(addr, uuid);
    if (i4_ret != 0)
    {
        BT_RC_LOG("c_btm_spp_connect fail\n");
    }
    else
    {
        BT_RC_LOG("c_btm_spp_connect success\n");
    }
    return i4_ret;
}

INT32 x_mtkapi_spp_disconnect(CHAR *addr, CHAR *uuid)
{
    INT32 i4_ret = 0;
    i4_ret = c_btm_spp_disconnect(addr, uuid);
    if (i4_ret != 0)
    {
        BT_RC_LOG("c_btm_spp_disconnect fail\n");
    }
    else
    {
        BT_RC_LOG("c_btm_spp_disconnect success\n");
    }
    return i4_ret;
}

INT32 x_mtkapi_spp_send_data(CHAR* bd_addr, CHAR *uuid, CHAR* str, INT32 len)
{
    INT32 i4_ret = 0;
    i4_ret = c_btm_spp_send_data(bd_addr, uuid, str, len);
    if (i4_ret != 0)
    {
        BT_RC_LOG("c_btm_spp_send_data fail\n");
    }
    else
    {
        BT_RC_LOG("c_btm_spp_send_data success\n");
    }
    return i4_ret;
}

INT32 x_mtkapi_spp_enable_devb(CHAR *servername, CHAR* uuid)
{
    INT32 i4_ret = 0;
    i4_ret = c_btm_spp_enable_devb(servername, uuid);
    if (i4_ret != 0)
    {
        BT_RC_LOG("c_btm_spp_enable_devb fail\n");
    }
    else
    {
        BT_RC_LOG("c_btm_spp_enable_devb success\n");
    }
    return i4_ret;
}

INT32 x_mtkapi_spp_disable_devb(CHAR *uuid)
{
    INT32 i4_ret = 0;
    i4_ret = c_btm_spp_disable_devb(uuid);
    if (i4_ret != 0)
    {
        BT_RC_LOG("c_btm_spp_disable_devb fail\n");
    }
    else
    {
        BT_RC_LOG("c_btm_spp_disable_devb success\n");
    }
    return i4_ret;
}



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

#include "mtk_bt_service_spp_wrapper.h"
#include "client_common.h"


#define BT_RW_LOG(_stmt...) \
        do{ \
            if(0){    \
                printf("[Client]Func:%s Line:%d--->: ", __FUNCTION__, __LINE__);   \
                printf(_stmt); \
                printf("\n"); \
            }        \
        }   \
        while(0)

EXPORT_SYMBOL INT32 a_mtkapi_spp_connect(CHAR* addr, CHAR* uuid)
{
    RPC_CLIENT_DECL(2, INT32);
    RPC_CLIENT_ARG_INP(ARG_TYPE_REF_STR, addr);
    RPC_CLIENT_ARG_INP(ARG_TYPE_REF_STR, uuid);
    RPC_BT_SERVICE_CLIENT_DO_OP("x_mtkapi_spp_connect");
    RPC_CLIENT_RETURN(ARG_TYPE_INT32, -1);
}

EXPORT_SYMBOL INT32 a_mtkapi_spp_disconnect(CHAR *addr, CHAR *uuid)
{
    RPC_CLIENT_DECL(2, INT32);
    RPC_CLIENT_ARG_INP(ARG_TYPE_REF_STR, addr);
    RPC_CLIENT_ARG_INP(ARG_TYPE_REF_STR, uuid);
    RPC_BT_SERVICE_CLIENT_DO_OP("x_mtkapi_spp_disconnect");
    RPC_CLIENT_RETURN(ARG_TYPE_INT32, -1);
}

EXPORT_SYMBOL INT32 a_mtkapi_spp_send_data(CHAR* addr, CHAR *uuid, CHAR* str, INT32 len)
{
    RPC_CLIENT_DECL(4, INT32);
    RPC_CLIENT_ARG_INP(ARG_TYPE_REF_STR, addr);
    RPC_CLIENT_ARG_INP(ARG_TYPE_REF_STR, uuid);
    RPC_CLIENT_ARG_INP(ARG_TYPE_REF_STR, str);
    RPC_CLIENT_ARG_INP(ARG_TYPE_INT32, len);
    RPC_BT_SERVICE_CLIENT_DO_OP("x_mtkapi_spp_send_data");
    RPC_CLIENT_RETURN(ARG_TYPE_INT32, -1);
}

EXPORT_SYMBOL INT32 a_mtkapi_spp_enable_devb(CHAR *servername, CHAR* uuid)
{
    RPC_CLIENT_DECL(2, INT32);
    RPC_CLIENT_ARG_INP(ARG_TYPE_REF_STR, servername);
    RPC_CLIENT_ARG_INP(ARG_TYPE_REF_STR, uuid);
    RPC_BT_SERVICE_CLIENT_DO_OP("x_mtkapi_spp_enable_devb");
    RPC_CLIENT_RETURN(ARG_TYPE_INT32, -1);
}

EXPORT_SYMBOL INT32 a_mtkapi_spp_disable_devb(CHAR *uuid)
{
    RPC_CLIENT_DECL(1, INT32);
    RPC_CLIENT_ARG_INP(ARG_TYPE_REF_STR, uuid);
    RPC_BT_SERVICE_CLIENT_DO_OP("x_mtkapi_spp_disable_devb");
    RPC_CLIENT_RETURN(ARG_TYPE_INT32, -1);
}

INT32 c_rpc_reg_mtk_bt_service_spp_cb_hndlrs(VOID)
{
    return RPCR_OK;
}



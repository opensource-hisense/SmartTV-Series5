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

#include "mtk_bt_service_avrcp_wrapper.h"
#include "mtk_bt_service_avrcp_ipcrpc_struct.h"
#include "client_common.h"

#define BT_AVRCP_LOG(_stmt...) \
        do{ \
            if(0){    \
                printf("[AVRCP]Func:%s Line:%d--->: ", __FUNCTION__, __LINE__);   \
                printf(_stmt); \
                printf("\n"); \
            }        \
        }   \
        while(0)


EXPORT_SYMBOL INT32 a_mtkapi_avrcp_passthrough_cmd(BTCmdType bt_cmd_type, BTN_ACTION button_action)
{
    BT_AVRCP_LOG("a_mtkapi_avrcp_passthrough_cmd");
    RPC_CLIENT_DECL(2, INT32);
    RPC_CLIENT_ARG_INP(ARG_TYPE_INT32, bt_cmd_type);
    RPC_CLIENT_ARG_INP(ARG_TYPE_INT32, button_action);
    RPC_BT_SERVICE_CLIENT_DO_OP("x_mtkapi_avrcp_passthrough_cmd");
    RPC_CLIENT_RETURN(ARG_TYPE_INT32, -1);
}

EXPORT_SYMBOL INT32 a_mtkapi_avrcp_get_mediaInfo(avrcp_medioInfo_t *mediaInfo)
{
    BT_AVRCP_LOG("a_mtkapi_avrcp_get_mediaInfo");
    RPC_CLIENT_DECL(1, INT32);
    RPC_CLIENT_CHECK(bt_rpc_add_ref_desc(RPC_DEFAULT_ID,
                    mediaInfo,
                    RPC_DESC_avrcp_medioInfo_t,
                    NULL));
    RPC_CLIENT_ARG_OUT(ARG_TYPE_REF_DESC, mediaInfo);
    RPC_BT_SERVICE_CLIENT_DO_OP("x_mtkapi_avrcp_get_mediaInfo");
    RPC_CLIENT_RETURN(ARG_TYPE_INT32, -1);
}

EXPORT_SYMBOL INT32 a_mtkapi_avrcp_get_play_status(bt_player_status_values_t *pt_playStatus)
{
    BT_AVRCP_LOG("a_mtkapi_avrcp_get_play_status");
    RPC_CLIENT_DECL(1, INT32);
    RPC_CLIENT_CHECK(bt_rpc_add_ref_desc(RPC_DEFAULT_ID,
                    pt_playStatus,
                    RPC_DESC_bt_player_status_values_t,
                    NULL));
    RPC_CLIENT_ARG_OUT(ARG_TYPE_REF_DESC, pt_playStatus);
    RPC_BT_SERVICE_CLIENT_DO_OP("x_mtkapi_avrcp_get_play_status");
    RPC_CLIENT_RETURN(ARG_TYPE_INT32, -1);
}

EXPORT_SYMBOL INT32 a_mtkapi_avrcp_set_volume(UINT8 volume)
{
    BT_AVRCP_LOG("a_mtkapi_avrcp_set_volume");
    RPC_CLIENT_DECL(1, INT32);
    RPC_CLIENT_ARG_INP(ARG_TYPE_INT8, volume);
    RPC_BT_SERVICE_CLIENT_DO_OP("x_mtkapi_avrcp_set_volume");
    RPC_CLIENT_RETURN(ARG_TYPE_INT32, -1);
}

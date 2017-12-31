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

#include "ri_common.h"
#include "rh_init_mtk_bt_service.h"
#include "mtk_bt_service_gattc_handle.h"
#include "mtk_bt_service_gatts_handle.h"
#include "mtk_bt_service_avrcp_handle.h"
#include "mtk_bt_service_a2dp_handle.h"
#include "mtk_bt_service_gap_handle.h"
#include "mtk_bt_service_hidh_handle.h"
#include "mtk_bt_service_hfclient_handle.h"
#include "mtk_bt_service_dbg_handle.h"
#include "mtk_bt_service_spp_handle.h"

EXPORT_SYMBOL RPC_ID_T c_rpc_start_mtk_bt_service_server(VOID)
{
  return bt_rpc_open_server("mtk_bt_service");
}

EXPORT_SYMBOL INT32 c_rpc_init_mtk_bt_service_server(VOID)
{
    c_rpc_server_init();
    c_rpc_reg_mtk_bt_service_gattc_op_hndlrs();
    c_rpc_reg_mtk_bt_service_gatts_op_hndlrs();
    c_rpc_reg_mtk_bt_service_avrcp_op_hndlrs();
    c_rpc_reg_mtk_bt_service_a2dp_op_hndlrs();
    c_rpc_reg_mtk_bt_service_gap_op_hndlrs();
    c_rpc_reg_mtk_bt_service_hidh_op_hndlrs();
    c_rpc_reg_mtk_bt_service_hfclient_op_hndlrs();
    c_rpc_reg_mtk_bt_service_spp_op_hndlrs();
#if defined(BT_RPC_DBG_SERVER)
    c_rpc_reg_mtk_bt_service_dbg_op_hndlrs();
#endif

    return 0;
}


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
 *     FEES OR SERVICE charGE PAID BY BUYER TO MEDIATEK FOR SUCH MEDIATEK SOFTWARE AT ISSUE.
 *
 *     THE TRANSACTION CONTEMPLATED HEREUNDER SHALL BE CONSTRUED IN ACCORDANCE WITH THE LAWS
 *     OF THE STATE OF CALIFORNIA, USA, EXCLUDING ITS CONFLICT OF LAWS PRINCIPLES.
 ************************************************************************************************/

#ifndef _MTK_BT_SERVICE_A2DP_WRAPPER_H_
#define _MTK_BT_SERVICE_A2DP_WRAPPER_H_

#include "u_rpcipc_types.h"
#include "u_bluetooth.h"

#ifdef  __cplusplus
extern "C" {
#endif

extern INT32 a_mtkapi_a2dp_src_set_audio_hw_dbg_lvl(UINT8 log_level);

extern INT32 a_mtkapi_a2dp_source_sink_on_off(BOOL fg_src_on, BOOL fg_sink_on);

extern INT32 a_mtkapi_a2dp_connect_audio_sources(char *addr, audio_conn_type_t type);

extern INT32 a_mtkapi_a2dp_disconnect_audio_sources(char *addr, audio_conn_type_t type);

extern INT32 a_mtkapi_a2dp_send_stream_data(const CHAR *data, INT32 len);

extern INT32 a_mtkapi_a2dp_get_role(VOID);

extern INT32 a_mtkapi_a2dp_stream_is_suspend(VOID);

extern INT32 a_mtkapi_a2dp_get_connect_status(VOID);

extern VOID a_mtkapi_a2dp_cmd(bt_a2dp_status_t status);

extern INT32 a_mtkapi_a2dp_get_paired_sink_dev_list(BT_TARGET_DEV_LIST *pt_device_list);

extern INT32 a_mtkapi_a2dp_get_paired_src_dev_list(BT_TARGET_DEV_LIST *pt_device_list);

extern VOID a_mtkapi_a2dp_mute_device(UINT8 fg_mute);

extern INT32 c_rpc_reg_mtk_bt_service_a2dp_cb_hndlrs(VOID);

#ifdef  __cplusplus
}
#endif
#endif

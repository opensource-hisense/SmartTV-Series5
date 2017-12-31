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

#include "mtk_bt_service_hfclient.h"
#include "c_bt_mw_hfclient.h"

INT32 x_mtkapi_bt_hfclient_connect(CHAR *bt_addr)
{
    return c_btm_bt_hfclient_connect(bt_addr);
}

INT32 x_mtkapi_bt_hfclient_disconnect(CHAR *bt_addr)
{
    return c_btm_bt_hfclient_disconnect(bt_addr);
}

INT32 x_mtkapi_bt_hfclient_connect_audio(CHAR *bt_addr)
{
    return c_btm_bt_hfclient_connect_audio(bt_addr);
}

INT32 x_mtkapi_bt_hfclient_disconnect_audio(CHAR *bt_addr)
{
    return c_btm_bt_hfclient_disconnect_audio(bt_addr);
}

INT32 x_mtkapi_bt_hfclient_start_voice_recognition(VOID)
{
    return c_btm_bt_hfclient_start_voice_recognition();
}

INT32 x_mtkapi_bt_hfclient_stop_voice_recognition(VOID)
{
    return c_btm_bt_hfclient_stop_voice_recognition();
}

INT32 x_mtkapi_bt_hfclient_volume_control(BT_HFCLIENT_VOLUME_TYPE_T type, INT32 volume)
{
    return c_btm_bt_hfclient_volume_control(type, volume);
}

INT32 x_mtkapi_bt_hfclient_dial(const CHAR *number)
{
    return c_btm_bt_hfclient_dial(number);
}

INT32 x_mtkapi_bt_hfclient_dial_memory(INT32 location)
{
    return c_btm_bt_hfclient_dial_memory(location);
}

INT32 x_mtkapi_bt_hfclient_handle_call_action(BT_HFCLIENT_CALL_ACTION_T action, INT32 idx)
{
    return c_btm_bt_hfclient_handle_call_action(action, idx);
}

INT32 x_mtkapi_bt_hfclient_query_current_calls(VOID)
{
    return c_btm_bt_hfclient_query_current_calls();
}

INT32 x_mtkapi_bt_hfclient_query_current_operator_name(VOID)
{
    return c_btm_bt_hfclient_query_current_operator_name();
}

INT32 x_mtkapi_bt_hfclient_retrieve_subscriber_info(VOID)
{
    return c_btm_bt_hfclient_retrieve_subscriber_info();
}

INT32 x_mtkapi_bt_hfclient_send_dtmf(CHAR code)
{
    return c_btm_bt_hfclient_send_dtmf(code);
}

INT32 x_mtkapi_bt_hfclient_request_last_voice_tag_number(VOID)
{
    return c_btm_bt_hfclient_request_last_voice_tag_number();
}

INT32 x_mtkapi_bt_hfclient_send_at_cmd(INT32 cmd, INT32 val1, INT32 val2, const CHAR *arg)
{
    return c_btm_bt_hfclient_send_at_cmd(cmd, val1, val2, arg);
}

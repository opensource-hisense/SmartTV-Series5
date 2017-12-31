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

#include "mtk_bt_service_avrcp.h"
#include "c_bt_mw_avrcp.h"

#define BT_RC_LOG(_stmt...) \
        do{ \
            if(0){    \
                printf("[AVRCP IPC Server]Func:%s Line:%d--->: ", __FUNCTION__, __LINE__);   \
                printf(_stmt); \
                printf("\n"); \
            }        \
        }   \
        while(0)



INT32 x_mtkapi_avrcp_passthrough_cmd(INT32 bt_cmd_type, INT32 button_action)
{
    BT_RC_LOG("[.c][%s]", __FUNCTION__);
    return c_btm_avrcp_passthrough_cmd(bt_cmd_type, button_action);
}

INT32 x_mtkapi_avrcp_get_mediaInfo(avrcp_medioInfo_t *mediaInfo)
{
    BT_RC_LOG("[.c][%s]", __FUNCTION__);
    INT32 i4_ret = 0;
    i4_ret = c_btm_avrcp_get_mediaInfo(mediaInfo);
    if (i4_ret != 0)
    {
        BT_RC_LOG("[.c][%s]c_btm_avrcp_get_mediaInfo fail", __FUNCTION__);
    }
    else
    {
        BT_RC_LOG("[.c][%s]c_btm_avrcp_get_mediaInfo success", __FUNCTION__);
    }

    return i4_ret;
}

INT32 x_mtkapi_avrcp_get_play_status(bt_player_status_values_t *pt_playStatus)
{
    BT_RC_LOG("[.c][%s]", __FUNCTION__);
    INT32 i4_ret = 0;
    i4_ret = c_btm_avrcp_get_play_status(pt_playStatus);
    if (i4_ret != 0)
    {
        BT_RC_LOG("[.c][%s]c_btm_avrcp_get_mediaInfo fail", __FUNCTION__);
    }
    else
    {
        BT_RC_LOG("[.c][%s]c_btm_avrcp_get_mediaInfo success", __FUNCTION__);
    }

    return i4_ret;

}

INT32 x_mtkapi_avrcp_set_volume(UINT8 volume)
{
    BT_RC_LOG("[.c][%s]", __FUNCTION__);
    return c_btm_set_volume(volume);
}

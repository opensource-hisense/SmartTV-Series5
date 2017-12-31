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
/* FILE NAME:  bt_mw_avrcp_cli.c
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

#include "bluetooth.h"
#if defined(MTK_LINUX_AVRCP_PLUS) && (MTK_LINUX_AVRCP_PLUS == TRUE)
#include "mtk_bt_rc.h"
#else
#include "bt_rc.h"
#endif
#include "bt_mw_common.h"
#include "bt_mw_avrcp.h"

#include "bluetooth_sync.h"
#include "bt_mw_avrcp_cli.h"
#include "c_bt_mw_avrcp.h"


INT32 _bluetooth_avrcp_passthrough_cmd_cli (INT32 argc, const CHAR **argv)
{
    CHAR ps_type[16];
    CHAR ps_action[8];
    BTCmdType bt_cmd_type;
    BTN_ACTION button_action;

    if (argc != 3)
    {
        BT_DBG_NOTICE(BT_DEBUG_COMM, "please input avrcp [play|pause|stop|fwd|bwd|ffwd|rwd|volume_up|volume_down] [press|release|auto]");
    }
    if (NULL == argv[1])
    {
        BT_DBG_NOTICE(BT_DEBUG_COMM, "type is null");
        BT_DBG_NOTICE(BT_DEBUG_COMM, "please input avrcp [play|pause|stop|fwd|bwd|ffwd|rwd|volume_up|volume_down] [press|release|auto]");
    }
    if (NULL == argv[2])
    {
        BT_DBG_NOTICE(BT_DEBUG_COMM, "action is null");
        BT_DBG_NOTICE(BT_DEBUG_COMM, "please input avrcp [play|pause|stop|fwd|bwd|ffwd|rwd|volume_up|volume_down] [press|release|auto]");
    }
    memset(ps_type, 0x0, sizeof(ps_type));
    memset(ps_action, 0x0, sizeof(ps_action));
    strncpy(ps_type,argv[1], sizeof(ps_type));
    ps_type[sizeof(ps_type)-1] = '\0';
    strncpy(ps_action,argv[2], sizeof(ps_action));
    ps_action[sizeof(ps_action)-1] = '\0';
    BT_DBG_NORMAL(BT_DEBUG_COMM, "avrcp %s %s", ps_type, ps_action);

    if (0 == strcmp(ps_type, "play"))
    {
        bt_cmd_type = BT_CMD_TYPE_PLAY;
    }
    else if (0 == strcmp(ps_type, "pause"))
    {
        bt_cmd_type = BT_CMD_TYPE_PAUSE;
    }
    else if (0 == strcmp(ps_type, "stop"))
    {
        bt_cmd_type = BT_CMD_TYPE_STOP;
    }
    else if (0 == strcmp(ps_type, "fwd"))
    {
        bt_cmd_type = BT_CMD_TYPE_FWD;
    }
    else if (0 == strcmp(ps_type, "bwd"))
    {
        bt_cmd_type = BT_CMD_TYPE_BWD;
    }
    else if (0 == strcmp(ps_type, "ffwd"))
    {
        bt_cmd_type = BT_CMD_TYPE_FFWD;
    }
    else if (0 == strcmp(ps_type, "rwd"))
    {
        bt_cmd_type = BT_CMD_TYPE_RWD;
    }
    else if (0 == strcmp(ps_type, "volume_up"))
    {
        bt_cmd_type = BT_CMD_TYPE_VOLUME_UP;
    }
    else if (0 == strcmp(ps_type, "volume_down"))
    {
        bt_cmd_type = BT_CMD_TYPE_VOLUME_DOWN;
    }
    else
    {
        BT_DBG_NOTICE(BT_DEBUG_COMM, "input cmd error");
        BT_DBG_NOTICE(BT_DEBUG_COMM, "please input avrcp [play|pause|stop|fwd|bwd|ffwd|rwd|volume_up|volume_down] [press|release|auto]");
        return -1;
    }

    if (0 == strcmp(ps_action, "press"))
    {
        button_action = BUTTON_ACT_PRESS;
    }
    else if (0 == strcmp(ps_action, "release"))
    {
        button_action = BUTTON_ACT_RELEASE;
    }
    else if (0 == strcmp(ps_action, "auto"))
    {
        button_action = BUTTON_ACT_AUTO;
    }
    else
    {
        BT_DBG_NOTICE(BT_DEBUG_COMM, "input button action error");
        BT_DBG_NOTICE(BT_DEBUG_COMM, "please input avrcp [play|pause|stop|fwd|bwd|ffwd|rwd|volume_up|volume_down] [press|release|auto]");
        return -1;
    }
    BT_DBG_NOTICE(BT_DEBUG_COMM, "avrcp %d %d", bt_cmd_type, button_action);
    bluetooth_avrcp_send_cmd(bt_cmd_type, button_action);
    return 0;
}

INT32 _bluetooth_avrcp_get_capabilities_cli (INT32 argc, const CHAR **argv)
{
    avrcp_apability_type_t type;

    if (2 == argc)
    {
        if (strncmp(argv[1], "companyid", strlen("companyid")) == 0)
        {
            type = CAPABILITY_COMPANYID;
        }
        else if (strncmp(argv[1], "events", strlen("events")) == 0)
        {
            type = CAPABILITY_EVENTS;
        }
        else
        {
            BT_DBG_NOTICE(BT_DEBUG_COMM, "unknow capability");
            type = CAPABILITY_UNKNOWN;
        }
        c_btm_bt_avrcp_get_capabilities(type);
    }
    else
    {
        BT_DBG_NOTICE(BT_DEBUG_COMM, "input error");
        BT_DBG_NOTICE(BT_DEBUG_COMM, "please input get_capabilities [companyid|events]");
    }
    return 0;
}

INT32 _bluetooth_avrcp_track_change_enable_cli (INT32 argc, const CHAR **argv)
{
    UINT8 track_selected;

    if (2 == argc)
    {
        track_selected = atol(argv[1]);

        c_btm_send_track_change_rsp_handler(track_selected);
    }
    else
    {
        BT_DBG_NOTICE(BT_DEBUG_COMM, "input error");
        BT_DBG_NORMAL(BT_DEBUG_COMM, "please input track_change_rsp [integer]");
    }
    return 0;
}

INT32 _bluetooth_avrcp_large_element_enable_cli (INT32 argc, const CHAR **argv)
{
    INT8 i1_enable;

    if (2 == argc)
    {
        i1_enable = atol(argv[1]);

        c_btm_set_large_element_attribute_response_handler(i1_enable);
    }
    else
    {
        BT_DBG_NOTICE(BT_DEBUG_COMM, "input error");
        BT_DBG_NOTICE(BT_DEBUG_COMM, "please input large_element [0|1]");
    }
    return 0;
}

INT32 _bluetooth_avrcp_pts_enable_cli (INT32 argc, const CHAR **argv)
{
    INT32 i4_enable;

    if (2 == argc)
    {
        i4_enable = atol(argv[1]);

        c_btm_send_pts_test_handler(i4_enable);
    }
    else
    {
        BT_DBG_NOTICE(BT_DEBUG_COMM, "input error");
        BT_DBG_NOTICE(BT_DEBUG_COMM, "please input pts_enable [0|1]");
    }
    return 0;
}



INT32 _bluetooth_avrcp_absolute_volume_rsp_cli (INT32 argc, const CHAR **argv)
{
    UINT8 i1_volume;

    if (2 == argc)
    {
        i1_volume = atol(argv[1]);

        c_btm_send_volume_change_rsp_handler(i1_volume);
    }
    else
    {
        BT_DBG_NOTICE(BT_DEBUG_COMM, "input error");
        BT_DBG_NOTICE(BT_DEBUG_COMM, "please input absolute_volume_rsp [integer]");
    }
    return 0;
}

INT32 _bluetooth_avrcp_avrcp_delay_cli (INT32 argc, const CHAR **argv)
{
    INT32 i4_delay;

    if (2 == argc)
    {
        i4_delay = atol(argv[1]);

        bluetooth_rc_set_avrcp_delay(i4_delay);
    }
    else
    {
        BT_DBG_NOTICE(BT_DEBUG_COMM, "input error");
        BT_DBG_NOTICE(BT_DEBUG_COMM, "please input avrcp_delay [integer]");
    }
    return 0;
}

/* for PTS test */
INT32 _bluetooth_set_trackstatus_cli(INT32 argc, const CHAR **argv)
{
    CHAR ptrack_status[16] = {0};
    if ( argc == 2 )
    {
        strncpy(ptrack_status, argv[1], strlen(argv[1]));
        ptrack_status[strlen(argv[1])] = '\0';
        c_btm_set_trackstatus(ptrack_status);
    }
    else
    {
        BT_DBG_ERROR(BT_DEBUG_COMM, "A parameter is required!");
        return -1;
    }

    return 0;
}




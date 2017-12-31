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
/* FILE NAME:  c_bt_mw_a2dp_common.c
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
#include "bt_mw_a2dp_common.h"
#if ENABLE_A2DP_SINK
#include "bt_mw_a2dp_snk.h"
#endif
#if ENABLE_A2DP_SRC
#include "bt_mw_a2dp_src.h"
#endif
#include "bluetooth_sync.h"
#include "bt_mw_avrcp.h"
#include "bt_mw_devices_info.h"
#include "linuxbt_a2dp_if.h"

extern BtAppEventCbk      BtAppCbk;

/* return value is TRUE: stream suspend, FALSE: stream start */
EXPORT_SYMBOL BOOL c_btm_stream_is_suspend(VOID)
{
    FUNC_ENTRY;
    return bt_audio_is_stream_suspend();
}

/*app should call it after A2DP connection notify*/
EXPORT_SYMBOL BT_A2DP_ROLE c_btm_get_a2dp_role(VOID)
{
    BT_A2DP_ROLE a2dp_role;
    if (!get_bt_a2dp_be_sink())
    {
        a2dp_role = BT_A2DP_ROLE_SRC;
    }
    else
    {
        a2dp_role = BT_A2DP_ROLE_SINK;
    }
    BT_DBG_NORMAL(BT_DEBUG_A2DP, "The role is:%s", !get_bt_a2dp_be_sink()?"Source":"Sink");
    return a2dp_role;
}

/*start to connect with SRC device(eg. cell phone), should paired with the SRC device ago*/
EXPORT_SYMBOL INT32 c_btm_connect_audio_sources(char *addr, audio_conn_type_t type)
{
    FUNC_ENTRY;
    BT_A2DP_ROLE a2dp_role;

    if (NULL == addr)
    {
        BT_DBG_ERROR(BT_DEBUG_A2DP, "null pointer!");
        return -1;
    }
    BT_DBG_NORMAL(BT_DEBUG_A2DP, "the MAC is: %s", addr);

    if (!get_bt_a2dp_be_sink())
    {
        a2dp_role = A2DP_LOCAL_ROLE_SRC;
    }
    else
    {
        a2dp_role = A2DP_LOCAL_ROLE_SNK;
    }

    if (CONN_TYPE_A2DP == type)
    {
        bluetooth_a2dp_connect(addr, a2dp_role);
    }
    else
    {
        bluetooth_avrcp_connect(addr);
    }

    return BT_SUCCESS;
}

/*only can disconnect one connection*/
EXPORT_SYMBOL INT32 c_btm_bt_disconnect_audio_sources(char *addr, audio_conn_type_t type)
{
    BT_A2DP_ROLE a2dp_role;

    if (CONN_TYPE_A2DP == type)
    {
        if (!get_bt_a2dp_be_sink())
        {
            a2dp_role = A2DP_LOCAL_ROLE_SRC;
        }
        else
        {
            a2dp_role = A2DP_LOCAL_ROLE_SNK;
        }
        bluetooth_a2dp_disconnect(addr, a2dp_role);
    }
    else
    {
        bluetooth_avrcp_disconnect(addr);
    }

    return BT_SUCCESS;
} /*c_btm_bt_disconnect_audio_sources*/

EXPORT_SYMBOL INT32 c_btm_source_sink_on_off(BOOL fg_src_on, BOOL fg_sink_on)
{
    FUNC_ENTRY;
    UINT8 old_role = get_bt_enabled_a2dp_role();
    UINT8 new_role = 0;

#if ENABLE_A2DP_SRC
    if (fg_src_on)
    {
        if (!(old_role & 0x2))
        {
            bluetooth_a2dp_src_enable(1);
        }
    }
    else
    {
        if (old_role & 0x2)
        {
            bluetooth_a2dp_src_enable(0);
        }
    }
#else
    BT_DBG_WARNING(BT_DEBUG_A2DP, "not support A2DP Source role");
#endif


#if ENABLE_A2DP_SINK
    if (fg_sink_on)
    {
        if (!(old_role & 0x1))
        {
            bluetooth_a2dp_sink_enable(1);
        }
    }
    else
    {
        if (old_role & 0x1)
        {
            bluetooth_a2dp_sink_enable(0);
        }
    }
#else
    BT_DBG_WARNING(BT_DEBUG_A2DP, "not support A2DP Sink role");
#endif

    new_role = get_bt_enabled_a2dp_role();
    if (new_role == old_role)
    {
        BT_DBG_WARNING(BT_DEBUG_A2DP, "A2DP role no change(%x)", new_role);
        return BT_SUCCESS;
    }

#if ENABLE_A2DP_SINK
    if (((new_role ^ old_role) & old_role) & 0x1) /* if disable sink */
    {
        set_bt_a2dp_be_sink(FALSE);
        set_bt_a2dp_rx_mode(0);
        bluetooth_a2dp_2nd_con_enable(0);

        linuxbt_a2dp_sink_deinit();
        usleep(500*1000);
#if ENABLE_AVRCP_PROFILE
        linuxbt_rc_deinit();
#endif
    }
#endif

#if ENABLE_A2DP_SRC
    if (((new_role ^ old_role) & old_role) & 0x2) /* if disable source */
    {
        linuxbt_a2dp_src_deinit();
        usleep(500*1000);
#if ENABLE_AVRCP_PROFILE
        linuxbt_rc_tg_deinit();
#endif
    }
#endif

#if ENABLE_A2DP_SINK
    if (((new_role ^ old_role) & new_role) & 0x1) /* if enable sink */
    {
        set_bt_a2dp_be_sink(TRUE);
        set_bt_a2dp_rx_mode(1);
        bluetooth_a2dp_2nd_con_enable(1);
        linuxbt_a2dp_sink_init();
        usleep(500*1000);

#if ENABLE_AVRCP_PROFILE
        linuxbt_rc_init();
#endif
    }
#endif

#if ENABLE_A2DP_SRC
    if (((new_role ^ old_role) & new_role) & 0x2) /* if enable source */
    {
        set_bt_a2dp_be_sink(FALSE);
        set_bt_a2dp_rx_mode(0);
        bluetooth_a2dp_2nd_con_enable(0);
        linuxbt_a2dp_src_init();
        //usleep(500*1000);
#if ENABLE_AVRCP_PROFILE
        linuxbt_rc_tg_init();
#endif
    }
#endif

    return BT_SUCCESS;
}

EXPORT_SYMBOL BOOL c_btm_get_connect_status(VOID)
{
    FUNC_ENTRY;

    return get_bt_connect();
}



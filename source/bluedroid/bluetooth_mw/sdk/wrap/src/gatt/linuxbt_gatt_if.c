/*******************************************************************************
*  Copyright Statement:
*  --------------------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of MediaTek Inc. (C) 2013
*
*  BY OPENING THIS FILE, BUYER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND AGREES
*  THAT THE SOFTWARE/FIRMWARE AND ITS DOCUMENTATIONS ("MEDIATEK SOFTWARE")
*  RECEIVED FROM MEDIATEK AND/OR ITS REPRESENTATIVES ARE PROVIDED TO BUYER ON
*  AN "AS-IS" BASIS ONLY. MEDIATEK EXPRESSLY DISCLAIMS ANY AND ALL WARRANTIES,
*  EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF
*  MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE OR NONINFRINGEMENT.
*  NEITHER DOES MEDIATEK PROVIDE ANY WARRANTY WHATSOEVER WITH RESPECT TO THE
*  SOFTWARE OF ANY THIRD PARTY WHICH MAY BE USED BY, INCORPORATED IN, OR
*  SUPPLIED WITH THE MEDIATEK SOFTWARE, AND BUYER AGREES TO LOOK ONLY TO SUCH
*  THIRD PARTY FOR ANY WARRANTY CLAIM RELATING THERETO. MEDIATEK SHALL ALSO
*  NOT BE RESPONSIBLE FOR ANY MEDIATEK SOFTWARE RELEASES MADE TO BUYER'S
*  SPECIFICATION OR TO CONFORM TO A PARTICULAR STANDARD OR OPEN FORUM.
*
*  BUYER'S SOLE AND EXCLUSIVE REMEDY AND MEDIATEK'S ENTIRE AND CUMULATIVE
*  LIABILITY WITH RESPECT TO THE MEDIATEK SOFTWARE RELEASED HEREUNDER WILL BE,
*  AT MEDIATEK'S OPTION, TO REVISE OR REPLACE THE MEDIATEK SOFTWARE AT ISSUE,
*  OR REFUND ANY SOFTWARE LICENSE FEES OR SERVICE CHARGE PAID BY BUYER TO
*  MEDIATEK FOR SUCH MEDIATEK SOFTWARE AT ISSUE.
*
*  THE TRANSACTION CONTEMPLATED HEREUNDER SHALL BE CONSTRUED IN ACCORDANCE
*  WITH THE LAWS OF THE STATE OF CALIFORNIA, USA, EXCLUDING ITS CONFLICT OF
*  LAWS PRINCIPLES.  ANY DISPUTES, CONTROVERSIES OR CLAIMS ARISING THEREOF AND
*  RELATED THERETO SHALL BE SETTLED BY ARBITRATION IN SAN FRANCISCO, CA, UNDER
*  THE RULES OF THE INTERNATIONAL CHAMBER OF COMMERCE (ICC).
*
*******************************************************************************/

/* FILE NAME:  linuxbt_gatt_if.c
 * AUTHOR: Xuemei Yang
 * PURPOSE:
 *      It provides GATT common operation interface to MW.
 * NOTES:
 */


/*-----------------------------------------------------------------------------
                    include files
 ----------------------------------------------------------------------------*/


#include <string.h>
#include <stdlib.h>
#include "bt_mw_common.h"
#include "linuxbt_gatt_if.h"
#include "linuxbt_gattc_if.h"
#include "linuxbt_gatts_if.h"
#include "linuxbt_common.h"


extern void *linuxbt_gap_get_profile_interface(const char *profile_id);
static btgatt_interface_t *linuxbt_gatt_interface = NULL;

#if defined(MTK_LINUX_GATT) && (MTK_LINUX_GATT == TRUE)
static btgatt_ex_interface_t *linuxbt_gatt_ex_interface = NULL;
extern const btgatt_ex_client_callbacks_t linuxbt_gattc_ex_callbacks;

const btgatt_ex_callbacks_t linuxbt_gatt_ex_callbacks =
{
    sizeof(btgatt_ex_callbacks_t),
    &linuxbt_gattc_ex_callbacks,
};
#endif

extern const btgatt_client_callbacks_t linuxbt_gattc_callbacks;
extern const btgatt_server_callbacks_t linuxbt_gatts_callbacks;

const btgatt_callbacks_t linuxbt_gatt_callbacks =
{
    sizeof(btgatt_callbacks_t),
    &linuxbt_gattc_callbacks,
    &linuxbt_gatts_callbacks,
};


/**
 * FUNCTION NAME: linuxbt_gatt_init
 * PURPOSE:
 *      The function is used to initialize gatt inteface.
 * INPUT:
 *      None.
 * OUTPUT:
 *      None
 * RETURN:
 *      BT_SUCCESS                    -- Operate success.
 *      BT_ERR_STATUS_FAILED          -- Operate fail.
 *      BT_ERR_STATUS_PARM_INVALID    -- parameter is invalid.
 *      BT_ERR_STATUS_DONE                -- request already completed.
 * NOTES:
 *      None
 */
int linuxbt_gatt_init(void)
{
    //FUNC_ENTRY;
    bt_status_t ret = BT_STATUS_SUCCESS;
    //BTUT_MOD hid_mod = {0};

    // Get GATT interface
    linuxbt_gatt_interface = (btgatt_interface_t *) linuxbt_gap_get_profile_interface(BT_PROFILE_GATT_ID);
    if (NULL == linuxbt_gatt_interface)
    {
        BT_DBG_ERROR(BT_DEBUG_GATT,"[GATT] Failed to get GATT interface");
        return BT_ERR_STATUS_PARM_INVALID;
    }

    // Init GATT interface
    ret = linuxbt_gatt_interface->init(&linuxbt_gatt_callbacks);
    if (BT_STATUS_SUCCESS == ret)
    {
        BT_DBG_NOTICE(BT_DEBUG_GATT, "[GATT] success to init GATT interface");
    }
    else if (BT_STATUS_DONE == ret)
    {
        BT_DBG_NOTICE(BT_DEBUG_GATT, "[GATT] already init GATT interface");
    }

    if (BT_SUCCESS != linuxbt_gattc_init(linuxbt_gatt_interface->client))
    {
        BT_DBG_ERROR(BT_DEBUG_GATT, "[GATT] Failed to init GATT client interface");
        return BT_ERR_STATUS_FAIL;
    }

    if (BT_SUCCESS != linuxbt_gatts_init(linuxbt_gatt_interface->server))
    {
        BT_DBG_ERROR(BT_DEBUG_GATT, "[GATT] Failed to init GATT server interface");
        return BT_ERR_STATUS_FAIL;
    }

#if defined(MTK_LINUX_GATT) && (MTK_LINUX_GATT == TRUE)
    linuxbt_gatt_ex_interface = (btgatt_ex_interface_t *) linuxbt_gap_get_profile_interface(BT_PROFILE_GATT_EX_ID);
    if (NULL == linuxbt_gatt_ex_interface)
    {
        BT_DBG_ERROR(BT_DEBUG_GATT,"[GATT] Failed to get GATT extened interface");
        return BT_ERR_STATUS_PARM_INVALID;
    }

    ret = linuxbt_gatt_ex_interface->init(&linuxbt_gatt_ex_callbacks);
    if (BT_STATUS_SUCCESS == ret)
    {
        BT_DBG_NOTICE(BT_DEBUG_GATT, "[GATT] success to init GATT extended interface");
    }
    else if (BT_STATUS_DONE == ret)
    {
        BT_DBG_NOTICE(BT_DEBUG_GATT, "[GATT] already init GATT extended interface");
    }

    if (BT_SUCCESS != linuxbt_gattc_ex_init(linuxbt_gatt_ex_interface->ex_client))
    {
        BT_DBG_ERROR(BT_DEBUG_GATT, "[GATT] Failed to init GATT client extented interface");
        return BT_ERR_STATUS_FAIL;
    }
#endif
    return BT_SUCCESS;//linuxbt_return_value_convert(ret);
}

/**
 * FUNCTION NAME: linuxbt_gatt_deinit
 * PURPOSE:
 *      The function is used to deinitialize gatt inteface.
 * INPUT:
 *      None.
 * OUTPUT:
 *      None
 * RETURN:
 *      BT_SUCCESS             -- Operate success.
 *      BT_ERR_STATUS_FAILED   -- Operate fail.
 * NOTES:
 *      None
 */
int linuxbt_gatt_deinit(void)
{
    //FUNC_ENTRY;
    BT_ERR_STATUS_T ret = BT_SUCCESS;
    // Deinit GATT interface
    if (NULL != linuxbt_gatt_interface)
    {
        linuxbt_gatt_interface->cleanup();
        if (BT_SUCCESS != linuxbt_gattc_deinit())
        {
            BT_DBG_ERROR(BT_DEBUG_GATT, "[GATT] Failed to deinit GATT client");
            return BT_ERR_STATUS_FAIL;
        }
        if (BT_SUCCESS != linuxbt_gatts_deinit())
        {
            BT_DBG_ERROR(BT_DEBUG_GATT, "[GATT] Failed to deinit GATT server");
            return BT_ERR_STATUS_FAIL;
        }
    }
    return ret;
}


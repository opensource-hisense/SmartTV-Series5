/*******************************************************************************
 * LEGAL DISCLAIMER
 *
 * (Header of MediaTek Software/Firmware Release or Documentation)
 *
 * BY OPENING OR USING THIS FILE, BUYER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND
 * AGREES THAT THE SOFTWARE/FIRMWARE AND ITS DOCUMENTATIONS ("MEDIATEK
 * SOFTWARE") RECEIVED FROM MEDIATEK AND/OR ITS REPRESENTATIVES ARE PROVIDED TO
 * BUYER ON AN "AS-IS" BASISONLY. MEDIATEK EXPRESSLY DISCLAIMS ANY AND ALL
 * WARRANTIES, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE OR
 * NONINFRINGEMENT. NEITHER DOES MEDIATEK PROVIDE ANY WARRANTY WHATSOEVER WITH
 * RESPECT TO THE SOFTWARE OF ANY THIRD PARTY WHICH MAY BE USED BY,
 * INCORPORATED IN, OR SUPPLIED WITH THE MEDIATEK SOFTWARE, AND BUYER AGREES TO
 * LOOK ONLY TO SUCH THIRD PARTY FOR ANY WARRANTY CLAIM RELATING THERETO.
 * MEDIATEK SHALL ALSO NOT BE RESPONSIBLE FOR ANY MEDIATEK SOFTWARE RELEASES
 * MADE TO BUYER'S SPECIFICATION OR TO CONFORM TO A PARTICULAR STANDARD OR OPEN
 * FORUM.
 *
 * BUYER'S SOLE AND EXCLUSIVE REMEDY AND MEDIATEK'S ENTIRE AND CUMULATIVE
 * LIABILITY WITH RESPECT TO THE MEDIATEK SOFTWARE RELEASED HEREUNDER WILL BE,
 * AT MEDIATEK'S OPTION, TO REVISE OR REPLACE THE MEDIATEK SOFTWARE AT ISSUE,
 * OR REFUND ANY SOFTWARE LICENSE FEES OR SERVICE CHARGE PAID BY BUYER TO
 * MEDIATEK FOR SUCH MEDIATEK SOFTWARE AT ISSUE.
 *
 * THE TRANSACTION CONTEMPLATED HEREUNDER SHALL BE CONSTRUED IN ACCORDANCE WITH
 * THE LAWS OF THE STATE OF CALIFORNIA, USA, EXCLUDING ITS CONFLICT OF LAWS
 * PRINCIPLES.
 ******************************************************************************/

/* FILE NAME:  bt_mw_common.c
 * AUTHOR: Changlu Yi
 * PURPOSE:
 *      It provides bluetooth common structure to MW.
 * NOTES:
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <assert.h>
#include <unistd.h>
#include <assert.h>
#include <time.h>
#include <sched.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <fcntl.h>
//#include "bt_mmi.h"
#include "u_bluetooth.h"
#include "bt_mw_common.h"
#if ENABLE_MTK_TV_PLATFORM
//#include "dbg/x_dbg.h"
#else
//#include "x_ipcd.h"
#endif

static CHAR         bt_prefix[] = "BLUETOOTH_MW";
CHAR* bt_get_log_prefix(VOID)
{
    return bt_prefix;
}

INT32 bt_get_dbg_level(BT_DEBUG_LAYER_NAME_T layer)
{
    INT32 i = 0;
    if (BT_DEBUG_MAX <= layer)
    {
        for (i = 0; i < BT_DEBUG_MAX; i++)
        {
            BT_DBG_ERROR(BT_DEBUG_COMM, "layer:%d level:%d", i, btmw_trc_map[i].trace_level);
        }
        return -1;
    }
    else
    {
        BT_DBG_ERROR(BT_DEBUG_COMM, "layer:%d level:%d", layer, btmw_trc_map[layer].trace_level);
        return btmw_trc_map[layer].trace_level;
    }
}

VOID bt_set_dbg_level(BT_DEBUG_LAYER_NAME_T layer, INT32 level)
{
    INT32 i = 0;
    if (BT_DEBUG_MAX <= layer)
    {
        for (i = 0; i < BT_DEBUG_MAX; i++)
        {
            btmw_trc_map[i].trace_level = level;
        }
    }
    else
    {
        btmw_trc_map[layer].trace_level = level;
    }
    BT_DBG_WARNING(BT_DEBUG_COMM, "layer:%d level:%d", layer, level);
}

//static BOOL fg_bt_businit;

static bt_error_str_t errorTbl[] =
{
    {BT_ERR_SUCCESS, "Success"},
    {BT_ERR_NULL_POINTER, "Null Pointer"},
    {BT_ERR_OUT_OF_RANGE, "Out of Range"},
    {BT_ERR_UNKNOWN_CMD, "Unknown Command"},
    {BT_ERR_INVALID_PARAM, "Invalid input paramter"},
    {BT_ERR_INVALID_PARAM_NUMS, "Invalid input paramter number"},
    {BT_ERR_NOT_SUPPORT, "Not supported"},
    {BT_ERR_FAILED, "Failed"},
};

CHAR* print_error_str(BT_ERR_TYPE errorId)
{
    INT16 i;

    for (i = 0; i < sizeof(errorTbl)/sizeof(bt_error_str_t); i++)
    {
        if (errorId == errorTbl[i].errorId)
        {
            return errorTbl[i].name;
        }
    }

    return "Unknown Error";
}

INT32 BluetoothInitPre (VOID)
{
    FUNC_ENTRY;

    return BT_SUCCESS;
}

INT32 BluetoothExitPre (VOID)
{
    FUNC_ENTRY;

    return BT_SUCCESS;
}

VOID bluetooth_uuid_stos(CHAR *uuid_s,  CHAR *uuid)
{
    INT32 i = 0,j = 0;
    INT32 size = 0;
    CHAR  temp[3];
    UINT8 temp_uuid[BT_GATT_UUID_ARRAY_SIZE] = {0};
    temp[2] = '\0';

    if ((NULL == uuid_s) || (NULL == uuid) || (0 == strlen(uuid_s)))
    {
        return;
    }

    size = strlen(uuid_s);
    while (i < size)
    {
        if (uuid_s[i] == '-' || uuid_s[i] == '\0')
        {
            i++;
            continue;
        }
        temp[0] = uuid_s[i];
        temp[1] = uuid_s[i+1];
        temp_uuid[j] = strtoul(temp, NULL, 16);
        i+=2;
        j++;
    }
    if (size <= 8)   // 16bits uuid or 32bits uuid
    {
        if (size == 4)
        {
            temp_uuid[2] = temp_uuid[0];
            temp_uuid[3] = temp_uuid[1];
            temp_uuid[0] = 0;
            temp_uuid[1] = 0;
        }
        temp_uuid[4] = 0x00;
        temp_uuid[5] = 0x00;
        temp_uuid[6] = 0x10;
        temp_uuid[7] = 0x00;

        temp_uuid[8] = 0x80;
        temp_uuid[9] = 0x00;
        temp_uuid[10] = 0x00;
        temp_uuid[11] = 0x80;

        temp_uuid[12] = 0x5F;
        temp_uuid[13] = 0x9B;
        temp_uuid[14] = 0x34;
        temp_uuid[15] = 0xFB;
    }


    CHAR *ptr;
    ptr = uuid;
    for (i = 0 ; i < BT_GATT_UUID_ARRAY_SIZE ; i++)
    {
        sprintf(ptr, "%02X", temp_uuid[i]);
        ptr+=2;
        if (i == 3 || i == 5 || i == 7 || i == 9)
        {
            *ptr = '-';
            ptr++;
        }
    }
    *ptr = '\0';
    return;
}

VOID bluetooth_uuid_stoh(CHAR *uuid_s,  CHAR *uuid)
{
    INT32 i = 0,j = 0;
    INT32 size = 0;
    CHAR  temp[3];
    temp[2] = '\0';
    UINT8 temp_uuid[BT_GATT_UUID_ARRAY_SIZE];
    if (NULL == uuid_s || NULL == uuid)
    {
        return;
    }

    size = strlen(uuid_s);
    while (i < size)
    {
        if (uuid_s[i] == '-' || uuid_s[i] == '\0')
        {
            i++;
            continue;
        }
        temp[0] = uuid_s[i];
        temp[1] = uuid_s[i+1];
        temp_uuid[j] = strtoul(temp, NULL, 16);
        i+=2;
        j++;
    }

    if (size <= 8)   // 16bits uuid or 32bits uuid
    {
        if (size == 4)
        {
            temp_uuid[2] = temp_uuid[0];
            temp_uuid[3] = temp_uuid[1];
            temp_uuid[0] = 0;
            temp_uuid[1] = 0;
        }
        temp_uuid[4] = 0x00;
        temp_uuid[5] = 0x00;
        temp_uuid[6] = 0x10;
        temp_uuid[7] = 0x00;

        temp_uuid[8] = 0x80;
        temp_uuid[9] = 0x00;
        temp_uuid[10] = 0x00;
        temp_uuid[11] = 0x80;

        temp_uuid[12] = 0x5F;
        temp_uuid[13] = 0x9B;
        temp_uuid[14] = 0x34;
        temp_uuid[15] = 0xFB;
    }

    for (i = 0; i < BT_GATT_UUID_ARRAY_SIZE; i++)
    {
        uuid[BT_GATT_UUID_ARRAY_SIZE - 1 - i] = temp_uuid[i];
    }
    return;
}



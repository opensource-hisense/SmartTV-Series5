
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

/* FILE NAME:  bt_mw_spp_cli.c
 * AUTHOR: Jigong Yin
 * PURPOSE:
 *      It provides spp operation command implementation.
 * NOTES:
 */


/*-----------------------------------------------------------------------------
                    include files
 ----------------------------------------------------------------------------*/

#include <pthread.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#if ENABLE_MTK_TV_PLATFORM
//#include "inc/x_common.h"
//#include "dbg/x_dbg.h"
#endif
//#include "x_cli.h"
#include "bt_mw_common.h"
#include "bluetooth_sync.h"
#include "bt_mw_spp_cli.h"
#include "c_bt_mw_spp.h"
#include "bt_mw_spp.h"


/**
 * FUNCTION NAME: _bluetooth_bt_spp_connect_cli
 * PURPOSE:
 *      The function is used to SPP connect.
 * INPUT:
 *      argc               -- paramter number
 *      argv[0]            -- spp_connect
 *      argv[1]            -- bd address
 *      argv[2]            -- uuid
 * OUTPUT:
 *      None
 * RETURN:
 *      BT_SUCCESS                        -- Operate success.
 *      BT_ERR_STATUS_FAIL                -- Operate fail.
 * NOTES:
 *      None
 */
INT32 _bluetooth_bt_spp_connect_cli (INT32 argc, const CHAR **argv)
{
    CHAR str_addr[18]={0};
    CHAR str_uuid[37]={0};

    if (argc == 3)
    {
        memset(str_uuid, 0, 37);
        strncpy(str_uuid, argv[2], 36);

        memset(str_addr, 0, 18);
        strncpy(str_addr, argv[1], 17);
        c_btm_spp_connect(str_addr, str_uuid);
    }
    else
    {
        BT_DBG_ERROR(BT_DEBUG_SPP, "input error, spp_connect [MAC ADDRESS][optional:00001101-0000-1000-8000-00805F9B34FB]");
    }

    return 0;
}

/**
 * FUNCTION NAME: _bluetooth_bt_spp_disconnect_cli
 * PURPOSE:
 *      The function is used to SPP disconnect.
 * INPUT:
 *      argc               -- paramter number
 *      argv[0]            -- spp_disconnect
 *      argv[1]            -- port id
 * OUTPUT:
 *      None
 * RETURN:
 *      BT_SUCCESS                        -- Operate success.
 *      BT_ERR_STATUS_FAIL                -- Operate fail.
 * NOTES:
 *      None
 */
INT32 _bluetooth_bt_spp_disconnect_cli (INT32 argc, const CHAR **argv)
{
    CHAR str_addr[18]={0};
    CHAR str_uuid[37]={0};

    if (argc == 3)
    {
        memset(str_uuid, 0, 37);
        strncpy(str_uuid, argv[2], 36);

        memset(str_addr, 0, 18);
        strncpy(str_addr, argv[1], 17);
        c_btm_spp_disconnect(str_addr, str_uuid);
    }
    else
    {
        BT_DBG_ERROR(BT_DEBUG_SPP, "input error, spp_disconnect [MAC ADDRESS][optional:00001101-0000-1000-8000-00805F9B34FB]");
    }

    return 0;
}

/**
 * FUNCTION NAME: _bluetooth_bt_spp_send_data_cli
 * PURPOSE:
 *      The function is used to SPP send data.
 * INPUT:
 *      argc               -- paramter number
 *      argv[0]            -- spp_send_data
 *      argv[1]            -- port id or uuid according to argv[3].
 *      argv[2]            -- data
 *      argv[3]            -- 0 using port id, else using uuid.
 * OUTPUT:
 *      None
 * RETURN:
 *      BT_SUCCESS                        -- Operate success.
 *      BT_ERR_STATUS_FAIL                -- Operate fail.
 * NOTES:
 *      None
 */
INT32 _bluetooth_bt_spp_send_data_cli (INT32 argc, const CHAR **argv)
{
    CHAR str_data[512]={0};
    //UINT8 port=0;
    INT32 len;
    CHAR str_addr[18]={0};
    CHAR str_uuid[37]={0};

    if (argc == 4)
    {
        memset(str_addr, 0, 18);
        strncpy(str_addr, argv[1], 17);

        memset(str_uuid, 0, 37);
        strncpy(str_uuid, argv[2], 36);

        memset(str_data, 0, sizeof(str_data));
        strncpy(str_data, argv[3], sizeof(str_data)-1);
        len = strlen(str_data);

        c_btm_spp_send_data(str_addr, str_uuid, str_data, len);
    }
    else
    {
        BT_DBG_ERROR(BT_DEBUG_SPP, "input error, spp_send_data [addr][UUID][STRING]");
    }

    return 0;
}

/**
 * FUNCTION NAME: _bluetooth_bt_spp_enable_devB_cli
 * PURPOSE:
 *      The function is used to enable deviceB.
 * INPUT:
 *      argc               -- paramter number
 *      argv[0]            -- spp_enable_devb
 *      argv[1]            -- uuid.
 * OUTPUT:
 *      None
 * RETURN:
 *      BT_SUCCESS                        -- Operate success.
 *      BT_ERR_STATUS_FAIL                -- Operate fail.
 * NOTES:
 *      None
 */
INT32 _bluetooth_bt_spp_enable_devB_cli (INT32 argc, const CHAR **argv)
{
    if (argc == 3)
    {
        CHAR uuid[48];
        CHAR name[MAX_NAME_LEN];
        memset(uuid, 0, sizeof(uuid));
        memset(name, 0, MAX_NAME_LEN);
        strncpy(name, argv[1], strlen(argv[1]));
        strncpy(uuid, argv[2], strlen(argv[2]));
        BT_DBG_INFO(BT_DEBUG_SPP, "Input name:%s", name);
        BT_DBG_INFO(BT_DEBUG_SPP, "Input uuid:%s", uuid);
        c_btm_spp_enable_devb(name, uuid);
    }
    else
    {
        BT_DBG_ERROR(BT_DEBUG_SPP, "input error, spp_enable_devb [uuid], enable SPP acceptor.");
    }

    return 0;
}

/**
 * FUNCTION NAME: _bluetooth_bt_spp_disable_devB_cli
 * PURPOSE:
 *      The function is used to disable deviceB.
 * INPUT:
 *      argc               -- paramter number
 *      argv[0]            -- spp_disable_devb
 *      argv[1]            -- uuid.
 * OUTPUT:
 *      None
 * RETURN:
 *      BT_SUCCESS                        -- Operate success.
 *      BT_ERR_STATUS_FAIL                -- Operate fail.
 * NOTES:
 *      None
 */
INT32 _bluetooth_bt_spp_disable_devB_cli (INT32 argc, const CHAR **argv)
{
    if (argc == 2)
    {
        CHAR uuid[48];
        memset(uuid, 0, sizeof(uuid));
        strncpy(uuid, argv[1], strlen(argv[1]));
        BT_DBG_INFO(BT_DEBUG_SPP, "Input uuid:%s", uuid);
        c_btm_spp_disable_devb(uuid);
    }
    else
    {
        BT_DBG_ERROR(BT_DEBUG_SPP, "input error, spp_disable_devb [uuid], disable SPP acceptor");
    }

    return 0;
}


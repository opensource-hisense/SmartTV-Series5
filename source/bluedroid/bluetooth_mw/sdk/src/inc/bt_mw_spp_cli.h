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

/* FILE NAME:  bt_mw_spp_cli.h
 * AUTHOR: Jigong Yin
 * PURPOSE:
 *      It provides SPP operation command definition.
 * NOTES:
 */


/*-----------------------------------------------------------------------------
                    include files
 ----------------------------------------------------------------------------*/

#ifndef __BT_MW_SPP_CLI_H__
#define __BT_MW_SPP_CLI_H__


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
INT32 _bluetooth_bt_spp_connect_cli (INT32 argc, const CHAR **argv);

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
INT32 _bluetooth_bt_spp_disconnect_cli (INT32 argc, const CHAR **argv);

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
INT32 _bluetooth_bt_spp_send_data_cli (INT32 argc, const CHAR **argv);

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
INT32 _bluetooth_bt_spp_enable_devB_cli (INT32 argc, const CHAR **argv);

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
INT32 _bluetooth_bt_spp_disable_devB_cli (INT32 argc, const CHAR **argv);

#endif /*__BT_MW_SPP_CLI_H__*/


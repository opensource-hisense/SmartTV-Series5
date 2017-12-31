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
/* FILE NAME:  bt_mw_gap_cli.h
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
#ifndef BT_MW_GAP_CLI_H
#define BT_MW_GAP_CLI_H
/* INCLUDE FILE DECLARATIONS
 */
/* NAMING CONSTANT DECLARATIONS
 */
/* MACRO FUNCTION DECLARATIONS
 */
/* DATA TYPE DECLARATIONS
 */
/* EXPORTED SUBPROGRAM SPECIFICATIONS
 */

INT32 _bluetooth_inquiry_one_cli (INT32 argc, const CHAR **argv);
INT32 _bluetooth_set_name_cli (INT32 argc, const CHAR **argv);
INT32 _bluetooth_scan_cli (INT32 argc, const CHAR **argv);
INT32 _bluetooth_stop_scan_cli (INT32 argc, const CHAR **argv);
INT32 _bluetooth_set_connectable_and_discoverable_cli(INT32 argc, const CHAR **argv);
INT32 _bluetooth_get_local_dev_info_cli(INT32 argc, const CHAR **argv);
INT32 _bluetooth_get_rssi_cli (INT32 argc, const CHAR **argv);
INT32 _bluetooth_stop_inquiry_cli (INT32 argc, const CHAR **argv);
INT32 _bluetooth_pair_cli (INT32 argc, const CHAR **argv);
INT32 _bluetooth_remove_paired_dev_cli(INT32 argc, const CHAR **argv);
INT32 _bluetooth_set_virtual_sniffer_cli(INT32 argc, const CHAR **argv);
//INT32 _bluetooth_gap_start_sniff_mode_handler(INT32 argc, const CHAR **argv);
INT32 _bluetooth_send_hci(INT32 argc, const CHAR **argv);
INT32 _bluetooth_gap_pts_enable_cli (INT32 argc, const CHAR **argv);
INT32 _bluetooth_set_afh_cli (INT32 argc, const CHAR **argv);

#if defined(MTK_LINUX_GAP) && (MTK_LINUX_GAP == TRUE)
INT32 _bluetooth_config_clear(INT32 argc, const CHAR **argv);
INT32 _bluetooth_dump(INT32 argc, const CHAR **argv);
INT32 _bluetooth_interop_database_add(INT32 argc, const CHAR **argv);
INT32 _bluetooth_interop_database_clear(INT32 argc, const CHAR **argv);
INT32 _bluetooth_ble_pair(INT32 argc, const CHAR **argv);
#endif

#endif /* End of BT_MW_GAP_CLI_H */


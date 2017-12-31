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

/* FILE NAME:  bt_mw_log.h
 * AUTHOR:
 * PURPOSE:
 *      It provides bluetooth mw log structure to MW.
 * NOTES:
 */


#ifndef _BT_MW_LOG_H_
#define _BT_MW_LOG_H_


/* Define trace levels */
#define BT_MW_TRACE_LEVEL_NONE    0          /* No trace messages to be generated    */
#define BT_MW_TRACE_LEVEL_ERROR   1          /* Error condition trace messages       */
#define BT_MW_TRACE_LEVEL_WARNING 2          /* Warning condition trace messages     */
#define BT_MW_TRACE_LEVEL_API     3          /* API traces                           */
#define BT_MW_TRACE_LEVEL_EVENT   4          /* Debug messages for events            */
#define BT_MW_TRACE_LEVEL_DEBUG   5          /* Full debug messages                  */
#define BT_MW_TRACE_LEVEL_VERBOSE 6          /* Verbose debug messages               */

#define MAX_MW_TRACE_LEVEL        6

#define BT_MW_TRC_ID_START                   1
#define BT_MW_TRC_ID_END                     10


typedef struct {
    const char                       *trc_name;
    UINT8                            trace_level;
}BT_MW_TRC_MAP;

extern BT_MW_TRC_MAP btmw_trc_map[];
void log_write(const char *format, ...);
void bt_mw_log_setStackLevel(const char *section_name, int level);

/* define traces for application */
#define BT_DBG_ERROR(tag, fmt, args...)          {if (btmw_trc_map[tag].trace_level >= BT_MW_TRACE_LEVEL_ERROR)      log_write("%s<E>:%s@%d " fmt "\n", btmw_trc_map[tag].trc_name, __func__, __LINE__, ## args);}
#define BT_DBG_WARNING(tag, fmt, args...)        {if (btmw_trc_map[tag].trace_level >= BT_MW_TRACE_LEVEL_WARNING)    log_write("%s<W>:%s@%d " fmt "\n", btmw_trc_map[tag].trc_name, __func__, __LINE__, ## args);}
#define BT_DBG_NORMAL(tag, fmt, args...)         {if (btmw_trc_map[tag].trace_level >= BT_MW_TRACE_LEVEL_API)        log_write("%s<N>:%s@%d " fmt "\n", btmw_trc_map[tag].trc_name, __func__, __LINE__, ## args);}
#define BT_DBG_NOTICE(tag, fmt, args...)         {if (btmw_trc_map[tag].trace_level >= BT_MW_TRACE_LEVEL_EVENT)      log_write("%s<T>:%s@%d " fmt "\n", btmw_trc_map[tag].trc_name, __func__, __LINE__, ## args);}
#define BT_DBG_INFO(tag, fmt, args...)           {if (btmw_trc_map[tag].trace_level >= BT_MW_TRACE_LEVEL_DEBUG)      log_write("%s<I>:%s@%d " fmt "\n", btmw_trc_map[tag].trc_name, __func__, __LINE__, ## args);}
#define BT_DBG_MINOR(tag, fmt, args...)          {if (btmw_trc_map[tag].trace_level >= BT_MW_TRACE_LEVEL_VERBOSE)    log_write("%s<M>:%s@%d " fmt "\n", btmw_trc_map[tag].trc_name, __func__, __LINE__, ## args);}


#endif /* _BT_MW_LOG_H_ */


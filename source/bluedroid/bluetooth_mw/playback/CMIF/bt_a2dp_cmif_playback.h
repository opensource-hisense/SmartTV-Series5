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
/* FILE NAME:  bt_a2dp_cmif_playback.h
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
#ifndef BT_A2DP_PLAYBACK_H
#define BT_A2DP_PLAYBACK_H
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


typedef struct
{
    BOOL fgPlayBackInit;    /* if playback is inited */
    BOOL fgAudioReset;      /* if reset audio        */
#if DISCARD_DATA_BEFORE_PLAY
    BOOL fgInterDiscard; /* internal discard data,TRUE discard data, FALSE not discard */
#endif
}BT_PLAYBACK_CB_T;

//extern INT32 g_remained_buf_size;

//extern BOOL fg_audio_reset;
extern HANDLE_T  h_bt_playback_mutex;
extern BT_PLAYBACK_CB_T g_bt_playback_cb;

INT32 bluetooth_a2dp_sink_player_init(VOID);
VOID bluetooth_a2dp_reconfig_pb_conf(VOID);
INT32 bluetooth_playback_init(INT32 trackFreq, INT32 channelType);
VOID bluetooth_a2dp_pb_stop(VOID);
VOID bluetooth_a2dp_pb_suspend (VOID);
VOID bluetooth_a2dp_pb_restart (VOID);
VOID bluetooth_a2dp_pb_close(VOID);

INT32 bt_reset_audio(VOID);
INT32 bt_track_change_reset_audio(VOID);
INT32 bluetooth_a2dp_pb_push_data(UINT8 *pPcmBuf, INT32 i4PcmLen);
BOOL bluetooth_a2dp_pb_getInit(VOID);
VOID bluetooth_a2dp_pb_setIntDiscardFg(BOOL fgIntDiscard);
BOOL bluetooth_a2dp_pb_getIntDiscardFg(VOID);
VOID bluetooth_a2dp_pb_setAudioResetFg(BOOL fgAudioReset);
BOOL bluetooth_a2dp_pb_getAudioResetFg(VOID);

#endif /* End of BT_A2DP_PLAYBACK_H */


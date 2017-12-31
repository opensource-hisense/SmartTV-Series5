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
/* FILE NAME:  bt_mw_a2dp_snk.h
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
#ifndef BT_MW_A2DP_SNK_H
#define BT_MW_A2DP_SNK_H

#include "bt_mw_a2dp_common.h"


#define IN_BUF_SIZE 2048*4//581
#define OU_BUF_SIZE 8192//5120


typedef struct bt_audio_dump_s
{
    CHAR dec_dump_path[BT_DUMP_DATA_MAX_PATH_SIZE];
    CHAR pcm_dec_path[BT_DUMP_DATA_MAX_PATH_SIZE];
    BOOL dec_dump_enable;
    FILE *dec_dump_fp;
    FILE *pcm_dec_fp;
}bt_dec_audio_dump_t;

typedef struct
{
    UINT8 u1InBuf[IN_BUF_SIZE];
    UINT32 u4InLen;
    INT32  i4LeftLen; //  the remain data length in u1InBuf
}BT_A2DP_SNK_CB_T;

extern INT32 custom_in_buf_size;
extern INT32 custom_ou_buf_size;

extern struct timeval g_INT32o_write_stream_tv;

//VOID bluetooth_a2dp_playback_init_ext(VOID);
void bluetooth_a2dp_playback_deinit(void);
INT32 bluetooth_a2dp_sink_enable(UINT8 sink_enable);
INT32 bluetooth_a2dp_sink_open_cb(VOID);
INT32 bluetooth_a2dp_sink_start_cb(VOID);
INT32 bluetooth_a2dp_sink_suspend_cb(VOID);
VOID bluetooth_a2dp_sink_reconfig_cb(UINT32 freq, UINT8 channel);
INT32 bluetooth_a2dp_sink_close_cb(CHAR *device_addr);
INT32 bluetooth_a2dp_register_player_cb(BT_PLAYER_SNK_CB_FUNC *func);

#endif /* End of BT_MW_A2DP_SNK_H */




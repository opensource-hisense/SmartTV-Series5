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
/* FILE NAME:  bt_mw_a2dp_common.h
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
#ifndef BT_MW_A2DP_COMMON_H
#define BT_MW_A2DP_COMMON_H
/* INCLUDE FILE DECLARATIONS
 */


/* NAMING CONSTANT DECLARATIONS
 */
/* MACRO FUNCTION DECLARATIONS
 */
/* DATA TYPE DECLARATIONS
 */



#include "bluetooth.h"

#include "bt_mw_common.h"
#include "bt_mw_gap.h"
//#include "bluetooth_a2dp_struct.h"
#include <bt_av.h>
#include "bluetooth_sync.h"
//#include "linuxbt_a2dp_if.h"
//#include "linuxbt_common.h"



#include "u_bluetooth.h"


#define AAC_BUFFER_SIZE         (1024+4)
#define INPUT_BUFFER_SIZE       (3*2048)//(2*2048)
#define OUTPUT_BUFFER_SIZE      (1024+4)

#define BT_DUMP_DATA_MAX_PATH_SIZE 128
#define BYTESIZE    8 /* Number of Bits in a Byte */
#define WORD_BYTE   1 /*  8bits/sample 2's Complement Integer Data */
#define WORD_SHORT  2 /* 16bits/sample 2's Complement Integer Data */
#define WORD_MIDDLE 3 /* 24bits/sample 2's Complement Integer Data */
#define WORD_LONG   4 /* 32bits/sample 2's Complement Integer Data */

typedef UINT8 CodecType;

#define CODEC_SBC           0x00
#define CODEC_MPEG_1_2_AAC  0x01
#define CODEC_MPEG_2_4_AAC  0x02

#define SBC_48000           0x01
#define SBC_44100           0x02
#define SBC_32000           0x04
#define SBC_16000           0x08

#define BLOCK_LENGTH_4      (1 << 3)
#define BLOCK_LENGTH_8      (1 << 2)
#define BLOCK_LENGTH_12     (1 << 1)
#define BLOCK_LENGTH_16     (1 << 0)

#define SBC_CHANNEL_MODE_MONO           (1 << 3)
#define SBC_CHANNEL_MODE_DUAL_CHANNEL   (1 << 2)
#define SBC_CHANNEL_MODE_STEREO         (1 << 1)
#define SBC_CHANNEL_MODE_JOINT_STEREO   (1 << 0)

#define SBC_FILE_PATH BT_TMP_PATH"/after_decode_sbc.wav"
#define AAC_FILE_PATH BT_TMP_PATH"/after_decode_aac.wav"
#define LDAC_FILE_PATH BT_TMP_PATH"/after_decode_ldac.wav"
#define SBC_FILE_NAME "after_decode_sbc.wav"
#define AAC_FILE_NAME "after_decode_aac.wav"
#define BEFORE_FILE_NAME "before_decode.data"

#define ENC_SBC_FILE_PATH           BT_TMP_PATH"/after_encode_sbc.sbc"
#define ENC_AAC_FILE_PATH           BT_TMP_PATH"/after_encode_aac.aac"
#define ENC_LDAC_FILE_PATH          BT_TMP_PATH"/after_encode.ldac"
#define ENC_PCM_FILE_PATH           BT_TMP_PATH"/before_encode.pcm"
#define ENC_PCM_UPLOAD_FILE_PATH   BT_TMP_PATH"/upload_pcm.pcm"
#define ENC_SBC_FILE_NAME           "after_encode.sbc"
#define ENC_AAC_FILE_NAME           "after_encode.aac"
#define ENC_LDAC_FILE_NAME          "after_encode.ldac"
#define ENC_PCM_FILE_NAME           "before_encode.pcm"
#define ENC_PCM_UPLOAD_FILE_NAME   "upload_pcm.pcm"
#define ENC_WAV_FILE_PATH           "before_encode.wav"





typedef struct bt_enc_audio_dump_s
{
    CHAR enc_dump_path[BT_DUMP_DATA_MAX_PATH_SIZE];
    CHAR pcm_enc_path[BT_DUMP_DATA_MAX_PATH_SIZE];
#ifdef DUMP_UPLOADER_DATA
    CHAR pcm_upload_path[BT_DUMP_DATA_MAX_PATH_SIZE];
    FILE *pcm_upload_fp;
#endif
    BOOL enc_dump_enable;
    FILE *enc_dump_fp;
    FILE *pcm_enc_fp;
}bt_enc_audio_dump_t;



//extern BT_AUDIO_CODEC_TYPE g_current_codec_type;
extern UINT8 g_scms_t_value;

#if ENABLE_LDAC_CODEC
/*the sample rate which ldac decode configuration has configurated*/
extern UINT8 fg_bt_ldac_config_sample_rate;
#endif

extern CHAR bt_last_device_addr[18];
extern CHAR bt_2nd_device_addr[18];
//extern btav_audio_cap_struct bt_current_config;
extern BT_TARGET_DEV_INFO g_bt_target_dev_info;

INT32 bluetooth_a2dp_connect(CHAR *addr, UINT8 local_role);
INT32 bluetooth_a2dp_connect_sync(CHAR *addr);
INT32 bluetooth_a2dp_disconnect(CHAR *addr, UINT8 local_role);
INT32 bluetooth_a2dp_disconnect_sync(VOID);

INT32 bluetooth_a2dp_start(CHAR *addr);
INT32 bluetooth_a2dp_suspend(CHAR *addr);
INT32 bluetooth_a2dp_abort(CHAR *addr);
INT32 bluetooth_a2dp_reconfig(CHAR *addr,
                              INT32 sample_rate,
                              INT32 channel_num);
INT32 bluetooth_a2dp_src_enable(UINT8 src_enable);
INT32 bluetooth_a2dp_aac_enable(UINT8 fg_enable);
INT32 bluetooth_a2dp_ldac_enable(UINT8 fg_enable);
INT32 bluetooth_a2dp_aptx_enable(UINT8 fg_enable);


INT32 bt_get_last_device_addr(CHAR *addr);

BOOL bt_audio_is_stream_suspend(VOID);

INT32 bt_audio_set_stream_suspend(BOOL fg_suspend);
VOID btaudio_handle_a2dp_stream_qos_ind(UINT8 qos);

/*start for set/get UI codec type*/
BT_AUDIO_CODEC_TYPE btaudio_get_codec_type(VOID);
UINT8 btaudio_get_enabled_codec(VOID);

/*end for set/get UI codec type*/

INT32 btaudio_get_current_target_dev_info(BT_TARGET_DEV_INFO *pt_target_dev_info);

INT32 btaudio_handle_a2dp_stream_open_cb(CHAR *device_addr);

INT32 btaudio_handle_a2dp_stream_start_cb(VOID);

INT32 btaudio_handle_a2dp_stream_suspend_cb(VOID);
INT32 btaudio_handle_a2dp_scmst_support_cb(BOOL bt_scmst_support);
INT32 btaudio_handle_a2dp_stream_close_cb(CHAR *device_addr);
INT32 bluetooth_a2dp_2nd_con_enable(UINT8 fg_enable);

INT32 bluetooth_a2dp_get_audio_cap_info(VOID);

VOID bluetooth_a2dp_get_cur_audio_info(UINT32 *u4SampleRate, UINT32 *u4ChannelCnt);

INT32 bluetooth_a2dp_init(VOID);

INT32 bluetooth_a2dp_deinit(VOID);

INT32 bluetooth_a2dp_get_sbc_info(UINT32 *u4SampleRate, UINT32 *u4ChannelCnt, UINT32 *u4Bitdepth);

INT32 bluetooth_a2dp_getUdiskPath(CHAR *szUPath);
INT32 bluetooth_a2dp_dump_data(CHAR *pbuf, INT32 i4length, FILE *pfp);
VOID bluetooth_a2dp_save_audio_cap_info(UINT32 sample_rate, UINT8 channel_count);
INT32 btaudio_handle_a2dp_stream_reconfig_cb(UINT32 freq, UINT8 channel);
INT32 btaudio_handle_a2dp_second_connection_cb(CHAR *device_addr);
VOID bt_print_codec_info(BT_AUDIO_CODEC_TYPE i4current_codec_type);

/* EXPORTED SUBPROGRAM SPECIFICATIONS
 */
#endif /* End of BT_MW_A2DP_COMMON_H */


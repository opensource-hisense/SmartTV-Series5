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

/* FILE NAME:  bt_mw_a2dp_cli.c
 * PURPOSE:
 *      It provides A2DP test cmd.
 * NOTES:
 */
#if 0
#include <pthread.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

//#include "x_cli.h"
#include "bt_mw_common.h"
#include "bluetooth_sync.h"
//#include "bt_types.h"
#include "mtk_bluetooth.h"
#include <hardware/bt_av.h>
#include "c_bt_mw_gap.h"
#include "c_bt_mw_a2dp_common.h"
#include "bt_mw_avrcp.h"
#include "linuxbt_a2dp_if.h"

#if ENABLE_A2DP_SINK
//#include "sbc_dec_exp.h"
//#include "bt_a2dp_sbc_dec.h"
#include <hardware/bt_audio_track.h>
#include "bt_mw_a2dp_snk.h"
//#include "bt_a2dp_cmif_playback.h"
#include "c_bt_mw_a2dp_snk.h"

#endif

#if ENABLE_A2DP_SRC
#include "bt_mw_a2dp_src.h"
//#include "sbc.h"
//#include "bt_a2dp_sbc_enc.h"
#endif


/*-----------------------------------------------------------------------------
                    variable declarations
 -----------------------------------------------------------------------------*/

#if ENABLE_A2DP_SINK
extern bt_dec_audio_dump_t* dec_audio_dump_conf;
INT32 optimise_latency = 1;
extern BT_PLAYER_SNK_CB_FUNC player_func;
#endif
#if ENABLE_A2DP_SRC
extern bt_enc_audio_dump_t* enc_audio_dump_conf;
static INT32 bt_send_audio_data_thread_terminate = 0;
static pthread_t bt_send_audio_data_thread;
static INT32 bt_send_audio_data_stream_handle;
CHAR bt_local_test_pcm_file[128] = "/mnt/usb/sda/music/48000/input_48000.pcm";
INT32 g_bt_a2dp_aac_bitrate;
extern INT32 linuxbt_a2dp_get_status( INT32* sample_rate,
                               INT32* channel_num,
                               INT32* prefer_size,
                               INT32* bitpool,
                               INT32* allocation_method,
                               INT32* block_len,
                               INT32* sub_bands);
#endif

/*if AVDTP START is not triggered by opposite device, platform trigger AVDTP START after some time*/
UINT32 fg_bt_trigger_avdtp_start_delay;

/*-----------------------------------------------------------------------------
                    private methods implementations
 -----------------------------------------------------------------------------*/

INT32 _bluetooth_set_avdtp_start_delay__cli (INT32 argc, const CHAR **argv)
{
    if ( argc == 2 )
    {
        fg_bt_trigger_avdtp_start_delay= atoi(argv[1]);
        BT_DBG_NORMAL(BT_DEBUG_A2DP, "set avdtp start delay %ld", (long)fg_bt_trigger_avdtp_start_delay);
    }
    else
    {
        BT_DBG_NORMAL(BT_DEBUG_A2DP, "input error, set_avdtp_start_delay [integer]");
    }

    return 0;
}

INT32 _bluetooth_a2dp_set_audio_log_lvl_cli(INT32 argc, const CHAR **argv)
{
    UINT32 u4_log_lvl = 0;
    if (argc != 2)
    {
        BT_DBG_NORMAL(BT_DEBUG_A2DP, "A parameter is required!");
        return -1;
    }

    u4_log_lvl =  atoi(argv[1]);
    bluetooth_a2dp_set_audio_log_lvl(u4_log_lvl);

    return 0;
}

INT32 _bluetooth_audio_connect_cli (INT32 argc, const CHAR **argv)
{
    CHAR ps_addr[18];
    CHAR ps_type[6];
    BT_A2DP_ROLE a2dp_role;

    if (argc != 3 )
    {
        BT_DBG_NORMAL(BT_DEBUG_A2DP, "please input cc [MAC] [a2dp|avrcp]");
    }
    if (NULL == argv[1])
    {
        BT_DBG_NORMAL(BT_DEBUG_A2DP, "MAC is null");
        BT_DBG_NORMAL(BT_DEBUG_A2DP, "please input cc [MAC] [a2dp|avrcp]");
    }
    if (NULL == argv[2])
    {
        BT_DBG_NORMAL(BT_DEBUG_A2DP, "type is null");
        BT_DBG_NORMAL(BT_DEBUG_A2DP, "please input cc [MAC] [a2dp|avrcp]");
    }
    strncpy(ps_addr,argv[1], 18);
    ps_addr[17] = '\0';
    strncpy(ps_type,argv[2], 6);
    ps_type[5] = '\0';
    BT_DBG_NORMAL(BT_DEBUG_A2DP, "cc %s %s", ps_addr, ps_type);

    if (0 == strcmp(ps_type, "a2dp"))
    {
        if (!get_bt_a2dp_be_sink())
        {
            a2dp_role = A2DP_LOCAL_ROLE_SRC;
        }
        else
        {
            a2dp_role = A2DP_LOCAL_ROLE_SNK;
        }
        bluetooth_a2dp_connect(ps_addr, a2dp_role);
    }
    else if (0 == strcmp(ps_type, "avrcp"))
    {
        bluetooth_avrcp_connect(ps_addr);
    }
    else
    {
        BT_DBG_NORMAL(BT_DEBUG_A2DP, "please input cc [MAC] [a2dp|avrcp]");
    }
    return 0;
}

INT32 _bluetooth_audio_disconnect_cli (INT32 argc, const CHAR **argv)
{
    CHAR ps_addr[18];
    CHAR ps_type[6];
    if (argc != 3 )
    {
        BT_DBG_NORMAL(BT_DEBUG_A2DP, "please input dc [MAC] [a2dp|avrcp]");
    }
    if (NULL == argv[1])
    {
        BT_DBG_NORMAL(BT_DEBUG_A2DP, "MAC is null");
        BT_DBG_NORMAL(BT_DEBUG_A2DP, "please input dc [MAC] [a2dp|avrcp]");
    }
    if (NULL == argv[2])
    {
        BT_DBG_NORMAL(BT_DEBUG_A2DP, "type is null");
        BT_DBG_NORMAL(BT_DEBUG_A2DP, "please input dc [MAC] [a2dp|avrcp]");
    }
    strncpy(ps_addr,argv[1], 18);
    ps_addr[17] = '\0';
    strncpy(ps_type,argv[2], 6);
    ps_type[5] = '\0';
    BT_DBG_NORMAL(BT_DEBUG_A2DP, "dc %s %s", ps_addr, ps_type);

    if (0 == strcmp(ps_type, "a2dp"))
    {
        bluetooth_a2dp_disconnect(ps_addr, A2DP_LOCAL_ROLE_SRC);
    }
    else if (0 == strcmp(ps_type, "avrcp"))
    {
        bluetooth_avrcp_disconnect(ps_addr);
    }
    else
    {
        BT_DBG_NORMAL(BT_DEBUG_A2DP, "please input dc [MAC] [a2dp|avrcp]");
    }
    return 0;
}

INT32 _bluetooth_a2dp_cmd_cli (INT32 argc, const CHAR **argv)
{
    CHAR ps_addr[18];
    CHAR ps_type[10];
    INT32 sample_rate = 0;
    INT32 channel_num = 0;

    if (argc < 3 )
    {
        BT_DBG_NORMAL(BT_DEBUG_A2DP, "please input a2dp [MAC] [start|suspend|abort|reconfig][sample rate][channel]");
    }
    if (NULL == argv[1])
    {
        BT_DBG_NORMAL(BT_DEBUG_A2DP, "MAC is null");
        BT_DBG_NORMAL(BT_DEBUG_A2DP, "please input a2dp [MAC] [start|suspend|abort|reconfig][sample rate][channel]");
    }
    if (NULL == argv[2])
    {
        BT_DBG_NORMAL(BT_DEBUG_A2DP, "cmd type is null");
        BT_DBG_NORMAL(BT_DEBUG_A2DP, "please input a2dp [MAC] [start|suspend|abort|reconfig][sample rate][channel]");
    }
    strncpy(ps_addr,argv[1], 18);
    ps_addr[17] = '\0';
    strncpy(ps_type,argv[2], 10);
    ps_type[9] = '\0';
    BT_DBG_NORMAL(BT_DEBUG_A2DP, "a2dp %s %s", ps_addr, ps_type);

    if (0 == strcmp(ps_type, "start"))
    {
        bluetooth_a2dp_start(ps_addr);
    }
    else if (0 == strcmp(ps_type, "suspend"))
    {
        bluetooth_a2dp_suspend(ps_addr);
    }
    else if (0 == strcmp(ps_type, "abort"))
    {
        bluetooth_a2dp_abort(ps_addr);
    }
    else if (0 == strcmp(ps_type, "reconfig"))
    {
        sample_rate = atoi(argv[3]);
        channel_num = atoi(argv[4]);
        bluetooth_a2dp_reconfig(ps_addr, sample_rate, channel_num);
    }
    else
    {
        BT_DBG_NORMAL(BT_DEBUG_A2DP, "please input a2dp [MAC] [start|suspend|abort|reconfig][sample rate][channel]");
    }
    BT_DBG_NORMAL(BT_DEBUG_A2DP, "");
    return 0;
}

INT32 _bluetooth_a2dp_src_sink_enable_cli (INT32 argc, const CHAR **argv)
{
    BOOL source_role = FALSE;
    BOOL sink_role = FALSE;

    if (argc != 3 )
    {
        BT_DBG_NORMAL(BT_DEBUG_A2DP, "please input a2dp_src_sink [src:1|0] [sink:1|0]");
    }
    if (NULL == argv[1])
    {
        BT_DBG_NORMAL(BT_DEBUG_A2DP, "please input a2dp_src_sink [src:1|0] [sink:1|0]");
    }
    if (NULL == argv[2])
    {
        BT_DBG_NORMAL(BT_DEBUG_A2DP, "please input a2dp_src_sink [src:1|0] [sink:1|0]");
    }
    if(0 < atoi(argv[1]))
    {
        source_role = TRUE;
    }
    if(0 < atoi(argv[2]))
    {
        sink_role = TRUE;
    }

    BT_DBG_NORMAL(BT_DEBUG_A2DP, "a2dp src:%s sink%s", source_role?"enable":"disable",sink_role?"enable":"disable");
    c_btm_source_sink_on_off(source_role, sink_role);

    return 0;
}

INT32 _bluetooth_a2dp_codec_enable_cli (INT32 argc, const CHAR **argv)
{
    CHAR ps_codec_type[6];
    CHAR ps_enable[4];
    UINT8 fg_enable = 0;

    if (argc != 3 )
    {
        BT_DBG_NORMAL(BT_DEBUG_A2DP, "please input codec [codec type] [1|0]");
    }
    if (NULL == argv[1])
    {
        BT_DBG_NORMAL(BT_DEBUG_A2DP, "codec type is null");
        BT_DBG_NORMAL(BT_DEBUG_A2DP, "please input codec [aac");
#if ENABLE_LDAC_CODEC
        BT_DBG_NORMAL(BT_DEBUG_A2DP, "|ldac");
#endif
#if ENABLE_LDAC_CODEC
        BT_DBG_NORMAL(BT_DEBUG_A2DP, "|aptx");
#endif
        BT_DBG_NORMAL(BT_DEBUG_A2DP, "] [[1|0]\n]");

    }
    if (NULL == argv[2])
    {
        BT_DBG_NORMAL(BT_DEBUG_A2DP, "enable/disable is null");
        BT_DBG_NORMAL(BT_DEBUG_A2DP, "please input codec [codec type] [1|0]");


    }
    strncpy(ps_codec_type,argv[1], 6);
    ps_codec_type[5] = '\0';
    strncpy(ps_enable,argv[2], 4);
    ps_enable[3] = '\0';
    BT_DBG_NORMAL(BT_DEBUG_A2DP, "a2dp %s %s", ps_codec_type, ps_enable);
    if (0 == strcmp(ps_enable, "1"))
    {
        fg_enable = 1;
    }
    else if (0 == strcmp(ps_enable, "0"))
    {
        fg_enable = 0;
    }
    else
    {
        BT_DBG_NORMAL(BT_DEBUG_A2DP, "please input codec [codec type] [1|0]");
    }

    if (0 == strcmp(ps_codec_type, "aac"))
    {
        bluetooth_a2dp_aac_enable(fg_enable);
    }
#if ENABLE_LDAC_CODEC
    else if (0 == strcmp(ps_codec_type, "ldac"))
    {
        bluetooth_a2dp_ldac_enable(fg_enable);
    }
#endif
#if ENABLE_APTX_CODEC
    else if (0 == strcmp(ps_codec_type, "aptx"))
    {
        bluetooth_a2dp_aptx_enable(fg_enable);
    }
#endif
    else
    {
        BT_DBG_NORMAL(BT_DEBUG_A2DP, "codec type error");
        BT_DBG_NORMAL(BT_DEBUG_A2DP, "please input codec [aac");
#if ENABLE_LDAC_CODEC
        BT_DBG_NORMAL(BT_DEBUG_A2DP, "|ldac");
#endif
#if ENABLE_LDAC_CODEC
        BT_DBG_NORMAL(BT_DEBUG_A2DP, "|aptx");
#endif
        BT_DBG_NORMAL(BT_DEBUG_A2DP, "] [[1|0]\n]");
    }
    return 0;
}

INT32 _bluetooth_a2dp_enabled_codec_cli (INT32 argc, const CHAR **argv)
{
    UINT8 u1_codec_type = 0;

    u1_codec_type= btaudio_get_enabled_codec();
    BT_DBG_NORMAL(BT_DEBUG_A2DP, "u1_codec_type:%d", u1_codec_type);
    if (u1_codec_type & BT_AUDIO_AAC)
    {
        BT_DBG_NORMAL(BT_DEBUG_A2DP, "AAC enabled");
    }
    else
    {
        BT_DBG_NORMAL(BT_DEBUG_A2DP, "AAC disabled");
    }
    if (u1_codec_type & BT_AUDIO_LDAC)
    {
        BT_DBG_NORMAL(BT_DEBUG_A2DP, "LDAC enabled");
    }
    else
    {
        BT_DBG_NORMAL(BT_DEBUG_A2DP, "LDAC disabled");
    }
    return 0;
}

#if ENABLE_LDAC_CODEC
INT32 _bluetooth_a2dp_ldac_enable_cli(INT32 argc, const CHAR **argv)
{
    INT32  flag;

    if (argc != 2)
    {
        BT_DBG_NORMAL(BT_DEBUG_A2DP, "A parameter is required!");
        return -1;
    }

    BT_DBG_NORMAL(BT_DEBUG_A2DP, "LDAC Enable: %s", argv[1]);
    flag = atoi(argv[1]);

    c_btm_set_ldac_enable(flag);
    return 0;
}



/*******************************************************************************
    rint out the filename only, deleting the path.  Returns a pointer to
    the start of the filename.
*******************************************************************************/
CHAR *delpath(CHAR *str)
{
    CHAR *ptr;
    if ((ptr = strrchr(str, '\\')) == (CHAR *)NULL)
    {
        return str;
    }
    else
    {
        return ptr+1;
    }
}

/******************************************************************************
    Open LDAC file for read
******************************************************************************/
FILE *fopen_ldac_read( CHAR *in_file, INT32 *sampling_freq, INT32 *channel_config_index, INT32 *channel )
{
    FILE *fp;
    /* Open Coded Bit Stream File */
    if ((fp = fopen(in_file, "rb")) == (FILE *)NULL)
    {
        perror(in_file);
        return NULL;
    }

    /* Read Codec Specific Info at Head of LDAC file */
    if (fread( &s_spec_info, sizeof(UINT8), LDAC_SPEC_INFO_SIZE, fp) != LDAC_SPEC_INFO_SIZE )
    {
        fprintf(stderr, "Error: Can not Get Codec Specific Info from file!");
        fclose(fp);
        return NULL;
    }

    /* check */
    if ( ( s_spec_info.vendor_id_0 != LDAC_VENDOR_ID0 ) ||
            ( s_spec_info.vendor_id_1 != LDAC_VENDOR_ID1 ) ||
            ( s_spec_info.vendor_id_2 != LDAC_VENDOR_ID2 ) ||
            ( s_spec_info.vendor_id_3 != LDAC_VENDOR_ID3 ) ||
            ( s_spec_info.vendor_codec_id_0 != LDAC_VENDOR_CODEC_ID0 ) ||
            ( s_spec_info.vendor_codec_id_1 != LDAC_VENDOR_CODEC_ID1 ) )
    {
        fprintf(stderr, "Error: Not a LDAC BT stream file!");
        fclose(fp);
        return NULL;
    }
    /* get sampling frequency */
    if ( s_spec_info.fs_info & LDAC_SAMPLING_FREQ_192000 )
    {
        *sampling_freq = 4*48000;
    }
    else if ( s_spec_info.fs_info & LDAC_SAMPLING_FREQ_176400 )
    {
        *sampling_freq = 4*44100;
    }
    else if ( s_spec_info.fs_info & LDAC_SAMPLING_FREQ_096000 )
    {
        *sampling_freq = 2*48000;
    }
    else if ( s_spec_info.fs_info & LDAC_SAMPLING_FREQ_088200 )
    {
        *sampling_freq = 2*44100;
    }
    else if ( s_spec_info.fs_info & LDAC_SAMPLING_FREQ_048000 )
    {
        *sampling_freq = 48000;
    }
    else if ( s_spec_info.fs_info & LDAC_SAMPLING_FREQ_044100 )
    {
        *sampling_freq = 44100;
    }
    else
    {
        fprintf(stderr, "Error: Unsupported FS INFO 0x%02x!", s_spec_info.fs_info);
        fclose(fp);
        return NULL;
    }
    /* get channel config index & channel */
    if ( s_spec_info.channel_mode & LDAC_CHANNEL_MODE_MONO )
    {
        *channel_config_index = LDAC_CCI_MONO;
        *channel = 1;
    }
    else if ( s_spec_info.channel_mode & LDAC_CHANNEL_MODE_DUAL_CHANNEL )
    {
        *channel_config_index = LDAC_CCI_DUAL_CHANNEL;
        *channel = 2;
    }
    else if ( s_spec_info.channel_mode & LDAC_CHANNEL_MODE_STEREO )
    {
        *channel_config_index = LDAC_CCI_STEREO;
        *channel = 2;
    }
    else
    {
        fprintf(stderr, "Error: Unsupported Channel Mode 0x%02x!", s_spec_info.channel_mode);
        fclose(fp);
        return NULL;
    }
    return fp;
}

#if ENABLE_A2DP_SINK
static VOID*  test_ldac(VOID *args)
{
    INT32   channel, wlength;
    INT32 sampling_freq = 96000;
    INT32 channel_config_index;
    INT32 nshift = 0;
    INT32 sampling_freq_out;
    CHAR *in_file;
    FILE *fpi = NULL;
    INT32   result, isf;
    //HANDLE_LDAC_BT h_ldac_data;
    INT32 n_read_bytes, used_bytes, wrote_bytes;
    INT32 total_used_bytes = 0;
    static INT32 remained_buf_bytes = 0;

    UINT8   a_pcm_out[LDAC_MAX_LSU*LDAC_PRCNCH*sizeof(INT32)];
    UINT8   a_stream[LDAC_MAX_NBYTES+2];
    //bt_a2dp_audio_cap_struct current_config;

    wlength = WORD_MIDDLE;
    in_file = ldac_filename;
    /* Open Coded Bit Stream File */
    if ((fpi = fopen_ldac_read( in_file, &sampling_freq, &channel_config_index, &channel )) == (FILE *)NULL)
    {
        BT_DBG_NORMAL(BT_DEBUG_A2DP, "Error: Can not read LDAC file!");
        goto FINISH;
    }

#ifdef ENABLE_LDAC_LOCAL_TEST
    ldac_all_dlopen_handle_init();
#endif
    /* Get LDAC Handler */
    if ((bt_ldac_dec_data_ctx.h_ldac_data = bt_ldac_all_dlopen_ctx.ldac_get_handle()) == (HANDLE_LDAC_BT)NULL)
    {
        BT_DBG_NORMAL(BT_DEBUG_A2DP, "Error: Can not Get LDAC Handle!");
        goto FINISH;
    }

    /* Initialize LDAC Handle as Decoder */
    if ( bt_ldac_all_dlopen_ctx.ldac_init_handle_decode( bt_ldac_dec_data_ctx.h_ldac_data, channel_config_index, sampling_freq, nshift, 0, 0 ) < 0 )
    {
        BT_DBG_NORMAL(BT_DEBUG_A2DP, "Error: Can not Get LDAC Handle!");
        goto FINISH;
    }

    if ( nshift == 0 )
    {
        sampling_freq_out = sampling_freq;
    }
    else if ( nshift > 0 )
    {
        sampling_freq_out = sampling_freq << nshift;
    }
    else
    {
        sampling_freq_out = sampling_freq >> -nshift;
    }

    /* Show Decoding Parameter */

    fprintf(stderr, "[IN]  LDAC Coded Bit Stream File : %s", delpath(in_file));
    fprintf(stderr, "    - Sampling Frequency        : %ld Hz", (long)sampling_freq);
    fprintf(stderr, "    - Channel                   : %ld ch (cci %s)", (long)channel, channel_config_index==LDAC_CCI_MONO?"MONO":
            channel_config_index==LDAC_CCI_DUAL_CHANNEL?"DUAL Channel":
            channel_config_index==LDAC_CCI_STEREO?"STEREO":"unknown");
    //fprintf(stderr, "[OUT] PCM Audio Data File       : %s", delpath(out_file));
    fprintf(stderr, "    - Bit Depth                 : %ld bit", (long)(BYTESIZE*wlength));
    if (nshift > 0)
    {
        fprintf(stderr, "    - Up Sampling Ratio         : %ld/1", (long)(1<<nshift));
    }
    else if (nshift < 0)
    {
        fprintf(stderr, "    - Down Sampling Ratio       : 1/%ld", (long)(1<<(-nshift)));
    }
    fprintf(stderr, "    - Sampling Frequency        : %ld Hz", (long)sampling_freq_out);
    fprintf(stderr, "    - Channel                   : %ld ch", (long)channel);

    /* Decoding */
    isf = 0;
    while (1)
    {

        /* For non-transport format (Read Maximum bytes at once ) */
        n_read_bytes = fread(&a_stream[remained_buf_bytes], sizeof(UINT8), LDAC_MAX_NBYTES-remained_buf_bytes, fpi);
        if (n_read_bytes <= 0)
        {
            BT_DBG_NORMAL(BT_DEBUG_A2DP, "read finished!");
            goto FINISH;
        }
        n_read_bytes += remained_buf_bytes;

        /* Decode Frame */
        remained_buf_bytes = n_read_bytes;
        used_bytes = 0;
        while (remained_buf_bytes - used_bytes >= 0)
        {
            result = bt_ldac_all_dlopen_ctx.ldac_decode( bt_ldac_dec_data_ctx.h_ldac_data,
                     &a_stream[n_read_bytes-remained_buf_bytes],
                     a_pcm_out,
                     wlength,
                     remained_buf_bytes,
                     &used_bytes,
                     &wrote_bytes );
            if (result)
            {
                INT32 error_code;
                error_code = bt_ldac_all_dlopen_ctx.ldac_get_error_code( bt_ldac_dec_data_ctx.h_ldac_data );
                BT_DBG_ERROR(BT_DEBUG_A2DP,"[ERR] At %ld-th su. error_code = %ld, %ld", (long)isf, LDACBT_HANDLE_ERR((long)error_code), LDACBT_BLOCK_ERR((long)error_code));
                /* Recovery Process */
                bt_ldac_all_dlopen_ctx.ldac_close_handle( bt_ldac_dec_data_ctx.h_ldac_data );
                /* Initialize LDAC Handle as Decoder */
                if ( bt_ldac_all_dlopen_ctx.ldac_init_handle_decode( bt_ldac_dec_data_ctx.h_ldac_data, channel_config_index, sampling_freq, nshift, 0, 0 ) < 0 )
                {
                    BT_DBG_ERROR(BT_DEBUG_A2DP,"Error: Can not Get LDAC Handle!");
                }

                memset(a_pcm_out, 0, sizeof(a_pcm_out));
            }
            total_used_bytes += used_bytes;
            remained_buf_bytes -= used_bytes;
            BT_DBG_INFO(BT_DEBUG_A2DP,"total_used_bytes:%ld", (long)total_used_bytes);
            BT_DBG_INFO(BT_DEBUG_A2DP,"remained_buf_bytes:%ld", (long)remained_buf_bytes);
            BT_DBG_INFO(BT_DEBUG_A2DP,"used_bytes:%ld", (long)used_bytes);
            BT_DBG_INFO(BT_DEBUG_A2DP,"wrote_bytes:%ld", (long)wrote_bytes);

            /* Write PCM Data */
            if ( wrote_bytes > 0)
            {
                //fwrite( a_pcm_out, sizeof(UINT8), wrote_bytes, fpo);
                bluetooth_pb_send_pcm(BT_AUDIO_LDAC, a_pcm_out, wrote_bytes, 0);
                //x_thread_delay(5);
            }
        }
        //remained_buf_size = n_read_bytes;
        memmove(a_stream, &a_stream[total_used_bytes], remained_buf_bytes);
        total_used_bytes = 0;
        //fseek(fpi, used_bytes-n_read_bytes, SEEK_CUR);

        isf++;
        if ( send_audio_data_thread_terminate )
            break;
    }

FINISH:
    /* Close Files */
    if (fpi)
    {
        fclose(fpi);
    }
    bt_ldac_all_dlopen_ctx.ldac_free_handle( bt_ldac_dec_data_ctx.h_ldac_data );
    return NULL;
}


INT32 _bluetooth_a2dp_test_ldac_cli(INT32 argc, const CHAR **argv)
{
    FUNC_ENTRY;

    INT32 i4_ret = 0;
    if ( argc == 2 )
    {
        memset(ldac_filename, 0, sizeof(ldac_filename));
        strncpy(ldac_filename, argv[1], sizeof(ldac_filename)-1);
    }
    else
    {
        BT_DBG_NORMAL(BT_DEBUG_A2DP, "A parameter is required!");
        return -1;
    }

    {
        INT32 version;
        version = bt_ldac_all_dlopen_ctx.ldac_get_version();
        fprintf(stderr, "\n=== %s : LDAC Decoder Simulation Program (version %ld.%02ld.%02ld) ===",
                delpath((CHAR*)argv[0]), (long)((version>>16)&0xFF), (long)((version>>8)&0xFF), (long)(version&0xFF) );
    }
    pthread_attr_t attr;
    i4_ret = pthread_attr_init(&attr);
    if (0 != i4_ret)
    {
        BT_DBG_ERROR(BT_DEBUG_A2DP,"pthread_attr_init i4_ret:%ld", (long)i4_ret);
        return i4_ret;
    }
    i4_ret = pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
    if (0 == i4_ret)
    {
        if (0 != (i4_ret = pthread_create(&h_ldac_thread,
                                          &attr,
                                          test_ldac,
                                          NULL)))
        {
            BT_DBG_ERROR(BT_DEBUG_A2DP,"pthread_create i4_ret:%ld", (long)i4_ret);
            assert(0);
        }
    }
    else
    {
        BT_DBG_ERROR(BT_DEBUG_A2DP,"pthread_attr_setdetachstate i4_ret:%ld", (long)i4_ret);
    }

    pthread_attr_destroy(&attr);
    BT_DBG_NORMAL(BT_DEBUG_A2DP,"create ldac decode thread ok!");
    return 0;
}



INT32 _bluetooth_a2dp_decode_ldac_cli(INT32 argc, const CHAR **argv)
{
    INT32 channel_idx=0;
    INT32 sample_rate=0;
    FILE *fp_ldac = NULL;
    FILE *fp_pcm = NULL;
    uint8 *ptr_pcm_buffer = NULL;
    INT32 n_read_bytes = 0;
    uint8 *ptr_ldac_stream = NULL;;//[LDAC_MAX_NBYTES+2];
    HANDLE_LDAC_BT ldacHandle;
    INT32 total_used_ldac_byte = 0, left_ldac_byte = 0;
    INT32 result1 = 0;

    memset(ldac_filename, 0, sizeof(ldac_filename));
    strncpy(ldac_filename, argv[3], sizeof(ldac_filename)-1);
    channel_idx=atoi(argv[1]);
    sample_rate=atoi(argv[2]);

    BT_DBG_NORMAL(BT_DEBUG_A2DP,"decode ldac channel idx=%ld, sample_rate=%ld file=%s",
                    (long)channel_idx, (long)sample_rate, ldac_filename);

    /* Open Coded Bit Stream File */
    if ((fp_ldac = fopen(ldac_filename, "rb")) == (FILE *)NULL)
    {
        BT_DBG_ERROR(BT_DEBUG_A2DP,"open ldac file %s failed", ldac_filename);
        return 0;
    }
    remove("/mnt/sda1/ldac_pcm.data");

    fp_pcm = fopen("/mnt/sda1/ldac_pcm.data", "w+");
    if (NULL == fp_pcm)
    {
        BT_DBG_ERROR(BT_DEBUG_A2DP," open ldac enc after file failed");
        return 0;
    }

    ptr_pcm_buffer = malloc(2*LDAC_MAX_LSU*LDAC_PRCNCH*sizeof(INT32));
    if (NULL == ptr_pcm_buffer)
    {
        BT_DBG_ERROR(BT_DEBUG_A2DP," allocate ldac test pcm buffer failed");
        return 0;
    }
    ptr_ldac_stream = malloc(LDAC_MAX_NBYTES+2);
    if (NULL == ptr_ldac_stream)
    {
        BT_DBG_ERROR(BT_DEBUG_A2DP," allocate ldac test buffer failed");
        return 0;
    }

    if (NULL == bt_ldac_all_dlopen_ctx.ldac_get_handle)
    {
        ldac_all_dlopen_handle_init();
    }

    ldacHandle = bt_ldac_all_dlopen_ctx.ldac_get_handle();
    if ( ldacHandle == NULL )
    {
        BT_DBG_ERROR(BT_DEBUG_A2DP,"ldac_get_handle() Failed!");
        return 0;
    }

    if (0 > bt_ldac_all_dlopen_ctx.ldac_init_handle_decode(ldacHandle,
            channel_idx, sample_rate, 0, 0, 0 ))
    {
        BT_DBG_ERROR(BT_DEBUG_A2DP," init ldac decoder failed");
        return 0;
    }

    total_used_ldac_byte = 0;
    left_ldac_byte = 0;
    while (1)
    {
        n_read_bytes = fread(&ptr_ldac_stream[left_ldac_byte], 1, LDAC_MAX_NBYTES-left_ldac_byte, fp_ldac);
        if (n_read_bytes <= 0)break;

        n_read_bytes += left_ldac_byte;
        BT_DBG_NORMAL(BT_DEBUG_A2DP," read ldac file data: %ld, left: %ld", (long)n_read_bytes, (long)left_ldac_byte);
        if (1)
        {
            INT32 used_pcm_byte = 0, used_ldac_byte = 0;
            INT32 error_code = 0;

            result1 = 0;
            total_used_ldac_byte = 0;
            while (n_read_bytes > total_used_ldac_byte)
            {
                result1 = bt_ldac_all_dlopen_ctx.ldac_decode(ldacHandle,
                          &ptr_ldac_stream[total_used_ldac_byte],
                          &ptr_pcm_buffer[0],
                          3,
                          n_read_bytes-total_used_ldac_byte,
                          &used_ldac_byte,
                          &used_pcm_byte );
                if (0 != result1)
                {
                    error_code = bt_ldac_all_dlopen_ctx.ldac_get_error_code(ldacHandle);
                    if (LDACBT_HANDLE_ERR(error_code) != LDACBT_ERR_INPUT_BUFFER_SIZE)
                    {
                        BT_DBG_ERROR(BT_DEBUG_A2DP,"ldac decode failed %ld", (long)result1);
                        BT_DBG_ERROR(BT_DEBUG_A2DP,"[ERR] handle err: %ld, block err: %ld",
                                       LDACBT_HANDLE_ERR((long)error_code), LDACBT_BLOCK_ERR((long)error_code));
                    }
                    else
                    {
                        BT_DBG_NORMAL(BT_DEBUG_A2DP," left ldac file data: %ld", (long)left_ldac_byte);
                        memmove(ptr_ldac_stream, &ptr_ldac_stream[total_used_ldac_byte], left_ldac_byte);
                        result1 = 0;
                    }
                    break;
                }

                total_used_ldac_byte += used_ldac_byte;
                left_ldac_byte = n_read_bytes - total_used_ldac_byte;
                if (used_pcm_byte > 0)
                {
                    fwrite((VOID*)ptr_pcm_buffer, 1, used_pcm_byte, fp_pcm);
                }
                //BT_DBG_NORMAL(BT_DEBUG_A2DP," decode ldac data: used ldac: %d, out pcm: %d, total ldac len: %d",
                //   used_ldac_byte, used_pcm_byte, total_used_ldac_byte);
            }
        }
        if (result1 != 0)break;
    }
    BT_DBG_NORMAL(BT_DEBUG_A2DP," ldac decoder test finish");

    if (fp_pcm != NULL)
    {
        fclose(fp_pcm);
    }

    if (fp_ldac != NULL)
    {
        fclose(fp_ldac);
    }

    if (ptr_pcm_buffer != NULL)
    {
        free(ptr_pcm_buffer);
    }
    if (ptr_ldac_stream != NULL)
    {
        free(ptr_ldac_stream);
    }
    bt_ldac_all_dlopen_ctx.ldac_free_handle(ldacHandle);

    return 0;
}

INT32 _bluetooth_ldac_set_quality_threshold(INT32 argc, const CHAR **argv)
{
    if (argc != 3)
    {
        BT_DBG_NORMAL(BT_DEBUG_A2DP, "two parameters are required!");
        return -1;
    }

    g_good_quality_threshold = atoi(argv[1]);
    g_bad_quality_threshold = atoi(argv[2]);

    BT_DBG_NORMAL(BT_DEBUG_A2DP, "good quality threshold:%ld", (long)g_good_quality_threshold);
    BT_DBG_NORMAL(BT_DEBUG_A2DP, "bad quality threshold:%ld", (long)g_bad_quality_threshold);

    return 0;
}
#endif /* ENABLE_A2DP_SINK */

INT32 _bluetooth_a2dp_set_qmode(INT32 argc, const CHAR **argv)
{
    //a2dp_set_qmode [0:H|1:S|2:M]
    if (argc != 2)
    {
        BT_DBG_NORMAL(BT_DEBUG_A2DP, "a parameters are required!");
        return -1;
    }

    bt_a2dp_data_ctx.ldac.q_mode = atoi(argv[1]);
    g_e_audio_quality = atoi(argv[1]);
    fgq_mode_init = TRUE;
    BT_DBG_NORMAL(BT_DEBUG_A2DP, "q_mode:%ld", (long)bt_a2dp_data_ctx.ldac.q_mode);

    return 0;
}

INT32 _bluetooth_a2dp_set_frameHeader(INT32 argc, const CHAR **argv)
{
    // "a2dp_set_frame_head [integer]",
    if (argc != 2)
    {
        BT_DBG_NORMAL(BT_DEBUG_A2DP, "a parameters are required!");
        return -1;
    }

    g_set_bad_data_in_frame_header = atoi(argv[1]);
    BT_DBG_NORMAL(BT_DEBUG_A2DP, "g_set_bad_data_in_frame_header:%lu", (UINT32)g_set_bad_data_in_frame_header);

    return 0;
}

static VOID a2dp_set_ldac_param(BT_A2DP_DATA_CTX *bt_a2dp_data)
{
    //if (bt_a2dp_data->audio_cap.codec_type == BT_A2DP_NON_A2DP)
    if ( 1 )
    {
        INT32 inBufSize = 0;
        INT32 outBufSize = 0;
        INT32 result;
        INT32 version;

        ldac_all_dlopen_handle_init();
        bt_a2dp_data->ldac.ldacHandle = bt_ldac_all_dlopen_ctx.ldac_get_handle();
        if ( bt_a2dp_data->ldac.ldacHandle == NULL )
        {
            BT_DBG_NORMAL(BT_DEBUG_A2DP, "ldac_get_handle() Failed!");
            return;
        }
#if 0
        if ( bt_a2dp_data->audio_cap.sep_type == BT_A2DP_SINK )
        {
            BT_DBG_NORMAL(BT_DEBUG_A2DP, "ldac_init_handle_decode(): ");
            BT_DBG_NORMAL(BT_DEBUG_A2DP, "    channel_config_index=%d", bt_a2dp_data->ldac.channel_config_index);
            BT_DBG_NORMAL(BT_DEBUG_A2DP, "    sampling_freq=%d", bt_a2dp_data->ldac.sampling_freq);
            BT_DBG_NORMAL(BT_DEBUG_A2DP, "    nshift=%d", bt_a2dp_data->ldac.nshift);
            result = ldac_init_handle_decode(bt_a2dp_data->ldac.ldacHandle,
                                             bt_a2dp_data->ldac.channel_config_index,
                                             bt_a2dp_data->ldac.sampling_freq,
                                             bt_a2dp_data->ldac.nshift, 0, 0 );
            if (  result < 0 )
            {
                BT_DBG_NORMAL(BT_DEBUG_A2DP, "ldac_init_handle_decode() Failed! (%d)", result);
                return;
            }

            inBufSize = LDAC_DEC_INPUT_BUF_SIZE;
            outBufSize = LDAC_DEC_OUTPUT_BUF_SIZE;
        }
        else
#endif
        {
            bt_a2dp_data->ldac.mtu = bt_a2dp_data->payload_size;
            if (!fgq_mode_init)
            {
                bt_a2dp_data->ldac.q_mode = 1;  // STANDARD_QUALITY
            }

//          bt_a2dp_data->ldac.channel = a2dp_get_channel_num(bt_a2dp_data);
            if ( bt_a2dp_data->ldac.sampling_freq == 96000
                    ||  bt_a2dp_data->ldac.sampling_freq == 88200 )
                bt_a2dp_data->ldac.wlength = 3;
            else
                bt_a2dp_data->ldac.wlength = 2;

//          bt_a2dp_data->ldac.sampling_freq = a2dp_get_sample_rate(bt_a2dp_data);


            BT_DBG_NORMAL(BT_DEBUG_A2DP, "ldac_init_handle_encode(): ");
            BT_DBG_NORMAL(BT_DEBUG_A2DP, "    mtu = %ld", (long)bt_a2dp_data->ldac.mtu);
            BT_DBG_NORMAL(BT_DEBUG_A2DP, "    q_mode = %ld", (long)bt_a2dp_data->ldac.q_mode);
            BT_DBG_NORMAL(BT_DEBUG_A2DP, "    channel = %ld", (long)bt_a2dp_data->ldac.channel);
            BT_DBG_NORMAL(BT_DEBUG_A2DP, "    wlength = %ld", (long)bt_a2dp_data->ldac.wlength);
            BT_DBG_NORMAL(BT_DEBUG_A2DP, "    sampling_freq = %ld", (long)bt_a2dp_data->ldac.sampling_freq);
            result = bt_ldac_all_dlopen_ctx.ldac_init_handle_encode(bt_a2dp_data->ldac.ldacHandle,
                     bt_a2dp_data->ldac.mtu,
                     bt_a2dp_data->ldac.q_mode,
                     bt_a2dp_data->ldac.channel,
                     bt_a2dp_data->ldac.wlength,
                     bt_a2dp_data->ldac.sampling_freq);

            if ( result < 0 )
            {
                BT_DBG_NORMAL(BT_DEBUG_A2DP, "ldac_init_handle_encode() Failed! (%ld)", (long)result);
                return;
            }
            else
                BT_DBG_NORMAL(BT_DEBUG_A2DP, "ldac_init_handle_encode() OK. (%ld)", (long)result);

            inBufSize = LDAC_ENC_INPUT_BUF_SIZE;
            outBufSize = LDAC_ENC_OUTPUT_BUF_SIZE;
        }
        version = bt_ldac_all_dlopen_ctx.ldac_get_version();

        BT_DBG_NORMAL(BT_DEBUG_A2DP,"=== LDAC Encoder Simulation Program (version %ld.%02ld.%02ld) ===",
                        (long)((version>>16)&0xFF), (long)((version>>8)&0xFF), (long)(version&0xFF) );

        bt_a2dp_data->ldac.OutBuf = malloc(outBufSize);
        if ( bt_a2dp_data->ldac.OutBuf == NULL )
        {
            BT_DBG_NORMAL(BT_DEBUG_A2DP, "[bt_a2dp_android] Allocate OutBuf fail");

            bt_ldac_all_dlopen_ctx.ldac_free_handle(bt_a2dp_data->ldac.ldacHandle);

            return;
        }
        bt_a2dp_data->ldac.InBuf = malloc(inBufSize);
        if ( bt_a2dp_data->ldac.InBuf == NULL )
        {
            BT_DBG_NORMAL(BT_DEBUG_A2DP, "[bt_a2dp_android] Allocate ldacInBuf fail");
            bt_ldac_all_dlopen_ctx.ldac_free_handle(bt_a2dp_data->ldac.ldacHandle);
            return;
        }
        memset(bt_a2dp_data->ldac.InBuf, 0, inBufSize);
        memset(bt_a2dp_data->ldac.OutBuf, 0, outBufSize);
        bt_a2dp_data->ldac.OutBufUsedBytes = 0;
        bt_a2dp_data->ldac.InBufUsedBytes = 0;
    }
}

static INT32 a2dp_ldac_Encode(ldac_t *ldac, const VOID *data, INT32 count)
{
    BT_DBG_MINOR(BT_DEBUG_A2DP,"+++into %s", __FUNCTION__);
    INT32 result;
    INT32 pcm_encoded_byte = 0;

    // buffer pointer error handling.
    if ( data && ldac->InBuf && ldac->OutBuf )
        {}
    else
    {
        BT_DBG_NORMAL(BT_DEBUG_A2DP, "ldac_Encode() error!  data=0x%lx, ldac->InBuf=0x%lx, ldac->OutBuf=0x%lx", (UINT32)data, (UINT32)ldac->InBuf, (UINT32)ldac->OutBuf);
        return -1;
    }
    // buffer size error handling.
    if ( ldac->InBufUsedBytes + count >= LDAC_ENC_INPUT_BUF_SIZE )
    {
        BT_DBG_NORMAL(BT_DEBUG_A2DP, "ldac_Encode() error! ldac->InBufUsedBytes=%ld, count=%ld, LDAC_ENC_INPUT_BUF_SIZE=%d",
                      (long)ldac->InBufUsedBytes,
                      (long)count,
                      LDAC_ENC_INPUT_BUF_SIZE);
        return -1;
    }

    //memset(ldac->OutBuf, 0, LDAC_ENC_OUTPUT_BUF_SIZE);
    ldac->OutBufUsedBytes = 0;
    //memset(ldac->InBuf, 0, LDAC_ENC_INPUT_BUF_SIZE);
    memcpy(ldac->InBuf+ldac->InBufUsedBytes, data, count);
    ldac->InBufUsedBytes += count;
    result = bt_ldac_all_dlopen_ctx.ldac_encode( ldac->ldacHandle,
             ldac->InBuf,
             &pcm_encoded_byte,
             (UINT8*)ldac->OutBuf,
             &ldac->OutBufUsedBytes,
             &ldac->encoded_frames_num );
    if (result)
    {
        BT_DBG_ERROR(BT_DEBUG_A2DP,"Error: LDAC analysis! code:%ld",(long)bt_ldac_all_dlopen_ctx.ldac_get_error_code(ldac->ldacHandle));
        /* Recovery Process */
        bt_ldac_all_dlopen_ctx.ldac_close_handle(ldac->ldacHandle);
        /* Initialize LDAC Handle as Decoder */
        if ( bt_ldac_all_dlopen_ctx.ldac_init_handle_encode(ldac->ldacHandle,
                ldac->mtu,
                ldac->q_mode,
                ldac->channel,
                ldac->wlength,
                ldac->sampling_freq) < 0 )
        {
            BT_DBG_NORMAL(BT_DEBUG_A2DP, "ldac_init_handle_encode Error: Can not Get LDAC Handle!");
        }

        memset(ldac->OutBuf, 0, sizeof(ldac->OutBuf));
        ldac->OutBufUsedBytes = 0;
        memset(ldac->InBuf, 0, sizeof(ldac->InBuf));
        ldac->InBufUsedBytes = 0;

        return -1;
    }

    // error handling for LDAC library.
    if ( pcm_encoded_byte > ldac->InBufUsedBytes )
        pcm_encoded_byte = ldac->InBufUsedBytes;

    memmove(ldac->InBuf, ldac->InBuf+pcm_encoded_byte, ldac->InBufUsedBytes-pcm_encoded_byte);
    ldac->InBufUsedBytes-=pcm_encoded_byte;

    return pcm_encoded_byte;
}

static UINT16 a2dp_enc_data_ldac(BT_A2DP_DATA_CTX *bt_a2dp_data, UINT8 stream_handle, const UINT8 *source, UINT16 src_len)
{
    BT_DBG_MINOR(BT_DEBUG_A2DP,"+++into %s", __FUNCTION__);
    INT32 remain_len = src_len;
    INT32 encode_buf_len = 128 * bt_a2dp_data->ldac.channel * bt_a2dp_data->ldac.wlength;
    INT32 pcm_sample_count =  0;
    INT32 used_buf_len = 0;
    UINT8* src_buf = (UINT8*)source;
    UINT16 total_encoded_len = 0;
    static INT32 total_pcm_len = 0;
    while ( remain_len > 0)
    {
        if ( remain_len < encode_buf_len )
            encode_buf_len = remain_len;
        used_buf_len = a2dp_ldac_Encode(&bt_a2dp_data->ldac, src_buf, encode_buf_len);
        if ( used_buf_len > 0 )
        {
            total_encoded_len += used_buf_len;
            BT_DBG_MINOR(BT_DEBUG_A2DP,"total_encoded_len %d", total_encoded_len);
            BT_DBG_MINOR(BT_DEBUG_A2DP,"OutBufUsedBytes %ld", (long)bt_a2dp_data->ldac.OutBufUsedBytes);
            // Add heading 1 byte for media payload header. (stack doesn't it.)(A2DP_SPEC_V1.3.pdf Figure 3.1 & 4.3)
            memcpy(bt_a2dp_data->enc_data+1, bt_a2dp_data->ldac.OutBuf, bt_a2dp_data->ldac.OutBufUsedBytes);
            bt_a2dp_data->enc_data_len = bt_a2dp_data->ldac.OutBufUsedBytes;
            bt_a2dp_data->sample_count = bt_a2dp_data->ldac.encoded_frames_num;
            bt_a2dp_data->enc_data[0] = bt_a2dp_data->sample_count;
            total_pcm_len += used_buf_len;
            if ( bt_a2dp_data->ldac.OutBufUsedBytes > 0 )
            {
                static UINT32 last_tx_time=0;
                UINT32 now = 0;
                UINT32 ldac_interval;

                now = bt_get_microseconds();
                ldac_interval = 1000 * (( 1000 * total_pcm_len ) /
                                        (bt_a2dp_data->ldac.channel * bt_a2dp_data->ldac.wlength * bt_a2dp_data->ldac.sampling_freq));

                if (!last_tx_time)
                {
                    last_tx_time = now;
                }
                else
                {
                    UINT32 wait = last_tx_time + ldac_interval - now;
                    if (wait > 0)
                    {
                        if ( wait > 10000 )
                        {
                            BT_DBG_NORMAL(BT_DEBUG_A2DP,"wait more than 100ms!, skip, don't sleep");
                            last_tx_time = now;
                        }
                        else
                        {
                            usleep(wait);
                            last_tx_time += ldac_interval;
                        }
                    }
                    else
                    {
                        last_tx_time = now;
                    }
                }

                pcm_sample_count = (total_pcm_len / (bt_a2dp_data->ldac.channel * bt_a2dp_data->ldac.wlength));
                linuxbt_send_stream_data((const CHAR*)bt_a2dp_data->enc_data, bt_a2dp_data->ldac.OutBufUsedBytes+1, pcm_sample_count);
                total_pcm_len = 0;
            }
            remain_len -= used_buf_len;
            src_buf += used_buf_len;

#ifdef A2DP_SRC_DUMP_TO_FILE
            if ( a2dp_src_ldac_fp == NULL )
                a2dp_src_ldac_fp = fopen(A2DP_SRC_DUMP_FILENAME_LDAC, "w+");

            if (!a2dp_src_ldac_fp)
            {
                BT_DBG_NORMAL(BT_DEBUG_A2DP, "dump ldac open file failed, path=[%s]!", A2DP_SRC_DUMP_FILENAME_LDAC);
            }
            else
            {
                if ( fwrite(bt_a2dp_data->ldac.OutBuf, 1, bt_a2dp_data->ldac.OutBufUsedBytes, a2dp_src_ldac_fp) != bt_a2dp_data->ldac.OutBufUsedBytes )
                {
                    BT_DBG_NORMAL(BT_DEBUG_A2DP, "write to ldac file error !");
                }
            }
#endif
        }
        else
        {
            BT_DBG_ERROR(BT_DEBUG_A2DP,"ldac_Encode return failed : %ld", (long)used_buf_len);
            break;
        }
    }
    return total_encoded_len;
}

#endif/* ENABLE_LDAC_CODEC */

#if ENABLE_A2DP_SINK

INT32 _bluetooth_optimise_latency_cli(INT32 argc, const CHAR **argv)
{
    if (1 < argc)
    {
        optimise_latency = atoi(argv[1]);
    }
    else
    {
        BT_DBG_NORMAL(BT_DEBUG_A2DP, "input error,optimise_latency [1|0]");
    }

    return 0;
}


INT32 _bluetooth_dec_dump_data_enable_cli(INT32 argc, const CHAR **argv)
{
    INT32 enable = 0;

    if (argc < 2)
    {
        BT_DBG_ERROR(BT_DEBUG_A2DP, "invaild argument!");
        BT_DBG_NORMAL(BT_DEBUG_A2DP, "please input: dec_dump_enable [0|1]");
        return BT_ERR_STATUS_PARM_INVALID;
    }
    if (!dec_audio_dump_conf)
    {
        dec_audio_dump_conf = (bt_dec_audio_dump_t *)malloc(sizeof(bt_dec_audio_dump_t));
        if (!dec_audio_dump_conf)
        {
            BT_DBG_ERROR(BT_DEBUG_A2DP, "%s: failed to malloc", __FUNCTION__);
            return BT_ERR_STATUS_NOMEM;
        }
        memset(dec_audio_dump_conf, 0, sizeof(bt_dec_audio_dump_t));
    }
    enable = atoi(argv[1]);
    if (enable)
    {
        BT_DBG_NORMAL(BT_DEBUG_A2DP, "enable dump a2dp data");
        dec_audio_dump_conf->dec_dump_enable = 1;
    }
    else
    {
        BT_DBG_NORMAL(BT_DEBUG_A2DP, "disable dump file");
        if (dec_audio_dump_conf->dec_dump_fp)
        {
            fclose(dec_audio_dump_conf->dec_dump_fp);
            dec_audio_dump_conf->dec_dump_fp = NULL;
        }
        if (dec_audio_dump_conf->pcm_dec_fp)
        {
            fclose(dec_audio_dump_conf->pcm_dec_fp);
            dec_audio_dump_conf->pcm_dec_fp = NULL;
        }

        dec_audio_dump_conf->dec_dump_enable = 0;
    }
    return BT_SUCCESS;
}

INT32 _bluetooth_audio_reset_cli (INT32 argc, const CHAR **argv)
{
    FUNC_ENTRY;

    c_btm_audio_reset();

    return 0;

}

INT32 _bluetooth_pb_cmd_cli (INT32 argc, const CHAR **argv)
{
    CHAR pb_cmd[8];
    if ( argc != 2 )
    {
        BT_DBG_NORMAL(BT_DEBUG_A2DP, "A parameter is required!");
        return -1;
    }
    strncpy(pb_cmd, argv[1], sizeof(pb_cmd));
    if (0 == strcmp(pb_cmd, "init"))
    {
        player_func.player_init_cb(44100, 2);
    }
    else if (0 == strcmp(pb_cmd, "uninit"))
    {
        player_func.player_deinit_cb();
    }
    else if (0 == strcmp(pb_cmd, "play"))
    {
        player_func.player_start_cb();
    }
    else if (0 == strcmp(pb_cmd, "pause"))
    {
        player_func.player_pause_cb();
    }
    else if (0 == strcmp(pb_cmd, "stop"))
    {
        player_func.player_stop_cb();
    }
    else
    {
        BT_DBG_NORMAL(BT_DEBUG_A2DP, "input error");
    }

    return 0;
}

#endif /* ENABLE_A2DP_SINK */
#if ENABLE_A2DP_SRC

INT32 _bluetooth_a2dp_set_aac_bitrate_cli(INT32 argc, const CHAR **argv)
{
    if (argc != 2)
    {
        BT_DBG_NORMAL(BT_DEBUG_A2DP, "A parameter is required!");
        return -1;
    }

    g_bt_a2dp_aac_bitrate =  atoi(argv[1]);
    BT_DBG_NORMAL(BT_DEBUG_A2DP, "aac bitrate: %ld", (long)g_bt_a2dp_aac_bitrate);

    return 0;
}

INT32 _bluetooth_enc_dump_data_enable_cli(INT32 argc, const CHAR **argv)
{
    INT32 enable = 0;

    if (argc < 2)
    {
        BT_DBG_ERROR(BT_DEBUG_A2DP, "invaild argument!");
        BT_DBG_NORMAL(BT_DEBUG_A2DP, "please input: enc_dump_enable [0|1]");
        return BT_ERR_STATUS_PARM_INVALID;
    }
    if (!enc_audio_dump_conf)
    {
        enc_audio_dump_conf = (bt_enc_audio_dump_t *)malloc(sizeof(bt_enc_audio_dump_t));
        if (!enc_audio_dump_conf)
        {
            BT_DBG_NORMAL(BT_DEBUG_A2DP, "%s: failed to malloc", __FUNCTION__);
            return BT_ERR_STATUS_FAIL;
        }
        memset(enc_audio_dump_conf, 0, sizeof(bt_enc_audio_dump_t));
    }
    enable = atoi(argv[1]);
    if (enable)
    {
        BT_DBG_NORMAL(BT_DEBUG_A2DP, "enable dump a2dp data");
        enc_audio_dump_conf->enc_dump_enable = 1;
    }
    else
    {
        BT_DBG_NORMAL(BT_DEBUG_A2DP, "disable dump file");
        if (enc_audio_dump_conf->enc_dump_fp)
        {
            fclose(enc_audio_dump_conf->enc_dump_fp);
            enc_audio_dump_conf->enc_dump_fp = NULL;
        }
        if (enc_audio_dump_conf->pcm_enc_fp)
        {
            fclose(enc_audio_dump_conf->pcm_enc_fp);
            enc_audio_dump_conf->pcm_enc_fp = NULL;
        }

#ifdef DUMP_UPLOADER_DATA
        if (enc_audio_dump_conf->pcm_upload_fp)
        {
            fclose(enc_audio_dump_conf->pcm_upload_fp);
            enc_audio_dump_conf->pcm_upload_fp = NULL;
        }
#endif
        enc_audio_dump_conf->enc_dump_enable = 0;
    }
    return BT_SUCCESS;
}

static VOID* _bluetooth_send_audio_data_thread(VOID *arg)
{
#define PCM_BUFFER_SIZE 512*7     // at least 512
    UINT8*pcm_buffer=NULL;
    CHAR* local_test_pcm_file;
    FILE *fInputPCM;
    INT32 result=0;

    INT16 pcm_frame_len = PCM_BUFFER_SIZE;
    UINT32 sample_count=0;   //reset when A2DP_START
    INT32 src_len;

    local_test_pcm_file = bt_local_test_pcm_file;


    BT_DBG_NORMAL(BT_DEBUG_A2DP, "Input file name  : %s", local_test_pcm_file);

    /* open file & allocate memory */
    fInputPCM = fopen(local_test_pcm_file, "rb" );
    if ( fInputPCM==NULL )
    {
        BT_DBG_ERROR(BT_DEBUG_A2DP, "Can't open input PCM file!");
        return NULL;
    }

    pcm_buffer = (UINT8 *)malloc(PCM_BUFFER_SIZE);
    if ( pcm_buffer == NULL )
    {
        fclose( fInputPCM );
        BT_DBG_NORMAL(BT_DEBUG_A2DP, "Can't allocat buffer for SBC encoding(PCM BUFFER)!");
        return NULL;
    }

    while ( 1 )
    {
        memset(pcm_buffer, 0, PCM_BUFFER_SIZE);

        src_len = fread(pcm_buffer, sizeof(UINT8), pcm_frame_len, fInputPCM );
        if ( src_len==0 )
        {
            goto send_audio_data_end;
        }

        result = linuxbt_send_stream_data((const CHAR*)pcm_buffer,src_len, sample_count);
        if ( result == 0 )
        {
            BT_DBG_NORMAL(BT_DEBUG_A2DP, "BtA2dpSendData Failed!");
            //goto send_audio_data_end;
        }
        else
        {
            BT_DBG_NORMAL(BT_DEBUG_A2DP, "send data successful. result:%ld   src_len:%ld", (long)result, (long)src_len);
            usleep(15000);
        }

        if ( bt_send_audio_data_thread_terminate )
            break;
    }
send_audio_data_end:

    fclose( fInputPCM );
    free(pcm_buffer);
    BT_DBG_NORMAL(BT_DEBUG_A2DP, "Send audio finished");
    return NULL;
}


static VOID _bluetooth_send_audio_data(UINT8 stream_handle)
{
    INT32 result;
    bt_send_audio_data_stream_handle = stream_handle;
    bt_send_audio_data_thread_terminate = 0;
    result = pthread_create(&bt_send_audio_data_thread, NULL, _bluetooth_send_audio_data_thread, NULL);

    if ( result )
    {
        BT_DBG_NORMAL(BT_DEBUG_A2DP, "pthread_create failed! (%ld)", (long)result);
    }

}

INT32 _bluetooth_a2dp_send_data_cli(INT32 argc, const CHAR **argv)
{
    if ( argc == 2 )
    {
        UINT8 handle;
        handle = atoi(argv[1]);
        _bluetooth_send_audio_data(handle);
    }
    else if ( argc == 3 )
    {
        UINT8 handle;
        handle = atoi(argv[1]);
        memset(bt_local_test_pcm_file, 0, sizeof(bt_local_test_pcm_file));
        memcpy(bt_local_test_pcm_file, argv[2], strlen(argv[2]));
        BT_DBG_NORMAL(BT_DEBUG_A2DP, "%s", bt_local_test_pcm_file);
        _bluetooth_send_audio_data(handle);
    }
    else
    {
        BT_DBG_NORMAL(BT_DEBUG_A2DP, "A parameter is required!");
        return -1;
    }

    return 0;
}


#endif /* ENABLE_A2DP_SRC */

#if ENABLE_MTK_TV_PLATFORM
INT32 _bluetooth_mute_speaker_cli(INT32 argc, const CHAR **argv)
{
    BOOL fg_mute = FALSE;
    if (argc < 2)
    {
        BT_DBG_NORMAL(BT_DEBUG_A2DP, "A parameter is required!");
        return -1;
    }
    fg_mute = (BOOL)atoi(argv[1]);
    bluetooth_a2dp_mute_speaker(fg_mute);

    return 0;
}
#endif
#endif

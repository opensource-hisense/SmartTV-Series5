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

/* FILE NAME:  bt_mw_a2dp_cli.h
 * AUTHOR: hongliang .hu
 * PURPOSE:
 *      It provides GATTC and GATTS operation command definition.
 * NOTES:
 */


/*-----------------------------------------------------------------------------
                    include files
 ----------------------------------------------------------------------------*/

#ifndef __BT_MW_A2DP_CLI_H__
#define __BT_MW_A2DP_CLI_H__

INT32 _bluetooth_a2dp_set_audio_log_lvl_cli(INT32 argc, const CHAR **argv);
INT32 _bluetooth_get_codec_type_cli(INT32 argc, const CHAR **argv);
INT32 _bluetooth_set_codec_type_cli(INT32 argc, const CHAR **argv);
INT32 _bluetooth_get_audio_codec_type_cli (INT32 argc, const CHAR **argv);
INT32 _bluetooth_audio_connect_cli (INT32 argc, const CHAR **argv);
INT32 _bluetooth_audio_disconnect_cli (INT32 argc, const CHAR **argv);
INT32 _bluetooth_a2dp_cmd_cli (INT32 argc, const CHAR **argv);
INT32 _bluetooth_a2dp_src_sink_enable_cli (INT32 argc, const CHAR **argv);
#if ENABLE_BUFFER_CONTROL
INT32 _bluetooth_enable_SBC_AAC_drift_compensation_cli(INT32 argc, const CHAR **argv);
INT32 _bluetooth_enable_detection_cli(INT32 argc, const CHAR **argv);
INT32 _bluetooth_set_ncomp_cli (INT32 argc, const CHAR **argv);
INT32 _bluetooth_set_drift_cli(INT32 argc, const CHAR **argv);
INT32 _bluetooth_set_g1_cli (INT32 argc, const CHAR **argv);
INT32 _bluetooth_set_g2_cli (INT32 argc, const CHAR **argv);
#if ENABLE_LDAC_CODEC
INT32 _bluetooth_ldac_frame_status_log_switch_cli(INT32 argc, const CHAR **argv);
INT32 _bluetooth_repeat_LDAC_frame_cli(INT32 argc, const CHAR **argv);
#endif
#endif /* ENABLE_BUFFER_CONTROL */

#if ENABLE_A2DP_SINK
INT32 _bluetooth_audio_reset_cli (INT32 argc, const CHAR **argv);
INT32 _bluetooth_pb_cmd_cli (INT32 argc, const CHAR **argv);
INT32 _bluetooth_dec_dump_data_enable_cli(INT32 argc, const CHAR **argv);
INT32 _bluetooth_optimise_latency_cli(INT32 argc, const CHAR **argv);
#if ENABLE_INTERNAL_SUSPEND
INT32 _bluetooth_set_in_inter_timegap_cli (INT32 argc, const CHAR **argv);
INT32 _bluetooth_set_out_inter_timegap_cli (INT32 argc, const CHAR **argv);
#endif
#endif /* ENABLE_A2DP_SINK */
#if ENABLE_A2DP_SRC
INT32 _bluetooth_a2dp_set_aac_bitrate_cli(INT32 argc, const CHAR **argv);
INT32 _bluetooth_enc_dump_data_enable_cli(INT32 argc, const CHAR **argv);
INT32 _bluetooth_a2dp_send_data_cli(INT32 argc, const CHAR **argv);
#if ENABLE_MTK_TV_PLATFORM
INT32 _bluetooth_mute_speaker_cli(INT32 argc, const CHAR **argv);
#endif
#if ENABLE_TRACK_EXCHANGE
INT32 _bluetooth_set_track_exchange_delay__cli (INT32 argc, const CHAR **argv);
#endif
#endif /* ENABLE_A2DP_SRC */
INT32 _bluetooth_set_avdtp_start_delay__cli (INT32 argc, const CHAR **argv);
#if ENABLE_LDAC_CODEC
INT32 _bluetooth_a2dp_ldac_enable_cli(INT32 argc, const CHAR **argv);
#if ENABLE_A2DP_SINK
INT32 _bluetooth_a2dp_test_ldac_cli(INT32 argc, const CHAR **argv);
INT32 _bluetooth_a2dp_decode_ldac_cli(INT32 argc, const CHAR **argv);
INT32 _bluetooth_ldac_set_quality_threshold(INT32 argc, const CHAR **argv);
#endif
INT32 _bluetooth_a2dp_set_qmode(INT32 argc, const CHAR **argv);
INT32 _bluetooth_a2dp_set_frameHeader(INT32 argc, const CHAR **argv);
#endif /* ENABLE_LDAC_CODEC */
#ifdef ENABLE_AAC_LOCAL_TEST
#if ENABLE_AAC_CODEC
INT32 _bluetooth_local_thread_aac_enc(INT32 argc, const CHAR **argv);
#endif
#endif /* ENABLE_AAC_LOCAL_TEST */
INT32 _bluetooth_a2dp_codec_enable_cli (INT32 argc, const CHAR **argv);
#if ENABLE_SYMPHONY
INT32 _bluetooth_a2dp_enabled_codec_cli (INT32 argc, const CHAR **argv);
#endif


#endif /*__BT_MW_A2DPP_CLI_H__*/


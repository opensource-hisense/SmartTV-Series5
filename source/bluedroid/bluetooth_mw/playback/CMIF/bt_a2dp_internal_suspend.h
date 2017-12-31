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

#ifndef BT_A2DP_INTERNAL_SUSPEND
#define BT_A2DP_INTERNAL_SUSPEND

#if ENABLE_INTERNAL_SUSPEND

VOID bt_set_in_internal_suspend_timegap(UINT32 u4timegap);
VOID bt_set_out_internal_suspend_timegap(UINT32 u4timegap);
VOID bt_internal_suspend_timer_init(VOID);
VOID bt_internal_suspend_timer_uninit(VOID);
INT32 bt_internal_suspend_sem_lock(VOID);
INT32 bt_internal_suspend_sem_unlock(VOID);
INT32 bt_internal_suspend_lock_uninit(VOID);
INT32 bt_internal_suspend_lock_init(VOID);
VOID*  i4InternalSuspendHandler(VOID *args);
INT32 i4InternalSuspendThreadCreate(VOID);
INT32 bt_out_internal_suspend_sem_lock(VOID);
INT32 bt_out_internal_suspend_sem_unlock(VOID);
INT32 bt_out_internal_suspend_lock_uninit(VOID);
INT32 bt_out_internal_suspend_lock_init(VOID);
VOID*  i4OutInternalSuspendHandler(VOID *args);
INT32 i4OutInternalSuspendThreadCreate(VOID);
VOID bt_in_internal_timer_cbk(timer_t pt_handle, VOID* pv_tag);
VOID bt_internal_suspend_init(VOID);
VOID bt_internal_suspend_uninit(VOID);
VOID bt_internal_suspend_start(VOID);
#endif


#endif



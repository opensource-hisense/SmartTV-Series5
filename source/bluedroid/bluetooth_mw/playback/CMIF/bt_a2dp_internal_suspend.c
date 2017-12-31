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

#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <assert.h>
#include <fcntl.h>
#include <sys/prctl.h>
#include <sys/select.h>
#include <sys/time.h>

#include "bt_mw_common.h"
#include "bluetooth_sync.h"
#include "bt_a2dp_cmif_playback.h"

#if ENABLE_INTERNAL_SUSPEND
#include "bt_a2dp_internal_suspend.h"

static pthread_t internal_suspend_thread;
static pthread_t out_internal_suspend_thread;
UINT32 g_u4internal_suspend_cnt = 0;
//UINT32 g_u4receive_data_cnt = 0;
UINT32 g_u4out_internal_suspend_cnt;
UINT32 u4in_time_gap = 18;
UINT32 u4out_time_gap = 20;
BOOL fg_is_inter_suspend = FALSE;
static BOOL g_fg_will_out_intenal_suspend = FALSE;
static BOOL fg_bt_is_receive_data = FALSE;
BOOL fg_bt_is_pb_pause = TRUE;

static BOOL g_bt_internal_suspend_lock_init = FALSE;
static sem_t g_bt_internal_suspend_lock;
static BOOL g_bt_out_internal_suspend_lock_init = FALSE;
static sem_t g_bt_out_internal_suspend_lock;

static UINT32 u4_lock_cnt = 10;


/* FUNCTION NAME: bt_set_in_internal_suspend_timegap
 * PURPOSE:
 *      { used for CLI, adjust the gap of playing and no data suddenly }
 * INPUT:
 *      parameter-1  -- u4timegap:  internal suspend time gap
 * OUTPUT:
 *      parameter-n  --
 * RETURN:
 *      return-value-1 --
 * NOTES:
 *
 */
VOID bt_set_in_internal_suspend_timegap(UINT32 u4timegap)
{
    FUNC_ENTRY;
    u4in_time_gap = u4timegap;
    BT_DBG_NORMAL(BT_DEBUG_A2DP,"in internal suspend time gap:%ld ms", (unsigned long)u4in_time_gap*10);
}

/* FUNCTION NAME: bt_set_out_internal_suspend_timegap
 * PURPOSE:
 *      {used for CLI, adjust the gap of the first audio data come and start to play}
 * INPUT:
 *      parameter-1  -- u4timegap: out internal suspend time gap
 * OUTPUT:
 *      parameter-n  --
 * RETURN:
 *      return-value-1 --
 * NOTES:
 *
 */
VOID bt_set_out_internal_suspend_timegap(UINT32 u4timegap)
{
    FUNC_ENTRY;
    u4out_time_gap = u4timegap;
    BT_DBG_NORMAL(BT_DEBUG_A2DP,"out internal suspend time gap:%ld ms", (unsigned long)u4out_time_gap*10);
}

/* FUNCTION NAME: bt_internal_suspend_timer_init
 * PURPOSE:
 *      {create internalsuspend timer}
 * INPUT:
 *      parameter-1  --
 * OUTPUT:
 *      parameter-n  --
 * RETURN:
 *      return-value-1 --
 * NOTES:
 *
 */
VOID bt_internal_suspend_timer_init(VOID)
{
    FUNC_ENTRY;
}

/* FUNCTION NAME: bt_internal_suspend_timer_uninit
 * PURPOSE:
 *      {delete internalsuspend timer}
 * INPUT:
 *      parameter-1  --
 * OUTPUT:
 *      parameter-n  --
 * RETURN:
 *      return-value-1 --
 * NOTES:
 *
 */
VOID bt_internal_suspend_timer_uninit(VOID)
{
    FUNC_ENTRY;
}

/* FUNCTION NAME: bt_internal_suspend_sem_lock
 * PURPOSE:
 *      {lock the internal suspend semaphore}
 * INPUT:
 *      parameter-1  --
 * OUTPUT:
 *      parameter-n  --
 * RETURN:
 *      return-value-1 -- not used
 * NOTES:
 *
 */
INT32 bt_internal_suspend_sem_lock(VOID)
{
    FUNC_ENTRY;
    if ( g_bt_internal_suspend_lock_init)
    {
        if ( 0 != sem_wait( &g_bt_internal_suspend_lock))
        {
            BT_DBG_ERROR(BT_DEBUG_A2DP, "internal sem_lock failed, errno: %ld.", (long)errno );
            return BT_ERR_STATUS_FAIL;
        }
    }

    return BT_SUCCESS;
}

/* FUNCTION NAME: bt_internal_suspend_sem_unlock
 * PURPOSE:
 *      {unlock the internal suspend semaphore}
 * INPUT:
 *      parameter-1  --
 * OUTPUT:
 *      parameter-n  --
 * RETURN:
 *      return-value-1 -- not used
 * NOTES:
 *
 */
INT32 bt_internal_suspend_sem_unlock(VOID)
{
    FUNC_ENTRY;
    u4_lock_cnt++;
    BT_DBG_NORMAL(BT_DEBUG_A2DP,"u4_lock_cnt:%ld", u4_lock_cnt);
    if ( g_bt_internal_suspend_lock_init )
    {
        if ( 0 != sem_post( &g_bt_internal_suspend_lock ))
        {
            BT_DBG_ERROR(BT_DEBUG_A2DP, "internal sem_unlockfailed, errno: %ld.", (long)errno );
            return BT_ERR_STATUS_FAIL;
        }
    }

    return BT_SUCCESS;
}

/* FUNCTION NAME: bt_internal_suspend_lock_uninit
 * PURPOSE:
 *      {uninit the internal suspend semaphore}
 * INPUT:
 *      parameter-1  --
 * OUTPUT:
 *      parameter-n  --
 * RETURN:
 *      return-value-1 -- not used
 * NOTES:
 *
 */
INT32 bt_internal_suspend_lock_uninit(VOID)
{
    FUNC_ENTRY;
    u4_lock_cnt--;
    BT_DBG_NORMAL(BT_DEBUG_A2DP,"u4_lock_cnt:%ld", (signed long)u4_lock_cnt);
    if ( g_bt_internal_suspend_lock_init )
    {
        if ( 0 != sem_destroy( &g_bt_internal_suspend_lock ))
        {
            BT_DBG_ERROR(BT_DEBUG_A2DP, "internal sem_destroy failed, errno: %ld.", (long)errno );
            return BT_ERR_STATUS_FAIL;
        }
        g_bt_internal_suspend_lock_init = FALSE;
    }

    else
    {
        return BT_ERR_STATUS_FAIL;
    }

    return BT_SUCCESS;
}


/* FUNCTION NAME: bt_internal_suspend_lock_init
 * PURPOSE:
 *      {{init the internal suspend semaphore}}
 * INPUT:
 *      parameter-1  --
 * OUTPUT:
 *      parameter-n  --
 * RETURN:
 *      return-value-1 -- not used
 * NOTES:
 *
 */
INT32 bt_internal_suspend_lock_init(VOID)
{
    FUNC_ENTRY;
    INT32 res;

    if ( !g_bt_internal_suspend_lock_init)
    {
        res = sem_init( &g_bt_internal_suspend_lock, 0, 0 );
        if ( 0 != res )
        {
            BT_DBG_ERROR(BT_DEBUG_A2DP, "internel sem_init failed, errno: %ld.", (long)errno );
            return BT_ERR_STATUS_FAIL;
        }
        g_bt_internal_suspend_lock_init = TRUE;
    }
    else
    {
        return BT_ERR_STATUS_FAIL;
    }

    return BT_SUCCESS;
}

/* FUNCTION NAME: i4InternalSuspendHandler
 * PURPOSE:
 *      { the routine of the internal suspenf thread }
 * INPUT:
 *      parameter-1  -- not used
 * OUTPUT:
 *      parameter-n  --
 * RETURN:
 *      return-value-1 --
 * NOTES:
 *
 */
VOID*  i4InternalSuspendHandler(VOID *args)
{
    prctl(PR_SET_NAME, "internal_suspend", 0, 0, 0);
    do
    {
        /* wait into internal suspend mode */
        bt_internal_suspend_sem_lock();
        if (fg_bt_is_receive_data)
        {
            /* avoid incaccurate set cmpb */
            if (!fg_bt_is_pb_pause && get_bt_a2dp_connect())
            {
                fg_bt_is_pb_pause = TRUE;
                //bluetooth_a2dp_pb_stop();
            }
            else
            {
                BT_DBG_INFO(BT_DEBUG_A2DP,"pb is pause no need suspend");
            }
        }
        else
        {
            //x_thread_delay(2000);
        }
    }
    while (get_bt_a2dp_connect());

    FUNC_EXIT;

    return NULL;
}

/* FUNCTION NAME: i4InternalSuspendThreadCreate
 * PURPOSE:
 *      { create the thread of internal suspend}
 * INPUT:
 *      parameter-1  --
 * OUTPUT:
 *      parameter-n  --
 * RETURN:
 *      return-value-1 -- not used
 * NOTES:
 *
 */
INT32 i4InternalSuspendThreadCreate(VOID)
{
    FUNC_ENTRY;

    INT32 i4_ret = 0;
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
        if (0 != (i4_ret = pthread_create(&internal_suspend_thread,
                                          &attr,
                                          i4InternalSuspendHandler,
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
    BT_DBG_NORMAL(BT_DEBUG_A2DP,"create internal suspend thread ok!");
    return BT_SUCCESS;
}

/* FUNCTION NAME: bt_out_internal_suspend_sem_lock
 * PURPOSE:
 *      {lock the out internal suspend semaphore}
 * INPUT:
 *      parameter-1  --
 * OUTPUT:
 *      parameter-n  --
 * RETURN:
 *      return-value-1 -- not used
 * NOTES:
 *
 */
INT32 bt_out_internal_suspend_sem_lock(VOID)
{
    FUNC_ENTRY;
    if ( g_bt_out_internal_suspend_lock_init)
    {
        if ( 0 != sem_wait( &g_bt_out_internal_suspend_lock))
        {
            BT_DBG_ERROR(BT_DEBUG_A2DP, "out sem_lock failed, errno: %ld.", (INT32)errno );
            return BT_ERR_STATUS_FAIL;
        }
    }

    return BT_SUCCESS;
}

/* FUNCTION NAME: bt_out_internal_suspend_sem_unlock
 * PURPOSE:
 *      {unlock the out internal suspend semaphore}
 * INPUT:
 *      parameter-1  --
 * OUTPUT:
 *      parameter-n  --
 * RETURN:
 *      return-value-1 -- not used
 * NOTES:
 *
 */
INT32 bt_out_internal_suspend_sem_unlock(VOID)
{
    FUNC_ENTRY;
    if ( g_bt_out_internal_suspend_lock_init )
    {
        if ( 0 != sem_post( &g_bt_out_internal_suspend_lock ))
        {
            BT_DBG_ERROR(BT_DEBUG_A2DP, "out sem_unlockfailed, errno: %ld.", (INT32)errno );
            return BT_ERR_STATUS_FAIL;
        }
    }

    return BT_SUCCESS;
}

/* FUNCTION NAME: bt_out_internal_suspend_lock_uninit
 * PURPOSE:
 *      { uninit the out internal suspend semaphore }
 * INPUT:
 *      parameter-1  --
 * OUTPUT:
 *      parameter-n  --
 * RETURN:
 *      return-value-1 --
 * NOTES:
 *
 */
INT32 bt_out_internal_suspend_lock_uninit(VOID)
{
    FUNC_ENTRY;
    if ( g_bt_out_internal_suspend_lock_init )
    {
        if ( 0 != sem_destroy( &g_bt_out_internal_suspend_lock ))
        {
            BT_DBG_ERROR(BT_DEBUG_A2DP, "out sem_destroy failed, errno: %ld.", (INT32)errno );
            return BT_ERR_STATUS_FAIL;
        }
        g_bt_out_internal_suspend_lock_init = FALSE;
    }

    else
    {
        return BT_ERR_STATUS_FAIL;
    }

    return BT_SUCCESS;
}


/* FUNCTION NAME: bt_out_internal_suspend_lock_init
 * PURPOSE:
 *      { init the out internal suspend semaphore }
 * INPUT:
 *      parameter-1  --
 * OUTPUT:
 *      parameter-n  --
 * RETURN:
 *      return-value-1 -- not used
 * NOTES:
 *
 */
INT32 bt_out_internal_suspend_lock_init(VOID)
{
    FUNC_ENTRY;
    INT32 res;

    if ( !g_bt_out_internal_suspend_lock_init)
    {
        res = sem_init( &g_bt_out_internal_suspend_lock, 0, 0 );
        if ( 0 != res )
        {
            BT_DBG_ERROR(BT_DEBUG_A2DP, "out sem_init failed, errno: %ld.", (INT32)errno );
            return BT_ERR_STATUS_FAIL;
        }
        g_bt_out_internal_suspend_lock_init = TRUE;
    }
    else
    {
        return BT_ERR_STATUS_FAIL;
    }

    return BT_SUCCESS;
}

/* FUNCTION NAME: i4OutInternalSuspendHandler
 * PURPOSE:
 *      { the routine of the out internal suspend thread }
 * INPUT:
 *      parameter-1  --
 * OUTPUT:
 *      parameter-n  --
 * RETURN:
 *      return-value-1 -- not used
 * NOTES:
 *
 */
VOID*  i4OutInternalSuspendHandler(VOID *args)
{
    prctl(PR_SET_NAME, "out_internal_suspend", 0, 0, 0);
    do
    {
        /* wait out internal suspend mode */
        bt_out_internal_suspend_sem_lock();
        if (fg_bt_is_receive_data)
        {
            /* avoid incaccurate set cmpb */
            if (fg_bt_is_pb_pause && get_bt_a2dp_connect())
            {
                fg_bt_is_pb_pause = FALSE;
                //bluetooth_a2dp_pb_restart();
            }
            else
            {
                BT_DBG_INFO(BT_DEBUG_A2DP,"pb is play no need restart");
            }
        }
        else
        {
            //x_thread_delay(2000);
        }
    }
    while (get_bt_a2dp_connect());

    FUNC_EXIT;

    return NULL;
}

/* FUNCTION NAME: i4OutInternalSuspendThreadCreate
 * PURPOSE:
 *      { create the out internal suspend thread }
 * INPUT:
 *      parameter-1  --
 * OUTPUT:
 *      parameter-n  --
 * RETURN:
 *      return-value-1 -- not used
 * NOTES:
 *
 */
INT32 i4OutInternalSuspendThreadCreate(VOID)
{
    FUNC_ENTRY;

    INT32 i4_ret = 0;
    pthread_attr_t attr;

    i4_ret = pthread_attr_init(&attr);
    if (0 != i4_ret)
    {
        BT_DBG_ERROR(BT_DEBUG_A2DP,"pthread_attr_init i4_ret:%ld", (long)i4_ret);
        return BT_ERR_STATUS_FAIL;
    }
    i4_ret = pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
    if (0 == i4_ret)
    {
        if (0 != (i4_ret = pthread_create(&out_internal_suspend_thread,
                                          &attr,
                                          i4OutInternalSuspendHandler,
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
    BT_DBG_NORMAL(BT_DEBUG_A2DP,"create out internal suspend thread ok!");
    return BT_SUCCESS;
}


/* FUNCTION NAME: bt_in_internal_timer_cbk
 * PURPOSE:
 *      {}
 * INPUT:
 *      parameter-1  --
 * OUTPUT:
 *      parameter-n  --
 * RETURN:
 *      return-value-1 --
 * NOTES:
 *
 */
VOID bt_in_internal_timer_cbk(timer_t pt_handle, VOID* pv_tag)
{
    /* timer processing start */
    /* for check into internal suspend mode */
    if (!fg_is_inter_suspend)
    {
        /* reset out internal suspend mode count */
        g_u4out_internal_suspend_cnt = 0;
        /* add counter */
        g_u4internal_suspend_cnt++;
        if (u4in_time_gap <= g_u4internal_suspend_cnt)
        {
            /* into internal suspend mode */
            fg_is_inter_suspend = TRUE;
            g_u4internal_suspend_cnt = 0;
            bt_internal_suspend_sem_unlock();

        }
    }
    else/* for check out internal suspend mode */
    {
        g_u4internal_suspend_cnt = 0;
        if (g_fg_will_out_intenal_suspend)
        {
            g_u4out_internal_suspend_cnt++;
            /* out internal suspend mode */
            if (u4out_time_gap <= g_u4out_internal_suspend_cnt)
            {
                g_u4out_internal_suspend_cnt = 0;
                fg_is_inter_suspend = FALSE;
                g_fg_will_out_intenal_suspend = FALSE;
                bt_out_internal_suspend_sem_unlock();
            }
        }
    }
    /* timer processing end */
}

/* FUNCTION NAME: bt_internal_suspend_init
 * PURPOSE:
 *      { initial internal suspend  }
 * INPUT:
 *      parameter-1  --
 * OUTPUT:
 *      parameter-n  --
 * RETURN:
 *      return-value-1 --
 * NOTES:
 *
 */
VOID bt_internal_suspend_init(VOID)
{
	FUNC_ENTRY;
    fg_bt_is_pb_pause = TRUE;
    g_u4internal_suspend_cnt = 0;
    fg_bt_is_receive_data = FALSE;
    bt_internal_suspend_timer_init();
    bt_internal_suspend_lock_init();
    bt_out_internal_suspend_lock_init();
    i4InternalSuspendThreadCreate();
    i4OutInternalSuspendThreadCreate();
}

/* FUNCTION NAME: bt_internal_suspend_uninit
 * PURPOSE:
 *      { uninitial internal suspend }
 * INPUT:
 *      parameter-1  --
 * OUTPUT:
 *      parameter-n  --
 * RETURN:
 *      return-value-1 --
 * NOTES:
 *
 */
VOID bt_internal_suspend_uninit(VOID)
{
	FUNC_ENTRY;
    bt_internal_suspend_sem_unlock();
    bt_out_internal_suspend_sem_unlock();
    bt_internal_suspend_lock_uninit();
    bt_out_internal_suspend_lock_uninit();
}

/* FUNCTION NAME: bt_internal_suspend_start
 * PURPOSE:
 *      {  to start internal_suspend }
 * INPUT:
 *      parameter-1  --
 * OUTPUT:
 *      parameter-n  --
 * RETURN:
 *      return-value-1 --
 * NOTES:
 *
 */
VOID bt_internal_suspend_start(VOID)
{
    fg_bt_is_receive_data = TRUE;
    g_u4internal_suspend_cnt = 0;
    /* from internal suspend to normal mode*/
    if (fg_is_inter_suspend && !g_fg_will_out_intenal_suspend)
    {
        g_fg_will_out_intenal_suspend = TRUE;
        g_u4out_internal_suspend_cnt = 0;
    }
}

#endif



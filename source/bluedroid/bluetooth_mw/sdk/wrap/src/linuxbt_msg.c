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

/* FILE NAME:  bt_mw_msg.c
 * AUTHOR: Changlu Yi
 * PURPOSE:
 *      It provides bluetooth common structure to MSG.
 * NOTES:
 */

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<errno.h>
#include<unistd.h>
#include<pthread.h>
#include<semaphore.h>
#include<assert.h>
#include<fcntl.h>
#include<sys/prctl.h>
#include<sys/types.h>
#include<sys/ipc.h>
#include<sys/msg.h>

#include"bt_mw_common.h"
#include"bluetooth_sync.h"
#include"linuxbt_common.h"
#include"linuxbt_msg.h"
#include"linuxbt_a2dp_if.h"



/*-----------------------------------------------------------------------------
                    data declarations
 ----------------------------------------------------------------------------*/
static pthread_t  h_linuxbt_thread;
static INT32   i4_linuxbt_msg_queue_id = 0;
static INT32   i4_linuxbt_msg_flag = 0;

static LINUXBT_QUEUE_T t_linuxbt_queue = {0};
static BOOL g_linuxbt_lock_init = FALSE;
static sem_t g_linuxbt_lock;

BOOL bLinuxBtQueueEmpty(VOID)
{
    //BT_DBG_NORMAL(BT_DEBUG_COMM,"Enter %s", __FUNCTION__);
    return (t_linuxbt_queue.currLen == 0)?TRUE:FALSE;
}

BOOL bLinuxBtQueueFull(VOID)
{
    //BT_DBG_NORMAL(BT_DEBUG_COMM,"Enter %s", __FUNCTION__);
    return (t_linuxbt_queue.currLen == t_linuxbt_queue.maxLen )?TRUE:FALSE;
}

void vInitLinuxBtQueue(VOID)
{
    //BT_DBG_NORMAL(BT_DEBUG_COMM,"Enter %s", __FUNCTION__);
    t_linuxbt_queue.currLen = 0;
    t_linuxbt_queue.maxLen = LINUXBT_QUEUE_NUM;
    t_linuxbt_queue.head = 0;
    memset(t_linuxbt_queue.msg, 0, LINUXBT_QUEUE_NUM*sizeof(LINUXBT_MSG_T*));
}

INT32 i4LinuxBtEnqueue(LINUXBT_MSG_T* msg)
{
    //BT_DBG_NORMAL(BT_DEBUG_COMM,"Enter %s", __FUNCTION__);

    UINT8 u1InsPos;
    LINUXBT_MSG_T* p_linuxbt_msg;

    if ( NULL == msg )
    {
        BT_DBG_ERROR(BT_DEBUG_COMM, "Msg cannot be null." );
        return BT_ERR_STATUS_PARM_INVALID;
    }

    if (bLinuxBtQueueFull())
    {
        BT_DBG_WARNING(BT_DEBUG_COMM, "bt queue is full." );
        return BT_ERR_STATUS_FAIL;
    }

    p_linuxbt_msg = (LINUXBT_MSG_T*)malloc(sizeof(LINUXBT_MSG_T));
    if (p_linuxbt_msg == NULL)
    {
        BT_DBG_ERROR(BT_DEBUG_COMM, "malloc failed." );
        return BT_ERR_STATUS_NOMEM;
    }
    memset(p_linuxbt_msg, 0, sizeof(LINUXBT_MSG_T));

    memcpy(p_linuxbt_msg, msg, sizeof(LINUXBT_MSG_T));
    //maybe need mutex
    u1InsPos = (t_linuxbt_queue.head + t_linuxbt_queue.currLen) % t_linuxbt_queue.maxLen;
    t_linuxbt_queue.currLen++;
    t_linuxbt_queue.msg[u1InsPos] = p_linuxbt_msg;
    //BT_DBG_NORMAL(BT_DEBUG_COMM,"<bt>msg enqueue, position: %d ", u1InsPos);

    return BT_SUCCESS;
}

LINUXBT_MSG_T* ptLinuxBtDequeue(VOID)
{
    //BT_DBG_NORMAL(BT_DEBUG_COMM,"Enter %s", __FUNCTION__);

    LINUXBT_MSG_T* p_linuxbt_msg;

    if (bLinuxBtQueueEmpty())
    {
        BT_DBG_WARNING(BT_DEBUG_COMM, "bt queue is empty. " );
        return NULL;
    }

    BT_DBG_NORMAL(BT_DEBUG_COMM,"<bt>msg dequeue, position: %d ", t_linuxbt_queue.head );
    p_linuxbt_msg = t_linuxbt_queue.msg[t_linuxbt_queue.head];
    t_linuxbt_queue.msg[t_linuxbt_queue.head] = NULL;

    t_linuxbt_queue.currLen--;
    t_linuxbt_queue.head++;
    if (t_linuxbt_queue.head >= t_linuxbt_queue.maxLen )
    {
        t_linuxbt_queue.head %= t_linuxbt_queue.maxLen;
    }

    return p_linuxbt_msg;
}


INT32 i4LinuxBtSendMsg(LINUXBT_MSG_T* msg)
{
    INT32 i4Ret;
    BT_DBG_NORMAL(BT_DEBUG_COMM,"Enter %s, msg type:%u", __FUNCTION__, msg->body.evtType);

    i4Ret = msgsnd(i4_linuxbt_msg_queue_id, msg, sizeof(LINUXBT_MSG_T), 0 );
    if (-1 == i4Ret)
    {
        /*fprintf(stdout, "http_ipc_send_msg failed %ld!", (long)errno);*/
        BT_DBG_ERROR(BT_DEBUG_COMM,"http_ipc_send_msg failed %ld!", (long)errno);
        return BT_ERR_STATUS_FAIL;
    }

    return BT_SUCCESS;
}

INT32 _linuxbt_sem_lock(VOID)
{
    //BT_DBG_NORMAL(BT_DEBUG_COMM,"Enter %s", __FUNCTION__);
    if ( g_linuxbt_lock_init)
    {
        if ( 0 != sem_wait( &g_linuxbt_lock))
        {
            BT_DBG_ERROR(BT_DEBUG_COMM, "sem_lock failed, errno: %ld.", (long)errno);
            return BT_ERR_STATUS_FAIL;
        }
    }
    else
    {
        BT_DBG_WARNING(BT_DEBUG_A2DP, "sem_lock not init");
    }

    return BT_SUCCESS;
}

INT32 _linuxbt_sem_unlock(VOID)
{
    //BT_DBG_NORMAL(BT_DEBUG_COMM,"Enter %s", __FUNCTION__);
    if ( g_linuxbt_lock_init )
    {
        if (0 != sem_post( &g_linuxbt_lock ))
        {
            BT_DBG_ERROR(BT_DEBUG_COMM, "sem_unlockfailed, errno: %ld.", (long)errno);
            return BT_ERR_STATUS_FAIL;
        }
    }
    else
    {
        BT_DBG_WARNING(BT_DEBUG_COMM, "sem_lock not init");
    }

    return BT_SUCCESS;
}

INT32 _linuxbt_lock_uninit(VOID)
{
    //BT_DBG_NORMAL(BT_DEBUG_COMM,"Enter %s", __FUNCTION__);
    if (g_linuxbt_lock_init)
    {
        if (0 != sem_destroy( &g_linuxbt_lock))
        {
            BT_DBG_ERROR(BT_DEBUG_COMM, "sem_destroy failed, errno: %ld.", (long)errno );
            return BT_ERR_STATUS_FAIL;
        }
        g_linuxbt_lock_init = FALSE;
    }
    else
    {
        BT_DBG_WARNING(BT_DEBUG_COMM, "sem_lock not init");
    }

    return BT_SUCCESS;
}


INT32 _linuxbt_lock_init(VOID)
{
    //BT_DBG_NORMAL(BT_DEBUG_COMM,"Enter %s", __FUNCTION__);
    INT32 res;

    if (!g_linuxbt_lock_init)
    {
        res = sem_init(&g_linuxbt_lock, 0, 0);
        if ( 0 != res )
        {
            BT_DBG_ERROR(BT_DEBUG_COMM, "sem_init failed, errno: %ld.", (long)errno );
            return BT_ERR_STATUS_FAIL;
        }
        g_linuxbt_lock_init = TRUE;
    }
    else
    {
        BT_DBG_WARNING(BT_DEBUG_COMM, "sem_lock not init");
    }

    return BT_SUCCESS;
}


INT32 i4ExecLinuxBtMsgHandler(LINUXBT_MSG_T* msg)
{
    //BT_DBG_NORMAL(BT_DEBUG_COMM,"Enter %s", __FUNCTION__);

    LINUXBT_BODY_T* pBody;
    pBody = &msg->body;

    BT_DBG_NORMAL(BT_DEBUG_COMM, "Receive message type %d, len: %ld", pBody->evtType, (long)pBody->len);
    if (LINUXBT_OFF >= pBody->evtType)
    {
        switch (pBody->evtType)
        {
        case LINUXBT_GAP_POWERED_ON:
        {
            break;
        }
        case LINUXBT_GAP_POWERED_OFF:
        {
            break;
        }
        default:
            break;
        }
    }
    /* AVRCP */
    else if ((LINUXBT_OFF < pBody->evtType) &&
             (LINUXBT_A2DP_SRC_EVENT_STREAM_OPEN > pBody->evtType)
            )
    {
        switch (pBody->evtType)
        {
        case LINUXBT_AVRCP_EVENT_CONNECT:
        {
            break;
        }
        default:
            break;
        }
    }
    /* A2DP */
    else if (LINUXBT_A2DP_SRC_EVENT_STREAM_OPEN <= pBody->evtType)
    {
        switch (pBody->evtType)
        {
        case LINUXBT_A2DP_SRC_EVENT_STREAM_OPEN:
        {
            linuxbt_a2dp_src_handle_connected_event();
            break;
        }
        case LINUXBT_A2DP_SRC_EVENT_STREAM_CLOSING:
        {
            uninit_audio_path();
            break;
        }
        case LINUXBT_A2DP_SRC_EVENT_STREAM_CLOSED:
        {
            linuxbt_a2dp_src_handle_disconnected_event();
            break;
        }
        case LINUXBT_A2DP_SINK_EVENT_STREAM_OPEN:
        {
            linuxbt_a2dp_sink_handle_connected_event();
            break;
        }
        case LINUXBT_A2DP_SINK_EVENT_STREAM_CLOSED:
        {
            linuxbt_a2dp_sink_handle_disconnected_event();
            break;
        }
        default:
            break;
        }
    }
    else
    {
        BT_DBG_WARNING(BT_DEBUG_COMM,"no that event!");
    }

    return BT_SUCCESS;
}

INT32 i4LinuxBtMsgHandler(VOID)
{
    //BT_DBG_NORMAL(BT_DEBUG_COMM,"Enter %s", __FUNCTION__);

    LINUXBT_MSG_T* p_linuxbt_msg = NULL;
    while (!bLinuxBtQueueEmpty())
    {
        p_linuxbt_msg = ptLinuxBtDequeue();
        if (NULL == p_linuxbt_msg)
        {
            BT_DBG_WARNING(BT_DEBUG_COMM,"msg is null. ");
            break;
        }

        BT_DBG_NOTICE(BT_DEBUG_COMM,"Try to execute msg handler. ");

        i4ExecLinuxBtMsgHandler(p_linuxbt_msg);
    }

    //BT_DBG_NORMAL(BT_DEBUG_COMM,"msg handler end. ");
    LINUXBT_FREE(p_linuxbt_msg);

    i4_linuxbt_msg_flag = 0;

    return BT_SUCCESS;
}


//this thread is used for handler recevied message
static VOID* _linuxbt_msg_handler_thread(VOID * args)
{
    //BT_DBG_NORMAL(BT_DEBUG_COMM,"Enter %s", __FUNCTION__);

    prctl(PR_SET_NAME, "linuxbt_msg_handle", 0, 0, 0);

    while(1)
    {
        BT_DBG_NOTICE(BT_DEBUG_COMM,"%s: wait for event. ", __FUNCTION__);
        _linuxbt_sem_lock();

        BT_DBG_NOTICE(BT_DEBUG_COMM,"%s: receive msg and go to handle it. ", __FUNCTION__);
        i4LinuxBtMsgHandler();
    }
    FUNC_EXIT;
    return NULL;
}

// this thread is used for recv msg sent from callback
static VOID* _linuxbt_msg_recv_thread(VOID * args)
{
    //BT_DBG_NORMAL(BT_DEBUG_COMM,"Enter %s", __FUNCTION__);

    INT32 i4_ret = 0;
    LINUXBT_MSG_T t_msg;
    pthread_t h_msg_handler_thread;
    pthread_attr_t attr;

    prctl(PR_SET_NAME, "linuxbt_msg_recv", 0, 0, 0);

    i4_ret = pthread_attr_init(&attr);
    if (0 != i4_ret)
    {
        BT_DBG_ERROR(BT_DEBUG_COMM, "pthread_attr_init i4_ret:%ld", (long)i4_ret);
        return NULL;
    }
    i4_ret = pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
    if (0 == i4_ret)
    {
        if (0 != (i4_ret = pthread_create(&h_msg_handler_thread,
                                          &attr,
                                          _linuxbt_msg_handler_thread,
                                          NULL)))
        {
            BT_DBG_ERROR(BT_DEBUG_COMM, "pthread_create i4_ret:%ld", (long)i4_ret);
            assert(0);
        }
    }
    else
    {
        BT_DBG_ERROR(BT_DEBUG_COMM, "pthread_attr_setdetachstate i4_ret:%ld", (long)i4_ret);
    }
    pthread_attr_destroy(&attr);

    while(1)
    {
        i4_ret = msgrcv( i4_linuxbt_msg_queue_id, &t_msg, sizeof(LINUXBT_MSG_T), LINUXBT_RECV_MSG_ALL, i4_linuxbt_msg_flag );
        BT_DBG_NORMAL(BT_DEBUG_COMM,"_linuxbt_msg_recv_thread recv msg size: %ld ", (long)i4_ret);

        if ( i4_ret > 0 )
        {
            i4LinuxBtEnqueue(&t_msg);
            //i4_search_msg_flag = IPC_NOWAIT;
            _linuxbt_sem_unlock();
        }
        else if ( i4_ret == 0 )
        {
            /*fprintf(stdout, "%s: no message received ", __FUNCTION__);*/
            BT_DBG_ERROR(BT_DEBUG_COMM, "%s: no message received ", __FUNCTION__);
        }
        else
        {
            /*fprintf(stdout, "%s: receive message failed %d", __FUNCTION__, errno);*/
            BT_DBG_ERROR(BT_DEBUG_COMM, "%s: receive message failed %d", __FUNCTION__, errno);
        }
    }
    FUNC_EXIT;

    return NULL;
}

INT32 linuxbt_msg_queue_init(VOID)
{
    //BT_DBG_NORMAL(BT_DEBUG_COMM,"Enter %s", __FUNCTION__);

    static INT32 i4_linuxbt_inited = 0;
    key_t t_key;
    UINT32 ui4_que_id;
    INT32 i4_ret = 0;

    if ( i4_linuxbt_inited )
    {
        return BT_SUCCESS;
    }

    _linuxbt_lock_init();

    vInitLinuxBtQueue();

    t_key = ftok((const char *)LINUXBT_MSG_PREFIX, BT_MSG_ID );
    if (-1 == t_key)
    {
        /*fprintf(stdout, "linuxbt_msg_queue_init get key failed %ld!!", (long)errno);*/
        BT_DBG_ERROR(BT_DEBUG_COMM, "linuxbt_msg_queue_init get key failed %ld!!", (long)errno);
        return BT_ERR_STATUS_FAIL;
    }
    /**
     * Create message queue
     */
    ui4_que_id = msgget(t_key, IPC_CREAT | 0777);
    if ((-1) == ui4_que_id)
    {
        /*fprintf(stdout, "linuxbt_msg_queue_init get queue id failed!!");*/
        BT_DBG_ERROR(BT_DEBUG_COMM, "linuxbt_msg_queue_init get queue id failed!!");
        return BT_ERR_STATUS_FAIL;
    }
    i4_linuxbt_msg_queue_id = ui4_que_id;

    /*fprintf(stdout,"+++ linuxbt_msg_queue_init +++");*/
    BT_DBG_NORMAL(BT_DEBUG_COMM, "+++ linuxbt_msg_queue_init +++");
    pthread_attr_t attr;
    i4_ret = pthread_attr_init(&attr);
    if (0 != i4_ret)
    {
        BT_DBG_ERROR(BT_DEBUG_COMM, "pthread_attr_init i4_ret:%ld", (long)i4_ret);
        return i4_ret;
    }
    i4_ret = pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
    if (0 == i4_ret)
    {
        if (0 != (i4_ret = pthread_create(&h_linuxbt_thread,
                                          &attr,
                                          _linuxbt_msg_recv_thread,
                                          NULL)))
        {
            BT_DBG_ERROR(BT_DEBUG_COMM, "pthread_create i4_ret:%ld", (long)i4_ret);
            assert(0);
        }
    }
    else
    {
        BT_DBG_ERROR(BT_DEBUG_COMM, "pthread_attr_setdetachstate i4_ret:%ld", (long)i4_ret);
    }

    pthread_attr_destroy(&attr);

    i4_linuxbt_inited = 1;

    return BT_SUCCESS;
}



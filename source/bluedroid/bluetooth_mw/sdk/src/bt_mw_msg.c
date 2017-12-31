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
//#include"bt_mmi.h"
#include "bluetooth.h"
#if defined(MTK_LINUX_AVRCP_PLUS) && (MTK_LINUX_AVRCP_PLUS == TRUE)
#include "mtk_bt_rc.h"
#else
#include "bt_rc.h"
#endif
#include"bt_mw_common.h"
#include"bt_mw_msg.h"
#include"c_bt_mw_manager.h"
#include"bt_mw_gap.h"
#include"bluetooth_sync.h"
#include"bt_mw_a2dp_common.h"
#include"bt_mw_avrcp.h"

#if ENABLE_A2DP_SINK
#include"bt_mw_avrcp_misc.h"
#include"bt_mw_a2dp_snk.h"
#endif
#if ENABLE_A2DP_SRC
#include"bt_mw_a2dp_src.h"
#endif


/*-----------------------------------------------------------------------------
                    data declarations
 ----------------------------------------------------------------------------*/
static pthread_t  h_bt_thread;
static INT32   i4_bt_msg_queue_id = 0;
static INT32   i4_bt_msg_flag = 0;

static BT_QUEUE_T t_bt_queue = {0};
static BOOL g_bt_lock_init = FALSE;
static sem_t g_bt_lock;

extern BtAppEventCbk      BtAppCbk;
extern BtAppAvrcpTrackChangeCbk AppTrackChangeCbk;
extern BtAppAvrcpMediaStatusCbk AppMediaStatusCbk;

extern UINT32 g_ui4_old_track_index;
#if ENABLE_A2DP_SINK
extern CHAR current_a2dp_addr[18];
#endif
BOOL bBtQueueEmpty()
{
    //BT_DBG_NORMAL(BT_DEBUG_COMM,"Enter %s", __FUNCTION__);
    return (t_bt_queue.currLen == 0)?TRUE:FALSE;
}

BOOL bBtQueueFull()
{
    //BT_DBG_NORMAL(BT_DEBUG_COMM,"Enter %s", __FUNCTION__);
    return (t_bt_queue.currLen == t_bt_queue.maxLen )?TRUE:FALSE;
}

void vInitBtQueue()
{
    //BT_DBG_NORMAL(BT_DEBUG_COMM,"Enter %s", __FUNCTION__);
    t_bt_queue.currLen = 0;
    t_bt_queue.maxLen = BT_MSG_QUEUE_NUM;
    t_bt_queue.head = 0;
    memset(t_bt_queue.msg, 0, BT_MSG_QUEUE_NUM*sizeof(BT_MSG_T*));
}

INT32 i4BtEnqueue(BT_MSG_T* msg)
{
    //BT_DBG_NORMAL(BT_DEBUG_COMM,"Enter %s", __FUNCTION__);

    UINT8 u1InsPos;
    BT_MSG_T* pMsg;

    if ( NULL == msg )
    {
        BT_DBG_ERROR(BT_DEBUG_COMM, "Msg cannot be null." );
        return BT_ERR_STATUS_PARM_INVALID;
    }

    if (bBtQueueFull())
    {
        BT_DBG_WARNING(BT_DEBUG_COMM, "bt queue is full." );
        return BT_ERR_STATUS_FAIL;
    }

    pMsg = (BT_MSG_T*)malloc(sizeof(BT_MSG_T));
    if (pMsg == NULL)
    {
        BT_DBG_ERROR(BT_DEBUG_COMM, "malloc failed." );
        return BT_ERR_STATUS_NOMEM;
    }
    memset(pMsg, 0, sizeof(BT_MSG_T));

    memcpy(pMsg, msg, sizeof(BT_MSG_T));
    //maybe need mutex
    u1InsPos = (t_bt_queue.head + t_bt_queue.currLen) % t_bt_queue.maxLen;
    t_bt_queue.currLen++;
    t_bt_queue.msg[u1InsPos] = pMsg;
    //BT_DBG_NORMAL(BT_DEBUG_COMM,"<bt>msg enqueue, position: %d ", u1InsPos);

    return BT_SUCCESS;
}

BT_MSG_T* ptBtDequeue()
{
    //BT_DBG_NORMAL(BT_DEBUG_COMM,"Enter %s", __FUNCTION__);

    BT_MSG_T* p_bt_msg;

    if (bBtQueueEmpty())
    {
        BT_DBG_WARNING(BT_DEBUG_COMM, "bt queue is empty. " );
        return NULL;
    }

    //BT_DBG_NORMAL(BT_DEBUG_COMM,"<bt>msg dequeue, position: %d ", t_bt_queue.head );
    p_bt_msg = t_bt_queue.msg[t_bt_queue.head];
    t_bt_queue.msg[t_bt_queue.head] = NULL;

    t_bt_queue.currLen--;
    t_bt_queue.head++;
    if (t_bt_queue.head >= t_bt_queue.maxLen )
    {
        t_bt_queue.head %= t_bt_queue.maxLen;
    }

    return p_bt_msg;
}


INT32 i4SendMsg(BT_MSG_T* msg)
{
    BT_DBG_NORMAL(BT_DEBUG_COMM,"Enter %s", __FUNCTION__);

    INT32 i4Ret;

    i4Ret = msgsnd(i4_bt_msg_queue_id, msg, sizeof(BT_MSG_T), 0 );
    if (-1 == i4Ret)
    {
        /*fprintf(stdout, "http_ipc_send_msg failed %ld!", (long)errno);*/
        BT_DBG_ERROR(BT_DEBUG_COMM,"http_ipc_send_msg failed %ld!", (long)errno);
        return BT_ERR_STATUS_FAIL;
    }

    return BT_SUCCESS;
}

INT32 _bt_sem_lock()
{
    //BT_DBG_NORMAL(BT_DEBUG_COMM,"Enter %s", __FUNCTION__);
    if ( g_bt_lock_init)
    {
        if ( 0 != sem_wait( &g_bt_lock))
        {
            BT_DBG_ERROR(BT_DEBUG_COMM, "sem_lock failed, errno: %ld.", (long)errno);
            return BT_ERR_STATUS_FAIL;
        }
    }

    return BT_SUCCESS;
}

INT32 _bt_sem_unlock( )
{
    //BT_DBG_NORMAL(BT_DEBUG_COMM,"Enter %s", __FUNCTION__);
    if ( g_bt_lock_init )
    {
        if (0 != sem_post( &g_bt_lock ))
        {
            BT_DBG_ERROR(BT_DEBUG_COMM, "sem_unlockfailed, errno: %ld.", (long)errno);
            return BT_ERR_STATUS_FAIL;
        }
    }

    return BT_SUCCESS;
}

INT32 _bt_lock_uninit()
{
    //BT_DBG_NORMAL(BT_DEBUG_COMM,"Enter %s", __FUNCTION__);
    if ( g_bt_lock_init )
    {
        if ( 0 != sem_destroy( &g_bt_lock ))
        {
            BT_DBG_ERROR(BT_DEBUG_COMM, "sem_destroy failed, errno: %ld.", (long)errno );
            return BT_ERR_STATUS_FAIL;
        }
        g_bt_lock_init = FALSE;
    }

    else
    {
        return BT_ERR_STATUS_FAIL;
    }

    return BT_SUCCESS;
}


INT32 _bt_lock_init()
{
    //BT_DBG_NORMAL(BT_DEBUG_COMM,"Enter %s", __FUNCTION__);
    INT32 res;

    if ( !g_bt_lock_init)
    {
        res = sem_init( &g_bt_lock, 0, 0 );
        if ( 0 != res )
        {
            BT_DBG_ERROR(BT_DEBUG_COMM, "sem_init failed, errno: %ld.", (long)errno );
            return BT_ERR_STATUS_FAIL;
        }
        g_bt_lock_init = TRUE;
    }
    else
    {
        return BT_ERR_STATUS_FAIL;
    }

    return BT_SUCCESS;
}

/*BT connection is triggered by SNK device(opposite device), but it only trigger AVDTP OPEN, not AVDTP START, platform should trigger AVDTP START*/
UINT8 fg_bt_avdtp_start_ind_is_triggered = 0;
/*if AVDTP START is not triggered by opposite device, platform trigger AVDTP START after some time*/
UINT32 fg_bt_trigger_avdtp_start_delay = 0;
#if ENABLE_TRACK_EXCHANGE
UINT32 fg_bt_track_exchange_delay = 0;
#endif

INT32 i4ExecBtMsgHandler(BT_MSG_T* msg)
{
    //BT_DBG_NORMAL(BT_DEBUG_COMM,"Enter %s", __FUNCTION__);

    BT_BODY_T* pBody;
    pBody = &msg->body;


    BT_DBG_NORMAL(BT_DEBUG_COMM, "Receive message type %d, len: %ld", pBody->evtType, (long)pBody->len);
    if (BT_OFF >= pBody->evtType)
    {
        switch (pBody->evtType)
        {
#if ENABLE_IOT_LINK_LOSS_RECONNECT
        case BT_GAP_LINK_LOSS_RECONNECT_SPEAKER:
        {
            CHAR address[MAX_BDADDR_LEN];
            memcpy(address, pBody->data, sizeof(address));
            sleep(3);
            if (!get_bt_a2dp_connect())
            {
                bluetooth_a2dp_connect(address, A2DP_LOCAL_ROLE_SRC);
            }
            else
            {
                BT_DBG_NORMAL(BT_DEBUG_GAP, "a2dp already connected.");
            }
            break;
        }
#endif
        default:
            break;
        }
    }
    /* AVRCP */
    else if ((BT_OFF < pBody->evtType) &&
             (BLUETOOTH_A2DP_EVENT_STREAM_OPEN_IND > pBody->evtType)
            )
    {
        switch (pBody->evtType)
        {
        case BT_AVRCP_EVENT_CONNECT_IND:
        {
            break;
        }
        case BT_AVRCP_EVENT_CONNECT_CNF:
        {
            BtAppCbk(BT_AVRCP_CONNECTION_SUCCESS, NULL);
            /* register playstatus change notify */
            if (get_bt_avrcp_connect())
            {
                if (get_bt_2nd_connect_comming())
                {
                    /*x_thread_delay(1500);*/
                    usleep(1500*1000);
                    set_bt_2nd_connect_comming(FALSE);
                }
                if (get_bt_avrcp_connect())
                {
#if ENABLE_A2DP_SINK
                    i4UinputThreadCreate();
#endif
                }
                if (!get_bt_avrcp_connect())
                {
                    BT_DBG_WARNING(BT_DEBUG_COMM,"avrcp disconnection");
                    break;
                }
            }
            else
            {
                //break;
            } /* if(get_bt_avrcp_connect()) */

            break;
        }
        case BT_AVRCP_EVENT_DISCONNECTED_IND:
        {
            break;
        }
        case BT_AVRCP_EVENT_DISCONNECTED_CNF:
        {
            break;
        }
        case BLUETOOTH_EVENT_TRACK_CHANGED:
        {
            /*may be not use this message*/
            BT_DBG_NORMAL(BT_DEBUG_COMM,"BLUETOOTH_EVENT_TRACK_CHANGED");
            /*x_thread_delay(1000);*/
            usleep(1000*1000);
            if (get_bt_a2dp_connect() && get_bt_avrcp_connect())
            {
                if (NULL != AppTrackChangeCbk)
                {
                    AppTrackChangeCbk(g_ui4_old_track_index);
                }
                else
                {
                    BT_DBG_ERROR(BT_DEBUG_AVRCP, "AppTrackChangeCbk is null");
                }
            }
            break;
        }
        default:
            break;
        }
    }
    /* A2DP */
    else if (BLUETOOTH_A2DP_EVENT_STREAM_ABORTED_CNF >= pBody->evtType)
    {
        switch (pBody->evtType)
        {
        case BLUETOOTH_A2DP_EVENT_STREAM_OPEN_IND:
        {
            break;
        }
        case BLUETOOTH_A2DP_EVENT_STREAM_OPEN_CNF:
        {
            if (get_bt_a2dp_be_sink())
            {
#if ENABLE_A2DP_SINK
                /*act as A2DP SINK*/
                return bluetooth_a2dp_sink_open_cb();
#endif
            }
            else
            {
#if ENABLE_A2DP_SRC
                /*act as A2DP SRC*/
                return bluetooth_a2dp_src_open_cb();
#endif
            }

            break;
        }
        case BLUETOOTH_A2DP_EVENT_STREAM_SUSPENDED_CNF:
        {
            break;
        }
        case BLUETOOTH_A2DP_EVENT_STREAM_CLOSED_IND:
        {
            break;
        }
        case BLUETOOTH_A2DP_EVENT_STREAM_CLOSED_CNF:
        {
            if (get_bt_a2dp_be_sink())
            {
#if ENABLE_A2DP_SINK
                /*act as A2DP SINK*/
                bluetooth_a2dp_sink_close_cb(current_a2dp_addr);
#endif
            }
            else
            {
#if ENABLE_A2DP_SRC
                /*act as A2DP SRC*/
                bluetooth_a2dp_src_close_cb(current_a2dp_addr);

#endif /* ENABLE_A2DP_SRC */
            }
            break;
        }
        default:
            break;
        }
    }
    else
    {
        BT_DBG_NORMAL(BT_DEBUG_COMM,"no that event!");
    }

    return BT_SUCCESS;
}

INT32 i4BtMsgHandler(VOID)
{
    //BT_DBG_NORMAL(BT_DEBUG_COMM,"Enter %s", __FUNCTION__);

    BT_MSG_T* pMsg = NULL;
    while (!bBtQueueEmpty())
    {
        pMsg = ptBtDequeue();
        if (NULL == pMsg)
        {
            break;
        }

        BT_DBG_NOTICE(BT_DEBUG_COMM,"Try to execute msg handler. ");

        i4ExecBtMsgHandler(pMsg);
        BT_FREE(pMsg);
    }

    //BT_DBG_NORMAL(BT_DEBUG_COMM,"msg handler end. ");
    //BT_FREE(pMsg);

    i4_bt_msg_flag = 0;

    return BT_SUCCESS;
}


//this thread is used for handler recevied message
static VOID* _bt_msg_handler_thread(VOID * args)
{
    //BT_DBG_NORMAL(BT_DEBUG_COMM,"Enter %s", __FUNCTION__);

    prctl(PR_SET_NAME, "bt_msg_handle", 0, 0, 0);

    do
    {
        BT_DBG_NOTICE(BT_DEBUG_COMM,"%s: wait for event. ", __FUNCTION__);
        _bt_sem_lock();

        BT_DBG_NOTICE(BT_DEBUG_COMM,"%s: receive msg and go to handle it. ", __FUNCTION__);
        i4BtMsgHandler();
    }
    while (1);
    FUNC_EXIT;
    return NULL;
}

// this thread is used for recv msg sent from callback
static VOID* _bt_msg_recv_thread(VOID * args)
{
    //BT_DBG_NORMAL(BT_DEBUG_COMM,"Enter %s", __FUNCTION__);

    INT32 i4_ret = 0;
    BT_MSG_T t_msg;
    pthread_t h_msg_handler_thread;
    pthread_attr_t attr;

    prctl(PR_SET_NAME, "bt_msg_recv", 0, 0, 0);

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
                                          _bt_msg_handler_thread,
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

    do
    {
        i4_ret = msgrcv( i4_bt_msg_queue_id, &t_msg, sizeof(BT_MSG_T), BT_RECV_MSG_ALL, i4_bt_msg_flag );
        BT_DBG_NORMAL(BT_DEBUG_COMM,"_bt_msg_recv_thread recv msg size: %ld ", (long)i4_ret);

        if ( i4_ret > 0 )
        {


            i4BtEnqueue(&t_msg);
            //i4_search_msg_flag = IPC_NOWAIT;
            _bt_sem_unlock();
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
    while (1);
    FUNC_EXIT;

    return NULL;
}

INT32 bt_msg_queue_init(VOID)
{
    //BT_DBG_NORMAL(BT_DEBUG_COMM,"Enter %s", __FUNCTION__);

    static INT32 i4_bt_inited = 0;
    key_t t_key;
    UINT32 ui4_que_id;
    INT32 i4_ret = 0;

    if ( i4_bt_inited )
    {
        return BT_SUCCESS;
    }

    _bt_lock_init();

    vInitBtQueue();

    t_key = ftok((const char *)BT_MSG_PREFIX, BT_MSG_ID );
    if (-1 == t_key)
    {
        /*fprintf(stdout, "bt_msg_queue_init get key failed %ld!!", (long)errno);*/
        BT_DBG_ERROR(BT_DEBUG_COMM, "bt_msg_queue_init get key failed %ld!!", (long)errno);
        return BT_ERR_STATUS_FAIL;
    }
    /**
     * Create message queue
     */
    ui4_que_id = msgget(t_key, IPC_CREAT | 0777);
    if ((-1) == ui4_que_id)
    {
        /*fprintf(stdout, "bt_msg_queue_init get queue id failed!!");*/
        BT_DBG_ERROR(BT_DEBUG_COMM, "bt_msg_queue_init get queue id failed!!");
        return BT_ERR_STATUS_FAIL;
    }
    i4_bt_msg_queue_id = ui4_que_id;

    /*fprintf(stdout,"+++ bt_msg_queue_init +++");*/
    BT_DBG_NORMAL(BT_DEBUG_COMM, "+++ bt_msg_queue_init +++");
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
        if (0 != (i4_ret = pthread_create(&h_bt_thread,
                                          &attr,
                                          _bt_msg_recv_thread,
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

    i4_bt_inited = 1;

    return BT_SUCCESS;
}



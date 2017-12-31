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
#include "c_mw_config.h"
#if ENABLE_A2DP_SINK
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <assert.h>
#include <fcntl.h>
#include <sys/prctl.h>
#include <sys/select.h>
#include <sys/time.h>
#include <linux/input.h>
#include <linux/uinput.h>
#include "bt_mw_common.h"
#include "bt_mw_avrcp_misc.h"
#include "bluetooth_sync.h"
#include "bt_mw_avrcp.h"

static pthread_t h_uinput_thread = -1;
extern BtAppAvrcpVolumeChangeCbk AppVolumeCbk;
extern BtAppAvrcpCmdCbk AppAvrcpcmdCbk;

static VOID*  i4UinputMsgHandler(VOID *args)
{
    FUNC_ENTRY;
    INT32 fd;
    INT32 err;
    INT32 i4_ret;
    INT32 i4_len;
    struct input_event event;
    BTVolType bt_volume;
    CHAR bluetooth_input_path[64];
    CHAR *szbluetooth_event_path;
    CHAR *temp;
    //BOOL fg_pts_enable;
    //bluetooth_dev = getenv("INPUTNO");
    FILE *fp = NULL;
    fd_set set;
    struct timeval uinput_tv;

    prctl(PR_SET_NAME, "bt_uinput", 0, 0, 0);
    fp = fopen(BT_TMP_PATH"/avrcp_uinput_dev.txt", "r");
    if (NULL == fp)
    {
        BT_DBG_ERROR(BT_DEBUG_AVRCP, "not create input file from udev:%d", errno);
        return NULL;
    }
    memset(bluetooth_input_path, 0, sizeof(bluetooth_input_path));
    i4_ret = fread(bluetooth_input_path, sizeof(CHAR),sizeof(bluetooth_input_path)-1,fp);
    if (0 < i4_ret)
    {
        BT_DBG_NOTICE(BT_DEBUG_AVRCP, "bluetooth_input_path:%s",bluetooth_input_path);
        temp = strstr(bluetooth_input_path, "event");
        if (NULL == temp)
        {
            BT_DBG_ERROR(BT_DEBUG_AVRCP, "can't find event");
            fclose(fp);
            return NULL;
        }
        i4_len = strlen(bluetooth_input_path);
        szbluetooth_event_path = (CHAR *)malloc(i4_len+1);
        if (NULL == szbluetooth_event_path)
        {
            BT_DBG_ERROR(BT_DEBUG_AVRCP, "can't malloc");
            fclose(fp);
            return NULL;
        }
        memset(szbluetooth_event_path, 0, i4_len+1);
        strncpy(szbluetooth_event_path, BLUETOOTH_UINPUT_PATH, i4_len);
        i4_len -= strlen(BLUETOOTH_UINPUT_PATH);
        strncat(szbluetooth_event_path, temp, i4_len);
        szbluetooth_event_path[i4_len] = '\0';

        BT_DBG_NOTICE(BT_DEBUG_AVRCP, "event path:%s",szbluetooth_event_path);
    }
    else
    {
        BT_DBG_ERROR(BT_DEBUG_AVRCP, "fread error!");
        fclose(fp);
        return NULL;
    }
    fclose(fp);

    fd = open(szbluetooth_event_path, O_RDONLY);
    if (fd < 0)
    {
        err = -errno;
        BT_DBG_ERROR(BT_DEBUG_AVRCP, "Can't open input device: %s (%ld)",
                       strerror(-err), (long)(-err));
        free(szbluetooth_event_path);
        szbluetooth_event_path = NULL;
        return NULL;
    }
    memset(&event, 0, sizeof(event));
    /* get pts mode */
    //c_btm_get_pts_mode(&fg_pts_enable);
    while (get_bt_avrcp_connect())
    {
        FD_ZERO(&set);
        FD_SET(fd, &set);
        uinput_tv.tv_sec = 1;
        uinput_tv.tv_usec = 0;
        i4_ret = select(fd+1, &set, NULL, NULL, &uinput_tv);
        /*Data is available now*/

        if (0 < i4_ret)
        {
            i4_ret = read(fd, &event, sizeof(event));
            if (sizeof(event) == i4_ret)
            {
                BT_DBG_NOTICE(BT_DEBUG_AVRCP,"type:%d,code:%d,value:%ld",
                               event.type,
                               event.code,
                               (long)event.value
                              );
                /* if pts mode enable will accept auto repeat key by system */
#ifdef BLUETOOTH_AVRCP_SEND_FROM_UINPUT
                unsigned char press_button = event.value;
                if (1 == event.value)
#endif
                {
                    BTCmdType bt_cmd_type = BT_CMD_TYPE_MAX;
                    switch (event.code)
                    {
                    case KEY_PLAYCD:
                    {
                        BT_DBG_NOTICE(BT_DEBUG_AVRCP, "KEY_PLAYCD");
                        bt_cmd_type = BT_CMD_TYPE_PLAY;
                        break;
                    }
                    case KEY_STOPCD:
                    {
                        BT_DBG_NOTICE(BT_DEBUG_AVRCP, "KEY_STOPCD");
                        bt_cmd_type = BT_CMD_TYPE_STOP;
                        break;
                    }
                    case KEY_PAUSECD:
                    {
                        BT_DBG_NOTICE(BT_DEBUG_AVRCP, "KEY_PAUSECD");
                        bt_cmd_type = BT_CMD_TYPE_PAUSE;
                        break;
                    }
                    case KEY_NEXTSONG:
                    {
                        BT_DBG_NOTICE(BT_DEBUG_AVRCP, "KEY_NEXTSONG");
                        bt_cmd_type = BT_CMD_TYPE_FWD;
                        break;
                    }
                    case KEY_PREVIOUSSONG:
                    {
                        bt_cmd_type = BT_CMD_TYPE_BWD;
                        BT_DBG_NOTICE(BT_DEBUG_AVRCP, "KEY_PREVIOUSSONG");
                        break;
                    }
                    case KEY_VOLUMEDOWN:
                    {
                        BT_DBG_NOTICE(BT_DEBUG_AVRCP, "KEY_VOLUMEDOWN");

                        bt_volume = BT_VOLUME_TYPE_DOWN;
                        if (1 == event.value) {
                            if(NULL != AppVolumeCbk)
                            {
                                AppVolumeCbk(bt_volume);
                            }
                            else
                            {
                                BT_DBG_ERROR(BT_DEBUG_AVRCP, "null pointer AppVolumeCbk");
                            }
                        }
                        break;
                    }
                    case KEY_VOLUMEUP:
                    {
                        BT_DBG_NOTICE(BT_DEBUG_AVRCP, "KEY_VOLUMEUP");
                        bt_volume = BT_VOLUME_TYPE_UP;
                        if (1 == event.value) {
                            if(NULL != AppVolumeCbk)
                            {
                                AppVolumeCbk(bt_volume);
                            }
                            else
                            {
                                BT_DBG_ERROR(BT_DEBUG_AVRCP, "null pointer AppVolumeCbk");
                            }
                        }
                        break;
                    }
                    default:
                        BT_DBG_NOTICE(BT_DEBUG_AVRCP, "not support that key!");
                        break;
                    }
#ifdef BLUETOOTH_AVRCP_SEND_FROM_UINPUT
                    if (BT_CMD_TYPE_MAX != bt_cmd_type) {
                        if(!AppAvrcpcmdCbk)
                        {
                            BT_DBG_ERROR(BT_DEBUG_AVRCP,"[AVRCP] AppAvrcpcmdCbk not init");
                            break;
                        }
                        AppAvrcpcmdCbk(bt_cmd_type, press_button);

                    }
#endif
                    BT_DBG_MINOR(BT_DEBUG_AVRCP, "bt_cmd_type = %d", bt_cmd_type);
                }

            }
            else
            {
                break;
            }
        }
        else if (-1 == i4_ret)
        {
            BT_DBG_ERROR(BT_DEBUG_AVRCP, "select error:-1");
            break;
        }
        if (get_bt_2nd_connect_comming())
        {
            BT_DBG_NOTICE(BT_DEBUG_AVRCP, "2nd connection comming");
            break;
        }
    }
    BT_DBG_NORMAL(BT_DEBUG_AVRCP, "will close fd");
    free(szbluetooth_event_path);
    szbluetooth_event_path = NULL;
    close(fd);
    FUNC_EXIT;

    return NULL;
}

INT32 i4UinputThreadCreate(VOID)
{
    FUNC_ENTRY;

    INT32 i4_ret = 0;

    pthread_attr_t attr;
    i4_ret = pthread_attr_init(&attr);
    if (0 != i4_ret)
    {
        BT_DBG_ERROR(BT_DEBUG_AVRCP, "pthread_attr_init i4_ret:%ld", (long)i4_ret);
        return BT_ERR_STATUS_FAIL;
    }
    i4_ret = pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
    if (0 == i4_ret)
    {
        if (0 != (i4_ret = pthread_create(&h_uinput_thread,
                                          &attr,
                                          i4UinputMsgHandler,
                                          NULL)))
        {
            BT_DBG_ERROR(BT_DEBUG_AVRCP, "pthread_create i4_ret:%ld", (long)i4_ret);
            assert(0);
        }
    }
    else
    {
        BT_DBG_ERROR(BT_DEBUG_AVRCP, "pthread_attr_setdetachstate i4_ret:%ld", (long)i4_ret);
    }

    pthread_attr_destroy(&attr);
    BT_DBG_NORMAL(BT_DEBUG_AVRCP, "create uinput listen thread ok!");
    return BT_SUCCESS;
}

#endif /* ENABLE_A2DP_SINK */

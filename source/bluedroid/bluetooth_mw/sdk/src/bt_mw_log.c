/******************************************************************************
 *
 *  Copyright (C) 2001-2012 Broadcom Corporation
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at:
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 ******************************************************************************/


#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <limits.h>
#include <pthread.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/time.h>
#include <time.h>

#include "config.h"
#include "c_mw_config.h"
#include "bt_mw_common.h"
#include "bt_mw_log.h"
#include "bluetooth_sync.h"


typedef UINT8 tBTTRC_MW_LAYER_ID;

typedef UINT8 (tBTTRC_SET_MW_TRACE_LEVEL)(UINT8);

typedef struct {
    const tBTTRC_MW_LAYER_ID         layer_id_start;
    const tBTTRC_MW_LAYER_ID         layer_id_end;
    tBTTRC_SET_MW_TRACE_LEVEL        *p_f;
    const char                       *trc_name;
    UINT8                            trace_level;
} tBTTRC_MW_FUNC_MAP;


#ifndef MW_DEFAULT_CONF_TRACE_LEVEL
#define MW_DEFAULT_CONF_TRACE_LEVEL BT_MW_TRACE_LEVEL_WARNING
#endif

#if defined(MTK_LINUX_MW_STACK_LOG2FILE) && (MTK_LINUX_MW_STACK_LOG2FILE == TRUE)
const char *BT_LOG_PATH_KEY = "BtStackFileName";
const char *BT_LOG2FILE_TURNED_ON_KEY = "BtStackLog2File";
const char *BT_SHOULD_SAVE_LAST_KEY = "BtStackSaveLog";
#endif
const char *MW_TRACE_CONFIG_ENABLED_KEY = "MWTraceConf";
static config_t *bt_mw_config;

#if defined(MTK_LINUX_STACK_TRAC_CONFIG) && (MTK_LINUX_STACK_TRAC_CONFIG == TRUE)
extern void set_trace_log_level(const char *trc_name, UINT8 trace_level);
#endif

EXPORT_SYMBOL BT_MW_TRC_MAP btmw_trc_map[] =
{
    {BT_DBG_COMM, BT_MW_TRACE_LEVEL_API},
    {BT_DBG_GAP, BT_MW_TRACE_LEVEL_API},
    {BT_DBG_A2DP, BT_MW_TRACE_LEVEL_API},
    {BT_DBG_AVRCP, BT_MW_TRACE_LEVEL_API},
    {BT_DBG_HID, BT_MW_TRACE_LEVEL_API},
    {BT_DBG_SPP, BT_MW_TRACE_LEVEL_API},
    {BT_DBG_GATT, BT_MW_TRACE_LEVEL_API},
    {BT_DBG_HFP, BT_MW_TRACE_LEVEL_API},
    {BT_DBG_PB, BT_MW_TRACE_LEVEL_API},
    {BT_DBG_UPL, BT_MW_TRACE_LEVEL_API}
};

static UINT8 BTMW_Set_Comm_TraceLevel(UINT8 new_level)
{
    if (new_level != 0xFF)
    {
        btmw_trc_map[BT_DEBUG_COMM].trace_level = new_level;
    }

    return btmw_trc_map[BT_DEBUG_COMM].trace_level;
}

static UINT8 BTMW_Set_Gap_TraceLevel(UINT8 new_level)
{
    if (new_level != 0xFF)
    {
        btmw_trc_map[BT_DEBUG_GAP].trace_level = new_level;
    }

    return btmw_trc_map[BT_DEBUG_GAP].trace_level;
}

static UINT8 BTMW_Set_A2dp_TraceLevel(UINT8 new_level)
{
    if (new_level != 0xFF)
    {
        btmw_trc_map[BT_DEBUG_A2DP].trace_level = new_level;
    }

    return btmw_trc_map[BT_DEBUG_A2DP].trace_level;
}

static UINT8 BTMW_Set_Avrcp_TraceLevel(UINT8 new_level)
{
    if (new_level != 0xFF)
    {
        btmw_trc_map[BT_DEBUG_AVRCP].trace_level = new_level;
    }

    return btmw_trc_map[BT_DEBUG_AVRCP].trace_level;
}

static UINT8 BTMW_Set_Hid_TraceLevel(UINT8 new_level)
{
    if (new_level != 0xFF)
    {
        btmw_trc_map[BT_DEBUG_HID].trace_level = new_level;
    }

    return btmw_trc_map[BT_DEBUG_SPP].trace_level;
}

static UINT8 BTMW_Set_Spp_TraceLevel(UINT8 new_level)
{
    if (new_level != 0xFF)
    {
        btmw_trc_map[BT_DEBUG_SPP].trace_level = new_level;
    }

    return btmw_trc_map[BT_DEBUG_SPP].trace_level;
}

static UINT8 BTMW_Set_Gatt_TraceLevel(UINT8 new_level)
{
    if (new_level != 0xFF)
    {
        btmw_trc_map[BT_DEBUG_GATT].trace_level = new_level;
    }

    return btmw_trc_map[BT_DEBUG_SPP].trace_level;
}

static UINT8 BTMW_Set_Hfp_TraceLevel(UINT8 new_level)
{
    if (new_level != 0xFF)
    {
        btmw_trc_map[BT_DEBUG_HFP].trace_level = new_level;
    }

    return btmw_trc_map[BT_DEBUG_HFP].trace_level;
}

static UINT8 BTMW_Set_Pb_TraceLevel(UINT8 new_level)
{
    if (new_level != 0xFF)
    {
        btmw_trc_map[BT_DEBUG_PB].trace_level = new_level;
    }

    return btmw_trc_map[BT_DEBUG_PB].trace_level;
}

static UINT8 BTMW_Set_Upl_TraceLevel(UINT8 new_level)
{
    if (new_level != 0xFF)
    {
        btmw_trc_map[BT_DEBUG_UPL].trace_level = new_level;
    }

    return btmw_trc_map[BT_DEBUG_UPL].trace_level;
}



/* make sure list is order by increasing layer id!!! */
static tBTTRC_MW_FUNC_MAP btmw_trc_set_level_map[] =
{
    {BT_MW_TRC_ID_START, BT_MW_TRC_ID_END, BTMW_Set_Comm_TraceLevel, "TRC_MW_COMM", MW_DEFAULT_CONF_TRACE_LEVEL},
    {BT_MW_TRC_ID_START, BT_MW_TRC_ID_END, BTMW_Set_Gap_TraceLevel, "TRC_MW_GAP", MW_DEFAULT_CONF_TRACE_LEVEL},
    {BT_MW_TRC_ID_START, BT_MW_TRC_ID_END, BTMW_Set_A2dp_TraceLevel, "TRC_MW_A2DP", MW_DEFAULT_CONF_TRACE_LEVEL},
    {BT_MW_TRC_ID_START, BT_MW_TRC_ID_END, BTMW_Set_Avrcp_TraceLevel, "TRC_MW_AVRCP", MW_DEFAULT_CONF_TRACE_LEVEL},
    {BT_MW_TRC_ID_START, BT_MW_TRC_ID_END, BTMW_Set_Hid_TraceLevel, "TRC_MW_HID", MW_DEFAULT_CONF_TRACE_LEVEL},
    {BT_MW_TRC_ID_START, BT_MW_TRC_ID_END, BTMW_Set_Spp_TraceLevel, "TRC_MW_SPP", MW_DEFAULT_CONF_TRACE_LEVEL},
    {BT_MW_TRC_ID_START, BT_MW_TRC_ID_END, BTMW_Set_Gatt_TraceLevel, "TRC_MW_GATT", MW_DEFAULT_CONF_TRACE_LEVEL},
    {BT_MW_TRC_ID_START, BT_MW_TRC_ID_END, BTMW_Set_Hfp_TraceLevel, "TRC_MW_HFP", MW_DEFAULT_CONF_TRACE_LEVEL},
    {BT_MW_TRC_ID_START, BT_MW_TRC_ID_END, BTMW_Set_Pb_TraceLevel, "TRC_MW_PB", MW_DEFAULT_CONF_TRACE_LEVEL},
    {BT_MW_TRC_ID_START, BT_MW_TRC_ID_END, BTMW_Set_Upl_TraceLevel, "TRC_MW_UPL", MW_DEFAULT_CONF_TRACE_LEVEL},

    {0, 0, NULL, NULL, MW_DEFAULT_CONF_TRACE_LEVEL}
};

static void bt_mw_load_levels_from_config(const config_t *config)
{
    tBTTRC_MW_FUNC_MAP *functions;
    assert(config != NULL);

    for (functions = &btmw_trc_set_level_map[0]; functions->trc_name; ++functions)
    {
        printf("BT_MW_InitTraceLevels -- %s\n", functions->trc_name);
        int value = config_get_int(config, CONFIG_DEFAULT_SECTION, functions->trc_name, -1);
        if (value != -1)
            functions->trace_level = value;

        if (functions->p_f)
            functions->p_f(functions->trace_level);
  }
}

static bool bt_mw_get_trace_config_enabled(void)
{
    return config_get_bool(bt_mw_config, CONFIG_DEFAULT_SECTION, MW_TRACE_CONFIG_ENABLED_KEY, false);
}

#if defined(MTK_LINUX_MW_STACK_LOG2FILE) && (MTK_LINUX_MW_STACK_LOG2FILE == TRUE)
static const char *get_btmw_log_path(void)
{
  return config_get_string(bt_mw_config, CONFIG_DEFAULT_SECTION, BT_LOG_PATH_KEY, "/3rd_rw/bluedroid/bt_stack.log");
}

static bool get_btmw_log2file_turned_on(void)
{
  return config_get_bool(bt_mw_config, CONFIG_DEFAULT_SECTION, BT_LOG2FILE_TURNED_ON_KEY, false);
}

static bool get_btmw_should_save_last(void)
{
  return config_get_bool(bt_mw_config, CONFIG_DEFAULT_SECTION, BT_SHOULD_SAVE_LAST_KEY, false);
}
#endif
void mw_log_init(void)
{
    bool fg_log2file = false;
    bool fg_savelog = false;
    const char *path = BLUETOOTH_STACK_LOCAL_FOLDER"/bt_stack.conf";
    const char *log_path = NULL;

    assert(path != NULL);

    BT_DBG_WARNING(BT_DEBUG_COMM, "%s attempt to load mw conf from %s", __func__, path);

    bt_mw_config = config_new(path);
    if (!bt_mw_config)
    {
        BT_DBG_NORMAL(BT_DEBUG_COMM, "%s file >%s< not found", __func__, path);
    }

    config_dump(bt_mw_config); /* dump config  */

    if (bt_mw_get_trace_config_enabled())
    {
        bt_mw_load_levels_from_config(bt_mw_config);
    }
    else
    {
        BT_DBG_WARNING(BT_DEBUG_COMM, "using compile default trace settings");
    }

#if defined(MTK_LINUX_MW_STACK_LOG2FILE) && (MTK_LINUX_MW_STACK_LOG2FILE == TRUE)
    fg_log2file = get_btmw_log2file_turned_on();
    fg_savelog = get_btmw_should_save_last();
    log_path = get_btmw_log_path();
    save_log2file_init(fg_log2file, fg_savelog, log_path);
#endif
}

EXPORT_SYMBOL void bt_mw_log_setStackLevel(const char *trc_name, int level)
{
#if defined(MTK_LINUX_STACK_TRAC_CONFIG) && (MTK_LINUX_STACK_TRAC_CONFIG == TRUE)
    set_trace_log_level(trc_name, level);
#else
    BT_DBG_WARNING(BT_DEBUG_COMM, "cannot set stack trace settings");
#endif
}



#ifndef _BT_DBG_CONFIG_GAP_H_
#define _BT_DBG_CONFIG_GAP_H_

#if defined(BT_RPC_DBG_SERVER) || defined(BT_RPC_DBG_CLIENT)
enum
{
    GAP_CMD_START = DBG_GAP << MOD_LOCATION & 0xFF000000,

    GAP_CMD_VAR,
    GAP_CMD_STR,
    GAP_CMD_LOCAL_PROPERTY,
    GAP_CMD_LOCAL_RESULT_NUMBER,
    GAP_CMD_INQUIRY_TYPE,
    GAP_CMD_DBG_LEVEL,
    GAP_CMD_END
};

#if defined(BT_RPC_DBG_SERVER)
extern int gap_var_test(int array_index, int offset, char *name, char *data, int length);
extern int gap_str_test(int array_index, int offset, char *name, char *data, int length);
extern int dbg_gap_get_g_local_property(int array_index, int offset, char *name, char *data, int length);
extern int dbg_gap_get_result_number(int array_index, int offset, char *name, char *data, int length);
extern int dbg_gap_get_inquiry_type(int array_index, int offset, char *name, char *data, int length);
extern int dbg_gap_get_dbg_level(int array_index, int offset, char *name, char *data, int length);



#endif

#if defined(BT_RPC_DBG_SERVER)
#define GAP_DATA_VAR (gap_var_test)
#define GAP_DATA_STR (gap_str_test)
#define GAP_DATA_LOCAL_PROPERTY (dbg_gap_get_g_local_property)
#define GAP_DATA_LOCAL_RESULT_NUMBER (dbg_gap_get_result_number)
#define GAP_DATA_INQUIRY_TYPE (dbg_gap_get_inquiry_type)
#define GAP_DATA_DBG_LEVEL (dbg_gap_get_dbg_level)

#endif

#if defined(BT_RPC_DBG_CLIENT)
#define GAP_DATA_VAR (HEAD"GAP"TYPE"VAR"ARRY"8"NAME"gap_demo_var"TAIL)
#define GAP_DATA_STR (HEAD"GAP"TYPE"STR"ARRY"16"NAME"gap_demo_str"TAIL)
#define GAP_DATA_LOCAL_PROPERTY (HEAD"GAP"TYPE"STR"NAME"g_local_property"TAIL)
#define GAP_DATA_LOCAL_RESULT_NUMBER (HEAD"GAP"TYPE"VAR"NAME"g_pt_result_number"TAIL)
#define GAP_DATA_INQUIRY_TYPE (HEAD"GAP"TYPE"VAR"NAME"fg_bt_inquiry_type"TAIL)
#define GAP_DATA_DBG_LEVEL (HEAD"GAP"TYPE"STR"NAME"btmw_trc_map"TAIL)

#endif




#define GAP_CMD_NUM     (GAP_CMD_END - GAP_CMD_START)

static SUB_CMD gap_cmd[GAP_CMD_NUM] =
{
    {GAP_CMD_VAR,   GAP_DATA_VAR},
    {GAP_CMD_STR,   GAP_DATA_STR},
    {GAP_CMD_LOCAL_PROPERTY,   GAP_DATA_LOCAL_PROPERTY},
    {GAP_CMD_LOCAL_RESULT_NUMBER, GAP_DATA_LOCAL_RESULT_NUMBER},
    {GAP_CMD_INQUIRY_TYPE, GAP_DATA_INQUIRY_TYPE},
    {GAP_CMD_DBG_LEVEL, GAP_DATA_DBG_LEVEL},
    {0,   NULL}
};
#endif

#endif


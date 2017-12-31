
#ifndef _BT_DBG_DEF_H_
#define _BT_DBG_DEF_H_

#if defined(BT_RPC_DBG_SERVER) || defined(BT_RPC_DBG_CLIENT)
#define MOD_LOCATION 24
#define BT_DBG_R_FILE "/tmp/bt-dbg-r"
#define BT_DBG_W_FILE "/tmp/bt-dbg-w"
#define HEAD "<"
#define TYPE "@"
#define ARRY "#"
#define NAME "$"
#define MEMB "^"
#define TAIL ">"
#define DATA "&"
#define HEAD_C '<'
#define TYPE_C '@'
#define ARRY_C '#'
#define NAME_C '$'
#define MEMB_C '^'
#define TAIL_C '>'
#define DATA_C '&'

#if defined(BT_RPC_DBG_SERVER)
typedef int (*DBG_R)(int array_index, int offset, char *name, char *data, int length);
#endif

typedef enum
{
    DBG_START = 0,

    DBG_GAP,
    DBG_L2CAP,
    DBG_GATT,
    DBG_A2DP,
    DBG_HID,
    DBG_AVRCP,
    DBG_HFP,
    DBG_SPP,

    DBG_END
}DBG_MOD;

enum
{
    DBG_OP_NONE,
    DBG_OP_READ,
    DBG_OP_WRITE
};

typedef struct {
    int     cmd;
#if defined(BT_RPC_DBG_SERVER)
    DBG_R   func_r;
#endif
#if defined(BT_RPC_DBG_CLIENT)
    char    *dscr;
#endif
}SUB_CMD;

typedef struct
{
    char    *c_mod;
    DBG_MOD d_mod;
    SUB_CMD *s_cmd;
    int     length;
    char    *dscr;
}MAIN_CMD;

typedef struct
{
    char buf[256];
    int  length;
}BT_RPC_DBG_BUF;

#ifndef NULL
#define NULL 0
#endif

#endif

#endif


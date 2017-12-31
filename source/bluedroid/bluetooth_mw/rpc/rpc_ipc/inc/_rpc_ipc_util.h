#ifndef __RPC_IPC_UTIL_H_
#define __RPC_IPC_UTIL_H_

#include <assert.h>
#include "u_rpc.h"

/* enable more than one client to connect different service in different process */
#define RPC_ENABLE_MULTI_CLIENT

extern char * g_ps_exe_name;
extern BOOL   g_b_log;

#ifndef RPC_DEBUG
#define RPC_DEBUG
#endif

#ifdef RPC_DEBUG

#define RPC_LOG(fmt, ...) \
do\
{\
        rpcu_log(RPC_LOG_ERROR, "<LOG %s, %s, #%d>"fmt, g_ps_exe_name, __FUNCTION__, __LINE__, ##__VA_ARGS__);\
}while(0)

#define RPC_WARNING(fmt, ...) \
do\
{\
        rpcu_log(RPC_LOG_ERROR, "<LOG %s, %s, #%d>"fmt, g_ps_exe_name, __FUNCTION__, __LINE__, ##__VA_ARGS__);\
}while(0)


#define RPC_INFO(fmt, ...) \
do\
{\
        rpcu_log(RPC_LOG_ERROR, "<LOG %s, %s, #%d>"fmt, g_ps_exe_name, __FUNCTION__, __LINE__, ##__VA_ARGS__);\
}while(0)

#define RPC_ERR(fmt, ...) \
do\
{\
        rpcu_log(RPC_LOG_ERROR, "<LOG %s, %s, #%d>"fmt, g_ps_exe_name, __FUNCTION__, __LINE__, ##__VA_ARGS__);\
}while(0)

#else

#define RPC_LOG(...)
#define RPC_WARNING(...)
#define RPC_INFO(...)
#define RPC_ERR(...)

#endif

#define IPC_ERR                 RPC_ERR
#define IPC_LOG                 RPC_LOG
#define IPC_TL_LOG              RPC_WARNING
#define IPC_INFO                RPC_INFO

#define IPC_ASSERT assert

#define RPC_MALLOC  malloc
#define RPC_CALLOC  calloc
#define RPC_FREE    free

#define RPC_DESC_SIZE(pv_desc) \
    (SIZE_T)(\
        sizeof(RPC_DESC_T) \
      + ((RPC_DESC_T *)(pv_desc))->ui4_num_entries * sizeof(RPC_DESC_ENTRY_T)\
    )

#define RPC_DESC_GET_ENTRY_START(pt_desc) \
    ((RPC_DESC_ENTRY_T *)((RPC_DESC_T *)(pt_desc) + 1))

#define RPC_DESC_GET_ENTRY_END(pt_desc) \
    ((RPC_DESC_ENTRY_T *) \
        (RPC_DESC_GET_ENTRY_START((pt_desc)) + ((RPC_DESC_T *)(pt_desc))->ui4_num_entries)\
    )

#define FOREACH_RPC_ENTRY(pt_rpc_desc, pt_rpc_entry) \
    for((pt_rpc_entry)  = RPC_DESC_GET_ENTRY_START((pt_rpc_desc));\
        (pt_rpc_entry) != RPC_DESC_GET_ENTRY_END((pt_rpc_desc));\
        (pt_rpc_entry) ++)

/*
 * IPC should not directly use this fields. They are internal
 * used to specify setting of thread.
 */
typedef struct RPCU_THREAD_DESC_T_
{
    INT32 i4_policy;
    INT32 i4_prio;
}RPCU_THREAD_DESC_T;

VOID    rpcu_os_init(const OS_FNCT_T * pt_os_fcts);
VOID    rpcu_os_uninit();

INT32   rpcu_os_create_thread(
    VOID        (* pf_main)(VOID *),
    VOID *      pv_arg,
    SIZE_T      z_size,
    UINT8       ui1_prio,
    SIZE_T      z_stack_size);

INT32   rpcu_os_get_cur_thd_desc(RPCU_THREAD_DESC_T * pt_thd_desc);
INT32   rpcu_os_set_cur_thd_desc(const RPCU_THREAD_DESC_T * pt_thd_desc);
BOOL    rpcu_os_thd_desc_eqaul(const RPCU_THREAD_DESC_T * pt_thd_desc1,
                               const RPCU_THREAD_DESC_T * pt_thd_desc2);
INT32   rpcu_os_thd_desc_cpy(RPCU_THREAD_DESC_T * pt_thd_desc_dest,
                             const RPCU_THREAD_DESC_T * pt_thd_desc_src);


INT32   rpcu_os_sem_create(INT32 i4_value, VOID ** ppv_sem);
VOID    rpcu_os_sem_delete(VOID * pv_sem);
INT32   rpcu_os_sem_lock(VOID * pv_sem);
INT32   rpcu_os_sem_lock_timeout(VOID * pv_sem, UINT32 ui4_timeout);
INT32   rpcu_os_sem_unlock(VOID * pv_sem);

INT32   rpcu_os_mutex_create(VOID ** ppv_mtx);
VOID    rpcu_os_mutex_delete(VOID * pv_mtx);
INT32   rpcu_os_mutex_lock(VOID * pv_mtx);
INT32   rpcu_os_mutex_trylock(VOID * pv_mtx);
INT32   rpcu_os_mutex_lock_timeout(VOID * pv_mtx,  UINT32 ui4_timeout);
INT32   rpcu_os_mutex_unlock(VOID * pv_mtx);

VOID *  rpcu_os_thread_self();

VOID *  rpcu_os_new_tls_key(VOID (* pf_dtor)(VOID *));
VOID    rpcu_os_delete_tls_key(VOID * pv_key);

VOID *  rpcu_os_tls_get(VOID * pv_key);

INT32   rpcu_os_tls_set(VOID * pv_key, VOID * pv_val);


INT32   rpcu_log(INT32 i4_level, CHAR * ps_fmt, ...);
INT32 rpcu_logger_stack_set(rpc_log_fct pf_log_fct, INT32 i4_level);
INT32 rpcu_push_logger(rpc_log_fct pf_log_fct, INT32 i4_level);
INT32 rpcu_push_default_logger();
INT32 rpcu_pop_logger();
INT32 rpcu_init_tls_logger();
INT32 rpcu_logger_stack_get(rpc_log_fct * ppf_log_fct, INT32 * pi4_level);

BOOL bt_rpcu_tl_log_start();
VOID bt_rpcu_tl_log_end();

VOID bt_rpcu_default_output_log(INT32 i4_level, CHAR* ps_string);

#endif /* __RPC_IPC_UTIL_H_ */



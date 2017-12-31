#ifndef _INIT_MTK_BT_SERVICE_CLIENT_H_
#define _INIT_MTK_BT_SERVICE_CLIENT_H_

#include "u_rpc.h"

extern RPC_ID_T c_rpc_start_mtk_bt_service_client(void);
extern RPC_ID_T c_rpc_init_mtk_bt_service_client(void);
extern INT32 c_rpc_uninit_mtk_bt_service_client(RPC_ID_T t_rpc_id);

#endif

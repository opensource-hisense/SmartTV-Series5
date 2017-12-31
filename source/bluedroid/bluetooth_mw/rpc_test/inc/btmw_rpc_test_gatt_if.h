#ifndef __BTMW_RPC_TEST_GATT_IF_H__
#define __BTMW_RPC_TEST_GATT_IF_H__

#include "u_rpcipc_types.h"

#define CMD_KEY_GATT        "GATT"

typedef struct
{
     unsigned char type;
     unsigned char len;
     void *value;
} btmw_rpc_test_gatt_adv_data_item_t;

typedef struct
{
    btmw_rpc_test_gatt_adv_data_item_t data[10];
} btmw_rpc_test_gatt_adv_data_t;

int btmw_rpc_test_gatt_init();
int btmw_rpc_test_gatt_deinit();

void btmw_rpc_test_gatt_decode_adv_data (UINT8* adv_data, btmw_rpc_test_gatt_adv_data_t *parse_data);

#endif /* __BTMW_RPC_TEST_GATT_IF_H__ */

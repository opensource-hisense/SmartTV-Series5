#ifndef __BTMW_TEST_GATT_IF_H__
#define __BTMW_TEST_GATT_IF_H__

#include "bt_gatt.h"

#define CMD_KEY_GATT        "GATT"

typedef struct
{
     unsigned char type;
     unsigned char len;
     void *value;
} btmw_test_gatt_adv_data_item_t;

typedef struct
{
    btmw_test_gatt_adv_data_item_t data[10];
} btmw_test_gatt_adv_data_t;

int btmw_test_gatt_init();
int btmw_test_gatt_deinit();
void btmw_test_gatt_btaddr_stoh(char *btaddr_s, bt_bdaddr_t *bdaddr_h);
void btmw_test_gatt_uuid_stoh(char *uuid_s,  bt_uuid_t *uuid);
bool btmw_test_gatt_is_uuid_equal(bt_uuid_t *uuid1,  bt_uuid_t *uuid2);
void btmw_test_gatt_print_uuid (bt_uuid_t* uuid, char* uuid_s);
void btmw_test_gatt_decode_adv_data (uint8_t* adv_data, btmw_test_gatt_adv_data_t *parse_data);

#endif /* __BTMW_TEST_GATT_IF_H__ */

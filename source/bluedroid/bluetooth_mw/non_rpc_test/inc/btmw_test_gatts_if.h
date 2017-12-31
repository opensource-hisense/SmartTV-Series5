#ifndef __BTMW_TEST_GATTS_IF_H__
#define __BTMW_TEST_GATTS_IF_H__

#include "bt_gatt.h"

#define BTMW_TEST_CMD_KEY_GATTS      "MW_GATTS"
#define BTMW_TEST_GATTS_SERVER_UUID  "49557E51-D815-11E4-8830-0800200C9A66"

int btmw_test_gatts_init();
int btmw_test_gatts_deinit();

#endif /* __BTMW_TEST_GATTS_IF_H__ */

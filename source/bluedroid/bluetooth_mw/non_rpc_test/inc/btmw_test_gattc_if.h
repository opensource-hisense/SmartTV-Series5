#ifndef __BTMW_TEST_GATTC_IF_H__
#define __BTMW_TEST_GATTC_IF_H__

#include "bt_gatt.h"

#define BTMW_TEST_CMD_KEY_GATTC   "MW_GATTC"
#define BTMW_TEST_GATTC_APP_UUID  "49557E50-D815-11E4-8830-0800200C9A66"

int btmw_test_gattc_init();
int btmw_test_gattc_deinit();

#endif /* __BTMW_TEST_GATTC_IF_H__ */

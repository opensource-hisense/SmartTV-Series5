#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "bluetooth.h"
#include "bt_gatt.h"

#include "btmw_test_cli.h"
#include "btmw_test_debug.h"
#include "btmw_test_gap_if.h"
#include "btmw_test_gatt_if.h"
#include "btmw_test_gattc_if.h"
#include "btmw_test_gatts_if.h"

#if 0
extern void *btmw_test_gap_get_profile_interface(const char *profile_id);

static btgatt_interface_t *btmw_test_gatt_interface = NULL;
extern const btgatt_client_callbacks_t btmw_test_gattc_callbacks;
extern const btgatt_server_callbacks_t btmw_test_gatts_callbacks;
const btgatt_callbacks_t btmw_test_gatt_callbacks =
{
        sizeof(btgatt_callbacks_t),
        &btmw_test_gattc_callbacks,
        &btmw_test_gatts_callbacks,
};
#endif
void btmw_test_gatt_btaddr_stoh(char *btaddr_s, bt_bdaddr_t *bdaddr_h)
{
    int i;

    for (i = 0; i <6; i++)
    {
        bdaddr_h->address[i] = strtoul(btaddr_s, &btaddr_s, 16);
        btaddr_s++;
    }
}

void btmw_test_gatt_uuid_stoh(char *uuid_s,  bt_uuid_t *uuid)
{
    int i = 0,j = 0;
    int size = strlen(uuid_s);
    char temp[3];
    temp[2] = '\0';
    while (i < size)
    {
        if (uuid_s[i] == '-' || uuid_s[i] == '\0')
        {
            i++;
            continue;
        }
        temp[0] = uuid_s[i];
        temp[1] = uuid_s[i+1];
        uuid->uu[j] = strtoul(temp, NULL, 16);
        i+=2;
        j++;
    }

    if (size <= 8)
    {
        // 16bits uuid or 32bits uuid
        if (size == 4)
        {
            uuid->uu[2] = uuid->uu[0];
            uuid->uu[3] = uuid->uu[1];
            uuid->uu[0] = 0;
            uuid->uu[1] = 0;
        }
        uuid->uu[4] = 0x00;
        uuid->uu[5] = 0x00;
        uuid->uu[6] = 0x10;
        uuid->uu[7] = 0x00;

        uuid->uu[8] = 0x80;
        uuid->uu[9] = 0x00;
        uuid->uu[10] = 0x00;
        uuid->uu[11] = 0x80;

        uuid->uu[12] = 0x5F;
        uuid->uu[13] = 0x9B;
        uuid->uu[14] = 0x34;
        uuid->uu[15] = 0xFB;
    }
}

bool btmw_test_gatt_is_uuid_equal(bt_uuid_t *uuid1,  bt_uuid_t *uuid2)
{
    int i = 0;

    for (i = 0; i < 16; i++)
    {
        if (uuid1->uu[i] != uuid2->uu[i]) return false;
    }
    return true;
}

void btmw_test_gatt_print_uuid (bt_uuid_t* uuid, char* uuid_s)
{
    int i;
    char *ptr;
    ptr = uuid_s;
    for (i = 0 ; i < 16 ; i++)
    {
        sprintf(ptr, "%02X", uuid->uu[i]);
        ptr+=2;
        if (i == 3 || i == 5 || i == 7 || i == 9)
        {
            *ptr = '-';
            ptr++;
        }
    }
    *(ptr+1) = '\0';
}

void btmw_test_gatt_decode_adv_data (uint8_t* adv_data, btmw_test_gatt_adv_data_t *parse_data)
{
    uint8_t* ptr = adv_data;
    unsigned char count = 0;
    while (1)
    {
        char length = *ptr;
        if (length == 0) break;
        unsigned char type = *(ptr+1);
        unsigned char value_len = length-1;
        unsigned char* value = (unsigned char*)malloc(value_len);
        memcpy(value, ptr+2, value_len);
        if (count <= 10)
        {
            parse_data->data[count].type = type;
            parse_data->data[count].len= value_len;
            parse_data->data[count].value= value;
        }
        ptr = ptr+length+1;
        switch (type)
        {
            case 0x01: //Flags
                BTMW_TEST_Logi("[GATT] Flags : %02X\n",value[0]);
                break;
            case 0x02: //16-bit uuid
            case 0x03: //16-bit uuid
                {
                    char temp[value_len*2+1];
                    int i = 0;
                    int j = 0;
                    for (i = value_len-1 ; i >= 0 ; i--)
                    {
                        sprintf(&temp[j*2],"%02X",value[i]);
                        j++;
                    }
                    BTMW_TEST_Logi("[GATT] 16-bit Service Class length: %d UUIDs : %s\n",value_len,temp);
                }
                break;
            case 0x04: //32-bit uuid
            case 0x05: //32-bit uuid
                {
                    char temp[value_len*2+1];
                    int i = 0;
                    int j = 0;
                    for (i = value_len-1 ; i >= 0 ; i--)
                    {
                        sprintf(&temp[j*2],"%02X",value[i]);
                        j++;
                    }
                    BTMW_TEST_Logi("[GATT] 32-bit Service Class length: %d UUIDs : %s\n",value_len,temp);
                }
                break;
            case 0x06: //128-bit uuid
            case 0x07: //128-bit uuid
                {
                    char temp[value_len*2+1];
                    int i = 0;
                    int j = 0;
                    for (i = value_len-1 ; i >= 0 ; i--)
                    {
                        sprintf(&temp[j*2],"%02X",value[i]);
                        j++;
                    }
                    BTMW_TEST_Logi("[GATT] 128-bit Service Class length: %d UUIDs : %s\n",value_len,temp);
                }
                break;
            case 0x08: //Shortened Local Name
                BTMW_TEST_Logi("[GATT] Shortened Local length: %d Name : %s\n",value_len,value);
                break;
            case 0x09: //Complete Local Name
                BTMW_TEST_Logi("[GATT] Complete Local length: %d Name : %s\n",value_len,value);
                break;
            case 0x0A: //Tx Power Level
                BTMW_TEST_Logi("[GATT] Tx Power Level : %d\n",value[0]);
                break;
            case 0x1B: //LE Bluetooth Device Address
                {
                    BTMW_TEST_Logi("[GATT] LE Bluetooth Device Address : %02X:%02X:%02X:%02X:%02X:%02X\n",
                    value[5], value[4], value[3],
                    value[2], value[1], value[0]);
                }
                break;
            case 0xFF: //Manufacturer Specific Data
                {
                    char temp[value_len*2+1];
                    int i = 0;
                    int j = 0;
                    for (i = value_len-1 ; i >= 0 ; i--)
                    {
                        sprintf(&temp[j*2],"%02X",value[i]);
                        j++;
                    }
                    BTMW_TEST_Logi("[GATT] Manufacturer Specific Data : %s\n",temp);
                }
                break;
            default:
                {
                    char temp[length*2];
                    int i = 0;
                    for (i = 0 ; i < length ; i++)
                    {
                        sprintf(&temp[i*2],"%02X",value[i]);
                    }
                    BTMW_TEST_Logi("[GATT] Type:%02X Length:%d Data:%s\n",type,length,temp);
                }
                break;
        }
        count++;
    }
}

int btmw_test_gatt_init()
{
    BTMW_TEST_Logd("[GATTC] %s()\n", __func__);
    int ret = 0;
    if ( btmw_test_gattc_init() != 0)
    {
        BTMW_TEST_Loge("[GATT] Failed to init GATT client interface\n");
        return -1;
    }
    if ( btmw_test_gatts_init() != 0)
    {
        BTMW_TEST_Loge("[GATT] Failed to init GATT server interface\n");
        return -1;
    }
    return ret;
}

int btmw_test_gatt_deinit()
{
    BTMW_TEST_Logd("[GATT] %s()\n", __func__);
    btmw_test_gattc_deinit();
    btmw_test_gatts_deinit();
    return 0;
}


#ifndef _BT_MW_GAP_H_
#define _BT_MW_GAP_H_

/*----------------------------------------------------------------------------
                    functions declarations
 ----------------------------------------------------------------------------*/



INT32 bluetooth_set_local_dev_name(CHAR *name);
INT32 bluetooth_get_properties(VOID);
INT32 bluetooth_set_power(BOOL fg_on);
INT32 bluetooth_set_power_sync(BOOL fg_on);
INT32 bluetooth_set_connectable_and_discoverable(BOOL fg_conn, BOOL fg_disc);
INT32 bluetooth_set_scanmode_sync(BOOL fg_conn, BOOL fg_disc);
INT32 bluetooth_scan(VOID);
INT32 bluetooth_send_get_rssi(CHAR *address);
INT32 bluetooth_set_rssi_value(INT16 rssi_value);
INT32 bluetooth_get_rssi_value(INT16 *prssi_value);
VOID bluetooth_gap_start_sniff_mode(UINT8 fg_start,
                                    CHAR *ptr_addr,
                                    INT32 i4maxInterval,
                                    INT32 i4minInterval,
                                    INT32 i4attempt,
                                    INT32 i4timeout);
INT32 bluetooth_inquiry_one(CHAR * ps_dev_mac, UINT32 ui4_inqDuration);
INT32 bluetooth_stop_scan(VOID);
INT32 bluetooth_sdp_request(CHAR *addr);
INT32 bluetooth_pair(CHAR *addr, int transport);
INT32 bluetooth_remove_paired_dev(CHAR *addr);
INT32 bluetooth_pre_init(VOID);
INT32 bluetooth_gap_init(VOID);
INT32 bluetooth_gap_deinit(VOID);
VOID bluetooth_clear_external_resource(VOID);
INT32 bluetooth_set_virtual_sniffer(INT32 enable);
VOID bt_gap_handle_every_result_ind(INT32 num_properties, bt_property_t *properties);
VOID bt_gap_handle_discovery_result_ind(INT32 num_properties, bt_property_t *properties);
VOID bt_gap_handle_discovery_update_ind(bt_bdaddr_t *bd_addr, INT32 num_properties, bt_property_t *properties);
VOID bt_gap_acl_disconnect_reason_ck(bt_bdaddr_t *bd_addr, UINT8 reason);
VOID bt_gap_handle_power_on_cnf(VOID);
VOID bt_gap_handle_power_off_cnf(VOID);
VOID bt_gap_handle_get_properties_cnf(BT_GAP_LOCAL_PROPERTIES_RESULT *localProperty);
VOID bt_gap_handle_set_scanmode_cnf(bt_status_t status, bt_property_t *properties);
VOID bt_gap_handle_link_state_ind(bt_status_t status,
                                  bt_bdaddr_t *remote_bd_addr,
                                  bt_acl_state_t state);
VOID bt_gap_get_rssi_result_cb(INT16 rssi_value);
VOID bt_gap_hci_response_cb(UINT8 *event, INT32 event_length);
INT32 bt_gap_get_self_rssi_value(INT16 *rssi_value);
INT32 send_buffer_to_hci(CHAR* buffer);
INT32 bluetooth_set_afh(UINT8* mask);
INT32 bluetooth_get_bonded_device(VOID);

#if ENABLE_HERCULES
void bt_gap_get_pin_code_cb(bt_bdaddr_t *remote_bd_addr, CHAR *name, bt_pin_code_t *pin, UINT8 *fg_accept);
void bt_gap_get_pass_key_cb(bt_bdaddr_t *remote_bd_addr, CHAR *name, UINT32 pass_key, UINT8 *fg_accept);
#else
void bt_gap_get_pin_code_cb(CHAR *pin, UINT8 *fg_accept);
void bt_gap_get_pass_key_cb(UINT32 pass_key, UINT8 *fg_accept);
#endif

VOID bt_gap_handle_bond_state_cb(bt_bond_state_t state, bt_bdaddr_t *remote_bd_addr);

#if defined(MTK_LINUX_GAP) && (MTK_LINUX_GAP == TRUE)
INT32 bluetooth_config_clear(VOID);
INT32 bluetooth_dump(CHAR *mode);
INT32 bluetooth_database_add(UINT16 feature, bt_bdaddr_t *remote_bd_addr, size_t len);
INT32 bluetooth_database_clear(VOID);
INT32 bluetooth_ble_pair(CHAR *pbt_addr);
INT32 bluetooth_get_device_type(CHAR *pbt_addr);
#endif

INT32 bt_gap_handle_discovery_stop_ind();

#endif /*  _BT_MW_GAP_H_ */


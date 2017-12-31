
#ifndef __LINUXBT_GAP_IF_H__
#define __LINUXBT_GAP_IF_H__


int linuxbt_gap_init(void);
int linuxbt_gap_deinit(void);
int linuxbt_gap_deinit_profiles(void);
int linuxbt_gap_enable_handler(void);
int linuxbt_gap_disable_handler(void);
int linuxbt_gap_start_discovery_handler(void);
int linuxbt_gap_cancel_discovery_handler(void);
int linuxbt_gap_create_bond_handler(char *pbt_addr, int transport);
int linuxbt_gap_remove_bond_handler(char *pbt_addr);
int linuxbt_gap_cancel_bond_handler(char *pbt_addr);
int linuxbt_gap_get_remote_device_properties_handler(char *pbt_addr);
int linuxbt_gap_set_scan_mode(int mode);
int linuxbt_gap_set_device_name_handler(char *pname);
int linuxbt_gap_get_adapter_properties_handler(void);
int linuxbt_gap_detrust_one_handler(char *pbt_addr);
int linuxbt_gap_cancel_sdp_services_handler(char *pbt_addr);
int linuxbt_gap_get_rssi_handler(char *pbt_addr);
int linuxbt_gap_config_hci_snoop_log_handler(unsigned char enable);

int linuxbt_gap_start_sniff_mode_handler(UINT8 fg_start,
                                                char *ptr_addr,
                                                int i4maxInterval,
                                                int i4minInterval,
                                                int i4attempt,
                                                int i4timeout);
int linuxbtgap_send_hci_handler(char *ptr);

int linuxbt_gap_get_remote_service_record_handler(char *pbd_addr);
int linuxbt_enable_external_log(int log_switch);

int linuxbt_set_log_lv_handler(int module, int log_level);
int linuxbt_gap_set_afh(uint8_t *mask);
int linuxbt_gap_set_bt_wifi_ratio(uint8_t bt_ratio, uint8_t wifi_ratio);

#if defined(MTK_LINUX_GAP) && (MTK_LINUX_GAP == TRUE)
int linuxbt_gap_config_clear();
int linuxbt_dump(char *mode);
int linuxbt_interop_database_add(uint16_t feature, bt_bdaddr_t *remote_bd_addr,size_t len);
int linuxbt_interop_database_clear();
int linuxbt_ble_pair(char *pbt_addr);
#endif

#endif /* __LINUXBT_GAP_IF_H__ */

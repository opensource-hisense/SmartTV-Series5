
#ifndef __LINUXBT_HID_IF_H__
#define __LINUXBT_HID_IF_H__

//int linuxbt_hid_activate_handler(void);
//int linuxbt_hid_deactivate_handler(void);
int linuxbt_hid_init(void);
int linuxbt_hid_deinit(void);
int linuxbt_hid_connect_int_handler(char *pbt_addr);
int linuxbt_hid_disconnect_handler(char *pbt_addr);
int linuxbt_hid_set_output_report_handler(char *pbt_addr, char *preport_data);
int linuxbt_hid_get_input_report_handler(char *pbt_addr, int reportId, int bufferSize);
int linuxbt_hid_get_feature_report_handler(char *pbt_addr, int reportId, int bufferSize);
int linuxbt_hid_set_feature_report_handler(char *pbt_addr, char *preport_data);
int linuxbt_hid_set_protocol_handler(char *pbt_addr, int protocol_mode);
//int linuxbt_hid_send_control_handler(char *pbt_addr, int pcontrol_mode);
int linuxbt_hid_get_protocol_handler(char *pbt_addr);
int linuxbt_hid_get_output_report_handler(char *pbt_addr, int reportId, int bufferSize);
int linuxbt_hid_set_input_report_handler(char *pbt_addr, char *preport_data);
int linuxbt_hid_virtual_unplug_handler(char *pbt_addr);
int linuxbt_hid_send_data_handler(char *pbt_addr,char *data);
#endif /* __LINUXBT_HID_IF_H__ */

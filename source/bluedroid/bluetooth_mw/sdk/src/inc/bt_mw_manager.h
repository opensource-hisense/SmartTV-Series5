#ifndef _BT_MW_MODE_H_
#define _BT_MW_MODE_H_


#include "u_bluetooth.h"



/*-----------------------------------------------------------------------------
                    functions declarations
 ----------------------------------------------------------------------------*/

INT32 bluetooth_mgr_save_device_history(VOID);
INT32 bluetooth_mgr_clear_local_dev_info(VOID);
INT32 bluetooth_mgr_factory_reset(CHAR *addr);
INT32 bluetooth_mgr_del_paired_av_dev_all(VOID);
INT32 bluetooth_mgr_del_paired_av_dev_one(BT_TARGET_DEV_INFO *pt_device_info);
INT32 bluetooth_mgr_btuninit(VOID);
INT32 bluetooth_mgr_connecting(CHAR* ps_mac);

INT32 bluetooth_mgr_bluetooth_on_off(BOOL fg_on);
INT32 bluetooth_mgr_register_cbk_fct(BT_APP_CB_FUNC *func);
INT32 bluetooth_mgr_btstart(const CHAR *ps_mode, UINT32 u4_overtime);
INT32 bluetooth_mgr_stop(BOOL fg_keep_conn, BOOL fg_discard_data);
INT32 bluetooth_mgr_av_auto_disconnection(VOID);
INT32 bluetooth_disconnect_both_sync(VOID);
INT32 bluetooth_mgr_av_disconnect_all(VOID);
INT32 bluetooth_mgr_get_num_in_pairlist(UINT32 *u4_dev_num);
INT32 bluetooth_mgr_clear_local_dev_info(VOID);
INT32 bluetooth_mgr_base_init(BT_APP_CB_FUNC *func);

/******************** GAP ********************/



/******************** A2DP ********************/




#endif /*  _BT_MW_MODE_H_  */

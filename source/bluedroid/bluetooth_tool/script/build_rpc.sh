
if [ ! $Bluetooth_Mw_Dir ]; then
    Bluetooth_Mw_Dir=${Bluetooth_Tool_Dir}/../bluetooth_mw
fi
if [ ! $Bluetooth_Prebuilts_Dir ]; then
    Bluetooth_Prebuilts_Dir=${Bluetooth_Tool_Dir}/prebuilts
fi
if [ ! $Bluedroid_Libs_Path ]; then
    Bluedroid_Libs_Path=${Bluetooth_Tool_Dir}/prebuilts/lib
fi
if [ ! $External_Libs_Path ]; then
    External_Libs_Path=${Bluetooth_Tool_Dir}/external_libs/platform
fi

Mw_Include_Path=${Bluetooth_Mw_Dir}/inc
Mw_Config_Path=${Bluetooth_Mw_Dir}/inc/config
Mw_ALSA_Inc_Path=${Bluetooth_Mw_Dir}/playback/ALSA

Libbt_Mw_Path=${Bluetooth_Prebuilts_Dir}/lib
Libbt_Alsa_Path=${External_Libs_Path}
Libz_Path=${External_Libs_Path}

cd ${Bluetooth_Mw_Dir}/rpc

rm -rf out

gn gen out/Default/ --args="mw_include_path=\"${Mw_Include_Path}\" mw_config_path=\"${Mw_Config_Path}\" mw_alsa_inc_path=\"${Mw_ALSA_Inc_Path}\" libbt_mw_path=\"-L${Libbt_Mw_Path}\" libbt_alsa_path=\"-L${Libbt_Alsa_Path}\" libz_path=\"-L${Libz_Path}\" bt_sys_log_flag=\"${BT_SYS_LOG_FLAG}\" bt_tmp_path=\"${Tmp_Path}\" bt_misc_path=\"${Tmp_Path}\" bt_etc_path=\"${Tmp_Path}\" cc=\"${CC}\" cxx=\"${CXX}\""
ninja -C out/Default all

cd ${Script_Dir}

if [ ! -d ${Bluetooth_Prebuilts_Dir}/lib ]; then
    mkdir -p ${Bluetooth_Prebuilts_Dir}/lib
fi
if [ ! -d ${Bluetooth_Prebuilts_Dir}/bin ]; then
    mkdir -p ${Bluetooth_Prebuilts_Dir}/bin
fi
if [ -f ${Bluetooth_Mw_Dir}/rpc/out/Default/libipcrpc.so ]; then
    cp ${Bluetooth_Mw_Dir}/rpc/out/Default/libipcrpc.so ${Bluetooth_Prebuilts_Dir}/lib/
else
    exit 1
fi
if [ -f ${Bluetooth_Mw_Dir}/rpc/out/Default/libmtk_bt_ipcrpc_struct.so ]; then
    cp ${Bluetooth_Mw_Dir}/rpc/out/Default/libmtk_bt_ipcrpc_struct.so ${Bluetooth_Prebuilts_Dir}/lib/
else
    exit 1
fi
if [ -f ${Bluetooth_Mw_Dir}/rpc/out/Default/libmtk_bt_service_client.so ]; then
    cp ${Bluetooth_Mw_Dir}/rpc/out/Default/libmtk_bt_service_client.so ${Bluetooth_Prebuilts_Dir}/lib/
else
    exit 1
fi
if [ -f ${Bluetooth_Mw_Dir}/rpc/out/Default/libmtk_bt_service_server.so ]; then
    cp ${Bluetooth_Mw_Dir}/rpc/out/Default/libmtk_bt_service_server.so ${Bluetooth_Prebuilts_Dir}/lib/
else
    exit 1
fi
if [ -f ${Bluetooth_Mw_Dir}/rpc/out/Default/btservice ]; then
    cp ${Bluetooth_Mw_Dir}/rpc/out/Default/btservice ${Bluetooth_Prebuilts_Dir}/bin/
else
    exit 1
fi

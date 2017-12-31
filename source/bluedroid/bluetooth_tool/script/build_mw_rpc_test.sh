
##############################################################################################
if [ ! $CC ]; then
    CC=/mtkoss/gnuarm/neon_4.8.2_2.6.35_cortex-a9-ubuntu/i686/bin/armv7a-mediatek482_001_neon-linux-gnueabi-gcc
    echo "single build?"
    echo "please asure the build environment correctly setting"
    echo $CC
fi
if [ ! $CXX ]; then
    CXX=/mtkoss/gnuarm/neon_4.8.2_2.6.35_cortex-a9-ubuntu/i686/bin/armv7a-mediatek482_001_neon-linux-gnueabi-g++
fi
if [ ! $Script_Dir ]; then
    Script_Dir=$(pwd)
    echo $Script_Dir
    if [[ $(pwd) =~ "script/common" ]]; then
        Bluetooth_Tool_Dir=${Script_Dir}/../..
    else
        Bluetooth_Tool_Dir=${Script_Dir}/..
    fi
fi
if [ ! $Bluetooth_Mw_Dir ]; then
    Bluetooth_Mw_Dir=${Bluetooth_Tool_Dir}/../bluetooth_mw
fi
if [ ! $Bluetooth_Prebuilts_Dir ]; then
    Bluetooth_Prebuilts_Dir=${Bluetooth_Tool_Dir}/prebuilts
fi

##############################################################################################

Mw_Include_Path=${Bluetooth_Mw_Dir}/inc
Mw_Config_Path=${Bluetooth_Mw_Dir}/inc/config
Mw_ALSA_Inc_Path=${Bluetooth_Mw_Dir}/playback/ALSA
MTK_RPC_IPC_Inc_Path=${Bluetooth_Mw_Dir}/inc_rpc

Libbt_Mw_Path=${Bluetooth_Prebuilts_Dir}/lib
Libbt_Alsa_Path=${Bluetooth_Prebuilts_Dir}/lib
Libz_Path=${Bluetooth_Prebuilts_Dir}/lib

cd ${Bluetooth_Mw_Dir}/rpc_test

rm -rf out

gn gen out/Default/ --args="mw_include_path=\"${Mw_Include_Path}\" mw_config_path=\"${Mw_Config_Path}\" mw_alsa_inc_path=\"${Mw_ALSA_Inc_Path}\" mtk_rpc_ipc_inc_path=\"${MTK_RPC_IPC_Inc_Path}\" libbt_mw_path=\"-L${Libbt_Mw_Path}\" libbt_alsa_path=\"-L${Libbt_Alsa_Path}\" libz_path=\"-L${Libz_Path}\" bt_sys_log_flag=\"${BT_SYS_LOG_FLAG}\" bt_tmp_path=\"${BT_Tmp_Path}\" bt_misc_path=\"${BT_Misc_Path}\" bt_etc_path=\"${BT_Etc_Path}\" cc=\"${CC}\" cxx=\"${CXX}\""
ninja -C out/Default all

cd ${Script_Dir}

if [ ! -d ${Bluetooth_Prebuilts_Dir}/bin ]; then
    mkdir -p ${Bluetooth_Prebuilts_Dir}/bin
fi

if [ -f ${Bluetooth_Mw_Dir}/rpc_test/out/Default/btmw-rpc-test ]; then
    cp ${Bluetooth_Mw_Dir}/rpc_test/out/Default/btmw-rpc-test ${Bluetooth_Prebuilts_Dir}/bin/
else
    exit 1
fi

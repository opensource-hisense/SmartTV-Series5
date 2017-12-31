#-----------------------------------------------------------------------------
# Copyright (c) 2010, MediaTek Inc.
# All rights reserved.
#
# Unauthorized use, practice, perform, copy, distribution, reproduction,
# or disclosure of this information in whole or in part is prohibited.
#-----------------------------------------------------------------------------

THIS_DIR := $(shell pwd)

.PHONY: all install clean
all:
	@echo 'Create bluedroid library!'
	@echo "Install bluedroid library. OSS_LIB_ROOT=$(OSS_LIB_ROOT) THIS_DIR=$(THIS_DIR)"
	mkdir -p $(OSS_LIB_ROOT)/bluedroid/include
	cp -r $(THIS_DIR)/bluetooth_mw/inc/u_*.h $(OSS_LIB_ROOT)/bluedroid/include
	cp -r $(THIS_DIR)/bluetooth_mw/inc/config/*.h $(OSS_LIB_ROOT)/bluedroid/include
	cp -r $(THIS_DIR)/bluetooth_mw/inc_rpc/*.h $(OSS_LIB_ROOT)/bluedroid/include
	@echo 'Release bluedroid library finished.'

clean:
		
notice:
	cp -rf NOTICE.txt $(OSS_NOTICE_OUTPUT)/bluedroid_NOTICE.txt
	
info:
	cp -rf OSS_INFO.txt $(OSS_INFO_OUTPUT)/bluedroid_INFO.txt

install:

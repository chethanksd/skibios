#
# Master MAKEFILE
#
# author: chetandev.ksd@gmail.com
#

MAKEFILE_PATH := $(abspath $(lastword $(MAKEFILE_LIST)))
CURRENT_DIR   := $(notdir $(patsubst %/,%,$(dir $(MAKEFILE_PATH))))

include $(CURRENT_DIR)/common.mak
include $(ARCH_PATH)/script/arch_def.mak

arg_check:
ifndef ARCH_PATH
	$(error "ERROR! ARCH_PATH not defined")
endif
ifndef BUILD_PATH
	$(error "ERROR! BUILD_PATH not defined")
endif
ifndef BIN_PATH
	$(error "ERROR! BIN_PATH not defined")
endif
ifndef CC
	$(error "ERROR! Complier tool name not defined")
endif
ifndef OC
	$(error "ERROR! Objcopy tool name not defined")
endif
ifndef OD
	$(error "ERROR! Objdump tool name not defined")
endif
ifndef SZ
	$(error "ERROR! Size tool name not defined")
endif

symgen: arg_check
	$(BIN_PATH)/$(CC) -S -I./src/include $(COMMON_CFLAGS) $(ARCH_CFLAGS) ./src/symgen.c -o$(BUILD_PATH)/symgen.S
	grep "OFFSET_OF" $(BUILD_PATH)/symgen.S > $(BUILD_PATH)/symgen.txt
	grep "SIZE_OF" $(BUILD_PATH)/symgen.S >> $(BUILD_PATH)/symgen.txt
	cat $(BUILD_PATH)/symgen.S | awk '($$1 == "->") { print "#define " $$2 " " $$3 }' > $(BUILD_PATH)/symgen.h
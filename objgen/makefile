#
# Master MAKEFILE
#
# author: chetandev.ksd@gmail.com
#


GRP_TOOL	  := $(ROOT_DIR)/tools/misc/grep.exe
CAT_TOOL	  := $(ROOT_DIR)/tools/misc/cat.exe
AWK_TOOL	  := $(ROOT_DIR)/tools/misc/awk.exe

include $(ROOT_DIR)/objgen/common.mak
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

COMPLIER_FLAGS = $(COMMON_CFLAGS) $(ARCH_CFLAGS)
INCLUDE_PATHS  = $(COMMON_IPATH) $(ARCH_IPATH)

symgen: arg_check
	$(BIN_PATH)/$(CC) -S $(INCLUDE_PATHS) $(COMPLIER_FLAGS) $(ROOT_DIR)/src/symgen.c -o$(BUILD_PATH)/symgen.S
	$(GRP_TOOL) "OFFSET_OF" $(BUILD_PATH)/symgen.S > $(BUILD_PATH)/symgen.txt
	$(GRP_TOOL) "SIZE_OF" $(BUILD_PATH)/symgen.S >> $(BUILD_PATH)/symgen.txt
	$(CAT_TOOL) $(BUILD_PATH)/symgen.S | $(AWK_TOOL) '($$1 == "->") { print "#define " $$2 " " $$3 }' > $(BUILD_PATH)/symgen.h


resource: arg_check
	$(BIN_PATH)/$(CC) $(INCLUDE_PATHS) $(COMPLIER_FLAGS) $(ROOT_DIR)/src/*.c -o$(BUILD_PATH)
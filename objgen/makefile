#
# Master MAKEFILE
#
# author: chetandev.ksd@gmail.com
#


GRP_TOOL	  := $(ROOT_DIR)/tools/misc/grep.exe
CAT_TOOL	  := $(ROOT_DIR)/tools/misc/cat.exe
AWK_TOOL	  := $(ROOT_DIR)/tools/misc/awk.exe
CP_TOOL		  := $(ROOT_DIR)/tools/misc/cp.exe
FIND_TOOL	  := $(ROOT_DIR)/tools/misc/find.exe

include $(ROOT_DIR)/objgen/common.mak
include $(ROOT_DIR)/objgen/source.mak
include $(ARCH_PATH)/script/arch_def.mak
include $(ARCH_PATH)/script/arch_source.mak

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

COMPILER_FLAGS 	 = $(COMMON_CFLAGS) 
COMPILER_FLAGS	+= $(ARCH_CFLAGS) 

INCLUDE_PATHS  	 = $(COMMON_IPATH)
INCLUDE_PATHS	+= $(ARCH_IPATH)

SOURCE_PATHS	 = $(COMMON_SRC_INC)
SOURCE_PATHS	+= $(ARCH_SRC_INC)

symgen: arg_check
	$(BIN_PATH)/$(CC) -S $(INCLUDE_PATHS) $(COMPILER_FLAGS) $(ROOT_DIR)/src/symgen.c -o$(SYMGEN_PATH)/symgen.S
	$(GRP_TOOL) "OFFSET_OF" $(SYMGEN_PATH)/symgen.S > $(SYMGEN_PATH)/symgen.txt
	$(GRP_TOOL) "SIZE_OF" $(SYMGEN_PATH)/symgen.S >> $(SYMGEN_PATH)/symgen.txt
	$(CAT_TOOL) $(SYMGEN_PATH)/symgen.S | $(AWK_TOOL) '($$1 == "->") { print "#define " $$2 " " $$3 }' > $(SYMGEN_PATH)/symgen.h
	$(CP_TOOL) $(SYMGEN_PATH)/symgen.h $(ALLSRC_PATH)/symgen.h


resource: arg_check
	$(BIN_PATH)/$(CC) $(INCLUDE_PATHS) $(UPC_SKI_PARAM) $(COMPILER_FLAGS) $(ROOT_DIR)/src/kvar.c -o$(RESOURCE_PATH)/upc_kvar.o
	$(BIN_PATH)/$(OD) -t $(RESOURCE_PATH)/*.o > $(RESOURCE_PATH)/resource.txt


define CREATE_EXCLUDE_ARG
! -name '$(EXCLUDE_FILE)'
endef


allsrc_copy: arg_check
	$(eval EXCLUDE_ARG += $(foreach EXCLUDE_FILE, $(COMMON_SRC_EXC), $(CREATE_EXCLUDE_ARG)))
	$(eval EXCLUDE_ARG += $(foreach EXCLUDE_FILE, $(ARCH_SRC_EXC), $(CREATE_EXCLUDE_ARG)))
	$(FIND_TOOL) $(SOURCE_PATHS) -maxdepth 1 -type f \( -name '*.c' -o -name '*.S' \) $(EXCLUDE_ARG) > $(BUILD_PATH)/copy_list.txt
	$(eval COPY_LIST = $(shell $(FIND_TOOL) $(SOURCE_PATHS)  -maxdepth 1 -type f \( -name '*.c' -o -name '*.S' \) $(EXCLUDE_ARG)))
	$(CP_TOOL) $(COPY_LIST) $(ALLSRC_PATH)
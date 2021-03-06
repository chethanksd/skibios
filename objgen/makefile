#
# Master MAKEFILE
#
# author: chetandev.ksd@gmail.com
#


GRP_TOOL	  := $(ROOT_DIR)/tools/misc/grep.exe
CAT_TOOL	  := $(ROOT_DIR)/tools/misc/cat.exe
AWK_TOOL	  := $(ROOT_DIR)/tools/misc/awk.exe
CP_TOOL		  := $(ROOT_DIR)/tools/misc/cp.exe
RM_TOOL		  := $(ROO_DIR)/tools/make/rm.exe
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

#
# COMMON and ARCH constant concatenation
#

COMPILER_FLAGS 	 	 = $(COMMON_CFLAGS) 
COMPILER_FLAGS		+= $(ARCH_CFLAGS) 

RAW_INCLUDE_PATHS  	 = $(COMMON_HDR_INC)
RAW_INCLUDE_PATHS	+= $(ARCH_HDR_INC)

SOURCE_PATHS	 	 = $(COMMON_SRC_INC)
SOURCE_PATHS		+= $(ARCH_SRC_INC)


#
# Get indexed include search paths
#
get_include_spath:
	$(eval INCLUDE_PATHS += $(foreach SEARCH_PATH, $(RAW_INCLUDE_PATHS), -I$(SEARCH_PATH)))

#
# symgen target
#

symgen: arg_check get_include_spath
	$(BIN_PATH)/$(CC) -S $(INCLUDE_PATHS) $(COMPILER_FLAGS) $(ROOT_DIR)/src/symgen.c -o$(SYMGEN_PATH)/symgen.S
	$(GRP_TOOL) "OFFSET_OF" $(SYMGEN_PATH)/symgen.S > $(SYMGEN_PATH)/symgen.txt
	$(GRP_TOOL) "SIZE_OF" $(SYMGEN_PATH)/symgen.S >> $(SYMGEN_PATH)/symgen.txt
	$(CAT_TOOL) $(SYMGEN_PATH)/symgen.S | $(AWK_TOOL) '($$1 == "->") { print "#define " $$2 " " $$3 }' > $(SYMGEN_PATH)/symgen.h
	$(CP_TOOL) $(SYMGEN_PATH)/symgen.h $(HDR_PATH)/symgen.h

#
# UPC resource calculation target
#

resource: arg_check get_include_spath
	$(BIN_PATH)/$(CC) $(INCLUDE_PATHS) $(UPC_SKI_PARAM) $(COMPILER_FLAGS) $(ROOT_DIR)/src/kvar.c -o$(RESOURCE_PATH)/upc_kvar.o
	$(BIN_PATH)/$(CC) $(INCLUDE_PATHS) $(UPC_SKI_PARAM) $(COMPILER_FLAGS) $(ARCH_PATH)/arch_kvar.c -o$(RESOURCE_PATH)/arch_upc_kvar.o	
	$(BIN_PATH)/$(OD) -t $(RESOURCE_PATH)/*.o > $(RESOURCE_PATH)/resource.txt


#
# source & include file copy target
#

define CREATE_EXCLUDE_ARG
! -name '$(EXCLUDE_FILE)'
endef

gen_copy_list: arg_check gen_src_list gen_hdr_list gen_api_list

gen_src_list:
	$(eval EXCLUDE_ARG += $(foreach EXCLUDE_FILE, $(COMMON_SRC_EXC), $(CREATE_EXCLUDE_ARG)))
	$(eval EXCLUDE_ARG += $(foreach EXCLUDE_FILE, $(ARCH_SRC_EXC), $(CREATE_EXCLUDE_ARG)))
	$(FIND_TOOL) $(SOURCE_PATHS) -maxdepth 1 -type f \( -name '*.c' -o -name '*.S' \) $(EXCLUDE_ARG) > $(BUILD_PATH)/misc/src_list.txt


gen_hdr_list:
	$(eval EXCLUDE_ARG += $(foreach EXCLUDE_FILE, $(COMMON_HDR_EXC), $(CREATE_EXCLUDE_ARG)))
	$(eval EXCLUDE_ARG += $(foreach EXCLUDE_FILE, $(ARCH_HDR_EXC), $(CREATE_EXCLUDE_ARG)))
	$(FIND_TOOL) $(RAW_INCLUDE_PATHS) -maxdepth 1 -type f \( -name '*.h' \) $(EXCLUDE_ARG) > $(BUILD_PATH)/misc/hdr_list.txt


gen_api_list:
	$(FIND_TOOL) $(COMMON_API_INC) -maxdepth 1 -type f \( -name '*.h' \) > $(BUILD_PATH)/misc/api_list.txt
	$(eval API_FILES += $(foreach API_FILE, $(COMMON_API_HDR_INC), "$(API_FILE)\n"))
	@echo -e $(API_FILES) >> $(BUILD_PATH)/misc/api_list.txt


#
# source file compilation target
#

CC_SRCS  := $(wildcard $(ALLSRC_PATH)/*.c)
CC_PROGS := $(patsubst %.c,%_c.o,$(CC_SRCS))

AS_SRCS  := $(wildcard $(ALLSRC_PATH)/*.S)
AS_PROGS := $(patsubst %.S,%_S.o,$(AS_SRCS))

compile_allsrc: arg_check compile_init $(AS_PROGS) $(CC_PROGS)


# pre compile rules

compile_init:
	@echo > $(BUILD_PATH)/misc/compile_list.txt

# Build targets for c files

%_c.o : %.c
	$(eval OUTPUT = $(subst /allsrc,/obj,$@))
	@echo 'converting $< to $(OUTPUT)' >> $(BUILD_PATH)/misc/compile_list.txt
	@echo 'converting $(notdir $<) to $(notdir $(OUTPUT))'
	@$(BIN_PATH)/$(CC) $(COMPILER_IPATH) $(COMPILER_FLAGS) $< -o $(OUTPUT)
	

# Build targets for asm files

%_S.o : %.S
	$(eval OUTPUT = $(subst /allsrc,/obj,$@))
	@echo 'converting $< to $(OUTPUT)' >> $(BUILD_PATH)/misc/compile_list.txt
	@echo 'converting $(notdir $<) to $(notdir $(OUTPUT))'
	@$(BIN_PATH)/$(CC) $(COMPILER_IPATH) $(COMPILER_FLAGS) $< -o $(OUTPUT)
#
# Makefile Common Source file path defintion
#
# author: chetandev.ksd@gmail.com
#


COMMON_SRC_INC       = $(ROOT_DIR)/src/ 
COMMON_SRC_INC      += $(ROOT_DIR)/src/service/ 

COMMON_SRC_EXC       = symgen.c


#
# Common Include search path
#
COMMON_IPATH         = -I$(ROOT_DIR)/src/include
COMMON_IPATH        += -I$(ROOT_DIR)/src/api
COMMON_IPATH        += -I$(ROOT_DIR)/src/service
COMMON_IPATH        += -I$(BUILD_PATH)
COMMON_IPATH        += -I$(ALLSRC_PATH)

#
# Common header file to be copied to inc folder
#
COMMON_HDR_INC       = $(ROOT_DIR)/src/include
COMMON_HDR_INC      += $(ROOT_DIR)/src/service
COMMON_HDR_INC      += $(BUILD_PATH)
COMMON_HDR_INC      += $(ALLSRC_PATH)

#
# Include files to exclude in inc folder of build
#

COMMON_HDR_EXC       = svc.h
COMMON_HDR_EXC      += proc_obj.h


COMMON_API_INC      += $(ROOT_DIR)/src/api

COMMON_API_HDR_INC   = $(ROOT_DIR)/src/include/svc.h
COMMON_API_HDR_INC  += $(ROOT_DIR)/src/include/proc_obj.h
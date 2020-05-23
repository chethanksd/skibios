#
# Makefile Common Source file path defintion
#
# author: chetandev.ksd@gmail.com
#


COMMON_SRC_INC       = $(ROOT_DIR)/src/ 
COMMON_SRC_INC      += $(ROOT_DIR)/src/service/ 

COMMON_SRC_EXC       = symgen.c


#
# Common header file to be copied to inc folder
#
COMMON_HDR_INC       = $(ROOT_DIR)/src/include
COMMON_HDR_INC      += $(ROOT_DIR)/src/service
COMMON_HDR_INC      += $(BUILD_PATH)
COMMON_HDR_INC      += $(ALLSRC_PATH)

COMMON_HDR_EXC       = svc.h
COMMON_HDR_EXC      += proc_obj.h


#
# Api Include files to be included in api folder of build
#
COMMON_API_INC      += $(ROOT_DIR)/src/api

COMMON_API_HDR_INC   = $(ROOT_DIR)/src/include/svc.h
COMMON_API_HDR_INC  += $(ROOT_DIR)/src/include/proc_obj.h
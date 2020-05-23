#
# Makefile Common definitions
#
# author: chetandev.ksd@gmail.com
#


#
# Common GCC complier flags
#
COMMON_CFLAGS  = -ffunction-sections
COMMON_CLFAGS += -fdata-sections 

COMMON_CFLAGS += -g -ggdb -gdwarf-3 -gstrict-dwarf 
COMMON_CFLAGS += -Wall 
COMMON_CFLAGS += -MD 
COMMON_CLFAGS += -std=c99 
COMMON_CFLAGS += -c



#
# Unit Process Count SKIBIOS Parameters for Resource Calculation
#
UPC_SKI_PARAM  =  -DNUM_OF_INTERRUPTS=255
UPC_SKI_PARAM +=  -DMAX_TASK_COUNT=1
UPC_SKI_PARAM +=  -DTASK_STACK_SIZE=1024
UPC_SKI_PARAM +=  -DKERNEL_REGION_SIZE=4
UPC_SKI_PARAM +=  -DENABLE_SKIBIOS=1
UPC_SKI_PARAM +=  -DTASK_PER_SEC=100
UPC_SKI_PARAM +=  -DHEAP_BOOKEEPING_SIZE=1
UPC_SKI_PARAM +=  -DUPC_RESOURCE_CAL

#
# Common folder paths
#
ALLSRC_PATH = $(BUILD_PATH)/allsrc
SYMGEN_PATH = $(BUILD_PATH)/symgen
OBJ_PATH    = $(BUILD_PATH)/obj
RESOURCE_PATH = $(BUILD_PATH)/resource_cal
HDR_PATH = $(BUILD_PATH)/hdr
API_PATH = $(BUILD_PATH)/api

#
# Compile include paths
#

COMPILER_IPATH   = -I$(BUILD_PATH)/hdr
COMPILER_IPATH  += -I$(BUILD_PATH)/api



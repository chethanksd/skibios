#
# Source path definition for ossim environment 
#
# author: chetandev.ksd@gmail.com
#

#
# Arch specific source paths to be included for compilation
#

ARCH_SRC_INC   = $(ARCH_PATH)
ARCH_SRC_INC  += $(ROOT_DIR)/ossim/src
ARCH_SRC_INC  += $(ROOT_DIR)/ossim/test/basic.c

ARCH_SRC_EXC  = startup.c
ARCH_SRC_EXC += heap.c
ARCH_SRC_EXC += umpu.c
ARCH_SRC_EXC += svr_umpu.c
ARCH_SRC_EXC += svr_heap.c

#
# Arch specific Include search paths
#

ARCH_HDR_INC  = $(ROOT_DIR)/src/arch/ossim
ARCH_HDR_INC += $(ROOT_DIR)/ossim/src/include

ARCH_HDR_EXC = app_support.h

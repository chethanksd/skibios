#
# Source path definetion for ARM M3/M4 devices 
#
# author: chetandev.ksd@gmail.com
#

#
# Arch specific source paths to be included for compilation
#

ARCH_SRC_INC = $(ARCH_PATH)

ARCH_SRC_EXC = NULL

#
# Arch specific Include search paths
#

ARCH_HDR_INC = $(ROOT_DIR)/src/arch/arm-m3m4

ARCH_HDR_EXC = app_support.h
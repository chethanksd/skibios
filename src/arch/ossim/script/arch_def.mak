#
# Makefile for ossim (only for windows)
#
# author: chetandev.ksd@gmail.com
#

CC = gcc.exe
OC = objcopy.exe
OD = objdump.exe
SZ = size.exe


#
# Arch specific GCC complier flags
#
#ARCH_CFLAGS += NULL

#
# Arch specific Include search paths
#
ARCH_IPATH = -I$(ROOT_DIR)/src/arch/ossim


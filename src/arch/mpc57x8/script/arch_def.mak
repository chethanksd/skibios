#
# Makefile for MPC5 devices 
#
# author: chetandev.ksd@gmail.com
#

CC = powerpc-eabivle-gcc.exe
OC = powerpc-eabivle-objcopy.exe
OD = powerpc-eabivle-objdump.exe
SZ = powerpc-eabivle-size.exe


#
# Arch specific GCC complier flags
#
ARCH_CFLAGS += -mcpu=e200z4 
ARCH_CFLAGS += -mbig 
ARCH_CFLAGS += -mvle
ARCH_CFLAGS += -mregnames
ARCH_CFLAGS += -mhard-float
ARCH_CFLAGS += -mlsp
ARCH_CFLAGS += -fomit-frame-pointer

#
# Arch specific Include search paths
#
ARCH_IPATH = -I$(ROOT_DIR)/src/arch/mpc57x8


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
ARCH_CFLAGS += -Wno-pointer-to-int-cast 
ARCH_CFLAGS += -Wno-int-to-pointer-cast 


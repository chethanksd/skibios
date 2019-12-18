#
# Makefile for ARM M3/M4 
#
# author: chetandev.ksd@gmail.com
#

CC = arm-none-eabi-gcc.exe
OC = arm-none-eabi-objcopy.exe
OD = arm-none-eabi-objdump.exe
SZ = arm-none-eabi-size.exe

ARCH_CFLAGS += -mcpu=cortex-m4 
ARCH_CFLAGS += -march=armv7e-m 
ARCH_CFLAGS += -mthumb 
ARCH_CFLAGS += -mfloat-abi=hard
ARCH_CFLAGS += -mfpu=fpv4-sp-d16
ARCH_CFLAGS += -mno-thumb-interwork


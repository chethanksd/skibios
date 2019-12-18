#
# Makefile for ARM M3/M4 
#
# author: chetandev.ksd@gmail.com
#

CC = arm-none-eabi-gcc.exe
OC = arm-none-eabi-objcopy.exe
OD = arm-none-eabi-objdump.exe
SZ = arm-none-eabi-size.exe

CFLAGS += -mcpu=cortex-m4 
CFLAGS += -march=armv7e-m 
CFLAGS += -mthumb 
CFLAGS += -mfloat-abi=hard
CFLAGS += -mfpu=fpv4-sp-d16
CFLAGS += -mno-thumb-interwork 

CFLAGS += -ffunction-sections
CLFAGS += -fdata-sections 

CFLAGS += -g -ggdb -gdwarf-3 -gstrict-dwarf 
CFLAGS += -Wall 
CFLAGS += -specs="nosys.specs"
CFLAGS += -MD 
CLFAGS += -std=c99 
CFLAGS += -c


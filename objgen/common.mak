#
# Makefile Common definitions
#
# author: chetandev.ksd@gmail.com
#


#
# Common GCC complier flags
#
COMMOM_CFLAGS = -ffunction-sections
COMMOM_CLFAGS += -fdata-sections 

COMMOM_CFLAGS += -g -ggdb -gdwarf-3 -gstrict-dwarf 
COMMOM_CFLAGS += -Wall 
COMMOM_CFLAGS += -specs="nosys.specs"
COMMOM_CFLAGS += -MD 
COMMOM_CLFAGS += -std=c99 
COMMOM_CFLAGS += -c
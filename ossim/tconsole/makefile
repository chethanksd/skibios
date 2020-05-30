#
# SKIBIOS Makefile
#
# Note: Following Paramaters has to be defined
# 
# $(BIN)    	- Location of GCC binaries

arg_check:
ifndef TC_BIN
	$(error "ERROR! TC_BIN path name not defined")
endif


TC_CC = gcc.exe

TC_CFLAGS  = -ffunction-sections
TC_CLFAGS += -fdata-sections 
TC_CFLAGS += -g -ggdb -gdwarf-3 -gstrict-dwarf 
TC_CFLAGS += -Wall 
TC_CFLAGS += -MD 
TC_CLFAGS += -std=c99 
TC_CFLAGS += -c
TC_CFLAGS += -Wno-pointer-to-int-cast 
TC_CFLAGS += -Wno-int-to-pointer-cast 


tconsole:
	$(TC_BIN)/$(TC_CC) $(TC_CFLAGS) ./task_console.c
	$(TC_BIN)/$(TC_CC) -Wl,--gc-sections -o./task_console.exe ./task_console.o
	rm ./task_console.o
	rm ./task_console.d
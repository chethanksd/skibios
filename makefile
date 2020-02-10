#
# SKIBIOS Makefile
#
# Note: Following Paramaters has to be defined
# 
# $(SKIBIOS)	- Root folder
# $(BIN)    	- Location of GCC binaries
# $(BUILD)  	- Output Build path

arg_check:
ifndef SKIBIOS_PATH
	$(error "ERROR! SKIBIOS_PATH path name not defined")
endif

ifndef BIN
	$(error "ERROR! BIN path name not defined")
endif

ifndef BUILD
	$(error "ERROR! BUILD path name not defined")
endif


.PHONY: objgen
objgen: arg_check
	$(PYTHON) $(SKIBIOS_PATH)/objgen/start.py $(SKIBIOS_PATH)/tm4c1294ncpdt.xml $(SKIBIOS_PATH)/param.xml $(BUILD) $(BIN)

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

#
# Local defines
#
START_SCRIPT := $(SKIBIOS_PATH)/objgen/start.py
OSSIM_PATH	 := $(SKIBIOS_PATH)/ossim
OSSIM_BUILD	 := $(SKIBIOS_PATH)/ossim/build


#
# For embedded target
#
#
.PHONY: objgen
objgen: arg_check
	$(PYTHON) $(START_SCRIPT) $(SKIBIOS_PATH)/tm4c1294ncpdt.xml $(SKIBIOS_PATH)/param.xml $(BUILD) $(BIN)


#
# for OS Simulation
#
#
.PHONY: ossim
ossim: arg_check
	$(PYTHON) $(START_SCRIPT) $(OSSIM_PATH)/ossim_device.xml $(OSSIM_PATH)/ossim_param.xml $(OSSIM_BUILD) $(BIN)

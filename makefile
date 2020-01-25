#
# SKIBIOS Makefile
#
# Note: Following Paramaters has to be defined
# 
# $(ROOT)   - Root folder
# $(BIN)    - Location of GCC binaries
# $(GCC)    - Name of the GCC compilier
# $(BUILD)  - Output Build path

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
# SKIBIOS Parameters
#
ENABLE_SKIBIOS			=		1
KERNEL_REGION_SIZE      = 		16
UPPER_REGION_SIZE		=		3
GHMB_REGION_SIZE		=		1
PROCESS_STACK_SIZE		=		512

PROCESS_PER_SEC			=		50
PROCESS_ID_START 		=		4000
ENABLE_SAFE_LOCK		= 		1
DISABLE_BUFFER			=  		0

SKIBIOS_PARAM= -DPROCESS_STACK_SIZE=$(PROCESS_STACK_SIZE) -DUPPER_REGION_SIZE=$(UPPER_REGION_SIZE) \
-DKERNEL_REGION_SIZE=$(KERNEL_REGION_SIZE) -DPROCESS_PER_SEC=$(PROCESS_PER_SEC) \
-DENABLE_SAFE_LOCK=$(ENABLE_SAFE_LOCK) -DDISABLE_BUFFER=$(DISABLE_BUFFER)



skibios:
	$(BIN)/$(GCC) -S -I$(ROOT)/skibios/src/include $(CFLAGS) $(SKIBIOS_PATH)/src/symgen.c -o$(BUILD)/symgen.S
	grep "OFFSET_OF" $(BUILD)/symgen.S > $(BUILD)/symgen.txt
	grep "SIZE_OF" $(BUILD)/symgen.S >> $(BUILD)/symgen.txt
	cat $(BUILD)/symgen.S | awk '($$1 == "->") { print "#define " $$2 " " $$3 }' > $(BUILD)/symgen.h
	$(SKIBIOS_PATH)/skibios.exe -b$(BIN) -o$(BUILD) -d$(SKIBIOS_PATH)/tm4c1294ncpdt.xml $(SKIBIOS_PARAM) -g

.PHONY: objgen
objgen: arg_check
	$(PYTHON) $(SKIBIOS_PATH)/objgen/start.py $(SKIBIOS_PATH)/tm4c1294ncpdt.xml $(SKIBIOS_PATH)/param.xml $(BUILD) $(BIN)

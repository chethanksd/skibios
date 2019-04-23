#
# SKIBIOS Makefile
#
# Note: Following Paramaters has to be defined
# 
# $(ROOT)   - Root folder
# $(BIN)    - Location of GCC binaries
# $(GCC)    - Name of the GCC compilier
# $(BUILD)  - Output Build path

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
	$(BIN)/$(GCC) -S -I./src/include $(CFLAGS) ./src/symgen.c -o$(BUILD)/symgen.S
	grep "OFFSET_OF" $(BUILD)/symgen.S > $(BUILD)/symgen.txt
	grep "SIZE_OF" $(BUILD)/symgen.S >> $(BUILD)/symgen.txt
	cat $(BUILD)/symgen.S | awk '($$1 == "->") { print "#define " $$2 " " $$3 }' > $(BUILD)/symgen.h
	./skibios.exe -b$(BIN) -o$(BUILD) -d./tm4c1294ncpdt.xml $(SKIBIOS_PARAM) -g

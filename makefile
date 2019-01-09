#
# SKIBIOS Makefile
#
# Note: Following Paramaters has to be defined
# 
# $(BIN)    - Location of GCC binaries

#
# SKIBIOS configurations
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


all: skibios

skibios:
	./skibios.exe -b$(BIN) -o$(ROOT)/proj/build -d./tm4c1294ncpdt.xml $(SKIBIOS_PARAM) -g


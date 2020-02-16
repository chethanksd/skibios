#
# Source path definetion for MPC5 devices 
#
# author: chetandev.ksd@gmail.com
#


ARCH_SRC_INC  = $(ARCH_PATH)

ARCH_SRC_EXC  = startup.c
ARCH_SRC_EXC += heap.c
ARCH_SRC_EXC += kernel.c
ARCH_SRC_EXC += interrupt.c
ARCH_SRC_EXC += umpu.c
ARCH_SRC_EXC += svr_interrupts.c
ARCH_SRC_EXC += svr_process.c
ARCH_SRC_EXC += svr_umpu.c


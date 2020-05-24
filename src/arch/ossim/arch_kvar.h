#ifndef _ARCH_KVAR_H_
#define _ARCH_KVAR_H_

#include <windows.h>

extern HANDLE kernel_service_lock;

#ifndef UPC_RESOURCE_CAL
extern uint32_t sim_kernel_region[];
#endif

extern uint32_t _proc_heap_addr;

#endif
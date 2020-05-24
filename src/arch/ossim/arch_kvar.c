/*
 * arch specific kernel variable file
 *
 * author: chetandev.ksd@gmail.com
 *
 */

#include <stdint.h>
#include <arch_kvar.h>

#ifndef UPC_RESOURCE_CAL
#include <param.h>
#endif

// local defines
#ifndef UPC_RESOURCE_CAL
#define KERNEL_SECTON_SIZE_IN_BYTES (KERNEL_SECTION_SIZE * 1024U)
#endif

// kenel service lock mutex
// this variable is required for synchornization between kernel service rountine
// scheduler and user tasks in OSSIM Environment
HANDLE kernel_service_lock;


// simulated kernel region as allocated in target mcu
#ifndef UPC_RESOURCE_CAL
uint32_t sim_kernel_region[KERNEL_SECTON_SIZE_IN_BYTES];
#endif

// process stack start address
uint32_t _proc_heap_addr;
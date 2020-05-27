/*
 * arch specific kernel variable file
 *
 * author: chetandev.ksd@gmail.com
 *
 */

#include <stdint.h>
#include <stdbool.h>
#include <arch_kvar.h>

#ifndef UPC_RESOURCE_CAL
#include <param.h>
#endif

// local defines
#ifndef UPC_RESOURCE_CAL
#define KERNEL_SECTON_SIZE_IN_BYTES (KERNEL_SECTION_SIZE * 1024U)
#endif

// kernel service lock mutex
// this variable is required for synchornization between kernel service routine,
// scheduler and user tasks in OSSIM Environment
HANDLE kernel_service_lock;

// simulated kernel region as allocated in target mcu
#ifndef UPC_RESOURCE_CAL
uint32_t sim_kernel_region[KERNEL_SECTON_SIZE_IN_BYTES];
#endif

// task stack start address
uint32_t _proc_heap_addr;

// pc task handle array
// this array is required to hold handle to the task which is required for 
// controlling that task
HANDLE pc_task_handle[MAX_TASK_COUNT];

// first schedule
// required for scheduler to setup process states at first time
// scheduling
bool first_schedule;
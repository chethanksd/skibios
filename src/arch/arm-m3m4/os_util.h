#ifndef _OS_UTIL_H_
#define _OS_UTIL_H_

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

extern uint8_t arch_kernel_init();
extern uint8_t arch_mpu_init();
extern uint8_t arch_task_stack_init(uint32_t *pheap_ptr, uint32_t ptr_func, uint32_t proc_arg);

#ifdef __cplusplus
}
#endif

#endif
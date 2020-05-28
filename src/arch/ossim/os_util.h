#ifndef _OS_UTIL_H_
#define _OS_UTIL_H_

#include <stdint.h>
#include <task_obj.h>

#ifdef __cplusplus
extern "C" {
#endif

extern uint8_t arch_kernel_init();
extern uint8_t arch_mpu_init();
extern uint8_t arch_task_stack_init(uint32_t task_index, uint32_t ptr_func, uint32_t proc_arg);
extern uint32_t call_kernel_service(uint32_t svc_code, uint32_t arg1, uint32_t arg2, uint32_t arg3, uint32_t arg4);

extern uint32_t enable_os_timer();
extern uint32_t trigger_os_timer();

extern uint8_t vector_table_relocate();
extern uint8_t arch_interrupt_enable(uint32_t interrupt);
extern uint8_t arch_interrupt_disable(uint32_t interrupt);
extern uint8_t arch_interrupt_register(uint32_t interrupt, uint32_t handler);
extern uint8_t arch_interrupt_priority(uint8_t interrupt, uint8_t priority);

#ifdef __cplusplus
}
#endif

#endif
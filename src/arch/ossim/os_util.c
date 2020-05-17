/*
 * os util code for arm-m3m4
 *
 *
 * author: chetandev.ksd@gmail.com
 *
 */


#include <os_util.h>
#include <access.h>
#include <error.h>
#include <param.h>
#include <kvar.h>
#include <interrupt.h>
#include <os_support.h>

// global variables
uint32_t _proc_heap_addr = 0;

uint8_t arch_kernel_init() {

    return ERROR_NONE;

}

uint8_t arch_mpu_init() {

    return ERROR_NONE;

}

uint8_t arch_task_stack_init(uint32_t task_index, uint32_t ptr_func, uint32_t proc_arg) {

    return ERROR_NONE;

}

uint32_t call_kernel_service(uint32_t svc_code, uint32_t arg1, uint32_t arg2, uint32_t arg3, uint32_t arg4) {

    return ERROR_NONE;
    
}

uint8_t vector_table_relocate() {

    return ERROR_NONE;

}


uint8_t arch_interrupt_enable(uint32_t interrupt) {

    return ERROR_NONE;

}

uint8_t arch_interrupt_disable(uint32_t interrupt) {

    return ERROR_NONE;

}

uint8_t arch_interrupt_register(uint32_t interrupt, uint32_t handler) {

    return ERROR_NONE;

}

uint8_t arch_interrupt_priority(uint8_t interrupt, uint8_t priority)  {

    return ERROR_NONE;

}

uint8_t switch_mcu_mode() {
    
    return ERROR_NONE;

}

uint8_t heap_init() {

    return ERROR_NONE;
    
}

uint8_t btask_psp_correction() {

    return ERROR_NONE;

}

uint8_t mutex_lock() {

    return ERROR_NONE;

}

uint8_t mutex_unlock() {

    return ERROR_NONE;

}

uint8_t arch_semaphore_inc() {

    return ERROR_NONE;

}

uint8_t arch_semaphore_dec() {

    return ERROR_NONE;
    
}
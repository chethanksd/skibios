/*
 * os util code for arm-m3m4
 *
 *
 * author: chetandev.ksd@gmail.com
 *
 */


#include <os_util.h>
#include <error.h>
#include <param.h>
#include <kvar.h>
#include <arch_kvar.h>
#include <interrupt.h>
#include <os_support.h>

#include <stdio.h>

// local defines
#define TASK_STACK_START_INDEX  (UPPER_REGION_SIZE * 1024)

// local function declaration
uint8_t print_welcome_message();


uint8_t arch_kernel_init() {

    print_welcome_message();

    // attempt to create kernel_service_lock mutex
    // default security attributes
    // initially not owned
    // unnamed mutex
    kernel_service_lock = CreateMutex(NULL, FALSE, NULL);             

    if (kernel_service_lock == NULL) 
    {
        printf("Failed to create kernel service mutex error: %lu\n", GetLastError());
        exit(1);
    

    // initialize _proc_heap_addr
    // this variable is used to hold address of task stack start in target device
    // but in ossim environment we dont need task stack, but we will simulate its
    // execution in ossim environment
    _proc_heap_addr = ((uint32_t) &sim_kernel_region) + TASK_STACK_START_INDEX;

    return ERROR_NONE;

}

uint8_t print_welcome_message() {

    printf("***********************************************\n");
    printf(" OSSIM Environment Version 1.0\n");
    printf("***********************************************\n");
    return ERROR_NONE;
}

uint8_t arch_mpu_init() {

    return ERROR_NONE;

}

uint8_t arch_task_stack_init(uint32_t task_index, uint32_t ptr_func, uint32_t proc_arg) {

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

uint8_t arch_semaphore_inc() {

    return ERROR_NONE;

}

uint8_t arch_semaphore_dec() {

    return ERROR_NONE;
    
}
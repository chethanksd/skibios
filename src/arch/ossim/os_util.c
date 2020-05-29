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
#include <svc.h>
#include <arch_kvar.h>
#include <interrupt.h>
#include <os_support.h>

#include <stdio.h>

// local defines
#define TASK_STACK_START_INDEX  (UPPER_REGION_SIZE * 1024)

// local function declaration
uint8_t print_welcome_message();

// external function declaration
extern void resolve_end(void);


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
    }

    // initialize _proc_heap_addr
    // this variable is used to hold address of task stack start in target device
    // but in ossim environment we dont need task stack, but we will simulate its
    // execution in ossim environment
    _proc_heap_addr = ((uint32_t) &sim_kernel_region) + TASK_STACK_START_INDEX;


    // arch specific init
    first_schedule = false;

    return ERROR_NONE;

}

uint8_t print_welcome_message() {

    printf("***********************************************\n");
    printf(" OSSIM Environment Version 1.0\n");
    printf("***********************************************\n");
    return ERROR_NONE;
}

uint8_t switch_mcu_mode() {

    uint32_t error;

    // call kernel service routine to implement START_SCHEDULER
    error = call_kernel_service(START_SCHEDULER, 0, 0, 0, 0);
    
    return error;

}

uint8_t arch_task_stack_init(uint32_t task_index, uint32_t ptr_func, uint32_t proc_arg) {

    task_func_t ptr_temp;
    uint32_t *pheap_ptr;
    HANDLE handle;

    // simulate stack preparation as per target mcu (currently below code simulates mpc5)
    PSP_Array[task_index] = ((unsigned int) (PSP_Array[task_index])) + ((TASK_STACK_SIZE - 1) * 4) - 22 * 4;
    pheap_ptr = (uint32_t*)PSP_Array[task_index];

    pheap_ptr[3] = ptr_func;
    pheap_ptr[4] = 0xD000;
    pheap_ptr[6] = (uint32_t) &resolve_end;
    pheap_ptr[10] = proc_arg;

    // this is where actual thread is created to simulate target mcu task
    ptr_temp = (task_func_t)ptr_func;
    handle = CreateThread(NULL, 0, ptr_temp, (LPVOID)proc_arg, CREATE_SUSPENDED, NULL);

    if(handle == NULL) {
       printf("failed to create task, error: %lu\n", GetLastError());
       goto quit_error;
    }
    
    pc_task_handle[task_index] = handle;

quit_error:

    return ERROR_NONE;

}

uint32_t ossim_suspend_task(uint32_t task) {

    if(first_schedule == true) {
        SuspendThread(pc_task_handle[task]);
    } else {
        first_schedule = true;
    }

    return ERROR_NONE;

}

uint32_t ossim_resume_task(uint32_t task) {
    
    ResumeThread(pc_task_handle[task]);

    return ERROR_NONE;

}

uint8_t arch_mpu_init() {

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
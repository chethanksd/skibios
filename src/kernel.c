//******************************************************************//
//    Kernel Code SKIBIOS RTOS                                      //
//            copyright@2018                                        //
//    Modified Date: 20-02-2018                                     //
//    Author: chetandev.ksd@gmail.com                               //
// Disclaimer:                                                      //
//******************************************************************//

#include <error.h>
#include <kernel.h>
#include <heap.h>
#include <share.h>
#include <task.h>
#include <interrupt.h>
#include <permissions.h>
#include <svc.h>
#include <os_support.h>
#include <kvar.h>
#include <param.h>
#include <access.h>
#include <svctable.h>
#include <defines.h>
#include <os_timer.h>

#define TASK_PRIO_CURRENT    0
#define TASK_PRIO_STASHED    1


/* Local Functions */
uint32_t svc_service_hand_over(uint32_t *svc_num, uint32_t *arguments);
uint32_t svc_service_device_reset(uint32_t *svc_num, uint32_t *arguments);
uint32_t svc_service_cpu_freq_update(uint32_t *svc_num, uint32_t *arguments);
uint32_t svc_service_start_scheduler(uint32_t *svc_num, uint32_t *arguments);
uint32_t svc_service_hwreg_write(uint32_t *svc_num, uint32_t *arguments);
uint32_t svc_service_hwreg_read(uint32_t *svc_num, uint32_t *arguments);

void scheduler(void);

/* External Kernel Function Declarations */
extern void switch_mcu_mode(void);
extern void btask_psp_correction();
extern void resolve_end(void);
extern void BaseTask();

/* External Kernel Variables */
extern uint32_t _proc_heap_addr;

/* FLASH Constants */
const uint32_t zero_ref = 0;
const uint32_t base_mutex = 1;


uint32_t kernel_init(void) {

    uint32_t error;
    uint16_t i;

    // set first start to false
    first_start = false;

    // Relocate the Vector Table
    vector_table_relocate();

    error = arch_kernel_init();

    if(error) {
        goto quit_error;
    }

    error = os_timer_init(cpu_freq);

    if(error) {
        goto quit_error;
    }

    // Initialize Process states
    for(i = 0; i < MAX_TASK_COUNT; i++) {
        state[i] = TASK_STATE_IDLE;
    }

    // Get Process Stack start Address
    #ifdef OSSIM_RUN
    pstack_addr = _proc_heap_addr;
    #else
    pstack_addr = (uint32_t)&_proc_heap_addr;
    #endif

    // Initialize MPU
    error = arch_mpu_init(); 

    if(error) {
        goto quit_error;
    }

    // Initialize Base task
    // with these lines of code it should be obvious that
    // always 0th index process will be BaseTask
    // which implies if current_task = 0 means BaseTask
    // is in active state
    // 
    task_init(&base_task);
    base_task.ptr_func = (TASK_RETURN_T*)&BaseTask;
    base_task.priority = 0;
    task_start(&base_task);
    
    // base task should have all permissions
    permissions[0] = 0xFFFF;
    current_task = 0;

    // Initialize Heap Memory
    heap_init(); 

quit_error:

    return ERROR_NONE;

}

uint32_t  start_scheduler(void) {

    if(first_start == true) {
        return ERROR_SCHEDULER_ALREADY_RUNNING;
    }

    current_task = 0;

    // call arch defined function to switch mcu mode from supervisor
    // to user mode and then call kernel service START_SCHEDULER
    switch_mcu_mode();

#ifndef OSSIM_RUN
    while(1) {
        // This part of code should never be reached in embedded target
        // In OSSIM environment this part of the code should be omitted
    }
#endif

    return ERROR_NONE;

}

// svc services dispatched by svc_dispatch table

uint32_t svc_service_hand_over(uint32_t *svc_num, uint32_t *arguments) {

    if(self_kill == false) {
        // set the State of current process to sleep
        state[current_task] = TASK_STATE_SLEEP;

    } else {

        self_kill=false;

    }

    normal_schedule = false;

    TRIGGER_SCHEDULER();

    return ERROR_NONE;

}

uint32_t svc_service_start_scheduler(uint32_t *svc_num, uint32_t *arguments) {

    uint32_t i;

    maxp_level = 127;

    //base task should have all permissions
    permissions[0] = 0xFFFF;

    //rest of the task will have by default no permissions
    for(i = 1; i < MAX_TASK_COUNT; i++) {
        permissions[i] = 0;
    }

    // at start current_stask is set as 0, which is always base_task
    // start_process prepares entire stack. During the first time context switch
    // between base_task -> high_priority task, the context siwtch algorithm may overwrite
    // existing stack signature for base_task. based on architecture appropriate 
    // psp correction needs to be applied
    btask_psp_correction();

    first_start = true;
    self_kill = false;
    *svc_num = HAND_OVER;
    
    return ERROR_NONE;

}

uint32_t svc_service_cpu_freq_update(uint32_t *svc_num, uint32_t *arguments) {

    uint32_t error = ERROR_NONE;

    uint32_t new_cpu_freq = arguments[0];

    error = os_timer_config(new_cpu_freq);

    return error;

}

uint32_t svc_service_hwreg_write(uint32_t *svc_num, uint32_t *arguments) {

    uint32_t error = ERROR_NONE;

    uint32_t hwreg = arguments[0];
    uint32_t value = arguments[1];

    // check access permission of currrent_task for hwreg operation
    if((permissions[current_task] & (1 << PERMISSION_HWREG)) != (1 << PERMISSION_HWREG)) {
        error = ERROR_INSUFFICIENT_PERMISSION;
        goto quit_error;
    }

    // ToDo: Argument check

    *((uint32_t*)hwreg) = value;

quit_error:

    return error;

}

uint32_t svc_service_hwreg_read(uint32_t *svc_num, uint32_t *arguments) {

    uint32_t error = ERROR_NONE;

    uint32_t hwreg = arguments[0];
    uint32_t return_value = arguments[1];

    // check access permission of currrent_task for hwreg operation
    if((permissions[current_task] & (1 << PERMISSION_HWREG)) != (1 << PERMISSION_HWREG)) {
        error = ERROR_INSUFFICIENT_PERMISSION;
        goto quit_error;
    }

    // ToDo: Argument check

    *((uint32_t*)return_value) = *((uint32_t*)hwreg);

quit_error:

    return error;
}

uint32_t svc_service_device_reset(uint32_t *svc_num, uint32_t *arguments) {

    // Perform a software reset request.  This request causes the device to
    // reset, no further code is executed

    TRIGGER_DEVICE_RESET();

    // The device should have reset, so this should never be reached.  Just in
    // case, loop forever

    while(1) {

    }

    return ERROR_NONE;

}

void scheduler() {

    uint8_t k;
    uint8_t level;
    uint8_t lptr;

    // Stop the scheduler Temporarily
    DISABLE_SCHEDULER();

    if(normal_schedule == true) {

        // Set the State of current process to sleep
        state[current_task] = TASK_STATE_SLEEP;

    } else {

        normal_schedule = true;

    }

    #ifdef OSSIM_RUN
    ossim_suspend_task(current_task);
    #endif

    // Check the Hibernating condition of all Hibernating function
    k = next_task;

    do {

        if(state[k] == TASK_STATE_HIBERNATE_G) {

            if((*(op1[k]) - *(op2[k])) >= hib_value[k]) {

                if(priority_Array[k][TASK_PRIO_CURRENT] == max_level) {

                    hlc--;
                    alc++;

                }

                state[k] = TASK_STATE_SLEEP;

            }

        }

        if(state[k] == TASK_STATE_HIBERNATE_L) {

            if((*(op1[k]) - *(op2[k])) <= hib_value[k]) {

                if(priority_Array[k][TASK_PRIO_CURRENT] == max_level) {

                    hlc--;
                    alc++;

                }

                state[k] = TASK_STATE_SLEEP;

            }

        }

        k = k + jmp_list[k];

    } while(k != next_task);

    level = max_level;
    lptr = lstash_ptr - 1;

    // Schedule Next Active Process Object
    while(1) {

        next_task = next_task + jmp_list[next_task];

        if(current_task == next_task && enable_dws==true) {
            lptr--;
            level = level_stash[lptr];
        }

        if(state[next_task] == TASK_STATE_SLEEP && priority_Array[next_task][TASK_PRIO_CURRENT] == level) {
            break;
        }
        
    }

    state[next_task] = TASK_STATE_ACTIVE;

    #ifdef OSSIM_RUN
    ossim_resume_task(next_task);
    #endif

    // Clear Current Register of os timer
    SCHEDULER_TIMER_RESET();

    // Disable the scheduler if there is only one process i.e, only Base Proccess is present
    if(alc == 1 && hlc == 0) {
        DISABLE_SCHEDULER();
    }else{
        ENABLE_SCHEDULER();
    }

    if(level != max_level) {
        ENABLE_SCHEDULER();
    }

    // set context_swtich handler to pending to perform context switch
    // when this scheduler exits
    TRIGGER_CONTEXT_SWITCH();

}

// Update CPU Frequency function

uint32_t cpu_freq_update(uint32_t frequency) {

    uint32_t error;

    SVC_CPU_FREQ_UPDATE(frequency, error);

    return error;

}

uint32_t hwreg_write(uint32_t register_address, uint32_t value) {

    uint32_t error;


    SVC_HWREG_WRITE(register_address, value, error);


    return error;

}

uint32_t hwreg_read(uint32_t register_address, uint32_t *value) {

    uint32_t error;

    SVC_HWREG_READ(register_address, (uint32_t)value, error);

    return error;

}

// Device Reset Function

uint32_t device_reset() {

    // only BaseTask have access to device_reset function
    if(current_task != 0) {
        return ERROR_INSUFFICIENT_PERMISSION;
    }

    svc(DEVICE_RESET);

    return ERROR_NONE;

}

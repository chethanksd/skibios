/*
 * os timer source code for ossim
 *
 * author: chetandev.ksd@gmail.com
 *
 */

#include <os_timer.h>
#include <access.h>
#include <error.h>
#include <kvar.h>
#include <param.h>
#include <arch_kvar.h>
#include <os_util.h>

#include <stdio.h>

// externel functions
extern void scheduler();

// local function declaration
static VOID CALLBACK timer_routine(PVOID lpParam, BOOLEAN TimerOrWaitFired);

// externel global variables
extern volatile bool hand_over_lock;

// local global variable declaration
static HANDLE timer_queue_handler = NULL;
static HANDLE timer_handler = NULL;

// global variables
uint32_t timer_code = 0;
volatile uint32_t scheduler_on = false;
volatile uint32_t schedule_count = 0;
volatile uint32_t scheduler_step = 0;
volatile bool halt_scheduler = false;



uint32_t os_timer_init(uint32_t new_cpu_freq) {

    uint32_t error = ERROR_NONE;

    // Create the timer queue.
    timer_queue_handler = CreateTimerQueue();
    if(timer_queue_handler == NULL)
    {
        printf("CreateTimerQueue failed (%lu)\n", GetLastError());
        exit(1);
    }

    // call dummy os_timer_config
    // requird for change CPU_FREQ svc code
    error = os_timer_config(new_cpu_freq);

    if(error) {
        goto quit_error;
    }

    // initialize variables
    scheduler_on = false;

quit_error:

    return error; 

}

uint32_t os_timer_config(uint32_t new_cpu_freq) {

    return ERROR_NONE;
 
}

uint32_t enable_os_timer() {
    
    uint32_t timer_load;

    timer_load = ((float)(1000.0/TASK_PER_SEC));

    while((halt_scheduler == true) && (scheduler_step == 0)) {
        // wait here until halt_scheduler is true;
    }

    if(scheduler_step != 0) {
        scheduler_step--;
    }

    if (!CreateTimerQueueTimer(&timer_handler, 
                                timer_queue_handler, 
                                (WAITORTIMERCALLBACK)timer_routine, 
                                &timer_code, 
                                timer_load, 
                                0, 
                                0)) {
        printf("os timer enable CreateTimerQueueTimer failed (%lu)\n", GetLastError());
    } else {
        scheduler_on = true;
    }

    return ERROR_NONE;

}

uint32_t trigger_os_timer() {

    // set hand_over_lock to true
    // only hand over, hibernat, priority promote/demote
    // will be calling this function
    hand_over_lock = true;
    
    // if scheduler is already on
    // then no need to trigger it or else mutiple timer callbacks
    // will be execcuted
    if(scheduler_on == true) {
        return ERROR_NONE;  
    }

    if (!CreateTimerQueueTimer(&timer_handler, 
                                timer_queue_handler, 
                                (WAITORTIMERCALLBACK)timer_routine, 
                                &timer_code, 
                                0, 
                                0, 
                                0)) {
        printf("os timer trigger CreateTimerQueueTimer failed (%lu)\n", GetLastError());
    } else {
        scheduler_on = true;
    }

    return ERROR_NONE;

}

uint32_t disable_os_timer() {
    
    scheduler_on = false;

    return ERROR_NONE;

}

VOID CALLBACK timer_routine(PVOID lpParam, BOOLEAN TimerOrWaitFired) {
    
    DWORD wait_result;    
    
    // when CreateTimerQueueTimer at os timer init is called with 
    // DueTimer=0, Period=0, this callback function is called. So,
    // we don't want any undesirable execution from this function
    if(first_start == false) {
        return;
    }

    // try to lock kernel service mutex
    // attempt to lock the mutex
    wait_result = WaitForSingleObject(kernel_service_lock, INFINITE);

    switch (wait_result) {

        // got ownership of kernel service lock mutex
        case WAIT_OBJECT_0: 
            break;

        // got ownership of abandoned kernel_service lock mutex
        case WAIT_ABANDONED: 
            return;
    }

    // call scheduler
    scheduler();

    // simulate one of the context switch action
    schedule_count++;
    current_task = next_task;

    // remove hand_over_lock unconditionally
    hand_over_lock = false;

    // release kernel service lock for other task to use
    ReleaseMutex(kernel_service_lock);

}
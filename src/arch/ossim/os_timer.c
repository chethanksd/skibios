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

// local function declaration
static VOID CALLBACK timer_routine(PVOID lpParam, BOOLEAN TimerOrWaitFired);

// local global variable declaratio
static HANDLE timer_queue_handler = NULL;
static HANDLE timer_handler = NULL;
uint32_t timer_code = 0;
uint32_t schedule_count = 0;

// externel functions
extern void scheduler();


uint32_t os_timer_init(uint32_t new_cpu_freq) {

    uint32_t error = ERROR_NONE;

    // Create the timer queue.
    timer_queue_handler = CreateTimerQueue();
    if(timer_queue_handler == NULL)
    {
        printf("CreateTimerQueue failed (%lu)\n", GetLastError());
        exit(1);
    }

    error = os_timer_config(new_cpu_freq);

    if(error) {
        goto quit_error;
    }

quit_error:

    return error; 

}

uint32_t os_timer_config(uint32_t new_cpu_freq) {

    return ERROR_NONE;
 
}

uint32_t enable_os_timer() {
    
    uint32_t timer_load;

    timer_load = ((float)(1000.0/TASK_PER_SEC));

    if (!CreateTimerQueueTimer(&timer_handler, 
                                timer_queue_handler, 
                                (WAITORTIMERCALLBACK)timer_routine, 
                                &timer_code, 
                                timer_load, 
                                0, 
                                0)) {
        printf("os timer enable CreateTimerQueueTimer failed (%lu)\n", GetLastError());
    }

    return ERROR_NONE;

}

uint32_t trigger_os_timer() {

    if (!CreateTimerQueueTimer(&timer_handler, 
                                timer_queue_handler, 
                                (WAITORTIMERCALLBACK)timer_routine, 
                                &timer_code, 
                                0, 
                                0, 
                                0)) {
        printf("os timer trigger CreateTimerQueueTimer failed (%lu)\n", GetLastError());
    }

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
    schedule_count++;
    printf("schedule_count: %d, current_task: %d, next_task: %d\n", schedule_count, current_task, next_task);
    current_task = next_task;

    // release kernel service lock for other task to use
    ReleaseMutex(kernel_service_lock);

}
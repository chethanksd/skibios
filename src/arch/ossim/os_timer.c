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

#include <stdio.h>

// local function declaration
static VOID CALLBACK timer_routine(PVOID lpParam, BOOLEAN TimerOrWaitFired);

// local global variable declaratio
static HANDLE timer_queue_handler = NULL;
static HANDLE timer_handler = NULL;
uint32_t arg = 1000;


uint32_t os_timer_init(uint32_t new_cpu_freq) {

    uint32_t error = ERROR_NONE;

    // Create the timer queue.
    timer_queue_handler = CreateTimerQueue();
    if(timer_queue_handler == NULL)
    {
        printf("CreateTimerQueue failed (%lu)\n", GetLastError());
        exit(1);
    }

    if (!CreateTimerQueueTimer(&timer_handler, 
                                timer_queue_handler, 
                                (WAITORTIMERCALLBACK)timer_routine, 
                                &arg, 
                                0, 
                                0, 
                                0)) {
        printf("CreateTimerQueueTimer failed (%lu)\n", GetLastError());
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
    printf("timer called with arg: %d\n", *((uint32_t*)lpParam));

    // release kernel service lock for other task to use
    ReleaseMutex(kernel_service_lock);

}
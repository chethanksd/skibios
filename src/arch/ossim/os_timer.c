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
    
    // when CreateTimerQueueTimer at os timer init is called with 
    // DueTimer=0, Period=0, this callback function is called. So,
    // we don't want any undesirable execution from this function
    if(first_start == false) {
        return;
    }

    printf("timer called with arg: %d\n", *((uint32_t*)lpParam));

}
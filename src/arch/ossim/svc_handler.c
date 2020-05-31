/*
 * svc handler code for ossim
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
#include <svctable.h>

#include <stdio.h>

// local type declaration
typedef struct{
    uint32_t svc_code;
    uint32_t arguments[0];
}svc_handler_param_t;

// local function declaration
TASK_RETURN_T svc_handler(svc_handler_param_t *svc_param);

// extern variables
const uint32_t TOTAL_SVC_COUNT;

// local global variables
static uint32_t (*svc_service)(uint32_t *svc_code, uint32_t *arguments);

// global variables
bool volatile hand_over_lock = false;


uint32_t call_kernel_service(uint32_t svc_code, uint32_t arg1, uint32_t arg2, uint32_t arg3, uint32_t arg4) {

    uint32_t error = ERROR_NONE;
    svc_handler_param_t svc_param;
    task_func_t ptr_temp;
    DWORD wait_result;
    DWORD exitcode;
    HANDLE handle;

    // prepare SVC param for kernel service handler
    svc_param.svc_code = svc_code;
    svc_param.arguments[0] = arg1;
    svc_param.arguments[1] = arg2;
    svc_param.arguments[3] = arg3;
    svc_param.arguments[4] = arg4;

    // attempt to lock the mutex
    wait_result = WaitForSingleObject(kernel_service_lock, INFINITE);

    switch (wait_result) {

        // got ownership of kernel service lock mutex
        case WAIT_OBJECT_0: 
            break;

        // got ownership of abandoned kernel_service lock mutex
        case WAIT_ABANDONED: 
            return error;
    }

    // start kernel service routine
    ptr_temp = (task_func_t)svc_handler;
    handle = CreateThread(NULL, 0, ptr_temp, (LPVOID)&svc_param, 0, NULL);

    if(handle == NULL) {
        printf("Kernel service routine CreateThread error: %lu\n", GetLastError());
        exit(1);
    }

    // wait for thread to exit
    wait_result = WaitForSingleObject(handle, INFINITE);

    switch (wait_result) {

         // the thread handle is signaled - the thread has terminated
        case WAIT_OBJECT_0: 
            break;

        // the thread handle is not signaled - the thread is still alive
        case WAIT_ABANDONED: 
            return error;
    }

    if (!GetExitCodeThread(handle, &exitcode)) {
        // handle error from GetExitCodeThread()...
        printf("unable to get exit code for kernel service routine error: %lu\n", GetLastError());
        exit(1);
    }

    // release kernel service lock for other task to use
    ReleaseMutex(kernel_service_lock);

    // if current task has hand_overed, hibernated etc..
    // wait for scheduler to release hand_over_lock
    while(hand_over_lock == true) {
        // wait until hand_over lock is released
    }

    return error;

}

TASK_RETURN_T svc_handler(svc_handler_param_t *svc_param) {

    uint32_t *arguments;
    uint32_t svc_stash;
    uint32_t svc_current;
    uint8_t error;
    uint32_t i;

    svc_current = svc_param->svc_code;
    arguments = svc_param->arguments;

    do {

        svc_stash = svc_current;

        for(i = 0 ; i < TOTAL_SVC_COUNT; i++) {
            if(svc_dispatch[i].svc_code == svc_current){
                svc_service = svc_dispatch[i].svc_service;
                break;
            }
        }

        // ran through all table and found no svc code match
        // maybe svc_current is 0, so break the loop
        if(i >= TOTAL_SVC_COUNT) {
            break;
        }

        error = (*svc_service)(&svc_current, arguments);

    } while(svc_stash != svc_current);

    return error;

}
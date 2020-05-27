/*
 * software based sercuity service api code
 *
 * author: chetandev.ksd@gmail.com
 *
 */

#include <error.h>
#include <param.h>
#include <svc.h>
#include <kvar.h>
#include <kernel.h>
#include <permissions.h>
#include <os_support.h>


uint32_t base_run(void *args) {

    uint32_t error;

    if(current_task == 0) {
        return ERROR_BASE_PROCESS;
    }

    SVC_INVOKE_BASE((uint32_t)args, error);

    if(error == ERROR_NONE) {
        error = hib_value[current_task];
    }

    return error;

}

uint32_t base_release(uint8_t error) {

    if(current_task != 0) {
        return ERROR_INSUFFICIENT_PERMISSION;
    }

    SVC_RELEASE_BASE(error, error);

    return error;
    
}

uint32_t get_invocation_args(uint32_t *pid, void **args) {

    if(current_task != 0) {
        return ERROR_INSUFFICIENT_PERMISSION;
    }

    if(invocated_task == 0) {
        return ERROR_NO_INVOCATION;
    }

    *pid = invocated_task;
    *args = (void*)invocated_args;

    return ERROR_NONE;

}

uint32_t grant_permission(uint32_t pid, uint16_t permission) {

    uint32_t error = ERROR_NONE;

    if(current_task != 0) {
        return ERROR_INSUFFICIENT_PERMISSION;
    }

    SVC_GRANT_PERMISSION(pid, permission, error);

    return error;

}
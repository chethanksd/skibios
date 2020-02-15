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


uint8_t base_run(void *args) {

    uint8_t error;

    if(current_task == 0) {
        return ERROR_BASE_PROCESS;
    }

    SVC_INVOKE_BASE(args);

    GET_SVC_RETURN_CODE(error);

    if(error == ERROR_NONE) {
        error = hib_value[current_task];
    }

    return error;

}

uint8_t base_release(uint8_t error) {

    if(current_task != 0) {
        return ERROR_ACCESS_DENIED;
    }

    SVC_RELEASE_BASE(error);

    GET_SVC_RETURN_CODE(error);

    return error;
    
}

uint8_t get_invocation_args(uint32_t *pid, void **args) {

    if(current_task != 0) {
        return ERROR_ACCESS_DENIED;
    }

    if(invocated_task == 0) {
        return ERROR_NO_INVOCATION;
    }

    *pid = invocated_task;
    *args = (void*)invocated_args;

    return ERROR_NONE;

}

uint8_t grant_permission(uint32_t pid, uint16_t permission) {

    uint8_t error = ERROR_NONE;

    if(current_task != 0) {
        return ERROR_ACCESS_DENIED;
    }

    SVC_GRANT_PERMISSION(pid, permission);

    GET_SVC_RETURN_CODE(error);

    return error;

}
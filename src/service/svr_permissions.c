/*
 * software defined security kernel service code
 *
 * author: chetandev.ksd@gmail.com
 *
 */

#include <regmap.h>
#include <error.h>
#include <param.h>
#include <kvar.h>
#include <svc.h>
#include <permissions.h>
#include <svr_permissions.h>

extern const uint32_t base_mutex;

uint32_t svc_service_invoke_base(uint32_t *svc_num, uint32_t *arguments) {
    
    uint32_t error = ERROR_NONE;
    uint32_t invocation_arg;

    invocation_arg = arguments[0];

    // base task cannot use this service
    if(current_task == 0) {
        error = ERROR_BASE_PROCESS;
        goto quit_error;
    }

    invocated_task = process_id[current_task];
    invocated_args = invocation_arg;
    arguments[0] = (uint32_t)&base_mutex;

    *svc_num = PRIORITY_PROMOTE;

quit_error:

    return error;

}

uint32_t svc_service_release_base(uint32_t *svc_num, uint32_t *arguments)  {

    uint32_t error = ERROR_NONE;
    uint32_t return_error;
    uint32_t i;

    return_error = arguments[0];

    // only base task can use this service
    if(current_task != 0) {
        error = ERROR_ACCESS_DENIED;
        goto quit_error;
    }

    // store invocation error on hib_value of task that invocated base
    for(i = 0; i < MAX_PROCESS_COUNT; i++) {
        if(process_id[i] == invocated_task) {
            hib_value[i] = return_error;
            break;
        }
    }

    if(i == MAX_PROCESS_COUNT) {
        error = ERROR_UNKNOWN_PROCESS_ID;
        goto quit_error;
    }
    
    // clear invocate info and return error
    invocated_task = 0;
    invocated_args = 0;

    *svc_num = PRIORITY_DEMOTE;

quit_error:

    return error;

}

uint32_t svc_service_grant_permission(uint32_t *svc_num, uint32_t *arguments) {

    uint32_t error = ERROR_NONE;
    uint32_t pid;
    uint32_t permission;
    uint32_t i;

    pid = arguments[0];
    permission = arguments[1];

    if(current_task != 0) {
        error = ERROR_ACCESS_DENIED;
        goto quit_error;
    }

    for(i = 0; i < MAX_PROCESS_COUNT; i++) {
        if(process_id[i] == pid) {
            permissions[i] = permission;
            break;
        }
    }

    if(i == MAX_PROCESS_COUNT) {
        error =  ERROR_UNKNOWN_PROCESS_ID;
    }

quit_error:

    return error;

}

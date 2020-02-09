/*
 * Default Base Task definition source file
 *
 * author : chetandev.ksd@gmail.com
 *
 */


#include <process.h>
#include <kernel.h>
#include <permissions.h>

extern uint32_t invocated_task;

static uint32_t *args;
static uint32_t task_id;
static uint16_t access_code;

void BaseTask() {

    uint32_t error;

    while(1) {

        if(invocated_task != 0) {

            error = get_invocation_args(&task_id, (void**)&args);

            if(error) {
                //ToDo : error handling
            }
            
            access_code = *args;

            if(access_code == 0) {
                device_reset();
            } 

            error = grant_permission(task_id, access_code);

            if(error) {
                //ToDo : error handling
            }

            error = base_release(error);

            if(error) {
                //ToDo : error handling
            }

        }

    }
    
}
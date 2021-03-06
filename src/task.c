/*
 *    process management API Code SKIBIOS RTOS                             
 *                               
 *    author: chetandev.ksd@gmail.com                                                                            
 */

#include <error.h>
#include <kernel.h>
#include <share.h>
#include <task.h>
#include <interrupt.h>
#include <svc.h>
#include <param.h>
#include <access.h>
#include <kvar.h>
#include <defines.h>
#include <os_support.h>


uint32_t task_init(task_t *myprocess) {
    
    uint8_t i;

    for(i = 0; i < MAX_TASK_COUNT; i++) {

        if(task_obj[i] == myprocess) {
            return ERROR_PROCESS_ALREADY_RUNNING;
        }

    }

    myprocess->priority = 1;
    myprocess->task_id = 0;
    myprocess->hibernate = 0;
    myprocess->op1 = 0;
    myprocess->op2 = 0;
    myprocess->value = 0;

    return ERROR_NONE;

}
    
uint32_t task_start(task_t *myprocess) {

    uint8_t i;

    // check if process is already running
    for(i = 0; i < MAX_TASK_COUNT; i++) {

        if(task_obj[i] == myprocess) {
            return ERROR_PROCESS_ALREADY_RUNNING;
        }

    }

    SVC_CREATE_TASK_NO_ARG((uint32_t)myprocess);

    return ERROR_NONE;

}
    
uint32_t task_start_arg(task_t *myprocess, void *arg) {

    uint8_t i;

    // check if process is already running
    for(i = 0; i < MAX_TASK_COUNT; i++) {

        if(task_obj[i] == myprocess) {
            return ERROR_PROCESS_ALREADY_RUNNING;
        }

    }

    SVC_CREATE_TASK_WITH_ARG((uint32_t)myprocess, (uint32_t)arg);

    return ERROR_NONE;

}

uint32_t task_kill(task_t *myprocess) {
    
    uint8_t i;

    // check if process is currently running
    for(i = 0; i < MAX_TASK_COUNT; i++) {

        if(task_obj[i] == myprocess) {
            break;
        }

    }

    if(task_obj[i] != myprocess) {

        return ERROR_PROCESS_IS_IDLE;

    }

    SVC_KILL_TASK_DIRECT(task_id[i]);   

    return ERROR_NONE;

}

uint32_t hibernate(uint32_t *op1, uint32_t *op2, uint32_t value, bool reverse) {

    uint8_t count=0;
    uint8_t i=0;
    uint32_t merged;

    // there is no point in hibernating when there is only one process
    if(task_count==1) {

        if(reverse == false) {
            while((*op1 - *op2) < value);
        } else {
            while((*op1 - *op2) > value);
        }

       return ERROR_NONE;

    }
 
    // there is no point in hibernating when there is one process in a level & dws is disabled
    if(enable_dws == false) {

        for(i=0; i < MAX_TASK_COUNT; i++) {

            if(state[i] != TASK_STATE_IDLE && priority_Array[i][TASK_PRIO_CURRENT] == max_level) {
                count++;
            }
            
        }

        if(count == 1) {

            if(reverse == false) {
                while((*op1 - *op2) < value);
            } else {
                while((*op1 - *op2) > value);
            }

            return ERROR_NONE;

        }
        
    }

    merged = current_task | (reverse == false ? 0 : HIBEARNTE_REV_MASK);

    SVC_HIBERNATE(merged, (uint32_t)op1, (uint32_t)op2, value);

    return ERROR_NONE;
    
}
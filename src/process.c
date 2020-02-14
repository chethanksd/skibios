/*
 *    process management API Code SKIBIOS RTOS                             
 *                               
 *    author: chetandev.ksd@gmail.com                                                                            
 */

#include <error.h>
#include <kernel.h>
#include <share.h>
#include <process.h>
#include <interrupt.h>
#include <svc.h>
#include <param.h>
#include <access.h>
#include <kvar.h>
#include <defines.h>
#include <os_support.h>


uint8_t process_init(process_t *myprocess) {
    
    uint8_t i;

    for(i = 0; i < MAX_PROCESS_COUNT; i++) {

        if(proc_obj[i] == myprocess) {
            return ERROR_PROCESS_ALREADY_RUNNING;
        }

    }

    myprocess->priority = 1;
    myprocess->process_id = 0;
    myprocess->hibernate = 0;
    myprocess->op1 = 0;
    myprocess->op2 = 0;
    myprocess->value = 0;

    return ERROR_NONE;

}
    
uint8_t process_start(process_t *myprocess) {

    uint8_t i;

    // check if process is already running
    for(i = 0; i < MAX_PROCESS_COUNT; i++) {

        if(proc_obj[i] == myprocess) {
            return ERROR_PROCESS_ALREADY_RUNNING;
        }

    }

    SVC_CREATE_PROCESS_NO_ARG(myprocess);

    return ERROR_NONE;

}
    
uint8_t process_start_arg(process_t *myprocess, void *arg) {

    uint8_t i;

    // check if process is already running
    for(i = 0; i < MAX_PROCESS_COUNT; i++) {

        if(proc_obj[i] == myprocess) {
            return ERROR_PROCESS_ALREADY_RUNNING;
        }

    }

    SVC_CREATE_PROCESS_WITH_ARG(myprocess, arg);

    return ERROR_NONE;

}

uint8_t process_kill(process_t *myprocess) {
    
    uint8_t i;

    // check if process is currently running
    for(i = 0; i < MAX_PROCESS_COUNT; i++) {

        if(proc_obj[i] == myprocess) {
            break;
        }

    }

    if(proc_obj[i] != myprocess) {

        return ERROR_PROCESS_IS_IDLE;

    }

    SVC_KILL_PROCESS_DIRECT(process_id[i]);   

    return ERROR_NONE;

}

uint8_t hibernate(uint32_t *op1, uint32_t *op2, uint32_t value, bool reverse) {

    uint8_t count=0;
    uint8_t i=0;
    uint32_t merged;

    // there is no point in hibernating when there is only one process
    if(process_count==1) {

        if(reverse == false) {
            while((*op1 - *op2) < value);
        } else {
            while((*op1 - *op2) > value);
        }

       return ERROR_NONE;

    }
 
    // there is no point in hibernating when there is one process in a level & dws is disabled
    if(enable_dws == false) {

        for(i=0; i < MAX_PROCESS_COUNT; i++) {

            if(state[i] != PROCESS_STATE_IDLE && priority_Array[i][PROCESS_PRIO_CURRENT] == max_level) {
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

    SVC_HIBERNATE(merged, op1, op2, value);

    return ERROR_NONE;
    
}
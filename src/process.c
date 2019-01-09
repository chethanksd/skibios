/*
 *    process management API Code SKIBIOS RTOS                             
 *                               
 *    Author: chetandev.ksd@gmail.com                                                                            
 */

#include <error.h>
#include <kernel.h>
#include <share.h>
#include <process.h>
#include <interrupt.h>
#include <svc.h>
#include <param.h>
#include <access.h>

#define PROCESS_PRIO_CURRENT    0
#define PROCESS_PRIO_STASHED    1

extern bool enable_dws;
extern uint8_t max_level;
extern uint32_t current_task;
extern uint16_t process_count;
extern Process *proc_obj[MAX_PROCESS_COUNT];
extern uint8_t state[MAX_PROCESS_COUNT];
extern uint32_t process_id[MAX_PROCESS_COUNT];
extern uint8_t  priority_Array[MAX_PROCESS_COUNT][2];

void resolve_end(void);


uint8_t process_init(Process *myprocess) {
    
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
    
uint8_t process_start(Process *myprocess) {

    uint8_t i;

    // check if process is already running
    for(i = 0; i < MAX_PROCESS_COUNT; i++) {

        if(proc_obj[i] == myprocess) {
            return ERROR_PROCESS_ALREADY_RUNNING;
        }

    }

    __asm volatile (" MOV R0, %[proc]   \n"
                    " MOV R1, #0        \n"
                    :
                    : [proc] "r" (myprocess)
                    :
            );

    svc(CREATE_PROCESS);

    return ERROR_NONE;

}
    
uint8_t process_start_arg(Process *myprocess, void *arg) {

    uint8_t i;

    // check if process is already running
    for(i = 0; i < MAX_PROCESS_COUNT; i++) {

        if(proc_obj[i] == myprocess) {
            return ERROR_PROCESS_ALREADY_RUNNING;
        }

    }

    __asm volatile (" MOV R0, %[proc]   \n"
                    " MOV R1, %[arg]    \n"
                :
                : [proc] "r" (myprocess), [arg] "r" (arg)
                :
        );
    
    svc(CREATE_PROCESS);

    return ERROR_NONE;

}

uint8_t process_kill(Process *myprocess) {
    
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

    __asm volatile (" MOV R0, %[proc_id]   \n"
                    " MOV R1, #0           \n"
            :
            : [proc_id] "r" (process_id[i])
            :
    );

    svc(KILL_PROCESS);   

    return ERROR_NONE;

}

/* Process End Resolver */

void resolve_end(void) {

    __asm volatile (" MOV R0, %[proc_id]   \n"
                    " MOV R1, #1           \n"
            :
            : [proc_id] "r" (process_id[current_task])
            :
    );

    svc(KILL_PROCESS);

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

    __asm volatile (" LDR R0, %[mrg]        \n"
                    " LDR R1, %[op1]        \n"
                    " LDR R2, %[op2]        \n"
                    " LDR R3, %[val]        \n"
            :
            : [mrg] "m" (merged), [op1] "m" (op1), [op2] "m" (op2), [val] "m" (value)
            :
    );

    svc(HIBERNATE);

    return ERROR_NONE;
}
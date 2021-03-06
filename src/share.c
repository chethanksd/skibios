/*
 *   resource management api definition source file                                                                                                         
 *                                  
 *   author : chetandev.ksd@gmail.com
 *                                                   
 */

#include <share.h>
#include <param.h>
#include <kernel.h>
#include <task.h>
#include <error.h>
#include <svc.h>
#include <kvar.h>
#include <os_support.h>

extern uint32_t arch_semaphore_inc(uint32_t *semaphore);
extern uint32_t arch_semaphore_dec(uint32_t *semaphore);

uint32_t spin_lock(uint32_t *mutex_st, uint16_t retry){

    uint8_t state;
    uint16_t loop=0;
    uint8_t index;

    while(loop < retry) {

        state=mutex_lock(mutex_st);

        // check if locking is successful
        if(state==0) {
            break;
        }

        index = (*mutex_st >> 16);

        #if(ENABLE_SAFE_LOCK >= 1)
        // the process locked the resource is idle. then it probably forget to unlock
        if(state[index] == TASK_STATE_IDLE) {

            if(*mutex_st != 0){

                mutex_unlock(mutex_st);
                continue;

            }

        } else {

            if(task_id[index] != *mutex_st) {

                mutex_unlock(mutex_st);
                continue;

            }
            
        }
        #endif

        // the process locked the resource has lower priority. then promote its priority
        if(priority_Array[index][0] < max_level && priority_Array[current_task][0] == max_level) {
            
            if(priority_Array[index][0] > 127) {
                continue;
            }

            SVC_PRIORITY_PROMOTE((uint32_t)mutex_st);
        
        }

        svc(HAND_OVER);
        loop++;

    }

    if(loop == retry){
        return ERROR_RESOURCE_LOCKED_INDEFINETLY;
    }

    return ERROR_NONE;

}

uint32_t semaphore_increment(uint32_t *semaphore, uint16_t retry) {

    uint32_t state;
    uint16_t loop=0;

    while(loop < retry){

        state = arch_semaphore_inc(semaphore);

        // check if increment is successful
        if(state == 0){
            break;
        }

        svc(HAND_OVER);
        loop++;

    }

    if(loop == retry){
        return ERROR_RESOURCE_LOCKED_INDEFINETLY;
    }

    return ERROR_NONE;
}

uint32_t semaphore_decrement(uint32_t *semaphore, uint16_t retry) {

    uint8_t state;
    uint16_t loop=0;

    while(loop < retry) {

        state = arch_semaphore_dec(semaphore);

        // check if decrement is successful
        if(state == 0){
            break;
        }
        svc(HAND_OVER);
        loop++;
    }

    if(loop == retry){
        return ERROR_RESOURCE_LOCKED_INDEFINETLY;
    }

    return ERROR_NONE;

}


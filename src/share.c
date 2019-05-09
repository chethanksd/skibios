/*
 *   resource management api definition source file                                                                                                         
 *                                  
 *   author : chetandev.ksd@gmail.com
 *                                                   
 */

#include <share.h>
#include <param.h>
#include <kernel.h>
#include <process.h>
#include <error.h>
#include <svc.h>

extern Process *proc_obj[MAX_PROCESS_COUNT];
extern uint8_t priority_Array[MAX_PROCESS_COUNT][2];
extern uint32_t mutex_stash[MAX_PROCESS_COUNT];
extern uint32_t process_id[MAX_PROCESS_COUNT];
extern uint8_t  state[MAX_PROCESS_COUNT];
extern uint32_t current_task;
extern uint8_t  max_level;

#if 0
uint8_t mutex_lock(uint32_t *mutex_st) {

    uint8_t state = 0;
    
    __asm volatile (
        "LDR R1,=0x00                               \n"
        "LDREXB R2,[%[mutex_st]]                    \n"
        "CMP R2,R1                                  \n"     // check if the mutex is locked?
        "ITT NE                                     \n"
        "LDRBNE %[state],=0x01                      \n"
        "BNE brch%=                                 \n"
        "LDR R1, =%[ct_proc]                        \n"     // get current_task
        "LDR R1, [R1]                               \n"
        "LDR R2, =%[proc_id]                        \n"     // get process_id of current_task
        "LDR R1, [R2, R1, LSL#2]                    \n"
        "STREX R2,R1,[%[mutex_st]]                  \n"     // attempt to lock the mutex     
        "CMP  R2,#1                                 \n"
        "ITE EQ                                     \n"
        "LDRBEQ %[state],=0x01                      \n"
        "LDRBNE %[state],=0x00                      \n"
        "brch%=: DMB                                \n"
        :[state] "=r"(state)
        :[mutex_st] "r" (mutex_st), [proc_id] "i" (&process_id[0]), [ct_proc] "i" (&current_task)
		: "r1", "r2", "r3"
    );

    return state;
}


void mutex_unlock(uint32_t *mutex_st) {

    __asm volatile(
        "LDR R1,=0x00                       \n"
        "DMB                                \n"
        "STRB R1,[%[mutex_st]]              \n"
        :
        :[mutex_st] "r" (mutex_st)
        : "r1"
    );

    if((uint32_t*)mutex_stash[current_task] == mutex_st) {

        svc(PRIORITY_DEMOTE);

    }
}

#endif

uint8_t spin_lock(uint32_t *mutex_st, uint16_t retry){

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
        if(state[index] == PROCESS_STATE_IDLE) {

            if(*mutex_st != 0){

                mutex_unlock(mutex_st);
                continue;

            }

        } else {

            if(process_id[index] != *mutex_st) {

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

            __asm volatile (" MOV R0, %[mutex]   \n"
            :
            : [mutex] "r" (mutex_st)
            :
            );

            svc(PRIORITY_PROMOTE);
        
        }

        svc(HAND_OVER);
        loop++;

    }

    if(loop == retry){
        return ERROR_RESOURCE_LOCKED_INDEFINETLY;
    }

    return ERROR_NONE;

}

uint8_t semaphore_increment(uint8_t *semaphore, uint16_t retry) {

    uint8_t state;
    uint16_t loop=0;

    while(loop < retry){

        __asm volatile(
            "           LDR    		R0, %1      	\n"
            "			LDREXB  	R1, [R0]    	\n"
            "           ADD			R1, #1			\n"
            "			STREXB		R2, R1, [R0]    \n"
            "           CMP    		R2, #0       	\n"
            "			ITE			EQ				\n"
            "           LDRBEQ 		%0, =0x00    	\n"
            "           LDRBNE 		%0, =0x01    	\n"
            " 			DMB      					\n"
            :"=r"(state)
            :"m" (semaphore)

            );

        // check if locking is successful
        if(state==0){
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

uint8_t semaphore_decrement(uint8_t *semaphore, uint16_t retry) {

    uint8_t state;
    uint16_t loop=0;

    while(loop < retry){

        __asm volatile(
            "           LDR    		R0, %1      	\n"
            "			LDREXB  	R1, [R0]    	\n"
            "           CMP    		R1, #0      	\n"
            "			ITEET		EQ				\n"
            "           LDRBEQ 		%0, =0x00   	\n"
            "           SUBNE		R1, #1			\n"
            "			STREXBNE	R2, R1, [R0]    \n"
            "           BEQ    		exit%=        	\n"
            "           CMP    		R2, #0       	\n"
            "			ITE			EQ				\n"
            "           LDRBEQ 		%0, =0x00    	\n"
            "           LDRBNE 		%0, =0x01    	\n"
            " exit%=:	DMB       					\n"
            :"=r"(state)
            :"m" (semaphore)

            );

        // check if locking is successful
        if(state==0){
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


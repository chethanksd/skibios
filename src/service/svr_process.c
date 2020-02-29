/*
 * process managenment service code
 *
 * author: chetandev.ksd@gmail.com
 *
 */

#include <svr_process.h>
#include <os_support.h>
#include <kvar.h>
#include <svc.h>
#include <error.h>
#include <param.h>
#include <kernel.h>
#include <process.h>
#include <access.h>
#include <regmap.h>
#include <defines.h>

uint32_t svc_service_create_process(uint32_t *svc_num, uint32_t *arguments) {

    uint32_t error = ERROR_NONE;
    uint32_t proc_obj_ptr;
    uint32_t proc_arg;
    uint32_t i;
    uint32_t j;
    uint8_t search_index;
    uint8_t lower_index;
    uint32_t *pheap_ptr;

    proc_obj_ptr = arguments[0];
    proc_arg = arguments[1];

    /* Find Idle Process Objects */
    if(process_count < MAX_PROCESS_COUNT) {

        for(i=0;i<MAX_PROCESS_COUNT;i++) {

            if(state[i] == PROCESS_STATE_IDLE) {
                break;
            }

        }

        /* Check if priority level request is valid */
        if(((process_t*)proc_obj_ptr)->priority > 127) {

            ((process_t*)proc_obj_ptr)->error = ERROR_INVALID_PRIORITY;
            error = ERROR_INVALID_PRIORITY;
            goto quit_error;
            
        }

        /* Check if Base process creation is attempted */
        if(((process_t*)proc_obj_ptr)->priority == 0 && first_start == true) {

            ((process_t*)proc_obj_ptr)->error = ERROR_INVALID_PRIORITY;
            error = ERROR_INVALID_PRIORITY;
            goto quit_error;

        }


        /* Dynamically initialize the Process Stack & Check if it is successful */
        if(!(PSP_Array[i] = pstack_addr + (i * PROCESS_STACK_SIZE * 4))) {

            ((process_t*)proc_obj_ptr)->error=ERROR_OUT_OF_MEMORY;
            error = ERROR_OUT_OF_MEMORY;
            goto quit_error;

        }


        /* Clear all locations of process stack */
        pheap_ptr = (uint32_t*)PSP_Array[i];
        for( j = 0; j < (PROCESS_STACK_SIZE/4); j++) {
            pheap_ptr[j] = 0;
        }

        /* calculation for jump list */
        if(i == 0) {

            jmp_list[i] = 0;

        } else {

            j = i - 1;
            jmp_list[i] = jmp_list[j] - (i - j);
            jmp_list[j] = i - j;
            
        }

        proc_obj[i] = (process_t*) proc_obj_ptr;
        proc_obj[i]->process_id = (i << 16) | (total_process_count + 1);
        process_id[i] = (i << 16) | (total_process_count + 1);

        /* Set Initial Process State */
        if((proc_obj[i]->hibernate & HIBERNATE_STATE_MASK) != HIBERNATE_STATE_MASK){
            state[i] = PROCESS_STATE_SLEEP;
        }

        /* Set current process priority */
        priority_Array[i][PROCESS_PRIO_CURRENT] = proc_obj[i]->priority;

        //use j as higher pointer of binary search
        j = lstash_ptr;
        lower_index = 0;

        while(lower_index <= j) {

            search_index = lower_index + ((j - lower_index)/2);

            if(level_stash[search_index] == priority_Array[i][PROCESS_PRIO_CURRENT]) {
                goto proc_continue;
            }

            if(level_stash[search_index] < priority_Array[i][PROCESS_PRIO_CURRENT]) {
                lower_index = search_index + 1;
            } else {
                j = search_index - 1;
            }

        }
        
        for(j = lstash_ptr; j > search_index; j--) {
            level_stash[j] = level_stash[j-1];
        }

        level_stash[search_index] = (uint32_t)priority_Array[i][PROCESS_PRIO_CURRENT];
        lstash_ptr++;
        
        proc_continue:

        /* Stack Initialization */
        arch_task_stack_init(i, (unsigned int)proc_obj[i]->ptr_func, proc_arg);


        total_process_count++;
        process_count++;

        /* Find new max level (Priority Level) */
        for(j=0; j < MAX_PROCESS_COUNT; j++) {

            if(priority_Array[j][PROCESS_PRIO_CURRENT] > max_level && state[j] != PROCESS_STATE_IDLE) {

                max_level = priority_Array[j][PROCESS_PRIO_CURRENT];
                alc=0;
                hlc=0;
                ENABLE_SCHEDULER();

            }

        }

        /* Update Active & Hibernation Process Level Counts */
        if(priority_Array[i][PROCESS_PRIO_CURRENT] == max_level) {

            if((state[i] & HIBERNATE_STATE_MASK) == HIBERNATE_STATE_MASK) {
                hlc++;
            } else {
                alc++;
            }

        }

        /* Start scheduler if there was only one process in  & start scheduler has been already executed */
        if((alc + hlc) == 2 && first_start == true) {
            ENABLE_SCHEDULER();
        }

    } else {

        ((process_t*)proc_obj_ptr)->error=ERROR_MAX_PROCESS_COUNT;
        error = ERROR_MAX_PROCESS_COUNT;

    }

quit_error:

    return error;
}

uint32_t svc_service_process_kill(uint32_t *svc_num, uint32_t *arguments) {
    
    uint32_t error = ERROR_NONE;
    uint32_t pid;
    uint32_t self_kill_flag;
    uint8_t i;
    uint8_t j;
    uint8_t found;

    pid = arguments[0];
    self_kill_flag = arguments[1];

    /* calculate index of the process to kill */
    i = (pid >> 16);

    /* if i == 0 ==> killing of BaseTask is attempted
        * MCU should fault in this case
        */

    /* Re-calculate level_stash */
    for(j = 0; j < MAX_PROCESS_COUNT; j++) {

        if(i == j) {
            continue;
        }

        if(priority_Array[j][PROCESS_PRIO_CURRENT] == priority_Array[i][PROCESS_PRIO_CURRENT]) {
            goto pkill_continue;
        }

    }

    found = 0;

    for(j = 0; j < lstash_ptr; j++) {

        if(level_stash[j] == priority_Array[i][PROCESS_PRIO_CURRENT]) {
            found = 1;
        }

        if(found == 1) {
            level_stash[j] = level_stash[j + 1];
            continue;
        }

    }

    lstash_ptr--;

pkill_continue:

    state[i] = PROCESS_STATE_IDLE;
    proc_obj[i]->process_id = 0;
    process_id[i] = 0;
    proc_obj[i] = 0;

    if(self_kill_flag == 1) {

        self_kill = true;
        *svc_num = HAND_OVER;

    }

    process_count--;

    /* calculate jmp_list */
    for(j = 0; j < i; j++) {

        if(i == (j + jmp_list[j])) {
            break;
        }

    }

    jmp_list[j] = jmp_list[j] + jmp_list[i];

    if(self_kill == true) {

        next_task = next_task + jmp_list[i];

    }

    jmp_list[i] = 0;
    
    /* Find new max level (Priority Level), alc & hlc */
    max_level = 0;
    alc = 0;
    hlc = 0;

    for(i=0; i < MAX_PROCESS_COUNT; i++) {

        if(priority_Array[i][PROCESS_PRIO_CURRENT] >= max_level && state[i] != PROCESS_STATE_IDLE) {
            max_level = priority_Array[i][PROCESS_PRIO_CURRENT];
        }

    }

    for(i=0; i < MAX_PROCESS_COUNT; i++) {

        if(priority_Array[i][PROCESS_PRIO_CURRENT] == max_level) {

            if((state[i] & HIBERNATE_STATE_MASK) == HIBERNATE_STATE_MASK) {
                hlc++;
            } else {
                alc++;
            }

        }

    }  

    /* Turn ON scheduler */
    ENABLE_SCHEDULER();

    return error;

}


uint32_t svc_service_hibernate(uint32_t *svc_num, uint32_t *arguments) {

    uint32_t process_no;
    uint32_t hib_op1;
    uint32_t hib_op2;
    uint32_t value;

    process_no = arguments[0];
    hib_op1 = arguments[1];
    hib_op2 = arguments[2];
    value = arguments[3];

    if((process_no & HIBEARNTE_REV_MASK) == HIBEARNTE_REV_MASK) {

        process_no = process_no & ~(HIBEARNTE_REV_MASK);
        state[current_task] = PROCESS_STATE_HIBERNATE_L;

    } else {

        state[current_task] = PROCESS_STATE_HIBERNATE_G;

    }
    
    op1[current_task] = (uint32_t*)hib_op1;
    op2[current_task] = (uint32_t*)hib_op2;
    hib_value[current_task] = value;


    if(priority_Array[current_task][PROCESS_PRIO_CURRENT] == max_level) {

        hlc++;
        alc--;

    }

    normal_schedule = false;
    TRIGGER_SCHEDULER();

    return ERROR_NONE;

}

uint32_t svc_service_priority_promote(uint32_t *svc_num, uint32_t *arguments) {

    uint32_t mutex_address;
    uint32_t index;
    
    mutex_address = arguments[0];

    if(maxp_level == 127) {

        maxp_level = max_level;
        max_level = 127;
        alcp = alc;
        hlcp = hlc;
        alc = 1;
        hlc = 0;

    }

    if(max_level == 255) {
        // ToDo : no priority level present after 255
        // Return error
    }
    
    max_level++;
    index = *((uint32_t*)mutex_address) >> 16;

    mutex_stash[index] = mutex_address;

    priority_Array[index][PROCESS_PRIO_STASHED] = priority_Array[index][PROCESS_PRIO_CURRENT];
    priority_Array[index][PROCESS_PRIO_CURRENT] = max_level;

    state[current_task] = PROCESS_STATE_HOLD;
    op1[current_task] = (uint32_t*)mutex_address;

    // Enable scheduler and set normal schedule as false
    normal_schedule = false;
    TRIGGER_SCHEDULER();

    return ERROR_NONE;

}

uint32_t svc_service_priority_demote(uint32_t *svc_num, uint32_t *arguments) {

    uint32_t i;

    priority_Array[current_task][PROCESS_PRIO_CURRENT] = priority_Array[current_task][PROCESS_PRIO_STASHED];
    priority_Array[current_task][PROCESS_PRIO_STASHED] = 0;

    max_level--;

    if(max_level == 127) {

        max_level = maxp_level;
        maxp_level = 127;
        alc = alcp;
        hlc = hlcp;

    }

    for(i = 0; i < MAX_PROCESS_COUNT; i++) {

        if(state[i] == PROCESS_STATE_HOLD) {

            if(op1[i] == (uint32_t*)mutex_stash[current_task]){

                state[i] = PROCESS_STATE_SLEEP;
                break;

            }
        }

    }

    mutex_stash[current_task] = 0;

    /* invoke HAND_OVER service call */
    *svc_num = HAND_OVER;

    return ERROR_NONE;

}


/* Process End Resolver */

void resolve_end(void) {

    SVC_KILL_PROCESS_RESOLVE_END(process_id[current_task]);

}
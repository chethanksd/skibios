/*
 * os util code for arm-m3m4
 *
 *
 * author: chetandev.ksd@gmail.com
 *
 */


#include <os_util.h>
#include <regmap.h>
#include <access.h>
#include <error.h>
#include <param.h>
#include <kvar.h>
#include <interrupt.h>
#include <os_support.h>

// external functions
extern void context_switch_handler();
extern void resolve_end(void);

// external variables
extern uint32_t __VECTOR_RAM[];

extern uint32_t __kdat_load__;
extern uint32_t __kdat_start__;
extern uint32_t __kdat_end__;
extern uint32_t __kbss_start__;
extern uint32_t __kbss_end__;


uint8_t arch_kernel_init() {

    uint32_t *source, *destination;

    // kdat initialization
    source = &__kdat_load__;
    for(destination = &__kdat_start__; destination < &__kdat_end__; ) {
        *destination++ = *source++;
    }

    // kbss initialization
    for(destination = &__kbss_start__; destination < &__kbss_end__; ) {
        *destination++ = 0;
    }


    // source context switch sw interrupt interrupt to core0
    INTC->PSR[CONTEXT_SWITCH_SW_INT].PRC_SELN = (8 >> 0);

    // register context switch sw interrupt handler
    __VECTOR_RAM[CONTEXT_SWITCH_SW_INT] = (uint32_t)(context_switch_handler);

    // enable STM interrupt in INTC
    // setup lowest priority to context switch sw interrupt
    INTC->PSR[CONTEXT_SWITCH_SW_INT].PRIN = 1;

    return ERROR_NONE;

}

uint8_t arch_mpu_init() {

    return ERROR_NONE;

}

uint8_t arch_task_stack_init(uint32_t task_index, uint32_t ptr_func, uint32_t proc_arg) {

    uint32_t *pheap_ptr;

    PSP_Array[task_index] = ((unsigned int) (PSP_Array[task_index])) + ((PROCESS_STACK_SIZE - 1) * 4) - 20 * 4;
    pheap_ptr = (uint32_t*)PSP_Array[task_index];

    pheap_ptr[3] = ptr_func;
    pheap_ptr[4] = 0x9000;
    pheap_ptr[6] = (uint32_t) &resolve_end;
    pheap_ptr[10] = proc_arg;

    return ERROR_NONE;

}

void heap_init() {
    
}
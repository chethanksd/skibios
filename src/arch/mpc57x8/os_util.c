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
#include <interrupt.h>
#include <os_support.h>

// external functions
extern void context_switch_handler();

// external variables
extern uint32_t __VECTOR_RAM[];

uint8_t arch_kernel_init() {

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

void heap_init() {
    
}
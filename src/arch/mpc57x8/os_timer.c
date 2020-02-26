/*
 * os timer source code for MPC5
 *
 * author: chetandev.ksd@gmail.com
 *
 * note:
 *    1) i/p clock src of STM timer is divided by 10 (for no good reason)
 *    2) STM timer is freezed when debugger is active
 */

#include <os_timer.h>
#include <regmap.h>
#include <error.h>
#include <param.h>

void os_timer_int_handler(void);

extern uint32_t __VECTOR_RAM[];

uint32_t os_timer_init(uint32_t new_cpu_freq) {

    uint32_t error = ERROR_NONE;

    error = os_timer_config(new_cpu_freq);

    if(error) {
        goto quit_error;
    }

    // source interrupt to core0
    INTC->PSR[STM_0_CHANNEL_0_INT].PRC_SELN = (8 >> 0);

    // register interrupt handler
    __VECTOR_RAM[STM_0_CHANNEL_0_INT] = (uint32_t)(os_timer_int_handler);

    // enable STM interrupt in INTC
    INTC->PSR[STM_0_CHANNEL_0_INT].PRIN = INTERRUPT_PRIORITY_DEFAULT;

quit_error:

    return error;

}

uint32_t os_timer_config(uint32_t new_cpu_freq) {

    uint8_t error = ERROR_NONE;
    uint64_t os_timer_load;

    // divide i/p of STM timer by 10 which will 
    // be pre-scaled clock source for STM
    // STM timer will freeze its timer when debugger is active

    os_timer_load = new_cpu_freq/(PROCESS_PER_SEC * 10);

    if(os_timer_load > 0xFFFFFFFFUL) {
        error =  ERROR_SYSTICK_TICK_VALUE_OVERFLOW;
        goto quit_error;
    }

    //STM->CR.CPS = 9
    //STM->CNT.FRZ = 1
    //STM->CR.TEN = 1
    *((uint32_t*) &STM->CR) = 0x903;

    // use channel_0 of STM timer for compare
    // which upon when STM timer reaches tick value of what is
    // being loaded on compare register will generate interrupt

    // enable channel_0 STM compare interrupt  
    STM->CHANNEL[0].CIR.CIF = 1;
    STM->CHANNEL[0].CCR.CEN = 1;
    STM->CHANNEL[0].CMP.CMP = os_timer_load;

    // clear content STM CNT Register
    *((uint32_t*) &STM->CNT) = 0;


quit_error:

    return error;

}

uint32_t int_counter = 0;

void os_timer_int_handler(void) {

    int_counter++;

}

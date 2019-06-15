/*
 * os timer source code for m3/m4
 *
 * author: chetandev.ksd@gmail.com
 *
 */

#include <os_timer.h>
#include <access.h>
#include <error.h>
#include <regmap.h>
#include <kvar.h>

extern void scheduler(void);

uint32_t os_timer_init(uint32_t new_cpu_freq) {

    uint32_t error = ERROR_NONE;

    error = os_timer_config(new_cpu_freq);

    if(error) {
        goto quit_error;
    }

    /* Set System Clock source for Systick timer */
    HWREG(STCTRL) |= SYSTICK_SYS_CLK;

    /* Register Systick Interrupt Handler */
    HWREG(SRAM_START_ADDRESS + (INT_NUM_SYSTICK * 4)) = (uint32_t) scheduler;

quit_error:

    return error; 

}

uint32_t os_timer_config(uint32_t new_cpu_freq) {

    uint32_t error = ERROR_NONE;
    uint32_t systick_load;

    // clear & configure reload register of systick timer
    systick_load = new_cpu_freq/PROCESS_PER_SEC;

    if(systick_load >= 16777216) {

        error = ERROR_SYSTICK_TICK_VALUE_OVERFLOW;
        goto quit_error;

    }

    cpu_freq = new_cpu_freq;

    HWREG(STRELOAD) = 0x00000000;
    HWREG(STRELOAD) = SYSTICK_RELOAD_MASK & systick_load;

    // clear current register of systick timer
    HWREG(STCURRENT) = 0x00000000;

quit_error:

    return error;
}
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

uint32_t os_timer_init(uint32_t new_cpu_freq) {

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
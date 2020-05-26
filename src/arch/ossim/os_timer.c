/*
 * os timer source code for ossim
 *
 * author: chetandev.ksd@gmail.com
 *
 */

#include <os_timer.h>
#include <access.h>
#include <error.h>
#include <kvar.h>
#include <param.h>



uint32_t os_timer_init(uint32_t new_cpu_freq) {

    uint32_t error = ERROR_NONE;

    error = os_timer_config(new_cpu_freq);

    if(error) {
        goto quit_error;
    }

quit_error:

    return error; 

}

uint32_t os_timer_config(uint32_t new_cpu_freq) {

    return ERROR_NONE;
    
}
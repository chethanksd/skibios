/*
 * interrupt functionality support for ARM M3/M4
 *
 * author: chetandev.ksd@gmail.com
 *
 */

#include <arch_interrupt.h>
#include <regmap.h>
#include <error.h>
#include <access.h>



uint8_t arch_interrupt_enable(uint32_t interrupt) {

    return ERROR_NONE;

}


uint8_t arch_interrupt_disable(uint32_t interrupt) {

    return ERROR_NONE;

}


uint8_t arch_interrupt_register(uint32_t interrupt, uint32_t handler) {

    return ERROR_NONE;

}

uint8_t arch_interrupt_priority(uint8_t interrupt, uint8_t priority) {

    return ERROR_NONE;
    
}
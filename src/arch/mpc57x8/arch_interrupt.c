/*
 * interrupt functionality support for ARM M3/M4
 *
 * author: chetandev.ksd@gmail.com
 *
 */

#include <arch_interrupt.h>
#include <os_support.h>
#include <regmap.h>
#include <error.h>
#include <access.h>
#include <param.h>


// brief Registers in which the start of interrupt vector table needs to be configured
#define FEATURE_INTERRUPT_INT_VECTORS {(uint32_t *)&INTC->IACKR[0], (uint32_t *) &INTC->IACKR[1], (uint32_t *)&INTC->IACKR[3]}

/* Addresses for VECTOR_TABLE and VECTOR_RAM come from the linker file */
extern uint32_t __VECTOR_TABLE[];
extern uint32_t __VECTOR_RAM[];


uint8_t arch_interrupt_enable(uint32_t interrupt) {

    // ToDo: argument check
    
    INTC->PSR[interrupt].PRIN = INTERRUPT_PRIORITY_DEFAULT;

    return ERROR_NONE;

}


uint8_t arch_interrupt_disable(uint32_t interrupt) {

    // ToDo: argument check
    
    INTC->PSR[interrupt].PRIN = 0;
    
    return ERROR_NONE;

}


uint8_t arch_interrupt_register(uint32_t interrupt, uint32_t handler) {

    uint8_t core_id;

    // ToDo: argument check

    core_id = (uint8_t)GET_CORE_ID();

    // direct interrupt to active core
    // register interrupt in SRAM vtable
    INTC->PSR[interrupt].PRC_SELN = (8 >> core_id);
    __VECTOR_RAM[interrupt] = (uint32_t)handler;

    return ERROR_NONE;

}

uint8_t arch_interrupt_priority(uint8_t interrupt, uint8_t priority) {

    return ERROR_NONE;
    
}


uint8_t vector_table_relocate() {

    uint8_t core_id;
    uint32_t n;

    volatile uint32_t *vectors[NUMBER_OF_CORES] = FEATURE_INTERRUPT_INT_VECTORS;

    core_id = (uint8_t)GET_CORE_ID();

    // copy the vector table from ROM to RAM
    for (n = 0; n < (NUM_OF_INTERRUPTS); n++)
    {
        __VECTOR_RAM[n] = __VECTOR_TABLE[n];
    }

    // point the VTOR to the position of vector table
    *vectors[core_id] = (uint32_t)__VECTOR_RAM;

    return ERROR_NONE;

}
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
#include <param.h>


// this is a mapping between priority grouping encodings and the number of preemption priority bits
const uint32_t g_pui32Priority[] = {
PRIGROUP_8_1,
PRIGROUP_4_2,
PRIGROUP_2_4,
PRIGROUP_1_8
};


// this is a mapping between interrupt number and the register that contains the priority encoding 
// for that interrupt
const uint32_t prioreg[] = {
 NVIC_PRI0,  NVIC_PRI1,  NVIC_PRI2,  NVIC_PRI3,  NVIC_PRI4,  NVIC_PRI5,  
 NVIC_PRI6,  NVIC_PRI7,  NVIC_PRI8,  NVIC_PRI9,  NVIC_PRI10, NVIC_PRI11, 
 NVIC_PRI12, NVIC_PRI13, NVIC_PRI14, NVIC_PRI15, NVIC_PRI16, NVIC_PRI17, 
 NVIC_PRI18, NVIC_PRI19, NVIC_PRI20, NVIC_PRI21, NVIC_PRI22, NVIC_PRI23, 
 NVIC_PRI24, NVIC_PRI25, NVIC_PRI26, NVIC_PRI27, NVIC_PRI28, NVIC_PRI29, 
 NVIC_PRI30, NVIC_PRI31, NVIC_PRI32, NVIC_PRI33, NVIC_PRI34
};


// this is a mapping between interrupt number (for the peripheral interrupts only)
// and the register that contains the interrupt disable for that interrupt
const uint32_t g_pui32Dii16Regs[] = {
    NVIC_DIS0, NVIC_DIS1, NVIC_DIS2, NVIC_DIS3
};

// this is a mapping between interrupt number (for the peripheral interrupts only) and 
// the register that contains the interrupt enable for that interrupt
const uint32_t g_pui32EnRegs[] = {
    NVIC_EN0, NVIC_EN1, NVIC_EN2, NVIC_EN3
};

// extern variables
extern uint32_t __VECTOR_RAM[];


uint8_t arch_interrupt_enable(uint32_t interrupt) {

    // valid interrupt number?
    if(interrupt < 16) {
        return ERROR_ACCESS_DENIED;
    }

    // enable specified interrupt
    HWREG(g_pui32EnRegs[(interrupt - 16) / 32]) = 1 << ((interrupt - 16) & 31);

    return ERROR_NONE;

}


uint8_t arch_interrupt_disable(uint32_t interrupt) {

    // valid interrupt number?
    if(interrupt < 16) {
        return ERROR_ACCESS_DENIED;
    }

    // disable specified interrupt
    HWREG(g_pui32Dii16Regs[(interrupt - 16) / 32]) = 1 << ((interrupt - 16) & 31);

    return ERROR_NONE;

}


uint8_t arch_interrupt_register(uint32_t interrupt, uint32_t handler) {

    // valid interrupt number?
    if(interrupt < 16) {
        return ERROR_ACCESS_DENIED;
    }

    // save the interrupt handler
    HWREG(KERNEL_START_ADDRESS + (interrupt * 4)) = handler;

    return ERROR_NONE;

}

uint8_t arch_interrupt_priority(uint8_t interrupt, uint8_t priority) {

    // valid interrupt number?
    if(interrupt < 16) {
        return ERROR_ACCESS_DENIED;
    }

    HWREG(prioreg[(interrupt - 16)/4]) |= (PRIORITY_WRITE_MASK & (uint32_t)priority) << (8 * ((interrupt - 16) & 3));

    return ERROR_NONE;
    
}


void vector_table_relocate(void){

    uint32_t index, value;

    // copy the vector table from the beginning of FLASH to the RAM vector table
    value = HWREG(VTABLE);
    for(index = 0; index < NUM_OF_INTERRUPTS; index++){
        __VECTOR_RAM[index] = HWREG((index * 4) + value);
    }

    // data memory barrier to ensure that write to memory is completed
    __asm("DMB \n");

    // point the NVIC at the RAM vector table
    HWREG(VTABLE) = KERNEL_START_ADDRESS;

    // data synchronization barrier to ensure all subsequent instruction use new configuration
    __asm("DSB \n");
    

}
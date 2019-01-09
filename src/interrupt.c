//**********************************************************************//
//    Interrrupt Library For TM4C1249 TIVA C Series MCU                 //
//                                                                      //
//    Modified Date: 04-04-2017                                         //
//    Author: chetandev.ksd@gmail.com                                   //
//**********************************************************************// 

#include <interrupt.h>
#include <access.h>
#include <error.h>
#include <svc.h>
#include <share.h>
#include <kernel.h>
#include <regmap.h>
#include <kvar.h>




/* This is a mapping between priority grouping encodings and the number of preemption priority bits.*/
const uint32_t g_pui32Priority[] ={
PRIGROUP_8_1,
PRIGROUP_4_2,
PRIGROUP_2_4,
PRIGROUP_1_8
};


/* This is a mapping between interrupt number and the register that contains the priority encoding 
for that interrupt.*/
const uint32_t prioreg[] =
{
 NVIC_PRI0,  NVIC_PRI1,  NVIC_PRI2,  NVIC_PRI3,  NVIC_PRI4,  NVIC_PRI5,  
 NVIC_PRI6,  NVIC_PRI7,  NVIC_PRI8,  NVIC_PRI9,  NVIC_PRI10, NVIC_PRI11, 
 NVIC_PRI12, NVIC_PRI13, NVIC_PRI14, NVIC_PRI15, NVIC_PRI16, NVIC_PRI17, 
 NVIC_PRI18, NVIC_PRI19, NVIC_PRI20, NVIC_PRI21, NVIC_PRI22, NVIC_PRI23, 
 NVIC_PRI24, NVIC_PRI25, NVIC_PRI26, NVIC_PRI27, NVIC_PRI28, NVIC_PRI29, 
 NVIC_PRI30, NVIC_PRI31, NVIC_PRI32, NVIC_PRI33, NVIC_PRI34
};


/* This is a mapping between interrupt number (for the peripheral interrupts only)
 and the register that contains the interrupt disable for that interrupt.*/
const uint32_t g_pui32Dii16Regs[] ={
    NVIC_DIS0, NVIC_DIS1, NVIC_DIS2, NVIC_DIS3
};

/* This is a mapping between interrupt number (for the peripheral interrupts only) and 
the register that contains the interrupt enable for that interrupt.*/
const uint32_t g_pui32EnRegs[] ={
    NVIC_EN0, NVIC_EN1, NVIC_EN2, NVIC_EN3
};

uint32_t __attribute__((naked)) global_interrupt_enable(){

    uint32_t ret;

    /* Read PRIMASK and enable interrupts. */
    __asm("    mrs     r0, PRIMASK\n"
          "    cpsie   i\n"
          "    bx      lr\n"
          : "=r" (ret));

    /* The return is handled in the inline assembly, but the compiler will
     * still complain if there is not an explicit return here (despite the fact
     * that this does not result in any code being produced because of the
     * naked attribute).
     */
    return(ret);

}

uint32_t __attribute__((naked)) global_interrupt_disable(){

    uint32_t ui32Ret;

    /* Read PRIMASK and disable interrupts. */
    __asm("    mrs     r0, PRIMASK\n"
          "    cpsid   i\n"
          "    bx      lr\n"
          : "=r" (ui32Ret));

    /* The return is handled in the inline assembly, but the compiler will
        still complain if there is not an explicit return here (despite the fact
        that this does not result in any code being produced because of the
        naked attribute).*/
    return(ui32Ret);

}

uint32_t __attribute__((naked)) ipsr(){
    
    uint32_t ui32Ret;

    /* Read PRIMASK and disable interrupts. */
    __asm("    mrs     r0, ipsr\n"
          "    bx      lr\n"
          : "=r" (ui32Ret));

    /* The return is handled in the inline assembly, but the compiler will
        still complain if there is not an explicit return here (despite the fact
        that this does not result in any code being produced because of the
        naked attribute).*/
    return(ui32Ret);

}

uint8_t interrupt_enable(uint8_t interrupt){

    uint8_t error;

    /* Valid Interrupt number */
    if(interrupt < 16) {
        return ERROR_ACCESS_DENIED;
    }

    __asm volatile (" MOV R0, %[interrupt]  \n"
            :
            : [interrupt] "r" (interrupt)
            :
    );

    svc(INT_ENABLE);

    __asm volatile (" STR R0, %[err] \n"
        : [err] "=m" (error)
        :
        :
    );

    return error;

}


uint8_t interrupt_disable(uint8_t interrupt) {

    uint8_t error;

    /* Valid Interrupt number */
    if(interrupt < 16) {
        return ERROR_ACCESS_DENIED;
    }

    __asm volatile (" MOV R0, %[interrupt]  \n"
            :
            : [interrupt] "r" (interrupt)
            :
    );

    svc(INT_DISABLE);

    __asm volatile (" STR R0, %[err] \n"
        : [err] "=m" (error)
        :
        :
    );

    return error;
    
}

uint8_t interrupt_register(uint32_t interrupt, void (*pfnHandler)(void)){

    uint8_t error;

    /* Valid Interrupt number */
    if(interrupt < 16) {
        return ERROR_ACCESS_DENIED;
    }

    __asm volatile (" MOV R0, %[interrupt]  \n"
                    " MOV R1, %[handler]    \n"
            :
            : [interrupt] "r" (interrupt), [handler] "r" (pfnHandler)
            :
    );

    svc(INT_REGISTER);

    __asm volatile (" STR R0, %[err] \n"
        : [err] "=m" (error)
        :
        :
    );

    return error;
}

uint8_t interrupt_set_priority(uint8_t interrupt, uint8_t priority){

    uint8_t error;

    if(interrupt < 16) {
        return ERROR_ACCESS_DENIED;
    }

    __asm volatile (" MOV R0, %[interrupt]  \n"
                    " MOV R1, %[prio]       \n"
            :
            : [interrupt] "r" (interrupt), [prio] "r" (priority)
            :
    );

    svc(SET_PRIORITY);

    __asm volatile (" STR R0, %[err] \n"
        : [err] "=m" (error)
        :
        :
    );

    return error;
    
}

void vector_table_relocate(void){

    uint32_t index, value;

    /* Already relocated to SRAM ? */
    if(HWREG(VTABLE) != SRAM_START_ADDRESS) {

        /* Copy the vector table from the beginning of FLASH to the RAM vector table. */
        value = HWREG(VTABLE);
        for(index = 0; index < NUM_OF_INTERRUPTS; index++){
            HWREG(SRAM_START_ADDRESS + (index * 4)) = HWREG((index * 4) + value);
        }


        /* Data Memory Barrier to ensure that write to memory is completed */
        __asm("DMB \n");

        /* Point the NVIC at the RAM vector table. */
        HWREG(VTABLE) = SRAM_START_ADDRESS;

        /* Data Synchronization Barrier to ensure all subsequent instruction use new configuration */
        __asm("DSB \n");
    
    }

}
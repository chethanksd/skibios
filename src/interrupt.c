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
#include <param.h>
#include <kernel.h>
#include <regmap.h>
#include <kvar.h>
#include <os_support.h>



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

    SVC_INT_ENABLE(interrupt);

    GET_SVC_RETURN_CODE(error);

    return error;

}


uint8_t interrupt_disable(uint8_t interrupt) {

    uint8_t error;

    SVC_INT_DISABLE(interrupt);

    GET_SVC_RETURN_CODE(error);

    return error;
    
}

uint8_t interrupt_register(uint32_t interrupt, void (*pfnHandler)(void)){

    uint8_t error;

    SVC_INT_REGISTER(interrupt, pfnHandler);

    GET_SVC_RETURN_CODE(error);

    return error;
}

uint8_t interrupt_set_priority(uint8_t interrupt, uint8_t priority){

    uint8_t error;

    SVC_SET_PRIORITY(interrupt, priority);

    GET_SVC_RETURN_CODE(error);

    return error;
    
}
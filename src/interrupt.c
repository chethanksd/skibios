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



uint8_t interrupt_enable(uint32_t interrupt){

    uint8_t error;

    SVC_INT_ENABLE(interrupt, error);

    return error;

}


uint8_t interrupt_disable(uint32_t interrupt) {

    uint8_t error;

    SVC_INT_DISABLE(interrupt, error);

    return error;
    
}

uint8_t interrupt_register(uint32_t interrupt, void (*pfnHandler)(void)){

    uint8_t error;

    SVC_INT_REGISTER(interrupt, pfnHandler, error);

    return error;
}

uint8_t interrupt_set_priority(uint32_t interrupt, uint8_t priority){

    uint8_t error;

    SVC_SET_PRIORITY(interrupt, priority, error);

    return error;
    
}
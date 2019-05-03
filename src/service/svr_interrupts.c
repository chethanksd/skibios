/*
 * interrupt management kernel service code
 *
 * author: chetandev.ksd@gmail.com
 *
 */

#include <interrupt.h>
#include <error.h>
#include <kvar.h>
#include <param.h>
#include <svc.h>
#include <access.h>
#include <regmap.h>
#include <kernel.h>
#include <permissions.h>
#include <svr_interrupts.h>


uint32_t svc_service_int_enable(uint32_t *svc_num, uint32_t *arguments) {
    
    uint32_t error = ERROR_NONE;
    uint32_t irq_no;

    irq_no = arguments[0];

    // check access permission of currrent_task for Interrupt control
    if((permissions[current_task] & (1 << PERMISSION_INTCTRL)) != (1 << PERMISSION_INTCTRL)) {
        error = ERROR_ACCESS_DENIED;
        goto quit_error;
    }

    /* Enable the general interrupt.*/
    HWREG(g_pui32EnRegs[(irq_no - 16) / 32]) = 1 << ((irq_no - 16) & 31);

 quit_error:

    return error;

}

uint32_t svc_service_int_disable(uint32_t *svc_num, uint32_t *arguments) {

    uint32_t error = ERROR_NONE;
    uint32_t irq_no;

    irq_no = arguments[0];

    // check access permission of currrent_task for Interrupt control
    if((permissions[current_task] & (1 << PERMISSION_INTCTRL)) != (1 << PERMISSION_INTCTRL)) {
        error = ERROR_ACCESS_DENIED;
        goto quit_error;
    }

    /*  Disable the general interrupt. */
    HWREG(g_pui32Dii16Regs[(irq_no - 16) / 32]) = 1 << ((irq_no - 16) & 31);

quit_error:

    return error;

}

uint32_t svc_service_int_set_priority(uint32_t *svc_num, uint32_t *arguments) {

    uint32_t error = ERROR_NONE;
    uint32_t irq_no;
    uint32_t priority;

    irq_no = arguments[0];
    priority = arguments[1];

    // check access permission of currrent_task for Interrupt control
    if((permissions[current_task] & (1 << PERMISSION_INTCTRL)) != (1 << PERMISSION_INTCTRL)) {
        error = ERROR_ACCESS_DENIED;
        goto quit_error;
    }

    HWREG(prioreg[(irq_no - 16)/4]) |= (PRIORITY_WRITE_MASK & (uint32_t)priority) << (8 * ((irq_no - 16) & 3));

quit_error:

    return error;

}

uint32_t svc_service_int_register(uint32_t *svc_num, uint32_t *arguments) {

    uint32_t error = ERROR_NONE;
    uint32_t irq_no;
    uint32_t handler_address;

    irq_no = arguments[0];
    handler_address = arguments[1];

    // check access permission of currrent_task for Interrupt control
    if((permissions[current_task] & (1 << PERMISSION_INTCTRL)) != (1 << PERMISSION_INTCTRL)) {
        error = ERROR_ACCESS_DENIED;
        goto quit_error;
    }

    /* Save the interrupt handler.*/
    HWREG(SRAM_START_ADDRESS + (irq_no * 4)) = handler_address;

quit_error:

    return error;

}
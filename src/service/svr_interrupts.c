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
#include <os_support.h>


uint32_t svc_service_int_enable(uint32_t *svc_num, uint32_t *arguments) {
    
    uint32_t error = ERROR_NONE;
    uint32_t irq_no;

    irq_no = arguments[0];

    // check access permission of currrent_task for Interrupt control
    if((permissions[current_task] & (1 << PERMISSION_INTCTRL)) != (1 << PERMISSION_INTCTRL)) {
        error = ERROR_ACCESS_DENIED;
        goto quit_error;
    }

    error = arch_interrupt_enable(irq_no);

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

    error = arch_interrupt_disable(irq_no);

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

    error = arch_interrupt_priority(irq_no, priority);

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

    error = arch_interrupt_register(irq_no, handler_address);

quit_error:

    return error;

}
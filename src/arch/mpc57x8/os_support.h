#ifndef _OS_SUPPORT_H_
#define _OS_SUPPORT_H_

#include <os_util.h>
#include <svc_macros.h>
#include <arch_interrupt.h>

extern void scheduler();
extern uint32_t call_kernel_service(uint32_t svc_code, uint32_t arg1, uint32_t arg2, uint32_t arg3, uint32_t arg4);


#define svc(code) call_kernel_service(code, 0, 0, 0, 0)

#define MFSPR( rn )        ({unsigned int rval; __asm__ volatile("mfspr %0,%1" : "=r" (rval) : "I" (rn)); rval;})


// GET_CORE_ID returns the processor identification number for e200z
#define GET_CORE_ID()	MFSPR(286)

// Enable SYSTICK timer to run scheduler
#define ENABLE_SCHEDULER() *((uint32_t*) &STM->CR) = 0x903

// Disable SYSTICK timer to stop scheduler
#define DISABLE_SCHEDULER() *((uint32_t*) &STM->CR) = 0x902

// Trigger SYSTICK interrupt to run scheduler immediately
#define TRIGGER_SCHEDULER() scheduler()

// Clear STM timer value
#define SCHEDULER_TIMER_RESET() *((uint32_t*) &STM->CNT) = 0

// Trigger Context Switch ISR
#define TRIGGER_CONTEXT_SWITCH() STM->CHANNEL[0].CIR.CIF = 1; INTC->SSCIR[CONTEXT_SWITCH_SW_INT].SET = 1

// Device reset
#define TRIGGER_DEVICE_RESET()

#endif

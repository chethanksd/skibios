#ifndef _OS_SUPPORT_H_
#define _OS_SUPPORT_H_

#include <mpu.h>
#include <os_util.h>
#include <svc_macros.h>
#include <arch_interrupt.h>
#include <regmap.h>


// inline assembler helper directive: call SVC with the given immediate
#define svc(code) __asm("svc %[immediate]"::[immediate] "I" (code));


// Enable STM timer to run scheduler
#define ENABLE_SCHEDULER() HWREG(STCTRL) |= SYSTICK_INT_ENABLE | SYSTICK_ENABLE

// Disable STM timer to stop scheduler
#define DISABLE_SCHEDULER() HWREG(STCTRL) &= SYSTICK_DISABLE & SYSTICK_INT_DISABLE

// Trigger STM interrupt to run scheduler immediately
#define TRIGGER_SCHEDULER() HWREG(INTCTRL) |= (1 << INTCTRL_PENDSTSET)

// Clear STM timer value
#define SCHEDULER_TIMER_RESET() HWREG(STCURRENT) = 0x00000000

// Triiger Context Switch ISR
#define TRIGGER_CONTEXT_SWITCH() HWREG(ICSR) |= 0x10000000

// Device reset
#define TRIGGER_DEVICE_RESET() HWREG(APINT) = APINT_VECKEY | APINT_SYSRESETREQ

#endif
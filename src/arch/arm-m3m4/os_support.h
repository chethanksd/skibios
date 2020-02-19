#ifndef _OS_SUPPORT_H_
#define _OS_SUPPORT_H_

#include <mpu.h>
#include <svc_macros.h>
#include <regmap.h>


// inline assembler helper directive: call SVC with the given immediate
#define svc(code) __asm("svc %[immediate]"::[immediate] "I" (code));


// Enable STM timer to run scheduler
#define ENABLE_SCHEDULER() 

// Disable STM timer to stop scheduler
#define DISABLE_SCHEDULER()

// Trigger STM interrupt to run scheduler immediately
#define TRIGGER_SCHEDULER() HWREG(INTCTRL) |= (1 << INTCTRL_PENDSTSET)


#endif
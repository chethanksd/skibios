#ifndef _OS_SUPPORT_H_
#define _OS_SUPPORT_H_

#include <svc_macros.h>
#include <arch_interrupt.h>


#define svc(code) \
    asm volatile ("e_li    3, %[i_code]     \n\t" \
                  "se_sc                    \n\t" \
                  :: [i_code] "I" (code) :  \
                  )

#define MFSPR( rn )        ({unsigned int rval; __asm__ volatile("mfspr %0,%1" : "=r" (rval) : "I" (rn)); rval;})


// GET_CORE_ID returns the processor identification number for e200z
#define GET_CORE_ID()	MFSPR(286)

// Enable SYSTICK timer to run scheduler
#define ENABLE_SCHEDULER() 

// Disable SYSTICK timer to stop scheduler
#define DISABLE_SCHEDULER()

// Trigger SYSTICK interrupt to run scheduler immediately
#define TRIGGER_SCHEDULER() 


#endif
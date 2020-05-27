#ifndef _OS_SUPPORT_H_
#define _OS_SUPPORT_H_

#include <os_util.h>
#include <svc_macros.h>

typedef TASK_RETURN_T(*task_func_t)();

extern uint32_t call_kernel_service(uint32_t svc_code, uint32_t arg1, uint32_t arg2, uint32_t arg3, uint32_t arg4);

#define svc(code) call_kernel_service(code, 0, 0, 0, 0)

// Enable SYSTICK timer to run scheduler
#define ENABLE_SCHEDULER() 

// Disable SYSTICK timer to stop scheduler
#define DISABLE_SCHEDULER() 

// Trigger SYSTICK interrupt to run scheduler immediately
#define TRIGGER_SCHEDULER() 

// Clear STM timer value
#define SCHEDULER_TIMER_RESET()  

// Trigger Context Switch ISR
#define TRIGGER_CONTEXT_SWITCH() 

// Device reset
#define TRIGGER_DEVICE_RESET()

#endif
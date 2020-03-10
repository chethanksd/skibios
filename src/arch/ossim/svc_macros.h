#ifndef _SVC_MACROS_H_
#define _SVC_MACROS_H_

#ifdef ARCH_SVC_VERSION_MAJOR
#error "ARCH_SVC_VERSION_MAJOR is defined elsewhere in the program"
#endif

#ifdef ARCH_SVC_VERSION_MINOR
#error "ARCH_SVC_VERSION_MINOR is defined elsewhere in the program"
#endif

#define ARCH_SVC_VERSION_MAJOR      1
#define ARCH_SVC_VERSION_MINOR      1


// CREATE_PROCESS without process argument
#define SVC_CREATE_PROCESS_NO_ARG(proc_obj) 


// CREATE_PROCESS with process argument
#define SVC_CREATE_PROCESS_WITH_ARG(proc_obj, proc_arg) 


// KILL_PROCESS for direct process kill
#define SVC_KILL_PROCESS_DIRECT(process_id) 
 

// KILL_PROCESS to resolve end of process
#define SVC_KILL_PROCESS_RESOLVE_END(process_id) 


// HIBERNATE 
#define SVC_HIBERNATE(merged, op1, op2, value) 


// PRIORITY_PROMOTE
#define SVC_PRIORITY_PROMOTE(mutex) 


// INT_ENABLE
#define SVC_INT_ENABLE(interrrupt, error) 


// INT_DISABLE
#define SVC_INT_DISABLE(interrupt, error) 


// INT_REGISTER
#define SVC_INT_REGISTER(interrupt, handler, error) 


// SET_PRIORITY
#define SVC_SET_PRIORITY(interrupt, priority, error) 


// INVOKE_BASE
#define SVC_INVOKE_BASE(argument, error) 


// RELEASE_BASE
#define SVC_RELEASE_BASE(error, return_error) 


// GRANT_PERMISSION
#define SVC_GRANT_PERMISSION(process_id, permission, error) 


// CPU_FREQ
#define SVC_CPU_FREQ_UPDATE(frequency, error) 


// ENABLE_UMPU
#define SVC_ENABLE_UMPU(process_id, permission, error) 


// DISABLE_UMPU
#define SVC_DISABLE_UMPU(region, error) 


// HWREG_WRITE
#define SVC_HWREG_WRITE(register_address, value, error) 


// HWREG_READ
#define SVC_HWREG_READ(register_address, value, error) 

#endif
#ifndef _SVC_MACROS_H_
#define _SVC_MACROS_H_

// defined in os_util.S 
extern uint32_t call_kernel_service(uint32_t svc_code, uint32_t arg1, uint32_t arg2, uint32_t arg3, uint32_t arg4);

// Get SVC Servie return code
#define GET_SVC_RETURN_CODE(error) \
    __asm volatile ("lwz 4, %[i_error] \n" \
                    "se_stw 3, 0(4)    \n" \
                :: [i_error] "m" (error):)


// CREATE_PROCESS without process argument
#define SVC_CREATE_PROCESS_NO_ARG(proc_obj) call_kernel_service(CREATE_PROCESS, proc_obj, 0, 0, 0)


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
#define SVC_INT_ENABLE(interrrupt)


// INT_DISABLE
#define SVC_INT_DISABLE(interrupt) 


// INT_REGISTER
#define SVC_INT_REGISTER(interrupt, handler) 


// SET_PRIORITY
#define SVC_SET_PRIORITY(interrupt, priority) 


// INVOKE_BASE
#define SVC_INVOKE_BASE(argument) 


// RELEASE_BASE
#define SVC_RELEASE_BASE(error) 


// GRANT_PERMISSION
#define SVC_GRANT_PERMISSION(process_id, permission)


#endif
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
#define SVC_CREATE_PROCESS_WITH_ARG(proc_obj, proc_arg) call_kernel_service(CREATE_PROCESS, proc_obj, proc_arg, 0, 0)


// KILL_PROCESS for direct process kill
#define SVC_KILL_PROCESS_DIRECT(process_id) call_kernel_service(KILL_PROCESS, 0, 0, 0, 0)
 

// KILL_PROCESS to resolve end of process
#define SVC_KILL_PROCESS_RESOLVE_END(process_id) call_kernel_service(KILL_PROCESS, 1, 0, 0, 0)


// HIBERNATE 
#define SVC_HIBERNATE(merged, op1, op2, value) call_kernel_service(HIBERNATE, merged, op1, op2, value)


// PRIORITY_PROMOTE
#define SVC_PRIORITY_PROMOTE(mutex) call_kernel_service(PRIORITY_PROMOTE, mutex, 0, 0, 0)


// INT_ENABLE
#define SVC_INT_ENABLE(interrrupt, error) error = call_kernel_service(INT_ENABLE, interrupt, 0, 0, 0)


// INT_DISABLE
#define SVC_INT_DISABLE(interrupt, error) error = call_kernel_service(INT_DISABLE, interrupt, 0, 0, 0)


// INT_REGISTER
#define SVC_INT_REGISTER(interrupt, handler, error) error = call_kernel_service(INT_REGISTER, interrupt, (uint32_t)handler, 0, 0)


// SET_PRIORITY
#define SVC_SET_PRIORITY(interrupt, priority, error) error = call_kernel_service(SET_PRIORITY, interrupt, priority, 0, 0)


// INVOKE_BASE
#define SVC_INVOKE_BASE(argument, error) error = call_kernel_service(INVOKE_BASE, argument, 0, 0, 0)


// RELEASE_BASE
#define SVC_RELEASE_BASE(error, return_error) return_error = call_kernel_service(RELEASE_BASE, error, 0, 0, 0)


// GRANT_PERMISSION
#define SVC_GRANT_PERMISSION(process_id, permission, error) error = call_kernel_service(GRANT_PERMISSION, process_id, permission, 0, 0)


// CPU_FREQ
#define SVC_CPU_FREQ_UPDATE(frequency, error) error = call_kernel_service(CPU_FREQ, frequency, 0, 0, 0)


// ENABLE_UMPU
#define SVC_ENABLE_UMPU(process_id, permission, error) error = call_kernel_service(ENABLE_UMPU, process_id, permission, 0, 0)


// DISABLE_UMPU
#define SVC_DISABLE_UMPU(region, error) error = call_kernel_service(DISABLE_UMPC, region, 0, 0, 0)

// HWREG_WRITE
#define SVC_HWREG_WRITE(register_address, value, error) error = call_kernel_service(HWREG_WRITE, register_address, value, 0, 0)

#endif
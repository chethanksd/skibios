#ifndef _SVC_MACROS_H_
#define _SVC_MACROS_H_


// Get SVC Servie return code
#define GET_SVC_RETURN_CODE(error) \
    __asm volatile (" STR R0, %[i_error]        \n" \
        : [i_error] "=m" (error)::)


// CREATE_PROCESS without process argument
#define SVC_CREATE_PROCESS_NO_ARG(proc_obj) \
    __asm volatile (" LDR R0, %[i_proc_obj]     \n" \
                    " MOV R1, #0                \n" \
                    " SVC %[svc_code]           \n" \
                    :: [i_proc_obj] "m" (proc_obj), [svc_code] "I" (CREATE_PROCESS):)


// CREATE_PROCESS with process argument
#define SVC_CREATE_PROCESS_WITH_ARG(proc_obj, proc_arg) \
    __asm volatile (" LDR R0, %[i_proc_obj]     \n" \
                    " LDR R1, %[i_proc_arg]     \n" \
                    " SVC %[svc_code]           \n" \
                    :: [i_proc_obj] "m" (proc_obj), [i_proc_arg] "m" (proc_arg), [svc_code] "I" (CREATE_PROCESS):)


// KILL_PROCESS for direct process kill
#define SVC_KILL_PROCESS_DIRECT(process_id) \
    __asm volatile (" LDR R0, %[i_proc_id]      \n" \
                    " MOV R1, #0                \n" \
                    " SVC %[svc_code]           \n" \
                    :: [i_proc_id] "m" (process_id), [svc_code] "I" (KILL_PROCESS):)
 

 // KILL_PROCESS to resolve end of process
 #define SVC_KILL_PROCESS_RESOLVE_END(process_id) \
     __asm volatile (" LDR R0, %[i_proc_id]      \n" \
                     " MOV R1, #1                \n" \
                     " SVC %[svc_code]           \n" \
                    :: [i_proc_id] "m" (process_id), [svc_code] "I" (KILL_PROCESS):)


// HIBERNATE 
#define SVC_HIBERNATE(merged, op1, op2, value) \
    __asm volatile (" LDR R0, %[i_mrg]          \n" \
                    " LDR R1, %[i_op1]          \n" \
                    " LDR R2, %[i_op2]          \n" \
                    " LDR R3, %[i_val]          \n" \
                    " SVC %[svc_code]           \n" \
                    :: [i_mrg] "m" (merged), [i_op1] "m" (op1), [i_op2] "m" (op2), [i_val] "m" (value),  [svc_code] "I" (HIBERNATE):)


// PRIORITY_PROMOTE
#define SVC_PRIORITY_PROMOTE(mutex) \
    __asm volatile (" LDR R0, %[i_mutex]        \n" \
                    " SVC %[svc_code]           \n" \
                    :: [i_mutex] "m" (mutex), [svc_code] "I" (PRIORITY_PROMOTE):)


// INT_ENABLE
#define SVC_INT_ENABLE(interrupt, error)   \
    __asm volatile (" MOV R0, %[i_interrupt]    \n" \
                    " SVC %[svc_code]           \n" \
                    :: [i_interrupt] "r" (interrupt),  [svc_code] "I" (INT_ENABLE):); \
    GET_SVC_RETURN_CODE(error)


// INT_DISABLE
#define SVC_INT_DISABLE(interrupt, error)  \
    __asm volatile (" MOV R0, %[i_interrupt]    \n" \
                    " SVC %[svc_code]           \n" \
                    :: [i_interrupt] "r" (interrupt),  [svc_code] "I" (INT_DISABLE):); \
    GET_SVC_RETURN_CODE(error)


// INT_REGISTER
#define SVC_INT_REGISTER(interrupt, handler, error)  \
    __asm volatile (" MOV R0, %[i_interrupt]    \n" \
                    " MOV R1, %[i_handler]      \n" \
                    " SVC %[svc_code]           \n" \
                    :: [i_interrupt] "r" (interrupt), [i_handler] "r" (handler), [svc_code] "I" (INT_REGISTER):); \
    GET_SVC_RETURN_CODE(error)


// SET_PRIORITY
#define SVC_SET_PRIORITY(interrupt, priority, error) \
    __asm volatile (" MOV R0, %[i_interrupt]    \n" \
                    " MOV R1, %[i_priority]     \n" \
                    " SVC %[svc_code]           \n" \
                    :: [i_interrupt] "r" (interrupt), [i_priority] "r" (priority),  [svc_code] "I" (SET_PRIORITY):); \
    GET_SVC_RETURN_CODE(error)


// INVOKE_BASE
#define SVC_INVOKE_BASE(argument, error) \
    __asm volatile (" LDR R0, %[i_argument]     \n" \
                    " SVC %[svc_code]           \n" \
                    ::[i_argument] "m" (argument), [svc_code] "I" (INVOKE_BASE):); \
    GET_SVC_RETURN_CODE(error)


// RELEASE_BASE
#define SVC_RELEASE_BASE(error, return_error)  \
    __asm volatile (" LDR R0, %[i_error]        \n" \
                    " SVC %[svc_code]           \n" \
                    ::[i_error] "m" (error), [svc_code] "I" (RELEASE_BASE):); \
    GET_SVC_RETURN_CODE(return_error)


// GRANT_PERMISSION
#define SVC_GRANT_PERMISSION(process_id, permission, error) \
    __asm volatile (" LDR   R0, %[i_process_id] \n" \
                    " LDRH  R1, %[i_permission] \n" \
                    " SVC %[svc_code]           \n" \
                    ::[i_process_id] "m" (process_id), [i_permission] "m" (permission), [svc_code] "I" (GRANT_PERMISSION):); \
    GET_SVC_RETURN_CODE(error)


// CPU_FREQ
#define SVC_CPU_FREQ_UPDATE(frequency, error) \
    __asm volatile (" LDR R1, %[i_freq]         \n" \
                    " SVC %[svc_code]           \n" \
                     :: [i_freq] "m" (frequency), [svc_code] "I" (CPU_FREQ):); \
    GET_SVC_RETURN_CODE(error)


// ENABLE_UMPU
#define SVC_ENABLE_UMPU(region, address, attributes, error) \
    __asm volatile (" LDRB R0, %[i_region]      \n" \
                    " LDR  R1, %[i_address]     \n" \
                    " LDR  R2, %[i_attributes]  \n" \
                    " SVC  %[svc_code]          \n" \
                    :: [i_region] "m" (region), [i_address] "m" (address), [i_attributes] "m" (attributes), [svc_code] "I" (ENABLE_UMPU):); \
    GET_SVC_RETURN_CODE(error)


// DISABLE_UMPU
#define SVC_DISABLE_UMPU(region, error) \
    __asm volatile (" LDRB R0, %[i_region]      \n" \
                    " SVC  %[svc_code]          \n" \
                    :: [i_region] "m" (region), [svc_code] "I" (DISABLE_UMPU):); \
    GET_SVC_RETURN_CODE(error)


// HWREG_WRITE
#define SVC_HWREG_WRITE(register_address, value, error) \
    __asm volatile (" LDR   R0, %[i_reg_addr]   \n" \
                    " LDR   R1, %[i_value]      \n" \
                    " SVC   %[svc_code]         \n" \
                    :: [i_reg_addr] "m" (register_address), [i_value] "m" (value), [svc_code] "I" (DISABLE_UMPU):); \
    GET_SVC_RETURN_CODE(error) 


// HWREG_READ
#define SVC_HWREG_READ(register_address, value, error) 


#endif
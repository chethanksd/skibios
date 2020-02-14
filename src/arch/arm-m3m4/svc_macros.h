#ifndef _SVC_MACROS_H_
#define _SVC_MACROS_H_


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

#endif
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

#endif
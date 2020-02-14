#ifndef _SVC_MACROS_H_
#define _SVC_MACROS_H_


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

#endif
#ifndef _TASK_OBJ_H_
#define _TASK_OBJ_H_

#include <stdint.h>
#include <stdbool.h>

#ifdef OSSIM_RUN
#include <windows.h>
#endif

#ifdef OSSIM_RUN
#define TASK_RETURN_T  DWORD WINAPI
#else
#define TASK_RETURN_T  void
#endif

typedef struct {

    uint8_t error;
    uint32_t process_id;
    uint8_t priority;
    TASK_RETURN_T *ptr_func;
    
    // hibernate variables
    uint8_t hibernate;
    uint32_t *op1;
    uint32_t *op2;
    uint32_t value;

} task_t;


#endif
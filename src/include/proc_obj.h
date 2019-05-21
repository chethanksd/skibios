#ifndef _PROC_OBJ_H_
#define _PROC_OBJ_H_

#include <stdint.h>
#include <stdbool.h>

typedef struct {

    uint8_t error;
    uint32_t process_id;
    uint8_t priority;
    void *ptr_func;
    
    // hibernate variables
    uint8_t hibernate;
    uint32_t *op1;
    uint32_t *op2;
    uint32_t value;

} process_t;


#endif
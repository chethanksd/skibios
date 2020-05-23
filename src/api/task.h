#ifndef _TASK_H_
#define _TASK_H_

#include <stdint.h>
#include <stdbool.h>
#include <task_obj.h>


#ifdef	__cplusplus
extern "C" {
#endif

extern uint8_t task_init(task_t *myprocess);
extern uint8_t task_start(task_t *myprocess);
extern uint8_t task_start_arg(task_t *myprocess, void *arg);
extern uint8_t task_kill(task_t *myprocess);
extern uint8_t hibernate(uint32_t *op1, uint32_t *op2, uint32_t value, bool reverse);

#ifdef	__cplusplus
}
#endif

#endif
#ifndef PROCESS_H
#define PROCESS_H

#include <stdint.h>
#include <stdbool.h>
#include <proc_obj.h>


#ifdef	__cplusplus
extern "C" {
#endif

extern uint8_t process_init(process_t *myprocess);
extern uint8_t process_start(process_t *myprocess);
extern uint8_t process_start_arg(process_t *myprocess, void *arg);
extern uint8_t process_kill(process_t *myprocess);
extern uint8_t hibernate(uint32_t *op1, uint32_t *op2, uint32_t value, bool reverse);

#ifdef	__cplusplus
}
#endif

#endif
#ifndef PROCESS_H
#define PROCESS_H

#include <stdint.h>
#include <stdbool.h>

// process states
#define PROCESS_STATE_ACTIVE        40
#define PROCESS_STATE_SLEEP         41
#define PROCESS_STATE_IDLE          42
#define PROCESS_STATE_HOLD          43
#define PROCESS_STATE_HIBERNATE_G   80
#define PROCESS_STATE_HIBERNATE_L   81

#define HIBERNATE_STATE_MASK        0x80
#define HIBEARNTE_REV_MASK          0x00010000

#ifdef	__cplusplus
extern "C" {
#endif

typedef struct{

    uint8_t error;
    uint32_t process_id;
    uint8_t priority;
    void *pfnProcess;
    
    // hibernate variables
    uint8_t hibernate;
    uint32_t *op1;
    uint32_t *op2;
    uint32_t value;

} Process;

extern uint8_t process_init(Process *myprocess);
extern uint8_t process_start(Process *myprocess);
extern uint8_t process_start_arg(Process *myprocess, void *arg);
extern uint8_t process_kill(Process *myprocess);
extern uint8_t hibernate(uint32_t *op1, uint32_t *op2, uint32_t value, bool reverse);

#ifdef	__cplusplus
}
#endif

#endif
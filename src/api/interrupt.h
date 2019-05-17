#ifndef INTERRUPT_H
#define INTERRUPT_H

#include <stdint.h>
#include <stdbool.h>

// macros to enable and disable fualt exceptions
#define FLE() __asm("cpsie F")
#define FLD() __asm("cpsid F")


#ifdef __cplusplus
extern "C"
{
#endif

extern uint32_t global_interrupt_enable(void);
extern uint32_t global_interrupt_disable(void);
extern uint32_t ipsr(void);
extern uint8_t interrupt_enable(uint8_t interrupt);
extern uint8_t interrupt_disable(uint8_t interrupt);
extern uint8_t interrupt_register(uint32_t interrupt, void (*pfnHandler)(void));
extern uint8_t interrupt_set_priority(uint8_t interrupt, uint8_t priority);

#ifdef __cplusplus
}
#endif

#endif
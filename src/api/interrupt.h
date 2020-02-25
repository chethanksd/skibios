#ifndef INTERRUPT_H
#define INTERRUPT_H

#include <stdint.h>
#include <stdbool.h>




#ifdef __cplusplus
extern "C"
{
#endif

extern uint8_t interrupt_enable(uint8_t interrupt);
extern uint8_t interrupt_disable(uint8_t interrupt);
extern uint8_t interrupt_register(uint32_t interrupt, void (*pfnHandler)(void));
extern uint8_t interrupt_set_priority(uint8_t interrupt, uint8_t priority);

#ifdef __cplusplus
}
#endif

#endif
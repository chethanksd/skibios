#ifndef _ARCH_INTERRUPT_H_
#define _ARCH_INTERRUPT_H_

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

extern uint8_t arch_interrupt_enable(uint32_t interrupt);
extern uint8_t arch_interrupt_disable(uint32_t interrupt);
extern uint8_t arch_interrupt_register(uint32_t interrupt, uint32_t handler);
extern uint8_t arch_interrupt_priority(uint8_t interrupt, uint8_t priority);

#ifdef __cplusplus
}
#endif

#endif
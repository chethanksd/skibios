#ifndef KERNEL_H
#define KERNEL_H

#include <stdint.h>
#include <stdbool.h>
#include <../src/include/svc.h>

// systick reload register mask
#define SYSTICK_RELOAD_MASK     0x00FFFFFF
#define SRAM_START_ADDRESS      0x20000000

// inline assembler helper directive: call SVC with the given immediate
#define svc(code) __asm("svc %[immediate]"::[immediate] "I" (code));

#ifdef	__cplusplus
extern "C" {
#endif

extern const uint32_t zero_ref;

extern uint8_t kernel_init();
extern void start_scheduler(void);
extern uint8_t device_reset();
extern uint8_t cpu_freq_update(uint32_t frequency);

#ifdef	__cplusplus
}
#endif

#endif

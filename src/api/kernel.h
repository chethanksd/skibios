#ifndef KERNEL_H
#define KERNEL_H

#include <stdint.h>
#include <stdbool.h>
#include <svc.h>
#include <app_support.h>


#ifdef	__cplusplus
extern "C" {
#endif

extern const uint32_t zero_ref;

extern uint32_t kernel_init();
extern void start_scheduler(void);
extern uint8_t device_reset();
extern uint8_t cpu_freq_update(uint32_t frequency);

extern uint8_t hwreg_write(uint32_t register_address, uint32_t value);

#ifdef	__cplusplus
}
#endif

#endif

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
extern uint32_t start_scheduler(void);
extern uint32_t device_reset();
extern uint32_t cpu_freq_update(uint32_t frequency);

extern uint32_t hwreg_write(uint32_t register_address, uint32_t value);
extern uint32_t hwreg_read(uint32_t register_address, uint32_t *value);

#ifdef	__cplusplus
}
#endif

#endif

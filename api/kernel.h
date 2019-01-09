#ifndef KERNEL_H
#define KERNEL_H

#include <stdint.h>
#include <stdbool.h>


// permission bits
#define PERMISSION_UMPU0_W      0
#define PERMISSION_UMPU0_R      1
#define PERMISSION_UMPU1_W      2
#define PERMISSION_UMPU1_R      3
#define PERMISSION_UMPU2_W      4
#define PERMISSION_UMPU2_R      5
#define PERMISSION_INTCTRL      6
#define PERMISSION_PROCESS      7
#define PERMISSION_SCHHALT      8

// systick reload register mask
#define SYSTICK_RELOAD_MASK     0x00FFFFFF
#define SRAM_START_ADDRESS      0x20000000

#ifdef	__cplusplus
extern "C" {
#endif

extern const uint32_t zero_ref;

extern uint8_t kernel_init();
extern void start_scheduler(void);
extern uint8_t device_reset();
extern uint8_t user_mpu_enable(uint8_t region, uint32_t address, uint32_t attributes);
extern uint8_t user_mpu_disable(uint8_t region);
extern uint8_t cpu_freq_update(uint32_t frequency);
extern uint8_t base_run(void *args);
extern uint8_t base_release(uint8_t error);
extern uint8_t get_invocation_args(uint32_t *pid, void **args);
extern uint8_t grant_permission(uint32_t pid, uint16_t permission);

#ifdef	__cplusplus
}
#endif

#endif

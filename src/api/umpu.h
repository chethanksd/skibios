#ifndef UMPU_H
#define UMPU_H

#include <stdint.h>
#include <stdbool.h>
#include <arch_util.h>

#ifdef __cplusplus
extern "C" {
#endif

extern uint8_t user_mpu_enable(uint8_t region, uint32_t address, uint32_t attributes);
extern uint8_t user_mpu_disable(uint8_t region);

#ifdef __cplusplus
}
#endif

#endif
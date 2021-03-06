#ifndef UMPU_H
#define UMPU_H

#include <stdint.h>
#include <stdbool.h>
#include <app_support.h>

#ifdef __cplusplus
extern "C" {
#endif

extern uint32_t user_mpu_enable(uint8_t region, uint32_t address, uint32_t attributes);
extern uint32_t user_mpu_disable(uint8_t region);

#ifdef __cplusplus
}
#endif

#endif
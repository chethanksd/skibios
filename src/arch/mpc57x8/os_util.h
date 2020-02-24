#ifndef _OS_UTIL_H_
#define _OS_UTIL_H_

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

extern uint8_t arch_kernel_init();
extern uint8_t arch_mpu_init();
extern void heap_init();

#ifdef __cplusplus
}
#endif

#endif
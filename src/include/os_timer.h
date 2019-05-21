#ifndef _OS_TIMER_
#define _OS_TIMER_

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

extern uint32_t os_timer_init(uint32_t new_cpu_freq);

#ifdef __cplusplus
}
#endif

#endif
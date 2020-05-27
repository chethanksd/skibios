#ifndef SHARE_H
#define SHARE_H

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>


#ifdef	__cplusplus
extern "C" {
#endif

extern uint32_t mutex_lock(uint32_t *mutex);
extern void mutex_unlock(uint32_t *mutex);
extern uint32_t spin_lock(uint32_t *mutex, uint16_t retry);
extern uint32_t semaphore_increment(uint32_t *semaphore, uint16_t retry);
extern uint32_t semaphore_decrement(uint32_t *semaphore, uint16_t retry);

#ifdef	__cplusplus
}
#endif

#endif
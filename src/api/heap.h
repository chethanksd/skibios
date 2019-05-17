#ifndef HEAP_H
#define HEAP_H

#include <stdint.h>
#include <stdbool.h>


#ifdef	__cplusplus
extern "C" {
#endif

extern void heap_init();
extern void* heap_allocate(uint32_t size);
extern uint8_t heap_release(void *ptr);

#ifdef	__cplusplus
}
#endif

#endif
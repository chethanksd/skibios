#ifndef ACCESS_H
#define ACCESS_H

#include <stdint.h>


#define HWREG(x)          (*((volatile uint32_t *)(x)))
#define HWREGH(x)         (*((volatile uint16_t *)(x)))
#define HWREGB(x)         (*((volatile uint8_t *)(x)))
#define HWREGBITW(x, b)   HWREG(((uint32_t)(x) & 0xF0000000) | 0x02000000 | (((uint32_t)(x) & 0x000FFFFF) << 5) | ((b) << 2))
#define HWREGBITH(x, b)   HWREGH(((uint32_t)(x) & 0xF0000000) | 0x02000000 | (((uint32_t)(x) & 0x000FFFFF) << 5) | ((b) << 2))
#define HWREGBITB(x, b)   HWREGB(((uint32_t)(x) & 0xF0000000) | 0x02000000 | (((uint32_t)(x) & 0x000FFFFF) << 5) | ((b) << 2))

#endif
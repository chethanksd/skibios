#ifndef PERMISSIONS_H
#define PERMISSIONS_H

#include <stdint.h>
#include <stdbool.h>


// permission bits
#define PERMISSION_HWREG        5
#define PERMISSION_INTCTRL      6
#define PERMISSION_PROCESS      7
#define PERMISSION_SCHHALT      8

#ifdef __cplusplus
extern "C" {
#endif

extern uint8_t base_run(void *args);
extern uint8_t base_release(uint8_t error);
extern uint8_t get_invocation_args(uint32_t *pid, void **args);
extern uint8_t grant_permission(uint32_t pid, uint16_t permission);

#ifdef __cplusplus
}
#endif

#endif
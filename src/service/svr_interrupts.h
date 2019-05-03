#ifndef SVR_INTERRUPTS_H
#define SVR_INTERRUPTS_H

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern {
#endif

extern uint32_t svc_service_int_register(uint32_t *svc_num, uint32_t *arguments);
extern uint32_t svc_service_int_set_priority(uint32_t *svc_num, uint32_t *arguments);
extern uint32_t svc_service_int_disable(uint32_t *svc_num, uint32_t *arguments);
extern uint32_t svc_service_int_enable(uint32_t *svc_num, uint32_t *arguments);

#ifdef __cplusplus
}
#endif

#endif
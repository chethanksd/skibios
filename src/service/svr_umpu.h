#ifndef SVR_UMPU_H
#define SVR_UMPU_H

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

extern uint32_t svc_service_umpu_enable(uint32_t *svc_num, uint32_t *arguments);
extern uint32_t svc_service_umpu_disable(uint32_t *svc_num, uint32_t *arguments);

#ifdef __cplusplus
}
#endif

#endif
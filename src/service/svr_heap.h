#ifndef SVR_HEAP_H
#define SVR_HEAP_H

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

extern uint32_t svc_service_gheap_allocate(uint32_t *svc_num, uint32_t *arguments);
extern uint32_t svc_service_gheap_release(uint32_t *svc_num, uint32_t *arguments);

#ifdef __cplusplus
}
#endif

#endif
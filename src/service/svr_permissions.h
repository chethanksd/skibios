#ifndef SVR_PERMISSIONS_H
#define SVR_PERMISSIONS_H

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

extern uint32_t svc_service_grant_permission(uint32_t *svc_num, uint32_t *arguments);
extern uint32_t svc_service_release_base(uint32_t *svc_num, uint32_t *arguments);
extern uint32_t svc_service_invoke_base(uint32_t *svc_num, uint32_t *arguments);

#ifdef __cplusplus
}
#endif

#endif
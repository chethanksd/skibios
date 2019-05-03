#ifndef SVR_PROCESS_H
#define SVR_PROCESS_H

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#ifdef __cpluscplus
extern "C" {
#endif

extern uint32_t svc_service_process_kill(uint32_t *svc_num, uint32_t *arguments);
extern uint32_t svc_service_create_process(uint32_t *svc_num, uint32_t *arguments);
extern uint32_t svc_service_hibernate(uint32_t *svc_num, uint32_t *arguments);
extern uint32_t svc_service_priority_demote(uint32_t *svc_num, uint32_t *arguments);
extern uint32_t svc_service_priority_promote(uint32_t *svc_num, uint32_t *arguments);

#ifdef __cpluscplus
}
#endif

#endif
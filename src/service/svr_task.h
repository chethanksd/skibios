#ifndef _SVR_TASK_H_
#define _SVR_TASK_H_

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#ifdef __cpluscplus
extern "C" {
#endif

extern uint32_t svc_service_task_kill(uint32_t *svc_num, uint32_t *arguments);
extern uint32_t svc_service_task_create(uint32_t *svc_num, uint32_t *arguments);
extern uint32_t svc_service_hibernate(uint32_t *svc_num, uint32_t *arguments);
extern uint32_t svc_service_priority_demote(uint32_t *svc_num, uint32_t *arguments);
extern uint32_t svc_service_priority_promote(uint32_t *svc_num, uint32_t *arguments);

#ifdef __cpluscplus
}
#endif

#endif
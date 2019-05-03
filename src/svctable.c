/*
 * SVC Dispatch Table
 *
 * author: chetandev.ksd@gmail.com
 *
 */

#include <stdlib.h>
#include <stdint.h>

#include <svctable.h>
#include <svc.h>

#include <svr_permissions.h>
#include <svr_interrupts.h>
#include <svr_process.h>
#include <svr_umpu.h>

// extern svc service functions
extern uint32_t svc_service_device_reset(uint32_t *svc_num, uint32_t *arguments);
extern uint32_t svc_service_hand_over(uint32_t *svc_num, uint32_t *arguments);
extern uint32_t svc_service_cpu_freq_update(uint32_t *svc_num, uint32_t *arguments);
extern uint32_t svc_service_gheap_allocate(uint32_t *svc_num, uint32_t *arguments);
extern uint32_t svc_service_gheap_release(uint32_t *svc_num, uint32_t *arguments);
extern uint32_t svc_service_start_scheduler(uint32_t *svc_num, uint32_t *arguments);


const svc_dispatch_table_t svc_dispatch[] = {
    {.svc_code = HAND_OVER,         .svc_service = svc_service_hand_over        },
    {.svc_code = HIBERNATE,         .svc_service = svc_service_hibernate        },
    {.svc_code = PRIORITY_PROMOTE,  .svc_service = svc_service_priority_promote },
    {.svc_code = PRIORITY_DEMOTE,   .svc_service = svc_service_priority_demote  },
    {.svc_code = GHEAP_ALLOCATE,    .svc_service = svc_service_gheap_allocate   },
    {.svc_code = GHEAP_RELEASE,     .svc_service = svc_service_gheap_release    },
    {.svc_code = CREATE_PROCESS,    .svc_service = svc_service_create_process   },
    {.svc_code = KILL_PROCESS,      .svc_service = svc_service_process_kill     },
    {.svc_code = INVOKE_BASE,       .svc_service = svc_service_invoke_base      },
    {.svc_code = RELEASE_BASE,      .svc_service = svc_service_release_base     },
    {.svc_code = GRANT_PERMISSION,  .svc_service = svc_service_grant_permission },
    {.svc_code = INT_ENABLE,        .svc_service = svc_service_int_enable       },
    {.svc_code = INT_DISABLE,       .svc_service = svc_service_int_disable      },
    {.svc_code = INT_REGISTER,      .svc_service = svc_service_int_register     },
    {.svc_code = SET_PRIORITY,      .svc_service = svc_service_int_set_priority },
    {.svc_code = ENABLE_UMPU,       .svc_service = svc_service_umpu_enable      },
    {.svc_code = DISABLE_UMPU,      .svc_service = svc_service_umpu_disable     },
    {.svc_code = CPU_FREQ,          .svc_service = svc_service_cpu_freq_update  },
    {.svc_code = START_SCHEDULER,   .svc_service = svc_service_start_scheduler  },
    {.svc_code = DEVICE_RESET,      .svc_service = svc_service_device_reset     },
    {.svc_code = 0,                 .svc_service = 0                            }
};


const uint32_t TOTAL_SVC_COUNT = sizeof(svc_dispatch) / sizeof(svc_dispatch_table_t);

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

// extern svc service functions
uint32_t svc_service_device_reset(uint32_t *svc_num, uint32_t *arguments);
uint32_t svc_service_hand_over(uint32_t *svc_num, uint32_t *arguments);

svc_dispatch_table_t svc_dispatch[] = {
    {.svc_code = DEVICE_RESET,      .svc_service = svc_service_device_reset },
    {.svc_code = HAND_OVER,         .svc_service = svc_service_hand_over    },
    {.svc_code = 0,                 .svc_service = 0                        }
};


const uint32_t TOTAL_SVC_COUNT = sizeof(svc_dispatch) / sizeof(svc_dispatch_table_t);

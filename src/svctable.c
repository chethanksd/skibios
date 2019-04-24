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

uint32_t svc_service_device_reset(uint32_t *svc_num, uint32_t *arguments);

svc_dispatch_table_t svc_dispatch[] = {
    {.svc_code = DEVICE_RESET,      .svc_service = svc_service_device_reset},
    {.svc_code = ENABLE_UMPU,       .svc_service = svc_service_device_reset}   
};


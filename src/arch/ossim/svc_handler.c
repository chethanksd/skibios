/*
 * os util code for arm-m3m4
 *
 *
 * author: chetandev.ksd@gmail.com
 *
 */


#include <os_util.h>
#include <error.h>
#include <param.h>
#include <kvar.h>
#include <arch_kvar.h>
#include <interrupt.h>
#include <os_support.h>
#include <svctable.h>

#include <stdio.h>

// local function declaration
uint32_t svc_handler(uint32_t svc_code, uint32_t *arguments);

// extern variables
const uint32_t TOTAL_SVC_COUNT;

// local variables
static uint32_t (*svc_service)(uint32_t *svc_code, uint32_t *arguments);


uint32_t call_kernel_service(uint32_t svc_code, uint32_t arg1, uint32_t arg2, uint32_t arg3, uint32_t arg4) {

    uint32_t argument[4];
    uint32_t error;

    // prepare argumnent array for kernel service handler
    argument[0] = arg1;
    argument[1] = arg2;
    argument[3] = arg3;
    argument[4] = arg4;

    // call kernel service handler
    error = svc_handler(svc_code, &argument[0]);

    return error;

}

uint32_t svc_handler(uint32_t svc_code, uint32_t *arguments) {

    uint32_t svc_stash;
    uint32_t svc_current;
    uint8_t error;
    uint32_t i;

    svc_current = svc_code;

    do {

        svc_stash = svc_current;

        for(i = 0 ; i < TOTAL_SVC_COUNT; i++) {
            if(svc_dispatch[i].svc_code == svc_current){
                svc_service = svc_dispatch[i].svc_service;
                break;
            }
        }

        // ran through all table and found no svc code match
        // maybe svc_current is 0, so break the loop
        if(i >= TOTAL_SVC_COUNT) {
            break;
        }

        error = (*svc_service)(&svc_current, arguments);

    } while(svc_stash != svc_current);

    return error;

}
/*
 * umpu kernel service code
 *
 * author: chetandev.ksd@gmail.com
 *
 */

#include <error.h>
#include <param.h>
#include <svc.h>
#include <kvar.h>
#include <svr_umpu.h>
#include <os_support.h>
#include <regmap.h>

uint32_t svc_service_umpu_disable(uint32_t *svc_num, uint32_t *arguments) {

    uint32_t error = ERROR_NONE;
    uint32_t region;

    region = arguments[0];

    // Check if MPU Hardware is supported
    if(HWREG(MPUTYPE) == 0) {

        error = ERROR_MPU_UNSUPPORTED;
        goto quit_error;

    }

    // check Region value passed is valid
    if(region < 2 || region > 5) {

        error = ERROR_INVALID_MPU_REGION;
        goto quit_error;

    }

    // Make sure that pending memory transfers are done
    __asm("DMB");

    // Disable the MPU
    HWREG(MPUCTRL) = 0;

    // Setup User MPU Region
    HWREG(MPURNR) = region;
    HWREG(MPURBAR) = 0;
    HWREG(MPURASR) = 0;

    HWREG(MPURNR) = 0;

    // Enable MPU and the Background Region
    HWREG(MPUCTRL) = MPU_PRIVDEFENA | MPU_ENABLE;

    __asm("DSB");

    __asm("ISB");

quit_error:

    return error;

}

uint32_t svc_service_umpu_enable(uint32_t *svc_num, uint32_t *arguments) {

    uint32_t error = ERROR_NONE;
    uint32_t region;
    uint32_t address;
    uint32_t attributes;

    region = arguments[0];
    address = arguments[1];
    attributes = arguments[2];

    // Check if MPU Hardware is supported
    if(HWREG(MPUTYPE) == 0){

        error = ERROR_MPU_UNSUPPORTED ;
        goto quit_error;

    }

    // check Region value passed is valid
    if(region < 2 || region > 5) {

        error = ERROR_INVALID_MPU_REGION;
        goto quit_error;

    }

    if((address & 0x0000001F) != 0) {

        error = ERROR_INVALID_MPU_ADDRESS;
        goto quit_error;

    }

    // Make sure that pending memory transfers are done
    __asm("DMB");

    // Disable the MPU
    HWREG(MPUCTRL) = 0;

    // Setup User MPU Region
    HWREG(MPURNR) = region;
    HWREG(MPURBAR) = address;
    HWREG(MPURASR) = attributes | MPU_REGION_ENABLE;

    HWREG(MPURNR) = 0;

    // Enable MPU and the Background Region
    HWREG(MPUCTRL) = MPU_PRIVDEFENA | MPU_ENABLE;

    __asm("DSB");

    __asm("ISB");

quit_error:
    
    return error;

}


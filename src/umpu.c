/*
 * user defined mpu region code
 *
 * author: chetandev.ksd@gmail.com
 *
 */

#include <os_support.h>
#include <access.h>
#include <error.h>
#include <kernel.h>
#include <kvar.h>
#include <umpu.h>


uint8_t user_mpu_enable(uint8_t region, uint32_t address, uint32_t attributes) {

    uint8_t error = 0;

    region = region + 3;

    SVC_ENABLE_UMPU(region, address, attributes);

    GET_SVC_RETURN_CODE(error);

    return error;

}

uint8_t user_mpu_disable(uint8_t region) {

    uint8_t error = 0;

    region = region + 3;

    SVC_DISABLE_UMPU(region);

    GET_SVC_RETURN_CODE(error);

    return error;


}




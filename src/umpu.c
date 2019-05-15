/*
 * user defined mpu region code
 *
 * author: chetandev.ksd@gmail.com
 *
 */

#include <mpu.h>
#include <access.h>
#include <error.h>
#include <kernel.h>
#include <kvar.h>
#include <umpu.h>


uint8_t user_mpu_enable(uint8_t region, uint32_t address, uint32_t attributes) {

    uint8_t error = 0;

    region = region + 3;

    __asm volatile (" LDRB R0, %[region] \n"
                    " LDR  R1, %[addr]   \n"
                    " LDR  R2, %[attr]   \n"
            :
            : [region] "m" (region), [addr] "m" (address), [attr] "m" (attributes)
            :
    );

    svc(ENABLE_UMPU);

    __asm volatile (" STR R0, %[err] \n"
        : [err] "=m" (error)
        :
        :
    );

    return error;

}

uint8_t user_mpu_disable(uint8_t region) {

    uint8_t error = 0;

    region = region + 3;

    __asm volatile (" LDRB R0, %[region] \n"
            :
            : [region] "m" (region)
            :
    );

    svc(DISABLE_UMPU);

    __asm volatile (" STR R0, %[err] \n"
        : [err] "=m" (error)
        :
        :
    );

    return error;


}




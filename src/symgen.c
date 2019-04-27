/*
 * ASSEMBLY symbol generator input file
 *
 * author: chetandev.ksd@gmail.com
 *
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

#include <svctable.h>

extern const svc_dispatch_table_t svc_dispatch[];

void symbol_generator() {

    OFFSETOF(SDT_SVC_CODE, svc_dispatch_table_t, svc_code);
    OFFSETOF(SDT_SVC_SERVICE, svc_dispatch_table_t, svc_service);
    SIZEOF(SDT, svc_dispatch_table_t);
    
}
#ifndef SVCTABLE_H

#include <stdint.h>


#define DEFINE(sym, val) \
    __asm ("\n-> " #sym " %0 " #val "\n" : : "i" (val))

#define OFFSETOF(n, s, m) \
    DEFINE(OFFSET_OF_##n, offsetof(s, m));

#define SIZEOF(n, s) \
    DEFINE(SIZE_OF_##n, sizeof(s));


#ifdef __cplusplus
extern "C" {
#endif


typedef struct {
    uint32_t svc_code;
    uint32_t (*svc_service)(uint32_t *arguments);
}svc_dispatch_table_t;


extern svc_dispatch_table_t svc_dispatch;


#ifdef __cplusplus
}
#endif

#endif
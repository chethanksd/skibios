#ifndef _APP_SUPPORT_H_
#define _APP_SUPPORT_H_

#include <mpu.h>


// inline assembler helper directive: call SVC with the given immediate
#define svc(code) __asm("svc %[immediate]"::[immediate] "I" (code));

#endif
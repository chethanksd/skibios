#ifndef _APP_SUPPORT_H_
#define _APP_SUPPORT_H_

#define USER_MPU_0              0
#define USER_MPU_1              1
#define USER_MPU_2              2

// inline assembler helper directive: call SVC with the given immediate
#define svc(code) __asm("svc %[immediate]"::[immediate] "I" (code));

#endif
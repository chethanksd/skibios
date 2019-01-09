#ifndef SVC_H
#define SVC_H

// inline assembler helper directive: call SVC with the given immediate
#define svc(code) __asm("svc %[immediate]"::[immediate] "I" (code));

// stack push iniline assembly code
#define svc_push(arg) __asm(" PUSH  {%[pushval]}            \n" \
                            :                                   \
                            :[pushval] "r" (arg)                \
                            :                                   \
                        );

#define KSECTION(str) __attribute__ ((section ("" #str "")))

// SVCall service codes
#define CREATE_PROCESS      10
#define HAND_OVER           11
#define KILL_PROCESS        12
#define HIBERNATE           13
#define ENABLE_DWS          14
#define DISABLE_DWS         15
#define PRIORITY_PROMOTE    16
#define PRIORITY_DEMOTE     17
#define INVOKE_BASE         18
#define RELEASE_BASE        19
#define START_SCHEDULER     20
#define INT_ENABLE          21
#define INT_DISABLE         22
#define SET_PRIORITY        23
#define INT_REGISTER        24
#define DEVICE_RESET        40
#define ENABLE_UMPU         41
#define DISABLE_UMPU        42
#define CPU_FREQ            43
#define GRANT_PERMISSION    44
#define GHEAP_ALLOCATE      60
#define GHEAP_RELEASE       61

#endif
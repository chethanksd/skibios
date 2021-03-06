#ifndef _SVC_H_
#define _SVC_H_

#ifdef SVC_VERSION_MAJOR
#error "SVC_VERSION_MAJOR is defined elsewhere in the program"
#endif

#ifdef SVC_VERSION_MINOR
#error "SVC_VERSION_MINOR is defined elsewhere in the program"
#endif

// SVC Version
#define SVC_VERSION_MAJOR   1
#define SVC_VERSION_MINOR   1

// SVCall service codes
#define CREATE_TASK         10
#define HAND_OVER           11
#define KILL_TASK           12
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
#define HWREG_WRITE         45
#define HWREG_READ          46
#define GHEAP_ALLOCATE      60
#define GHEAP_RELEASE       61

#endif
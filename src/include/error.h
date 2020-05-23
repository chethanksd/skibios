#ifndef SKIBIOS_ERROR_H
#define SKIBIOS_ERROR_H

#define ERROR_NONE                               0
#define ERROR_SYSTICK_TICK_VALUE_OVERFLOW        1
#define ERROR_OUT_OF_MEMORY                      2
#define ERROR_RESOURCE_LOCKED_INDEFINETLY        3
#define ERROR_MAX_PROCESS_COUNT                  4
#define ERROR_PROCESS_ALREADY_RUNNING            5
#define ERROR_PROCESS_IS_IDLE                    6
#define ERROR_SINGLE_PROCESS                     7
#define ERROR_WRONG_HEAP_POINTER                 8
#define ERROR_EMPTY_HEAP                         9
#define ERROR_FULL_HEAP                          10
#define ERROR_INSUFFICIENT_PERMISSION            11
#define ERROR_MPU_UNSUPPORTED                    12
#define ERROR_INVALID_TASK_PRIORITY              13
#define ERROR_INVALID_MPU_REGION                 14
#define ERROR_INVALID_MPU_ADDRESS                15
#define ERROR_BASE_PROCESS                       16
#define ERROR_NO_INVOCATION                      17
#define ERROR_UNKNOWN_PROCESS_ID                 18

// useful constants
#define NULL_POINTER            0x00000000


#endif
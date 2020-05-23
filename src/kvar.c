/*
 * kernel variable file
 *
 * author: chetandev.ksd@gmail.com
 *
 */

#include <stdint.h>
#include <kvar.h>

#ifndef UPC_RESOURCE_CAL
#include <param.h>
#endif

volatile uint32_t  KSECTION(.kdat) current_task             = 0;
volatile uint32_t  KSECTION(.kdat) next_task                = 0;
volatile uint8_t   KSECTION(.kdat) max_level                = 0;
volatile uint8_t   KSECTION(.kdat) maxp_level               = 0;
volatile uint8_t   KSECTION(.kdat) alc                      = 0;
volatile uint8_t   KSECTION(.kdat) hlc                      = 0;
volatile uint8_t   KSECTION(.kdat) alcp                     = 0;
volatile uint8_t   KSECTION(.kdat) hlcp                     = 0;
volatile uint16_t  KSECTION(.kdat) task_count               = 0;
volatile uint16_t  KSECTION(.kdat) total_task_count         = 0;
volatile uint8_t   KSECTION(.kdat) lstash_ptr               = 1;

volatile uint32_t  KSECTION(.kbss) pstack_addr;
volatile uint32_t  KSECTION(.kbss) cpu_freq;

volatile task_t    KSECTION(.kbss) *task_obj[MAX_PROCESS_COUNT];
volatile uint32_t  KSECTION(.kbss) PSP_Array[MAX_PROCESS_COUNT];
volatile uint8_t   KSECTION(.kbss) priority_Array[MAX_PROCESS_COUNT][2];
volatile uint32_t  KSECTION(.kbss) mutex_stash[MAX_PROCESS_COUNT];
volatile uint8_t   KSECTION(.kbss) level_stash[MAX_PROCESS_COUNT];
volatile int8_t    KSECTION(.kbss) jmp_list[MAX_PROCESS_COUNT];
volatile uint8_t   KSECTION(.kbss) state[MAX_PROCESS_COUNT];
volatile uint32_t  KSECTION(.kbss) task_id[MAX_PROCESS_COUNT];
volatile uint16_t  KSECTION(.kbss) permissions[MAX_PROCESS_COUNT];

volatile uint32_t  KSECTION(.kbss) *op1[MAX_PROCESS_COUNT];
volatile uint32_t  KSECTION(.kbss) *op2[MAX_PROCESS_COUNT];
volatile uint32_t  KSECTION(.kbss) hib_value[MAX_PROCESS_COUNT];

task_t    KSECTION(.kbss) base_task;
uint32_t  KSECTION(.kbss) invocated_task;
uint32_t  KSECTION(.kbss) invocated_args;

bool KSECTION(.kdat) self_kill      = false;
bool KSECTION(.kdat) enable_dws     = true;
bool KSECTION(.kdat) first_start    = false;
bool KSECTION(.kdat) normal_schedule = true;


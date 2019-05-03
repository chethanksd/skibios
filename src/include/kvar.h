#ifndef KVAR_H
#define KVAR_H

#include <process.h>

extern const uint32_t g_pui32Priority[];
extern const uint32_t prioreg[];
extern const uint32_t g_pui32Dii16Regs[];
extern const uint32_t g_pui32EnRegs[];

extern volatile uint32_t current_task;
extern volatile uint32_t next_task;
extern volatile uint8_t  max_level;
extern volatile uint8_t  maxp_level;
extern volatile uint8_t  alc;
extern volatile uint8_t  hlc;
extern volatile uint8_t  alcp;
extern volatile uint8_t  hlcp;
extern volatile uint16_t process_count;
extern volatile uint16_t total_process_count;
extern volatile uint8_t  lstash_ptr;

extern volatile uint32_t  pstack_addr;
extern volatile uint32_t  cpu_freq;

extern volatile Process  *proc_obj[];
extern volatile uint32_t PSP_Array[];
extern volatile uint8_t  priority_Array[][2];
extern volatile uint32_t mutex_stash[];
extern volatile uint8_t  level_stash[];
extern volatile int8_t   jmp_list[];
extern volatile uint8_t  state[];
extern volatile uint32_t process_id[];
extern volatile uint16_t permissions[];

extern volatile uint32_t *op1[];
extern volatile uint32_t *op2[];
extern volatile uint32_t hib_value[];

extern Process  base_task;
extern uint32_t invocated_task;
extern uint32_t invocated_args;

extern bool self_kill;
extern bool enable_dws;
extern bool first_start;
extern bool normal_schedule;

#endif
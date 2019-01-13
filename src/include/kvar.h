#ifndef KVAR_H
#define KVAR_H

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

#endif
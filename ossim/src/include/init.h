#ifndef _INIT_H_
#define _INIT_H_

#ifdef	__cplusplus
extern "C" {
#endif

extern uint32_t adapter_init();
extern uint32_t get_self_path(char *path_buffer, uint32_t size);
extern void debugf(const char* format, ...);
extern uint32_t write_slot(char *message);

#ifdef	__cplusplus
}
#endif

#endif
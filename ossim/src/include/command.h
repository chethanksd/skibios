#ifndef _COMMAND_H_
#define _COMMAND_H_


#define MAX_COMMAND_LINE_SIZE       102
#define MAX_COMMAND_SIZE            50
#define MAX_ARGUMENT_SIZE           50

#ifdef	__cplusplus
extern "C" {
#endif

extern uint32_t run_command_processor();

#ifdef	__cplusplus
}
#endif

#endif
/*
 * kernel variable file
 *
 * author: chetandev.ksd@gmail.com
 *
 */

#include <stdint.h>
#include <kvar.h>
#include <svc.h>

volatile uint32_t  KSECTION(.kdat) current_task             = 0;
volatile uint32_t  KSECTION(.kdat) next_task                = 0;
volatile uint8_t   KSECTION(.kdat) max_level                = 0;
volatile uint8_t   KSECTION(.kdat) maxp_level               = 0;
volatile uint8_t   KSECTION(.kdat) alc                      = 0;
volatile uint8_t   KSECTION(.kdat) hlc                      = 0;
volatile uint8_t   KSECTION(.kdat) alcp                     = 0;
volatile uint8_t   KSECTION(.kdat) hlcp                     = 0;
volatile uint16_t  KSECTION(.kdat) process_count            = 0;
volatile uint16_t  KSECTION(.kdat) total_process_count      = 0;
volatile uint8_t   KSECTION(.kdat) lstash_ptr               = 1;

volatile uint32_t  KSECTION(.kbss) pstack_addr;
volatile uint32_t  KSECTION(.kbss) cpu_freq;
volatile uint16_t  KSECTION(.kbss) i, j;


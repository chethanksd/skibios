/*
 * arch specific kernel variable file
 *
 * author: chetandev.ksd@gmail.com
 *
 */

#include <stdint.h>
#include <arch_kvar.h>

#ifndef UPC_RESOURCE_CAL
#include <param.h>
#endif


volatile uint32_t  KSECTION(.kbss) kernel_sp;
volatile uint32_t  KSECTION(.kbss) task_sp;
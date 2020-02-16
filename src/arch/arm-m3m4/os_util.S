/* 
 * svc handler code for arm m3/m4
 *
 * author: chetandev.ksd@gmail.com
 *
 */

#include <svc.h>


.text
.syntax unified
.thumb

.extern PSP_Array

.type switch_mcu_mode, %function
.global switch_mcu_mode


switch_mcu_mode:
        LDR         R3, =PSP_Array    
        LDR         R0, [R3] 
        MSR         PSP, R0
        MOV         R0, #3
    	MSR         CONTROL, R0
    	ISB

        // Call kernel service START_SCHEDULER
        SVC         START_SCHEDULER

        // code will never reach here
        BX          LR




// at start current_stask is set as 0, which is always base_task
// start_process prepares entire stack. During the first time context switch
// between base_task -> high_priority task, the pendsv will write {R2-R11} on 
// top of already prepared stack signature for base_task if PSP value for base_task
// is kept as it is. So, we will move PSP value for base_task {R1-R11} below, so
// that pendsv can rewrite the {R1-R11} base_task stack signature

.type btask_psp_correction, %function
.global btask_psp_correction

btask_psp_correction:
        LDR         R3, =PSP_Array
        LDR         R0, [R3]
        ADD         R0, R0, #40
        MSR         PSP, R0
        BX          LR

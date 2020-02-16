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
        MSR         PSP,R0
        MOV         R0, #3
    	MSR         CONTROL, R0
    	ISB

        // Call kernel service START_SCHEDULER
        SVC         START_SCHEDULER

        // code will never reach here
        BX          LR
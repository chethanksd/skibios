/* 
 * pendsv handler for arm m3/m4
 *
 * author: chetandev.ksd@gmail.com
 *
 */

#include <svc.h>
#include "symgen.h"

.text
.syntax unified
.thumb

.extern current_task
.extern next_task
.extern PSP_Array
.extern mpu_table

.type pendsv_handler, %function
.global pendsv_handler


pendsv_handler:

        MRS         R0, PSP
        TST         LR, #0x10          
        IT          EQ                  
        VSTMDBEQ    R0!, {S16-S31}
        MOV         R2, LR             
        MRS         R3, CONTROL        
        STMDB       R0!,{R2-R11}     
    	LDR         R1, =current_task	   
        LDR         R2, [R1]            
        LDR         R3, =PSP_Array    
        STR         R0,[ R3,R2, LSL#2]  
        LDR         R4, =next_task     
    	LDR         R4, [R4]			   
        STR         R4, [R1]            
        LDR         R0, [R3,R4, LSL#2]

        //Disbale MPU
        LDR         R1, =0xE000ED94               
        MOV         R2, #0             
        STR         R2, [R1]

        //Load Next Process MPU settings from mpu_table (Assum R4 = nx_task)
        LDR         R2, =0xE000ED9C               
        LDR         R4, =next_task     
        LDR         R4, [R4]		
        LDR         R3, =mpu_table 
        ADD         R3, R3, R4, LSL #3 
        LDM         R3, {R4-R5}        
        STM         R2, {R4-R5}

        //Enable MPU (and Background Region)
        MOV         R2, #5                        
        STR         R2, [R1]

        LDMIA       R0!, {R2-R11}     
        MOV         LR, R2             
        MSR         CONTROL, R3         
        TST         LR, #0x10           
        IT          EQ                  
        VLDMIAEQ    R0!, {S16-S31} 
        MSR         PSP, R0            
    	DSB					   
        ISB                    
    	BX LR				
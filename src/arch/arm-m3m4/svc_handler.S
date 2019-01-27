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

SVC_CODE .req R0
ADDR .req R1

.extern arg1
.extern arg2
.extern arg3
.extern arg4

.extern process_svc_request

.type svc_handler, %function
.global svc_handler


svc_handler:

        // get stack pointer
        TST    LR, #4                     
        ITE    EQ                        
        MRSEQ  ADDR, MSP             
        MRSNE  ADDR, PSP

        // get svc code
        LDR    SVC_CODE, [ADDR, #24]       
        LDRB   SVC_CODE, [SVC_CODE, #-2]
        PUSH   {R6, R7}

        // get argument1
        LDR    R7, =arg1      
        LDR    R6, [ADDR, #0]        
        STR    R6, [R7] 

        // get argument2
        LDR    R7, =arg2
        LDR    R6, [ADDR, #4]        
        STR    R6, [R7]    

        // get argument3
        LDR    R7, =arg3
        LDR    R6, [ADDR, #8]        
        STR    R6, [R7]    

        // get argument4
        LDR    R7, =arg4
        LDR    R6, [ADDR, #12]       
        STR    R6, [R7]

        // store LR to exc_return for temporary storage
        MOV    R7, SP
        SUB    R7, R7, #4
        MOV    SP, R7
        STR    LR, [R7, #0]
        PUSH   {R7}
                  
        BL     process_svc_request        

        // restore EXC_RETURN value
        POP       {R7}
        LDR       LR, [R7, #0]
        ADD       R7, R7, #4
        MOV       SP, R7
        DMB

        // return value to caller function
        TST       LR, #4                
        ITE       EQ                    
        MRSEQ     ADDR, MSP         
        MRSNE     ADDR, PSP
        LDR       R7, =arg1
        LDR       R6, [R7]           
        STR       R6, [ADDR, #8]    

        POP       {R6, R7}
        BX        LR		            

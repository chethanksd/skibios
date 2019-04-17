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

SVC_STASH       .req r3
SVC_CODE        .req R2
ADDR            .req R1

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

        // SVC_STASH = SVC_CODE
        MOV SVC_STASH, SVC_CODE

        // store LR to exc_return in stack
        // store svc_code in stack
        // pass pointer where svc_code is stored
        MOV    R7, SP
        SUB    R7, R7, #8
        MOV    SP, R7

        STR    LR, [R7, #4]
        STR    SVC_CODE, [R7, #0]
        MOV    R0, R7   

        BL     process_svc_request 

        // check if hand_over service needs to be executed
        LDR     SVC_CODE, [R7, #0]
        CMP     SVC_CODE, SVC_STASH
        IT      EQ
        BEQ     skip_check

        CMP     SVC_CODE, HAND_OVER
        IT      NE 
        BNE     skip_check

        PUSH    {R0}
        BL      svc_service_hand_over
        POP     {R0}

skip_check:
        // restore EXC_RETURN value
        LDR     LR, [R7, #4]
        ADD     R7, R7, #8
        MOV     SP, R7
        DMB

        // return value to caller function
        TST       LR, #4                
        ITE       EQ                    
        MRSEQ     ADDR, MSP         
        MRSNE     ADDR, PSP          
        STR       R0, [ADDR, #8]    

        POP       {R6, R7}
        BX        LR		            

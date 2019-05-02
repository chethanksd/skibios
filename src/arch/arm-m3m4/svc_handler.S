/* 
 * svc handler code for arm m3/m4
 *
 * author: chetandev.ksd@gmail.com
 *
 */

#include <svc.h>
#include "symgen.h"

.text
.syntax unified
.thumb

SVC_STASH       .req r3
SVC_CODE        .req R2
ADDR            .req R1

.extern process_svc_request
.extern svc_dispatch

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

        PUSH   {R4, R5, R6, R7}

        // store LR to exc_return in stack
        // store svc_code in stack
        // pass pointer where svc_code is stored
        MOV    R7, SP
        SUB    R7, R7, #8
        MOV    SP, R7

        STR    LR, [R7, #4]
        STR    SVC_CODE, [R7, #0]

svc_cycle:

        // SVC_STASH = SVC_CODE
        MOV    SVC_STASH, SVC_CODE
        MOV    R0, R7
        LDR    R4, =svc_dispatch

svc_again:
        LDR    R5, [R4, OFFSET_OF_SDT_SVC_CODE]
        CMP    R5, #0
        IT     EQ
        BEQ    skip_check

        CMP    SVC_CODE, R5
        ITEE   EQ
        LDREQ  R5, [R4, OFFSET_OF_SDT_SVC_SERVICE]
        ADDNE  R4, R4, SIZE_OF_SDT
        BNE    svc_again

        PUSH   {R1, R3}
        BLX    R5
        POP    {R1, R3}
    
        LDR    SVC_CODE, [R7, #0]
        CMP    SVC_CODE, SVC_STASH
        IT     EQ
        BEQ    skip_check

        B      svc_cycle       

skip_check:

        // restore EXC_RETURN value
        LDR     LR, [R7, #4]
        ADD     R7, R7, #8
        MOV     SP, R7
        DMB

        POP       {R4, R5, R6, R7}

        // return value to caller function
        TST       LR, #4                
        ITE       EQ                    
        MRSEQ     ADDR, MSP         
        MRSNE     ADDR, PSP          
        STR       R0, [ADDR, #0]


        BX        LR		            

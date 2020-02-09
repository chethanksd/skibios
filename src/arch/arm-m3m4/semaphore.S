/* 
 * semaphore code
 *
 * author: chetandev.ksd@gmail.com
 *
 */
#include <svc.h>
#include "symgen.h"

.text
.syntax unified
.thumb

.type arch_semaphore_inc, %function
.global arch_semaphore_inc

arch_semaphore_inc:
            PUSH        {R1, R2}
            LDREXB      R1, [R0]   
            ADD		    R1, #1	
            STREXB	    R2, R1, [R0]  
            CMP    	    R2, #0      
            ITE		    EQ		
            MOVEQ 	    R0, #0  
            MOVNE 	    R0, #1   
            DMB
            POP         {R1, R2}      			
            BX          LR



.type arch_semaphore_dec, %function
.global arch_semaphore_dec

arch_semaphore_dec:
            PUSH        {R0, R1}
        	LDREXB  	R1, [R0]   
            CMP    		R1, #0     
            ITEET		EQ		
            MOVEQ 		R0, #0  
            SUBNE		R1, #1	
            STREXBNE	R2, R1, [R0]  
            BEQ    		dec_exit       
            CMP    		R2, #0      
        	ITE			EQ		
            MOVEQ 		R0, #0   
            MOVNE 		R0, #1   
dec_exit:	DMB     
            POP         {R1, R2}
            BX          LR

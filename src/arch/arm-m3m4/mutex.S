/* 
 * mutex lock code
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
.extern process_id
.extern mutex_stash

.type mutex_lock, %function
.global mutex_lock

mutex_lock:

            // check if the mutex is locked?
            LDREXB      R2, [R0]           
            CMP         R2, #0                     
            ITT         NE                      
            MOVNE       R0, #1       
            BNE         lock_exit  

            // get current_task
            LDR         R1, =current_task           
            LDR         R1, [R1]  

            // get process_id of current_task
            LDR         R2, =process_id              
            LDR         R1, [R2, R1, LSL#2]

            // attempt to lock the mutex
            STREX       R2, R1, [R0]             

            // check if locking has been succesful  
            CMP         R2, #1    
            ITE         EQ                      
            MOVEQ       R0, #1       
            MOVNE       R0, #0 

lock_exit:  DMB 
            BX          LR



.type mutex_unlock, %function
.global mutex_unlock

mutex_unlock:
            MOV         R1, #0         
            DMB                  
            STRB        R1, [R0]
       
            // get current_task
            LDR         R2, =current_task
            LDR         R2, [R2]

            // get stashed mutex content of current task
            LDR         R1, =mutex_stash
            LDR         R1, [R1, R2, LSL #2]

            // check if stashed mutex is equal to mutex to be unlocked
            CMP         R1, R0
            IT          EQ
            SVCEQ       PRIORITY_DEMOTE

            BX          LR

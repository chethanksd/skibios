/*
 * mutex lock and unlock source code for MPC5
 *
 * author: chetandev.ksd@gmail.com
 *
 * Note: stwcx instruction is not available for MPC5 devices. Executing this 
 * instruction will yield illegal instruction exception. lharx is available
 * in these devices. lharx provides same operation as lwarx but on a half word 
 * data type. Since skibios uses 4byte mutex locks, we need to ensure atomicity
 * of mutex  locking using lharx - sthcx on 4 bytes mutex lock. We only need 
 * to ensure that LSB part of mutex is locked using lharx - sthcx pair. Once the 
 * LSB part is sucessfully locked. We can perform simple write on MSB part.
 *
 */



.section .text, "ax"

.extern current_task
.extern process_id
.extern mutex_stash

.type mutex_lock, %function
.globl    mutex_lock
.align 4

mutex_lock:
       
       // prologue
       e_addi   r1, r1, -12
       se_stw   r4, 0(r1)
       se_stw   r5, 4(r1)
       se_stw   r6, 8(r1)

       // assume locking of mutex is unsucessful
       e_li     r5, 1

       // check if mutex is already locked?
       e_addi   r3, r3, 2
       lharx    r6, 0, r3
       e_lis    r4, 0
       cmpw     r6, r4
       e_bne    exit_lock

       // attempt locking the mutex
       e_lis    r6, current_task@h
       e_or2i   r6, current_task@l
       se_lwz   r4, 0(r6)
       e_slwi   r4, r4, 2

       e_lis    r6, process_id@h
       e_or2i   r6, process_id@l
       e_addi   r6, r6, 2
       lhax     r6, r6, r4

       sthcx.   r6, 0, r3
       e_bne    exit_lock

       // locking is sucessful
       // write remaining msb
       e_lis    r6, process_id@h
       e_or2i   r6, process_id@l
       lhax     r6, r6, r4

       e_addi   r3, r3, -2
       se_sth   r6, 0(r3)
       e_lis    r5, 0

exit_lock:

       // epilogue
       msync
       e_addi   r3, r5, 0
       e_lwz    r6, 8(r1)
       e_lwz    r5, 4(r1)
       e_lwz    r4, 0(r1)
       e_addi   r1, r1, 12
       se_blr





.type mutex_unlock, %function
.globl    mutex_unlock
.align 4

mutex_unlock:

       // prologue
       e_addi   r1, r1, -16
       se_stw   r4, 0(r1)
       se_stw   r5, 4(r1)
       se_stw   r6, 8(r1)
       se_stw   r7, 12(r1)

       // store temporary copy of mutex value
       se_lwz   r7, 0(r3)

       // release the mutex
       e_li     r4, 0
       msync
       se_stw   r4, 0(r3)

       // get current task
       e_lis    r4, current_task@h
       e_or2i   r4, current_task@l
       se_lwz   r5, 0(r4)
       e_slwi   r5, r5, 2

       // get stashed mutex
       e_lis    r4, mutex_stash@h
       e_or2i   r4, mutex_stash@l
       lx    	  r6, r4, r5

       // check if stashed and mutex to unlocked are same?
       cmpw     r6, r7
       e_bne    exit_unlock

       // ToDo: priority_demote() kernel service call

exit_unlock:
       // epilogue
       e_lwz    r7, 12(r1)
       e_lwz    r6, 8(r1) 
       e_lwz    r5, 4(r1)
       e_lwz    r4, 0(r1)      
       e_addi   r1, r1, 12
       se_blr

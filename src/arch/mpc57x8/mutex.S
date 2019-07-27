/*
 * mutex lock and unlock source code for MPC5
 *
 * author: chetandev.ksd@gmail.com
 *
 * Note: stwcx instruction is not available forr MPC5 devices. Executing this 
 * instruction will yield illegal instruction exception. lharx is available
 * in these devices. lharx provides same operation as lwarx but on a half word data type.
 * Since skibios uses 4byte mutex locks, we need to ensure atomicity of mutex
 * locking using lharx - sthcx on 4 bytes mutex lock. We only need to ensure that
 * LSB part of mutex is locked using lharx -sthcx pair. Once the LSB part is
 * sucessfully locked. We can perform simple write on MSB part.
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

       // assume locking mutex is unsucessful
       e_li     r5, 1

       // check if mutex is already locked?
       e_addi   r3, r3, 2
       lharx    r6, 0, r3
       e_lis    r4, 0
       cmpw     r6, r4
       e_bne    exit

       // attempt locking the mutex
       e_li     r6, 1
       sthcx.   r6, 0, r3
       e_bne    exit

       // locking is sucessful
       // write remaining msb bytes
       e_addi   r3, r3, -2
       se_stw   r4, 0(r3)  
       e_lis    r5, 0

exit:

       // epilogue
       e_addi   r3, r5, 0
       e_lwz    r6, 8(r1)
       e_lwz    r5, 4(r1)
       e_lwz    r4, 0(r1)
       e_addi   r1, r1, 12
       se_blr





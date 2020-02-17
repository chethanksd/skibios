/*
 * svc handler source file
 *
 * author: chetandev.ksd@gmail.com
 *
 *
 */



.section .text, "ax"

.extern kernel_sp
.extern task_sp

.type svc_handler, %function
.globl    svc_handler
.align 4

svc_handler:

	// check cpu mode
    mfspr       r3, SRR1
    e_andi      r3, r3, 0x4000
    e_lis		r4, 0
    cmpw      	r3, r4
    e_beq     	prelouge_sp_banking_exit

    // stash sp --> task_sp
    e_lis       r4, task_sp@h
    e_or2i      r4, task_sp@l
    se_stw      r1, 0(r4)

    // load sp <-- kernel_sp x
    e_lis       r4, kernel_sp@h
    e_or2i      r4, kernel_sp@l
    e_lwz	    r1, 0(r4)

prelouge_sp_banking_exit:

    // store PR value in SRR1 to stack (-4)
    e_addi      r1, r1, -4
    se_stw      r3, 0(r1)

    // 
    // main svc handler part goes here
    //

    // retrive cpmputed PR value in SRR1 from stack
    se_lwz      r3, 0(r1)
    e_addi      r1, r1, 4

    // check if stack restoration is required (+4)
    e_lis       r4, 0
    cmpw        r3, r4
    e_beq       epilouge_sp_banking_exit

    // stash sp --> kernel_sp
    e_lis       r4, kernel_sp@h
    e_or2i      r4, kernel_sp@l
    se_stw      r1, 0(r4)

    // load sp <-- task_sp
    e_lis       r4, task_sp@h
    e_or2i      r4, task_sp@l
    e_lwz	    r1, 0(r4)

epilouge_sp_banking_exit:      
    
    se_rfi

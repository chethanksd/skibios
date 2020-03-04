/*
 * svc handler source file
 *
 * author: chetandev.ksd@gmail.com
 *
 *
 */

#include <symgen.h>

#define  SVC_STASH  r6
#define  SVC_CODE   r5
#define  ADDR       r4


.section .text, "ax"

.extern kernel_sp
.extern tasvk_sp
.extern svc_dispatch

.type svc_handler, %function
.globl    svc_handler
.align 4

svc_handler:

    // store r4, r5, r6, r7, r8, r9 to stack
    // store SRR0-1 to stack
    e_addi      r1, r1, -32
    se_stw      r4, 0(r1)
    se_stw      r5, 4(r1)
    se_stw      r6, 8(r1)
    se_stw      r7, 12(r1)
    e_stw       r8, 16(r1)
    e_stw       r9, 20(r1)

    e_stmvsrrw  24(r1)

    // store svc arg address in ADDR
    e_addi      ADDR, r1, 32

	// check cpu mode
    mfspr       r5, SRR1
    e_andi      r5, r5, 0x4000
    e_lis		r6, 0
    cmpw      	r5, r6
    e_beq     	prelouge_sp_banking_exit

    // stash sp --> task_sp
    e_lis       r6, task_sp@h
    e_or2i      r6, task_sp@l
    se_stw      r1, 0(r6)

    // load sp <-- kernel_sp
    e_lis       r6, kernel_sp@h
    e_or2i      r6, kernel_sp@l
    e_lwz	    r1, 0(r6)

prelouge_sp_banking_exit:

    // store PR value in SRR1 to stack (-12)
    // store svc_code to stack
    // store LR value to stack
    e_addi      r1, r1, -12
    se_stw      r5, 4(r1)
    se_stw      r3, 0(r1)

 	// store LR value in stack
    se_mflr		r7
    e_stw       r7, 8(r1)

    // move r3 to SVC_CODE
    e_addi      SVC_CODE, r3, 0

svc_cycle:

    // set first argument of service function to address 
    // of svc_code in stack
    e_addi      r3, r1, 0

    // SVC_CODE --> SVC_STASH
    e_addi      SVC_STASH, SVC_CODE, 0

    //get svc-dispatch table address
    e_lis       r8, svc_dispatch@h
    e_or2i      r8, svc_dispatch@l

svc_again:

    e_lwz       r9, OFFSET_OF_SDT_SVC_CODE(r8)
    e_lis       r7, 0
    cmpw        r9, r7
    e_beq       skip_check

    cmpw        SVC_CODE, r9
    e_beq       call_service
    e_addi      r8, r8, SIZE_OF_SDT
    se_b        svc_again

call_service:
    e_lwz       r0, OFFSET_OF_SDT_SVC_SERVICE(r8)

	// store important registers to stack
    e_addi      r1, r1,-24
    se_stw		r4, 4(r1)
    se_stw		r5, 8(r1)
    se_stw		r6, 12(r1)
    se_stw		r7, 16(r1)
    e_stw		r8, 20(r1)
    e_stw		r9, 24(r1)

    se_mtctr    r0
    se_bctrl

	// restore important register stacked values
    se_lwz		r4, 4(r1)
    se_lwz		r5, 8(r1)
    se_lwz		r6, 12(r1)
    se_lwz		r7, 16(r1)
    e_lwz		r8, 20(r1)
    e_lwz		r9, 24(r1)
    e_addi      r1, r1, 24

    e_lwz       SVC_CODE, 0(r1)
    cmpw        SVC_CODE, SVC_STASH
    e_beq       skip_check

    se_b        svc_cycle

skip_check:

	// retrive LR value from stack
    e_lwz       r7, 8(r1)
    se_mtlr		r7

    // retrive computed PR value in SRR1 from stack
    se_lwz      r5, 4(r1)
    e_addi      r1, r1, 12

    // check if stack restoration is required (+12)
    e_lis       r6, 0
    cmpw        r5, r6
    e_beq       epilouge_sp_banking_exit

    // stash sp --> kernel_sp
    e_lis       r6, kernel_sp@h
    e_or2i      r6, kernel_sp@l
    se_stw      r1, 0(r6)

    // load sp <-- task_sp
    e_lis       r6, task_sp@h
    e_or2i      r6, task_sp@l
    e_lwz	    r1, 0(r6)

epilouge_sp_banking_exit:

    // restore r4, r5, r6, r7, r8, r9 from stack
    // restore SRR0-1 from stack

    e_lmvsrrw   24(r1)

    e_lwz       r9, 20(r1)
    e_lwz       r8, 16(r1)
    se_lwz      r7, 12(r1)
    se_lwz      r6, 8(r1)
    se_lwz      r5, 4(r1)
    se_lwz      r4, 0(r1)
    e_addi      r1, r1, 32
    
    se_rfi

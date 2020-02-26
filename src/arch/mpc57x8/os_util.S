/*
 * os utility functions
 *
 * author: chetandev.ksd@gmail.com
 *
 * Note: stack pointer support is not provided by e200z4
 *
 */

#include <svc.h>


.section .text, "ax"


.type switch_mcu_mode, %function
.globl    switch_mcu_mode
.align 4

switch_mcu_mode:

       // stash sp --> kernel_sp
       e_lis       r4, kernel_sp@h
       e_or2i      r4, kernel_sp@l
       se_stw      r1, 0(r4)

       // load sp <-- PSP_Array[0]
       e_lis       r4, PSP_Array@h
       e_or2i      r4, PSP_Array@l
       e_lwz       r1, 0(r4)

       mfmsr       r3
       e_li        r4, 0x4000
       se_or       r3, r4
       mtmsr       r3
       msync
       isync

       // call kernel service START_SCHEDULER
       e_li       r3, START_SCHEDULER
       se_sc

       // code will never reach here
       se_blr


.type btask_psp_co4rrection, %function
.global btask_psp_correction

btask_psp_correction:
       e_addi   r3, r3, 0
       se_blr



//
// arch specific util functions
//

.type call_kernel_service, %function
.global call_kernel_service

call_kernel_service:

       e_addi  r1, r1, -20

       // store arguments in stack
       se_stw  r4, 0(r1)
       se_stw  r5, 4(r1)
       se_stw  r6, 8(r1)
       se_stw  r7, 12(r1)

       // store lr value in stack
       se_mflr r4
       se_stw  r4, 16(r1)

       // call kernel service
       se_sc

       // restore lr value from stack
       se_lwz  r4, 16(r1)
       se_mtlr r4

       // return to caller
       e_addi	r1, r1, 20
       se_blr


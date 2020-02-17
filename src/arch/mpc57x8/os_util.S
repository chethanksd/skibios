/*
 * os utility functions
 *
 * author: chetandev.ksd@gmail.com
 *
 * Note: stack pointer support is not provided by e200z4
 *
 */



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
       se_blr


.type btask_psp_co4rrection, %function
.global btask_psp_correction

btask_psp_correction:
       e_addi   r3, r3, 0
       se_blr
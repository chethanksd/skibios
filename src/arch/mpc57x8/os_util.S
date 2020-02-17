/*
 * os utility functions
 *
 * author: chetandev.ksd@gmail.com
 *
 *
 */



.section .text, "ax"


.type switch_mcu_mode, %function
.globl    switch_mcu_mode
.align 4

switch_mcu_mode:
       mfmsr       r3
       e_lis       r4, 0x2
       se_or       r3, r4
       mtmsr       r3
       se_blr


.type btask_psp_co4rrection, %function
.global btask_psp_correction

btask_psp_correction:
       e_addi   r3, r3, 0
       se_blr
/*
 * mutex lock and unlock source code for MPC5
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
       e_addi   r3, r3, 0
       se_blr
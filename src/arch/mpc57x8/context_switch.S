/*
 * svc handler source file
 *
 * author: chetandev.ksd@gmail.com
 *
 *
 */



.section .text, "ax"

.extern current_task
.extern next_task
.extern PSP_Array
.extern task_sp

.type context_switch_handler, %function
.globl    context_switch_handler
.align 4

context_switch_handler:

        e_lis       r5, current_task@h
        e_or2i      r5, current_task@l
        se_lwz      r5, 0(r5)
        e_slwi      r6, r5, 2

        e_lis       r3, PSP_Array@h
        e_or2i      r3, PSP_Array@l

        // store current_task SP --> PSP_Array[current_task]
        stwx        r1, r3, r6

        e_lis       r4, next_task@h
        e_or2i      r4, next_task@l
        se_lwz      r4, 0(r4)
        e_slwi      r6, r4, 2

        // load next_task SP <-- PSP_Array[next_task]
        lwzx        r1, r3, r6
        
        // current_task <-- next_task
        e_lis       r5, current_task@h
        e_or2i      r5, current_task@l
        se_stw      r4, 0(r5) 

        // clear sw0 interrupt
        e_lis       r4, 0xFC040040@h
        e_or2i      r4, 0xFC040040@l
        e_li        r5, 1
        e_stb       r5, 0(r4)

        se_blr
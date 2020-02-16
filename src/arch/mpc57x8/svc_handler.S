/*
 * svc handler source file
 *
 * author: chetandev.ksd@gmail.com
 *
 *
 */



.section .text, "ax"

.extern current_task
.extern process_id
.extern mutex_stash

.type svc_handler, %function
.globl    svc_handler
.align 4

svc_handler:

    e_addi   r3, r3, 0
    se_rfi
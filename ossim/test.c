/*
 * OSSIM test program
 *
 * author: chetandev.ksd@gmail.com
 *
 */


#include <stdio.h>
#include <conio.h>

#include <kernel.h>


int main() {

    kernel_init();

    printf("OSSIM test program\n");

    start_scheduler();

    return 0;
}
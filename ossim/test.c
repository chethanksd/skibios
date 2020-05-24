/*
 * OSSIM test program
 *
 * author: chetandev.ksd@gmail.com
 *
 */


#include <stdio.h>
#include <conio.h>

#include <kernel.h>
#include <param.h>

extern uint32_t  task_id[MAX_TASK_COUNT];

int main() {

    int i;

    kernel_init();

    printf("OSSIM test program\n");

    for(i = 0; i < MAX_TASK_COUNT; i++) {
        printf("task_id[%d]: %d\n", i, task_id[i]);
    }

    start_scheduler();

    return 0;
}
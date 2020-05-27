/*
 * OSSIM test program
 *
 * author: chetandev.ksd@gmail.com
 *
 */


#include <stdio.h>
#include <conio.h>

#include <kernel.h>
#include <task.h>


void mytask1(void);
void mytask2(void);
void mytask3(void);
void mytask4(void);

task_t myprocess1;
task_t myprocess2;
task_t myprocess3;
task_t myprocess4;


int main() {

    kernel_init();

    printf("OSSIM test program\n");

	task_init(&myprocess1);
	myprocess1.ptr_func=(void*)&mytask1;
	myprocess1.priority=1;
	task_start(&myprocess1);

	task_init(&myprocess2);
	myprocess2.ptr_func=(void *)&mytask2;
	myprocess2.priority=1;
	task_start(&myprocess2);


	task_init(&myprocess3);
	myprocess3.ptr_func=(void *)&mytask3;
	myprocess3.priority=1;
	task_start(&myprocess3);

	task_init(&myprocess4);
	myprocess4.ptr_func=(void *)&mytask4;
	myprocess4.priority=1;
	task_start(&myprocess4);

    start_scheduler();

	while(1) {
		// infinite loop
	}

    return 0;
}

/* Process Thread */
void mytask1(void) {
	uint32_t i = 0;
	uint16_t count = 0;

	while(1) {
		count++;
		for(i = 0; i < 1500000; i++) {

		}
		printf("Task1 count=%d\r\n", count);
	}
}


/* Process Thread */
void mytask2(void) {
	uint32_t i = 0;
	uint16_t count = 0;

	while(1) {
		for(i = 0; i < 2000000; i++) {

		}
		count++;
		printf("Task2 count=%d\r\n", count);
	}
}


/* Process Thread */
void mytask3(void) {
	uint32_t i = 0;
	uint16_t count = 0;

	while(1) {
		for(i = 0; i < 2500000; i++) {

		}
		count++;
		printf("Task3 count=%d\r\n", count);
	}
}


/* Process Thread */
void mytask4(void) {
	uint32_t i = 0;
	uint16_t count = 0;
	while(1) {
		for(i = 0; i < 3000000; i++){

		}
		count++;
		printf("Task4 count=%d\r\n", count);
	}
}

/*
 * OSSIM Hiberate test program
 *
 * author: chetandev.ksd@gmail.com
 *
 */


#include <stdio.h>
#include <conio.h>

#include <kernel.h>
#include <task.h>
#include <init.h>


void task1(void);
void task2(void);
void task3(void);
void task4(void);

task_t mytask1;
task_t mytask2;
task_t mytask3;
task_t mytask4;

uint32_t task4_count;
uint32_t task1_count;

uint32_t user_init() {

	task_init(&mytask1);
	mytask1.ptr_func=(void*)&task1;
	mytask1.priority=1;
	task_start(&mytask1);

	task_init(&mytask2);
	mytask2.ptr_func=(void *)&task2;
	mytask2.priority=1;
	task_start(&mytask2);

	task_init(&mytask3);
	mytask3.ptr_func=(void *)&task3;
	mytask3.priority=1;
	task_start(&mytask3);

	task_init(&mytask4);
	mytask4.ptr_func=(void *)&task4;
	mytask4.priority=1;
	task_start(&mytask4);

    return 0;

}

// task1 thread
void task1(void) {
	uint32_t i = 0;
	uint16_t count = 0;

	while(1) {
		count++;
        task1_count++;
		for(i = 0; i < 0x08000000; i++) {

		}
		debugf("Task1 count=%d\r\n", count);
		
        // wait up to 10 counts, then try enanble hibernation if needed
        if((task1_count > task4_count) && (task1_count > 10)) {
            hibernate(&task4_count,  (uint32_t*)&zero_ref, task1_count, false);
        }

	}
}


// task2 thread
void task2(void) {
	uint32_t i = 0;
	uint16_t count = 0;

	while(1) {
		for(i = 0; i < 0x0E000000; i++) {

		}
		count++;
		debugf("Task2 count=%d\r\n", count);
	}
}


// task3 thread
void task3(void) {
	uint32_t i = 0;
	uint16_t count = 0;

	while(1) {
		for(i = 0; i < 0x18000000; i++) {

		}
		count++;
		debugf("Task3 count=%d\r\n", count);
	}
}


// task4 thread
void task4(void) {
	uint32_t i = 0;
	uint16_t count = 0;
	while(1) {
		for(i = 0; i < 0x1E000000; i++){

		}
		count++;
        task4_count++;
		debugf("Task4 count=%d\r\n", count);
	}
}

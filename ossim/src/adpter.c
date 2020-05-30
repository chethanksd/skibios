/*
 * OSSIM Adapter program
 *
 * author: chetandev.ksd@gmail.com
 *
 */


#include <stdio.h>
#include <conio.h>

#include <Windows.h>
#include <kernel.h>
#include <task.h>
#include <ossim.h>
#include <init.h>


// local function declaration
uint8_t print_welcome_message();

// global variable
char self_path[SELF_PATH_BUFFER_SIZE];

int main() {

    // get current working directory path
    get_self_path(&self_path[0], SELF_PATH_BUFFER_SIZE);

    // print welcome message
    print_welcome_message();

    // initialize kernel
    kernel_init();

    // run ossim adapter specific initialization routine
    adapter_init();

    // run application specific initialization routine
    user_init();

    // start OS
    start_scheduler();

	while(1) {
        WriteSlot(TEXT("Hello world!\n"));
		Sleep(1000);
	}

    return 0;
}

uint8_t print_welcome_message() {

    printf("***********************************************\n");
    printf(" OSSIM Environment Version 1.0\n");
    printf("***********************************************\n");
    return 0;
}
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
#include <command.h>


// local function declaration
uint32_t print_welcome_message();


// global variable
char self_path[SELF_PATH_BUFFER_SIZE];
bool ossim_started = false;

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

    // run command processor
    while(ossim_started == false) {
        run_command_processor();
    }

    // start OS
    start_scheduler();

	while(1) {

        run_command_processor();

	}

    return 0;
}

uint32_t print_welcome_message() {

    printf("***********************************************\n");
    printf(" OSSIM Environment Version 1.0\n");
    printf("***********************************************\n\n");
    printf("type in [start] to being OSSIM\n");
    printf("type in [exit] to quit\n\n");

    return 0;

}
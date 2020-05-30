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
uint32_t print_welcome_message();
uint32_t run_command_processor();

// global variable
char self_path[SELF_PATH_BUFFER_SIZE];
char command[MAX_COMMAND_SIZE];
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
    printf("type in start to being OSSIM\n\n");

    return 0;

}

uint32_t run_command_processor() {

    printf("type in command [type exit to quit]:");
    scanf("%s", &command[0]);

    if(strcmp(&command[0], "exit") == 0) {
        close_task_console();
        exit(0);
    }

    if(strcmp(&command[0], "start") == 0) {
        
        if(ossim_started == false) {
            ossim_started = true;
            printf("\n");
            return 0;
        }

        printf("OSSIM already running\n\n");

    }

    printf("Unkown command: %s\n\n", command);

    return 0;

}
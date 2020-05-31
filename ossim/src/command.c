/*
 * OSSIM Adapter program
 *
 * author: chetandev.ksd@gmail.com
 *
 */


#include <stdio.h>
#include <conio.h>
#include <stdlib.h> 
#include <windows.h>


#include <kernel.h>
#include <task.h>
#include <init.h>
#include <kvar.h>
#include <ossim.h>
#include <error.h>
#include <param.h>
#include <command.h>


// external functions
extern uint32_t print_welcome_message();

// local functions
static void process_command();
static void process_zero_arg_command();

// extern global variables
extern bool ossim_started;
extern volatile uint32_t schedule_count;
extern volatile uint32_t scheduler_step;
extern volatile bool halt_scheduler;
extern volatile uint8_t ossim_real_task_state[];

// local global variables
static char command_line[MAX_COMMAND_LINE_SIZE];
static char command [MAX_COMMAND_SIZE];
static char argument[MAX_ARGUMENT_SIZE];


uint32_t run_command_processor() {

    char *token;
    char *result;

    memset(&command[0], 0, MAX_COMMAND_SIZE);
    memset(&argument[0], 0, MAX_ARGUMENT_SIZE);
    memset(&command_line[0], 0, MAX_COMMAND_LINE_SIZE);

    printf("type in command:");
    result = fgets(&command_line[0], MAX_COMMAND_LINE_SIZE, stdin);

    if(result == NULL) {
        printf("command length exceeds set limite\n\n");
        goto quit;
    }

    command_line[strcspn(command_line, "\n")] = 0;

    if(strcmp(&command_line[0], "") == 0) {
        printf("\n");
        goto quit;
    }

    // get command
    token = strtok(&command_line[0], " ");
    if(token != NULL) {
        strcpy(&command[0], token);
    }

    // get argument
    token = strtok(NULL, " ");
    if(token != NULL) {
        strcpy(&argument[0], token);
    }

    #ifdef COMMAND_VERBOSE
    printf("command : %s\n", command);
    printf("argument: %s\n", argument);
    #endif
    
    // more data then print command line as invalid
    token = strtok(NULL, " ");
    if(token != NULL) {
        printf("so many arguments.only one arugment supported\n\n");
        goto quit;
    }

    process_command();

quit:

    return 0;

}


void process_command() {

    uint32_t value;
    char *endptr;

    // step command
    if(strcmp(&command[0], "step") == 0) {

        if(ossim_started == false){
            printf("OSSIM not started!\n\n");
            goto quit;
        }

        if(halt_scheduler == false) {
            printf("scheduler not halted!\n\n");
            goto quit;
        } 

        if(scheduler_step != 0) {
            printf("scheduler step execution in progress\n\n");
            goto quit;
        }   

        if(strcmp(&argument[0], "") == 0) {
            scheduler_step = 1;
            printf("\n");
            goto quit;
        }

        value = strtol(&argument[0], &endptr, 10);
        
        // endptr points to null for failed conversion
        if(*endptr) {
            printf("Bad argument, unable to convert to integer!\n\n");
            goto quit;
        }

        scheduler_step = value;
        printf("\n");

        goto quit;

    }

    // any command after here will be single argument command
    // so check if argument is empty
    if(strcmp(argument, "") != 0) {
        printf("argument unexpected!\n\n");
        goto quit;
    } else {
        process_zero_arg_command();
        goto quit;
    }

    printf("bad argument\n\n");

quit:

    return;

}

void process_zero_arg_command() {

    uint32_t i = 0;

    // exit command
    if(strcmp(&command[0], "exit") == 0) {
        close_task_console();
        exit(0);
    }

    // start command
    if(strcmp(&command[0], "start") == 0) {
        
        if(ossim_started == false) {
            ossim_started = true;
            printf("\n");
            goto quit;
        }

        printf("OSSIM already running\n\n");
        goto quit;

    }

    // stop command
    if(strcmp(&command[0], "stop") == 0) {

        if(ossim_started == false){
            printf("OSSIM not started!\n\n");
            goto quit;
        }
        
        if(halt_scheduler == true) {
            printf("scheduler already halted!\n\n");
            goto quit;
        }

        halt_scheduler = true;
        printf("\n");
        goto quit;

    }

    // resume command
    if(strcmp(&command[0], "resume") == 0) {

        if(ossim_started == false){
            printf("OSSIM not started!\n\n");
            goto quit;
        }

        if(halt_scheduler == false) {
            printf("scheduler already running!\n\n");
            goto quit;
        }

        if(scheduler_step != 0) {
            printf("scheduler step execution in progress\n\n");
            goto quit;
        } 

        halt_scheduler = false;
        printf("\n");
        goto quit;

    }

    // clr command
    if(strcmp(&command[0], "clr") == 0) {
        system("cls");
        print_welcome_message();
        goto quit;
    }

    // csc command
    if(strcmp(&command[0], "csc") == 0) {
        printf("current scheduler count: %d\n\n", schedule_count);
        goto quit;
    }

    // ttc command
    if(strcmp(&command[0], "ttc") == 0) {
        printf("total task count: %d\n\n", total_task_count);
        goto quit;
    }

    // ctc command
    if(strcmp(&command[0], "ctc") == 0) {
        printf("current task count: %d\n\n", task_count);
        goto quit;
    }

    // grts command
    if(strcmp(&command[0], "grts") == 0) {

        printf("current task state: ");
        for(i = 0; i < MAX_TASK_COUNT; i++) {
            printf("%d, ", ossim_real_task_state[i]);
        }

        printf("\n\n");
        goto quit;

    }

    printf("unknown command: %s\n\n", command_line);

quit:

    return;

}
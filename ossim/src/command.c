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
#include <command.h>


// external functions
extern uint32_t print_welcome_message();

// local functions
static void process_command();
static void process_zero_arg_command();

// extern global variables
extern bool ossim_started;
extern uint32_t schedule_count;


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

    printf("unknown command: %s\n\n", command_line);

quit:

    return;

}
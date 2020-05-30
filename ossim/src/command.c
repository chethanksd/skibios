/*
 * OSSIM Adapter program
 *
 * author: chetandev.ksd@gmail.com
 *
 */


#include <stdio.h>
#include <conio.h>
#include <windows.h>


#include <kernel.h>
#include <task.h>
#include <init.h>
#include <ossim.h>
#include <error.h>
#include <command.h>


// extern global variables
extern bool ossim_started;

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
        goto quit_error;
    }

    command_line[strcspn(command_line, "\n")] = 0;

    if(strcmp(&command_line[0], "") == 0) {
        goto quit_error;
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
        goto quit_error;
    }

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

    printf("unknown command: %s\n\n", command_line);

quit_error:

    return 0;

}
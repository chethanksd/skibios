/*
 * OSSIM Adapter program
 *
 * author: chetandev.ksd@gmail.com
 *
 */


#include <stdio.h>
#include <conio.h>
#include <stdarg.h>
#include <windows.h>

#include <kernel.h>
#include <task.h>
#include <init.h>
#include <ossim.h>
#include <error.h>

// define
#define TASK_CONSOLE_PATH_SIZE      400


// external global variables
extern char self_path[];

// local global functions
DWORD task_console_pid;
DWORD current_console_pid;
static STARTUPINFO task_console_si;
static PROCESS_INFORMATION task_console_pi;
static char task_console_path[TASK_CONSOLE_PATH_SIZE];

uint32_t adapter_init() {

    uint32_t error;

    // string concat task console path with self path
    memset(&task_console_path[0], 0, TASK_CONSOLE_PATH_SIZE);
    memcpy(task_console_path, self_path, SELF_PATH_BUFFER_SIZE);
    strcat(task_console_path, "\\..\\test\\task_console.exe");

    // create task console process
    ZeroMemory(&task_console_si, sizeof(task_console_si));
    task_console_si.cb = sizeof(task_console_si);
    ZeroMemory(&task_console_pi, sizeof(task_console_pi));

    if(!CreateProcess(&task_console_path[0],
                        NULL,                   // Command line.
                        NULL,                   // Process handle not inheritable.
                        NULL,                   // Thread handle not inheritable.
                        FALSE,                  // Set handle inheritance to FALSE.
                        CREATE_NEW_CONSOLE,     // No creation flags.
                        NULL,                   // Use parent's environment block.
                        NULL,                   // Use parent's starting directory.
                        &task_console_si,       // Pointer to STARTUPINFO structure.
                        &task_console_pi)       // Pointer to PROCESS_INFORMATION structure.
                        ){
    error = GetLastError();
    printf( "CreateProcess failed (%d).\n", error);
    goto quit_error;
    }

    // get pid of task console thraad
    task_console_pid = GetProcessId(task_console_pi.hProcess);

    // get pid of ossim adapter console
    current_console_pid = GetCurrentProcessId();

quit_error:
    return ERROR_NONE;

}

uint32_t get_self_path(char *path_buffer, uint32_t size) {
    
    DWORD ret;
    char *spos;
    
    memset(path_buffer, 0, size);
    ret = GetModuleFileNameA(NULL, path_buffer, size);

    if(ret == 0) {
        printf("GetCurrentDirectory failed (%lu)\n", GetLastError());
        exit(1);
    }

    if(ret > size) {
        printf("ERROR! Not enough value in SELF_PATH_BUFFER_SIZE macro\n");
        exit(1);
    }

    // remove exe filename from path
    spos = strrchr(path_buffer, '\\');
    if (spos != NULL) {
    *spos = '\0'; 
    } else {
        printf("ERROR! Bad self path\n");
        exit(1);
    }

    return ERROR_NONE;

}
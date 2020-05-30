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
#include <winbase.h>

#include <kernel.h>
#include <task.h>
#include <init.h>
#include <ossim.h>
#include <error.h>

// define
#define TASK_CONSOLE_PATH_SIZE      400
#define TASK_CONSOLE_MSG_SIZE       512

// external global variables
extern char self_path[];

// local global functions
DWORD task_console_pid;
DWORD current_console_pid;
HANDLE mslot_file; 
HANDLE mslot_handle;
static STARTUPINFO task_console_si;
static PROCESS_INFORMATION task_console_pi;
static char task_console_path[TASK_CONSOLE_PATH_SIZE];
static char task_console_msg[TASK_CONSOLE_MSG_SIZE];

char *slot_name = "\\\\.\\mailslot\\console_task";

uint32_t adapter_init() {

    // string concat task console path with self path
    memset(&task_console_path[0], 0, TASK_CONSOLE_PATH_SIZE);
    memcpy(task_console_path, self_path, SELF_PATH_BUFFER_SIZE);
    strcat(task_console_path, "\\task_console.exe");

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
                        &task_console_pi)){     // Pointer to PROCESS_INFORMATION structure.
    printf( "CreateProcess failed (%lu).\n", GetLastError());
    exit(1);
    }

    // get pid of task console thraad
    task_console_pid = GetProcessId(task_console_pi.hProcess);

    // get pid of ossim adapter console
    current_console_pid = GetCurrentProcessId();

    // wait unitil task console creates mailbox
    // CreateFile will fail unitl mailbox is created
    do {
        mslot_file = CreateFile(slot_name, 
                                GENERIC_WRITE, 
                                FILE_SHARE_READ,
                                (LPSECURITY_ATTRIBUTES) NULL, 
                                OPEN_EXISTING, 
                                FILE_ATTRIBUTE_NORMAL, 
                                (HANDLE) NULL); 
    
    } while (mslot_file == INVALID_HANDLE_VALUE);

    return ERROR_NONE;

}

uint32_t write_slot(char *message) {
   BOOL result; 
   DWORD written; 
 
   result = WriteFile(mslot_file, 
                        message, 
                        (DWORD) (lstrlen(message)+1)*sizeof(TCHAR),  
                        &written, 
                        (LPOVERLAPPED) NULL); 
 
   if (!result) { 
      printf("WriteFile failed with %lu.\n", GetLastError()); 
      return 1; 
   } 

   return 0;

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

void debugf(const char* format, ...) {

    va_list args;

    va_start(args, format);
    memset(&task_console_msg[0], 0, TASK_CONSOLE_MSG_SIZE);
    vsnprintf (&task_console_msg[0], TASK_CONSOLE_MSG_SIZE, format, args);
    
    write_slot(task_console_msg);
  
    va_end(args); 

}
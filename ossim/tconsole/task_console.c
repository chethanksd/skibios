/*
 * OSSIM Task Console program
 *
 * author: chetandev.ksd@gmail.com
 *
 */

#include <windows.h>
#include <tchar.h>
#include <stdio.h>
#include <strsafe.h>

#define MAX_READ_BUFFER_SIZE        512

// global variable
HANDLE mslot_handle;
LPCTSTR slot_name = TEXT("\\\\.\\mailslot\\console_task");

char msg_buffer[MAX_READ_BUFFER_SIZE];

BOOL read_slot() 
{   
    HANDLE event_handle;
    DWORD msg_size, msg_idx, read; 
    BOOL result;  
    OVERLAPPED ov;
 
    msg_size = msg_idx = read = 0; 

    event_handle = CreateEvent(NULL, FALSE, FALSE, TEXT("console_slot"));
    if(event_handle == NULL) {
        return FALSE;
    }

    ov.Offset = 0;
    ov.OffsetHigh = 0;
    ov.hEvent = event_handle;
 
    result = GetMailslotInfo( mslot_handle, // mailslot handle 
        (LPDWORD) NULL,               // no maximum message size 
        &msg_size,                   // size of next message 
        &msg_idx,                    // number of messages 
        (LPDWORD) NULL);              // no read time-out 
 
    if (!result) 
    { 
        printf("GetMailslotInfo failed with %lu.\n", GetLastError()); 
        return FALSE; 
    } 
 
    if (msg_size == MAILSLOT_NO_MESSAGE) 
    { 
        return TRUE; 
    } 
 
    while (msg_idx != 0)  // retrieve all messages
    { 

        memset(&msg_buffer[0], 0, MAX_READ_BUFFER_SIZE);
        result = ReadFile(mslot_handle, 
            msg_buffer, 
            msg_size, 
            &read, 
            &ov); 
 
        if (!result) 
        { 
            printf("ReadFile failed with %lu.\n", GetLastError()); 
            return FALSE; 
        } 

 
        // Display the message. 
        printf("%s", msg_buffer); 
 
 
        result = GetMailslotInfo(mslot_handle,  // mailslot handle 
            (LPDWORD) NULL,               // no maximum message size 
            &msg_size,                   // size of next message 
            &msg_idx,                    // number of messages 
            (LPDWORD) NULL);              // no read time-out 
 
        if (!result) 
        { 
            printf("GetMailslotInfo failed (%lu)\n", GetLastError());
            return FALSE; 
        } 
    } 

    CloseHandle(event_handle);

    return TRUE; 
}

BOOL WINAPI make_slot(LPCTSTR lpszSlotName) 
{ 
    mslot_handle = CreateMailslot(lpszSlotName, 
        0,                             // no maximum message size 
        MAILSLOT_WAIT_FOREVER,         // no time-out for operations 
        (LPSECURITY_ATTRIBUTES) NULL); // default security
 
    if (mslot_handle == INVALID_HANDLE_VALUE) 
    { 
        printf("CreateMailslot failed with %lu\n", GetLastError());
        return FALSE; 
    } 
    return TRUE; 
}

int main(int argc, char *argv[], char *envp[]) {
    
    make_slot(slot_name);

    while(TRUE)
    {
        read_slot();
    }

    return 0;

}


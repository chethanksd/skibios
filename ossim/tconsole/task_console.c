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

// local define
#define MAX_READ_BUFFER_SIZE        512

// global variable
HANDLE mslot_handle;
HANDLE event_handle;
char *slot_name = "\\\\.\\mailslot\\console_task";

char msg_buffer[MAX_READ_BUFFER_SIZE];



static void read_slot() {   
    DWORD msg_size, msg_idx, read; 
    BOOL result;  
    OVERLAPPED ov;
 
    msg_size = msg_idx = read = 0; 

    ov.Offset = 0;
    ov.OffsetHigh = 0;
    ov.hEvent = event_handle;
    
    // check if there is any message in mailslot
    // if there is a message then retrive it
    result = GetMailslotInfo(mslot_handle,          // mailslot handle 
                            (LPDWORD) NULL,         // no maximum message size 
                            &msg_size,              // size of next message 
                            &msg_idx,               // number of messages 
                            (LPDWORD) NULL);        // no read time-out 
 
    if (!result) { 
        printf("GetMailslotInfo failed with %lu.\n", GetLastError()); 
        return ; 
    } 
 
    if (msg_size == MAILSLOT_NO_MESSAGE) { 
        return; 
    } 
 
    // retrieve all messages
    while (msg_idx != 0)  { 

        memset(&msg_buffer[0], 0, MAX_READ_BUFFER_SIZE);
        result = ReadFile(mslot_handle, 
            msg_buffer, 
            msg_size, 
            &read, 
            &ov); 
 
        if (!result) { 
            printf("ReadFile failed with %lu.\n", GetLastError()); 
            return; 
        } 

 
        // display the message. 
        printf("%s", msg_buffer); 
 
        // get next message from mailslot
        result = GetMailslotInfo(mslot_handle,      // mailslot handle 
                                (LPDWORD) NULL,     // no maximum message size 
                                &msg_size,          // size of next message 
                                &msg_idx,           // number of messages 
                                (LPDWORD) NULL);    // no read time-out 
 
        if (!result) { 
            printf("GetMailslotInfo failed (%lu)\n", GetLastError());
            return; 
        } 
    } 

}

static void make_slot() {

    mslot_handle = CreateMailslot(slot_name, 
                                    0,                             // no maximum message size 
                                    MAILSLOT_WAIT_FOREVER,         // no time-out for operations 
                                    (LPSECURITY_ATTRIBUTES) NULL); // default security
 
    if (mslot_handle == INVALID_HANDLE_VALUE) { 
        printf("CreateMailslot failed with %lu\n", GetLastError());
        return; 
    }
 
}

int main() {

    DWORD wait_result;

    make_slot();

    event_handle = CreateEvent(NULL, FALSE, FALSE, "console_slot");
    if(event_handle == NULL) {
        printf("failed to create event (%lu)\n", GetLastError());
        return 0;
    }

    while(1) {

        read_slot();
        
        wait_result = WaitForSingleObject(event_handle, 10);

        switch (wait_result)  {

            // Event object was signaled
            case WAIT_OBJECT_0: 
                CloseHandle(event_handle);

                event_handle = CreateEvent(NULL, FALSE, FALSE, "console_slot");
                if(event_handle == NULL) {
                    printf("failed to create event (%lu)\n", GetLastError());
                    return 0;
                }

                break; 
        }
    }

    return 0;

}


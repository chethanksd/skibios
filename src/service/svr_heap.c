/*
 * heap management kernel servie code
 *
 * author: chetandev.ksd@gmail.com
 *
 */

#include <stdlib.h>
#include <error.h>
#include <param.h>
#include <kernel.h>
#include <kvar.h>
#include <svc.h>
#include <svr_heap.h>

extern void* search_free_marker(uint32_t size);
extern void* add_new_allocated_marker(uint32_t size);
extern uint32_t release_allocated_marker(uint32_t address);

// extern kernel variables
extern uint32_t blist_size;
extern uint32_t _proc_heap_addr;
extern uint32_t heap_remaining;

uint32_t svc_service_gheap_release(uint32_t *svc_num, uint32_t *arguments) {

    uint32_t error = ERROR_NONE;
    uint32_t status;
    uint32_t address;

    address = arguments[1];

    if(blist_size == 0) {

        error = ERROR_EMPTY_HEAP;
        goto quit_error;

    }

    status = release_allocated_marker(address);

    if(status == 0 && blist_size != 0) {
        error = ERROR_WRONG_HEAP_POINTER;
    }

quit_error:

    return error;

}

uint32_t svc_service_gheap_allocate(uint32_t *svc_num, uint32_t *arguments) {

    uint32_t *allocated_address;
    uint32_t  size_request;

    allocated_address = NULL;
    size_request = arguments[1];

    if(blist_size == 0) {

        allocated_address =(uint32_t*) add_new_allocated_marker(size_request);
        goto search_done;

    }

    allocated_address = (uint32_t*) search_free_marker(size_request);

    if(allocated_address == NULL) {

        allocated_address = (uint32_t*) add_new_allocated_marker(size_request);

    }

search_done:

    if(allocated_address != NULL) {
        heap_remaining = heap_remaining - size_request;
    }

    return (uint32_t)allocated_address;
    
}
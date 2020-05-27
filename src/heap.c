/* 
 *  meta-data based heap Mangament Library              
 *                   
 *  author: chetandev.ksd@gmail.com                
 * 
 *                                    
 */

#include <heap.h>
#include <svc.h>
#include <kernel.h>
#include <share.h>
#include <param.h>
#include <task.h>
#include <error.h>
#include <access.h>
#include <kvar.h>

#define FREE_HEADER_MASK        0x00008000

#define DEVICE_SRAM_SIZE        0x40000
#define DEVICE_SRAM_START       0x20000000

#define DUMMY_MARKER_ALLOCATED  0xFFFF0020
#define DUMMY_MARKER_FREE       0xFFFF8200
#define GHEAP_END_MARKER        0x0000FFFF

extern void* __heap_start__;
extern void* _ghmb_addr;

uint32_t  KSECTION(.kbss) hstart_addr;
uint32_t  KSECTION(.kbss) bstart_addr;
uint32_t  KSECTION(.kbss) heap_remaining;

uint32_t  KSECTION(.kbss) fsll_start;
uint32_t  KSECTION(.kbss) fsll_end;
uint32_t  KSECTION(.kbss) blist_size;

void* search_free_marker(uint32_t size);
void* add_new_allocated_marker(uint32_t size);
uint32_t release_allocated_marker(uint32_t address);

void heap_init() {
    
    uint8_t rem;

    // ajust alignment of heap start pointer
    hstart_addr = (uint32_t)&__heap_start__;
    rem = hstart_addr % 4;
    rem = rem == 0 ? 0 : rem;
    hstart_addr += rem;

    // obtain start of bookeeping region
    bstart_addr = (uint32_t) &_ghmb_addr;

    // clear entire kernel heap. written in assembly for optimized implementation
    __asm volatile( "           LDR R3, =%[hsp]         		\n"
                    "           LDR R1, =%[hbsz]                \n"
                    "           LDR R2, [R3]            		\n"
                    "           MOV R3, R2              		\n"
                    "           MOV R2, #0              		\n"
                    " again%=:  STR R2, [R3, R1, LSL #2]        \n"
                    "           SUB R1, R1, #1        			\n"
                    "           CMP R1, #0              		\n"
                    "           IT NE                   		\n"
                    "           BNE again%=             		\n"
                    :
                    : [hsp] "i" (&bstart_addr), [hbsz] "i" (HEAP_BOOKEEPING_SIZE)
                    : "r1", "r2", "r3"
    );

    bstart_addr += (HEAP_BOOKEEPING_SIZE * 4) - 4;

    // place no allocation marker at the start bookeeping region
    fsll_start = bstart_addr;
    fsll_end = bstart_addr;
    blist_size=0;

    heap_remaining = (DEVICE_SRAM_START + DEVICE_SRAM_SIZE) - hstart_addr;

}

void* heap_allocate(uint32_t size){
    
    void *free_ptr;
    uint32_t rem;

    // make size adjustment for word aligned allocation
    rem = size % 4;
    rem = rem == 0 ? 0 : (4 - rem);
    size = size + rem;

    if(size > 32767){
        return NULL_POINTER;
    }

    if(heap_remaining < size) {
        return NULL_POINTER;
    }

    __asm volatile (" MOV R1, %[size] "
                :
                : [size] "r" (size)
                :
        );

    svc(GHEAP_ALLOCATE);

    __asm volatile (" MOV %[free], R0 "
            : [free] "=r" (free_ptr)
            ::
        );

    return (void *) free_ptr;
}

uint32_t heap_release(void *ptr){
    
    uint32_t state = 0;

    __asm volatile (" MOV R1, %[ptr] "
            :
            : [ptr] "r" (ptr)
            :
    );

    svc(GHEAP_RELEASE);

    __asm volatile (" MOV %[state], R0 "
        : [state] "=r" (state)
        ::
    );

    return state;
}

void* __attribute__((naked)) search_free_marker(uint32_t size) {
    
    __asm volatile( "			PUSH	{R1-R6, LR}			    \n"
                    "           LDR     R5, =%[fslls]           \n"         //Obtain fsll_start content
                    "           LDR     R1, [R5]                \n"         
                    "           LDR     R5, =%[bsize]           \n"         //Obtain blist_size content     
                    "           LDR     R2, [R5]                \n"
                    "           LDR     R6, =%[hstart]          \n"
                    "           LDR     R5, [R6]                \n"         //Set R5 as RAM tracking pointer
                    "  again%=:	CMP		R2, #0					\n"
                    "           ITT    	EQ						\n"
                    "           MOVEQ   R5, #0                  \n"
                    "           BEQ     exit%=                  \n"
                    "           LDRH    R3, [R1, #0]            \n"         //Obtain the Data from linked list
                    "           TST     R3, %[free_mask]        \n"         //Is there any free marker
                    "           IT      NE                      \n"
                    "           BNE     szchk%=                 \n"
                    "  retrn%=: ADD     R5, R5, R3              \n"         //Update RAM tracking pointer     
                    "           LDRSH   R4, [R1, #2]            \n"         //Obtain next linked list pointer
                    "           ADD     R1, R1, R4, LSL #2      \n"
                    "           SUB     R2, R2, #1              \n"         //Decrement blist counter and check
                    "			B		again%=             	\n"      
                    "  szchk%=: BIC     R3, R3, %[free_mask]    \n"         //R3 = R3 & (~alloc_mask)
                    "           CMP     R3, R0                  \n"         //Marker_size >= size ?
                    "           IT      LT                      \n"
                    "           BLT     retrn%=                 \n"
                    "           SUB     R3, R3, R0              \n"         //We have got free space. Calculate remaining unallocated space
                    "           CBZ     R3, convm%=             \n"         //no remaining space (R3 holds remaining)?
                    "           LDR     R4, =%[bstart]          \n"         //there is remaining space, find free space
                    "           LDR     R2, [R4]                \n"
                    "           LDR     R6, =%[hbsz]            \n"
                    "  loop%=:  CBZ     R6, convm%=             \n"         //Searched all spaces? then jump to convm
                    "           SUB     R4, R2, R6, LSL #2      \n"
                    "           LDR     R4, [R4]                \n"         //Load content of the given space
                    "           CMP     R4, #0                  \n"         //Is space empty?
                    "           ITT     NE                      \n"
                    "           SUBNE   R6, R6, #1              \n"
                    "           BNE     loop%=                  \n"         //If not empty then continue search
                    "           SUB     R2, R2, R6, LSL #2      \n"         //calculate split marker address      
                    "           SUB     R4, R1, R2              \n"         //Calculate AllocM to SplitM index
                    "           MOV     R6, #4                  \n"
                    "           SDIV    R4, R4, R6              \n"
                    "           LDRSH   R6, [R1, #2]            \n"   
                    "           ADD     R4, R4, R6              \n"
                    "           LDR     R6, =0x8000             \n"
                    "           CMN     R4, R6                  \n"         //if(new offset < -32768 && > 32768)
                    "           IT      LE                      \n"
                    "           BLE     convm%=                 \n"         //else
                    "           CMP     R4, R6                  \n"
                    "           IT      GE                      \n"
                    "           BGE     convm%=                 \n"         //else
                    "           ORR     R6, R3, %[free_mask]    \n"
                    "           STRH    R6, [R2, #0]            \n"         //store remaining in split marker
                    "           STRH    R4, [R2, #2]            \n"
                    "           SUB     R4, R2, R1              \n"
                    "           MOV     R6, #4                  \n"
                    "           SDIV    R4, R4, R6              \n"
                    "           LDR     R6, =0x8000             \n"
                    "           CMN     R4, R6                  \n"         //if((alloc - split) < -32768 && > 32768)
                    "           ITTT    LE                      \n"
                    "           MOVLE   R4, #0                  \n"
                    "           STRLE   R4, [R2]                \n"
                    "           BLE     convm%=                 \n"         //else
                    "           CMP     R4, R6                  \n"
                    "           ITTT    GE                      \n"
                    "           MOVGE   R4, #0                  \n"
                    "           STRGE   R4, [R2]                \n"
                    "           BGE     convm%=                 \n"         //else
                    "           STRH    R4, [R1, #2]            \n"         //There is no pointer overflow. So, store alloc ->split pointer
                    "           LDR     R2, =%[bsize]           \n"         //Increment blist_size
                    "           LDR     R4, [R2]                \n"
                    "           ADD     R4, R4, #1              \n"
                    "           STR     R4, [R2]                \n"
                    "           MOV     R3, #0                  \n"
                    " convm%=:  ADD     R3, R3, R0              \n"         //Convert Free Marker to Allocated Marker
                    "           STRH    R3, [R1, #0]            \n"
                    "  exit%=:  MOV     R0, R5                  \n"         //Return free_ptr
                    "           POP		{R1-R6, PC}             \n"
                    :
                    : [bstart] "i" (&bstart_addr), [bsize] "i" (&blist_size), [size] "r" (size), \
                        [free_mask]  "i" (FREE_HEADER_MASK), [fslls] "i" (&fsll_start), \
                        [fslle] "i" (&fsll_end), [hbsz] "i" (HEAP_BOOKEEPING_SIZE), [hstart] "i" (&hstart_addr)
                    : "r1", "r2", "r3", "r4", "r5", "r6", "lr"
    );  
    return NULL_POINTER;
}

void* __attribute__((naked)) add_new_allocated_marker(uint32_t size) {

    __asm volatile( "           PUSH    {R1-R6, LR}             \n"
                    "           LDR     R5, =%[bstart]          \n"
                    "           LDR     R1, [R5]                \n"
                    "           LDR     R5, =%[fslle]           \n"
                    "           LDR     R2, [R5]                \n"
                    "           MOV     R5, #0                  \n"         //Set R5 as Allocated RAM Tracking pointer
                    "           MOV     R6, #0                  \n"
                    " loop%=:   CMP     R6, #%c[hbsz]           \n" 
                    "           IT      EQ                      \n"
                    "           BEQ     exit%=                  \n"
                    "           LDR     R3, [R1]                \n"
                    "           CBZ     R3, proc%=              \n"
                    "           SUB     R1, R1, #4              \n"
                    " retrn%=:  ADD     R6, R6, #4              \n"
                    "           B       loop%=                  \n"
                    " proc%=:   SUB     R4, R1, R2              \n"
                    "           MOV     R3, #4                  \n"
                    "           SDIV    R4, R4, R3              \n"
                    "           LDR     R3, =0x8000             \n"
                    "           CMN     R4, R3                  \n"         //if((freespace - fslle) < -32768 && > 32768)
                    "           IT      LE                      \n"
                    "           BLE     retrn%=                 \n"         //else
                    "           CMP     R4, R3                  \n"
                    "           IT      GE                      \n"
                    "           BGE     retrn%=                 \n"         //else
                    "           STRH    R0, [R1, #0]            \n"         //Create New Allocated Marker
                    "           STRH    R4, [R2, #2]            \n"         //Old End marker now points to new marker
                    "           LDR     R3, =%[fslle]           \n"         //Store new fsll_end (which is new allocated marker)
                    "           STR     R1, [R3]                \n"        
                    "           LDR     R3, =%[fslls]           \n"         //Obtain RAM Address of new allocated
                    "           LDR     R2, [R3]                \n"
                    "           LDR     R3, =%[hstart]          \n"         // R5 = hstart_addr
                    "           LDR     R5, [R3]                \n"
                    " again%=:  CMP     R1, R2                  \n"         //is R2 == fslle ? then exit
                    "           IT      EQ                      \n"
                    "           BEQ     done%=                  \n"
                    "           LDRH    R3, [R2, #0]            \n"         //obtain size
                    "           BIC     R3, R3, %[free_mask]    \n"     
                    "           ADD     R5, R5, R3              \n"  
                    "           LDRSH   R3, [R2, #2]            \n" 
                    "           ADD     R2, R2, R3, LSL #2      \n"
                    "           B       again%=                 \n"
                    " done%=:   LDR     R3, =%[blist]           \n"         //Update blist_size
                    "           LDR     R2, [R3]                \n"
                    "           ADD     R2, R2, #1              \n"
                    "           STR     R2, [R3]                \n"  
                    " exit%=:   MOV     R0, R5                  \n"     
                    "           POP     {R1-R6, PC}             \n"
                    :
                    : [bstart] "i" (&bstart_addr), [size] "r" (size), [fslls] "i" (&fsll_start), \
                        [fslle] "i" (&fsll_end), [hbsz] "i" (HEAP_BOOKEEPING_SIZE), [blist] "i" (&blist_size),\
                        [free_mask]  "i" (FREE_HEADER_MASK), [hstart] "i" (&hstart_addr)
                    : "r1", "r2", "r3", "r4", "r5", "r6"
    );

    return NULL_POINTER;
}


uint32_t __attribute__((naked)) release_allocated_marker(uint32_t address) {

    __asm volatile( "           PUSH    {R1-R7, LR}             \n"
                    "           LDR     R5, =%[fslls]           \n"
                    "           LDR     R1, [R5]                \n"
                    "           LDR     R5, =%[bstart]          \n"
                    "           LDR     R2, [R5]                \n"
                    "           LDR     R7, =%[fslle]           \n"
                    "           LDR     R5, =%[blist]           \n"
                    "           LDR     R6, [R5]                \n"
                    "           LDR     R3, =%[hstart]          \n"         //Set R5 as RAM Tracking pointer
                    "           LDR     R5, [R3]                \n"     
                    "  loop%=:  LDRH    R3, [R1, #0]            \n"         //Get size from marker
                    "           BIC     R3, R3, %[free_mask]    \n"
                    "           CMP     R5, R0                  \n"         //Request addr == Tracking Addr
                    "           IT      EQ                      \n"
                    "           BEQ     proc%=                  \n"
                    "           ADD     R5, R5, R3              \n"         //Update RAM Tracking pointer
                    "           LDRSH   R4, [R1, #2]            \n"
                    "           ADD     R1, R1, R4, LSL #2      \n"
                    "           SUB     R6, R6, #1              \n"
                    "           CMP     R6, #0                  \n"         //Done searching all marker, then exit
                    "           IT      EQ                      \n"         //Done use CBZ instruction (out of range problem)
                    "           BEQ     exit%=                  \n"
                    "           B       loop%=                  \n"
                    "  proc%=:  LDR     R5, =%[hrem]            \n"         //Update heap_remaining variable
                    "           LDR     R6, [R5]                \n"
                    "           ADD     R6, R6, R3              \n"
                    "           STR     R6, [R5]                \n"
                    "           LDR     R5, =%[blist]           \n"
                    "           LDR     R6, [R5]                \n"
                    "           ORR     R3, R3, %[free_mask]    \n"
                    "           STRH    R3, [R1, #0]            \n"         //convert alloc to free
                    "           LDRSH   R4, [R1, #2]            \n"         //Club Next Free marker with current one if possible
                    "           CMP     R4, #0                  \n"         //No next marker? then delete current marker
                    "           ITTT    EQ                      \n"
                    "           STREQ   R4, [R1]                \n"
                    "           SUBEQ   R6, R6, #1              \n"
                    "           BEQ     skip%=                  \n"
                    "           ADD     R2, R1, R4, LSL #2      \n"         
                    "           LDRH    R4, [R2, #0]            \n"
                    "           TST     R4, %[free_mask]        \n"         //Is there any free marker
                    "           IT      EQ                      \n"
                    "           BEQ     skip%=                  \n"
                    "           BIC     R4, R4, %[free_mask]    \n"
                    "           ADD     R3, R3, R4              \n"
                    "           LDR     R4, =0xFFFF             \n"         //Check if new size is OK
                    "           CMP     R3, R4                  \n"
                    "           IT      GE                      \n"
                    "           BGE     skip%=                  \n"
                    "           LDRSH   R4, [R2, #2]            \n"         //Current free maker will point to next allocatd marker is offset is OK
                    "           SUB     R0, R2, R1              \n"         //Calculate new index
                    "           MOV     R5, #4                  \n"
                    "           SDIV    R0, R5                  \n"
                    "           ADD     R0, R0, R4              \n"
                    "           LDR     R5, =0x8000             \n"
                    "           CMN     R0, R5                  \n"         //if((next - current) < -32768 && > 32768)
                    "           IT      LE                      \n"
                    "           BLE     skip%=                  \n"         //else
                    "           CMP     R0, R5                  \n"
                    "           IT      GE                      \n"
                    "           BGE     skip%=                  \n"         //else
                    "           STRH    R0, [R1, #2]            \n"
                    "           STRH    R3, [R1, #0]            \n"         //Size OK! then Store clubbed free space in current marker
                    "           MOV     R4, #0                  \n"         //Make Next marker empty space
                    "           STR     R4, [R2]                \n"
                    "           SUB     R6, R6, #1              \n"         //Update blist_size
                    "  skip%=:  LDR     R5, =%[fslls]           \n"
                    "           LDR     R4, [R5]                \n"
                    "           CMP     R4, R1                  \n"         //current marker == fslls
                    "           IT      EQ                      \n"
                    "           BEQ     uexit%=                 \n"
                    "  again%=: CMP     R4, R1                  \n"         //Find the Marker which points to current marker
                    "           IT      EQ                      \n"
                    "           BEQ     club%=                  \n"
                    "           LDRSH   R0, [R4, #2]            \n"
                    "           MOV     R2, R4                  \n"
                    "           ADD     R4, R4, R0, LSL #2      \n"
                    "           B       again%=                 \n"
                    "  club%=:  LDR     R0, [R1]                \n"
                    "           CMP     R0, #0                  \n"         //Currrent marker is free space, then next points to nothing
                    "           ITTT    EQ                      \n"
                    "           STRHEQ  R0, [R2, #2]            \n"
                    "           STREQ   R2, [R7]                \n"         //update fsll_end
                    "           BEQ     uexit%=                 \n"
                    "           LDRH    R0, [R2, #0]            \n"
                    "           TST     R0, %[free_mask]        \n"         //Is there any free marker
                    "           IT      EQ                      \n"
                    "           BEQ     uexit%=                 \n"
                    "           BIC     R0, R0, %[free_mask]    \n"
                    "           ADD     R3, R3, R0              \n"
                    "           LDRSH   R0, [R1, #2]            \n"
                    "           SUB     R4, R1, R2              \n"
                    "           MOV     R5, #4                  \n"
                    "           SDIV    R4, R5                  \n"
                    "           ADD     R4, R0                  \n"
                    "           LDR     R5, =0x8000             \n"
                    "           CMN     R4, R5                  \n"         //if((next - current) < -32768 && > 32768)
                    "           IT      LE                      \n"
                    "           BLE     uexit%=                 \n"         //else
                    "           CMP     R4, R5                  \n"
                    "           IT      GE                      \n"
                    "           BGE     uexit%=                 \n"         //else
                    "           STRH    R4, [R2, #2]            \n"
                    "           STRH    R3, [R2, #0]            \n"
                    "           MOV     R4, #0                  \n"
                    "           STR     R4, [R1]                \n"
                    "           SUB     R6, R6, #1              \n"         //Update blist_size
                    "  uexit%=: LDR     R5, =%[blist]           \n"
                    "           STR     R6, [R5]                \n"
                    "  exit%=:  MOV     R0, R6                  \n"
                    "           POP     {R1-R7, PC}             \n"
                    :
                    : [bstart] "i" (&bstart_addr), [address] "r" (address), [fslls] "i" (&fsll_start), \
                        [fslle] "i" (&fsll_end), [hbsz] "i" (HEAP_BOOKEEPING_SIZE), [blist] "i" (&blist_size), \
                        [free_mask]  "i" (FREE_HEADER_MASK), [hstart] "i" (&hstart_addr), [hrem] "i" (&heap_remaining)
                    : "r1", "r2", "r3", "r4", "r5", "r6"
    );

}
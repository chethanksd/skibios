//******************************************************************//
//    Kernel Code SKIBIOS RTOS                                      //
//            copyright@2018                                        //
//    Modified Date: 20-02-2018                                     //
//    Author: chetandev.ksd@gmail.com                               //
// Disclaimer:                                                      //
//******************************************************************//

#include <error.h>
#include <kernel.h>
#include <heap.h>
#include <share.h>
#include <process.h>
#include <interrupt.h>
#include <permissions.h>
#include <svc.h>
#include <mpu.h>
#include <kvar.h>
#include <param.h>
#include <access.h>
#include <regmap.h>
#include <svctable.h>
#include <defines.h>
#include <os_timer.h>

#define PROCESS_PRIO_CURRENT    0
#define PROCESS_PRIO_STASHED    1


/* Local Functions */
uint32_t svc_service_hand_over(uint32_t *svc_num, uint32_t *arguments);
uint32_t svc_service_device_reset(uint32_t *svc_num, uint32_t *arguments);
uint32_t svc_service_cpu_freq_update(uint32_t *svc_num, uint32_t *arguments);
uint32_t svc_service_start_scheduler(uint32_t *svc_num, uint32_t *arguments);

void mem_fault_handler(void);
void bus_fault_handler(void);
void scheduler(void);
static uint8_t mpu_init(void);

/* External Kernel Function Declarations */
extern void svc_handler(void);
extern void resolve_end(void);
extern void pendsv_handler(void);
extern void vector_table_relocate(void);
extern void BaseTask();

/* External Kernel Variables */
extern uint32_t _proc_heap_addr;
extern const uint32_t  mpu_table[];

/* FLASH Constants */
const uint32_t zero_ref = 0;
const uint32_t base_mutex = 1;


uint32_t kernel_init(void) {

    uint32_t error;
    uint32_t ticks;
    uint8_t i;

    /* Relocate the Vector Table */
    vector_table_relocate();

    /* Set 8 Levels of pre-emtive groups & No Sub Priority Settings
     * Change PRIGROUP constant below to the desired one for different levels and sub priority
     */
     HWREG(APINT) = APINT_VECKEY | PRIGROUP_8_1 | (HWREG(APINT) & 0x0000FFFF);
     HWREG(APINT) &= 0x0000FFFF;

    /* Register SVCall Interrupt Handler */
    HWREG(SRAM_START_ADDRESS + (INT_NUM_SVC * 4)) = (uint32_t) svc_handler;

    /* Disable Memory buffering if requested (Required for Debug) */
    #if(DISABLE_BUFFER >= 1)
    HWREG(ACTLR) |= ACTLR_DISWBUF;
    #endif

    error = os_timer_init(cpu_freq);

    if(error) {
        goto quit_error;
    }

    /* Register PendSV Interrupt Handler */
    HWREG(SRAM_START_ADDRESS + (INT_NUM_PENDSV * 4)) = (uint32_t) pendsv_handler;

    /* Setup Hightest priority for+ SVC Interrupt */
    HWREG(SYSPRI2) |= 0x00000000;

    /* Setup priority of Systick Interrupt */
    HWREG(SYSPRI3) |= 0xE0000000;

    /* Set Lowest Priority to PendSV Interrupt */
    HWREG(SYSPRI3) |= 0x00E00000;

    /* Initialize Process states*/
    for(i = 0; i < MAX_PROCESS_COUNT; i++) {
        state[i] = PROCESS_STATE_IDLE;
    }

    /* Get Process Stack start Address */
    pstack_addr = (uint32_t)&_proc_heap_addr;

    /* Initialize MPU */
    mpu_init(); 

    /* Initialize Base task
     * with these lines of code it should be obvious that
     * always 0th index process will be BaseTask
     * which implies if current_task = 0 means BaseTask
     * is in active state
     */
    process_init(&base_task);
    base_task.ptr_func = (void*)&BaseTask;
    base_task.priority = 0;
    process_start(&base_task);
    
    //base task should have all permissions
    permissions[0] = 0xFFFF;
    current_task = 0;

    /* Initialize Heap Memory */
    heap_init(); 

    /* Setup lower priotiry than SVC for all interrupts */
    for(i = 16; i < NUM_OF_INTERRUPTS; i++) {
        interrupt_set_priority(i, 0x20);
    }

quit_error:

    return ERROR_NONE;

}

void  __attribute__((naked)) start_scheduler(void) {

    uint32_t value;

    current_task = 0;
    first_start = true;

    value = PSP_Array[current_task] + 10 * 4;

    __asm volatile (" MSR PSP, %[value] \n"
    				" LDR R0, =0x3      \n"
    		        " MSR CONTROL, R0   \n"
    				" ISB			    \n"
                    :
                    : [value] "r" (value)
                );

    svc(START_SCHEDULER);

}

// svc services dispatched by svc_dispatch table

uint32_t svc_service_hand_over(uint32_t *svc_num, uint32_t *arguments) {

    if(self_kill == false) {
        // set the State of current process to sleep
        state[current_task] = PROCESS_STATE_SLEEP;

    } else {

        self_kill=false;

    }

    normal_schedule = false;

    HWREG(INTCTRL) |= (1 << INTCTRL_PENDSTSET);

    return ERROR_NONE;

}

uint32_t svc_service_start_scheduler(uint32_t *svc_num, uint32_t *arguments) {

    uint32_t value;
    uint32_t i;

    maxp_level = 127;
    value = (uint32_t)proc_obj[current_task]->ptr_func;

    //base task should have all permissions
    permissions[0] = 0xFFFF;

    //reset of the task will have by default no permissions
    for(i = 1; i < MAX_PROCESS_COUNT; i++) {
        permissions[i] = 0;
    }

    __asm volatile (" MRS R0,PSP        \n"
                    " ADD R0,R0,#24     \n"
                    " LDR R1,%0			\n"
                    " STR R1,[R0]		\n"
                    " ISB"
                    :
                    : "m" (value)
                    );
                    
    HWREG(STCTRL) |= SYSTICK_INT_ENABLE | SYSTICK_ENABLE;
    
    return ERROR_NONE;

}

uint32_t svc_service_cpu_freq_update(uint32_t *svc_num, uint32_t *arguments) {

    uint32_t error = ERROR_NONE;

    uint32_t new_cpu_freq = arguments[1];

    error = os_timer_config(new_cpu_freq);

    return error;

}

uint32_t svc_service_device_reset(uint32_t *svc_num, uint32_t *arguments) {

    /* Perform a software reset request.  This request causes the device to
    reset, no further code is executed */

    HWREG(APINT) = APINT_VECKEY | APINT_SYSRESETREQ;

    /* The device should have reset, so this should never be reached.  Just in
    case, loop forever. */

    while(1) {

    }

    return ERROR_NONE;

}

void scheduler(){

    uint8_t k;
    uint8_t level;
    uint8_t lptr;

    /* Stop the scheduler Temporarily */
    HWREG(STCTRL) &= SYSTICK_DISABLE & SYSTICK_INT_DISABLE;

    if(normal_schedule == true) {

        /* Set the State of current process to sleep */
        state[current_task] = PROCESS_STATE_SLEEP;

    } else {

        normal_schedule = true;

    }

    /* Check the Hibernating condition of all Hibernating function */
    k = next_task;

    do {

        if(state[k] == PROCESS_STATE_HIBERNATE_G) {

            if((*(op1[k]) - *(op2[k])) >= hib_value[k]) {

                if(priority_Array[k][PROCESS_PRIO_CURRENT] == max_level) {

                    hlc--;
                    alc++;

                }

                state[k] = PROCESS_STATE_SLEEP;

            }

        }

        if(state[k] == PROCESS_STATE_HIBERNATE_L) {

            if((*(op1[k]) - *(op2[k])) <= hib_value[k]) {

                if(priority_Array[k][PROCESS_PRIO_CURRENT] == max_level) {

                    hlc--;
                    alc++;

                }

                state[k] = PROCESS_STATE_SLEEP;

            }

        }

        k = k + jmp_list[k];

    } while(k != next_task);

    level = max_level;
    lptr = lstash_ptr - 1;

    /* Schedule Next Active Process Object */
    while(1) {

        next_task = next_task + jmp_list[next_task];

        if(current_task == next_task && enable_dws==true) {
            lptr--;
            level = level_stash[lptr];
        }

        if(state[next_task] == PROCESS_STATE_SLEEP && priority_Array[next_task][PROCESS_PRIO_CURRENT] == level) {
            break;
        }
        
    }

    state[next_task] = PROCESS_STATE_ACTIVE;

    /* Clear Current Register of Systick timer */
    HWREG(STCURRENT) = 0x00000000;

    /* Disable the scheduler if there is only one process i.e, only Base Proccess is present */

    if(alc==1 && hlc==0){
        HWREG(STCTRL) &= SYSTICK_DISABLE & SYSTICK_INT_DISABLE;
    }else{
        HWREG(STCTRL) |= SYSTICK_INT_ENABLE | SYSTICK_ENABLE;
    }

    if(level != max_level) {
        HWREG(STCTRL) |= SYSTICK_INT_ENABLE | SYSTICK_ENABLE;
    }

    /* Set PendSV Pending for context switching */
    HWREG(ICSR) |= 0x10000000;

}

uint8_t mpu_init() {

    uint8_t i;

     // Check if MPU Hardware is supported
    if(HWREG(MPUTYPE) == 0){
        return ERROR_MPU_UNSUPPORTED ;
    }

    // Make sure that pending memory transfers are done
    __asm("DMB");

    // Disable the MPU
    HWREG(MPUCTRL) = 0;

    // Setup Region 0 for Flash
    HWREG(MPURNR) = 0;
    HWREG(MPURBAR) = 0x00000000;
    HWREG(MPURASR) = MPU_CACHEABLE | (MPU_REGION_SIZE_512MB << 1) | (MPU_AP_PRO_URO << 24)  | MPU_REGION_ENABLE;

    // Setup Region 1 for SRAM
    HWREG(MPURNR) = 1;
    HWREG(MPURBAR) = 0x20000000;
    HWREG(MPURASR) = MPU_SHAREABLE | MPU_CACHEABLE | (MPU_REGION_SIZE_256KB << 1) | MPU_EXECUTE_NEVER | \
                        (MPU_AP_PRW_URW << 24) | MPU_REGION_ENABLE;

    // Setup Region 2 for Peripherals
    HWREG(MPURNR) = 2;
    HWREG(MPURBAR) = 0x40000000;
    HWREG(MPURASR) = MPU_SHAREABLE | MPU_BUFFERABLE | (MPU_REGION_SIZE_512MB << 1) |  \
                        (MPU_AP_PRW_URW << 24) | MPU_REGION_ENABLE;

    //Setup Region 6 for Kernel Section on SRAM
    HWREG(MPURNR) = 6;
    HWREG(MPURBAR) = 0x20000000;
    HWREG(MPURASR) = (KERNEL_REGION_SIZE << 1) | MPU_KERNEL_DEFAULT | (MPU_AP_PRW_URO << 24) | \
                        (KERNEL_REGION_SRD << 8) | MPU_REGION_ENABLE;

    //Setupt Region 7 for Process No. 1
    HWREG(MPURNR) = 7;
    HWREG(MPURBAR) = mpu_table[0];
    HWREG(MPURASR) = (MPU_PSTACK_SIZE << 1)  | MPU_KERNEL_DEFAULT | (MPU_AP_PRW_URW << 24) | MPU_REGION_ENABLE;

    // Clear the setup of lower unused MPU Regions
    for( i = 3; i < 6; i++){

        HWREG(MPURNR) = i;
        HWREG(MPURBAR) = 0;
        HWREG(MPURASR) = 0;

    }

    HWREG(MPURNR) = 0;

    // Register Memory Mangement Fault Handler
    HWREG(SRAM_START_ADDRESS + (INT_NUM_MEMFAULT * 4)) = (uint32_t) &mem_fault_handler;

    // Register Bus Fault Handler
    HWREG(SRAM_START_ADDRESS + (INT_NUM_BUSFAULT * 4)) = (uint32_t) &bus_fault_handler;

    // Enable Memory Management and Bus Fault Interrupt
    HWREG(SYSHNDCTRL) |= 0x00010000 | 0x00020000;

    // Enable MPU and the Background Region
    HWREG(MPUCTRL) = MPU_PRIVDEFENA | MPU_ENABLE;

    __asm("DSB");

    __asm("ISB");

    return ERROR_NONE;

}


void mem_fault_handler() {

    while(1) {
        //infinite loop
    }

}

void bus_fault_handler() {

    while(1) {
        //infinite loop
    }

}

/* Update CPU Frequency function */

uint8_t cpu_freq_update(uint32_t frequency) {

    uint8_t error;

    __asm volatile (" LDR R1, %[freq] \n"
            :
            : [freq] "m" (frequency)
            :
    );

    svc(CPU_FREQ);

    __asm volatile (" STR R0, %[err] \n"
        : [err] "=m" (error)
        :
        :
    );

    return error;

}

/* Device Reset Function */

uint8_t device_reset() {

    // only BaseTask have access to device_reset function
    if(current_task != 0) {
        return ERROR_ACCESS_DENIED;
    }

    svc(DEVICE_RESET);

    return ERROR_NONE;

}

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
#include <svc.h>
#include <mpu.h>
#include <kvar.h>
#include <param.h>
#include <access.h>
#include <regmap.h>
#include <svctable.h>

#define PROCESS_PRIO_CURRENT    0
#define PROCESS_PRIO_STASHED    1


volatile Process   KSECTION(.kbss) *proc_obj[MAX_PROCESS_COUNT];
volatile uint32_t  KSECTION(.kbss) PSP_Array[MAX_PROCESS_COUNT];
volatile uint8_t   KSECTION(.kbss) priority_Array[MAX_PROCESS_COUNT][2];
volatile uint32_t  KSECTION(.kbss) mutex_stash[MAX_PROCESS_COUNT];
volatile uint8_t   KSECTION(.kbss) level_stash[MAX_PROCESS_COUNT];
volatile int8_t    KSECTION(.kbss) jmp_list[MAX_PROCESS_COUNT];
volatile uint8_t   KSECTION(.kbss) state[MAX_PROCESS_COUNT];
volatile uint32_t  KSECTION(.kbss) process_id[MAX_PROCESS_COUNT];
volatile uint16_t  KSECTION(.kbss) permissions[MAX_PROCESS_COUNT];

volatile uint32_t  KSECTION(.kbss) *op1[MAX_PROCESS_COUNT];
volatile uint32_t  KSECTION(.kbss) *op2[MAX_PROCESS_COUNT];
volatile uint32_t  KSECTION(.kbss) hib_value[MAX_PROCESS_COUNT];

Process   KSECTION(.kbss) base_task;
uint32_t  KSECTION(.kbss) invocated_task;
uint32_t  KSECTION(.kbss) invocated_args;
uint32_t  KSECTION(.kbss) svc_exec;

bool KSECTION(.kdat) self_kill      = false;
bool KSECTION(.kdat) enable_dws     = true;
bool KSECTION(.kdat) first_start    = false;
bool KSECTION(.kdat) normal_schedule = true;

/* Local Functions */
uint32_t process_svc_request(uint32_t *svc_num, uint32_t *arguments);
uint32_t svc_service_hand_over(uint32_t *svc_num, uint32_t *arguments);
uint32_t svc_service_device_reset(uint32_t *svc_num, uint32_t *arguments);
uint32_t svc_service_cpu_freq_update(uint32_t *svc_num, uint32_t *arguments);
uint32_t svc_service_gheap_allocate(uint32_t *svc_num, uint32_t *arguments);
uint32_t svc_service_gheap_release(uint32_t *svc_num, uint32_t *arguments);
uint32_t svc_service_umpu_enable(uint32_t *svc_num, uint32_t *arguments);
uint32_t svc_service_umpu_disable(uint32_t *svc_num, uint32_t *arguments);
uint32_t svc_service_int_register(uint32_t *svc_num, uint32_t *arguments);
uint32_t svc_service_int_set_priority(uint32_t *svc_num, uint32_t *arguments);
uint32_t svc_service_int_disable(uint32_t *svc_num, uint32_t *arguments);
uint32_t svc_service_int_enable(uint32_t *svc_num, uint32_t *arguments);
void pendsv_handler(void);
void mem_fault_handler(void);
void bus_fault_handler(void);
static void scheduler(void);
static uint8_t mpu_init(void);

/* External Kernel Function Declarations */
extern void svc_handler(void);
extern void resolve_end(void);
extern void* search_free_marker(uint32_t size);
extern void* add_new_allocated_marker(uint32_t size);
extern uint32_t release_allocated_marker(uint32_t address);
extern void vector_table_relocate(void);
extern void BaseTask();

/* External Kernel Variables */
extern uint32_t blist_size;
extern uint32_t _proc_heap_addr;
extern uint32_t heap_remaining;
extern const uint32_t  mpu_table[];
extern const svc_dispatch_table_t svc_dispatch[];
extern const uint32_t TOTAL_SVC_COUNT ;

/* FLASH Constants */
const uint32_t zero_ref = 0;
const uint32_t base_mutex = 1;


uint8_t kernel_init(void) {

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

    /* Clear & Configure Reload Register of Systick timer */
    ticks = cpu_freq/PROCESS_PER_SEC;

    if(ticks >= 16777216) {
        return ERROR_SYSTICK_TICK_VALUE_OVERFLOW;
    }

    HWREG(STRELOAD) = 0x00000000;
    HWREG(STRELOAD) = SYSTICK_RELOAD_MASK & ticks;

    /* Clear Current Register of Systick timer */
    HWREG(STCURRENT) = 0x00000000;

    /* Set System Clock source for Systick timer */
    HWREG(STCTRL) |= SYSTICK_SYS_CLK;

    /* Register Systick Interrupt Handler */
    HWREG(SRAM_START_ADDRESS + (INT_NUM_SYSTICK * 4)) = (uint32_t) scheduler;

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
    base_task.pfnProcess = (void*)&BaseTask;
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

uint32_t process_svc_request(uint32_t *svc_num, uint32_t *arguments) {

    uint32_t value;
    uint32_t i;
    uint32_t j;

    uint32_t arg1 = arguments[0];
    uint32_t arg2 = arguments[1];
    uint32_t arg3 = arguments[2];
    uint32_t arg4 = arguments[3];

    // temp code to facilitate svc code transfer
    svc_exec = 1;
    for(i = 0; i < TOTAL_SVC_COUNT; i++) {
        if(svc_dispatch[i].svc_code == *svc_num) {
            svc_exec = 0;
            break;
        }
    }

    /* Process the requested service */
    switch(*svc_num) {

        case CREATE_PROCESS:

            /* Arugment assigments
             * arg1 = New Process Pointer
             * arg2 = New Argument
             */
        
            /* Find Idle Process Objects */
            if(process_count < MAX_PROCESS_COUNT) {

                for(i=0;i<MAX_PROCESS_COUNT;i++) {

                    if(state[i] == PROCESS_STATE_IDLE) {
                        break;
                    }

                }

                /* Check if priority level request is valid */
                if(((Process*)arg1)->priority > 127) {

                    ((Process*)arg1)->error = ERROR_INVALID_PRIORITY;
                    break;
                    
                }

                /* Check if Base process creation is attempted */
                if(((Process*)arg1)->priority == 0 && first_start == true) {

                    ((Process*)arg1)->error = ERROR_INVALID_PRIORITY;
                    break;

                }


                /* Dynamically initialize the Process Stack & Check if it is successful */
                if(!(PSP_Array[i] = pstack_addr + (i * PROCESS_STACK_SIZE * 4))) {

                    ((Process*)arg1)->error=ERROR_OUT_OF_MEMORY;
                    break;

                }


                /* Clear all locations of process stack */
                for( j = 0; j < PROCESS_STACK_SIZE; j++) {
                    *((uint32_t*)(PSP_Array[i] + j)) = 0;
                }

                /* calculation for jump list */
                if(i == 0) {

                    jmp_list[i] = 0;

                } else {

                    j = i - 1;
                    jmp_list[i] = jmp_list[j] - (i - j);
                    jmp_list[j] = i - j;
                    
                }

                proc_obj[i] = (Process*) arg1;
                proc_obj[i]->process_id = (i << 16) | (total_process_count + 1);
                process_id[i] = (i << 16) | (total_process_count + 1);

                /* Set Initial Process State */
                if((proc_obj[i]->hibernate & HIBERNATE_STATE_MASK) != HIBERNATE_STATE_MASK){
                    state[i] = PROCESS_STATE_SLEEP;
                }

                /* Set current process priority */
                priority_Array[i][PROCESS_PRIO_CURRENT] = proc_obj[i]->priority;
                
                //use arg3 as binary search index
                //use arg4 as lower pointer of binary search
                //use j as higher pointer of binary search
                j = lstash_ptr;
                arg4 = 0;

                while(arg4 <= j) {

                    arg3 = arg4 + ((j - arg4)/2);

                    if(level_stash[arg3] == priority_Array[i][PROCESS_PRIO_CURRENT]) {
                        goto proc_continue;
                    }

                    if(level_stash[arg3] < priority_Array[i][PROCESS_PRIO_CURRENT]) {
                        arg4 = arg3 + 1;
                    } else {
                        j = arg3 - 1;
                    }

                }
                
                for(j = lstash_ptr; j > arg3; j--) {
                    level_stash[j] = level_stash[j-1];
                }

                level_stash[arg3] = (uint32_t)priority_Array[i][PROCESS_PRIO_CURRENT];
                lstash_ptr++;
                
                proc_continue:

                PSP_Array[i] = ((unsigned int) (PSP_Array[i])) + ((PROCESS_STACK_SIZE - 1) * 4) - 18 * 4;

                /* Stack Initialization */
                HWREG(PSP_Array[i] + (10 << 2)) = (uint32_t)arg2;
                HWREG(PSP_Array[i] + (15 << 2)) = (uint32_t)&resolve_end;
                HWREG(PSP_Array[i] + (16 << 2)) = (unsigned int) proc_obj[i]->pfnProcess;
                HWREG(PSP_Array[i] + (17 << 2)) = 0x01000000;
                HWREG(PSP_Array[i] ) = 0xFFFFFFFD;
                HWREG(PSP_Array[i] + (1 << 2)) = 0x3;


                total_process_count++;
                process_count++;

                /* Find new max level (Priority Level) */
                for(j=0; j < MAX_PROCESS_COUNT; j++) {

                    if(priority_Array[j][PROCESS_PRIO_CURRENT] > max_level && state[j] != PROCESS_STATE_IDLE) {

                        max_level = priority_Array[j][PROCESS_PRIO_CURRENT];
                        alc=0;
                        hlc=0;
                        HWREG(STCTRL) |= SYSTICK_INT_ENABLE | SYSTICK_ENABLE;

                    }

                }

                /* Update Active & Hibernation Process Level Counts */
                if(priority_Array[i][PROCESS_PRIO_CURRENT] == max_level) {

                    if((state[i] & HIBERNATE_STATE_MASK) == HIBERNATE_STATE_MASK) {
                        hlc++;
                    } else {
                        alc++;
                    }

                }

                /* Start scheduler if there was only one process in  & start scheduler has been already executed */
                if((alc + hlc) == 2 && first_start == true) {
                    HWREG(STCTRL) |= SYSTICK_INT_ENABLE | SYSTICK_ENABLE;
                }

            } else {

                ((Process*)arg1)->error=ERROR_MAX_PROCESS_COUNT;

            }
            
        break;

        case KILL_PROCESS:

            /* Arugment assigments
             * arg1 = Process ID to kill
             * arg2 = Self Kill indicator (1 -> self kill)
             */

            /* calculate index of the process to kill */
            i = (arg1 >> 16);

            /* if i == 0 ==> killing of BaseTask is attempted
             * MCU should fault in this case
             */

            /* Re-calculate level_stash */
            for(j = 0; j < MAX_PROCESS_COUNT; j++) {

                if(i == j) {
                    continue;
                }

                if(priority_Array[j][PROCESS_PRIO_CURRENT] == priority_Array[i][PROCESS_PRIO_CURRENT]) {
                    goto pkill_continue;
                }

            }

            //use arg3 as found flag
            arg3 = 0;

            for(j = 0; j < lstash_ptr; j++) {

                if(level_stash[j] == priority_Array[i][PROCESS_PRIO_CURRENT]) {
                    arg3 = 1;
                }

                if(arg3 == 1) {
                    level_stash[j] = level_stash[j + 1];
                    continue;
                }

            }

            lstash_ptr--;

            pkill_continue:

            state[i] = PROCESS_STATE_IDLE;
            proc_obj[i]->process_id = 0;
            process_id[i] = 0;
            proc_obj[i] = 0;

            if(arg2 == 1) {

                self_kill = true;
                *svc_num = HAND_OVER;

            }
        
            process_count--;

            /* calculate jmp_list */
            for(j = 0; j < i; j++) {

                if(i == (j + jmp_list[j])) {
                    break;
                }

            }

            jmp_list[j] = jmp_list[j] + jmp_list[i];

            if(self_kill == true) {

                next_task = next_task + jmp_list[i];

            }

            jmp_list[i] = 0;
            
            /* Find new max level (Priority Level), alc & hlc */
            max_level = 0;
            alc = 0;
            hlc = 0;

            for(i=0; i < MAX_PROCESS_COUNT; i++) {

                if(priority_Array[i][PROCESS_PRIO_CURRENT] >= max_level && state[i] != PROCESS_STATE_IDLE) {
                    max_level = priority_Array[i][PROCESS_PRIO_CURRENT];
                }

            }

            for(i=0; i < MAX_PROCESS_COUNT; i++) {

                if(priority_Array[i][PROCESS_PRIO_CURRENT] == max_level) {

                    if((state[i] & HIBERNATE_STATE_MASK) == HIBERNATE_STATE_MASK) {
                        hlc++;
                    } else {
                        alc++;
                    }

                }

            }  

            /* Turn ON scheduler */
            HWREG(STCTRL) |= SYSTICK_INT_ENABLE | SYSTICK_ENABLE;

        break;

        case INVOKE_BASE:

            /* Arugment assigments
             * arg1 = Invocation argument
             */

            // base task cannot use this service
            if(current_task == 0) {
                arg1 = ERROR_BASE_PROCESS;
                break;
            }

            invocated_task = process_id[current_task];
            invocated_args = arg1;
            arg1 = (uint32_t)&base_mutex;

            *svc_num = PRIORITY_PROMOTE;

        break;


        case RELEASE_BASE:

            /* Arugment assigments
             * arg1 = invocation return error
             */

            // only base task can use this service
            if(current_task != 0) {
                arg1 = ERROR_ACCESS_DENIED;
                break;
            }

            // store invocation error on hib_value of task that invocated base
            for(i = 0; i < MAX_PROCESS_COUNT; i++) {
                if(process_id[i] == invocated_task) {
                    hib_value[i] = arg1;
                    arg1 = 0;
                    break;
                }
            }

            if(i == MAX_PROCESS_COUNT) {
                arg1 = ERROR_UNKNOWN_PROCESS_ID;
            }
            
            // clear invocate info and return error
            invocated_task = 0;
            invocated_args = 0;

            *svc_num = PRIORITY_DEMOTE;

        break;

        case GRANT_PERMISSION:

            /* Arugment assigments
             * arg1 = Process ID
             * arg2 = Permission
             */

            if(current_task != 0) {
                arg1 = ERROR_ACCESS_DENIED;
                break;
            }

            for(i = 0; i < MAX_PROCESS_COUNT; i++) {
                if(process_id[i] == arg1) {
                    permissions[i] = arg2;
                    break;
                }
            }

            if(i == MAX_PROCESS_COUNT) {
                arg1 =  ERROR_UNKNOWN_PROCESS_ID;
            } else {
                arg1 = 0;
            }

        break;

    }

    switch(*svc_num) {

        case PRIORITY_PROMOTE:

            /* Arugment assigments
             * arg1 = mutex address
             */

            if(maxp_level == 127) {

                maxp_level = max_level;
                max_level = 127;
                alcp = alc;
                hlcp = hlc;
                alc = 1;
                hlc = 0;

            }

            if(max_level == 255) {
                // ToDo : no priority level present after 255
                // Return error
            }
            
            max_level++;
            arg2 = *((uint32_t*)arg1) >> 16;

            mutex_stash[arg2] = arg1;

            priority_Array[arg2][PROCESS_PRIO_STASHED] = priority_Array[arg2][PROCESS_PRIO_CURRENT];
            priority_Array[arg2][PROCESS_PRIO_CURRENT] = max_level;

            state[current_task] = PROCESS_STATE_HOLD;
            op1[current_task] = (uint32_t*)arg1;

            // clear arg1 (required for INVOKE_BASE)
            arg1 = 0;

            // Enable scheduler and set normal scheule as false
            normal_schedule = false;
            HWREG(INTCTRL) |= (1 << INTCTRL_PENDSTSET);

        break;

        case PRIORITY_DEMOTE:

            priority_Array[current_task][PROCESS_PRIO_CURRENT] = priority_Array[current_task][PROCESS_PRIO_STASHED];
            priority_Array[current_task][PROCESS_PRIO_STASHED] = 0;

            max_level--;

            if(max_level == 127) {

                max_level = maxp_level;
                maxp_level = 127;
                alc = alcp;
                hlc = hlcp;

            }

            for(i = 0; i < MAX_PROCESS_COUNT; i++) {

                if(state[i] == PROCESS_STATE_HOLD) {

                    if(op1[i] == (uint32_t*)mutex_stash[current_task]){

                        state[i] = PROCESS_STATE_SLEEP;
                        break;

                    }
                }

            }

            mutex_stash[current_task] = 0;

            /* invoke HAND_OVER service call */
            *svc_num = HAND_OVER;

        break;

        case HIBERNATE:

            /* Arugment assigments
             * arg1 = Hibernation current process Number | reversed state
             * arg2 = op1 argument
             * arg3 = op2 argument
             * arg4 = value
             */

            if((arg1 & HIBEARNTE_REV_MASK) == HIBEARNTE_REV_MASK) {

                arg1 = arg1 & ~(HIBEARNTE_REV_MASK);
                state[current_task] = PROCESS_STATE_HIBERNATE_L;

            } else {

                state[current_task] = PROCESS_STATE_HIBERNATE_G;

            }
            
            op1[current_task] = (uint32_t*)arg2;
            op2[current_task] = (uint32_t*)arg3;
            hib_value[current_task] = arg4;


            if(priority_Array[current_task][PROCESS_PRIO_CURRENT] == max_level) {

                hlc++;
                alc--;

            }

            normal_schedule = false;

            HWREG(INTCTRL) |= (1 << INTCTRL_PENDSTSET);

            break;

        case START_SCHEDULER:

            maxp_level = 127;
        	value = (uint32_t)proc_obj[current_task]->pfnProcess;

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

            break;

        case INT_ENABLE:
#if 0
            /* Arugment assigments
             * arg1 = Interrupt Number
             */
            
            // check access permission of currrent_task for Interrupt control
            if((permissions[current_task] & (1 << PERMISSION_INTCTRL)) != (1 << PERMISSION_INTCTRL)) {
                arg1 = ERROR_ACCESS_DENIED;
                break;
            }
        
            /* Enable the general interrupt.*/
            HWREG(g_pui32EnRegs[(arg1 - 16) / 32]) = 1 << ((arg1 - 16) & 31);

            // return ERROR_NONE
            arg1 = ERROR_NONE;
#endif
            break;

        case INT_DISABLE:
#if 0
            /* Arugment assigments
             * arg1 = Interrupt Number
             */

            // check access permission of currrent_task for Interrupt control
            if((permissions[current_task] & (1 << PERMISSION_INTCTRL)) != (1 << PERMISSION_INTCTRL)) {
                arg1 = ERROR_ACCESS_DENIED;
                break;
            }

            /*  Disable the general interrupt. */
            HWREG(g_pui32Dii16Regs[(arg1 - 16) / 32]) = 1 << ((arg1 - 16) & 31);

            // return ERROR_NONE
            arg1 = ERROR_NONE;
#endif
            break;

        default:

            //ToDo: Determine actions to be done if process issues WRONG SVC_CALL

        break;

    }

    return arg1;

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

uint32_t svc_service_int_enable(uint32_t *svc_num, uint32_t *arguments) {
    
    uint32_t error = ERROR_NONE;
    uint32_t irq_no;

    irq_no = arguments[0];

    /* Arugment assigments
        * arg1 = Interrupt Number
        */
    
    // check access permission of currrent_task for Interrupt control
    if((permissions[current_task] & (1 << PERMISSION_INTCTRL)) != (1 << PERMISSION_INTCTRL)) {
        error = ERROR_ACCESS_DENIED;
        goto quit_error;
    }

    /* Enable the general interrupt.*/
    HWREG(g_pui32EnRegs[(irq_no - 16) / 32]) = 1 << ((irq_no - 16) & 31);

 quit_error:

    return error;

}

uint32_t svc_service_int_disable(uint32_t *svc_num, uint32_t *arguments) {

    uint32_t error = ERROR_NONE;
    uint32_t irq_no;

    irq_no = arguments[0];

    /* Arugment assigments
        * arg1 = Interrupt Number
        */

    // check access permission of currrent_task for Interrupt control
    if((permissions[current_task] & (1 << PERMISSION_INTCTRL)) != (1 << PERMISSION_INTCTRL)) {
        error = ERROR_ACCESS_DENIED;
        goto quit_error;
    }

    /*  Disable the general interrupt. */
    HWREG(g_pui32Dii16Regs[(irq_no - 16) / 32]) = 1 << ((irq_no - 16) & 31);

quit_error:

    return error;

}

uint32_t svc_service_int_set_priority(uint32_t *svc_num, uint32_t *arguments) {

    uint32_t error = ERROR_NONE;
    uint32_t irq_no;
    uint32_t priority;

    irq_no = arguments[0];
    priority = arguments[1];

    // check access permission of currrent_task for Interrupt control
    if((permissions[current_task] & (1 << PERMISSION_INTCTRL)) != (1 << PERMISSION_INTCTRL)) {
        error = ERROR_ACCESS_DENIED;
        goto quit_error;
    }

    HWREG(prioreg[(irq_no - 16)/4]) |= (PRIORITY_WRITE_MASK & (uint32_t)priority) << (8 * ((irq_no - 16) & 3));

quit_error:

    return error;

}

uint32_t svc_service_int_register(uint32_t *svc_num, uint32_t *arguments) {

    uint32_t error = ERROR_NONE;
    uint32_t irq_no;
    uint32_t handler_address;

    irq_no = arguments[0];
    handler_address = arguments[1];

    // check access permission of currrent_task for Interrupt control
    if((permissions[current_task] & (1 << PERMISSION_INTCTRL)) != (1 << PERMISSION_INTCTRL)) {
        error = ERROR_ACCESS_DENIED;
        goto quit_error;
    }

    /* Save the interrupt handler.*/
    HWREG(SRAM_START_ADDRESS + (irq_no * 4)) = handler_address;

quit_error:

    return error;

}


uint32_t svc_service_umpu_disable(uint32_t *svc_num, uint32_t *arguments) {

    uint32_t error = ERROR_NONE;
    uint32_t region;

    region = arguments[0];

    /* Arugment assigments
        * arg1 = Region
        */

    // Check if MPU Hardware is supported
    if(HWREG(MPUTYPE) == 0) {

        error = ERROR_MPU_UNSUPPORTED;
        goto quit_error;

    }

    // check Region value passed is valid
    if(region < 2 || region > 5) {

        error = ERROR_INVALID_MPU_REGION;
        goto quit_error;

    }

    // Make sure that pending memory transfers are done
    __asm("DMB");

    // Disable the MPU
    HWREG(MPUCTRL) = 0;

    // Setup User MPU Region
    HWREG(MPURNR) = region;
    HWREG(MPURBAR) = 0;
    HWREG(MPURASR) = 0;

    HWREG(MPURNR) = 0;

    // Enable MPU and the Background Region
    HWREG(MPUCTRL) = MPU_PRIVDEFENA | MPU_ENABLE;

    __asm("DSB");

    __asm("ISB");

quit_error:

    return error;

}

uint32_t svc_service_umpu_enable(uint32_t *svc_num, uint32_t *arguments) {

    uint32_t error = ERROR_NONE;
    uint32_t region;
    uint32_t address;
    uint32_t attributes;

    region = arguments[0];
    address = arguments[1];
    attributes = arguments[2];

    // Check if MPU Hardware is supported
    if(HWREG(MPUTYPE) == 0){

        error = ERROR_MPU_UNSUPPORTED ;
        goto quit_error;

    }

    // check Region value passed is valid
    if(region < 2 || region > 5) {

        error = ERROR_INVALID_MPU_REGION;
        goto quit_error;

    }

    if((address & 0x0000001F) != 0) {

        error = ERROR_INVALID_MPU_ADDRESS;
        goto quit_error;

    }

    // Make sure that pending memory transfers are done
    __asm("DMB");

    // Disable the MPU
    HWREG(MPUCTRL) = 0;

    // Setup User MPU Region
    HWREG(MPURNR) = region;
    HWREG(MPURBAR) = address;
    HWREG(MPURASR) = attributes | MPU_REGION_ENABLE;

    HWREG(MPURNR) = 0;

    // Enable MPU and the Background Region
    HWREG(MPUCTRL) = MPU_PRIVDEFENA | MPU_ENABLE;

    __asm("DSB");

    __asm("ISB");

quit_error:
    
    return error;

}

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

uint32_t svc_service_cpu_freq_update(uint32_t *svc_num, uint32_t *arguments) {

    uint32_t error = ERROR_NONE;
    int32_t systick_load;

    uint32_t new_cpu_freq = arguments[1];

    /* Clear & Configure Reload Register of Systick timer */
    systick_load = new_cpu_freq/PROCESS_PER_SEC;

    if(systick_load >= 16777216) {

        error = ERROR_SYSTICK_TICK_VALUE_OVERFLOW;
        goto quit_error;

    }

    cpu_freq = new_cpu_freq;

    HWREG(STRELOAD) = 0x00000000;
    HWREG(STRELOAD) = SYSTICK_RELOAD_MASK & systick_load;

    /* Clear Current Register of Systick timer */
    HWREG(STCURRENT) = 0x00000000;

quit_error:

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

/* pendsv_handler Function */
void __attribute__((naked)) pendsv_handler(void) {

    __asm volatile(
        " MRS R0, PSP             \n"           // Obtain PSP of current process
        " TST LR, #0x10           \n"
        " IT EQ                   \n"
        " VSTMDBEQ R0!, {S16-S31} \n"
        " MOV R2, LR              \n"
        " MRS R3, CONTROL         \n"
        " STMDB R0!,{R2-R11}      \n"
    	" LDR R1,=%[ct_task]	  \n"
        " LDR R2,[R1]             \n"
        " LDR R3,=%[psp_ptr]      \n"
        " STR R0,[R3,R2, LSL#2]   \n"
        " LDR R4,=%[nx_task]      \n"
    	" LDR R4,[R4]			  \n"
        " STR R4,[R1]             \n"
        " LDR R0,[R3,R4, LSL#2]   \n"
        " LDR R1, =0xE000ED94     \n"           //Disbale MPU
        " MOV R2, #0              \n"
        " STR R2, [R1]            \n"
        " LDR R2, =0xE000ED9C     \n"           //Load Next Process MPU settings from mpu_table (Assum R4 = nx_task)
        " LDR R4,=%[nx_task]      \n"
        " LDR R4, [R4]			  \n"
        " LDR R3, =%[mpu_table]   \n"
        " ADD R3, R3, R4, LSL #3  \n"
        " LDM R3, {R4-R5}         \n"
        " STM R2, {R4-R5}         \n"
        " MOV R2, #5              \n"           //Enable MPU (and Background Region)
        " STR R2, [R1]            \n"           
        " LDMIA R0!,{R2-R11}      \n"
        " MOV LR, R2              \n"
        " MSR CONTROL,R3          \n"
        " TST LR,#0x10            \n"
        " IT EQ                   \n"
        " VLDMIAEQ R0!,{S16-S31}  \n"
        " MSR PSP, R0             \n"
    	" DSB					  \n"
        " ISB                     \n"
    	" BX LR					  \n"
        : 
        : [psp_ptr] "i" (&PSP_Array[0]), [nx_task] "i" (&next_task), [ct_task] "i" (&current_task), \
            [psz] "i" (PROCESS_STACK_SIZE), [ps_addr] "i" (&pstack_addr), [mpu_table] "i" (&mpu_table[0])
		: "r0","r1","r2","r3","r4","lr"
    );

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

uint8_t user_mpu_enable(uint8_t region, uint32_t address, uint32_t attributes) {

    uint8_t error = 0;

    region = region + 3;

    __asm volatile (" LDRB R0, %[region] \n"
                    " LDR  R1, %[addr]   \n"
                    " LDR  R2, %[attr]   \n"
            :
            : [region] "m" (region), [addr] "m" (address), [attr] "m" (attributes)
            :
    );

    svc(ENABLE_UMPU);

    __asm volatile (" STR R0, %[err] \n"
        : [err] "=m" (error)
        :
        :
    );

    return error;

}

uint8_t user_mpu_disable(uint8_t region) {

    uint8_t error = 0;

    region = region + 3;

    __asm volatile (" LDRB R0, %[region] \n"
            :
            : [region] "m" (region)
            :
    );

    svc(DISABLE_UMPU);

    __asm volatile (" STR R0, %[err] \n"
        : [err] "=m" (error)
        :
        :
    );

    return error;


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

uint8_t base_run(void *args) {

    uint8_t error;

    if(current_task == 0) {
        return ERROR_BASE_PROCESS;
    }

    __asm volatile (" LDR R0, %[args] \n"
        : 
        :[args] "m" (args)
        :
    );

    svc(INVOKE_BASE);

    __asm volatile (" STR R0, %[err] \n"
        : [err] "=m" (error)
        :
        :
    );

    if(error == ERROR_NONE) {
        error = hib_value[current_task];
    }

    return error;

}

uint8_t base_release(uint8_t error) {

    if(current_task != 0) {
        return ERROR_ACCESS_DENIED;
    }

    __asm volatile (" LDR R0, %[err] \n"
        : 
        :[err] "m" (error)
        :
    );

    svc(RELEASE_BASE);

    __asm volatile (" STR R0, %[err] \n"
        : [err] "=m" (error)
        :
        :
    );

    return error;
    
}

uint8_t get_invocation_args(uint32_t *pid, void **args) {

    if(current_task != 0) {
        return ERROR_ACCESS_DENIED;
    }

    if(invocated_task == 0) {
        return ERROR_NO_INVOCATION;
    }

    *pid = invocated_task;
    *args = (void*)invocated_args;

    return ERROR_NONE;

}

uint8_t grant_permission(uint32_t pid, uint16_t permission) {

    uint8_t error = ERROR_NONE;

    if(current_task != 0) {
        return ERROR_ACCESS_DENIED;
    }

    __asm volatile (" LDR   R0, %[proc_id]  \n"
                    " LDRH  R1, %[prm]      \n"
        : 
        :[proc_id] "m" (pid), [prm] "m" (permission)
        :
    );

    svc(GRANT_PERMISSION);

    __asm volatile (" STR R0, %[err] \n"
        : [err] "=m" (error)
        :
        :
    );

    return error;

}

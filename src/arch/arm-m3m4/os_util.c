/*
 * os util code for arm-m3m4
 *
 *
 * author: chetandev.ksd@gmail.com
 *
 */


#include <os_util.h>
#include <regmap.h>
#include <access.h>
#include <error.h>
#include <param.h>
#include <kvar.h>
#include <os_support.h>
#include <interrupt.h>

// external functions
extern void svc_handler(void);
extern void pendsv_handler(void);
extern void resolve_end(void);

// external variables
extern const uint32_t  mpu_table[];

// local functions
static void mem_fault_handler(void);
static void bus_fault_handler(void);

uint8_t arch_kernel_init() {

    uint16_t i;

    // Set 8 Levels of pre-emtive groups & No Sub Priority Settings
    // Change PRIGROUP constant below to the desired one for different levels and sub priority
    //
    HWREG(APINT) = APINT_VECKEY | PRIGROUP_8_1 | (HWREG(APINT) & 0x0000FFFF);
    HWREG(APINT) &= 0x0000FFFF;

    // Register SVCall Interrupt Handler
    HWREG(KERNEL_START_ADDRESS + (INT_NUM_SVC * 4)) = (uint32_t) svc_handler;

    // Register PendSV Interrupt Handler
    HWREG(KERNEL_START_ADDRESS + (INT_NUM_PENDSV * 4)) = (uint32_t) pendsv_handler;

    // Setup Hightest priority for+ SVC Interrupt
    HWREG(SYSPRI2) |= 0x00000000;

    // Setup priority of Systick Interrupt
    HWREG(SYSPRI3) |= 0xE0000000;

    // Set Lowest Priority to PendSV Interrupt
    HWREG(SYSPRI3) |= 0x00E00000;

    // Disable Memory buffering if requested (Required for Debug)
    #if(DISABLE_BUFFER >= 1)
    HWREG(ACTLR) |= ACTLR_DISWBUF;
    #endif

    // Setup lower priotiry than SVC for all interrupts
    for(i = 16; i < NUM_OF_INTERRUPTS; i++) {
        interrupt_set_priority(i, 0x20);
    }

    return ERROR_NONE;

}


uint8_t arch_mpu_init() {

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
    HWREG(KERNEL_START_ADDRESS + (INT_NUM_MEMFAULT * 4)) = (uint32_t) &mem_fault_handler;

    // Register Bus Fault Handler
    HWREG(KERNEL_START_ADDRESS + (INT_NUM_BUSFAULT * 4)) = (uint32_t) &bus_fault_handler;

    // Enable Memory Management and Bus Fault Interrupt
    HWREG(SYSHNDCTRL) |= 0x00010000 | 0x00020000;

    // Enable MPU and the Background Region
    HWREG(MPUCTRL) = MPU_PRIVDEFENA | MPU_ENABLE;

    __asm("DSB");

    __asm("ISB");

    return ERROR_NONE;

}

uint8_t arch_task_stack_init(uint32_t task_index, uint32_t ptr_func, uint32_t proc_arg) {

    uint32_t *pheap_ptr;

    PSP_Array[task_index] = ((unsigned int) (PSP_Array[task_index])) + ((TASK_STACK_SIZE - 1) * 4) - 18 * 4;
    pheap_ptr = (uint32_t*)PSP_Array[task_index];

    pheap_ptr[0] = 0xFFFFFFFD;
    pheap_ptr[1] = 0x3;
    pheap_ptr[10] = (uint32_t)proc_arg;
    pheap_ptr[15] = (uint32_t)&resolve_end;
    pheap_ptr[16] = ptr_func;
    pheap_ptr[17] = 0x01000000;

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

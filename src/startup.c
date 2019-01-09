/*
 * skibios startup code
 *
 * author : chetandev.ksd@gmail.com
 *
 */

#include <stdint.h>
#include <kernel.h>
#include <access.h>


void skibios_boot(void);
static void nmi_handler(void);
static void fault_handler(void);
static void default_handler(void);

extern int main(void);

// linker constants
extern uint32_t _kstack_end_addr;

extern uint32_t __data_load__;
extern uint32_t __data_start__;
extern uint32_t __data_end__;
extern uint32_t __bss_start__;
extern uint32_t __bss_end__;
extern uint32_t __kdat_load__;
extern uint32_t __kdat_start__;
extern uint32_t __kdat_end__;
extern uint32_t __kbss_start__;
extern uint32_t __kbss_end__;


// vector table
__attribute__ ((section(".intvecs")))
void (* const g_pfnVectors[])(void) =
{
    (void (*)(void))&_kstack_end_addr,       
                                            // (New MSP Value) The initial stack pointer
    skibios_boot,                           // The reset handler
    nmi_handler,                            // The NMI handler
    fault_handler,                          // The hard fault handler
    default_handler,                        // The MPU fault handler
    default_handler,                        // The bus fault handler
    default_handler,                        // The usage fault handler
    0,                                      // Reserved
    0,                                      // Reserved
    0,                                      // Reserved
    0,                                      // Reserved
    default_handler,                        // SVCall handler
    default_handler,                        // Debug monitor handler
    0,                                      // Reserved
    default_handler,                        // The PendSV handler
    default_handler,                        // The SysTick handler
    default_handler,                        // GPIO Port A
    default_handler,                        // GPIO Port B
    default_handler,                        // GPIO Port C
    default_handler,                        // GPIO Port D
    default_handler,                        // GPIO Port E
	default_handler,                        // UART0 Rx and Tx
    default_handler,                        // UART1 Rx and Tx
    default_handler,                        // SSI0 Rx and Tx
    default_handler,                        // I2C0 Master and Slave
    default_handler,                        // PWM Fault
    default_handler,                        // PWM Generator 0
    default_handler,                        // PWM Generator 1
    default_handler,                        // PWM Generator 2
    default_handler,                        // Quadrature Encoder 0
    default_handler,                        // ADC Sequence 0
    default_handler,                        // ADC Sequence 1
    default_handler,                        // ADC Sequence 2
    default_handler,                        // ADC Sequence 3
    default_handler,                        // Watchdog timer
    default_handler,                        // Timer 0 subtimer A
    default_handler,                        // Timer 0 subtimer B
    default_handler,                        // Timer 1 subtimer A
    default_handler,                        // Timer 1 subtimer B
    default_handler,                        // Timer 2 subtimer A
    default_handler,                        // Timer 2 subtimer B
    default_handler,                        // Analog Comparator 0
    default_handler,                        // Analog Comparator 1
    default_handler,                        // Analog Comparator 2
    default_handler,                        // System Control (PLL, OSC, BO)
    default_handler,                        // FLASH Control
    default_handler,                        // GPIO Port F
    default_handler,                        // GPIO Port G
    default_handler,                        // GPIO Port H
    default_handler,                        // UART2 Rx and Tx
    default_handler,                        // SSI1 Rx and Tx
    default_handler,                        // Timer 3 subtimer A
    default_handler,                        // Timer 3 subtimer B
    default_handler,                        // I2C1 Master and Slave
    default_handler,                        // CAN0
    default_handler,                        // CAN1
    default_handler,                        // Ethernet
    default_handler,                        // hibernate
    default_handler,                        // USB0
    default_handler,                        // PWM Generator 3
    default_handler,                        // uDMA Software Transfer
    default_handler,                        // uDMA Error
    default_handler,                        // ADC1 Sequence 0
    default_handler,                        // ADC1 Sequence 1
    default_handler,                        // ADC1 Sequence 2
    default_handler,                        // ADC1 Sequence 3
    default_handler,                        // External Bus Interface 0
    default_handler,                        // GPIO Port J
    default_handler,                        // GPIO Port K
    default_handler,                        // GPIO Port L
    default_handler,                        // SSI2 Rx and Tx
    default_handler,                        // SSI3 Rx and Tx
    default_handler,                        // UART3 Rx and Tx
    default_handler,                        // UART4 Rx and Tx
    default_handler,                        // UART5 Rx and Tx
    default_handler,                        // UART6 Rx and Tx
    default_handler,                        // UART7 Rx and Tx
    default_handler,                        // I2C2 Master and Slave
    default_handler,                        // I2C3 Master and Slave
    default_handler,                        // Timer 4 subtimer A
    default_handler,                        // Timer 4 subtimer B
    default_handler,                        // Timer 5 subtimer A
    default_handler,                        // Timer 5 subtimer B
    default_handler,                        // FPU
    0,                                      // Reserved
    0,                                      // Reserved
    default_handler,                        // I2C4 Master and Slave
    default_handler,                        // I2C5 Master and Slave
    default_handler,                        // GPIO Port M
    default_handler,                        // GPIO Port N
    0,                                      // Reserved
    default_handler,                        // Tamper
    default_handler,                        // GPIO Port P (Summary or P0)
    default_handler,                        // GPIO Port P1
    default_handler,                        // GPIO Port P2
    default_handler,                        // GPIO Port P3
    default_handler,                        // GPIO Port P4
    default_handler,                        // GPIO Port P5
    default_handler,                        // GPIO Port P6
    default_handler,                        // GPIO Port P7
    default_handler,                        // GPIO Port Q (Summary or Q0)
    default_handler,                        // GPIO Port Q1
    default_handler,                        // GPIO Port Q2
    default_handler,                        // GPIO Port Q3
    default_handler,                        // GPIO Port Q4
    default_handler,                        // GPIO Port Q5
    default_handler,                        // GPIO Port Q6
    default_handler,                        // GPIO Port Q7
    default_handler,                        // GPIO Port R
    default_handler,                        // GPIO Port S
    default_handler,                        // SHA/MD5 0
    default_handler,                        // AES 0
    default_handler,                        // DES3DES 0
    default_handler,                        // LCD Controller 0
    default_handler,                        // Timer 6 subtimer A
    default_handler,                        // Timer 6 subtimer B
    default_handler,                        // Timer 7 subtimer A
    default_handler,                        // Timer 7 subtimer B
    default_handler,                        // I2C6 Master and Slave
    default_handler,                        // I2C7 Master and Slave
    default_handler,                        // HIM Scan Matrix Keyboard 0
    default_handler,                        // One Wire 0
    default_handler,                        // HIM PS/2 0
    default_handler,                        // HIM LED Sequencer 0
    default_handler,                        // HIM Consumer IR 0
    default_handler,                        // I2C8 Master and Slave
    default_handler,                        // I2C9 Master and Slave
    default_handler,                        // GPIO Port T
    default_handler,                        // Fan 1
    0,                                      // Reserved
};


// reset entry program
void skibios_boot(void) {

    uint32_t *pui32Src, *pui32Dest;

    // copy the data segment initializers from flash to SRAM.
    pui32Src = &__data_load__;
    for(pui32Dest = &__data_start__; pui32Dest < &__data_end__; )
    {
        *pui32Dest++ = *pui32Src++;
    }

    #if (ENABLE_SKIBIOS >= 1)
    pui32Src = &__kdat_load__;
    for(pui32Dest = &__kdat_start__; pui32Dest < &__kdat_end__; )
    {
        *pui32Dest++ = *pui32Src++;
    }

    __asm("             ldr     r0, =__kbss_start__     \n"
        "               ldr     r1, =__kbss_end__       \n"
        "               mov     r2, #0                  \n"
        "               .thumb_func                     \n"
        "   kloop:                                      \n"
        "               cmp     r0, r1                  \n"
        "               it      lt                      \n"
        "               strlt   r2, [r0], #4            \n"
        "               blt     kloop                   "
        );
    #endif

    // zero fill the bss segment.
    __asm("             ldr     r0, =__bss_start__      \n"
          "             ldr     r1, =__bss_end__        \n"
          "             mov     r2, #0                  \n"
          "             .thumb_func                     \n"
          " zero_loop:                                  \n"
          "             cmp     r0, r1                  \n"
          "             it      lt                      \n"
          "             strlt   r2, [r0], #4            \n"
          "             blt     zero_loop               "
          );

    /*
     * enable the floating-point unit.  This must be done here to handle the
     * case where main() uses floating-point and the function prologue saves
     * floating-point registers (which will fault if floating-point is not
     * enabled).  Any configuration of the floating-point unit using DriverLib
     * APIs must be done here prior to the floating-point unit being enabled.
     *
     * Note that this does not use DriverLib since it might not be included in
     * this project.
     */
    HWREG(0xE000ED88) = ((HWREG(0xE000ED88) & ~0x00F00000) | 0x00F00000);
    
    kernel_init();

    // call the application's entry point.
    main();

}

// non-maskable interrupt handler
static void nmi_handler(void) {

    while(1) {
        // infinite loop
    }

}

// fault exception handler
static void fault_handler(void) {

    while(1) {
        // infinite loop
    }

}

// default interrupt handler
static void default_handler(void) {

    while(1) {
        // infinite loop
    }

}

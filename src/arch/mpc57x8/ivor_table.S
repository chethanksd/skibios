/*
 * IVORx vector table and default handlers definition
 *
 * author: chetandev.ksd@gmail.com
 *
 */

.globl VTABLE
.globl IVOR0_Vector
.globl IVOR1_Vector
.globl IVOR2_Vector
.globl IVOR3_Vector
.globl IVOR4_Vector
.globl IVOR5_Vector
.globl IVOR6_Vector
.globl IVOR7_Vector
.globl IVOR8_Vector
.globl IVOR9_Vector
.globl IVOR10_Vector
.globl IVOR11_Vector
.globl IVOR12_Vector
.globl IVOR13_Vector
.globl IVOR14_Vector
.globl IVOR15_Vector

.section    .core_exceptions_table, "axv"

.align 4
VTABLE:
IVOR0_Vector:
            e_b   IVOR0_Handler

.align 4
IVOR1_Vector:
            e_b   IVOR1_Handler

.align 4
IVOR2_Vector:
            e_b   IVOR2_Handler

.align 4
IVOR3_Vector:
            e_b   IVOR3_Handler

.align 4
IVOR4_Vector:
            e_b   IVOR4_Handler

.align 4
IVOR5_Vector:
            e_b   IVOR5_Handler

.align 4
IVOR6_Vector:
            e_b   IVOR6_Handler

.align 4
IVOR7_Vector:
            e_b   IVOR7_Handler

.align 4
IVOR8_Vector:
            e_b  svc_handler

.align 4
IVOR9_Vector:
            e_b   IVOR9_Handler

.align 4
IVOR10_Vector:
            e_b   IVOR10_Handler

.align 4
IVOR11_Vector:
            e_b   IVOR11_Handler

.align 4
IVOR12_Vector:
            e_b   IVOR12_Handler

.align 4
IVOR13_Vector:
            e_b   IVOR13_Handler

.align 4
IVOR14_Vector:
            e_b   IVOR14_Handler

.align 4
IVOR15_Vector:
            e_b   IVOR15_Handler


.align  2
.weak IVOR0_Handler
.type IVOR0_Handler, %function
IVOR0_Handler:
    e_b       IVOR0_Handler
    .size IVOR0_Handler, . - IVOR0_Handler

.align  2
.weak IVOR1_Handler
.type IVOR1_Handler, %function
IVOR1_Handler:
    e_b       IVOR1_Handler
    .size IVOR1_Handler, . - IVOR1_Handler

.align  2
.weak IVOR2_Handler
.type IVOR2_Handler, %function
IVOR2_Handler:
    e_b       IVOR2_Handler
    .size IVOR2_Handler, . - IVOR2_Handler

.align  2
.weak IVOR3_Handler
.type IVOR3_Handler, %function
IVOR3_Handler:
    e_b       IVOR3_Handler
    .size IVOR3_Handler, . - IVOR3_Handler

// IVOR04 definition
// this interrupt handler the entry point for isr request in SW mode
// it reads vector from IACKR and executes it 
#ifdef __ghs__
    .section    .vletext, axv
    .vle
#endif

    .equ  INTC_IACKR, 0xfc040020  ;# Interrupt Acknowledge Register address for core 0
    .equ  INTC_EOIR,  0xfc040030   ;# End Of Interrupt Register address for core 0

    .align 4

IVOR4_Handler:
prologue:
    e_stwu      r1,-0x50 (r1)           ;# Create stack frame and store back chain
    e_stmvsrrw      0x0c (r1)           ;# Save SRR[0-1] (must be done before enabling MSR[EE])
    se_stw      r3, 0x08 (r1)           ;# Save working register (r3)
    e_lis       r3, INTC_IACKR@ha       ;# Save address  of INTC_IACKR in r3
    e_lwz       r3, INTC_IACKR@l(r3)    ;# Save contents of INTC_IACKR in r3 (this is vector table address)
    wrteei      1                       ;# Set MSR[EE] (must wait a couple clocks after reading IACKR)
    se_lwz      r3, 0x0(r3)             ;# Read ISR address from Interrupt Vector Table using pointer
    e_stmvsprw      0x14 (r1)           ;# Save CR, LR, CTR, XER
    se_mtLR     r3                      ;# Copy ISR address (from IACKR) to LR for next branch
    e_stmvgprw      0x24 (r1)           ;# Save GPRs, r[0,3-12]
    se_blrl                             ;# Branch to ISR, with return to next instruction (epilogue)

epilogue:
    e_lmvsprw       0x14 (r1)           ;# Restore CR, LR, CTR, XER
    e_lmvgprw       0x24 (r1)           ;# Restore GPRs, r[0,3-12]
    e_lis       r3, INTC_EOIR@ha        ;# Load upper half of INTC_EOIR address to r3
    mbar                                ;# Ensure prior clearing of interrupt flag conmpleted.
    wrteei      0                       ;# Disable interrupts
    e_stw       r3, INTC_EOIR@l(r3)     ;# Load lower half of INTC_EOIR address to r3 and
                                        ;# write contents of r3 to INTC_EOIR
    se_lwz      r3, 0x08 (r1)           ;# Restore working register (r3) (original value)
    e_lmvsrrw       0x0c (r1)           ;# Restore SRR[0-1]
    e_add16i    r1, r1, 0x50            ;# Reclaim stack space
    se_rfi                              ;# End of Interrupt Handler - re-enables interrupts


.align  2
.weak IVOR5_Handler
.type IVOR5_Handler, %function
IVOR5_Handler:
    e_b       IVOR5_Handler
    .size IVOR5_Handler, . - IVOR5_Handler

.align  2
.weak IVOR6_Handler
.type IVOR6_Handler, %function
IVOR6_Handler:
    e_b       IVOR6_Handler
    .size IVOR6_Handler, . - IVOR6_Handler

.align  2
.weak IVOR7_Handler
.type IVOR7_Handler, %function
IVOR7_Handler:
    e_b       IVOR7_Handler
    .size IVOR7_Handler, . - IVOR7_Handler

.align  2
.weak IVOR9_Handler
.type IVOR9_Handler, %function
IVOR9_Handler:
    e_b       IVOR9_Handler
    .size IVOR9_Handler, . - IVOR9_Handler

.align  2
.weak IVOR10_Handler
.type IVOR10_Handler, %function
IVOR10_Handler:
    e_b       IVOR10_Handler
    .size IVOR10_Handler, . - IVOR10_Handler

.align  2
.weak IVOR11_Handler
.type IVOR11_Handler, %function
IVOR11_Handler:
    e_b       IVOR11_Handler
    .size IVOR11_Handler, . - IVOR11_Handler

.align  2
.weak IVOR12_Handler
.type IVOR12_Handler, %function
IVOR12_Handler:
    e_b       IVOR12_Handler
    .size IVOR12_Handler, . - IVOR12_Handler

.align  2
.weak IVOR13_Handler
.type IVOR13_Handler, %function
IVOR13_Handler:
    e_b       IVOR13_Handler
    .size IVOR13_Handler, . - IVOR13_Handler

.align  2
.weak IVOR14_Handler
.type IVOR14_Handler, %function
IVOR14_Handler:
    e_b       IVOR14_Handler
    .size IVOR14_Handler, . - IVOR14_Handler

.align  2
.weak IVOR15_Handler
.type IVOR15_Handler, %function
IVOR15_Handler:
    e_b       IVOR15_Handler
    .size IVOR15_Handler, . - IVOR15_Handler

#ifndef REGMAP_H
#define REGMAP_H


// systick registers
#define STCTRL                  0xE000E010
#define STRELOAD                0xE000E014
#define STCURRENT               0xE000E018

// systick misc
#define SYSTICK_SYS_CLK         0x00000004
#define SYSTICK_INT_ENABLE      0x00000002
#define SYSTICK_ENABLE          0x00000001
#define SYSTICK_DISABLE         0xFFFFFFFE
#define SYSTICK_INT_DISABLE     0xFFFFFFFD

// systick reload register mask
#define SYSTICK_RELOAD_MASK     0x00FFFFFF

// mpu registers
#define MPUTYPE         0xE000ED90
#define MPUCTRL         0xE000ED94
#define MPURNR          0xE000ED98
#define MPURBAR         0xE000ED9C
#define MPURASR         0xE000EDA0
#define MPURBAR_A1      0xE000EDA4
#define MPURASR_A1      0xE000EDA8
#define MPURBAR_A2      0xE000EDAC
#define MPURASR_A2      0xE000EDB0
#define MPURBAR_A3      0xE000EDB4
#define MPURASR_A3      0xE000EDB8

// nvic registers
#define SYSHNDCTRL              0xE000ED24  // System Handler Control and State Register
#define FAULTSTAT               0xE000ED28  // Configurable Fault Status Register
#define HFAULTSTAT              0xE000ED2C  // HardFault Status Register
#define APINT                   0xE000ED0C  // Application Interrupt & control register
#define VTABLE                  0xE000ED08  // Vector Table Offset Register
#define INTCTRL                 0xE000ED04  // Interrupt Control & Status Register
#define ACTLR                   0xE000E008  // Auxialliary control


/* Alias Definition for compactibility with ARM Register naming convention */
#define VTOR    VTABLE
#define AIRCR   APINT
#define ICSR    INTCTRL
#define SHCSR   SYSHNDCTRL
#define CFSR    FAULTSTAT
#define HFSR    HFAULTSTAT

#define APINT_VECKEY            0x05FA0000  //Safe-Guard Key for APINT Register to avoid accidental writing
#define PRIORITY_WRITE_MASK     0x000000E0
#define ACTLR_DISWBUF           0x00000002
#define APINT_SYSRESETREQ       0x00000004  // System Reset Request

#define FAULT_MPU               4           // MPU fault
#define FAULT_BUS               5           // Bus fault
#define FAULT_USAGE             6           // Usage fault

#define NVIC_SYS_HND_CTRL_MEM       0x00010000  // Memory Management Fault Enable
#define NVIC_SYS_HND_CTRL_BUS       0x00020000  // Bus Fault Enable
#define NVIC_SYS_HND_CTRL_USAGE     0x00040000  // Usage Fault Enable

//INTCTRL Bit field definition
#define INTCTRL_PENDSTCLR       25
#define INTCTRL_PENDSTSET       26


/* Priority Group & Sub-Priority Splits*/
#define PRIGROUP_8_1    0x00000400  // Priority group 8.1 split
#define PRIGROUP_4_2    0x00000500  // Priority group 4.2 split
#define PRIGROUP_2_4    0x00000600  // Priority group 2.4 split
#define PRIGROUP_1_8    0x00000700  // Priority group 1.8 split

/* System Exception Priority Register */
#define SYSPRI1                 0xE000ED18  // System Handler Priority 1 (USAGE, BUS and MEM)
#define SYSPRI2                 0xE000ED1C  // System Handler Priority 2 (SVC)
#define SYSPRI3                 0xE000ED20  // System Handler Priority 3 (TICK, PENDSV, DEBUG)

#define NVIC_PRI0               0xE000E400  // Interrupt 0-3 Priority
#define NVIC_PRI1               0xE000E404  // Interrupt 4-7 Priority
#define NVIC_PRI2               0xE000E408  // Interrupt 8-11 Priority
#define NVIC_PRI3               0xE000E40C  // Interrupt 12-15 Priority
#define NVIC_PRI4               0xE000E410  // Interrupt 16-19 Priority
#define NVIC_PRI5               0xE000E414  // Interrupt 20-23 Priority
#define NVIC_PRI6               0xE000E418  // Interrupt 24-27 Priority
#define NVIC_PRI7               0xE000E41C  // Interrupt 28-31 Priority
#define NVIC_PRI8               0xE000E420  // Interrupt 32-35 Priority
#define NVIC_PRI9               0xE000E424  // Interrupt 36-39 Priority
#define NVIC_PRI10              0xE000E428  // Interrupt 40-43 Priority
#define NVIC_PRI11              0xE000E42C  // Interrupt 44-47 Priority
#define NVIC_PRI12              0xE000E430  // Interrupt 48-51 Priority
#define NVIC_PRI13              0xE000E434  // Interrupt 52-55 Priority
#define NVIC_PRI14              0xE000E438  // Interrupt 56-59 Priority
#define NVIC_PRI15              0xE000E43C  // Interrupt 60-63 Priority
#define NVIC_PRI16              0xE000E440  // Interrupt 64-67 Priority
#define NVIC_PRI17              0xE000E444  // Interrupt 68-71 Priority
#define NVIC_PRI18              0xE000E448  // Interrupt 72-75 Priority
#define NVIC_PRI19              0xE000E44C  // Interrupt 76-79 Priority
#define NVIC_PRI20              0xE000E450  // Interrupt 80-83 Priority
#define NVIC_PRI21              0xE000E454  // Interrupt 84-87 Priority
#define NVIC_PRI22              0xE000E458  // Interrupt 88-91 Priority
#define NVIC_PRI23              0xE000E45C  // Interrupt 92-95 Priority
#define NVIC_PRI24              0xE000E460  // Interrupt 96-99 Priority
#define NVIC_PRI25              0xE000E464  // Interrupt 100-103 Priority
#define NVIC_PRI26              0xE000E468  // Interrupt 104-107 Priority
#define NVIC_PRI27              0xE000E46C  // Interrupt 108-111 Priority
#define NVIC_PRI28              0xE000E470  // Interrupt 112-115 Priority
#define NVIC_PRI29              0xE000E474  // Interrupt 116-119 Priority
#define NVIC_PRI30              0xE000E478  // Interrupt 120-123 Priority
#define NVIC_PRI31              0xE000E47C  // Interrupt 124-127 Priority
#define NVIC_PRI32              0xE000E480  // Interrupt 128-131 Priority
#define NVIC_PRI33              0xE000E484  // Interrupt 132-135 Priority
#define NVIC_PRI34              0xE000E488  // Interrupt 136-139 Priority

#define NVIC_DIS0               0xE000E180  // Interrupt 0-31 Clear Enable
#define NVIC_DIS1               0xE000E184  // Interrupt 32-63 Clear Enable
#define NVIC_DIS2               0xE000E188  // Interrupt 64-95 Clear Enable
#define NVIC_DIS3               0xE000E18C  // Interrupt 96-127 Clear Enable

#define NVIC_EN0                0xE000E100  // Interrupt 0-31 Set Enable
#define NVIC_EN1                0xE000E104  // Interrupt 32-63 Set Enable
#define NVIC_EN2                0xE000E108  // Interrupt 64-95 Set Enable
#define NVIC_EN3                0xE000E10C  // Interrupt 96-127 Set Enable

#define NVIC_PEND0              0xE000E200  // Interrupt 0-31 Set Pending
#define NVIC_PEND1              0xE000E204  // Interrupt 32-63 Set Pending
#define NVIC_PEND2              0xE000E208  // Interrupt 64-95 Set Pending
#define NVIC_PEND3              0xE000E20C  // Interrupt 96-127 Set Pending

#define NVIC_UNPEND0            0xE000E280   // Interrupt 0-31 Set Unpend
#define NVIC_UNPEND1            0xE000E284   // Interrupt 32-63 Set Unpend
#define NVIC_UNPEND2            0xE000E288   // Interrupt 64-95 Set Unpend
#define NVIC_UNPEND3            0xE000E28C   // Interrupt 96-127 Set Unpend


#define INT_NUM_SYSTICK     15          //Systick Count Zero
#define INT_NUM_SVC         11          //SVCall 
#define INT_NUM_PENDSV      14          //PendSV
#define INT_NUM_MEMFAULT    4           //MemManage Fault
#define INT_NUM_BUSFAULT    5           //Bus Fault

#endif
# skibios
small RTOS developed as an hobby project than runs on any MCU supporting ARM cortex M3/M4  architecture.

# features

-  Pre-emptive scheduler with priority inversion
-  Kernel resources like vector table, main stack, kernel variables and heap makers are protected by MPU
-  Process stack are also protected by MPU
-  2 layers of security (1 layer of hardware based with MPU and 1 layer of software defined security)
-  3 user defined MPU regions
-  supports on the go isr update with Vector Table proetected by MPU
-  supports meta data based heap management api's
-  mutex locks, semaphores and spiclocks for resource sharing

# SRAM allocation

the SRAM is divided into various regions - Kernel Region, User region and Heap. The kernel occupies from the starting of SRAM followed user region where memory allocation for application program is done. The remaining SRAM is utilized as heap. The design of this RTOS is centered upon maximum and efficient use of MPU peripheral and hence this RTOS has maximum limit the process can be running at a time.

  Vector Table
-------------------
  Kernel Variables
--------------------
  Main Stack
--------------------
  process Stack 1st
-------------------
    .
    .
    .
--------------------
  Process Stack nth
--------------------
  Heap Marker Bookeeping
--------------------
  .bss and .dat region (Application and device driver memory region)
  
--------------------
  heap region


# compiling

use skibios.exe to generate obj files. See Makefile for sample invovation. You have to define BIN to locate arm-gcc bin folder. There are few configuration variables in makefile which you can ignore for all mcu that has SRAM size greater than or equal to 32KB. Here are some details of these configuration variables:

1) KERNEL_REGION_SIZE: Define total SRAM size in KB that kernel region can occupy. Here are the supported values : 8, 10, 12, 14, 16, 20, 24, 28, 32, 40, 48, 56, 64.

2) UPPER_REGION_SIZE: Basically Vector table + Kernel Variables + Main stack constitutes Upper Kernel Region. Using this variable we can define maximum size the upper kernel region can take. Here are the supported values: 3, 4, 5

3) GHMB_REGION_SIZE: Define total size in KB that Heap Marker Bookeeping region can take. Supported values are : 1, 2

4) PROCCESS_STACK_SIZE: Define in Bytes the fixed stack size of each user process. Supported values are: 512, 1024

    MAX_PROCESS_COUNT = (((KERNEL_REGION_SIZE - (UPPER_REGION_SIZE + GHMB_REGION_SIZE)) * 1024)/PROCESS_STACK_SIZE
    
    for the given configuration in makefile,
    
    MAX_PROCESS_COUNT = ((16 - (3 + 1)) * 1024)/512
                      =  (12 * 1024)/512
                      =   24  Process can be allocated at a time
                      
5) PROCESS_PER_SECOND: define scheduler speed per second.

Rest of the configuration ignore it for now. Detailed documentation for skibios is on progress.

Don't define your own startup or use any other startup codes. You must linker script supplied in this repository only as an input for linking


  
  
  
    

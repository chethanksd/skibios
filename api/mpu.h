#ifndef MPU_H
#define MPU_H

#include <access.h>


// region size 
#define MPU_REGION_SIZE_32B     0b00100
#define MPU_REGION_SIZE_64B     0b00101
#define MPU_REGION_SIZE_128B    0b00110
#define MPU_REGION_SIZE_256B    0b00111
#define MPU_REGION_SIZE_512B    0b01000
#define MPU_REGION_SIZE_1KB     0b01001
#define MPU_REGION_SIZE_2KB     0b01010
#define MPU_REGION_SIZE_4KB     0b01011
#define MPU_REGION_SIZE_8KB     0b01100
#define MPU_REGION_SIZE_16KB    0b01101
#define MPU_REGION_SIZE_32KB    0b01110
#define MPU_REGION_SIZE_64KB    0b01111
#define MPU_REGION_SIZE_128KB   0b10000
#define MPU_REGION_SIZE_256KB   0b10001
#define MPU_REGION_SIZE_512KB   0b10010
#define MPU_REGION_SIZE_1MB     0b10011
#define MPU_REGION_SIZE_2MB     0b10100
#define MPU_REGION_SIZE_4MB     0b10101
#define MPU_REGION_SIZE_8MB     0b10110
#define MPU_REGION_SIZE_16MB    0b10111
#define MPU_REGION_SIZE_32MB    0b11000
#define MPU_REGION_SIZE_64MB    0b11001
#define MPU_REGION_SIZE_128MB   0b11010
#define MPU_REGION_SIZE_256MB   0b11011
#define MPU_REGION_SIZE_512MB   0b11100
#define MPU_REGION_SIZE_1GB     0b11101
#define MPU_REGION_SIZE_2GB     0b11110
#define MPU_REGION_SIZE_4GB     0b11111

//MPUCTRL register fields
#define MPU_PRIVDEFENA          0x00000004
#define MPU_HFNMIENA            0x00000002
#define MPU_ENABLE              0x00000001

//MPURASR register fields
#define MPU_SHAREABLE           0x00040000
#define MPU_CACHEABLE           0x00020000
#define MPU_BUFFERABLE          0x00010000
#define MPU_EXECUTE_NEVER       0x10000000
#define MPU_REGION_ENABLE       0x00000001

#define MPU_AP_PNA_UNA          0b000
#define MPU_AP_PRW_UNA          0b001
#define MPU_AP_PRW_URO          0b010
#define MPU_AP_PRW_URW          0b011
#define MPU_AP_PRO_UNA          0b101
#define MPU_AP_PRO_URO          0b110

#define MPU_KERNEL_DEFAULT     (MPU_SHAREABLE | MPU_CACHEABLE | MPU_EXECUTE_NEVER) 

#define USER_MPU_0              0
#define USER_MPU_1              1
#define USER_MPU_2              2

#endif

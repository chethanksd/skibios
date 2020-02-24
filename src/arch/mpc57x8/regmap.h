#ifndef _REGMAP_H_
#define _REGMAP_H_

#include <stdint.h>
#include <stdbool.h>

#define NUMBER_OF_CORES                 3
#define STM_0_CHANNEL_0_INT             36
#define INTERRUPT_PRIORITY_DEFAULT      15


#ifdef __cplusplus
extern "C" {
#endif


/*
 * STM Register set definition
 * 
 */                   

typedef struct {                        // STM Control Register
    volatile uint32_t   :16;
    volatile uint32_t  CPS:8;
    volatile uint32_t   :3;
    volatile uint32_t  CSL:1;
    volatile uint32_t   :2;
    volatile uint32_t  FRZ:1;
    volatile uint32_t  TEN:1;
} CR_T;
                        
typedef struct {                        // STM Count Register
    volatile uint32_t  CNT:32;
} CNT_T;

typedef struct {                        // STM Channel Control Register
    volatile uint32_t   :31;
    volatile uint32_t  CEN:1;
} CCR_T;
                            
  
typedef struct {                        // STM Channel Interrupt Register
    volatile uint32_t   :31;
    volatile uint32_t  CIF:1; 
} CIR_T;
                            
  
typedef struct {                        // STM Channel Compare Register
    volatile uint32_t  CMP:32;
} CMP_T;

typedef struct {
    CR_T CR;
    CNT_T CNT;
    uint8_t RESERVED0[8];
    struct {
        CCR_T CCR;
        CIR_T CIR;
        CMP_T CMP;
        uint8_t RESERVED1[4];
    } CHANNEL[4];
} STM_T;

// STM - Peripheral instance base addresses
// Peripheral STM base address
#define STM_0_BASE                              (0xFC068000UL)
// Peripheral STM base pointer
#define STM                                     ((STM_T *)STM_0_BASE)


// INTC - Size of Registers Arrays
#define INTC_SSCIR_COUNT                         24U
#define INTC_PSR_COUNT                           1024U

typedef struct {                                        // INTC Block Configuration Register
  volatile uint32_t  :19;
  volatile uint32_t HVEN3:1;
  volatile uint32_t  :3;
  volatile uint32_t HVEN2:1;
  volatile uint32_t  :3;
  volatile uint32_t HVEN1:1;
  volatile uint32_t  :3;
  volatile uint32_t HVEN0:1;
} BCR_T;

typedef struct {                                        // INTC Master Protection Register
  volatile uint32_t  :26;
  volatile uint32_t ID:2;
  volatile uint32_t  :3;
  volatile uint32_t MPROT:1;
} MPROT_T;

typedef struct {                                        // INTC Current Priority Register for Processor n
  volatile uint32_t  :28;
  volatile uint32_t PRI:4;
} CPR_T;

typedef struct {                                        // INTC Interrupt Acknowledge Register for Processor n
  volatile uint32_t VTBA:20;
  volatile uint32_t INTVEC:10;
  volatile uint32_t  :2;
} IACKR_T;

typedef struct {                                        // INTC End Of Interrupt Register for Processor n
  volatile uint32_t EOI:32;
} EOIR_T;

typedef struct {                                        // INTC Software Set/Clear Interrupt Register
  volatile uint8_t  :6;
  volatile uint8_t SET:1;
  volatile uint8_t CLR:1;
} SSCIR_T;

typedef struct {                                        // INTC Priority Select Register
  volatile uint16_t PRC_SELN:4;
  volatile uint16_t  :3;
  volatile uint16_t SWTN:1;
  volatile uint16_t  :4;
  volatile uint16_t PRIN:4;
} PSR_T;

// INTC - Register Layout Typedef
typedef struct {
  BCR_T               BCR;                               // Block Configuration Register, offset: 0x0
  MPROT_T             MPROT;                             // Master Protection Register, offset: 0x4
  volatile uint8_t    RESERVED_0[8];
  CPR_T               CPR[3];                            // Current Priority Register, offset: 0x10
  volatile uint8_t    RESERVED_1[4]; 
  IACKR_T             IACKR[3];                          // Interrupt Acknowledge Register, offset: 0x20
  volatile uint8_t    RESERVED_2[4];
  EOIR_T              EOIR[3];                           // End Of Interrupt Register, offset: 0x30
  volatile uint8_t    RESERVED_3[4];
  SSCIR_T             SSCIR[INTC_SSCIR_COUNT];           // Software Set/Clear Interrupt Register, array offset: 0x40, array step: 0x1
  volatile uint8_t    RESERVED_4[8];
  PSR_T               PSR[INTC_PSR_COUNT];               // Priority Select Register, array offset: 0x60, array step: 0x2
} INTC_Type;

// INTC - Peripheral instance base addresses
// Peripheral INTC base address
#define INTC_BASE                                (0xFC040000U)
// Peripheral INTC base pointer
#define INTC                                     ((INTC_Type *)INTC_BASE)
// Array initializer of INTC peripheral base addresses
#define INTC_BASE_ADDRS                          { INTC_BASE }
// Array initializer of INTC peripheral base pointers
#define INTC_BASE_PTRS                           { INTC }


#ifdef __cplusplus
}
#endif

#endif

// clock.h
// Reset and Clock Control (RCC) driver for the STM32L432KC.
// Register map hand-written from RM0394 (STM32L41xxx/42xxx/43xxx/44xxx/45xxx/46xxx
// Reference Manual), section 6.4. 

#ifndef CLOCK_H
#define CLOCK_H

#include <stdint.h>

///////////////////////////////////////////////////////////////////////////////
// Base address
///////////////////////////////////////////////////////////////////////////////

#define RCC_BASE (0x40021000UL)

///////////////////////////////////////////////////////////////////////////////
// Register structure (only registers up to APB2ENR are needed for this lab)
///////////////////////////////////////////////////////////////////////////////

typedef struct {
  volatile uint32_t CR;          // 0x00 Clock control
  volatile uint32_t ICSCR;       // 0x04 Internal clock sources calibration
  volatile uint32_t CFGR;        // 0x08 Clock configuration
  volatile uint32_t PLLCFGR;     // 0x0C PLL configuration
  volatile uint32_t PLLSAI1CFGR; // 0x10 PLLSAI1 configuration
  uint32_t          RESERVED0;   // 0x14
  volatile uint32_t CIER;        // 0x18 Clock interrupt enable
  volatile uint32_t CIFR;        // 0x1C Clock interrupt flag
  volatile uint32_t CICR;        // 0x20 Clock interrupt clear
  uint32_t          RESERVED1;   // 0x24
  volatile uint32_t AHB1RSTR;    // 0x28
  volatile uint32_t AHB2RSTR;    // 0x2C
  volatile uint32_t AHB3RSTR;    // 0x30
  uint32_t          RESERVED2;   // 0x34
  volatile uint32_t APB1RSTR1;   // 0x38
  volatile uint32_t APB1RSTR2;   // 0x3C
  volatile uint32_t APB2RSTR;    // 0x40
  uint32_t          RESERVED3;   // 0x44
  volatile uint32_t AHB1ENR;     // 0x48
  volatile uint32_t AHB2ENR;     // 0x4C
  volatile uint32_t AHB3ENR;     // 0x50
  uint32_t          RESERVED4;   // 0x54
  volatile uint32_t APB1ENR1;    // 0x58
  volatile uint32_t APB1ENR2;    // 0x5C
  volatile uint32_t APB2ENR;     // 0x60
} RCC_TypeDef;

#define RCC ((RCC_TypeDef *) RCC_BASE)

///////////////////////////////////////////////////////////////////////////////
// Bit definitions
///////////////////////////////////////////////////////////////////////////////

// RCC_CR
#define RCC_CR_MSION          (1UL << 0)
#define RCC_CR_MSIRDY         (1UL << 1)
#define RCC_CR_MSIRGSEL       (1UL << 3)
#define RCC_CR_MSIRANGE_Pos   4
#define RCC_CR_MSIRANGE_Msk   (0xFUL << RCC_CR_MSIRANGE_Pos)
#define RCC_CR_MSIRANGE_4MHZ  (0x6UL << RCC_CR_MSIRANGE_Pos)  // Range 6 = 4 MHz

// RCC_CFGR
#define RCC_CFGR_SW_Msk       (0x3UL << 0)   // System clock switch (00 = MSI)
#define RCC_CFGR_SWS_Msk      (0x3UL << 2)   // System clock switch status
#define RCC_CFGR_HPRE_Msk     (0xFUL << 4)   // AHB prescaler  (0xxx = /1)
#define RCC_CFGR_PPRE1_Msk    (0x7UL << 8)   // APB1 prescaler (0xx  = /1)
#define RCC_CFGR_PPRE2_Msk    (0x7UL << 11)  // APB2 prescaler (0xx  = /1)

// RCC_AHB2ENR
#define RCC_AHB2ENR_GPIOAEN   (1UL << 0)

// RCC_APB2ENR
#define RCC_APB2ENR_TIM15EN   (1UL << 16)
#define RCC_APB2ENR_TIM16EN   (1UL << 17)

///////////////////////////////////////////////////////////////////////////////
// Clock constants
///////////////////////////////////////////////////////////////////////////////

// SYSCLK = HCLK = PCLK2 = 4 MHz (MSI range 6, all bus prescalers /1).
// With the APB2 prescaler at /1
#define SYSCLK_HZ  4000000UL
#define TIMCLK_HZ  SYSCLK_HZ

///////////////////////////////////////////////////////////////////////////////
// Function prototypes
///////////////////////////////////////////////////////////////////////////////

void configureSystemClock(void);

#endif // CLOCK_H

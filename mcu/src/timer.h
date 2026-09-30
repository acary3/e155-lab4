// timer.h
// Driver for general-purpose timers TIM15 and TIM16 on the STM32L432KC.
// Timers I used:
//   TIM16 -> tone generator. PWM mode 1, 50% duty, output on TIM16_CH1 (PA6, AF14).
//   TIM15 -> blocking millisecond delay used for note durations.

#ifndef TIMER_H
#define TIMER_H

#include <stdint.h>

///////////////////////////////////////////////////////////////////////////////
// Base addresses
///////////////////////////////////////////////////////////////////////////////

#define TIM15_BASE (0x40014000UL)
#define TIM16_BASE (0x40014400UL)

///////////////////////////////////////////////////////////////////////////////
// Register structure (common layout of TIM15/TIM16; SMCR and CCR2 exist only
// on TIM15 and are reserved on TIM16)
///////////////////////////////////////////////////////////////////////////////

typedef struct {
  volatile uint32_t CR1;        // 0x00 Control 1
  volatile uint32_t CR2;        // 0x04 Control 2
  volatile uint32_t SMCR;       // 0x08 Slave mode control (TIM15)
  volatile uint32_t DIER;       // 0x0C DMA/interrupt enable
  volatile uint32_t SR;         // 0x10 Status
  volatile uint32_t EGR;        // 0x14 Event generation
  volatile uint32_t CCMR1;      // 0x18 Capture/compare mode 1
  uint32_t          RESERVED0;  // 0x1C
  volatile uint32_t CCER;       // 0x20 Capture/compare enable
  volatile uint32_t CNT;        // 0x24 Counter
  volatile uint32_t PSC;        // 0x28 Prescaler
  volatile uint32_t ARR;        // 0x2C Auto-reload
  volatile uint32_t RCR;        // 0x30 Repetition counter
  volatile uint32_t CCR1;       // 0x34 Capture/compare 1
  volatile uint32_t CCR2;       // 0x38 Capture/compare 2 (TIM15)
  uint32_t          RESERVED1[2]; // 0x3C, 0x40
  volatile uint32_t BDTR;       // 0x44 Break and dead-time
  volatile uint32_t DCR;        // 0x48 DMA control
  volatile uint32_t DMAR;       // 0x4C DMA address for full transfer
  volatile uint32_t OR1;        // 0x50 Option 1
  uint32_t          RESERVED2[3]; // 0x54-0x5C
  volatile uint32_t OR2;        // 0x60 Option 2
} TIM_TypeDef;

#define TIM15 ((TIM_TypeDef *) TIM15_BASE)
#define TIM16 ((TIM_TypeDef *) TIM16_BASE)

///////////////////////////////////////////////////////////////////////////////
// Bit definitions
///////////////////////////////////////////////////////////////////////////////

#define TIM_CR1_CEN        (1UL << 0)   // Counter enable
#define TIM_CR1_ARPE       (1UL << 7)   // Auto-reload preload enable
#define TIM_SR_UIF         (1UL << 0)   
#define TIM_EGR_UG         (1UL << 0)   
#define TIM_CCMR1_CC1S_Msk (0x3UL << 0)                  
#define TIM_CCMR1_OC1PE    (1UL << 3)                   
#define TIM_CCMR1_OC1M_Msk ((0x7UL << 4) | (1UL << 16))  
#define TIM_CCMR1_OC1M_PWM1 (0x6UL << 4)          
#define TIM_CCER_CC1E      (1UL << 0)   
#define TIM_CCER_CC1P      (1UL << 1)   
#define TIM_BDTR_MOE       (1UL << 15)  // Main output enable 

///////////////////////////////////////////////////////////////////////////////
// Timing constants (see writeup for derivations)
///////////////////////////////////////////////////////////////////////////////

// Tone timer: 4 MHz / (PSC + 1) = 1 MHz 
#define TONE_TICK_HZ        1000000UL
// Lowest integer frequency whose rounded period fits in the 16-bit counter
#define TONE_MIN_HZ         16
// Highest frequency guaranteed within 1%: period >= 50 ticks -> error <= 0.5/50
#define TONE_MAX_HZ         20000
#define DELAY_TICK_HZ       10000UL
#define DELAY_TICKS_PER_MS  (DELAY_TICK_HZ / 1000UL)
#define DELAY_MAX_CHUNK_MS  6553UL

///////////////////////////////////////////////////////////////////////////////
// Function prototypes
///////////////////////////////////////////////////////////////////////////////

void initPitchTimerPWM(TIM_TypeDef *TIMx);
void setPitchFrequency(TIM_TypeDef *TIMx, int freqHz);

void initDurationTimer(TIM_TypeDef *TIMx);
void delayMilliseconds(TIM_TypeDef *TIMx, uint32_t ms);

#endif // TIMER_H

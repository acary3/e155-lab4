// timer.c
// Tone generation (PWM) and millisecond delays using TIM15/TIM16.

#include "timer.h"
#include "clock.h"

///////////////////////////////////////////////////////////////////////////////
// Tone generator
///////////////////////////////////////////////////////////////////////////////

// Configure TIMx channel 1 as a PWM output with a 1 MHz counter clock.
void initPitchTimerPWM(TIM_TypeDef *TIMx) {
  TIMx->CR1 = 0;                                   // stop

  TIMx->PSC  = (TIMCLK_HZ / TONE_TICK_HZ) - 1;     // 4 MHz / 4 = 1 MHz -> PSC = 3
  TIMx->ARR  = 0xFFFF;
  TIMx->CCR1 = 0;                      

  // Channel 1
  TIMx->CCMR1 &= ~(TIM_CCMR1_CC1S_Msk | TIM_CCMR1_OC1M_Msk);
  TIMx->CCMR1 |=  TIM_CCMR1_OC1M_PWM1 | TIM_CCMR1_OC1PE;

  TIMx->CCER &= ~TIM_CCER_CC1P;                    // active high
  TIMx->CCER |=  TIM_CCER_CC1E;                    // enable CH1 output
  TIMx->BDTR |=  TIM_BDTR_MOE;                     

  TIMx->CR1 |= TIM_CR1_ARPE;                    
  TIMx->EGR  = TIM_EGR_UG;                        
  TIMx->SR   = (uint32_t)~TIM_SR_UIF;                      
  TIMx->CR1 |= TIM_CR1_CEN;                     
}

// Output a 50% duty square wave at freqHz.
void setPitchFrequency(TIM_TypeDef *TIMx, int freqHz) {
  if (freqHz < TONE_MIN_HZ || freqHz > TONE_MAX_HZ) {
    TIMx->CCR1 = 0;                  
    TIMx->EGR  = TIM_EGR_UG;        
    return;
  }

  uint32_t f      = (uint32_t)freqHz;                 // f >= 16, never zero
  uint32_t period = (TONE_TICK_HZ + f / 2) / f;       

  TIMx->ARR  = period - 1;
  TIMx->CCR1 = period / 2;
  TIMx->EGR  = TIM_EGR_UG;          
}

///////////////////////////////////////////////////////////////////////////////
// Delay timer
///////////////////////////////////////////////////////////////////////////////

// Configure TIMx as a 10 kHz up-counter 
void initDurationTimer(TIM_TypeDef *TIMx) {
  TIMx->CR1 = 0;                                   
  TIMx->PSC = (TIMCLK_HZ / DELAY_TICK_HZ) - 1;     
  TIMx->ARR = 0xFFFF;
  TIMx->EGR = TIM_EGR_UG;                          
  TIMx->SR  = (uint32_t)~TIM_SR_UIF;
}

//wait for updates
void delayMilliseconds(TIM_TypeDef *TIMx, uint32_t ms) {
  while (ms > 0) {
    uint32_t chunk = (ms > DELAY_MAX_CHUNK_MS) ? DELAY_MAX_CHUNK_MS : ms;

    TIMx->ARR = chunk * DELAY_TICKS_PER_MS - 1;    
    TIMx->EGR = TIM_EGR_UG;                        
    TIMx->SR  = (uint32_t)~TIM_SR_UIF;                      
    TIMx->CR1 |= TIM_CR1_CEN;

    while (!(TIMx->SR & TIM_SR_UIF));              
    TIMx->CR1 &= ~TIM_CR1_CEN;
    TIMx->SR   = (uint32_t)~TIM_SR_UIF;
    ms -= chunk;
  }
}

// clock.c
// Clock configuration for Lab 4.

#include "clock.h"

// Put the chip in a known clock state: SYSCLK = MSI at 4 MHz, and
// AHB, APB1, APB2 prescalers all /1
void configureSystemClock(void) {
  //Verify sure MSI is on and stable
  RCC->CR |= RCC_CR_MSION;
  while (!(RCC->CR & RCC_CR_MSIRDY));

  // Select MSI as the system clock 
  RCC->CFGR &= ~RCC_CFGR_SW_Msk;
  while ((RCC->CFGR & RCC_CFGR_SWS_Msk) != 0);

  RCC->CR = (RCC->CR & ~RCC_CR_MSIRANGE_Msk) | RCC_CR_MSIRANGE_4MHZ | RCC_CR_MSIRGSEL;
  while (!(RCC->CR & RCC_CR_MSIRDY));

  // AHB, APB1, APB2 prescalers = /1
  RCC->CFGR &= ~(RCC_CFGR_HPRE_Msk | RCC_CFGR_PPRE1_Msk | RCC_CFGR_PPRE2_Msk);
}

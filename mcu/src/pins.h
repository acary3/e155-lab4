// pins.h
// GPIO driver for the STM32L432KC.
// Register map hand-written from RM0394, section 8.5. 

#ifndef PINS_H
#define PINS_H

#include <stdint.h>

///////////////////////////////////////////////////////////////////////////////
// Base addresses
///////////////////////////////////////////////////////////////////////////////

#define GPIOA_BASE (0x48000000UL)
#define GPIOB_BASE (0x48000400UL)

///////////////////////////////////////////////////////////////////////////////
// Register structure
///////////////////////////////////////////////////////////////////////////////

typedef struct {
  volatile uint32_t MODER;    // 0x00 Mode
  volatile uint32_t OTYPER;   // 0x04 Output type
  volatile uint32_t OSPEEDR;  // 0x08 Output speed
  volatile uint32_t PUPDR;    // 0x0C Pull-up/pull-down
  volatile uint32_t IDR;      // 0x10 Input data
  volatile uint32_t ODR;      // 0x14 Output data
  volatile uint32_t BSRR;     // 0x18 Bit set/reset
  volatile uint32_t LCKR;     // 0x1C Lock
  volatile uint32_t AFR[2];   // 0x20 AFRL (pins 0-7), 0x24 AFRH (pins 8-15)
  volatile uint32_t BRR;      // 0x28 Bit reset
} GPIO_TypeDef;

#define GPIOA ((GPIO_TypeDef *) GPIOA_BASE)
#define GPIOB ((GPIO_TypeDef *) GPIOB_BASE)

///////////////////////////////////////////////////////////////////////////////
// MODER values
///////////////////////////////////////////////////////////////////////////////

#define GPIO_INPUT   0UL
#define GPIO_OUTPUT  1UL
#define GPIO_ALT     2UL
#define GPIO_ANALOG  3UL

///////////////////////////////////////////////////////////////////////////////
// Function prototypes
///////////////////////////////////////////////////////////////////////////////

void gpioSetPinMode(GPIO_TypeDef *port, int pin, uint32_t mode);
void gpioSetAltFunction(GPIO_TypeDef *port, int pin, uint32_t af);

#endif // PINS_H

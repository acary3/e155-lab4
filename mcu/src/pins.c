// pins.c
// GPIO helper functions.

#include "pins.h"

// Set a pin's mode:
void gpioSetPinMode(GPIO_TypeDef *port, int pin, uint32_t mode) {
  if (pin < 0 || pin > 15) return;
  port->MODER &= ~(0x3UL << (2 * pin));
  port->MODER |=  ((mode & 0x3UL) << (2 * pin));
}

// Select alternate function AF0-AF15 for a pin
void gpioSetAltFunction(GPIO_TypeDef *port, int pin, uint32_t af) {
  if (pin < 0 || pin > 15) return;
  int reg   = pin / 8;
  int shift = 4 * (pin % 8);
  port->AFR[reg] &= ~(0xFUL << shift);
  port->AFR[reg] |=  ((af & 0xFUL) << shift);
}

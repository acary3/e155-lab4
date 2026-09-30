// lab4_starter.c -> main.c
// Fur Elise, E155 Lab 4
// Updated Fall 2024
//
// Plays Fur Elise (provided score, unchanged) followed by Ode to Joy on a
// speaker driven through an LM386 amplifier.
// Timers:
//   TIM16: PWM tone generator, 1 MHz counter clock
//   TIM15: note-duration delays, 10 kHz counter clock

#include <stdint.h>
#include "clock.h"
#include "pins.h"
#include "timer.h"

#define SPEAKER_PORT  GPIOA
#define SPEAKER_PIN   6       // PA6
#define SPEAKER_AF    14      // AF14 = TIM16_CH1 on PA6
#define PITCH_TIMER      TIM16
#define DURATION_TIMER     TIM15
#define GAP_BETWEEN_SONGS_MS   1500    // silence between songs

// Pitch in Hz, duration in ms
const int notes[][2] = {
{659,	125},
{623,	125},
{659,	125},
{623,	125},
{659,	125},
{494,	125},
{587,	125},
{523,	125},
{440,	250},
{  0,	125},
{262,	125},
{330,	125},
{440,	125},
{494,	250},
{  0,	125},
{330,	125},
{416,	125},
{494,	125},
{523,	250},
{  0,	125},
{330,	125},
{659,	125},
{623,	125},
{659,	125},
{623,	125},
{659,	125},
{494,	125},
{587,	125},
{523,	125},
{440,	250},
{  0,	125},
{262,	125},
{330,	125},
{440,	125},
{494,	250},
{  0,	125},
{330,	125},
{523,	125},
{494,	125},
{440,	250},
{  0,	125},
{494,	125},
{523,	125},
{587,	125},
{659,	375},
{392,	125},
{699,	125},
{659,	125},
{587,	375},
{349,	125},
{659,	125},
{587,	125},
{523,	375},
{330,	125},
{587,	125},
{523,	125},
{494,	250},
{  0,	125},
{330,	125},
{659,	125},
{  0,	250},
{659,	125},
{1319,	125},
{  0,	250},
{623,	125},
{659,	125},
{  0,	250},
{623,	125},
{659,	125},
{623,	125},
{659,	125},
{623,	125},
{659,	125},
{494,	125},
{587,	125},
{523,	125},
{440,	250},
{  0,	125},
{262,	125},
{330,	125},
{440,	125},
{494,	250},
{  0,	125},
{330,	125},
{416,	125},
{494,	125},
{523,	250},
{  0,	125},
{330,	125},
{659,	125},
{623,	125},
{659,	125},
{623,	125},
{659,	125},
{494,	125},
{587,	125},
{523,	125},
{440,	250},
{  0,	125},
{262,	125},
{330,	125},
{440,	125},
{494,	250},
{  0,	125},
{330,	125},
{523,	125},
{494,	125},
{440,	500},
{  0,	0}};

// Extra composition: Ode to Joy (Beethoven, Symphony No. 9), C major.
// Quarter note = 300 ms. Each note is followed by a 20 ms rest so repeated
// notes are articulated; note + rest adds up to the full musical value.
//   C4 = 262, D4 = 294, E4 = 330, F4 = 349, G4 = 392 Hz
const int ode_to_joy_notes[][2] = {
// Phrase 1: E E F G | G F E D | C C D E | E. D D-
{330, 280}, {0, 20}, {330, 280}, {0, 20}, {349, 280}, {0, 20}, {392, 280}, {0, 20},
{392, 280}, {0, 20}, {349, 280}, {0, 20}, {330, 280}, {0, 20}, {294, 280}, {0, 20},
{262, 280}, {0, 20}, {262, 280}, {0, 20}, {294, 280}, {0, 20}, {330, 280}, {0, 20},
{330, 430}, {0, 20}, {294, 130}, {0, 20}, {294, 580}, {0, 20},
// Phrase 2: E E F G | G F E D | C C D E | D. C C-
{330, 280}, {0, 20}, {330, 280}, {0, 20}, {349, 280}, {0, 20}, {392, 280}, {0, 20},
{392, 280}, {0, 20}, {349, 280}, {0, 20}, {330, 280}, {0, 20}, {294, 280}, {0, 20},
{262, 280}, {0, 20}, {262, 280}, {0, 20}, {294, 280}, {0, 20}, {330, 280}, {0, 20},
{294, 430}, {0, 20}, {262, 130}, {0, 20}, {262, 580}, {0, 20},
{  0,   0}};

void playScore(const int song[][2]) {
  for (int i = 0; song[i][1] != 0; i++) {
    int freq = song[i][0];
    int dur  = song[i][1];
    if (dur < 0) continue;

    setPitchFrequency(PITCH_TIMER, freq);
    delayMilliseconds(DURATION_TIMER, (uint32_t)dur);
  }
  setPitchFrequency(PITCH_TIMER, 0);   // silence at end of song
}

int main(void) {
  // Known clock: 4 MHz MSI, all bus prescalers /1
  configureSystemClock();

  // Enable peripheral clocks
  RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN;
  RCC->APB2ENR |= RCC_APB2ENR_TIM15EN | RCC_APB2ENR_TIM16EN;
  (void)RCC->APB2ENR;            

  // PA6 -> TIM16_CH1
  gpioSetAltFunction(SPEAKER_PORT, SPEAKER_PIN, SPEAKER_AF);
  gpioSetPinMode(SPEAKER_PORT, SPEAKER_PIN, GPIO_ALT);

  initPitchTimerPWM(PITCH_TIMER);
  initDurationTimer(DURATION_TIMER);

  while (1) {
    playScore(notes);
    delayMilliseconds(DURATION_TIMER, GAP_BETWEEN_SONGS_MS);
    playScore(ode_to_joy_notes);
    delayMilliseconds(DURATION_TIMER, GAP_BETWEEN_SONGS_MS);
  }
}

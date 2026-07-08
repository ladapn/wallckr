#include <avr/interrupt.h>
#include <avr/sleep.h>

void stop_simavr(void) {
  cli();
  sleep_cpu();
}
#ifndef STOP_SIMAVR_H
#define STOP_SIMAVR_H

#ifdef __AVR__
#include <avr/interrupt.h>
#include <avr/sleep.h>
#endif

/**
 * Halts the simavr simulator after unit tests finish. simavr detects a
 * sleeping core with interrupts disabled as "done" and exits; on a native
 * (host) test build there is no simulator to stop, so this is a no-op.
 */
inline void stop_simavr(void) {
#ifdef __AVR__
  cli();
  sleep_cpu();
#endif
}

#endif // STOP_SIMAVR_H

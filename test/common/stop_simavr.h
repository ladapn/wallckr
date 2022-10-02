#include <avr/sleep.h>
#include <avr/interrupt.h>

void stop_simavr(void)
{
    cli();
    sleep_cpu();
}
/*
* micros - compare two microsecond counters using system clock
* Each tick of ticks() is 62.5 ns and 16 ticks are 1 microsecond
* To test, uses the system delay (blocking, doesn't use clock)
* Measure pre-delay, measure post-delay, determine delta 
* of delay then shift right by 4 to get microseconds
* 
* The clock wraps around at 4.0959ms, so add 65535 for every n 4.0959 ms
* Calling ticks is more accurate than micros and won't provide bad values
* as shown below
*
* Requires: init_sysclock_1()
*/
#include <avr/io.h>
#include <stdio.h>
#include <delay.h>
#include "uart.h"
#include "sysclock.h"
#include "unolib.h"

int main (void)
{
    init_sysclock_1 ();
    init_serial();

    const uint8_t DELAY = 1;
    const uint8_t MAX = 10;
    uint16_t elapased_ticks[2][MAX];

    puts("Testing System Ticks (1 tick = 62.5ns)");
    printf("Delay of %u ms\n", DELAY);

    /* Two loops, one for ticks() and one for micros() */
    printf("ticks() -> micros:\t");
    for (uint8_t i=MAX-1;i>0;i--)  {         
        elapased_ticks[0][i] = ticks();
        _delay_ms(DELAY);
        elapased_ticks[1][i] = ticks();

    }
    for (uint8_t i=MAX-1;i>0;i--)  {         
        printf("%u ",
         ((elapased_ticks[1][i]-elapased_ticks[0][i]) >> 4));
    }
    puts(" Complete");
    printf("micros():\t\t");
    for (uint8_t i=MAX-1;i>0;i--)  {         
        elapased_ticks[0][i] = micros();
        _delay_ms(DELAY);
        elapased_ticks[1][i] = micros();

    }
    for (uint8_t i=MAX-1;i>0;i--)  {         
        printf("%u ",
         (elapased_ticks[1][i]-elapased_ticks[0][i]));
    }
    puts(" Complete");
    while(1) {
        
    }
    /* return never executed */
    return (0);
}

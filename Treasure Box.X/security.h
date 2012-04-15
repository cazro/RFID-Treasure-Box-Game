#include <p18f6722.h>
#include <delays.h>
#include "defines.h"
#include "globals.h"

void unlock(void);
void lock(void);

void lock(void)
{
    char temptime;
#if FINAL

    REDLED;
    time = TIMELIMIT;
    PORTD = 0x00;
    PORTD = 0x10;
    PORTD = 0x90;
    Delay100TCYx(1);
    temptime = time/10;
    PORTD = 0b00110000 | temptime;
    PORTD = 0b10110000 | temptime;
    Delay100TCYx(1);
    temptime = time%10;
    PORTD = 0b00100000 | temptime;
    PORTD = 0b10100000 | temptime;
   // LATA |= 0x04;
   // LATA &= ~0x02;
    LOCK;

#else
    LATA |= 0x08; // Turns Green off and turns on the Red LED.
    LATA &= ~0x02;
#endif
    time = TIMELIMIT;
    command = RCREG1;    // In case they press something while it's firing or unlocking,
    PIR1bits.RC1IF = 0;  // It won't think that's what they wanted as their seed
}
void unlock(void)
{
#if FINAL
    GREENLED;
    UNLOCK;
    stopTimer();
    playUnlock();
  //  LATA |= 0x02;
  //  LATA &= ~0x04;
    Delay10KTCYx(250);  // 2,500,000 Clock cycles ~ 1.25 Seconds
    Delay10KTCYx(250);
    Delay10KTCYx(250);
    Delay10KTCYx(250);
#else
    stopTimer();
    playUnlock();
    LATA |= 0x02;       // Unlock Chest (send signal to pin)
    LATA &= ~0x08;
    Delay10KTCYx(250);  // 2,500,000 Clock cycles ~ 1.25 Seconds
    Delay10KTCYx(250); // Approximately 2.5 seconds of "unlock" time total
#endif
}

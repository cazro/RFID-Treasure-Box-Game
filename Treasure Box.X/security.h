#include <p18f6722.h>
#include <delays.h>
#include "defines.h"
#include "globals.h"
#include "visuals.h"

void unlock(void);
void lock(void);
void unlockMaster(void);

void lock(void)
{
#if FINAL

    REDLED;
    stopTimer();
    time = TIMELIMIT;
    displayCorrectKeys(0);
    displayTime(time);
    LATF = 0x00;
    RCSTA1bits.SPEN = 1;
    LOCK;

#else
    LATA |= 0x08; // Turns Green off and turns on the Red LED.
    LATA &= ~0x02;
#endif
    time = TIMELIMIT;
    correctKeys = 0;
    command = RCREG1;    // In case they press something while it's firing or unlocking,
    PIR1bits.RC1IF = 0;  // It won't think that's what they wanted as their seed
}
void unlock(void)
{
#if FINAL
    GREENLED;
    UNLOCK;
    RCSTA1bits.SPEN = 0;
    stopTimer();
    playUnlock();
    Delay10KTCYx(250);  // 2,500,000 Clock cycles ~ 1.25 Seconds
    Delay10KTCYx(250);
    Delay10KTCYx(250);
    Delay10KTCYx(250);
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
void unlockMaster(void)
{
#if FINAL
    GREENLED;
    UNLOCK;
    RCSTA1bits.SPEN = 0;
    stopTimer();
    playMaster();
    Delay10KTCYx(250);  // 2,500,000 Clock cycles ~ 1.25 Seconds
    Delay10KTCYx(250);
    Delay10KTCYx(250);
    Delay10KTCYx(250);
    Delay10KTCYx(250);  // 2,500,000 Clock cycles ~ 1.25 Seconds
    Delay10KTCYx(250);
    Delay10KTCYx(250);
    Delay10KTCYx(250);
#endif
}

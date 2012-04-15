#include <p18f6722.h>
#include <delays.h>
#include "defines.h"
#include "globals.h"

#ifndef VISUALS
#define VISUALS
void displayCorrectKeys(void);
void displayTime(void);
void displayAttack(void);


void displayCorrectKeys(void)
{
#if FINAL
    PORTD = 0x00;
    PORTD = 0b00010000 | correctKeys;
    Delay1TCY();
    PORTD = 0b10010000 | correctKeys;
    Delay100TCYx(1);
    PORTD = 0b00010000 | correctKeys;

#else
    correctKeys += 48;
#if DEBUG
    putByteUSART(correctKeys);
#endif
#endif
}
void displayTime(void)
{
    char temptime;
#if FINAL
    temptime = time/10;
    PORTD = 0b00110000 | temptime;
    PORTD = 0b10110000 | temptime;
    Delay100TCYx(1);
    temptime = time%10;
    PORTD = 0b00100000 | temptime;
    PORTD = 0b10100000 | temptime;

#if DEBUG
    temptime = (time/10) + 48;
    putByteUSART(temptime);
    temptime = (time%10) + 48;
    putByteUSART(temptime);
    putrs1USART("\r\n");
#endif
#endif
}
void displayAttack(void)
{
    char i;
#if FINAL
    for(i=0;i<75;i++)
    {
        REDLED;
        ATTACKLEDON;
        Delay10KTCYx(12);
        GREENLED;
        ATTACKLEDOFF;
        Delay10KTCYx(12);
    }
#else
    for(i=0;i<20;i++)
    {
        LATA ^= 0x08;
        Delay10KTCYx(25);
    }
#endif
}
#endif

#include <p18f6722.h>
#include <delays.h>
#include "defines.h"
#include "globals.h"

#ifndef VISUALS
#define VISUALS

void displayCorrectKeys(char correct);
void displayTime(char disp);
void displayAttack(void);
void disp2Digs(char dig1, char dig2);
void scrollClass(void);

void displayCorrectKeys(char correct)
{
#if FINAL
    PORTD = 0x00;
    PORTD = 0b00010000 | correct;
    Delay1TCY();
    PORTD = 0b10010000 | correct;
    Delay100TCYx(1);
    PORTD = 0b00010000 | correct;

#else
    correctKeys += 48;
#if DEBUG
    putByteUSART(correctKeys);
#endif
#endif
}
void disp2Digs(char dig1, char dig2)
{
    PORTD = 0b00110000 | dig1;
    PORTD = 0b10110000 | dig1;
    Delay100TCYx(1);
    PORTD = 0b00100000 | dig2;
    PORTD = 0b10100000 | dig2;

}
void displayTime(char disp)
{
    char temp;
#if FINAL
    temp = disp/10;
    PORTD = 0b00110000 | temp;
    PORTD = 0b10110000 | temp;
    Delay100TCYx(1);
    temp = disp%10;
    PORTD = 0b00100000 | temp;
    PORTD = 0b10100000 | temp;

#if DEBUG
    temp = (time/10) + 48;
    putByteUSART(temp);
    temp = (time%10) + 48;
    putByteUSART(temp);
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
void scrollClass(void)
{
    displayTime(0);
    Delay10KTCYx(scrollDelay);
    disp2Digs(0x0A,0x0C); // -H
    Delay10KTCYx(scrollDelay);
    disp2Digs(0x0C,0x0B); // HE
    Delay10KTCYx(scrollDelay);
    disp2Digs(0x0B,0x0D); // EL
    Delay10KTCYx(scrollDelay);
    disp2Digs(0x0D,0x0E); // LP
    Delay10KTCYx(scrollDelay);
    disp2Digs(0x0E,0x0A); // P-
    Delay10KTCYx(scrollDelay);
    disp2Digs(0x0A,0x04); // -4
    Delay10KTCYx(scrollDelay);
    disp2Digs(0x04,0x00); // 40
    Delay10KTCYx(scrollDelay);
    disp2Digs(0x00,0x01); // 01
    Delay10KTCYx(scrollDelay);
    disp2Digs(0x01,0x03); // 13
    Delay10KTCYx(scrollDelay);
    disp2Digs(0x03,0x0A); // 3-
    Delay10KTCYx(scrollDelay);
    disp2Digs(0x0A,0x0A); // --
    Delay10KTCYx(75);
}
#endif

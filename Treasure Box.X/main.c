#include <p18f6722.h>
#include <usart.h>
#include <timers.h>
#include <stdlib.h>
#include <stdio.h>
#include <delays.h>
#include "IR.h"
#include "rfid.h"
#include "defines.h"
#include "constants.h"
#include "globals.h"
#include "visuals.h"
#include "main.h"
#include "xbee.h"
#include "audio.h"
#include "security.h"

// ^^^ NOMINATE THESE GUYS FOR BEST SUPPORTING ACTORS ^^^

// PRAGLE ROCKS BELOW
#pragma config WDT = OFF
#pragma config OSC = INTIO67

void main(void)
{
    init();
    irTXInit();
    while(1)
    {
        lock();
        getMAGECombo();
    #if DEBUG
        putrsUSART("Please Enter ");
        putByteUSART(NUMBEROFKEYS);
        putrsUSART(" separate keys.\r\n");
    #endif
        getKeys(0);
        if(checkCombo())
        {
        #if DEBUG
            putrsUSART("Congrats.  You got it right.  Here's your cookie!\r\n");
        #endif
            displayCorrectKeys();
            unlock();
        }
        else
        {
            playWrongCombo();
        #if DEBUG
            putrsUSART("Drats.  You suck at this.  You got ");
            displayCorrectKeys();
            putrsUSART(" right.\r\n");
        #endif
            displayCorrectKeys();
         #if DEBUG
            putrsUSART("You now have ");
            putByteUSART(TIMELIMIT/10 + 48);
            putByteUSART(TIMELIMIT%10 +48);
            putrsUSART(" seconds to fix this mess or die.\r\n");
         #endif
            startTimer();
            while(correctKeys != NUMBEROFKEYS && (time > 0))
            {
                getKeys(1);
                if(checkCombo() && time > 0)
                {
                #if DEBUG
                    putrsUSART("Congrats.  You finally got it right. \r\n");
                #endif
                    displayCorrectKeys();
                    unlock();
                }
                else
                {
                    if(time != 0)
                    {
                        playWrongCombo();
                     #if DEBUG
                        putrsUSART("Nope, but you got ");
                        displayCorrectKeys();
                        putrsUSART(" correct!\r\n");
                     #endif
                        displayCorrectKeys();
                    }
                }
             } //end  while
             if(time <= 0)
             {
             #if DEBUG
                putrsUSART("You are now dead.  Say, \"Hi\" to your mom for me.\r\n");
             #endif
                attack();
             }
        }//end if/else checkCombo
    }
}
// Init good to be an initialization function
void init(void)
{
        OSCCONbits.IRCF2 = 1;
        OSCCONbits.IRCF1 = 1;
        OSCCONbits.IRCF0 = 1;
        TRISA = 0x00;           // Make Pins 0, and 3 output
        TRISC = 0x80;           // Makes UART Recieve pin an Input, TX an output
        TRISD = 0x00;
        TRISB = 0x00;
        TRISF = 0x00;
        TRISG = 0x40;

        LATAbits.LATA3 = 1;     // Make the "Play" switch high

        RCSTA1 = 0x90; // Enables Serial, Continuous Receive
        TXSTA1 = 0x24; //Enables Transmit,Asynch, BRGH (Baud High)
        SPBRG1 = 51;     // 9600 BAUD for 8Mhz intosc
        RCSTA2 = 0x90; // Enables Serial, Continuous Receive
        TXSTA2 = 0x24; //Enables Transmit,Asynch, BRGH (Baud High)
        SPBRG2 = 51;     // 9600 BAUD for 8Mhz intosc
        PIE1bits.RC1IE = 1;  // Enable Receive Interrupts
        PIE1bits.TX1IE = 1;  // Enable Transmit Interrupts
        PIE3bits.RC2IE = 1;
        PIE3bits.TX2IE = 1;
        INTCONbits.TMR0IE = 1; // Enable Timer0 Overflow interrupt
        // CA Wrong 97 Attack 64 Correct
        j=0;
}
void startTimer(void)
{
    time = TIMELIMIT;
    T0CON = 0x84; // Enables timer0 in timer mode with 1:32 prescaler
    TMR0L = 0;
    TMR0H = 0;
}
void stopTimer(void)
{
    T0CON = 0x00;
}
char checkCombo(void)   // The self-explaining function name
{
    char i;
    correctKeys = 0;
    for(i=0;i<NUMBEROFKEYS;i++)
    {
        if(userCombo[i] == mageCombo[i])
           correctKeys++;
    }
    if(correctKeys == NUMBEROFKEYS)
        return 1;
    else
        return 0;
}
void attack(void)
{
    stopTimer();
    IRAttack();
    playAttack();
    displayAttack();
}

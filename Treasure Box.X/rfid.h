#include <p18f6722.h>
#include "defines.h"
#include "constants.h"
#include "globals.h"
#include "main.h"
#include "visuals.h"
#include "audio.h"

#ifndef RFID
#define RFID

void putByteUSART(char data);
char getByteUSART(void);
void getKeys(char timeCheck);
char getRFID(void);
void blinkRFIDLED(void);

void getKeys(char timeCheck)
{
    char i;
    for(i=0;i<NUMBEROFKEYS;i++)
    {
        if(time > 0 && timeCheck)
        {
            userCombo[i] = getRFID();
            if(userCombo[i]== 'M')
                i=NUMBEROFKEYS;
#if DEBUG
            putByteUSART(userCombo[i]);
            putrs1USART("\r\n");
#endif
        }
        else if(!timeCheck)
        {
            userCombo[i] = getRFID();
            if(userCombo[i]== 'M')
                i=NUMBEROFKEYS;
#if DEBUG
            putByteUSART(userCombo[i]);
            if(i==(NUMBEROFKEYS-1))
                putrs1USART("\r\n");
#endif
        }
    }

}
void blinkRFIDLED(void)
{
    GREENLED;
    ATTACKLEDON;
    Delay10KTCYx(10);
    REDLED;
    ATTACKLEDOFF;
}
char getRFID(void)
{
    char rfid[RFIDBYTES]; // RFIDBYTES = 16
    int i = 0;

    for(j=0;j<RFIDBYTES;j++)
        if(time > 0)
        {
            rfid[j] = getByteUSART();
            RCSTA1bits.CREN1 = 0;
            RCSTA1bits.CREN1 = 1;
            //if(rfid[0] != 0x02)
               // j--;
        }
        else{
            rfid[j] = '0';
        }


    j=0;

    blinkRFIDLED();
    for(i=0;i<RFIDBYTES;i++)
        if(rfid[i] != master[i])
            break;
        else
            if(i==(RFIDBYTES-1))
                return 'M';
    for(i=0;i<RFIDBYTES;i++)
        if(rfid[i] != key1[i])
            break;
        else
            if(i==(RFIDBYTES-1))
            {
            #if THECHEAT
                displayCorrectKeys(1);
            #endif
                return '1';
            }
    for(i=0;i<RFIDBYTES;i++)
        if(rfid[i] != key2[i])
            break;
        else
            if(i==(RFIDBYTES-1))
            {
             #if THECHEAT
                displayCorrectKeys(2);
             #endif
                return '2';
            }
    for(i=0;i<RFIDBYTES;i++)
        if(rfid[i] != key3[i])
            break;
        else
            if(i==(RFIDBYTES-1))
            {
            #if THECHEAT
                displayCorrectKeys(3);
            #endif
                return '3';
            }
    for(i=0;i<RFIDBYTES;i++)
        if(rfid[i] != key4[i])
            break;
        else
            if(i==(RFIDBYTES-1))
            {
            #if THECHEAT
                displayCorrectKeys(4);
            #endif
                return '4';
            }
    return '\r';
}

void putByteUSART(char data) // Put? More like forcefully shove dem bytes
{
    while(!(TXSTA1 & 0x02));
    TXREG1 = data;
    Delay1TCY();
}
char getByteUSART(void)
{
    while(!PIR1bits.RC1IF && time > 0)
    {
        if(INTCONbits.TMR0IF == 1 && time > 0 && j==0)     // Timer0 Overflow interrupt flag is thrown
        {
            //INTCONbits.TMR0IE = 0; //Disable interrupts
            time--;
            displayTime(time);
            INTCONbits.TMR0IF=0;   // Reset flag
            if(time == 0)
            {
                stopTimer();
                break;
            }
            else if(time == (TIMELIMIT / 5))
            {
                playTimeClose();
            }

            // INTCONbits.TMR0IE = 1; // Re-enable interrupt
        }
        if(RCSTA1bits.OERR)
        {
            RCSTA1bits.CREN = 0;
            RCSTA1bits.CREN = 1;
            return RCREG1;
        }
    }
    if(time > 0)
        return RCREG1;
    else
        return '0';
}
#endif

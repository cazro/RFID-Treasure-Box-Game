
#include <p18f6722.h>
#include <usart.h>
#include <timers.h>
#include <stdlib.h>
#include <stdio.h>
#include <delays.h>
#include "IR.h"
//#include "rfid.h"
#include "defines.h"

// ^^^ NOMINATE THESE GUYS FOR BEST SUPPORTING ACTORS ^^^

// PRAGLE ROCKS BELOW
#pragma config WDT = OFF
#pragma config OSC = INTIO67

// Yes compiler, I didn't just make these functions on accident
void startProgram(void);
void InterruptServiceHigh(void);
char checkCombo(void);
void init(void);
void startTimer(void);
void stopTimer(void);
void unlock(void);
void lock(void);
void displayTime(void);
void displayCorrectKeys(void);
void getMAGECombo(void);
void displayAttack(void);
void IRAttack(void);
void playUnlock(void);
void playAttack(void);
void playWrongCombo(void);
void play(void);
void attack(void);
void putByteUSART(char data);
char getByteUSART(void);
void getKeys(char timeCheck);
char getRFID(void);
void blinkRFIDLED(void);
char getByteXbee(void);
void putByteXbee(char data);
void Xbee_senddata(void);
void Xbee_recdata(void);

// Constant Variables
const char key1[16] = {0x02,'4','E','0','0','0','4','8','F','9','B','5','E',0x0D,0x0A,0x03};
const char key2[16] = {0x02,'4','C','0','0','3','5','3','1','D','A','9','2',0x0D,0x0A,0x03};
const char key3[16] = {0x02,'8','4','0','0','3','3','8','F','4','3','7','B',0x0D,0x0A,0x03};
const char key4[16] = {0x02,'8','4','0','0','3','3','8','A','3','5','0','8',0x0D,0x0A,0x03};

// Stand back and don't shake things, these be volatile!
volatile char userCombo[NUMBEROFKEYS];
volatile char time;
volatile char temptime;
volatile signed char j;
volatile char mageCombo[NUMBEROFKEYS] = {'1','1','1','1'};
volatile char correctKeys;
volatile char command;
volatile char firstRun;
volatile char keys[16];

void main(void)
{
    init();
    irTXInit();
    while(1)
    {
        startProgram();
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
void startProgram(void) // Basically main() in disguise
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

void getMAGECombo(void)
{
#if FINAL
    Xbee_senddata();
   // Xbee_recdata();

#else
    char numCheck[NUMBEROFKEYS];
    char i,tempKey;
    int seed;
    for(i=0;i<NUMBEROFKEYS;i++)
        numCheck[i] = 0;
    putrs1USART("\r\nRetrieving New Combination");

    ADCON1 = 0x0E;
    ADCON0 = 0x00;
    ADCON2 = 0x80;
    ADCON0 = 0x01;
    ADCON0 |= 0x02;
    while((ADCON0 & 0x02));
    seed = ADRESH;
    seed <<= 8;
    seed += ADRESL;

    srand(seed);

#if CHEAT
    putrs1USART("\r\nCheats are ON\r\n");
#endif

    for(i=0;i<NUMBEROFKEYS;i++)
    {
        tempKey = (char)(rand() % NUMBEROFKEYS);
        ADCON0 |= 0x02;
        while((ADCON0 & 0x02));
        seed = ADRESH;
        seed <<= 8;
        seed += ADRESL;
        srand(seed);
        while(numCheck[tempKey])
        {
            tempKey = rand() % NUMBEROFKEYS;
        }
        mageCombo[i] = tempKey+49;
#if CHEAT
        putByteUSART(mageCombo[i]);
#endif
        numCheck[tempKey] = 1;
    }


    putrs1USART("\r\nRandom MAGE Combination Generated\r\n");
#endif
}
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
void IRAttack(void)
{
    putcIR(214);
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

void lock(void)
{
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

void playUnlock(void)
{
    LATF = 0x4E;
    Delay1TCY();
    play();
}
void attack(void)
{
    stopTimer();
    IRAttack();
    playAttack();
    displayAttack();
}
void play(void)
{
    LATA &= ~0x08;  // Clear pin 3
    Delay10KTCYx(70);
    LATA |= 0x08;   // Set Pin 3
}
void playAttack(void)
{
    //LATA &= ~0x08;
    //Delay10KTCYx(25);
   // LATA |= 0x08;
    LATF = 0x6D;
    Delay1TCY();
    //play();
}
void playWrongCombo(void)
{
    LATF = 0x73;
    Delay1TCY();
    //play();
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

void getKeys(char timeCheck)
{
    char i;
    for(i=0;i<NUMBEROFKEYS;i++)
    {
        if(time > 0 && timeCheck)
        {
            userCombo[i] = getRFID();
#if DEBUG
            putByteUSART(userCombo[i]);
            putrs1USART("\r\n");
#endif
        }
        else if(!timeCheck)
        {
            userCombo[i] = getRFID();
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
RFIDLEDON;
ATTACKLEDON;
Delay10KTCYx(10);
RFIDLEDOFF;
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
        if(rfid[i] != key1[i])
            break;
        else
            if(i==(RFIDBYTES-1))
                return '1';
    for(i=0;i<RFIDBYTES;i++)
        if(rfid[i] != key2[i])
            break;
        else
            if(i==(RFIDBYTES-1))
                return '2';
    for(i=0;i<RFIDBYTES;i++)
        if(rfid[i] != key3[i])
            break;
        else
            if(i==(RFIDBYTES-1))
                return '3';
    for(i=0;i<RFIDBYTES;i++)
        if(rfid[i] != key4[i])
            break;
        else
            if(i==(RFIDBYTES-1))
                return '4';

    return '\r';
}
// This little piggy went to the UART store and got me a Byte to eat
char getByteUSART(void)
{
    while(!PIR1bits.RC1IF && time > 0)
    {
        if(INTCONbits.TMR0IF == 1 && time > 0 && j==0)     // Timer0 Overflow interrupt flag is thrown
        {
            //INTCONbits.TMR0IE = 0; //Disable interrupts
            time--;
            displayTime();
            INTCONbits.TMR0IF=0;   // Reset flag
            if(time == 0)
            {
                stopTimer();
                break;
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
void putByteUSART(char data) // Put? More like forcefully shove dem bytes
{
    while(!(TXSTA1 & 0x02));
    TXREG1 = data;
    Delay1TCY();
}
char getByteXbee(void)
{
    while(!PIR3bits.RC2IF);
    return RCREG2;
}
void Xbee_recvdata(void){
    char i,p;
    char receivedata[100];

    receivedata[0] = getByteXbee();
    receivedata[1] = getByteXbee();
    receivedata[2] = getByteXbee();

    p = receivedata[2];

    for(i=3;i<(p+3);i++)
	receivedata[i] = getByteXbee();

    mageCombo[0] = receivedata[p-1];
    mageCombo[1] = receivedata[p];
    mageCombo[2] = receivedata[p+1];
    mageCombo[3] = receivedata[p+2];

    receivedata[p+3] = getByteXbee();

  /*  for(i=0;i<(p+4);i++)
    {
        putByteXbee(receivedata[i]);
    }
*/
}
void putByteXbee(char data)
{
    while(!(TXSTA2 & 0x02));
    TXREG2 = data;
}
void Xbee_senddata()
{

     putByteXbee(0x7E);
//Length
     putByteXbee(0x00);
     putByteXbee(0x0D);
//Command
     putByteXbee(0x01);
//Frame ID
     putByteXbee(0x00);
//Address
     putByteXbee(0xFF);
     putByteXbee(0xFF);
//Options
     putByteXbee(0x00);
//Payload
     putByteXbee(0xFF);
     putByteXbee(0xFF);	// Server address
     putByteXbee(0x11);
     putByteXbee(0x11); // Our Address
     putByteXbee(0x00); // 4 Bytes of data
     putByteXbee(0x00);
     putByteXbee(0x00);
     putByteXbee(0x00);

     putByteXbee(0xE0); // Checksum
}
/*
#pragma code InterruptVectorHigh = 0x08
void InterruptVectorHigh (void)
{
_asm
     goto InterruptServiceHigh //jump to interrupt routine
 _endasm
}
#pragma interrupt InterruptServiceHigh  // "interrupt" pragma also for high priority
void InterruptServiceHigh(void)
{
    if (INTCONbits.TMR0IF == 1)     // Timer0 Overflow interrupt flag is thrown
    {
        INTCONbits.TMR0IE = 0; //Disable interrupts
        time--;
        putsUSART(time);
        putrsUSART("\n");
	INTCONbits.TMR0IF=0;   // Reset flag
        INTCONbits.TMR0IE = 1; // Re-enable interrupt
    }
}*/
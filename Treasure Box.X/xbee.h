#include <p18f6722.h>
#include "defines.h"
#include "constants.h"
#include "globals.h"
#include "delays.h"

#ifndef XBEE
#define XBEE

char getByteXbee(void);
void putByteXbee(char data);
void Xbee_senddata(void);
void Xbee_recdata(void);
void getMAGECombo(void);
void genRandCombo(void);
void initADCON(void);
int getSeed(void);

char getByteXbee(void)
{
    while(!PIR3bits.RC2IF);
    return RCREG2;
}
void Xbee_recdata(void){
    char i,p;
    char receivedata[100];

    receivedata[0] = getByteXbee();
    receivedata[1] = getByteXbee();
    receivedata[2] = getByteXbee();

    p = receivedata[2];

    for(i=3;i<(p+3);i++)
	receivedata[i] = getByteXbee();

    receivedata[p+3] = getByteXbee();
    if(receivedata[p-1] == 0xFF || receivedata[p] == 0xFF || receivedata[p+1] == 0xFF || receivedata[p+2] == 0xFF)
    {
        genRandCombo();
    }
    else
    {
        mageCombo[0] = receivedata[p-1];
        mageCombo[1] = receivedata[p];
        mageCombo[2] = receivedata[p+1];
        mageCombo[3] = receivedata[p+2];
    }
#if THECHEAT
    Delay10KTCYx(100);
    displayCorrectKeys(mageCombo[0]-48);
    Delay10KTCYx(150);
    displayCorrectKeys(mageCombo[1]-48);
    Delay10KTCYx(150);
    displayCorrectKeys(mageCombo[2]-48);
    Delay10KTCYx(150);
    displayCorrectKeys(mageCombo[3]-48);
    Delay10KTCYx(150);
    displayCorrectKeys(0);
#endif
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
void getMAGECombo(void)
{
    RCSTA1bits.SPEN = 0;
    Xbee_senddata();
    Xbee_recdata();
    RCSTA1bits.SPEN = 1;
}
void initADCON(void)
{
    ADCON1 = 0x0E;
    ADCON0 = 0x00;
    ADCON2 = 0x80;
    ADCON0 = 0x01;
}
int getSeed(void)
{
    int seed;
    seed = ADRESH;
    seed <<= 8;
    seed += ADRESL;
    return seed;
}
void genRandCombo(void)
{
    char numCheck[NUMBEROFKEYS];
    char i,tempKey;

    for(i=0;i<NUMBEROFKEYS;i++)
        numCheck[i] = 0;

    initADCON();

    ADCON0bits.GO = 1;

    //while((!ADCON0bits.DONE));
    seed = (seed * 25173 + 13849)%65536;
    srand(seed);

    for(i=0;i<NUMBEROFKEYS;i++)
    {
        tempKey = (char)((rand()+i) % NUMBEROFKEYS);
 
        ADCON0bits.GO = 1;
        //while((!ADCON0bits.DONE));

        while(numCheck[tempKey])
            tempKey = (rand()+i) % NUMBEROFKEYS;

        mageCombo[i] = tempKey+49;
        
#if THECHEAT
        displayCorrectKeys(tempKey+1);
        Delay10KTCYx(150);
#endif
        numCheck[tempKey] = 1;
    }
    displayCorrectKeys(0);
}
#endif
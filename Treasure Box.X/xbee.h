#include <p18f6722.h>
#include "defines.h"
#include "constants.h"
#include "globals.h"

char getByteXbee(void);
void putByteXbee(char data);
void Xbee_senddata(void);
void Xbee_recdata(void);
void getMAGECombo(void);


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

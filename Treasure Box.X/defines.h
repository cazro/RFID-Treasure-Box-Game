// DEFINES
#ifndef DEFINES
#define DEFINES

#define THECHEAT 0
#define FINAL 1
#define DEBUG 0
#define NUMBEROFKEYS  4
#define RFIDBYTES  16
#define TIMELIMIT  45
#define DIGIT1 0b11111111
#define DIGIT2 0b11101111
#define DIGIT3 0b11011111
#define CSHIGH 0b01111111
#define CSLOW  0b00111111
#define REDLED LATBbits.LATB0 = 0
#define ATTACKLEDON LATBbits.LATB1 = 1
#define ATTACKLEDOFF LATBbits.LATB1 = 0
#define GREENLED LATBbits.LATB0 = 1
#define LOCK LATB |= 0x0C
#define UNLOCK LATB &= ~0x0C
#endif
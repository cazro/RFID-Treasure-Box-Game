#include <p18f6722.h>
#include <delays.h>
#ifndef AUDIOS
#define AUDIOS

void playUnlock(void);
void playAttack(void);
void playWrongCombo(void);
void play(void);
void playTimeClose(void);
void playMaster(void);

void playUnlock(void)
{
    LATF = 0x20;
    Delay1TCY();
    play();
}
void play(void)
{
    LATAbits.LATA2 = 0;  // Clear pin 3
    Delay10KTCYx(10);
    LATAbits.LATA2 = 1;   // Set Pin 3
}
void playAttack(void)
{
    LATF = 0x73;
    Delay1TCY();
    play();
}
void playWrongCombo(void)
{
    LATF = 0x98;
    Delay1TCY();
    play();

}
void playTimeClose(void)
{
    LATF = 0xBA;
    Delay1TCY();
    play();
}
void playMaster(void)
{
    LATF = 0xFC;
    Delay1TCY();
    play();
}
#endif
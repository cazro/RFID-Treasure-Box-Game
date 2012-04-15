#include <p18f6722.h>
#include <delays.h>

void playUnlock(void);
void playAttack(void);
void playWrongCombo(void);
void play(void);

void playUnlock(void)
{
    LATF = 0x4E;
    Delay1TCY();
    play();
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

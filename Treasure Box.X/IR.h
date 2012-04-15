#include <p18f6722.h>
/* Enable PWM Mode on CCP1 pin */
#define LED_high (CCP4CON|=0x0C)
#define LED_low (CCP4CON&=0xf0)
#define Timer0_TX (TIMER_INT_ON&T0_8BIT&T0_SOURCE_INT&T0_PS_1_32)

void irTXInit(void);
void putcIR(char data);
void IRAttack(void);

unsigned char IRData[11];

void irTXInit(void)
{
        /* Reset the CCP1 module */
        CCP4CON = 0x00;
        /* Set the PWM period */
        //PR2 = 0x59;
        PR2=0x08;
        /* Set the PWM duty cycle */
        //CCPR1L = 0x2d;
        CCPR4L = 0x04;
        //CCP1CON &= 0b11100111;
        CCP4CON &= 0b00011100;
        /* Configure CCP1 pin as an output */
        TRISGbits.TRISG3 = 0;
        LATGbits.LATG2=0;
        /* Set the TMR2 prescale value */
        T2CONbits.T2CKPS1 = 0;
        T2CONbits.T2CKPS0 = 1;

        /* Turn on Timer2 */
        T2CONbits.TMR2ON = 1;

}
void putcIR(char data)
{
        unsigned char i;
        unsigned char p;

        p=0;
        IRData[0]=1;
        IRData[1]=0;

        for(i=0;i<8;i++)
        {
            if((1<<i)&data){
                    IRData[9-i]=1;
                    p^=1;
            }
            else{IRData[9-i]=0;}
        }

        IRData[10]=p;

        OpenTimer0(Timer0_TX);

        for(i = 0;i<11;i++)
        {
            if(IRData[i])
                LED_high;
            else
                LED_low;

            while(!INTCONbits.TMR0IF);     // Timer 1 interrupt flag
            INTCONbits.TMR0IF=0;
        }
        LED_low;
        CloseTimer0();
}
void IRAttack(void)
{
    putcIR(214);
}

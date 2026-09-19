#include <xc.h>
#include "timer0.h"
#include "main.h"

//packet model -> <10,50>


volatile unsigned char receive_buf[10];

volatile unsigned char index = 0;
unsigned char ch;
volatile unsigned char packet_received = 0;

extern volatile unsigned int blink_count;
extern volatile unsigned char blink_active;
void __interrupt() isr(void)
{
    if(RCIF == 1)
    {
        
        if(OERR == 1)
        {
            CREN = 0;
            CREN = 1;
        }
        ch = RCREG;
        if(ch =='<')
            index = 0;
        else
        {
            if(ch=='>')
            {
                receive_buf[index] = '\0';
                packet_received = 1;
            }
            else
            {
                
                receive_buf[index++] = ch;
            }

        }
        RCIF = 0;
    }
    if(TMR0IF)
    {
        TMR0 = TMR0 + 8;
        ++blink_count;
        if(blink_active)
        {
            if(blink_count == BLINK_COUNT)
            {
                PICKTOLIGHT = !PICKTOLIGHT;
                blink_count = 0;
            }
        }
        TMR0IF = 0; //clearing timer0 flag
        
    }
}

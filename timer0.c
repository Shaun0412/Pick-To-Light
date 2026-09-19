#include <xc.h>
#include "timer0.h"

void timer0_init(void)
{
    GIE = 1;
    
    OPTION_REG = 0x02; //1:8 prescale, timer mode, scaling assigned to timer0
    TMR0 = 8 + TMR0; //preload

    TMR0IE = 1;
    TMR0IF = 0;
}


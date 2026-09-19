#include <xc.h>
#include "uart.h"
void init_uart(unsigned long baud)
{
    /* Setting RC6 and RC7 to work as Serial Port */
    SPEN = 1;
    
    /* Continuous Reception Enable Bit */
    CREN = 1;
    
    /* Baud Rate Setting Register */
    SPBRG = (FOSC / (16 * baud)) - 1;

    /*Enabling Global interrupt*/
    GIE = 1;

    /*Enabling Peripheral Interrupts*/
    PEIE = 1;

    /* Receive Interruot Enable Bit (Enabling the serial port Interrupt) */
    RCIE = 1;

    TXEN = 0;
}


void putchar(unsigned char data)
{
    TXREG = data;
    while(TXIF != 1); //the interupt flag will be set when data is done transmitting
    TXIF = 0;
}



void puts(const char *s)
{
    while (*s)
    {
        putchar(*s++);
    }
}

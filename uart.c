#include <xc.h>
#include "uart.h"
#include "main.h"
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

/*============================EXTRAS================================================*/
extern unsigned char my_port_id;
void send_stock_received_info(unsigned char stock_received)
{
    putchar('<');
    send_number(my_port_id);
    putchar(',');
    send_number(stock_received);
    putchar('>');
}

void send_number(unsigned char number)
{
    if(number >= 100)
        putchar((number / 100) + '0');

    if(number >= 10)
        putchar(((number / 10) % 10) + '0');

    putchar((number % 10) + '0');
}
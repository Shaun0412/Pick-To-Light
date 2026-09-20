

#include "ssd.h"
#include <xc.h>

unsigned char ssd_numbers[]= {ssd_ZERO,ssd_ONE,ssd_TWO,ssd_THREE,ssd_FOUR,ssd_FIVE,ssd_FIVE,ssd_SIX,ssd_SEVEN,ssd_EIGHT,ssd_NINE};
void init_ssd_display(void)
{
    SSD_DATA_PORT_DDR = 0x00;//All output pins connected to segments of SSD
    SSD_CONTROL_PORT_DDR &= ~(0X3C); // RA2 to RA5 as output pins


    //setting all the ssd's to be OFF at first
    SSD_CONTROL_PORT &= ~(0x3C);


}
void delay(unsigned int ms)
{
    volatile unsigned int wait;

    while(ms--)
    {
        for(wait = 500; wait > 0; wait--);
    }
}
void display(unsigned char array[])
{
    for(unsigned int i=0;i<4;i++)
    {
        SSD_CONTROL_PORT &= ~(0x3C);
        SSD_DATA_PORT = array[i];
        SSD_CONTROL_PORT |= (4<<i);
        delay(1);
    }



}
/*==================================EXTRAS=========================================*/

void display_partially(unsigned char array[],int size)
{
    for(unsigned char i = 0;i<size;i++)
    {
        SSD_CONTROL_PORT &= ~(0x3C);
        SSD_DATA_PORT = array[i];
        SSD_CONTROL_PORT |= (4<<i);
        delay(1);    
    }
}

void print_number_on_ssd(unsigned char number)
{
    unsigned char ssd_display_buf[4];
    ssd_display_buf[0] = ssd_numbers[(number / 1000) % 10];
    ssd_display_buf[1] = ssd_numbers[(number / 100) % 10];
    ssd_display_buf[2] = ssd_numbers[(number / 10) % 10];
    ssd_display_buf[3] = ssd_numbers[number % 10];
    display(ssd_display_buf);

}
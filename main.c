/* 
 * File:   main.c
 * Author: shaunvarghese
 *
 * Created on September 18, 2026, 9:46 AM
 */

#pragma config WDTE = OFF


//#include "xc.h"
#include "ssd.h"
#include "keypad.h"
#include "uart.h"
#include "main.h"
#include "timer0.h"

extern volatile unsigned char packet_received = 0;
extern volatile unsigned char receive_buf[];
extern unsigned char ssd_numbers[];


unsigned char my_port_id;

volatile unsigned int blink_count; //for timer0 led blinking
volatile unsigned char blink_active = 0;

void init_config(void) 
{
    init_ssd_display();
    digital_keypad_init();
    init_uart(9600);
    timer0_init();
    PICKTOLIGHT_PORT = 0; //LED port as output
    PICKTOLIGHT = 0;

    eeprom_write(EEPROM_NODEID_ADDRESS,DEFAULT_NODE_ID);
    

}

void main(void) 
{
    state_t mode = OPERATION_MODE;
    my_port_id = eeprom_read(EEPROM_NODEID_ADDRESS);
    init_config();
    unsigned char key;
    while (1) 
    {
        key = read_digital_keypad(STATE);
        if(key == MODE)
        {
            if(mode == OPERATION_MODE)
                mode = CONFIG_MODE;
            else
                mode = OPERATION_MODE;
        }
        switch(mode)
        {
            case OPERATION_MODE:
            {
                
                operation_mode();   
            }
            case CONFIG_MODE:
            {

            }
        }

    }

}

void operation_mode()
{
    SSD_CONTROL_PORT &= ~(0x3C); // all ssd's OFF at first
    unsigned char received_port_id = 0,stock_needed = 0;
    
    unsigned char  stock_received;
    while(1)
    {
        //if data is received
        if(packet_received)
        {
            //build node id from packet, first two characters of the packet into an integer
            get_port_id_and_stock(&received_port_id,&stock_needed);
            //confirm whether its the same node id as this node
            if(my_port_id == received_port_id)
            {
                //if yes then print the data on the ssd
                print_on_ssd(stock_needed);
                //display the stock

                packet_received = 0;
                stock_received= stock_picking_handler(stock_needed);
                //transmission enabled
                TXEN = 1;
                send_stock_received_info(stock_received);
                TXEN = 0;
                //end of transmission
                
                return;

            }
            else
            {
                //not for me 
                packet_received = 0;
                continue;
            }

        }
    }
    
}
unsigned char stock_picking_handler(unsigned char stock_received)
{
    unsigned char key = ALL_RELEASED;
    blink_active = 1;
    while(1)
    {      
        print_on_ssd(stock_received);
        key = read_digital_keypad(STATE);
        if(key == ACK)
        {
            blink_active = 0;
            return stock_received;
        }
        else if(key == INC && stock_received < 255)
        {
            ++stock_received;
        }
        else if(key == DEC && stock_received > 0 )
        {
            --stock_received;    
        }
            
    }
}
    
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

void print_on_ssd(unsigned char number)
{
    unsigned char ssd_display_buf[4];
    ssd_display_buf[0] = ssd_numbers[(number / 1000) % 10];
    ssd_display_buf[1] = ssd_numbers[(number / 100) % 10];
    ssd_display_buf[2] = ssd_numbers[(number / 10) % 10];
    ssd_display_buf[3] = ssd_numbers[number % 10];
    display(ssd_display_buf);

}

void get_port_id_and_stock(unsigned char* received_port_id,unsigned char* stock_needed)
{
    unsigned char i=0;
    *received_port_id = 0;
    *stock_needed = 0;
    while(receive_buf[i] && receive_buf[i] !=',')
    {
        *received_port_id = (*received_port_id * 10) + (receive_buf[i] - '0');
        ++i;
    }
    ++i;
    while(receive_buf[i])
    {
        *stock_needed = (*stock_needed*10) + (receive_buf[i] -  '0');
        ++i;
    }
}

/*======================================================================================*/
/*----------------------------CONFIGURATION MODE------------------------*/
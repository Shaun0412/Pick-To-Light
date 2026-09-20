/* 
 * File:   main.c
 * Author: shaunvarghese
 *
 * Created on September 18, 2026, 9:46 AM
 */

#pragma config WDTE = OFF


#include "xc.h"
#include "ssd.h"
#include "keypad.h"
#include "uart.h"
#include "main.h"
#include "timer0.h"

extern volatile unsigned char packet_received;
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
    
    

}

void main(void) 
{
    state_t mode = OPERATION_MODE;
    unsigned char nodeid_change_flag = eeprom_read(NODE_ID_CHANGED_MAGICSTRING_ADDR);
    if(nodeid_change_flag != NODE_ID_CHANGED_IDENTIFICATION)
    {
        eeprom_write(EEPROM_NODEID_ADDRESS,DEFAULT_NODE_ID);
    }
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
                if(packet_received)
                {
                    //call active mode in operation mode
                    operation_mode();  
                }
                else
                {
                    //keep the display off thats all
                    SSD_CONTROL_PORT &= ~(0x3C); // all ssd's OFF at first
                }
                break; 
            }
            case CONFIG_MODE:
            {
                configuration_mode();
                mode = OPERATION_MODE;
                break;

            }
            default:
                continue;
        }

    }

}

void operation_mode()
{
    
    unsigned char received_port_id = 0,stock_needed = 0;
    
    unsigned char  stock_received;
    
    //build node id from packet, first two characters of the packet into an integer
    get_port_id_and_stock(&received_port_id,&stock_needed);

    //clear the packet_received flag
    packet_received = 0;
    //confirm whether its the same node id as this node
    if(my_port_id == received_port_id)
    {
        //if yes then print the data on the ssd
        print_number_on_ssd(stock_needed);
        //display the stock
        stock_received= stock_picking_handler(stock_needed);
        //transmission enabled
        TXEN = 1;
        send_stock_received_info(stock_received);
        TXEN = 0;
        //end of transmission
        
        
    }
    
    return;
    
    
}
unsigned char stock_picking_handler(unsigned char stock_received)
{
    unsigned char key = ALL_RELEASED;
    blink_count = 0;
    blink_active = 1;
    while(1)
    {      
        print_number_on_ssd(stock_received);
        key = read_digital_keypad(STATE);
        if(key == ACK)
        {
            blink_active = 0;
            PICKTOLIGHT = 0;
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



#include <xc.h>
#include "main.h"
#include "keypad.h"
#include "ssd.h"
#include "uart.h"

#define UPDATE_STOCK 10
#define UPDATE_NODE_ID 11
unsigned char ssd_message_update_stock[] = {0x3E,0x40,0x6D,0x78};
unsigned char ssd_message_node_id_update[] = {0x54,0x40,0x10,0x5E};



void configuration_mode()
{
    unsigned char key = ALL_RELEASED;
    unsigned char mode =  UPDATE_STOCK;
    display(ssd_message_update_stock);
    while(1)
    {

        if(mode == UPDATE_STOCK)
            display(ssd_message_update_stock);
        else if(mode == UPDATE_NODE_ID)
            display(ssd_message_node_id_update);
        
        key = read_digital_keypad(STATE);
        switch(key)
        {
            case CONFIRM:
            {
                //togggle between nodes
                if(mode == UPDATE_STOCK)
                {
                    mode = UPDATE_NODE_ID;

                }
                else if(mode == UPDATE_NODE_ID)
                {
                    mode = UPDATE_STOCK;
                }
                break;
            }
            

            case ACK:
            {
                //select a node
                if(mode == UPDATE_STOCK)
                {                    
                    update_stock();
                    
                }
                else if(mode == UPDATE_NODE_ID)
                {
                    update_node_id();
                }
                return;
            }
            case MODE:
            {
                return;
            }
            default:
                continue;


        }
    }
}

void update_stock()
{
    unsigned char stock_count = 0;
    unsigned char key = ALL_RELEASED;
    //
    while(1)
    {
        print_number_on_ssd(stock_count);
        key = read_digital_keypad(STATE);
        if(key == INC && stock_count < 255)
        {
            ++stock_count;
        }
        else if(key == DEC && stock_count > 0)
        {
            --stock_count;
        }
        else if(key == ACK)
        {
            //send the stock_count to server
            send_stock_received_info(stock_count);
            return;
        }
        else if(key == MODE)
        {
            return;
        }
        else
            continue;
    
    }

}

void update_node_id()
{
    unsigned char key = ALL_RELEASED;
    unsigned char new_node_id = 0;
    while(1)
    {
        print_number_on_ssd(new_node_id);
        key = read_digital_keypad(STATE);
        if(key == INC && new_node_id < 255)
        {
            ++new_node_id;
        }
        else if(key == DEC && new_node_id > 0)
        {
            --new_node_id;
        }
        else if(key == ACK)
        {
            eeprom_write(NODE_ID_CHANGED_MAGICSTRING_ADDR,NODE_ID_CHANGED_IDENTIFICATION);
            eeprom_write(EEPROM_NODEID_ADDRESS,new_node_id);
            return;
        }
        else if(key == MODE)
        {
            return;
        }
        else
        {
            continue;
        }

    }

    
}
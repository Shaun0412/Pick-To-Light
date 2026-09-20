/* 
 * File:   main.h
 * Author: shaunvarghese
 *
 * Created on September 18, 2026, 12:10 PM
 */

#ifndef MAIN_H
#define	MAIN_H

#define PICKTOLIGHT_PORT TRISB7
#define PICKTOLIGHT RB7

#define DEFAULT_NODE_ID 10
#define EEPROM_NODEID_ADDRESS 0X01

#define NODE_ID_CHANGED_MAGICSTRING_ADDR 0x00
#define NODE_ID_CHANGED_IDENTIFICATION 0xAA

typedef enum
{
    OPERATION_MODE,
    CONFIG_MODE,
    UPDATE_STOCK,
    SET_NODE_ID,
    SEND_PICKED
}state_t;



/*--------------prototypes-------------*/
//utils
void get_port_id_and_stock(unsigned char* received_port_id,unsigned char* stock_needed);
void print_number_on_ssd(unsigned char number);
void send_stock_received_info(unsigned char stock_received);
void send_number(unsigned char number);


/*-------------operation mode------------------*/
void operation_mode();
unsigned char stock_picking_handler(unsigned char stock_received);


/*======================================================================================*/
/*----------------------------CONFIGURATION MODE------------------------*/
void configuration_mode();
void update_stock();
void update_node_id();

#endif	/* MAIN_H */


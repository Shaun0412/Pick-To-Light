/* 
 * File:   SSD.h
 * Author: shaunvarghese
 *
 * Created on July 30, 2026, 12:28 PM
 */

#ifndef SSD_H
#define	SSD_H


#define MAX_SSD_CNT 4 
#define SSD_DATA_PORT_DDR TRISD 
#define SSD_CONTROL_PORT_DDR TRISA 
#define SSD_DATA_PORT PORTD 
#define SSD_CONTROL_PORT PORTA 


/* For Common Cathode Display 
*/ 
#define ssd_ZERO 0x3F 
#define ssd_ONE 0x06 
#define ssd_TWO 0x5B 
#define ssd_THREE 0x4F 
#define ssd_FOUR 0x66 
#define ssd_FIVE 0x6D 
#define ssd_SIX 0x7D 
#define ssd_SEVEN 0x07 
#define ssd_EIGHT 0x7F 
#define ssd_NINE 0x6F



void init_ssd_display(void);
void display(unsigned char array[]);
void display_partially(unsigned char array[],int size);

#endif	/* SSD_H */


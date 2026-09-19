/* 

 * Author: shaunvarghese
 *
 * Created on July 31, 2026, 10:22 PM
 */

#ifndef KEYPAD_H
#define	KEYPAD_H


#define KEYPORT TRISB


#define LEVEL 5
#define STATE 6

#define ALL_RELEASED 0x3F
#define MODE 0x3E //RB0
#define CONFIRM 0x3D //RB1
#define ACK 0x3B //RB2
#define INC 0x37 //RB3
#define DEC 0x2F //RB4
#define SW5 0x1F //RB5

void digital_keypad_init(void);

unsigned char read_digital_keypad(unsigned char mode);


#endif	/* KEYPAD_H */


/*************************************************************************
Title:    Testing output to a HD44780 based LCD display.
Author:   Peter Fleury  <pfleury@gmx.ch>  http://tinyurl.com/peterfleury
File:     $Id: test_lcd.c,v 1.8 2015/01/31 18:04:08 peter Exp $
Software: AVR-GCC 4.x
Hardware: HD44780 compatible LCD text display
          AVR with external SRAM interface if memory-mapped LCD interface is used
          any AVR with 7 free I/O pins if 4-bit IO port mode is used
**************************************************************************/

/******************************************************************************
2
3     Program to learn the use of Multiplexed 4x3 keypad with AVR Microcontroller.
4
5     Specific Skills Required
6        >> AVR GPIO details.(http://bit.ly/aq3ouw)
7        >> LCD Library.(http://bit.ly/agVUVc)
8        >> Operations on bits using C.(http://bit.ly/aFqg5n)
9
10
11    General Skills Required
12       >> AVR Studio Setup and use. (http://bit.ly/aZ43SZ)
13       >> avr-gcc setup and use.
14
15
16    Hardware
17    --------
18    ATmega32 @ 16MHz external crystal.
19    Fuse Byte setting HIGH = C9 and LOW = FF (MOST IMP.)
20
21
22    LCD   <->   AVR Connection
23
24       VSS ->GND
25       VDD ->+5V
26       VEE -> CENTER PIN OF 10K POT (OTHER TWO PIN OF POT TO +5V AND GND)
27             ADJ. THE POT UNTIL YOU HAVE A CLEAR TEXT DISPLAY.
28
29       RS -> PD3
30       RW -> PD6
31       E  -> PB4
32
33       DB0 -> N/C
34       DB1 -> N/C
35       DB2 -> N/C
36       DB3 -> N/C
37
38       DB4 -> PB0
39       DB5 -> PB1
40       DB6 -> PB2
41       DB7 -> PB3
42
43       LED+ ->+5V (VIA 100 OHM RES)
44       LED- ->GND
45
46    KEYPAD
47
48       COL1 ->  PA6
49       COL2 ->  PA5
50       COL3 ->  PA4
51
52       ROW1 ->  PA3
53       ROW2 -> PA2
54       ROW3 ->  PA1
55       ROW4 -> PA0
56
57                                NOTICE
58                               --------
59    NO PART OF THIS WORK CAN BE COPIED, DISTRIBUTED OR PUBLISHED WITHOUT A
60    WRITTEN PERMISSION FROM EXTREME ELECTRONICS INDIA. THE LIBRARY, NOR ANY PART
61    OF IT CAN BE USED IN COMMERCIAL APPLICATIONS. IT IS INTENDED TO BE USED FOR
62    HOBBY, LEARNING AND EDUCATIONAL PURPOSE ONLY. IF YOU WANT TO USE THEM IN
63    COMMERCIAL APPLICATION PLEASE WRITE TO THE AUTHOR.
64
65
66    WRITTEN BY:
67    AVINASH GUPTA
68    me@avinashgupta.com
69
70
71
72    ******************************************************************************/
#include <stdlib.h>
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <util/delay.h>
#include "lcd.h"
// #include "myutils.h"
 
#define USART_BAUDRATE 9600
#define BAUD_PRESCALE (((F_CPU/(USART_BAUDRATE*16UL)))-1)
#define MAX_DATA_LEN 20
#define KEY_ADDR 0

#define OK 12	// OK on keypad 		// @harshana
#define BACKSPACE 10	// OK on keypad 		// @harshana


#define KEYPAD A  //KEYPAD IS ATTACHED ON PORTA

//Don't Touch the lines below
//*******************************
#define KEYPAD_PORT PORT(KEYPAD)
#define KEYPAD_DDR   DDR(KEYPAD)
#define KEYPAD_PIN   PIN(KEYPAD)
//*******************************


/*******************************************
91
92    Function return the keycode of keypressed
93    on the Keypad. Keys are numbered as follows
94
95    [00] [01] [02]
96    [03] [04] [05]
97    [06] [07] [08]
98    [09] [10] [11]
99
100   Arguments:
101      None
102
103   Return:
104      Any number between 0-11 depending on
105      keypressed.
106
107      255 (hex 0xFF) if NO keypressed.
108
109   Precondition:
110      None. Can be called without any setup.
111
112   *******************************************/



char recieved_byte;
char num_buffer[7];
uint8_t data_buffer[MAX_DATA_LEN];


void usart_init(){

	UCSR0B |= (1<<RXEN0)  | (1<<TXEN0);		// Turn on the transmission and reception circuitry
	//UCSR0C = 0							// USART = 00, NoParity = 00, StopBit = 0
	UCSR0C |= (1<<UCSZ00) | (1<<UCSZ01);	// Use 8-bit character sizes
	UBRR0H  = (BAUD_PRESCALE >> 8);			// Load upper 8-bits of the baud rate value into the high byte of the UBRR register
	UBRR0L  = BAUD_PRESCALE;				// Load lower 8-bits of the baud rate value into the low byte of the UBRR register	
}

void usart_send(char c){
	while( ( UCSR0A & ( 1 << UDRE0 ) ) == 0 ){}
	UDR0 = c;
}

void usart_receive(){
	while( ( UCSR0A & ( 1 << RXC0 ) ) == 0 ){}
	recieved_byte = UDR0;
}

void EEPROMwrite(unsigned int address, char data){

	// wait for completion of previous write
	while (EECR & (1<<EEPE));

	// set up address and data regs
	EEAR = address;
	EEDR = data;

	// write logical 1 to EEMPE
	EECR |= (1<<EEMPE);

	// start eeprom write
	EECR |= (1<<EEPE);
}

char EEPROMread(unsigned int address){

	// wait for completion of previous write
	while (EECR & (1<<EEPE));

	// set up address
	EEAR = address;

	// start eeprom read
	EECR |= (1<<EERE);

	return EEDR;
}

/**
* On key press return the index of key
* eg : R1, C1, index = 0 ; R2, C1, index = 3
* Note values and index are different
* Value = 1, index = 0
* Value = 3, index = 2
* ...
* Vale = 0, index = 10
*/
uint8_t GetKeyPressed(){		// @harshana
	uint8_t r,c;

	KEYPAD_PORT|= 0X0F;

	while(1){
		for(c=0;c<3;c++){
			KEYPAD_DDR&=~(0X7F);

			KEYPAD_DDR|=(0X40>>c);
			for(r=0;r<4;r++){
				if(!(KEYPAD_PIN & (0X08>>r))){
					return (r*3+c);
				}
			}
		}
	}

	return 0XFF;//Indicate No key pressed
}

uint8_t initiate(){

	lcd_clrscr();     /* clear display home cursor */
	_delay_ms(1000);

	uint8_t key;

	lcd_puts( "1 : Change key\n" );
	lcd_puts( "2 : Encrypt" );

    while(1){
        key=GetKeyPressed(); //Get the keycode of pressed key

        if (key == 0x00 || key == 0x01){
        	return key;
        }
    }
}

void keyToVal(){
	unsigned char i;
	for (i = 0; i < MAX_DATA_LEN && data_buffer[i] != 0xFF; ++i){
		if (data_buffer[i] < 9){
			data_buffer[i] += 1;
		}else{
			data_buffer[i] = 0x00;
		}
	}
}

uint8_t keyVal(uint8_t i){
	if (i < 9){
		return i+1;
	}else{
		return 0;
	}
}

// Get int upto count number of characters
// If count is 0 len is unknown
void getInt(uint8_t count){
    
	uint8_t key = 0XFF;
	uint8_t i = 0;
	data_buffer[0] = 0xFF;
	uint8_t num;
	char num_c;

	// If count is 0 input is unknown lenght
	count = count == 0 ? MAX_DATA_LEN : count;

	while(1){

		key = GetKeyPressed();

	    // while(1){
	    //     key=GetKeyPressed(); //Get the keycode of pressed key

	    //     if (key != 0xFF) break;
	    // }


	    if (key == BACKSPACE){
	    	// Delete from screen

	    	// Delete from buffer
	    	i = i-1 < 0 ? 0 : i-1;
	    	data_buffer[i] = 0xFF;
	    }else if (key == OK){
	    	data_buffer[i] = 0xFF;
	    	return;
	    }else if (i < count){
	    	// key to actula value
	    	num = keyVal(key);

			// convert interger into string
			num_c = num + '0';
	        
	    	// Write to screen
	        lcd_gotoxy(i,1);
	        lcd_putc(num_c);

	    	// Write to buffer
	    	data_buffer[i++] = num;
	    }
	}
}


void encryptUtil(){
	uint8_t i;
	uint8_t key = (uint8_t) EEPROMread(KEY_ADDR);

	for (i = 0; i < MAX_DATA_LEN && data_buffer[i] != 0xFF; ++i){
		data_buffer[i] = (data_buffer[i] + key) % 10;
	}

}

void showVal(){
    uint8_t i;
    char num_c;

    // lcd_clrscr();

	for (i = 0; i < MAX_DATA_LEN && data_buffer[i] != 0xFF; ++i){
		num_c = data_buffer[i] + '0';
		// Write to LCD
        lcd_gotoxy(i,1);
        lcd_putc(num_c);
	}

}

void changeKey(){
    lcd_clrscr();
	lcd_puts( "Enter Key\n");
	getInt(1);		// Get one char
	// keyToVal();		// Keypad values to actual values
	EEPROMwrite(KEY_ADDR, data_buffer[0]);

    lcd_clrscr();
	lcd_puts( "Key changed\n");
}

void encrypt(){
    lcd_clrscr();
	lcd_puts( "Enter Val\n");
	getInt(0);		// Get random len char
	// keyToVal();		// Keypad values to actual values
	encryptUtil();	// Encrypt

    lcd_clrscr();
	lcd_puts( "Encrypted message: \n");
	showVal();		// Show data buffer on screen
}


int main(void){

	usart_init();

    lcd_init(LCD_DISP_ON);

    uint8_t key;

    while(1){

	    // Change key or encrypt
	    key = initiate();


	    if (key == 0x00){
	    	// Change key
	    	changeKey();
	    	usart_send(key + '0');
	    }else if(key == 0x01){
	    	// Encrypt
	    	encrypt();
	    	usart_send(key + '0');
	    }

	    // Wait till key press
	    key = GetKeyPressed();
		// while(1){
		// 	key=GetKeyPressed(); //Get the keycode of pressed key
		// }


    }

}
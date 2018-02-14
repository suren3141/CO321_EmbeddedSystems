#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
unsigned char count = 0;

ISR(INT0_vect){ // handler for INT0 interrupt
	count = (count << 1);
	PORTB = (count >> 2);
	PORTD = (count << 6);
}

ISR(INT1_vect){ // handler for INT0 interrupt
	count = (count << 1) | 1;
	PORTB = (count >> 2);
	PORTD = (count << 6);
}


int main(void) {

	DDRD = 0b11000000;	//PD
	DDRB = 0xFF;		//PB0

	EICRA |= 0b1111;	//Rising edge
	EIMSK |= 0b11;		//Enable interrupt

	sei();

	while(1){}

	return 0;
}
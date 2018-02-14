#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
volatile unsigned char count = 0;

ISR(INT0_vect){ // handler for INT0 interrupt
	count  = (count+1) % 64;
	PORTB = count;
}

int main(void) {

	DDRD &= ~(1<<2);	//PD2

	DDRB |= 0xFF;		//PB0

	EICRA |= 0b10;	//Fallindg edge
	EIMSK |= (1<<INT0);

	sei();

	while(1){};

	return 0;
}
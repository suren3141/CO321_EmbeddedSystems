#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

ISR(INT0_vect){ // handler for INT0 interrupt
	PORTB = PORTB ^ (1<<0);
}

int main(void) {

	DDRD &= ~(1<<2);	//PD2

	DDRB |= (1<<0);		//PB0

	EICRA |= 0b10;	//Rising edge

	sei();

	EIMSK |= (1<<INT0);

	while(1){};

	return 0;
}
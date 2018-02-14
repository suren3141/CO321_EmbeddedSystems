#include <avr/io.h>
#include <avr/interrupt.h>
volatile int overflow_count;
ISR(TIMER1_OVF_vect){ // handler for Timer1 overflow interrupt
	TCNT1 = 0x85EE;
	PORTB = PORTB ^ 1<<5;
}

ISR(TIMER0_OVF_vect){ // handler for Timer0 overflow interrupt
	if (overflow_count == 12){
		TCNT0 = 0xCB;
		overflow_count = 0;
		PORTB = PORTB ^ 1<<4;
	}
	overflow_count ++;
	TCNT0 = 0x00;
}

int main(void) {
	DDRB = 0xFF; // set port B for output
	PORTB = 0x00; // initial value of PORTB

	//Timer0
	overflow_count = 0; // initialise overflow count
	TCCR0A = 0x00; // normal mode
	TCCR0B = 0x04; // no prescaler, internal clock
	TIMSK0 = 0x01; // enable Timer 1 overflow interrupt
	TCNT0 = 0x00;


	//Timer1
	TCCR1A = 0x00; // normal mode
	TCCR1B = 0x04; // no prescaler, internal clock
	TIMSK1 = 0x01; // enable Timer 1 overflow interrupt
	TCNT1 = 0x85EE;

	sei(); 
	while (1){} // infinite loop
	return 0;
}
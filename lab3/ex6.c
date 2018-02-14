#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

unsigned char count =0;

ISR (TIMER0_OVF_vect){    // Timer1 ISR
	count++;
	if(count==24){
		PORTB= PORTB ^ (1 << 5);
		TCNT0 = 0x96;	//150
		count=0;
	}

}


void main(int argc, char const *argv[]){
	DDRB = 0xFF;
	PORTB = 0x00;
	
	TCCR0A=0b00000000;//Normal mode
	TCCR0B = 0b00000100;//(1<<CS12) ;//256 prescaler
	TCNT0 = 0x96;	//150
	TIMSK0 = (1 << TOIE0) ;   // Enable timer1 overflow interrupt(TOIE1)

	sei();
	while(1){
	}
}

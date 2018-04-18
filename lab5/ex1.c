#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

//16 MHz / (256 * 16) = 976.56

int main(void) {

	DDRD |= (1<<6);				//PD6 (OC0A)

	TCCR0A = (1 << WGM01) | (1 << WGM00);			//Fast PWM
	TCCR0A |= (1 << COM0A1);			//Non inverting
	TCCR0B = 0x03;//(1 << CS01) | (1 << CS00)	// 64 prescaler 

	OCR0A = 0b011111111; 			//127(+1)
	//OCR0A = 0xFF; 			//255(+1)
	sei();

	while(1){};

	return 0;
}
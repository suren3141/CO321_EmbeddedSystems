#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#define BLINK_DELAY_MS 2000
//16 MHz / (256 * 16) = 976.56

int main(void) {

	unsigned char i;

	DDRD |= (1<<6);				//PD6 (OC0A)

	TCCR0A = (1 << WGM01) | (1 << WGM00);			//Fast PWM
	TCCR0A |= (1 << COM0A1);			//Non inverting
	TCCR0B = 0x01;//(1 << CS01) | (1 << CS00)	// 64 prescaler 

	//OCR0A = 0b001111111; 			//127(+1)
	OCR0A = 0x7F; 			//255(+1)
	sei();

	while(1){
		for (i = 1; i < 6; i++){
			TCCR0B = i;
			_delay_ms(BLINK_DELAY_MS);			
			TCCR0B = 0;
		}
	};

	return 0;
}
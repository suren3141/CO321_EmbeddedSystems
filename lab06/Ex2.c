#include <avr/io.h>
#include <avr/interrupt.h>

unsigned char x;
unsigned char y;
unsigned char comp;

// ISR(ADC_vect){ // handler for ADC interrupt
// 	//x++;
// 	PORTB = ADCH;

// 	// if (x >= 128){
// 	// PORTB = 0xFF;		
// 	// }
// 	// else{
// 	// PORTB = 0x00;		
// 	// }
// 	//PORTB |= (1 << PB5);
// 	//PORTD = ADCH;
// 	// if (ADCH > 128){
// 	// 	PORTB = 0xFF;
// 	// }else{
// 	// 	PORTB = 0;
// 	// }

// 	//ADCSRA |= (1<<ADSC);
// }

int main(void) {
	DDRC &= ~(1<<1);				//PC1

	DDRB = 0xFF;
	DDRD |= (1<<6);				//PD6 (OC0A)

	PORTB = 0;

	// PORTB |= (1<<5);

	// ADCSRA = 0b10000001;
	// ADCSRA |= (1<<ADIF);			// Clear ADC


	// PORTB |= (1<<5);

	ADCSRA |= (1 << ADEN);			// Enable ADC
	ADCSRA |= (1 << ADPS2);			// 16 Prescaler
	ADCSRA |= (1<<ADIE);			// Interrupt enable (bit 3)

	// ADMUX = 0b01100001;

	// PORTB |= (1<<5);

	ADMUX &= ~(1<<REFS1);
	ADMUX |= (1<<REFS0);			// Reference Voltage (bit 7:6)	
	ADMUX |= (1 << MUX0);			// PC1			(bit 3:0)
	ADMUX |= (1<<ADLAR);			// Left align (bit 5)


	TCCR0A = (1 << WGM01) | (1 << WGM00);			//Fast PWM
	TCCR0A |= (1 << COM0A1);			//Non inverting
	TCCR0B = 0x03;//(1 << CS01) | (1 << CS00)	// 64 prescaler 

	sei();

	while(1){

		ADCSRA |= (1<<ADSC);

		while(ADCSRA & (1<<ADIF)==0)

		x = ADCL;
		y = ADCH;


		if (y < 100){
			y = 0;
		}else{
			y -= 100;
			y *= 2;
		}


		OCR0A = y;


		ADCSRA |= (1<<ADIF);			// Clear ADC flag

	}

	return 0;
}
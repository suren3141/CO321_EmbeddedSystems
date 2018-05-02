#include <avr/io.h>
#include <util/delay.h>


int main(){
	DDRB=DDRB| (1<<5); // Now PORTB[0] is the output pin for bulb
	DDRC=DDRC & ~1; // Now PINC[0] is input
	
	//ADMUX register
	ADMUX=ADMUX & (~(1<<REFS0) & ~(1<<REFS1)); //Use the external reference AREF pin voltage instead of internal pin 
	ADMUX=ADMUX|(1<<ADLAR); //8 bits to Left register ADCH, and 2 bits to ADCL
	ADMUX=ADMUX & (~(1<<MUX0) & ~(1<<MUX1) & ~(1<<MUX2) & ~(1<<MUX3)); //ADC0 pin is the analog in of the comparator


	//ADCSRA register

	ADCSRA=ADCSRA & ~(1<<ADEN);//Analog comparator multiplexer enabled
	ADCSRB=ADCSRB|(1<<ACME);//The ADC is switched off
	ADCSRA=ADCSRA & ~(1<<ADATE);//Auto trigger disabled
	ADCSRA=(ADCSRA | 1<<ADPS0 | 1<<ADPS1 ) & ~(1<<ADPS2);//The input clock to ADC is 8 times slower than the system clock

//	ADCSRA=ADCSRA | (1<<ADIF);//Clear the polling flag for conversion completion
	ADCSRA=ADCSRA & ~(1<<ADIE);//Clear the polling flag by clearing interrupt

	ADCSRA=ADCSRA | (1<<ADSC);//Start conversion
	
	while(1){
		ADCSRB=ADCSRB & ~(1<<ACME);//The ADC is switched ON


		while(ADCSRA & (1<<ADIF)==0){//polling for the conversion to finish
		}

		if(ADCH>128){//The data is read from ADCH
			PORTB=PORTB|(1<<5);// write the result to PORTB[0]
		}
		else{
			PORTB=PORTB & ~(1<<5);// write the result to PORTB[0]
		}
	}


	return 0;
}

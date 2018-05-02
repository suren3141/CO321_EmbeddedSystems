#include <avr/io.h>
#include <util/delay.h>


int main(){
	DDRB=DDRB| 0b1111111; // Now PORTD[0,1] is the output pin for bulb
	DDRC=DDRC & ~(1<<1); // Now PINC[1] is input
	
	//ADMUX register
	ADMUX=(ADMUX & ~(1<<REFS1)) | (1<<REFS0); //The internal Vref is used
	ADMUX=ADMUX|(1<<ADLAR); //8 bits to Left register ADCH, and 2 bits to ADCL
	ADMUX=ADMUX & (~(1<<MUX1) & ~(1<<MUX2) & ~(1<<MUX3)); 
	ADMUX=ADMUX | 1<<MUX0; //ADC1 pin is the analog in of the comparator

	//ADCSRA register

	ADCSRA=ADCSRA & ~(1<<ADEN);//Analog comparator multiplexer enabled
	ADCSRB=ADCSRB|(1<<ACME);//The ADC is switched off
	ADCSRA=ADCSRA & ~(1<<ADATE);//Auto trigger disabled
	ADCSRA=(ADCSRA | 1<<ADPS0 | 1<<ADPS1 ) & ~(1<<ADPS2);//The input clock to ADC is 8 times slower than the system clock

	ADCSRA=ADCSRA | (1<<ADIF);//Clear the polling flag for conversion completion
	//ADCSRA=ADCSRA & ~(1<<ADIE);//Clear the polling flag by clearing interrupt

	
	while(1){


		PORTB=PORTB| (1<<5);_delay_ms(400);PORTB=PORTB& ~(1<<5);_delay_ms(400);		
		
		ADCSRB=ADCSRB & ~(1<<ACME);//The ADC is switched ON	
		ADCSRA=ADCSRA | (1<<ADSC);//Start conversion

		while(ADCSRA & (1<<ADIF)==0){//polling for the conversion to finish
		
		}



		if(ADCH>128){//The data is read from ADCH
			PORTB=2;// write the result to PORTB[0]
		}
		else{
			PORTB=1;// write the result to PORTB[0]
		}
	}


	return 0;
}

#include <avr/io.h>
#include <util/delay.h>

#define BLINK_DELAY_MS 1000

void delay_timer0(){
	//3036
	TCNT1H = 0x0B;
	TCNT1L = 0xDC;

	TCCR1A = 0x00;
	TCCR1B = 0x04;

	while ((TIFR1 & 0x01) == 0);

	TCCR1A = 0x00;
	TCCR1B = 0x00;

	TIFR1 = 0x01;

}

int main(void){

	DDRB = DDRB | (1<<5);

//	unsigned char arr[] = {1, 2, 3, 4, 3, 2};
	unsigned char arr[] = {2, 1, 2, 3, 4, 3, 2};
	unsigned char i;
	unsigned char val1;
	unsigned char val2 = 0;

	PORTB = PORTB | 1<<2;


	while(1){
		/*
		for (i = 0; i < 6; i++){
			val1 = (1<<arr[i])
			val2 = val2 ^ (1<<5)
			PORTB = val1|val2;
			_delay_ms(BLINK_DELAY_MS);
		}*/

		
		for (i = 0; i < 6; i++){
			PORTB = PORTB ^ ((1<<5) | 1<<arr[i+1] | 1<<arr[i]);
			_delay_ms(BLINK_DELAY_MS);
		}
	}


}
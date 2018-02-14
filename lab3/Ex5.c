#include <avr/io.h>
#include <util/delay.h>
#include<avr/interrupt.h>

#define TRUE 1
#define FALSE 0

#define FREQ 16


int state_LED =0;
int state_dir=0;
int state_KR=0;

ISR (TIMER1_OVF_vect){    // Timer1 ISR
	if(state_LED){
		PORTB=0b100000;
	}
	else{
		PORTB=0;
	}


	PORTB = PORTB | (1<<state_KR);

	if(state_dir){
		if(state_KR==3){
			state_dir=~state_dir;
			state_KR--;
		}
		else state_KR++;
	}
	else{
		if(state_KR==0){
			state_dir=~state_dir;
			state_KR++;
		}
		else state_KR--;
	}

		
	state_LED=~state_LED;

}


void main(int argc, char const *argv[]){
	DDRB=0b101111;
	TCCR1A=0b00000000;//Normal mode
	TCCR1B = 0b00000100;//(1<<CS10) | (1<<CS12);//1024 prescaler
	TIMSK1 = (1 << TOIE1) ;   // Enable timer1 overflow interrupt(TOIE1)

	sei();
	while(1){
	}









}

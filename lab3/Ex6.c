#include <avr/io.h>
#include <util/delay.h>
#include<avr/interrupt.h>

#define TRUE 1
#define FALSE 0

#define FREQ 16


unsigned char count =0;
int state_dir=0;
int state_KR=0;



ISR (TIMER0_OVF_vect){    // Timer1 ISR
	count++;
	if(count==7){
		PORTB=~PORTB & 1 ;
		count=0;
	}

}


void main(int argc, char const *argv[]){
	DDRB=1;
	TCCR0A=0b00000000;//Normal mode
	TCCR0B = 0b00000101;//(1<<CS10) | (1<<CS12);//1024 prescaler
	
	TIMSK0 = (1 << TOIE0) ;   // Enable timer1 overflow interrupt(TOIE1)

	sei();
	while(1){
	}









}

#include <avr/io.h>
#include <util/delay.h>

#define TRUE 1
#define FALSE 0

#define FREQ 16

enum {INPUT,OUTPUT};
enum {LOW,HIGH};



void pinMode(int pin,int mode){
	if(pin<=7){
		if(mode==OUTPUT){
			DDRD=DDRD | (1<<pin);
		}
		else{
			DDRD=DDRD & ~(1<<pin);	
		}
	}
	else{
		if(mode==OUTPUT){
			DDRB=DDRB | (1<<(pin-8));
		}
		else{
			DDRB=DDRB & ~(1<<(pin-8));	
		}
	}
}


void digitalWrite(int pin,int level){
	if(pin<=7){
		if(level==HIGH){
			PORTD=PORTD | (1<<pin);
		}
		else{
			PORTD=PORTD & (~(1<<pin));
		}
	}
	else{
		if(level==HIGH){
			PORTB=PORTB | (1<<(pin-8));
		}
		else{
			PORTB=PORTB & (~(1<<(pin-8)));
		}
	}
}

void delayCycles(long cycles){
	

	TCCR0A=0x00; //Normal mode

	long temp=cycles;
	int prescaler=1;
	while(temp>256){
		prescaler=prescaler+1;
		temp/=256;
	}

	TCNT0=(256-temp);
	TCCR0B=prescaler;

	while(!(TIFR0 & 0x01));

	TCCR0A=0x00; //Stop timer
	TCCR0B=0x00; //Stop timer

	TIFR0=0x01; //Clear the overflow bit

}

void delayMicroSec(long us){
	delayCycles(FREQ*us);

}


void delayMilliSec(int ms){
	delayMicroSec(1000l*ms);
}

void delay(int s){
	delayMilliSec(1000l*s);
}


int main(){
	int x;
	pinMode(13,OUTPUT);
	while(TRUE){
		digitalWrite(13,HIGH);	
		delay(1);
		for(x=10;x<14;x++)digitalWrite(x,LOW);	
		delay(2);
	}	

	return 0;
}
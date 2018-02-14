#include <avr/io.h>
#include <util/delay.h>

#define TRUE 1
#define FALSE 0


enum {INPUT,OUTPUT};
enum {LOW,HIGH};

void delay(){
	_delay_ms(1000);
}

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

int main(){
	int x;
	for(x=10;x<14;x++)pinMode(x,OUTPUT);
	while(TRUE){
		for(x=10;x<14;x++)digitalWrite(x,HIGH);	
		_delay_ms(200);
		for(x=10;x<14;x++)digitalWrite(x,LOW);	
		_delay_ms(200);
	}	

	return 0;
}
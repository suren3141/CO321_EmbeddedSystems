#include<avr/io.h>
 
#define USART_BAUDRATE 9600
#define BAUD_PRESCALE (((F_CPU/(USART_BAUDRATE*16UL)))-1)

char recieved_byte;

void usart_init(){

	UCSR0B |= (1<<RXEN0)  | (1<<TXEN0);		// Turn on the transmission and reception circuitry
	//UCSR0C = 0							// USART = 00, NoParity = 00, StopBit = 0
	UCSR0C |= (1<<UCSZ00) | (1<<UCSZ01);	// Use 8-bit character sizes
	UBRR0H  = (BAUD_PRESCALE >> 8);			// Load upper 8-bits of the baud rate value into the high byte of the UBRR register
	UBRR0L  = BAUD_PRESCALE;				// Load lower 8-bits of the baud rate value into the low byte of the UBRR register	
}

void usart_send(char c){
	while( ( UCSR0A & ( 1 << UDRE0 ) ) == 0 ){}
	UDR0 = c;
}

void usart_receive(){
	while( ( UCSR0A & ( 1 << RXC0 ) ) == 0 ){}
	recieved_byte = UDR0;
}

void EEPROMwrite(unsigned int address, char data){

	// wait for completion of previous write
	while (EECR & (1<<EEPE));

	// set up address and data regs
	EEAR = address;
	EEDR = data;

	// write logical 1 to EEMPE
	EECR |= (1<<EEMPE);

	// start eeprom write
	EECR |= (1<<EEPE);
}

char EEPROMread(unsigned int address){

	// wait for completion of previous write
	while (EECR & (1<<EEPE));

	// set up address
	EEAR = address;

	// start eeprom read
	EECR |= (1<<EERE);

	return EEDR;
}

int main(void){
	char c;
	unsigned int i = 0;

	usart_init();  
	// DDRB = 0xFF;
	while(1){
		// PORTB = 0;
		// c = EEPROMread(i);
		// usart_send(c);


		usart_receive();
		// PORTB = (1<<5);
		c = recieved_byte;

		EEPROMwrite(i++, c);

		// if (c >= 'a' && c <= 'z'){
		// 	c = (((c % 'a') + 3) % 26) + 'a';
		// }else if (c >= 'A' && c <= 'Z'){
		// 	c = (((c % 'A') + 3) % 26) + 'A';
		// }
	}

    return 0;   /* never reached */
}

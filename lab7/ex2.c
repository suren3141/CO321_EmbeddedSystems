#include <avr/io.h>
#include <util/delay.h>

#define USART_BAUDRATE 9600
#define BAUD_PRESCALE (((F_CPU/(USART_BAUDRATE*16UL)))-1)

char recieved_byte;

void usart_init(){

  UCSR0B |= (1<<RXEN0)  | (1<<TXEN0);   // Turn on the transmission and reception circuitry
  //UCSR0C = 0              // USART = 00, NoParity = 00, StopBit = 0
  UCSR0C |= (1<<UCSZ00) | (1<<UCSZ01);  // Use 8-bit character sizes
  UBRR0H  = (BAUD_PRESCALE >> 8);     // Load upper 8-bits of the baud rate value into the high byte of the UBRR register
  UBRR0L  = BAUD_PRESCALE;        // Load lower 8-bits of the baud rate value into the low byte of the UBRR register  
}

void usart_send(char c){
  while( ( UCSR0A & ( 1 << UDRE0 ) ) == 0 ){}
  UDR0 = c;
}

void usart_receive(){
  while( ( UCSR0A & ( 1 << RXC0 ) ) == 0 ){}
  recieved_byte = UDR0;
}

void send_str(char *c){
  char x;
  while(*c){
    x = *c++;
    usart_send(x);
  }
}

int main(void){

  usart_init();  

  char m1[] = "E/14/339 Suren\n";
  char m2[] = "E/14/158 Gihan\n";
  char m3[] = "E/14/378 Harshana\n";
  char m4[] = "E/14/237 Punky\n";

  while(1){
    send_str(m1);
    send_str(m3);
    send_str(m2);
    send_str(m4);
    _delay_ms(1000);
  }

    return 0;   /* never reached */
}

# include <avr/io.h>
#define F_CPU (4000000UL) 
#define USART3_BAUD_RATE(BAUD_RATE)((float)(64*F_CPU/(16*(float)BAUD_RATE))+ 0.5) 

#include <util/delay.h>
#include <string.h>
#include <stdio.h>
#include <string.h>

void USART3_init(void);
void USART3_sendChar(char c);
void USART3_sendString(char *str);


static FILE USART_stream = FDEV_SETUP_STREAM(USART3_sendChar, NULL, _FDEV_SETUP_WRITE);

int main(void)
{
	USART3_init();
	stdout = &USART_stream;

    uint8_t count = 0;
	while (1)
	{
		printf("Counter value is: %d\r\n", count++);
		_delay_ms(500);
	}
}


void USART3_init(void)
{
	PORTB.DIR &= ~PIN1_bm;
	PORTB.DIR |= PIN0_bm;
	USART3.BAUD = (uint16_t)USART3_BAUD_RATE(9600);
	USART3.CTRLB |= USART_TXEN_bm;
}

void USART3_sendChar(char c)
{
	while (!(USART3.STATUS & USART_DREIF_bm))
	{
		;
	}
	USART3.TXDATAL = c;
}

void USART3_sendString(char *str)
{
	for(size_t i = 0; i < strlen(str); i++)
	{
		USART3_sendChar(str[i]);
	}
}



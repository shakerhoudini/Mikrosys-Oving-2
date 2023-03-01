# include <avr/io.h>
#define F_CPU (4000000UL) 
#define USART3_BAUD_RATE(BAUD_RATE)((float)(64*F_CPU/(16*(float)BAUD_RATE))+ 0.5) 

int main (void)
{
	USART3.BAUD = USART3_BAUD_RATE(9600); 
	
	PORTB.DIRSET = PIN0_bm; 
	PORTB.DIRCLR = PIN1_bm; 
	
	USART3.CTRLB |= USART_RXEN_bm | USART_TXEN_bm; 
	
	while (1)
	{
		do
		{
			;
			} while (!( USART3.STATUS & USART_RXCIF_bm)); 
			USART3.TXDATAL = USART3.RXDATAL; 
		}
			
		}

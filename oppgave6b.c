#define F_CPU 4000000UL
#define USART3_BAUD_RATE(BAUD_RATE)	((float)(64*F_CPU/(16*(float)BAUD_RATE))+0.5)

#include <avr/io.h>
#include <util/delay.h>
#include <string.h>
#include <stdio.h>

//Initialsiering 
void USART3_init(void);
static void USART3_sendChar(char c);
//static int USART3_printChar(char c, FILE *stream);
void USART3_sendString(char *str);
void executeCommand(char *command);
uint8_t USART_read();

static FILE Usart_stream = FDEV_SETUP_STREAM(USART3_sendChar,NULL,_FDEV_SETUP_WRITE);


int main(void)
{
	PORTE.DIRSET = 0b00000011; //Output for LEDLYS
	const int maxVal = 10; //Maksimal lengde for input 
	
	//char sTarg = "";
	char command[maxVal];
	uint8_t index = 0;
	
	char out;
	
	USART3_init();
	_delay_ms(10);
	while (1)
	{
		out =(char) USART_read();
		if(out != 0x0D && out != 0x0A){
			command[index++] = out;
			if(index>maxVal){
				printf("Input Too Long\r\n");
				index = 0;
			}
		}

		if(out == 0x0D){
			command[index] = '\0';
			index = 0;
			printf("%s",command);
			executeCommand(command);
		}
	}
}

void USART3_init(void)
{
	PORTB.DIRCLR = PIN1_bm; //= PORTB.DIRSET = PIN1_bm;
	PORTB.DIRSET = PIN0_bm;  //= PORTB.DIRCLR = PIN0_bm;
	USART3.BAUD = (uint16_t)USART3_BAUD_RATE(9600);
	USART3.CTRLB |= USART_TXEN_bm | USART_RXEN_bm;
	
	stdout = &Usart_stream;
}

static void USART3_sendChar(char c) //Sender karakter
{
	while (!(USART3.STATUS & USART_DREIF_bm))
	{
		;
	}
	USART3.TXDATAL = c;
}

void USART3_sendString(char *str) /Sender string 
{
	for(size_t i = 0; i < strlen(str); i++)
	{
		USART3_sendChar(str[i]);
	}
}

uint8_t USART_read(){ //Leser av inputs 
	while(!(USART3.STATUS & USART_RXCIF_bm)){
		;
	}
	uint8_t srt = USART3.RXDATAL;
	//USART3_sendChar(srt);
	return srt;
}

//static int USART3_printChar(char c, FILE *stream){
	//USART3_sendChar(c);
	//return 0;
//}

void executeCommand(char *command) //Switch case som leser av input og hendholvis setter lys på
{
	if(strcmp(command, "ledon 1") == 0)
	{
		PORTE.OUTSET = PIN0_bm;
		USART3_sendString("LED1 ON.\r\n");
	}
	else if(strcmp(command, "ledoff 1") == 0)
	{
		PORTE.OUTCLR = PIN0_bm;
		USART3_sendString("LED1 OFF.\r\n");
	}
	else if(strcmp(command, "ledon 2") == 0)
	{
		PORTE.OUTSET = PIN1_bm;
		USART3_sendString("LED2 ON.\r\n");
	}
	else if(strcmp(command, "ledoff 2") == 0)
	{
		PORTE.OUTCLR = PIN1_bm;
		USART3_sendString("LED2 OFF.\r\n");
	}
	else
	{
		USART3_sendString("Incorrect prompt");
	}
}

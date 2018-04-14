/*
Mora Scorpions
*/

#define F_CPU 8000000UL

/* file inclusions */
#include <avr/io.h>
#include <stdio.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdlib.h>


//SD card headers
#include "SPI_routines.h"
#include "SD_routines.h"
#include "i2c_routines.h"
#include "FAT32.h"
#include "RTC_routines.h"


void writeToSDCard(char * station, char * date , char * time);
void uart_init (void);
unsigned char uart_recieve (void);
void uart_transmit (unsigned char data);


uint8_t bodima = 0;
uint8_t moratuwa = 0;
uint8_t rathmalana = 0;
uint8_t galkissa = 0;
uint8_t wellawatta = 0;
uint8_t bambalapitiya = 0;
uint8_t slaveIsland = 0;
uint8_t colomboFort = 0;
void port_init(void)
{
	 
	
	DDRA = 0xFF;		//for audio module 
	DDRB  = 0b10110011; // miso input other output PB7,PB6,PB5,PB4 for SD card , PB0 and PB1 for SD card error LEDs
	DDRD = 0xff;		//output for other micro controllers
	
}

void device_init(void)
{
	
	spi_init();
	twi_init();
	uart_init ();
	
}

void main(void)
{
	_delay_ms(100); //delay for power stabilization
	port_init();
	device_init();
	
	_delay_ms(2000);
	int error1 = 0;
	error1 = SD_init();
	if(error1)
	{
		PORTB |= 0b00000001;
	}

	getBootSectorData(); 
	_delay_ms(1000);

	cardType = 0;

	unsigned char Temp;
	char lat[15] = "LAT: ";
	char lon[16] = "LON: ";
	char la[10];
	char lo[11];
	char time[9];
	double latitude,longitude;
	while(1)
	{
		int index = 0;
		Temp=uart_recieve();
		if(Temp=='G')					//get into $GPGGA 
		{
			Temp=uart_recieve();
			if(Temp=='G')
			{
				Temp=uart_recieve();
				if(Temp=='A')
				{
					Temp = uart_recieve();         // skipping ','
					Temp = uart_recieve();
					
					
					while (Temp != ',')
					{
						
						Temp = uart_recieve();	  //Skipping time
					}
					
					Temp = uart_recieve();
					
					while (Temp != ',')				//getting latitude
					{
						
						la[index] = Temp;
						Temp = uart_recieve();
						
						index++;
					}
					
					
					//index = 5;
					index = 0;
					
					Temp = uart_recieve();
					Temp = uart_recieve();
					Temp = uart_recieve();
					
					while (Temp != ',')			//getting longitude
					{
						
						
						lo[index] = Temp;
						Temp = uart_recieve();
						
						index++;
					}
					
					latitude = atof(la);
					longitude = atof(lo);
					compare(latitude,longitude);
				}
				
			}
		}
	}
}


void writeToSDCard(char * station, char * date , char * time)
{    
	//write to SD card
	sprintf(dataString, "%s,%s,%s\r\n\0", station, date ,time);
	char fileName[] = "READ.CSV";
	int error = writeFile(fileName);
	if(error)
	{ 
		PORTB |= 0b00000010;
	}

}


//UART functions
void uart_init (void)
{
	UBRRH |= (103>>8);							//set UBBR value for 9600 baud rate
	UBRRL |= 103;
	UCSRB |= (1<<TXEN)|(1<<RXEN);                // enable receiver and transmitter
	UCSRC &= ~(1<<UMSEL);
	UCSRC|= (1<<URSEL)|(1<<UCSZ0)|(1<<UCSZ1);   // 8bit data format
	UCSRA = (1<<U2X);
}

unsigned char uart_recieve (void)
{
	while(!((UCSRA) & (1<<RXC)));                   // wait while data is being received
	return UDR;                                   // return 8-bit data
}

void uart_transmit (unsigned char data)
{
	while (!( UCSRA & (1<<UDRE)));                // wait while register is free
	UDR = data;                                   // load data in the register
}

void compare(double latitude, double longitude)
{	
	if(latitude>646.31268 && latitude<646.56456 && longitude>7952.75848 && longitude<7953.03142 && moratuwa == 0)
	{
			char station[] = "Moratuwa";
			char date[] = "2017/06/3";
			char time[] = "14:00";
			
			
			//Write info to SD card
			writeToSDCard(station, date , time);
			_delay_ms(1000);
			
			
			//audio
			PORTA |= (1<<PINA0);
			_delay_ms(500);
			PORTA &= ~(1<<PINA0);
			_delay_ms(25000);
			
			
			
			//led + gsm signal out
			PORTD &= ~(1<<PIND4);
			PORTD &= ~(1<<PIND5);
			PORTD |= (1<<PIND6);
			
			moratuwa = 1;
			
	}
		
		
	if(latitude>648.71538 && latitude<649.02858 && longitude>7951.91626 && longitude<7952.11258 && rathmalana == 0)
	{
		char station[] = "Rathmalana";
		char date[] = "2017/03/4";
		char time[] = "9.02";
					
					
		//Write info to SD card
		writeToSDCard(station, date , time);
		_delay_ms(1000);
					
					
		//audio
		PORTA |= (1<<PINA1);
		_delay_ms(500);
		PORTA &= ~(1<<PINA1);
		_delay_ms(25000);
					
					
		//led + gsm signal out
		PORTD &= ~(1<<PIND4);
		PORTD |= (1<<PIND5);
		PORTD &= ~(1<<PIND6);
					
		rathmalana++;
					
		}
				
				
		if(latitude>648.71538 && latitude<649.02858 && longitude>7951.91626 && longitude<7952.11258 && galkissa == 0)
		{
			char station[] = "Mount Lavinia";
			char date[] = "2017/03/4";
			char time[] = "9.02";
					
					
			//Write info to SD card
			writeToSDCard(station, date , time);
			_delay_ms(1000);
					
					
			//audio
			PORTA |= (1<<PINA2);
			_delay_ms(500);
			PORTA &= ~(1<<PINA2);
			_delay_ms(25000);
							
			//led + gsm signal out
			PORTD &= ~(1<<PIND4);
			PORTD |= (1<<PIND5);
			PORTD |= (1<<PIND6);
					
			galkissa++;
					
			}
							
				
			if(latitude>652.40262 && latitude<652.62186 && longitude>7951.3756 && longitude<7951.5217 && wellawatta == 0)
			{
				char station[] = "Wellawatta";
				char date[] = "2017/03/4";
				char time[] = "9.02";
							
				//Write info to SD card
				writeToSDCard(station, date , time);
				_delay_ms(1000);
						
				//audio
				PORTA |= (1<<PINA3);
				_delay_ms(500);
				PORTA &= ~(1<<PINA3);
				_delay_ms(25000);
						
				//led + gsm signal out
				PORTD |= (1<<PIND4);
				PORTD &= ~(1<<PIND5);
				PORTD |= (1<<PIND6);
					
				wellawatta++;	
				}			
		
		
				if(latitude>653.5014 && latitude<653.72124 && longitude>7951.11508 && longitude<7951.25992 && bambalapitiya == 2)
				{
					char station[] = "Bambalapiyita";
					char date[] = "2017/03/4";
					char time[] = "9.02";
					
					
					//Write info to SD card
					writeToSDCard(station, date , time);
					_delay_ms(1000);
					
					
					//audio
					PORTA |= (1<<PINA4);
					_delay_ms(500);
					PORTA &= ~(1<<PINA4);
					_delay_ms(5000);
					
					//led + gsm signal out
					PORTD |= (1<<PIND4);
					PORTD &= ~(1<<PIND5);
					PORTD |= (1<<PIND6);
					_delay_ms(10000);
					
					bambalapitiya++;
					
				}
				
				if(latitude>655.25412 && latitude<655.51932 && longitude>7950.90376 && longitude<7951.03252 &&  slaveIsland== 0)
				{
					char station[] = "Slave Island";
					char date[] = "2017/03/4";
					char time[] = "9.02";
					
					
					//Write info to SD card
					writeToSDCard(station, date , time);
					_delay_ms(1000);
					
					
					//audio
					PORTA |= (1<<PINA5);
					_delay_ms(500);
					PORTA &= ~(1<<PINA5);
					_delay_ms(25000);
						
					//led + gsm signal out
					PORTD |= (1<<PIND4);
					PORTD |= (1<<PIND5);
					PORTD |= (1<<PIND6);
					
					slaveIsland++;
				}
				
				
				if(latitude>655.95186 && latitude<656.04834 && longitude>7950.8761 && longitude<7951.23388 &&  colomboFort== 0)
				{
					char station[] = "Colombo Fort";
					char date[] = "2017/03/4";
					char time[] = "9.02";
					
					
					//Write info to SD card
					writeToSDCard(station, date , time);
					_delay_ms(1000);
					
					
					//audio
					PORTA |= (1<<PINA6);
					_delay_ms(500);
					PORTA &= ~(1<<PINA6);
					_delay_ms(25000);
					
					//led + gsm signal out
					PORTD &= ~(1<<PIND4);
					PORTD |= (1<<PIND5);
					PORTD |= (1<<PIND6);
					
					colomboFort++;
					
				}
}
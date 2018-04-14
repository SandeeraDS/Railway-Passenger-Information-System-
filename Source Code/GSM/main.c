
/*
Mora Scorpions
*/
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#include "lib/sim300/sim300.h"	//library for GSM module
#include "string.h"


char colomboFort[5][12];
char slaveIsland[5][12];
char bambalapitiya[5][12];
char wellawaththa[5][12];
char mountLavinia[5][12];
char rathmalana[5][12];
char moratuwa[5][12];

int one = 0;
int two = 0;
int three = 0;
int four = 0;
int five = 0;
int six = 0;
int seven = 0;


char phoneNo[11];


//interrupt occurs when train reaches a certain coordinate range
ISR(INT1_vect)
{
	_delay_ms(1000);
	
	uint8_t ref;
	//check input from micro controller with GPS
	if((bit_is_set(PINA,1)) &&(bit_is_set(PINA,2)) &&(bit_is_set(PINA,3)))
	{
		for(int i = 0; i < one; i++)
		{
			int8_t r=SIM300SendMsg(moratuwa[i],"Dear passenger you have reached your destination!",&ref);//Send message to relevant passenger
		}
	}
	if (bit_is_set(PINB,0) && bit_is_clear(PINB,1) && bit_is_clear(PINB,2)){
		for(int i = 0; i < two; i++)
		{
			int8_t r=SIM300SendMsg(rathmalana[i],"Dear passenger you have reached your destination!",&ref);//Send message to relevant passenger
		}
	}
	else if (bit_is_clear(PINB,0) && bit_is_set(PINB,1) && bit_is_clear(PINB,2)){
		for(int i = 0; i < three; i++)
		{
			int8_t r=SIM300SendMsg(mountLavinia[i],"Dear passenger you have reached your destination!",&ref);//Send message to relevant passenger
		}
	}
	else if (bit_is_set(PINB,0) && bit_is_set(PINB,1) && bit_is_clear(PINB,2)){
		for(int i = 0; i < four; i++)
		{
			int8_t r=SIM300SendMsg(wellawaththa[i],"Dear passenger you have reached your destination!",&ref);//Send message to relevant passenger
		}
	}
	else if (bit_is_clear(PINB,0) && bit_is_clear(PINB,1) && bit_is_set(PINB,2)){
		for(int i = 0; i < five; i++)
		{
			int8_t r=SIM300SendMsg(bambalapitiya[i],"Dear passenger you have reached your destination!",&ref);//Send message to relevant passenger
		}
	}
	else if (bit_is_set(PINB,0) && bit_is_clear(PINB,1) && bit_is_set(PINB,2)){
		for(int i = 0; i < six; i++)
		{
			int8_t r=SIM300SendMsg(slaveIsland[i],"Dear passenger you have reached your destination!",&ref);//Send message to relevant passenger
		}
	}
	else if (bit_is_clear(PINB,0) && bit_is_set(PINB,1) && bit_is_set(PINB,2)){
		for(int i = 0; i < seven; i++)
		{
			int8_t r=SIM300SendMsg(colomboFort[i],"Dear passenger you have reached your destination!",&ref);//Send message to relevant passenger
		}
	}

}



int main(void)
{
	DDRD = 0b00000000;	//Set PORTD as input
	DDRD |= 1<<PIND3;		// Set PD3 as input (Using for interupt INT1)
	GICR = 1<<INT1;					// Enable INT1
	MCUCR = 1<<ISC11 | 1<<ISC10 ;	// Trigger INT1 on rising edge
	sei();
	
	
	
	_delay_ms(3000);
	int8_t r= SIM300Init();
	_delay_ms(3000);
	

	
	//Waiting for MSG
	uint8_t id;
	uint8_t x=0;

	UFlushBuffer();
	
	while(1)
	{

		x=0;
		int8_t vx=1;

		while(SIM300WaitForMsg(&id)!=SIM300_OK)
		{

			x+=vx;

			if(x==15 || x==0) vx=vx*-1;
		}


		_delay_ms(1000);

		char msg[14];

		r=SIM300ReadMsg(id,msg);//read message

		if(r==SIM300_OK)
		{
			
			_delay_ms(3000);
		
			char station=msg[0];
			
			for(int i =2;i<14;i++)
			{
				phoneNo[i-2] = msg[i];	
			}
			switch(station)
			{
				case '7' :
					strcpy(colomboFort[seven],phoneNo);
					one++;
				case '6' :
					strcpy(slaveIsland[six],phoneNo);
					one++;
				case '5' :
					strcpy(bambalapitiya[five],phoneNo);
					one++;
				case '4' :
					strcpy(wellawaththa[four],phoneNo);
					one++;
				case '3' :
					strcpy(mountLavinia[three],phoneNo);
					one++;
				case '2' :
					strcpy(rathmalana[two],phoneNo);
					one++;
				case '1' :
					strcpy(moratuwa[one],phoneNo);
					one++;
			}
			
		}
		
		_delay_ms(5000);
		
		//Finally delete the msg
		if (SIM300DeleteMsg(id)!=SIM300_OK)
		{
			_delay_ms(3000);
		}

	}

}


/*
 * 32x16display.c
 *
 * Created: 1/1/2017 7:17:48 PM
 * Author : Achintha
 */ 

#include <avr/io.h>
#define F_CPU 1000000UL
#include <util/delay.h>

#include <string.h>

#define SH_CP 2
#define ST_CP 1
#define DS 3

#define SIZE 10
#define WIDTH 11

#include "characters3.c"
#include "charcheck.c"

void move(char name[]);

int main(void)
{
	DDRA = 0xFF;
	DDRD = 0xFF;
	PORTD = 0x01;
	DDRB = 0x00;
    while (1) 
    {
		move("  NOW  ");
		if (bit_is_set(PINB,0) && bit_is_clear(PINB,1) && bit_is_clear(PINB,2)){
			move("MORATUWA");
			move("  NEXT  ");
			move("RATHMALANA");
			move("EST 10MIN");
		}
		else if (bit_is_clear(PINB,0) && bit_is_set(PINB,1) && bit_is_clear(PINB,2)){
			move("RATHMALANA");
			move("  NEXT  ");
			move("MT LAVINIA");
			move("EST 10MIN");
		}
		else if (bit_is_set(PINB,0) && bit_is_set(PINB,1) && bit_is_clear(PINB,2)){
			move("MT LAVINIA");
			move("  NEXT  ");
			move("WELLAWATTA");
			move("EST 10MIN");
		}
		else if (bit_is_clear(PINB,0) && bit_is_clear(PINB,1) && bit_is_set(PINB,2)){
			move("WELLAWATTA");
			move("  NEXT  ");
			move("BAMBALAPITIYA");
			move("EST 10MIN");
		}
		else if (bit_is_set(PINB,0) && bit_is_clear(PINB,1) && bit_is_set(PINB,2)){
			move("BAMBALAPITIYA");
			move("  NEXT  ");
			move("SLV ISLAND");
			move("EST 10MIN");
		}
		else if (bit_is_clear(PINB,0) && bit_is_set(PINB,1) && bit_is_set(PINB,2)){
			move("SLV ISLAND");
			move("  NEXT  ");
			move("COLOMBO");
			move("EST 10MIN");
		}
		else if (bit_is_set(PINB,0) && bit_is_set(PINB,1) && bit_is_set(PINB,2)){
			move("COLOMBO");
		}
		else {
			move("NO SMOKING");
		}
		move("NO SMOKING");
    }
	return 0;
}

void move(char name[]){
	uint16_t k;
	uint8_t c,i,j;
	uint8_t display[SIZE][strlen(name)*WIDTH];
	for (i=0;i<strlen(name);i++){
		c = check_char(name[i]);
		for (j=0;j<SIZE;j++){
			for (k=0;k<WIDTH;k++){
				if ((1024>>k) & characters[c][j])
				display[j][i*WIDTH+k] = 1;
				else
				display[j][i*WIDTH+k] = 0;
			}
		}
	}
	for (i=0;i<(strlen(name)*WIDTH-63);i++){
		for (uint8_t count=0;count<20;count++){
			for (j=0;j<SIZE;j++){
				for (k=0;k<64;k++){
					if (display[j][i+k])
					PORTD &= ~(1<<DS);
					else
					PORTD |= (1<<DS);
					
					PORTD |= (1<<SH_CP);
					PORTD &= ~(1<<SH_CP);
				}
				PORTA = j+3;
				PORTD |= (1<<ST_CP);
				PORTD &= ~(1<<ST_CP);
			}
		}
	}
}

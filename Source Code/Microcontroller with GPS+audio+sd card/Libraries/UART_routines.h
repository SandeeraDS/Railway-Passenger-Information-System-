

#ifndef _UART_ROUTINES_H_
#define _UART_ROUTINES_H_

#define CHAR 0
#define INT  1
#define LONG 2

#define TX_NEWLINE {transmitByte(0x0d); transmitByte(0x0a);}

void uart0_init(void);
unsigned char receiveByte(void);
void transmitByte(unsigned char);
void transmitString_F(char*);
void transmitString(unsigned char*);
void transmitHex( unsigned char dataType, unsigned long data );


#endif

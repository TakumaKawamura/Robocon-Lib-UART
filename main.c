/*
 * main.c
 *
 * Created: 2016/12/05 17:24:44
 * Author : takuma
 */ 

//#define F_CPU 20000000UL
#define BAUD 9600

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "UART_ATMega328p.h"

void strWrite(char *str);

int main(void){
  uint8_t data = sizeof(int);

  UART_Begin();
  sei();
  while(1){
    
    UART_Write('0' + data);
    _delay_ms(500);
  }
}

void strWrite(char *str){
  while( *str != 0 ){
    UART_Write(*str);
    str++;
  }
}

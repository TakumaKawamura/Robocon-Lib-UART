
/*!
 @file		UART_ATMega328p.h
 @brief	 	ATMega328p用UART通信ライブラリ

 UART(USART)通信で1byteのデータをやりとりするライブラリです。

 @author	T.Kawamura
 @version	1.0
 @date		2016-11-21	T.Kawamura	新規作成
 @date		2016-12-27	T.Kawamura	正式版完成
 
 @copyright	(C) 2016 T.Kawamura All rights reserved.
*/

#ifndef UART_ATMEGA328P_H
#define UART_ATMEGA328P_H


/*!
  @name   nop()関数（何もしない）
*/
#define nop() __asm__ __volatile__ ("nop")

/*!
  @name   UART送信可能割り込み禁止／許可
*/
#define E_TX_INT {UCSR0B |= _BV(UDRIE0);} 
#define D_TX_INT {UCSR0B &= ~_BV(UDRIE0);}
#define IS_E_TX_INT {((UCSR0B & _BV(TXCIE0) != 0)}

/*!
  @name		バッファサイズ
*/
#define TX_BUFFER_SIZE 64
#define RX_BUFFER_SIZE 64

void UART_Begin(void);
void UART_End(void);
int16_t UART_Read(void);
int16_t UART_Peek(void);
uint16_t UART_Available(void);
void UART_Flush(void);
uint8_t UART_Write(uint8_t data);


#endif

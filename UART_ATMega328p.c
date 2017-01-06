
/*!
 @file		UART_ATMega328p.c
 @brief	 	ATMega328p用UART通信ライブラリ

 UART(USART)通信でデータをやりとりするライブラリです。

 @author	T.Kawamura
 @version	1.0
 @date		2016-11-21	T.Kawamura	新規作成
 @date		2016-12-27	T.Kawamura	正式版完成
 @copyright	(C) 2016 T.Kawamura All rights reserved.
*/

//#define F_CPU 20000000UL

/*!
  util/setbaud.h関連
*/
#ifndef BAUD
#define BAUD 9600
#endif

#include <avr/io.h>
#include <avr/common.h>
#include <avr/sfr_defs.h>
#include <avr/interrupt.h>
#include <string.h>
#include "FIFO.h"
#include "UART_ATMega328p.h"
#include <util/setbaud.h>

/*!
  新規FIFOバッファ作成
*/
FIFO_New(TX_BUFFER, TX_BUFFER_SIZE);
FIFO_New(RX_BUFFER, RX_BUFFER_SIZE);

/*!
  @brief	UART通信の初期化

  FIFOバッファを初期化し、UART通信の初期設定を行います。
  
  @param	なし
  @return	なし
*/
void UART_Begin(void){
  FIFO_Clear(&TX_BUFFER);
  FIFO_Clear(&RX_BUFFER);

  UBRR0L = UBRRH_VALUE;
  UBRR0L = UBRRL_VALUE;
  UCSR0A &= ~_BV(U2X0);					//倍速非許可
  UCSR0B |= _BV(RXCIE0) | _BV(RXEN0) | _BV(TXEN0);	//受信データレジスタ空き割り込み許可、受信／送信許可
  UCSR0C |= _BV(UCSZ01) | _BV(UCSZ00);			//8N1
}

/*!
  @brief	UART通信の停止

  TXD,RXDピンの標準ポート動作を有効にします。
  
  @param  なし
  @return	なし
*/
void UART_End(void){
    UCSR0B &= ~(_BV(RXEN0) | _BV(TXEN0) | _BV(RXCIE0) | _BV(UDRIE0));	//受信／送信禁止
}

/*!
  @brief	1Byte読み込む

  受信バッファから1Byte読み込みます。
  
  @param  なし
  @retval -1  エラー（バッファにデータなし）
  @retval それ以外  読み出したデータ
*/
int16_t UART_Read(void){
  return FIFO_Get(&RX_BUFFER);
}

/*!
  @brief	1Byte読み込む(読み取り位置変更なし)

  受信バッファから1Byte読み込みます。読み取り位置は変更しません。
  すなわち、そのつぎのデータ読み取りでも同じデータを読み取ることになります。

  @param  なし
  @retval -1  エラー（バッファにデータなし）
  @retval それ以外  読み出したデータ
*/
int16_t UART_Peek(void){
  return FIFO_Peek(&RX_BUFFER);
}

/*!
  @brief	受信バッファに到着しているデータサイズを取得する

  受信バッファに何Byteのデータが到着しているかを返します。

  @param  なし
  @return 到着しているデータサイズ
*/
uint16_t UART_Available(void){
  return FIFO_Count(&RX_BUFFER);
}

/*!
  @brief	受信バッファを強制的に初期化する

  受信バッファを強制的に初期化します。

  @param  なし
  @return なし
*/
void UART_Flush(void){
  FIFO_Clear(&RX_BUFFER);
}

/*!
  @brief	送信バッファが空になるのを待つ

  送信バッファが空になるのを待ちます。
  能動的にバッファを送信するのではなく、バッファが送信されるのを待つ関数です。

  @param  なし
  @return なし
*/
void UART_Wait(void){
  while( FIFO_Count(&TX_BUFFER) > 0 ){
    nop(); 
  }
}

/*!
  @brief	データを1Byte送信する

  送信割り込みを使いデータを1Byte送信します。

  @param  data  送信するデータ
  @retval 0 エラー
  @retval それ以外  成功
*/
uint8_t UART_Write(uint8_t data){
  uint8_t status;
  
  status = FIFO_Put(&TX_BUFFER, data);
  if( status != 0 ){
    E_TX_INT;
    return 1;
  }else{
    return 0;
  }
}

/*!
  UART送信割り込みハンドラ
  
  送信できる状態になったら、送信バッファからデータを取り出して送信します。
*/
ISR(USART_UDRE_vect){
  uint8_t data;
  
  if( FIFO_Count(&TX_BUFFER) > 0 ){   //送信バッファにデータがある
    data = (uint8_t)FIFO_Get(&TX_BUFFER);
    UDR0 = data;
  }else{                              //送信バッファにデータがもうなかった
    D_TX_INT;                         //送信割り込み禁止
  }
}

/*!
  UART受信割り込みハンドラ
  
  受信バッファにデータを入れます。
*/
ISR(USART_RX_vect){
  uint8_t data;

  PORTB ^= 0x04;

  data = UDR0;
  FIFO_Put(&RX_BUFFER, data);
}

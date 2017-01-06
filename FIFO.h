
/*!
 @file		FIFO.h
 @brief	 	1Byte ソフトウェアFIFOバッファのライブラリ

 USART, I2C(TWI), SPIなどのシリアル通信において必要となる、ソフトウェアバッファのライブラリです。
 AVR以外でもつかえます。日本語で言うところのところてん式バッファ。先に放り込んだデータから順に読み出せます。
 なお、このFIFOバッファは非上書きモードで動作します。
 すなわち，フルの状態でさらにデータを追加しようとすると，エラーを吐き、最も古いデータを残します。

 @author	T.Kawamura
 @version	1.0
 @date		2016-11-20	T.Kawamura	新規作成
 @date		2016-11-20	T.Kawamura	FIFO_Peek関数を追加、その他軽微な修正
 @date   2016-11-22  T.Kawamura  軽微な修正
 
 @copyright	(C) 2016 T.Kawamura All rights reserved.
*/

#ifndef FIFO_H
#define FIFO_H

typedef struct {
  uint8_t *buffer;	//! バッファ
  int16_t size;	//!バッファサイズ
  volatile uint8_t getindex;	//! 読み込むデータの位置
  volatile uint8_t putindex;	//! 書き込むデータの位置
  volatile int16_t count;	//! 入っているデータのサイズ
}fifo_type_s;

#define FIFO_New(name, size)\
  static uint8_t fifobuff##name[size]; \
  fifo_type_s name = {fifobuff##name, (size), 0, 0, 0 };

extern void FIFO_Clear(fifo_type_s *fifo);			//初期化
extern int16_t FIFO_Size(fifo_type_s *fifo);			//FIFOのサイズ
extern int16_t FIFO_Get(fifo_type_s *fifo);		       	//１バイト読み込み
extern int16_t FIFO_Peek(fifo_type_s *fifo);			//1バイト読み込み（読み取り位置変更なし）
extern uint8_t FIFO_Put(fifo_type_s *fifo, uint8_t putdata);	//１バイト書き込み
extern uint16_t FIFO_Count(fifo_type_s *fifo);			//入っているデータのサイズ

#endif

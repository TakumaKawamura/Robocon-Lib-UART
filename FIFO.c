
/*!
  @file		FIFO.c
  @brief	1Byte ソフトウェアFIFOバッファのライブラリ

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

#include <avr/io.h>
#include <avr/common.h>
#include <avr/interrupt.h>
#include "FIFO.h"

/*!
  @brief	初期状態にする

  インデックスの位置、バッファ内のデータの数を０にします。
  
  @param	fifo	初期状態にするバッファ
*/
void FIFO_Clear(fifo_type_s *fifo){
  uint8_t sreg_save = SREG;
  cli();
  
  fifo->count = 0;
  fifo->getindex = 0;
  fifo->putindex = 0;

  SREG = sreg_save;
}

/*!
  @brief	FIFOのサイズを返す

  FIFOバッファのデータサイズ(Byte)を返します。

  @param	fifo	サイズ取得先のバッファ
  @return	FIFOのサイズ(Byte)
*/
int16_t FIFO_Size(fifo_type_s *fifo){
  return fifo->size;
}

/*!
  @brief	データを取り出す

  バッファ内のデータを1Byte取り出します。

  @param	fifo	データを取り出すバッファ(参照渡し)
  @retval	-1	エラー
  @retval	0~255	取り出したデータ（1Byte）
*/
int16_t FIFO_Get(fifo_type_s *fifo){
  uint8_t sreg_save = SREG;
  
  uint8_t getdata;	//読み込んだデータの格納先
  
  cli();

  if ( fifo->count <= 0 ){	//もし何もデータが書き込みされていない状態で読み込みが実行されたら
    SREG = sreg_save;
    return -1;
  }
  
  getdata = fifo->buffer[fifo->getindex];	//データを読み込む
  
  fifo->getindex++;	//次に一つ先のデータを読み込むように設定する
  if ( fifo->getindex >= fifo->size ){	//もし先ほど読み込んだデータがバッファの終端なら次に書き込む場所を最初に戻す
    fifo->getindex = 0;
  }
  fifo->count--;	//データを1バイト読み込んだので入っているデータのサイズを減算

  SREG = sreg_save;

  return getdata;
}

/*!
  @brief	データを取り出す (読み取り位置変更なし) 

  バッファ内のデータを1Byte取り出します。(読み取り位置変更なし) 

  @param	fifo	データを取り出すバッファ(参照渡し)
  @retval	-1	エラー
  @retval	0~255	取り出したデータ（1Byte）
*/
int16_t FIFO_Peek(fifo_type_s *fifo){
  uint8_t sreg_save = SREG;
  
  uint8_t getdata;	//読み込んだデータの格納先
  
  cli();

  if ( fifo->count <= 0 ){	//もし何もデータが書き込みされていない状態で読み込みが実行されたら
    SREG = sreg_save;
    return -1;
  }
  
  getdata = fifo->buffer[fifo->getindex];	//データを読み込む

  SREG = sreg_save;

  return getdata;
}

/*!
  @brief	データを書き込む

  バッファにデータを1Byte書き込みます。

  @param	fifo	データを書き込むバッファ(参照渡し)
  @param	putdata		書き込むデータ(1Byte)
  @retval	0	エラー
  @retval	0以外	成功
*/
uint8_t FIFO_Put(fifo_type_s *fifo, uint8_t putdata){
  uint8_t sreg_save = SREG;
  cli();
  
  if ( fifo->count >= fifo->size ){				//バッファが未読み込みデータでいっぱいになったら
    SREG = sreg_save;
    return 0;
  }
  
  fifo->buffer[fifo->putindex] = putdata;		//データを書き込み
  
  fifo->putindex++;						//次に一つ先のデータを書き込むように設定する
  if ( fifo->putindex >= fifo->size ){	//もし先ほど書き込んだデータがバッファの終端なら次に書き込む場所を最初に戻す
    fifo->putindex = 0;
  }
  
  fifo->count++;						//データを1バイト書き込んだので入っているデータのサイズを加算

  SREG = sreg_save;

  return 1;
}

/*!
  @brief	データのサイズを返す

  バッファ内のデータのサイズを返します。(1Byte)

  @param	fifo	バッファ(参照渡し)
  @return	バッファに入っているデータのサイズ
*/
uint16_t FIFO_Count(fifo_type_s *fifo){
  uint8_t sreg_save = SREG;

  int16_t c = 0;
  
  cli();

  c = fifo->count;

  SREG = sreg_save;
  
  return c;
}

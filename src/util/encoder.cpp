/**
 * @file encoder.cpp
 * @brief
 *  モータ軸に取り付けられたエンコーダにより，
 * モータ軸の累計回転数を取得するライブラリ．
 *
 * エンコーダはモータ制御基板に設置されている．
 * メイン基板とモータ制御基板は分離しており，シリアル通信（Ｉ２Ｃ）によりコマンドの送受信を行う．
 * モータ制御基板のＩ２Ｃアドレスは0x0fに設定されている．
 */

#include <Arduino.h>
#include <Wire.h>

short int right_encoder = 0;
short int left_encoder = 0;

void getResetEncoder() {
  Wire.beginTransmission(0x39);
  Wire.write(0x12);
  Wire.endTransmission();

  Wire.requestFrom(0x39, 4);
  delayMicroseconds(25);

  right_encoder = Wire.read() << 8;
  right_encoder |= Wire.read();

  left_encoder = Wire.read() << 8;
  left_encoder |= Wire.read();
}

void getEncoder() {
  Wire.beginTransmission(0x39);
  Wire.write(0x11);
  Wire.endTransmission();

  Wire.requestFrom(0x39, 4);
  delayMicroseconds(25);

  right_encoder = Wire.read() << 8;
  right_encoder |= Wire.read();

  left_encoder = Wire.read() << 8;
  left_encoder |= Wire.read();
  // Serial.println(left_encoder);
}

void resetEncoder() {
  Wire.beginTransmission(0x39);
  Wire.write(0x10);
  Wire.endTransmission();
}

/**
 * @file attitude_sensor.cpp
 * @brief
 * 9軸姿勢センサ（MPU-9250）により姿勢情報を得る
 *
 * センサＩＣとマイコンはシリアル通信（I2C）によりコマンドの送受信をする．
 */
#include <Arduino.h>
#include <Wire.h>
#include "attitude_sensor.h"

int attitude_data[10];

void initAttitudeSensor() {
  Wire.beginTransmission(0x68);
  Wire.write(0x75);
  Wire.endTransmission();
  Wire.requestFrom(0x68, 1);

  delay(10);

  if (Wire.read() == 0x71) {
    Serial.println("Success");
  } else {
    Serial.println("Failed");
    while (1) continue;
  }

  Wire.beginTransmission(0x68);
  Wire.write(0x6B);
  Wire.write(0x00);
  Wire.endTransmission();

  Wire.beginTransmission(0x68);
  Wire.write(0x37);
  Wire.write(0x02);
  Wire.endTransmission();

  Wire.beginTransmission(0x0C);
  Wire.write(0x0A);
  Wire.write(0x12);
  Wire.endTransmission();

  for (int i = 0; i < 10; i++) {
    attitude_data[i] = 0;
  }
}

void getAttitude() {
  int calc_temp;
  byte calc_temp_b;

  Wire.beginTransmission(0x68);
  Wire.write(0x3B);
  Wire.endTransmission();
  Wire.requestFrom(0x68, 14);

  for (int i = 0; i < 7; i++) {
    calc_temp = Wire.read();
    calc_temp = calc_temp << 8;
    calc_temp |= Wire.read();
    attitude_data[i] = calc_temp;
  }

  Wire.beginTransmission(0x0C);
  Wire.write(0x03);
  Wire.endTransmission();
  Wire.requestFrom(0x0C, 7);

  for (int i = 7; i < 10; i++) {
    calc_temp_b = Wire.read();
    calc_temp = Wire.read();
    calc_temp = calc_temp << 8;
    calc_temp |= calc_temp_b;
    attitude_data[i] = calc_temp;
  }

  Wire.read();
}

#include <Arduino.h>
#include <Wire.h>
#include "crawl_drive.h"

void initMotor() {
  stopMotor();
  Wire.beginTransmission(0x39);  // モータ基板のI2Cアドレスを指定
  Wire.write(0x02);              // 回転指令コマンド
  Wire.write(0x00);              // 回転方向指示(左右とも前進)
  Wire.write(0x00);              // 右モータPWM指定(停止)
  Wire.write(0x00);              // 左モータPWM指定(停止)
  Wire.endTransmission();        // 通信終了

  Wire.beginTransmission(0x39);  // モータ基板のI2Cアドレスを指定
  Wire.write(0x01);              // モータ出力を有効化
  Wire.endTransmission();        // 通信終了
}

void stopMotor() {
  Wire.beginTransmission(0x39);  // モータ基板のI2Cアドレスを指定
  Wire.write(0x00);              // モータ出力を無効化
  Wire.endTransmission();        // 通信終了
}

void setMoterPower(int left_power, int right_power) {
  byte motor_directions = 0x00;
  byte left_pwm;
  byte right_pwm;

  // left_power,right_powerを-255〜255の範囲にする
  if (left_power > 255) left_power = 255;
  if (left_power < -255) left_power = -255;
  if (right_power > 255) right_power = 255;
  if (right_power < -255) right_power = -255;

  // left_power,right_powerが0〜255なら
  // left_pwm,right_pwmをそのまま0〜255に
  // left_power,right_powerが-255〜0なら
  // left_pwm,right_pwmが255〜0になるように変換する
  left_pwm = abs(left_power);
  right_pwm = abs(right_power);

  // left_power,right_powerの符号を元に方向を指定
  if (left_power < 0) motor_directions |= 0x10;
  if (right_power < 0) motor_directions |= 0x01;

  Wire.beginTransmission(0x39);  // モータ基板のI2Cアドレスを指定
  Wire.write(0x02);              // 回転指令コマンド

  // motor_directions
  //   １ビット目が0なら右モータ正回転（前進）
  //   １ビット目が1なら右モータ逆回転（後退）
  //   ５ビット目が0なら左モータ正回転（前進）
  //   ５ビット目が1なら左モータ逆回転（後退）
  //   例
  //      0x01
  //     　左モータ前進，右モータ後退
  //
  //      0x10
  //     　左モータ後退，右モータ前進
  Wire.write(motor_directions);  // 回転方向指示

  // left_pwm， right_pwm
  //    左右モータのパワー（PWM）
  //    0x00なら停止
  //    0xffなら最大パワー
  Wire.write(right_pwm);   // 右モータPWM指定
  Wire.write(left_pwm);    // 左モータPWM指定
  Wire.endTransmission();  // 通信終了
}

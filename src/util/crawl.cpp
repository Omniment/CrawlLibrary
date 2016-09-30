/**
 * @file crawl.cpp
 * @brief
 * クロールに対して指令を出す,クロールの持っているセンサの情報を取得するためのライブラリ.
 */

#include <Arduino.h>
// 姿勢センサ
#include "attitude_sensor.h"
// モーター指令
#include "crawl_drive.h"
// クロールロボット
#include "crawl.h"
// エンコーダ読み取り
#include "encoder.h"
// センサヒュージョン
#include "kalmanfilter.h"
// Declared weak in Arduino.h to allow user redefinitions.
int atexit(void (*/*func*/)()) { return 0; }

// Weak empty variant initialization function.
// May be redefined by variant files.
void initVariant() __attribute__((weak));
void initVariant() {}

void setupUSB() __attribute__((weak));
void setupUSB() {}

// I2C
#include <Wire.h>

// 数学処理
#include <math.h>

CrlRobot crl;
KalmanFilter kf;
void CrlRobot::init() {
  ::init();
  ::initVariant();

#if defined(USBCON)
  ::USBDevice.attach();
#endif

  Wire.begin();          // I2Cセットアップ
  pinMode(13, OUTPUT);   // LEDピン設定
  initAttitudeSensor();  // 姿勢センサ機能の初期化
  delay(100);

  Serial.begin(9600);     // UARTを9600bpsでセットアップ
  digitalWrite(13, LOW);  // LEDピン設定

  resetEncoder();  // 累計回転数を初期化
  initMotor();     // 累計回転数を初期化

  t2 = micros();
  t1 = t2;

  kEtoMM = 1.95 / 7000.0;
  rate_theta = 0.99;

  this->encoder_left = 0;
  this->encoder_right = 0;

  initGyroOffset();
  initTheta();
  delay(300);
  digitalWrite(13, HIGH);  // LEDピン設定
}
void CrlRobot::initGyroOffset() {
  int i;
  float update_rate = 0.98;
  delay(500);
  getAttitude();
  this->offset_gx = attitude_data[4];
  this->offset_gy = attitude_data[5];
  this->offset_gz = attitude_data[6];
  for (i = 0; i < 200; i++) {
    getAttitude();
    delay(1);
    this->offset_gx =
        this->offset_gx * update_rate + attitude_data[4] * (1.0 - update_rate);
    this->offset_gy =
        this->offset_gy * update_rate + attitude_data[5] * (1.0 - update_rate);
    this->offset_gz =
        this->offset_gz * update_rate + attitude_data[6] * (1.0 - update_rate);
  }
}
void CrlRobot::initTheta() {
  int i;
  calcState();
  this->theta = M_PI / 2 - atan2(acc_y, acc_x);
  for (i = 0; i < 200; i++) {
    delay(1);
    getAttitude();
    calcState();
  }
}
void CrlRobot::realtimeLoop() { makeTiming(); }

void CrlRobot::makeTiming() {
  // Serial.println(this->tt);
  while (this->t1 - this->t2 < this->dt_us) this->t1 = micros();
  this->tt = this->t1 - this->t2;
  this->t2 = this->t1;
}

void CrlRobot::updateState() {
  getAttitude();
  getResetEncoder();
  setMoterPower(this->motor_left * 255, this->motor_right * 255);
  calcState();
  calcTheta();
  calcThetaKalmanFilter();
}

void CrlRobot::calcState() {
  float ax, ay, az, gx, gy, gz;

  this->encoder_left += left_encoder;
  this->encoder_right += right_encoder;

  ax = attitude_data[0];
  ay = attitude_data[1];
  az = attitude_data[2];

  gx = (attitude_data[4] - offset_gx);
  gy = (attitude_data[5] - offset_gy);
  gz = (attitude_data[6] - offset_gz);

  this->acc_x = ay * 0.00059855;  // 2/2^15*9.80665=0.00059855
  this->acc_y = az * 0.00059855;
  this->acc_z = ax * 0.00059855;

  this->theta_dot_x = gy * 0.00013316;  // 250/(2^15)/360*2*pi=0.00013316
  this->theta_dot_y = gz * 0.00013316;
  this->theta_dot_z = gx * 0.00013316;
}

void CrlRobot::calcTheta() {
  float theta1;
  theta1 = M_PI / 2 - atan2(acc_y, acc_x);
  this->theta =
      this->theta * this->rate_theta + theta1 * (1.0 - this->rate_theta);
  this->theta = this->theta + this->theta_dot_z * this->dt;
}
void CrlRobot::calcThetaKalmanFilter() {
  float theta, gyro;
  theta = M_PI / 2 - atan2(acc_y, acc_x);
  gyro = this->theta_dot_z;
  kf.update(theta, gyro);
  this->theta_kalman = kf.getTheta();
}

// 各種アクセサ
void CrlRobot::setDt(float _dt) {
  this->dt = _dt;
  this->dt_us = _dt * 1000000;
}

void CrlRobot::setMotorLeft(float motor_left) { this->motor_left = motor_left; }

void CrlRobot::setMotorRight(float motor_right) {
  this->motor_right = motor_right;
}

void CrlRobot::resetEncoderLeft() { this->encoder_left = 0; }

void CrlRobot::resetEncoderRight() { this->encoder_right = 0; }

float CrlRobot::getThetaX() { return this->theta; }

float CrlRobot::getThetaY() { return this->theta; }

float CrlRobot::getThetaZ() { return this->theta; }

float CrlRobot::getThetaKalman() { return this->theta_kalman; }

float CrlRobot::getAccX() { return this->acc_x; }

float CrlRobot::getAccY() { return this->acc_y; }

float CrlRobot::getAccZ() { return this->acc_z; }

float CrlRobot::getThetaDotX() { return this->theta_dot_x; }

float CrlRobot::getThetaDotY() { return this->theta_dot_y; }

float CrlRobot::getThetaDotZ() { return this->theta_dot_z; }

float CrlRobot::getEncoderLeft() { return this->encoder_left; }

float CrlRobot::getEncoderRight() { return this->encoder_right; }

float CrlRobot::getOdometryLeft() { return this->encoder_left * this->kEtoMM; }

float CrlRobot::getOdometryRight() {
  return this->encoder_right * this->kEtoMM;
}

FirtstOrderFilter::FirtstOrderFilter() : y(0), T(1), dt(0.001) {}

void FirtstOrderFilter::setDt(float dt) { this->dt = dt; }

void FirtstOrderFilter::setT(float T) {
  if (0.0 < T) {
    this->T = T;
  } else {
    this->T = 1.0;
  }
}

float FirtstOrderFilter::calculate(float x) {
  float k, k0, k1, k2, k3;
  k0 = (x - y) / T;
  k1 = (x - (y + k0 * dt * 0.5)) / T;
  k2 = (x - (y + k1 * dt * 0.5)) / T;
  k3 = (x - (y + k1 * dt)) / T;
  k = (k0 + 2 * k1 + 2 * k2 + k3) / 6;
  y = y + k * dt;
}
float FirtstOrderFilter::getOutput() { return y; }

float LaggedDerivative::calculate(float x) {
  FirtstOrderFilter::calculate(x);
  this->y = (x - FirtstOrderFilter::getOutput()) / FirtstOrderFilter::T;
}
float LaggedDerivative::getOutput() { return this->y; }

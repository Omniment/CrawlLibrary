#include <Arduino.h>
#include "common.h"
// Declared weak in Arduino.h to allow user redefinitions.
int atexit(void (* /*func*/ )()) { return 0; }

// Weak empty variant initialization function.
// May be redefined by variant files.
void initVariant() __attribute__((weak));
void initVariant() { }

void setupUSB() __attribute__((weak));
void setupUSB() { }

// I2C
#include<Wire.h>

// 数学処理
#include <math.h>

CrlRobot crl;

void CrlRobot::init(){
  ::init();
  ::initVariant();
  
#if defined(USBCON)
  ::USBDevice.attach();
#endif

  Wire.begin();           // I2Cセットアップ
  pinMode(13, OUTPUT);    // LEDピン設定
  initAttitudeSensor();   // 姿勢センサ機能の初期化
  delay(100);
  
  Serial.begin(9600);     // UARTを9600bpsでセットアップ
  digitalWrite(13, LOW);  // LEDピン設定
  
  resetEncoder();         // 累計回転数を初期化
  initMotor();            // 累計回転数を初期化

  t2 = micros();
  t1 = t2;

  kEtoMM = 1.95 / 7.0;
  rate_theta = 0.99;

  this->encoder_l = 0;
  this->encoder_r = 0;

  initGyroOffset();
  initTheta();
  delay(300);
  digitalWrite(13, HIGH);  // LEDピン設定
}
void CrlRobot::initGyroOffset(){
  int i;
  float update_rate = 0.98;
  delay(500);
  getAttitude();
  this->offset_gx=attitude_data[4];
  this->offset_gy=attitude_data[5];
  this->offset_gz=attitude_data[6];
  for(i=0; i<200; i++){
    getAttitude();
    delay(1);
    this->offset_gx = this->offset_gx * update_rate + attitude_data[4] * (1.0 - update_rate);
    this->offset_gy = this->offset_gy * update_rate + attitude_data[5] * (1.0 - update_rate);
    this->offset_gz = this->offset_gz * update_rate + attitude_data[6] * (1.0 - update_rate);
  }
}
void CrlRobot::initTheta(){
  int i;
  calcState();
  this->theta = M_PI/2-atan2( acc_y, acc_x );
  for(i=0; i<200; i++){
    delay(1);
    getAttitude();
    calcState();
  }
}
void CrlRobot::realtimeLoop(){
  makeTiming();
}

void CrlRobot::makeTiming(){
  //Serial.println(this->tt);
  while(this->t1 - this->t2 < this->dt_us) this->t1 = micros();
  this->tt = this->t1 - this->t2;
  this->t2 = this->t1;
}

void CrlRobot::updateState(){
  getAttitude();
  getResetEncoder();
  setMoterPower(this->motor_l * 255, this->motor_r * 255);
  calcState();
  calcTheta();
}

void CrlRobot::calcState(){
  float ax,ay,az,gx,gy,gz;
  
  this->encoder_l += left_encoder * this->kEtoMM;
  this->encoder_r += right_encoder * this->kEtoMM;

  ax = attitude_data[0];
  ay = attitude_data[1];
  az = attitude_data[2];

  gx = (attitude_data[4] - offset_gx);
  gy = (attitude_data[5] - offset_gy);
  gz = (attitude_data[6] - offset_gz);

  this->acc_x =  ay*0.00059855;    // 2/2^15*9.80665=0.00059855
  this->acc_y =  az*0.00059855;
  this->acc_z =  ax*0.00059855;

  this->theta_dot_x = gy*0.00013316;     // 250/(2^15)/360*2*pi=0.00013316
  this->theta_dot_y = gz*0.00013316;
  this->theta_dot_z = gx*0.00013316;
}

void CrlRobot::calcTheta(){
  float theta1;
  theta1 = M_PI/2-atan2( acc_y, acc_x );
  this->theta = this->theta * this->rate_theta + theta1 * (1.0 - this->rate_theta);
  this->theta = this->theta + this->theta_dot_z * this->dt;
}

// 各種アクセサ
void CrlRobot::set_dt(float _dt){
  this->dt = _dt;
  this->dt_us = _dt * 1000000;
}
 
void CrlRobot::set_motor_l(float motor_l){
  this->motor_l = motor_l;
}

void CrlRobot::set_motor_r(float motor_r){
  this->motor_r = motor_r;
}

float CrlRobot::get_theta(){
  return this->theta;
}
    
float CrlRobot::get_acc_x(){
  return this->acc_x;
}

float CrlRobot::get_acc_y(){
  return this->acc_y;
}

float CrlRobot::get_acc_z(){
  return this->acc_z;
}

float CrlRobot::get_theta_dot_x(){
  return this->theta_dot_x;
}

float CrlRobot::get_theta_dot_y(){
  return this->theta_dot_y;
}

float CrlRobot::get_theta_dot_z(){
  return this->theta_dot_z;
}

float CrlRobot::get_encoder_l(){
  return this->encoder_l;
}

float CrlRobot::get_encoder_r(){
  return this->encoder_r;
}


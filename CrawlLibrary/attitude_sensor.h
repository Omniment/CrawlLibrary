#ifndef INCLUDED_attitude_sensor_h
#define INCLUDED_attitude_sensor_h

//
// attitude_sensor.h
//
// 概要
//
// 9軸姿勢センサ（MPU-9250）により姿勢情報を得る．
// センサＩＣとマイコンはシリアル通信（I2C）によりコマンドの送受信をする．
// 
// void initAttitudeSensor();
// 姿勢センサ機能の初期化関数．
// 姿勢センサを使用する前に一度呼び出す．
//
// void getAttitude();
// 姿勢データを取得する．
// 取得された姿勢データはグローバル変数attitude_dataに格納される．

void initAttitudeSensor();
void getAttitude();

extern int attitude_data[10];

#endif

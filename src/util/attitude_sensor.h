/**
 * @file attitude_sensor.h
 * @brief
 * 9軸姿勢センサ（MPU-9250）により姿勢情報を得る
 *
 * センサＩＣとマイコンはシリアル通信（I2C）によりコマンドの送受信をする．
 */
#ifndef INCLUDED_attitude_sensor_h
#define INCLUDED_attitude_sensor_h
/**
 * @brief 姿勢センサ機能の初期化関数
 *
 * 姿勢センサを使用する前に必ず呼び出してください.
 * @return なし
 */
void initAttitudeSensor();
/**
 * @brief 姿勢データを取得する
 *
 * 取得された姿勢データはグローバル変数attitude_dataに格納される．
 * @return なし
 */
void getAttitude();
/** 姿勢データ */
extern int attitude_data[10];
#endif

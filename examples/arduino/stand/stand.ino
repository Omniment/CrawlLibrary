#include <crawl.h>
#define PI 3.14159265358979  //円周率を定義

int main() {
  float Kp1 = 10.0;  // 角度制御比例ゲイン (調節パラメータ)
  float Kp2 = 2.5;  // 上端速度制御比例ゲイン (調節パラメータ)
  float Ki2 = 120.0;  // 上端速度制御積分ゲイン (調節パラメータ)
  float theta_d_offset = 0.036;  // 重心の偏り(調節パラメータ)

  float dt = 0.010;             // サンプリング時間 [s]
  float theta;                  // 角度 [rad]
  float head_velocity;          // 上端速度 [m/s]
  float theta_d = PI / 2.0;     // 目標角度 [rad]
  float head_velocity_d = 0.0;  // 目標上端速度 [m/s]
  float err1;                   // 目標角度と実角度θの偏差
  float err2;                   // 目標上端速度と実上端速度の偏差
  float u;                      // 制御入力  -1.0〜0〜1.0

  Integral err2_i;                 // 上端速度用の制限付き積分要素
  err2_i.setDt(dt);                // サンプリング時間を設定
  err2_i.setLimit(-0.001, 0.001);  // 下限値,上限値を設定

  crl.init();     // ロボットの初期化
  crl.setDt(dt);  // サンプリング時間を設定

  while (1) {
    crl.realtimeLoop();  // dt[s]ごとに以下ループを実行
    crl.updateState();   // 各種センサ情報取得,モータ出力の更新
    theta = crl.getThetaZ();  // クロールの実姿勢角度を取得
    head_velocity = crl.getHeadVelocity();  // クロールの実上端速度を取得

    err1 = (theta_d + theta_d_offset) - theta;  // 目標角度と実角度の偏差を計算
    err2 = head_velocity_d -
           head_velocity;  // 目標上端速度と実上端速度の偏差を計算
    err2_i.calculate(err2);  // 目標上端速度と実上端速度の偏差の積分値を計算
    u = err1 * Kp1 + err2 * Kp2 +
        err2_i.getOutput() * Ki2;  // PI制御により制御入力を計算

    if (theta < PI * 1.0 / 4.0 ||
        PI * 3.0 / 4.0 <
            theta) {  // クロールの姿勢θがPI/2付近以外でモータを停止
      u = 0;
    }
    crl.setMotorLeft(u);   // 制御入力を左モータに設定
    crl.setMotorRight(u);  // 制御入力を右モータに設定
  }
}

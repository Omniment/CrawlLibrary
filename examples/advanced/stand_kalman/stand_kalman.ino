#include <crawl.h>
#define CRL_PI 3.14159265358979  //円周率を定義

int main() {
  float kp1 = 5.0;   // 角度制御比例ゲイン (調節パラメータ)
  float kp2 = 8.0;   // 上端速度制御比例ゲイン (調節パラメータ)
  float ki2 = 40.0;  // 上端速度制御積分ゲイン (調節パラメータ)

  float dt = 0.010;              // サンプリング時間 [s]
  float theta;                   // 角度 [rad]
  float head_velocity;           // 上端速度 [m/s]
  float theta_d = CRL_PI / 2.0;  // 目標角度 [rad]
  float head_velocity_d = 0.0;   // 目標上端速度 [m/s]
  float err1;                    // 目標角度と実角度θの偏差
  float err2;                    // 目標上端速度と実上端速度の偏差
  float u;                       // 制御入力  -1.0〜0〜1.0

  Integral err2i;
  err2i.setDt(dt);
  err2i.setLimit(-5.0, 5.0);

  FirstOrderFilter fof_err2;
  fof_err2.setDt(dt);
  fof_err2.setT(1.0 / 15);

  FirstOrderFilter fof_err2i;
  fof_err2i.setDt(dt);
  fof_err2i.setT(1.0 / 5);

  crl.init();            // ロボットの初期化
  crl.setDt(dt);         // サンプリング時間を設定
  crl.setKalman(false);  // センサヒュージョンの方法を設定
  while (1) {
    crl.realtimeLoop();                     // dt[s]ごとに以下ループを実行
    crl.updateState();                      // 各種センサ情報取得,モータ出力の更新
    theta = crl.getThetaZ();                // クロールの実姿勢角度を取得
    head_velocity = crl.getHeadVelocity();  // クロールの実上端速度を取得

    err1 = (theta_d + fof_err2i.getOutput() * ki2) - theta;  // 目標角度と実角度の偏差を計算
    err2 = head_velocity_d - head_velocity;                  // 目標上端速度と実上端速度の偏差を計算
    fof_err2.calculate(err2);
    err2i.calculate(err2);
    fof_err2i.calculate(err2i.getOutput());
    u = err1 * kp1 + fof_err2.getOutput() * kp2;  // P制御により制御入力を計算

    if (theta < CRL_PI * 1.0 / 4.0 || CRL_PI * 3.0 / 4.0 < theta) {  // クロールの姿勢θがPI/2付近以外でモータを停止
      u = 0;
    }
    crl.setMotorLeft(u);   // 制御入力を左モータに設定
    crl.setMotorRight(u);  // 制御入力を右モータに設定
  }
}

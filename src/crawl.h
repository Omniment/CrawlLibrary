/**
 * @file crawl.h
 * @brief
 * クロールに対して指令を出す,クロールの持っているセンサの情報を取得するためのライブラリ.
 */

#ifndef INCLUDED_Crawl_h
#define INCLUDED_Crawl_h
#include <Arduino.h>

class CrlRobot {
 public:
  /**
   * @brief クロールの初期化
   *
   * クロールを使用する前に必ず呼び出してください.
   * 加速度,ジャイロセンサのキャリブレーションを行うので約1.5秒程度の時間を要します.
   * その際,クロールは水平な場所に置き,安静にしてください.
   *
   * @return なし
   * @warning
   *このメンバ関数を呼ばずに他のメンバ関数を呼び出すと予期せぬエラーが発生します.
   * @attention
   *キャリブレーション中に水平な場所に置き,安静にしない場合,正しくセンサの値を得られません.
   */
  void init();
  /**
   * @brief ループが一定間隔になるよう調節する
   *
   * このメンバ関数は,前回このメンバ関数がリターンした時刻からdt秒後にリターンします.
   * このメンバ関数をループの中で呼び出すと,そのループは正確にdt秒起きに実行されます.
   * この特性はPID制御など,正確なリアルタイム性を求められる場合に非常に有効です.
   *
   * @return なし
   * @warning
   *ループ内の処理がdt秒を超える場合にはこのメンバ関数は正しく動作せず,直ちにリターンします.
   * @sa set_dt(float dt)
   */
  void realtimeLoop();
  /**
   * @brief ループ間隔を設定する
   *
   * このメンバ関数は,realtimeLoop()で実現されるループの時間間隔を指定します.
   *
   * @param dt ループ間隔 単位:秒
   * @return なし
   * @warning
   *このメンバ関数を呼び出さない場合ループ間隔として,初期値として0秒が指定され,
   * realtimeLoop()が呼ばれた場合には,即時リターンされます.
   * @sa realtimeLoop()
   */
  void set_dt(float dt);
  /**
   * @brief X軸周りの姿勢角度を取得する
   * @return X軸周りの姿勢角度 単位:rad [-pi/2, +3pi/2]
   * @attention updateState()を呼び出さない限り,情報は更新されません
   * @sa updateState()
   */
  float get_theta_x();
  /**
   * @brief Y軸周りの姿勢角度を取得する
   * @return Y軸周りの姿勢角度 単位:rad [?,?]
   * @attention updateState()を呼び出さない限り,情報は更新されません
   * @sa updateState()
   */
  float get_theta_y();
  /**
   * @brief Z軸周りの姿勢角度を取得する
   * @return Z軸周りの姿勢角度 単位:rad [?,?]
   * @attention updateState()を呼び出さない限り,情報は更新されません
   * @sa updateState(),updateState()
   */
  float get_theta_z();
  /**
   * @brief X軸方向の加速度を取得する
   * @return X軸方向の加速度 単位:m/s^2
   * @attention updateState()を呼び出さない限り,情報は更新されません
   * @sa updateState()
   */
  float get_acc_x();
  /**
   * @brief Y軸方向の加速度を取得する
   * @return Y軸方向の加速度 単位:m/s^2
   * @attention updateState()を呼び出さない限り,情報は更新されません
   * @sa updateState()
   */
  float get_acc_y();
  /**
   * @brief Z軸方向の加速度を取得する
   * @return Z軸方向の加速度 単位:m/s^2
   * @attention updateState()を呼び出さない限り,情報は更新されません
   * @sa updateState()
   */
  float get_acc_z();
  /**
   * @brief X軸周りの角速度を取得する
   * @return X軸周りの角速度 単位:rad/srealtimeLoop()
   * @attention updateState()を呼び出さない限り,情報は更新されません
   * @sa updateState()
   */
  float get_theta_dot_x();
  /**
   * @brief Y軸周りの角速度を取得する
   * @return Y軸周りの角速度 単位:rad/s
   * @attention updateState()を呼び出さない限り,情報は更新されません
   * @sa updateState()
   */
  float get_theta_dot_y();
  /**
   * @brief Z軸周りの角速度を取得する
   * @return Z軸周りの角速度 単位:rad/s
   * @attention updateState()を呼び出さない限り,情報は更新されません
   * @sa updateState()
   */
  float get_theta_dot_z();
  /**
   * @brief 左エンコーダの値を取得する
   *
   *左クローラの進んだ距離の累積値を取得します.
   * @return 左エンコーダの値 単位:mm
   * @attention updateState()を呼び出さない限り,情報は更新されません
   * @sa updateState()
   */
  float get_encoder_l();
  /**
   * @brief 右エンコーダの値を取得する
   *
   *右クローラの進んだ距離の累積値を取得します.
   * @return 右エンコーダの値 単位:mm
   * @attention updateState()を呼び出さない限り,情報は更新されません
   * @sa updateState()
   */
  float get_encoder_r();
  /**
   * @brief 左モーターの出力を設定
   *
   * 左モーターの出力を-1.0〜0.0〜1.0でリニアに設定します.
   * @param motor_l 左モータの出力
   * -1.0:最大出力で後退〜0.0:停止〜1.0:最大出力で前進
   * @return なし
   * @attention updateState()を呼び出さない限り,情報は更新されません
   * @sa updateState()
   */
  void set_motor_l(float motor_l);
  /**
   * @brief 右モーターの出力を設定
   *
   * 右モーターの出力を-1.0〜0.0〜1.0でリニアに設定します.
   * @param motor_l 右モータの出力
   * -1.0:最大出力で後退〜0.0:停止〜1.0:最大出力で前進
   * @return なし
   * @attention updateState()を呼び出さない限り,情報は更新されません
   * @sa updateState()
   */
  void set_motor_r(float motor_r);
  /**
   * @brief 左エンコーダの累積値を設定
   *
   * 左エンコーダの累積値などのリセットなどに利用ください.
   * @param encoder_l 左エンコーダの累積値
   * @return なし
   */
  void set_encoder_l(float encoder_l);
  /**
   * @brief 右エンコーダの累積値を設定
   *
   * 右エンコーダの累積値などのリセットなどに利用ください.
   * @param encoder_l 右エンコーダの累積値
   * @return なし
   */
  void set_encoder_r(float encoder_r);
  /**
   * @brief クロールの状態を更新
   *
   * このメンバ関数では,set_motor_l(),set_motor_r()などで設定したモータ出力を実際の出力に適用します.
   * また,get_encoder_l()など各種状態･センサ取得関数で得られる情報の更新も行われます.
   * @return なし
   * @attention
   *このメンバ関数が呼ばれない限り,モータ出力や,センサの値は更新されません.
   * @note
   *制御ループを実装する場合にはrealtimeLoop()の直下で呼び出すことを推奨します.
   */
  void updateState();

  /// @cond develop
  /** ループ間隔 単位:秒 */
  float dt;
  /** ループ間隔 単位:マイクロ秒 */
  int dt_us;
  /** Z軸周りの姿勢角度 単位:rad */
  float theta;
  /** X軸方向の加速度 単位:m/s^2 */
  float acc_x;
  /** Y軸方向の加速度 単位:m/s^2 */
  float acc_y;
  /** Z軸方向の加速度 単位:m/s^2 */
  float acc_z;
  /** X軸周りの角速度 単位:rad/s */
  float theta_dot_x;
  /** Y軸周りの角速度 単位:rad/s */
  float theta_dot_y;
  /** Z軸周りの角速度 単位:rad/s */
  float theta_dot_z;
  /** X軸方向の加速度のセンサの真値との偏差 単位:m/s^2 */
  float offset_gx;
  /** Y軸方向の加速度のセンサの真値との偏差 単位:m/s^2 */
  float offset_gy;
  /** Z軸方向の加速度のセンサの真値との偏差 単位:m/s^2 */
  float offset_gz;
  /** 左モータの出力設定値 */
  float motor_l;
  /** 右モータの出力設定値 */
  float motor_r;
  /** 左エンコーダ累積値 */
  float encoder_l;
  /** 右エンコーダ累積値 */
  float encoder_r;
  /** リアルタイムを実現するための時間計測用変数 */
  unsigned long t1;
  /** リアルタイムを実現するための時間計測用変数 */
  unsigned long t2;
  ;
  /** リアルタイムを実現するための時間計測用変数 */
  unsigned long tt;
  /** エンコーダパルス数を移動距離に変換するための係数 */
  float kEtoMM;
  /**
   * 姿勢角度計算用相補フィルターの係数(角速度センサーから求まる姿勢角度と加速度センサーから求まる姿勢角度の寄与度)*/
  float rate_theta;

  /**
   * @brief 姿勢角度計算用相補フィルター
   *
   * 角速度センサーから求まる姿勢角度と加速度センサーから求まる姿勢角度をもとに相補フィルターによって計算し姿勢角度を求めます.
   * @return なし
   * @sa rate_theta
   */
  void calcTheta();
  /**
   * @brief realtimeLoop()のサブ関数
   *
   * @return なし
   * @sa realtimeLoop()
   */
  void makeTiming();
  /**
   * @brief updateState()のサブ関数
   *
   * 取得した生のセンサ情報をキャリブレーション情報を元に加工し,使用しやすい値に変換します.
   * @return なし
   * @sa updateState()
   */
  void calcState();
  /**
   * @brief updateState()のサブ関数
   *
   * 取得した生のセンサ情報をキャリブレーション情報を元に加工し,使用しやすい値に変換します.
   * @return なし
   * @sa updateState()
   */
  void initGyroOffset();
  /**
   * @brief init()のサブ関数
   *
   * ジャイロセンサのキャリブレーションを行う.700ミリ秒要する
   * この時,クロールはなるべく水平にし,衝撃などは与えないでください.
   * @return なし
   * @sa init()
   */
  void initTheta();
  /**
   * @brief init()のサブ関数
   *
   * 姿勢角度のキャリブレーションを行う.200ミリ秒要する
   * この時,クロールはなるべく水平にし,衝撃などは与えないでください.
   * @return なし
   * @sa init()
   */
   /// @endcond
};

/** CrlRobotのインスタンス */
extern CrlRobot crl;

#endif

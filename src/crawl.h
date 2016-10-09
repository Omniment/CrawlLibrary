/**
 * @file crawl.h
 * @brief
 * クロールに対して指令を出す,クロールの持っているセンサの情報を取得するためのライブラリ.
 * 制御に必要とされる一次遅れフィルターや不完全微分
 */

#ifndef INCLUDED_Crawl_h
#define INCLUDED_Crawl_h
#include <Arduino.h>

/**
 * @class FirstOrderFilter
 * @brief
 * 一次遅れフィルタークラス.
 */
class FirstOrderFilter {
  /// @cond develop
 protected:
  /** サンプリング時間 */
  float dt;
  /** 時定数 */
  float T;
  /** 出力値 */
  float y;
  /// @endcond
 public:
  /**
   * @brief コンストラクタ
   *
   * 一時遅れフィルタのコンストラクタ.出力初期値は0,時定数は1秒,サンプリングタイムは1ミリ秒で初期化されます.
   * @return なし
   */
  FirstOrderFilter();
  /**
   * @brief サンプリングタイムを設定する
   *
   * @param dt ループ間隔 単位:秒
   * @return なし
   */
  void setDt(float dt);
  /**
   * @brief 時定数を設定する
   *
   * @param T 時定数 単位:秒
   * @return なし
   */
  void setT(float T);

  /**
   * @brief 出力値を取得する
   *
   * フィルタを通した値を取得します.
   * @return フィルタを通した値
   */
  float getOutput();
  /**
   * @brief 入力から出力を計算する
   *
   * ルンゲクッタ法により4次の精度で一時遅れフィルターを計算します.
   * @param x 入力値
   * @return なし
   * @attention ループ中では一回だけ呼び出すようにしてください.
   */
  float calculate(float x);
};

/**
 * @class LaggedDerivative
 * @brief
 * 不完全微分要素クラス.
 *
 * 一次遅れフィルタークラスを継承
 */
class LaggedDerivative : public FirstOrderFilter {
  /// @cond develop
  /** 出力値 */
  float y;
  /// @endcond
 public:
  /**
   * @brief 入力から出力を計算する
   *
   * ルンゲクッタ法により4次の精度で不完全微分を計算します.
   * @param x 入力値
   * @return なし
   * @attention ループ中では一回だけ呼び出すようにしてください.
   */
  float calculate(float x);
  /**
   * @brief 不完全微分した値を取得する
   *
   * 不完全微分した値を取得します.
   * @return 不完全微分した値
   */
  float getOutput();
};
/**
 * @class Integral
 * @brief
 * 制限付き積分要素クラス
 *
 * 制限値を設定できる積分要素クラスです.積分値が制限値よりも大きく(または小さく)
 * なる場合制限値の範囲に修正され収められます.
 * 制限値は任意で設定できます.設定しない場合,下限値としてfloatの負の最大値,
 * 上限値として,floatの正の最大値が設定されます.
 * このクラスはサンプリング時間[s]を指定する必要があります.ループ内で使用する場合,
 * 一般的に一回のループ時間を指定ください.
 */
class Integral {
 public:
  /**
   * @brief コンストラクタ
   *
   * 制限付き積分要素クラスのコンストラクタ.出力初期値は0,時定数は1秒,サンプリングタイムは1ミリ秒で初期化されます.
   * 制限値は下限値としてfloatの負の最大値,上限値として,floatの正の最大値が設定されます.
   * @return なし
   */
  Integral();
  /**
   * @brief サンプリングタイムを設定する
   *
   * @param dt ループ間隔 単位:秒
   * @return なし
   */
  void setDt(float dt);
  /**
   * @brief 出力される積分値を設定する
   *
   *
   * 積分値をリセットしたい場合,
   * 任意の値を予め与えておきたい場合に使用してください
   * @param y 設定したい値
   * @return なし
   */
  void setOutput(float y);
  /**
   * @brief 積分の制限値を設定する
   *
   * 積分値が大きく(または小さく)なりすぎることを制限することができます.
   * 下限値と上限値を指定することができ,この範囲を積分値が超える場合
   * その範囲に収まるように修正されます.
   * @param limit_low 下限値
   * @param limit_high 上限値
   * @return なし
   */
  void setLimit(float limit_low, float limit_high);
  /**
   * @brief 入力から出力を計算する
   *
   * 制限付き積分を計算します.
   * @param x 入力値
   * @return なし
   * @attention ループ中では一回だけ呼び出すようにしてください.
   */
  void calculate(float x);
  /**
   * @brief 制限付き積分した値を取得する
   *
   * 制限付き積分した値を取得します.
   * @return 制限付き積分した値
   */
  float getOutput();

  /// @cond develop
  /** 時定数 */
  float y;
  /** 下限値 */
  float limit_low;
  /** 上限値 */
  float limit_high;
  /** サンプリング時間 */
  float dt;
  /// @endcond
};
/**
 * @class CrlRobot
 * @brief
 * クロールに対して指令を出す,クロールの持っているセンサの情報を取得するためのクラス.
 */
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
   * @sa setDt(float dt)
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
  void setDt(float dt);
  /**
   * @brief X軸周りの姿勢角度を取得する
   * @return X軸周りの姿勢角度 単位:rad [-pi/2, +3pi/2]
   * @attention updateState()を呼び出さない限り,情報は更新されません
   * @sa updateState()
   */
  float getThetaX();
  /**
   * @brief Y軸周りの姿勢角度を取得する
   * @return Y軸周りの姿勢角度 単位:rad [?,?]
   * @attention updateState()を呼び出さない限り,情報は更新されません
   * @sa updateState()
   */
  float getThetaY();
  /**
   * @brief Z軸周りの姿勢角度を取得する
   * @return Z軸周りの姿勢角度 単位:rad [?,?]
   * @attention updateState()を呼び出さない限り,情報は更新されません
   * @sa updateState()
   */
  float getThetaZ();
  /**
   * @brief 倒立時のクロール上端の速度を取得する
   *
   * 倒立時のクロール上端の速度を取得する.取得する速度はグローバル座標のX軸方向
   * の速度であり,グローバル座標でX軸正方向にクロール上端が移動すると正の速度が
   * 取得され,X軸負方向への移動の場合も同様に負の速度が取得されます.
   * @return 倒立時のクロール上端の速度 単位:m/s
   * @attention updateState()を呼び出さない限り,情報は更新されません
   * @sa updateState()
   */
  float getHeadVelocity();
  /**
   * @brief X軸方向の加速度を取得する
   * @return X軸方向の加速度 単位:m/s^2
   * @attention updateState()を呼び出さない限り,情報は更新されません
   * @sa updateState()
   */
  float getAccX();
  /**
   * @brief Y軸方向の加速度を取得する
   * @return Y軸方向の加速度 単位:m/s^2
   * @attention updateState()を呼び出さない限り,情報は更新されません
   * @sa updateState()
   */
  float getAccY();
  /**
   * @brief Z軸方向の加速度を取得する
   * @return Z軸方向の加速度 単位:m/s^2
   * @attention updateState()を呼び出さない限り,情報は更新されません
   * @sa updateState()
   */
  float getAccZ();
  /**
   * @brief X軸周りの角速度を取得する
   * @return X軸周りの角速度 単位:rad/srealtimeLoop()
   * @attention updateState()を呼び出さない限り,情報は更新されません
   * @sa updateState()
   */
  float getThetaDotX();
  /**
   * @brief Y軸周りの角速度を取得する
   * @return Y軸周りの角速度 単位:rad/s
   * @attention updateState()を呼び出さない限り,情報は更新されません
   * @sa updateState()
   */
  float getThetaDotY();
  /**
   * @brief Z軸周りの角速度を取得する
   * @return Z軸周りの角速度 単位:rad/s
   * @attention updateState()を呼び出さない限り,情報は更新されません
   * @sa updateState()
   */
  float getThetaDotZ();
  /**
   * @brief 左エンコーダの値を取得する
   *
   *左クローラの進んだ距離の累積値を取得します.
   * @return 左エンコーダの値 単位:パルス
   * @attention updateState()を呼び出さない限り,情報は更新されません
   * @sa updateState()
   */
  float getEncoderLeft();
  /**
   * @brief 右エンコーダの値を取得する
   *
   *右クローラの進んだ距離の累積値を取得します.
   * @return 右エンコーダの値 単位:パルス
   * @attention updateState()を呼び出さない限り,情報は更新されません
   * @sa updateState()
   */
  float getEncoderRight();
  /**
   * @brief 左クローラの進んだ距離の値を取得する
   *
   *左クローラの進んだ距離の累積値をメートル単位で取得します.
   * @return 左エンコーダの値 単位:m
   * @attention updateState()を呼び出さない限り,情報は更新されません
   * @sa updateState()
   */
  float getOdometryLeft();
  /**
   * @brief 右クローラの進んだ距離を取得する
   *
   *右クローラの進んだ距離の累積値をメートル単位で取得します.
   * @return 右エンコーダの値 単位:m
   * @attention updateState()を呼び出さない限り,情報は更新されません
   * @sa updateState()
   */
  float getOdometryRight();
  /**
   * @brief 左モーターの出力を設定
   *
   * 左モーターの出力を-1.0〜0.0〜1.0でリニアに設定します.
   * @param motor_left 左モータの出力
   * -1.0:最大出力で後退〜0.0:停止〜1.0:最大出力で前進
   * @return なし
   * @attention updateState()を呼び出さない限り,情報は更新されません
   * @sa updateState()
   */
  void setMotorLeft(float motor_left);
  /**
   * @brief 右モーターの出力を設定
   *
   * 右モーターの出力を-1.0〜0.0〜1.0でリニアに設定します.
   * @param motor_right 右モータの出力
   * -1.0:最大出力で後退〜0.0:停止〜1.0:最大出力で前進
   * @return なし
   * @attention updateState()を呼び出さない限り,情報は更新されません
   * @sa updateState()
   */
  void setMotorRight(float motor_right);
  /**
   * @brief 左エンコーダの累積値を0に設定する
   *
   * 左エンコーダの累積値などのリセットなどに利用ください.
   * @return なし
   */
  void resetEncoderLeft();
  /**
   * @brief 右エンコーダの累積値を0に設定する
   *
   * 右エンコーダの累積値などのリセットなどに利用ください.
   * @return なし
   */
  void resetEncoderRight();
  /**
   * @brief クロールの状態を更新
   *
   * このメンバ関数では,setMotorLeft(),setMotorRight()などで設定したモータ出力を実際の出力に適用します.
   * また,getEncoderLeft()など各種状態･センサ取得関数で得られる情報の更新も行われます.
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
  /** クロール上端のX軸方向速度 単位:m/s */
  float head_velocity;
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
  /** X軸周りの角速度の真値との偏差 */
  float offset_gx;
  /** Y軸周りの角速度の真値との偏差 */
  float offset_gy;
  /** Z軸周りの角速度の真値との偏差 */
  float offset_gz;
  /** 左モータの出力設定値 */
  float motor_left;
  /** 右モータの出力設定値 */
  float motor_right;
  /** 左エンコーダ累積値 */
  float encoder_left;
  /** 右エンコーダ累積値 */
  float encoder_right;
  /** リアルタイムを実現するための時間計測用変数 */
  unsigned long t1;
  /** リアルタイムを実現するための時間計測用変数 */
  unsigned long t2;
  /** リアルタイムを実現するための時間計測用変数 */
  unsigned long tt;
  /** エンコーダパルス数を移動距離に変換するための係数 */
  float kEtoMM;
  /**
   * 姿勢角度計算用相補フィルターの係数(角速度センサーから求まる姿勢角度と加速度センサーから求まる姿勢角度の寄与度)*/
  float rate_theta;
  /** X軸方向の加速度センサ用の一次遅れフィルタ */
  FirstOrderFilter fof_acc_x;
  /** Y軸方向の加速度センサ用の一次遅れフィルタ */
  FirstOrderFilter fof_acc_y;
  /** Z軸方向の加速度センサ用の一次遅れフィルタ */
  FirstOrderFilter fof_acc_z;
  /** オドメトリをから移動速度を計算するための不完全微分 */
  LaggedDerivative ld_odometry;

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
   * 倒立時のクロール上端の速度を計算する.
   * @return なし
   * @sa updateState()
   */
  void calcHeadVelocity();
  /**
   * @brief init()のサブ関数
   *
   * ジャイロセンサのキャリブレーションを行う.700ミリ秒要する
   * この時,クロールはなるべく水平にし,衝撃などは与えないでください.
   * @return なし
   * @sa init()
   */
  void initGyroOffset();
  /**
   * @brief init()のサブ関数
   *
   * 姿勢角度のキャリブレーションを行う.200ミリ秒要する
   * この時,クロールはなるべく水平にし,衝撃などは与えないでください.
   * @return なし
   * @sa init()
   */
  void initTheta();
  /// @endcond
};

/** CrlRobotのインスタンス */
extern CrlRobot crl;

#endif

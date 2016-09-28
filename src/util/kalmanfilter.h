
/**
 * @file kalmanfilter.h
 * @brief
 * カルマンフィルタによって加速度センサとジャイロセンサから姿勢情報を推定する.
 */
class KalmanFilter {
 private:
  void add(float* A, float* B, int m, int n, float* C);
  void subtract(float* A, float* B, int m, int n, float* C);
  void multiply(float* A, float* B, int m, int p, int n, float* C);
  void multiply3(float* A, float* B, float* C, int m, int p, int r, int n,
                 float* D);
  void transpose(float* A, int m, int n, float* C);
  void inverse2d(float* A, float* AInverse);
  /** ループ間隔 初期値は0.01秒 */
  float dt;
  /** 事前状態推定値 */
  float priori_state[3];
  /** 事後状態推定値 */
  float posteriori_state[3];
  /** 事前推定量の分散共分散行列 */
  float priori_covariance[3][3];
  /** 事後推定量の分散共分散行列 */
  float posteriori_covariance[3][3];
  /** 転移行列 */
  float F[3][3];
  /** 転移行列の転置行列 */
  float FT[3][3];
  /** 観測行列 */
  float H[2][3];
  /** 観測行列の転置行列 */
  float HT[3][2];
  /** プロセスノイズの分散 */
  float q1, q2, q3;
  /** 観測ノイズの分散 */
  float r1, r2;
  /** 観測予測誤差の分散共分散行列 */
  float S[2][2];
  /** 観測予測誤差の分散共分散行列の転置行列 */
  float S_inverse[2][2];
  /** カルマンゲイン */
  float K[3][2];
  /** カルマンゲインの転置行列 */
  float KT[2][3];
  /** 推定観測値 */
  float estimated_observation[2];
  /** 観測値と推定観測値の誤差 */
  float e[2];
  /** 状態量の修正量 */
  float state_modify[3];
  /** 推定量の分散共分散行列の修正量 */
  float covariance_modify[3][3];

 public:
  KalmanFilter();
  /**
* @brief 状態量の更新
*
* 加速度センサから算出した角度とジャイロセンサから取得された値を用いて状態量(角度,角速度,角速度バイアス)を更新する.
* @param theta 加速度センサから算出した角度
* @param gyro ジャイロセンサの値
* @return なし
* @warning 時間間隔dtごとに呼び出して下さい。
*/
  void update(float theta, float gyro);

  /**
 * @brief カルマンフィルタによって推定された角度を取得する
 * @return カルマンフィルタによって推定された角度
 * @attention update()を呼び出さない限り,情報は更新されません
 * @sa update()
 */
  float getTheta();
  /**
 *@brief カルマンフィルタによって推定された角度の推定値の分散を取得する
 *@return カルマンフィルタによって推定された角度の推定値の分散
 *@attention update()を呼び出さない限り,情報は更新されません
 *@sa update()
 */
  float getThetaVariance();
  /**
 * @brief カルマンフィルタによって推定された角速度を取得する
 * @return カルマンフィルタによって推定された角速度
 * @attention update()を呼び出さない限り,情報は更新されません
 * @sa update()
 */
  void getThetaDot(float);
  /**
 * @brief ループ間隔を設定する
 * @return なし
 */
  float setDt();
};

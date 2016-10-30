
/**
 * @file kalmanfilter.h
 * @brief
 * カルマンフィルタによって加速度センサとジャイロセンサから姿勢情報を推定する.
 */
class KalmanFilter {
 private:
  /**
   * @brief 行列の和を計算する
   * @param A 入力行列
   * @param B 入力行列
   * @param m 入力行列の列数
   * @param n 入力行列の行数
   * @param C 出力行列 (A + B)
   * @return なし
   */
  void add(float* A, float* B, int m, int n, float* C);
  /**
   * @brief 行列の差を計算する
   * @param A 入力行列
   * @param B 入力行列
   * @param m 入力行列の列数
  * @param n 入力行列の行数
   * @param C 出力行列 (A - B)
   * @return なし
   */
  void subtract(float* A, float* B, int m, int n, float* C);
  /**
   * @brief 行列の積を計算する
   * @param A 入力行列
   * @param B 入力行列
   * @param m 入力行列Aの列数
   * @param p 入力行列Aの列数, 入力行列Bの行数
   * @param n 入力行列Bの列数
   * @param C 出力行列 (A * B)
   * @return なし
   */
  void multiply(float* A, float* B, int m, int p, int n, float* C);
  /**
   * @brief 3つの行列の積を計算する
   * @param A 入力行列
   * @param B 入力行列
   * @param C 入力行列
   * @param m 入力行列Aの列数
   * @param p 入力行列Aの列数, 入力行列Bの行数
   * @param r 入力行列Bの列数, 入力行列Cの行数
   * @param n 入力行列Cの列数
   * @param D 出力行列 (A * B * C)
   * @return なし
   */
  void multiply3(float* A, float* B, float* C, int m, int p, int r, int n, float* D);
  /**
   * @brief 転置行列を計算する
   * @param A 入力行列
   * @param m 入力行列の列数
   * @param n 入力行列の行数
   * @param B 出力行列 A^T
   * @return なし
   */
  void transpose(float* A, int m, int n, float* B);
  /**
   * @brief 2行2列の行列の逆行列を計算する
   * @param A 2行2列入力行列
   * @param B 出力行列 A^{-1}
   * @return なし
   */
  void inverse2d(float* A, float* B);
  /** ループ間隔 初期値は0.01秒 */
  float dt;
  /** 事前状態推定値 */
  float priori_state[2];
  /** 事後状態推定値 */
  float posteriori_state[2];
  /** 事前推定量の分散共分散行列 */
  float priori_covariance[2][2];
  /** 事後推定量の分散共分散行列 */
  float posteriori_covariance[2][2];
  /** 転移行列 */
  float F[2][2];
  /** 転移行列の転置行列 */
  float FT[2][2];
  /** 観測行列 */
  float H[2][2];
  /** 観測行列の転置行列 */
  float HT[2][2];
  /** プロセスノイズの分散 */
  float q1, q2;
  /** 観測ノイズの分散 */
  float r1, r2;
  /** 観測予測誤差の分散共分散行列 */
  float S[2][2];
  /** 観測予測誤差の分散共分散行列の転置行列 */
  float S_inverse[2][2];
  /** カルマンゲイン */
  float K[2][2];
  /** カルマンゲインの転置行列 */
  float KT[2][2];
  /** 推定観測値 */
  float estimated_observation[2];
  /** 観測値と推定観測値の誤差 */
  float e[2];
  /** 状態量の修正量 */
  float state_modify[2];
  /** 推定量の分散共分散行列の修正量 */
  float covariance_modify[2][2];

 public:
  KalmanFilter();
  /**
   * @brief 状態量の更新
   *
   * 加速度センサから算出した角度とジャイロセンサから取得された値を用いて状態量(角度,角速度,角速度バイアス)を更新する.
   * @param theta 加速度センサから算出した角度 [rad]
   * @param gyro ジャイロセンサの値 [rad/s]
   * @param gyro ジャイロセンサのオフセット値 [rad/s]
   * @return なし
   * @warning 時間間隔dtごとに呼び出して下さい。
   */
  void update(float theta, float gyro, float gyro_offset);
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
  float getThetaDot();
  /**
   * @brief ループ間隔を設定する
   * @return なし
   */
  void setDt(float);
};

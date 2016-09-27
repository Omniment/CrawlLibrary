
class KalmanFilter {
 public:
  KalmanFilter();

  void update(float theta, float gyro);
  void add(float *A, float *B, int m, int n, float *C);
  void subtract(float *A, float *B, int m, int n, float *C);
  void multiply(float *A, float *B, int m, int p, int n, float *C);
  void multiply3(float *A, float *B, float *C, int m, int p, int r, int n,
                 float *D);
  void transpose(float *A, int m, int n, float *C);
  void inverse2d(float *A, float *AInverse);
  float getTheta();
  float getThetaDot();
  float getThetaVariance();

  float dt;
  float priori_state[3];
  float posteriori_state[3];

  float priori_covariance[3][3];
  float posteriori_covariance[3][3];

  float F[3][3];
  float H[2][3];

  float FT[3][3];
  float HT[3][2];

  float q1, q2, q3;
  float r1, r2;

  float S[2][2];
  float S_inverse[2][2];
  float K[3][2];
  float KT[2][3];
  float e[2];
  float estimated_observation[2];
  float state_modify[3];
  float covariance_modify[3][3];
};

#include "kalmanfilter.h"

KalmanFilter::KalmanFilter() {
  this->dt = 0.01;
  this->posteriori_state[0] = 0;
  this->posteriori_state[1] = 0;
  this->F[0][0] = 1;
  this->F[0][1] = this->dt;

  this->F[1][0] = 0;
  this->F[1][1] = 1;

  this->transpose((float*)this->F, 2, 2, (float*)this->FT);

  this->H[0][0] = 1;
  this->H[0][1] = 0;

  this->H[1][0] = 0;
  this->H[1][1] = 1;

  this->transpose((float*)this->H, 2, 2, (float*)this->HT);

  this->posteriori_covariance[0][0] = 1;
  this->posteriori_covariance[0][1] = 0;

  this->posteriori_covariance[1][0] = 0;
  this->posteriori_covariance[1][1] = 1;

  this->q1 = 0.0001;
  this->q2 = 0.001;

  this->r1 = 1;
  this->r2 = 1;
}

void KalmanFilter::add(float* A, float* B, int m, int n, float* C) {
  int i, j;
  for (i = 0; i < m; i++)
    for (j = 0; j < n; j++) C[n * i + j] = A[n * i + j] + B[n * i + j];
}
void KalmanFilter::subtract(float* A, float* B, int m, int n, float* C) {
  int i, j;
  for (i = 0; i < m; i++)
    for (j = 0; j < n; j++) C[n * i + j] = A[n * i + j] - B[n * i + j];
}
void KalmanFilter::multiply(float* A, float* B, int m, int p, int n, float* C) {
  int i, j, k;
  for (i = 0; i < m; i++)
    for (j = 0; j < n; j++) {
      C[n * i + j] = 0;
      for (k = 0; k < p; k++) C[n * i + j] = C[n * i + j] + A[p * i + k] * B[n * k + j];
    }
}
void KalmanFilter::multiply3(float* A, float* B, float* C, int m, int p, int r, int n, float* D) {
  float AB[m][r];
  multiply((float*)A, (float*)B, m, p, r, (float*)AB);
  multiply((float*)AB, (float*)C, m, r, n, (float*)D);
}
void KalmanFilter::transpose(float* A, int m, int n, float* B) {
  int i, j;
  for (i = 0; i < m; i++)
    for (j = 0; j < n; j++) B[m * j + i] = A[n * i + j];
}
void KalmanFilter::inverse2d(float* A, float* B) {
  float det = A[0] * A[3] - A[1] * A[2];
  B[0] = A[3] / det;
  B[1] = -A[1] / det;
  B[2] = -A[2] / det;
  B[3] = A[0] / det;
}

void KalmanFilter::update(float theta, float gyro, float gyro_offset) {
  float observation[2];
  observation[0] = theta;
  observation[1] = gyro - gyro_offset;

  // z = F z
  this->priori_state[0] = this->posteriori_state[0] + this->posteriori_state[1] * dt;
  this->priori_state[1] = this->posteriori_state[1];

  // P = F P F.T + Q
  this->multiply3((float*)F, (float*)this->posteriori_covariance, (float*)this->FT, 2, 2, 2, 2,
                  (float*)this->priori_covariance);
  this->priori_covariance[0][0] += this->q1;
  this->priori_covariance[1][1] += this->q2;

  // S = H P H.T + R
  this->multiply3((float*)H, (float*)this->priori_covariance, (float*)this->HT, 2, 2, 2, 2, (float*)this->S);
  this->S[0][0] += this->r1;
  this->S[1][1] += this->r2;

  // K = P H.T S^{-1}
  this->inverse2d((float*)this->S, (float*)this->S_inverse);
  this->multiply3((float*)this->priori_covariance, (float*)this->HT, (float*)this->S_inverse, 2, 2, 2, 2,
                  (float*)this->K);

  // e = (x - H z)
  this->estimated_observation[0] = this->priori_state[0];
  this->estimated_observation[1] = this->priori_state[1];
  this->subtract((float*)observation, (float*)this->estimated_observation, 2, 1, (float*)this->e);

  // z = z + K e
  this->multiply((float*)this->K, (float*)this->e, 2, 2, 1, (float*)this->state_modify);
  this->add((float*)this->priori_state, (float*)this->state_modify, 2, 1, (float*)this->posteriori_state);

  // P = P - K  S  K.T
  this->transpose((float*)this->K, 2, 2, (float*)this->KT);
  this->multiply3((float*)this->K, (float*)this->S, (float*)this->KT, 2, 2, 2, 2, (float*)this->covariance_modify);

  this->subtract((float*)this->priori_covariance, (float*)this->covariance_modify, 2, 2,
                 (float*)this->posteriori_covariance);
}

float KalmanFilter::getTheta() { return this->posteriori_state[0]; }

float KalmanFilter::getThetaDot() { return this->posteriori_state[1]; }

float KalmanFilter::getThetaVariance() { return this->posteriori_covariance[0][0]; }

void KalmanFilter::setDt(float dt) { this->dt = dt; }

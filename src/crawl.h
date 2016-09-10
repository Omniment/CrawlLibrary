#ifndef INCLUDED_Crawl_h
#define INCLUDED_Crawl_h
#include <Arduino.h>

class CrlRobot {
 public:
  void init();
  void realtimeLoop();
  void set_dt(float dt);
  float get_theta_x();
  float get_theta_y();
  float get_theta_z();
  float get_acc_x();
  float get_acc_y();
  float get_acc_z();
  float get_theta_dot_x();
  float get_theta_dot_y();
  float get_theta_dot_z();
  float get_encoder_l();
  float get_encoder_r();
  void set_motor_l(float motor_l);
  void set_motor_r(float motor_r);
  void set_encoder_l(float encoder_l);
  void set_encoder_r(float encoder_r);
  void updateState();

#ifdef DEBUG
 private:
#endif
  
  float dt;
  int dt_us;
  float theta;
  float acc_x, acc_y, acc_z;
  float theta_dot_x, theta_dot_y, theta_dot_z;
  float offset_gx, offset_gy, offset_gz;
  float motor_l, motor_r;
  float encoder_l, encoder_r;
  unsigned long t1, t2, tt;
  float kEtoMM;
  float rate_theta;

  void calcTheta();
  void makeTiming();
  void calcState();
  void initGyroOffset();
  void initTheta();
};

extern CrlRobot crl;

#endif

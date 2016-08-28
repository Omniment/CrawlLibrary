#ifndef INCLUDED_encoder_h
#define INCLUDED_encoder_h

//
// encoder.h
//
// 概要
//
// モータ軸に取り付けられたエンコーダにより，
// モータ軸の累計回転数を取得するライブラリ．
// エンコーダはモータ制御基板に設置されている．
// メイン基板とモータ制御基板は分離しており，シリアル通信（Ｉ２Ｃ）によりコマンドの送受信を行う．
// モータ制御基板のＩ２Ｃアドレスは0x0fに設定されている．
//
// void getRestEncoder();
// モータ軸累計回転数を取得する．
// 累計回転数はグローバル変数right_encoder，left_encoderに格納される．
// 前進していれば正の数，後退していれば負の数となる．
// その後,累計回転数をゼロに戻る．
//
// void getEncoder();
// モータ軸累計回転数を取得する．
// 累計回転数はグローバル変数right_encoder，left_encoderに格納される．
// 前進していれば正の数，後退していれば負の数となる．
//
// void resetEncoder();
// 累計回転数をゼロに戻す．

void getResetEncoder();
void getEncoder();
void resetEncoder();

extern short int right_encoder;
extern short int left_encoder;

#endif

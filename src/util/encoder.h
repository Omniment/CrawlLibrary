/**
 * @file encoder.h
 * @brief
 *  モータ軸に取り付けられたエンコーダにより，
 * モータ軸の累計回転数を取得するライブラリ．
 *
 * エンコーダはモータ制御基板に設置されている．
 * メイン基板とモータ制御基板は分離しており，シリアル通信（Ｉ２Ｃ）によりコマンドの送受信を行う．
 * モータ制御基板のＩ２Ｃアドレスは0x0fに設定されている．
 */
#ifndef INCLUDED_encoder_h
#define INCLUDED_encoder_h
/**
 * @brief  モータ軸累計回転数を取得し,累計回転数をゼロに戻す
 *
 * 累計回転数はグローバル変数right_encoder，left_encoderに格納される．
 * 前進していれば正の数，後退していれば負の数となる．
 * その後,累計回転数をゼロに戻す．
 * @return なし
 */
void getResetEncoder();
/**
 * @brief  モータ軸累計回転数を取得する
 *
 * 累計回転数はグローバル変数right_encoder，left_encoderに格納される．
 * 前進していれば正の数，後退していれば負の数となる．
 * @return なし
 */
void getEncoder();
/**
 * @brief  累計回転数をゼロに戻す
 *
 * @return なし
 */
void resetEncoder();
/** 左累計回転数 */
extern short int left_encoder;
/** 右累計回転数 */
extern short int right_encoder;
#endif

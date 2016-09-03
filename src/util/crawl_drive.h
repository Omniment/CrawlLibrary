#ifndef INCLUDED_crawl_drive_h
#define INCLUDED_crawl_drive_h

//
// motor.h
//
// 概要
//
// ロボットに搭載された左右のモータを制御ための関数群である．
// メイン基板とモータ制御基板は分離しており，シリアル通信（Ｉ２Ｃ）によりコマンドの送受信を行う．
// モータ制御基板のＩ２Ｃアドレスは0x0fに設定されている．
//
// void initMotor();
// モータ機能の初期化関数．
// モータを使用する前に一度呼び出す．
//
// void stopMotor() ;
// モータを停止させる.
//
// void setMoterPower(int left_power,int right_power);
// 左右のモータのパワーを指定する
//
// 引数
//
// left_power, right_power
//
//    整数で-255〜0〜255の範囲で指定する．
//    -255ではモータは最大パワーで逆回転（後退）する．
//    0ではモータは停止する．
//    255ではモータは最大パワーで正回転（前進）する．
//
//  備考
//
//     引数の値が-255より小さく，255より大きく指定された場合，
//     それぞれ-255と255に設定される
//

void initMotor();
void stopMotor();
void setMoterPower(int left_power, int right_power);

#endif

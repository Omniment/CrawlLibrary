/*********************************************
   Crawl Sample Program
   C,C++記述
   LED1 点滅プログラム

   クロールのLED1を1秒ごとに点滅させるプログラムです

   Written by RuiTamura
   2016 Omniment Inc.
*********************************************/

#include <crawl.h>  //クロールライブラリを読み込みます

int LED1 = 13;  // D13に接続されているLED1をわかりやすく定義します

int main() {
  crl.init();  //クロールの初期設定を行います crlはCrawlの略称です
  pinMode(LED1, OUTPUT);  // LED1(D13)の使い方を"出力(OUTPUT)"に設定します

  while (1) {               //プログラムはこの中でループします
    digitalWrite(LED1, 1);  // LED1を点灯させます
    delay(1000);            // 1秒(1000mSec)待ちます
    digitalWrite(LED1, 0);  // LED1を消灯させます
    delay(1000);            // 1秒待ちます
  }
}

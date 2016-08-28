/*********************************************
   Crawl Sample Program
   Arduino記述
   LED1 LED2 点滅プログラム

   クロールのLED1を1秒ごとに点滅
   LED2を少しずつ明るくして、少しずつ暗くしていくプログラムです

   Written by RuiTamura
   2016 Omniment Inc.
*********************************************/

#include <Crawl.h>

int LED1 = 13;                //D13に接続されているLED1をわかりやすく定義します
int LED2 = 9;                 //D9に接続されているLED2をわかりやすく定義します

int bright = 0;               //LED2の明るさを記憶する変数を作ります

void setup() {
  crl.init();
  pinMode(LED1, OUTPUT);      //LED1(D13)の使い方を"出力(OUTPUT)"に設定します
  pinMode(LED2, OUTPUT);      //LED2(D9)の使い方を"出力(OUTPUT)"に設定します

}

void loop() {
  digitalWrite(LED1, 1);      //LED1を点灯させます
  //for文を使って
  //brightを0から初めて、255を超えるまで、brightに1ずつ足していくようにします
  for (bright = 0; bright < 255; bright = bright + 1) {
    analogWrite(LED2, bright); //LED2をbrightの明るさで点灯させます
    delay(5);              //少し待ちます
  }

  digitalWrite(LED1, 0);      //LED1を消灯させます
  //for文を使って
  //brightを255から初めて、0になるまで、brightに1ずつ引いていくようにします
  for (bright = 255; bright > 0; bright = bright - 1) {
    analogWrite(LED2, bright); //LED2をbrightの明るさで点灯させます
    delay(5);              //少し待ちます
  }
}


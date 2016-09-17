/**
 * @file crawl_drive.h
 * @brief
 * ロボットに搭載された左右のモータを制御ための関数群
 *
 * メイン基板とモータ制御基板は分離しており，シリアル通信（Ｉ２Ｃ）によりコマンドの送受信を行う．
 * モータ制御基板のＩ２Ｃアドレスは0x0fに設定されている．
 */
#ifndef INCLUDED_crawl_drive_h
#define INCLUDED_crawl_drive_h

/**
 * @brief モータ機能の初期化
 *
 * モータを使用する前に必ず呼び出してください.
 * @return なし
 */
void initMotor();
/**
 * @brief モータ機能の無効化
 *
 * 無効化の状態ではモータのパワーを指定しても動作しません.
 * 有効化する場合はinitMotor()を呼び出す必要があります.
 * @return なし
 * @sa initMotor()
 */
void stopMotor();
/**
 * @brief 左右のモータのパワーを指定する
 *
 * @param left_power 左のモータのパワー
 * @param right_power 右のモータのパワー
 *
 * 整数で-255〜0〜255の範囲で指定する．
 * -255ではモータは最大パワーで逆回転（後退）する．
 * 0ではモータは停止する．
 * 255ではモータは最大パワーで正回転（前進）する．
 * @return なし
 * @note
 * 引数の値が-255より小さく，255より大きく指定された場合，
 * それぞれ-255と255に設定される
 */
void setMoterPower(int left_power, int right_power);

#endif

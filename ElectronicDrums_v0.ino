// Mozziライブラリを使用しています
// 必要なファイルをインクルード
#include <MozziGuts.h>
#include <Sample.h>
#include <samples/snare.h>  // 音源のファイル(今回はスネアドラム)
Sample <snare_NUM_CELLS, AUDIO_RATE> snare(snare_DATA);

#define CONTROL_RATE 64

// 入力を受け取るピン
const int analogInPin0 = A0;  // 圧電素子につなぐ
const int analogInPin1 = A1;  // 可変抵抗につなぐ

// 出力はD9ピン(Mozziライブラリは出力ピンが固定)

// 音源のピッチに関するパラメータ
const float BasicFrequency = (float) snare_SAMPLERATE / (float) snare_NUM_CELLS;  // 音源を再生するサンプリング周波数の係数
float ratio;  // ゲインの比率(ratiomin~ratiomax[%])
int ratiomax = 125;  // ゲインの比率[%]の最大値
int ratiomin = 75;  // ゲインの比率[%]の最小値
float pitch;  // 最終的なサンプリング周波数(BasicFrequency*ratio)
int pitchSet;  // 入力信号の読み取りにおいて、pitchSet回に1回pitchを更新する

// 音源のゲイン(0-255)に関するパラメータ
const int cutoff = 30;  // (素子の入力値)<cutoff であればノイズとみなしカットする
const int threshold = 30;  // (素子の入力値)=threshold であれば最小ゲインで音源を再生する
const int cap = 150;  // (素子の入力値)=cap であれば最大ゲインで音源を再生する
const int ampmax = 255;  // ゲインの最大値
const int ampmin = 30;  // ゲインの最小値
byte gain = 255;  // 最終的なゲイン

// 素子の値に関するパラメータ
int sensorValue_past;  // 一つ前の素子の値(ノイズカットしたもの)
int sensorValue_past_modified;  // 一つ前の素子の値を修正したもの
int sensorValue_present;  // 現在の素子の値
int sensorValue_present_modified;  // 現在の素子の値を修正したもの(ノイズカットしたもの)
int delta_present; // (現在の素子の値)-(一つ前の素子の値)
int delta_past;  // (一つ前の素子の値)-(二つ前の素子の値)
int delta_trigger;  // |素子の値の差分|>delta_trigger であれば音源を流す

// 初期設定
void setup() {
  startMozzi(CONTROL_RATE);
  snare.setFreq(BasicFrequency);
  snare.start();
  sensorValue_past = 0;
  delta_past = 0;
  pitchSet = 0;
}

// 繰り返し処理
void updateControl(){

  // 圧電素子から信号の読み取り＋修正
  sensorValue_present = analogRead(analogInPin0);  // 圧電素子の信号を読み取る
  if (sensorValue_present < cutoff){  // 値がcutoff以下であれば0に修正する
    sensorValue_present_modified = 0;
  }else{
    sensorValue_present_modified = sensorValue_present;
  }

  // 音源のサンプリング周波数の設定
  pitchSet++;
  if (pitchSet == 20){
    ratio = map(mozziAnalogRead(analogInPin1),0,255,ratiomin,ratiomax) / 100.0;
    pitch = BasicFrequency * ratio;
    snare.setFreq(pitch);
    pitchSet = 0;
  }

  // 信号の差分を計算+トリガー判定(改善の余地大)
  delta_present = sensorValue_present_modified - sensorValue_past;  // 現在の値-ひとつ前の値
  if (delta_past > delta_trigger && delta_present < -delta_trigger){  // (現在の値-ひとつ前の値)<-(delta_trigger) かつ (ひとつ前の値-ふたつ前の値)>(delta_trigger) なら音源を流す
    if (sensorValue_past >= threshold){
          if (sensorValue_past >= cap){  // 素子の値(gainを決めるもの)がcap以上であればcapに修正する
            sensorValue_past_modified = cap;
          }else{
            sensorValue_past_modified = sensorValue_past;
          }
          gain = map(sensorValue_past_modified, threshold, cap, ampmin, ampmax);
          snare.start();
          //Serial.print("hit");
    }
  }

  // 信号の値の受け継ぎ(現在の値を一つ前の値とする)
  sensorValue_past = sensorValue_present_modified;
  delta_past = delta_present;
}

// 再生用の関数
int updateAudio(){
  return (snare.next()*gain) >> 8;
}

// 再生用の関数
void loop(){
  audioHook();
}

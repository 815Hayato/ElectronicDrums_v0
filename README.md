# ElectronicDrums_v0
## 使用技術
Arduino(C/C++),電子回路

## 制作時期
2023冬制作→2024夏修正

## 概要
電子ドラム(スネアのみ)

## 使用例
ElectronicDrums_v0.mp4で実際に使用した様子を見ることができます

## 特徴
1. ドラムを叩いてかた音が鳴るまでのレイテンシが小さい(Arduinoの繰り返し処理が比較的単純だから)
2. ドラムを叩く強さに応じて音源のゲインが変わる(加えてアンプ上のつまみでメインボリュームを変更できる)
3. 音源のピッチをリアルタイムで変えることができる(つまみによって音源のサンプリング周波数を変える)
4. あるサイズ以下であれば任意の音源を再生できる(後述)

## 使い方
1. Arduino・Mozziライブラリのインストール
2. Arduino/libraries/Mozzi/samplesフォルダに音源データ(snare.h)を入れる
3. ElectronicDrums_v0_breadboardの通りに電子回路を組む(アンプはLM386,ArduinoはNanoを使用しました)
4. ElectronicDrums_v0.inoをArduinoに書き込む
5. 圧電素子を叩くと音源が再生されます
6. 圧電素子をドラムに装着する(今回粉ふるい機で代用)
7. ドラム(粉ふるい機)を叩くと音源が再生されます

## 音源の前処理について(Audacity使用)
1. Audacityに使いたい音源をインポートする
2. ヘッダ「Raw(header-less)」、エンコーディング「Signed 8 bit PCM」の状態で書き出す
3. 書き出したファイルをArduino/libraries/Mozzi/extras/pythonに入れる
4. ターミナルで上記ディレクトリに移動し、python char2mozzi.py (ファイル名.raw) (ファイル名.h) (ファイル名) (サンプリング周波数) と書いて実行
5. ファイル名.hが生成されるので、それをArduino/libraries/Mozzi/samplesに入れる

## 改善点・今後の展望
1. トリガー(圧電素子の値から音源の再生のタイミングを決定する仕組み)の改善：圧電素子の値がドラム素材の伝達特性の影響下にあることを踏まえて、その逆フィルタをArduinoで実装する？
2. ピッチ変化の仕組みを工夫する：距離センサを使ってテルミンのようにピッチを変える、フットペダルを使ってティンパニのようにピッチを変える、など
3. アンプの改善：音源が歪んで再生される場合があるので、原因を探って修正する
4. 使用音源の制限の緩和：あるデータサイズ(小さい)以下の音源を使用できないので、その制約を失くす(DFPlayerの利用?)
5. 叩き分け(ヘッドかリムか)を判定できるようにして音源にその差異を反映する
6. 同じドラムを二つ用意して電子ボンゴ・コンガを作ってみる、四つ用意して電子ティンパニを作ってみる、もっと用意して電子ドラムを作ってみる

## 参照
『Arduinoではじめる手作り電子楽器』(中西宣人)  
音源の再生方法・ドラム素材の代用方法について参考にしました

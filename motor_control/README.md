## Mortor controller
### Initialize procedure
1. Run init.sh  to load motor module
2. 

### エンディアンについて

iMCs01はリトルエンディアンを使用している。プロセッサ側が異なるエンディアンの場合はバイトを入れ替える必要があるが、今回使用するRaspberry Piは基本的にリトルエンディアンで動作するためバイト入れ替えの必要はない。
可搬性は低下するがバイトオーダーを無視した実装をしてもかまわない。


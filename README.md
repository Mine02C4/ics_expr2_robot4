# ics_expr2_robot4

## 必要なライブラリ

- OpenCV 3.1
- Julius 4.2.3

## Raspberry Pi実機でのコンパイル

いろいろ実機で設定してるので

```
make
```

のみ

## テスト環境

### Windows

http://opencv.org/downloads.html の"OpenCV 3.1 for Windows"をダウンロード。展開した中のbuildフォルダの中身を `/opencv` に配置。

```
- /
-- opencv/
--- bin/
--- etc/
--- include/
...
```

のようなディレクトリツリーになる。この状態で直下の core.sln をVisual Studio Community 2015で開くとビルドできる。

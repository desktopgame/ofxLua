ofxLua
=====================================

ofxLuaは、openFrameworksからLUAを呼び出すための簡単なラッパーです。
既に[同名のリポジトリ](https://github.com/danomatika/ofxLua)が存在しますが、こちらは少し使い方が違うのと、Windowsに**のみ**対応しています。

Introduction
------------
luaの簡単なラッパーです。
C++側からはグローバル変数の設定/読み込みと関数の定義などが出来ます。
また、luaがCスタイルの関数ポインタしか受け入れないことによる問題をコンテキストクラスで解決しています。

License
-------
MIT License

Installation
------------
このフォルダを`openFrameworks/addons/`に置いてください。
そのあと、`このアドオン/scripts/init.bat` を実行してください。
これによってnugetからluaが適切な場所にダウンロードされます。

Dependencies
------------
このアドオンを使用するのに必要なほかのアドオンはありません。

Compatibility
------------
`of_v0.10.1_vs2017_release`でのみ動作を確認しています。
また、Windows10以外のOSではチェックしていません。

Known issues
------------
現在のところ、既知の不具合はありません。

Version history
------------

### Version 0.1 (2020/01/26):
最初のリリースです。


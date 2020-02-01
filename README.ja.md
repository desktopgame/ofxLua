# ofxLua

ofxLua は、openFrameworks から LUA を呼び出すための簡単なラッパーです。  
既に[同名のリポジトリ](https://github.com/danomatika/ofxLua)が存在しますが、こちらは少し使い方が違います。

## Introduction

lua の簡単なラッパーです。  
C++側からはグローバル変数の設定/読み込みと関数の定義などが出来ます。  
また、lua が C スタイルの関数ポインタしか受け入れないことによる問題をコンテキストクラスで解決しています。

以下は簡単なサンプルです。

```
#include "ofApp.h"
#include "ofxLua.h"
#include <iostream>

#include <lua.hpp>
#include <lauxlib.h>
#include <lualib.h>

int toNum(lua_State* state) {
	std::string str = lua_tostring(state, -1);
	lua_pushinteger(state, std::atoi(str.c_str()));
	return 1;
}

int seq(lua_State* state) {
	ofxLua::Context::Instance ctx = ofxLua::Context::top();
	std::cout << ctx->get<const char*>("str") << std::endl;
	if (!ctx->contains("i")) {
		ctx->set("i", 0);
		lua_pushinteger(state, 0);
	}
	else {
		int i = ctx->get<int>("i");
		ctx->set("i", i + 1);
		lua_pushinteger(state, i + 1);
	}
	return 1;
}

//--------------------------------------------------------------
void ofApp::setup() {
	ofxLua::Interpreter lua;
	ofxLua::Context::Instance ctx = ofxLua::Context::push();
	lua.loadString(
		R"(
	function hello()
		print("hello ofxLua!")
	end

	function printCppVariable()
		print(cppVariable);
	end

	function callCppFunction()
		print(toNum("100") + 11);
	end

	function callCppFunction2()
		print(seq());
		print(seq());
	end
)");
	lua.call("hello", std::vector<ofxLua::Object>{}, std::vector<ofxLua::Type>{}).check();
	// define variable
	lua.setGlobalString("cppVariable", "Cpp String").check();
	lua.call("printCppVariable", std::vector<ofxLua::Object>{}, std::vector<ofxLua::Type>{}).check();
	// define function
	lua.define("toNum", toNum);
	lua.call("callCppFunction", std::vector<ofxLua::Object>{}, std::vector<ofxLua::Type>{}).check();
	// use Context
	ctx->set("str", "FirstString");
	lua.define("seq", seq);
	lua.call("callCppFunction2", std::vector<ofxLua::Object>{}, std::vector<ofxLua::Type>{}).check();

	ctx->set("str", "SecondString");
	lua.call("callCppFunction2", std::vector<ofxLua::Object>{}, std::vector<ofxLua::Type>{}).check();

	ofxLua::Context::pop();
}

//--------------------------------------------------------------
void ofApp::update() {

}

//--------------------------------------------------------------
void ofApp::draw() {

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key) {

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h) {

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg) {

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo) {

}
```

実行結果

```
hello ofxLua!
Cpp String
111
FirstString
0
FirstString
1
SecondString
2
SecondString
3
```

## License

MIT License

## Installation

### WindowsOS の場合

このフォルダを`openFrameworks/addons/`に置いてください。  
そのあと、`このアドオン/scripts/init.bat` を実行してください。  
これによって nuget から lua が適切な場所にダウンロードされます。

### macOS の場合

このフォルダを`openFrameworks/addons/`に置いてください。  
また、事前に lua の導入が必要です。

```
brew install lua
```

## Dependencies

このアドオンを使用するのに必要なほかのアドオンはありません。

## Compatibility

`of_v0.10.1_vs2017_release`でのみ動作を確認しています。
サポートされているプラットフォーム

- Windows10
- macos-mojave

## Known issues

現在のところ、既知の不具合はありません。

## Version history

### Version 0.1 (2020/01/26):

最初のリリースです。

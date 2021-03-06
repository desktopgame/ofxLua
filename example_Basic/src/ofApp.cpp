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
	} else {
		int i = ctx->get<int>("i");
		ctx->set("i", i + 1);
		lua_pushinteger(state, i+1);
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

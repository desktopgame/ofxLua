#include "ofxLua.h"

#include <lua.hpp>
#include <lauxlib.h>
#include <lualib.h>

namespace ofxLua {
Interpreter::Interpreter() : state(nullptr), occurredError(false), lastError() {
	this->state = luaL_newstate();
	luaL_openlibs(state);
}

Interpreter::~Interpreter() {
	lua_close(state);
}
TResult<Unit, ErrorCode> Interpreter::loadFile(const std::string & file) {
	int status = luaL_dofile(state, file.c_str());
	if (status == LUA_OK) {
		return TResult<Unit, ErrorCode>(Unit());
	} else if (status == LUA_ERRSYNTAX) {
		return TResult<Unit, ErrorCode>(errorAndGetString(), ErrorCode::E_SYNTAX);
	} else if (status == LUA_ERRMEM) {
		return TResult<Unit, ErrorCode>(errorAndGetString(), ErrorCode::E_MEM);
	} else if (status == LUA_ERRGCMM) {
		return TResult<Unit, ErrorCode>(errorAndGetString(), ErrorCode::E_GCMM);
	}
	return TResult<Unit, ErrorCode>(errorAndGetString(), ErrorCode::E_UNKNOWN);
}
TResult<Unit, ErrorCode> Interpreter::loadString(const std::string & str) {
	int status = luaL_loadstring(state, str.c_str());
	if (status == LUA_OK) {
		lua_pcall(state, 0, LUA_MULTRET, 0);
		return TResult<Unit, ErrorCode>(Unit());
	} else if (status == LUA_ERRSYNTAX) {
		return TResult<Unit, ErrorCode>(errorAndGetString(), ErrorCode::E_SYNTAX);
	} else if (status == LUA_ERRMEM) {
		return TResult<Unit, ErrorCode>(errorAndGetString(), ErrorCode::E_MEM);
	} else if (status == LUA_ERRGCMM) {
		return TResult<Unit, ErrorCode>(errorAndGetString(), ErrorCode::E_GCMM);
	}
	return TResult<Unit, ErrorCode>(errorAndGetString(), ErrorCode::E_UNKNOWN);
}
TResult<std::vector<Variant>, Unit> Interpreter::call(const std::string & function, std::vector<Object> args, std::vector<Type> returnType) {
	int type = lua_getglobal(state, function.c_str());
	if (type == LUA_TNIL) {
		return TResult<std::vector<Variant>, Unit>("function is not found", Unit());
	}
	std::vector<Variant> returnOut;
	// push arguments
	for (auto& obj : args) {
		if (obj.type == Type::T_NUMBER) {
			lua_pushnumber(state, obj.value.number);
		} else if (obj.type == Type::T_BOOL) {
			lua_pushboolean(state, obj.value.boolean);
		} else if (obj.type == Type::T_STRING) {
			lua_pushstring(state, obj.value.string.c_str());
		}
	}
	// get return values
	int status = lua_pcall(state, static_cast<int>(args.size()), static_cast<int>(returnType.size()), 0);
	int offset = static_cast<int>(returnType.size());
	for (int i = 0; i < static_cast<int>(returnType.size()); i++) {
		Type ty = returnType.at(i);
		Variant v;
		if (ty == Type::T_NUMBER) {
			v.number = luaL_checknumber(state, -offset);
			returnOut.emplace_back(v);
		} else if (ty == Type::T_BOOL) {
			v.boolean = lua_toboolean(state, -offset);
			returnOut.emplace_back(v);
		} else if (ty == Type::T_STRING) {
			v.string = luaL_checkstring(state, -offset);
			returnOut.emplace_back(v);
		}
		offset--;
		lua_pop(state, 1);
	}
	if (status == LUA_OK) {
		return TResult<std::vector<Variant>, Unit>(returnOut);
	}
	return TResult<std::vector<Variant>, Unit>(errorAndGetString(), Unit());
}
Result Interpreter::define(const std::string & name, Function fn) {
	if (isOccurredError()) {
		return Result("lua interpreter is already crush.", Unit());
	}
	lua_pushcfunction(state, fn);
	lua_setglobal(state, name.c_str());
	return Result(Unit());
}
Result Interpreter::setGlobalNumber(const std::string & name, double value) {
	if (isOccurredError()) {
		return Result("lua interpreter is already crush.", Unit());
	}
	lua_pushnumber(state, value);
	lua_setglobal(state, name.c_str());
	return Result(Unit());
}
Result Interpreter::setGlobalInt(const std::string & name, int value) {
	if (isOccurredError()) {
		return Result("lua interpreter is already crush.", Unit());
	}
	lua_pushinteger(state, value);
	lua_setglobal(state, name.c_str());
	return Result(Unit());
}
Result Interpreter::setGlobalBool(const std::string & name, bool value) {
	if (isOccurredError()) {
		return Result("lua interpreter is already crush.", Unit());
	}
	lua_pushboolean(state, value ? 1 : 0);
	lua_setglobal(state, name.c_str());
	return Result(Unit());
}
Result Interpreter::setGlobalString(const std::string & name, const std::string & value) {
	if (isOccurredError()) {
		return Result("lua interpreter is already crush.", Unit());
	}
	lua_pushstring(state, value.c_str());
	lua_setglobal(state, name.c_str());
	return Result(Unit());
}
TResult<double, Unit> Interpreter::getGlobalNumber(const std::string & name) {
	int type = lua_getglobal(state, name.c_str());
	if (type != LUA_TNUMBER) {
		return TResult<double, Unit>("value is not number", Unit());
	}
	double number = luaL_checknumber(state, -1);
	lua_pop(state, 1);
	return TResult<double, Unit>(number);
}
TResult<int, Unit> Interpreter::getGlobalInt(const std::string & name) {
	int type = lua_getglobal(state, name.c_str());
	if (type != LUA_TNUMBER) {
		return TResult<int, Unit>("value is not number", Unit());
	}
	int integer = static_cast<int>(luaL_checkinteger(state, -1));
	lua_pop(state, 1);
	return TResult<int, Unit>(integer);
}
TResult<bool, Unit> Interpreter::getGlobalBool(const std::string & name) {
	int type = lua_getglobal(state, name.c_str());
	if (type != LUA_TBOOLEAN) {
		return TResult<bool, Unit>("value is not boolean", Unit());
	}
	bool boolean = lua_toboolean(state, -1) == 1;
	lua_pop(state, 1);
	return TResult<bool, Unit>(boolean);
}
TResult<std::string, Unit> Interpreter::getGlobalString(const std::string & name) {
	int type = lua_getglobal(state, name.c_str());
	if (type != LUA_TSTRING) {
		return TResult<std::string, Unit>("value is not string", Unit());
	}
	std::string str = luaL_checkstring(state, -1);
	lua_pop(state, 1);
	return TResult<std::string, Unit>(str);
}

TResult<std::unordered_map<std::string, Object>, Unit> Interpreter::getAllVariables() const {
	std::unordered_map<std::string, Object> ret;
	lua_pushglobaltable(state);
	lua_pushnil(state);
	while (lua_next(state, -2) != 0) {
		std::string key = lua_tostring(state, -2);
		int type = lua_getglobal(state, key.c_str());
		if (type == LUA_TNUMBER) {
			ret.insert_or_assign(key,
				create(lua_tonumber(state, -1)));
		} else if (type == LUA_TBOOLEAN) {
			ret.insert_or_assign(
				key, create(lua_toboolean(state, -1) == 1));
		} else if (type == LUA_TSTRING) {
			ret.insert_or_assign(key, create(std::string(lua_tostring(state, -1))));
		}
		lua_pop(state, 1);
		lua_pop(state, 1);
	}
	lua_pop(state, 1);
	return TResult<std::unordered_map<std::string, Object>, Unit>(ret);
}
bool Interpreter::isOccurredError() const {
	return this->occurredError;
}
std::string Interpreter::getLastError() const {
	return this->lastError;
}
// private
std::string Interpreter::errorAndGetString() {
	std::string err = lua_tostring(state, -1);
	lua_pop(state, 1);
	this->occurredError = true;
	this->lastError = err;
	return err;
}
//
// Function
//
Object::Object() {}
Object create(int i) {
	Object obj;
	obj.type = Type::T_NUMBER;
	obj.value.number = i;
	return obj;
}
Object create(double d) {
	Object obj;
	obj.type = Type::T_NUMBER;
	obj.value.number = d;
	return obj;
}
Object create(bool b) {
	Object obj;
	obj.type = Type::T_BOOL;
	obj.value.boolean = b;
	return obj;
}
Object create(const std::string& str) {
	Object obj;
	obj.type = Type::T_STRING;
	obj.value.string = str;
	return obj;
}
}
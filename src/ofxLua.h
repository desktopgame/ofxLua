#pragma once
#ifndef OFXLUA_H
#define OFXLUA_H
#include <string>
#include <vector>
#include <unordered_map>
#include <stdexcept>
#include "Context.h"
#include "Result.h"

struct lua_State;

namespace ofxLua {
/**
 * Type is lua object type.
 */
using Type = enum {
	T_BOOL = 0,
	// T_LIGHT_USERDATA,
	// T_NIL,
	// T_NONE,
	T_NUMBER,
	T_STRING,
	// T_TABLE,
	// T_THREAD,
	T_USERDATA
};
/**
 * ErrorCode is lua error code.
 */
using ErrorCode = enum {
	E_SYNTAX = 0,
	E_MEM,
	E_GCMM,
	E_UNKNOWN,
};

/**
 * Variant.
 */
struct Variant {
	double number;
	bool boolean;
	std::string string;
};
/**
 * Object.
 */
struct Object {
	explicit Object();
	Type type;
	Variant value;
};
using Function = int(struct lua_State*);
/**
 * Interpreter.
 */
class Interpreter {
public:
	explicit Interpreter();
	~Interpreter();

	TResult<Unit, ErrorCode> loadFile(const std::string& file);

	TResult<Unit, ErrorCode> loadString(const std::string& str);

	TResult<std::vector<Variant>, Unit> call(const std::string& function, std::vector<Object> args, std::vector<Type> returnType);

	Result define(const std::string& name, Function fn);

	Result setGlobalNumber(const std::string& name, double value);

	Result setGlobalInt(const std::string& name, int value);

	Result setGlobalBool(const std::string& name, bool value);

	Result setGlobalString(const std::string& name, const std::string& value);

	TResult<double, Unit> getGlobalNumber(const std::string& name);

	TResult<int, Unit> getGlobalInt(const std::string& name);

	TResult<bool, Unit> getGlobalBool(const std::string& name);

	TResult<std::string, Unit> getGlobalString(const std::string& name);

	TResult<std::unordered_map<std::string, Object>, Unit> getAllVariables() const;

	bool isOccurredError() const;
	std::string getLastError() const;

private:
	std::string errorAndGetString();
	struct lua_State* state;
	bool occurredError;
	std::string lastError;
};


Object create(int i);
Object create(double d);
Object create(bool b);
Object create(const std::string& str);
}
#endif
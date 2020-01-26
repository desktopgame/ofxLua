#pragma once
#ifndef OFXLUA_CONTEXT_H
#define OFXLUA_CONTEXT_H
#include <memory>
#include <string>
#include <stack>
#include <unordered_map>

#include "any/any.hpp"

namespace ofxLua {
/**
 * Context is variable table.
 */
class Context {
public:
	using Instance = std::shared_ptr<Context>;
	using Reference = std::weak_ptr<Context>;

	/**
	 * create new context.
	 * @return
	 */
	static Instance push();

	/**
	 * return a last created context.
	 * @return
	 */
	static Instance top();

	/**
	 * remove a last created context.
	 */
	static void pop();

	/**
	 * create binding.
	 * @param name
	 * @param value
	 */
	template<typename T>
	void set(const std::string& name, T value);

	/**
	 * return a variable from bindings.
	 * lookup by name string.
	 * @param name
	 * @return
	 */
	template<typename T>
	T get(const std::string& name) const;

	/**
	 * return true if variable exists.
	 * @param name
	 * @return
	 */
	bool contains(const std::string& name) const;
private:
	static std::stack<Instance> stack;
	std::unordered_map<std::string, linb::any> map;
	Context();
};

template<typename T>
inline void Context::set(const std::string & name, T value) {
	map.insert_or_assign(name, value);
}

template<typename T>
inline T Context::get(const std::string & name) const {
	return linb::any_cast<T>(map.at(name));
}
}
#endif
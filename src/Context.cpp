#include "Context.h"
#include <stdexcept>

namespace ofxLua {
std::stack<Context::Instance> Context::stack;

Context::Instance Context::push() {
	Instance inst = std::shared_ptr<Context>(new Context());
	stack.push(inst);
	return inst;
}
Context::Instance Context::top() {
	if (stack.empty()) {
		throw std::logic_error("stack is empty");
	}
	return stack.top();
}
void Context::pop() {
	if (stack.empty()) {
		throw std::logic_error("stack is empty");
	}
	stack.pop();
}
bool Context::contains(const std::string & name) const {
	return map.count(name);
}
// private
Context::Context() : map() {
}
}
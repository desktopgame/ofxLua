#pragma once
#ifndef OFXLUA_RESULT_H
#define OFXLUA_RESULT_H
#include <string>

namespace ofxLua {
/**
 * Unit is empty struct.
 */
struct Unit { };

/**
 * Result is failable value.
 */
template<typename T, typename E>
class TResult {
public:
	/**
	 * create result as `success`
	 * @param value
	 */
	explicit TResult(T value);

	/**
	 * create result as `failure`
	 * @param errorMessage
	 * @param errorInfo
	 */
	explicit TResult(std::string& errorMessage, E errorInfo);
	explicit TResult(const char* errorMessage, E errorInfo);

	/**
	 * throw exception if failed
	 */
	void check();

	/**
	 * return a value.
	 * throw exception if failed
	 * @return
	 */
	T get();

	/**
	 * return a value.
	 * however, return default value if failed
	 * @param defaultValue
	 * @return
	 */
	T getOrDefault(T defaultValue) const;

	/**
	 * return true if failed.
	 * @return
	 */
	bool isFailed() const;

	/**
	 * return a error message.
	 * @return
	 */
	std::string getErrorMessage() const;

	/**
	 * return a error information.
	 * @return
	 */
	E getErrorInfo() const;
private:
	T value;
	std::string errorMessage;
	E errorInfo;
	bool failed;
};

using Result = TResult<Unit, Unit>;
template<typename T, typename E>
inline TResult<T, E>::TResult(T value) : value(value), errorMessage(), errorInfo(), failed(false) {
}

template<typename T, typename E>
inline TResult<T, E>::TResult(std::string & errorMessage, E errorInfo) : value(), errorMessage(errorMessage), errorInfo(errorInfo), failed(true) {
}

template<typename T, typename E>
inline TResult<T, E>::TResult(const char * errorMessage, E errorInfo) : value(), errorMessage(errorMessage), errorInfo(errorInfo), failed(true) {
}

template<typename T, typename E>
inline void TResult<T, E>::check() {
	if (failed) {
		throw std::logic_error(errorMessage);
	}
}

template<typename T, typename E>
inline T TResult<T, E>::get() {
	check();
	return value;
}
template<typename T, typename E>
inline T TResult<T, E>::getOrDefault(T defaultValue) const {
	return failed ? defaultValue : value;
}
template<typename T, typename E>
inline bool TResult<T, E>::isFailed() const {
	return failed;
}
template<typename T, typename E>
inline std::string TResult<T, E>::getErrorMessage() const {
	return failed ? errorMessage : "";
}
template<typename T, typename E>
inline E TResult<T, E>::getErrorInfo() const {
	return failed ? : errorInfo : E();
}
}
#endif
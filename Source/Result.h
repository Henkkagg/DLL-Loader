#pragma once

#include <variant>
#include <stdexcept>
#include <string>

struct Error {
	std::string message;
	explicit Error(const std::string& message) : message(message) {}
};

//Result works like std::expected

template <typename T>
class Result {

	std::variant<T, Error> _data;
	const bool _isSuccess;

public:

	Result(const T& value) : _data(value), _isSuccess(true) {}
	Result(const Error& error) : _data(error), _isSuccess(false) {}

	T& operator*() {
		if (!_isSuccess) {

			std::cerr << getError() << std::endl;
			throw std::runtime_error("Can't access data of a failed Result.");
		}
		return std::get<T>(_data);
	}

	const T& operator*() const {
		if (!_isSuccess) {

			std::cerr << getError() << std::endl;
			throw std::runtime_error("Can't access data of a failed Result.");
		}
		return std::get<T>(_data);
	}

	const std::string& getError() const {
		if (_isSuccess) {

			throw std::runtime_error("Can't access error message of a successful Result.");
		}
		return std::get<Error>(_data).message;
	}

	explicit operator bool() const {

		return _isSuccess;
	}
};
#pragma once
#include <exception>

class InvalidOpException : public std::exception
{
public:
	const char* what() const noexcept override { return "Invalid OP code or parameters."; }
};

class InvalidFileException : public std::exception
{
public:
	InvalidFileException(const char* msg) : mExceptionCause(msg) { }
	const char* what() const noexcept override { return mExceptionCause; }

private:
	const char* mExceptionCause;
};
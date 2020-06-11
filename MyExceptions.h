#pragma once
#ifndef MYEXCEPTIONS_H_
#define MYEXCEPTIONS_H_

#include <exception>
#include <string>

class TreeExceptions
{
public:
	TreeExceptions(std::string error) : m_error{ error } {}
	~TreeExceptions() = default;
	const char* what() const noexcept { return m_error.c_str(); }

private:
	std::string m_error;
};

#endif // !MYEXCEPTIONS_H_

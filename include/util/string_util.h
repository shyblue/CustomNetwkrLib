#pragma once

#ifndef UTILITY_STRINGUTIL_H_
#define UTILITY_STRINGUTIL_H_

#include <string>

class StringUtil
{
public:
	StringUtil(void);
	~StringUtil(void);

	static std::string trim_left(const std::string& str);
	static std::string trim_right(const std::string& str);
	static std::string trim(const std::string& str);

};

#endif


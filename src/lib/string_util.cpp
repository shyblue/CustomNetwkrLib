#include "util/string_util.h"

StringUtil::StringUtil(void)
{
}

StringUtil::~StringUtil(void)
{
}

std::string StringUtil::trim_left(const std::string& str)  
{  
	std::string::size_type n = str.find_first_not_of(" \t\v\n");  
	return n == std::string::npos ? str : str.substr(n, str.length());  
}  
  
std::string StringUtil::trim_right(const std::string& str)  
{  
	std::string::size_type n = str.find_last_not_of(" \t\v\n");  
	return n == std::string::npos ? str : str.substr(0, n + 1);  
}  

std::string StringUtil::trim(const std::string& str)
{
	return trim_left(trim_right(str));
}

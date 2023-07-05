#pragma once

#include <vector>
#include <string>

class StringUtil
{
public:
	static std::vector<std::string> split(std::string s, std::string delimiter);
	static char* NewUTFCharString(std::string str);
};


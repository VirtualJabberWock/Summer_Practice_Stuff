#include "StringUtil.h"

std::vector<std::string> StringUtil::split(std::string s, std::string delimiter)
{
    size_t pos_start = 0, pos_end, delim_len = delimiter.length();
    std::string token;
    std::vector<std::string> res;

    while ((pos_end = s.find(delimiter, pos_start)) != std::string::npos) {
        token = s.substr(pos_start, pos_end - pos_start);
        pos_start = pos_end + delim_len;
        res.push_back(token);
    }

    res.push_back(s.substr(pos_start));
    return res;
}

char* StringUtil::NewUTFCharString(std::string str)
{
    char* result = (char*) calloc(str.size() + 1, sizeof(char));
    if (result == 0) {
        exit(-1000);
    }
    for (int i = 0; i < str.size(); i++) {
        result[i] = str[i];
    }
    result[str.size()] = '\0';
    return result;
}

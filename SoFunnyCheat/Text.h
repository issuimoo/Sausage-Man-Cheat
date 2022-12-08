#pragma once
#include <string>
namespace Text
{
	const char* GetNameFromFile(const char* fullpathname);
	std::string tf8(const std::string& str);
	std::string wstring2utf8string(const std::wstring& str);
}
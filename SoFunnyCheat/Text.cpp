#include <Text.h>
#include <Windows.h>
#include <stdio.h>
#include <iostream>
#include <string>
#include <codecvt>
namespace Text
{
	const char* GetNameFromFile(const char* fullpathname)
	{
		char* save_name, * pos;
		int name_len;
		name_len = strlen(fullpathname);
		pos = (char*)fullpathname + name_len;
		while (*pos != '\\' && pos != fullpathname)
			pos--;
		if (pos == fullpathname)
		{
			save_name = (char*)fullpathname + 1;
			return save_name;
		}
		name_len = name_len - (pos - fullpathname);
		save_name = (char*)malloc(name_len + 1);
		memcpy(save_name, pos + 1, name_len);
		return save_name;
	}
	std::string tf8(const std::string& str)
	{
		int nwLen = MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, NULL, 0);
		wchar_t* pwBuf = new wchar_t[nwLen + 1];
		memset(pwBuf, 0, nwLen * 2 + 2);
		MultiByteToWideChar(CP_ACP, 0, str.c_str(), str.length(), pwBuf, nwLen);

		int nLen = WideCharToMultiByte(CP_UTF8, 0, pwBuf, -1, NULL, NULL, NULL, NULL);
		char* pBuf = new char[nLen + 1];
		memset(pBuf, 0, nLen + 1);
		WideCharToMultiByte(CP_UTF8, 0, pwBuf, nwLen, pBuf, nLen, NULL, NULL);

		std::string ret = pBuf;
		delete[]pwBuf;
		delete[]pBuf;

		return ret;
	}
	std::string wstring2utf8string(const std::wstring& str)
	{
		static std::wstring_convert<std::codecvt_utf8<wchar_t>> strCnv;
		return strCnv.to_bytes(str);
	}
}
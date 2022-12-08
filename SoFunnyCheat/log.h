#pragma once
#include <Windows.h>
#include <string>
namespace logs
{
	enum Level :int
	{
		L_INFO = 1,//信息
		L_DEBUG,//调试
		L_WARNING,//警告
		L_ERROR//错误
	};
#define LOG_OUT(M,Char) LOG_OUT_(M,Char,__FILE__,__LINE__)
	//控制台输出(模式Level, 内容文本, 是否换行, 文件, 行号)
	void LOG_OUT_(Level M, const char* Char, std::string code, int line);
	//打开控制台(标题 , 是否可以关闭)
	HWND LOG_START(const char* title, bool Close);
	//关闭控制台()
	void LOG_END();
}
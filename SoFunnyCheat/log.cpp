#include <log.h>
#include <Windows.h>
#include <stdio.h>
#include <iostream>
#include <Times.h>
#include <Text.h>
namespace logs
{
	void LOG_OUT_(Level M, const char* Char, std::string code, int line)
	{
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
		std::cout << "[";
		switch (M)
		{
		case Level::L_INFO:
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 2);
			std::cout << "Info";
			break;
		case Level::L_DEBUG:
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 9);
			std::cout << "Debug";
			break;
		case Level::L_WARNING:
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 6);
			std::cout << "Warning";
			break;
		case Level::L_ERROR:
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 4);
			std::cout << "Error";
			break;
		}
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
		std::cout << "] [";
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 5);
		std::cout << Text::GetNameFromFile(code.c_str()) << ":" << line;
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
		std::cout << "] ";
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 8);
		std::cout << times::GetHour() << ":" << times::GetMin() << ":" << times::GetSec() << "." << times::GetMilliSec();
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
		std::cout << ">> " << Char;
		std::cout << std::endl;
	}

	HWND LOG_START(const char* title, bool Close)
	{
		HANDLE g_hOutput = 0;
		HWND hwnd = NULL;
		HMENU hmenu = NULL;
		AllocConsole();
		g_hOutput = GetStdHandle(STD_OUTPUT_HANDLE);
		//设置控制台窗口的属性
		SetConsoleTitle(title);
		SetConsoleTextAttribute((HANDLE)g_hOutput, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
		while (NULL == hwnd) hwnd = ::FindWindow(NULL, (LPCTSTR)title);
		//屏蔽掉控制台窗口的关闭按钮，以防窗口被删除
		hmenu = ::GetSystemMenu(hwnd, FALSE);
		if (!Close)
		{
			DeleteMenu(hmenu, SC_CLOSE, MF_BYCOMMAND);
		}
		freopen_s((FILE**)stdout, "CONOUT$", "w", stdout);
		freopen_s((FILE**)stderr, "CONOUT$", "w", stderr);
		return hwnd;
	}
	void LOG_END()
	{
		fclose(stdout);
		fclose(stderr);
		FreeConsole();
	}
}
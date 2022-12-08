// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include <Windows.h>
#include <Process.hpp>
#include <inject/manual-map.h>
#include <inject/load-library.h>
#include <log.h>
#include <stdlib.h>
#include <iostream>
#include <stdio.h>
#include <fstream>
#include <process.h>
using namespace logs;
using namespace Process;

HMODULE hModule_;

void inject()
{
	LOG_START("inject", false);
	LOG_OUT(L_DEBUG, std::string("PID:"+std::to_string(_getpid())).c_str());
	if (Process::GetProcessIdByName("GameLoader.exe") == NULL)
	{
		LOG_OUT(L_ERROR, "请先启动游戏");
		Beep(1000, 1000);
		MessageBoxA(NULL, "请先启动游戏", NULL,NULL);
		Sleep(10000);
		LOG_END();
		FreeLibraryAndExitThread(hModule_, 0);
	}
	if (Process::GetProcessIdByName("GameLoader.exe") != _getpid())
	{
		LOG_OUT(L_ERROR, "注入错误进程,请注入GameLoader.exe进程");
		Beep(1000, 1000);
		MessageBox(NULL, "注入错误进程,请注入GameLoader.exe进程", "注入错误进程,请注入GameLoader.exe进程", NULL);
		Sleep(10000);
		LOG_END();
		FreeLibraryAndExitThread(hModule_, 0);
	}
	LOG_OUT(L_INFO, "等待游戏运行...");
	while (Process::GetProcessIdByName("Sausage Man.exe") == NULL)
	{
		Sleep(1);
	}
	LOG_OUT(L_INFO, "内存注入dll...");
	std::ifstream MOUDLE(Process::GetModulePath(hModule_) + "\\RTSSHooks.dll");
	if (!MOUDLE)
	{
		LOG_OUT(L_ERROR, "无法获取RTSSHooks.dll路径请确保其与inject.dll同一目录");
		Beep(1000, 1000);
		MessageBox(NULL, "无法获取RTSSHooks.dll路径请确保其与inject.dll同一目录", "无法获取RTSSHooks.dll路径请确保其与inject.dll同一目录", NULL);
		Sleep(10000);
		LOG_END();
		FreeLibraryAndExitThread(hModule_, 0);
	}
	MOUDLE.close();
	bool injectDone  = ManualMapDLL(OpenProcess(PROCESS_ALL_ACCESS, NULL, Process::GetProcessIdByName("Sausage Man.exe")), (Process::GetModulePath(hModule_) + "\\RTSSHooks.dll"),true,true,true,true);
	if(!injectDone)LOG_OUT(L_INFO, "注入失败!");
	LOG_OUT(L_INFO, "卸载dll...");
	Sleep(5000);
	LOG_END();
	FreeLibraryAndExitThread(hModule_, 0);
}
BOOL WINAPI DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
	hModule_ = hModule;
	if (ul_reason_for_call == DLL_PROCESS_ATTACH)
		CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)inject, NULL, NULL, NULL);
	return TRUE;
}
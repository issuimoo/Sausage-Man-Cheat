#pragma once
#include <Windows.h>
#include <string>
namespace Process
{
	DWORD GetProcessIdByName(const char* name);
	int  GetHandleById(DWORD id);
	LPDWORD CreateThreads(void(*p));
	bool UpPrivilegeValue();
	std::string GetModulePath(HMODULE hModule /*= nullptr*/);
	WCHAR getProcessNameByProcessId(DWORD cur_process_id);
	bool CloseHandleByName(const wchar_t* name);
}
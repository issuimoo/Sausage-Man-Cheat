#pragma once
#include <vector>
#include <Windows.h>
namespace Memory
{		//x32读x64请使用Ntdll或Driver
	enum Type :int
	{
		Ntdll,//内核
		Common,//普通
		Driver//驱动
	};
	class Memory
	{
	public:
		//初始化(读写类型)
		void initialize(Type readtype);

		//写内存
		bool Read(HANDLE hProcess, ULONG64 address, LPVOID IPBUFFER, SIZE_T size);
		//读内存
		bool Write(HANDLE hProcess, ULONG64 address, LPVOID IPBUFFER, SIZE_T size);

		//读字节型(进程句柄,地址)
		BYTE Read_byte(DWORD pid, ULONG64 address);
		//写字节型(进程句柄,地址,值)
		bool Write_byte(DWORD pid, ULONG64 address, BYTE value);

		//读整数型(进程句柄,地址)
		DWORD Read_int(DWORD pid, ULONG64 address);
		//写整数型(进程句柄,地址,值)
		bool Write_int(DWORD pid, ULONG64 address, DWORD value);

		//读长整数型(进程句柄,地址)
		DWORD64 Read_long_int(DWORD pid, ULONG64 address);
		//写长整数型(进程句柄,地址,值)
		bool Write_long_int(DWORD pid, ULONG64 address, DWORD64 value);

		//读浮点型(进程句柄,地址)
		float Read_float(DWORD pid, ULONG64 address);
		//写浮点型(进程句柄,地址,值)
		bool Write_float(DWORD pid, ULONG64 address, float value);

		//读双浮点型(进程句柄,地址)
		double Read_double(DWORD pid, ULONG64 address);
		//写双浮点型(进程句柄,地址,值)
		bool Write_double(DWORD pid, ULONG64 address, double value);

		//搜索特征码(进程句柄,特征码)
		std::vector<DWORD64> Search_Signatures(DWORD pid, ULONG64 START, ULONG64 END, const char* Signatures);
	private:
		bool initializeNT();
		bool initializeDR();
		void ReadProcessMemorys();
		int Mode;
	};
}
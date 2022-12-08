#include <Memory.hpp>
#include <Windows.h>
#include <stdio.h>
#include <string>
#include <log.h>
#include <Process.hpp>

typedef NTSTATUS(NTAPI* LPFN_NTWOW64READVIRTUALMEMORY64)(
	IN  HANDLE   ProcessHandle,
	IN  ULONG64  BaseAddress,
	OUT PVOID    BufferData,
	IN  ULONG64  BufferLength,
	OUT PULONG64 ReturnLength OPTIONAL);

typedef NTSTATUS(NTAPI* LPFN_NTWOW64WRITEVIRTUALMEMORY64)(
	IN  HANDLE   ProcessHandle,
	IN  ULONG64  BaseAddress,
	OUT PVOID    BufferData,
	IN  ULONG64  BufferLength,
	OUT PULONG64 ReturnLength OPTIONAL);

HMODULE NtdllModuleBase = NULL;
LPFN_NTWOW64READVIRTUALMEMORY64 __NtWow64ReadVirtualMemory64;
LPFN_NTWOW64WRITEVIRTUALMEMORY64 __NtWow64WriteVirtualMemory64;

typedef struct
{
	DWORD pid;
	ULONG64 address;
	UINT bytes_toread;
	DWORD64 data;
}ProcessIntMemory;

// 读写内存偏移整数型
typedef struct
{
	DWORD pid;
	ULONG64 base_address;
	DWORD offset[32];
	DWORD offset_len;
	INT64 data;
}ProcessDeviationIntMemory;

// 读写多级偏移内存地址
typedef struct
{
	DWORD pid;
	ULONG64 base_address;
	DWORD offset[32];
	DWORD offset_len;
	DWORD64 data;
}ProcessDeviationMemory;

// 读写内存字节型
typedef struct
{
	DWORD pid;
	ULONG64 base_address;
	BYTE OpCode;
}ProcessByteMemory;

// 取模块名称结构体
typedef struct ModuleInfoStruct
{
	CHAR ModuleName[1024];
}ModuleInfoStruct, * LPModuleInfoStruct;

bool initializeDR_ = false;
typedef int(*InstallDriver_)();
InstallDriver_ InstallDriver;
typedef int(*RemoveDriver_)();
RemoveDriver_ RemoveDriver;
typedef BYTE(*ReadProcessMemoryByte_)(DWORD pid, ULONG64 address);
ReadProcessMemoryByte_ ReadProcessMemoryByte;
typedef BOOL(*WriteProcessMemoryByte_)(DWORD Pid, ULONG64 Address, BYTE bytef);
WriteProcessMemoryByte_ WriteProcessMemoryByte;
typedef DWORD(*ReadProcessMemoryInt32_)(DWORD Pid, ULONG64 Address);
ReadProcessMemoryInt32_ ReadProcessMemoryInt32;
typedef DWORD(*ReadProcessMemoryInt64_)(DWORD Pid, ULONG64 Address);
ReadProcessMemoryInt64_ ReadProcessMemoryInt64;
typedef BOOL(*WriteProcessMemoryInt32_)(DWORD Pid, ULONG64 Address, DWORD write);
WriteProcessMemoryInt32_ WriteProcessMemoryInt32;
typedef BOOL(*WriteProcessMemoryInt64_)(DWORD Pid, ULONG64 Address, DWORD write);
WriteProcessMemoryInt64_ WriteProcessMemoryInt64;
typedef FLOAT(*ReadProcessMemoryFloat_)(DWORD Pid, ULONG64 Address);
ReadProcessMemoryFloat_ ReadProcessMemoryFloat;
typedef DOUBLE(*ReadProcessMemoryDouble_)(DWORD Pid, ULONG64 Address);
ReadProcessMemoryDouble_ ReadProcessMemoryDouble;
typedef BOOL(*WriteProcessMemoryFloat_)(DWORD Pid, ULONG64 Address, FLOAT write);
WriteProcessMemoryFloat_ WriteProcessMemoryFloat;
typedef BOOL(*WriteProcessMemoryDouble_)(DWORD Pid, ULONG64 Address, DOUBLE write);
WriteProcessMemoryDouble_ WriteProcessMemoryDouble;
typedef INT32(*ReadProcessDeviationInt32_)(ProcessDeviationIntMemory read_offset_struct);
ReadProcessDeviationInt32_ ReadProcessDeviationInt32;
typedef INT64(*ReadProcessDeviationInt64_)(ProcessDeviationIntMemory read_offset_struct);
ReadProcessDeviationInt64_ ReadProcessDeviationInt64;
typedef BOOL(*WriteProcessDeviationInt32_)(ProcessDeviationIntMemory write_offset_struct);
WriteProcessDeviationInt32_ WriteProcessDeviationInt32;
typedef BOOL(*WriteProcessDeviationInt64_)(ProcessDeviationIntMemory write_offset_struct);
WriteProcessDeviationInt64_ WriteProcessDeviationInt64;
typedef DWORD(*ReadDeviationMemory32_)(ProcessDeviationMemory read_offset_struct);
ReadDeviationMemory32_ ReadDeviationMemory32;
typedef DWORD64(*ReadDeviationMemory64_)(ProcessDeviationMemory read_offset_struct);
ReadDeviationMemory64_ ReadDeviationMemory64;
typedef BYTE(*ReadDeviationByte_)(ProcessDeviationMemory read_offset_struct);
ReadDeviationByte_ ReadDeviationByte;
typedef FLOAT(*ReadDeviationFloat_)(ProcessDeviationMemory read_offset_struct);
ReadDeviationFloat_ ReadDeviationFloat;
typedef BOOL(*WriteDeviationByte_)(ProcessDeviationMemory write_offset_struct, BYTE write_byte);
WriteDeviationByte_ WriteDeviationByte;
typedef BOOL(*WriteDeviationFloat_)(ProcessDeviationMemory write_offset_struct, FLOAT write_float);
WriteDeviationFloat_ WriteDeviationFloat;

typedef BOOL(*SetPid_)(DWORD Pid);
SetPid_ SetPid;
typedef void (*ReadMemoryDWORD_)(DWORD pid, ULONG64 addre, DWORD ret);
ReadMemoryDWORD_ ReadMemoryDWORD;
typedef void (*ReadMemoryDWORD64_)(DWORD pid, ULONG64 addre, DWORD64 ret);
ReadMemoryDWORD64_ ReadMemoryDWORD64;
typedef void (*ReadMemoryBytes_)(DWORD pid, ULONG64 addre, BYTE* ret, DWORD sizes);
ReadMemoryBytes_ ReadMemoryBytes;
typedef void (*ReadMemoryFloat_)(DWORD pid, ULONG64 addre, float ret);
ReadMemoryFloat_ ReadMemoryFloat;
typedef void (*ReadMemoryDouble_)(DWORD pid, ULONG64 addre, double ret);
ReadMemoryDouble_ ReadMemoryDouble;
typedef void (*WriteMemoryBytes_)(DWORD pid, ULONG64 addre, BYTE data, DWORD sizes);
WriteMemoryBytes_ WriteMemoryBytes;
typedef void (*WriteMemoryDWORD_)(DWORD pid, ULONG64 addre, DWORD ret);
WriteMemoryDWORD_ WriteMemoryDWORD;
typedef void (*WriteMemoryDWORD64_)(DWORD pid, ULONG64 addre, DWORD64 ret);
WriteMemoryDWORD64_ WriteMemoryDWORD64;
typedef void (*WriteMemoryFloat_)(DWORD pid, ULONG64 addre, float ret);
WriteMemoryFloat_ WriteMemoryFloat;
typedef void (*WriteMemoryDouble_)(DWORD pid, ULONG64 addre, double ret);
WriteMemoryDouble_ WriteMemoryDouble;
typedef DWORD64(*GetModuleAddress_)(DWORD pid, std::string dllname);
GetModuleAddress_ GetModuleAddress;
typedef DWORD(*GetProcessID_)(std::string procname);
GetProcessID_ GetProcessID;
typedef DWORD64(*GetSystemRoutineAddress_)(std::string funcname);
GetSystemRoutineAddress_ GetSystemRoutineAddress;
typedef DWORD64(*CreateRemoteMemory_)(DWORD length);
CreateRemoteMemory_ CreateRemoteMemory;
typedef DWORD(*DeleteRemoteMemory_)(DWORD64 address, DWORD length);
DeleteRemoteMemory_ DeleteRemoteMemory;

namespace Memory
{
	void Memory::initialize(Type readtype)
	{
		Mode = readtype;
		switch (readtype)
		{
		case Ntdll: //nt函数读内存
			initializeNT();
			break;
		case Common: //普通函数
			//initialize?
			break;
		case Driver:  //驱动读内存
			initializeDR_ = initializeDR();
			break;
		default:
			break;
		}
	}

	bool Memory::initializeDR()
	{
		HMODULE module = LoadLibrary("Engine.dll");
		if (module == NULL)
		{
			LOG_OUT(logs::L_ERROR, "LoadLibrary driver.dll error!");
			return false;
		}
		InstallDriver = (InstallDriver_)GetProcAddress(module, "InstallDriver");
		RemoveDriver = (RemoveDriver_)GetProcAddress(module, "RemoveDriver");
		ReadProcessMemoryByte = (ReadProcessMemoryByte_)GetProcAddress(module, "ReadProcessMemoryByte");
		WriteProcessMemoryByte = (WriteProcessMemoryByte_)GetProcAddress(module, "WriteProcessMemoryByte");
		ReadProcessMemoryInt32 = (ReadProcessMemoryInt32_)GetProcAddress(module, "ReadProcessMemoryInt32");
		ReadProcessMemoryInt64 = (ReadProcessMemoryInt64_)GetProcAddress(module, "ReadProcessMemoryInt64");
		WriteProcessMemoryInt32 = (WriteProcessMemoryInt32_)GetProcAddress(module, "WriteProcessMemoryInt32");
		WriteProcessMemoryInt64 = (WriteProcessMemoryInt64_)GetProcAddress(module, "WriteProcessMemoryInt64");
		ReadProcessMemoryFloat = (ReadProcessMemoryFloat_)GetProcAddress(module, "ReadProcessMemoryFloat");
		ReadProcessMemoryDouble = (ReadProcessMemoryDouble_)GetProcAddress(module, "ReadProcessMemoryDouble");
		WriteProcessMemoryFloat = (WriteProcessMemoryFloat_)GetProcAddress(module, "WriteProcessMemoryFloat");
		WriteProcessMemoryDouble = (WriteProcessMemoryDouble_)GetProcAddress(module, "WriteProcessMemoryDouble");
		ReadProcessDeviationInt32 = (ReadProcessDeviationInt32_)GetProcAddress(module, "ReadProcessDeviationInt32");
		ReadProcessDeviationInt64 = (ReadProcessDeviationInt64_)GetProcAddress(module, "ReadProcessDeviationInt64");
		WriteProcessDeviationInt32 = (WriteProcessDeviationInt32_)GetProcAddress(module, "WriteProcessDeviationInt32");
		WriteProcessDeviationInt64 = (WriteProcessDeviationInt64_)GetProcAddress(module, "WriteProcessDeviationInt64");
		SetPid = (SetPid_)GetProcAddress(module, "SetPid");
		ReadDeviationMemory32 = (ReadDeviationMemory32_)GetProcAddress(module, "ReadDeviationMemory32");
		ReadDeviationMemory64 = (ReadDeviationMemory64_)GetProcAddress(module, "ReadDeviationMemory64");
		ReadDeviationByte = (ReadDeviationByte_)GetProcAddress(module, "ReadDeviationByte");
		ReadDeviationFloat = (ReadDeviationFloat_)GetProcAddress(module, "ReadDeviationFloat");
		WriteDeviationByte = (WriteDeviationByte_)GetProcAddress(module, "WriteDeviationByte");
		WriteDeviationFloat = (WriteDeviationFloat_)GetProcAddress(module, "WriteDeviationFloat");
		ReadMemoryDWORD = (ReadMemoryDWORD_)GetProcAddress(module, "ReadMemoryDWORD");
		ReadMemoryDWORD64 = (ReadMemoryDWORD64_)GetProcAddress(module, "ReadMemoryDWORD64");
		ReadMemoryBytes = (ReadMemoryBytes_)GetProcAddress(module, "ReadMemoryBytes");
		ReadMemoryFloat = (ReadMemoryFloat_)GetProcAddress(module, "ReadMemoryFloat");
		ReadMemoryDouble = (ReadMemoryDouble_)GetProcAddress(module, "ReadMemoryDouble");
		WriteMemoryBytes = (WriteMemoryBytes_)GetProcAddress(module, "WriteMemoryBytes");
		WriteMemoryDWORD = (WriteMemoryDWORD_)GetProcAddress(module, "WriteMemoryDWORD");
		WriteMemoryDWORD64 = (WriteMemoryDWORD64_)GetProcAddress(module, "WriteMemoryDWORD64");
		WriteMemoryFloat = (WriteMemoryFloat_)GetProcAddress(module, "WriteMemoryFloat");
		WriteMemoryDouble = (WriteMemoryDouble_)GetProcAddress(module, "WriteMemoryDouble");
		GetModuleAddress = (GetModuleAddress_)GetProcAddress(module, "GetModuleAddress");
		GetProcessID = (GetProcessID_)GetProcAddress(module, "GetProcessID");
		GetSystemRoutineAddress = (GetSystemRoutineAddress_)GetProcAddress(module, "GetSystemRoutineAddress");
		CreateRemoteMemory = (CreateRemoteMemory_)GetProcAddress(module, "CreateRemoteMemory");
		DeleteRemoteMemory = (DeleteRemoteMemory_)GetProcAddress(module, "DeleteRemoteMemory");
		Process::UpPrivilegeValue();
		if (InstallDriver() == -1)
		{
			LOG_OUT(logs::L_ERROR, "InstallDriver error!");
			return false;
		}
		else
		{
			LOG_OUT(logs::L_DEBUG, "InstallDriver Done!");
		}
		return true;
	}

	bool Memory::initializeNT()
	{
		NtdllModuleBase = GetModuleHandle("Ntdll.dll");
		if (NtdllModuleBase == NULL)
		{
			LOG_OUT(logs::L_ERROR, "Get Ntdll.dll Handle error!");
			return false;
		}
		__NtWow64ReadVirtualMemory64 = (LPFN_NTWOW64READVIRTUALMEMORY64)GetProcAddress(NtdllModuleBase,
			"NtWow64ReadVirtualMemory64");
		__NtWow64WriteVirtualMemory64 = (LPFN_NTWOW64WRITEVIRTUALMEMORY64)GetProcAddress(NtdllModuleBase,
			"NtWow64WriteVirtualMemory64");
		return	 true;
	}

	bool Memory::Read(HANDLE hProcess, ULONG64 address, LPVOID IPBUFFER, SIZE_T size)
	{
		;
		switch (Mode)
		{
		case Ntdll:
			return __NtWow64ReadVirtualMemory64(hProcess, address, IPBUFFER, size, NULL);
			break;
		case Common:
			return ReadProcessMemory(hProcess, (LPVOID)address, IPBUFFER, size, NULL);
			break;
		case Driver:
			LOG_OUT(logs::L_ERROR, "unsupport driver!");
			return 0;
			break;
		default:
			break;
		}
	}

	bool Memory::Write(HANDLE hProcess, ULONG64 address, LPVOID IPBUFFER, SIZE_T size)
	{
		switch (Mode)
		{
		case Ntdll:
			return __NtWow64WriteVirtualMemory64(hProcess, address, IPBUFFER, size, NULL);
			break;
		case Common:
			return WriteProcessMemory(hProcess, (LPVOID)address, IPBUFFER, size, NULL);
			break;
		case Driver:
			LOG_OUT(logs::L_ERROR, "unsupport driver!");
			return 0;
			break;
		default:
			break;
		}
	}

	BYTE Memory::Read_byte(DWORD pid, ULONG64 address)
	{
		BYTE Temp;
		HANDLE hProcess = (HANDLE)Process::GetHandleById(pid);
		switch (Mode)
		{
		case Ntdll:
			__NtWow64ReadVirtualMemory64(hProcess, address, &Temp, sizeof(BYTE), NULL);
			return Temp;
			break;
		case Common:
			ReadProcessMemory(hProcess, (LPVOID)address, &Temp, sizeof(BYTE), NULL);
			return Temp;
			break;
		case Driver:
			if (!initializeDR_)
			{
				LOG_OUT(logs::L_ERROR, "Not initialize driver!");
				return 0;
			}
			return ReadProcessMemoryByte(pid, address);
			break;
		default:
			break;
		}
		CloseHandle(hProcess);
	}

	bool Memory::Write_byte(DWORD pid, ULONG64 address, BYTE value)
	{
		bool Done = false;
		HANDLE hProcess = (HANDLE)Process::GetHandleById(pid);
		switch (Mode)
		{
		case Ntdll:
			Done = __NtWow64WriteVirtualMemory64(hProcess, address, &value, sizeof(BYTE), NULL);
			return Done;
			break;
		case Common:
			Done = WriteProcessMemory(hProcess, (LPVOID)address, &value, sizeof(BYTE), NULL);
			return Done;
			break;
		case Driver:
			if (!initializeDR_)
			{
				LOG_OUT(logs::L_ERROR, "Not initialize driver!");
				return 0;
			}
			return WriteProcessMemoryByte(pid, address, value);
			break;
		default:
			break;
		}
		CloseHandle(hProcess);
	}

	DWORD Memory::Read_int(DWORD pid, ULONG64 address)
	{
		DWORD Temp;
		HANDLE hProcess = (HANDLE)Process::GetHandleById(pid);
		switch (Mode)
		{
		case Ntdll:
			__NtWow64ReadVirtualMemory64(hProcess, address, &Temp, sizeof(DWORD), NULL);
			return Temp;
			break;
		case Common:
			ReadProcessMemory(hProcess, (LPVOID)address, &Temp, sizeof(DWORD), NULL);
			return Temp;
			break;
		case Driver:
			if (!initializeDR_)
			{
				LOG_OUT(logs::L_ERROR, "Not initialize driver!");
				return 0;
			}
			return ReadProcessMemoryInt32(pid, address);
			break;
		default:
			break;
		}
		CloseHandle(hProcess);
	}

	bool Memory::Write_int(DWORD pid, ULONG64 address, DWORD value)
	{
		bool Done = false;
		HANDLE hProcess = (HANDLE)Process::GetHandleById(pid);
		switch (Mode)
		{
		case Ntdll:
			Done = __NtWow64WriteVirtualMemory64(hProcess, address, &value, sizeof(DWORD), NULL);
			return Done;
			break;
		case Common:
			Done = WriteProcessMemory(hProcess, (LPVOID)address, &value, sizeof(DWORD), NULL);
			return Done;
			break;
		case Driver:
			if (!initializeDR_)
			{
				LOG_OUT(logs::L_ERROR, "Not initialize driver!");
				return 0;
			}
			return WriteProcessMemoryInt32(pid, address, value);
			break;
		default:
			break;
		}
		CloseHandle(hProcess);
	}

	DWORD64 Memory::Read_long_int(DWORD pid, ULONG64 address)
	{
		DWORD64 Temp;
		HANDLE hProcess = (HANDLE)Process::GetHandleById(pid);
		switch (Mode)
		{
		case Ntdll:
			__NtWow64ReadVirtualMemory64(hProcess, address, &Temp, sizeof(DWORD64), NULL);
			return Temp;
			break;
		case Common:
			ReadProcessMemory(hProcess, (LPVOID)address, &Temp, sizeof(DWORD64), NULL);
			return Temp;
			break;
		case Driver:
			if (!initializeDR_)
			{
				LOG_OUT(logs::L_ERROR, "Not initialize driver!");
				return 0;
			}
			return ReadProcessMemoryInt64(pid, address);
			break;
		default:
			break;
		}
		CloseHandle(hProcess);
	}

	bool Memory::Write_long_int(DWORD pid, ULONG64 address, DWORD64 value)
	{
		bool Done = false;
		HANDLE hProcess = (HANDLE)Process::GetHandleById(pid);
		switch (Mode)
		{
		case Ntdll:
			Done = __NtWow64WriteVirtualMemory64(hProcess, address, &value, sizeof(DWORD64), NULL);
			return Done;
			break;
		case Common:
			Done = WriteProcessMemory(hProcess, (LPVOID)address, &value, sizeof(DWORD64), NULL);
			return Done;
			break;
		case Driver:
			if (!initializeDR_)
			{
				LOG_OUT(logs::L_ERROR, "Not initialize driver!");
				return 0;
			}
			return WriteProcessMemoryInt64(pid, address, value);
			break;
		default:
			break;
		}
		CloseHandle(hProcess);
	}

	float Memory::Read_float(DWORD pid, ULONG64 address)
	{
		float Temp;
		HANDLE hProcess = (HANDLE)Process::GetHandleById(pid);
		switch (Mode)
		{
		case Ntdll:
			__NtWow64ReadVirtualMemory64(hProcess, address, &Temp, sizeof(float), NULL);
			return Temp;
			break;
		case Common:
			ReadProcessMemory(hProcess, (LPVOID)address, &Temp, sizeof(float), NULL);
			return Temp;
			break;
		case Driver:
			if (!initializeDR_)
			{
				LOG_OUT(logs::L_ERROR, "Not initialize driver!");
				return 0;
			}
			return ReadProcessMemoryFloat(pid, address);
			break;
		default:
			break;
		}
		CloseHandle(hProcess);
	}

	bool Memory::Write_float(DWORD pid, ULONG64 address, float value)
	{
		bool Done = false;
		HANDLE hProcess = (HANDLE)Process::GetHandleById(pid);
		switch (Mode)
		{
		case Ntdll:
			Done = __NtWow64WriteVirtualMemory64(hProcess, address, &value, sizeof(float), NULL);
			return Done;
			break;
		case Common:
			Done = WriteProcessMemory(hProcess, (LPVOID)address, &value, sizeof(float), NULL);
			return Done;
			break;
		case Driver:
			if (!initializeDR_)
			{
				LOG_OUT(logs::L_ERROR, "Not initialize driver!");
				return 0;
			}
			return WriteProcessMemoryFloat(pid, address, value);
			break;
		default:
			break;
		}
		CloseHandle(hProcess);
	}

	double Memory::Read_double(DWORD pid, ULONG64 address)
	{
		double Temp;
		HANDLE hProcess = (HANDLE)Process::GetHandleById(pid);
		switch (Mode)
		{
		case Ntdll:
			__NtWow64ReadVirtualMemory64(hProcess, address, &Temp, sizeof(double), NULL);
			return Temp;
			break;
		case Common:
			ReadProcessMemory(hProcess, (LPVOID)address, &Temp, sizeof(double), NULL);
			return Temp;
			break;
		case Driver:
			if (!initializeDR_)
			{
				LOG_OUT(logs::L_ERROR, "Not initialize driver!");
				return 0;
			}
			return ReadProcessMemoryDouble(pid, address);
			break;
		default:
			break;
		}
		CloseHandle(hProcess);
	}

	bool Memory::Write_double(DWORD pid, ULONG64 address, double value)
	{
		bool Done = false;
		HANDLE hProcess = (HANDLE)Process::GetHandleById(pid);
		switch (Mode)
		{
		case Ntdll:
			Done = __NtWow64WriteVirtualMemory64(hProcess, address, &value, sizeof(double), NULL);
			return Done;
			break;
		case Common:
			Done = WriteProcessMemory(hProcess, (LPVOID)address, &value, sizeof(double), NULL);
			return Done;
			break;
		case Driver:
			if (!initializeDR_)
			{
				LOG_OUT(logs::L_ERROR, "Not initialize driver!");
				return 0;
			}
			return WriteProcessMemoryDouble(pid, address, value);
			break;
		default:
			break;
		}
		CloseHandle(hProcess);
	}
}
#include <Process.hpp>
#include <Windows.h>
#include <stdio.h>
#include <string>
#include <iostream>
#include <filesystem>
#include <TlHelp32.h>
namespace Process
{
	DWORD GetProcessIdByName(const char* name)
	{
		HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
		if (INVALID_HANDLE_VALUE == hSnapshot) {
			return NULL;
		}
		PROCESSENTRY32 pe = { sizeof(pe) };
		for (BOOL ret = Process32First(hSnapshot, &pe); ret; ret = Process32Next(hSnapshot, &pe)) {
			if (strcmp(pe.szExeFile, name) == 0) {
				CloseHandle(hSnapshot);
				return pe.th32ProcessID;
			}
			//printf("%-6d %s\n", pe.th32ProcessID, pe.szExeFile);
		}
		CloseHandle(hSnapshot);
		return 0;
	}

	int  GetHandleById(DWORD id)
	{
		return (int)OpenProcess(PROCESS_ALL_ACCESS, false, id);
	}

	LPDWORD CreateThreads(void(*p))
	{
		LPDWORD ID = nullptr;
		CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)p, 0, 0, ID);
		return ID;
	}

	bool UpPrivilegeValue()
	{
		//OpenProcessToken()函数用来打开与进程相关联的访问令牌
		HANDLE hToken = nullptr;
		if (FALSE == OpenProcessToken(GetCurrentProcess(), TOKEN_ALL_ACCESS, &hToken))
		{
			return false;
		}
		//LookupPrivilegeValue()函数查看系统权限的特权值
		LUID luid;
		if (FALSE == LookupPrivilegeValue(nullptr, SE_DEBUG_NAME, &luid))
		{
			CloseHandle(hToken);
			return false;
		}
		//调整权限设置
		TOKEN_PRIVILEGES Tok;
		Tok.PrivilegeCount = 1;
		Tok.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
		Tok.Privileges[0].Luid = luid;
		if (FALSE == AdjustTokenPrivileges(hToken, FALSE, &Tok, sizeof(Tok), nullptr, nullptr))
		{
			CloseHandle(hToken);
			return false;
		}

		if (GetLastError() == ERROR_NOT_ALL_ASSIGNED)
		{
			CloseHandle(hToken);
			return false;
		}

		CloseHandle(hToken);
		return true;
	}
	std::string GetModulePath(HMODULE hModule /*= nullptr*/)
	{
		char pathOut[MAX_PATH] = {};
		GetModuleFileNameA(hModule, pathOut, MAX_PATH);

		return std::filesystem::path(pathOut).parent_path().string();
	}
	WCHAR getProcessNameByProcessId(DWORD cur_process_id)
	{
		HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
		if (INVALID_HANDLE_VALUE == hSnapshot) {
			return NULL;
		}
		PROCESSENTRY32 pe = { sizeof(pe) };
		for (BOOL ret = Process32First(hSnapshot, &pe); ret; ret = Process32Next(hSnapshot, &pe)) {
			if (pe.th32ProcessID == cur_process_id)
			{
				return pe.szExeFile[MAX_PATH];
			}
		}
		CloseHandle(hSnapshot);
		WCHAR noFind_str = NULL;
		return noFind_str;
	}

#pragma comment(lib,"ntdll.lib")

#define NT_SUCCESS(x) ((x) >= 0)
#define STATUS_INFO_LENGTH_MISMATCH 0xc0000004

#define SystemHandleInformation 16
#define ObjectBasicInformation 0
#define ObjectNameInformation 1
#define ObjectTypeInformation 2

	static int num = 0;

	typedef NTSTATUS(NTAPI* _NtQuerySystemInformation)(
		ULONG SystemInformationClass,
		PVOID SystemInformation,
		ULONG SystemInformationLength,
		PULONG ReturnLength
		);

	typedef NTSTATUS(NTAPI* _NtDuplicateObject)(
		HANDLE SourceProcessHandle,
		HANDLE SourceHandle,
		HANDLE TargetProcessHandle,
		PHANDLE TargetHandle,
		ACCESS_MASK DesiredAccess,
		ULONG Attributes,
		ULONG Options
		);

	typedef NTSTATUS(NTAPI* _NtQueryObject)(
		HANDLE ObjectHandle,
		ULONG ObjectInformationClass,
		PVOID ObjectInformation,
		ULONG ObjectInformationLength,
		PULONG ReturnLength
		);

	typedef struct _UNICODE_STRING
	{
		USHORT Length;
		USHORT MaximumLength;
		PWSTR Buffer;
	} UNICODE_STRING, * PUNICODE_STRING;

	typedef struct _SYSTEM_HANDLE
	{
		ULONG ProcessId;
		BYTE ObjectTypeNumber;
		BYTE Flags;
		USHORT Handle;
		PVOID Object;
		ACCESS_MASK GrantedAccess;
	} SYSTEM_HANDLE, * PSYSTEM_HANDLE;

	typedef struct _SYSTEM_HANDLE_INFORMATION
	{
		ULONG HandleCount;
		SYSTEM_HANDLE Handles[1];
	} SYSTEM_HANDLE_INFORMATION, * PSYSTEM_HANDLE_INFORMATION;

	typedef enum _POOL_TYPE
	{
		NonPagedPool,
		PagedPool,
		NonPagedPoolMustSucceed,
		DontUseThisType,
		NonPagedPoolCacheAligned,
		PagedPoolCacheAligned,
		NonPagedPoolCacheAlignedMustS
	} POOL_TYPE, * PPOOL_TYPE;

	typedef struct _OBJECT_TYPE_INFORMATION
	{
		UNICODE_STRING Name;
		ULONG TotalNumberOfObjects;
		ULONG TotalNumberOfHandles;
		ULONG TotalPagedPoolUsage;
		ULONG TotalNonPagedPoolUsage;
		ULONG TotalNamePoolUsage;
		ULONG TotalHandleTableUsage;
		ULONG HighWaterNumberOfObjects;
		ULONG HighWaterNumberOfHandles;
		ULONG HighWaterPagedPoolUsage;
		ULONG HighWaterNonPagedPoolUsage;
		ULONG HighWaterNamePoolUsage;
		ULONG HighWaterHandleTableUsage;
		ULONG InvalidAttributes;
		GENERIC_MAPPING GenericMapping;
		ULONG ValidAccess;
		BOOLEAN SecurityRequired;
		BOOLEAN MaintainHandleCount;
		USHORT MaintainTypeList;
		POOL_TYPE PoolType;
		ULONG PagedPoolUsage;
		ULONG NonPagedPoolUsage;
	} OBJECT_TYPE_INFORMATION, * POBJECT_TYPE_INFORMATION;

	typedef struct _SYSTEM_PROCESS_INFO
	{
		ULONG                   NextEntryOffset;
		ULONG                   NumberOfThreads;
		LARGE_INTEGER           Reserved[3];
		LARGE_INTEGER           CreateTime;
		LARGE_INTEGER           UserTime;
		LARGE_INTEGER           KernelTime;
		UNICODE_STRING          ImageName;
		ULONG                   BasePriority;
		HANDLE                  ProcessId;
		HANDLE                  InheritedFromProcessId;
	}SYSTEM_PROCESS_INFO, * PSYSTEM_PROCESS_INFO;

	static PVOID GetLibraryProcAddress(LPCSTR LibraryName, LPCSTR ProcName)
	{
		auto hModule = GetModuleHandleA(LibraryName);
		if (hModule == NULL)
			return nullptr;
		return GetProcAddress(hModule, ProcName);
	}
	bool CloseHandleByName(const wchar_t* name)
	{
		_NtQuerySystemInformation NtQuerySystemInformation =
			(_NtQuerySystemInformation)GetLibraryProcAddress("ntdll.dll", "NtQuerySystemInformation");
		_NtDuplicateObject NtDuplicateObject =
			(_NtDuplicateObject)GetLibraryProcAddress("ntdll.dll", "NtDuplicateObject");
		_NtQueryObject NtQueryObject =
			(_NtQueryObject)GetLibraryProcAddress("ntdll.dll", "NtQueryObject");
		NTSTATUS status;

		ULONG handleInfoSize = 0x10000;
		PSYSTEM_HANDLE_INFORMATION handleInfo = (PSYSTEM_HANDLE_INFORMATION)malloc(handleInfoSize);

		ULONG pid = 0;
		HANDLE processHandle = GetCurrentProcess();
		ULONG i;

		/* NtQuerySystemInformation won't give us the correct buffer size,
		   so we guess by doubling the buffer size. */
		while ((status = NtQuerySystemInformation(
			SystemHandleInformation,
			handleInfo,
			handleInfoSize,
			NULL
		)) == STATUS_INFO_LENGTH_MISMATCH)
			handleInfo = (PSYSTEM_HANDLE_INFORMATION)realloc(handleInfo, handleInfoSize *= 2);

		/* NtQuerySystemInformation stopped giving us STATUS_INFO_LENGTH_MISMATCH. */
		if (!NT_SUCCESS(status))
		{
			return false;
		}

		bool closed = false;
		for (i = 0; i < handleInfo->HandleCount; i++)
		{
			if (closed)
				break;

			SYSTEM_HANDLE handle = handleInfo->Handles[i];
			HANDLE dupHandle = NULL;
			POBJECT_TYPE_INFORMATION objectTypeInfo;
			PVOID objectNameInfo;
			UNICODE_STRING objectName;
			ULONG returnLength;

			/* Duplicate the handle so we can query it. */
			if (!NT_SUCCESS(NtDuplicateObject(processHandle, (HANDLE)handle.Handle, GetCurrentProcess(), &dupHandle, 0, 0, 0)))
				continue;

			/* Query the object type. */
			objectTypeInfo = (POBJECT_TYPE_INFORMATION)malloc(0x1000);
			if (!NT_SUCCESS(NtQueryObject(dupHandle, ObjectTypeInformation, objectTypeInfo, 0x1000, NULL)))
			{
				CloseHandle(dupHandle);
				continue;
			}

			/* Query the object name (unless it has an access of
			   0x0012019f, on which NtQueryObject could hang. */
			if (handle.GrantedAccess == 0x0012019f)
			{
				free(objectTypeInfo);
				CloseHandle(dupHandle);
				continue;
			}

			objectNameInfo = malloc(0x1000);
			if (!NT_SUCCESS(NtQueryObject(dupHandle, ObjectNameInformation, objectNameInfo, 0x1000, &returnLength)))
			{
				/* Reallocate the buffer and try again. */
				objectNameInfo = realloc(objectNameInfo, returnLength);
				if (!NT_SUCCESS(NtQueryObject(dupHandle, ObjectNameInformation, objectNameInfo, returnLength, NULL)))
				{
					free(objectTypeInfo);
					free(objectNameInfo);
					CloseHandle(dupHandle);
					continue;
				}
			}

			/* Cast our buffer into an UNICODE_STRING. */
			objectName = *(PUNICODE_STRING)objectNameInfo;

			/* Print the information! */
			if (objectName.Length && lstrcmpiW(objectName.Buffer, name) == 0)
			{
				CloseHandle((HANDLE)handle.Handle);
				closed = true;
			}

			free(objectTypeInfo);
			free(objectNameInfo);
			CloseHandle(dupHandle);
		}

		free(handleInfo);
		CloseHandle(processHandle);
		return closed;
	}
}
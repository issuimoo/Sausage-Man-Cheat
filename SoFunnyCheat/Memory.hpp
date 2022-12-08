#pragma once
#include <vector>
#include <Windows.h>
namespace Memory
{		//x32��x64��ʹ��Ntdll��Driver
	enum Type :int
	{
		Ntdll,//�ں�
		Common,//��ͨ
		Driver//����
	};
	class Memory
	{
	public:
		//��ʼ��(��д����)
		void initialize(Type readtype);

		//д�ڴ�
		bool Read(HANDLE hProcess, ULONG64 address, LPVOID IPBUFFER, SIZE_T size);
		//���ڴ�
		bool Write(HANDLE hProcess, ULONG64 address, LPVOID IPBUFFER, SIZE_T size);

		//���ֽ���(���̾��,��ַ)
		BYTE Read_byte(DWORD pid, ULONG64 address);
		//д�ֽ���(���̾��,��ַ,ֵ)
		bool Write_byte(DWORD pid, ULONG64 address, BYTE value);

		//��������(���̾��,��ַ)
		DWORD Read_int(DWORD pid, ULONG64 address);
		//д������(���̾��,��ַ,ֵ)
		bool Write_int(DWORD pid, ULONG64 address, DWORD value);

		//����������(���̾��,��ַ)
		DWORD64 Read_long_int(DWORD pid, ULONG64 address);
		//д��������(���̾��,��ַ,ֵ)
		bool Write_long_int(DWORD pid, ULONG64 address, DWORD64 value);

		//��������(���̾��,��ַ)
		float Read_float(DWORD pid, ULONG64 address);
		//д������(���̾��,��ַ,ֵ)
		bool Write_float(DWORD pid, ULONG64 address, float value);

		//��˫������(���̾��,��ַ)
		double Read_double(DWORD pid, ULONG64 address);
		//д˫������(���̾��,��ַ,ֵ)
		bool Write_double(DWORD pid, ULONG64 address, double value);

		//����������(���̾��,������)
		std::vector<DWORD64> Search_Signatures(DWORD pid, ULONG64 START, ULONG64 END, const char* Signatures);
	private:
		bool initializeNT();
		bool initializeDR();
		void ReadProcessMemorys();
		int Mode;
	};
}
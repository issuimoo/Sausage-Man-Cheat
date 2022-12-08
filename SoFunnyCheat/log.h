#pragma once
#include <Windows.h>
#include <string>
namespace logs
{
	enum Level :int
	{
		L_INFO = 1,//��Ϣ
		L_DEBUG,//����
		L_WARNING,//����
		L_ERROR//����
	};
#define LOG_OUT(M,Char) LOG_OUT_(M,Char,__FILE__,__LINE__)
	//����̨���(ģʽLevel, �����ı�, �Ƿ���, �ļ�, �к�)
	void LOG_OUT_(Level M, const char* Char, std::string code, int line);
	//�򿪿���̨(���� , �Ƿ���Թر�)
	HWND LOG_START(const char* title, bool Close);
	//�رտ���̨()
	void LOG_END();
}
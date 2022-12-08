#pragma once
#include <Windows.h>
#include <chrono>
#include <stdio.h>
#include <iostream>
#include <WinUser.h>
#include <TlHelp32.h>
#include <time.h>
namespace times
{
	struct Time
	{
		int nanosec;
		int microsec;
		int millisec;
		int sec;
		int min;
		int hour;
		int day;
		int mon;
		int year;
		uint64_t timestamp;
	};

	//��ȡʱ��
	Time GetNowTime();
	//��ȡ����
	int GetMilliSec();
	//��ȡ΢��
	int Getmicrosec();
	//��ȡ����
	int Getnanosec();
	//��ȡ��
	int GetSec();
	//��ȡ����
	int GetMin();
	//��ȡСʱ
	int GetHour();
	//��ȡ��
	int GetDay();
	//��ȡ��
	int GetMon();
	//��ȡ��
	int GetYear();
	//��ȡʱ���
	uint64_t GetTimeStamp();

	class Timer
	{
	public:
		//��ʱ��ʼ
		void Timer_Start();
		//��ʱ����
		void Timer_End();
		//��ʱת�ı�
		const char* Timer_string();
		//��ȡʱ��
		Time GetTimer();
	private:
		Time Temp_Time;
		Time Time_Temp;
	};
}
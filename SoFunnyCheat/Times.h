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

	//获取时间
	Time GetNowTime();
	//获取毫秒
	int GetMilliSec();
	//获取微秒
	int Getmicrosec();
	//获取纳秒
	int Getnanosec();
	//获取秒
	int GetSec();
	//获取分钟
	int GetMin();
	//获取小时
	int GetHour();
	//获取天
	int GetDay();
	//获取月
	int GetMon();
	//获取年
	int GetYear();
	//获取时间戳
	uint64_t GetTimeStamp();

	class Timer
	{
	public:
		//计时开始
		void Timer_Start();
		//计时结束
		void Timer_End();
		//计时转文本
		const char* Timer_string();
		//获取时间
		Time GetTimer();
	private:
		Time Temp_Time;
		Time Time_Temp;
	};
}
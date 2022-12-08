#include <Times.h>
#include <Windows.h>
#include <chrono>
#include <stdio.h>
#include <iostream>
#include <WinUser.h>
#include <TlHelp32.h>
#include <time.h>
namespace times
{
	Time GetNowTime()
	{
		Time time;
		time.year = GetYear();
		time.mon = GetMon();
		time.day = GetDay();
		time.hour = GetHour();
		time.min = GetMin();
		time.sec = GetSec();
		time.millisec = GetMilliSec();
		time.microsec = Getmicrosec();
		time.nanosec = Getnanosec();
		time.timestamp = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
		return time;
	}
	//获取毫秒
	int GetMilliSec()
	{
		return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count() % 1000;
	}
	//获取微秒
	int Getmicrosec()
	{
		return std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch()).count() % 1000;
	}
	//获取纳秒
	int Getnanosec()
	{
		return std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now().time_since_epoch()).count() % 1000;
	}
	//获取秒
	int GetSec()
	{
		time_t time_tm = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
		auto time_now = localtime(&time_tm);
		return time_now->tm_sec;
	}
	//获取分钟
	int GetMin()
	{
		time_t time_tm = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
		auto time_now = localtime(&time_tm);
		return time_now->tm_min;
	}
	//获取小时
	int GetHour()
	{
		time_t time_tm = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
		auto time_now = localtime(&time_tm);
		return time_now->tm_hour;
	}
	//获取天
	int GetDay()
	{
		time_t time_tm = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
		auto time_now = localtime(&time_tm);
		return time_now->tm_mday;
	}
	//获取月
	int GetMon()
	{
		time_t time_tm = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
		auto time_now = localtime(&time_tm);
		return time_now->tm_mon + 1;
	}
	//获取年
	int GetYear()
	{
		time_t time_tm = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
		auto time_now = localtime(&time_tm);
		return time_now->tm_year + 1900;
	}
	//获取时间戳
	uint64_t GetTimeStamp()
	{
		return std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
	}

	void Timer::Timer_Start()
	{
		Temp_Time = GetNowTime();
	}
	void Timer::Timer_End()
	{
		Time_Temp = GetNowTime();
		uint64_t timestamp = Time_Temp.timestamp - Temp_Time.timestamp;
		Time_Temp.timestamp = timestamp;
		time_t time_tm = (int)(timestamp / 1000000000);
		//cout << (int)(timestamp / 1000000000) << endl;
		auto time_now = localtime(&time_tm);
		Time_Temp.year = time_now->tm_year - 70;
		Time_Temp.mon = time_now->tm_mon;
		Time_Temp.day = time_now->tm_mday - 1;
		Time_Temp.hour = time_now->tm_hour - 8;
		Time_Temp.min = time_now->tm_min;
		Time_Temp.sec = time_now->tm_sec;
		Time_Temp.nanosec = timestamp % 1000;
		Time_Temp.microsec = (int)(timestamp / 1000000 % 1000);
		Time_Temp.millisec = (int)(timestamp / 1000 % 1000);
	}
	const char* Timer::Timer_string()
	{
		std::string Temp__;
		Temp__ += std::to_string(Time_Temp.year);
		Temp__ += "-";
		Temp__ += std::to_string(Time_Temp.mon);
		Temp__ += "-";
		Temp__ += std::to_string(Time_Temp.day);
		Temp__ += " ";
		Temp__ += std::to_string(Time_Temp.hour);
		Temp__ += ":";
		Temp__ += std::to_string(Time_Temp.min);
		Temp__ += ":";
		Temp__ += std::to_string(Time_Temp.sec);
		Temp__ += " ";
		Temp__ += std::to_string(Time_Temp.millisec);
		Temp__ += ".";
		Temp__ += std::to_string(Time_Temp.microsec);
		Temp__ += std::to_string(Time_Temp.nanosec);
		Temp__ += "ms";
		return Temp__.c_str();
	}
	Time Timer::GetTimer()
	{
		return Time_Temp;
	}
}
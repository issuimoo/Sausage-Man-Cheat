#pragma once
#include <dx-hook.hpp>
#include <process.hpp>
#include <Windows.h>
#include <stdio.h>
#include <iostream>
#include <WinUser.h>
#include <TlHelp32.h>
#include <chrono>
#include <filesystem>
#include <iosfwd>
#include <fstream>
#include <string>
#include <vector>
#include <cstdio>
#include <map>
#include <Text.h>
#include <json/nlohmann/json.hpp>
#include <codecvt>
#include <locale>
#include <utility>
using namespace Text;
using namespace dx_hook;
using namespace Process;
using namespace std;

namespace Cheat
{
	void run();
	void Draws();

	const char* Ver = "1.7.3";
	namespace Memory
	{
		HMODULE event_module;
		HMODULE matrix_module;
		HMODULE NtdllModuleBase;

		INT64 event_Base;
		INT64 matrix_Base;

		enum eventcar_module_Pointer :int
		{
			car_Pointer_L7 = 0x60,
			car_Pointer_Num = 0xC,
			car_Pointer_L8 = 0x8,
			car_x = 0x34,
			car_y = 0x38,
			car_z = 0x3C,
			car_Name_L1 = 0x58,
			car_Name_L2 = 0x8,
			car_Name_L3 = 0xC,
			car_L1 = 0x14,
			car_oil = 0x6C,
			car_oilmax = 0x70,
			car_hp = 0x78,
			car_hpmax = 0x74,
		};

		enum event_module_Pointer : int //TODO:更新更改eventBase
		{
			//eventBase = 0x03917908, //old
			//eventBase = 0x3919974, //old
			eventBase = 0x391ABB0, //new
			event_Pointer_L1 = 0x5C,
			event_Pointer_L2 = 0x14,
			event_Pointer_L3 = 0x10,
			event_Pointer_L4 = 0x8,
			event_Pointer_L5 = 0x54,
			event_Pointer_L6 = 0x18,
			event_Pointer_L7 = 0x2C,
			event_Pointer_L8 = 0x8,
			Pointer_L6_Product = 0x4,
			Pointer_L6_Products = 0x10,
			Pointer_X = 0x118,
			Pointer_Y = 0x11C,
			Pointer_Z = 0x120,
			Pointer_Kills = 0x90,
			Pointer_Healthy = 0x220,
			Pointer_Energy = 0x22C,
			Pointer_Team = 0xF0,
			Pointer_Mate = 0xEC,
			Pointer_Num = 0xC,
			Pointer_DieHealthy = 0x228,
			Pointer_Die = 0x3D8,
			Pointer_state = 0x68,
			Pointer_Name = 0xC,
			Pointer_Name_Pointer = 0xB8,
			Pointer_Name_Long = 0x8,
			Pointer_PlayerSize = 0x30C,
			Pointer_SKESS = 0x3C7,
			Pointer_CONN = 0x3C4,
			Pointer_SW = 0x3C9,
			Pointer_mood = 0x370,
			Pointer_OX = 0x224,
			Pointer_PPBALL = 0x268,
			Pointer_car = 0x154,
			Pointer_WAR = 0x264,
			Pointer_WARNAME = 0xC,
			Pointer_WAR_L1 = 0xD8,
			Pointer_WARARROWNAME_L2 = 0x14,
			Pointer_WARARROWNAME = 0xC,
			Pointer_WARARROWNUM = 0xA8,
			Pointer_WARSPEED = 0xD0,
			Pointer_WARLM = 0x11C,
			Pointer_WARAIM = 0x140,
			Pointer_WARMAX = 0x98,
			Pointer_WARMIN = 0x9C,
			Pointer_WARLR = 0x78,
			Pointer_WARUDMAX = 0x6C,
			Pointer_WARUDMIN = 0x68,
		};

		enum matrix_module_Pointer :int
		{
			matrixBase = 0x12C6558,
			matrix_Pointer_L1 = 0x5C,
			matrix_Pointer_L2 = 0x10,
			matrix_Pointer_L3 = 0x10,
			matrix_Pointer_L4 = 0x8,
			matrix_Pointer_L5 = 0x2CC,
		};
		void GetMem();
	}

	namespace Draw
	{
		struct DrawInfo
		{
			bool C_ShowFPS = true;
			bool C_ShowStatus = true;
			bool C_FPSMove = true;
			bool C_StatusMove = true;
			bool C_fpsQ = true;
		};
		struct CmdLog
		{
			bool C_Show = true;
			bool C_LogDebug = false;
			bool C_EspDebug = false;
		};
		struct ESPSet
		{
			bool C_ShowESP = false;
			bool T3Dbox = false;
			string name = "2D";
			bool Showarrow = true;
			float OutlineThickness = 1;
			float TracerSize = 1;
			float ArrowRadius = 100;
			int Linexy = 1;
			string Linexyname = tf8("顶部").c_str();
			bool C_ShowLine = false;
			bool C_ShowInfo = false;
			bool C_ShowBox = false;
			bool C_ShowMate = false;
			bool C_ShowKnockdown = false;
			int LM = 400;
			bool showwar = false;
		};
		struct AimSet
		{
			bool AimBot = false;
			bool AimMate = false;
			bool autofire = false;
			float autodwRL = 20;
			float autodwUD = 20;
			int time = 50;
			bool AimShowKnockdown = false;
			bool ShowM = false;
			int Range = 50;
			int Key = 16;
			const char* KeyName = "Shift";
			int Hit = 1;
			const char* HitName = "头";
			int AimM = 400;
		};
		struct carSet
		{
			bool show = false;
			int m = 400;
		};
		struct MemSet
		{
			bool IsPlayerSize = false;
			float PlayerSize = 1;
			bool IsHWARSPEED = false;
			float HWARSPEED = 0.1;
			bool IsWARLM = false;
			float WARLM = 1000;
			bool IsWARAIM = false;
			float WARAIM = 1;
			bool IsWAR = false;
			float WARMAX = 1;
			float WARMIN = 1;
			bool IsWARHZLRL = false;
			float WARHZLRLMAX = 1;
			bool IsWARHZLUD = false;
			float WARHZLUDMAX = 1;
			float WARHZLUDMIN = 1;
		};
		class DrawClass
		{
		public:
			void GetJson(string name);
			void OutJson(string name);
			void DeleJson(string name);
			void DrawMainMenu();
			void DrawCheatInfo(DrawInfo Info);
			void AimBot(AimSet Aim, ImDrawList* DrawList);
			void DrawEsp(ESPSet Drawing, ImDrawList* DrawList);
			void DrawCar(carSet set, ImDrawList* DrawList);
		private:
		};
	}
}
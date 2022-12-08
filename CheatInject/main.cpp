// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include <log.h>
#include "main.hpp"
using namespace logs;
DWORD dwMemPro = NULL;
HMODULE MYModuleBase;
HWND HOOKHWND;
bool m_IsBlockingInput;
Cheat::Draw::MemSet E_Memory;
static float values[5000] = { 0 };
static unsigned long values_offset = 0;
float avg_fps = 0.0f;
float low_fps = 9000.0f;
float hig_fps = 0.0f;
unsigned long C_List = 0;
bool C_Customthemes = false;
bool Mem_Show = true;
const char* JsonName = "配置json-1";
Cheat::Draw::DrawClass MENU;
Cheat::Draw::DrawInfo C_ShowInfo;
Cheat::Draw::CmdLog CMDLOG;
Cheat::Draw::AimSet Aim;
Cheat::Draw::ESPSet ESP;
Cheat::Draw::carSet car;
bool ImGuishowcolor = false;
unsigned long event_L10, carevent_L10;
unsigned long NUM, carNUM, width = 0, height = 0;
RECT rect;
void autoc()
{
	while (true)
	{
		if ((Aim.autofire) && GetKeyState(Aim.Key) <= -127 && (Aim.Key != 1))
		{
			mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
			Sleep(1);
			mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
		}
		Sleep(Aim.time);
	}
}
BOOL WINAPI DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		LOG_START("LOG", false);
		LOG_OUT(L_INFO, "注入成功!");
		MYModuleBase = hModule;
		LOG_OUT(L_DEBUG, string("模块句柄:" + to_string((unsigned long)MYModuleBase)).c_str());
		LOG_OUT(L_DEBUG, std::string("PID:" + std::to_string(_getpid())).c_str());
		CreateThreads(&Cheat::run);//启动线程
		CreateThreads(&autoc);
		break;
	}
	return TRUE;
}
void savetheme()
{
	nlohmann::json C_Json;
	string FileName = string("\\RTSStheme.json");
	ofstream Json_Data_out(FileName);
	C_Json = ImGui::GetCurrentStyles(ImGui::GetStyle());
	Json_Data_out << C_Json << endl;
	Json_Data_out.close();
}
void Loadtheme()
{
	nlohmann::json C_Json;
	string FileName = string(("\\RTSStheme.json"));
	ifstream Json_Data(FileName);
	if (Json_Data)
	{
		Json_Data >> C_Json;
		ImGui::GetStyle() = ImGui::OutCurrentStyles(C_Json);
		Json_Data.close();
	}
}
void Cheat::Draw::DrawClass::GetJson(string name)
{
	try
	{
		nlohmann::json C_Json;
		string FileName = string(("\\" + name + ".json"));
		ifstream Json_Data(FileName);
		if (Json_Data)
		{
			Json_Data >> C_Json;
			if (C_Json.find("AimBot") != C_Json.end())Aim.AimBot = C_Json["AimBot"];
			if (C_Json.find("AimM") != C_Json.end())Aim.AimM = C_Json["AimM"];
			if (C_Json.find("AimMate") != C_Json.end())Aim.AimMate = C_Json["AimMate"];
			if (C_Json.find("AimShowKnockdown") != C_Json.end())Aim.AimShowKnockdown = C_Json["AimShowKnockdown"];
			if (C_Json.find("Hit") != C_Json.end())Aim.Hit = C_Json["Hit"];
			if (Aim.Hit == 1) { Aim.HitName = "头部"; }
			if (Aim.Hit == 2) { Aim.HitName = "胸部"; }
			if (C_Json.find("Key") != C_Json.end())Aim.Key = C_Json["Key"];
			if (Aim.Key == 1) { Aim.KeyName = "LButton"; }
			if (Aim.Key == 2) { Aim.KeyName = "RButton"; }
			if (Aim.Key == 16) { Aim.KeyName = "Shift"; }
			if (Aim.Key == 17) { Aim.KeyName = "Ctrl"; }
			if (C_Json.find("Range") != C_Json.end())Aim.Range = C_Json["Range"];
			if (C_Json.find("ShowM") != C_Json.end())Aim.ShowM = C_Json["ShowM"];
			if (C_Json.find("Speed") != C_Json.end())Aim.autodwRL = C_Json["Speed"];
			if (C_Json.find("UDSpeed") != C_Json.end())Aim.autodwUD = C_Json["UDSpeed"];
			if (C_Json.find("C_ShowBox") != C_Json.end())ESP.C_ShowBox = C_Json["C_ShowBox"];
			if (C_Json.find("T3Dbox") != C_Json.end())ESP.T3Dbox = C_Json["T3Dbox"];
			if (ESP.T3Dbox == false) { ESP.name = "2D"; }
			if (ESP.T3Dbox == true) { ESP.name = "3D"; }
			if (C_Json.find("C_ShowESP") != C_Json.end())ESP.C_ShowESP = C_Json["C_ShowESP"];
			if (C_Json.find("Showarrow") != C_Json.end())ESP.Showarrow = C_Json["Showarrow"];
			if (C_Json.find("OutlineThickness") != C_Json.end())ESP.OutlineThickness = C_Json["OutlineThickness"];
			if (C_Json.find("TracerSize") != C_Json.end())ESP.TracerSize = C_Json["TracerSize"];
			if (C_Json.find("ArrowRadius") != C_Json.end())ESP.ArrowRadius = C_Json["ArrowRadius"];
			if (C_Json.find("C_ShowInfo") != C_Json.end())ESP.C_ShowInfo = C_Json["C_ShowInfo"];
			if (C_Json.find("C_ShowKnockdown") != C_Json.end())ESP.C_ShowKnockdown = C_Json["C_ShowKnockdown"];
			if (C_Json.find("C_ShowLine") != C_Json.end())ESP.C_ShowLine = C_Json["C_ShowLine"];
			if (C_Json.find("C_ShowMate") != C_Json.end())ESP.C_ShowMate = C_Json["C_ShowMate"];
			if (C_Json.find("LM") != C_Json.end())ESP.LM = C_Json["LM"];
			if (C_Json.find("time") != C_Json.end())Aim.time = C_Json["time"];
			if (C_Json.find("C_FPSMove") != C_Json.end())C_ShowInfo.C_FPSMove = C_Json["C_FPSMove"];
			if (C_Json.find("C_ShowFPS") != C_Json.end())C_ShowInfo.C_ShowFPS = C_Json["C_ShowFPS"];
			if (C_Json.find("C_ShowStatus") != C_Json.end())C_ShowInfo.C_ShowStatus = C_Json["C_ShowStatus"];
			if (C_Json.find("C_StatusMove") != C_Json.end())C_ShowInfo.C_StatusMove = C_Json["C_StatusMove"];
			if (C_Json.find("Linexy") != C_Json.end())ESP.Linexy = C_Json["Linexy"];
			if (C_Json.find("Linexyname") != C_Json.end())ESP.Linexyname = C_Json["Linexyname"];
			if (C_Json.find("SHOWCAR") != C_Json.end())car.show = C_Json["SHOWCAR"];
			if (C_Json.find("SHOWM") != C_Json.end())car.m = C_Json["SHOWM"];
			if (C_Json.find("IsPlayerSize") != C_Json.end())E_Memory.IsPlayerSize = C_Json["IsPlayerSize"];
			if (C_Json.find("PlayerSize") != C_Json.end())E_Memory.PlayerSize = C_Json["PlayerSize"];
			if (C_Json.find("IsHWARSPEED") != C_Json.end())E_Memory.IsHWARSPEED = C_Json["IsHWARSPEED"];
			if (C_Json.find("HWARSPEED") != C_Json.end())E_Memory.HWARSPEED = C_Json["HWARSPEED"];
			if (C_Json.find("IsWARLM") != C_Json.end())E_Memory.IsWARLM = C_Json["IsWARLM"];
			if (C_Json.find("WARLM") != C_Json.end())E_Memory.WARLM = C_Json["WARLM"];
			if (C_Json.find("IsWARAIM") != C_Json.end())E_Memory.IsWARAIM = C_Json["IsWARAIM"];
			if (C_Json.find("WARAIM") != C_Json.end())E_Memory.WARAIM = C_Json["WARAIM"];
			if (C_Json.find("IsWAR") != C_Json.end())E_Memory.IsWAR = C_Json["IsWAR"];
			if (C_Json.find("WARMAX") != C_Json.end())E_Memory.WARMAX = C_Json["WARMAX"];
			if (C_Json.find("WARMIN") != C_Json.end())E_Memory.WARMIN = C_Json["WARMIN"];
			if (C_Json.find("IsWARHZLRL") != C_Json.end())E_Memory.IsWARHZLRL = C_Json["IsWARHZLRL"];
			if (C_Json.find("WARHZLRLMAX") != C_Json.end())E_Memory.WARHZLRLMAX = C_Json["WARHZLRLMAX"];
			if (C_Json.find("IsWARHZLUD") != C_Json.end())E_Memory.IsWARHZLUD = C_Json["IsWARHZLUD"];
			if (C_Json.find("WARHZLUDMAX") != C_Json.end())E_Memory.WARHZLUDMAX = C_Json["WARHZLUDMAX"];
			if (C_Json.find("WARHZLUDMIN") != C_Json.end())E_Memory.WARHZLUDMIN = C_Json["WARHZLUDMIN"];
			if (C_Json.find("showwar") != C_Json.end())ESP.showwar = C_Json["showwar"];
			if (C_Json.find("FPSQ") != C_Json.end()) C_ShowInfo.C_fpsQ = C_Json["FPSQ"];
			Json_Data.close();
		}
	}
	catch (...)
	{
		if (CMDLOG.C_LogDebug) { LOG_OUT(L_ERROR, string("Error code:" + GetLastError()).c_str()); }
		return;
	}
}
void Cheat::Draw::DrawClass::OutJson(string name)
{
	nlohmann::json C_Json;
	string FileName = string(("\\" + name + ".json"));
	ofstream Json_Data_out(FileName);
	LOG_OUT(L_INFO, "save json");
	C_Json["AimBot"] = Aim.AimBot;
	C_Json["AimM"] = Aim.AimM;
	C_Json["AimMate"] = Aim.AimMate;
	C_Json["AimShowKnockdown"] = Aim.AimShowKnockdown;
	C_Json["Hit"] = Aim.Hit;
	C_Json["Key"] = Aim.Key;
	C_Json["Range"] = Aim.Range;
	C_Json["ShowM"] = Aim.ShowM;
	C_Json["Speed"] = Aim.autodwRL;
	C_Json["UDSpeed"] = Aim.autodwUD;
	C_Json["time"] = Aim.time;
	C_Json["C_ShowBox"] = ESP.C_ShowBox;
	C_Json["T3Dbox"] = ESP.T3Dbox;
	C_Json["C_ShowESP"] = ESP.C_ShowESP;
	C_Json["C_ShowInfo"] = ESP.C_ShowInfo;
	C_Json["C_ShowKnockdown"] = ESP.C_ShowKnockdown;
	C_Json["C_ShowLine"] = ESP.C_ShowLine;
	C_Json["C_ShowMate"] = ESP.C_ShowMate;
	C_Json["LM"] = ESP.LM;
	C_Json["Linexy"] = ESP.Linexy;
	C_Json["Linexyname"] = ESP.Linexyname;
	C_Json["Showarrow"] = ESP.Showarrow;
	C_Json["OutlineThickness"] = ESP.Showarrow;
	C_Json["TracerSize"] = ESP.TracerSize;
	C_Json["ArrowRadius"] = ESP.ArrowRadius;
	C_Json["showwar"] = ESP.showwar;
	C_Json["C_FPSMove"] = C_ShowInfo.C_FPSMove;
	C_Json["C_ShowFPS"] = C_ShowInfo.C_ShowFPS;
	C_Json["C_ShowStatus"] = C_ShowInfo.C_ShowStatus;
	C_Json["C_StatusMove"] = C_ShowInfo.C_StatusMove;
	C_Json["SHOWCAR"] = car.show;
	C_Json["SHOWM"] = car.m;
	C_Json["IsPlayerSize"] = E_Memory.IsPlayerSize;
	C_Json["PlayerSize"] = E_Memory.PlayerSize;
	C_Json["IsHWARSPEED"] = E_Memory.IsHWARSPEED;
	C_Json["HWARSPEED"] = E_Memory.HWARSPEED;
	C_Json["IsWARLM"] = E_Memory.IsWARLM;
	C_Json["WARLM"] = E_Memory.WARLM;
	C_Json["IsWARAIM"] = E_Memory.IsWARAIM;
	C_Json["WARAIM"] = E_Memory.WARAIM;
	C_Json["IsWAR"] = E_Memory.IsWAR;
	C_Json["WARMAX"] = E_Memory.WARMAX;
	C_Json["WARMIN"] = E_Memory.WARMIN;
	C_Json["IsWARHZLRL"] = E_Memory.IsWARHZLRL;
	C_Json["WARHZLRLMAX"] = E_Memory.WARHZLRLMAX;
	C_Json["IsWARHZLUD"] = E_Memory.IsWARHZLUD;
	C_Json["WARHZLUDMAX"] = E_Memory.WARHZLUDMAX;
	C_Json["WARHZLUDMIN"] = E_Memory.WARHZLUDMIN;
	C_Json["FPSQ"] = C_ShowInfo.C_fpsQ;
	LOG_OUT(L_INFO, "save done");
	Json_Data_out << C_Json << endl;
	Json_Data_out.close();
}
void Cheat::Draw::DrawClass::DeleJson(string name)
{
	string FileName = string(("\\" + name + ".json"));
	ifstream Json_Data(FileName);
	if (Json_Data)
	{
		Json_Data.close();
		ofstream Json_Data_out(FileName);
		LOG_OUT(L_INFO, "dele json");
		Json_Data_out << "";
		LOG_OUT(L_INFO, "dele done");
		Json_Data_out.close();
	}
}
namespace Cheat
{
	void run()
	{
		Memory::GetMem();
		LOG_OUT(L_INFO, "等待10秒...");
		Sleep(10000);
		InitD3DHook(D3D11, &Draws, &HOOKHWND, MYModuleBase, 13,"C:\\RTSSHOOK\\imgui.ini");
		Sleep(50);
		ImGui::AnemoTheme();
		Loadtheme();
		MENU.GetJson(JsonName);
	}

	void Draws()
	{
		MENU.DrawMainMenu();
		ImDrawList* DrawList = ImGui::GetBackgroundDrawList();
		MENU.DrawCheatInfo(C_ShowInfo);
		MENU.DrawCar(car, DrawList);
		MENU.AimBot(Aim, DrawList);
		MENU.DrawEsp(ESP, DrawList);
	}
}
namespace Cheat::Memory
{
	void Memory::GetMem()
	{
		LOG_OUT(L_INFO, "等待游戏模块加载...");
		//取模块内存地址
		while ((Memory::event_module == NULL) || (Memory::matrix_module == NULL))
		{
			Memory::event_module = GetModuleHandleA("GameAssembly.dll");
			Memory::matrix_module = GetModuleHandleA("UnityPlayer.dll");
			Sleep(1);//等待模块加载
		}
		//输出内存地址
		LOG_OUT(L_DEBUG, string("GameAssembly.dll:" + to_string((unsigned long)Memory::event_module)).c_str());
		LOG_OUT(L_DEBUG, string("UnityPlayer.dll:" + to_string((unsigned long)Memory::matrix_module)).c_str());
		//加上偏移
		Memory::event_Base = ((unsigned long)event_module + Memory::event_module_Pointer::eventBase);
		Memory::matrix_Base = ((unsigned long)matrix_module + Memory::matrix_module_Pointer::matrixBase);
		LOG_OUT(L_DEBUG, string("玩家结构体基址:" + to_string((unsigned long)Memory::event_Base)).c_str());
		LOG_OUT(L_DEBUG, string("矩阵基址:" + to_string((unsigned long)Memory::matrix_Base)).c_str());
	}
}
ImU32 GetTeamColor(unsigned long Team, unsigned long A)
{
	switch (Team)
	{
	case 0: return D3DCOLOR_RGBA(0, 0, 0, A); break;
	case 1: return D3DCOLOR_RGBA(133, 151, 21, A); break;
	case 2: return D3DCOLOR_RGBA(173, 29, 210, A); break;
	case 3: return D3DCOLOR_RGBA(148, 221, 196, A); break;
	case 4: return D3DCOLOR_RGBA(118, 25, 57, A); break;
	case 5: return D3DCOLOR_RGBA(49, 241, 173, A); break;
	case 6: return D3DCOLOR_RGBA(181, 88, 240, A); break;
	case 7: return D3DCOLOR_RGBA(147, 151, 50, A); break;
	case 8: return D3DCOLOR_RGBA(25, 43, 209, A); break;
	case 9: return D3DCOLOR_RGBA(192, 253, 22, A); break;
	case 10: return D3DCOLOR_RGBA(142, 78, 72, A); break;
	case 11: return D3DCOLOR_RGBA(155, 11, 245, A); break;
	case 12: return D3DCOLOR_RGBA(59, 73, 168, A); break;
	case 13: return D3DCOLOR_RGBA(99, 93, 222, A); break;
	case 14: return D3DCOLOR_RGBA(63, 223, 109, A); break;
	case 15: return D3DCOLOR_RGBA(104, 180, 135, A); break;
	case 16: return D3DCOLOR_RGBA(154, 170, 205, A); break;
	case 17: return D3DCOLOR_RGBA(220, 247, 193, A); break;
	case 18: return D3DCOLOR_RGBA(68, 129, 41, A); break;
	case 19: return D3DCOLOR_RGBA(8, 27, 64, A); break;
	case 20: return D3DCOLOR_RGBA(98, 56, 48, A); break;
	case 21: return D3DCOLOR_RGBA(78, 148, 212, A); break;
	case 22: return D3DCOLOR_RGBA(17, 208, 222, A); break;
	case 23: return D3DCOLOR_RGBA(196, 17, 157, A); break;
	case 24: return D3DCOLOR_RGBA(75, 63, 156, A); break;
	case 25: return D3DCOLOR_RGBA(70, 187, 239, A); break;
	case 26: return D3DCOLOR_RGBA(199, 84, 33, A); break;
	case 27: return D3DCOLOR_RGBA(80, 43, 208, A); break;
	case 28: return D3DCOLOR_RGBA(239, 90, 244, A); break;
	case 29: return D3DCOLOR_RGBA(9, 207, 95, A); break;
	case 30: return D3DCOLOR_RGBA(53, 145, 148, A); break;
	case 31: return D3DCOLOR_RGBA(54, 127, 137, A); break;
	case 32: return D3DCOLOR_RGBA(112, 153, 177, A); break;
	case 33: return D3DCOLOR_RGBA(30, 103, 204, A); break;
	case 34: return D3DCOLOR_RGBA(17, 84, 3, A); break;
	case 35: return D3DCOLOR_RGBA(127, 156, 3, A); break;
	case 36: return D3DCOLOR_RGBA(74, 246, 155, A); break;
	case 37: return D3DCOLOR_RGBA(30, 237, 103, A); break;
	case 38: return D3DCOLOR_RGBA(119, 59, 194, A); break;
	case 39: return D3DCOLOR_RGBA(164, 206, 80, A); break;
	case 40: return D3DCOLOR_RGBA(116, 249, 198, A); break;
	case 41: return D3DCOLOR_RGBA(187, 122, 88, A); break;
	case 42: return D3DCOLOR_RGBA(162, 134, 69, A); break;
	case 43: return D3DCOLOR_RGBA(179, 147, 232, A); break;
	case 44: return D3DCOLOR_RGBA(190, 170, 208, A); break;
	case 45: return D3DCOLOR_RGBA(15, 239, 102, A); break;
	case 46: return D3DCOLOR_RGBA(232, 28, 0, A); break;
	case 47: return D3DCOLOR_RGBA(197, 87, 112, A); break;
	case 48: return D3DCOLOR_RGBA(102, 183, 88, A); break;
	case 49: return D3DCOLOR_RGBA(38, 87, 232, A); break;
	case 50: return D3DCOLOR_RGBA(251, 224, 129, A); break;
	case 51: return D3DCOLOR_RGBA(159, 119, 199, A); break;
	case 52: return D3DCOLOR_RGBA(251, 230, 193, A); break;
	case 53: return D3DCOLOR_RGBA(205, 124, 235, A); break;
	case 54: return D3DCOLOR_RGBA(94, 54, 203, A); break;
	case 55: return D3DCOLOR_RGBA(166, 117, 86, A); break;
	case 56: return D3DCOLOR_RGBA(118, 188, 40, A); break;
	case 57: return D3DCOLOR_RGBA(156, 199, 163, A); break;
	case 58: return D3DCOLOR_RGBA(36, 207, 244, A); break;
	case 59: return D3DCOLOR_RGBA(7, 119, 152, A); break;
	case 60: return D3DCOLOR_RGBA(150, 109, 163, A); break;
	case 61: return D3DCOLOR_RGBA(65, 92, 23, A); break;
	case 62: return D3DCOLOR_RGBA(240, 188, 1, A); break;
	case 63: return D3DCOLOR_RGBA(39, 6, 231, A); break;
	case 64: return D3DCOLOR_RGBA(123, 7, 186, A); break;
	case 65: return D3DCOLOR_RGBA(14, 118, 7, A); break;
	case 66: return D3DCOLOR_RGBA(41, 177, 0, A); break;
	case 67: return D3DCOLOR_RGBA(72, 3, 136, A); break;
	case 68: return D3DCOLOR_RGBA(134, 218, 42, A); break;
	case 69: return D3DCOLOR_RGBA(197, 255, 33, A); break;
	case 70: return D3DCOLOR_RGBA(124, 153, 103, A); break;
	case 71: return D3DCOLOR_RGBA(125, 236, 249, A); break;
	case 72: return D3DCOLOR_RGBA(111, 41, 216, A); break;
	case 73: return D3DCOLOR_RGBA(162, 115, 100, A); break;
	case 74: return D3DCOLOR_RGBA(151, 91, 172, A); break;
	case 75: return D3DCOLOR_RGBA(207, 81, 122, A); break;
	case 76: return D3DCOLOR_RGBA(167, 23, 19, A); break;
	case 77: return D3DCOLOR_RGBA(169, 245, 211, A); break;
	case 78: return D3DCOLOR_RGBA(34, 234, 37, A); break;
	case 79: return D3DCOLOR_RGBA(176, 144, 217, A); break;
	case 80: return D3DCOLOR_RGBA(239, 203, 14, A); break;
	case 81: return D3DCOLOR_RGBA(49, 178, 94, A); break;
	case 82: return D3DCOLOR_RGBA(1, 146, 33, A); break;
	case 83: return D3DCOLOR_RGBA(159, 216, 130, A); break;
	case 84: return D3DCOLOR_RGBA(56, 229, 156, A); break;
	case 85: return D3DCOLOR_RGBA(177, 118, 13, A); break;
	case 86: return D3DCOLOR_RGBA(200, 129, 70, A); break;
	case 87: return D3DCOLOR_RGBA(28, 92, 197, A); break;
	case 88: return D3DCOLOR_RGBA(190, 37, 45, A); break;
	case 89: return D3DCOLOR_RGBA(201, 139, 110, A); break;
	case 90: return D3DCOLOR_RGBA(127, 148, 44, A); break;
	case 91: return D3DCOLOR_RGBA(206, 213, 105, A); break;
	case 92: return D3DCOLOR_RGBA(95, 89, 8, A); break;
	case 93: return D3DCOLOR_RGBA(148, 217, 64, A); break;
	case 94: return D3DCOLOR_RGBA(151, 99, 58, A); break;
	case 95: return D3DCOLOR_RGBA(242, 62, 114, A); break;
	case 96: return D3DCOLOR_RGBA(64, 135, 202, A); break;
	case 97: return D3DCOLOR_RGBA(43, 59, 95, A); break;
	case 98: return D3DCOLOR_RGBA(43, 142, 193, A); break;
	case 99: return D3DCOLOR_RGBA(6, 203, 46, A); break;
	case 100: return D3DCOLOR_RGBA(162, 91, 30, A); break;
	default:return 0; break;
	}return 0;
}
void Getmatrix()
{
	if (GetClientRect(FindWindowA(NULL, "Sausage Man"), &rect)) { width = rect.right - rect.left; height = rect.bottom - rect.top; }
}
void GetPlayEvent()//TODO:unsigned long -> int64
{
	unsigned long event_L2 = 0, event_L1 = 0;
	unsigned long event_L3 = 0, event_L5 = 0, NUM_P = 0;
	unsigned long event_L8 = 0, event_L4 = 0,event_L9 = 0;
	try {
		if (CMDLOG.C_LogDebug) { LOG_OUT(L_DEBUG, string("event_Base:" + to_string(*((unsigned long*)(Cheat::Memory::event_Base)))).c_str()); }
		if ((Cheat::Memory::event_Base > 10000) && (*((unsigned long*)(Cheat::Memory::event_Base)) > 10000)) { event_L1 = *((unsigned long*)(Cheat::Memory::event_Base)) + Cheat::Memory::event_module_Pointer::event_Pointer_L1; if (CMDLOG.C_LogDebug) { LOG_OUT(L_DEBUG, string("event_L1:" + to_string(event_L1)).c_str()); } }
		else { return; }
		if (((event_L1) > 10000) && (*(unsigned long*)(event_L1) > 10000)) { event_L2 = *(unsigned long*)event_L1 + Cheat::Memory::event_module_Pointer::event_Pointer_L2; if (CMDLOG.C_LogDebug) { LOG_OUT(L_DEBUG, string("event_L2:" + to_string(event_L2)).c_str()); } }
		else { return; }
		//TODO:event_L3 need unsigned long? no
		if (((event_L2) > 10000) && (*(unsigned long*)(event_L2) > 10000)) { event_L3 = *(unsigned long*)event_L2 + Cheat::Memory::event_module_Pointer::event_Pointer_L3; if (CMDLOG.C_LogDebug) { LOG_OUT(L_DEBUG, string("event_L3:" + to_string(event_L3)).c_str()); } }
		else { return; }
		if (((event_L3) > 10000) && (*(unsigned long*)(event_L3) > 10000)) { event_L4 = *(unsigned long*)event_L3 + Cheat::Memory::event_module_Pointer::event_Pointer_L4; if (CMDLOG.C_LogDebug) { LOG_OUT(L_DEBUG, string("event_L4:" + to_string(event_L4)).c_str()); } }
		else { return; }
		if (((event_L4) > 10000) && (*(unsigned long*)(event_L4) > 10000)) { event_L5 = *(unsigned long*)event_L4 + Cheat::Memory::event_module_Pointer::event_Pointer_L5; if (CMDLOG.C_LogDebug) { LOG_OUT(L_DEBUG, string("event_L5:" + to_string(event_L5)).c_str()); } }
		else { return; }
		if (((event_L5) > 10000) && (*(unsigned long*)(event_L5) > 10000)) { event_L8 = *(unsigned long*)event_L5 + Cheat::Memory::event_module_Pointer::event_Pointer_L6; if (CMDLOG.C_LogDebug) { LOG_OUT(L_DEBUG, string("event_L8:" + to_string(event_L8)).c_str()); } }
		else { return; }
		if (((event_L8) > 10000) && (*(unsigned long*)(event_L8) > 10000)) { event_L9 = *(unsigned long*)event_L8 + Cheat::Memory::event_module_Pointer::event_Pointer_L7; if (CMDLOG.C_LogDebug) { LOG_OUT(L_DEBUG, string("event_L9:" + to_string(event_L9)).c_str()); } }
		else { return; }//FUCK BUG
		if (((event_L9) > 10000) && (*(unsigned long*)(event_L9) > 10000)) { event_L10 = *(unsigned long*)event_L9 + Cheat::Memory::event_module_Pointer::event_Pointer_L8; if (CMDLOG.C_LogDebug) { LOG_OUT(L_DEBUG, string("event_L10:" + to_string(event_L10)).c_str()); } }
		else { return; }
		if (((event_L4) > 10000) && (*(unsigned long*)(event_L9) > 10000)) { NUM_P = *(unsigned long*)event_L9 + Cheat::Memory::event_module_Pointer::Pointer_Num; if (CMDLOG.C_LogDebug) { LOG_OUT(L_DEBUG, string("event_NUM:" + to_string(NUM_P)).c_str()); } }
		else { return; }
	}
	catch (...) {
		if (CMDLOG.C_LogDebug) {if (CMDLOG.C_LogDebug) {LOG_OUT(L_ERROR, string("Error code:" + GetLastError()).c_str());}}
		NUM = 0;
	}
	if (((NUM_P) > 10000))NUM = *(unsigned long*)NUM_P; else NUM = 0; //FUCK BUG
}
void GetCarEvent()
{
	unsigned long event_L2 = 0, event_L1 = 0;
	unsigned long event_L9 = 0, event_L8 = 0, event_L3 = 0, event_L4 = 0, event_L5 = 0, NUM_P = 0;
	try
	{
		if (CMDLOG.C_LogDebug) { LOG_OUT(L_DEBUG, string("car_Base:" + to_string(*((unsigned long*)(Cheat::Memory::event_Base)))).c_str()); }
		if ((Cheat::Memory::event_Base > 10000) && (*((unsigned long*)(Cheat::Memory::event_Base)) > 10000)) { event_L1 = *((unsigned long*)(Cheat::Memory::event_Base)) + Cheat::Memory::event_module_Pointer::event_Pointer_L1; if (CMDLOG.C_LogDebug) { LOG_OUT(L_DEBUG, string("car_L1:" + to_string(event_L1)).c_str()); } }
		else { return; }
		if (((event_L1) > 10000) && (*(unsigned long*)(event_L1) > 10000)) { event_L2 = *(unsigned long*)event_L1 + Cheat::Memory::event_module_Pointer::event_Pointer_L2; if (CMDLOG.C_LogDebug) { LOG_OUT(L_DEBUG, string("car_L2:" + to_string(event_L2)).c_str()); } }
		else { return; }
		if (((event_L2) > 10000) && (*(unsigned long*)(event_L2) > 10000)) { event_L3 = *(unsigned long*)event_L2 + Cheat::Memory::event_module_Pointer::event_Pointer_L3; if (CMDLOG.C_LogDebug) { LOG_OUT(L_DEBUG, string("car_L3:" + to_string(event_L3)).c_str()); } }
		else { return; }
		if (((event_L3) > 10000) && (*(unsigned long*)(event_L3) > 10000)) { event_L4 = *(unsigned long*)event_L3 + Cheat::Memory::event_module_Pointer::event_Pointer_L4; if (CMDLOG.C_LogDebug) { LOG_OUT(L_DEBUG, string("car_L4:" + to_string(event_L4)).c_str()); } }
		else { return; }
		if (((event_L4) > 10000) && (*(unsigned long*)(event_L4) > 10000)) { event_L5 = *(unsigned long*)event_L4 + Cheat::Memory::event_module_Pointer::event_Pointer_L5; if (CMDLOG.C_LogDebug) { LOG_OUT(L_DEBUG, string("car_L5:" + to_string(event_L5)).c_str()); } }
		else { return; }
		if (((event_L5) > 10000) && (*(unsigned long*)(event_L5) > 10000)) { event_L8 = *(unsigned long*)event_L5 + Cheat::Memory::event_module_Pointer::event_Pointer_L6; if (CMDLOG.C_LogDebug) { LOG_OUT(L_DEBUG, string("car_L8:" + to_string(event_L8)).c_str()); } }
		else { return; }
		if (((event_L8) > 10000) && (*(unsigned long*)(event_L8) > 10000)) { event_L9 = *(unsigned long*)event_L8 + Cheat::Memory::eventcar_module_Pointer::car_Pointer_L7; if (CMDLOG.C_LogDebug) { LOG_OUT(L_DEBUG, string("car_L9:" + to_string(event_L9)).c_str()); } }
		else { return; }
		if (((event_L9) > 10000) && (*(unsigned long*)(event_L9) > 10000)) { carevent_L10 = *(unsigned long*)event_L9 + Cheat::Memory::eventcar_module_Pointer::car_Pointer_L8; if (CMDLOG.C_LogDebug) { LOG_OUT(L_DEBUG, string("car_L10:" + to_string(carevent_L10)).c_str()); } }
		else { return; }
		if (((event_L4) > 10000) && (*(unsigned long*)(event_L9) > 10000)) { NUM_P = *(unsigned long*)event_L9 + Cheat::Memory::eventcar_module_Pointer::car_Pointer_Num; if (CMDLOG.C_LogDebug) { LOG_OUT(L_DEBUG, string("car_NUM:" + to_string(NUM_P)).c_str()); } }
		else { return; }
	}
	catch (...) {
		if (CMDLOG.C_LogDebug) {LOG_OUT(L_ERROR, string("Error code:" + GetLastError()).c_str());}
		carNUM = 0;
	}
	if (((NUM_P) > 10000))  carNUM = *(unsigned long*)NUM_P; else carNUM = 0;
}
void Cheat::Draw::DrawClass::DrawCar(Cheat::Draw::carSet set, ImDrawList* DrawList)
{
	if (!set.show) { return; }
	unsigned long  matrix_L1 = 0, matrix_L2 = 0, matrix_L3 = 0, matrix_L4 = 0, matrix_address = 0, event_Name, event_Name2, event_L8, event_L6 = 0, event_L7 = 0;
	float m_y = 0, m_x = 0, m_z = 0, Healthy = 0, X = 0, Y = 0, Z = 0, ViewW = 0, ViewX = 0, ViewY = 0, ViewY2 = 0, Energy = 0, Healthymax, Energymax;
	if ((carNUM == 0) || (event_L10 < 10000) || (carevent_L10 < 10000)) return;
	try {
		if (((event_L10) > 10000) && (*(unsigned long*)(event_L10) > 10000)) { event_L7 = *(unsigned long*)event_L10 + (Cheat::Memory::event_module_Pointer::Pointer_L6_Products); if (event_L7 < 10000) { return; } }
		if (((event_L7) > 10000) && (*(unsigned long*)(event_L7) > 10000)) { m_x = *(float*)(*(unsigned long*)event_L7 + Cheat::Memory::event_module_Pointer::Pointer_X); m_y = *(float*)(*(unsigned long*)event_L7 + Cheat::Memory::event_module_Pointer::Pointer_Y); m_z = *(float*)(*(unsigned long*)event_L7 + Cheat::Memory::event_module_Pointer::Pointer_Z); }
		if (*((unsigned long*)(Cheat::Memory::matrix_Base)) > 10000) { matrix_L1 = *((unsigned long*)(Cheat::Memory::matrix_Base)) + Cheat::Memory::matrix_module_Pointer::matrix_Pointer_L1; if (*(unsigned long*)matrix_L1 < 10000) { return; } }
		if (((matrix_L1) > 10000) && (*(unsigned long*)(matrix_L1) > 10000)) { matrix_L2 = *(unsigned long*)matrix_L1 + Cheat::Memory::matrix_module_Pointer::matrix_Pointer_L2; if (*(unsigned long*)matrix_L2 < 10000) { return; } }
		if (((matrix_L2) > 10000) && (*(unsigned long*)(matrix_L2) > 10000)) { matrix_L3 = *(unsigned long*)matrix_L2 + Cheat::Memory::matrix_module_Pointer::matrix_Pointer_L3; if (*(unsigned long*)matrix_L3 < 10000) { return; } }
		if (((matrix_L3) > 10000) && (*(unsigned long*)(matrix_L3) > 10000)) { matrix_L4 = *(unsigned long*)matrix_L3 + Cheat::Memory::matrix_module_Pointer::matrix_Pointer_L4; if (*(unsigned long*)matrix_L4 < 10000) { return; } }
		if (((matrix_L4) > 10000) && (*(unsigned long*)(matrix_L4) > 10000)) { matrix_address = *(unsigned long*)matrix_L4 + Cheat::Memory::matrix_module_Pointer::matrix_Pointer_L5; if (matrix_address < 10000) { return; } }
		float matrix[4][4] =
		{
		*(float*)(matrix_address + 0x00),*(float*)(matrix_address + 0x04),*(float*)(matrix_address + 0x08),*(float*)(matrix_address + 0x0C),
		*(float*)(matrix_address + 0x10),*(float*)(matrix_address + 0x14),*(float*)(matrix_address + 0x18),*(float*)(matrix_address + 0x1C),
		*(float*)(matrix_address + 0x20),*(float*)(matrix_address + 0x24),*(float*)(matrix_address + 0x28),*(float*)(matrix_address + 0x2C),
		*(float*)(matrix_address + 0x30),*(float*)(matrix_address + 0x34),*(float*)(matrix_address + 0x38),*(float*)(matrix_address + 0x3C)
		};
		for (unsigned long i = 0; i < carNUM; i++)
		{
			string Names;
			if (((event_L10) > 10000) && (*(unsigned long*)(event_L10) > 10000)) { event_L6 = *(unsigned long*)carevent_L10 + (Cheat::Memory::event_module_Pointer::Pointer_L6_Products + (i * Cheat::Memory::event_module_Pointer::Pointer_L6_Product)); if (event_L6 < 10000)  continue; if (*(unsigned long*)event_L6 < 10000)  continue; }if (event_L6 < 10000) continue;  if (*(unsigned long*)event_L6 < 10000) continue;
			if ((event_L6 > 10000) && (*(unsigned long*)event_L6 > 10000)) { event_L8 = *(unsigned long*)(*(unsigned long*)event_L6 + Cheat::Memory::eventcar_module_Pointer::car_L1); }
			if ((event_L8 > 10000) && (*(unsigned long*)event_L8 > 10000)) { Healthy = *(float*)(event_L8 + Cheat::Memory::eventcar_module_Pointer::car_hp); }if (Healthy < 0) continue;
			if ((event_L8 > 10000) && (*(unsigned long*)event_L8 > 10000)) { Energy = *(float*)(event_L8 + Cheat::Memory::eventcar_module_Pointer::car_oil); }
			if ((event_L8 > 10000) && (*(unsigned long*)event_L8 > 10000)) { Energymax = *(float*)(event_L8 + Cheat::Memory::eventcar_module_Pointer::car_oilmax); }
			if ((event_L8 > 10000) && (*(unsigned long*)event_L8 > 10000)) { Healthymax = *(float*)(event_L8 + Cheat::Memory::eventcar_module_Pointer::car_hpmax); }

			if ((event_L6 > 10000) && (*(unsigned long*)event_L6 > 10000)) {
				event_Name = *(unsigned long*)((*(unsigned long*)event_L6) + Cheat::Memory::eventcar_module_Pointer::car_Name_L1);
				if ((event_Name > 10000))
				{
					event_Name2 = *(unsigned long*)(event_Name + Cheat::Memory::eventcar_module_Pointer::car_Name_L2);
					event_Name2 = (event_Name2 + Cheat::Memory::eventcar_module_Pointer::car_Name_L3);
					if ((event_Name2 > 10000))
					{
						unsigned long i3 = 0;
						wchar_t names[15];
						try {
							for (unsigned long i2 = 0; i2 < 15; i2++)
							{
								names[i2] = *((wchar_t*)(event_Name2 + i3));
								i3 += 2;
							}
						}
						catch (...) {
							return;
						}
						wstring Name = names;
						Names = wstring2utf8string(Name);
					}
				}
			}

			string name = Names;
			if ((event_L6 > 10000) && (*(unsigned long*)event_L6 > 10000)) { X = *(float*)(*(unsigned long*)event_L6 + Cheat::Memory::eventcar_module_Pointer::car_x); Y = *(float*)(*(unsigned long*)event_L6 + Cheat::Memory::eventcar_module_Pointer::car_y); Z = *(float*)(*(unsigned long*)event_L6 + Cheat::Memory::eventcar_module_Pointer::car_z); }
			if (Names == tf8("Jeep")) { name = tf8("吉普车"); }
			if (Names == tf8("JetCar")) { name = tf8("飞车"); }
			if (Names == tf8("Buggy")) { name = tf8("蹦蹦车"); }
			if (Names == tf8("UFO")) { name = tf8("小飞碟"); }
			if (Names == tf8("Peterosaur")) { name = tf8("飘飘龙"); }
			if (Names == tf8("Triceratops")) { name = tf8("憨憨龙"); }
			if (Names == tf8("Kayak")) { name = tf8("快艇"); }
			if (Names == tf8("Raptors")) { name = tf8("奔奔龙"); }
			if (Names == tf8("Dragon")) { name = tf8("呆呆龙"); }
			if (Names == tf8("TRexKing")) { name = tf8("凶凶龙王"); }
			if (Names == tf8("ArmoredBus")) { name = tf8("装甲巴士"); }
			if (Names == tf8("PonyVehicle")) { name = tf8("小马"); }
			if (Names == tf8("PoseidonShark")) { name = tf8("鲨鱼"); }
			if (Names == tf8("Machine_Carrier")) { name = tf8("机甲"); }
			if ((sqrt(pow(X - m_x, 2) + pow(Z - m_z, 2)) > set.m)) { continue; }
			float ViewWs = matrix[0][3] * X + matrix[1][3] * Y + matrix[2][3] * Z + matrix[3][3]; if (ViewWs < 0) { continue; }; ViewW = 1 / ViewWs;
			ViewX = width / 2 + (matrix[0][0] * X + matrix[1][0] * Y + matrix[2][0] * Z + matrix[3][0]) * ViewW * width / 2;
			ViewY = height / 2 - (matrix[0][1] * X + matrix[1][1] * Y + matrix[2][1] * Z + matrix[3][1]) * ViewW * height / 2;
			DrawList->AddRectFilled(ImVec2(ViewX - 2, ViewY - 2), ImVec2(ViewX + 106, ViewY + 55), ImColor(0, 0, 0, 80), 0, 0);
			ImGui::DrawTextWithOutline(DrawList, ImVec2(ViewX, ViewY), name.c_str(), ImColor(255, 255, 255, 255), 1, ImGui::OutlineSide::All, ImColor(0, 0, 0, 255));
			ImGui::DrawTextWithOutline(DrawList, ImVec2(ViewX, ViewY + 13), string(tf8("血量:") + " [" + to_string((unsigned long)Healthy) + "/" + to_string((unsigned long)Healthymax) + "]").c_str(), ImColor(255, 177, 61, 255), 1, ImGui::OutlineSide::All, ImColor(0, 0, 0, 255));
			ImGui::DrawTextWithOutline(DrawList, ImVec2(ViewX, ViewY + 26), string(tf8("油量:") + " [" + to_string((unsigned long)Energy) + "/" + to_string((unsigned long)Energymax) + "]").c_str(), ImColor(255, 177, 61, 255), 1, ImGui::OutlineSide::All, ImColor(0, 0, 0, 255));
			ImGui::DrawTextWithOutline(DrawList, ImVec2(ViewX, ViewY + 39), string(tf8("距离:") + " [" + to_string((unsigned long)sqrt(pow(X - m_x, 2) + pow(Y - m_y, 2) + pow(Z - m_z, 2))) + "]M").c_str(), ImColor(255, 177, 61, 255), 1, ImGui::OutlineSide::All, ImColor(0, 0, 0, 255));
		}
	}
	catch (...) {
		if (CMDLOG.C_LogDebug) {LOG_OUT(L_ERROR, string("Error code:" + GetLastError()).c_str());}
		return;
	}
}
void Cheat::Draw::DrawClass::DrawEsp(Cheat::Draw::ESPSet Drawing, ImDrawList* DrawList)
{
	if (!Drawing.C_ShowESP) { return; }
	unsigned long  WAR, matrix_L1 = 0, matrix_L2 = 0, matrix_L3 = 0, matrix_L4 = 0, matrix_address = 0, event_Name, event_Name2, event_L6 = 0, event_L7 = 0;
	unsigned long m_team, NameLong, Team, NNUM = 0, Die = 0, Mate = 0, state = 0, kills = 0, BH = 0, EVENT = 0, BOT = 0;
	float mood = 0, m_y = 0, m_x = 0, m_z = 0, Healthy = 0, DieHealthy = 0, X = 0, Y = 0, Z = 0, ViewW = 0, ViewX = 0, ViewY = 0, ViewY2 = 0, BoxW = 0, BoxH = 0, BoxL = 0, BoxT = 0, Energy = 0;
	if (NUM == 0) return;
	try
	{
		if (((event_L10) > 10000) && (*(unsigned long*)(event_L10) > 10000)) { event_L7 = *(unsigned long*)event_L10 + (Cheat::Memory::event_module_Pointer::Pointer_L6_Products); if (event_L7 < 10000) { return; } }
		if (((event_L7) > 10000) && (*(unsigned long*)(event_L7) > 10000)) { m_x = *(float*)(*(unsigned long*)event_L7 + Cheat::Memory::event_module_Pointer::Pointer_X); m_y = *(float*)(*(unsigned long*)event_L7 + Cheat::Memory::event_module_Pointer::Pointer_Y); m_z = *(float*)(*(unsigned long*)event_L7 + Cheat::Memory::event_module_Pointer::Pointer_Z); }
		NNUM = 0;
		if (*((unsigned long*)(Cheat::Memory::matrix_Base)) > 10000) { matrix_L1 = *((unsigned long*)(Cheat::Memory::matrix_Base)) + Cheat::Memory::matrix_module_Pointer::matrix_Pointer_L1; if (*(unsigned long*)matrix_L1 < 10000) { return; } }
		if (((matrix_L1) > 10000) && (*(unsigned long*)(matrix_L1) > 10000)) { matrix_L2 = *(unsigned long*)matrix_L1 + Cheat::Memory::matrix_module_Pointer::matrix_Pointer_L2; if (*(unsigned long*)matrix_L2 < 10000) { return; } }
		if (((matrix_L2) > 10000) && (*(unsigned long*)(matrix_L2) > 10000)) { matrix_L3 = *(unsigned long*)matrix_L2 + Cheat::Memory::matrix_module_Pointer::matrix_Pointer_L3; if (*(unsigned long*)matrix_L3 < 10000) { return; } }
		if (((matrix_L3) > 10000) && (*(unsigned long*)(matrix_L3) > 10000)) { matrix_L4 = *(unsigned long*)matrix_L3 + Cheat::Memory::matrix_module_Pointer::matrix_Pointer_L4; if (*(unsigned long*)matrix_L4 < 10000) { return; } }
		if (((matrix_L4) > 10000) && (*(unsigned long*)(matrix_L4) > 10000)) { matrix_address = *(unsigned long*)matrix_L4 + Cheat::Memory::matrix_module_Pointer::matrix_Pointer_L5; if (matrix_address < 10000) { return; } }
		float matrix[4][4] =
		{
		*(float*)(matrix_address + 0x00),*(float*)(matrix_address + 0x04),*(float*)(matrix_address + 0x08),*(float*)(matrix_address + 0x0C),
		*(float*)(matrix_address + 0x10),*(float*)(matrix_address + 0x14),*(float*)(matrix_address + 0x18),*(float*)(matrix_address + 0x1C),
		*(float*)(matrix_address + 0x20),*(float*)(matrix_address + 0x24),*(float*)(matrix_address + 0x28),*(float*)(matrix_address + 0x2C),
		*(float*)(matrix_address + 0x30),*(float*)(matrix_address + 0x34),*(float*)(matrix_address + 0x38),*(float*)(matrix_address + 0x3C)
		};
		for (unsigned long i = 0; i < NUM; i++)
		{
			string Names;
			string WARNames = tf8("空手").c_str();
			string WARARNames = tf8("").c_str();
			if (i == 0) { continue; }//TODO: unsigned long?
			if (((event_L10) > 10000) && (*(unsigned long*)(event_L10) > 10000)) { event_L6 = *(unsigned long*)event_L10 + (Cheat::Memory::event_module_Pointer::Pointer_L6_Products + (i * Cheat::Memory::event_module_Pointer::Pointer_L6_Product)); if (event_L6 < 10000)  continue; if (*(unsigned long*)event_L6 < 10000)  continue; }if (event_L6 < 10000) continue;  if (*(unsigned long*)event_L6 < 10000) continue;
			if ((event_L6 > 10000) && (*(unsigned long*)event_L6 > 10000)) { Die = *(unsigned long*)(*(unsigned long*)event_L6 + Cheat::Memory::event_module_Pointer::Pointer_Die); if (Die > 0)  continue; }
			if ((event_L6 > 10000) && (*(unsigned long*)event_L6 > 10000)) { Healthy = *(float*)(*(unsigned long*)event_L6 + Cheat::Memory::event_module_Pointer::Pointer_Healthy); if (Healthy < 0 || Healthy > 100)  continue; }
			if ((event_L6 > 10000) && (*(unsigned long*)event_L6 > 10000)) { DieHealthy = *(float*)((*(unsigned long*)event_L6 + Cheat::Memory::event_module_Pointer::Pointer_DieHealthy)); if (DieHealthy < 0 || DieHealthy > 100)  continue; }if (DieHealthy == 0) { continue; }//TODO:Fix Die has Box
			if ((event_L6 > 10000) && (*(unsigned long*)event_L6 > 10000)) { Team = *(unsigned long*)(*(unsigned long*)event_L6 + Cheat::Memory::event_module_Pointer::Pointer_Team); }
			if ((event_L6 > 10000) && (*(unsigned long*)event_L6 > 10000)) { Mate = *(unsigned long*)(*(unsigned long*)event_L6 + Cheat::Memory::event_module_Pointer::Pointer_Mate); if ((Mate > 0) && !ESP.C_ShowMate)  continue; }
			if ((event_L6 > 10000) && (*(unsigned long*)event_L6 > 10000)) { state = *(unsigned long*)(*(unsigned long*)event_L6 + Cheat::Memory::event_module_Pointer::Pointer_state); }
			if ((event_L6 > 10000) && (*(unsigned long*)event_L6 > 10000)) { kills = *(unsigned long*)(*(unsigned long*)event_L6 + Cheat::Memory::event_module_Pointer::Pointer_Kills); }
			if ((event_L6 > 10000) && (*(unsigned long*)event_L6 > 10000)) { Energy = *(float*)(*(unsigned long*)event_L6 + Cheat::Memory::event_module_Pointer::Pointer_Energy); }
			if ((event_L6 > 10000) && (*(unsigned long*)event_L6 > 10000)) { mood = *(float*)(*(unsigned long*)event_L6 + Cheat::Memory::event_module_Pointer::Pointer_mood); }if (DieHealthy == 0) { continue; }
			if ((event_L6 > 10000) && (*(unsigned long*)event_L6 > 10000)) { if (*(unsigned long*)(*(unsigned long*)event_L6 + Cheat::Memory::event_module_Pointer::Pointer_car) != 0) { state = 9; }if (*(unsigned long*)(*(unsigned long*)event_L6 + Cheat::Memory::event_module_Pointer::Pointer_PPBALL) != 0) { state = 8; }if (*(unsigned long*)(*(unsigned long*)event_L6 + Cheat::Memory::event_module_Pointer::Pointer_SKESS) == 1) { state = 5; }if (*(unsigned long*)(*(unsigned long*)event_L6 + Cheat::Memory::event_module_Pointer::Pointer_CONN) == 1) { state = 6; } if (*(unsigned long*)(*(unsigned long*)event_L6 + Cheat::Memory::event_module_Pointer::Pointer_SW) == 1) { state = 7; } }
			
			if ((event_L6 > 10000) && (*(unsigned long*)event_L6 > 10000)) {
				event_Name = *(unsigned long*)((*(unsigned long*)event_L6) + Cheat::Memory::event_module_Pointer::Pointer_Name_Pointer);
				if ((event_Name > 10000))
				{
					event_Name2 = (event_Name + Cheat::Memory::event_module_Pointer::Pointer_Name);
					NameLong = *(unsigned long*)(event_Name + Cheat::Memory::event_module_Pointer::Pointer_Name_Long);
					if ((event_Name2 > 10000))
					{
						unsigned long i3 = 0;
						wchar_t name[12];
						for (unsigned long i2 = 0; i2 < 12; i2++)
						{
							name[i2] = *((wchar_t*)(event_Name2 + i3));
							i3 += 2;
						}
						wstring Name = name;
						Names = wstring2utf8string(Name);
					}
				}
			}
			
			if ((event_L6 > 10000) && (*(unsigned long*)event_L6 > 10000)) { WAR = (*(unsigned long*)event_L6 + Cheat::Memory::event_module_Pointer::Pointer_WAR); }
			if ((WAR > 10000) && (*(unsigned long*)WAR > 10000)) {
				unsigned long WAR_Name = (*(unsigned long*)WAR + Cheat::Memory::event_module_Pointer::Pointer_WARNAME);
				if ((WAR_Name > 10000))
				{
					unsigned long WAR_Name2 = (*(unsigned long*)WAR_Name + Cheat::Memory::event_module_Pointer::Pointer_WARNAME);
					if ((WAR_Name2 > 10000))
					{
						int i4 = 0;
						wchar_t WARnamess[12];
						for (int i5 = 0; i5 < 12; i5++)
						{
							WARnamess[i5] = *((wchar_t*)(WAR_Name2 + i4));
							i4 += 2;
						}
						wstring WARName = WARnamess;
						WARNames = wstring2utf8string(WARName);
					}
				}
			}

			if ((WAR > 10000) && (*(unsigned long*)WAR > 10000)) {
				unsigned long WAR_Name = (*(unsigned long*)WAR + Cheat::Memory::event_module_Pointer::Pointer_WAR_L1);
				WAR_Name = (*(unsigned long*)WAR_Name + Cheat::Memory::event_module_Pointer::Pointer_WARARROWNAME_L2);
				if ((WAR_Name > 10000))
				{
					unsigned long WAR_Name2 = (*(unsigned long*)WAR_Name + Cheat::Memory::event_module_Pointer::Pointer_WARARROWNAME);
					if ((WAR_Name2 > 10000))
					{
						int i4 = 0;
						wchar_t WARnamess[13];
						for (int i5 = 0; i5 < 13; i5++)
						{
							WARnamess[i5] = *((wchar_t*)(WAR_Name2 + i4));
							i4 += 2;
						}
						wstring WARName = WARnamess;
						WARARNames = wstring2utf8string(WARName);
					}
				}
			}

			if ((event_L6 > 10000) && (*(unsigned long*)event_L6 > 10000)) { X = *(float*)(*(unsigned long*)event_L6 + Cheat::Memory::event_module_Pointer::Pointer_X); Y = *(float*)(*(unsigned long*)event_L6 + Cheat::Memory::event_module_Pointer::Pointer_Y); Z = *(float*)(*(unsigned long*)event_L6 + Cheat::Memory::event_module_Pointer::Pointer_Z); }
			if ((sqrt(pow(X - m_x, 2) + pow(Z - m_z, 2)) < 0)) { continue; }
			if ((sqrt(pow(X - m_x, 2) + pow(Z - m_z, 2)) > Drawing.LM)) { continue; }
			if ((Healthy == 0) && (DieHealthy != 0)) { state = 4; }
			if ((state == 4) && (!Drawing.C_ShowKnockdown)) { continue; }
			if (!(Mate > 0)) { NNUM++; }
			if ((Team > 0) && (Mate == 0)) { EVENT++; };
			if ((Team == 0) && (Mate == 0)) { BOT++; };
			float ViewWs = matrix[0][3] * X + matrix[1][3] * Y + matrix[2][3] * Z + matrix[3][3]; if (ViewWs < 0) { if ((Mate == 0)) { BH++; } continue; }; ViewW = 1 / ViewWs;
			ViewX = width / 2 + (matrix[0][0] * X + matrix[1][0] * Y + matrix[2][0] * Z + matrix[3][0]) * ViewW * width / 2;
			float size = 0, ball = 0;
			if (E_Memory.PlayerSize < 1.5)
			{
				size = E_Memory.PlayerSize / 2;
			}
			if (E_Memory.PlayerSize > 1.5)
			{
				size = E_Memory.PlayerSize / 1.1;
			}
			if (state == 8)
			{
				ball = 0.9;
			}
			if (E_Memory.IsPlayerSize)
			{
				ViewY = height / 2 - (matrix[0][1] * X + matrix[1][1] * (Y + 1.75 + ball + size) + matrix[2][1] * Z + matrix[3][1]) * ViewW * height / 2;
			}
			else
			{
				ViewY = height / 2 - (matrix[0][1] * X + matrix[1][1] * (Y + 1.75 + ball) + matrix[2][1] * Z + matrix[3][1]) * ViewW * height / 2;
			}
			ViewY2 = height / 2 - (matrix[0][1] * X + matrix[1][1] * Y + matrix[2][1] * Z + matrix[3][1]) * ViewW * height / 2;
			ViewY2 -= ViewY; BoxW = ViewY2 / 2; BoxL = ViewX - BoxW / 2; BoxH = ViewY2; BoxT = ViewY;
			if (Mate == 0 && Drawing.Showarrow) { CALC::DrawOffscreenArrows(ViewX, ViewY, ViewW, GetTeamColor(Team, 100), ESP.OutlineThickness, ESP.TracerSize, ESP.ArrowRadius); }
			//DrawList->AddText(ImVec2(ViewX, ViewY), D3DCOLOR_RGBA(22, 50, 179, 255), string("E_X:" + to_string(X) + " E_Y:" + to_string(Y) + " E_Z:" + to_string(Z)).c_str());
			if (((ViewX > 0) && (ViewX < 5000)) && ((ViewY > 0) && (ViewY < 5000)) && (ViewY2 > 0) && (ViewY2 < 5000)) {
				if (Drawing.C_ShowBox && (Drawing.T3Dbox)) {
					unsigned long T3Dbox1, T3Dbox2, T3Dbox3, T3Dbox4, T3Dbox5, T3Dbox6, T3Dbox7, T3Dbox8, T3Dbox1X, T3Dbox2X, T3Dbox3X, T3Dbox4X, T3Dbox5X, T3Dbox6X, T3Dbox7X, T3Dbox8X;
					if (E_Memory.IsPlayerSize)
					{
						T3Dbox1 = height / 2 - (matrix[0][1] * (X + 0.5) + matrix[1][1] * (Y + 1.75 + ball + size) + matrix[2][1] * (Z + 0.5) + matrix[3][1]) * ViewW * height / 2;
						T3Dbox1X = width / 2 + (matrix[0][0] * (X + 0.5) + matrix[1][0] * (Y + 1.75 + ball + size) + matrix[2][0] * (Z + 0.5) + matrix[3][0]) * ViewW * width / 2;
					}
					else
					{
						T3Dbox1 = height / 2 - (matrix[0][1] * (X + 0.5) + matrix[1][1] * (Y + 1.75) + matrix[2][1] * (Z + 0.5) + matrix[3][1]) * ViewW * height / 2;
						T3Dbox1X = width / 2 + (matrix[0][0] * (X + 0.5) + matrix[1][0] * (Y + 1.75) + matrix[2][0] * (Z + 0.5) + matrix[3][0]) * ViewW * width / 2;
					}
					if (E_Memory.IsPlayerSize)
					{
						T3Dbox2 = height / 2 - (matrix[0][1] * (X - 0.5) + matrix[1][1] * (Y + 1.75 + ball + size) + matrix[2][1] * (Z - 0.5) + matrix[3][1]) * ViewW * height / 2;
						T3Dbox2X = width / 2 + (matrix[0][0] * (X - 0.5) + matrix[1][0] * (Y + 1.75 + ball + size) + matrix[2][0] * (Z - 0.5) + matrix[3][0]) * ViewW * width / 2;
					}
					else
					{
						T3Dbox2 = height / 2 - (matrix[0][1] * (X - 0.5) + matrix[1][1] * (Y + 1.75) + matrix[2][1] * (Z - 0.5) + matrix[3][1]) * ViewW * height / 2;
						T3Dbox2X = width / 2 + (matrix[0][0] * (X - 0.5) + matrix[1][0] * (Y + 1.75) + matrix[2][0] * (Z - 0.5) + matrix[3][0]) * ViewW * width / 2;
					}
					if (E_Memory.IsPlayerSize)
					{
						T3Dbox3 = height / 2 - (matrix[0][1] * (X + 0.5) + matrix[1][1] * (Y + 1.75 + ball + size) + matrix[2][1] * (Z - 0.5) + matrix[3][1]) * ViewW * height / 2;
						T3Dbox3X = width / 2 + (matrix[0][0] * (X + 0.5) + matrix[1][0] * (Y + 1.75 + ball + size) + matrix[2][0] * (Z - 0.5) + matrix[3][0]) * ViewW * width / 2;
					}
					else
					{
						T3Dbox3 = height / 2 - (matrix[0][1] * (X + 0.5) + matrix[1][1] * (Y + 1.75) + matrix[2][1] * (Z - 0.5) + matrix[3][1]) * ViewW * height / 2;
						T3Dbox3X = width / 2 + (matrix[0][0] * (X + 0.5) + matrix[1][0] * (Y + 1.75) + matrix[2][0] * (Z - 0.5) + matrix[3][0]) * ViewW * width / 2;
					}
					if (E_Memory.IsPlayerSize)
					{
						T3Dbox4 = height / 2 - (matrix[0][1] * (X - 0.5) + matrix[1][1] * (Y + 1.75 + ball + size) + matrix[2][1] * (Z + 0.5) + matrix[3][1]) * ViewW * height / 2;
						T3Dbox4X = width / 2 + (matrix[0][0] * (X - 0.5) + matrix[1][0] * (Y + 1.75 + ball + size) + matrix[2][0] * (Z + 0.5) + matrix[3][0]) * ViewW * width / 2;
					}
					else
					{
						T3Dbox4 = height / 2 - (matrix[0][1] * (X - 0.5) + matrix[1][1] * (Y + 1.75) + matrix[2][1] * (Z + 0.5) + matrix[3][1]) * ViewW * height / 2;
						T3Dbox4X = width / 2 + (matrix[0][0] * (X - 0.5) + matrix[1][0] * (Y + 1.75) + matrix[2][0] * (Z + 0.5) + matrix[3][0]) * ViewW * width / 2;
					}
					T3Dbox5 = T3Dbox1 + BoxH;
					T3Dbox5X = width / 2 + (matrix[0][0] * (X + 0.5) + matrix[1][0] * (Y)+matrix[2][0] * (Z + 0.5) + matrix[3][0]) * ViewW * width / 2;
					T3Dbox6 = T3Dbox2 + BoxH;
					T3Dbox6X = width / 2 + (matrix[0][0] * (X - 0.5) + matrix[1][0] * (Y)+matrix[2][0] * (Z - 0.5) + matrix[3][0]) * ViewW * width / 2;
					T3Dbox7 = T3Dbox3 + BoxH;
					T3Dbox7X = width / 2 + (matrix[0][0] * (X + 0.5) + matrix[1][0] * (Y)+matrix[2][0] * (Z - 0.5) + matrix[3][0]) * ViewW * width / 2;
					T3Dbox8 = T3Dbox4 + BoxH;
					T3Dbox8X = width / 2 + (matrix[0][0] * (X - 0.5) + matrix[1][0] * (Y)+matrix[2][0] * (Z + 0.5) + matrix[3][0]) * ViewW * width / 2;
					if (((T3Dbox1 > 0) && (T3Dbox1 < 5000)) && ((T3Dbox1X > 0) && (T3Dbox1X < 5000))) {
						DrawList->AddLine(ImVec2(T3Dbox1X, T3Dbox1), ImVec2(T3Dbox3X, T3Dbox3), GetTeamColor(Team, 200), 1);
						DrawList->AddLine(ImVec2(T3Dbox5X, T3Dbox5), ImVec2(T3Dbox7X, T3Dbox7), GetTeamColor(Team, 200), 1);
						DrawList->AddLine(ImVec2(T3Dbox2X, T3Dbox2), ImVec2(T3Dbox4X, T3Dbox4), GetTeamColor(Team, 200), 1);
						DrawList->AddLine(ImVec2(T3Dbox6X, T3Dbox6), ImVec2(T3Dbox8X, T3Dbox8), GetTeamColor(Team, 200), 1);

						DrawList->AddLine(ImVec2(T3Dbox2X, T3Dbox2), ImVec2(T3Dbox3X, T3Dbox3), GetTeamColor(Team, 200), 1);
						DrawList->AddLine(ImVec2(T3Dbox4X, T3Dbox4), ImVec2(T3Dbox1X, T3Dbox1), GetTeamColor(Team, 200), 1);
						DrawList->AddLine(ImVec2(T3Dbox6X, T3Dbox6), ImVec2(T3Dbox7X, T3Dbox7), GetTeamColor(Team, 200), 1);
						DrawList->AddLine(ImVec2(T3Dbox8X, T3Dbox8), ImVec2(T3Dbox5X, T3Dbox5), GetTeamColor(Team, 200), 1);

						DrawList->AddLine(ImVec2(T3Dbox1X, T3Dbox1), ImVec2(T3Dbox5X, T3Dbox5), GetTeamColor(Team, 200), 1);
						DrawList->AddLine(ImVec2(T3Dbox2X, T3Dbox2), ImVec2(T3Dbox6X, T3Dbox6), GetTeamColor(Team, 200), 1);
						DrawList->AddLine(ImVec2(T3Dbox3X, T3Dbox3), ImVec2(T3Dbox7X, T3Dbox7), GetTeamColor(Team, 200), 1);
						DrawList->AddLine(ImVec2(T3Dbox4X, T3Dbox4), ImVec2(T3Dbox8X, T3Dbox8), GetTeamColor(Team, 200), 1);
					}
				}
				if (Drawing.C_ShowBox && !(Drawing.T3Dbox)) { DrawList->AddRect(ImVec2(BoxL, BoxT), ImVec2(BoxL + BoxW, BoxT + BoxH), GetTeamColor(Team, 200), 0, 0, 1); }
				if (Drawing.C_ShowLine && (Drawing.Linexy == 1)) { DrawList->AddLine(ImVec2(width / 2, 0), ImVec2((BoxL + BoxW / 2), BoxT), GetTeamColor(Team, 200), 1); }
				if (Drawing.C_ShowLine && (Drawing.Linexy == 2)) { DrawList->AddLine(ImVec2(width / 2, height / 2), ImVec2((BoxL + BoxW / 2), BoxT), GetTeamColor(Team, 200), 1); }
				if (Drawing.C_ShowLine && (Drawing.Linexy == 3)) { DrawList->AddLine(ImVec2(width / 2, height), ImVec2((BoxL + BoxW / 2), BoxT), GetTeamColor(Team, 200), 1); }
				if (Drawing.C_ShowInfo)
				{
					DrawList->AddRectFilled(ImVec2((BoxL + BoxW / 2) - 77, BoxT - 35), ImVec2(((BoxL + BoxW / 2) - 77) + 154, (BoxT - 35) + 20), GetTeamColor(Team, 100), 0, 0);
					DrawList->AddRectFilled(ImVec2((BoxL + BoxW / 2) - 77, BoxT - 35), ImVec2(((BoxL + BoxW / 2) - 77) + 25, (BoxT - 35) + 20), GetTeamColor(Team, 120), 0, 0);
					DrawList->AddRectFilled(ImVec2((BoxL + BoxW / 2) + 50, BoxT - 35), ImVec2(((BoxL + BoxW / 2) + 50) + 27, (BoxT - 35) + 20), GetTeamColor(Team, 120), 0, 0);
					if ((Team == 0) && (Mate == 0)) { ImGui::DrawTextWithOutline(DrawList, ImVec2((BoxL + BoxW / 2) - 75, BoxT - 32), tf8("人机").c_str(), ImColor(255, 255, 255, 255), 1, ImGui::OutlineSide::All, ImColor(0, 0, 0, 255));}
					if ((Mate == 0) && (Team != 0)) { ImGui::DrawTextWithOutline(DrawList, ImVec2((BoxL + BoxW / 2) - 75, BoxT - 32), string(to_string((unsigned long)Team)).c_str(), ImColor(255, 255, 255, 255), 1, ImGui::OutlineSide::All, ImColor(0, 0, 0, 255));}
					if ((Mate > 0)) { ImGui::DrawTextWithOutline(DrawList, ImVec2((BoxL + BoxW / 2) - 75, BoxT - 32), tf8("队友").c_str(), ImColor(255, 255, 255, 255), 1, ImGui::OutlineSide::All, ImColor(0, 0, 0, 255)); }
					ImGui::DrawTextWithOutline(DrawList, ImVec2((BoxL + BoxW / 2) + 51, BoxT - 32), string(to_string((unsigned long)kills) + tf8("杀")).c_str(), ImColor(255, 255, 255, 255), 1, ImGui::OutlineSide::All, ImColor(0, 0, 0, 255));
					ImGui::DrawTextWithOutline(DrawList, ImVec2((BoxL + BoxW / 2), BoxT - 48), string(tf8("距离: [") + to_string((unsigned long)sqrt(pow(X - m_x, 2) + pow(Y - m_y, 2) + pow(Z - m_z, 2))) + "]M").c_str(), ImColor(230, 230, 230, 255), 1, ImGui::OutlineSide::All, ImColor(0, 0, 0, 255));
					ImGui::DrawTextWithOutline(DrawList, ImVec2((BoxL + BoxW / 2) - 77, BoxT - 48), string(tf8("血量: [") + to_string((unsigned long)Healthy) + "]").c_str(), ImColor(230, 230, 230, 230), 1, ImGui::OutlineSide::All, ImColor(0, 0, 0, 255));
					ImGui::DrawTextWithOutline(DrawList, ImVec2(((BoxL + BoxW / 2) - 77) + 27, BoxT - 32), Names.c_str(), ImColor(255, 255, 255, 255), 1, ImGui::OutlineSide::All, ImColor(0, 0, 0, 255));
					DrawList->AddRectFilled(ImVec2((BoxL + BoxW / 2) - 77, (BoxT - 35) + 20), ImVec2((((BoxL + BoxW / 2) - 77) + 154), (BoxT - 35) + 24), D3DCOLOR_RGBA(0, 0, 0, 120), 0, 0);
					if (state == 4) { DrawList->AddRectFilled(ImVec2((BoxL + BoxW / 2) - 77, (BoxT - 35) + 20), ImVec2((((BoxL + BoxW / 2) - 77) + (154 * (DieHealthy / 100))), (BoxT - 35) + 24), D3DCOLOR_RGBA(5, 5, 255, 255), 0, 0); }
					else { DrawList->AddRectFilled(ImVec2((BoxL + BoxW / 2) - 77, (BoxT - 35) + 20), ImVec2((((BoxL + BoxW / 2) - 77) + (154 * (Healthy / 100))), (BoxT - 35) + 24), D3DCOLOR_RGBA(255, 255, 255, 255), 0, 0); }
					switch (state)
					{
					case 1:ImGui::DrawTextWithOutline(DrawList, ImVec2((BoxL + BoxW / 2) - 14, BoxT + BoxH + 1), tf8("[站立]").c_str(), ImColor(230, 230, 100, 255), 1, ImGui::OutlineSide::All, ImColor(0, 0, 0, 255)); break;
					case 2:ImGui::DrawTextWithOutline(DrawList, ImVec2((BoxL + BoxW / 2) - 14, BoxT + BoxH + 1), tf8("[蹲下]").c_str(), ImColor(230, 230, 100, 255), 1, ImGui::OutlineSide::All, ImColor(0, 0, 0, 255)); break;
					case 3:ImGui::DrawTextWithOutline(DrawList, ImVec2((BoxL + BoxW / 2) - 14, BoxT + BoxH + 1), tf8("[趴下]").c_str(), ImColor(230, 230, 100, 255), 1, ImGui::OutlineSide::All, ImColor(0, 0, 0, 255)); break;
					case 4:ImGui::DrawTextWithOutline(DrawList, ImVec2((BoxL + BoxW / 2) - 14, BoxT + BoxH + 1), tf8("[倒地]").c_str(), ImColor(230, 230, 100, 255), 1, ImGui::OutlineSide::All, ImColor(0, 0, 0, 255)); break;
					case 5:ImGui::DrawTextWithOutline(DrawList, ImVec2((BoxL + BoxW / 2) - 14, BoxT + BoxH + 1), tf8("[技能]").c_str(), ImColor(230, 230, 100, 255), 1, ImGui::OutlineSide::All, ImColor(0, 0, 0, 255)); break;
					case 6:ImGui::DrawTextWithOutline(DrawList, ImVec2((BoxL + BoxW / 2) - 14, BoxT + BoxH + 1), tf8("[大炮]").c_str(), ImColor(230, 230, 100, 255), 1, ImGui::OutlineSide::All, ImColor(0, 0, 0, 255)); break;
					case 7:ImGui::DrawTextWithOutline(DrawList, ImVec2((BoxL + BoxW / 2) - 14, BoxT + BoxH + 1), tf8("[游泳]").c_str(), ImColor(230, 230, 100, 255), 1, ImGui::OutlineSide::All, ImColor(0, 0, 0, 255)); break;
					case 8:ImGui::DrawTextWithOutline(DrawList, ImVec2((BoxL + BoxW / 2) - 14, BoxT + BoxH + 1), tf8("[球上]").c_str(), ImColor(230, 230, 100, 255), 1, ImGui::OutlineSide::All, ImColor(0, 0, 0, 255)); break;
					case 9:ImGui::DrawTextWithOutline(DrawList, ImVec2((BoxL + BoxW / 2) - 14, BoxT + BoxH + 1), tf8("[车上]").c_str(), ImColor(230, 230, 100, 255), 1, ImGui::OutlineSide::All, ImColor(0, 0, 0, 255)); break;
					default:break;
					}
					unsigned long LeftVec = 4;
					for (unsigned long i = 0; i < 18; i++) { DrawList->AddRectFilled(ImVec2(((BoxL + BoxW / 2) - 77) + LeftVec, (BoxT - 35) + 20), ImVec2((((BoxL + BoxW / 2) - 77) + LeftVec + 3), (BoxT - 35) + 27), D3DCOLOR_RGBA(0, 0, 0, 100), 0, 0); LeftVec += 8; }
					DrawList->AddRectFilled(ImVec2((BoxL + BoxW / 2) - 77, (BoxT - 35) + 24), ImVec2((((BoxL + BoxW / 2) - 77) + 154), (BoxT - 35) + 27), D3DCOLOR_RGBA(0, 0, 0, 120), 0, 0);
					DrawList->AddRectFilled(ImVec2((BoxL + BoxW / 2) - 77, (BoxT - 35) + 24), ImVec2((((BoxL + BoxW / 2) - 77) + (154 * (Energy / 100))), (BoxT - 35) + 27), -65536, 0, 0); LeftVec = 4;
					for (unsigned long i = 0; i < 18; i++) { DrawList->AddRectFilled(ImVec2(((BoxL + BoxW / 2) - 77) + LeftVec, (BoxT - 35) + 24), ImVec2((((BoxL + BoxW / 2) - 77) + LeftVec + 3), (BoxT - 35) + 27), D3DCOLOR_RGBA(0, 0, 0, 100), 0, 0); LeftVec += 8; }
					DrawList->AddRectFilled(ImVec2((BoxL + BoxW / 2) - 77, (BoxT - 35) + 27), ImVec2((((BoxL + BoxW / 2) - 77) + 154), (BoxT - 35) + 30), ImColor(0, 0, 0, 120), 0, 0);
					DrawList->AddRectFilled(ImVec2((BoxL + BoxW / 2) - 77, (BoxT - 35) + 27), ImVec2((((BoxL + BoxW / 2) - 77) + (154 * (mood / 100))), (BoxT - 35) + 30), ImColor(255, 128, 0, 255), 0, 0); LeftVec = 4;
					for (unsigned long i = 0; i < 18; i++) { DrawList->AddRectFilled(ImVec2(((BoxL + BoxW / 2) - 77) + LeftVec, (BoxT - 35) + 27), ImVec2((((BoxL + BoxW / 2) - 77) + LeftVec + 3), (BoxT - 35) + 30), ImColor(0, 0, 0, 120), 0, 0); LeftVec += 8; }
				}if (Drawing.showwar) { ImGui::DrawTextWithOutline(DrawList, ImVec2((BoxL + BoxW / 2) - 28, BoxT + BoxH + 14), string("[" + WARARNames + "][" + WARNames + "]").c_str(), ImColor(5, 5, 230, 255), 1, ImGui::OutlineSide::All, ImColor(0, 0, 0, 255)); }
			}//fuck bug
		}
		ImGui::DrawTextWithOutline(DrawList, ImVec2((width / 2) - 20, 50), string(tf8("附近人数: <") + to_string((unsigned long)NNUM) + ">").c_str(), ImColor(255, 255, 255, 255), 1, ImGui::OutlineSide::All, ImColor(0, 0, 0, 255));
		ImGui::DrawTextWithOutline(DrawList, ImVec2((width / 2) - 65, 70), string(tf8("附近真人: [") + to_string((unsigned long)EVENT) + "]").c_str(), ImColor(255, 5, 5, 255), 1, ImGui::OutlineSide::All, ImColor(0, 0, 0, 255));
		ImGui::DrawTextWithOutline(DrawList, ImVec2((width / 2) + 25, 70), string(tf8("附近人机: [") + to_string((unsigned long)BOT) + "]").c_str(), ImColor(5, 5, 255, 255), 1, ImGui::OutlineSide::All, ImColor(0, 0, 0, 255));
		ImGui::DrawTextWithOutline(DrawList, ImVec2((width / 2) - 20, 90), string(tf8("背后人数: [") + to_string((unsigned long)BH) + "]").c_str(), ImColor(255, 128, 0, 255), 1, ImGui::OutlineSide::All, ImColor(0, 0, 0, 255));
	}
	catch (...) {
		if (CMDLOG.C_LogDebug) {LOG_OUT(L_ERROR, string("Error code:" + GetLastError()).c_str());}
		return;
	}
}
void Cheat::Draw::DrawClass::AimBot(Cheat::Draw::AimSet Aimbot, ImDrawList* DrawList)
{
	if (!Aimbot.AimBot)return;
	unsigned long AimDistance = Aimbot.Range;
	unsigned long  matrix_L1 = 0, matrix_L2 = 0, matrix_L3 = 0, matrix_L4 = 0, matrix_address = 0, event_L6 = 0, event_L7 = 0;
	unsigned long NNUM = 0, Die = 0, Mate = 0, state = 0, Team = 0, kills = 0;
	float m_y = 0, Aimhits, AimViewX, hits, m_x = 0, m_z = 0, Healthy = 0, DieHealthy = 0, X = 0, Y = 0, Z = 0, ViewW = 0, ViewX = 0, ViewY = 0, ViewY2 = 0, BoxW = 0, BoxH = 0, BoxL = 0, BoxT = 0, Energy = 0;
	bool CanAim = false;
	if (NUM == 0)return;
	try
	{
		if (((event_L10) > 10000) && (*(unsigned long*)(event_L10) > 10000)) { event_L7 = *(unsigned long*)event_L10 + (Cheat::Memory::event_module_Pointer::Pointer_L6_Products); if (event_L7 < 10000) { return; } }
		if (((event_L7) > 10000) && (*(unsigned long*)(event_L7) > 10000)) { m_x = *(float*)(*(unsigned long*)event_L7 + Cheat::Memory::event_module_Pointer::Pointer_X); m_y = *(float*)(*(unsigned long*)event_L7 + Cheat::Memory::event_module_Pointer::Pointer_Y); m_z = *(float*)(*(unsigned long*)event_L7 + Cheat::Memory::event_module_Pointer::Pointer_Z); }
		if (*((unsigned long*)(Cheat::Memory::matrix_Base)) > 10000) { matrix_L1 = *((unsigned long*)(Cheat::Memory::matrix_Base)) + Cheat::Memory::matrix_module_Pointer::matrix_Pointer_L1; if (*(unsigned long*)matrix_L1 < 10000) { return; } }
		if (((matrix_L1) > 10000) && (*(unsigned long*)(matrix_L1) > 10000)) { matrix_L2 = *(unsigned long*)matrix_L1 + Cheat::Memory::matrix_module_Pointer::matrix_Pointer_L2; if (*(unsigned long*)matrix_L2 < 10000) { return; } }
		if (((matrix_L2) > 10000) && (*(unsigned long*)(matrix_L2) > 10000)) { matrix_L3 = *(unsigned long*)matrix_L2 + Cheat::Memory::matrix_module_Pointer::matrix_Pointer_L3; if (*(unsigned long*)matrix_L3 < 10000) { return; } }
		if (((matrix_L3) > 10000) && (*(unsigned long*)(matrix_L3) > 10000)) { matrix_L4 = *(unsigned long*)matrix_L3 + Cheat::Memory::matrix_module_Pointer::matrix_Pointer_L4; if (*(unsigned long*)matrix_L4 < 10000) { return; } }
		if (((matrix_L4) > 10000) && (*(unsigned long*)(matrix_L4) > 10000)) { matrix_address = *(unsigned long*)matrix_L4 + Cheat::Memory::matrix_module_Pointer::matrix_Pointer_L5; if (matrix_address < 10000) { return; } }
		float matrix[4][4] =
		{
		*(float*)(matrix_address + 0x00),*(float*)(matrix_address + 0x04),*(float*)(matrix_address + 0x08),*(float*)(matrix_address + 0x0C),
		*(float*)(matrix_address + 0x10),*(float*)(matrix_address + 0x14),*(float*)(matrix_address + 0x18),*(float*)(matrix_address + 0x1C),
		*(float*)(matrix_address + 0x20),*(float*)(matrix_address + 0x24),*(float*)(matrix_address + 0x28),*(float*)(matrix_address + 0x2C),
		*(float*)(matrix_address + 0x30),*(float*)(matrix_address + 0x34),*(float*)(matrix_address + 0x38),*(float*)(matrix_address + 0x3C)
		};
		for (unsigned long i = 0; i < NUM; i++)
		{
			if (i == 0) { continue; }
			string Names;
			if (((event_L10) > 10000) && (*(unsigned long*)(event_L10) > 10000)) { event_L6 = *(unsigned long*)event_L10 + (Cheat::Memory::event_module_Pointer::Pointer_L6_Products + (i * Cheat::Memory::event_module_Pointer::Pointer_L6_Product));  if (event_L6 < 10000) { continue; }if (*(unsigned long*)event_L6 < 10000) { continue; } }if (event_L6 < 10000) { continue; } if (*(unsigned long*)event_L6 < 10000) { continue; }
			if ((event_L6 > 10000) && (*(unsigned long*)event_L6 > 10000)) { Die = *(unsigned long*)(*(unsigned long*)event_L6 + Cheat::Memory::event_module_Pointer::Pointer_Die);  if (Die > 0)  continue; }
			if ((event_L6 > 10000) && (*(unsigned long*)event_L6 > 10000)) { Healthy = *(float*)(*(unsigned long*)event_L6 + Cheat::Memory::event_module_Pointer::Pointer_Healthy); if (Healthy < 0 || Healthy > 100) { continue; } }
			if ((event_L6 > 10000) && (*(unsigned long*)event_L6 > 10000)) { DieHealthy = *(float*)((*(unsigned long*)event_L6 + Cheat::Memory::event_module_Pointer::Pointer_DieHealthy)); if (DieHealthy < 0 || DieHealthy > 100) { continue; } }
			if ((event_L6 > 10000) && (*(unsigned long*)event_L6 > 10000)) { Mate = *(unsigned long*)(*(unsigned long*)event_L6 + Cheat::Memory::event_module_Pointer::Pointer_Mate); if ((Mate > 0) && !Aim.AimMate) { continue; } }
			if ((event_L6 > 10000) && (*(unsigned long*)event_L6 > 10000)) { state = *(unsigned long*)(*(unsigned long*)event_L6 + Cheat::Memory::event_module_Pointer::Pointer_state); }
			if ((event_L6 > 10000) && (*(unsigned long*)event_L6 > 10000)) { X = *(float*)(*(unsigned long*)event_L6 + Cheat::Memory::event_module_Pointer::Pointer_X); Y = *(float*)(*(unsigned long*)event_L6 + Cheat::Memory::event_module_Pointer::Pointer_Y); Z = *(float*)(*(unsigned long*)event_L6 + Cheat::Memory::event_module_Pointer::Pointer_Z); }
			if ((event_L6 > 10000) && (*(unsigned long*)event_L6 > 10000)) { if (*(unsigned long*)(*(unsigned long*)event_L6 + Cheat::Memory::event_module_Pointer::Pointer_PPBALL) != 0) { state = 8; } }
			if ((Healthy == 0) && (DieHealthy == 0)) { continue; }
			if ((sqrt(pow(X - m_x, 2) + pow(Y - m_y, 2) + pow(Z - m_z, 2)) < 0)) { continue; }
			if ((sqrt(pow(X - m_x, 2) + pow(Z - m_z, 2)) > Aimbot.AimM)) { continue; }
			if ((Healthy == 0) && (DieHealthy != 0)) { state = 4; }
			if ((state == 4) && (!Aimbot.AimShowKnockdown)) { continue; }
			ViewW = matrix[0][3] * X + matrix[1][3] * Y + matrix[2][3] * Z + matrix[3][3]; if (ViewW < 0) { continue; }ViewW = 1 / ViewW;
			ViewX = width / 2 + (matrix[0][0] * X + matrix[1][0] * Y + matrix[2][0] * Z + matrix[3][0]) * ViewW * width / 2;
			if (((ViewX > 0) && (ViewX < 5000))) {
				if (Aimbot.ShowM) { DrawList->AddCircle(ImVec2(width / 2, height / 2), Aimbot.Range, D3DCOLOR_RGBA(255, 255, 255, 100), 180, 1); }
				float size = 0, ball = 0;
				if (E_Memory.PlayerSize < 1.5)
				{
					size = E_Memory.PlayerSize / 2;
				}
				if (E_Memory.PlayerSize > 1.5)
				{
					size = E_Memory.PlayerSize / 1.1;
				}
				if (state == 8)
				{
					ball = 0.9;
				}
				if (Aimbot.Hit == 1) {
					if (E_Memory.IsPlayerSize)
					{
						hits = height / 2 - (matrix[0][1] * X + matrix[1][1] * (Y + 1.5 + ball + size) + matrix[2][1] * Z + matrix[3][1]) * ViewW * height / 2;
					}
					else
					{
						hits = height / 2 - (matrix[0][1] * X + matrix[1][1] * (Y + 1.5 + ball) + matrix[2][1] * Z + matrix[3][1]) * ViewW * height / 2;
					}
				}
				if (Aimbot.Hit == 2) {
					if (E_Memory.IsPlayerSize)
					{
						hits = height / 2 - (matrix[0][1] * X + matrix[1][1] * (Y + 1.0 + ball + size) + matrix[2][1] * Z + matrix[3][1]) * ViewW * height / 2;
					}
					else
					{
						hits = height / 2 - (matrix[0][1] * X + matrix[1][1] * (Y + 1.0 + ball) + matrix[2][1] * Z + matrix[3][1]) * ViewW * height / 2;
					}
				}
				if (state == 3) {
					hits = height / 2 - (matrix[0][1] * X + matrix[1][1] * (Y + 0.2 + size) + matrix[2][1] * Z + matrix[3][1]) * ViewW * height / 2;
				}
				if (state == 2) {
					hits = height / 2 - (matrix[0][1] * X + matrix[1][1] * (Y + 0.5 + size) + matrix[2][1] * Z + matrix[3][1]) * ViewW * height / 2;
				}
				unsigned long CHDistance = sqrt(pow(ViewX - (width / 2), 2) + pow(hits - (height / 2), 2));
				if (Aimbot.Range > CHDistance) {
					if (CHDistance < AimDistance) {
						AimDistance = CHDistance;
						Aimhits = hits;
						AimViewX = ViewX;
						CanAim = true;
					}
				}
			}
		}
		if ((CanAim == true) && (Aimbot.AimBot))
		{
			DrawList->AddLine(ImVec2(AimViewX, Aimhits), ImVec2(width / 2, height / 2), D3DCOLOR_RGBA(255, 215, 0, 200), 1);
			if (GetKeyState(Aimbot.Key) <= -127)
			{
				unsigned long MouseX = (AimViewX - (width / 2)) / Aimbot.autodwRL;
				unsigned long MouseY = (Aimhits - (height / 2)) / Aimbot.autodwUD;
				mouse_event(MOUSEEVENTF_MOVE, MouseX, MouseY, 0, 0);
			}
		}
	}
	catch (...) {
		if (CMDLOG.C_LogDebug) {LOG_OUT(L_ERROR, string("Error code:" + GetLastError()).c_str());}
		return;
	}
}
void Cheat::Draw::DrawClass::DrawCheatInfo(Cheat::Draw::DrawInfo Info)
{
	if (Info.C_ShowFPS)
	{
		ImGuiWindowFlags flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoFocusOnAppearing
			| ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_AlwaysAutoResize;
		if (!Info.C_FPSMove)
			flags |= ImGuiWindowFlags_NoInputs | ImGuiWindowFlags_NoMove;
		if (ImGui::Begin("FPS", nullptr, flags))
		{
			values[values_offset++] = ImGui::GetIO().Framerate;
			if (values_offset >= IM_ARRAYSIZE(values))
				values_offset = 0;
			for (unsigned long i = 0; i < IM_ARRAYSIZE(values); i++)
				avg_fps += values[i];
			avg_fps /= IM_ARRAYSIZE(values);
			ImGui::Text("FPS: %.1f", ImGui::GetIO().Framerate);
			ImGui::Text("FPS: %.1f (high)", hig_fps);
			ImGui::Text("FPS: %.1f (low)", low_fps);
			ImGui::Text("FPS: %.1f (avg)", avg_fps);
			if (C_ShowInfo.C_fpsQ)
			{
				values[values_offset++] = ImGui::GetIO().Framerate;
				if (values_offset >= IM_ARRAYSIZE(values))
					values_offset = 0;
				ImGui::PlotLines("", values, IM_ARRAYSIZE(values), values_offset, "", 0.0f,500.0f, ImVec2(0, 80));
				for (unsigned long i = 0; i < IM_ARRAYSIZE(values); i++)
					avg_fps += values[i];
				avg_fps /= IM_ARRAYSIZE(values);
			}
			ImGui::End();
		}
	}
	if (Info.C_ShowStatus)
	{
		// Drawing status window
		ImGuiWindowFlags flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoBackground |
			ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoBringToFrontOnFocus |
			ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoCollapse;

		if (!Info.C_StatusMove)
			flags |= ImGuiWindowFlags_NoInputs | ImGuiWindowFlags_NoMove;

		ImGui::Begin("Cheat status", nullptr, flags);

		static ImGuiTableFlags tabFlags = ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg;

		if (ImGui::BeginTable("activesTable", 1, tabFlags))
		{
			ImGui::TableSetupColumn(tf8("已启用").c_str());
			ImGui::TableHeadersRow();

			unsigned long row = 0;
			if (ESP.C_ShowESP)
			{
				ImGui::TableNextRow();
				ImGui::TableSetColumnIndex(0);
				ImGui::Text(tf8("透视").c_str());
				ImU32 row_bg_color = ImGui::GetColorU32(ImVec4(0.2f + row * 0.1f, 0.1f + row * 0.05f, 0.1f + row * 0.03f, 0.85f));
				ImGui::TableSetBgColor(ImGuiTableBgTarget_RowBg0, row_bg_color);
				row++;
				if (ESP.T3Dbox) {
					ImGui::TableNextRow();
					ImGui::TableSetColumnIndex(0);
					ImGui::Text(tf8("3D方框").c_str());
					ImU32 row_bg_color = ImGui::GetColorU32(ImVec4(0.2f + row * 0.1f, 0.1f + row * 0.05f, 0.1f + row * 0.03f, 0.85f));
					ImGui::TableSetBgColor(ImGuiTableBgTarget_RowBg0, row_bg_color);
					row++;
				}
				if (ESP.Showarrow) {
					ImGui::TableNextRow();
					ImGui::TableSetColumnIndex(0);
					ImGui::Text(tf8("显示箭头").c_str());
					ImU32 row_bg_color = ImGui::GetColorU32(ImVec4(0.2f + row * 0.1f, 0.1f + row * 0.05f, 0.1f + row * 0.03f, 0.85f));
					ImGui::TableSetBgColor(ImGuiTableBgTarget_RowBg0, row_bg_color);
					row++;
				}
				if (ESP.C_ShowMate) {
					ImGui::TableNextRow();
					ImGui::TableSetColumnIndex(0);
					ImGui::Text(tf8("显示队友").c_str());
					ImU32 row_bg_color = ImGui::GetColorU32(ImVec4(0.2f + row * 0.1f, 0.1f + row * 0.05f, 0.1f + row * 0.03f, 0.85f));
					ImGui::TableSetBgColor(ImGuiTableBgTarget_RowBg0, row_bg_color);
					row++;
				}
				if (ESP.C_ShowLine) {
					ImGui::TableNextRow();
					ImGui::TableSetColumnIndex(0);
					ImGui::Text(tf8("显示射线").c_str());
					ImU32 row_bg_color = ImGui::GetColorU32(ImVec4(0.2f + row * 0.1f, 0.1f + row * 0.05f, 0.1f + row * 0.03f, 0.85f));
					ImGui::TableSetBgColor(ImGuiTableBgTarget_RowBg0, row_bg_color);
					row++;
				}
				if (ESP.C_ShowInfo) {
					ImGui::TableNextRow();
					ImGui::TableSetColumnIndex(0);
					ImGui::Text(tf8("显示信息").c_str());
					ImU32 row_bg_color = ImGui::GetColorU32(ImVec4(0.2f + row * 0.1f, 0.1f + row * 0.05f, 0.1f + row * 0.03f, 0.85f));
					ImGui::TableSetBgColor(ImGuiTableBgTarget_RowBg0, row_bg_color);
					row++;
				}
				if (ESP.C_ShowBox) {
					ImGui::TableNextRow();
					ImGui::TableSetColumnIndex(0);
					ImGui::Text(tf8("显示方框").c_str());
					ImU32 row_bg_color = ImGui::GetColorU32(ImVec4(0.2f + row * 0.1f, 0.1f + row * 0.05f, 0.1f + row * 0.03f, 0.85f));
					ImGui::TableSetBgColor(ImGuiTableBgTarget_RowBg0, row_bg_color);
					row++;
				}
				if (ESP.C_ShowKnockdown) {
					ImGui::TableNextRow();
					ImGui::TableSetColumnIndex(0);
					ImGui::Text(tf8("显示倒地").c_str());
					ImU32 row_bg_color = ImGui::GetColorU32(ImVec4(0.2f + row * 0.1f, 0.1f + row * 0.05f, 0.1f + row * 0.03f, 0.85f));
					ImGui::TableSetBgColor(ImGuiTableBgTarget_RowBg0, row_bg_color);
					row++;
				}
			}
			if (car.show) {
				ImGui::TableNextRow();
				ImGui::TableSetColumnIndex(0);
				ImGui::Text(tf8("显示载具").c_str());
				ImU32 row_bg_color = ImGui::GetColorU32(ImVec4(0.2f + row * 0.1f, 0.1f + row * 0.05f, 0.1f + row * 0.03f, 0.85f));
				ImGui::TableSetBgColor(ImGuiTableBgTarget_RowBg0, row_bg_color);
				row++;
			}
			if (E_Memory.IsPlayerSize) {
				ImGui::TableNextRow();
				ImGui::TableSetColumnIndex(0);
				ImGui::Text(tf8("人物变大").c_str());
				ImU32 row_bg_color = ImGui::GetColorU32(ImVec4(0.2f + row * 0.1f, 0.1f + row * 0.05f, 0.1f + row * 0.03f, 0.85f));
				ImGui::TableSetBgColor(ImGuiTableBgTarget_RowBg0, row_bg_color);
				row++;
			}
			if (Aim.AimBot)
			{
				ImGui::TableNextRow();
				ImGui::TableSetColumnIndex(0);
				ImGui::Text(tf8("自瞄").c_str());
				ImU32 row_bg_color = ImGui::GetColorU32(ImVec4(0.2f + row * 0.1f, 0.1f + row * 0.05f, 0.1f + row * 0.03f, 0.85f));
				ImGui::TableSetBgColor(ImGuiTableBgTarget_RowBg0, row_bg_color);
				row++;
				if (Aim.AimMate) {
					ImGui::TableNextRow();
					ImGui::TableSetColumnIndex(0);
					ImGui::Text(tf8("自瞄队友").c_str());
					ImU32 row_bg_color = ImGui::GetColorU32(ImVec4(0.2f + row * 0.1f, 0.1f + row * 0.05f, 0.1f + row * 0.03f, 0.85f));
					ImGui::TableSetBgColor(ImGuiTableBgTarget_RowBg0, row_bg_color);
					row++;
				}
				if (Aim.AimShowKnockdown) {
					ImGui::TableNextRow();
					ImGui::TableSetColumnIndex(0);
					ImGui::Text(tf8("自瞄倒地").c_str());
					ImU32 row_bg_color = ImGui::GetColorU32(ImVec4(0.2f + row * 0.1f, 0.1f + row * 0.05f, 0.1f + row * 0.03f, 0.85f));
					ImGui::TableSetBgColor(ImGuiTableBgTarget_RowBg0, row_bg_color);
					row++;
				}
				if (Aim.ShowM) {
					ImGui::TableNextRow();
					ImGui::TableSetColumnIndex(0);
					ImGui::Text(tf8("显示范围").c_str());
					ImU32 row_bg_color = ImGui::GetColorU32(ImVec4(0.2f + row * 0.1f, 0.1f + row * 0.05f, 0.1f + row * 0.03f, 0.85f));
					ImGui::TableSetBgColor(ImGuiTableBgTarget_RowBg0, row_bg_color);
					row++;
				}
			}
			if (Info.C_ShowFPS)
			{
				ImGui::TableNextRow();
				ImGui::TableSetColumnIndex(0);
				ImGui::Text(tf8("显示帧数").c_str());
				ImU32 row_bg_color = ImGui::GetColorU32(ImVec4(0.2f + row * 0.1f, 0.1f + row * 0.05f, 0.1f + row * 0.03f, 0.85f));
				ImGui::TableSetBgColor(ImGuiTableBgTarget_RowBg0, row_bg_color);
				row++;
			}
			ImGui::EndTable();
		}

		ImGui::End();
	}
}
void MemorySet()
{
	if (NUM == 0)return;
	unsigned long event_L6, event_L6M, M_WAR, M_WAR2;
	unsigned long Mate = 0;
	try
	{
		if (((event_L10) > 10000) && (*(unsigned long*)(event_L10) > 10000)) { event_L6M = *(unsigned long*)event_L10 + (Cheat::Memory::event_module_Pointer::Pointer_L6_Products + (0 * Cheat::Memory::event_module_Pointer::Pointer_L6_Product)); }
		if ((event_L6M > 10000) && (*(unsigned long*)event_L6M > 10000)) { M_WAR = (*(unsigned long*)event_L6M + Cheat::Memory::event_module_Pointer::Pointer_WAR); }
		if ((M_WAR > 10000) && (*(unsigned long*)M_WAR > 10000)) { M_WAR2 = *(unsigned long*)(*(unsigned long*)M_WAR + Cheat::Memory::event_module_Pointer::Pointer_WAR_L1); }
		if ((M_WAR2 > 10000) && (*(unsigned long*)M_WAR2 > 10000) && E_Memory.IsHWARSPEED) { *(float*)(M_WAR2 + Cheat::Memory::event_module_Pointer::Pointer_WARSPEED) = E_Memory.HWARSPEED; }
		if ((M_WAR2 > 10000) && (*(unsigned long*)M_WAR2 > 10000) && E_Memory.IsWARLM) { *(float*)(M_WAR2 + Cheat::Memory::event_module_Pointer::Pointer_WARLM) = E_Memory.WARLM; }
		if ((M_WAR2 > 10000) && (*(unsigned long*)M_WAR2 > 10000) && E_Memory.IsWARAIM) { *(float*)(M_WAR2 + Cheat::Memory::event_module_Pointer::Pointer_WARAIM) = E_Memory.WARAIM; }
		if (E_Memory.WARMAX >= E_Memory.WARMIN)
		{
			if ((M_WAR2 > 10000) && (*(unsigned long*)M_WAR2 > 10000) && E_Memory.IsWAR) { *(float*)(M_WAR2 + Cheat::Memory::event_module_Pointer::Pointer_WARMAX) = E_Memory.WARMAX; }
			if ((M_WAR2 > 10000) && (*(unsigned long*)M_WAR2 > 10000) && E_Memory.IsWAR) { *(float*)(M_WAR2 + Cheat::Memory::event_module_Pointer::Pointer_WARMIN) = E_Memory.WARMIN; }
		}
		if ((M_WAR2 > 10000) && (*(unsigned long*)M_WAR2 > 10000) && E_Memory.IsWARHZLRL) { *(float*)(M_WAR2 + Cheat::Memory::event_module_Pointer::Pointer_WARLR) = E_Memory.WARHZLRLMAX; }
		if (E_Memory.WARHZLUDMAX >= E_Memory.WARHZLUDMIN)
		{
			if ((M_WAR2 > 10000) && (*(unsigned long*)M_WAR2 > 10000) && E_Memory.IsWARHZLUD) { *(float*)(M_WAR2 + Cheat::Memory::event_module_Pointer::Pointer_WARUDMAX) = E_Memory.WARHZLUDMAX; }
			if ((M_WAR2 > 10000) && (*(unsigned long*)M_WAR2 > 10000) && E_Memory.IsWARHZLUD) { *(float*)(M_WAR2 + Cheat::Memory::event_module_Pointer::Pointer_WARUDMIN) = E_Memory.WARHZLUDMIN; }
		}
		for (unsigned long i = 0; i < NUM; i++)
		{
			if (i == 0) { continue; }
			if (((event_L10) > 10000) && (*(unsigned long*)(event_L10) > 10000)) { event_L6 = *(unsigned long*)event_L10 + (Cheat::Memory::event_module_Pointer::Pointer_L6_Products + (i * Cheat::Memory::event_module_Pointer::Pointer_L6_Product));  if (event_L6 < 10000) { continue; }if (*(unsigned long*)event_L6 < 10000) { continue; } }if (event_L6 < 10000) { continue; } if (*(unsigned long*)event_L6 < 10000) { continue; }
			if ((event_L6 > 10000) && (*(unsigned long*)event_L6 > 10000)) { Mate = *(unsigned long*)(*(unsigned long*)event_L6 + Cheat::Memory::event_module_Pointer::Pointer_Mate); if ((Mate != 0))  continue; }
			if ((Mate == 0)) {
				if ((event_L6 > 10000) && (*(unsigned long*)event_L6 > 10000) && E_Memory.IsPlayerSize) { *(float*)(*(unsigned long*)event_L6 + Cheat::Memory::event_module_Pointer::Pointer_PlayerSize) = E_Memory.PlayerSize; }
				if ((event_L6 > 10000) && (*(unsigned long*)event_L6 > 10000) && !E_Memory.IsPlayerSize) { *(float*)(*(unsigned long*)event_L6 + Cheat::Memory::event_module_Pointer::Pointer_PlayerSize) = 1; }
			}
		}
	}
	catch (...) {
		if (CMDLOG.C_LogDebug) {LOG_OUT(L_ERROR, string("Error code:" + GetLastError()).c_str());}
		return;
	}
}
void Cheat::Draw::DrawClass::DrawMainMenu()
{
	if (ImGui::GetIO().Framerate > hig_fps) hig_fps = ImGui::GetIO().Framerate;
	if (ImGui::GetIO().Framerate < low_fps)  low_fps = ImGui::GetIO().Framerate;
	ImDrawList* DrawList = ImGui::GetBackgroundDrawList();
	ImGuiIO& io = ImGui::GetIO();

	if (Mem_Show)
	{
		ImGui::SetNextWindowSize(ImVec2(600, 300), ImGuiCond_FirstUseEver);
		if (ImGui::Begin(string("Sausage Man Cheat By:SUI.MO." + tf8("遂沫") + ")" + tf8(" 本辅助完全免费禁止倒卖")).c_str()))
		{
			ImGui::BeginGroup();
			if (ImGui::Checkbox(tf8("禁止键鼠穿透").c_str(), &m_IsBlockingInput)) { SetInputLock(this, m_IsBlockingInput); }
			if (ImGui::BeginListBox("##listbox 2", ImVec2(175, -FLT_MIN)))
			{
				if (ImGui::Selectable(tf8("透视").c_str())) { ImGui::SetItemDefaultFocus(); C_List = 0; }
				if (ImGui::Selectable(tf8("自瞄").c_str())) { ImGui::SetItemDefaultFocus(); C_List = 1; }
				if (ImGui::Selectable(tf8("内存").c_str())) { ImGui::SetItemDefaultFocus(); C_List = 2; }
				if (ImGui::Selectable(tf8("设置").c_str())) { ImGui::SetItemDefaultFocus(); C_List = 3; }
				if (ImGui::Selectable(tf8("调试").c_str())) { ImGui::SetItemDefaultFocus(); C_List = 4; }
				if (ImGui::Selectable(tf8("关于").c_str())) { ImGui::SetItemDefaultFocus(); C_List = 5; }
				ImGui::TextColored(ImColor(255, 5, 0, 255), tf8("本辅助完全免费").c_str());
				ImGui::TextColored(ImColor(255, 5, 0, 255), tf8("禁止倒卖本辅助").c_str());
				ImGui::TextColored(ImColor(255, 136, 0, 255), tf8("更新及源码:").c_str());
				ImGui::PushStyleColor(ImGuiCol_ButtonHovered, 0xC0D5C8FF);
				ImGui::TextURL("GitHub", "https://github.com/issuimoo/Sausage-Man-Cheat", true, false);
				ImGui::PopStyleColor();
				ImGui::TextColored(ImColor(255, 136, 0, 255), tf8("加群:").c_str());
				ImGui::PushStyleColor(ImGuiCol_ButtonHovered, 0xC0D5C8FF);
				ImGui::TextURL("QQ", "https://jq.qq.com/?_wv=1027&k=l9rmwm8g", true, false);
				ImGui::PopStyleColor();
				ImGui::TextColored(ImColor(255, 136, 0, 255), tf8("赞助:").c_str());
				ImGui::PushStyleColor(ImGuiCol_ButtonHovered, 0xC0D5C8FF);
				ImGui::TextURL(tf8("爱发电").c_str(), "https://afdian.net/a/SoFunny", true, false);
				ImGui::PopStyleColor();
				ImGui::TextColored(ImColor(255, 255, 255, 255), string(tf8("版本:") + tf8(Ver)).c_str());
				ImGui::EndListBox();
			}
			ImGui::EndGroup();
			ImGui::SameLine();
			ImGui::BeginGroup();
			constexpr float width = 200.0f;
			ImGui::SetNextItemWidth(width);
			if (ImGui::BeginCombo(tf8("配置").c_str(), tf8(JsonName).c_str()))
			{
				if (ImGui::Selectable(tf8("配置json-1").c_str())) { JsonName = "配置json-1"; }
				if (ImGui::Selectable(tf8("配置json-2").c_str())) { JsonName = "配置json-2"; }
				if (ImGui::Selectable(tf8("配置json-3").c_str())) { JsonName = "配置json-3"; }
				if (ImGui::Selectable(tf8("配置json-4").c_str())) { JsonName = "配置json-4"; }
				ImGui::EndCombo();
			}
			ImGui::SameLine();
			if (ImGui::Button(tf8("保存").c_str())) { OutJson(JsonName); }
			ImGui::SameLine();
			if (ImGui::Button(tf8("加载").c_str())) { GetJson(JsonName); }
			ImGui::SameLine();
			if (ImGui::Button(tf8("清空").c_str())) { DeleJson(JsonName); }
			ImGuiWindowFlags UI = ImGuiWindowFlags_None;
			ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 0.0f);
			ImGui::BeginChild("1", ImVec2(0, 0), true, UI);
			switch (C_List)
			{
			case 0://ESP
				ImGui::BeginGroupPanel(tf8("透视").c_str(), false, ImVec2(0, 0));
				ImGui::Checkbox(tf8("启用").c_str(), &ESP.C_ShowESP);
				ImGui::BeginGroupPanel(tf8("玩家").c_str(), false, ImVec2(0, 0));
				ImGui::Checkbox(tf8("显示方框").c_str(), &ESP.C_ShowBox);
				ImGui::SameLine();
				ImGui::Checkbox(tf8("显示信息").c_str(), &ESP.C_ShowInfo);
				ImGui::SameLine();
				ImGui::Checkbox(tf8("显示线条").c_str(), &ESP.C_ShowLine);
				ImGui::Checkbox(tf8("显示倒地").c_str(), &ESP.C_ShowKnockdown);
				ImGui::SameLine();
				ImGui::Checkbox(tf8("显示队友").c_str(), &ESP.C_ShowMate);
				ImGui::SameLine();
				ImGui::Checkbox(tf8("显示武器").c_str(), &ESP.showwar);
				ImGui::SliderInt(tf8("距离").c_str(), &ESP.LM, 5, 500, nullptr, ImGuiSliderFlags_AlwaysClamp);
				if (ImGui::BeginCombo(tf8("方框").c_str(), ESP.name.c_str()))
				{
					if (ImGui::Selectable("2D")) { ESP.T3Dbox = false; ESP.name = "2D"; }
					if (ImGui::Selectable("3D")) { ESP.T3Dbox = true; ESP.name = "3D"; }
					ImGui::EndCombo();
				}
				if (ImGui::BeginCombo(tf8("射线位置").c_str(), ESP.Linexyname.c_str()))
				{
					if (ImGui::Selectable(tf8("顶部").c_str())) { ESP.Linexy = 1; ESP.Linexyname = tf8("顶部").c_str(); }
					if (ImGui::Selectable(tf8("中间").c_str())) { ESP.Linexy = 2; ESP.Linexyname = tf8("中间").c_str(); }
					if (ImGui::Selectable(tf8("底边").c_str())) { ESP.Linexy = 3; ESP.Linexyname = tf8("底边").c_str(); }
					ImGui::EndCombo();
				}
				ImGui::Checkbox(tf8("视野外箭头提示").c_str(), &ESP.Showarrow);
				ImGui::SliderFloat(tf8("箭头半径").c_str(), &ESP.ArrowRadius, 50, 300, nullptr, ImGuiSliderFlags_AlwaysClamp);
				ImGui::SliderFloat(tf8("箭头大小").c_str(), &ESP.TracerSize, 0.1f, 10.0f, nullptr, ImGuiSliderFlags_AlwaysClamp);
				ImGui::SliderFloat(tf8("箭头描边").c_str(), &ESP.OutlineThickness, 0.0f, 10.0f, nullptr, ImGuiSliderFlags_AlwaysClamp);
				ImGui::EndGroupPanel();
				ImGui::BeginGroupPanel(tf8("载具").c_str(), false, ImVec2(0, 0));
				ImGui::Checkbox(tf8("启用").c_str(), &car.show);
				ImGui::SliderInt(tf8("距离").c_str(), &car.m, 5, 500, nullptr, ImGuiSliderFlags_AlwaysClamp);
				ImGui::EndGroupPanel();
				ImGui::EndGroupPanel();
				break;
			case 1://AimBot
				ImGui::BeginGroupPanel(tf8("自动连点").c_str(), false, ImVec2(0, 0));
				ImGui::Checkbox(tf8("启用").c_str(), &Aim.autofire);
				ImGui::TextColored(ImColor(255, 165, 0, 255), tf8("注意:与自瞄同一热键!(不包含左键)").c_str());
				ImGui::SliderInt(tf8("间隔(ms)").c_str(), &Aim.time, 1, 1000, nullptr, ImGuiSliderFlags_AlwaysClamp);
				ImGui::EndGroupPanel();
				ImGui::BeginGroupPanel(tf8("自瞄").c_str(), false, ImVec2(0, 0));
				ImGui::Checkbox(tf8("启用").c_str(), &Aim.AimBot);
				if (ImGui::BeginCombo(tf8("热键").c_str(), Aim.KeyName))
				{
					if (ImGui::Selectable("Shift")) { Aim.Key = 16; Aim.KeyName = "Shift"; }
					if (ImGui::Selectable("RButton")) { Aim.Key = 2; Aim.KeyName = "RButton"; }
					if (ImGui::Selectable("LButton")) { Aim.Key = 1; Aim.KeyName = "LButton"; }
					if (ImGui::Selectable("Ctrl")) { Aim.Key = 17; Aim.KeyName = "Ctrl"; }
					ImGui::EndCombo();
				}
				if (ImGui::BeginCombo(tf8("位置").c_str(), tf8(Aim.HitName).c_str()))
				{
					if (ImGui::Selectable(tf8("头").c_str())) { Aim.Hit = 1; Aim.HitName = "头"; }
					if (ImGui::Selectable(tf8("胸").c_str())) { Aim.Hit = 2; Aim.HitName = "胸"; }
					ImGui::EndCombo();
				}
				ImGui::SliderInt(tf8("范围").c_str(), &Aim.Range, 5, 1000, nullptr, ImGuiSliderFlags_AlwaysClamp);
				ImGui::SliderFloat(tf8("上下平滑度").c_str(), &Aim.autodwUD, 3, 50, nullptr, ImGuiSliderFlags_AlwaysClamp);
				ImGui::SameLine();
				ImGui::HelpMarker((tf8("越大越大越慢").c_str()));
				ImGui::SliderFloat(tf8("左右平滑度").c_str(), &Aim.autodwRL, 3, 50, nullptr, ImGuiSliderFlags_AlwaysClamp);
				ImGui::SameLine();
				ImGui::HelpMarker((tf8("越大越大越慢").c_str()));
				ImGui::SliderInt(tf8("距离").c_str(), &Aim.AimM, 5, 500, nullptr, ImGuiSliderFlags_AlwaysClamp);
				ImGui::Checkbox(tf8("自瞄倒地").c_str(), &Aim.AimShowKnockdown);
				ImGui::SameLine();
				ImGui::Checkbox(tf8("自瞄队友").c_str(), &Aim.AimMate);
				ImGui::SameLine();
				ImGui::Checkbox(tf8("显示范围").c_str(), &Aim.ShowM);
				ImGui::EndGroupPanel();
				break;
			case 2://Memory
				ImGui::BeginGroupPanel(tf8("内存").c_str(), false, ImVec2(0, 0));
				ImGui::TextColored(ImColor(255, 165, 0, 255), tf8("你应该知道想稳定上分是不能开内存的!").c_str());
				ImGui::BeginGroupPanel(tf8("敌人").c_str(), false, ImVec2(0, 0));
				ImGui::Checkbox(tf8("人物变大").c_str(), &E_Memory.IsPlayerSize);
				ImGui::SliderFloat(tf8("大小").c_str(), &E_Memory.PlayerSize, 1.0f, 2.5f, nullptr, ImGuiSliderFlags_AlwaysClamp); ImGui::SameLine(); ImGui::HelpMarker((tf8("不建议大于2.0").c_str()));
				ImGui::EndGroupPanel();
				ImGui::BeginGroupPanel(tf8("武器(开启后无法还原)").c_str(), false, ImVec2(0, 0));
				ImGui::Checkbox(tf8("射速加快").c_str(), &E_Memory.IsHWARSPEED);
				ImGui::SliderFloat(tf8("间隔").c_str(), &E_Memory.HWARSPEED, 0.001f, 1.00000f, nullptr, ImGuiSliderFlags_AlwaysClamp); ImGui::SameLine(); ImGui::HelpMarker((tf8("越小越快,不建议低于0.65").c_str()));
				ImGui::Checkbox(tf8("射程").c_str(), &E_Memory.IsWARLM);
				ImGui::SliderFloat(tf8("射程大小").c_str(), &E_Memory.WARLM, 50.0f, 1000.0f, nullptr, ImGuiSliderFlags_AlwaysClamp); ImGui::SameLine(); ImGui::HelpMarker((tf8("越大越远").c_str()));
				ImGui::Checkbox(tf8("开镜倍数").c_str(), &E_Memory.IsWARAIM);
				ImGui::SliderFloat(tf8("大小").c_str(), &E_Memory.WARAIM, 1.0f, 32.0f, nullptr, ImGuiSliderFlags_AlwaysClamp); ImGui::SameLine(); ImGui::HelpMarker((tf8("观战可见").c_str()));
				ImGui::Checkbox(tf8("扩散").c_str(), &E_Memory.IsWAR);
				ImGui::SliderFloat(tf8("扩散最大").c_str(), &E_Memory.WARMAX, 0.000f, 5.000f, nullptr, ImGuiSliderFlags_AlwaysClamp);
				ImGui::SliderFloat(tf8("扩散最小").c_str(), &E_Memory.WARMIN, 0.000f, 5.000f, nullptr, ImGuiSliderFlags_AlwaysClamp);
				ImGui::TextColored(ImColor(255, 165, 0, 255), tf8("注意:最小扩散不能大于最大扩散!").c_str());
				ImGui::Checkbox(tf8("后坐力(左右)").c_str(), &E_Memory.IsWARHZLRL);
				ImGui::SliderFloat(tf8("最大").c_str(), &E_Memory.WARHZLRLMAX, 0.000f, 5.000f, nullptr, ImGuiSliderFlags_AlwaysClamp);
				ImGui::Checkbox(tf8("后坐力(上下)").c_str(), &E_Memory.IsWARHZLUD);
				ImGui::SliderFloat(tf8("后坐力最大").c_str(), &E_Memory.WARHZLUDMAX, 0.000f, 5.000f, nullptr, ImGuiSliderFlags_AlwaysClamp);
				ImGui::SliderFloat(tf8("后坐力最小").c_str(), &E_Memory.WARHZLUDMIN, 0.000f, 5.000f, nullptr, ImGuiSliderFlags_AlwaysClamp);
				ImGui::TextColored(ImColor(255, 165, 0, 255), tf8("注意:最小后坐力不能大于最大后坐力!").c_str());
				ImGui::EndGroupPanel();
				ImGui::EndGroupPanel();
				break;
			case 3://Settings
				ImGui::BeginGroupPanel(tf8("帧数").c_str(), false, ImVec2(0, 0));
				ImGui::Checkbox(tf8("显示帧数").c_str(), &C_ShowInfo.C_ShowFPS);
				ImGui::Checkbox(tf8("显示曲线图").c_str(), &C_ShowInfo.C_fpsQ);
				ImGui::SameLine();
				ImGui::Checkbox(tf8("移动帧数").c_str(), &C_ShowInfo.C_FPSMove);
				ImGui::EndGroupPanel();
				ImGui::BeginGroupPanel(tf8("已启用功能").c_str(), false, ImVec2(0, 0));
				ImGui::Checkbox(tf8("显示已启用功能").c_str(), &C_ShowInfo.C_ShowStatus);
				ImGui::SameLine();
				ImGui::Checkbox(tf8("移动已启用功能").c_str(), &C_ShowInfo.C_StatusMove);
				ImGui::EndGroupPanel();
				ImGui::BeginGroupPanel(tf8("窗口皮肤").c_str(), false, ImVec2(0, 0));
				ImGui::Checkbox(tf8("显示自定义窗口").c_str(), &ImGuishowcolor);
				ImGui::SameLine();
				if (ImGui::Button(tf8("保存").c_str())) { savetheme(); }
				ImGui::SameLine();
				if (ImGui::Button(tf8("默认").c_str())) { ImGui::DefaultTheme(); }
				ImGui::EndGroupPanel();
				break;
			case 4://Debug
				ImGui::BeginGroupPanel(tf8("调试").c_str(), false, ImVec2(0, 0));
				ImGui::Checkbox(tf8("输出调试").c_str(), &CMDLOG.C_LogDebug);
				ImGui::SameLine();
				ImGui::TextColored(ImColor(255, 165, 0, 255), tf8("注意!该选项影响性能!仅供开发人员使用!").c_str());
				if (ImGui::Button(tf8("卸载dll").c_str())) { FreeLibraryAndExitThread(MYModuleBase, 0); }
				ImGui::EndGroupPanel();
				ImGui::BeginGroupPanel(tf8("演示").c_str(), false, ImVec2(0, 0));
				ImGui::Checkbox(tf8("显示演示").c_str(), &C_Customthemes);
				ImGui::SameLine();
				ImGui::TextColored(ImColor(255, 165, 0, 255), tf8("注意!该选项影响性能!仅供开发人员使用!").c_str());
				ImGui::EndGroupPanel();
				ImGui::BeginGroupPanel("FPS", false, ImVec2(0, 0));
				values[values_offset++] = ImGui::GetIO().Framerate;
				if (values_offset >= IM_ARRAYSIZE(values))
					values_offset = 0;
				ImGui::PlotLines("", values, IM_ARRAYSIZE(values), values_offset, "", 0.0f, 300.0f, ImVec2(0, 80));
				ImGui::Text("%.1f FPS", ImGui::GetIO().Framerate);
				for (unsigned long i = 0; i < IM_ARRAYSIZE(values); i++)
					avg_fps += values[i];
				avg_fps /= IM_ARRAYSIZE(values);
				ImGui::Text("%.1f FPS (avg)", avg_fps);
				ImGui::Text("%.1f FPS (low)", low_fps);
				ImGui::Text("%.1f FPS (high)", hig_fps);
				ImGui::EndGroupPanel();
				break;
			case 5://About
				ImGui::Text(tf8("版本:%s").c_str(), Ver);
				ImGui::SameLine();
				ImGui::Text(tf8("开发:").c_str());
				ImGui::TextColored(ImColor(0, 102, 255, 255), tf8("SUI.MO.(遂沫)").c_str());
				ImGui::SameLine();
				ImGui::TextURL("Github", "https://github.com/issuimoo/", true, false);
				ImGui::Text(tf8("贡献者:(排名不分前后)").c_str());
				ImGui::TextColored(ImColor(0, 102, 255, 255), "%s\n%s\n%s\n%s\n%s", tf8("ZanWu --- 提供CT").c_str(), tf8("飞翔的丘丘 --- 提供CT和技术支持").c_str(), tf8("半月 --- 提供磁性自瞄算法").c_str(), tf8("洛小米 --- 提供CT").c_str(), tf8("限定阔落:D --- 提供物资偏移").c_str());
				ImGui::Text(tf8("赞助(各位捐点吧)").c_str());
				ImGui::TextColored(ImColor(0, 102, 255, 255), "%s\n%s\n%s", tf8("快乐小乖").c_str(), tf8("奔放哥").c_str(), "Lucky Bear Team_Draper");
				break;
			default:
				break;
			}
			ImGui::EndChild();
			ImGui::PopStyleVar();
			ImGui::EndGroup();
		}
		ImGui::End();
	}
	if (C_Customthemes)
	{
		ImGui::ShowDemoWindow();
		ImPlot::ShowDemoWindow();
	}
	if (ImGuishowcolor)
	{
		ImGui::ShowThemeSetting();
	}
	Getmatrix();
	GetPlayEvent();
	GetCarEvent();
	MemorySet();
}
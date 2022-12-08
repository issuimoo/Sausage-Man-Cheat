#pragma once
#include <Windows.h>
#include <string>
#include <imgui.h>
#include <imgui_stdlib.h>
#include <imgui.h>
#include <imgui_stdlib.h>
#include <imgui_impl_dx11.h>
#include <imgui_impl_dx10.h>
#include <imgui_impl_dx9.h>
#include <imgui_impl_win32.h>
#include <imgui_stdlib.h>
#include <d3d11.h>
#include <d3d10.h>
#include <d3dcompiler.h>
#include <dxgi.h>
#include <d3d9.h>
#include <dwmapi.h>
#include <string>
#include <detours.h>
#include <implot.h>
#include <implot_internal.h>
#include <imnodes.h>
#include <imnodes_internal.h>
#include <unordered_set>
#include <Font.h>
#include <Process.hpp>
#include <array>
#include <json/nlohmann/json.hpp>

#pragma comment (lib, "D3dcompiler.lib")
#pragma comment (lib, "winmm.lib")
#pragma comment (lib, "d3d9.lib")
#pragma comment (lib, "d3d10.lib")
#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "dxgi.lib")
#pragma comment (lib,"User32.lib")
#pragma comment (lib,"Gdi32.lib")
#pragma comment(lib, "shlwapi.lib")
namespace dx_hook
{
	enum DXVersion :int
	{
		D3D9 = 9,
		D3D10,
		D3D11,
		D3D12,
	};
	void InitD3DHook(DXVersion version, void (*p)(), HWND* hwnd, HMODULE Module, int FontSize_, std::string inifile);
	void SetInputLock(void* this_, bool value);
	bool IsInputLocked();
}

namespace ImGui
{
	struct GroupPanelHeaderBounds
	{
		ImRect left;
		ImRect right;
		bool collapsed;
	};
	enum class OutlineSide : uint32_t
	{
		Left = 1,
		Right = 2,
		Top = 4,
		Bottom = 8,
		All = Left | Right | Top | Bottom
	};
	bool operator&(OutlineSide lhs, OutlineSide rhs);
	static ImVector<GroupPanelHeaderBounds> _groupPanelStack;
	static bool GroupPanelIsOpen(ImGuiID id);
	static void GroupPanelSetOpen(ImGuiID id, bool open);
	void AddUnderLine(ImColor col_);
	void TextURL(const char* name_, const char* URL_, bool SameLineBefore_, bool SameLineAfter_);
	void ShowHelpText(const char* text);
	void HelpMarker(const char* desc);
	bool BeginGroupPanel(const char* label, bool node, const ImVec2& size);
	void EndGroupPanel();
	void NextGroupPanelHeaderItem(const ImVec2& size, bool rightAlign);
	bool BeginSelectableGroupPanel(const char* label, bool& value, bool& changed, bool node, const ImVec2& size, const char* selectLabel);
	void EndSelectableGroupPanel();
	ImVec2 CalcButtonSize(const char* label);
	static ImGuiKey LegacyToInput(short key);
	static short InputToLegacy(ImGuiKey inputkey);
	static bool IsKeyDown(ImGuiKey key);
	static bool IsKeyReleased(ImGuiKey key);
	void FixModKey(short& legacyKey);
	std::string GetKeyName(short key);
	ImVec4 HexToColor(std::string hexString);
	std::string ColorToHex(ImVec4& color);
	nlohmann::json GetCurrentStyles(ImGuiStyle style);
	void AnemoTheme();
	ImGuiStyle ShowThemeSetting();
	void DefaultTheme();
	ImGuiStyle OutCurrentStyles(nlohmann::json stylesData);
	float CalcWidth(const std::string_view& view);
	void DrawTextWithOutline(ImDrawList* drawList, ImFont* font, float fontSize, const ImVec2& screenPos, const char* text, const ImColor& textColor, float outlineThickness, OutlineSide sides, const ImColor& outlineColor);
	void DrawTextWithOutline(ImDrawList* drawList, const ImVec2& screenPos, const char* text, const ImColor& textColor, float outlineThickness, OutlineSide sides, const ImColor& outlineColor);
}

namespace CALC
{
	ImVec2 matrix_clac_OpenGL(float matrix[4][4], float X, float Y, float Z, ImVec2 screen);
	ImVec2 matrix_clac_D3D(float matrix[4][4], float X, float Y, float Z, ImVec2 screen);
	void DrawOffscreenArrows(float x, float y, float z, const ImColor& color, float OutlineThickness, float TracerSize, float ArrowRadius);
}
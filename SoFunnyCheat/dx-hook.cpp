#include <dx-hook.hpp>
#if _WIN64
#include <imgui_impl_dx12.h>
#pragma comment (lib,"detours-x64.lib")
#include <dxgi1_4.h>
#include <d3d12.h>
#pragma comment (lib, "d3d12.lib")
#else
#pragma comment (lib,"detours-x86.lib")
#endif
#include <Process.hpp>
#include <log.h>
using namespace logs;
struct xyz
{
	float x;
	float y;
	float z;
};
#define PI 3.14159265358979323846f
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
HWND HHWND;
HMODULE ModuleH_;
int FontSize;
std::string fileName = "";

#define IM_VK_KEYPAD_ENTER (VK_RETURN + 256)
const char* Theme = "";

namespace ImGui
{
	bool operator&(OutlineSide lhs, OutlineSide rhs) {
		return
			static_cast<std::underlying_type<OutlineSide>::type>(lhs) &
			static_cast<std::underlying_type<OutlineSide>::type>(rhs);
	}
	void DrawTextWithOutline(ImDrawList* drawList, const ImVec2& screenPos, const char* text, const ImColor& textColor,float outlineThickness, OutlineSide sides, const ImColor& outlineColor)
	{
		DrawTextWithOutline(drawList, nullptr, 0.0f, screenPos, text, textColor, outlineThickness, sides, outlineColor);
	}
	void DrawTextWithOutline(ImDrawList* drawList, ImFont* font, float fontSize, const ImVec2& screenPos,const char* text, const ImColor& textColor, float outlineThickness, OutlineSide sides, const ImColor& outlineColor)
	{
		if (outlineThickness == 0.0f)
		{
			drawList->AddText(font, fontSize, screenPos, outlineColor, text);
		}
		else
		{
			if (sides & OutlineSide::Left)
				drawList->AddText(font, fontSize,
					{ screenPos.x - outlineThickness, screenPos.y }, outlineColor, text);

			if (sides & OutlineSide::Right)
				drawList->AddText(font, fontSize,
					{ screenPos.x + outlineThickness, screenPos.y }, outlineColor, text);

			if (sides & OutlineSide::Bottom)
				drawList->AddText(font, fontSize,
					{ screenPos.x, screenPos.y - outlineThickness }, outlineColor, text);

			if (sides & OutlineSide::Top)
				drawList->AddText(font, fontSize,
					{ screenPos.x, screenPos.y + outlineThickness }, outlineColor, text);
		}

		drawList->AddText(font, fontSize, screenPos, textColor, text);
	}
	float CalcWidth(const std::string_view& view)
	{
		ImGuiContext& g = *GImGui;
		return ImGui::CalcTextSize(view.data()).x + g.Style.FramePadding.x * 2.0f + 25.0f;
	}
	ImGuiStyle ShowThemeSetting()
	{
		ImGuiStyle Style;
		ImGui::Begin("Color");
		ImGui::BeginGroupPanel("OtherStyles", false, ImVec2(0, 0));
		if (ImGui::BeginCombo("Styles", Theme))
		{
			if (ImGui::Selectable("Default")) { DefaultTheme(); Theme = "Default"; }
			if (ImGui::Selectable("Anemo")) { AnemoTheme(); Theme = "Anemo";}
			ImGui::EndCombo();
		}
		ImGui::EndGroupPanel();
		ImGui::BeginGroupPanel("DomeStyles", false, ImVec2(0, 0));
		ImGui::ShowStyleEditor(&Style);
		ImGui::EndGroupPanel();
		ImGui::End();
		return Style;
	}
	void AnemoTheme()
	{
		auto& styles = ImGui::GetStyle();

		// Colors
		auto colors = styles.Colors;
		colors[ImGuiCol_Border] = HexToColor("0C846ED5");
		colors[ImGuiCol_BorderShadow] = HexToColor("00000000");
		colors[ImGuiCol_Button] = HexToColor("0D9F9D9F");
		colors[ImGuiCol_ButtonActive] = HexToColor("076B3A94");
		colors[ImGuiCol_ButtonHovered] = HexToColor("0CAD4FCA");
		colors[ImGuiCol_CheckMark] = HexToColor("E6E6E680");
		colors[ImGuiCol_ChildBg] = HexToColor("00000045");
		colors[ImGuiCol_DragDropTarget] = HexToColor("333333FF");
		colors[ImGuiCol_FrameBg] = HexToColor("00000066");
		colors[ImGuiCol_FrameBgActive] = HexToColor("0D7F33B0");
		colors[ImGuiCol_FrameBgHovered] = HexToColor("09818B66");
		colors[ImGuiCol_Header] = HexToColor("0297959F");
		colors[ImGuiCol_HeaderActive] = HexToColor("07834ACC");
		colors[ImGuiCol_HeaderHovered] = HexToColor("00B8588A");
		colors[ImGuiCol_MenuBarBg] = HexToColor("069981CC");
		colors[ImGuiCol_ModalWindowDimBg] = HexToColor("3333335A");
		colors[ImGuiCol_NavHighlight] = HexToColor("7373E6CC");
		colors[ImGuiCol_NavWindowingDimBg] = HexToColor("CCCCCC33");
		colors[ImGuiCol_NavWindowingHighlight] = HexToColor("FFFFFFB3");
		colors[ImGuiCol_PlotHistogram] = HexToColor("E6B300FF");
		colors[ImGuiCol_PlotHistogramHovered] = HexToColor("FF9900FF");
		colors[ImGuiCol_PlotLines] = HexToColor("FFFFFFFF");
		colors[ImGuiCol_PlotLinesHovered] = HexToColor("E6B300FF");
		colors[ImGuiCol_PopupBg] = HexToColor("1D1D24EB");
		colors[ImGuiCol_ResizeGrip] = HexToColor("FFFFFF1A");
		colors[ImGuiCol_ResizeGripActive] = HexToColor("C7D2FFE6");
		colors[ImGuiCol_ResizeGripHovered] = HexToColor("C7D2FF99");
		colors[ImGuiCol_ScrollbarBg] = HexToColor("33404D99");
		colors[ImGuiCol_ScrollbarGrab] = HexToColor("0C864E65");
		colors[ImGuiCol_ScrollbarGrabActive] = HexToColor("02B3916F");
		colors[ImGuiCol_ScrollbarGrabHovered] = HexToColor("05925966");
		colors[ImGuiCol_Separator] = HexToColor("1CA09A7E");
		colors[ImGuiCol_SeparatorActive] = HexToColor("0099B1FF");
		colors[ImGuiCol_SeparatorHovered] = HexToColor("048897FF");
		colors[ImGuiCol_SliderGrab] = HexToColor("FFFFFF4D");
		colors[ImGuiCol_SliderGrabActive] = HexToColor("04882F99");
		colors[ImGuiCol_Tab] = HexToColor("18999F60");
		colors[ImGuiCol_TabActive] = HexToColor("038B4CD7");
		colors[ImGuiCol_TabHovered] = HexToColor("1E784BCC");
		colors[ImGuiCol_TabUnfocused] = HexToColor("FFFFFF00");
		colors[ImGuiCol_TabUnfocusedActive] = HexToColor("5A5AA700");
		colors[ImGuiCol_TableBorderLight] = HexToColor("FFFFFF00");
		colors[ImGuiCol_TableBorderStrong] = HexToColor("00014000");
		colors[ImGuiCol_TableHeaderBg] = HexToColor("2E8D89FF");
		colors[ImGuiCol_TableRowBg] = HexToColor("3E72769A");
		colors[ImGuiCol_TableRowBgAlt] = HexToColor("154D3E9B");
		colors[ImGuiCol_Text] = HexToColor("E6E6E6FF");
		colors[ImGuiCol_TextDisabled] = HexToColor("999999FF");
		colors[ImGuiCol_TextSelectedBg] = HexToColor("00A87E5A");
		colors[ImGuiCol_TitleBg] = HexToColor("00998D8A");
		colors[ImGuiCol_TitleBgActive] = HexToColor("008D556B");
		colors[ImGuiCol_TitleBgCollapsed] = HexToColor("27B6C14A");
		colors[ImGuiCol_WindowBg] = HexToColor("00000096");

		//Styles
		styles.Alpha = 1.0;
		styles.AntiAliasedFill = true;
		styles.AntiAliasedLines = true;
		styles.AntiAliasedLinesUseTex = true;
		styles.ButtonTextAlign = ImVec2(0.5, 0.5);
		styles.CellPadding = ImVec2(4.0, 2.0);
		styles.ChildBorderSize = 1.0;
		styles.ChildRounding = 0.0;
		styles.CircleTessellationMaxError = 0.30000001192092896;
		styles.ColorButtonPosition = 1;
		styles.ColumnsMinSpacing = 6.0;
		styles.CurveTessellationTol = 1.25;
		styles.DisabledAlpha = 0.6000000238418579;
		styles.DisplaySafeAreaPadding = ImVec2(3.0, 3.0);
		styles.DisplayWindowPadding = ImVec2(19.0, 19.0);
		styles.FrameBorderSize = 0.0;
		styles.FramePadding = ImVec2(4.0, 3.0);
		styles.FrameRounding = 0.0;
		styles.GrabMinSize = 10.0;
		styles.GrabRounding = 0.0;
		styles.IndentSpacing = 21.0;
		styles.ItemInnerSpacing = ImVec2(4.0, 4.0);
		styles.ItemSpacing = ImVec2(8.0, 4.0);
		styles.LogSliderDeadzone = 4.0;
		styles.MouseCursorScale = 1.0;
		styles.PopupBorderSize = 0.0;
		styles.PopupRounding = 0.0;
		styles.ScrollbarRounding = 0.0;
		styles.ScrollbarSize = 14.0;
		styles.SelectableTextAlign = ImVec2(0.0, 0.0);
		styles.TabBorderSize = 0.0;
		styles.TabMinWidthForCloseButton = 0.0;
		styles.TabRounding = 0.0;
		styles.TouchExtraPadding = ImVec2(0.0, 0.0);
		styles.WindowBorderSize = 0.0;
		styles.WindowMenuButtonPosition = 0;
		styles.WindowMinSize = ImVec2(32.0, 32.0);
		styles.WindowPadding = ImVec2(8.0, 8.0);
		styles.WindowRounding = 0.0;
		styles.WindowTitleAlign = ImVec2(0.0, 0.5);
	}
	void DefaultTheme()
	{
		auto& styles = ImGui::GetStyle();

		// Colors
		auto colors = styles.Colors;
		colors[ImGuiCol_Border] = HexToColor("26383FFF");
		colors[ImGuiCol_BorderShadow] = HexToColor("33333300");
		colors[ImGuiCol_Button] = HexToColor("23303DFF");
		colors[ImGuiCol_ButtonActive] = HexToColor("474968FF");
		colors[ImGuiCol_ButtonHovered] = HexToColor("444C70FF");
		colors[ImGuiCol_CheckMark] = HexToColor("A5BCDBFF");
		colors[ImGuiCol_ChildBg] = HexToColor("1E262BFF");
		colors[ImGuiCol_DragDropTarget] = HexToColor("FFFF00E5");
		colors[ImGuiCol_FrameBg] = HexToColor("2D3F44FF");
		colors[ImGuiCol_FrameBgActive] = HexToColor("30383DFF");
		colors[ImGuiCol_FrameBgHovered] = HexToColor("26303DFF");
		colors[ImGuiCol_Header] = HexToColor("0000003D");
		colors[ImGuiCol_HeaderActive] = HexToColor("0070EAFF");
		colors[ImGuiCol_HeaderHovered] = HexToColor("1E2833CC");
		colors[ImGuiCol_MenuBarBg] = HexToColor("1E232DFF");
		colors[ImGuiCol_ModalWindowDimBg] = HexToColor("CCCCCC59");
		colors[ImGuiCol_NavHighlight] = HexToColor("4296F9FF");
		colors[ImGuiCol_NavWindowingDimBg] = HexToColor("CCCCCC33");
		colors[ImGuiCol_NavWindowingHighlight] = HexToColor("FFFFFFB2");
		colors[ImGuiCol_PlotHistogram] = HexToColor("E5B200FF");
		colors[ImGuiCol_PlotHistogramHovered] = HexToColor("FF9900FF");
		colors[ImGuiCol_PlotLines] = HexToColor("9B9B9BFF");
		colors[ImGuiCol_PlotLinesHovered] = HexToColor("FF6D59FF");
		colors[ImGuiCol_PopupBg] = HexToColor("14161CEF");
		colors[ImGuiCol_ResizeGrip] = HexToColor("A3C9F93F");
		colors[ImGuiCol_ResizeGripActive] = HexToColor("6D8CB2F2");
		colors[ImGuiCol_ResizeGripHovered] = HexToColor("A5BFDDAA");
		colors[ImGuiCol_ScrollbarBg] = HexToColor("1C1C1C63");
		colors[ImGuiCol_ScrollbarGrab] = HexToColor("875E5EFF");
		colors[ImGuiCol_ScrollbarGrabActive] = HexToColor("8E1919FF");
		colors[ImGuiCol_ScrollbarGrabHovered] = HexToColor("7C3A3AFF");
		colors[ImGuiCol_Separator] = HexToColor("333F49FF");
		colors[ImGuiCol_SeparatorActive] = HexToColor("6B91BFFF");
		colors[ImGuiCol_SeparatorHovered] = HexToColor("4F7299C6");
		colors[ImGuiCol_SliderGrab] = HexToColor("5977ADFF");
		colors[ImGuiCol_SliderGrabActive] = HexToColor("ADCCFFFF");
		colors[ImGuiCol_Tab] = HexToColor("1C262BFF");
		colors[ImGuiCol_TabActive] = HexToColor("333F49FF");
		colors[ImGuiCol_TabHovered] = HexToColor("969696CC");
		colors[ImGuiCol_TabUnfocused] = HexToColor("1C262BFF");
		colors[ImGuiCol_TabUnfocusedActive] = HexToColor("1C262BFF");
		colors[ImGuiCol_TableBorderLight] = HexToColor("3A3A3FFF");
		colors[ImGuiCol_TableBorderStrong] = HexToColor("4F4F59FF");
		colors[ImGuiCol_TableHeaderBg] = HexToColor("303033FF");
		colors[ImGuiCol_TableRowBg] = HexToColor("333F49FF");
		colors[ImGuiCol_TableRowBgAlt] = HexToColor("1C262BFF");
		colors[ImGuiCol_Text] = HexToColor("F2F4F9FF");
		colors[ImGuiCol_TextDisabled] = HexToColor("2B353DFF");
		colors[ImGuiCol_TextSelectedBg] = HexToColor("4296F959");
		colors[ImGuiCol_TitleBg] = HexToColor("232D38A5");
		colors[ImGuiCol_TitleBgActive] = HexToColor("212830FF");
		colors[ImGuiCol_TitleBgCollapsed] = HexToColor("26262682");
		colors[ImGuiCol_WindowBg] = HexToColor("1E2623FF");

		//Styles
		styles.Alpha = 1.0;
		styles.AntiAliasedFill = true;
		styles.AntiAliasedLines = true;
		styles.AntiAliasedLinesUseTex = true;
		styles.ButtonTextAlign = ImVec2(0.5, 0.5);
		styles.CellPadding = ImVec2(4.0, 2.0);
		styles.ChildBorderSize = 1.0;
		styles.ChildRounding = 5.0;
		styles.CircleTessellationMaxError = 0.30000001192092896;
		styles.ColorButtonPosition = 1;
		styles.ColumnsMinSpacing = 6.0;
		styles.CurveTessellationTol = 1.25;
		styles.DisabledAlpha = 0.6000000238418579;
		styles.DisplaySafeAreaPadding = ImVec2(3.0, 3.0);
		styles.DisplayWindowPadding = ImVec2(19.0, 19.0);
		styles.FrameBorderSize = 0.0;
		styles.FramePadding = ImVec2(4.0, 3.0);
		styles.FrameRounding = 4.0;
		styles.GrabMinSize = 10.0;
		styles.GrabRounding = 4.0;
		styles.IndentSpacing = 21.0;
		styles.ItemInnerSpacing = ImVec2(4.0, 4.0);
		styles.ItemSpacing = ImVec2(8.0, 4.0);
		styles.LogSliderDeadzone = 4.0;
		styles.MouseCursorScale = 1.0;
		styles.PopupBorderSize = 1.0;
		styles.PopupRounding = 0.0;
		styles.ScrollbarRounding = 9.0;
		styles.ScrollbarSize = 14.0;
		styles.SelectableTextAlign = ImVec2(0.0, 0.0);
		styles.TabBorderSize = 0.0;
		styles.TabMinWidthForCloseButton = 0.0;
		styles.TabRounding = 4.0;
		styles.TouchExtraPadding = ImVec2(0.0, 0.0);
		styles.WindowBorderSize = 1.0;
		styles.WindowMenuButtonPosition = 0;
		styles.WindowMinSize = ImVec2(32.0, 32.0);
		styles.WindowPadding = ImVec2(8.0, 8.0);
		styles.WindowRounding = 0.0;
		styles.WindowTitleAlign = ImVec2(0.0, 0.5);
	}
	ImGuiStyle OutCurrentStyles(nlohmann::json stylesData)
	{
		ImGuiStyle style;
		style.Colors[ImGuiCol_Border] = HexToColor(stylesData["ImGuiCol_Border"]);
		style.Colors[ImGuiCol_BorderShadow] = HexToColor(stylesData["ImGuiCol_BorderShadow"]);
		style.Colors[ImGuiCol_Button] = HexToColor(stylesData["ImGuiCol_Button"]);
		style.Colors[ImGuiCol_ButtonActive] = HexToColor(stylesData["ImGuiCol_ButtonActive"]);
		style.Colors[ImGuiCol_ButtonHovered] = HexToColor(stylesData["ImGuiCol_ButtonHovered"]);
		style.Colors[ImGuiCol_CheckMark] = HexToColor(stylesData["ImGuiCol_CheckMark"]);
		style.Colors[ImGuiCol_ChildBg] = HexToColor(stylesData["ImGuiCol_ChildBg"]);
		style.Colors[ImGuiCol_DragDropTarget] = HexToColor(stylesData["ImGuiCol_DragDropTarget"]);
		style.Colors[ImGuiCol_FrameBg] = HexToColor(stylesData["ImGuiCol_FrameBg"]);
		style.Colors[ImGuiCol_FrameBgActive] = HexToColor(stylesData["ImGuiCol_FrameBgActive"]);
		style.Colors[ImGuiCol_FrameBgHovered] = HexToColor(stylesData["ImGuiCol_FrameBgHovered"]);
		style.Colors[ImGuiCol_Header] = HexToColor(stylesData["ImGuiCol_Header"]);
		style.Colors[ImGuiCol_HeaderActive] = HexToColor(stylesData["ImGuiCol_HeaderActive"]);
		style.Colors[ImGuiCol_HeaderHovered] = HexToColor(stylesData["ImGuiCol_HeaderHovered"]);
		style.Colors[ImGuiCol_MenuBarBg] = HexToColor(stylesData["ImGuiCol_MenuBarBg"]);
		style.Colors[ImGuiCol_ModalWindowDimBg] = HexToColor(stylesData["ImGuiCol_ModalWindowDimBg"]);
		style.Colors[ImGuiCol_NavHighlight] = HexToColor(stylesData["ImGuiCol_NavHighlight"]);
		style.Colors[ImGuiCol_NavWindowingDimBg] = HexToColor(stylesData["ImGuiCol_NavWindowingDimBg"]);
		style.Colors[ImGuiCol_NavWindowingHighlight] = HexToColor(stylesData["ImGuiCol_NavWindowingHighlight"]);
		style.Colors[ImGuiCol_PlotHistogram] = HexToColor(stylesData["ImGuiCol_PlotHistogram"]);
		style.Colors[ImGuiCol_PlotHistogramHovered] = HexToColor(stylesData["ImGuiCol_PlotHistogramHovered"]);
		style.Colors[ImGuiCol_PlotLines] = HexToColor(stylesData["ImGuiCol_PlotLines"]);
		style.Colors[ImGuiCol_PlotLinesHovered] = HexToColor(stylesData["ImGuiCol_PlotLinesHovered"]);
		style.Colors[ImGuiCol_PopupBg] = HexToColor(stylesData["ImGuiCol_PopupBg"]);
		style.Colors[ImGuiCol_ResizeGrip] = HexToColor(stylesData["ImGuiCol_ResizeGrip"]);
		style.Colors[ImGuiCol_ResizeGripActive] = HexToColor(stylesData["ImGuiCol_ResizeGripActive"]);
		style.Colors[ImGuiCol_ResizeGripHovered] = HexToColor(stylesData["ImGuiCol_ResizeGripHovered"]);
		style.Colors[ImGuiCol_ScrollbarBg] = HexToColor(stylesData["ImGuiCol_ScrollbarBg"]);
		style.Colors[ImGuiCol_ScrollbarGrab] = HexToColor(stylesData["ImGuiCol_ScrollbarGrab"]);
		style.Colors[ImGuiCol_ScrollbarGrabActive] = HexToColor(stylesData["ImGuiCol_ScrollbarGrabActive"]);
		style.Colors[ImGuiCol_ScrollbarGrabHovered] = HexToColor(stylesData["ImGuiCol_ScrollbarGrabHovered"]);
		style.Colors[ImGuiCol_Separator] = HexToColor(stylesData["ImGuiCol_Separator"]);
		style.Colors[ImGuiCol_SeparatorActive] = HexToColor(stylesData["ImGuiCol_SeparatorActive"]);
		style.Colors[ImGuiCol_SeparatorHovered] = HexToColor(stylesData["ImGuiCol_SeparatorHovered"]);
		style.Colors[ImGuiCol_SliderGrab] = HexToColor(stylesData["ImGuiCol_SliderGrab"]);
		style.Colors[ImGuiCol_SliderGrabActive] = HexToColor(stylesData["ImGuiCol_SliderGrabActive"]);
		style.Colors[ImGuiCol_Tab] = HexToColor(stylesData["ImGuiCol_Tab"]);
		style.Colors[ImGuiCol_TabActive] = HexToColor(stylesData["ImGuiCol_TabActive"]);
		style.Colors[ImGuiCol_TabHovered] = HexToColor(stylesData["ImGuiCol_TabHovered"]);
		style.Colors[ImGuiCol_TabUnfocused] = HexToColor(stylesData["ImGuiCol_TabUnfocused"]);
		style.Colors[ImGuiCol_TabUnfocusedActive] = HexToColor(stylesData["ImGuiCol_TabUnfocusedActive"]);
		style.Colors[ImGuiCol_TableBorderLight] = HexToColor(stylesData["ImGuiCol_TableBorderLight"]);
		style.Colors[ImGuiCol_TableBorderStrong] = HexToColor(stylesData["ImGuiCol_TableBorderStrong"]);
		style.Colors[ImGuiCol_TableHeaderBg] = HexToColor(stylesData["ImGuiCol_TableHeaderBg"]);
		style.Colors[ImGuiCol_TableRowBg] = HexToColor(stylesData["ImGuiCol_TableRowBg"]);
		style.Colors[ImGuiCol_TableRowBgAlt] = HexToColor(stylesData["ImGuiCol_TableRowBgAlt"]);
		style.Colors[ImGuiCol_Text] = HexToColor(stylesData["ImGuiCol_Text"]);
		style.Colors[ImGuiCol_TextDisabled] = HexToColor(stylesData["ImGuiCol_TextDisabled"]);
		style.Colors[ImGuiCol_TextSelectedBg] = HexToColor(stylesData["ImGuiCol_TextSelectedBg"]);
		style.Colors[ImGuiCol_TitleBg] = HexToColor(stylesData["ImGuiCol_TitleBg"]);
		style.Colors[ImGuiCol_TitleBgActive] = HexToColor(stylesData["ImGuiCol_TitleBgActive"]);
		style.Colors[ImGuiCol_TitleBgCollapsed] = HexToColor(stylesData["ImGuiCol_TitleBgCollapsed"]);
		style.Colors[ImGuiCol_WindowBg] = HexToColor(stylesData["ImGuiCol_WindowBg"]);

		style.Alpha = stylesData["Alpha"];
		style.DisabledAlpha = stylesData["DisabledAlpha"];
		style.WindowPadding.x = stylesData["WindowPaddingx"];
		style.WindowPadding.y = stylesData["WindowPaddingy"];
		style.WindowRounding = stylesData["WindowRounding"];
		style.WindowBorderSize = stylesData["WindowBorderSize"];
		style.WindowMinSize.x = stylesData["WindowMinSizex"];
		style.WindowMinSize.y = stylesData["WindowMinSizey"];
		style.WindowTitleAlign.x = stylesData["WindowTitleAlignx"];
		style.WindowTitleAlign.y = stylesData["WindowTitleAligny"];
		style.WindowMenuButtonPosition = stylesData["WindowMenuButtonPosition"];
		style.ChildRounding = stylesData["ChildRounding"];
		style.ChildBorderSize = stylesData["ChildBorderSize"];
		style.PopupRounding = stylesData["PopupRounding"];
		style.PopupBorderSize = stylesData["PopupBorderSize"];
		style.FramePadding.x = stylesData["FramePaddingx"];
		style.FramePadding.y = stylesData["FramePaddingy"];
		style.FrameRounding = stylesData["FrameRounding"];
		style.FrameBorderSize = stylesData["FrameBorderSize"];
		style.ItemSpacing.x = stylesData["ItemSpacingx"];
		style.ItemSpacing.y = stylesData["ItemSpacingy"];
		style.ItemInnerSpacing.x = stylesData["ItemInnerSpacingx"];
		style.ItemInnerSpacing.y = stylesData["ItemInnerSpacingy"];
		style.CellPadding.x = stylesData["CellPaddingx"];
		style.CellPadding.y = stylesData["CellPaddingy"];
		style.TouchExtraPadding.x = stylesData["TouchExtraPaddingx"];
		style.TouchExtraPadding.y = stylesData["TouchExtraPaddingy"];
		style.IndentSpacing = stylesData["IndentSpacing"];
		style.ColumnsMinSpacing = stylesData["ColumnsMinSpacing"];
		style.ScrollbarSize = stylesData["ScrollbarSize"];
		style.ScrollbarRounding = stylesData["ScrollbarRounding"];
		style.GrabMinSize = stylesData["GrabMinSize"];
		style.GrabRounding = stylesData["GrabRounding"];
		style.LogSliderDeadzone = stylesData["LogSliderDeadzone"];
		style.TabRounding = stylesData["TabRounding"];
		style.TabBorderSize = stylesData["TabBorderSize"];
		style.TabMinWidthForCloseButton = stylesData["TabMinWidthForCloseButton"];
		style.ColorButtonPosition = stylesData["ColorButtonPosition"];
		style.ButtonTextAlign.x = stylesData["ButtonTextAlignx"];
		style.ButtonTextAlign.y = stylesData["ButtonTextAligny"];
		style.SelectableTextAlign.x = stylesData["SelectableTextAlignx"];
		style.SelectableTextAlign.y = stylesData["SelectableTextAligny"];
		style.DisplayWindowPadding.x = stylesData["DisplayWindowPaddingx"];
		style.DisplayWindowPadding.y = stylesData["DisplayWindowPaddingy"];
		style.DisplaySafeAreaPadding.x = stylesData["DisplaySafeAreaPaddingx"];
		style.DisplaySafeAreaPadding.y = stylesData["DisplaySafeAreaPaddingy"];
		style.MouseCursorScale = stylesData["MouseCursorScale"];
		style.AntiAliasedLines = stylesData["AntiAliasedLines"];
		style.AntiAliasedLinesUseTex = stylesData["AntiAliasedLinesUseTex"];
		style.AntiAliasedFill = stylesData["AntiAliasedFill"];
		style.CurveTessellationTol = stylesData["CurveTessellationTol"];
		style.CircleTessellationMaxError = stylesData["CircleTessellationMaxError"];
		return style;
	}
	nlohmann::json GetCurrentStyles(ImGuiStyle style)
	{
		nlohmann::json stylesData;
		stylesData["ImGuiCol_Border"] = ColorToHex(style.Colors[ImGuiCol_Border]);
		stylesData["ImGuiCol_BorderShadow"] = ColorToHex(style.Colors[ImGuiCol_BorderShadow]);
		stylesData["ImGuiCol_Button"] = ColorToHex(style.Colors[ImGuiCol_Button]);
		stylesData["ImGuiCol_ButtonActive"] = ColorToHex(style.Colors[ImGuiCol_ButtonActive]);
		stylesData["ImGuiCol_ButtonHovered"] = ColorToHex(style.Colors[ImGuiCol_ButtonHovered]);
		stylesData["ImGuiCol_CheckMark"] = ColorToHex(style.Colors[ImGuiCol_CheckMark]);
		stylesData["ImGuiCol_ChildBg"] = ColorToHex(style.Colors[ImGuiCol_ChildBg]);
		stylesData["ImGuiCol_DragDropTarget"] = ColorToHex(style.Colors[ImGuiCol_DragDropTarget]);
		stylesData["ImGuiCol_FrameBg"] = ColorToHex(style.Colors[ImGuiCol_FrameBg]);
		stylesData["ImGuiCol_FrameBgActive"] = ColorToHex(style.Colors[ImGuiCol_FrameBgActive]);
		stylesData["ImGuiCol_FrameBgHovered"] = ColorToHex(style.Colors[ImGuiCol_FrameBgHovered]);
		stylesData["ImGuiCol_Header"] = ColorToHex(style.Colors[ImGuiCol_Header]);
		stylesData["ImGuiCol_HeaderActive"] = ColorToHex(style.Colors[ImGuiCol_HeaderActive]);
		stylesData["ImGuiCol_HeaderHovered"] = ColorToHex(style.Colors[ImGuiCol_HeaderHovered]);
		stylesData["ImGuiCol_MenuBarBg"] = ColorToHex(style.Colors[ImGuiCol_MenuBarBg]);
		stylesData["ImGuiCol_ModalWindowDimBg"] = ColorToHex(style.Colors[ImGuiCol_ModalWindowDimBg]);
		stylesData["ImGuiCol_NavHighlight"] = ColorToHex(style.Colors[ImGuiCol_NavHighlight]);
		stylesData["ImGuiCol_NavWindowingDimBg"] = ColorToHex(style.Colors[ImGuiCol_NavWindowingDimBg]);
		stylesData["ImGuiCol_NavWindowingHighlight"] = ColorToHex(style.Colors[ImGuiCol_NavWindowingHighlight]);
		stylesData["ImGuiCol_PlotHistogram"] = ColorToHex(style.Colors[ImGuiCol_PlotHistogram]);
		stylesData["ImGuiCol_PlotHistogramHovered"] = ColorToHex(style.Colors[ImGuiCol_PlotHistogramHovered]);
		stylesData["ImGuiCol_PlotLines"] = ColorToHex(style.Colors[ImGuiCol_PlotLines]);
		stylesData["ImGuiCol_PlotLinesHovered"] = ColorToHex(style.Colors[ImGuiCol_PlotLinesHovered]);
		stylesData["ImGuiCol_PopupBg"] = ColorToHex(style.Colors[ImGuiCol_PopupBg]);
		stylesData["ImGuiCol_ResizeGrip"] = ColorToHex(style.Colors[ImGuiCol_ResizeGrip]);
		stylesData["ImGuiCol_ResizeGripActive"] = ColorToHex(style.Colors[ImGuiCol_ResizeGripActive]);
		stylesData["ImGuiCol_ResizeGripHovered"] = ColorToHex(style.Colors[ImGuiCol_ResizeGripHovered]);
		stylesData["ImGuiCol_ScrollbarBg"] = ColorToHex(style.Colors[ImGuiCol_ScrollbarBg]);
		stylesData["ImGuiCol_ScrollbarGrab"] = ColorToHex(style.Colors[ImGuiCol_ScrollbarGrab]);
		stylesData["ImGuiCol_ScrollbarGrabActive"] = ColorToHex(style.Colors[ImGuiCol_ScrollbarGrabActive]);
		stylesData["ImGuiCol_ScrollbarGrabHovered"] = ColorToHex(style.Colors[ImGuiCol_ScrollbarGrabHovered]);
		stylesData["ImGuiCol_Separator"] = ColorToHex(style.Colors[ImGuiCol_Separator]);
		stylesData["ImGuiCol_SeparatorActive"] = ColorToHex(style.Colors[ImGuiCol_SeparatorActive]);
		stylesData["ImGuiCol_SeparatorHovered"] = ColorToHex(style.Colors[ImGuiCol_SeparatorHovered]);
		stylesData["ImGuiCol_SliderGrab"] = ColorToHex(style.Colors[ImGuiCol_SliderGrab]);
		stylesData["ImGuiCol_SliderGrabActive"] = ColorToHex(style.Colors[ImGuiCol_SliderGrabActive]);
		stylesData["ImGuiCol_Tab"] = ColorToHex(style.Colors[ImGuiCol_Tab]);
		stylesData["ImGuiCol_TabActive"] = ColorToHex(style.Colors[ImGuiCol_TabActive]);
		stylesData["ImGuiCol_TabHovered"] = ColorToHex(style.Colors[ImGuiCol_TabHovered]);
		stylesData["ImGuiCol_TabUnfocused"] = ColorToHex(style.Colors[ImGuiCol_TabUnfocused]);
		stylesData["ImGuiCol_TabUnfocusedActive"] = ColorToHex(style.Colors[ImGuiCol_TabUnfocusedActive]);
		stylesData["ImGuiCol_TableBorderLight"] = ColorToHex(style.Colors[ImGuiCol_TableBorderLight]);
		stylesData["ImGuiCol_TableBorderStrong"] = ColorToHex(style.Colors[ImGuiCol_TableBorderStrong]);
		stylesData["ImGuiCol_TableHeaderBg"] = ColorToHex(style.Colors[ImGuiCol_TableHeaderBg]);
		stylesData["ImGuiCol_TableRowBg"] = ColorToHex(style.Colors[ImGuiCol_TableRowBg]);
		stylesData["ImGuiCol_TableRowBgAlt"] = ColorToHex(style.Colors[ImGuiCol_TableRowBgAlt]);
		stylesData["ImGuiCol_Text"] = ColorToHex(style.Colors[ImGuiCol_Text]);
		stylesData["ImGuiCol_TextDisabled"] = ColorToHex(style.Colors[ImGuiCol_TextDisabled]);
		stylesData["ImGuiCol_TextSelectedBg"] = ColorToHex(style.Colors[ImGuiCol_TextSelectedBg]);
		stylesData["ImGuiCol_TitleBg"] = ColorToHex(style.Colors[ImGuiCol_TitleBg]);
		stylesData["ImGuiCol_TitleBgActive"] = ColorToHex(style.Colors[ImGuiCol_TitleBgActive]);
		stylesData["ImGuiCol_TitleBgCollapsed"] = ColorToHex(style.Colors[ImGuiCol_TitleBgCollapsed]);
		stylesData["ImGuiCol_WindowBg"] = ColorToHex(style.Colors[ImGuiCol_WindowBg]);

		stylesData["Alpha"] = style.Alpha;
		stylesData["DisabledAlpha"] = style.DisabledAlpha;
		stylesData["WindowPaddingx"] = style.WindowPadding.x;
		stylesData["WindowPaddingy"] = style.WindowPadding.y;
		stylesData["WindowRounding"] = style.WindowRounding;
		stylesData["WindowBorderSize"] = style.WindowBorderSize;
		stylesData["WindowMinSizex"] = style.WindowMinSize.x;
		stylesData["WindowMinSizey"] = style.WindowMinSize.y;
		stylesData["WindowTitleAlignx"] = style.WindowTitleAlign.x;
		stylesData["WindowTitleAligny"] = style.WindowTitleAlign.y;
		stylesData["WindowMenuButtonPosition"] = style.WindowMenuButtonPosition;
		stylesData["ChildRounding"] = style.ChildRounding;
		stylesData["ChildBorderSize"] = style.ChildBorderSize;
		stylesData["PopupRounding"] = style.PopupRounding;
		stylesData["PopupBorderSize"] = style.PopupBorderSize;
		stylesData["FramePaddingx"] = style.FramePadding.x;
		stylesData["FramePaddingy"] = style.FramePadding.y;
		stylesData["FrameRounding"] = style.FrameRounding;
		stylesData["FrameBorderSize"] = style.FrameBorderSize;
		stylesData["ItemSpacingx"] = style.ItemSpacing.x;
		stylesData["ItemSpacingy"] = style.ItemSpacing.y;
		stylesData["ItemInnerSpacingx"] = style.ItemInnerSpacing.x;
		stylesData["ItemInnerSpacingy"] = style.ItemInnerSpacing.y;
		stylesData["CellPaddingx"] = style.CellPadding.x;
		stylesData["CellPaddingy"] = style.CellPadding.y;
		stylesData["TouchExtraPaddingx"] = style.TouchExtraPadding.x;
		stylesData["TouchExtraPaddingy"] = style.TouchExtraPadding.y;
		stylesData["IndentSpacing"] = style.IndentSpacing;
		stylesData["ColumnsMinSpacing"] = style.ColumnsMinSpacing;
		stylesData["ScrollbarSize"] = style.ScrollbarSize;
		stylesData["ScrollbarRounding"] = style.ScrollbarRounding;
		stylesData["GrabMinSize"] = style.GrabMinSize;
		stylesData["GrabRounding"] = style.GrabRounding;
		stylesData["LogSliderDeadzone"] = style.LogSliderDeadzone;
		stylesData["TabRounding"] = style.TabRounding;
		stylesData["TabBorderSize"] = style.TabBorderSize;
		stylesData["TabMinWidthForCloseButton"] = style.TabMinWidthForCloseButton;
		stylesData["ColorButtonPosition"] = style.ColorButtonPosition;
		stylesData["ButtonTextAlignx"] = style.ButtonTextAlign.x;
		stylesData["ButtonTextAligny"] = style.ButtonTextAlign.y;
		stylesData["SelectableTextAlignx"] = style.SelectableTextAlign.x;
		stylesData["SelectableTextAligny"] = style.SelectableTextAlign.y;
		stylesData["DisplayWindowPaddingx"] = style.DisplayWindowPadding.x;
		stylesData["DisplayWindowPaddingy"] = style.DisplayWindowPadding.y;
		stylesData["DisplaySafeAreaPaddingx"] = style.DisplaySafeAreaPadding.x;
		stylesData["DisplaySafeAreaPaddingy"] = style.DisplaySafeAreaPadding.y;
		stylesData["MouseCursorScale"] = style.MouseCursorScale;
		stylesData["AntiAliasedLines"] = style.AntiAliasedLines;
		stylesData["AntiAliasedLinesUseTex"] = style.AntiAliasedLinesUseTex;
		stylesData["AntiAliasedFill"] = style.AntiAliasedFill;
		stylesData["CurveTessellationTol"] = style.CurveTessellationTol;
		stylesData["CircleTessellationMaxError"] = style.CircleTessellationMaxError;
		return stylesData;
	}
	std::string ColorToHex(ImVec4& color)
	{
		char hex[16];
		snprintf(hex, sizeof(hex), "%02x%02x%02x%02x",
			static_cast<int>(ceil(color.x * 255)),
			static_cast<int>(ceil(color.y * 255)),
			static_cast<int>(ceil(color.z * 255)),
			static_cast<int>(ceil(color.w * 255))
		);
		for (int i = 0; i < 16; i++)
			hex[i] = toupper(hex[i]);
		return hex;
	}
	ImVec4 HexToColor(std::string hexString)
	{
		int r, g, b, a;
		sscanf_s(hexString.c_str(), "%02x%02x%02x%02x", &r, &g, &b, &a);
		ImVec4 color{ (float(r) / 255), (float(g) / 255), (float(b) / 255), (float(a) / 255) };
		return color;
	}
	std::string GetKeyName(short key)
	{
		if (key > 5)
			return ImGui::GetKeyName(key);

		switch (key)
		{
		case ImGuiMouseButton_Left:
			return "LMB";
		case ImGuiMouseButton_Right:
			return "RMB";
		case ImGuiMouseButton_Middle:
			return "MMB";
		case 3:
			return "Mouse X1";
		case 4:
			return "Mouse X2";
		}

		return "Unknown";
	}
	void FixModKey(short& legacyKey)
	{
		// Can cause incorrect input when both keys pressed, need to fix!
		switch (legacyKey)
		{

		case VK_CONTROL:
		{
			if (IsKeyDown(ImGuiKey_LeftCtrl))
				legacyKey = VK_LCONTROL;
			else if (IsKeyDown(ImGuiKey_RightCtrl))
				legacyKey = VK_RCONTROL;

			return;
		}
		case VK_SHIFT:
		{
			if (IsKeyDown(ImGuiKey_LeftShift))
				legacyKey = VK_LSHIFT;
			else if (IsKeyDown(ImGuiKey_RightShift))
				legacyKey = VK_RSHIFT;

			return;
		}

		}
	}
	static bool IsKeyReleased(ImGuiKey key)
	{
		if (key > 6)
			return ImGui::IsKeyReleased(key);

		switch (key)
		{
		case 1:
		case 2:
			return ImGui::IsMouseReleased(key - 1);
		case 4:
		case 5:
		case 6:
			return ImGui::IsMouseReleased(key - 2);
		}
		return false;
	}
	static bool IsKeyDown(ImGuiKey key)
	{
		if (key > 6)
			return ImGui::IsKeyDown(key);

		switch (key)
		{
		case 1:
		case 2:
			return ImGui::IsMouseDown(key - 1);
		case 4:
		case 5:
		case 6:
			return ImGui::IsMouseDown(key - 2);
		}
		return false;
	}
	static ImGuiKey LegacyToInput(short key)
	{
		switch (key)
		{
		case VK_TAB: return ImGuiKey_Tab;
		case VK_LEFT: return ImGuiKey_LeftArrow;
		case VK_RIGHT: return ImGuiKey_RightArrow;
		case VK_UP: return ImGuiKey_UpArrow;
		case VK_DOWN: return ImGuiKey_DownArrow;
		case VK_PRIOR: return ImGuiKey_PageUp;
		case VK_NEXT: return ImGuiKey_PageDown;
		case VK_HOME: return ImGuiKey_Home;
		case VK_END: return ImGuiKey_End;
		case VK_INSERT: return ImGuiKey_Insert;
		case VK_DELETE: return ImGuiKey_Delete;
		case VK_BACK: return ImGuiKey_Backspace;
		case VK_SPACE: return ImGuiKey_Space;
		case VK_RETURN: return ImGuiKey_Enter;
		case VK_ESCAPE: return ImGuiKey_Escape;
		case VK_OEM_7: return ImGuiKey_Apostrophe;
		case VK_OEM_COMMA: return ImGuiKey_Comma;
		case VK_OEM_MINUS: return ImGuiKey_Minus;
		case VK_OEM_PERIOD: return ImGuiKey_Period;
		case VK_OEM_2: return ImGuiKey_Slash;
		case VK_OEM_1: return ImGuiKey_Semicolon;
		case VK_OEM_PLUS: return ImGuiKey_Equal;
		case VK_OEM_4: return ImGuiKey_LeftBracket;
		case VK_OEM_5: return ImGuiKey_Backslash;
		case VK_OEM_6: return ImGuiKey_RightBracket;
		case VK_OEM_3: return ImGuiKey_GraveAccent;
		case VK_CAPITAL: return ImGuiKey_CapsLock;
		case VK_SCROLL: return ImGuiKey_ScrollLock;
		case VK_NUMLOCK: return ImGuiKey_NumLock;
		case VK_SNAPSHOT: return ImGuiKey_PrintScreen;
		case VK_PAUSE: return ImGuiKey_Pause;
		case VK_NUMPAD0: return ImGuiKey_Keypad0;
		case VK_NUMPAD1: return ImGuiKey_Keypad1;
		case VK_NUMPAD2: return ImGuiKey_Keypad2;
		case VK_NUMPAD3: return ImGuiKey_Keypad3;
		case VK_NUMPAD4: return ImGuiKey_Keypad4;
		case VK_NUMPAD5: return ImGuiKey_Keypad5;
		case VK_NUMPAD6: return ImGuiKey_Keypad6;
		case VK_NUMPAD7: return ImGuiKey_Keypad7;
		case VK_NUMPAD8: return ImGuiKey_Keypad8;
		case VK_NUMPAD9: return ImGuiKey_Keypad9;
		case VK_DECIMAL: return ImGuiKey_KeypadDecimal;
		case VK_DIVIDE: return ImGuiKey_KeypadDivide;
		case VK_MULTIPLY: return ImGuiKey_KeypadMultiply;
		case VK_SUBTRACT: return ImGuiKey_KeypadSubtract;
		case VK_ADD: return ImGuiKey_KeypadAdd;
		case IM_VK_KEYPAD_ENTER: return ImGuiKey_KeypadEnter;
		case VK_LSHIFT: return ImGuiKey_LeftShift;
		case VK_LCONTROL: return ImGuiKey_LeftCtrl;
		case VK_LMENU: return ImGuiKey_LeftAlt;
		case VK_LWIN: return ImGuiKey_LeftSuper;
		case VK_RSHIFT: return ImGuiKey_RightShift;
		case VK_RCONTROL: return ImGuiKey_RightCtrl;
		case VK_RMENU: return ImGuiKey_RightAlt;
		case VK_RWIN: return ImGuiKey_RightSuper;
		case VK_APPS: return ImGuiKey_Menu;
		case '0': return ImGuiKey_0;
		case '1': return ImGuiKey_1;
		case '2': return ImGuiKey_2;
		case '3': return ImGuiKey_3;
		case '4': return ImGuiKey_4;
		case '5': return ImGuiKey_5;
		case '6': return ImGuiKey_6;
		case '7': return ImGuiKey_7;
		case '8': return ImGuiKey_8;
		case '9': return ImGuiKey_9;
		case 'A': return ImGuiKey_A;
		case 'B': return ImGuiKey_B;
		case 'C': return ImGuiKey_C;
		case 'D': return ImGuiKey_D;
		case 'E': return ImGuiKey_E;
		case 'F': return ImGuiKey_F;
		case 'G': return ImGuiKey_G;
		case 'H': return ImGuiKey_H;
		case 'I': return ImGuiKey_I;
		case 'J': return ImGuiKey_J;
		case 'K': return ImGuiKey_K;
		case 'L': return ImGuiKey_L;
		case 'M': return ImGuiKey_M;
		case 'N': return ImGuiKey_N;
		case 'O': return ImGuiKey_O;
		case 'P': return ImGuiKey_P;
		case 'Q': return ImGuiKey_Q;
		case 'R': return ImGuiKey_R;
		case 'S': return ImGuiKey_S;
		case 'T': return ImGuiKey_T;
		case 'U': return ImGuiKey_U;
		case 'V': return ImGuiKey_V;
		case 'W': return ImGuiKey_W;
		case 'X': return ImGuiKey_X;
		case 'Y': return ImGuiKey_Y;
		case 'Z': return ImGuiKey_Z;
		case VK_F1: return ImGuiKey_F1;
		case VK_F2: return ImGuiKey_F2;
		case VK_F3: return ImGuiKey_F3;
		case VK_F4: return ImGuiKey_F4;
		case VK_F5: return ImGuiKey_F5;
		case VK_F6: return ImGuiKey_F6;
		case VK_F7: return ImGuiKey_F7;
		case VK_F8: return ImGuiKey_F8;
		case VK_F9: return ImGuiKey_F9;
		case VK_F10: return ImGuiKey_F10;
		case VK_F11: return ImGuiKey_F11;
		case VK_F12: return ImGuiKey_F12;
		case VK_LBUTTON: return ImGuiKey_MouseLeft;
		case VK_RBUTTON: return ImGuiKey_MouseRight;
		case VK_MBUTTON: return ImGuiKey_MouseMiddle;
		case VK_XBUTTON1: return ImGuiKey_MouseX1;
		case VK_XBUTTON2: return ImGuiKey_MouseX2;
		default: return ImGuiKey_None;
		}
	}
	static short InputToLegacy(ImGuiKey inputkey)
	{
		auto& io = ImGui::GetIO();
		if (inputkey > 4)
			return io.KeyMap[inputkey];

		switch (inputkey)
		{
		case ImGuiMouseButton_Left:
			return VK_LBUTTON;
		case ImGuiMouseButton_Right:
			return VK_RBUTTON;
		case ImGuiMouseButton_Middle:
			return VK_MBUTTON;
		case 3:
			return VK_XBUTTON1;
		case 4:
			return VK_XBUTTON2;
		}

		LOG_OUT(L_ERROR, "Failed to find legacy input");
		return -1;
	}
	static bool GroupPanelIsOpen(ImGuiID id)
	{
		ImGuiContext& g = *GImGui;
		ImGuiWindow* window = g.CurrentWindow;
		ImGuiStorage* storage = window->DC.StateStorage;

		return storage->GetInt(id, 1) != 0;
	}
	static void GroupPanelSetOpen(ImGuiID id, bool open)
	{
		ImGuiContext& g = *GImGui;
		ImGuiWindow* window = g.CurrentWindow;
		ImGuiStorage* storage = window->DC.StateStorage;

		storage->SetInt(id, open ? 1 : 0);
	}
	void AddUnderLine(ImColor col_)
	{
		ImVec2 min = ImGui::GetItemRectMin();
		ImVec2 max = ImGui::GetItemRectMax();
		min.y = max.y;
		ImGui::GetWindowDrawList()->AddLine(min, max, col_, 1.0f);
	}
	void TextURL(const char* name_, const char* URL_, bool SameLineBefore_, bool SameLineAfter_)
	{
		if (SameLineBefore_) { ImGui::SameLine(0.0f, ImGui::GetStyle().ItemInnerSpacing.x); }
		ImGui::PushStyleColor(ImGuiCol_Text, ImGui::GetStyle().Colors[ImGuiCol_ButtonHovered]);
		ImGui::Text(name_);
		ImGui::PopStyleColor();
		if (ImGui::IsItemHovered())
		{
			if (ImGui::IsMouseClicked(0))
			{
				ShellExecute(0, 0, URL_, 0, 0, SW_SHOW);
			}
			AddUnderLine(ImGui::GetStyle().Colors[ImGuiCol_ButtonHovered]);
			ImGui::SetTooltip("Open in browser: \n%s", URL_);
		}
		else
		{
			AddUnderLine(ImGui::GetStyle().Colors[ImGuiCol_Button]);
		}
		if (SameLineAfter_) { ImGui::SameLine(0.0f, ImGui::GetStyle().ItemInnerSpacing.x); }
	}
	void ShowHelpText(const char* text)
	{
		ImGui::BeginTooltip();
		ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
		ImGui::TextUnformatted(text);
		ImGui::PopTextWrapPos();
		ImGui::EndTooltip();
	}
	void HelpMarker(const char* desc)
	{
		ImGui::TextDisabled("(?)");
		if (ImGui::IsItemHovered())
			ShowHelpText(desc);
	}
	bool BeginGroupPanel(const char* label, bool node, const ImVec2& size)
	{
		ImGuiContext& g = *GImGui;
		ImGuiWindow* window = g.CurrentWindow;

		const ImGuiID id = window->GetID(label);
		ImGui::PushID(id);

		auto groupPanelPos = window->DC.CursorPos;
		auto itemSpacing = ImGui::GetStyle().ItemSpacing;

		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0.0f, 0.0f));
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0.0f, 0.0f));

		ImGui::BeginGroup(); // Outer group

		ImVec2 effectiveSize = size;
		if (size.x < 0.0f)
			effectiveSize.x = ImGui::GetContentRegionAvail().x;
		else
			effectiveSize.x = size.x;

		ImGui::Dummy(ImVec2(effectiveSize.x, 0.0f)); // Adjusting group x size

		auto frameHeight = ImGui::GetFrameHeight();
		ImGui::Dummy(ImVec2(frameHeight * 0.5f, 0.0f)); ImGui::SameLine(0.0f, 0.0f); // Inner group spacing
		ImGui::BeginGroup(); // Inner group

		ImGui::Dummy(ImVec2(frameHeight * 0.5f, 0.0f)); ImGui::SameLine(0.0f, 0.0f); // Name text spacing
		ImGui::TextUnformatted(label);

		ImRect leftRect = { ImGui::GetItemRectMin(), ImGui::GetItemRectMax() };
		ImVec2 rightMax = ImVec2(groupPanelPos.x + effectiveSize.x - frameHeight, leftRect.Max.y);
		ImRect rightRect = { { rightMax.x, leftRect.Min.x }, rightMax };
		ImGui::SameLine(0.0f, 0.0f);

		ImGui::Dummy(ImVec2(0.0, frameHeight + itemSpacing.y));

		if (node)
		{
			leftRect.Min.x = groupPanelPos.x;

			const ImVec2 text_size = ImGui::CalcTextSize(label);
			bool isOpen = GroupPanelIsOpen(id);

			bool hovered;
			bool toggled = ImGui::ButtonBehavior(leftRect, id, &hovered, nullptr, ImGuiButtonFlags_PressedOnClick);
			if (toggled)
			{
				isOpen = !isOpen;
				GroupPanelSetOpen(id, isOpen);
			}

			const ImU32 text_col = ImGui::GetColorU32(ImGuiCol_Text);
			ImGui::RenderArrow(window->DrawList, { groupPanelPos.x, groupPanelPos.y + text_size.y * 0.15f }, text_col,
				isOpen ? ImGuiDir_Down : ImGuiDir_Right, 0.7f);

			if (!isOpen)
			{
				ImGui::PopStyleVar(2);
				ImGui::EndGroup();
				ImGui::EndGroup();
				ImGui::PopID();

				_groupPanelStack.push_back({ leftRect, rightRect, true });
				return false;
			}
		}

		ImGui::PopStyleVar(2);

		ImGui::GetCurrentWindow()->ContentRegionRect.Max.x -= frameHeight * 0.5f;
		ImGui::GetCurrentWindow()->WorkRect.Max.x -= frameHeight * 0.5f;
		ImGui::GetCurrentWindow()->InnerRect.Max.x -= frameHeight * 0.5f;
		ImGui::GetCurrentWindow()->Size.x -= frameHeight;

		auto itemWidth = ImGui::CalcItemWidth();
		ImGui::PushItemWidth(ImMax(0.0f, itemWidth - frameHeight));

		_groupPanelStack.push_back({ leftRect, rightRect, false });
		return true;
	}
	void EndGroupPanel()
	{
		IM_ASSERT(_groupPanelStack.Size > 0); // Mismatched BeginGroupPanel()/EndGroupPanel() calls
		auto& info = _groupPanelStack.back();
		_groupPanelStack.pop_back();

		if (info.collapsed)
			return;

		ImGui::PopItemWidth();

		auto itemSpacing = ImGui::GetStyle().ItemSpacing;
		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0.0f, 0.0f));
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0.0f, 0.0f));

		ImGui::EndGroup(); // Inner group

		auto frameHeight = ImGui::GetFrameHeight();
		ImGui::SameLine(0.0f, 0.0f);
		ImGui::Dummy(ImVec2(frameHeight * 0.5f, 0.0f));
		ImGui::Dummy(ImVec2(0.0, frameHeight - frameHeight * 0.5f - itemSpacing.y));

		ImGui::EndGroup(); // Outer group

		// Outer group rect
		auto itemMin = ImGui::GetItemRectMin();
		auto itemMax = ImGui::GetItemRectMax();

		ImVec2 halfFrame = ImVec2(frameHeight * 0.25f, frameHeight) * 0.5f;
		ImRect frameRect = ImRect(itemMin + halfFrame, itemMax - ImVec2(halfFrame.x, 0.0f));

		auto& leftRect = info.left;
		leftRect.Min.x -= itemSpacing.x;
		leftRect.Max.x += itemSpacing.x;

		bool hasRightPart = info.right.Min.x != info.right.Max.x;
		auto& rightRect = info.right;

		if (hasRightPart)
		{
			rightRect.Min.x -= itemSpacing.x;
			rightRect.Max.x += itemSpacing.x;
		}

		// Drawing rectangle
		for (int i = 0; i < (hasRightPart ? 5 : 3); ++i)
		{
			switch (i)
			{
				// left half-plane
			case 0: ImGui::PushClipRect(ImVec2(-FLT_MAX, -FLT_MAX), ImVec2(leftRect.Min.x, FLT_MAX), true); break;
				// right half-plane
			case 1: ImGui::PushClipRect(ImVec2(leftRect.Max.x, -FLT_MAX), ImVec2(hasRightPart ? rightRect.Min.x : FLT_MAX, FLT_MAX), true); break;
				// bottom
			case 2: ImGui::PushClipRect(ImVec2(leftRect.Min.x, leftRect.Max.y), ImVec2(leftRect.Max.x, FLT_MAX), true); break;
				// bottom select
			case 3: ImGui::PushClipRect(ImVec2(rightRect.Min.x, rightRect.Max.y), ImVec2(rightRect.Max.x, FLT_MAX), true); break;
				// right hand-plane
			case 4: ImGui::PushClipRect(ImVec2(rightRect.Max.x, -FLT_MAX), ImVec2(FLT_MAX, FLT_MAX), true); break;
			}

			ImGui::GetWindowDrawList()->AddRect(
				frameRect.Min, frameRect.Max,
				ImColor(ImGui::GetStyleColorVec4(ImGuiCol_Border)),
				halfFrame.x);

			ImGui::PopClipRect();
		}

		ImGui::PopStyleVar(2);

		// Restore content region
		ImGui::GetCurrentWindow()->ContentRegionRect.Max.x += frameHeight * 0.5f;
		ImGui::GetCurrentWindow()->WorkRect.Max.x += frameHeight * 0.5f;
		ImGui::GetCurrentWindow()->InnerRect.Max.x += frameHeight * 0.5f;
		ImGui::GetCurrentWindow()->Size.x += frameHeight;

		// Add vertical spacing
		ImGui::Dummy(ImVec2(0.0f, 0.0f));

		ImGui::PopID();
	}
	void NextGroupPanelHeaderItem(const ImVec2& size, bool rightAlign)
	{
		IM_ASSERT(size.x > 0.0f); // Size should be specified
		IM_ASSERT(_groupPanelStack.Size > 0); // Mismatched BeginGroupPanel()/EndGroupPanel() calls
		auto& info = _groupPanelStack.back();

		ImGuiContext& g = *GImGui;
		ImGuiWindow* window = g.CurrentWindow;

		if (rightAlign)
		{
			if (info.right.Min.x != info.right.Max.x)
				info.right.Min.x -= g.Style.ItemSpacing.x;

			info.right.Min.x -= size.x;
		}
		else
			info.left.Max.x += g.Style.ItemSpacing.x;

		window->DC.CursorPos.x = rightAlign ? info.right.Min.x : info.left.Max.x;
		window->DC.CursorPos.y = info.left.Min.y - (size.y - ImGui::GetFrameHeight() + g.Style.FramePadding.y) / 2;

		if (!rightAlign)
			info.left.Max.x += size.x;
	}
	bool BeginSelectableGroupPanel(const char* label, bool& value, bool& changed, bool node, const ImVec2& size, const char* selectLabel)
	{
		bool opened = BeginGroupPanel(label, node, size);

		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0.0f, 0.0f));

		ImGuiContext& g = *GImGui;
		const ImGuiStyle& style = g.Style;

		const ImVec2 label_size = CalcTextSize(selectLabel, NULL, true);
		const float square_sz = GetFrameHeight();
		const ImVec2 checkbox_size = ImVec2(square_sz + (label_size.x > 0.0f ? style.ItemInnerSpacing.x + label_size.x : 0.0f), label_size.y + style.FramePadding.y * 2.0f);

		NextGroupPanelHeaderItem(checkbox_size, true);
		changed = Checkbox(selectLabel, &value);

		ImGui::PopStyleVar();
		return opened;
	}
	void EndSelectableGroupPanel()
	{
		EndGroupPanel();
	}
	ImVec2 CalcButtonSize(const char* label)
	{
		const ImVec2 label_size = CalcTextSize(label, NULL, true);

		ImGuiContext& g = *GImGui;
		const ImGuiStyle& style = g.Style;
		ImVec2 size = CalcItemSize({}, label_size.x + style.FramePadding.x * 2.0f, label_size.y + style.FramePadding.y * 2.0f);
		return size;
	}
}
namespace CALC
{
	ImVec2 matrix_clac_OpenGL(float matrix[4][4], float X, float Y, float Z, ImVec2 screen)
	{
		ImVec2 vec2;
		float ViewW = matrix[0][3] * X + matrix[1][3] * Y + matrix[2][3] * Z + matrix[3][3]; if (ViewW < 0) { vec2.x = -1; vec2.y = -1; return vec2; } ViewW = 1 / ViewW;
		vec2.x = screen.x / 2 + (matrix[0][0] * X + matrix[1][0] * Y + matrix[2][0] * Z + matrix[3][0]) * ViewW * screen.x / 2;
		vec2.y = screen.y / 2 - (matrix[0][1] * X + matrix[1][1] * (Y)+matrix[2][1] * Z + matrix[3][1]) * ViewW * screen.y / 2;
	}
	ImVec2 matrix_clac_D3D(float matrix[4][4], float X, float Y, float Z, ImVec2 screen)
	{
		ImVec2 vec2;
		float ViewW = matrix[3][0] * X + matrix[3][1] * Y + matrix[3][2] * Z + matrix[3][3]; if (ViewW < 0) { vec2.x = -1; vec2.y = -1; return vec2; } ViewW = 1 / ViewW;
		vec2.x = screen.x / 2 + (matrix[0][0] * X + matrix[0][1] * Y + matrix[0][2] * Z + matrix[0][3]) * ViewW * screen.x / 2;
		vec2.y = screen.y / 2 - (matrix[1][0] * X + matrix[1][1] * (Y)+matrix[1][2] * Z + matrix[1][3]) * ViewW * screen.y / 2;
	}
	void DrawOffscreenArrows(float x, float y, float z, const ImColor& color, float OutlineThickness, float TracerSize, float ArrowRadius)
	{
		ImRect screen_rect = { 0.0f, 0.0f, ImGui::GetIO().DisplaySize.x, ImGui::GetIO().DisplaySize.y };
		xyz entity_pos = { x,y,z };
		if (entity_pos.z > 0 && screen_rect.Contains({ entity_pos.x, entity_pos.y }))
			return;

		auto screen_center = screen_rect.GetCenter();
		auto angle = atan2(screen_center.y - entity_pos.y, screen_center.x - entity_pos.x);
		angle += entity_pos.z > 0 ? PI : 0.0f;

		ImVec2 arrow_center{
			screen_center.x + ArrowRadius * cosf(angle),
			screen_center.y + ArrowRadius * sinf(angle)
		};

		// Triangle
		std::array<ImVec2, 4> points{
			ImVec2(-22.0f, -8.6f),
			ImVec2(0.0f, 0.0f),
			ImVec2(-22.0f, 8.6f),
			ImVec2(-18.0f, 0.0f)
		};

		for (auto& point : points)
		{
			auto x = point.x * TracerSize;
			auto y = point.y * TracerSize;

			point.x = arrow_center.x + x * cosf(angle) - y * sinf(angle);
			point.y = arrow_center.y + x * sinf(angle) + y * cosf(angle);
		}

		auto draw = ImGui::GetBackgroundDrawList();

		float alpha = 1.0f;
		if (entity_pos.z > 0)
		{
			constexpr float nearThreshold = 200.0f * 200.0f;
			ImVec2 screen_outer_diff = {
				entity_pos.x < 0 ? abs(entity_pos.x) : (entity_pos.x > screen_rect.Max.x ? entity_pos.x - screen_rect.Max.x : 0.0f),
				entity_pos.y < 0 ? abs(entity_pos.y) : (entity_pos.y > screen_rect.Max.y ? entity_pos.y - screen_rect.Max.y : 0.0f),
			};
			float distance = static_cast<float>(std::pow(screen_outer_diff.x, 2) + std::pow(screen_outer_diff.y, 2));
			alpha = entity_pos.z < 0 ? 1.0f : (distance / nearThreshold);
		}
		auto arrowColor = color; // Copy
		arrowColor.Value.w = min(alpha, 1.0f);

		// Draw the arrow
		draw->AddTriangleFilled(points[0], points[1], points[3], arrowColor);
		draw->AddTriangleFilled(points[2], points[1], points[3], arrowColor);
		// draw->AddQuad(points[0], points[1], points[2], points[3], ImColor(0.0f, 0.0f, 0.0f, alpha), 0.6f);
		draw->AddQuad(points[0], points[1], points[2], points[3], ImColor(0.0f, 0.0f, 0.0f, alpha), OutlineThickness);
	}
}
namespace dx_hook
{
	struct Data
	{
		LPBYTE data;
		DWORD size;
	};
	static Data _customFontData{};
	// D3X HOOK DEFINITIONS
	typedef HRESULT(__stdcall* IDXGISwapChainPresent)(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags);
	void (*p_)();
	static std::unordered_set<void*> _inputLockers;
	// Definition of WndProc Hook. Its here to avoid dragging dependencies on <windows.h> types.
	static IDXGISwapChainPresent fnIDXGISwapChainPresent;
	static ID3D11Device* pDevice = nullptr;
	static float _globalFontSize = 16.0f;
	static bool _isCustomFontLoaded = false;
	static constexpr int _fontSizeStep = 13.0f;
	static constexpr int _fontSizeMax = 64;
	static constexpr int _fontsCount = _fontSizeMax / _fontSizeStep;
	static WNDPROC OriginalWndProcHandler;
	static ID3D11RenderTargetView* mainRenderTargetView;

	void AddInputLocker(void* id)
	{
		if (_inputLockers.count(id) == 0)
			_inputLockers.insert(id);
	}
	void RemoveInputLocker(void* id)
	{
		if (_inputLockers.count(id) > 0)
			_inputLockers.erase(id);
	}
	void SetInputLock(void* this_, bool value)
	{
		if (value)
			AddInputLocker(this_);
		else
			RemoveInputLocker(this_);
	}
	bool IsInputLocked()
	{
		return _inputLockers.size() > 0;
	}

	static LRESULT CALLBACK hWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		ImGuiIO& io = ImGui::GetIO();
		POINT mPos;
		GetCursorPos(&mPos);
		ScreenToClient(hWnd, &mPos);
		ImGui::GetIO().MousePos.x = static_cast<float>(mPos.x);
		ImGui::GetIO().MousePos.y = static_cast<float>(mPos.y);

		ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam);

		short key;
		bool keyUpEvent = true;
		switch (uMsg)
		{
		case WM_LBUTTONUP:
			key = VK_LBUTTON;
			break;
		case WM_RBUTTONUP:
			key = VK_RBUTTON;
			break;
		case WM_MBUTTONUP:
			key = VK_MBUTTON;
			break;
		case WM_XBUTTONUP:
			key = GET_XBUTTON_WPARAM(wParam) == XBUTTON1 ? VK_XBUTTON1 : VK_XBUTTON2;
			break;
		case WM_KEYUP:
			key = static_cast<short>(wParam);
			break;
		default:
			keyUpEvent = false;
			break;
		}

		if (IsInputLocked())
			return true;

		return CallWindowProc(OriginalWndProcHandler, hWnd, uMsg, wParam, lParam);
	}

#if _WIN64
	typedef HRESULT(APIENTRY* IDXGISwapChainPresent12)(IDXGISwapChain3* pSwapChain, UINT SyncInterval, UINT Flags);
	typedef void(APIENTRY* DrawInstanced)(ID3D12GraphicsCommandList* dCommandList, UINT VertexCountPerInstance, UINT InstanceCount, UINT StartVertexLocation, UINT StartInstanceLocation);
	typedef void(APIENTRY* DrawIndexedInstanced)(ID3D12GraphicsCommandList* dCommandList, UINT IndexCountPerInstance, UINT InstanceCount, UINT StartIndexLocation, INT BaseVertexLocation, UINT StartInstanceLocation);
	typedef void(APIENTRY* ExecuteCommandLists)(ID3D12CommandQueue* queue, UINT NumCommandLists, ID3D12CommandList* ppCommandLists);
	IDXGISwapChainPresent12 presentFuncDX12;
	ExecuteCommandLists executeCommandListFuncDX12;
	struct DirectX12MainFunctions
	{
		IDXGISwapChainPresent12 presentFunc12;
		DrawInstanced drawInstancedFunc12;
		DrawIndexedInstanced drawIndexedInstancedFunc12;
		ExecuteCommandLists executeCommandListFunc12;
	};

	ID3D12Device* Device12 = nullptr;
	ID3D12DescriptorHeap* DescriptorHeapBackBuffers12;
	ID3D12DescriptorHeap* DescriptorHeapImGuiRender12;
	ID3D12GraphicsCommandList* CommandList12;
	ID3D12CommandQueue* CommandQueue12;

	struct _FrameContext {
		ID3D12CommandAllocator* CommandAllocator12;
		ID3D12Resource* Resource12;
		D3D12_CPU_DESCRIPTOR_HANDLE DescriptorHandle12;
	};

	UINT BuffersCounts12 = -1;
	_FrameContext* FrameContext12;
	void OnInitializeDX12(HWND window, ID3D12Device* pDevice, UINT buffersCounts, ID3D12DescriptorHeap* pDescriptorHeapImGuiRender);
	void OnPostRenderDX12(ID3D12GraphicsCommandList* commandList);
	void OnPreRenderDX12();
	static HRESULT __stdcall Present_Hook12(IDXGISwapChain3* pChain, const UINT SyncInterval, const UINT Flags)
	{
		static BOOL g_bInitialised = false;

		if (!g_bInitialised)
		{
			auto result = (HRESULT)pChain->GetDevice(__uuidof(Device12), reinterpret_cast<void**>(&Device12));

			if (SUCCEEDED(result))
			{
				DXGI_SWAP_CHAIN_DESC Desc;
				pChain->GetDesc(&Desc);
				Desc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
				Desc.Windowed = ((GetWindowLongPtr(Desc.OutputWindow, GWL_STYLE) & WS_POPUP) != 0) ? false : true;

				BuffersCounts12 = Desc.BufferCount;
				FrameContext12 = new _FrameContext[BuffersCounts12];

				D3D12_DESCRIPTOR_HEAP_DESC DescriptorImGuiRender = {};
				DescriptorImGuiRender.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
				DescriptorImGuiRender.NumDescriptors = BuffersCounts12;
				DescriptorImGuiRender.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;

				if (Device12->CreateDescriptorHeap(&DescriptorImGuiRender, IID_PPV_ARGS(&DescriptorHeapImGuiRender12)) != S_OK)
					return presentFuncDX12(pChain, SyncInterval, Flags);

				ID3D12CommandAllocator* Allocator;
				if (Device12->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&Allocator)) != S_OK)
					return presentFuncDX12(pChain, SyncInterval, Flags);

				for (size_t i = 0; i < BuffersCounts12; i++) {
					FrameContext12[i].CommandAllocator12 = Allocator;
				}

				if (Device12->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, Allocator, NULL, IID_PPV_ARGS(&CommandList12)) != S_OK ||
					CommandList12->Close() != S_OK)
					return presentFuncDX12(pChain, SyncInterval, Flags);

				D3D12_DESCRIPTOR_HEAP_DESC DescriptorBackBuffers;
				DescriptorBackBuffers.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
				DescriptorBackBuffers.NumDescriptors = BuffersCounts12;
				DescriptorBackBuffers.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
				DescriptorBackBuffers.NodeMask = 1;

				if (Device12->CreateDescriptorHeap(&DescriptorBackBuffers, IID_PPV_ARGS(&DescriptorHeapBackBuffers12)) != S_OK)
					return presentFuncDX12(pChain, SyncInterval, Flags);

				const auto RTVDescriptorSize = Device12->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
				D3D12_CPU_DESCRIPTOR_HANDLE RTVHandle = DescriptorHeapBackBuffers12->GetCPUDescriptorHandleForHeapStart();

				for (size_t i = 0; i < BuffersCounts12; i++) {
					ID3D12Resource* pBackBuffer = nullptr;
					FrameContext12[i].DescriptorHandle12 = RTVHandle;
					pChain->GetBuffer(i, IID_PPV_ARGS(&pBackBuffer));
					Device12->CreateRenderTargetView(pBackBuffer, nullptr, RTVHandle);
					FrameContext12[i].Resource12 = pBackBuffer;
					RTVHandle.ptr += RTVDescriptorSize;
				}

				OnInitializeDX12(Desc.OutputWindow, Device12, BuffersCounts12, DescriptorHeapImGuiRender12);

				g_bInitialised = true;
			}
		}

		// render function
		if (!g_bInitialised || CommandQueue12 == nullptr)
			return presentFuncDX12(pChain, SyncInterval, Flags);

		OnPreRenderDX12();

		_FrameContext& CurrentFrameContext = FrameContext12[pChain->GetCurrentBackBufferIndex()];
		CurrentFrameContext.CommandAllocator12->Reset();

		D3D12_RESOURCE_BARRIER Barrier;
		Barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
		Barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
		Barrier.Transition.pResource = CurrentFrameContext.Resource12;
		Barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
		Barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;
		Barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;

		CommandList12->Reset(CurrentFrameContext.CommandAllocator12, nullptr);
		CommandList12->ResourceBarrier(1, &Barrier);
		CommandList12->OMSetRenderTargets(1, &CurrentFrameContext.DescriptorHandle12, FALSE, nullptr);
		CommandList12->SetDescriptorHeaps(1, &DescriptorHeapImGuiRender12);

		OnPostRenderDX12(CommandList12);

		Barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
		Barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT;
		CommandList12->ResourceBarrier(1, &Barrier);
		CommandList12->Close();
		CommandQueue12->ExecuteCommandLists(1, reinterpret_cast<ID3D12CommandList* const*>(&CommandList12));

		return presentFuncDX12(pChain, SyncInterval, Flags);
	}

	void ExecuteCommandLists_Hook(ID3D12CommandQueue* queue, UINT NumCommandLists, ID3D12CommandList* ppCommandLists) {
		if (!CommandQueue12)
			CommandQueue12 = queue;
		executeCommandListFuncDX12(queue, NumCommandLists, ppCommandLists);
	}

	static void GetHardwareAdapter(IDXGIFactory4* pFactory, IDXGIAdapter1** ppAdapter)
	{
		*ppAdapter = nullptr;
		for (UINT adapterIndex = 0; ; ++adapterIndex)
		{
			IDXGIAdapter1* pAdapter = nullptr;
			if (DXGI_ERROR_NOT_FOUND == pFactory->EnumAdapters1(adapterIndex, &pAdapter))
			{
				// No more adapters to enumerate.
				break;
			}

			// Check to see if the adapter supports Direct3D 12, but don't create the
			// actual device yet.
			if (SUCCEEDED(D3D12CreateDevice(pAdapter, D3D_FEATURE_LEVEL_11_0, _uuidof(ID3D12Device), nullptr)))
			{
				*ppAdapter = pAdapter;
				return;
			}
			pAdapter->Release();
		}
	}

	WNDCLASSEX WindowClass;
	HWND WindowHwnd;

	bool InitWindow() {
		WindowClass.cbSize = sizeof(WNDCLASSEX);
		WindowClass.style = CS_HREDRAW | CS_VREDRAW;
		WindowClass.lpfnWndProc = DefWindowProc;
		WindowClass.cbClsExtra = 0;
		WindowClass.cbWndExtra = 0;
		WindowClass.hInstance = GetModuleHandle(NULL);
		WindowClass.hIcon = NULL;
		WindowClass.hCursor = NULL;
		WindowClass.hbrBackground = NULL;
		WindowClass.lpszMenuName = NULL;
		WindowClass.lpszClassName = "MJ";
		WindowClass.hIconSm = NULL;
		RegisterClassEx(&WindowClass);
		WindowHwnd = CreateWindow(WindowClass.lpszClassName, "DirectX Window", WS_OVERLAPPEDWINDOW, 0, 0, 100, 100, NULL, NULL, WindowClass.hInstance, NULL);
		HHWND = WindowHwnd;
		if (WindowHwnd == NULL) {
			return false;
		}
		return true;
	}

	bool DeleteWindow() {
		DestroyWindow(WindowHwnd);
		UnregisterClass(WindowClass.lpszClassName, WindowClass.hInstance);
		if (WindowHwnd != NULL) {
			return false;
		}
		return true;
	}

	bool FindFunctions(DirectX12MainFunctions& mainFunctions) {
		if (InitWindow() == false) {
			return false;
		}

		HMODULE D3D12Module = GetModuleHandle("d3d12.dll");
		HMODULE DXGIModule = GetModuleHandle("dxgi.dll");
		if (D3D12Module == NULL || DXGIModule == NULL) {
			DeleteWindow();
			return false;
		}

		void* CreateDXGIFactory = GetProcAddress(DXGIModule, "CreateDXGIFactory");
		if (CreateDXGIFactory == NULL) {
			DeleteWindow();
			return false;
		}

		IDXGIFactory* Factory;
		if (((long(__stdcall*)(const IID&, void**))(CreateDXGIFactory))(__uuidof(IDXGIFactory), (void**)&Factory) < 0) {
			DeleteWindow();
			return false;
		}

		IDXGIAdapter* Adapter;
		if (Factory->EnumAdapters(0, &Adapter) == DXGI_ERROR_NOT_FOUND) {
			DeleteWindow();
			return false;
		}

		void* D3D12CreateDevice = GetProcAddress(D3D12Module, "D3D12CreateDevice");
		if (D3D12CreateDevice == NULL) {
			DeleteWindow();
			return false;
		}

		ID3D12Device* Device;
		if (((long(__stdcall*)(IUnknown*, D3D_FEATURE_LEVEL, const IID&, void**))(D3D12CreateDevice))(Adapter, D3D_FEATURE_LEVEL_11_0, __uuidof(ID3D12Device), (void**)&Device) < 0) {
			DeleteWindow();
			return false;
		}

		D3D12_COMMAND_QUEUE_DESC QueueDesc;
		QueueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
		QueueDesc.Priority = 0;
		QueueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
		QueueDesc.NodeMask = 0;

		ID3D12CommandQueue* CommandQueue;
		if (Device->CreateCommandQueue(&QueueDesc, __uuidof(ID3D12CommandQueue), (void**)&CommandQueue) < 0) {
			DeleteWindow();
			return false;
		}

		ID3D12CommandAllocator* CommandAllocator;
		if (Device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, __uuidof(ID3D12CommandAllocator), (void**)&CommandAllocator) < 0) {
			DeleteWindow();
			return false;
		}

		ID3D12GraphicsCommandList* CommandList;
		if (Device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, CommandAllocator, NULL, __uuidof(ID3D12GraphicsCommandList), (void**)&CommandList) < 0) {
			DeleteWindow();
			return false;
		}

		DXGI_RATIONAL RefreshRate;
		RefreshRate.Numerator = 60;
		RefreshRate.Denominator = 1;

		DXGI_MODE_DESC BufferDesc;
		BufferDesc.Width = 100;
		BufferDesc.Height = 100;
		BufferDesc.RefreshRate = RefreshRate;
		BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

		DXGI_SAMPLE_DESC SampleDesc;
		SampleDesc.Count = 1;
		SampleDesc.Quality = 0;

		DXGI_SWAP_CHAIN_DESC SwapChainDesc = {};
		SwapChainDesc.BufferDesc = BufferDesc;
		SwapChainDesc.SampleDesc = SampleDesc;
		SwapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		SwapChainDesc.BufferCount = 2;
		SwapChainDesc.OutputWindow = WindowHwnd;
		SwapChainDesc.Windowed = 1;
		SwapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
		SwapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

		IDXGISwapChain* SwapChain;
		if (Factory->CreateSwapChain(CommandQueue, &SwapChainDesc, &SwapChain) < 0) {
			DeleteWindow();
			return false;
		}

		typedef uint64_t uintx_t;
		auto MethodsTable = (uintx_t*)::calloc(150, sizeof(uintx_t));
		memcpy(MethodsTable, *(uintx_t**)Device, 44 * sizeof(uintx_t));
		memcpy(MethodsTable + 44, *(uintx_t**)CommandQueue, 19 * sizeof(uintx_t));
		memcpy(MethodsTable + 44 + 19, *(uintx_t**)CommandAllocator, 9 * sizeof(uintx_t));
		memcpy(MethodsTable + 44 + 19 + 9, *(uintx_t**)CommandList, 60 * sizeof(uintx_t));
		memcpy(MethodsTable + 44 + 19 + 9 + 60, *(uintx_t**)SwapChain, 18 * sizeof(uintx_t));

		mainFunctions.presentFunc12 = reinterpret_cast<decltype(mainFunctions.presentFunc12)>(MethodsTable[140]);
		mainFunctions.drawInstancedFunc12 = reinterpret_cast<decltype(mainFunctions.drawInstancedFunc12)>(MethodsTable[84]);
		mainFunctions.drawIndexedInstancedFunc12 = reinterpret_cast<decltype(mainFunctions.drawIndexedInstancedFunc12)>(MethodsTable[85]);
		mainFunctions.executeCommandListFunc12 = reinterpret_cast<decltype(mainFunctions.executeCommandListFunc12)>(MethodsTable[54]);

		free(MethodsTable);

		Device->Release();
		Device = NULL;
		CommandQueue->Release();
		CommandQueue = NULL;
		CommandAllocator->Release();
		CommandAllocator = NULL;
		CommandList->Release();
		CommandList = NULL;
		SwapChain->Release();
		SwapChain = NULL;
		DeleteWindow();
		return true;
	}

	void OnPreRenderDX12()
	{
		ImGui_ImplDX12_NewFrame();
		ImGui_ImplWin32_NewFrame();

		ImGui::NewFrame();

		(*p_)();

		ImGui::EndFrame();
	}

	void OnPostRenderDX12(ID3D12GraphicsCommandList* commandList)
	{
		ImGui::Render();
		ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), commandList);
	}

	void OnInitializeDX12(HWND window, ID3D12Device* pDevice, UINT buffersCounts, ID3D12DescriptorHeap* pDescriptorHeapImGuiRender)
	{
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

		_customFontData.data = getallfont();

		if (_customFontData.data == nullptr)
			return;

		for (int i = 0; i < _fontsCount; i++)
		{
			ImGuiIO& io = ImGui::GetIO();
			auto newFont = io.Fonts->AddFontFromMemoryTTF(_customFontData.data, _customFontData.size, static_cast<float>((i + 1) * _fontSizeStep), NULL, io.Fonts->GetGlyphRangesChineseSimplifiedCommon());
			if (newFont == nullptr)
				return;

			_fonts[i] = newFont;
		}
		SetupImGuiStyle();

		//Set OriginalWndProcHandler to the Address of the Original WndProc function
		OriginalWndProcHandler = reinterpret_cast<WNDPROC>(SetWindowLongPtr(window, GWLP_WNDPROC,
			reinterpret_cast<LONG_PTR>(hWndProc)));

		ImGui_ImplWin32_Init(window);
		ImGui_ImplDX12_Init(pDevice, buffersCounts, DXGI_FORMAT_R8G8B8A8_UNORM,
			pDescriptorHeapImGuiRender,
			pDescriptorHeapImGuiRender->GetCPUDescriptorHandleForHeapStart(),
			pDescriptorHeapImGuiRender->GetGPUDescriptorHandleForHeapStart());

		ImGui_ImplDX12_CreateDeviceObjects();
		ImGui::GetIO().ImeWindowHandle = window;

		io.SetPlatformImeDataFn = nullptr; // F**king bug take 4 hours of my life
	}

	void InitializeDX12Hooks()
	{
		DirectX12MainFunctions mainFuncs;
		if (!FindFunctions(mainFuncs))
		{
			return;
		}
		DetourTransactionBegin();
		DetourUpdateThread(GetCurrentThread());
		DetourAttach(&(PVOID&)mainFuncs.presentFunc12, Present_Hook12);
		DetourTransactionCommit();

		DetourTransactionBegin();
		DetourUpdateThread(GetCurrentThread());
		DetourAttach(&(PVOID&)mainFuncs.executeCommandListFunc12, ExecuteCommandLists_Hook);
		DetourTransactionCommit();
	}

#endif // _WIN64
	static void OnInitializeDX11(HWND window, ID3D11Device* pDevice, ID3D11DeviceContext* pContext, IDXGISwapChain* pChain)
	{
		ImGui::CreateContext();
		LOG_OUT(L_DEBUG, std::string("ImGui.CreateContext").c_str());
		ImPlot::CreateContext();
		LOG_OUT(L_DEBUG, std::string("ImPlot.CreateContext").c_str());
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
		io.Fonts->AddFontFromMemoryTTF(Font::getallfont(), sizeof(Font::getallfont()), FontSize, NULL, io.Fonts->GetGlyphRangesChineseFull());
		LOG_OUT(L_DEBUG, std::string("ImGui.AddFontFromMemoryTTF").c_str());
		if (fileName == "")
		{
			io.IniFilename = std::string(Process::GetModulePath(ModuleH_) + "\\imgui.ini").c_str();
			LOG_OUT(L_DEBUG, std::string("ImGui.File:" + Process::GetModulePath(ModuleH_) + "\\imgui.ini").c_str());
		}
		else
		{
			io.IniFilename = fileName.c_str();
			LOG_OUT(L_DEBUG, fileName.c_str());
		}
		/*
		ifstream fin("C:/simheibd.ttf");
		if (!fin)
		{
			LPBYTE data = getallfont();
			ofstream fout("C:/simheibd.ttf", ios::out | ios::binary);
			for (size_t i = 0; i < sizeof(data); i++)
			{
				fout << data[i];
			}
			fout.close();
		}
		if (fin){fin.close();}
		io.Fonts->AddFontFromFileTTF("c:/simheibd.ttf", 13.0f, NULL, io.Fonts->GetGlyphRangesChineseFull());
		*/
		/*
		_customFontData.data = getallfont();

		if (_customFontData.data == nullptr)
			return;

		for (int i = 0; i < _fontsCount; i++)
		{
			ImGuiIO& io = ImGui::GetIO();
			auto newFont = io.Fonts->AddFontFromMemoryTTF(_customFontData.data, _customFontData.size, static_cast<float>((i + 1) * _fontSizeStep),NULL, io.Fonts->GetGlyphRangesChineseSimplifiedCommon());
			if (newFont == nullptr)
				return;

			_fonts[i] = newFont;
		}
		*/
		ImGui::DefaultTheme();
		LOG_OUT(L_DEBUG, std::string("ImGui.SetupImGuiStyle").c_str());
		//Set OriginalWndProcHandler to the Address of the Original WndProc function
		OriginalWndProcHandler = reinterpret_cast<WNDPROC>(SetWindowLongPtr(window, GWLP_WNDPROC,
			reinterpret_cast<LONG_PTR>(hWndProc)));

		ImGui_ImplWin32_Init(window);
		LOG_OUT(L_DEBUG, std::string("ImGui.ImGui_ImplWin32_Init").c_str());
		ImGui_ImplDX11_Init(pDevice, pContext);
		LOG_OUT(L_DEBUG, std::string("ImGui.ImGui_ImplDX11_Init").c_str());
		ID3D11Texture2D* pBackBuffer;
		pChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<LPVOID*>(&pBackBuffer));
		LOG_OUT(L_DEBUG, std::string("ImGui.GetBuffer").c_str());
		pDevice->CreateRenderTargetView(pBackBuffer, nullptr, &mainRenderTargetView);
		LOG_OUT(L_DEBUG, std::string("ImGui.CreateRenderTargetView").c_str());
		pBackBuffer->Release();

		io.SetPlatformImeDataFn = nullptr; // F**king bug take 4 hours of my life
	}
	static void OnRenderDX11(ID3D11DeviceContext* pContext)
	{
		ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame();

		ImGuiIO& io = ImGui::GetIO(); (void)io;
		ImGui::NewFrame();

		(p_)();//绘制函数指针

		ImGui::EndFrame();
		ImGui::Render();

		pContext->OMSetRenderTargets(1, &mainRenderTargetView, nullptr);
		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
	}
	static HRESULT __stdcall Present_Hook(IDXGISwapChain* pChain, const UINT SyncInterval, const UINT Flags)
	{
		static BOOL g_bInitialised = false;

		// Main D3D11 Objects
		static ID3D11DeviceContext* pContext = nullptr;

		if (!g_bInitialised)
		{
			auto result = (HRESULT)pChain->GetDevice(__uuidof(pDevice), reinterpret_cast<void**>(&pDevice));

			if (SUCCEEDED(result))
			{
				pDevice->GetImmediateContext(&pContext);
				LOG_OUT(L_DEBUG, std::string("D3D11.GetImmediateContext").c_str());
				DXGI_SWAP_CHAIN_DESC sd;
				pChain->GetDesc(&sd);
				LOG_OUT(L_DEBUG, std::string("D3D11.GetDesc").c_str());
				OnInitializeDX11(sd.OutputWindow, pDevice, pContext, pChain);
				LOG_OUT(L_DEBUG, std::string("D3D11.OnInitializeDX11").c_str());
				g_bInitialised = true;
			}
		}

		// render function
		if (g_bInitialised)
			OnRenderDX11(pContext);

		return fnIDXGISwapChainPresent(pChain, SyncInterval, Flags);
	}
	static IDXGISwapChainPresent findDirect11Present()
	{
		WNDCLASSEX wc{ 0 };
		wc.cbSize = sizeof(wc);
		wc.lpfnWndProc = DefWindowProc;
		wc.lpszClassName = TEXT("Class");

		if (!RegisterClassEx(&wc))
		{
			return nullptr;
		}

		HWND hWnd = CreateWindow(wc.lpszClassName, TEXT(""), WS_DISABLED, 0, 0, 0, 0, NULL, NULL, NULL, nullptr);
		HHWND = hWnd;
		IDXGISwapChain* pSwapChain;

		D3D_FEATURE_LEVEL featureLevel;
		DXGI_SWAP_CHAIN_DESC swapChainDesc;
		ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));
		swapChainDesc.BufferCount = 1;
		swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		swapChainDesc.OutputWindow = hWnd;
		swapChainDesc.SampleDesc.Count = 1;
		swapChainDesc.Windowed = TRUE;  //((GetWindowLong(hWnd, GWL_STYLE) & WS_POPUP) != 0) ? FALSE : TRUE;
		swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
		swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

		// Main D3D11 Objects
		ID3D11DeviceContext* pContext = nullptr;
		ID3D11Device* pDevice = nullptr;

		if (FAILED(D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_WARP, nullptr, 0, nullptr, 1, D3D11_SDK_VERSION,
			&swapChainDesc, &pSwapChain, &pDevice, &featureLevel, &pContext)) &&
			FAILED(D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, 0, nullptr, 0, D3D11_SDK_VERSION,
				&swapChainDesc, &pSwapChain, &pDevice, &featureLevel, &pContext)))
		{
			DestroyWindow(swapChainDesc.OutputWindow);
			UnregisterClass(wc.lpszClassName, GetModuleHandle(nullptr));

			return nullptr;
		}
		LOG_OUT(L_DEBUG, std::string("D3D11.D3D11CreateDeviceAndSwapChain").c_str());
		const DWORD_PTR* pSwapChainVtable = reinterpret_cast<DWORD_PTR*>(pSwapChain);
		pSwapChainVtable = reinterpret_cast<DWORD_PTR*>(pSwapChainVtable[0]);

		auto swapChainPresent = reinterpret_cast<IDXGISwapChainPresent>(pSwapChainVtable[8]);

		pDevice->Release();
		pContext->Release();
		pSwapChain->Release();

		DestroyWindow(swapChainDesc.OutputWindow);
		LOG_OUT(L_DEBUG, std::string("D3D11.DestroyWindow").c_str());
		UnregisterClass(wc.lpszClassName, GetModuleHandle(nullptr));

		return swapChainPresent;
	}
	void InitializeDX11Hooks()
	{
		fnIDXGISwapChainPresent = findDirect11Present();
		if (fnIDXGISwapChainPresent == nullptr)
		{
			return;
		}
		DetourTransactionBegin();
		LOG_OUT(L_DEBUG, std::string("Detour.TransactionBegin").c_str());
		DetourUpdateThread(GetCurrentThread());
		LOG_OUT(L_DEBUG, std::string("Detour.UpdateThread").c_str());
		DetourAttach(&(PVOID&)fnIDXGISwapChainPresent, Present_Hook);
		LOG_OUT(L_DEBUG, std::string("Detour.Attach").c_str());
		DetourTransactionCommit();
		LOG_OUT(L_DEBUG, std::string("Detour.TransactionCommit").c_str());
	}
	void InitializeDX9Hooks()
	{
	}
	void InitializeDX10Hooks()
	{
	}
	void InitD3DHook(DXVersion version, void (*p)(), HWND* hwnd, HMODULE Module, int FontSize_,std::string inifile)
	{
		p_ = p;
		ModuleH_ = Module;
		FontSize = FontSize_;
		fileName = inifile;
		LOG_OUT(L_DEBUG, std::string("Hook.InitD3DHook").c_str());
		switch (version)
		{
		case DXVersion::D3D11:
			CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)InitializeDX11Hooks, 0, 0, 0);
			break;
		case DXVersion::D3D9:
			CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)InitializeDX9Hooks, 0, 0, 0);
			break;
		case DXVersion::D3D10:
			CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)InitializeDX10Hooks, 0, 0, 0);
			break;
		case DXVersion::D3D12:  //你应该知道IMGUI DX12默认只能编译成x64程序
#if _WIN32
			break;
#else
			CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)InitializeDX12Hooks, 0, 0, 0);
#endif // _WIN64

			break;
		default:
			break;
		}

		while (HHWND == 0)
		{
			Sleep(1);
		}
		hwnd = &HHWND;
	}
}
#include <d3d9.h>
#include <tchar.h>
#include <stdexcept>


#include "gui.h"
#include "../Xor/xor.hpp"
#include "../string.hpp"

#include "../Includes/settings.hpp"
#include "../imgui/imgui.h"
#include "../imgui/imgui_internal.h"

#define MENU_TAB_AIMBOT 1
#define MENU_TAB_VISUAL 2
#define MENU_TAB_RADAR 3
#define MENU_TAB_COLOR 4
#define MENU_TAB_MISC 5
#define MENU_TAB_SETTINGS 6

#define WEAPON_TAB_DEFAULT 0
#define WEAPON_TAB_RIFLE 1
#define WEAPON_TAB_SHOTGUN 2
#define WEAPON_TAB_SMG 3
#define WEAPON_TAB_PISTOL 4
#define WEAPON_TAB_SNIPER 5

#define MENU_SETTINGS_SETTINGS 1


namespace menu {
	int tab = MENU_TAB_AIMBOT;
	int weapon_tab = WEAPON_TAB_DEFAULT;
	int settings_page = MENU_SETTINGS_SETTINGS;
}

namespace image {
	IDirect3DTexture9* scene = nullptr;
	bool style = false;
}

namespace ImGui
{
	IMGUI_API bool Tab(unsigned int index, const char* label, int* selected, float width = 60, float height = 23)
	{
		ImGuiStyle& style = ImGui::GetStyle();
		ImVec4* colors = ImGui::GetStyle().Colors;
		ImColor color = ImLerp(colors[ImGuiCol_Header], colors[ImGuiCol_TitleBgActive], 0.80f);
		ImColor colorActive = ImColor(79, 79, 81, 255); /*style.Colors[ImGuiCol_ButtonActive]*/;
		ImColor colorHover = ImColor(62, 62, 66, 255)/*style.Colors[ImGuiCol_ButtonHovered]*/;


		if (index > 0)
			ImGui::SameLine();

		if (index == *selected)
		{
			style.Colors[ImGuiCol_Button] = colorActive;
			style.Colors[ImGuiCol_ButtonActive] = colorActive;
			style.Colors[ImGuiCol_ButtonHovered] = colorActive;
		}
		else
		{
			style.Colors[ImGuiCol_Button] = color;
			style.Colors[ImGuiCol_ButtonActive] = colorActive;
			style.Colors[ImGuiCol_ButtonHovered] = colorHover;
		}

		if (ImGui::Button(label, ImVec2(width, height)))
			*selected = index;

		style.Colors[ImGuiCol_Button] = color;
		style.Colors[ImGuiCol_ButtonActive] = colorActive;
		style.Colors[ImGuiCol_ButtonHovered] = colorHover;

		return *selected == index; 
	}
}

static int curtab;
#define LOADERW 400
#define LOADERH 400


void gui::render(/*LPDIRECT3DDEVICE9 D3dDevice*/)
{
	if (GetAsyncKeyState(VK_INSERT) & 1) gui::is_open = !gui::is_open;

	if (!gui::is_setup || !gui::is_open) return; // don't really need this but i like sanity checks
	auto* colors = ImGui::GetStyle().Colors;
	colors[ImGuiCol_Text] = ImVec4(0.43f, 0.43f, 0.43f, 1.00f);
	colors[ImGuiCol_TextDisabled] = ImVec4(0.43f, 0.43f, 0.43f, 1.00f);
	colors[ImGuiCol_WindowBg] = ImVec4(0.18f, 0.18f, 0.18f, 1.00f);
	colors[ImGuiCol_ChildBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	colors[ImGuiCol_PopupBg] = ImVec4(0.08f, 0.08f, 0.08f, 0.94f);
	colors[ImGuiCol_Border] = ImVec4(0.43f, 0.43f, 0.50f, 0.50f);
	colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	colors[ImGuiCol_FrameBg] = ImVec4(0.30f, 0.30f, 0.30f, 0.54f);
	colors[ImGuiCol_FrameBgHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.40f);
	colors[ImGuiCol_FrameBgActive] = ImVec4(0.26f, 0.59f, 0.98f, 0.67f);
	colors[ImGuiCol_TitleBg] = ImVec4(0.16f, 0.16f, 0.16f, 1.00f);
	colors[ImGuiCol_TitleBgActive] = ImVec4(0.16f, 0.16f, 0.16f, 1.00f);
	colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.00f, 0.00f, 0.00f, 0.51f);
	colors[ImGuiCol_MenuBarBg] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
	colors[ImGuiCol_ScrollbarBg] = ImVec4(0.02f, 0.02f, 0.02f, 0.53f);
	colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.31f, 0.31f, 0.31f, 1.00f);
	colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.41f, 0.41f, 0.41f, 1.00f);
	colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.51f, 0.51f, 0.51f, 1.00f);
	colors[ImGuiCol_CheckMark] = ImVec4(0.18f, 0.18f, 0.18f, 1.00f);
	colors[ImGuiCol_SliderGrab] = ImVec4(0.18f, 0.18f, 0.18f, 1.00f);
	colors[ImGuiCol_SliderGrabActive] = ImVec4(0.18f, 0.18f, 0.18f, 1.00f);
	colors[ImGuiCol_Button] = ImVec4(0.34f, 0.34f, 0.34f, 0.00f);
	colors[ImGuiCol_ButtonHovered] = ImVec4(0.53f, 0.53f, 0.53f, 0.00f);
	colors[ImGuiCol_ButtonActive] = ImVec4(0.44f, 0.44f, 0.44f, 1.00f);
	colors[ImGuiCol_Header] = ImVec4(0.26f, 0.59f, 0.98f, 0.31f);
	colors[ImGuiCol_HeaderHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.80f);
	colors[ImGuiCol_HeaderActive] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
	colors[ImGuiCol_Separator] = ImVec4(0.43f, 0.43f, 0.50f, 0.50f);
	colors[ImGuiCol_SeparatorHovered] = ImVec4(0.10f, 0.40f, 0.75f, 0.78f);
	colors[ImGuiCol_SeparatorActive] = ImVec4(0.10f, 0.40f, 0.75f, 1.00f);
	colors[ImGuiCol_ResizeGrip] = ImVec4(0.26f, 0.59f, 0.98f, 0.20f);
	colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.67f);
	colors[ImGuiCol_ResizeGripActive] = ImVec4(0.26f, 0.59f, 0.98f, 0.95f);
	colors[ImGuiCol_Tab] = ImVec4(0.18f, 0.35f, 0.58f, 0.86f);
	colors[ImGuiCol_TabHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.80f);
	colors[ImGuiCol_TabActive] = ImVec4(0.20f, 0.41f, 0.68f, 1.00f);
	colors[ImGuiCol_TabUnfocused] = ImVec4(0.07f, 0.10f, 0.15f, 0.97f);
	colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.14f, 0.26f, 0.42f, 1.00f);
	colors[ImGuiCol_PlotLines] = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
	colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
	colors[ImGuiCol_PlotHistogram] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
	colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
	colors[ImGuiCol_TableHeaderBg] = ImVec4(0.19f, 0.19f, 0.20f, 1.00f);
	colors[ImGuiCol_TableBorderStrong] = ImVec4(0.31f, 0.31f, 0.35f, 1.00f);
	colors[ImGuiCol_TableBorderLight] = ImVec4(0.23f, 0.23f, 0.25f, 1.00f);
	colors[ImGuiCol_TableRowBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	colors[ImGuiCol_TableRowBgAlt] = ImVec4(1.00f, 1.00f, 1.00f, 0.06f);
	colors[ImGuiCol_TextSelectedBg] = ImVec4(0.26f, 0.59f, 0.98f, 0.35f);
	colors[ImGuiCol_DragDropTarget] = ImVec4(1.00f, 1.00f, 0.00f, 0.90f);
	colors[ImGuiCol_NavHighlight] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
	colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
	colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
	colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.35f);


	ImGui::SetNextWindowSize(ImVec2(LOADERW, LOADERH));
	//ImGui::Begin(_xor_("##Dimensions").c_str(), &Settings.ShowMenu, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoScrollbar); // Create a window called "Hello, world!" and append into it.
	ImGui::Begin(("i love thermall!!!!"), nullptr, ImGuiWindowFlags_NoDecoration);
	ImGui::Text("Pasta School");// Create a window called "Hello, world!" and append into it.	
	ImGui::SetCursorPos({ 15,39.f });
	if (ImGui::Button("Visual", { 80, 26 }))
	{
		curtab = 0;
	}

	ImGui::SetCursorPos({ 105,39.f });
	if (ImGui::Button("Aimbot", { 80, 26 }))
	{
		curtab = 1;
	}
	ImGui::SetCursorPos({ 195,39.f });
	if (ImGui::Button("Close Aim", { 80, 26 }))
	{
		curtab = 2;
	}
	ImGui::SetCursorPos({ 285,39.f });
	if (ImGui::Button("Misc", { 80, 26 }))
	{
		curtab = 3;
	}
	if (curtab == 0)
	{
		ImGui::Checkbox("Box", &settings::visuals::box);
		ImGui::Checkbox("Rounded Box", &settings::visuals::roundedboxes);
		ImGui::Checkbox("3D Box", &settings::visuals::threed);
		ImGui::Checkbox("Corner Box", &settings::visuals::cornered_box);
		ImGui::Checkbox("Skeleton", &settings::visuals::gayskeleton);
		ImGui::Checkbox("Distance", &settings::visuals::distance);
		ImGui::Checkbox("Snaplines", &settings::visuals::line);
        // Edit 1 float using a slider from 0.0f to 1.0f

	}
	if (curtab == 1) {
		ImGui::Checkbox("Enable", &settings::aimbot::aimbot12);
		static const char* bonename1[] =
		{
			"LMB",
			"RMB",
		};
		ImGui::Combo(("Aimkey"), &settings::aimbot::key_wow, bonename1, IM_ARRAYSIZE(bonename1));
		ImGui::Checkbox("Enable Fov", &settings::aimbot::show_fov);
		ImGui::Checkbox("Trigger Bot", &settings::aimbot::bTargetedBuild);
		ImGui::Checkbox("Visible Check", &settings::visuals::vischeck);
		ImGui::SliderInt("Fov", &settings::aimbot::fov , 1, 400);
		ImGui::SliderInt("Smooth", &settings::aimbot::smoothness, 1, 25);
		ImGui::PushItemWidth(150);
		static const char* bonename[] =
		{
			"Head",
			"Neck",
			"Chest"
		};
		ImGui::Combo(("Hitbox"), &settings::aimbot::hitbox, bonename, IM_ARRAYSIZE(bonename));


	}
	if (curtab == 2)
	{
	}
	if (curtab == 3)
	{

		ImGui::ColorEdit3(("fov"), settings::visuals::FovColor);
		ImGui::Spacing();
		ImGui::ColorEdit3(("Box Visible"), settings::visuals::VisCOlor);
		ImGui::Spacing();
		ImGui::ColorEdit3(("Box InVisible"), settings::visuals::NOtVIs);
		ImGui::Spacing();
		ImGui::ColorEdit3(("Skeleton"), settings::visuals::skeletoncolor);
		//ImGui::SameLine();

	}


	ImGui::End();
	
}

	


//////////////////////////////////////////////////////////////////////////////
// This file is part of the Raven Game Engine			                    //
// Copyright ?2021-2023 Prime Zeng                                          //
//////////////////////////////////////////////////////////////////////////////

#include "ImGuiEngine.h"
#include "Engine.h"
#include "Window/Window.h"
#include <IconsMaterialDesignIcons.h>
#include <MaterialDesign.inl>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

namespace Raven 
{
	void ImGuiEngine::Initialize()
	{
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO();
		ImGui::StyleColorsDark();

		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
		io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;
		//io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows
		ImGuiStyle& style = ImGui::GetStyle();
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			style.WindowRounding = 0.0f;
			style.Colors[ImGuiCol_WindowBg].w = 1.0f;
		}
		io.Fonts->AddFontDefault();
		AddIcon();
		ImGui_ImplGlfw_InitForOpenGL(Engine::Get().GetModule<Window>()->GetNativeWindow(), true);
		ImGui_ImplOpenGL3_Init("#version 330");
	}

	void ImGuiEngine::Destroy()
	{

	}

	void ImGuiEngine::Render()
	{
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}

	void ImGuiEngine::Prepare()
	{
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
	}

	void ImGuiEngine::AddIcon()
	{
		ImGuiIO& io = ImGui::GetIO();

		static const ImWchar icons_ranges[] = { ICON_MIN_MDI, ICON_MAX_MDI, 0 };
		ImFontConfig icons_config;
		// merge in icons from Font Awesome
		icons_config.MergeMode = true;
		icons_config.PixelSnapH = true;
		icons_config.GlyphOffset.y = 1.0f;
		icons_config.OversampleH = icons_config.OversampleV = 1;
		icons_config.PixelSnapH = true;
		icons_config.SizePixels = 13.0f * 1.0f;

		io.Fonts->AddFontFromMemoryCompressedTTF(MaterialDesign_compressed_data, MaterialDesign_compressed_size, 16, &icons_config, icons_ranges);

	}

};

//////////////////////////////////////////////////////////////////////////////
// This file is part of the Raven Game Engine			                    //

//////////////////////////////////////////////////////////////////////////////

#include "SceneWindow.h"

namespace Raven 
{

	SceneWindow::SceneWindow()
	{
		title = "Scene";
	}

	void SceneWindow::OnImGui()
	{
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
		auto flags = ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse;
		ImGui::SetNextWindowBgAlpha(0.0f);
		ImGui::Begin(title.c_str(), &active, flags);
		ImVec2 offset = { 0.0f, 0.0f };


		ImGui::End();
		ImGui::PopStyleVar();
		
	}



	auto SceneWindow::Resize(uint32_t width, uint32_t height) -> void
	{
		bool resized = false;

		if (this->width != width || this->height != height)
		{
			resized = true;
			this->width = width;
			this->height = height;
		}
	}

};


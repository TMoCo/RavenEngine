//////////////////////////////////////////////////////////////////////////////
// This file is part of the Raven Game Engine			                    //

//////////////////////////////////////////////////////////////////////////////

#include "IconsMaterialDesignIcons.h"
#include "SceneWindow.h"
#include "ImGui/ImGuiHelpers.h"
#include "Render/RenderModule.h"
#include "Render/RenderTarget.h"
#include "Render/OpenGL/GLTexture.h"
#include <imgui_internal.h>
#include <ImGuizmo.h>
#include "Logger/Console.h"
#include "Editor.h"


namespace Raven 
{
	const ImVec4 SelectedColor(0.28f, 0.56f, 0.9f, 1.0f);

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

		DrawToolBar();

		auto sceneViewSize = ImGui::GetWindowContentRegionMax() - ImGui::GetWindowContentRegionMin() - offset / 2.0f;// - offset * 0.5f;
		auto sceneViewPosition = ImGui::GetWindowPos() + offset;

		sceneViewSize.x -= static_cast<int>(sceneViewSize.x) % 2 != 0 ? 1.0f : 0.0f;
		sceneViewSize.y -= static_cast<int>(sceneViewSize.y) % 2 != 0 ? 1.0f : 0.0f;

		Resize(static_cast<uint32_t>(sceneViewSize.x), static_cast<uint32_t>(sceneViewSize.y));

		// Scene Render Target
		Engine::GetModule<RenderModule>()->SetRTToWindow(false);
		auto previewTexture = Engine::GetModule<RenderModule>()->GetSceneRT();

		glm::vec2 sizeFactor; // scale factor defined by the render size over the texture actual size.
		sizeFactor.x = (float)previewTexture->GetSize().x / (float)previewTexture->GetActualSize().x;
		sizeFactor.y = (float)previewTexture->GetSize().y / (float)previewTexture->GetActualSize().y;


		ImGui::Image(
			(ImTextureID)previewTexture->GetRTTextureID(),
			{ sceneViewSize.x, sceneViewSize.y },
			{ sizeFactor.x, sizeFactor.y },
			{0,0}
		);

		auto windowSize = ImGui::GetWindowSize();
		ImVec2 minBound = sceneViewPosition;

		ImVec2 maxBound = { minBound.x + windowSize.x, minBound.y + windowSize.y };
		bool updateCamera = ImGui::IsMouseHoveringRect(minBound, maxBound);

		if (updateCamera)
		{
		

		}

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

		if (resized) 
		{
			auto previewTexture = Engine::GetModule<RenderModule>()->GetSceneRT();
			previewTexture->Resize(glm::ivec2(width, height));
		}
		
	}

	void SceneWindow::DrawToolBar()
	{

		ImGui::Indent();
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
		bool selected = false;
		auto& editor = static_cast<Editor&>(Engine::Get());
		{
			selected = editor.GetImGuizmoOperation() == 4;
			if (selected)
				ImGui::PushStyleColor(ImGuiCol_Text, SelectedColor);
			ImGui::SameLine();
			if (ImGui::Button(ICON_MDI_CURSOR_DEFAULT))
				editor.SetImGuizmoOperation(4);

			if (selected)
				ImGui::PopStyleColor();
			ImGuiHelper::Tooltip("Select");
		}

		ImGui::SameLine();
		ImGui::SeparatorEx(ImGuiSeparatorFlags_Vertical);
		ImGui::SameLine();

		{
			selected = editor.GetImGuizmoOperation() == ImGuizmo::TRANSLATE;
			if (selected)
				ImGui::PushStyleColor(ImGuiCol_Text, SelectedColor);
			ImGui::SameLine();
			if (ImGui::Button(ICON_MDI_ARROW_ALL))
				editor.SetImGuizmoOperation(ImGuizmo::TRANSLATE);

			if (selected)
				ImGui::PopStyleColor();
			ImGuiHelper::Tooltip("Translate");
		}

		{
			selected = editor.GetImGuizmoOperation() == ImGuizmo::ROTATE;
			if (selected)
				ImGui::PushStyleColor(ImGuiCol_Text, SelectedColor);

			ImGui::SameLine();
			if (ImGui::Button(ICON_MDI_ROTATE_ORBIT))
				editor.SetImGuizmoOperation(ImGuizmo::ROTATE);

			if (selected)
				ImGui::PopStyleColor();
			ImGuiHelper::Tooltip("Rotate");
		}

		{
			selected = editor.GetImGuizmoOperation() == ImGuizmo::SCALE;
			if (selected)
				ImGui::PushStyleColor(ImGuiCol_Text, SelectedColor);

			ImGui::SameLine();
			if (ImGui::Button(ICON_MDI_ARROW_EXPAND_ALL))
				editor.SetImGuizmoOperation(ImGuizmo::SCALE);

			if (selected)
				ImGui::PopStyleColor();
			ImGuiHelper::Tooltip("Scale");
		}

		ImGui::SameLine();
		ImGui::SeparatorEx(ImGuiSeparatorFlags_Vertical);
		ImGui::SameLine();

		{
			selected = editor.GetImGuizmoOperation() == ImGuizmo::BOUNDS;
			if (selected)
				ImGui::PushStyleColor(ImGuiCol_Text,SelectedColor);

			ImGui::SameLine();
			if (ImGui::Button(ICON_MDI_BORDER_NONE))
				editor.SetImGuizmoOperation(ImGuizmo::BOUNDS);

			if (selected)
				ImGui::PopStyleColor();
			ImGuiHelper::Tooltip("Bounds");
		}

		

		ImGui::SameLine();
		ImGui::SeparatorEx(ImGuiSeparatorFlags_Vertical);
		ImGui::SameLine();

		if (ImGui::Button("Gizmos " ICON_MDI_CHEVRON_DOWN))
			ImGui::OpenPopup("GizmosPopup");
		if (ImGui::BeginPopup("GizmosPopup"))
		{
			LOGW("TODO --- GizmosPopup");
			ImGui::EndPopup();
		}

		ImGui::PopStyleColor();
		ImGui::Unindent();
	}

};
	

//////////////////////////////////////////////////////////////////////////////
// This file is part of the Raven Game Engine			                    //

//////////////////////////////////////////////////////////////////////////////

#include "IconsMaterialDesignIcons.h"
#include "SceneWindow.h"
#include "ImGui/ImGuiHelpers.h"
#include "Render/RenderModule.h"
#include "Render/RenderTarget.h"
#include "Render/OpenGL/GLTexture.h"

#include "Logger/Console.h"
#include "Editor.h"
#include "Scene/Scene.h"
#include "Core/Camera.h"

#include <imgui_internal.h>
#include <glm/gtc/type_ptr.hpp>
#include "Scene/SceneManager.h"
#include "Devices/Input.h"

#include <ImGuizmo.h>



namespace Raven 
{
	const ImVec4 SelectedColor(0.28f, 0.56f, 0.9f, 1.0f);

	SceneWindow::SceneWindow()
	{
		title = "Scene";
	}

	void SceneWindow::OnImGui()
	{
		auto& editor = static_cast<Editor&>(Editor::Get());

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
		auto flags = ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse;
		ImGui::SetNextWindowBgAlpha(0.0f);
		ImGui::Begin(title.c_str(), &active, flags);
		Camera* camera = nullptr;
		Transform* transform = nullptr;

		bool gameView = false;

		if (editor.GetEditorState() == EditorState::Preview && !showCamera)
		{
			camera = editor.GetCamera().get();
			transform = &editor.GetEditorCameraTransform();
			auto currentScene = editor.GetModule<SceneManager>()->GetCurrentScene();
			currentScene->SetOverrideCamera(camera);
			currentScene->SetOverrideTransform(transform);
		}
		else
		{
			gameView = true;
			auto currentScene = editor.GetModule<SceneManager>()->GetCurrentScene();
			currentScene->SetOverrideCamera(nullptr);
			currentScene->SetOverrideTransform(nullptr);

			auto& registry = currentScene->GetRegistry();
			auto cameraView = registry.view<Camera>();
			if (!cameraView.empty())
			{
				camera = &registry.get<Camera>(cameraView.front());
			}
		}

		ImVec2 offset = { 0.0f, 0.0f };

		DrawToolBar();


		ImGuizmo::SetDrawlist();

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
			{ 0, sizeFactor.y },
			{ sizeFactor.x,0}
		);

		auto windowSize = ImGui::GetWindowSize();
		ImVec2 minBound = sceneViewPosition;

		ImVec2 maxBound = { minBound.x + windowSize.x, minBound.y + windowSize.y };
		bool updateCamera = ImGui::IsMouseHoveringRect(minBound, maxBound);

		editor.SetSceneActive(ImGui::IsWindowFocused() && !ImGuizmo::IsUsing() && updateCamera);

		ImGuizmo::SetRect(sceneViewPosition.x, sceneViewPosition.y, sceneViewSize.x, sceneViewSize.y);
		ImGui::GetWindowDrawList()->PushClipRect(sceneViewPosition, { sceneViewSize.x + sceneViewPosition.x, sceneViewSize.y + sceneViewPosition.y - 2.0f });;

		if (editor.GetEditorState() == EditorState::Preview && !showCamera && transform != nullptr)
		{
			const float* cameraViewPtr = glm::value_ptr(glm::inverse(transform->GetWorldMatrix()));

			ImGuizmo::DrawGrid(
				cameraViewPtr,
				glm::value_ptr(camera->GetProjectionMatrix()),
				glm::value_ptr(glm::mat4(1)), 100.f);

			//2D Grid..here

			ImGui::GetWindowDrawList()->PushClipRect(sceneViewPosition, { sceneViewSize.x + sceneViewPosition.x, sceneViewSize.y + sceneViewPosition.y - 2.0f });

			float viewManipulateRight = sceneViewPosition.x + sceneViewSize.x;
			float viewManipulateTop = sceneViewPosition.y;

			ImGuizmo::ViewManipulate(const_cast<float*>(cameraViewPtr), 8, ImVec2(viewManipulateRight - 70, viewManipulateTop + 32), ImVec2(64, 64), 0x10101010);

			editor.OnImGuizmo();

			if (editor.IsSceneActive() && !ImGuizmo::IsUsing() && Input::GetInput()->IsMouseClicked(KeyCode::MouseKey::ButtonLeft))
			{
				auto clickPos = Input::GetInput()->GetMousePosition() - glm::vec2(sceneViewPosition.x, sceneViewPosition.y);
				editor.SelectObject(editor.SendScreenRay(int32_t(clickPos.x), int32_t(clickPos.y), camera, int32_t(sceneViewSize.x), int32_t(sceneViewSize.y)));
			}
			DrawGizmos(sceneViewSize.x, sceneViewSize.y, offset.x, offset.y, editor.GetModule<SceneManager>()->GetCurrentScene());
		}
	

		if (ImGui::BeginDragDropTarget())
		{
			auto data = ImGui::AcceptDragDropPayload("AssetFile", ImGuiDragDropFlags_AcceptNoDrawDefaultRect);
			if (data)
			{
				std::string file = (char*)data->Data;
				LOGV("Receive file from assets window : {0}",file);
				editor.OpenFile(file);
			}
			ImGui::EndDragDropTarget();
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


		if (ImGui::Checkbox("Preview Camera", &showCamera)) 
		{
			auto& editor = static_cast<Editor&>(Editor::Get());
			auto currentScene = editor.GetModule<SceneManager>()->GetCurrentScene();
			currentScene->SetForceCamera(showCamera);
		}
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




	void SceneWindow::DrawGizmos(float width, float height, float xpos, float ypos, Scene* scene)
	{
		auto& editor = static_cast<Editor&>(Engine::Get());
		auto& camera = editor.GetCamera();
		auto & cameraTransform = editor.GetEditorCameraTransform();
		auto & registry = scene->GetRegistry();
		auto view = glm::inverse(cameraTransform.GetWorldMatrix());
		auto & proj = camera->GetProjectionMatrix();

	
	}


};
	

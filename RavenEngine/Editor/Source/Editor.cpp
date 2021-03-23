#include "EntryPoint.h"
#include "Editor.h"
#include "SceneWindow.h"
#include "HierarchyWindow.h"
#include "PropertiesWindow.h"
#include "AssetsWindow.h"
#include "Scene/Scene.h"
#include "Scene/SceneManager.h"
#include "Scene/Component/Component.h"
#include "Scene/Component/Transform.h"
#include "Scene/Component/Light.h"

#include "ResourceManager/Resources/Model.h"

#include "Core/Camera.h"
#include "Window/Window.h"
#include "ImGui/ImGuiHelpers.h"
#include <ImGuizmo.h>
#include <imgui_internal.h>
#include <imgui.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace Raven 
{

	void Editor::Initialize()
	{
		Engine::Initialize();

		editorWindows.emplace_back(std::make_unique<SceneWindow>());
		editorWindows.emplace_back(std::make_unique<HierarchyWindow>());
		editorWindows.emplace_back(std::make_unique<PropertiesWindow>());
		editorWindows.emplace_back(std::make_unique<AssetsWindow>());

		GetModule<SceneManager>()->AddScene(new Scene("Test"));

		iconMap[typeid(Transform).hash_code()] = ICON_MDI_VECTOR_LINE;
		iconMap[typeid(Editor).hash_code()] = ICON_MDI_SQUARE;
		iconMap[typeid(Light).hash_code()] = ICON_MDI_LIGHTBULB;
		iconMap[typeid(Camera).hash_code()] = ICON_MDI_CAMERA;
		iconMap[typeid(Model).hash_code()] = ICON_MDI_SHAPE;

		ImGuizmo::SetGizmoSizeClipSpace(0.25f);
		auto winSize = Engine::Get().GetModule<Window>()->GetWindowSize();
		camera = std::make_unique<Camera>(
			-20.0f,
			-40.0f,
			glm::vec3(-31.0f, 12.0f, 51.0f),
			45.0f,
			0.1f,
			1000.0f,
			(float)winSize.x / (float)winSize.y);

		editorCameraTransform.SetLocalPosition({ -31.0f, 12.0f, 51.0f });
		editorCameraTransform.SetLocalOrientation(glm::radians(glm::vec3{ -20.0f, -40.0f, 0.0f }));
		SetEditorState(EditorState::Preview);
	}

	void Editor::OnImGui()
	{
		DrawMenu();
		BeginDockSpace();
		for (auto& win : editorWindows)
		{
			win->OnImGui();
		}
		EndDockSpace();
		Engine::OnImGui();
	}

	void Editor::SetSelected(const entt::entity& node)
	{
		selectedNode = node;
	}

	void Editor::SetCopiedEntity(const entt::entity& selectedNode,bool cut) 
	{
		copiedNode = selectedNode;
	}

	void Editor::OnSceneCreated(Scene* scene)
	{
		for (auto & w : editorWindows)
		{
			w->OnSceneCreated(scene);
		}
	}

	void Editor::OnImGuizmo()
	{
		auto view = glm::inverse(editorCameraTransform.GetWorldMatrix());
		auto proj = camera->GetProjectionMatrix();

		view = glm::transpose(view);
		proj = glm::transpose(proj);

		if (selectedNode == entt::null || imGuizmoOperation == 4)
			return;

		if (showGizmos)
		{
			ImGuizmo::SetDrawlist();
			ImGuizmo::SetOrthographic(camera->IsOrthographic());

			auto& registry = Editor::GetModule<SceneManager>()->GetCurrentScene()->GetRegistry();
			auto transform = registry.try_get<Transform>(selectedNode);
			if (transform != nullptr)
			{
				auto model = glm::transpose(transform->GetWorldMatrix());
				float delta[16];

				ImGuizmo::Manipulate(
					glm::value_ptr(view),
					glm::value_ptr(proj),
					static_cast<ImGuizmo::OPERATION>(imGuizmoOperation),
					ImGuizmo::LOCAL,
					glm::value_ptr(model),
					delta,
					nullptr);

				if (ImGuizmo::IsUsing())
				{
					if (static_cast<ImGuizmo::OPERATION>(imGuizmoOperation) == ImGuizmo::OPERATION::SCALE)
					{
						auto mat = glm::transpose(glm::make_mat4(delta));
						
						transform->SetLocalScale(transform->GetLocalScale() * Transform::GetScaleFromMatrix(mat));
					}
					else
					{
						auto mat = glm::transpose(glm::make_mat4(delta)) * transform->GetLocalMatrix();
						transform->SetLocalTransform(mat);
						//TOOD
					}
				}
			}
		}
	}

	void Editor::SelectObject(const Ray& ray)
	{
		/*auto& registry = GetModule<SceneManager>()->GetCurrentScene()->GetRegistry();
		float closestEntityDist = std::numeric_limits<float>::infinity();

		entt::entity currentClosestEntity = entt::null;

		auto group = registry.group<Model>(entt::get<Transform>);

		for (auto entity : group)
		{
			const auto& [model, trans] = group.get<Model, Transform>(entity);

			auto& meshes = model.GetMeshes();

			for (auto mesh : meshes)
			{
				if (mesh->IsActive())
				{
					auto& worldTransform = trans.GetWorldMatrix();

					auto bbCopy = mesh->GetBoundingBox()->Transformed(worldTransform);
					float dist = ray.HitDistance(bbCopy);

					if (dist < std::numeric_limits<float>::infinity())
					{
						if (dist < closestEntityDist)
						{
							closestEntityDist = dist;
							currentClosestEntity = entity;
						}
					}
				}
			}
		}

		if (selectedNode != entt::null)
		{
			if (selectedNode == currentClosestEntity)
			{
				
				auto& trans = registry.get<Transform>(selectedNode);
				auto& model = registry.get<Model>(selectedNode);
				auto bb = model.GetMeshes().front()->GetBoundingBox()->Transformed(trans.GetWorldMatrix());

				FocusCamera(trans.GetWorldPosition(), (bb.max_ - bb.min_).Length());
			}

			selectedNode = currentClosestEntity;
			return;
		}*/
	}

	Ray Editor::SendScreenRay(int32_t x, int32_t y, Camera* camera, int32_t width, int32_t height)
	{
		if (!camera)
			return Ray();

		float screenX = (float)x / (float)width;
		float screenY = (float)y / (float)height;

		bool flipY = true;

		return camera->GetScreenRay(screenX, screenY, glm::inverse(editorCameraTransform.GetWorldMatrix()), flipY);
	}

	void Editor::DrawMenu()
	{
		if (ImGui::BeginMainMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("Exit"))
				{
				}

				if (ImGui::MenuItem("Open File"))
				{
				}
				ImGui::EndMenu();
			}

			ImGui::SameLine((ImGui::GetWindowContentRegionMax().x / 2.0f) - (1.5f * (ImGui::GetFontSize() + ImGui::GetStyle().ItemSpacing.x)));

			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.1f, 0.2f, 0.7f, 0.0f));

			if (Engine::Get().GetEditorState() == EditorState::Next)
				Engine::Get().SetEditorState(EditorState::Paused);

			bool selected;
			{
				selected = Engine::Get().GetEditorState() == EditorState::Play;
				if (selected)
					ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.28f, 0.56f, 0.9f, 1.0f));

				if (ImGui::Button(ICON_MDI_PLAY))
				{
					Engine::Get().SetEditorState(selected ? EditorState::Preview : EditorState::Play);

					selectedNode = entt::null;
					if (selected)
						LoadCachedScene();
					else
					{
						CacheScene();
						Engine::Get().GetModule<SceneManager>()->GetCurrentScene()->OnInit();
					}
				}


				ImGuiHelper::Tooltip("Play");


				if (selected)
					ImGui::PopStyleColor();
			}

			ImGui::SameLine();

			{
				selected = Engine::Get().GetEditorState() == EditorState::Paused;
				if (selected)
					ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.28f, 0.56f, 0.9f, 1.0f));

				if (ImGui::Button(ICON_MDI_PAUSE))
					Engine::Get().SetEditorState(selected ? EditorState::Play : EditorState::Paused);

				ImGuiHelper::Tooltip("Pause");


				if (selected)
					ImGui::PopStyleColor();
			}

			ImGui::SameLine();

			{
				selected = Engine::Get().GetEditorState() == EditorState::Next;
				if (selected)
					ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.28f, 0.56f, 0.9f, 1.0f));

				if (ImGui::Button(ICON_MDI_STEP_FORWARD))
					Engine::Get().SetEditorState(EditorState::Next);


				ImGuiHelper::Tooltip("Next");

				if (selected)
					ImGui::PopStyleColor();
			}

			ImGui::SameLine(ImGui::GetWindowContentRegionMax().x - 240.0f);
			ImGui::PopStyleColor();

			ImGui::EndMainMenuBar();
		}
	}

	void Editor::BeginDockSpace()
	{
		static bool p_open = true;
		static bool opt_fullscreen_persistant = true;
		static ImGuiDockNodeFlags opt_flags = ImGuiDockNodeFlags_NoWindowMenuButton | ImGuiDockNodeFlags_NoCloseButton;
		bool opt_fullscreen = opt_fullscreen_persistant;

		ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDocking;
		if (opt_fullscreen)
		{
			ImGuiViewport* viewport = ImGui::GetMainViewport();

			auto pos = viewport->Pos;
			auto size = viewport->Size;
			bool menuBar = true;
			if (menuBar)
			{
				const float infoBarSize = ImGui::GetFrameHeight();
				pos.y += infoBarSize;
				size.y -= infoBarSize;
			}

			ImGui::SetNextWindowPos(pos);
			ImGui::SetNextWindowSize(size);
			ImGui::SetNextWindowViewport(viewport->ID);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
			window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize
				| ImGuiWindowFlags_NoMove;
			window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
		}

		if (opt_flags & ImGuiDockNodeFlags_DockSpace)
			window_flags |= ImGuiWindowFlags_NoBackground;

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::Begin("MyDockspace", &p_open, window_flags);
		ImGui::PopStyleVar();

		if (opt_fullscreen)
			ImGui::PopStyleVar(2);

		ImGuiID DockspaceID = ImGui::GetID("MyDockspace");

		if (!ImGui::DockBuilderGetNode(DockspaceID))
		{
			ImGui::DockBuilderRemoveNode(DockspaceID); // Clear out existing layout
			ImGui::DockBuilderAddNode(DockspaceID); // Add empty node
			ImGui::DockBuilderSetNodeSize(DockspaceID, ImGui::GetIO().DisplaySize);

			ImGuiID dock_main_id = DockspaceID;
			ImGuiID DockBottom = ImGui::DockBuilderSplitNode(dock_main_id, ImGuiDir_Down, 0.3f, nullptr, &dock_main_id);
			ImGuiID DockLeft = ImGui::DockBuilderSplitNode(dock_main_id, ImGuiDir_Left, 0.2f, nullptr, &dock_main_id);
			ImGuiID DockRight = ImGui::DockBuilderSplitNode(dock_main_id, ImGuiDir_Right, 0.20f, nullptr, &dock_main_id);

			ImGuiID DockLeftChild = ImGui::DockBuilderSplitNode(DockLeft, ImGuiDir_Down, 0.875f, nullptr, &DockLeft);
			ImGuiID DockRightChild = ImGui::DockBuilderSplitNode(DockRight, ImGuiDir_Down, 0.875f, nullptr, &DockRight);
			ImGuiID DockingLeftDownChild = ImGui::DockBuilderSplitNode(DockLeftChild, ImGuiDir_Down, 0.06f, nullptr, &DockLeftChild);
			ImGuiID DockingRightDownChild = ImGui::DockBuilderSplitNode(DockRightChild, ImGuiDir_Down, 0.06f, nullptr, &DockRightChild);

			ImGuiID DockBottomChild = ImGui::DockBuilderSplitNode(DockBottom, ImGuiDir_Down, 0.2f, nullptr, &DockBottom);
			ImGuiID DockingBottomLeftChild = ImGui::DockBuilderSplitNode(DockBottomChild, ImGuiDir_Left, 0.5f, nullptr, &DockBottomChild);
			ImGuiID DockingBottomRightChild = ImGui::DockBuilderSplitNode(DockBottomChild, ImGuiDir_Right, 0.5f, nullptr, &DockBottomChild);

			ImGuiID DockMiddle = ImGui::DockBuilderSplitNode(dock_main_id, ImGuiDir_Right, 0.8f, nullptr, &dock_main_id);

			ImGui::DockBuilderDockWindow("Scene", DockMiddle);

			ImGui::DockBuilderDockWindow("Properties", DockRight);

			ImGui::DockBuilderDockWindow("Assets", DockingBottomRightChild);
			ImGui::DockBuilderDockWindow("GraphicsInfo", DockLeft);
			ImGui::DockBuilderDockWindow("ApplicationInfo", DockLeft);
			ImGui::DockBuilderDockWindow("Hierarchy", DockLeft);

			ImGui::DockBuilderFinish(DockspaceID);
		}

		// Dockspace
		ImGuiIO& io = ImGui::GetIO();
		if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
		{
			ImGui::DockSpace(DockspaceID, ImVec2(0.0f, 0.0f), opt_flags);
		}
	}

	void Editor::EndDockSpace()
	{
		ImGui::End();
	}

	void Editor::LoadCachedScene()
	{
		//load from disk
	}

	void Editor::CacheScene()
	{
		//Serialize the scene
	}
};

Raven::Engine* CreateEngine() {
	return new Raven::Editor();
}
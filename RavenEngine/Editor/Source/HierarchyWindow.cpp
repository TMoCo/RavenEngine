//////////////////////////////////////////////////////////////////////////////
// This file is part of the Raven Game Engine			                    //

//////////////////////////////////////////////////////////////////////////////
#include "HierarchyWindow.h"
#include "Scene/Component/Component.h"
#include "Scene/SceneManager.h"
#include "Scene/Scene.h"
#include "IconsMaterialDesignIcons.h"
#include "Editor.h"

namespace Raven
{


	HierarchyWindow::HierarchyWindow()
	{
		title = "Hierarchy";
	}

	void HierarchyWindow::OnImGui() 
	{
		const auto flags = ImGuiWindowFlags_NoCollapse;

		ImGui::Begin(title.c_str(), &active, flags);
		{
			auto scene = Editor::Get().GetModule<SceneManager>()->GetCurrentScene();
			auto& registry = scene->GetRegistry();
	
			if (ImGui::BeginPopupContextWindow())
			{
				if (ImGui::Selectable("Add Empty Entity"))
				{
					scene->CreateEntity();
				}
				ImGui::EndPopup();
			}
			DrawName();
			DrawFilter();
			DragEntity();
		}
		ImGui::End();
	}

	void HierarchyWindow::DrawName()
	{
		constexpr size_t INPUT_BUFFER = 256;

		auto scene = Editor::Get().GetModule<SceneManager>()->GetCurrentScene();
		auto& registry = scene->GetRegistry();

		const auto & sceneName = scene->GetName();

		static char objName[INPUT_BUFFER];
		strcpy(objName, sceneName.c_str());

		ImGui::PushItemWidth(-1);
		if (ImGui::InputText("##Name", objName, IM_ARRAYSIZE(objName), 0))
			scene->SetName(objName);
		ImGui::Separator();
	}

	void HierarchyWindow::DrawFilter()
	{

		ImGui::PushStyleColor(ImGuiCol_MenuBarBg, ImGui::GetStyleColorVec4(ImGuiCol_TabActive));
		ImGui::TextUnformatted(ICON_MDI_MAGNIFY);
		ImGui::SameLine();
		hierarchyFilter.Draw("##HierarchyFilter", ImGui::GetContentRegionAvail().x - ImGui::GetStyle().IndentSpacing);
		ImGui::PopStyleColor();
		ImGui::Unindent();
	}

	void HierarchyWindow::DragEntity()
	{
		auto scene = Editor::Get().GetModule<SceneManager>()->GetCurrentScene();
		auto& registry = scene->GetRegistry();

		if (ImGui::BeginDragDropTarget())
		{
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("Drag_Entity"))
			{
				RAVEN_ASSERT(payload->DataSize == sizeof(entt::entity*), "Error ImGUI drag entity");
				auto entity = *reinterpret_cast<entt::entity*>(payload->Data);
				auto hierarchyComponent = registry.try_get<Hierarchy>(entity);
				if (hierarchyComponent)
				{
					Hierarchy::Reparent(entity, entt::null, registry, *hierarchyComponent);
				}
			}
			ImGui::EndDragDropTarget();
		}

		ImGui::Indent();

		registry.each([&](auto entity) {
			if (registry.valid(entity))
			{
				auto hierarchyComponent = registry.try_get<Hierarchy>(entity);

				if (!hierarchyComponent || hierarchyComponent->Parent() == entt::null)
					DrawNode(entity, registry);
			}
		});
	}

	void HierarchyWindow::DrawNode(const entt::entity& node, entt::registry& registry)
	{
		bool show = true;

		if (!registry.valid(node))
			return;

		const auto nameComponent = registry.try_get<NameComponent>(node);
		std::string name = nameComponent ? nameComponent->name : std::to_string(entt::to_integral(node));

		if (hierarchyFilter.IsActive())
		{
			if (!hierarchyFilter.PassFilter(name.c_str()))
			{
				show = false;
			}
		}

		if (show)
		{
			auto hierarchyComponent = registry.try_get<Hierarchy>(node);
			bool noChildren = true;

			if (hierarchyComponent != nullptr && hierarchyComponent->First() != entt::null)
				noChildren = false;

			ImGuiTreeNodeFlags nodeFlags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_FramePadding;

			if (noChildren)
			{
				nodeFlags |= ImGuiTreeNodeFlags_Leaf;
			}

			auto activeComponent = registry.try_get<ActiveComponent>(node);
			bool active = activeComponent ? activeComponent->active : true;

			if (!active)
				ImGui::PushStyleColor(ImGuiCol_Text, ImGui::GetStyleColorVec4(ImGuiCol_TextDisabled));

			bool nodeOpen = ImGui::TreeNodeEx((void*)(intptr_t)entt::to_integral(node), nodeFlags, name.c_str());
			if (nodeOpen) 
			{
				ImGui::TreePop();
			}
		}
	}

};


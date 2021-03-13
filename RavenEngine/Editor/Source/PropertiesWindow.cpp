//////////////////////////////////////////////////////////////////////////////
// This file is part of the Raven Game Engine			                    //

//////////////////////////////////////////////////////////////////////////////

#include "PropertiesWindow.h"
#include "Scene/SceneManager.h"
#include "Scene/Scene.h"
#include "Scene/Component/Component.h"
#include "Scene/Component/Light.h"
#include "Scene/Component/Transform.h"
#include "Core/Camera.h"
#include "ImGui/ImGuiHelpers.h"
#include <glm/gtc/type_ptr.hpp>
#include "Editor.h"


namespace MM 
{
	using namespace Raven;
	template<>
	void ComponentEditorWidget<Transform>(entt::registry& reg, entt::registry::entity_type e)
	{
		auto& transform = reg.get<Transform>(e);

		auto rotation = glm::eulerAngles(transform.GetLocalOrientation());
		auto position = transform.GetLocalPosition();
		auto scale = transform.GetLocalScale();

		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(2, 2));
		ImGui::Columns(2);
		ImGui::Separator();

		ImGui::TextUnformatted("Position");
		ImGui::NextColumn();
		ImGui::PushItemWidth(-1);

		if (ImGui::DragFloat3("##Position", glm::value_ptr(position), 3, 0.05f))
		{
			transform.SetLocalPosition(position);
		}

		ImGui::PopItemWidth();
		ImGui::NextColumn();

		ImGui::TextUnformatted("Rotation");
		ImGui::NextColumn();
		ImGui::PushItemWidth(-1);
		if (ImGui::DragFloat3("##Rotation", glm::value_ptr(rotation), 3, 0.05f))
		{
			float pitch = std::min(rotation.x, 89.9f);
			pitch = std::max(pitch, -89.9f);
			transform.SetLocalOrientation(glm::radians(glm::vec3{ pitch, rotation.y, rotation.z }));
		}

		ImGui::PopItemWidth();
		ImGui::NextColumn();

		ImGui::TextUnformatted("Scale");
		ImGui::NextColumn();
		ImGui::PushItemWidth(-1);
		if (ImGui::DragFloat3("##Scale", glm::value_ptr(scale), 3, 0.05f))
		{
			transform.SetLocalScale(scale);
		}

		ImGui::PopItemWidth();
		ImGui::NextColumn();

		ImGui::Columns(1);
		ImGui::Separator();
		ImGui::PopStyleVar();
	}

	template<>
	void ComponentEditorWidget<Light>(entt::registry& reg, entt::registry::entity_type e)
	{
		auto& light = reg.get<Light>(e);
		light.OnImGui();
	}


	template<>
	void ComponentEditorWidget<Camera>(entt::registry& reg, entt::registry::entity_type e)
	{
		auto& camera = reg.get<Camera>(e);

		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(2, 2));
		ImGui::Columns(2);
		ImGui::Separator();


		float aspect = camera.GetAspectRatio();
		
		if (ImGuiHelper::Property("Aspect", aspect, 0.0f, 10.0f))
			camera.SetAspectRatio(aspect);

		float fov = camera.GetFov();
		if (ImGuiHelper::Property("Fov", fov, 1.0f, 120.0f))
			camera.SetFov(fov);

		float near = camera.GetNear();
		if (ImGuiHelper::Property("Near", near, 0.0f, 10.0f))
			camera.SetNear(near);

		float far = camera.GetFar();
		if (ImGuiHelper::Property("Far", far, 10.0f, 10000.0f))
			camera.SetFar(far);

		float scale = camera.GetScale();
		if (ImGuiHelper::Property("Scale", scale, 0.0f, 1000.0f))
			camera.SetScale(scale);

		bool ortho = camera.IsOrthographic();
		if (ImGuiHelper::Property("Orthograhic", ortho))
			camera.SetOrthographic(ortho);


		ImGui::Columns(1);
		ImGui::Separator();
		ImGui::PopStyleVar();

	}
};

namespace Raven
{

	
	constexpr size_t INPUT_BUFFER = 256;
	PropertiesWindow::PropertiesWindow()
	{
		title = "Properties";
	}

	void PropertiesWindow::OnImGui()
	{
		auto& editor = static_cast<Editor&>(Engine::Get());

		auto& registry = editor.GetModule<SceneManager>()->GetCurrentScene()->GetRegistry();
		auto selected = editor.GetSelected();

		if (ImGui::Begin(title.c_str(), &active))
		{
			if (selected == entt::null)
			{
				ImGui::End();
				return;
			}

			auto activeComponent = registry.try_get<ActiveComponent>(selected);
			bool active = activeComponent ? activeComponent->active : true;
			if (ImGui::Checkbox("##ActiveCheckbox", &active))
			{
				if (!activeComponent)
					registry.emplace<ActiveComponent>(selected, active);
				else
					activeComponent->active = active;
			}
			ImGui::SameLine();
			ImGui::TextUnformatted(ICON_MDI_CUBE);
			ImGui::SameLine();

			bool hasName = registry.has<NameComponent>(selected);
			std::string name;
			if (hasName)
				name = registry.get<NameComponent>(selected).name;
			else
				name = std::to_string(entt::to_integral(selected));

			static char objName[INPUT_BUFFER];
			strcpy(objName, name.c_str());
		
			if (ImGui::InputText("##Name", objName, IM_ARRAYSIZE(objName)))
				registry.get_or_emplace<NameComponent>(selected).name = objName;

			ImGui::Separator();

			enttEditor.RenderImGui(registry, selected);
		}
		ImGui::End();
	}

	void PropertiesWindow::OnSceneCreated(Scene* scene)
	{
		if (init)
			return;
		auto& editor = static_cast<Editor&>(Engine::Get());
		auto& iconMap = editor.GetComponentIconMap();

#define TRIVIAL_COMPONENT(ComponentType) \
	{ \
		std::string name; \
		if(iconMap.find(typeid(ComponentType).hash_code()) != iconMap.end()) \
			name += iconMap[typeid(ComponentType).hash_code()]; \
        else \
            name += iconMap[typeid(Editor).hash_code()]; \
		name += "\t"; \
		name += ###ComponentType; \
		enttEditor.registerComponent<ComponentType>(name.c_str()); \
	}

		TRIVIAL_COMPONENT(Transform);
		TRIVIAL_COMPONENT(Light);
		TRIVIAL_COMPONENT(Camera);
		init = true;
	}

};


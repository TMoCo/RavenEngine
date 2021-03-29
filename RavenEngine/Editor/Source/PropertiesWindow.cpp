//////////////////////////////////////////////////////////////////////////////
// This file is part of the Raven Game Engine			                    //

//////////////////////////////////////////////////////////////////////////////

#include "PropertiesWindow.h"
#include "Scene/SceneManager.h"
#include "Scene/Scene.h"
#include "Scene/Component/Component.h"
#include "Scene/Component/Light.h"
#include "Scene/Component/Transform.h"
#include "Scene/Component/Model.h"
#include "Core/Camera.h"
#include "ImGui/ImGuiHelpers.h"
#include "ResourceManager/MeshFactory.h"
#include <glm/gtc/type_ptr.hpp>
#include "Editor.h"

#include "ResourceManager/ResourceManager.h"
#include "ResourceManager/Resources/Mesh.h"
#include "Engine.h"


namespace MM 
{

	
	using namespace Raven;
	template<>
	void ComponentEditorWidget<Transform>(entt::registry& reg, entt::registry::entity_type e)
	{
		auto& transform = reg.get<Transform>(e);

		auto rotation = glm::degrees(transform.GetLocalOrientation());
		auto position = transform.GetLocalPosition();
		auto scale = transform.GetLocalScale();

		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(2, 2));
		ImGui::Columns(2);
		ImGui::Separator();

		ImGui::TextUnformatted("Position");
		ImGui::NextColumn();
		ImGui::PushItemWidth(-1);

		if (ImGui::DragFloat3("##Position", glm::value_ptr(position), 0.05))
		{
			transform.SetLocalPosition(position);
		}

		ImGui::PopItemWidth();
		ImGui::NextColumn();

		ImGui::TextUnformatted("Rotation");
		ImGui::NextColumn();
		ImGui::PushItemWidth(-1);
		

		if (ImGui::DragFloat3("##Rotation", glm::value_ptr(rotation),0.1))
		{
			transform.SetLocalOrientation(glm::radians(rotation));
		}

		ImGui::PopItemWidth();
		ImGui::NextColumn();

		ImGui::TextUnformatted("Scale");
		ImGui::NextColumn();
		ImGui::PushItemWidth(-1);
		if (ImGui::DragFloat3("##Scale", glm::value_ptr(scale), 0.05))
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
		if (ImGuiHelper::Property("Near", near, 0.01, 10.f))
			camera.SetNear(near);

		float far = camera.GetFar();
		if (ImGuiHelper::Property("Far", far, 10.0f, 10000.0f))
			camera.SetFar(far);

		float scale = camera.GetScale();
		if (ImGuiHelper::Property("Scale", scale, 0.0f, 1000.0f))
			camera.SetScale(scale);

		bool ortho = camera.IsOrthographic();
		if (ImGuiHelper::Property("Orthographic", ortho))
			camera.SetOrthographic(ortho);


		ImGui::Columns(1);
		ImGui::Separator();
		ImGui::PopStyleVar();

	}

	std::string GetPrimitiveName(PrimitiveType type)
	{
		switch (type)
		{
		case PrimitiveType::Cube: return "Cube";
		case PrimitiveType::Plane: return "Plane";
		case PrimitiveType::Quad: return "Quad";
		case PrimitiveType::Sphere: return "Sphere";
		case PrimitiveType::Pyramid: return "Pyramid";
		case PrimitiveType::Capsule: return "Capsule";
		case PrimitiveType::Cylinder: return "Cylinder";
		case PrimitiveType::Terrain: return "Terrain";
		case PrimitiveType::File: return "File";
		}
		return "";
	};

	PrimitiveType GetPrimitiveName(const std::string& type)
	{
		if (type == "Cube")
		{
			return PrimitiveType::Cube;
		}
		else if (type == "Quad")
		{
			return PrimitiveType::Quad;
		}
		else if (type == "Sphere")
		{
			return PrimitiveType::Sphere;
		}
		else if (type == "Pyramid")
		{
			return PrimitiveType::Pyramid;
		}
		else if (type == "Capsule")
		{
			return PrimitiveType::Capsule;
		}
		else if (type == "Cylinder")
		{
			return PrimitiveType::Cylinder;
		}
		else if (type == "Terrain")
		{
			return PrimitiveType::Terrain;
		}

		return PrimitiveType::Cube;
	};

	template<>
	void ComponentEditorWidget<Model>(entt::registry& reg, entt::registry::entity_type e)
	{
		auto& model = reg.get<Model>(e);
		auto& meshes = model.GetMeshes();
		
		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(2, 2));
		ImGui::Columns(2);
		ImGui::Separator();

	
		ImGui::TextUnformatted("Primitive Type");

		ImGui::NextColumn();
		ImGui::PushItemWidth(-1);

		const char* shapes[] = { "Sphere", "Cube", "Pyramid", "Capsule", "Cylinder", "Terrain", "File", "Quad" };
		std::string shapeCurrent = GetPrimitiveName(model.GetPrimitiveType());
		if (ImGui::BeginCombo("", shapeCurrent.c_str(), 0))
		{
			for (auto n = 0; n < 8; n++)
			{
				bool isSelected = (shapeCurrent.c_str() == shapes[n]);
				if (ImGui::Selectable(shapes[n], shapeCurrent.c_str()))
				{
					meshes.clear();
					if (strcmp(shapes[n], "File") != 0)
					{
						//add new mesh here..
						meshes.emplace_back(MeshFactory::CreatePrimitive(GetPrimitiveName(shapes[n])));
						model.SetPrimitiveType(GetPrimitiveName(shapes[n]));
					}
					else
						model.SetPrimitiveType(PrimitiveType::File);
				}
				if (isSelected)
					ImGui::SetItemDefaultFocus();
			}
			ImGui::EndCombo();
		}

		ImGui::PopItemWidth();
		ImGui::NextColumn();

		if (model.GetPrimitiveType() == PrimitiveType::File)
		{
			ImGui::TextUnformatted("FilePath");

			ImGui::NextColumn();
			ImGui::PushItemWidth(-1);

			ImGui::TextUnformatted(model.GetFileName().c_str());

		/*	
			static char buffer[128] = {};
			if (ImGui::InputText("filePath", buffer,128))
			{
				//model.AddMesh(Engine::Get().GetModule<ResourceManager>()->GetResource<Mesh>(buffer));
			}*/

			//model.AddMesh(Engine::Get().GetModule<ResourceManager>()->GetResource<Mesh>(buffer));

			ImGui::PopItemWidth();
			ImGui::NextColumn();

			for (auto & mesh : model.GetMeshes())
			{
				ImGui::TextUnformatted(std::to_string((int64_t)mesh.get()).c_str());
			}

		}

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
		TRIVIAL_COMPONENT(Model);
		init = true;
	}

};


//////////////////////////////////////////////////////////////////////////////
// This file is part of the Raven Game Engine			                    //

//////////////////////////////////////////////////////////////////////////////

#include "Engine.h"

#include "Editor.h"
#include "PropertiesWindow.h"
#include "HierarchyWindow.h"

#include "Scene/SceneManager.h"
#include "Scene/Scene.h"
#include "Scene/Entity/Entity.h"
#include "Scene/Component/Component.h"
#include "Scene/Component/Light.h"
#include "Scene/Component/Transform.h"
#include "Scene/Component/MeshComponent.h"
#include "Scene/Component/SkinnedMeshComponent.h"
#include "Scene/Component/CameraControllerComponent.h"
#include "Scene/Component/MeshRenderer.h"
#include "Scene/Component/RigidBody.h"

#include "Scripts/LuaComponent.h"

#include "ImGui/ImGuiHelpers.h"

#include "Core/Camera.h"
#include "Utilities/StringUtils.h"

#include "ResourceManager/ResourceManager.h"
#include "ResourceManager/MeshFactory.h"
#include "ResourceManager/Resources/Mesh.h"

#include "Animation/Animator.h"
#include "Animation/AnimationController.h"


#include <glm/gtx/string_cast.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <filesystem>
#include <functional>

namespace MM 
{
	using namespace Raven;
	template<>
	void ComponentEditorWidget<Transform>(entt::registry& reg, entt::registry::entity_type e)
	{
		auto& transform = reg.get<Transform>(e);

		auto rotation = glm::degrees(transform.GetRotationEuler());
		auto position = transform.GetPosition();
		auto scale = transform.GetScale();

		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(2, 2));
		ImGui::Columns(2);
		ImGui::Separator();

		ImGui::TextUnformatted("Position");
		ImGui::NextColumn();
		ImGui::PushItemWidth(-1);

		if (ImGui::DragFloat3("##Position", glm::value_ptr(position), 0.05))
		{
			transform.SetPosition(position);
		}

		ImGui::PopItemWidth();
		ImGui::NextColumn();

		ImGui::TextUnformatted("Rotation");
		ImGui::NextColumn();
		ImGui::PushItemWidth(-1);
		

		if (ImGui::DragFloat3("##Rotation", glm::value_ptr(rotation),0.1))
		{
			rotation = glm::radians(rotation);
			transform.SetRotationEuler(rotation.x, rotation.y, rotation.z);
		}

		ImGui::PopItemWidth();
		ImGui::NextColumn();

		ImGui::TextUnformatted("Scale");
		ImGui::NextColumn();
		ImGui::PushItemWidth(-1);
		if (ImGui::DragFloat3("##Scale", glm::value_ptr(scale), 0.05))
		{
			transform.SetScale(scale);
		}

		ImGui::PopItemWidth();
		ImGui::NextColumn();

		ImGui::Columns(1);
		ImGui::Separator();
		ImGui::PopStyleVar();

		// also update the rigid body if available
		auto rb = reg.try_get<RigidBody>(e);
		if (rb)
		{
			rb->SetInitTransform(transform);
		}

	}

	template<>
	void ComponentEditorWidget<Light>(entt::registry& reg, entt::registry::entity_type e)
	{
		auto& light = reg.get<Light>(e);
	

		light.OnImGui();
	}

	template<>
	void ComponentEditorWidget<LuaComponent>(entt::registry& reg, entt::registry::entity_type e)
	{
		auto& lua = reg.get<LuaComponent>(e);
		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(2, 2));
		ImGui::Columns(2);
		ImGui::PushItemWidth(-1);
		
		if (ImGui::BeginCombo("", lua.GetFileName().c_str(), 0))
		{
			for (const auto& entry : std::filesystem::directory_iterator("./scripts/"))
			{
				if (!StringUtils::IsLuaFile(entry.path().string()))
				{
					continue;
				}

				bool isDir = std::filesystem::is_directory(entry);
				auto isSelected = StringUtils::GetFileName(lua.GetFileName()) == StringUtils::GetFileName(entry.path().string());

				if (!isDir && !isSelected)
				{
					if (ImGui::Selectable(entry.path().string().c_str()))
					{
						lua.SetFilePath(entry.path().string());
					}
				}
				if (isSelected)
					ImGui::SetItemDefaultFocus();

			}
			ImGui::EndCombo();
		}
		ImGui::NextColumn();
		if (ImGui::Button(lua.IsLoaded() ? "Reload" : "Load"))
		{
			lua.Reload();
		}
		ImGui::PopItemWidth();
		ImGui::Columns(1);
		ImGui::PopStyleVar();
		lua.OnImGui();
		
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

	template<>
	void ComponentEditorWidget<CameraControllerComponent>(entt::registry& reg, entt::registry::entity_type e)
	{
		auto& controllerComp = reg.get<CameraControllerComponent>(e);
		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(2, 2));
		ImGui::Columns(2);
		ImGui::Separator();

		ImGui::AlignTextToFramePadding();
		ImGui::TextUnformatted("Controller Type");
		ImGui::NextColumn();
		ImGui::PushItemWidth(-1);

		const std::array<std::string,2> controllerTypes = { "FPS" ,"Editor"};
		std::string currentController = CameraControllerComponent::TypeToString(controllerComp.GetType());
		if (ImGui::BeginCombo("", currentController.c_str(), 0)) // The second parameter is the label previewed before opening the combo.
		{
			for (auto n = 0; n < controllerTypes.size(); n++)
			{
				bool isSelected = currentController == controllerTypes[n];
				if (ImGui::Selectable(controllerTypes[n].c_str(), currentController.c_str()))
				{
					controllerComp.SetControllerType(CameraControllerComponent::StringToType(controllerTypes[n]));
				}
				if (isSelected)
					ImGui::SetItemDefaultFocus();
			}
			ImGui::EndCombo();
		}

		if (controllerComp.GetController())
			controllerComp.GetController()->OnImGui();

		ImGui::Columns(1);
		ImGui::Separator();
		ImGui::PopStyleVar();
	}

	template<>
	void ComponentEditorWidget<MeshComponent>(entt::registry& reg, entt::registry::entity_type e)
	{
		auto& model = reg.get<MeshComponent>(e);
		
		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(2, 2));
		ImGui::Columns(2);
		ImGui::Separator();

	

		ImGui::NextColumn();
		ImGui::PushItemWidth(-1);

#if 0
		auto& meshes = model.GetMeshes();
		const char* shapes[] = { "Sphere", "Cube", "Pyramid", "Capsule", "Cylinder", "Terrain", "File", "Quad" };

		std::string shapeCurrent = PrimitiveType::GetPrimativeName(model.GetPrimitiveType());
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
						meshes.emplace_back(MeshFactory::CreatePrimitive(PrimitiveType::GetPrimativeName(shapes[n])));
						model.SetPrimitiveType(PrimitiveType::GetPrimativeName(shapes[n]));
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
			ImGui::Columns(1);
			int32_t i = 0;
			for (auto & mesh : model.GetMeshes())
			{
				ImGui::Separator();
				
				ImGui::TextUnformatted((mesh->name + " : " + std::to_string(i++)).c_str());
			}

		}
#endif


		ImGui::Separator();
		ImGui::PopStyleVar();

	}

	template<>
	void ComponentEditorWidget<SkinnedMeshRenderer>(entt::registry& reg, entt::registry::entity_type e) 
	{
		auto& model = reg.get<SkinnedMeshRenderer>(e);
		model.OnImGui();
	}

	template<>
	void ComponentEditorWidget<Animator>(entt::registry& reg, entt::registry::entity_type e)
	{
		auto& model = reg.get<Animator>(e);

		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(2, 2));
		ImGui::Columns(2);
		ImGui::Separator();

		model.OnImGui();

		ImGui::Columns(1);
		ImGui::Separator();
		ImGui::PopStyleVar();
	}

	template<>
	void ComponentEditorWidget<RigidBody>(entt::registry& reg, entt::registry::entity_type e)
	{
		//
		// Edit rigid body properties
		//
		auto& rigidBody = reg.get<RigidBody>(e);

		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(2, 2));
		ImGui::Columns(2);
		ImGui::Separator();

		bool canSleep = rigidBody.CanSleep();

		ImGui::TextUnformatted("Can sleep");
		ImGui::NextColumn();
		ImGui::PushItemWidth(-1);
		if (ImGui::Checkbox("##Can sleep", &canSleep))
		{
			rigidBody.SetIsAllowedToSleep(canSleep);
		}

		ImGui::PopItemWidth();
		ImGui::NextColumn();

		bool gravityEnabled = rigidBody.GravityEnabled();

		ImGui::TextUnformatted("Enable gravity");
		ImGui::NextColumn();
		ImGui::PushItemWidth(-1);
		if (ImGui::Checkbox("##Enable gravity", &gravityEnabled))
		{
			rigidBody.EnableGravity(gravityEnabled);
		}

		ImGui::PopItemWidth();
		ImGui::NextColumn();

		bool toppleEnabled = rigidBody.ToppleEnabled();

		ImGui::TextUnformatted("Enable toppling");
		ImGui::NextColumn();
		ImGui::PushItemWidth(-1);
		if (ImGui::Checkbox("##Enable toppling", &toppleEnabled))
		{
			rigidBody.EnableTopple(toppleEnabled);
		}

		ImGui::PopItemWidth();
		ImGui::NextColumn();

		float mass = rigidBody.GetMass();

		ImGui::TextUnformatted("Set mass");
		ImGui::NextColumn();
		ImGui::PushItemWidth(-1);
		if (ImGui::DragFloat("##Set mass", &mass, 0.05f, 0.0f, 100000.0f))
		{
			rigidBody.SetMass(mass);
		}
		
		ImGui::PopItemWidth();
		ImGui::NextColumn();

		float lDamping = rigidBody.GetLinearDamping();

		ImGui::TextUnformatted("Linear damping");
		ImGui::NextColumn();
		ImGui::PushItemWidth(-1);
		if (ImGui::DragFloat("##Linear damping", &lDamping, 0.05f, 0.0f, 1.0f))
		{
			rigidBody.SetLinearDamping(lDamping);
		}

		ImGui::PopItemWidth();
		ImGui::NextColumn();

		float aDamping = rigidBody.GetAngularDamping();

		ImGui::TextUnformatted("Angular damping");
		ImGui::NextColumn();
		ImGui::PushItemWidth(-1);
		if (ImGui::DragFloat("##Angular damping", &aDamping, 0.05f, 0.0f, 1.0f))
		{
			rigidBody.SetAngularDamping(aDamping);
		}

		ImGui::PopItemWidth();
		ImGui::NextColumn();

		static const char* types[3] = { "Static", "Kinematic", "Dynamic"};
		int selected = static_cast<int>(rigidBody.GetBodyType());

		ImGui::TextUnformatted("Set body type");
		ImGui::NextColumn();
		ImGui::PushItemWidth(-1);
		ImGui::ListBox("##Set body type", &selected, types, IM_ARRAYSIZE(types), 3);
		if (selected != static_cast<int>(rigidBody.GetBodyType()))
		{
			rigidBody.SetBodyType(static_cast<RigidBodyType>(selected));
		}

		ImGui::PopItemWidth();
		ImGui::NextColumn();

				
		//
		// Edit rigid body's colliders
		//


		if (rigidBody.GetNumColliders() != 0)
		{
			ImGui::Separator();
			ImGui::TextUnformatted("Select collider");
			ImGui::NextColumn();

			ImGui::PushItemWidth(-1);
			ImGui::ListBoxHeader("##Select collider", rigidBody.GetNumColliders(), 1);

			static int colliderIdx  = 0;
			auto colliders = rigidBody.GetAllColliders(); // ptr to the body's colliders
			// loop over all the body's colliders
			for (int i = 0; i < rigidBody.GetNumColliders(); i++)
			{
				const bool isSelected = colliderIdx == i;
				if (ImGui::Selectable(ColliderPrimitive::GetColliderPrimitiveName((*colliders)[i]->GetColliderType()).c_str(), isSelected))
				{
					colliderIdx = i;
				}
				if (isSelected)
				{
					ImGui::SetItemDefaultFocus();
				}

			}
			ImGui::ListBoxFooter();

			ImGui::PopItemWidth();
			ImGui::NextColumn();

			auto& collider = (*colliders)[colliderIdx];

			// ui for editing the type of collider
			collider->OnImGui();

			// ui for editing the collider's relative transform to the body it belongs to
			auto& transform = collider->GetRelativeTransform();

			auto rotation = glm::degrees(transform.GetRotationEuler());
			auto position = transform.GetPosition();
			// no scaling

			ImGui::TextUnformatted("Position");
			ImGui::NextColumn();
			ImGui::PushItemWidth(-1);

			if (ImGui::DragFloat3("##Position", glm::value_ptr(position), 0.05))
			{
				// update the tansform
				transform.SetPosition(position);
				// in the physics engine as well
				collider->SetTransform(transform);

			}

			ImGui::PopItemWidth();
			ImGui::NextColumn();

			ImGui::TextUnformatted("Rotation");
			ImGui::NextColumn();
			ImGui::PushItemWidth(-1);


			if (ImGui::DragFloat3("##Rotation", glm::value_ptr(rotation), 0.1))
			{
				transform.SetRotationEuler(glm::radians(rotation));
				// in the physics engine as well
				collider->SetTransform(transform);
			}

			ImGui::PopItemWidth();
			ImGui::NextColumn();

			ImGui::Columns(1);
			// button for removing the collider 
			if (ImGui::Button("Remove collider"))
			{
				rigidBody.RemoveCollider(colliderIdx);
			}
		}

		// 
		// Add colliders to the rigid body
		// 

		ImGui::Columns(1);
		ImGui::Separator();

		if (ImGui::BeginMenu("Add new collider"))
		{
			for (auto& name : ColliderPrimitive::NAMES)
			{
				if (ImGui::MenuItem(name.c_str()))
				{
					// add a collider to the rigid body
					rigidBody.AddCollider(ColliderFactory::CreateCollider(ColliderPrimitive::GetColliderPrimitiveType(name)));
				}
			}
			ImGui::EndMenu();
		}
		
		ImGui::Columns(2);
		ImGui::Separator();

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
			if (controller != nullptr)
			{
				controller->OnImGui();
				ImGui::End();
				return;
			}

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

			//enttEditor.RenderImGui(registry, selected);
			enttEditor.renderEditor(registry, selected);

			if (ImGui::BeginDragDropTarget())
			{
				auto data = ImGui::AcceptDragDropPayload("AssetFile", ImGuiDragDropFlags_None);
				if (data)
				{
					std::string file = (char*)data->Data;
					if (StringUtils::IsLuaFile(file))
					{
						auto scene = editor.GetModule<SceneManager>()->GetCurrentScene();
						auto name = StringUtils::GetFileName(file);
						registry.emplace<LuaComponent>(selected, file, scene).SetEntity_Evil(selected, scene);
					}
				}
				ImGui::EndDragDropTarget();
			}
		}
		ImGui::End();
	}

	void PropertiesWindow::OnSceneCreated(Scene* scene)
	{
		auto& editor = static_cast<Editor&>(Engine::Get());
		auto& iconMap = editor.GetComponentIconMap();

		if (!init)
		{
#define TRIVIAL_COMPONENT(ComponentType,show) \
	{ \
		std::string name; \
		if(iconMap.find(typeid(ComponentType).hash_code()) != iconMap.end()) \
			name += iconMap[typeid(ComponentType).hash_code()]; \
        else \
            name += iconMap[typeid(Editor).hash_code()]; \
		name += "\t"; \
		name += ###ComponentType; \
		enttEditor.registerComponent<ComponentType>(name,show); \
	}

		TRIVIAL_COMPONENT(Transform,true);
		TRIVIAL_COMPONENT(Light, true);
		TRIVIAL_COMPONENT(Camera, true);
		TRIVIAL_COMPONENT(CameraControllerComponent, true);
		TRIVIAL_COMPONENT(MeshComponent, false);
		TRIVIAL_COMPONENT(SkinnedMeshComponent, false);
		TRIVIAL_COMPONENT(LuaComponent, true);
		TRIVIAL_COMPONENT(Animator, true);
		TRIVIAL_COMPONENT(RigidBody, true);

		init = true;
		}

		enttEditor.addCreateCallback([&](entt::registry & r, entt::entity entity) {
			auto lua = r.try_get<LuaComponent>(entity);
			if (lua) 
			{
				lua->SetScene(editor.GetModule<SceneManager>()->GetCurrentScene());
			}
			auto rb = r.try_get<RigidBody>(entity);
			if (rb)
			{
				// when creating a rigid body, we need to set some data... like the object transform (if any)
				auto t = r.try_get<Transform>(entity);
				// if transform does exists, change the rigidbody's initial transform
				if (t)
				{
					LOGV("Has tranform");
					Transform initT = *t;
					initT.SetScale(glm::vec3(1.0f, 1.0f, 1.0f));
					rb->SetInitTransform(initT);
					rb->InitRigidBody();
				}
			}
		});
	}

};


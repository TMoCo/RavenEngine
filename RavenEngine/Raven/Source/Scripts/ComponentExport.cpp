#include "ComponentExport.h"

#include "LuaVirtualMachine.h"
#include <LuaBridge/LuaBridge.h>
#include <string>
#include <functional>

#include "Scene/Component/Transform.h"
#include "Scene/Component/Component.h"
#include "Scene/Component/Light.h"
#include "Animation/Animator.h"

#include "LuaComponent.h"

#include "Scene/Entity/Entity.h"
#include "Scene/Entity/EntityManager.h"


namespace Raven
{
#define EXPORT_COMPONENTS(Comp) \
		addFunction("Add" #Comp,&Entity::AddComponent<Comp>) \
		.addFunction("Remove" #Comp, &Entity::RemoveComponent<Comp>) \
		.addFunction("Get" #Comp, &Entity::GetComponent<Comp>) \
		.addFunction("GetOrAdd" #Comp, &Entity::GetOrAddComponent<Comp>) \
		.addFunction("TryGet" #Comp, &Entity::TryGetComponent<Comp>) \
        .addFunction("AddOrReplace" #Comp, &Entity::AddOrReplaceComponent<Comp>) \
        .addFunction("Has" #Comp, &Entity::HasComponent<Comp>) \

	namespace ComponentExport
	{
		void Export(lua_State* L)
		{
			luabridge::getGlobalNamespace(L)
				.beginNamespace("entt")
				.beginClass<entt::registry>("registry")
				.endClass()
				.beginClass<entt::entity>("entity")
				.endClass()
				.endNamespace()



				.beginClass <Entity>("Entity")
				.addConstructor <void (*) (entt::entity, Scene*)>()
				.addConstructor <void (*) ()>()
				.addFunction("Valid", &Entity::Valid)
				.addFunction("Destroy", &Entity::Destroy)
				.addFunction("SetParent", &Entity::SetParent)
				.addFunction("GetParent", &Entity::GetParent)
				.addFunction("IsParent", &Entity::IsParent)
				.addFunction("GetChildren", &Entity::GetChildren)
				.addFunction("SetActive", &Entity::SetActive)
				.addFunction("IsActive", &Entity::IsActive)
				.addFunction("GetChildInChildren", &Entity::GetChildInChildren)

				.EXPORT_COMPONENTS(NameComponent)
				.EXPORT_COMPONENTS(ActiveComponent)
				.EXPORT_COMPONENTS(Transform)
				.EXPORT_COMPONENTS(Light)
				.EXPORT_COMPONENTS(Animator)
				.EXPORT_COMPONENTS(LuaComponent)

				.endClass()

				.beginClass<EntityManager>("EntityManager")
				.addFunction("Create", static_cast<Entity(EntityManager::*)()> (&EntityManager::Create))
				.addFunction("GetRegistry", &EntityManager::GetRegistry)
				.endClass()

				.beginClass<NameComponent>("NameComponent")
				.addProperty("name", &NameComponent::name)
				.addFunction("GetEntity", &NameComponent::GetEntity)
				.endClass()

				.beginClass<Light>("Light")
				.addProperty("color", &Light::color)
				.addProperty("position", &Light::position)
				.addProperty("direction", &Light::direction)
				.addProperty("intensity", &Light::intensity)
				.addProperty("angle", &Light::innerAngle)
				.addProperty("angle", &Light::outerAngle)
				.addProperty("angle", &Light::clipDistance)
				.addFunction("GetEntity", &Light::GetEntity)
				.endClass()

				.beginClass<ActiveComponent>("ActiveComponent")
				.addProperty("active", &ActiveComponent::active)
				.addFunction("GetEntity", &ActiveComponent::GetEntity)
				.endClass()

				.beginClass<LuaComponent>("LuaComponent")
				.endClass()

				.beginClass<Animator>("Animator")
				.addFunction("SetValue", &Animator::SetValue<float>)
				.addFunction("SetValue", &Animator::SetValue<int32_t>)
				.addFunction("SetValue", &Animator::SetValue<std::string>)
				.addFunction("SetValue", &Animator::SetValue<bool>)
				.addFunction("GetEntity", &Animator::GetEntity)
				.endClass();
				

		}
	};
};
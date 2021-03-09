//////////////////////////////////////////////////////////////////////////////
// This file is part of the Raven Game Engine			                    //

//////////////////////////////////////////////////////////////////////////////

#pragma once
#include <string>
#include <memory>
#include <entt/entt.hpp>
#include "Utilities/Core.h"

namespace Raven
{
	class EntityManager;
	class Scene 
	{
	public:
		Scene(const std::string& name);
		virtual ~Scene() = default;

		virtual void OnInit();
		virtual void OnClean();
		virtual void OnUpdate(float dt);

		inline auto& GetName() const { return name; };
		inline void SetName(const std::string& name) { this->name = name; }

		void SetSize(uint32_t w, uint32_t h);
		entt::registry& GetRegistry();

		virtual void Serialize(const std::string& filePath, bool binary = false);
		virtual void Load(const std::string& filePath, bool binary = false);

		void CreateEntity();

	private:
		std::shared_ptr<EntityManager> entityManager;
		std::string name;
		uint32_t width = 0;
		uint32_t height = 0;
		NOCOPYABLE(Scene);
	};
};
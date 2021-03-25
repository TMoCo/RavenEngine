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
	class Entity;
	class SceneGraph;
	class Camera;
	class Transform;

	class Scene 
	{
	public:
		Scene(const std::string& name);
		virtual ~Scene() = default;

		virtual void OnInit();
		virtual void OnClean();
		virtual void OnUpdate(float dt);

		inline auto& GetEntityManager() { return entityManager; }
		inline auto& GetName() const { return name; };
		inline void SetName(const std::string& name) { this->name = name; }

		void SetSize(uint32_t w, uint32_t h);
		entt::registry& GetRegistry();

		virtual void Save(const std::string& filePath, bool binary = false);
		virtual void Load(const std::string& filePath, bool binary = false);

		Entity CreateEntity();
		Entity CreateEntity(const std::string & name);

		void DuplicateEntity(const Entity& entity, const Entity& parent);
		void DuplicateEntity(const Entity& entity);


		Camera* GetTargetCamera();
		Transform* GetCameraTransform();

		inline auto SetOverrideCamera(Camera* overrideCamera) { this->overrideCamera = overrideCamera; }
		inline auto SetOverrideTransform(Transform* overrideTransform) { this->overrideTransform = overrideTransform; }

	private:
		std::shared_ptr<SceneGraph> sceneGraph;
		std::shared_ptr<EntityManager> entityManager;
		std::string name;
		uint32_t width = 0;
		uint32_t height = 0;
		NOCOPYABLE(Scene);

		void CopyComponents(const Entity& from, const Entity& to );
		bool inited = false;
		Camera* overrideCamera = nullptr;
		Transform* overrideTransform = nullptr;

	public:
	};
};
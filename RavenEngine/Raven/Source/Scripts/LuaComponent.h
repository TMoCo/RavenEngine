//////////////////////////////////////////////////////////////////////////////
// This file is part of the Raven Game Engine			                    //
//////////////////////////////////////////////////////////////////////////////

#pragma once
#include <string>
extern "C" {
# include "lua.h"
# include "lauxlib.h"
# include "lualib.h"
}
#include <LuaBridge/LuaBridge.h>
#include <entt/entt.hpp>
#include "Engine.h"
#include "Scene/SceneManager.h"

namespace Raven 
{
	class Scene;
	class LuaComponent 
	{
	public:
		LuaComponent(entt::entity entity,const std::string& file, Scene* scene);
		LuaComponent();
		~LuaComponent();
		void OnInit();
		void OnUpdate(float dt);
		void Reload();
		void LoadScript();

		template<typename Archive>
		void save(Archive& archive) const
		{
			archive(
				cereal::make_nvp("FilePath", file),
				cereal::make_nvp("Parent", entity)
			
			);
		}

		template<typename Archive>
		void load(Archive& archive)
		{
			scene = Engine::Get().GetModule<SceneManager>()->GetCurrentScene();
			archive(
				cereal::make_nvp("FilePath", file),
				cereal::make_nvp("Parent", entity)
			);
			Init();
		}

		void OnImGui();
		bool IsLoaded();
		void SetFilePath(const std::string& fileName);

		inline auto& GetFileName() const { return file; }

	private:
		void SaveNewFile(const std::string & fileName);
		void Init();
		std::string file;
		std::string className;
		entt::entity entity = entt::null;
		std::shared_ptr<luabridge::LuaRef> table;
		std::shared_ptr<luabridge::LuaRef> onInitFunc;
		std::shared_ptr<luabridge::LuaRef> onUpdateFunc;
		Scene* scene = nullptr;
	};
};
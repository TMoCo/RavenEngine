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
#include "MetaFile.h"
#include "Scene/Component/Component.h"

namespace Raven 
{
	class Scene;
	class LuaComponent : public Component
	{
	public:
		friend class MetaFile;
		LuaComponent(const std::string& file, Scene* scene);
		LuaComponent();
		~LuaComponent();
		void OnInit();
		void OnUpdate(float dt);
		void Reload();
		void LoadScript();

		template<typename Archive>
		void save(Archive& archive) const
		{
			archive(cereal::base_class<Component>(this));
			archive(
				cereal::make_nvp("FilePath", file)
			);

			metaFile.Save(this,file + ".meta");
		}

		template<typename Archive>
		void load(Archive& archive)
		{
			scene = Engine::Get().GetModule<SceneManager>()->GetCurrentScene();
			archive(cereal::base_class<Component>(this));
			archive(
				cereal::make_nvp("FilePath", file)
			);
			Init();
		}

		void OnImGui();
		bool IsLoaded();
		void SetFilePath(const std::string& fileName);

		inline auto& GetFileName() const { return file; }

		void SetScene(Scene* scene) { this->scene = scene; }

	private:

		void SaveNewFile(const std::string & fileName);
		void Init();
		std::string file;
		std::string className;

		std::shared_ptr<luabridge::LuaRef> table;
		std::shared_ptr<luabridge::LuaRef> onInitFunc;
		std::shared_ptr<luabridge::LuaRef> onUpdateFunc;
		Scene* scene = nullptr;

		MetaFile metaFile;
	};

};
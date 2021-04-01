//////////////////////////////////////////////////////////////////////////////
// This file is part of the Raven Game Engine			                    //
//////////////////////////////////////////////////////////////////////////////

#pragma once

#include <string>

namespace Raven 
{
	class Scene;
	class LuaComponent 
	{
	public:
		LuaComponent(const std::string& file, Scene* scene);
		LuaComponent();
		~LuaComponent();
		void OnInit();
		void OnUpdate(float dt);
		void Reload();
		void Load(const std::string& fileName);
		void LoadScript();


		template<typename Archive>
		void save(Archive& archive) const
		{
			archive(cereal::make_nvp("FilePath", file));
		}

		template<typename Archive>
		void load(Archive& archive)
		{
			archive(cereal::make_nvp("FilePath", file));
			Init();
		}
	private:
		void Init();
		std::string file;
	};
};
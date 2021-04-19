//////////////////////////////////////////////////////////////////////////////
// This file is part of the Raven Game Engine			                    //
//////////////////////////////////////////////////////////////////////////////

#pragma once
#include <string>



namespace Raven 
{
	class LuaComponent;
	class Scene;
	class MetaFile 
	{
	public:
		MetaFile() = default;
		void Save(const LuaComponent* comp, const std::string& name) const;
		void Load(LuaComponent* comp, const std::string& name, Scene* scene) ;
	private:
		friend class LuaComponent;
	};
};
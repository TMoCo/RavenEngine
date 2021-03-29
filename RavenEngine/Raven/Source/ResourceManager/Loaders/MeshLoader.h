//////////////////////////////////////////////////////////////////////////////
// This file is part of the Raven Game Engine			                    //
//////////////////////////////////////////////////////////////////////////////

#pragma once

#include "ResourceManager/Loaders/ILoader.h"
#include "ResourceManager/ResourceManager.h"

//
// A class for loading models, stored in various formats (just obj for the time being)
//

namespace Raven 
{
	class MeshLoader : public ILoader
	{
	public:
		MeshLoader(ResourceManager& initResourceManager) : ILoader(initResourceManager, ELoaderType::LT_Mesh) {}

		inline static auto Type() { return ELoaderType::LT_Mesh; } // type of loader

		bool LoadAsset(const std::string& path) override;

		bool LoadOBJ(const std::string& path);

		virtual bool LoadOnGPU() override { return true; }
	};
}

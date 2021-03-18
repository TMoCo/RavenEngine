//////////////////////////////////////////////////////////////////////////////
// This file is part of the Raven Game Engine			                    //
//////////////////////////////////////////////////////////////////////////////

#pragma once

#include "ILoader.h"

//
// A class for loading material data
//

namespace Raven
{
	class MaterialLoader : ILoader
	{
	public:
		MaterialLoader(ResourceManager& initResourceManager);

		inline static auto Type() { return ELoaderType::LT_Material; }

		bool virtual LoadAsset(const std::string& path);

		virtual bool LoadOnGPU() override { return true; }
	};
}
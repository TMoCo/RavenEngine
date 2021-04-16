//////////////////////////////////////////////////////////////////////////////
// This file is part of the Raven Game Engine			                    //
//////////////////////////////////////////////////////////////////////////////

#pragma once

#include "ResourceManager/Loaders/ILoader.h"
#include "ResourceManager/ResourceManager.h"

//
// A class for loading gui layouts
//

namespace Raven
{
	class LayoutLoader : public ILoader
	{
	public:
		LayoutLoader(ResourceManager& initResourceManager) : ILoader(initResourceManager, ELoaderType::LT_GuiLayout) {}

		inline static auto Type() { return ELoaderType::LT_GuiLayout; } // type of loader

		bool LoadAsset(const std::string& path) override;

		bool LoadOnGPU() override { return false; }
	};
}

//////////////////////////////////////////////////////////////////////////////
// This file is part of the Raven Game Engine			                    //
//////////////////////////////////////////////////////////////////////////////

#pragma once

#include "ResourceManager/Loaders/ILoader.h"
#include "ResourceManager/ResourceManager.h"

//
// A class for loading image data
//

namespace Raven
{
	class ImageLoader : public ILoader
	{
	public:
		ImageLoader(ResourceManager& initResourceManager);

		inline static auto Type() { return ELoaderType::LT_Image; }

		virtual bool LoadAsset(const std::string& path) override;

		virtual bool LoadOnGPU() override { return true; }
	};
}
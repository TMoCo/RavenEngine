//////////////////////////////////////////////////////////////////////////////
// This file is part of the Raven Game Engine			                    //
//////////////////////////////////////////////////////////////////////////////

#pragma once

#include <string>
#include <iostream>

#include "Utilities/Core.h"

// for serialization
#include "cereal/archives/json.hpp"
#include "cereal/archives/binary.hpp"

//
// The base loader class 
//

namespace Raven 
{
	class ResourceManager;

	// the type of loader (for each type of resource)
	enum class ELoaderType {
		LT_Image,
		LT_Mesh,
		LT_Material,
		LT_Scene,
		LT_Audio,
		LT_GuiLayout,
		LT_MAX
	};

	// base loader class interface
	class ILoader 
	{

	public:
		// constructor always needs to know which loader type the loader is
		ILoader(ResourceManager& initResourceManager, ELoaderType initType)
			: resourceManager(&initResourceManager), type(initType) {}

		ILoader()		   = delete;
		virtual ~ILoader() = default;

		// the type of an instantiated loader
		inline auto GetType() const noexcept { return type; }

		inline static std::string TypeToString(ELoaderType type)
		{
			switch (type)
			{
			case ELoaderType::LT_Image:
				return "Type: Image Loader.";
			case ELoaderType::LT_Mesh:
				return "Type: Mesh Loader.";
			case ELoaderType::LT_Material:
				return "Type: Material Loader.";
			case ELoaderType::LT_Scene:
				return "Type: Scene Loader.";
			case ELoaderType::LT_Audio:
				return "Type: Audio Loader.";
			case ELoaderType::LT_GuiLayout:
				return "Type: GUI Layout Loader.";
			default:
				return "Loader type not recognised";
			}
		}

		// must be overridden
		virtual bool LoadAsset(const std::string& path) = 0; // load asset from a file (non serialized)

		virtual bool LoadOnGPU() = 0; // load data to gpu using graphics api

	protected:
		const ELoaderType type;
		ResourceManager* resourceManager;

		NOCOPYABLE(ILoader);
	};
}

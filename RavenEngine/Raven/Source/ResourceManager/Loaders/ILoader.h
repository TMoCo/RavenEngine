#pragma once

#include <string>
#include <iostream>

#include "Utilities/Core.h"
//#include "ResourceManager/Resources/IResource.h"

// for serialization
#include "cereal/archives/json.hpp"
#include "cereal/archives/binary.hpp"

// the base loader class 

namespace Raven 
{
	class ResourceManager;

	// the type of loader (for each type of resource)
	enum class ELoaderType {
		LT_Image,
		LT_Model,
		LT_Material,
		LT_Scene,
		LT_Audio,
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

		// the type of loader
		inline auto GetType() const noexcept { return type; }

		inline static std::string TypeToString(ELoaderType type)
		{
			switch (type)
			{
			case ELoaderType::LT_Image:
				return "Type: Image Loader.";
			case ELoaderType::LT_Model:
				return "Type: Model Loader.";
			case ELoaderType::LT_Material:
				return "Type: Material Loader.";
			case ELoaderType::LT_Scene:
				return "Type: Scene Loader.";
			case ELoaderType::LT_Audio:
				return "Type: Audio Loader.";
			default:
				return "Loader type not recognised";
			}
		}

		// must be overridden
		virtual bool LoadAsset(const std::string& path) = 0;

	protected:

		const ELoaderType type;
		ResourceManager* resourceManager;

		NOCOPYABLE(ILoader);
	};
}

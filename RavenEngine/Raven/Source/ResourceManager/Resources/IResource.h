#pragma once

#include "Utilities/Core.h"

namespace Raven
{
	// enum for distinguishing between resource types
	enum class EResourceType
	{
		RT_Image,
		RT_Model,
		RT_Mesh,
		RT_Terrain,
		RT_Material,
		RT_Shader,
		RT_Audio
	};

	// the base resource class, inherited by resources. Basic resource info (type)
	class IResource
	{
	public:
		IResource(EResourceType initType, bool render=false) : type(initType), isRenderable(render), onGPU(false) {}
		virtual ~IResource() = default;

		// string from given resource type
		inline static std::string TypeToString(EResourceType type)
		{
			switch (type)
			{
			case EResourceType::RT_Image:
				return "Type: Image.";
			case EResourceType::RT_Mesh:
				return "Type: Mesh.";
			case EResourceType::RT_Terrain:
				return "Type: Terrain.";
			case EResourceType::RT_Material:
				return "Type: Material.";
			case EResourceType::RT_Audio:
				return "Type: Audio.";
			case EResourceType::RT_Model:
				return "Type: Model";
			default:
				return "Resource type not recognised";
			}
		} // 

		inline auto GetType() const noexcept { return type; }

		inline auto IsOnGPU() const noexcept { return onGPU; } // query if a resource is loaded on to GPU

	protected:
		const EResourceType type;
		const bool isRenderable;
		bool onGPU;

		NOCOPYABLE(IResource);
	};



}
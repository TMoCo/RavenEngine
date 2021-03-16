#pragma once

#include "Utilities/Core.h"

namespace Raven
{
	// enum for distinguishing between resource types
	enum class EResourceType
	{
		RT_Image,
		RT_Mesh,
		RT_Terrain,
		RT_Material,
		RT_Audio
	};

	// the base resource class, inherited by resources. Basic resource info (type)
	class IResource
	{
	public:
		IResource(EResourceType initType) : type(initType) {}
		virtual ~IResource() = default;

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
			default:
				return "Resource type not recognised";
			}
		}

		inline EResourceType GetType() const noexcept { return type; }

	protected:
		const EResourceType type;

		NOCOPYABLE(IResource);
	};



}
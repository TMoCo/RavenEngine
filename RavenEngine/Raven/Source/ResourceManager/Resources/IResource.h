#pragma once

#include "Utilities/Core.h"

namespace Raven
{
	// enum for distinguishing between resource types
	enum class EResourceType
	{
		RT_Image
		//RT_Mesh,
		//RT_Material,
		//RT_Scene,
		//RT_Audio
	};

	// the base resource class, inherited by resources. Basic resource info (type)
	class IResource
	{
	public:
		IResource(EResourceType initType) : type(initType) {}

		inline EResourceType GetType() const noexcept { return type; }

	protected:
		const EResourceType type;

		NOCOPYABLE(IResource);
	};



}
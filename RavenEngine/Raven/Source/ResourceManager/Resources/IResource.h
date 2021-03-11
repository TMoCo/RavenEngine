#pragma once

namespace Raven
{
	class Texture2D;

	// enum for distinguishing between resource types
	enum class EResourceType
	{
		RT_Image
		//RT_Mesh,
		//RT_Material,
		//RT_Scene,
		//RT_Audio
	};

	// the base resource class
	class IResource
	{
		// static factory method
		static IResource* CreateResource(EResourceType initType);

	protected:
		IResource(EResourceType initType) : type(initType) {}

		inline EResourceType GetType()  const noexcept { return type; }

		// where resource data goes, for now just its type 
		const EResourceType type;
	};



}
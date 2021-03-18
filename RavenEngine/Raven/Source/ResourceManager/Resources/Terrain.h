#pragma once

#include "glm/glm.hpp"

#include "ResourceManager/Resources/IResource.h"
#include "Render/RenderResource/RenderRscTerrain.h"

namespace Raven
{
	class Terrain : public IResource
	{
	public:
		Terrain() : IResource(EResourceType::RT_Terrain, true) {}
		// TODO: free data on destruction
		virtual ~Terrain() = default;

		inline static EResourceType Type() noexcept { return EResourceType::RT_Terrain; } // return the resource type

		Texture2D* heightMap;

		RenderRscTerrain* renderResource = nullptr; // interface with renderer

		NOCOPYABLE(Terrain);
	};
}

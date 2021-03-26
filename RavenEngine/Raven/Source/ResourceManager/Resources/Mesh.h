//////////////////////////////////////////////////////////////////////////////
// This file is part of the Raven Game Engine			                    //
//////////////////////////////////////////////////////////////////////////////

#pragma once

#include "glm/glm.hpp"

#include "ResourceManager/Resources/IResource.h"
#include "Render/RenderResource/RenderRscMesh.h"

//
// A class for a 3D mesh and its data
//

namespace Raven
{
	class Mesh : public IResource
	{
	// This type of resource will rarely if not ever be on its own in the resource register.
	// It is almost definitely part of a Model resource.
	public:
		Mesh() : IResource(EResourceType::RT_Mesh, true) {}
		// TODO: free data on destruction
		virtual ~Mesh() = default;

		inline static EResourceType Type() noexcept { return EResourceType::RT_Mesh; } // return the resource type

		inline void LoadOnGpu()
		{
			if (!onGPU)
			{
				renderRscMesh.Load(positions, normals, texCoords, indices); // call interface method
				onGPU = true;
			}
		}

		inline auto IsActive() const { return active; }

		std::vector<glm::vec3> positions;
		std::vector<glm::vec3> normals;
		std::vector<glm::vec2> texCoords;

		std::vector<uint32_t> indices;

		RenderRscMesh renderRscMesh; // interface with renderer (default constructor)

		bool active = true;
	};
}

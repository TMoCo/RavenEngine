#pragma once

#include "glm/glm.hpp"

#include "ResourceManager/Resources/IResource.h"
#include "Render/RenderResource/RenderRscMesh.h"

namespace Raven
{
	class Mesh : public virtual IResource
	{
	public:
		Mesh() : IResource(EResourceType::RT_Mesh, true) {}
		// TODO: free data on destruction
		virtual ~Mesh() = default;

		inline static EResourceType Type() noexcept { return EResourceType::RT_Mesh; } // return the resource type

		inline void LoadOnGpu()
		{
			if (!onGPU)
			{
				renderRscMesh.Load(verts, indices); // call interface method
				onGPU = true;
			}
		}

		std::vector<glm::vec3> verts;
		std::vector<glm::vec3> normals;
		std::vector<glm::vec2> texCoords;

		std::vector<uint32_t> indices;

		RenderRscMesh renderRscMesh; // interface with renderer (default constructor)

		NOCOPYABLE(Mesh);
	};
}

#pragma once

#include "glm/glm.hpp"

#include "ResourceManager/Resources/IResource.h"
#include "Render/RenderResource/RenderRscMesh.h"

namespace Raven
{
	class Mesh : public IResource
	{
	public:
		Mesh() : IResource(EResourceType::RT_Mesh) {}
		// TODO: free data on destruction
		virtual ~Mesh() = default;

		std::vector<glm::vec3> verts;
		std::vector<glm::vec3> normals;
		std::vector<glm::vec2> texCoords;

		std::vector<uint32_t> indices;

		RenderRscMesh* renderResource = nullptr; // interface with renderer

		NOCOPYABLE(Mesh);
	};
}

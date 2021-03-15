#pragma once

#include "glm/glm.hpp"

#include "ResourceManager/Resources/IResource.h"

namespace Raven
{
	class RenderMeshResource;

	class Mesh : public IResource
	{
	public:
		Mesh() : IResource(EResourceType::RT_Image) {}

		std::vector<glm::vec3> verts;
		std::vector<glm::vec3> normals;
		std::vector<glm::vec2> texCoords;

		std::vector<uint32_t> indices;

		RenderMeshResource* renderResource = nullptr; // interface with renderer

		NOCOPYABLE(Mesh);
	};
}

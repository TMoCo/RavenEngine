//////////////////////////////////////////////////////////////////////////////
// This file is part of the Raven Game Engine			                    //
//////////////////////////////////////////////////////////////////////////////

#pragma once

#include "glm/glm.hpp"

#include "Math/BoundingBox.h"

#include "ResourceManager/Resources/IResource.h"
#include "Render/RenderResource/Primitives/RenderRscMesh.h"

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
		// Construct.
		Mesh() 
			: IResource(EResourceType::RT_Mesh, true) 
			, isTranslucentShadowCast(false)
		{
		}

		// TODO: free data on destruction
		virtual ~Mesh()
		{
			delete renderRscMesh;
		}

		inline static EResourceType Type() noexcept { return EResourceType::RT_Mesh; } // return the resource type

		inline void LoadOnGpu()
		{
			if (!onGPU)
			{
				renderRscMesh->Load(positions, normals, texCoords, indices); // call interface method
				onGPU = true;
			}
		}

		inline auto IsActive() const { return active; }

		std::vector<glm::vec3> positions;
		std::vector<glm::vec3> normals;
		std::vector<glm::vec2> texCoords;

		std::vector<uint32_t> indices;

		// The Bounding box the mesh.
		MathUtils::BoundingBox bounds;

		RenderRscMesh* renderRscMesh = nullptr; // interface with renderer (default constructor)

		bool active = true;

		// Should this mesh cast shadow even if it is translucent.
		bool isTranslucentShadowCast;
	};
}

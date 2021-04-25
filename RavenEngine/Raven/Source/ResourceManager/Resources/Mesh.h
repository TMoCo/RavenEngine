//////////////////////////////////////////////////////////////////////////////
// This file is part of the Raven Game Engine			                    //
//////////////////////////////////////////////////////////////////////////////

#pragma once

#include "glm/glm.hpp"

#include "Math/BoundingBox.h"

#include "ResourceManager/Resources/IResource.h"
#include "Render/RenderResource/Primitives/RenderRscMesh.h"

#include "Animation/Bone.h"
//
// A class for a 3D mesh and its data
//

namespace ofbx
{
	struct Mesh;
};

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
				RAVEN_ASSERT(renderRscMesh == nullptr, "");

				if (blendWeights.empty())
					renderRscMesh = new RenderRscMesh();
				else
					renderRscMesh = new RenderRscSkinnedMesh();

				renderRscMesh->Load(
					positions, normals, texCoords, indices, blendWeights,blendIndices
				); // call interface method
				onGPU = true;
			}
		}


		inline void NormalizeBlendWeights()
		{
			RAVEN_ASSERT(positions.size() == blendWeights.size(), "size is not correct");
			for (int32_t i = 0; i < positions.size(); i++)
			{
				float sum = 0;
				for (int32_t j = 0; j < 4; j++)
				{
					sum += blendWeights[i][j];
				}
				const float invSum = sum > MathUtils::EPS ? 1.0f / sum : 0.0f;
				blendWeights[i] *= invSum;
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
		std::string name;
		bool active = true;

		/// Skinned mesh blend indices (max 4 per bone)
		std::vector<glm::ivec4> blendIndices;

		/// Skinned mesh index buffer (max 4 per bone)
		std::vector<glm::vec4> blendWeights;

	};

}

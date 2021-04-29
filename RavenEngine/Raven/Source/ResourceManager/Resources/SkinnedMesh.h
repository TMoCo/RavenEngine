//////////////////////////////////////////////////////////////////////////////
// This file is part of the Raven Game Engine			                    //
//////////////////////////////////////////////////////////////////////////////

#pragma once

#include "glm/glm.hpp"

#include "Math/BoundingBox.h"

#include "ResourceManager/Resources/IResource.h"
#include "Render/RenderResource/Primitives/RenderRscMesh.h"






namespace Raven
{
	//
	//
	class MeshSection
	{
	public:
		MeshSection()
		{

		}

		// Position Buffer.
		std::vector<glm::vec3> positions;

		// Normals Buffer.
		std::vector<glm::vec3> normals;

		// Texture Coordinate Buffer.
		std::vector<glm::vec2> texCoords;

		// indices Buffer.
		std::vector<uint32_t> indices;

		// The bounding box of this mesh section.
		MathUtils::BoundingBox bounds;

		// Render Resrouce Data.
		Ptr<RenderRscMesh> renderRscMesh;
	};


	// Mesh:
	//  - resrouce for static meshes.
	class Mesh : public IResource
	{
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


		bool active = true;

		/// Skinned mesh blend indices (max 4 per bone)
		std::vector<glm::ivec4> blendIndices;

		/// Skinned mesh index buffer (max 4 per bone)
		std::vector<glm::vec4> blendWeights;

	};

}

//////////////////////////////////////////////////////////////////////////////
// This file is part of the Raven Game Engine			                    //
//////////////////////////////////////////////////////////////////////////////

#pragma once

#include "Utilities/Core.h"
#include "Math/BoundingBox.h"

#include "ResourceManager/Resources/IResource.h"
#include "Render/RenderResource/Primitives/RenderRscMesh.h"

#include "glm/glm.hpp"
#include "glm/glm.hpp"




namespace Raven
{
	class Skeleton;


	// SkinnedMeshSection:
	//			- 
	class SkinnedMeshSection
	{
	public:
		// Construct.
		SkinnedMeshSection()
		{

		}

		// Load Render Resrouces.
		inline void LoadRenderResource()
		{
			RAVEN_ASSERT(!renderRscMesh, "Resrouce already on GPU. use UpdateRenderRsc to update.");

			renderRscMesh = Ptr<RenderRscSkinnedMesh>(new RenderRscSkinnedMesh());
			renderRscMesh->Load(
				positions,
				normals,
				texCoords,
				indices,
				blendWeights,
				blendIndices
			);
		}

		// Normalize weights.
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


		// Position Buffer.
		std::vector<glm::vec3> positions;

		// Normals Buffer.
		std::vector<glm::vec3> normals;

		// Texture Coordinate Buffer.
		std::vector<glm::vec2> texCoords;

		// indices Buffer.
		std::vector<uint32_t> indices;

		// Skinned mesh blend indices (max 4 per bone).
		std::vector<glm::ivec4> blendIndices;

		// Skinned mesh index buffer (max 4 per bone).
		std::vector<glm::vec4> blendWeights;

		// The bounding box of this mesh section.
		MathUtils::BoundingBox bounds;

		// Render Resrouce Data.
		Ptr<RenderRscSkinnedMesh> renderRscMesh;
	};


	// SkinnedMesh:
	//  - resrouce for skinned meshes.
	//
	class SkinnedMesh : public IResource
	{
	public:
		// Construct.
		SkinnedMesh()
			: IResource()
		{
			type = SkinnedMesh::StaticGetType();
			hasRenderResources = true;
		}

		// return the resource type
		inline static EResourceType StaticGetType() noexcept { return EResourceType::RT_SkinnedMesh; }

		// Load Render Resrouces.
		inline virtual void LoadRenderResource() override
		{
			RAVEN_ASSERT(!isOnGPU, "Resrouce already on GPU. use UpdateRenderRsc to update.");

			for (auto& section : sections)
			{
				// Invalid Section?
				if (!section)
					continue;

				section->LoadRenderResource();
			}
		}

		// Update Render Resrouces.
		inline virtual void UpdateRenderResource() override
		{
			RAVEN_ASSERT(isOnGPU, "Resrouce not on GPU. use LoadRenderRsc to load it first.");
			// TODO: update.
		}

		// Set a new mesh section.
		inline void SetMeshSection(uint32_t index, Ptr<SkinnedMeshSection> section)
		{
			if (sections.size() < index + 1)
				sections.resize(index + 1);

			sections[index] = section;

			UpdateBounds();
		}

		// Add new mesh section at the end of the sections list.
		inline void AddMeshSection(Ptr<SkinnedMeshSection> section)
		{
			sections.push_back(section);

			UpdateBounds();
		}

		// Return mesh section.
		inline SkinnedMeshSection* GetMeshSection(uint32_t index) { return sections[index].get(); }

		// Return mesh section list.
		inline auto& GetMeshSections() { return sections; }
		inline const auto& GetMeshSections() const { return sections; }

		// Return the number of sections in a mesh.
		inline uint32_t GetNumSections() const { return sections.size(); }

		// Return the bounds of this mesh.
		inline MathUtils::BoundingBox GetBounds() const { return bounds; }

		// Return the skeleton of this skinned mesh.
		inline const Ptr<Skeleton>& GetSkeleton() const { return skeleton; }

		// Set the skeleton of this skinned mesh.
		inline void SetSkeleton(Ptr<Skeleton> inSkeleton) { skeleton = inSkeleton; }


		// Normalize weights for all skinned mesh sections.
		inline void NormalizeBlendWeights()
		{
			for (auto& section : sections)
			{
				// Invalid Section?
				if (!section)
					continue;

				section->NormalizeBlendWeights();
			}
		}


	private:
		// Recompute/Update the bounding box the model.
		inline void UpdateBounds()
		{
			bounds.Reset();

			for (const auto& ms : sections)
			{
				bounds.Add(ms->bounds);
			}
		}

	private:
		// List of all mesh sections.
		std::vector< Ptr<SkinnedMeshSection> > sections;

		// The boudning box of all mesh sections.
		MathUtils::BoundingBox bounds;

		// The skeleton of this skinned mesh.
		Ptr<Skeleton> skeleton;
	};

}

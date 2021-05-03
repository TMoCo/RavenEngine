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
	// MeshSection:
	//    - contain the data of a single section in a mesh.
	class MeshSection
	{
	public:
		MeshSection()
		{

		}


		// Load Render Resrouces.
		inline void LoadRenderResource()
		{
			RAVEN_ASSERT(!renderRscMesh, "Resrouce already on GPU. use UpdateRenderRsc to update.");

			renderRscMesh = Ptr<RenderRscMesh>(new RenderRscMesh());
			renderRscMesh->Load(
				positions,
				normals,
				tangents,
				texCoords,
				indices
			);
		}


		// Serialization Save.
		template<typename Archive>
		void save(Archive& archive) const
		{
			// Save Geometry.
			SaveVectorBinary(archive, positions);
			SaveVectorBinary(archive, normals);
			SaveVectorBinary(archive, tangents);
			SaveVectorBinary(archive, texCoords);
			SaveVectorBinary(archive, indices);
			archive(bounds);
		}


		// Serialization Load.
		template<typename Archive>
		void load(Archive& archive)
		{
			// Save Load.
			LoadVectorBinary(archive, positions);
			LoadVectorBinary(archive, normals);
			LoadVectorBinary(archive, tangents);
			LoadVectorBinary(archive, texCoords);
			LoadVectorBinary(archive, indices);
			archive(bounds);
		}


		// Position Buffer.
		std::vector<glm::vec3> positions;

		// Normals Buffer.
		std::vector<glm::vec3> normals;

		// Tangents Buffer.
		std::vector<glm::vec3> tangents;

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
	//  - a mesh that is made of multiple seprate sections.
	//
	class Mesh : public IResource
	{
	public:
		// Construct.
		Mesh() 
			: IResource() 
		{
			type = Mesh::StaticGetType();
			hasRenderResources = true;
		}

		// return the resource type
		inline static EResourceType StaticGetType() noexcept { return EResourceType::RT_Mesh; }

		// Load Render Resrouces.
		inline virtual void LoadRenderResource() override
		{
			RAVEN_ASSERT(!isOnGPU, "Resrouce already on GPU. use UpdateRenderRsc to update.");
			isOnGPU = true;

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
		inline void SetMeshSection(uint32_t index, Ptr<MeshSection> section)
		{
			if (sections.size() < index + 1)
				sections.resize(index + 1);

			sections[index] = section;
			UpdateBounds();
		}

		// Add new mesh section at the end of the sections list.
		inline void AddMeshSection(Ptr<MeshSection> section)
		{
			sections.push_back(section);
			UpdateBounds();
		}

		// Return mesh section.
		inline MeshSection* GetMeshSection(uint32_t index) { return sections[index].get(); }

		// Return mesh section list.
		inline auto& GetMeshSections() { return sections; }
		inline const auto& GetMeshSections() const { return sections; }

		// Return the number of sections in a mesh.
		inline uint32_t GetNumSections() const { return static_cast<uint32_t>(sections.size()); }

		// Return the bounds of this mesh.
		inline MathUtils::BoundingBox GetBounds() const { return bounds; }

		// Serialization Save.
		template<typename Archive>
		void save(Archive& archive) const
		{
			archive(cereal::base_class<IResource>(this));

			uint32_t numSections = (uint32_t)sections.size();
			archive(numSections);

			// Only archive valid mesh sections.
			for (uint32_t i = 0; i < numSections; ++i)
			{
				bool isValid = sections[i] != nullptr;
				archive(isValid);

				if (isValid)
					archive(*sections[i]);
			}

		}

		// Serialization Load.
		template<typename Archive>
		void load(Archive& archive)
		{
			archive(cereal::base_class<IResource>(this));

			uint32_t numSections;
			archive(numSections);
			sections.resize(numSections);

			// Only archive valid mesh sections.
			for (uint32_t i = 0; i < numSections; ++i)
			{
				bool isValid = false;
				archive(isValid);
				if (isValid)
				{
					sections[i] = Ptr<MeshSection>(new MeshSection());
					archive(*sections[i]);
				}
			}

			// Update bounds after loading.
			UpdateBounds();
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
		std::vector< Ptr<MeshSection> > sections;

		// The boudning box of all mesh sections.
		MathUtils::BoundingBox bounds;
	};

}

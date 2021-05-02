//////////////////////////////////////////////////////////////////////////////
// This file is part of the Raven Game Engine			                    //
//////////////////////////////////////////////////////////////////////////////

#pragma once

#include <memory>
#include <vector>
#include "Component.h"
#include "Animation/Skeleton.h"
#include <cereal/types/memory.hpp>

namespace Raven 
{
	class SkinnedMeshSection;


	class MeshRenderer : public Component
	{
	public:

		template<class Archive>
		void load(Archive& archive)
		{
			archive(cereal::base_class<Component>(this));

			archive(
				cereal::make_nvp("MeshIndex", meshIndex)
			);
			GetMeshFromModel();
		}

		template<class Archive>
		void save(Archive& archive) const
		{
			archive(cereal::base_class<Component>(this));

			archive(
				cereal::make_nvp("MeshIndex", meshIndex)
			);
		}

		void GetMeshFromModel();

		int32_t meshIndex;
		SkinnedMeshSection* mesh;
	};

	
	class SkinnedMeshRenderer : public Component
	{
	public:
		template<class Archive>
		void load(Archive& archive)
		{
			archive(cereal::base_class<Component>(this));
			archive(
				cereal::make_nvp("MeshIndex", meshIndex),
				cereal::make_nvp("Skeleton", skeleton)
			);
			GetMeshFromModel();
		}

		template<class Archive>
		void save(Archive& archive) const
		{
			archive(cereal::base_class<Component>(this));
			archive(
				cereal::make_nvp("MeshIndex", meshIndex),
				cereal::make_nvp("Skeleton", skeleton)
			);
		}

		void OnImGui();

		void GetMeshFromModel();
		inline auto& getSkeleton() const { return skeleton; }

		//
		void UpdateBones();

		int32_t meshIndex;
		SkinnedMeshSection* mesh;
		Skeleton skeleton;

		// Current Bones Transform.
		mutable std::vector<glm::mat4> bones;
	};

};

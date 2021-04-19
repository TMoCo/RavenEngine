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
	class Mesh;
	class MeshRenderer : public Component
	{
	public:

		template<class Archive>
		void load(Archive& archive)
		{
			archive(
				cereal::make_nvp("MeshIndex", meshIndex),
				cereal::make_nvp("Id", entity)
			);
			GetMeshFromModel();
		}

		template<class Archive>
		void save(Archive& archive) const
		{
			archive(
				cereal::make_nvp("MeshIndex", meshIndex),
				cereal::make_nvp("Id", entity)
			);
		}

		void GetMeshFromModel();

		int32_t meshIndex;
		std::shared_ptr<Mesh> mesh;
	};

	
	class SkinnedMeshRenderer : public Component
	{
	public:
		template<class Archive>
		void load(Archive& archive)
		{
			archive(
				cereal::make_nvp("MeshIndex", meshIndex),
				cereal::make_nvp("Id", entity),
				cereal::make_nvp("Skeleton", skeleton)
			);
			GetMeshFromModel();
		}

		template<class Archive>
		void save(Archive& archive) const
		{
			archive(
				cereal::make_nvp("MeshIndex", meshIndex),
				cereal::make_nvp("Id", entity),
				cereal::make_nvp("Skeleton", skeleton)
			);
		}

		void OnImGui();

		void GetMeshFromModel();
		inline auto& getSkeleton() const { return skeleton; }

		int32_t meshIndex;
		std::shared_ptr<Mesh> mesh;
		Skeleton skeleton;
	private:
	};

};

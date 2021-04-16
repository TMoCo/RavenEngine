//////////////////////////////////////////////////////////////////////////////
// This file is part of the Raven Game Engine			                    //
//////////////////////////////////////////////////////////////////////////////

#pragma once

#include <memory>
#include <string>
#include <vector>

#include <cereal/cereal.hpp>
#include <cereal/types/vector.hpp>
#include <cereal/types/memory.hpp>

namespace Raven 
{
	struct Bone;

	class Skeleton final
	{
	public:
		int32_t AddBone(Bone* bone, int32_t parentId);
		int32_t IndexOf(const std::string& name);
		inline auto GetBoneSize() const { return bones.size(); }
		inline auto& GetBones() const { return bones; }
		inline auto& GetBones() { return bones; }
		inline auto GetBone(int32_t i) { return bones[i]; }
		inline auto GetRoot() { return root; }

		template<class Archive>
		void load(Archive& archive)
		{
			archive(
				cereal::make_nvp("bones", bones)
			);
			for (auto & bone : bones)
			{
				if (bone->parentIdx == -1) 
				{
					root = bone;
					break;
				}
			}
		}

		template<class Archive>
		void save(Archive& archive) const
		{
			archive(
				cereal::make_nvp("bones", bones)
			);
		}

	private:
		std::vector<std::shared_ptr<Bone>> bones;
		std::shared_ptr<Bone> root;
	};


};
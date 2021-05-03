#pragma once


#include "Utilities/Core.h"
#include "ResourceManager/Resources/IResource.h"
#include "Bone.h"


#include <cereal/cereal.hpp>
#include <cereal/types/vector.hpp>
#include <cereal/types/memory.hpp>

#include <memory>
#include <string>
#include <vector>



namespace Raven 
{

	// Skeleton:
	//		- a skeleton made of bones.
	//
	class Skeleton : public IResource
	{
	public:
		// Contruct.
		Skeleton();

		// Destruct.
		virtual ~Skeleton();

		// return the resource type.
		inline static EResourceType StaticGetType() noexcept { return EResourceType::RT_Skeleton; }

		// Create a new bone, if parentId = -1 then there is not parent.
		Bone& CreateBone(int32_t parentId);

		// Return a bone index of that name.
		int32_t GetBoneIndex(const std::string& name);

		// Return a bone at index.
		inline Bone& GetBone(int32_t index) { return bones[index]; }
		inline const Bone& GetBone(int32_t index) const { return bones[index]; }

		// Return all the bones
		inline const std::vector<Bone>& GetBones() const { return bones; }

		// Return the root bone.
		inline const Bone* GetRoot() const { return root; }
	
		// Check if a bone index is valid.
		inline bool IsValidBoneIndex(int32_t index) const { return index < bones.size() && index >= 0; }

		// Serialization Load.
		template<class Archive>
		void load(Archive& archive)
		{
			archive(cereal::base_class<IResource>(this));

			LoadVector(archive, bones);

			// Build loaded skeleton.
			Build();
		}

		// Serialization Save.
		template<class Archive>
		void save(Archive& archive) const
		{
			archive(cereal::base_class<IResource>(this));

			SaveVector(archive, bones);
		}

		// Reset Bone Transforms.
		void ResetTransforms();

		// Build the skeleton.
		void Build();

	private:
		// List of all bones that makes the skeleton.
		std::vector<Bone> bones;

		// The Root Bone.
		Bone* root;

		// If true the skeleton is built and ready to use.
		bool isBuilt;
	};


	// SkeletonInstance:
	//    - Dynamic skeleton data for updating the bone transforms for each SkinnedMeshComponent.
	class SkeletonInstance
	{
	public:
		// Construct.
		SkeletonInstance(Ptr<Skeleton> inParent);

		// Destruct.
		~SkeletonInstance();

		// Serialization Save.
		template<typename Archive>
		void save(Archive& archive) const
		{
			RAVEN_ASSERT(0, "Not Implemented");
		}

		// Serialization Load.
		template<typename Archive>
		void load(Archive& archive)
		{
			RAVEN_ASSERT(0, "Not Implemented");
		}

		// Update the bones transforms to match the current skeleton bones.
		void UpdateBones();

		// Return a pointer to the list of all bones tranforms of this instance.
		inline const std::vector<glm::mat4>* GetBones() const { return &boneTransforms; }

		// Return the parent skeleton of this instance.
		inline Skeleton* GetParent() const { return parent.get(); }

		// Dirty bone transforms.
		void DirtyTransforms();

	private:
		// All bone transforms for this instance.
		std::vector<glm::mat4> boneTransforms;

		// Parent Skeleton of this instance.
		Ptr<Skeleton> parent;
	};

}

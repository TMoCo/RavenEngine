#include "Skeleton.h"



#include <vector>
#include <iostream>




namespace Raven
{


Skeleton::Skeleton()
	: IResource()
	, isBuilt(false)
	, root(nullptr)
{
	type = Skeleton::StaticGetType();
}


Skeleton::~Skeleton()
{

}


Bone& Skeleton::CreateBone(int32_t parentId)
{
	RAVEN_ASSERT(!isBuilt, "Skeleton is built and finished, you can't create new bones.");

	// New Bone.
	auto& bone = bones.emplace_back();

	// Index mapping.
	bone.id = bones.size() - 1;
	bone.parentIdx = parentId;

	return bone;
}


int32_t Skeleton::GetBoneIndex(const std::string& name)
{
	for (int32_t i = 0; i < bones.size(); i++)
	{
		if (bones[i].name == name)
			return i;
	}
	
	return -1;
}


void Skeleton::ResetTransforms()
{
	RAVEN_ASSERT(isBuilt, "Skeleton is not built.");

	for (auto & bone : bones)
	{
		bone.SetPosition(glm::vec3(0.0f));
		bone.SetRotation(glm::vec3(0.0f));
	}
}


void Skeleton::Build()
{
	RAVEN_ASSERT(!isBuilt, "Skeleton rebuild not allowed.");

	// Build skeleton bones.
	for (Bone& bone : bones)
	{
		// Has Parent?
		if (bone.parentIdx != -1)
		{
			bone.parent = &bones[bone.parentIdx];
			bone.parent->children.push_back(&bone);
		}
		else
		{
			root = &bone;
			root->parent = nullptr;
		}

		// Rest to rest-pose.
		bone.ResetPose();
	}

	// No Root Found?
	RAVEN_ASSERT(root != nullptr, "Root Not Found.");

	// Done.
	isBuilt = true;
}




// -- - --- - -- - --- - -- - --- - -- - --- - -- - --- - -- - --- - -- - --- - -- - --- - -- - --- - 



SkeletonInstance::SkeletonInstance(Ptr<Skeleton> inParent)
	: parent(inParent)
{
	RAVEN_ASSERT(parent != nullptr, "Invalid Skeleton.");

	boneTransforms.resize(parent->GetBones().size(), glm::mat4(1.0f));
}


SkeletonInstance::~SkeletonInstance()
{

}


void SkeletonInstance::UpdateBones()
{
	// First mark all cached transform dirty.
	DirtyTransforms();

	const auto& bones = parent->GetBones();
	int32_t boneCount = static_cast<int32_t>(bones.size());

	for (int32_t i = 0; i < boneCount; ++i)
	{
		boneTransforms[i] = bones[i].GetWorldTransform() * bones[i].GetOffsetMatrix();
	}

}


void SkeletonInstance::DirtyTransforms()
{
	auto& bones = parent->GetBones();
	int32_t boneCount = static_cast<int32_t>(bones.size());

	for (int32_t i = 0; i < boneCount; ++i)
	{
		bones[i].DirtyWorldMatrix();
	}
}



} // End of namespace Raven.

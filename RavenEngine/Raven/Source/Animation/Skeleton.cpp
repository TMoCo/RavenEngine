

#include "Skeleton.h"
#include <vector>
#include <iostream>
#include "Bone.h"
#include "Scene/Entity/Entity.h"

namespace Raven
{
	Bone& Skeleton::AddBone(int32_t parentId)
	{
		auto& bone = bones.emplace_back();
		bone.id = bones.size() - 1;
		bone.parentIdx = parentId;
		if (parentId == -1) 
		{
			root = &bone;
		}
		return bone;
	}

	int32_t Skeleton::IndexOf(const std::string& name)
	{
		for (int32_t i = 0; i < bones.size(); i++)
		{
			if (bones[i].name == name)
				return i;
		}
		return -1;
	}

	void Skeleton::ResetTransfromTarget(Entity entity)
	{
		for (auto & bone : bones)
		{
			auto & en = entity.GetChildInChildren(bone.name);
			if (bone.parentIdx == -1) 
			{
				root = &bone;
			}
			if (en.Valid()) 
			{
				bone.localTransform = en.TryGetComponent<Transform>();
			}
		}
	}

};
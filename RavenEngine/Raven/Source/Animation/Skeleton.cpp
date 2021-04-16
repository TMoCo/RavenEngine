

#include "Skeleton.h"
#include <vector>
#include <iostream>
#include "Bone.h"

namespace Raven
{
	int32_t Skeleton::AddBone(Bone* bone,int32_t parentId)
	{
		bone->id = bones.size();
		bone->parentIdx = parentId;
		auto b = std::shared_ptr<Bone>(bone);
		bones.emplace_back(b);
		if (parentId == -1) 
		{
			root = b;
		}
		return bone->id;
	}

	int32_t Skeleton::IndexOf(const std::string& name)
	{
		for (int32_t i = 0; i < bones.size(); i++)
		{
			if (bones[i]->name == name)
				return i;
		}
		return -1;
	}

};
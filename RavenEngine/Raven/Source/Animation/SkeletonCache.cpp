
#include "SkeletonCache.h"
#include "ResourceManager/FbxLoader_deprecated.h"

namespace Raven 
{
	SkeletonCache& SkeletonCache::Get()
	{
		static SkeletonCache cache;
		return cache;
	}

	Skeleton* SkeletonCache::Save(const std::string& name, const Skeleton& skeleton)
	{
		return &skeletons.emplace(std::piecewise_construct,
			std::forward_as_tuple(name),
			std::forward_as_tuple(skeleton)).first->second;
	}

	Skeleton* SkeletonCache::Get(const std::string& name)
	{
		auto iter = skeletons.find(name);
		if (iter == skeletons.end())
		{
			return nullptr;
		}
		return &iter->second;
	}

};


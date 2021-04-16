
#include "SkeletonCache.h"
#include "ResourceManager/FbxLoader.h"

namespace Raven 
{
	SkeletonCache& SkeletonCache::Get()
	{
		static SkeletonCache cache;
		return cache;
	}

	void SkeletonCache::Save(const std::string& name, const std::shared_ptr<Skeleton>& skeleton)
	{
		skeletons.emplace(std::piecewise_construct,
			std::forward_as_tuple(name),
			std::forward_as_tuple(skeleton));
	}

	std::shared_ptr<Skeleton> SkeletonCache::Get(const std::string& name)
	{
		auto iter = skeletons.find(name);
		if (iter == skeletons.end())
		{
			return nullptr;
		}
		return iter->second;
	}

};


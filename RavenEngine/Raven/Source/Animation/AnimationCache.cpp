
#include "AnimationCache.h"
#include "ResourceManager/FbxLoader.h"

namespace Raven 
{
	AnimationCache& AnimationCache::Get()
	{
		static AnimationCache cache;
		return cache;
	}

	Animation* AnimationCache::Get(const std::string& name)
	{
		auto iter = animations.find(name);
		if (iter == animations.end())
		{
			FbxLoader loader;
			loader.LoadAnimation(name, nullptr);
			auto anim = loader.GetAnimation();
			if (anim == nullptr) {
				return nullptr;
			}
			return animations.emplace(std::piecewise_construct, 
				std::forward_as_tuple(name),
				std::forward_as_tuple(anim)).first->second.get();
		}
		return iter->second.get();
	}

};


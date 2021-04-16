//////////////////////////////////////////////////////////////////////////////
// This file is part of the Raven Game Engine			                    //
//////////////////////////////////////////////////////////////////////////////
#pragma once
#include <unordered_map>
#include <memory>
#include <string>
#include "Skeleton.h"

namespace Raven
{

	class SkeletonCache final
	{
	public:
		static SkeletonCache& Get();
		Skeleton* Get(const std::string& name);
		Skeleton* Save(const std::string& name,const Skeleton & skeleton);
	private:
		SkeletonCache()= default;
		std::unordered_map<std::string, Skeleton> skeletons;
	};

};
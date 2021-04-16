//////////////////////////////////////////////////////////////////////////////
// This file is part of the Raven Game Engine			                    //
//////////////////////////////////////////////////////////////////////////////
#pragma once
#include <unordered_map>
#include <memory>
#include <string>


namespace Raven
{
	class Skeleton;

	class SkeletonCache final
	{
	public:
		static SkeletonCache& Get();
		std::shared_ptr<Skeleton> Get(const std::string& name);
		void Save(const std::string& name,const std::shared_ptr<Skeleton> & skeleton);
	private:
		SkeletonCache()= default;
		std::unordered_map<std::string, std::shared_ptr<Skeleton>> skeletons;
	};

};
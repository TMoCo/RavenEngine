//////////////////////////////////////////////////////////////////////////////
// This file is part of the Raven Game Engine			                    //
//////////////////////////////////////////////////////////////////////////////
#pragma once
#include <unordered_map>
#include <memory>
#include <string>


namespace Raven
{
	class Animation;

	class AnimationCache final
	{
	public:
		static AnimationCache& Get();
		std::shared_ptr<Animation> Get(const std::string& name);
	private:
		AnimationCache()= default;
		std::unordered_map<std::string, std::shared_ptr<Animation>> animations;
	};

};
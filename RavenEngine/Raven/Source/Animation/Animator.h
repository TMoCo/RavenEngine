//////////////////////////////////////////////////////////////////////////////
// This file is part of the Raven Game Engine			                    //
//////////////////////////////////////////////////////////////////////////////

#pragma once
#include <string>
#include <vector>
#include <memory>
#include "Scene/Component/Component.h"
#include "Scene/Entity/Entity.h"

#include <cereal/cereal.hpp>
#include "AnimationController.h"

namespace Raven
{
	class Animation;

	class Animator : public Component
	{
	public:
		friend class AnimationSystem;
		void OnImGui();

		template<typename Archive>
		void save(Archive& archive) const
		{
			archive(cereal::make_nvp("controllerFile", animFile));
			archive(cereal::make_nvp("rootMotion", rootMotion));
		}

		template<typename Archive>
		void load(Archive& archive)
		{
			archive(cereal::make_nvp("controllerFile", animFile));
			archive(cereal::make_nvp("rootMotion", rootMotion));
			CreateController();
		}

		template<typename T>
		void SetValue(const std::string& name, const T& value);

		inline auto GetController() { return controller; }

	private:
		void CreateController();
		bool rootMotion = false;
		std::string animFile;
		std::shared_ptr<AnimationController> controller;

	public:
		bool isSimpleAnimator = false;
		Animation* anime = nullptr;
	};

	template<>
	inline void Raven::Animator::SetValue(const std::string& name, const std::string& value)
	{
		if (controller != nullptr) 
		{
			controller->SetValue(name, value);
		}
	}

	template<typename T>
	inline void Raven::Animator::SetValue(const std::string& name, const T& value)
	{
		if (controller != nullptr)
		{
			controller->SetValue(name, std::to_string(value));
		}
	}

};
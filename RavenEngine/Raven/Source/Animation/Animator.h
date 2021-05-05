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
			archive(cereal::make_nvp("rootMotion", rootMotion));

			// Save Resrouce Reference -> AnimationController.
			ResourceRef::Save(archive, controller.get());
		}

		template<typename Archive>
		void load(Archive& archive)
		{
			archive(cereal::make_nvp("rootMotion", rootMotion));

			// Load Resrouce Reference -> AnimationController.
			controller = ResourceRef::Load(archive).FindOrLoad<AnimationController>();
		}

		template<typename T>
		void SetValue(const std::string& name, const T& value);

		inline auto GetController() { return controller; }

	private:
		bool rootMotion = false;

		// The Animation Controller, its a resource so loading/saving is handled by the Resource Manager.
		Ptr<AnimationController> controller;
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
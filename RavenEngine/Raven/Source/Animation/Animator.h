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
			ResourceRef::Save(archive, controllerInstance ? controllerInstance->GetParentController().get() : nullptr);
		}

		template<typename Archive>
		void load(Archive& archive)
		{
			archive(cereal::make_nvp("rootMotion", rootMotion));

			// Load Resrouce Reference -> AnimationController.
			Ptr<AnimationController> controller = ResourceRef::Load(archive).FindOrLoad<AnimationController>();

			// Create Instance from resrouce...
			if (controller)
			{
				controllerInstance = Ptr<AnimationControllerInstance>(new AnimationControllerInstance(controller));
			}
		}

		template<typename T>
		void SetValue(const std::string& name, const T& value);

		inline auto GetController() { return controllerInstance; }

	private:
		bool rootMotion = false;

		// The Animation Controller, its a resource so loading/saving is handled by the Resource Manager.
		Ptr<AnimationControllerInstance> controllerInstance;
	};

	template<>
	inline void Raven::Animator::SetValue(const std::string& name, const std::string& value)
	{
		if (controllerInstance != nullptr)
		{
			controllerInstance->Get()->SetValue(name, value);
		}
	}

	template<typename T>
	inline void Raven::Animator::SetValue(const std::string& name, const T& value)
	{
		if (controllerInstance != nullptr)
		{
			controllerInstance->Get()->SetValue(name, std::to_string(value));
		}
	}

};
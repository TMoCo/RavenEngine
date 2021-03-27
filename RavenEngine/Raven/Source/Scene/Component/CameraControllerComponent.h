//////////////////////////////////////////////////////////////////////////////
// This file is part of the Raven Game Engine			                    //
//////////////////////////////////////////////////////////////////////////////

#pragma once
#include <string>
#include <memory>

namespace Raven
{
	class CameraController;

	class CameraControllerComponent
	{
	public:
		enum class ControllerType : int32_t
		{
			FPS = 0,
			EditorCamera,
			Custom
		};

		static std::string TypeToString(ControllerType type);
		static ControllerType StringToType(const std::string& type);

		CameraControllerComponent()
			: type(ControllerType::Custom)
		{
		}

		CameraControllerComponent(ControllerType type);
	
		void SetControllerType(CameraControllerComponent::ControllerType type);

		inline auto & GetController() {return cameraController;}

		template<typename Archive>
		void save(Archive& archive) const
		{
			archive(cereal::make_nvp("ControllerType", type));
		}

		template<typename Archive>
		void load(Archive& archive)
		{
			archive(cereal::make_nvp("ControllerType",type));
			SetControllerType(type);
		}

		inline auto GetType() const 
		{
			return type;
		}

	private:
		ControllerType type = ControllerType::Custom;
		std::shared_ptr<CameraController> cameraController;
	};
};
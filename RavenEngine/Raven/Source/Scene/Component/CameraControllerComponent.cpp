//////////////////////////////////////////////////////////////////////////////
// This file is part of the Raven Game Engine			                    //
//////////////////////////////////////////////////////////////////////////////
#include "CameraControllerComponent.h"
#include "Core/CameraController.h"
#include "Core/EditorCamera.h"
#include "Logger/Console.h"

namespace Raven 
{
	CameraControllerComponent::ControllerType CameraControllerComponent::StringToType(const std::string& type)
	{
		if (type == "FPS")
			return ControllerType::FPS;
		if (type == "Editor")
			return ControllerType::EditorCamera;
		return ControllerType::Custom;
	}

	CameraControllerComponent::CameraControllerComponent(ControllerType type)
	{
		SetControllerType(type);
	}

	void CameraControllerComponent::SetControllerType(CameraControllerComponent::ControllerType type)
	{
		this->type = type;
		switch (type)
		{
		case ControllerType::FPS:
			LOGW("{0} does not implement", __FUNCTION__);
			break;
		case ControllerType::EditorCamera:
			cameraController = std::make_shared<EditorCameraController>();
			break;
		}
	}

	std::string CameraControllerComponent::TypeToString(ControllerType type)
	{
		switch (type)
		{
		case ControllerType::FPS:
			return "FPS";
		case ControllerType::EditorCamera:
			return "Editor";
		}
		return "Custom";
	}

};


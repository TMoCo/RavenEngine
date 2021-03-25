#pragma once
#include "CameraController.h"

namespace Raven
{
	class EditorCameraController : public CameraController
	{
	public:
		EditorCameraController();
		~EditorCameraController();

		void HandleMouse(Transform& transform, float dt, float xpos, float ypos) override;
		void HandleKeyboard(Transform& transform, float dt) override;
        void UpdateScroll(Transform& transform, float offset, float dt) override;

    };
}

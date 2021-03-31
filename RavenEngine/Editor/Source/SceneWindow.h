//////////////////////////////////////////////////////////////////////////////
// This file is part of the Raven Game Engine			                    //

//////////////////////////////////////////////////////////////////////////////

#pragma  once
#include <string>
#include <imgui.h>
#include <memory>

#include "EditorWindow.h"

namespace Raven 
{

	class Scene;

	class SceneWindow : public EditorWindow
	{
	public:
		SceneWindow();
		virtual void OnImGui()  ;
		virtual void Resize(uint32_t width, uint32_t height);
		void DrawGizmos(float width, float height, float xpos, float ypos, Scene* scene);
	private:
		void DrawToolBar();
		uint32_t width;
		uint32_t height;
		bool showCamera = false;
	};
};
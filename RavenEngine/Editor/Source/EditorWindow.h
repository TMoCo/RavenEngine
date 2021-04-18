//////////////////////////////////////////////////////////////////////////////
// This file is part of the Raven Game Engine			                    //

//////////////////////////////////////////////////////////////////////////////

#pragma  once
#include <string>

namespace Raven 
{
	class Scene;

	class EditorWindow 
	{
	public:
		virtual ~EditorWindow() = default;
		virtual void OnImGui()  = 0;
		virtual void OnSceneCreated(Scene* scene) {}
		virtual void SaveWorkspace() {};
	public:
        inline auto& GetTitle() const { return title; }
        inline auto IsActive() const { return active; }
        inline auto SetActive(bool active) { this->active = active; }
	protected:
		std::string title;
		bool active = false;
	};
};
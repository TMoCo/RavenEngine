//////////////////////////////////////////////////////////////////////////////
// This file is part of the Raven Game Engine			                    //
//////////////////////////////////////////////////////////////////////////////


namespace Raven 
{
	class Scene;
	class ISystem
	{
	public:
		ISystem() = default;
		virtual ~ISystem() = default;
		virtual void OnInit() = 0;
		virtual void OnUpdate(float dt, Scene* scene) = 0;
		virtual void OnImGui() = 0;
	};
};
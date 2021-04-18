#pragma once




#include "glm/vec3.hpp"
#include "glm/vec4.hpp"



namespace Raven
{
	class RenderPrimitive;


	// RenderLight:
	//		- 
	class RenderLight
	{
		// Friend...
		friend class RenderScene;

	public:
		// Construct. 
		RenderLight()
			: castShadow(false)
			, type(0)
			, indexInScene(-1)
		{

		}

		// Destruct. 
		~RenderLight()
		{

		}

		// Return the type of the light.
		inline int32_t GetType() const { return type; }

	public:
		// General Light Data.
		float radius;
		float innerAngle;
		float outerAngle;
		glm::vec3 postion;
		glm::vec3 dir;
		glm::vec4 colorAndPower;

		// Does this light cast shadow.
		bool castShadow;

		// If this light cast 
		std::vector<RenderPrimitive*> casters;

	private:
		// Set by RenderScene, The Light Type.
		int32_t type;

		// Used by RenderScene, the index of this primitive in the render scene.
		int32_t indexInScene;
	};


}



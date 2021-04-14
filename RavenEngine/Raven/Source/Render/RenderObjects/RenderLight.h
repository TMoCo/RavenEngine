#pragma once




#include "glm/vec3.hpp"
#include "glm/vec4.hpp"



namespace Raven
{
	// RenderLight:
	//		- 
	//		
	class RenderLight
	{
	public:
		// Construct. 
		RenderLight()
		{

		}

		// Destruct. 
		~RenderLight()
		{

		}

		//
		int32_t type;
		float radius;
		float innerAngle;
		float outerAngle;
		glm::vec3 postion;
		glm::vec3 dir;
		glm::vec4 colorAndPower;
	};


}



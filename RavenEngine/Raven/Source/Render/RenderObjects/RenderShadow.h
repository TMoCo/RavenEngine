#pragma once




#include "Math/Frustum.h"


#include "glm/vec3.hpp"
#include "glm/vec4.hpp"
#include "glm/matrix.hpp"


#include <vector>



namespace Raven
{
	//
	struct ShadowCascadeData
	{
		//
		MathUtils::Frustum frustum;

		//
		glm::mat4	viewProj;

		//
	};



	// RenderShadowCascaded:
	//		- Shadow for sun directional light with cascading.
	class RenderShadowCascade
	{
	public:
		// Construct. 
		RenderShadowCascade(int32_t cascadeCount);

		// Destruct. 
		~RenderShadowCascade();

		//
		void ComputeCascading(const glm::vec3& lightDir, const MathUtils::Frustum& viewFrustum);


	private:
		//
		std::vector<ShadowCascadeData> cascade;

		//
		glm::vec3 lightDir;

	};


}



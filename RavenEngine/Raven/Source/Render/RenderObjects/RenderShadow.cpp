#include "RenderShadow.h"


namespace Raven {




// --- -- --- -- --- -- --- -- --- -- --- -- --- -- --- -- --- -- --- -- 



RenderShadowCascade::RenderShadowCascade(int32_t cascadeCount)
{
	cascade.resize(cascadeCount);

}


RenderShadowCascade::~RenderShadowCascade()
{

}


void RenderShadowCascade::ComputeCascading(const glm::vec3& lightDir, const MathUtils::Frustum& viewFrustum)
{

}



// --- -- --- -- --- -- --- -- --- -- --- -- --- -- --- -- --- -- --- -- 






} // End of namespace Raven.

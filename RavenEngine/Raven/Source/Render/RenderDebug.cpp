#include "RenderDebug.h"
#include "RenderResource/RenderRscDebugMesh.h"
#include "RenderResource/RenderRscShader.h"
#include "RenderResource/RenderRscMaterial.h"
#include "RenderObjects/RenderDebugPrimitive.h"


#include "glm/vec3.hpp"
#include "glm/gtx/transform.hpp"



#define RENDER_RSC_BOX 0
#define RENDER_RSC_SPHERE 1
#define RENDER_RSC_LINES 2



using namespace Raven;





RenderDebugDraw::~RenderDebugDraw()
{
	delete primitive;
}





// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --




RenderDebug::RenderDebug()
	: debugShader(nullptr)
{

}


RenderDebug::~RenderDebug()
{
	debugDrawList.clear();
}


void RenderDebug::Setup()
{
	//
	debugShader = new RenderRscShader();
	debugShader->Load(ERenderShaderType::Debug, "DrawDebugShader");

	//
	debugMaterial = new RenderRscMaterial(debugShader);

	//
	RenderRscDebugMesh* rscMesh = new RenderRscDebugMesh();
	rscMesh->CreateBox();
	resources[RENDER_RSC_BOX] = rscMesh;

	resources[RENDER_RSC_SPHERE] = nullptr;
	resources[RENDER_RSC_LINES] = nullptr;

}


void RenderDebug::Destroy()
{
	// ...
	delete debugShader;
	delete debugMaterial;

	// Delete debug resoruces...
	for (int i = 0; i < resources.size(); ++i)
	{
		delete resources[i];
	}

	debugDrawList.clear();

}


void RenderDebug::Update(float dt)
{
	debugPrimitives.clear();
	uint32_t i = 0;
	std::list<RenderDebugDraw>::iterator iter = debugDrawList.begin();

	while (iter != debugDrawList.end())
	{
		auto& db = *iter;

		// Has been drawn? make sure you draw it at least once.
		if (db.isDrawn)
		{
			// Expired?
			if (db.time < 0.0)
			{
				// Remove...
				debugDrawList.erase(iter);

				// Recalculate Iterator + Index.
				iter = debugDrawList.begin();
				std::advance(iter, i);
				continue;
			}
		}

		db.time -= dt;
		db.isDrawn = true;
		debugPrimitives.push_back(db.primitive);
		++i;
		++iter;
	}

}


const std::vector<RenderPrimitive*>& RenderDebug::GetRenderPrimitive() const
{
	return debugPrimitives;
}


void RenderDebug::DrawBox(const glm::vec3& position, const glm::vec3& scale, glm::vec4 color, float time)
{
	//
	glm::mat4 mtx;
	mtx = glm::translate(position) * glm::scale(scale);

	// New Render Primitive.
	RenderDebugPrimitive* prim = new RenderDebugPrimitive();
	prim->SetColor(color);
	prim->SetWorldMatrix(mtx);
	prim->SetMaterial(debugMaterial);
	prim->SetDebugMesh( resources[RENDER_RSC_BOX] );


	//
	debugDrawList.push_back( RenderDebugDraw() );
	RenderDebugDraw& elem = debugDrawList.back();
	elem.primitive = prim;
	elem.time = time;
}

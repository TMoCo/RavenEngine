#include "RenderScene.h"

#include "RenderPrimitive.h"
#include "RenderTerrain.h"
#include "Render/RenderResource/RenderRscMaterial.h"
#include "Render/RenderResource/RenderRscShader.h"

#include "Render/OpenGL/GLShader.h"

#include "GL/glew.h"



using namespace Raven;




RenderScene::RenderScene()
{

}


RenderScene::~RenderScene()
{

}


void RenderScene::Build(RenderTerrain* terrain)
{
	GetBatch(ERSceneBatch::Opaque).Add(terrain);


	//
	TraverseScene();

}


void RenderScene::SetView(const glm::mat4& mtx)
{
	view = mtx;
}


void RenderScene::SetProjection(const glm::mat4& mtx)
{
	projection = mtx;
}


void RenderScene::AddDebugPrimitives(const std::vector<RenderPrimitive*>& primitives)
{
	RenderBatch& batch = GetBatch(ERSceneBatch::Debug);

	for (auto prim : primitives)
	{
		batch.Add(prim);
	}

}


void RenderScene::TraverseScene()
{

}


void RenderScene::Clear()
{
	for (auto& batch : batches)
	{
		batch.Clear();
	}

}


void RenderScene::Draw(ERSceneBatch type)
{
	const RenderBatch& batch = GetBatch(type);

	for (auto& prim : batch.elements)
	{
		GLShader* shader = prim->GetMaterial()->GetShaderRsc()->GetShader();
		shader->Use();
		shader->SetUniform("proj", projection);
		shader->SetUniform("view", view);
		shader->SetUniform("model", prim->GetWorldMatrix());

		prim->Draw(shader);
	}

	//
	glBindVertexArray(0);
}



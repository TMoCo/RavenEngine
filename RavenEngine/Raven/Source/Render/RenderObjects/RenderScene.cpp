#include "RenderScene.h"

#include "RenderPrimitive.h"
#include "RenderTerrain.h"
#include "Render/RenderResource/RenderRscMaterial.h"
#include "Render/RenderResource/RenderRscShader.h"

#include "Render/OpenGL/GLShader.h"

#include "Scene/Scene.h"
#include "Scene/Component/Model.h"
#include "Scene/Component/Transform.h"
#include <entt/entt.hpp>

#include "GL/glew.h"



using namespace Raven;




RenderScene::RenderScene()
{

}


RenderScene::~RenderScene()
{

}


void RenderScene::Build(RenderTerrain* terrain, Scene* scene)
{
	GetBatch(ERSceneBatch::Opaque).Add(terrain);

	//
	TraverseScene(scene);
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


void RenderScene::TraverseScene(Scene* scene)
{

}


void RenderScene::Clear()
{
	for (auto& batch : batches)
	{
		batch.Clear();
	}

	for (auto& prim : dynamicPrimitive)
	{
		delete prim;
	}

	dynamicPrimitive.clear();
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



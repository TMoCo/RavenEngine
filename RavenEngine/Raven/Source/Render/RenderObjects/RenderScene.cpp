#include "RenderScene.h"

#include "RenderPrimitive.h"
#include "RenderTerrain.h"
#include "Render/RenderResource/RenderRscMaterial.h"
#include "Render/RenderResource/RenderRscShader.h"

#include "Render/OpenGL/GLShader.h"
#include "Render/OpenGL/GLBuffer.h"

#include "Scene/Scene.h"
#include "Scene/Component/Transform.h"
#include "Scene/Component/Light.h"
#include "Core/Camera.h"
#include "Scene/Component/Model.h"
#include "Scene/Component/Transform.h"
#include "Scene/Entity/EntityManager.h"
#include <entt/entt.hpp>

#include "GL/glew.h"

#include "Render/RenderResource/RenderRscDebugMesh.h"
#include "Render/RenderObjects/RenderDebugPrimitive.h"



#include "glm/gtc/type_ptr.hpp"




namespace Raven {

// ~Testing .............................................................
RenderRscShader* debugShader;
RenderRscMaterial* debugMaterial;
RenderRscDebugMesh* debugRscMesh;

struct TransformUBO
{
	glm::mat4 modelMatrix;
	glm::mat4 viewMatrix;
	glm::mat4 projectionMatrix;
};


struct LightingUBO
{
	// View
	glm::vec4 viewDir;
	glm::vec4 viewPos;

	// Dir-Light
	glm::vec4 lightDir;
	glm::vec4 lightColor;
	float lightPower;
};


struct MaterialUBO
{
	glm::vec4 diffuse;
	glm::vec4 specular;
	glm::vec4 emission;
	float shininess;
	float ambient;
	float alpha;
};


TransformUBO trData;
TransformUBO lightData;
TransformUBO matData;

// ~Testing .............................................................



RenderScene::RenderScene()
	: trUBO(nullptr)
	, lightingUBO(nullptr)
{

}


RenderScene::~RenderScene()
{
	delete trUBO;
	delete debugRscMesh;
	delete debugMaterial;
	delete debugShader;
}


void RenderScene::Setup()
{
	// ~Testing.....................................................
	if (!debugShader)
	{
		debugShader = new RenderRscShader();
		debugShader->Load(ERenderShaderType::Debug, "DrawDebugShader");

		//
		debugMaterial = new RenderRscMaterial(debugShader);

		//
		debugRscMesh = new RenderRscDebugMesh();
		debugRscMesh->CreateBox();
	}
	// ~Testing.....................................................



	//
	trUBO = GLBuffer::Create(EGLBufferType::Uniform, sizeof(TransformUBO), EGLBufferUsage::DynamicDraw);
	trUBO->BindBase(ShaderInput::TRANSFORM_BINDING);

	lightingUBO = GLBuffer::Create(EGLBufferType::Uniform, sizeof(LightingUBO), EGLBufferUsage::DynamicDraw);
	lightingUBO->BindBase(ShaderInput::LGHTING_BINDING);

	materialUBO = GLBuffer::Create(EGLBufferType::Uniform, sizeof(MaterialUBO), EGLBufferUsage::DynamicDraw);
	materialUBO->BindBase(2);


}


void RenderScene::Build(Scene* scene)
{
	


	// View & Projection...
	auto camsEttView = scene->getEntityManager()->GetEntitiesWithType<Camera>();
	if (!camsEttView.Empty())
	{
		Camera& sceneCam = camsEttView[0].GetComponent<Camera>();
		Transform& sceneCamTr = camsEttView[0].GetComponent<Transform>();

		SetProjection(sceneCam.GetProjectionMatrix());
		SetView(glm::inverse(sceneCamTr.GetWorldMatrix()));
	}


	// Lights...
	auto lightsEttView = scene->GetRegistry().group<Light>(entt::get<Transform>);


	for (auto entity : lightsEttView)
	{
		const auto& [light, trans] = lightsEttView.get<Light, Transform>(entity);
		if (light.type != (int32_t)LightType::DirectionalLight)
			continue;
	}


/*

	if (!lightsEttView.Empty())
	{
		Light* dirLight = nullptr;

		// Find the first Directional Light.
		for (auto& light : lightsEttView)
		{
			Light& sceneLight = camsEttView[0].GetComponent<Light>();

			if (sceneLight.type != (int32_t)LightType::DirectionalLight)
				continue;
		}

		// Light Data...
	}*/

	// Traverse the scene to collected render primitives.
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
	// Get All transforms in the scene
	// TODO: Traverse on Models while drawing building render scene.
	auto trEnttView = scene->getEntityManager()->GetEntitiesWithType<Transform>();

	// No Transforms?
	if (trEnttView.Empty())
		return;

	

/*
* 
*   a demo
	auto group = scene->GetRegistry().group<Model>(entt::get<Transform>);

	for (auto entity : group)
	{
		const auto& [model, trans] = group.get<Model, Transform>(entity);
	}
*/


	// Clear Debug just for testing...
	GetBatch(ERSceneBatch::Debug).Clear();

	for (auto& tr : trEnttView)
	{
		Transform& sceneCamTr = tr.GetComponent<Transform>();

		RenderDebugPrimitive* trPrim = NewPrimitive<RenderDebugPrimitive>();
		trPrim->SetWorldMatrix(sceneCamTr.GetWorldMatrix());
		trPrim->SetMaterial(debugMaterial);
		trPrim->SetDebugMesh(debugRscMesh);
		trPrim->SetColor(glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));

		GetBatch(ERSceneBatch::Opaque).Add(trPrim);
	}


}


void RenderScene::Clear()
{
	// Clear Render Batches...
	for (auto& batch : batches)
	{
		batch.Clear();
	}

	// Clear/Delete Dynamic Render Primitives created while building render scene...
	for (auto& prim : dynamicPrimitive)
		delete prim;

	dynamicPrimitive.clear();


}


void RenderScene::Draw(ERSceneBatch type)
{
	const RenderBatch& batch = GetBatch(type);

	trData.modelMatrix = glm::mat4(1.0f);
	trData.viewMatrix = view;
	trData.projectionMatrix = projection;
	trUBO->UpdateSubData(sizeof(TransformUBO), 0, &trData);

	lightingUBO->UpdateSubData(sizeof(LightingUBO), 0, &lightData);
	materialUBO->UpdateSubData(sizeof(MaterialUBO), 0, &matData);


	for (auto& prim : batch.elements)
	{
		GLShader* shader = prim->GetMaterial()->GetShaderRsc()->GetShader();

		// Model Matrix...
		trUBO->UpdateSubData(
			sizeof(glm::mat4), 
			offsetof(TransformUBO, modelMatrix), 
			glm::value_ptr(prim->GetWorldMatrix())
		);

		shader->Use();
		prim->Draw(shader);
	}

	//
	glBindVertexArray(0);
}





} // End of namespace Raven.

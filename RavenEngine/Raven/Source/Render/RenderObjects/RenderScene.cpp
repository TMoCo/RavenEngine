#include "RenderScene.h"

#include "Engine.h"
#include "RenderPrimitive.h"
#include "RenderTerrain.h"
#include "Render/RenderResource/RenderRscMaterial.h"
#include "Render/RenderResource/RenderRscShader.h"
#include "Render/RenderResource/RenderRscTerrain.h"
#include "Render/OpenGL/GLShader.h"
#include "Render/OpenGL/GLBuffer.h"
#include "RenderDebugPrimitive.h"
#include "RenderPrimitive.h"
#include "RenderMesh.h"
#include "RenderTerrain.h"

#include "Core/Camera.h"
#include "Scene/Scene.h"
#include "Scene/Component/Transform.h"
#include "Scene/Component/Light.h"
#include "Scene/Component/Model.h"
#include "Scene/Component/Transform.h"
#include "Scene/Component/TerrainComponent.h"
#include "Scene/Entity/EntityManager.h"
#include <entt/entt.hpp>

#include "Scene/Component/Model.h"

#include "GL/glew.h"
#include "glm/gtc/type_ptr.hpp"

#include "ResourceManager/Resources/Mesh.h"


namespace Raven {




// ~MinimalSolution --- ---- --- ---- --- ---- ---
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
LightingUBO lightData;
MaterialUBO matData;
// ~MinimalSolution --- ---- --- ---- --- ---- ---





RenderScene::RenderScene()
	: trUBO(nullptr)
	, lightingUBO(nullptr)
{

}


RenderScene::~RenderScene()
{
	delete trUBO;
	delete lightingUBO;
	delete materialUBO;
	delete defaultShader;
	delete terrainMaterail;
	delete terrainShader;
	delete defaultMaterail;
}


void RenderScene::Setup()
{
	defaultShader = new RenderRscShader();
	defaultShader->Load(ERenderShaderType::MaterialOpaque, "Default_Materail");
	defaultMaterail = new RenderRscMaterial(defaultShader);

	terrainShader = new RenderRscShader();
	terrainShader->Load(ERenderShaderType::Terrain, "Default_Terrain");
	terrainMaterail = new RenderRscMaterial(terrainShader);


	// ~MinimalSolution --- ---- --- ---- --- ---- ---
	trUBO = GLBuffer::Create(EGLBufferType::Uniform, sizeof(TransformUBO), EGLBufferUsage::DynamicDraw);
	trUBO->BindBase(ShaderInput::TRANSFORM_BINDING);

	lightingUBO = GLBuffer::Create(EGLBufferType::Uniform, sizeof(LightingUBO), EGLBufferUsage::DynamicDraw);
	lightingUBO->BindBase(ShaderInput::LGHTING_BINDING);

	materialUBO = GLBuffer::Create(EGLBufferType::Uniform, sizeof(MaterialUBO), EGLBufferUsage::DynamicDraw);
	materialUBO->BindBase(2);

	matData.diffuse = glm::vec4(0.7f);
	matData.ambient = 0.05f;
	matData.specular = glm::vec4(1.0f);
	matData.shininess = 32.0f;
	matData.emission = glm::vec4(0.0f);
	matData.alpha = 1.0f;
	// ~MinimalSolution --- ---- --- ---- --- ---- ---


}


void RenderScene::Build(Scene* scene)
{
	// View & Projection...
	glm::mat4 camTr;

	// Has Camera? 
	if (scene->GetTargetCamera())
	{
		SetProjection(scene->GetTargetCamera()->GetProjectionMatrix());
		camTr = scene->GetCameraTransform()->GetWorldMatrix();
		SetView(glm::inverse(camTr));
	}
	else
	{
		camTr = glm::inverse(view);
	}

	lightData.viewDir = glm::normalize(camTr  * glm::vec4(Raven::FORWARD, 0.0f));
	lightData.viewPos = camTr * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);


	// Lights...
	auto lightsEttView = scene->GetRegistry().group<Light>(entt::get<Transform>);

	for (auto entity : lightsEttView)
	{
		const auto& [light, trans] = lightsEttView.get<Light, Transform>(entity);
		if (light.type != (int32_t)LightType::DirectionalLight)
			continue;

		lightData.lightDir = glm::normalize(glm::vec4(light.direction, 0.0f));
		lightData.lightColor = light.color;
		lightData.lightPower = light.intensity;
	}

	// Terrain...
	auto TerrainEttView = scene->GetRegistry().view<TerrainComponent>();

	if (!TerrainEttView.empty())
	{
		auto entity = TerrainEttView.front();
		const auto& theTerrain = TerrainEttView.get<TerrainComponent>(entity);
		auto terrainRsc = theTerrain.GetTerrainResource();

		if (!terrainRsc->IsOnGPU())
		{
			terrainRsc->renderRscTerrain = new RenderRscTerrain();
			terrainRsc->LoadOnGPU();
		}

		RenderTerrain* renderTerrain = NewPrimitive<RenderTerrain>();
		renderTerrain->SetTerrainRsc(terrainRsc->renderRscTerrain);
		renderTerrain->SetWorldMatrix(glm::mat4(1.0f));
		renderTerrain->SetMaterial(terrainMaterail);
		GetBatch(ERSceneBatch::Opaque).Add(renderTerrain);
	}

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
	// Get All models in the scene
	auto group = scene->GetRegistry().group<Model>(entt::get<Transform>);
	if (group.empty())
		return; 

	// ~Testing---------------------------------------
	// Clear Debug just for testing...
	GetBatch(ERSceneBatch::Debug).Clear();
	// ~Testing---------------------------------------


	// Iterate over all Models in the scene.
	// TODO: Culling should be here, for both the view and lights shadow frustums.
	for (auto entity : group)
	{
		const auto& [model, trans] = group.get<Model, Transform>(entity);
		const auto& meshes = model.GetMeshes();

		// Create a RenderPrimitive for each model, and add it to the correct batch.
		for (uint32_t i = 0; i < meshes.size(); ++i)
		{
			const auto& mesh = meshes[i];

			// Update Mesh on GPU if not loaded yet.
			if (!mesh->IsOnGPU())
			{
				mesh->renderRscMesh = new RenderRscMesh();
				mesh->LoadOnGpu();
			}

			RenderMesh* rmesh = NewPrimitive<RenderMesh>();
			rmesh->SetWorldMatrix(trans.GetWorldMatrix());
			rmesh->SetMesh(mesh->renderRscMesh);

			// TODO: Render with Model Materials...
			rmesh->SetMaterial(defaultMaterail);

			// Add to opaque...
			GetBatch(ERSceneBatch::Opaque).Add(rmesh);
		}
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

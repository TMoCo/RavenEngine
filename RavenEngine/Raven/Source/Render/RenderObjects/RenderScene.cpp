#include "RenderScene.h"

#include "Engine.h"
#include "Primitives/RenderPrimitive.h"
#include "Primitives/RenderTerrain.h"
#include "Primitives/RenderDebugPrimitive.h"
#include "Primitives/RenderPrimitive.h"
#include "Primitives/RenderMesh.h"
#include "Primitives/RenderTerrain.h"
#include "Render/RenderResource/Shader/RenderRscShader.h" 
#include "Render/RenderResource/Shader/RenderRscMaterial.h"
#include "Render/RenderResource/Shader/UniformBuffer.h"


#include "Render/OpenGL/GLBuffer.h"
#include "Render/OpenGL/GLShader.h"

#include "ResourceManager/Resources/Mesh.h"
#include "ResourceManager/Resources/Material.h"


#include "Core/Camera.h"
#include "Scene/Scene.h"
#include "Scene/Component/Transform.h"
#include "Scene/Component/Light.h"
#include "Scene/Component/Model.h"
#include "Scene/Component/Transform.h"
#include "Scene/Component/TerrainComponent.h"
#include "Scene/Entity/EntityManager.h"
#include <entt/entt.hpp>


#include "GL/glew.h"
#include "glm/gtc/type_ptr.hpp"



namespace Raven {




// ~MinimalSolution --- ---- --- ---- --- ---- ---
struct TransformUBO
{
	glm::mat4 modelMatrix;
	glm::mat4 normalMatrix;
	glm::mat4 viewProjMatrix;
} trData;


struct LightingUBO
{
	// Dir-Light
	glm::vec4 lightDir;
	glm::vec4 lightColor;
	float lightPower;
} lightData;


// ~MinimalSolution --- ---- --- ---- --- ---- ---





RenderScene::RenderScene()
	: trUBO(nullptr)
	, lightingUBO(nullptr)
	, commonUBO(nullptr)
	, time(0.0f)
{

}


RenderScene::~RenderScene()
{
	delete trUBO;
	delete lightingUBO;
	delete commonUBO;
	delete defaultShader;
	delete terrainMaterail;
	delete terrainShader;
	delete defaultMaterail;
}


void RenderScene::Setup()
{
	// Default Mesh
	{
		RenderRscShaderCreateData shaderData;
		shaderData.type = ERenderShaderType::Opaque;
		shaderData.name = "Default_Material";

		// Add Default Materail Function
		shaderData.AddFunction(EGLShaderStageBit::FragmentBit, "shaders/Materials/DefaultMaterial.glsl");

		defaultShader = RenderRscShader::Create(ERenderShaderDomain::Mesh, shaderData);
		defaultShader->BindBlockInputs();
		defaultShader->BindSamplers();

		defaultMaterail = new RenderRscMaterial(defaultShader);
	}

	{
		RenderRscShaderCreateData shaderData;
		shaderData.type = ERenderShaderType::Opaque;
		shaderData.name = "Terrain_Material";

		// Add Terrain Materail Function

		terrainShader = RenderRscShader::Create(ERenderShaderDomain::Terrain, shaderData);
		terrainShader->BindBlockInputs();
		terrainShader->BindSamplers();

		terrainMaterail = new RenderRscMaterial(terrainShader);
	}


	// ~MinimalSolution --- ---- --- ---- --- ---- ---
	commonUBO = UniformBuffer::Create(RenderShaderInput::CommonBlock);
	trUBO = UniformBuffer::Create(RenderShaderInput::TransfromBlock);
	lightingUBO = UniformBuffer::Create(RenderShaderInput::LightingBlock);
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

	viewDir = glm::normalize(camTr  * glm::vec4(Raven::FORWARD, 0.0f));
	viewPos = camTr * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);

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
	// TODO: Culling should be here, for both the view, lights, and shadow frustums.
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

			// Mesh Materail...
			Material* meshMaterail = model.GetMaterial(i);

			if (meshMaterail)
			{
				// Update Material Paramters if Dirty.
				if (meshMaterail->IsDirty())
				{
					meshMaterail->Update();
				}

				rmesh->SetMaterial( meshMaterail->GetRenderRsc() );
			}
			else
			{
				rmesh->SetMaterial( defaultMaterail );
			}

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

	{
		// Testing the Block..................................
		int32_t inputIdx = commonUBO->GetInputIndex("viewDir");
		commonUBO->UpdateData(
			RenderShaderInput::GetSize(commonUBO->GetInput(inputIdx).inputType),
			commonUBO->GetInputOffset(inputIdx),
			glm::value_ptr(viewDir)
		);

		inputIdx = commonUBO->GetInputIndex("viewPos");
		commonUBO->UpdateData(
			RenderShaderInput::GetSize(commonUBO->GetInput(inputIdx).inputType),
			commonUBO->GetInputOffset(inputIdx),
			glm::value_ptr(viewPos)
		);

		inputIdx = commonUBO->GetInputIndex("time");
		commonUBO->UpdateData(
			RenderShaderInput::GetSize(commonUBO->GetInput(inputIdx).inputType),
			commonUBO->GetInputOffset(inputIdx),
			&time
		);

		commonUBO->BindBase();
	}



	trData.modelMatrix = glm::mat4(1.0f);
	trData.viewProjMatrix = projection * view;
	trUBO->UpdateData(sizeof(TransformUBO), 0, &trData);
	trUBO->BindBase();

	lightingUBO->UpdateData(sizeof(LightingUBO), 0, &lightData);
	lightingUBO->BindBase();


	for (auto& prim : batch.elements)
	{
		// Transfromation...
		trData.modelMatrix = prim->GetWorldMatrix();
		trData.normalMatrix = prim->GetWorldMatrix();
		trUBO->UpdateData( sizeof(glm::mat4) * 2, 0, &trData); // Model & Normal Matrix.

		// Materail...
		RenderRscMaterial* material = prim->GetMaterial();

		if (material->HasMaterialData())
		{
			material->GetUniformBuffer()->BindBase();
			material->UpdateUniformBuffer();
		}

		GLShader* shader = prim->GetMaterial()->GetShaderRsc()->GetShader();
		shader->Use();
		prim->Draw(shader);
	}

	//
	glBindVertexArray(0);
}





} // End of namespace Raven.

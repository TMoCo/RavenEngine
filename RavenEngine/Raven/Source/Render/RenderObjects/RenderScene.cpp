#include "RenderScene.h"

#include "Engine.h"
#include "Render/RenderModule.h"

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



// Data reflect TransformVertex Uniform Block.
struct TransformVertexData
{
	glm::mat4 modelMatrix;
	glm::mat4 normalMatrix;
} trData;







RenderScene::RenderScene()
	: near(0.0f)
	, far(0.0f)
{

}


RenderScene::~RenderScene()
{

}


void RenderScene::Setup()
{
	// Transfrom Vertex Uniform 
	transformUB = Ptr<UniformBuffer>( UniformBuffer::Create(RenderShaderInput::TransfromBlock, false) );

}


void RenderScene::Build(Scene* scene)
{
	// View & Projection...
	CollectSceneView(scene);

	// Lights...
	CollectSceneLights(scene);

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

		// Default Terrain Material.
		const auto& defaultMaterials = Engine::GetModule<RenderModule>()->GetDefaultMaterials();
		renderTerrain->SetMaterial( defaultMaterials.terrain->GetRenderRsc() );

		GetBatch(ERSceneBatch::Opaque).Add(renderTerrain);
	}

	// Traverse the scene to collected render primitives.
	TraverseScene(scene);
}


void RenderScene::SetView(const glm::mat4& mtx)
{
	view = mtx;
}


void RenderScene::SetProjection(const glm::mat4& mtx, float n, float f)
{
	projection = mtx;
	near = n;
	far = f;
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
	// Default Materials.
	const auto& defaultMaterials = Engine::GetModule<RenderModule>()->GetDefaultMaterials();


	// Get All models in the scene
	auto group = scene->GetRegistry().group<Model>(entt::get<Transform>);
	if (group.empty())
		return; 


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

			if (meshMaterail && 0)
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
				rmesh->SetMaterial( defaultMaterials.model->GetRenderRsc() );
			}

			// Add to opaque...
			GetBatch(ERSceneBatch::Opaque).Add(rmesh);
		}
	}
}


void RenderScene::CollectSceneView(Scene* scene)
{
	glm::mat4 camTr;

	// Has Camera? 
	if (scene->GetTargetCamera())
	{
		const auto& targetCam = scene->GetTargetCamera();
		SetProjection(targetCam->GetProjectionMatrix(), targetCam->GetNear(), targetCam->GetFar());
		camTr = scene->GetCameraTransform()->GetWorldMatrix();
		SetView(glm::inverse(camTr));
	}
	else
	{
		camTr = glm::inverse(view);
	}

	viewDir = glm::normalize(camTr * glm::vec4(Raven::FORWARD, 0.0f));
	viewPos = camTr * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);

	viewProjMatrix = projection * view;
	viewProjMatrixInverse = glm::inverse(viewProjMatrix);

}


void RenderScene::CollectSceneLights(Scene* scene)
{
	auto lightsEttView = scene->GetRegistry().group<Light>(entt::get<Transform>);

	for (auto entity : lightsEttView)
	{
		const auto& [light, trans] = lightsEttView.get<Light, Transform>(entity);
		if (light.type != (int32_t)LightType::DirectionalLight)
			continue;

		environment.sunDir = glm::normalize(glm::vec4(light.direction, 0.0f));
		environment.sunColor = light.color;
		environment.sunPower = light.intensity;
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

	//...
	near = 0.0f;
	far = 0.0f;
}


void RenderScene::Draw(ERSceneBatch type)
{
	const RenderBatch& batch = GetBatch(type);

	// Bind Transform UB
	transformUB->BindBase();


	for (auto& prim : batch.elements)
	{
		// Transfromation...
		trData.modelMatrix = prim->GetWorldMatrix();
		trData.normalMatrix = prim->GetWorldMatrix();
		transformUB->UpdateData( sizeof(TransformVertexData), 0, &trData ); // Model & Normal Matrix.

		// Shader & Materail...
		RenderRscMaterial* material = prim->GetMaterial();
		GLShader* shader = material->GetShaderRsc()->GetShader();
		shader->Use();


		if (material->HasMaterialData())
		{
			material->GetUniformBuffer()->BindBase();
			material->UpdateUniformBuffer();
		}


		material->MakeTexturesActive(); // Material Textures...


		// Draw...
		prim->Draw(shader);
	}

	//
	glBindVertexArray(0);
}





} // End of namespace Raven.

#include "RenderScene.h"

#include "Engine.h"
#include "Render/RenderDebug.h"
#include "Render/RenderModule.h"

#include "Primitives/RenderPrimitive.h"
#include "Primitives/RenderTerrain.h"
#include "Primitives/RenderDebugPrimitive.h"
#include "Primitives/RenderPrimitive.h"
#include "Primitives/RenderMesh.h"
#include "Primitives/RenderSkinnedMesh.h"
#include "Primitives/RenderTerrain.h"
#include "RenderLight.h"


#include "Render/RenderResource/Shader/RenderRscShader.h" 
#include "Render/RenderResource/Shader/RenderRscMaterial.h"
#include "Render/RenderResource/Shader/UniformBuffer.h"
#include "Render/OpenGL/GLBuffer.h"
#include "Render/OpenGL/GLShader.h"


#include "ResourceManager/ResourceManager.h"
#include "ResourceManager/Resources/Mesh.h"
#include "ResourceManager/Resources/Material.h"


#include "Core/Camera.h"
#include "Scene/Scene.h"
#include "Scene/Component/Transform.h"
#include "Scene/Component/Light.h"
#include "Scene/Component/MeshComponent.h"
#include "Scene/Component/SkinnedMeshComponent.h"
#include "Scene/Component/Transform.h"
#include "Scene/Component/TerrainComponent.h"
#include "Scene/Entity/EntityManager.h"
#include <entt/entt.hpp>

#include "Logger/Console.h"


#include "GL/glew.h"
#include "glm/gtc/type_ptr.hpp"









namespace Raven {



// Data reflect TransformVertex Uniform Block.
struct TransformVertexData
{
	glm::mat4 modelMatrix;
	glm::mat4 normalMatrix;
} trData;



// Data reflect TransformVertex Uniform Block for skinned mesh
struct TransformBoneVertexData
{
	glm::mat4 modelMatrix;
	glm::mat4 normalMatrix;
	glm::mat4 bones[RENDER_SKINNED_MAX_BONES];
} trBoneData;



// --- -- --- -- --- -- --- -- --- -- --- -- --- -- --- -- --- -- --- -- --- -- --- -- 




void RenderSceneEnvironment::Reset()
{
	sunColor = glm::vec3(1.0f, 1.0f, 1.0f);
	sunDir = glm::normalize(glm::vec4(-1.0f));
	sunPower = 1.0f;
}





// --- -- --- -- --- -- --- -- --- -- --- -- --- -- --- -- --- -- --- -- --- -- --- -- 





RenderScene::RenderScene()
	: near(0.0f)
	, far(0.0f)
	, frustum(glm::mat4(1.0f))
	, isGrid(true)
	, isSky(false)
{
	environment.Reset();
}


RenderScene::~RenderScene()
{

}


void RenderScene::Setup()
{
	// Transfrom Vertex Uniform 
	transformUniform = Ptr<UniformBuffer>( UniformBuffer::Create(RenderShaderInput::TransformBlock, false) );
	transformBoneUniform = Ptr<UniformBuffer>( UniformBuffer::Create(RenderShaderInput::TransformBoneBlock, false));

	RAVEN_ASSERT(transformUniform->GetDescription().size == sizeof(TransformVertexData), "Invalid Size.");
	RAVEN_ASSERT(transformBoneUniform->GetDescription().size == sizeof(TransformBoneVertexData), "Invalid Size.");

	// Default Textures...
	defaultTextures.resize(3);
	defaultTextures[(int32_t)ESInputDefaultFlag::Normal] =
		Engine::GetModule<ResourceManager>()->GetResource<Texture2D>("assets/Textures/T_Default_Normal.raven");

	defaultTextures[(int32_t)ESInputDefaultFlag::White] =
		Engine::GetModule<ResourceManager>()->GetResource<Texture2D>("assets/Textures/T_Default_White.raven");

	defaultTextures[(int32_t)ESInputDefaultFlag::Black] =
		Engine::GetModule<ResourceManager>()->GetResource<Texture2D>("assets/Textures/T_Default_Black.raven");
}


void RenderScene::Build(Scene* scene)
{
	// View & Projection.
	CollectSceneView(scene);

	// Lights.
	CollectSceneLights(scene);

	// Terrain.
	CollectTerrain(scene);

	// Traverse the scene to collected render primitives.
	TraverseScene(scene);

	// ...
	translucentBatch.Sort();
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


void RenderScene::SetDebugPrimitives(const std::vector<RenderPrimitive*>* primitives)
{
	debugPrimitives = primitives;
}


void RenderScene::TraverseScene(Scene* scene)
{
	// Default Materials.
	const auto& defaultMaterials = Engine::GetModule<RenderModule>()->GetDefaultMaterials();

	// Primitives Collector.
	RenderPrimitiveCollector collector(this);

	// Get All models in the scene
	std::vector<ScenePrimitiveData> outPrimitiveComponents;
	GatherScenePrimitives(scene, outPrimitiveComponents);


	// Iterate over all Models in the scene.
	for (auto& scenePrim : outPrimitiveComponents)
	{
		// Primitive Component - Entity Data
		PrimitiveComponent* primComp = scenePrim.comp;
		Transform* trComp = scenePrim.tr;

		// Compute World Bounds.
		MathUtils::BoundingBox bounds = primComp->GetLocalBounds().Transform(trComp->GetWorldMatrix());

		float radius;
		glm::vec3 center;
		bounds.GetSphere(center, radius);

		// Test if its in scene view frustum?
		if (!frustum.IsInFrustum(center, radius))
		{
			continue;
		}

		// distance to view.
		glm::vec3 v = (viewPos - trComp->GetWorldPosition());
		float viewDist2 = v.x * v.x + v.y * v.y + v.z * v.z;


		// Collect Render Render Primitives...
		collector.Reset();
		collector.SetTransform(&trComp->GetWorldMatrix(), &trComp->GetWorldMatrix());
		primComp->CollectRenderPrimitives(collector);

		// Create a RenderPrimitive for each model, and add it to the correct batch.
		for (uint32_t i = 0; i < collector.primitive.size(); ++i)
		{
			RenderPrimitive* rprim = collector.primitive[i];

			// Vlidate Material.
			if (rprim->GetMaterial())
			{
				// Domain Missmatch?
				if (rprim->GetMaterialDomain() != rprim->GetDomain())
				{
					rprim->SetMaterial(nullptr);
				}
			}

			// No Material?
			if (!rprim->GetMaterial())
			{
				Material* defaultMaterial = rprim->isSkinned ? defaultMaterials.skinned.get() : defaultMaterials.mesh.get();
				rprim->SetMaterial(defaultMaterial->GetRenderRsc());
			}


			// Translucent?
			if (rprim->GetShaderType() == ERenderShaderType::Translucent)
			{
				// Gather lights that affect this translucent primitive.
				std::vector<RenderLight*> litPrim;
				GatherLights(center, radius, litPrim);

				for (const auto& lit : litPrim)
					rprim->AddLight(lit->indexInScene);


				// TRANSLUCENT BATCH.
				translucentBatch.Add(rprim, viewDist2);
			}
			else
			{
				// DEFERRED BATCH.
				deferredBatch.Add(rprim);
			}
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

	// Set the new Frustum
	frustum = MathUtils::Frustum(viewProjMatrix);
}


void RenderScene::CollectSceneLights(Scene* scene)
{
	auto lightsEttView = scene->GetRegistry().group<Light>(entt::get<Transform>);

	//
	bool isSearchForSun = true;


	// Iterate over all lights in the scene.
	for (auto entity : lightsEttView)
	{
		const auto& [light, trans] = lightsEttView.get<Light, Transform>(entity);

		// TODO: Add Scene Environment Settings to collect sun lights from.
		if (isSearchForSun && light.type == (int32_t)LightType::DirectionalLight)
		{
			// Check if it is the sun
			const auto nameComponent = scene->GetRegistry().try_get<NameComponent>(entity);

			if (nameComponent && nameComponent->name == "THE_SUN")
			{
				environment.sunDir = light.direction;
				environment.sunColor = light.color;
				environment.sunPower = light.intensity;

				isSearchForSun = false;
				continue;
			}
		}

		// ...
		float lightRadius = glm::max(light.radius, 1.0f);


		//  Clipping...
		if (light.type != (int32_t)LightType::DirectionalLight)
		{
			// Clip based on distance from the view.
			glm::vec3 v = (viewPos - trans.GetWorldPosition());
			float d2 = v.x * v.x + v.y * v.y + v.z * v.z;

			if (d2 > (light.clipDistance * light.clipDistance))
			{
				continue;
			}

			// Clip based on view frustum
			if (!frustum.IsInFrustum(trans.GetWorldPosition(), lightRadius))
			{
				continue;
			}
		}

		// Add new light...
		RenderLight* newLight = NewLight<RenderLight>();
		newLight->type = light.type + 1;
		newLight->colorAndPower = glm::vec4(light.color.x, light.color.y, light.color.z, light.intensity);
		newLight->postion = trans.GetWorldPosition();
		newLight->dir = light.direction;
		newLight->radius = lightRadius;
		newLight->innerAngle = glm::cos(glm::radians(light.innerAngle));
		newLight->outerAngle = glm::cos(glm::radians(light.outerAngle));

	}

}


void RenderScene::CollectTerrain(Scene* scene)
{
	auto TerrainEttView = scene->GetRegistry().view<TerrainComponent>();

	// No Terrain Found?
	if (TerrainEttView.empty())
		return;

	auto entity = TerrainEttView.front();
	const auto& theTerrain = TerrainEttView.get<TerrainComponent>(entity);
	auto terrainRsc = theTerrain.GetTerrainResource();
	
	RAVEN_ASSERT(terrainRsc->IsOnGPU(), "Terrain Not Loaded on GPU. Make sure its loaded before rendering.");
	
	RenderTerrain* renderTerrain = NewPrimitive<RenderTerrain>();
	renderTerrain->SetTerrainRsc(terrainRsc->renderRscTerrain);
	renderTerrain->SetWorldMatrix(glm::mat4(1.0f));
	
	// Default Terrain Material.
	const auto& defaultMaterials = Engine::GetModule<RenderModule>()->GetDefaultMaterials();
	renderTerrain->SetMaterial(defaultMaterials.terrain->GetRenderRsc());

	// Add the terrain to the deferred batch.
	deferredBatch.Add(renderTerrain);
}


void RenderScene::Clear()
{
	// Clear Render Batches...
	deferredBatch.Reset();
	translucentBatch.Reset();


	// Delete Dynamic Render Primitives created while building render scene...
	for (auto& prim : rprimitives)
		delete prim;

	// Delete Render Lights created while building render scene...
	for (auto& light : rlights)
		delete light;

	//...
	rprimitives.clear();
	rlights.clear();
	environment.Reset();
	near = 0.0f;
	far = 0.0f;
	debugPrimitives = nullptr;
}


void RenderScene::DrawDeferred()
{
	// Bind Transform Uniform Buffer.
	transformUniform->BindBase();
	transformBoneUniform->BindBase();


	// All The Batch Primitives.
	const auto& primitives = deferredBatch.GetPrimitives();

	// First: iterate on shaders...
	for (uint32_t is = 0; is < deferredBatch.GetNumShaders(); ++is)
	{
		const auto& shaderBatch = deferredBatch.GetShaderBatch(is);

		// The Shader
		GLShader* shader = shaderBatch.shader->GetShader();
		shader->Use();

		// Second: iterate on materails...
		for (uint32_t im = 0; im < shaderBatch.materials.size(); ++im)
		{
			const auto& materialBatch = deferredBatch.GetMaterialBatch( shaderBatch.materials[im] );

			// The Material
			RenderRscMaterial* material = materialBatch.material;
			material->MakeTexturesActive(defaultTextures);

			if (material->HasMaterialData())
			{
				material->GetUniformBuffer()->BindBase();
				material->UpdateUniformBuffer();
			}


			// Finally: Draw Primitives...
			for (uint32_t ip = 0; ip < materialBatch.primitives.size(); ++ip)
			{
				RenderPrimitive* prim = primitives[ materialBatch.primitives[ip] ];

				// Transform.
				if (prim->isSkinned)
				{
					auto skinned = static_cast<RenderSkinnedMesh*>(prim);

					// Model & Normal & Bones.
					trBoneData.modelMatrix = prim->GetWorldMatrix();
					trBoneData.normalMatrix = prim->GetWorldMatrix();
					memcpy(&trBoneData.bones, skinned->GetBones()->data(), sizeof(glm::mat4) * skinned->GetBones()->size());
					transformBoneUniform->UpdateData(sizeof(TransformBoneVertexData), 0, (void*)(&trBoneData));
				}
				else
				{
					// Model & Normal.
					trData.modelMatrix = prim->GetWorldMatrix();
					trData.normalMatrix = prim->GetWorldMatrix();
					transformUniform->UpdateData(sizeof(TransformVertexData), 0, &trData);
				}


				// Draw...
				prim->Draw(shader);
			}
		}
	}

	
	// End Drawing...
	glBindVertexArray(0);

}



void RenderScene::DrawDebug()
{
	// Is Empty?
	if (!debugPrimitives || debugPrimitives->empty())
		return;

	const auto& prims = *debugPrimitives;

	// Bind Transform Uniform Buffer.
	transformUniform->BindBase();

	
	// All debug primitives have the same shader & materail.
	RenderRscMaterial* materail = prims[0]->GetMaterial();
	GLShader* shader = materail->GetShaderRsc()->GetShader();


	// Draw Debug Primitives...
	for (uint32_t ip = 0; ip < prims.size(); ++ip)
	{
		RenderPrimitive* prim = prims[ip];

		// Transform.
		trData.modelMatrix = prim->GetWorldMatrix();
		trData.normalMatrix = prim->GetWorldMatrix();
		transformUniform->UpdateData(sizeof(TransformVertexData), 0, &trData); // Model & Normal Matrix.

		// Draw...
		prim->Draw(shader);
	}

	glBindVertexArray(0);
}


void RenderScene::DrawTranslucent(UniformBuffer* lightUB)
{
	// Bind Transform Uniform Buffer.
	transformUniform->BindBase();
	transformBoneUniform->BindBase();

	// All The Batch Primitives.
	const auto& primitives = translucentBatch.GetPrimitives();

	// Iterate over all translucent primitives.
	for (const auto& prim : primitives)
	{
		// The Lights.
		UpdateLights_FORWARD(lightUB, prim.primitive);


		// The Material
		RenderRscMaterial* material = prim.primitive->GetMaterial();
		material->MakeTexturesActive(defaultTextures);

		if (material->HasMaterialData())
		{
			material->GetUniformBuffer()->BindBase();
			material->UpdateUniformBuffer();
		}

		// The Shader.
		GLShader *shader = material->GetShaderRsc()->GetShader();
		shader->Use();

		// Transform.
		if (prim.primitive->isSkinned)
		{
			auto skinned = static_cast<RenderSkinnedMesh*>(prim.primitive);

			// Model & Normal & Bones.
			trBoneData.modelMatrix = prim.primitive->GetWorldMatrix();
			trBoneData.normalMatrix = prim.primitive->GetWorldMatrix();
			memcpy(&trBoneData.bones, skinned->GetBones()->data(), sizeof(glm::mat4) * skinned->GetBones()->size());
			transformBoneUniform->UpdateData(sizeof(TransformBoneVertexData), 0, (void*)(&trBoneData));
		}
		else
		{
			// Model & Normal.
			trData.modelMatrix = prim.primitive->GetWorldMatrix();
			trData.normalMatrix = prim.primitive->GetWorldMatrix();
			transformUniform->UpdateData(sizeof(TransformVertexData), 0, &trData);
		}

		// Draw...
		prim.primitive->Draw(shader);
	}

}


void RenderScene::UpdateLights_FORWARD(UniformBuffer* lightUB, RenderPrimitive* prim)
{
	static std::vector<glm::vec4> lightPos(RENDER_PASS_FORWARD_MAX_LIGHTS);
	static std::vector<glm::vec4> lightDir(RENDER_PASS_FORWARD_MAX_LIGHTS);
	static std::vector<glm::vec4> lightPower(RENDER_PASS_FORWARD_MAX_LIGHTS);
	static std::vector<glm::vec4> lightData(RENDER_PASS_FORWARD_MAX_LIGHTS);

	const auto& lit = prim->GetLights();

	for (int32_t i = 0; i < RENDER_PASS_FORWARD_MAX_LIGHTS; ++i)
	{
		if (i >= lit.size())
		{
			lightData[i].r = 0.0f; // No Light.
			continue;
		}

		const auto& light = rlights[ lit[i] ];
		lightPos[i] = glm::vec4(light->postion, 0.0f);
		lightDir[i] = glm::vec4(light->dir, 0.0f);
		lightData[i].r = light->GetType();
		lightData[i].g = light->innerAngle;
		lightData[i].b = light->outerAngle;
		lightData[i].a = light->radius;
		lightPower[i] = light->colorAndPower;
	}



	lightUB->SetDataValues(0, lightDir);
	lightUB->SetDataValues(1, lightPos);
	lightUB->SetDataValues(2, lightPower);
	lightUB->SetDataValues(3, lightData);
	lightUB->Update();
}


void RenderScene::GatherLights(const glm::vec3& center, float radius, std::vector<RenderLight*>& outLights)
{
	float r2 = radius * radius;

	for (uint32_t il = 0; il < rlights.size(); ++il)
	{
		// Invalid or Directional?
		if (rlights[il]->type <= 1)
			continue;

		glm::vec3 v = (center - rlights[il]->postion);
		float d2 = v.x * v.x + v.y * v.y + v.z * v.z;

		float lr2 = rlights[il]->radius;
		lr2 = lr2 * lr2;


		// No intersection?
		if ((d2 - lr2) > r2)
			continue;

		outLights.push_back(rlights[il]);
	}

}


void RenderScene::GatherScenePrimitives(Scene* scene, std::vector<ScenePrimitiveData>& outPrimitivesComp)
{
	// Iterate over all MeshComponent in the scene.
	{
		auto meshPrimitives = scene->GetRegistry().group<MeshComponent>(entt::get<Transform>);

		for (auto entity : meshPrimitives)
		{
			auto& [mesh, trans] = meshPrimitives.get<MeshComponent, Transform>(entity);

			ScenePrimitiveData scenePrim;
			scenePrim.comp = &mesh;
			scenePrim.tr = &trans;
			outPrimitivesComp.push_back(scenePrim);
		}
	}


	// Iterate over all SkinnedMeshComponent in the scene.
	{
		auto skinnedPrimitives = scene->GetRegistry().group<SkinnedMeshComponent>(entt::get<Transform>);

		for (auto entity : skinnedPrimitives)
		{
			auto& [skinned, trans] = skinnedPrimitives.get<SkinnedMeshComponent, Transform>(entity);

			ScenePrimitiveData scenePrim;
			scenePrim.comp = &skinned;
			scenePrim.tr = &trans;
			outPrimitivesComp.push_back(scenePrim);
		}
	}


}






} // End of namespace Raven.

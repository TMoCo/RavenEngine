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
#include "Scene/Component/MeshRenderer.h"
#include "Scene/Component/TerrainComponent.h"
#include "Scene/Entity/EntityManager.h"
#include <entt/entt.hpp>

#include "Scene/Component/Model.h"

#include "GL/glew.h"
#include "glm/gtc/type_ptr.hpp"

#include "ResourceManager/Resources/Mesh.h"

#define CHECK_GL_ERROR() 																\
																						\
char errorMessage[128] = "\0";															\
do																						\
{																						\
	GLenum error = glGetError();														\
	if (error != GL_NO_ERROR)															\
	sprintf(errorMessage, "OpenGL error 0x%04x at %s:%i",  error, __func__, __LINE__);	\
} while (0)


namespace Raven {




// ~MinimalSolution --- ---- --- ---- --- ---- ---
struct TransformUBO
{
	glm::mat4 modelMatrix;
	glm::mat4 viewMatrix;
	glm::mat4 projectionMatrix;
};

// ~MinimalSolution --- ---- --- ---- --- ---- ---
struct SkinnedTransformUBO
{
	glm::mat4 modelMatrix;
	glm::mat4 viewMatrix;
	glm::mat4 projectionMatrix;
	glm::mat4 bones[52];
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


SkinnedTransformUBO skinnedTrData;
TransformUBO trData;
LightingUBO lightData;
MaterialUBO matData;
// ~MinimalSolution --- ---- --- ---- --- ---- ---





RenderScene::RenderScene()
	: trUBO(nullptr)
	, lightingUBO(nullptr)
	, skinnedUBO(nullptr)
{

}


RenderScene::~RenderScene()
{
	delete skinnedUBO;
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

	skinnedShader = new RenderRscShader();
	skinnedShader->Load(ERenderShaderType::SkinnedMesh, "Default_Skined");
	skinnedMaterail = new RenderRscMaterial(skinnedShader);


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



	skinnedUBO = GLBuffer::Create(EGLBufferType::Uniform, sizeof(SkinnedTransformUBO), EGLBufferUsage::DynamicDraw);
	skinnedUBO->BindBase(3);

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
	auto group = scene->GetRegistry().group<MeshRenderer>(entt::get<Transform>);
	auto group2 = scene->GetRegistry().group<SkinnedMeshRenderer>(entt::get<Transform>);
	if (group.empty() && group2.empty())
		return; 

	// ~Testing---------------------------------------
	// Clear Debug just for testing...
	GetBatch(ERSceneBatch::Debug).Clear();
	// ~Testing---------------------------------------


	// Iterate over all Models in the scene.
	// TODO: Culling should be here, for both the view and lights shadow frustums.
	
	
	for (auto entity : group2)
	{
		const auto& [renderer, trans] = group2.get<SkinnedMeshRenderer, Transform>(entity);

		if (renderer.mesh == nullptr) 
		{
			renderer.GetMeshFromModel();
		}

		// Create a RenderPrimitive for each model, and add it to the correct batch.
		const auto& mesh = renderer.mesh;
	
		if (mesh == nullptr) 
		{
			continue;
		}

		// Update Mesh on GPU if not loaded yet.
		if (!mesh->IsOnGPU())
		{
			mesh->renderRscMesh = new RenderSkinnedMesh();
			mesh->LoadOnGpu();
			static_cast<RenderSkinnedMesh*>(mesh->renderRscMesh)->bones.resize(Bone::MAX_COUNT);
		}

		//need to be optimized.
		if (auto skeleton = renderer.getSkeleton()) 
		{
			for (auto i = 0; i < skeleton->GetBoneSize(); i++)
			{
				auto bone = skeleton->GetBone(i);
				static_cast<RenderSkinnedMesh*>(mesh->renderRscMesh)->bones[i] = 
					bone->localTransform->GetWorldMatrix() * bone->offsetMatrix;
			}
		}

		RenderMesh* rmesh = NewPrimitive<RenderMesh>();
		rmesh->SetWorldMatrix(trans.GetWorldMatrix());
		rmesh->SetMesh(mesh->renderRscMesh);

		// TODO: Render with Model Materials...
		rmesh->SetMaterial(skinnedMaterail);

		// Add to opaque...
		GetBatch(ERSceneBatch::Opaque).Add(rmesh);
	}


	for (auto entity : group)
	{
		const auto& [renderer, trans] = group.get<MeshRenderer,Transform>(entity);
		if (renderer.mesh == nullptr)
		{
			renderer.GetMeshFromModel();
		}
		// Create a RenderPrimitive for each model, and add it to the correct batch.
		const auto& mesh = renderer.mesh;

		if (mesh == nullptr)
		{
			continue;
		}

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


//#######################################add
	skinnedTrData.modelMatrix = glm::mat4(1.0f);
	skinnedTrData.viewMatrix = view;
	skinnedTrData.projectionMatrix = projection;
	skinnedUBO->UpdateSubData(sizeof(SkinnedTransformUBO),0, &skinnedTrData);
//#######################################add


	lightingUBO->UpdateSubData(sizeof(LightingUBO), 0, &lightData);
	materialUBO->UpdateSubData(sizeof(MaterialUBO), 0, &matData);

	for (auto& prim : batch.elements)
	{
		GLShader* shader = prim->GetMaterial()->GetShaderRsc()->GetShader();

		auto skinned = dynamic_cast<RenderSkinnedMesh*>(prim->GetRsc());
		
		if (skinned) 
		{
			skinnedTrData.modelMatrix = prim->GetWorldMatrix();
			memcpy(&skinnedTrData.bones, skinned->bones.data(), sizeof(glm::mat4) * 52);
			skinnedUBO->UpdateData(sizeof(SkinnedTransformUBO), (void*)(&skinnedTrData));
		}
		else 
		{
			// Model Matrix...
			trUBO->UpdateSubData(
				sizeof(glm::mat4),
				offsetof(TransformUBO, modelMatrix),
				glm::value_ptr(prim->GetWorldMatrix())
			);
		}

		shader->Use();
		prim->Draw(shader);
	}

	//
	glBindVertexArray(0);
}





} // End of namespace Raven.

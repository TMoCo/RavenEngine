#include "ProceduralGenerator/ProceduralGenerator.h"


#include "Scene/Scene.h"
#include "Scene/Entity/Entity.h"
#include "Scene/Component/Transform.h"
#include "Scene/Component/TerrainComponent.h"
#include "Scene/Component/Light.h"


#include "Engine.h"
#include "ResourceManager/ResourceManager.h"
#include "ResourceManager/Resources/Terrain.h"
#include "ResourceManager/Resources/Mesh.h"




namespace Raven {


ProceduralGenerator::ProceduralGenerator()
{
	// Create instane of TerrainGeneration.
	terrainGen = Ptr<TerrainGeneration>(new TerrainGeneration());
}


ProceduralGenerator::~ProceduralGenerator()
{

}


void ProceduralGenerator::Initialize()
{

}


void ProceduralGenerator::Destroy()
{

}


Scene* ProceduralGenerator::GenerateNewScene(const glm::vec2& size, const glm::vec2& height)
{
	// --- - -- - --- -- --- --- ---
	// Terrain.
	Ptr<HeightMap> heightMap = terrainGen->GenerateHeightMap(512, 512);
	terrainGen->WriteHeightMap(heightMap.get(), "C:/Temp/heightmap.png");


	Ptr<Terrain> terrain( new Terrain() );
	terrain->SetName("The_Terrain");
	terrain->SetTerrainData(heightMap, size, 4, height);
	terrain->LoadRenderResource();

	// --- - -- - --- -- --- --- ---
	// Scene.
	Scene* scene = new Scene("Procedural_Scene");

	Entity terrainEntity = scene->CreateEntity("The_Terrain");
	TerrainComponent& terrainComp = terrainEntity.GetOrAddComponent<TerrainComponent>();
	terrainComp.SetTerrainResource(terrain);

	{
		auto& lightEntity = scene->CreateEntity("THE_SUN");
		auto& lightComp = lightEntity.GetOrAddComponent<Light>();
		auto& tr = lightEntity.GetOrAddComponent<Transform>();

		lightComp.type = (int32_t)LightType::DirectionalLight;
		lightComp.color = glm::vec4(1.0f, 0.95f, 0.8f, 1.0f);
		lightComp.intensity = 2.4f;
		lightComp.direction = glm::normalize(glm::vec3(-1.0f));
	}


	return scene;
}



} // End of namespace Raven




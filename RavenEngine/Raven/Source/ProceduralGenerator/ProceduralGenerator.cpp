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


void AddGrassLayer(Ptr<Terrain> terrain, Ptr<HeightMap> heightMap)
{
	Ptr<Mesh> grassMesh = Engine::GetModule<ResourceManager>()->GetResource<Mesh>(
		"assets/Meshes/Grass/SMs_Grass_1_1.raven");

	Ptr<Material> grassMaterail = Engine::GetModule<ResourceManager>()->GetResource<Material>(
		"assets/Materials/M_Grass_1_Instanced.raven");

	std::vector< Ptr<Material> > grassMaterails;
	grassMaterails.push_back(grassMaterail);


	terrain->NewLayer("Grass_1", grassMesh, grassMaterails);

	for (size_t i = 0; i < terrain->GetBins().size(); ++i)
	{
		const auto& bin = terrain->GetBins()[i];

		glm::vec3 bcenter = bin.bounds.GetCenter();
		glm::vec3 bext = bin.bounds.GetExtent();
		glm::vec3 binCorner = bcenter - bext;

		for (float z = binCorner.z; z < (binCorner.z + bext.z * 2.0f); z += 1.5f)
		{
			for (float x = binCorner.x; x < (binCorner.x + bext.x * 2.0f); x += 1.5f)
			{
				float h = heightMap->GetHeight(x, z);

				if (h < 6.5 || h > 50.0)
				{
					continue;
				}


				auto p = glm::vec3(x, h - 2.5f, z);
				auto noiseg = glm::perlin(p + 3000.0f);

				if (h < 20 || h > 25.0)
				{
					if (noiseg * 10000.0f > 0.07f)
						continue;
				}

				p.x += glm::fract(noiseg) * 2.4f;

				auto noise = glm::mix(0.6, 1.0, glm::perlin(p));
				auto noisex = glm::simplex(p + 100.0f);
				p.z += glm::fract(noisex) * 2.4f;

				glm::mat4 tr;
				tr = glm::translate(glm::mat4(1.0f), p);
				tr = tr * glm::rotate(glm::mat4(1.0f), noisex, glm::vec3(0.0, 1.0, 0.0));
				tr = tr * glm::scale(glm::mat4(1.0f), abs(glm::vec3(2.0 * noise)));


				terrain->AddFoliageInstance(i, 0, tr);
			}

		}

	}



}


Scene* ProceduralGenerator::GenerateNewScene(const glm::vec2& size, const glm::vec2& height)
{
	// --- - -- - --- -- --- --- ---
	// Terrain.
	Ptr<HeightMap> heightMap = terrainGen->GenerateHeightMap(512, 512);
	heightMap->SetHeightScale(height);
	heightMap->SetSizeScale(size);
	heightMap->ComputeTangents();

	terrainGen->WriteHeightMap(heightMap.get(), "C:/Temp/heightmap.png");
	terrainGen->WriteHeightMapNormal(heightMap.get(), "C:/Temp/heightmap_n.png");


	Ptr<Terrain> terrain( new Terrain() );
	terrain->SetName("The_Terrain");
	terrain->SetTerrainData(heightMap, size, 4, height);
	terrain->LoadRenderResource();

	Ptr<Material> terrainMaterial = Engine::GetModule<ResourceManager>()->GetResource<Material>(
		"assets/Materials/M_Terrain_A.raven");

	terrain->SetMaterial(terrainMaterial);

	AddGrassLayer(terrain, heightMap);

	// --- - -- - --- -- --- --- ---
	// Scene.
	Scene* scene = new Scene("Procedural_Scene");

	Entity terrainEntity = scene->CreateEntity("The_Terrain");
	TerrainComponent& terrainComp = terrainEntity.GetOrAddComponent<TerrainComponent>();
	terrainComp.SetTerrainResource(terrain);


	return scene;
}



} // End of namespace Raven




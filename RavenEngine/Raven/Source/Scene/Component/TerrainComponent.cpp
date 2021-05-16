#pragma once


#include "Engine.h"
#include "ResourceManager/ResourceManager.h"
#include "Scene/Component/TerrainComponent.h"




namespace Raven
{


TerrainComponent::TerrainComponent()
{

}


TerrainComponent::~TerrainComponent()
{

}


void TerrainComponent::SetTerrainResource(Ptr<Terrain> newTerrain)
{
	terrain = newTerrain;
}


}

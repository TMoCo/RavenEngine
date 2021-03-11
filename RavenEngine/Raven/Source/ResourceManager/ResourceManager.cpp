#include <iostream>

#include "ResourceManager.h"
#include "ResourceManager/Loaders/ILoader.h"
#include "ResourceManager/Loaders/ImageLoader.h"


namespace Raven {

	void ResourceManager::Initialize()
	{
		std::cout << "Initialised the resource manager" << '\n';
		AddLoader(std::make_unique<ImageLoader>(*this)); // create an image loader (resource manager as constructor argument
	}

	void ResourceManager::Destroy()
	{
		std::cout << "Destroyed the resource manager" << std::endl;

	}

	void ResourceManager::AddLoader(std::unique_ptr<ILoader> loader)
	{
		if (std::find(loaders.begin(), loaders.end(), loader) == loaders.end())
		{
			std::cout << "Added a loader of type " << ILoader::TypeToString(loader->GetType()) << '\n';
			loaders.push_back(std::move(loader));
			std::cout << '\n';
		}
	}

	IResource* ResourceManager::GetResource(const std::string& id) 
	{
		auto resourceIter = resourceMap.find(id); // a key/value pair iterator
		if (resourceIter == resourceMap.end())
		{
			return nullptr;
		}
		else
		{
			return resourceIter->second;
		}
	}

	bool ResourceManager::HasResource(const std::string& id)
	{
		// true if a key exists
		return resourceMap.count(id);
	}

	/*
	* // load an obj file from a specified path
	bool ResourceManager::LoadOBJ(const std::string& path)
	{
		tinyobj::attrib_t attributes;
		std::vector<tinyobj::shape_t> shapes;
		std::vector<tinyobj::material_t> materials;
		std::string err;

		if (!tinyobj::LoadObj(&attributes, &shapes, &materials, &err, path.c_str())) 
		{
			std::cerr << err << std::endl;
			return false;
		}

		// loop over the shapes (one shape = one mesh)
		for (size_t shape = 0; shape < shapes.size(); shape++)
		{
			// loop over the faces
			for (size_t face = 0; face < shapes[shape].mesh.num_face_vertices.size(); face++)
			{
				size_t faceVertices = shapes[shape].mesh.num_face_vertices[face];
				// loop over the face's vertices
				for (size_t vertex = 0; vertex < faceVertices; vertex++)
				{
					// create vertices here
				}
			}
		}
		return true;
	}
	*/
}
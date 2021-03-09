#include "ResourceManager.h"

#include <iostream>

// for obj loading
#define TINYOBJLOADER_IMPLEMENTATION 
#include <tinyobjloader/tiny_obj_loader.h>

// for image loading
#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

namespace Raven {

	ResourceManager::ResourceManager()
	{

	}

	ResourceManager::~ResourceManager()
	{

	}

	// load an obj file from a specified path

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

	// load a image from a specified path

	bool ResourceManager::LoadImage(const std::string& path)
	{

	}
}
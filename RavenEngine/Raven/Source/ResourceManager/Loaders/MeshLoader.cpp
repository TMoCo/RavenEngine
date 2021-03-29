//////////////////////////////////////////////////////////////////////////////
// This file is part of the Raven Game Engine			                    //
//////////////////////////////////////////////////////////////////////////////

#define TINYOBJLOADER_IMPLEMENTATION
#include <tinyobjloader/tiny_obj_loader.h>
#include <glm/glm.hpp>

#include "Utilities/StringUtils.h"

#include "ResourceManager/Loaders/MeshLoader.h"
#include "ResourceManager/Resources/Mesh.h"

namespace Raven
{
	bool MeshLoader::LoadAsset(const std::string& path)
	{
        std::string extension = StringUtils::GetExtension(path);
        // file extension calls appropriate loading function
        if (extension == "obj")
        {
            return LoadOBJ(path);
        }
        else
        {
            return false;
        }
	}

    bool MeshLoader::LoadOBJ(const std::string& path)
    {
     //   int meshNum = 0;
     //   char buffer[100]; // up to 100 different meshes in a model
     //  std::string meshId = path + std::string(StringUtils::IntToString(meshNum++, buffer, StringUtils::Decimal));

        if (resourceManager->HasResource(path))
        {
            // already loaded
            return true;
        }

        // setup variables to get model info
        tinyobj::attrib_t attrib; // contains all the positions, normals, textures and faces
        std::vector<tinyobj::shape_t> shapes; // all the separate objects and their faces
        std::vector<tinyobj::material_t> materials; // object materials
        std::string err;

        // load the model, show error if not
        if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &err, path.c_str())) {
            LOGE(err);
            return false;
        }

        // loop over the shapes in the model
        for (const auto& shape : shapes)
        {
            // one shape = one mesh
            Mesh* mesh = new Mesh();
            // resize the mesh data
            for (const auto& index : shape.mesh.indices)
            {
                // set vertex data 
                mesh->positions.push_back(glm::vec3(
                    attrib.vertices[3 * index.vertex_index + 0],
                    attrib.vertices[3 * index.vertex_index + 1],
                    attrib.vertices[3 * index.vertex_index + 2]));

                mesh->bbox.AddVertex(*mesh->positions.rbegin());

                // check that normals and texcoords exist, add accordingly
                if (index.normal_index == -1)
                {
                    mesh->normals.push_back(glm::vec3(0.0f));
                }
                else
                {

                    mesh->normals.push_back(glm::vec3(
                        attrib.normals[3 * index.normal_index + 0],
                        attrib.normals[3 * index.normal_index + 1],
                        attrib.normals[3 * index.normal_index + 2]));
                }

                if (index.texcoord_index == -1)
                {
                    mesh->texCoords.push_back(glm::vec2(0.0f));
                }
                else
                {
                    mesh->texCoords.push_back(glm::vec2(
                        attrib.texcoords[2 * index.texcoord_index + 0],
                        1.0f - attrib.texcoords[2 * index.texcoord_index + 1]));
                }

                mesh->indices.push_back(static_cast<uint32_t>(mesh->indices.size()));
            }
            // once all vertices have been processed, update the bbox's centre
            mesh->bbox.UpdateCentre();
            resourceManager->AddResource(path, mesh); // file path is resource id
            //meshId = path + std::string(StringUtils::IntToString(meshNum++, buffer, StringUtils::Decimal));
        }
        // TODO: process Material data
        return true;
    }
}
#include "ModelLoader.h"
#include "ResourceManager/Resources/Mesh.h"
#include "ResourceManager/Resources/Model.h"
#include "Utilities/StringUtils.h"

#define TINYOBJLOADER_IMPLEMENTATION
#include <tinyobjloader/tiny_obj_loader.h>
#include <glm/glm.hpp>

namespace Raven
{
	// calls the inherited constructor
	ModelLoader::ModelLoader(ResourceManager& initResourceManager) : ILoader(initResourceManager, ELoaderType::LT_Model) {}

	bool ModelLoader::LoadAsset(const std::string& path)
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

    bool ModelLoader::LoadOBJ(const std::string& path)
    {
        // setup variables to get model info
        tinyobj::attrib_t attrib; // contains all the positions, normals, textures and faces
        std::vector<tinyobj::shape_t> shapes; // all the separate objects and their faces
        std::vector<tinyobj::material_t> materials; // object materials
        std::string err;

        // load the model, show error if not
        if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &err, path.c_str())) {
            throw std::runtime_error(err);
        }

        // The model class, just a container for mesh resources and to easily access the meshes
        Model* model = new Model();

        // loop over the shapes in the model
        for (size_t i = 0; i < shapes.size(); i++)
        {
            // one shape = one mesh
            Mesh* mesh = new Mesh();
            // resize the mesh data
            for (const auto& index : shapes[i].mesh.indices)
            {
                // set vertex data
                mesh->verts.push_back(glm::vec3(
                    attrib.vertices[3 * index.vertex_index + 0],
                    attrib.vertices[3 * index.vertex_index + 1],
                    attrib.vertices[3 * index.vertex_index + 2]));

                mesh->normals.push_back(glm::vec3(
                    attrib.normals[3 * index.normal_index + 0],
                    attrib.normals[3 * index.normal_index + 1],
                    attrib.normals[3 * index.normal_index + 2]));

                mesh->texCoords.push_back(glm::vec2(
                    attrib.texcoords[2 * index.texcoord_index + 0],
                    1.0f - attrib.texcoords[2 * index.texcoord_index + 1]));

                mesh->indices.push_back(static_cast<uint32_t>(mesh->indices.size()));
            }
            model->AddMesh(mesh);
        }
        resourceManager->AddResource(path, model); // path 
        return true;
    }
}
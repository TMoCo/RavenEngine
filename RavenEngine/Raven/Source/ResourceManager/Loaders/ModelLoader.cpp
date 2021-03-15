#include "ModelLoader.h"
#include "ResourceManager/Resources/Model.h"

#include <glm/glm.hpp>

namespace Raven
{
	// calls the inherited constructor
	ModelLoader::ModelLoader(ResourceManager& initResourceManager) : ILoader(initResourceManager, ELoaderType::LT_Model) {}

	bool ModelLoader::LoadAsset(const std::string& path)
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

        // TODO: Put model in memory manager
        Model* model = new Model();

        // combine all the shapes into a single model (shape = mesh)
        for (const auto& shape : shapes) {

            Mesh* mesh = new Mesh();

            for (const auto& index : shape.mesh.indices) {
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
            model->meshes.push_back(mesh);
        }
		return true;
	}
}
#pragma once
#include <glm/glm.hpp>

namespace Raven
{
	class Mesh;
	enum class PrimitiveType : int32_t
	{
		Plane = 0,
		Quad = 1,
		Cube = 2,
		Pyramid = 3,
		Sphere = 4,
		Capsule = 5,
		Cylinder = 6,
		Terrain = 7,
		File = 8,
	};

	namespace MeshFactory
	{
		Mesh* CreatePrimative(PrimitiveType type);
		Mesh* CreateQuad();
		Mesh* CreateQuad(float x, float y, float width, float height);
		Mesh* CreateQuad(const glm::vec2& position, const glm::vec2& size);
		Mesh* CreateCube();
		Mesh* CreatePyramid();
		Mesh* CreateSphere(uint32_t xSegments = 64, uint32_t ySegments = 64);
		Mesh* CreateCapsule(float radius = 1.0f, float midHeight = 1.0f, int32_t radialSegments = 64, int32_t rings = 8);
		Mesh* CreatePlane(float width, float height, const glm::vec3& normal);
		Mesh* CreateCylinder(float bottomRadius = 1.0f, float topRadius = 1.0f, float height = 1.0f, int32_t radialSegments = 64, int32_t rings = 8);
		Mesh* CreateTerrain();
	};
};

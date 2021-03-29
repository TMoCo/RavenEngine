#include "MeshFactory.h"

#include "ResourceManager/Resources/Mesh.h"
#define _USE_MATH_DEFINES
#include <math.h>
#include <glm/gtc/quaternion.hpp>


namespace Raven
{
	namespace MeshFactory
	{
		struct Vertex
		{
			glm::vec3 position;
			glm::vec3 normal;
			glm::vec2 texCoord;
		};

		Mesh* CreateQuad(float x, float y, float width, float height)
		{

			Mesh* mesh = new Mesh();

			mesh->positions.resize(4);
			mesh->texCoords.resize(4);

			mesh->positions[0] = glm::vec3(x, y, 0.0f);
			mesh->texCoords[0] = glm::vec2(0.0f, 1.0f);

			mesh->positions[1] = glm::vec3(x + width, y, 0.0f);
			mesh->texCoords[1] = glm::vec2(0, 0);

			mesh->positions[2] = glm::vec3(x + width, y + height, 0.0f);
			mesh->texCoords[2] = glm::vec2(1, 0);

			mesh->positions[3] = glm::vec3(x, y + height, 0.0f);
			mesh->texCoords[3] = glm::vec2(1, 1);

			mesh->indices = { 0, 1, 2, 2, 3, 0, };

			return mesh;
		}

		Mesh* CreateQuad(const glm::vec2& position, const glm::vec2& size)
		{
			return CreateQuad(position.x, position.y, size.x, size.y);
		}

		Mesh* CreateQuad()
		{
			Mesh* mesh = new Mesh();
			mesh->positions.resize(4);
			mesh->texCoords.resize(4);

			mesh->positions[0] = glm::vec3(-1.0f, -1.0f, 0.0f);
			mesh->texCoords[0] = glm::vec2(0.0f, 0.0f);

			mesh->positions[1] = glm::vec3(1.0f, -1.0f, 0.0f);
			mesh->texCoords[1] = glm::vec2(1.0f, 0.0f);

			mesh->positions[2] = glm::vec3(1.0f, 1.0f, 0.0f);
			mesh->texCoords[2] = glm::vec2(1.0f, 1.0f);

			mesh->positions[3] = glm::vec3(-1.0f, 1.0f, 0.0f);
			mesh->texCoords[3] = glm::vec2(0.0f, 1.0f);

			mesh->indices = { 0, 1, 2, 2, 3, 0, };

			return mesh;
		}

		Mesh* CreateCube()
		{
			//    v6----- v5
			//   /|      /|
			//  v1------v0|
			//  | |     | |
			//  | |v7---|-|v4
			//  |/      |/
			//  v2------v3
			Mesh* mesh = new Mesh();
			mesh->texCoords.resize(24);
			mesh->normals.resize(24);
			mesh->positions.resize(24);

			mesh->positions[0] = glm::vec3(1.0f, 1.0f, 1.0f);
			mesh->normals[0] = glm::vec3(0.0f, 0.0f, 1.0f);

			mesh->positions[1] = glm::vec3(-1.0f, 1.0f, 1.0f);
			mesh->normals[1] = glm::vec3(0.0f, 0.0f, 1.0f);

			mesh->positions[2] = glm::vec3(-1.0f, -1.0f, 1.0f);
			mesh->normals[2] = glm::vec3(0.0f, 0.0f, 1.0f);

			mesh->positions[3] = glm::vec3(1.0f, -1.0f, 1.0f);
			mesh->normals[3] = glm::vec3(0.0f, 0.0f, 1.0f);

			mesh->positions[4] = glm::vec3(1.0f, 1.0f, 1.0f);
			mesh->normals[4] = glm::vec3(1.0f, 0.0f, 0.0f);

			mesh->positions[5] = glm::vec3(1.0f, -1.0f, 1.0f);
			mesh->normals[5] = glm::vec3(1.0f, 0.0f, 0.0f);

			mesh->positions[6] = glm::vec3(1.0f, -1.0f, -1.0f);
			mesh->normals[6] = glm::vec3(1.0f, 0.0f, 0.0f);

			mesh->positions[7] = glm::vec3(1.0f, 1.0f, -1.0f);
			mesh->texCoords[7] = glm::vec2(0.0f, 1.0f);
			mesh->normals[7] = glm::vec3(1.0f, 0.0f, 0.0f);

			mesh->positions[8] = glm::vec3(1.0f, 1.0f, 1.0f);
			mesh->normals[8] = glm::vec3(0.0f, 1.0f, 0.0f);

			mesh->positions[9] = glm::vec3(1.0f, 1.0f, -1.0f);
			mesh->normals[9] = glm::vec3(0.0f, 1.0f, 0.0f);

			mesh->positions[10] = glm::vec3(-1.0f, 1.0f, -1.0f);
			mesh->texCoords[10] = glm::vec2(0.0f, 1.0f);
			mesh->normals[10] = glm::vec3(0.0f, 1.0f, 0.0f);

			mesh->positions[11] = glm::vec3(-1.0f, 1.0f, 1.0f);
			mesh->normals[11] = glm::vec3(0.0f, 1.0f, 0.0f);

			mesh->positions[12] = glm::vec3(-1.0f, 1.0f, 1.0f);
			mesh->normals[12] = glm::vec3(-1.0f, 0.0f, 0.0f);

			mesh->positions[13] = glm::vec3(-1.0f, 1.0f, -1.0f);
			mesh->normals[13] = glm::vec3(-1.0f, 0.0f, 0.0f);

			mesh->positions[14] = glm::vec3(-1.0f, -1.0f, -1.0f);
			mesh->normals[14] = glm::vec3(-1.0f, 0.0f, 0.0f);

			mesh->positions[15] = glm::vec3(-1.0f, -1.0f, 1.0f);
			mesh->normals[15] = glm::vec3(-1.0f, 0.0f, 0.0f);

			mesh->positions[16] = glm::vec3(-1.0f, -1.0f, -1.0f);
			mesh->normals[16] = glm::vec3(0.0f, -1.0f, 0.0f);

			mesh->positions[17] = glm::vec3(1.0f, -1.0f, -1.0f);
			mesh->normals[17] = glm::vec3(0.0f, -1.0f, 0.0f);

			mesh->positions[18] = glm::vec3(1.0f, -1.0f, 1.0f);
			mesh->normals[18] = glm::vec3(0.0f, -1.0f, 0.0f);

			mesh->positions[19] = glm::vec3(-1.0f, -1.0f, 1.0f);
			mesh->normals[19] = glm::vec3(0.0f, -1.0f, 0.0f);

			mesh->positions[20] = glm::vec3(1.0f, -1.0f, -1.0f);
			mesh->normals[20] = glm::vec3(0.0f, 0.0f, -1.0f);

			mesh->positions[21] = glm::vec3(-1.0f, -1.0f, -1.0f);
			mesh->normals[21] = glm::vec3(0.0f, 0.0f, -1.0f);

			mesh->positions[22] = glm::vec3(-1.0f, 1.0f, -1.0f);
			mesh->normals[22] = glm::vec3(0.0f, 0.0f, -1.0f);

			mesh->positions[23] = glm::vec3(1.0f, 1.0f, -1.0f);
			mesh->normals[23] = glm::vec3(0.0f, 0.0f, -1.0f);

			for (int i = 0; i < 6; i++)
			{
				mesh->texCoords[i * 4 + 0] = glm::vec2(0.0f, 0.0f);
				mesh->texCoords[i * 4 + 1] = glm::vec2(1.0f, 0.0f);
				mesh->texCoords[i * 4 + 2] = glm::vec2(1.0f, 1.0f);
				mesh->texCoords[i * 4 + 3] = glm::vec2(0.0f, 1.0f);
			}

			mesh->indices = {
				0,1,2,
				0,2,3,
				4,5,6,
				4,6,7,
				8,9,10,
				8,10,11,
				12,13,14,
				12,14,15,
				16,17,18,
				16,18,19,
				20,21,22,
				20,22,23
			};
			return mesh;
		}

		Mesh* CreatePyramid()
		{
			Vertex* data = new Vertex[18];


			data[0].position = glm::vec3(1.0f, 1.0f, -1.0f);
			data[0].texCoord = glm::vec2(0.24f, 0.20f);
			data[0].normal = glm::vec3(0.0f, 0.8948f, 0.4464f);

			data[1].position = glm::vec3(-1.0f, 1.0f, -1.0f);
			data[1].texCoord = glm::vec2(0.24f, 0.81f);
			data[1].normal = glm::vec3(0.0f, 0.8948f, 0.4464f);

			data[2].position = glm::vec3(0.0f, 0.0f, 1.0f);
			data[2].texCoord = glm::vec2(0.95f, 0.50f);
			data[2].normal = glm::vec3(0.0f, 0.8948f, 0.4464f);

			data[3].position = glm::vec3(-1.0f, 1.0f, -1.0f);
			data[3].texCoord = glm::vec2(0.24f, 0.21f);
			data[3].normal = glm::vec3(-0.8948f, 0.0f, 0.4464f);

			data[4].position = glm::vec3(-1.0f, -1.0f, -1.0f);
			data[4].texCoord = glm::vec2(0.24f, 0.81f);
			data[4].normal = glm::vec3(-0.8948f, 0.0f, 0.4464f);

			data[5].position = glm::vec3(0.0f, 0.0f, 1.0f);
			data[5].texCoord = glm::vec2(0.95f, 0.50f);
			data[5].normal = glm::vec3(-0.8948f, 0.0f, 0.4464f);

			data[6].position = glm::vec3(1.0f, 1.0f, -1.0f);
			data[6].texCoord = glm::vec2(0.24f, 0.81f);
			data[6].normal = glm::vec3(0.8948f, 0.0f, 0.4475f);

			data[7].position = glm::vec3(0.0f, 0.0f, 1.0f);

			data[7].texCoord = glm::vec2(0.95f, 0.50f);
			data[7].normal = glm::vec3(0.8948f, 0.0f, 0.4475f);

			data[8].position = glm::vec3(1.0f, -1.0f, -1.0f);
			data[8].texCoord = glm::vec2(0.24f, 0.21f);
			data[8].normal = glm::vec3(0.8948f, 0.0f, 0.4475f);

			data[9].position = glm::vec3(-1.0f, -1.0f, -1.0f);
			data[9].texCoord = glm::vec2(0.24f, 0.21f);
			data[9].normal = glm::vec3(0.0f, -0.8948f, 0.448f);

			data[10].position = glm::vec3(1.0f, -1.0f, -1.0f);
			data[10].texCoord = glm::vec2(0.24f, 0.81f);
			data[10].normal = glm::vec3(0.0f, -0.8948f, 0.448f);

			data[11].position = glm::vec3(0.0f, 0.0f, 1.0f);
			data[11].texCoord = glm::vec2(0.95f, 0.50f);
			data[11].normal = glm::vec3(0.0f, -0.8948f, 0.448f);

			data[12].position = glm::vec3(-1.0f, 1.0f, -1.0f);
			data[12].texCoord = glm::vec2(0.0f, 0.0f);
			data[12].normal = glm::vec3(0.0f, 0.0f, -1.0f);

			data[13].position = glm::vec3(1.0f, 1.0f, -1.0f);
			data[13].texCoord = glm::vec2(0.0f, 1.0f);
			data[13].normal = glm::vec3(0.0f, 0.0f, -1.0f);

			data[14].position = glm::vec3(1.0f, -1.0f, -1.0f);
			data[14].texCoord = glm::vec2(1.0f, 1.0f);
			data[14].normal = glm::vec3(0.0f, 0.0f, -1.0f);

			data[15].position = glm::vec3(-1.0f, -1.0f, -1.0f);
			data[15].texCoord = glm::vec2(0.96f, 0.0f);
			data[15].normal = glm::vec3(0.0f, 0.0f, -1.0f);

			data[16].position = glm::vec3(0.0f, 0.0f, 0.0f);
			data[16].texCoord = glm::vec2(0.0f, 0.0f);
			data[16].normal = glm::vec3(0.0f, 0.0f, 0.0f);

			data[17].position = glm::vec3(0.0f, 0.0f, 0.0f);
			data[17].texCoord = glm::vec2(0.0f, 0.0f);
			data[17].normal = glm::vec3(0.0f, 0.0f, 0.0f);



			Mesh* mesh = new Mesh();

			mesh->indices = {
				0,1,2,
				3,4,5,
				6,7,8,
				9,10,11,
				12,13,14,
				15,12,14
			};


			for (auto i = 0; i < 18; i++)
			{
				mesh->positions.emplace_back(data[i].position);
				mesh->texCoords.emplace_back(data[i].texCoord);
				mesh->normals.emplace_back(data[i].normal);
			}

			return mesh;

		}

		Mesh* CreateSphere(uint32_t xSegments, uint32_t ySegments)
		{
			auto data = std::vector<Vertex>();

			float sectorCount = static_cast<float>(xSegments);
			float stackCount = static_cast<float>(ySegments);
			float sectorStep = 2 * M_PI / sectorCount;
			float stackStep = M_PI / stackCount;
			float radius = 1.0f;

			Mesh* mesh = new Mesh();

			for (int i = 0; i <= stackCount; ++i)
			{
				float stackAngle = M_PI / 2 - i * stackStep;        // starting from pi/2 to -pi/2
				float xy = radius * cos(stackAngle);             // r * cos(u)
				float z = radius * sin(stackAngle);              // r * sin(u)


				for (int32_t j = 0; j <= sectorCount; ++j)
				{
					float sectorAngle = j * sectorStep;           // starting from 0 to 2pi

					// vertex position (x, y, z)
					float x = xy * cosf(sectorAngle);             // r * cos(u) * cos(v)
					float y = xy * sinf(sectorAngle);             // r * cos(u) * sin(v)

					// vertex tex coords (s, t) range between [0, 1]
					float s = static_cast<float>(j / sectorCount);
					float t = static_cast<float>(i / stackCount);

					mesh->positions.emplace_back(glm::vec3(x, y, z));
					mesh->texCoords.emplace_back(glm::vec2(s, t));
					mesh->normals.emplace_back(glm::normalize(glm::vec3(x, y, z)));
				}
			}


			uint32_t k1, k2;
			for (uint32_t i = 0; i < stackCount; ++i)
			{
				k1 = i * (static_cast<uint32_t>(sectorCount) + 1U);     // beginning of current stack
				k2 = k1 + static_cast<uint32_t>(sectorCount) + 1U;      // beginning of next stack

				for (uint32_t j = 0; j < sectorCount; ++j, ++k1, ++k2)
				{
					// 2 triangles per sector excluding first and last stacks
					// k1 => k2 => k1+1
					if (i != 0)
					{
						mesh->indices.emplace_back(k1);
						mesh->indices.emplace_back(k2);
						mesh->indices.emplace_back(k1 + 1);
					}

					// k1+1 => k2 => k2+1
					if (i != (stackCount - 1))
					{
						mesh->indices.emplace_back(k1 + 1);
						mesh->indices.emplace_back(k2);
						mesh->indices.emplace_back(k2 + 1);
					}
				}
			}
			return mesh;
		}

		Mesh* CreatePlane(float width, float height, const glm::vec3& normal)
		{

			Mesh* mesh = new Mesh();

			glm::vec3 vec = normal * 90.0f;
			glm::quat rotation =
				glm::quat(vec.z, glm::vec3(1.0f, 0.0f, 0.0f)) *
				glm::quat(vec.y, glm::vec3(0.0f, 1.0f, 0.0f)) *
				glm::quat(vec.x, glm::vec3(0.0f, 0.0f, 1.0f));

			Vertex data[4];
			memset(data, 0, 4 * sizeof(Vertex));


			data[0].position = rotation * glm::vec3(-width / 2.0f, -1.0f, -height / 2.0f);
			data[0].normal = normal;
			data[0].texCoord = glm::vec2(0.0f, 0.0f);

			data[1].position = rotation * glm::vec3(-width / 2.0f, -1.0f, height / 2.0f);
			data[1].normal = normal;
			data[1].texCoord = glm::vec2(0.0f, 1.0f);

			data[2].position = rotation * glm::vec3(width / 2.0f, 1.0f, height / 2.0f);
			data[2].normal = normal;
			data[2].texCoord = glm::vec2(1.0f, 1.0f);

			data[3].position = rotation * glm::vec3(width / 2.0f, 1.0f, -height / 2.0f);
			data[3].normal = normal;
			data[3].texCoord = glm::vec2(1.0f, 0.0f);


			for (auto& v : data)
			{
				mesh->positions.emplace_back(v.position);
				mesh->texCoords.emplace_back(v.texCoord);
				mesh->normals.emplace_back(v.normal);
			}

			mesh->indices =
			{
				0, 1, 2,
				2, 3, 0
			};

			return mesh;
		}

		Mesh* CreateCapsule(float radius, float midHeight, int32_t radialSegments, int32_t rings)
		{
			int32_t i, j, prevrow, thisrow, point;
			float x, y, z, u, v, w;
			float onethird = 1.0f / 3.0f;
			float twothirds = 2.0f / 3.0f;


			Mesh* mesh = new Mesh();


			point = 0;

			/* top hemisphere */
			thisrow = 0;
			prevrow = 0;
			for (j = 0; j <= (rings + 1); j++)
			{
				v = float(j);

				v /= (rings + 1);
				w = sin(0.5f * M_PI * v);
				z = radius * cos(0.5f * M_PI * v);

				for (i = 0; i <= radialSegments; i++)
				{
					u = float(i);
					u /= radialSegments;

					x = sin(u * (M_PI * 2.0f));
					y = -cos(u * (M_PI * 2.0f));

					glm::vec3 p = glm::vec3(x * radius * w, y * radius * w, z);

					mesh->positions.emplace_back(p + glm::vec3(0.0f, 0.0f, 0.5f * midHeight));
					mesh->normals.emplace_back(glm::normalize(p + glm::vec3(0.0f, 0.0f, 0.5f * midHeight)));
					mesh->texCoords.emplace_back(glm::vec2(u, onethird * v));


					point++;

					if (i > 0 && j > 0)
					{
						mesh->indices.push_back(thisrow + i - 1);
						mesh->indices.push_back(prevrow + i);
						mesh->indices.push_back(prevrow + i - 1);

						mesh->indices.push_back(thisrow + i - 1);
						mesh->indices.push_back(thisrow + i);
						mesh->indices.push_back(prevrow + i);
					};
				};

				prevrow = thisrow;
				thisrow = point;
			};

			/* cylinder */
			thisrow = point;
			prevrow = 0;
			for (j = 0; j <= (rings + 1); j++)
			{
				v = float(j);
				v /= (rings + 1);

				z = midHeight * v;
				z = (midHeight * 0.5f) - z;

				for (i = 0; i <= radialSegments; i++)
				{
					u = float(i);
					u /= radialSegments;

					x = sin(u * (M_PI * 2.0f));
					y = -cos(u * (M_PI * 2.0f));

					glm::vec3 p = glm::vec3(x * radius, y * radius, z);

					mesh->positions.emplace_back(p);
					mesh->normals.emplace_back(glm::vec3(x, 0.0f, z));
					mesh->texCoords.emplace_back(glm::vec2(u, onethird * v));



					point++;

					if (i > 0 && j > 0)
					{
						mesh->indices.emplace_back(thisrow + i - 1);
						mesh->indices.emplace_back(prevrow + i);
						mesh->indices.emplace_back(prevrow + i - 1);
						mesh->indices.emplace_back(thisrow + i - 1);
						mesh->indices.emplace_back(thisrow + i);
						mesh->indices.emplace_back(prevrow + i);
					};
				};

				prevrow = thisrow;
				thisrow = point;
			};

			/* bottom hemisphere */
			thisrow = point;
			prevrow = 0;

			for (j = 0; j <= (rings + 1); j++)
			{
				v = float(j);

				v /= (rings + 1);
				v += 1.0f;
				w = sin(0.5f * M_PI * v);
				z = radius * cos(0.5f * M_PI * v);

				for (i = 0; i <= radialSegments; i++)
				{
					float u2 = float(i);
					u2 /= radialSegments;

					x = sin(u2 * (M_PI * 2.0f));
					y = -cos(u2 * (M_PI * 2.0f));

					glm::vec3 p = glm::vec3(x * radius * w, y * radius * w, z);

					mesh->positions.emplace_back(p + glm::vec3(0.0f, 0.0f, -0.5f * midHeight));
					mesh->normals.emplace_back(glm::normalize(p + glm::vec3(0.0f, 0.0f, -0.5f * midHeight)));
					mesh->texCoords.emplace_back(glm::vec2(u2, twothirds + ((v - 1.0f) * onethird)));


					point++;

					if (i > 0 && j > 0)
					{
						mesh->indices.emplace_back(thisrow + i - 1);
						mesh->indices.emplace_back(prevrow + i);
						mesh->indices.emplace_back(prevrow + i - 1);
						mesh->indices.emplace_back(thisrow + i - 1);
						mesh->indices.emplace_back(thisrow + i);
						mesh->indices.emplace_back(prevrow + i);
					};
				};

				prevrow = thisrow;
				thisrow = point;
			}

			return mesh;
		}

		Mesh* CreateCylinder(float bottomRadius, float topRadius, float height, int32_t radialSegments, int32_t rings)
		{
			int32_t i, j, prevrow, thisrow, point = 0;
			float x, y, z, u, v, radius;


			Mesh* mesh = new Mesh();

			thisrow = 0;
			prevrow = 0;
			for (j = 0; j <= (rings + 1); j++)
			{
				v = float(j);
				v /= (rings + 1);

				radius = topRadius + ((bottomRadius - topRadius) * v);

				y = height * v;
				y = (height * 0.5f) - y;

				for (i = 0; i <= radialSegments; i++)
				{
					u = float(i);
					u /= radialSegments;

					x = sin(u * (M_PI * 2.0f));
					z = cos(u * (M_PI * 2.0f));

					glm::vec3 p = glm::vec3(x * radius, y, z * radius);

					mesh->positions.emplace_back(p);
					mesh->normals.emplace_back(glm::vec3(x, 0.0f, z));
					mesh->texCoords.emplace_back(glm::vec2(u, v * 0.5f));

					point++;

					if (i > 0 && j > 0)
					{
						mesh->indices.emplace_back(thisrow + i - 1);
						mesh->indices.emplace_back(prevrow + i);
						mesh->indices.emplace_back(prevrow + i - 1);
						mesh->indices.emplace_back(thisrow + i - 1);
						mesh->indices.emplace_back(thisrow + i);
						mesh->indices.emplace_back(prevrow + i);
					};
				};

				prevrow = thisrow;
				thisrow = point;
			};

			// add top
			if (topRadius > 0.0f)
			{
				y = height * 0.5f;

				mesh->positions.emplace_back(glm::vec3(0.0f, y, 0.0f));
				mesh->normals.emplace_back(glm::vec3(0.0f, 1.0f, 0.0f));
				mesh->texCoords.emplace_back(glm::vec2(0.25f, 0.75f));

				point++;

				for (i = 0; i <= radialSegments; i++)
				{
					float r = float(i);
					r /= radialSegments;

					x = sin(r * (M_PI * 2.0f));
					z = cos(r * (M_PI * 2.0f));

					u = ((x + 1.0f) * 0.25f);
					v = 0.5f + ((z + 1.0f) * 0.25f);

					glm::vec3 p = glm::vec3(x * topRadius, y, z * topRadius);

					mesh->positions.emplace_back(p);
					mesh->normals.emplace_back(glm::vec3(0.0f, 1.0f, 0.0f));
					mesh->texCoords.emplace_back(glm::vec2(u, v));

					point++;

					if (i > 0)
					{
						mesh->indices.emplace_back(point - 2);
						mesh->indices.emplace_back(point - 1);
						mesh->indices.emplace_back(thisrow);
					};
				};
			};

			// add bottom
			if (bottomRadius > 0.0f)
			{
				y = height * -0.5f;

				thisrow = point;


				mesh->positions.emplace_back(glm::vec3(0.0f, y, 0.0f));
				mesh->normals.emplace_back(glm::vec3(0.0f, -1.0f, 0.0f));
				mesh->texCoords.emplace_back(glm::vec2(0.75f, 0.75f));

				point++;

				for (i = 0; i <= radialSegments; i++)
				{
					float r = float(i);
					r /= radialSegments;

					x = std::sin(r * (M_PI * 2.0f));
					z = std::cos(r * (M_PI * 2.0f));

					u = 0.5f + ((x + 1.0f) * 0.25f);
					v = 1.0f - ((z + 1.0f) * 0.25f);

					glm::vec3 p = glm::vec3(x * bottomRadius, y, z * bottomRadius);

					mesh->positions.emplace_back(p);
					mesh->normals.emplace_back(glm::vec3(0.0f, -1.0f, 0.0f));
					mesh->texCoords.emplace_back(glm::vec2(u, v));

					point++;

					if (i > 0)
					{
						mesh->indices.emplace_back(point - 1);
						mesh->indices.emplace_back(point - 2);
						mesh->indices.emplace_back(thisrow);
					};
				};
			};

			return mesh;
		}

		Mesh* CreatePrimitive(PrimitiveType type)
		{
			switch (type)
			{
			case PrimitiveType::Cube: return CreateCube();
			case PrimitiveType::Plane: return CreatePlane(1.0f, 1.0f, glm::vec3(0.0f, 1.0f, 0.0f));
			case PrimitiveType::Quad: return CreateQuad();
			case PrimitiveType::Sphere: return CreateSphere();
			case PrimitiveType::Pyramid: return CreatePyramid();
			case PrimitiveType::Capsule: return CreateCapsule();
			case PrimitiveType::Cylinder: return CreateCylinder();
			case PrimitiveType::Terrain: return CreateTerrain();
			case PrimitiveType::File:
				LOGW("Trying to create primitive of type File"); return nullptr;
			}

			return nullptr;
		}

		Mesh* CreateTerrain()
		{
			return nullptr;
		}
	};
};


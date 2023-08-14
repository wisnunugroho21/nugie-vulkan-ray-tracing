#include "load_model.hpp"

#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>

namespace nugiEngine
{
  LoadedModel loadModelFromFile(const std::string &filePath, uint32_t transformIndex, uint32_t materialIndex, uint32_t vertexOffsetIndex) {
		tinyobj::attrib_t attrib{};
		std::vector<tinyobj::shape_t> shapes{};
		std::vector<tinyobj::material_t> materials{};
		// std::unordered_map<RayTraceVertex, uint32_t> uniqueVertices{};
		std::string warn, err;

		if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, filePath.c_str())) {
			throw std::runtime_error(warn + err);
		}
		
		auto primitives = std::make_shared<std::vector<Primitive>>();
		auto vertices = std::make_shared<std::vector<Vertex>>();
		auto indices = std::make_shared<std::vector<uint32_t>>();

		for (const auto &shape: shapes) {
			uint32_t numTriangle = static_cast<uint32_t>(shape.mesh.indices.size()) / 3;

			for (uint32_t i = 0; i < numTriangle; i++) {
				int vertexIndex0 = shape.mesh.indices[3 * i + 0].vertex_index;
				int vertexIndex1 = shape.mesh.indices[3 * i + 1].vertex_index;
				int vertexIndex2 = shape.mesh.indices[3 * i + 2].vertex_index;

				int textCoordIndex0 = shape.mesh.indices[3 * i + 0].texcoord_index;
				int textCoordIndex1 = shape.mesh.indices[3 * i + 1].texcoord_index;
				int textCoordIndex2 = shape.mesh.indices[3 * i + 2].texcoord_index;

				int normalIndex0 = shape.mesh.indices[3 * i + 0].normal_index;
				int normalIndex1 = shape.mesh.indices[3 * i + 1].normal_index;
				int normalIndex2 = shape.mesh.indices[3 * i + 2].normal_index;

				Vertex vertex0;
				Vertex vertex1;
				Vertex vertex2;
				
				vertex0.position = glm::vec4{
					attrib.vertices[3 * vertexIndex0 + 0],
					attrib.vertices[3 * vertexIndex0 + 1],
					attrib.vertices[3 * vertexIndex0 + 2],
          1.0f
				};

				vertex1.position = glm::vec4{
					attrib.vertices[3 * vertexIndex1 + 0],
					attrib.vertices[3 * vertexIndex1 + 1],
					attrib.vertices[3 * vertexIndex1 + 2],
          1.0f
				};

				vertex2.position = glm::vec4{
					attrib.vertices[3 * vertexIndex2 + 0],
					attrib.vertices[3 * vertexIndex2 + 1],
					attrib.vertices[3 * vertexIndex2 + 2],
          1.0f
				};

        vertex0.normal = glm::vec4{
					attrib.normals[3 * normalIndex0 + 0],
					attrib.normals[3 * normalIndex0 + 1],
					attrib.normals[3 * normalIndex0 + 2],
          1.0f
				};

				vertex1.normal = glm::vec4{
					attrib.normals[3 * normalIndex1 + 0],
					attrib.normals[3 * normalIndex1 + 1],
					attrib.normals[3 * normalIndex1 + 2],
          1.0f
				};

				vertex2.normal = glm::vec4{
					attrib.normals[3 * normalIndex2 + 0],
					attrib.normals[3 * normalIndex2 + 1],
					attrib.normals[3 * normalIndex2 + 2],
          1.0f
				};

				vertex0.textCoord = glm::vec4{
					attrib.texcoords[2 * textCoordIndex0 + 0],
					1.0f - attrib.texcoords[2 * textCoordIndex0 + 1],
          1.0f,
          1.0f
				};

				vertex1.textCoord = glm::vec4{
					attrib.texcoords[2 * textCoordIndex1 + 0],
					1.0f - attrib.texcoords[2 * textCoordIndex1 + 1],
          1.0f,
          1.0f
				};

				vertex2.textCoord = glm::vec4{
					attrib.texcoords[2 * textCoordIndex2 + 0],
					1.0f - attrib.texcoords[2 * textCoordIndex2 + 1],
          1.0f,
          1.0f
				};

				vertex0.transformIndex = transformIndex;
				vertex1.transformIndex = transformIndex;
				vertex2.transformIndex = transformIndex;

				vertex0.materialIndex = materialIndex;
				vertex1.materialIndex = materialIndex;
				vertex2.materialIndex = materialIndex;

				/* if (uniqueVertices.count(vertex0) == 0) {
					uniqueVertices[vertex0] = static_cast<uint32_t>(vertices->size()) + offsetIndex;
					vertices->emplace_back(vertex0);
				}

				if (uniqueVertices.count(vertex1) == 0) {
					uniqueVertices[vertex1] = static_cast<uint32_t>(vertices->size()) + offsetIndex;
					vertices->emplace_back(vertex1);
				}

				if (uniqueVertices.count(vertex2) == 0) {
					uniqueVertices[vertex2] = static_cast<uint32_t>(vertices->size()) + offsetIndex;
					vertices->emplace_back(vertex2);
				}

				uint32_t index0 = uniqueVertices[vertex0];
				uint32_t index1 = uniqueVertices[vertex1];
				uint32_t index2 = uniqueVertices[vertex2]; */

				uint32_t index0 = static_cast<uint32_t>(vertices->size()) + vertexOffsetIndex;
				vertices->emplace_back(vertex0);
				indices->emplace_back(index0);

				uint32_t index1 = static_cast<uint32_t>(vertices->size()) + vertexOffsetIndex;
				vertices->emplace_back(vertex1);
				indices->emplace_back(index1);

				uint32_t index2 = static_cast<uint32_t>(vertices->size()) + vertexOffsetIndex;
				vertices->emplace_back(vertex2);
				indices->emplace_back(index2);

				primitives->emplace_back(Primitive{
					glm::uvec3{ index0, index1, index2 },
					materialIndex
				});
			}
		}

		return LoadedModel{ primitives, vertices, indices };
	}
  
} // namespace nugiEngine